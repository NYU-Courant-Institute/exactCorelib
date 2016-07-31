#include <ompl/base/goals/GoalState.h>
#include <ompl/base/spaces/RealVectorStateSpace.h>
#include <ompl/base/spaces/SO2StateSpace.h>
#include <ompl/base/MotionValidator.h>
#include <ompl/base/SpaceInformation.h>


#include <ompl/geometric/SimpleSetup.h>
#include <ompl/geometric/planners/rrt/RRT.h>
#include <ompl/geometric/planners/kpiece/KPIECE1.h>
#include <ompl/geometric/planners/est/EST.h>
#include <ompl/geometric/planners/prm/PRM.h>
#include <ompl/geometric/planners/stride/STRIDE.h>

#include <omplapp/apps/SE2RigidBodyPlanning.h>
#include <omplapp/config.h>

#include <boost/math/constants/constants.hpp>
#include <boost/format.hpp>

#include "main.h"

namespace oa = ompl::app;
namespace ob = ompl::base;
namespace og = ompl::geometric;

#define mw_out (*window)
static MainWindow *window;

//environment related parameters
string egName("sampling_T-room_rrt.eg");    // Input example name
string inputDir("inputs");              // Path for input files
string fileName("T-room.txt");          // Input file name
char egNameList[200][200];
int numEg = 0;
vector<c_ply> Objs;

CFG start, goal;
int l1,l2,thickness;
float scale=1.0;
float deltaX=0.0, deltaY=0.0;
int seed;

int windowPosX = 320;			// X Position of Window
int windowPosY = 20;			// Y Position of Window
float env_width = 512;			// ENV WIDTH
float env_height = 512;			// ENV HEIGHT
float env_TR = 0.001;			// TRANSLATIONAL RESOLUTION
float env_RR = 0.01;			// Rotational RESOLUTION (deg)

//common samping-based motion planner parameters
string method="rrt"; //planner, prm, ...
int SearchType = 0;
unsigned int max_sample_size=10000;

//planner parameters
float rrt_step_size=0.01;
float rrt_bias = 0.1;					// bias towards to the goal
float rrt_close_to_goal = env_TR;

//prm parameters
unsigned int prm_closest_k=15;

//gaussian prm paramters
float gauss_mean_d=0.1;
float gauss_std=0.1;

bool noPath(false);
bool showAnim(true);
bool pauseAnim(false);
bool replayAnim(false);
int path_index(0);
bool rrt_graph(true);
bool prm_graph(false);
bool non_crossing(false);
int animationSpeed(99);
int animationSpeedScale(5000);

double elapsedTime, elapsedCPUTime;

double roundAngle(double theta);
double orientation2OMPL(double theta);
double orientation2Physical(double theta_ompl);
double orientation2Radius(double degree);
double orientation2Degree(double radius);

// simply use a random projection
class myProjector : public ob::ProjectionEvaluator
{
public:
    myProjector(const ob::StateSpace *space) : ob::ProjectionEvaluator(space)
    {
        int dimension = std::max(2, (int)ceil(log((double) space->getDimension())));
        projectionMatrix_.computeRandom(space->getDimension(), dimension);
    }
    virtual unsigned int getDimension(void) const
    {
        return projectionMatrix_.mat.size1();
    }
    void project(const ob::State *state, ob::EuclideanProjection &projection) const
    {
        std::vector<double> v(space_->getDimension());
        space_->copyToReals(v, state);
        projectionMatrix_.project(&v[0], projection);
    }
protected:
    ob::ProjectionMatrix projectionMatrix_;
};

double getAngleBetween(double t1, double t2){
    double t = t2 - t1;

    if(t < 0){
        t = -t;
    }
    if(t > PI){
        t = 2*PI-t;
    }

    return t;
}

class mySpace : public ob::CompoundStateSpace
{
public:
    mySpace() : ob::CompoundStateSpace()
    {
        ob::StateSpacePtr R1(new ob::RealVectorStateSpace(1));
        ob::StateSpacePtr R2(new ob::RealVectorStateSpace(1));
        // set the bounds for the R^2 part
        ob::RealVectorBounds bounds(1);
        bounds.setLow(0);
        bounds.setHigh(512);
        R1->as<ob::RealVectorStateSpace>()->setBounds(bounds);
        R2->as<ob::RealVectorStateSpace>()->setBounds(bounds);

        addSubspace(R1, 1.0/*weight*/);
        addSubspace(R2, 1.0);
        addSubspace(ob::StateSpacePtr(new ob::SO2StateSpace()), 1.0);
        addSubspace(ob::StateSpacePtr(new ob::SO2StateSpace()), 1.0);
        lock();
    }

