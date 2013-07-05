/* **************************************
 File: tri.cpp

 Description:
 This is the entry point for the running the SSS algorithm
 for a triangle robot amidst a collection of polygonal obstacles.

 To run, call with these positional arguments:

 > ./tri [interactive = 0] \
			[alpha-x = 10] [alpha-y = 360] [alpha-theta = 0]\
			[beta-x = 500] [beta-y = 20][beta-theta = 0] \
			[epsilon = 1] \
			[R0 = 30] \
			[fileName = input2.txt] \
			[boxWidth = 512] [boxHeight = 512] \
			[windoxPosX = 400] [windowPosY = 200] \
			[Qtype = 0] [seed = 111] [inputDir = inputs] \
			[deltaX = 0] [deltaY = 0] [scale = 1] \
	where 
 interactive 	 	is nature of run
 (0=interactive, >0 is non-interactive)
 alpha			is start configuration
 beta			is goal configuration
 epsilon			is resolution parameter
 R0			is robot radius
 fileName		is input file describing the environment
 box Width/Height	is initial box dimensions
 windowPos		is position of window
 Qtype			is type of the priority queue
 (sequential=0, random=1)
 seed			is seed for random number generator
 inputDir		is directory for input files
 deltaX, deltaY, scale	is the translation and scaling of input environment

 NOTE: see several examples of running this program in the Makefile.

 Format of input environment: see README FILE

 HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang

 Since Core Library  Version 2.1
 $Id: tri.cpp,v 1.3 2012/10/26 04:26:52 cheeyap Exp cheeyap $
 ************************************** */

#ifdef __CYGWIN32__
#include "GL/glui.h"
#endif
#ifdef __linux__
#include <GL/glut.h>
#include "GL/glui.h"
#endif
#ifdef _WIN32
#include <gl/glui.h>
#endif
#ifdef __APPLE__
#include <GLUI/glui.h>
#endif

#include "QuadTree.h"
#include "PriorityQueue.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Graph.h"
#include "Timer.h"
#include "stdlib.h"

#include <set>
//#include "CoreIo.h"
//#include <pthread.h>

using namespace std;

// SHAPE OF TRIANGULAR ROBOT:
// 	It is a triangle inscribed in a disc centered at the origin.
// 	The radius of the disc is R0, which the user can specify below.
//
// 	The triangle is determined by two angles 
//
//		0 < theta_1 < theta_2 < 2
//
//	There is an implicit third angle, which is theta_0 = 0.
//
//      The angles values are in multiples of Pi radians
//	(so theta_1 = 1.0 corresponds to Pi radians or 180 degrees)
//
//     	E.g., an equilateral robot would be 
//		theta_1 = 0.6667, theta_2 = 1.3333.
//
// CHOOSE ONE OF THESE:
//
// (a) Acute Triangle Robot:  THIS IS THE DEFAULT -- most examples
// 	in the Makefile are designed to give interesting results with this robot.
//
//double triRobo[2] = { 0.833333333, 1.0 };
//
// (b) Equilateral Triangle Robot:
// double triRobo[2] = {0.666666667, 1.333333333};
//
// (c) Stick Robot (very thin)
//  double triRobo[2] = {0.95, 1.05};
//
// (d) Right-Angle Isosceles Robot 
// double triRobo[2] = {0.5, 1.0};
//
// (e) Off-Center Robot 
// double triRobo[2] = {0.3, 0.6};

// GLOBAL INPUT Parameters ========================================
//////////////////////////////////////////////////////////////////////////////////
double alpha[4] = { 200, 350, 80, 30 };		// start configuration
double beta[4] = { 30, 30, 100, 50 };		// goal configuration
double epsilon = 2;			// resolution parameter
Box* boxA;				// start box (containing alpha)
Box* boxB;				// goal box (containing beta)
double boxWidth = 512;			// Initial box width
double boxHeight = 512;			// Initial box height
double R0 = 8;				// Robot radius

// Added by Zhongdi 05/08/2013 begin
// length of 2 links
double L1 = 6;
double L2 = 10;
// Added by Zhongdi 05/08/2013 end

int windowPosX = 400;			// X Position of Window
int windowPosY = 200;			// Y Position of Window
string fileName("bugtrap.txt"); 		// Input file name
//string fileName("input150.txt"); 		// Input file name
string inputDir("inputs"); 		// Path for input files
int QType = 2;				// The Priority Queue can be
//    sequential (0) or random (1)
int interactive = 0;			// Run interactively?
//    Yes (0) or No (1)
int seed = 111;				// seed for random number generator
// (Could also be used for BFS, etc)
double deltaX = 0;			// x-translation of input environment
double deltaY = 0;			// y-translation of input environment
double scale = 1;				// scaling of input environment
bool noPath = true;			// True means there is "No path.

bool hideBoxBoundary = false;  		// don't draw box boundary
bool verboseOption = false;		// don't print various statistics
string title("Triangle Robot Demos");	// title for control panel

// GLOBAL VARIABLES ========================================
//////////////////////////////////////////////////////////////////////////////////
int freeCount = 0;
int stuckCount = 0;
int mixCount = 0;
int mixSmallCount = 0;

//controls triangle drawing along path
const int TRIS_TO_SKIP = 20;
const double DIST_TO_SKIP = 4;

int renderCount = 0;
int countAAA = 0;
int countBBB = 0;
int countCCC = 0;

volatile bool renderReady = false;

// GLUI controls ========================================
//////////////////////////////////////////////////////////////////////////////////
GLUI_RadioGroup* radioStepsPerFrame;
GLUI_EditText* textCurrentStep;
GLUI_RadioGroup* radioQType;
GLUI_RadioGroup* radioDrawOption;
GLUI_RadioGroup* radioVerboseOption;
GLUI_EditText* editInput;
GLUI_EditText* editDir;
//GLUI_EditText* editRadius;
GLUI_EditText* editL1;
GLUI_EditText* editL2;
GLUI_EditText* editEpsilon;
GLUI_EditText* editAlphaX;
GLUI_EditText* editAlphaY;
GLUI_EditText* editAlphaTheta1;
GLUI_EditText* editAlphaTheta2;
GLUI_EditText* editBetaX;
GLUI_EditText* editBetaY;
GLUI_EditText* editBetaTheta1;
GLUI_EditText* editBetaTheta2;
GLUI_EditText* editSeed;

GLUI_TextBox* textBox;

