// Header
#include "Display.h"

// Standard Library
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <ctime>

#define SQR(x) ((x)*(x))

// Global Variables in triangle-qt.cpp
extern std::string fileName;

extern CFG start, goal;
extern vector<c_ply> Objs;

extern int l1, l2, thickness;
extern int SearchType;
extern double boxWidth;
extern double boxHeight;
extern bool noPath;
extern bool showAnim;
extern bool pauseAnim;
extern int path_index;
extern bool replayAnim;
extern bool prm_graph;
extern bool rrt_graph;

// usleep((99-animationSpeed)*animationSpeedScale);
extern int animationSpeed;         // control the speed on the slider
extern int animationSpeedScale;    // the scale is used when we are not rendering the boxes


/*
 * CONSTRUCTOR
 *
 * Define data members
 */
Display::Display(QWidget* parent):
    QOpenGLWidget(parent)
{}

/*
 * DESTRUCTOR
 *
 * Destroy shader program
 */
Display::~Display()
{}

void Display::initializeGL() {
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    glClearDepth(1.0f);
}


/*
 * PAINT GL
 *
 * Regenerates (if necessary) and draws scene to
 * dispaly screen.
 */
void Display::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    renderScene();
    glFlush();
}


/*
 * RESIZE GL
 *
 * Sets size of viewport.
 * If 'Display' is the wooden frame of a painting,
 * the viewport is the canvas.
 */
void Display::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height); // set origin to bottom left corner
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Display::renderScene() {

//    if (!noPath) {
//        drawPath(planner, planner->getPath());

//        if (replayAnim) {
//            replayAnim = false;

//            showAnim = true;
//            pauseAnim = false;
//        }

//        if(showAnim){
//            usleep((99-animationSpeed)*animationSpeedScale);

//            const PATH& path = planner->getPath();

//            if(path_index < 0) path_index = 0;
//            if(path_index >= path.size()) path_index = path.size()-1;

//            if (pauseAnim) {
//                drawRobot(planner->getRobot(), planner->to_physical(path[path_index]));
//            }
//            else {
//                if(path_index+1 < planner->getPath().size()){
//                    path_index++;
//                    drawRobot(planner->getRobot(), planner->to_physical(path[path_index]));
//                } else if(path_index+1 == planner->getPath().size()){
//                    drawRobot(planner->getRobot(), planner->to_physical(path[path_index]));

//                    showAnim=false;
//                    path_index = 0;
//                }
//            }
//            update();
//        }
//    }



    drawPolygons();

//    if(dynamic_cast<PRM*>(planner) != NULL && prm_graph) {
//        PRM * prm=dynamic_cast<PRM*>(planner);
//        drawGraph(prm, prm->getGraph());
//        update();
//    }
//    if(dynamic_cast<RRT*>(planner) !=NULL && rrt_graph) {
//        RRT * rrt=dynamic_cast<RRT*>(planner);
//        drawTree(rrt, rrt->getTree());
//        update();
//    }

    drawRobot(start);
    drawRobot(goal);

    //if(!noPath) drawPath();
}

//===========================================================//
//======            Functions to Draw Shapes           ======//
//===========================================================//

void Display::drawPolygons() {
    glColor3f(0.0, 0.0, 1.0);
    glLineWidth(2);

    vector<c_ply>& objs = Objs;

    for(vector<c_ply>::iterator it = objs.begin(); it != objs.end(); ++it) {
        glBegin(GL_LINE_LOOP);
        size_t size = it->getSize();
        for(size_t i=0;i<size;i++) {
            ply_vertex* p = it->operator [](i);
            glVertex2dv(p->getPos().get());
        }
        glEnd();
    }
}

void Display::drawRobot(CFG cfg){
    if(thickness == 0)
        glLineWidth(3);
    else
        glLineWidth(thickness);

    glBegin(GL_LINES);

    double deg2rad = PI/180.0f;
    Vector2d v0(cfg.x, cfg.y);
    Vector2d v1((cfg.x+(double)l1*cos(cfg.t1*deg2rad)), (cfg.y+(double)l1*sin(cfg.t1*deg2rad)));
    Vector2d v2((cfg.x+(double)l2*cos(cfg.t2*deg2rad)), (cfg.y+(double)l2*sin(cfg.t2*deg2rad)));

    glColor3f(0.0, 1.0, 0.0);
    glVertex2d(v1[0], v1[1]);
    glVertex2d(v0[0], v0[1]);

    glColor3f(1.0, 0.0, 1.0);
    glVertex2d(v0[0], v0[1]);
    glVertex2d(v2[0], v2[1]);
    glEnd();
}

void Display::drawPath(){
//    glColor3f(0.0, 0.0, 0.0);
//    glLineWidth(3);
//    glBegin(GL_LINE_STRIP);
//    for(unsigned int i=0;i<path.size();++i){
//        //drawRobot(path[i]);
//    }
//    glEnd();
}