    void registerProjections()
    {
        registerDefaultProjection(ob::ProjectionEvaluatorPtr(new myProjector(this)));
    }

    double distance(const ob::State *state1, const ob::State *state2) const
    {
        const StateType *cstate1 = state1->as<StateType>();
        double const *x1 = cstate1->as<ob::RealVectorStateSpace::StateType>(0)->values;
        double const *y1 = cstate1->as<ob::RealVectorStateSpace::StateType>(1)->values;
        double t11 = cstate1->as<ob::SO2StateSpace::StateType>(2)->value;
        double t12 = cstate1->as<ob::SO2StateSpace::StateType>(3)->value;


        const StateType *cstate2 = state2->as<StateType>();
        double const *x2 = cstate2->as<ob::RealVectorStateSpace::StateType>(0)->values;
        double const *y2 = cstate2->as<ob::RealVectorStateSpace::StateType>(1)->values;
        double t21 = cstate2->as<ob::SO2StateSpace::StateType>(2)->value;
        double t22 = cstate2->as<ob::SO2StateSpace::StateType>(3)->value;


        double dist = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));

        dist += (double)l1*getAngleBetween(t11, t12); // 2*PI*R*(degree)/360.0f (t: [-pi, pi))
        dist += (double)l2*getAngleBetween(t21, t22);

        return dist;
    }
};

class myValidityChecker : public ob::StateValidityChecker
{
public:
    myValidityChecker(const ob::SpaceInformationPtr &si) : ob::StateValidityChecker(si)
    {
    }

    bool isValid(const ompl::base::State *state) const
    {
        const mySpace::StateType *s = state->as<mySpace::StateType>();

        double const *x = s->as<ob::RealVectorStateSpace::StateType>(0)->values;
        double const *y = s->as<ob::RealVectorStateSpace::StateType>(1)->values;

        double t1 = s->as<ob::SO2StateSpace::StateType>(2)->value+PI; // radius
        double t2 = s->as<ob::SO2StateSpace::StateType>(3)->value+PI;

        // thick links
        //double delta = this->m_robot.Thickness;
        //delta /= 2.0f;
        Point2d ori(*x, *y);
        vector<Point2d> p_link;
        p_link.push_back(Point2d(((*x)+(double)l1*cos(t1)), ((*y)+(double)l1*sin(t1))));
        p_link.push_back(Point2d(((*x)+(double)l2*cos(t2)), ((*y)+(double)l2*sin(t2))));

    //    // thick link1
    //    Point2d thick_link1[5];
    //    Vector2d dir_link1 = p_link1-ori;
    //    dir_link1 = dir_link1/dir_link1.norm();
    //    dir_link1[0] = -dir_link1[0];
    //    thick_link1[0] = ori+dir_link1*delta;
    //    thick_link1[1] = ori-dir_link1*delta;
    //    thick_link1[2] = p_link1+dir_link1*delta;
    //    thick_link1[3] = p_link1-dir_link1*delta;
    //    thick_link1[4] = thick_link1[0];

    //    // thick link2
    //    Point2d thick_link2[5];
    //    Vector2d dir_link2 = p_link2-ori;
    //    dir_link2 = dir_link2/dir_link2.norm();
    //    dir_link2[0] = -dir_link2[0];
    //    thick_link2[0] = ori+dir_link2*delta;
    //    thick_link2[1] = ori-dir_link2*delta;
    //    thick_link2[2] = p_link2+dir_link2*delta;
    //    thick_link2[3] = p_link2-dir_link2*delta;
    //    thick_link2[4] = thick_link2[0];

        for(int i=0;i<(int)Objs.size();++i)
        {
            c_ply *it = &Objs[i];
            // check points
            // thin link's origin
            if(it->getType() == c_ply::POUT) {
                if(it->enclosed(ori)){
                    return false;
                }
            }
            else {
                if(!it->enclosed(ori)){
                    return false;
                }
            }

    //        // thick link's
    //        for(int i=0;i<4;++i){
    //            if(it->getType() == c_ply::POUT) {
    //                if(it->enclosed(thick_link1[i])) return false;
    //            }
    //            else {
    //                if(!it->enclosed(thick_link1[i])) return false;
    //            }
    //            if(it->getType() == c_ply::POUT) {
    //                if(it->enclosed(thick_link2[i])) return false;
    //            }
    //            else {
    //                if(!it->enclosed(thick_link2[i])) return false;
    //            }
    //        }

            // check edges...
            // thin links
            for(int l=0;l<2;++l) {
                //robot link
                const Point2d& a = ori;
                const Point2d& b = p_link[l];

                ply_vertex *ptr=it->getHead();
                if(ptr == NULL) fprintf(stderr, "ptr GG\n");

                do{
                    ply_vertex *next=ptr->getNext();
                    if(ptr == NULL) fprintf(stderr, "next GG\n");

                    //create p
                    const Point2d& c=ptr->getPos();
                    const Point2d& d=next->getPos();

                    //
                    bool r=SegSegInt(a.get(),b.get(),c.get(),d.get());
                    if(r){
                        return false; //collision found
                    }

                    //next
                    ptr=next;
                }
                while(ptr!=it->getHead());
            }//end for

    //        // thick link1
    //        for(int l=0;l<4;l++) {
    //            //robot link
    //            const Point2d& a=thick_link1[l];
    //            const Point2d& b=thick_link1[l+1];

    //            ply_vertex * ptr=it->getHead();

    //            do{
    //                ply_vertex * next=ptr->getNext();

    //                //create p
    //                const Point2d& c=ptr->getPos();
    //                const Point2d& d=next->getPos();

    //                //
    //                bool r=SegSegInt(a.get(),b.get(),c.get(),d.get());
    //                if(r) return false; //collision found

    //                //next
    //                ptr=next;
    //            }
    //            while(ptr!=it->getHead());
    //        }//end for

    //        // thick link2
    //        for(int l=0;l<4;l++) {
    //            //robot link
    //            const Point2d& a=thick_link2[l];
    //            const Point2d& b=thick_link2[l+1];

    //            ply_vertex * ptr=it->getHead();

    //            do{
    //                ply_vertex * next=ptr->getNext();

    //                //create p
    //                const Point2d& c=ptr->getPos();
    //                const Point2d& d=next->getPos();

    //                //
    //                bool r=SegSegInt(a.get(),b.get(),c.get(),d.get());
    //                if(r) return false; //collision found

    //                //next
    //                ptr=next;
    //            }
    //            while(ptr!=it->getHead());
    //        }//end for
        }
        return true;
    }
};