// External Routines ========================================
//////////////////////////////////////////////////////////////////////////////////
void renderScene(void);
void parseConfigFile(Box*);
void idle();
void replay();
void moveAlongPath();
void run();
void genEmptyTree();
void drawPath(vector<Box*>&);
extern int fileProcessor(string inputfile);
void drawCircle(float Radius, int numPoints, double x, double y, double r,
		double g, double b);
void drawLine();
//void drawTri(Box*);
//void drawTri(Box*, double, double);
void drawLinks(Box*);
void drawLinks(Box*, double, double);

//void *thread_render(void* arg);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<Box*> allLeaf;
vector<Set*> allSet;
QuadTree* QT;

int totalSteps = 0;
int currentStep = 0;
int stepIncrease = 0;
int currentPathStep = 0;

// 0: no animation 1: animation 2: stop 3: move along path
int animationOption = 0;

GLUI_Button* buttonRun;
GLUI_Button* buttonReplay;
GLUI_Button* buttonMoveAlongPath;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//find path using simple heuristic:
//use distance to beta as key in PQ, see dijkstraQueue
template<typename Cmp>
bool findPath(Box* a, Box* b, QuadTree* QT, int& ct) {
	bool isPath = false;
	vector<Box*> toReset;
	a->dist2Source = 0;
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!237 a  " << a->x << " " << a->y
			<< " " << a->xi[0] << " " << a->xi[1] << " " << a->xi[2] << " "
			<< a->xi[3] << " " << a->status << endl;
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!238 a->dist2Source  "
			<< a->dist2Source << endl;
	dijkstraQueue<Cmp> dijQ;
	dijQ.push(a);
	toReset.push_back(a);
	while (!dijQ.empty()) {

		Box* current = dijQ.extract();
		current->visited = true;

		// if current is MIXED, try expand it and push the children that is
		// ACTUALLY neighbors of the source set (set containing alpha) into the dijQ again
		if (current->status == Box::MIXED) {
			vector<Box*> cldrn;
			if (QT->expand(current, cldrn)) {
				++ct;
				for (int i = 0; i < (int) cldrn.size(); ++i) {
//					cout<<"CountAAA: "<< countAAA << endl;
//					countAAA++;
//					glutPostRedisplay();
//					glFlush();
					// go through neighbors of each child to see if it's in source set
					// if yes, this child go into the dijQ					
					bool isNeighborOfSourceSet = false;
					for (int j = 0; j < 4 && !isNeighborOfSourceSet; ++j) {
						for (vector<Box*>::iterator iter =
								cldrn[i]->Nhbrs[j].begin();
								iter < cldrn[i]->Nhbrs[j].end(); ++iter) {
							Box* n = *iter;
							cout
									<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!264 dist2Source != 0  "
									<< n->x << " " << n->y << " " << n->xi[0]
									<< " " << n->xi[1] << " " << n->xi[2] << " "
									<< n->xi[3] << " " << endl;
							cout
									<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!267 a->dist2Source  "
									<< a->dist2Source << endl;
							cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!268 a  "
									<< a->x << " " << a->y << " " << a->xi[0]
									<< " " << a->xi[1] << " " << a->xi[2] << " "
									<< a->xi[3] << " " << endl;
							if (n->dist2Source == 0) {
								cout
										<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!265 dist2Source = 0  "
										<< n->x << " " << n->y << endl;
								isNeighborOfSourceSet = true;
								break;
							}
						}

					}

					if (isNeighborOfSourceSet) {
						switch (cldrn[i]->getStatus()) {
						//if it's FREE, also insert to source set
						case Box::FREE:
							cldrn[i]->dist2Source = 0;
							cout
									<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!277 dist2Source = 0  "
									<< cldrn[i]->x << " " << cldrn[i]->y
									<< endl;
							dijQ.push(cldrn[i]);
							toReset.push_back(cldrn[i]);
							break;
						case Box::MIXED:
							dijQ.push(cldrn[i]);
							toReset.push_back(cldrn[i]);
							break;
						case Box::STUCK:
							//cerr << "inside FindPath: STUCK case not treated" << endl;
							break;
						case Box::UNKNOWN:
							//cerr << "inside FindPath: UNKNOWN case not treated" << endl;
							break;
						}
					}
				}
			}
			continue;
		}

		cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!302 a->dist2Source  "
				<< a->dist2Source << endl;
		//found path!
		if (current == b) {
			isPath = true;
			break;
		}

		// if current is not MIXED, then must be FREE
		// go through it's neighbors and add FREE and MIXED ones to dijQ
		// also add FREE ones to source set 
		for (int i = 0; i < 4; ++i) {
			for (vector<Box*>::iterator iter = current->Nhbrs[i].begin();
					iter < current->Nhbrs[i].end(); ++iter) {
				Box* neighbor = *iter;
				if (!neighbor->visited && neighbor->dist2Source == -1
						&& (neighbor->status == Box::FREE
								|| neighbor->status == Box::MIXED)) {
					cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!317 "
							<< neighbor->x << " " << neighbor->y << endl;
					if (neighbor->status == Box::FREE) {
						neighbor->dist2Source = 0;
						cout
								<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!316 dist2Source = 0  "
								<< neighbor->x << " " << neighbor->y << endl;
					}
					dijQ.push(neighbor);
					toReset.push_back(neighbor);
				}
			}
		}
	}

	//these two fields are also used in dijkstraShortestPath
	// need to reset
	for (int i = 0; i < (int) toReset.size(); ++i) {
		toReset[i]->visited = false;
		toReset[i]->dist2Source = -1;
	}

	return isPath;
}

void TimerFunction(int p) {
	glutPostRedisplay();
	glutTimerFunc(16, TimerFunction, 1);
}

//void run_thread(){
//	// new thread for render
//	int res;
//	pthread_t a_thread;
//	void *thread_result;
//
//	res = pthread_create(&a_thread, NULL, thread_render, NULL);
//	if (0 != res) {
//		perror("Thread creation faied");
//		exit(EXIT_FAILURE);
//	}
//}

