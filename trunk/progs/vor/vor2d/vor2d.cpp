/* **************************************
   File: disc.cpp

   Description: 
	This is the entry point for the running the SSS algorithm
	for a disc robot amidst a collection of polygonal obstacles.

	To run, call with these positional arguments:

	> ./disc 	[interactive = 0] \
			[alpha-x = 10] [alpha-y = 360] \
			[beta-x = 500] [beta-y = 20] \
			[epsilon = 1] \
			[R0 = 30] \
			[fileName = input2.txt] \
			[boxWidth = 512] [boxHeight = 512] \
			[windoxPosX = 400] [windowPosY = 200] \
			[Qtype = 0] [seed = 111] [inputDir = inputs]
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

	NOTE: see several examples of running this program in the Makefile.
	
	Format of input environment: see README FILE

   HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang

   Since Core Library  Version 2.1
   $Id: $
 ************************************** */

#include "QuadTree.h"
#include "PriorityQueue.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Graph.h"
#include "Timer.h"

#ifdef __CYGWIN32__
#include "glui.h"
#endif
#ifdef _WIN32
#include <gl/glui.h>
#endif
#ifdef __APPLE__
#include "glui.h"
#endif

#include <set>
//#include "CoreIo.h"

using namespace std;

QuadTree* QT;

// GLOBAL INPUT Parameters ========================================
//
	double alpha[2] = {10, 360};		// start configuration
	double beta[2] = {500, 20};		// goal configuration
	double epsilon = 1;			// resolution parameter
	Box* boxA;				// start box (containing alpha)
	Box* boxB;				// goal box (containing beta)
	double boxWidth = 512;			// Initial box width
	double boxHeight = 512;			// Initial box height
	double R0 = 30;				// Robot radius 
	int windowPosX = 400;			// X Position of Window
	int windowPosY = 200;			// Y Position of Window
	string fileName("input2.txt"); 		// Input file name
	string inputDir("inputs"); 		// Path for input files 
	int QType = 0;				// The Priority Queue can be
						//    sequential (0) or random (1)
	int interactive = 0;			// Run interactively?
						//    Yes (0) or No (1)
	int seed = 111;				// seed for random number generator
						// (Could also be used for BFS, etc)
	bool noPath = true;			// True means there is "No path.
 
	bool hideBoxBoundary = false;  //don't draw box boundary

	int freeCount = 0;
	int stuckCount = 0;
	int mixCount = 0;
	int mixSmallCount = 0;

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
	GLUI_EditText* editBetaX;
	GLUI_EditText* editBetaY;
	GLUI_EditText* editSeed;

// External Routines ========================================
//
void renderScene(void);
void parseConfigFile(Box*);
void run();
void genEmptyTree();
void drawPath(vector<Box*>&);
extern int fileProcessor(string inputfile);
void Keyboard( unsigned char key, int x, int y );