bool isStateValid(const ob::SpaceInformation *si, const ob::State *state)
{
    if(!si->satisfiesBounds(state) || state == NULL) return false;
    const mySpace::StateType *s = state->as<mySpace::StateType>();

    double const *x = s->as<ob::RealVectorStateSpace::StateType>(0)->values;
    double const *y = s->as<ob::RealVectorStateSpace::StateType>(1)->values;

    double t1 = s->as<ob::SO2StateSpace::StateType>(2)->value+PI; // radius
    double t2 = s->as<ob::SO2StateSpace::StateType>(3)->value+PI;

    //fprintf(stderr, "state %lf %lf %lf %lf\n", *x, *y, t1, t2);


    // thick links
    //double delta = this->m_robot.Thickness;
    //delta /= 2.0f;
    Point2d ori(*x, *y);
    vector<Point2d> p_link;
    p_link.push_back(Point2d(((*x)+(double)l1*cos(t1)), ((*y)+(double)l1*sin(t1))));
    p_link.push_back(Point2d(((*x)+(double)l2*cos(t2)), ((*y)+(double)l2*sin(t2))));

//    // thick link1
//    Point2d thick_link1[5];
//    Vector2d dir_link1 = p_link1-ori;
//    dir_link1 = dir_link1/dir_link1.norm();
//    dir_link1[0] = -dir_link1[0];
//    thick_link1[0] = ori+dir_link1*delta;
//    thick_link1[1] = ori-dir_link1*delta;
//    thick_link1[2] = p_link1+dir_link1*delta;
//    thick_link1[3] = p_link1-dir_link1*delta;
//    thick_link1[4] = thick_link1[0];

//    // thick link2
//    Point2d thick_link2[5];
//    Vector2d dir_link2 = p_link2-ori;
//    dir_link2 = dir_link2/dir_link2.norm();
//    dir_link2[0] = -dir_link2[0];
//    thick_link2[0] = ori+dir_link2*delta;
//    thick_link2[1] = ori-dir_link2*delta;
//    thick_link2[2] = p_link2+dir_link2*delta;
//    thick_link2[3] = p_link2-dir_link2*delta;
//    thick_link2[4] = thick_link2[0];

    for(int i=0;i<(int)Objs.size();++i)
    {
        c_ply *it = &Objs[i];
        // check points
        // thin link's origin
        if(it->getType() == c_ply::POUT) {
            if(it->enclosed(ori)){
                return false;
            }
        }
        else {
            if(!it->enclosed(ori)){
                return false;
            }
        }

//        // thick link's
//        for(int i=0;i<4;++i){
//            if(it->getType() == c_ply::POUT) {
//                if(it->enclosed(thick_link1[i])) return false;
//            }
//            else {
//                if(!it->enclosed(thick_link1[i])) return false;
//            }
//            if(it->getType() == c_ply::POUT) {
//                if(it->enclosed(thick_link2[i])) return false;
//            }
//            else {
//                if(!it->enclosed(thick_link2[i])) return false;
//            }
//        }

        // check edges...
        // thin links
        for(int l=0;l<2;++l) {
            //robot link
            const Point2d& a = ori;
            const Point2d& b = p_link[l];

            ply_vertex *ptr=it->getHead();

            do{
                ply_vertex *next=ptr->getNext();

                //create p
                const Point2d& c=ptr->getPos();
                const Point2d& d=next->getPos();

                //
                bool r=SegSegInt(a.get(),b.get(),c.get(),d.get());
                if(r){
                    return false; //collision found
                }

                //next
                ptr=next;
            }
            while(ptr!=it->getHead());
        }//end for

//        // thick link1
//        for(int l=0;l<4;l++) {
//            //robot link
//            const Point2d& a=thick_link1[l];
//            const Point2d& b=thick_link1[l+1];

//            ply_vertex * ptr=it->getHead();

//            do{
//                ply_vertex * next=ptr->getNext();

//                //create p
//                const Point2d& c=ptr->getPos();
//                const Point2d& d=next->getPos();

//                //
//                bool r=SegSegInt(a.get(),b.get(),c.get(),d.get());
//                if(r) return false; //collision found

//                //next
//                ptr=next;
//            }
//            while(ptr!=it->getHead());
//        }//end for

//        // thick link2
//        for(int l=0;l<4;l++) {
//            //robot link
//            const Point2d& a=thick_link2[l];
//            const Point2d& b=thick_link2[l+1];

//            ply_vertex * ptr=it->getHead();

//            do{
//                ply_vertex * next=ptr->getNext();

//                //create p
//                const Point2d& c=ptr->getPos();
//                const Point2d& d=next->getPos();

//                //
//                bool r=SegSegInt(a.get(),b.get(),c.get(),d.get());
//                if(r) return false; //collision found

//                //next
//                ptr=next;
//            }
//            while(ptr!=it->getHead());
//        }//end for
    }
    return true;
}