// MAIN PROGRAM: ========================================
int main(int argc, char* argv[]) {
	if (argc > 1)
		interactive = atoi(argv[1]);	// Interactive (0) or no (>0)
	if (argc > 2)
		alpha[0] = atof(argv[2]);		// start x
	if (argc > 3)
		alpha[1] = atof(argv[3]);		// start y
	if (argc > 4)
		alpha[2] = atof(argv[4]);// start theta1, convert from degree to radian
	if (argc > 5)
		alpha[3] = atof(argv[5]);// start theta2, convert from degree to radian
	if (argc > 6)
		beta[0] = atof(argv[6]);		// goal x
	if (argc > 7)
		beta[1] = atof(argv[7]);		// goal y
	if (argc > 8)
		beta[2] = atof(argv[8]);// goal theta1, convert from degree to radian
	if (argc > 9)
		beta[3] = atof(argv[9]);// goal theta2, convert from degree to radian
	if (argc > 10)
		epsilon = atof(argv[10]);		// epsilon (resolution)
	if (argc > 11)
		L1 = atof(argv[11]);		// robot length1
	if (argc > 12)
		L2 = atof(argv[12]);		// robot length2
	if (argc > 13)
		fileName = argv[13]; 		// Input file name
	if (argc > 14)
		boxWidth = atof(argv[14]);		// boxWidth
	if (argc > 15)
		boxHeight = atof(argv[15]);	// boxHeight
	if (argc > 16)
		windowPosX = atoi(argv[16]);	// window X pos
	if (argc > 17)
		windowPosY = atoi(argv[17]);	// window Y pos
	if (argc > 18)
		QType = atoi(argv[18]);	// PriorityQ Type (random or no)
	if (argc > 19)
		seed = atoi(argv[19]);		// for random number generator
	if (argc > 20)
		inputDir = argv[20];		// path for input files
	if (argc > 21)
		deltaX = atof(argv[21]);	// x-translation of input file
	if (argc > 22)
		deltaY = atof(argv[22]);	// y-translation of input file
	if (argc > 23)
		scale = atof(argv[23]);		// scaling of input file
	if (argc > 24)
		verboseOption = atoi(argv[24]);	// verboseOption
	if (argc > 25)
		title = argv[25];		// title

	// Added by Zhongdi 05/08/2013 begin
	// calculate the R of the robot
	R0 = max(L1, L2);
	// Added by Zhongdi 05/08/2013 end

	cout << "Before interactive, Qtype= " << QType << "\n";

	if (interactive > 0) {	// non-interactive
		// do something...
		cout << "Non Interactive Run of Disc Robot" << endl;
		//if (noPath)
		//	cout << "No Path Found!" << endl;
		//else
		//	cout << "Path was Found!" << endl;
		//return 0;
	}

//	alpha[2] /= 180.0;		// start theta, convert from degree to radian
//	beta[2] /= 180.0;		// goal theta, convert from degree to radian
//	alpha[3] /= 180.0;		// start theta, convert from degree to radian
//	beta[3] /= 180.0;		// goal theta, convert from degree to radian

	// Else, set up for GLUT/GLUI interactive display:

	if (interactive == 0) {
//cout<<"before glutInit\n";
		glutInit(&argc, argv);
		glutInitWindowPosition(windowPosX, windowPosY);
		glutInitWindowSize(boxWidth, boxWidth);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		int windowID = glutCreateWindow("Motion Planning");
		glutDisplayFunc(renderScene);
//		glutIdleFunc(&idle);
//		glutTimerFunc(16, TimerFunction,1);

		GLUI_Master.set_glutIdleFunc(NULL);

		//Chee: trying to get demos to take a "title string"
		std::stringstream sss;
		sss << "2-Links Control: " << title;	// create full file name
		std::string title_string = sss.str();
		const char * test("2-Links Demo");

		GLUI *glui = GLUI_Master.create_glui(test, 0,
				windowPosX + boxWidth + 20, windowPosY);

		// SETTING UP THE CONTROL PANEL:

		editInput = glui->add_edittext("Input file:", GLUI_EDITTEXT_TEXT);
		editInput->set_text((char*) fileName.c_str());
		editDir = glui->add_edittext("Input Directory:", GLUI_EDITTEXT_TEXT);
		editDir->set_text((char*) inputDir.c_str());
		// Chee added this "grouping" for Robot Specifications:
		GLUI_Panel * robot_box_panel = glui->add_panel("Robot Specs");
//		editRadius = glui->add_edittext_to_panel(robot_box_panel, "Radius:",
//				GLUI_EDITTEXT_FLOAT);
//		editRadius->set_float_val(R0);
		editL1 = glui->add_edittext_to_panel(robot_box_panel, "L1:",
				GLUI_EDITTEXT_FLOAT);
		editL1->set_float_val(L1);
		editL2 = glui->add_edittext_to_panel(robot_box_panel, "L2:",
				GLUI_EDITTEXT_FLOAT);
		editL2->set_float_val(L2);
		editEpsilon = glui->add_edittext_to_panel(robot_box_panel, "Epsilon:",
				GLUI_EDITTEXT_FLOAT);
		editEpsilon->set_float_val(epsilon);
		//editRadius = glui->add_edittext( "Radius:", GLUI_EDITTEXT_FLOAT );
		//editRadius->set_float_val(R0);
		//editEpsilon = glui->add_edittext( "Epsilon:", GLUI_EDITTEXT_FLOAT );
		//editEpsilon->set_float_val(epsilon);

		// Chee added this "grouping" for Alpha
		GLUI_Panel * alpha_box_panel = glui->add_panel(
				"Alpha (start configuration)");
		editAlphaX = glui->add_edittext_to_panel(alpha_box_panel, "x:",
				GLUI_EDITTEXT_FLOAT);
		editAlphaX->set_float_val(alpha[0]);
		editAlphaY = glui->add_edittext_to_panel(alpha_box_panel, "y:",
				GLUI_EDITTEXT_FLOAT);
		editAlphaY->set_float_val(alpha[1]);
		editAlphaTheta1 = glui->add_edittext_to_panel(alpha_box_panel, "theta1",
				GLUI_EDITTEXT_FLOAT);
		editAlphaTheta1->set_float_val(alpha[2]);
		editAlphaTheta2 = glui->add_edittext_to_panel(alpha_box_panel, "theta2",
				GLUI_EDITTEXT_FLOAT);
		editAlphaTheta2->set_float_val(alpha[3]);
		// ORIGINAL:
		//editAlphaX = glui->add_edittext( "alpha.x:", GLUI_EDITTEXT_FLOAT );
		//editAlphaX->set_float_val(alpha[0]);
		//editAlphaY = glui->add_edittext( "alpha.y:", GLUI_EDITTEXT_FLOAT );
		//editAlphaY->set_float_val(alpha[1]);
		//editAlphaTheta = glui->add_edittext("alpha.theta:",GLUI_EDITTEXT_FLOAT);
		//editAlphaTheta->set_float_val(alpha[2]);

		// Chee added this "grouping" for Beta
		GLUI_Panel * beta_box_panel = glui->add_panel(
				"Beta (goal configuration)");
		editBetaX = glui->add_edittext_to_panel(beta_box_panel, "x:",
				GLUI_EDITTEXT_FLOAT);
		editBetaX->set_float_val(beta[0]);
		editBetaY = glui->add_edittext_to_panel(beta_box_panel, "y:",
				GLUI_EDITTEXT_FLOAT);
		editBetaY->set_float_val(beta[1]);
		editBetaTheta1 = glui->add_edittext_to_panel(beta_box_panel, "theta1",
				GLUI_EDITTEXT_FLOAT);
		editBetaTheta1->set_float_val(beta[2]);
		editBetaTheta2 = glui->add_edittext_to_panel(beta_box_panel, "theta2",
				GLUI_EDITTEXT_FLOAT);
		editBetaTheta2->set_float_val(beta[3]);
		// ORIGINAL:
		//editBetaX = glui->add_edittext( "beta.x:", GLUI_EDITTEXT_FLOAT );
		//editBetaX->set_float_val(beta[0]);
		//editBetaY = glui->add_edittext( "beta.y:", GLUI_EDITTEXT_FLOAT );
		//editBetaY->set_float_val(beta[1]);
		//editBetaTheta =glui->add_edittext("beta.theta:",GLUI_EDITTEXT_FLOAT );
		//editBetaTheta->set_float_val(beta[2]);

		editSeed = glui->add_edittext("seed:", GLUI_EDITTEXT_INT);
		editSeed->set_int_val(seed);

		buttonRun = glui->add_button("Run", -1, (GLUI_Update_CB) run);
		buttonRun->set_name("Run"); // Hack, to avoid "unused warning" (Chee)

		GLUI_Panel * replay_panel = glui->add_panel("replay configuration");
		buttonReplay = glui->add_button_to_panel(replay_panel, "Replay", -1,
				(GLUI_Update_CB) replay);
		buttonReplay->set_name("Replay");

		radioStepsPerFrame = glui->add_radiogroup_to_panel(replay_panel);
		glui->add_radiobutton_to_group(radioStepsPerFrame, "1 step/frame");
		glui->add_radiobutton_to_group(radioStepsPerFrame, "10 steps/frame");
		glui->add_radiobutton_to_group(radioStepsPerFrame, "100 steps/frame");

		textCurrentStep = glui->add_edittext_to_panel(replay_panel,
				"Current Step", GLUI_EDITTEXT_INT);
		textCurrentStep->set_int_val(currentStep);

		buttonMoveAlongPath = glui->add_button_to_panel(replay_panel,
				"Move Along Path", -1, (GLUI_Update_CB) moveAlongPath);
		buttonMoveAlongPath->set_name("Move Along Path");

		// New column:
		glui->add_column(true);

		glui->add_separator();
		radioQType = glui->add_radiogroup();

		glui->add_radiobutton_to_group(radioQType, "Random Heuristic");
		glui->add_radiobutton_to_group(radioQType, "BFS");
		glui->add_radiobutton_to_group(radioQType, "Greedy");
		glui->add_radiobutton_to_group(radioQType, "Dist+Size");
		glui->add_radiobutton_to_group(radioQType, "Voronoi Heuristic");

		glui->add_separator();
		radioQType->set_int_val(QType);

		radioDrawOption = glui->add_radiogroup(0, -1,
				(GLUI_Update_CB) renderScene);
		glui->add_radiobutton_to_group(radioDrawOption, "Show Box Boundary");
		glui->add_radiobutton_to_group(radioDrawOption, "Hide Box Boundary");

		glui->add_separator();
		radioVerboseOption = glui->add_radiogroup();

		glui->add_radiobutton_to_group(radioVerboseOption, "Non-Verbose");
		glui->add_radiobutton_to_group(radioVerboseOption,
				"Verbose (print statistics)");

		radioVerboseOption->set_int_val(verboseOption);
		glui->add_separator();

		textBox = new GLUI_TextBox(glui, true);
		textBox->set_h(250);
		textBox->set_w(310);
		textBox->disable();

		// Quit button
		glui->add_button("Quit", 0, (GLUI_Update_CB) exit);

		glui->set_main_gfx_window(windowID);
	}
//	// new thread for render
//	int res;
//	pthread_t a_thread;
//	void *thread_result;
//
//	res = pthread_create(&a_thread, NULL, thread_render, NULL);
//	if (0 != res) {
//		perror("Thread creation faied");
//		exit(EXIT_FAILURE);
//	}

	run();

//cout<<"before run\n";
	// PERFORM THE INITIAL RUN OF THE ALGORITHM
	//==========================================
//	run(); 	// make it do something interesting from the start!!!

	// SHOULD WE STOP or GO INTERACTIVE?
	//==========================================
//cout<<"after run\n";
	if (interactive > 0) {	// non-interactive
		// do something...
		cout << "Non Interactive Run of Disc Robot" << endl;
		if (noPath)
			cout << "No Path Found!" << endl;
		else
			cout << "Path was Found!" << endl;
		return 0;
	} else {

		cout << "33333333333333333333333333333333333333" << endl;
		glutMainLoop();
	}

	return 0;
}