// MAIN PROGRAM: ========================================
int main(int argc, char* argv[])
{
	if (argc > 1) interactive = atoi(argv[1]);	// Interactive (0) or no (>0)
	if (argc > 2) alpha[0] = atof(argv[2]);		// start x
	if (argc > 3) alpha[1] = atof(argv[3]);		// start y
	if (argc > 4) beta[0] = atof(argv[4]);		// goal x
	if (argc > 5) beta[1] = atof(argv[5]);		// goal y
	if (argc > 6) epsilon = atof(argv[6]);		// epsilon (resolution)
	if (argc > 7) R0      = atof(argv[7]);		// robot radius
	if (argc > 8) fileName = argv[8]; 		// Input file name
	if (argc > 9) boxWidth = atof(argv[9]);		// boxWidth
	if (argc > 10) boxHeight = atof(argv[10]);	// boxHeight
	if (argc > 11) windowPosX = atoi(argv[11]);	// window X pos
	if (argc > 12) windowPosY = atoi(argv[12]);	// window Y pos
	if (argc > 13) QType   = atoi(argv[13]);	// PriorityQ Type (random or no)
	if (argc > 14) seed   = atoi(argv[14]);		// for random number generator
	if (argc > 15) inputDir  = argv[15];		// path for input files

	// Else, set up for GLUT/GLUI interactive display:
	glutInit(&argc, argv);
	glutInitWindowPosition(windowPosX, windowPosY);
	glutInitWindowSize(boxWidth, boxWidth);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);


	int windowID = glutCreateWindow("Voronoi 2D");
	glutDisplayFunc(renderScene);
	GLUI_Master.set_glutIdleFunc( NULL );
	GLUI_Master.set_glutKeyboardFunc(Keyboard);
	GLUI *glui = GLUI_Master.create_glui( "control", 0, windowPosX + boxWidth + 20, windowPosY );
	
	// SETTING UP THE CONTROL PANEL:
	editInput = glui->add_edittext( "Input file:", GLUI_EDITTEXT_TEXT );
	editInput->set_text((char*)fileName.c_str());
	editDir = glui->add_edittext( "Input Directory:", GLUI_EDITTEXT_TEXT );
	editDir->set_text((char*)inputDir.c_str());
	editEpsilon = glui->add_edittext( "Epsilon:", GLUI_EDITTEXT_FLOAT );
	editEpsilon->set_float_val(epsilon);

	GLUI_Button* buttonRun = glui->add_button( "Run", -1, (GLUI_Update_CB)run);
	buttonRun->set_name("Run me"); // Hack, but to avoid "unused warning" (Chee)


	// New column:
	glui->add_column(true);

	glui->add_separator();
	radioDrawOption = glui->add_radiogroup(0, -1, (GLUI_Update_CB)renderScene);
	glui->add_radiobutton_to_group(radioDrawOption, "Show Box Boundary");
	glui->add_radiobutton_to_group(radioDrawOption, "Hide Box Boundary");
	glui->add_separator();

	// Quit button
	glui->add_button( "Quit", 0, (GLUI_Update_CB)exit );

	glui->set_main_gfx_window( windowID );

	// PERFORM THE INITIAL RUN OF THE ALGORITHM
	//==========================================
	run(); 	// make it do something interesting from the start!!!

	// SHOULD WE STOP or GO INTERACTIVE?
	//==========================================
	if (interactive > 0) {	// non-interactive
	    // do something...
	    cout << "Non Interactive Run of Disc Robot" << endl;
	    if (noPath)
	    	cout << "No Path Found!" << endl;
	    else
	    	cout << "Path was Found!" << endl;
	    return 0;
	}
	else
		glutMainLoop();

	return 0;
}

void genEmptyTree()
{
	Box* root = new Box(boxWidth/2, boxHeight/2, boxWidth, boxHeight);
	Box::r0 = R0;

	parseConfigFile(root);
	root->updateStatus();

	if (QT)
	{
		delete(QT);
	}
	QT = new QuadTree(root, epsilon, QType, seed++);  // Note that seed keeps changing!

cout<<"inside genEmpty:  Qtype= " << QType << "\n";
}

void run()
{
	//update from glui live variables
	fileName = editInput->get_text();
	inputDir = editDir->get_text();
	epsilon = editEpsilon->get_float_val();

	Timer t;

	// start timer
	t.start();

	genEmptyTree();

	noPath = false;	// Confusing use of "noPath"
	int ct = 0;

    while(QT->PQ->empty()==false)
    {
        QT->expand();
        ct++;
    }

	// stop timer
	t.stop();
	// print the elapsed time in millisec
	cout << "Time used: " << t.getElapsedTimeInMilliSec() << " ms.\n";

	glutPostRedisplay();


	cout << "Expanded " << ct << " times" << endl;
	cout << "total Free boxes: " << freeCount << endl;
	cout << "total Stuck boxes: " << stuckCount << endl;
	cout << "total Mixed boxes smaller than epsilon: " << mixSmallCount << endl;
	cout << "total Mixed boxes bigger than epsilon: " << mixCount - ct - mixSmallCount << endl;
	freeCount = stuckCount = mixCount = mixSmallCount = 0;
}