//class myMotionValidator : public ob::MotionValidator
//{
//public:

//    /// \brief Constructor
//    myMotionValidator(ob::SpaceInformation* si, oa::MotionModel mm) : ob::MotionValidator(si)
//    {
//        defaultSettings(mm);
//    }

//    /// \brief Constructor
//    myMotionValidator(const ob::SpaceInformationPtr &si, oa::MotionModel mm) : ob::MotionValidator(si)
//    {
//        defaultSettings(mm);
//    }

//    /// \brief Destructor
//    virtual ~myMotionValidator(void)
//    {
//    }

//    /// \brief Returns true if motion between s1 and s2 is collision free.
//    virtual bool checkMotion(const ob::State *s1, const ob::State *s2) const
//    {
//        double unused;

//        // assume motion starts in a valid configuration so s1 is valid
//        // Must check validity of s2 before performing collision check between s1 and s2
//        bool valid = si_->isValid(s2) && myWrapper_->isValid (s1, s2, unused);

//        // Increment valid/invalid motion counters
//        valid ? valid_++ : invalid_++;

//        return valid;
//    }

//    /// \brief Checks the motion between s1 and s2. If the motion is
//    /// invalid, lastValid contains the last valid state and the
//    /// parameterized time [0,1) when this state occurs.
//    virtual bool checkMotion(const ob::State *s1, const ob::State *s2, std::pair<ob::State*, double> &lastValid) const
//    {
//        bool valid = false;

//        // if there is a collision, collisionTime will contain the time to collision,
//        // parameterized from [0,1), where s1 is 0 and s2 is 1.
//        double collisionTime;
//        valid = myWrapper_->isValid (s1, s2, collisionTime);