void genEmptyTree() {
	Box* root = new Box(boxWidth / 2, boxHeight / 2, boxWidth, boxHeight);

	Box::r0 = R0;
	Box::l1 = L1;
	Box::l2 = L2;

//	cout << "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
//			<< endl;
	//todo
//	Box::THETA_MIN = min(min(triRobo[0], triRobo[1] - triRobo[0]),
//			2 - triRobo[1]);
	// todo added by Zhongdi 05/08/2013
//	Box::THETA_MIN = 0.1;

	Box::pAllLeaf = &allLeaf;

//	cout << "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee" << endl;
	if (!allLeaf.empty() && allLeaf.size() != 0) {
//		cout << "ggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg" <<allLeaf.size()<< endl;
		for (vector<Box*>::iterator it = allLeaf.begin(); it != allLeaf.end();
				++it) {
//					cout << "hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh" <<(*it)->x<< endl;
			delete *it;
		}
	}

	if (!allSet.empty() && allSet.size() != 0) {
		for (vector<Set*>::iterator it = allSet.begin(); it != allSet.end();
				++it) {
			delete *it;
		}
	}

//	cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" << endl;
	allLeaf.clear();
	allLeaf.push_back(root);
	allSet.clear();

	parseConfigFile(root);
	root->updateStatus();

//	cout << "ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" << endl;
	if (QT) {
		delete (QT);
	}
	QT = new QuadTree(root, epsilon, QType, seed++); // Note that seed keeps changing!

	if (verboseOption)
		cout << "done genEmptyTree \n";
}
void idle() {
//	cout << "idleidleidleidleidleidleidleidleidle" << endl;
	renderScene();
}

