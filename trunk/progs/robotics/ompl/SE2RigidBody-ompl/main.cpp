/*
 * main.cpp
 *
 *  Created on: Nov 26, 2012
 *      Author: zhonghua, George Mason University
 *  
 *  Modified: Jyh-Ming Lien, Nov. 23, 2013
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

#include <Qdir>

#include <ompl/geometric/SimpleSetup.h>
#include <ompl/geometric/planners/rrt/RRT.h>
#include <ompl/geometric/planners/bitstar/BITstar.h>
#include <ompl/geometric/planners/kpiece/KPIECE1.h>
#include <ompl/geometric/planners/est/EST.h>
#include <ompl/geometric/planners/prm/PRM.h>
#include <ompl/geometric/planners/stride/STRIDE.h>
#include <omplapp/apps/SE2RigidBodyPlanning.h>
#include <omplapp/config.h>

using namespace ompl;

//#define mw_out (*window)
//static MainWindow *window;

void parseExampleFile();
void run();

std::string workingDir = QDir::currentPath().toStdString();
int main(int argc, char** argv) {
    bool foundFiles = false;

    // Test if the build directory is SE2RigidBody-ompl. If so,
    // the path to the current working directory will
    // include /SE2RigidBody-ompl
    unsigned long indexOfDesiredDir = workingDir.rfind("/SE2RigidBody-ompl/");
    if (indexOfDesiredDir != std::string::npos) {
        workingDir = workingDir.substr(0, indexOfDesiredDir + 5);

        // Set current working directory to SE2RigidBody-ompl
        QDir::setCurrent(workingDir.c_str());
        foundFiles = true;
    }

    // Test if program was downloaded from Github, and is the build directory.
    // Downloading it from Github will result in the folder having the name
    // /SE2RigidBody-ompl-master instead of /SE2RigidBody-ompl
    if (!foundFiles && (indexOfDesiredDir = workingDir.rfind("/SE2RigidBody-ompl/")) != std::string::npos) {
        workingDir = workingDir.substr(0, indexOfDesiredDir + 12);

        // Set current working directory to /SE2RigidBody-ompl-master
        QDir::setCurrent(workingDir.c_str());
        foundFiles = true;
    }

    // Test if a build directory (/build-SE2RigidBody-ompl-...) was created. This directory
    // will reside in the same directory as /SE2RigidBody-ompl
    if (!foundFiles && (indexOfDesiredDir = workingDir.rfind("/build-SE2RigidBody-ompl")) != std::string::npos) {
        QDir dir(workingDir.substr(0, indexOfDesiredDir).c_str());

        if (dir.exists("SE2RigidBody-ompl/SE2RigidBody-ompl.pro")) { // Test if /SE2RigidBody-ompl exists
            workingDir = workingDir.substr(0, indexOfDesiredDir) + "/SE2RigidBody-ompl";

            // Set current working directory to SE2RigidBody-ompl
            QDir::setCurrent(workingDir.c_str());
            foundFiles = true;
        }
    }

    // /SE2RigidBody-ompl could not be found
    if (!foundFiles) {
        std::cerr << std::endl << "!! WARNING !!\n"
        << "The program may not work correctly or at all because the folder "
        "containing the program's files cannot be found.\n"
        "Make sure that the program is inside of a folder named \"SE2RigidBody-ompl\".\n";
    }

    //QApplication app(argc, argv);
    //window = new MainWindow();
    parseExampleFile();
    run();

    //window->show();

    //return app.exec();
    return 0;
}

double roundAngle(double theta){
    while(theta >= 360) theta -= 360;
    while(theta < 0)    theta += 360;
    return theta;
}

double PI = 3.1415926535897f;
double orientation2OMPL(double theta){
    return PI*(roundAngle(theta)-180.0f)/180.0f;
}

std::string cfgName("T-room3.cfg");
std::string robot("link-70-8_c.raw");
std::string world("bugtrap2_c.raw");

struct CFG{
    double x, y, theta;
};

CFG start, goal;
char cfgPath[1000], tmp[1000];

void parseExampleFile() {

    sprintf(cfgPath, "inputs/%s", cfgName.c_str());
    FILE *fptr = fopen(cfgPath, "r");
    if (fptr == NULL) return ;

    while (fgets(tmp, 200, fptr) != NULL){
        char *sptr = strtok(tmp, "=: \t");

        // comments
        if (strcmp(sptr, "#") == 0) {
            continue;
        }

        // start configuration
        if (strcmp(sptr, "start.x") == 0) {
            sptr = strtok(NULL, "=: \t");
            start.x = atof(sptr);
        }
        if (strcmp(sptr, "start.y") == 0) {
            sptr = strtok(NULL, "=: \t");
            start.y = atof(sptr);
        }
        if (strcmp(sptr, "start.theta") == 0) {
            sptr = strtok(NULL, "=: ");
            start.theta = atof(sptr);
        }

        // goal configuration
        if (strcmp(sptr, "goal.x") == 0) {
            sptr = strtok(NULL, "=: \t");
            goal.x = atof(sptr);
        }
        if (strcmp(sptr, "goal.y") == 0) {
            sptr = strtok(NULL, "=: \t");
            goal.y = atof(sptr);
        }
        if (strcmp(sptr, "goal.theta") == 0) {
            sptr = strtok(NULL, "=: \t");
            goal.theta = atof(sptr);
        }

        if (strcmp(sptr, "robot") == 0) {
            sptr = strtok(NULL, "=: #\t\n");
            robot = sptr;
        }
        if (strcmp(sptr, "world") == 0) {
            sptr = strtok(NULL, "=: #\t\n");
            world = sptr;
        }
    }

    fclose(fptr);
}

int runCount = 1;
void run() {
    // plan in SE2
    app::SE2RigidBodyPlanning setup;


    // load the robot and the environment
    std::string robot_fname = workingDir + "/inputs/" + robot;
    std::string env_fname = workingDir + "/inputs/" + world;
    fprintf(stderr, "%s %s\n", robot_fname.c_str(), env_fname.c_str());
    fprintf(stderr, "%lf %lf %lf -> %lf %lf %lf\n", start.x, start.y, start.theta, goal.x, goal.y, goal.theta);
    setup.setRobotMesh(robot_fname.c_str());
    setup.setEnvironmentMesh(env_fname.c_str());

    double round = 100;
    double tt[100];
    double suc = 0;
    for(int i=0;i<(int)round;++i){
        // define starting state
        base::ScopedState<base::SE2StateSpace> m_start(setup.getSpaceInformation());
        m_start->setX(start.x);
        m_start->setY(start.y);
        m_start->setYaw(start.theta);

        // define goal state
        base::ScopedState<base::SE2StateSpace> m_goal(m_start);
        m_goal->setX(goal.x);
        m_goal->setY(goal.y);
        m_goal->setYaw(goal.theta);

        // set the start & goal states
        setup.setStartAndGoalStates(m_start, m_goal);
        // choose different sampling method
        ompl::base::SpaceInformationPtr si_ptr = setup.getSpaceInformation();

        ompl::base::PlannerPtr target_planner_ptr(new ompl::geometric::RRT(si_ptr));
        //ompl::base::PlannerPtr target_planner_ptr(new ompl::geometric::PRM(si_ptr));
        //ompl::base::PlannerPtr target_planner_ptr(new ompl::geometric::BITstar(si_ptr));
        //ompl::base::PlannerPtr target_planner_ptr(new ompl::geometric::EST(si_ptr));
        setup.setPlanner(target_planner_ptr);

        // attempt to solve the problem, and print it to screen if a solution is found
        // Jul. 16 Tom
        // not a good way for evaluating since it is an approximate path ...
        //if(setup.solve()){
        //    suc++;
        //}

        // Jul. 20 Tom
        bool solved = setup.solve();
        if(solved && setup.haveExactSolutionPath()){ // Fail easily, even for simple cases!
        //if(solved && setup.haveSolutionPath()){
            suc++;
        }
        //setup.getSolutionPath().print(std::cout);

        tt[i] = setup.getLastPlanComputationTime()*1000.0f;
    }

    double total = 0;
    double sd = 0, ave = 0;
    for(int i=0;i<(int)round;++i){
        total += tt[i];
    }
    ave = total/round;
    for(int i=0;i<(int)round;++i){
        sd += (tt[i]-ave)*(tt[i]-ave);
    }
    sd = sqrt(sd/round);
    double best = FLT_MAX;
    for(int i=0;i<(int)round;++i){
        if(tt[i] < best){
            best = tt[i];
        }
    }
    fprintf(stderr, "%s %s\n", cfgName.c_str(), setup.getPlanner()->getName().c_str());
    fprintf(stderr, "%lf %lf %lf %lf\n", ave, best, sd, suc/round);
}