//        // Find the last valid state before collision...
//        // NOTE: This should probably be refactored so that the continuous checker
//        // returns the last valid time.  Last valid transformation may also be
//        // possible, but that introduces a dependency on the geometry.
//        if (!valid)
//        {
//            ob::State *lastValidState;
//            if (lastValid.first)
//                lastValidState = lastValid.first;
//            else
//                lastValidState = si_->allocState ();

//            collisionTime -= 0.01;
//            stateSpace_->interpolate (s1, s2, collisionTime, lastValidState);

//            while (!si_->isValid (lastValidState) && collisionTime > 0)
//            {
//                collisionTime -= 0.01;
//                stateSpace_->interpolate (s1, s2, collisionTime, lastValidState);
//            }

//            // ensure that collisionTime is greater than zero
//            if (collisionTime < 0.01)
//            {
//                collisionTime = 0.0;
//                si_->copyState (lastValidState, s1);
//            }

//            lastValid.second = collisionTime;

//            if (!lastValid.first)
//                si_->freeState (lastValidState);
//        }

//        // Increment valid/invalid motion counters
//        valid ? valid_++ : invalid_++;

//        return valid;
//    }

//protected:

//    /// \brief Restore settings to default values.
//    void defaultSettings(oa::MotionModel mm)
//    {
//        stateSpace_ = si_->getStateSpace().get();
//        if (!stateSpace_){
//            fprintf(stderr, "No state space for motion validator\n");
//            return ;
//        }

//        // Extract FCLWrapper from FCLStateValidityChecker.
//        switch (mm)
//        {
//            case oa::Motion_2D:
//                const oa::FCLStateValidityChecker<oa::Motion_2D> *fcl_2d_state_checker;
//                fcl_2d_state_checker = dynamic_cast <const oa::FCLStateValidityChecker<oa::Motion_2D>* > (si_->getStateValidityChecker ().get ());

//                if (!fcl_2d_state_checker)
//                {
//                    // Be extra verbose in this fatal error
//                    OMPL_ERROR("Unable to cast state validity checker to FCLStateValidityChecker.");
//                    assert (fcl_2d_state_checker != 0);
//                }

//                myWrapper_ = fcl_2d_state_checker->getFCLWrapper ();
//                break;

////            case oa::Motion_3D:
////                const oa::FCLStateValidityChecker<oa::Motion_3D> *fcl_3d_state_checker;
////                fcl_3d_state_checker = dynamic_cast <const oa::FCLStateValidityChecker<oa::Motion_3D>* > (si_->getStateValidityChecker ().get ());

////                if (!fcl_3d_state_checker)
////                {
////                    // Be extra verbose in this fatal error
////                    OMPL_ERROR("Unable to cast state validity checker to FCLStateValidityChecker.");
////                    assert (fcl_3d_state_checker != 0);
////                }

////                myWrapper_ = fcl_3d_state_checker->getFCLWrapper ();
////                break;

//            default:
//                OMPL_WARN("Unknown motion model specified: %u", mm);
//                break;
//        }

//        if (!myWrapper_)
//        {
//            // Be extra verbose in this fatal error
//            OMPL_ERROR("myWrapper object is not valid.");
//            assert (myWrapper_ != 0);
//        }
//    }

//    /// \brief Wrapper for my collision and distance methods
//    oa::FCLMethodWrapperPtr     myWrapper_;

//    /// \brief Handle to the statespace that this motion validator operates in.
//    ob::StateSpace*             stateSpace_;
//};

void run();

/* ********************************************************************** */
// skip blanks, tabs, line breaks and comment lines,
//  leaving us at the beginning of a token (or EOF)
//  (This code is taken from CORE lib)
inline int skip_comment_line (std::ifstream & in)
{
      int c;

      do {
        c = in.get();
        while ( c == '#' ) {
          do {// ignore the rest of this line
            c = in.get();
          } while ( c != '\n' );
          c = in.get(); // now, reach the beginning of the next line
        }
      } while (c == ' ' || c == '\t' || c == '\n' || c == '\r'); //ignore white spaces and newlines

      in.putback(c);  // this is non-white and non-comment char!
      return c;

}//skip_comment_line