void moveAlongPath() {
//	textCurrentStep->set_int_val(currentStep);
	if (animationOption != 3) {
		animationOption = 3;
//		if (radioStepsPerFrame->get_int_val() == 0) {
//			stepIncrease = 1;
//		} else if (radioStepsPerFrame->get_int_val() == 1) {
//			stepIncrease = 10;
//		} else {
//			stepIncrease = 100;
//		}

//		if (animationOption == 0) {
//			currentStep = 1;
//		}
		buttonMoveAlongPath->set_name("Stop");
	} else {
		animationOption = 0;
		stepIncrease = 0;
		buttonMoveAlongPath->set_name("Move Along Path");
	}

	if (interactive == 0 && animationOption == 3) {
		glutPostRedisplay();
	}
}

void replay() {
	textCurrentStep->set_int_val(currentStep);
	if (animationOption != 1) {
		animationOption = 1;
		if (radioStepsPerFrame->get_int_val() == 0) {
			stepIncrease = 1;
		} else if (radioStepsPerFrame->get_int_val() == 1) {
			stepIncrease = 10;
		} else {
			stepIncrease = 100;
		}

//		if (animationOption == 0) {
//			currentStep = 1;
//		}
		buttonReplay->set_name("Stop");
	} else {
		animationOption = 2;
		stepIncrease = 0;
		buttonReplay->set_name("Replay");
	}

	if (interactive == 0 && animationOption == 1) {
		glutPostRedisplay();
	}
}

void run() {
//	cout << "1111111111111111111111111111111111111111111111111" << endl;

	animationOption = 0;
	currentStep = 0;

	if (interactive == 0) {
		//update from glui live variables
		fileName = editInput->get_text();
		inputDir = editDir->get_text();
//		R0 = editRadius->get_float_val();
		L1 = editL1->get_float_val();
		L2 = editL2->get_float_val();
		if (L1 > L2) {
			R0 = L1;
		} else {
			R0 = L2;
		}
		epsilon = editEpsilon->get_float_val();
		alpha[0] = editAlphaX->get_float_val();
		alpha[1] = editAlphaY->get_float_val();
		alpha[2] = editAlphaTheta1->get_float_val();
		while (alpha[2] >= 360) {
			alpha[2] -= 360;
		}
		while (alpha[2] < 0) {
			alpha[2] += 360;
		}
		editAlphaTheta1->set_float_val(alpha[2]);
		alpha[3] = editAlphaTheta2->get_float_val();
		while (alpha[3] >= 360) {
			alpha[3] -= 360;
		}
		while (alpha[3] < 0) {
			alpha[3] += 360;
		}
		editAlphaTheta2->set_float_val(alpha[3]);
		beta[0] = editBetaX->get_float_val();
		beta[1] = editBetaY->get_float_val();
		beta[2] = editBetaTheta1->get_float_val();
		while (beta[2] >= 360) {
			beta[2] -= 360;
		}
		while (beta[2] < 0) {
			beta[2] += 360;
		}
		editBetaTheta1->set_float_val(beta[2]);
		beta[3] = editBetaTheta2->get_float_val();
		while (beta[3] >= 360) {
			beta[3] -= 360;
		}
		while (beta[3] < 0) {
			beta[3] += 360;
		}
		editBetaTheta2->set_float_val(beta[3]);

		QType = radioQType->get_int_val();
	}

	if (verboseOption) {
		cout << "   radius = " << R0 << ", eps = " << epsilon << endl;
		cout << "   alpha = (" << alpha[0] << ", " << alpha[1] << ", "
				<< alpha[2] << ", " << alpha[3] << ")" << endl;
		cout << "   beta = (" << beta[0] << ", " << beta[1] << ", " << beta[2]
				<< ", " << beta[3] << ")" << endl;
	}
//	cout << "222222222222222222222222222222222222222222222222" << endl;
	genEmptyTree();

//	cout << "0000000000000000000000000000000000000000000000000" << endl;
//	renderReady = true;

	if (interactive == 0) {
		cout << "interactive : " << interactive << endl;
//		glutPostRedisplay();
	}
	Timer t;

	t.start();

	noPath = false;	// initially, pretend we have found path 
	int ct = 0;	// number of times a node is expanded

	if (QType == 0 || QType == 1) {
		boxA = QT->getBox(alpha[0], alpha[1], alpha[2], alpha[3], ct);
		if (!boxA) {
			noPath = true;
			cout << "Start Configuration is not free\n";
		}

		boxB = QT->getBox(beta[0], beta[1], beta[2], beta[3], ct);
		if (!boxB) {
			noPath = true;
			cout << "Goal Configuration is not free\n";
		}

		// In the following loop, "noPath" should really mean "hasPath"
		//	Otherwise, we should pre-initialize "noPath" to true
		//	before entering loop...
		while (!noPath && !QT->isConnected(boxA, boxB)) {
			if (interactive == 0) {
//				glutPostRedisplay();
			}
			if (!QT->expand()) {
				noPath = true;
			}
			++ct;
		}
	} else if (QType == 2 || QType == 3 || QType == 4) {
		boxA = QT->getBox(alpha[0], alpha[1], alpha[2], alpha[3], ct);
		if (!boxA) {
			noPath = true;
			cout << "Start Configuration is not free\n";
		}

		boxB = QT->getBox(beta[0], beta[1], beta[2], beta[3], ct);
		if (!boxB) {
			noPath = true;
			cout << "Goal Configuration is not free\n";
		}
		if (!noPath) {
			if (QType == 2) {
				noPath = !findPath<DistCmp>(boxA, boxB, QT, ct);
			} else if (QType == 3) {
				noPath = !findPath<DistPlusSizeCmp>(boxA, boxB, QT, ct);
			} else if (QType == 4) {
				noPath = !findPath<VorCmp>(boxA, boxB, QT, ct);
			}
		}
	}

	t.stop();

	if (interactive == 0) {
		glutPostRedisplay();
	}
	if (verboseOption)
		cout << ">>>>>>>>>>>>>>> > > > > > > >>>>>>>>>>>>>>>>>>\n";
	cout << ">>\n";
	if (!noPath)
		cout << ">>      ----->>  Path Found !" << endl;
	else
		cout << ">>      ----->>  No Path !" << endl;
	cout << ">>\n";
	cout << ">>      ----->>  Time used: " << t.getElapsedTimeInMilliSec()
			<< " ms" << endl;
	cout << ">>\n";
	// cout << ">>      ----->>  Qtype: " << QType << "\n";
	cout << ">>      ----->>  Qtype: ";
	switch (QType) {
	case 0:
		cout << "Random Strategy\n";
		break;
	case 1:
		cout << "BFS Strategy\n";
		break;
	case 2:
		cout << "Greedy Strategy\n";
		break;
	case 3:
		cout << "Dist+Size Strategy\n";
		break;
	case 4:
		cout << "Voronoi Strategy\n";
		break;
	}
	cout << ">>\n";
	if (verboseOption)
		cout << ">>>>>>>>>>>>>>> > > > > > > >>>>>>>>>>>>>>>>>>\n";
	if (verboseOption) {
		cout << "Expanded " << ct << " times" << endl;
		cout << "total Free boxes: " << freeCount << endl;
		cout << "total Stuck boxes: " << stuckCount << endl;
		cout << "total Mixed boxes smaller than epsilon: " << mixSmallCount
				<< endl;
		cout << "total Mixed boxes bigger than epsilon: "
				<< mixCount - ct - mixSmallCount << endl;
	}

	totalSteps = allLeaf.size();

	stringstream ssout;
	if (!noPath)
		ssout << "    ---->>   PATH FOUND !" << endl;
	else
		ssout << "    ---->>  NO PATH !" << endl;
	ssout << "    ---->>   TIME USED: " << t.getElapsedTimeInMilliSec() << " ms"
			<< endl;
	ssout << "    ---->>   TOTAL STEPS: " << totalSteps << endl;
	if (verboseOption) {
		ssout << "Expanded " << ct << " times" << endl;
		ssout << "total Free boxes: " << freeCount << endl;
		ssout << "total Stuck boxes: " << stuckCount << endl;
		ssout << "total Mixed boxes smaller than epsilon: " << mixSmallCount
				<< endl;
		ssout << "total Mixed boxes bigger than epsilon: "
				<< mixCount - ct - mixSmallCount << endl;
	}
	textBox->set_text(ssout.str().c_str());

	freeCount = stuckCount = mixCount = mixSmallCount = 0;
	cout << "####################### END of RUN ######################\n";

//	renderReady = false;
//	renderReady = true;
}		//run

