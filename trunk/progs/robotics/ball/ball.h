#include "Octree.h"
#include "PriorityQueue.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Graph.h"
#include "Timer.h"
#include <set>
#include "Parser.h"

#ifdef __CYGWIN32__
#include "glui.h"
#endif
#ifdef _WIN32
#include <gl/glui.h>
#endif
#ifdef __APPLE__
#include "glui.h"
#endif
#ifdef __linux__
#include <GL/glui.h>
#endif

// External Routines ========================================
//
void renderCustomView(void);
void keyOperations(void);
void reshapeCustomView(int width, int height);
void idle(int v);
void keyPressed (unsigned char key, int x, int y);
void keyUp (unsigned char key, int x, int y);
void parseConfigFile(Box*);
void run();
Octree* genEmptyTree();
void drawPath(vector<Box*>&);

Octree* OT;

// GLOBAL INPUT Parameters ========================================
//
double alpha[3] = {75, 360, 100};  // start configuration
double beta[3] = {450, 50, 400};  // goal configuration
double epsilon = 5;      // resolution parameter
Box* boxA;        // start box (containing alpha)
Box* boxB;        // goal box (containing beta)
double boxWidth = 512;      // Initial box width
double R0 = 30;        // Robot radius
int windowPosX = 250;      // X Position of Window
int windowPosY = 150;      // Y Position of Window
int QType = 1;        // The Priority Queue can be sequential (1) or random (0)
int interactive = 0;      // Run interactively? Yes (0) or No (1)
int seed = 111;        // seed for random number generator (Could also be used for BFS, etc)
double eye[3] = {0, 0, 0};
double at[3] = {0, 0, -1};
double up[3] = {0, 1, 0};
int transparency = 80;

vector<Box*> path;
bool noPath = true;      // True means there is "No path".

// GLUI controls ========================================
//
GLUI_RadioGroup* radioQType;
GLUI_RadioGroup* radioDrawOption;
GLUI_EditText* editInput;
GLUI_EditText* editDir;
GLUI_EditText* editRadius;
GLUI_EditText* editEpsilon;
GLUI_EditText* editAlphaX;
GLUI_EditText* editAlphaY;
GLUI_EditText* editAlphaZ;
GLUI_EditText* editBetaX;
GLUI_EditText* editBetaY;
GLUI_EditText* editBetaZ;
GLUI_EditText* editSeed;
GLUI_Rotation *viewRot;

// Global Variables ========================================
//
int customViewWindowID;
int topViewWindowID;
float xy_aspect;
float obj_pos[] = { (float)(-boxWidth/2.), (float)(-boxWidth/2.), (float)(2.*boxWidth) };
float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

GLfloat light0_ambient[] =  {0.0, 0.0, 0.0};
GLfloat light0_diffuse[] =  {1.0, 1.0, 1.0};
GLfloat light0_position[] = {.5f, .5f, 1.0f, 0.0f};
GLfloat light0_specular[] = {1.0, 1.0, 1.0};

bool showAnim = true;
unsigned int iPathSeg = 0;
unsigned int inSegCount = 0;
bool finishedAnim = false;
int frameRate = 65;
float segCount = 0;

int freeCount = 0;
int stuckCount = 0;
int mixCount = 0;
int mixSmallCount = 0;

bool useUserDefinedViewpoint = false;