char egPath[256], tmp[256];
void parseExampleList() {
    sprintf(egPath, "ls -R > tmpList");
    system(egPath);

    sprintf(egPath, "tmpList");
    FILE *fptr = fopen(egPath, "r");
    if(fptr == NULL) return ;
    while(fgets(tmp, 200, fptr) != NULL){
        char *sptr = strtok(tmp, " \n");
        while(sptr != NULL){
            int len = strlen(sptr);
            if(len > 3 && sptr[len-1] == 'g' && sptr[len-2] == 'e' && sptr[len-3] == '.'){
                strcpy(egNameList[numEg], sptr);
                ++numEg;
            }
            sptr = strtok(NULL, " \n");
        }
    }

    sprintf(egPath, "rm -rf tmpList");
    system(egPath);
}

void parseExampleFile() {

    sprintf(egPath, "%s/%s", inputDir.c_str(), egName.c_str());
    FILE *fptr = fopen(egPath, "r");
    if (fptr == NULL) return ;

    while (fgets(tmp, 256, fptr) != NULL){
        char *sptr = strtok(tmp, "=: \t");

        // comments
        if (strcmp(sptr, "#") == 0) {
            continue;
        }

        // start configuration
        if (strcmp(sptr, "startX") == 0) {
            sptr = strtok(NULL, "=: \t");
            start.x = atof(sptr);
        }
        if (strcmp(sptr, "startY") == 0) {
            sptr = strtok(NULL, "=: \t");
            start.y = atof(sptr);
        }
        if (strcmp(sptr, "startTheta1") == 0) {
            sptr = strtok(NULL, "=: \t");
            start.t1 = atof(sptr);
        }
        if (strcmp(sptr, "startTheta2") == 0) {
            sptr = strtok(NULL, "=: \t");
            start.t2 = atof(sptr);
        }

        // goal configuration
        if (strcmp(sptr, "goalX") == 0) {
            sptr = strtok(NULL, "=: \t");
            goal.x = atof(sptr);
        }
        if (strcmp(sptr, "goalY") == 0) {
            sptr = strtok(NULL, "=: \t");
            goal.y = atof(sptr);
        }
        if (strcmp(sptr, "goalTheta1") == 0) {
            sptr = strtok(NULL, "=: \t");
            goal.t1 = atof(sptr);
        }
        if (strcmp(sptr, "goalTheta2") == 0) {
            sptr = strtok(NULL, "=: \t");
            goal.t2 = atof(sptr);
        }

        if (strcmp(sptr, "len1") == 0) {
            sptr = strtok(NULL, "=: \t");
            l1 = atoi(sptr);
        }
        if (strcmp(sptr, "len2") == 0) {
            sptr = strtok(NULL, "=: \t");
            l2 = atoi(sptr);
        }
        if (strcmp(sptr, "thickness") == 0) {
            sptr = strtok(NULL, "=: \t");
            thickness = atoi(sptr);
        }

        if (strcmp(sptr, "inputDir") == 0) {
            sptr = strtok(NULL, "=: #\t");
            inputDir = sptr;
        }
        if (strcmp(sptr, "fileName") == 0) {
            sptr = strtok(NULL, "=: #\t");
            fileName = sptr;
        }

        // box width and height
        if (strcmp(sptr, "boxWidth") == 0) {
            sptr = strtok(NULL, "=: \t");
            env_width = atof(sptr);
        }
        if (strcmp(sptr, "boxHeight") == 0) {
            sptr = strtok(NULL, "=: \t");
            env_height = atof(sptr);
        }

        // windows position
        if (strcmp(sptr, "windowPosX") == 0) {
            sptr = strtok(NULL, "=: \t");
            windowPosX = atoi(sptr);
        }
        if (strcmp(sptr, "windowPosY") == 0) {
            sptr = strtok(NULL, "=: \t");
            windowPosY = atoi(sptr);
        }

        if (strcmp(sptr, "method") == 0) {
            sptr = strtok(NULL, "=: \t\n");
            method = sptr;
        }

        if (strcmp(sptr, "seed") == 0) {
            sptr = strtok(NULL, "=: \t");
            seed = atoi(sptr);
        }

        if (strcmp(sptr, "xtrans") == 0) {
            sptr = strtok(NULL, "=: \t");
            deltaX = atof(sptr);
        }

        if (strcmp(sptr, "ytrans") == 0) {
            sptr = strtok(NULL, "=: \t");
            deltaY = atof(sptr);
        }

        if (strcmp(sptr, "scale") == 0) {
            sptr = strtok(NULL, "=: \t");
            scale = atof(sptr);
        }

        if (strcmp(sptr, "maxSampleSize") == 0) {
            sptr = strtok(NULL, "=: \t");
            max_sample_size = atoi(sptr);
        }

        if (strcmp(sptr, "prmClosestK") == 0) {
            sptr = strtok(NULL, "=: \t");
            prm_closest_k = atoi(sptr);
        }

        if (strcmp(sptr, "GaussianMean") == 0) {
            sptr = strtok(NULL, "=: \t");
            gauss_mean_d = atof(sptr);
        }

        if (strcmp(sptr, "GaussianStd") == 0) {
            sptr = strtok(NULL, "=: \t");
            gauss_std = atof(sptr);
        }

        if (strcmp(sptr, "rrtStepSize") == 0) {
            sptr = strtok(NULL, "=: \t");
            rrt_step_size = atof(sptr);
        }

        if (strcmp(sptr, "rrtBias") == 0) {
            sptr = strtok(NULL, "=: \t");
            rrt_bias = atof(sptr);
        }

        if (strcmp(sptr, "rrtClose2Goal") == 0) {
            sptr = strtok(NULL, "=: \t");
            rrt_close_to_goal = atof(sptr);
        }

        if (strcmp(sptr, "animationSpeed") == 0) {
            sptr = strtok(NULL, "=: \t");
            animationSpeed = atoi(sptr);
        }
        if (strcmp(sptr, "animationSpeedScale") == 0) {
            sptr = strtok(NULL, "=: \t");
            animationSpeedScale = atoi(sptr);
        }
    }
}