//void drawTri(Box* b) {
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	glColor3f(1, 0, 0);
//	glLineWidth(2);
//	glBegin(GL_TRIANGLES);
//
//	glVertex2f(R0 * cos((b->xi[0]) * PI) + b->x,
//			R0 * sin((b->xi[0]) * PI) + b->y);
//	glVertex2f(R0 * cos((triRobo[0] + b->xi[0]) * PI) + b->x,
//			R0 * sin((triRobo[0] + b->xi[0]) * PI) + b->y);
//	glVertex2f(R0 * cos((triRobo[1] + b->xi[0]) * PI) + b->x,
//			R0 * sin((triRobo[1] + b->xi[0]) * PI) + b->y);
//
//	glEnd();
//	glLineWidth(1.0);
//}
//
//void drawTri(Box* b, double x, double y) {
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	glColor3f(1, 0, 0);
//	glLineWidth(2);
//	glBegin(GL_TRIANGLES);
//
//	glVertex2f(R0 * cos((b->xi[0]) * PI) + x, R0 * sin((b->xi[0]) * PI) + y);
//	glVertex2f(R0 * cos((triRobo[0] + b->xi[0]) * PI) + x,
//			R0 * sin((triRobo[0] + b->xi[0]) * PI) + y);
//	glVertex2f(R0 * cos((triRobo[1] + b->xi[0]) * PI) + x,
//			R0 * sin((triRobo[1] + b->xi[0]) * PI) + y);
//
//	glEnd();
//	glLineWidth(1.0);
//}

void drawLinks(Box* b) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1, 0, 0);
	glLineWidth(2);
	glBegin(GL_LINES);

//	glVertex2f(R0 * cos((b->xi[0]) * PI) + b->x,
//			R0 * sin((b->xi[0]) * PI) + b->y);
//	glVertex2f(R0 * cos((triRobo[0] + b->xi[0]) * PI) + b->x,
//			R0 * sin((triRobo[0] + b->xi[0]) * PI) + b->y);
//	glVertex2f(R0 * cos((triRobo[1] + b->xi[0]) * PI) + b->x,
//			R0 * sin((triRobo[1] + b->xi[0]) * PI) + b->y);
	glVertex2d(b->x, b->y);
	glVertex2d(L1 * cos((b->xi[0] / 180) * PI) + b->x,
			L1 * sin((b->xi[0] / 180) * PI) + b->y);
	glEnd();
	glColor3f(1, 0, 1);
	glBegin(GL_LINES);
	glVertex2d(b->x, b->y);
	glVertex2d(L2 * cos((b->xi[2] / 180) * PI) + b->x,
			L2 * sin((b->xi[2] / 180) * PI) + b->y);
	glEnd();

	std::cout << "hahahahhhhhhhhhhhhhhhhhh  box x=" << b->x << " y=" << b->y
			<< endl;
	std::cout << "hahahahhhhhhhhhhhhhhhhhh  box xi[0]=" << b->xi[0] << " xi[1]="
			<< b->xi[1] << endl;
	glLineWidth(1.0);
}

void drawLinksSrcDst(double* configuration) {
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1, 0, 0);
	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex2f(configuration[0], configuration[1]);
	glVertex2f(L1 * cos((configuration[2] / 180) * PI) + configuration[0],
			L1 * sin((configuration[2] / 180) * PI) + configuration[1]);
	glEnd();
	glColor3f(1, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(configuration[0], configuration[1]);
	glVertex2f(L2 * cos((configuration[3] / 180) * PI) + configuration[0],
			L2 * sin(configuration[3] / 180 * PI) + configuration[1]);
	glEnd();
	glLineWidth(1.0);
}

void drawLinks(Box* b, double x, double y) {
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1, 0, 0);
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(L1 * cos((b->xi[0] / 180) * PI) + x,
			L1 * sin((b->xi[0] / 180) * PI) + y);
	glEnd();
	glColor3f(1, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(L2 * cos((b->xi[2] / 180) * PI) + x,
			L2 * sin(b->xi[2] / 180 * PI) + y);
	glEnd();
	glLineWidth(1.0);
}