void drawPath(vector<Box*>& path)
{
	glColor3f(0.5, 0, 0.25);
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);	
	glVertex2f(beta[0], beta[1]);
	for (int i = 0; i < (int)path.size(); ++i)
	{
		glVertex2f(path[i]->x, path[i]->y);	
	}
	glVertex2f(alpha[0], alpha[1]);
	glEnd();
	glLineWidth(1.0);
}

void drawQuad(Box* b)
{
	switch(b->status)
	{
	case Box::OFF:
		glColor3f(0.25, 1, 0.25);
		break;
	case Box::ON:
	    glColor3f(0.85, 0.85, 0.85);
		break;
	case Box::IN:
		glColor3f(1, 1, 0.25);
		if (b->height < epsilon || b->width < epsilon)
		{
			glColor3f(0.5, 0.5, 0.5);
		}
		break;
    case Box::TRICKY:
        glColor3f(1, 0.25, 0.25);
        break;
	case Box::UNKNOWN:
		std::cout << "UNKNOWN in drawQuad" << std::endl;
		break;
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex2f(b->x - b->width / 2, b->y - b->height / 2);
	glVertex2f(b->x + b->width / 2, b->y - b->height / 2);
	glVertex2f(b->x + b->width / 2, b->y + b->height / 2);
	glVertex2f(b->x - b->width / 2, b->y + b->height / 2);
	glEnd();

	if (!hideBoxBoundary)
	{
		glColor3f(0, 0 , 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_POLYGON);
		glVertex2f(b->x - b->width / 2, b->y - b->height / 2);
		glVertex2f(b->x + b->width / 2, b->y - b->height / 2);
		glVertex2f(b->x + b->width / 2, b->y + b->height / 2);
		glVertex2f(b->x - b->width / 2, b->y + b->height / 2);
		glEnd();
	}	
}

void drawWalls(Box* b)
{
	glColor3f(1, 1, 1);
	glLineWidth(2.0);
	for (list<Wall*>::iterator iter = b->walls.begin(); iter != b->walls.end(); ++iter)
	{
		Wall* w = *iter;
		glBegin(GL_LINES);
		glVertex2f(w->src->x, w->src->y);
		glVertex2f(w->dst->x, w->dst->y);
		glEnd();
	}
	glLineWidth(1.0);
}

void treeTraverse(Box* b)
{	
	if (!b)
	{
		return;
	}
	if (b->isLeaf)
	{
		drawQuad(b);
		return;
	}
	for (int i = 0; i < 4; ++i)
	{
		treeTraverse(b->pChildren[i]);
	}
}

void drawCircle( float Radius, int numPoints, double x, double y, double r, double g, double b)
{	
	glColor3d(r,g,b);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	
	glBegin(GL_POLYGON);
	for( int i = 0; i <= numPoints; ++i )
	{
		float Angle = i * (2.0* 3.1415926 / numPoints);  
		float X = cos( Angle )*Radius;  
		float Y = sin( Angle )*Radius;
		glVertex2f( X + x, Y + y);
	}
	glEnd();
}

void filledCircle( double radius, double x, double y, double r, double g, double b) 
{
	int numPoints = 100;
	glColor3d(r,g,b);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	for( int i = 0; i <= numPoints; ++i )
	{
		float Angle = i * (2.0* 3.1415926 / numPoints);  
		float X = cos( Angle )*radius;  
		float Y = sin( Angle )*radius;
		glVertex2f( X + x, Y + y);
	}
	glEnd();
}

//void drawLine()
//{
//	if (noPath)
//	{
//		glColor3f(0, 0, 0);
//	}
//	else
//	{
//		glColor3f(1, 0, 0);
//	}
//	glLineWidth(3.0);
//	glBegin(GL_LINES);
//	glVertex2f(alpha[0], alpha[1]);
//	glVertex2f(beta[0], beta[1]);
//	glEnd();
//	glLineWidth(1.0);
//}


void renderScene(void) 
{
	hideBoxBoundary = radioDrawOption->get_int_val();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glScalef(2.0/boxWidth, 2.0/boxHeight, 0);
	glTranslatef(-boxWidth/2, -boxHeight/2, 0);

	//draw quad tree
	treeTraverse(QT->pRoot);

	//draw obstacles
	glPolygonMode(GL_FRONT, GL_LINE);
	drawWalls(QT->pRoot);

	glutSwapBuffers();
}

/* ********************************************************************** */
// skip blanks, tabs, line breaks and comment lines,
// 	leaving us at the beginning of a token (or EOF)
// 	(This code is taken from core2/src/CoreIo.cpp)
int skip_comment_line (std::ifstream & in) {
	  int c;
	
	  do {
	    c = in.get();
	    while ( c == '#' ) {
	      do {// ignore the rest of this line
	        c = in.get();
	      } while ( c != '\n' );
	      c = in.get(); // now, reach the beginning of the next line
	    }
	  } while (c == ' ' || c == '\t' || c == '\n');	//ignore white spaces and newlines
	
	  if (c == EOF)
	    std::cout << "unexpected end of file." << std::endl;
	
	  in.putback(c);  // this is non-white and non-comment char!
	  return c;
}//skip_comment_line

// skips '\' followed by '\n'
// 	NOTE: this assumes a very special file format (e.g., our BigInt File format)
// 	in which the only legitimate appearance of '\' is when it is followed
// 	by '\n' immediately!  
int skip_backslash_new_line (std::istream & in) {
	  int c = in.get();
	
	  while (c == '\\') {
	    c = in.get();
	
	    if (c == '\n')
	      c = in.get();
	    else // assuming the very special file format noted above!
	      cout<< "continuation line \\ must be immediately followed by new line.\n";
	  }//while
	  return c;
}//skip_backslash_new_line

/* ********************************************************************** */

void parseConfigFile(Box* b)
{	
	std::stringstream ss;
	ss << inputDir << "/" << fileName;	// create full file name 
	std::string s = ss.str();
cout << "input file name = " << s << endl;	

	fileProcessor(s);	// this will clean the input and put in
				// output-tmp.txt
	
	ifstream ifs( "output-tmp.txt" );
	if (!ifs)
	{
		cerr<< "cannot open input file" << endl;
		exit(1);
	}

	// First, get to the beginning of the first token:
	skip_comment_line ( ifs );

	int nPt, nPolygons;	// previously, nPolygons was misnamed as nFeatures
	ifs >> nPt;
cout<< "nPt=" << nPt << endl;

	//skip_comment_line ( ifs );	// again, clear white space
	vector<double> pts(nPt*2);
	for (int i = 0; i < nPt; ++i)
	{
		ifs >> pts[i*2] >> pts[i*2+1];
	}

	//skip_comment_line ( ifs );	// again, clear white space
	ifs >> nPolygons;
	//skip_comment_line ( ifs );	// again, clear white space
cout<< "nPolygons=" << nPolygons << endl;
	string temp;
	std::getline(ifs, temp);
	for (int i = 0; i < nPolygons; ++i)
	{
		string s;
		std::getline(ifs, s);
		stringstream ss(s);
		vector<Corner*> ptVec;
		set<int> ptSet;
		while (ss)
		{
			int pt;
			/// TODO:
			ss >> pt;
			pt -= 1; //1 based array
			if (ptSet.find(pt) == ptSet.end())
			{
				ptVec.push_back(new Corner(pts[pt*2], pts[pt*2+1]));
				b->addCorner(ptVec.back());
				ptSet.insert(pt);
				if (ptVec.size() > 1)
				{
					Wall* w = new Wall(ptVec[ptVec.size()-2], ptVec[ptVec.size()-1]);
					b->addWall(w);
				}				
			}
			//new pt already appeared, a loop is formed. should only happen on first and last pt
			else
			{
				if (ptVec.size() > 1)
				{
					Wall* w = new Wall(ptVec[ptVec.size()-1], ptVec[0]);
					b->addWall(w);
					break;
				}	
			}
		}
	}
	ifs.close();

}

void Keyboard( unsigned char key, int x, int y )
{
    // find closest colorPt3D if ctrl is pressed...
    switch( key ){
        case 27: exit(0);
    }
}