void parseMapFile() {

    Objs.clear();

    string fullName = inputDir+"/"+fileName;
    ifstream fin;
    fin.open(fullName.c_str());

    char buf[1024];
    int size = -1;
    int left = -1;
    int num_of_polygons = -1;
    vector<float> pts;
    while(skip_comment_line(fin)!=EOF) {
        fin.getline(buf, 1024);
        string lline(buf);
        if(buf[0] == '#' || lline.size() == 0) continue;

        //remove trailing white space chars and checking "\\"
        do{
            while(lline.at(lline.size()-1) == ' ') {
                lline.erase(lline.size()-2, lline.size()-1);
            }
            if(lline.at(lline.size()-1) == '\\'){ //there is "\\" at the end... read more
                lline.erase(lline.size()-2, lline.size()-1);
                fin.getline(buf, 1024);
                string append(buf);
                lline.append(append.begin(),append.end());
            }
            else break;
        }while(true);

        stringstream ss(lline);
        if(size == -1) {
            ss >> size;		//read point size
            left = size*2;
        }
        else if(left != 0)
        {
            float p;
            while(ss>>p) {
                pts.push_back(p);
                left--;
            }
        }
        else if(num_of_polygons == -1)
        {
            ss>>num_of_polygons;
        }
        // polygon
        else if(num_of_polygons>0)
        {
            c_ply ply(c_ply::POUT);
            ply.beginPoly();
            int index;

            while(ss>>index) {
                index--;
                ply.addVertex(pts[index*2]* scale + deltaX, pts[index*2+1]* scale + deltaY, false);
            }
            ply.endPoly(false);
            Objs.push_back(ply);
            num_of_polygons--;
        }
        //
        //sstream
    }
}

int main(int argc, char** argv) {
    bool foundFiles = false;
    std::string workingDir = QDir::currentPath().toStdString();

    // Test if the build directory is 2-links-ompl. If so,
    // the path to the current working directory will
    // include /2-links-ompl
    unsigned long indexOfDesiredDir = workingDir.rfind("/2-links-ompl/");
    if (indexOfDesiredDir != std::string::npos) {
        workingDir = workingDir.substr(0, indexOfDesiredDir + 5);

        // Set current working directory to 2-links-ompl
        QDir::setCurrent(workingDir.c_str());
        foundFiles = true;
    }

    // Test if program was downloaded from Github, and is the build directory.
    // Downloading it from Github will result in the folder having the name
    // /2-links-ompl-master instead of /2-links-ompl
    if (!foundFiles && (indexOfDesiredDir = workingDir.rfind("/2-links-ompl/")) != std::string::npos) {
        workingDir = workingDir.substr(0, indexOfDesiredDir + 12);

        // Set current working directory to /2-links-ompl-master
        QDir::setCurrent(workingDir.c_str());
        foundFiles = true;
    }

    // Test if a build directory (/build-2-links-ompl-...) was created. This directory
    // will reside in the same directory as /2-links-ompl
    if (!foundFiles && (indexOfDesiredDir = workingDir.rfind("/build-2-links-ompl")) != std::string::npos) {
        QDir dir(workingDir.substr(0, indexOfDesiredDir).c_str());

        if (dir.exists("2-links-ompl/2-links-ompl.pro")) { // Test if /2-links-ompl exists
            workingDir = workingDir.substr(0, indexOfDesiredDir) + "/2-links-ompl";

            // Set current working directory to 2-links-ompl
            QDir::setCurrent(workingDir.c_str());
            foundFiles = true;
        }
    }

    // /2-links-ompl could not be found
    if (!foundFiles) {
        std::cerr << std::endl << "!! WARNING !!\n"
        << "The program may not work correctly or at all because the folder "
        "containing the program's files cannot be found.\n"
        "Make sure that the program is inside of a folder named \"2-links-ompl\".\n";
    }

    //QApplication app(argc, argv);
    //fptr = fopen("debug.txt", "w");
    //if(fptr == NULL) return 0;

    parseExampleList();
    parseExampleFile();
    parseMapFile();
    //window = new MainWindow();
    run();

    //window->show();

    //return app.exec();
    return 0;
}