void drawPath(vector<Box*>& path) {
	glColor3f(0.5, 0, 0.25);
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);
	glVertex2d(beta[0], beta[1]);
	for (int i = 0; i < (int) path.size(); ++i) {
		glVertex2d(path[i]->x, path[i]->y);
	}
	glVertex2d(alpha[0], alpha[1]);
	glEnd();
	glLineWidth(1.0);

	int skipped = 0;
	double distSkipped = 0;
	drawLinksSrcDst(alpha);
	drawLinksSrcDst(beta);
	if (path.size() != 0) {
		for (int i = path.size() - 1; i >= 0; i--) {
			if (i > 0) {
				double dist = sqrt(
						(path[i]->x - path[i - 1]->x)
								* (path[i]->x - path[i - 1]->x)
								+ (path[i]->y - path[i - 1]->y)
										* (path[i]->y - path[i - 1]->y));
				distSkipped += dist;
				++skipped;
				//control triangles drawing:
				//enable (&& dist>= 1e-9) to hide same spot rotation
				if ((skipped > TRIS_TO_SKIP || distSkipped > DIST_TO_SKIP))	// && dist>= 1e-9 )
				{
					drawLinks(path[i]);
//					drawCircle(R0, 100, path[i]->x, path[i]->y, 0, 0, 1);
					skipped = 0;
					distSkipped = 0;
				}
			} else {
				drawLinks(path[i]);
				drawCircle(R0, 100, path[i]->x, path[i]->y, 0, 0, 1);
			}
			if (animationOption == 3
					&& i == path.size() - 1 - currentPathStep) {
//				for (int i = 0; i < 60000000; i++) {
//					for (int j = 0; j < 10000000; j++) {
//						i = i + 1;
//						j = j + 1;
//					}
//				}
				sleep(1);
				break;
			}
		}
	}

	if (animationOption == 3) {
		currentPathStep++;
//		for(int i = 0; i < 6000000; i++){
//			for(int j = 0; j < 1000000; j++){
//				i = i + 1;
//				j = j + 1;
//			}
//		}
		if (currentPathStep == path.size()) {
			currentPathStep = 0;
		}
	}

}

void drawQuad(Box* b) {

//	cout<<"888888888888888888888888888888888888"<<endl;

	if (b == NULL || b == 0 || !b->isLeaf || b->status == Box::UNKNOWN) {
		return;
	}

	//if (b->status != Box::FREE)
	//{
	//	return;
	//}
//	cout<<"EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"<<endl;
	switch (b->status) {
	case Box::FREE:
		glColor4f(0.25, 1, 0.25, 0.5);
		break;
	case Box::STUCK:
		glColor4f(1, 0.25, 0.25, 0.5);
		break;
	case Box::MIXED:
		glColor4f(1, 1, 0.25, 0.1);
		if (b->height < epsilon || b->width < epsilon) {
			glColor4f(0.5, 0.5, 0.5, 0.1);
		}
		break;
	case Box::UNKNOWN:
//todo
//std::cout << "UNKNOWN in drawQuad" << std::endl;
		break;
	}
//	cout<<"999999999999999999999999999999999999999999999999999999999"<<endl;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex2d(b->x - b->width / 2, b->y - b->height / 2);
	glVertex2d(b->x + b->width / 2, b->y - b->height / 2);
	glVertex2d(b->x + b->width / 2, b->y + b->height / 2);
	glVertex2d(b->x - b->width / 2, b->y + b->height / 2);
	glEnd();

	if (!hideBoxBoundary) {
		glColor3f(0, 0, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_POLYGON);
		glVertex2d(b->x - b->width / 2, b->y - b->height / 2);
		glVertex2d(b->x + b->width / 2, b->y - b->height / 2);
		glVertex2d(b->x + b->width / 2, b->y + b->height / 2);
		glVertex2d(b->x - b->width / 2, b->y + b->height / 2);
		glEnd();
	}
}

void drawWalls(Box* b) {
	glColor3f(0, 0, 0.7);
	glLineWidth(2.0);
	for (list<Wall*>::iterator iter = b->walls.begin(); iter != b->walls.end();
			++iter) {
		Wall* w = *iter;
		glBegin(GL_LINES);
		glVertex2d(w->src->x, w->src->y);
		glVertex2d(w->dst->x, w->dst->y);
		glEnd();
	}
	glLineWidth(1.0);
}

void drawCircle(float Radius, int numPoints, double x, double y, double r,
		double g, double b) {
	glColor3d(r, g, b);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= numPoints; ++i) {
		float Angle = i * (2.0 * 3.1415926 / numPoints);
		float X = cos(Angle) * Radius;
		float Y = sin(Angle) * Radius;
		glVertex2f(X + x, Y + y);
	}
	glEnd();
}

void filledCircle(double radius, double x, double y, double r, double g,
		double b) {
	int numPoints = 100;
	glColor3d(r, g, b);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= numPoints; ++i) {
		float Angle = i * (2.0 * 3.1415926 / numPoints);
		float X = cos(Angle) * radius;
		float Y = sin(Angle) * radius;
		glVertex2f(X + x, Y + y);
	}
	glEnd();
}

void drawLine() {
	if (noPath) {
		glColor3f(0, 0, 0);
	} else {
		glColor3f(1, 0, 0);
	}
	glLineWidth(3.0);
	glBegin(GL_LINES);
	glVertex2f(alpha[0], alpha[1]);
	glVertex2f(beta[0], beta[1]);
	glEnd();
	glLineWidth(1.0);
}