int runCount = 1;
void run() {



    // create state space
    //ob::StateSpacePtr stateSpace(new mySpace());
    ob::StateSpacePtr SE2(new ob::SE2StateSpace());
    ob::StateSpacePtr SO2(new ob::SO2StateSpace());
    ob::StateSpacePtr stateSpace = SE2 + SO2;

    // set the bounds for the R^2 part of SE(2)
    ob::RealVectorBounds bounds(2);
    bounds.setLow(0);
    bounds.setHigh(512);
    SE2->as<ob::SE2StateSpace>()->setBounds(bounds);


    // set up state checker and motion xhecker
    og::SimpleSetup ss(stateSpace);
    // over-write state checker
    ss.setStateValidityChecker(std::bind(&isStateValid, ss.getSpaceInformation().get(), std::placeholders::_1));
    // over-write motion checker(need to define motion)
    //ob::MotionValidatorPtr mv(new myMotionValidator(ss.getSpaceInformation(), oa::Motion_2D));
    //ss.getSpaceInformation()->setMotionValidator(mv);



    // create start and goal states
    ob::ScopedState<> m_start(stateSpace);
    ob::ScopedState<> m_goal(stateSpace);

    m_start[0] = start.x; // position X
    m_start[1] = start.y; // position Y
    m_start[2] = orientation2OMPL(start.t1); // orientation link1 [-pi, pi)
    m_start[3] = orientation2OMPL(start.t2); // orientation link2 [-pi, pi)

    fprintf(stderr, "start %lf %lf  %lf %lf  %lf %lf\n", start.x, start.y, start.t1, start.t2, m_start[2], m_start[3]);

    m_goal[0] = goal.x; // position X
    m_goal[1] = goal.y; // position Y
    m_goal[2] = orientation2OMPL(goal.t1); // orientation link1 [-pi, pi)
    m_goal[3] = orientation2OMPL(goal.t2); // orientation link2 [-pi, pi)

    ss.setStartAndGoalStates(m_start, m_goal);

    fprintf(stderr, "goal %lf %lf  %lf %lf  %lf %lf\n", goal.x, goal.y, goal.t1, goal.t2, m_goal[2], m_goal[3]);


    // choose different sampling method
    ompl::base::SpaceInformationPtr si_ptr = ss.getSpaceInformation();
    ompl::base::PlannerPtr target_planner_ptr(new ompl::geometric::RRT(si_ptr));
    //ompl::base::PlannerPtr target_planner_ptr(new ompl::geometric::PRM(si_ptr));
    //ompl::base::PlannerPtr target_planner_ptr(new ompl::geometric::EST(si_ptr));
    ss.setPlanner(target_planner_ptr);



    noPath = !ss.solve();
    if (!noPath)
        ss.getSolutionPath().print(std::cout);
    else
        fprintf(stderr, "No PATH.\n");
}


double roundAngle(double theta){
    while(theta >= 360) theta -= 360;
    while(theta < 0)    theta += 360;
    return theta;
}

double orientation2OMPL(double theta){
    return PI*(roundAngle(theta)-180.0f)/180.0f;
}

double orientation2Physical(double theta_ompl){
    return theta_ompl*180.0f/PI+180.0f;
}

double orientation2Radius(double degree){
    return degree*PI/180.0f;
}

double orientation2Degree(double radius){
    return radius*180.0f/PI;
}