void renderScene(void) {

	if (animationOption == 1) {
		currentStep += stepIncrease;
	}
	cout << "renderCount :" << renderCount << endl;
	renderCount++;

//	while (!renderReady) {
//			cout<<"44444444444444444444444444444444444444444"<<endl;
//	}

	hideBoxBoundary = radioDrawOption->get_int_val();
	verboseOption = radioVerboseOption->get_int_val();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glScalef(2.0 / boxWidth, 2.0 / boxHeight, 0);
	glTranslatef(-boxWidth / 2, -boxHeight / 2, 0);

//	cout
//			<< "666666666666666666666666666666666666666666666666666666666666666666666"
//			<< endl;
//	cout
//			<< "666666666666666666666666666666666666666666666666666666666666666666666"
//			<< endl;

//render top level leaves w/o blending to avoid "black" boxes
//just a hack
	glDisable(GL_BLEND);
//note here we render even if b is not a leaf
	Box* b = allLeaf[0];
	switch (b->status) {
	case Box::FREE:
		glColor4f(0.25, 1, 0.25, 0.5);
		break;
	case Box::STUCK:
		glColor4f(1, 0.25, 0.25, 0.5);
		break;
	case Box::MIXED:
		glColor4f(1, 1, 0.25, 0.1);
		if (b->height < epsilon || b->width < epsilon) {
			glColor4f(0.5, 0.5, 0.5, 0.1);
		}
		break;
	case Box::UNKNOWN:
		std::cerr << "UNKNOWN value unexpected!" << std::endl;
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex2d(b->x - b->width / 2, b->y - b->height / 2);
	glVertex2d(b->x + b->width / 2, b->y - b->height / 2);
	glVertex2d(b->x + b->width / 2, b->y + b->height / 2);
	glVertex2d(b->x - b->width / 2, b->y + b->height / 2);
	glEnd();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
//	cout
//			<< "555555555555555555555555555555555555555555555555555555555555555555555"
//			<< endl;
//	cout
//			<< "555555555555555555555555555555555555555555555555555555555555555555555"
//			<< endl;

	if (!allLeaf.empty() && allLeaf.size() != 0) {
		int i = 0;
		for (vector<Box*>::iterator it = allLeaf.begin(); it != allLeaf.end();
				++it) {
			i++;
			if (animationOption == 0 || animationOption == 3) {
				drawQuad(*it);
			}

			if (animationOption == 1 || animationOption == 2) {
				if (currentStep >= i) {
					drawQuad(*it);
					cout << "currentStep:  " << currentStep << endl;
				}

				if (currentStep == i) {
					break;
				}
			}

		}
	}

//	cout
//			<< "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
//			<< endl;
	glPolygonMode(GL_FRONT, GL_LINE);

	drawCircle(R0, 100, alpha[0], alpha[1], 0, 0, 1);	// start
	drawCircle(R0, 100, beta[0], beta[1], 0, 0, 1);	// goal

	double r0 = 5;
	if (r0 > R0)
		r0 = R0;
	filledCircle(r0, alpha[0], alpha[1], 0.2, 0.2, 1.0);	//blue start center
	filledCircle(r0, beta[0], beta[1], 0.8, 0.8, 0.2);	//yellow goal center

//	cout
//			<< "7777777777777777777777777777777777777777777777777777777777777777777777"
//			<< endl;
	drawWalls(QT->pRoot);

	drawLine();

	if (boxA) {
		drawLinks(boxA, alpha[0], alpha[1]);
	}
	if (boxB) {
		drawLinks(boxB, beta[0], beta[1]);
	}

	if (!noPath) {
		Graph graph;
		vector<Box*> path = graph.dijkstraShortestPath(boxA, boxB);
		drawPath(path);
//Graph::bfsPath(boxA, boxB);
	}

//	glFlush();
	glutSwapBuffers();
	if (animationOption == 1 || animationOption == 3) {
		glutPostRedisplay();
	}
//	glutPostRedisplay();
}

//void *thread_render(void* arg) {
//
//	cout << "laksjdlfka;klsdjf" << endl;
//
//	run();
//
//}

/* ********************************************************************** */
// skip blanks, tabs, line breaks and comment lines,
// 	leaving us at the beginning of a token (or EOF)
// 	(This code is taken from core2/src/CoreIo.cpp)
int skip_comment_line(std::ifstream & in) {
	int c;

	do {
		c = in.get();
		while (c == '#') {
			do {	// ignore the rest of this line
				c = in.get();
			} while (c != '\n');
			c = in.get(); // now, reach the beginning of the next line
		}
	} while (c == ' ' || c == '\t' || c == '\n'); //ignore white spaces and newlines

	if (c == EOF)
		std::cout << "unexpected end of file." << std::endl;

	in.putback(c); // this is non-white and non-comment char!
	return c;
}  //skip_comment_line

// skips '\' followed by '\n'
// 	NOTE: this assumes a very special file format (e.g., our BigInt File format)
// 	in which the only legitimate appearance of '\' is when it is followed
// 	by '\n' immediately!  
int skip_backslash_new_line(std::istream & in) {
	int c = in.get();

	while (c == '\\') {
		c = in.get();

		if (c == '\n')
			c = in.get();
		else
// assuming the very special file format noted above!
			cout
					<< "continuation line \\ must be immediately followed by new line.\n";
	} //while
	return c;
} //skip_backslash_new_line

/* ********************************************************************** */

void parseConfigFile(Box* b) {
	std::stringstream ss;
	ss << inputDir << "/" << fileName;	// create full file name
	std::string s = ss.str();

	fileProcessor(s);	// this will clean the input and put in
// output-tmp.txt

	ifstream ifs("output-tmp.txt");
	if (!ifs) {
		cerr << "cannot open input file" << endl;
		exit(1);
	}

	// First, get to the beginning of the first token:
	skip_comment_line(ifs);

	int nPt, nPolygons;	// previously, nPolygons was misnamed as nFeatures
	ifs >> nPt;

//skip_comment_line ( ifs );	// again, clear white space
	vector<double> pts(nPt * 2);
	for (int i = 0; i < nPt; ++i) {
		ifs >> pts[i * 2] >> pts[i * 2 + 1];
	}

	//skip_comment_line ( ifs );	// again, clear white space
	ifs >> nPolygons;
	//skip_comment_line ( ifs );	// again, clear white space
	string temp;
	std::getline(ifs, temp);
	for (int i = 0; i < nPolygons; ++i) {
		string s;
		std::getline(ifs, s);
		stringstream ss(s);
		vector<Corner*> ptVec;
		set<int> ptSet;
		while (ss) {
			int pt;
			/// TODO:
			ss >> pt;
			pt -= 1;	//1 based array
			if (ptSet.find(pt) == ptSet.end()) {
				ptVec.push_back(
						new Corner(pts[pt * 2] * scale + deltaX,
								pts[pt * 2 + 1] * scale + deltaY));
				cout << "point: X=" << pts[pt * 2] * scale + deltaX << "   Y="
						<< pts[pt * 2 + 1] * scale + deltaY;

				b->addCorner(ptVec.back());
				b->vorCorners.push_back(ptVec.back());
				ptSet.insert(pt);
				if (ptVec.size() > 1) {
					Wall* w = new Wall(ptVec[ptVec.size() - 2],
							ptVec[ptVec.size() - 1]);
					b->addWall(w);
					b->vorWalls.push_back(w);
				}
			}
			//new pt already appeared, a loop is formed. should only happen on first and last pt
			else {
				if (ptVec.size() > 1) {
					Wall* w = new Wall(ptVec[ptVec.size() - 1], ptVec[0]);
					b->addWall(w);
					b->vorWalls.push_back(w);
					break;
				}
			}
		}
	}
	ifs.close();
	if (true) {
		cout << "input file name = " << s << endl;
		cout << "nPt=" << nPt << endl;
		cout << "nPolygons=" << nPolygons << endl;
	}

}

//
//
//
//
//
//
//
//
//
//
//
//

