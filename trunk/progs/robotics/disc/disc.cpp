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
			[Qtype = 0]
		
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

	NOTE: there are several examples of running
		this program in the Makefile.
	
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

#ifdef __CYGWIN32__
#include "glui.h"
#endif
#ifdef _WIN32
#include <gl/glui.h>
#endif

//#include <gl/glut.h>
//#include <GL/glui.h>
#include "glui.h"
#include <set>

using namespace std;

QuadTree* QT;

// INPUT Parameters ========================================
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
	int QType = 0;				// The Priority Queue can be
						//    sequential (0) or random (1)
	int interactive = 0;			// Run interactively?
						//    Yes (0) or No (1)
	bool noPath = true;			// Path Found?

// GLUI controls ========================================
//
	GLUI_RadioGroup* radioQType;
	GLUI_EditText* editInput;
	GLUI_EditText* editRadius;
	GLUI_EditText* editEpsilon;
	GLUI_EditText* editAlphaX;
	GLUI_EditText* editAlphaY;
	GLUI_EditText* editBetaX;
	GLUI_EditText* editBetaY;

// External Routines ========================================
//
void renderScene(void);
void parseConfigFile(Box*);
void run();
void genEmptyTree();
void drawPath(vector<Box*>&);


// MAIN PROGRAM: ========================================
int main(int argc, char* argv[])
{
cout << "before args\n";
	if (argc > 1) interactive = atoi(argv[1]);	// Interactive (0) or no (>0)
	if (argc > 2) alpha[0] = atof(argv[2]);		// start x
	if (argc > 3) alpha[1] = atof(argv[3]);		// start y
	if (argc > 4) beta[0] = atof(argv[4]);		// goal x
	if (argc > 5) beta[0] = atof(argv[5]);		// goal y
	if (argc > 6) epsilon = atof(argv[6]);		// epsilon (resolution)
	if (argc > 7) R0      = atof(argv[7]);		// robot radius
	if (argc > 8) fileName = argv[8]; 		// Input file name
	if (argc > 9) boxWidth = atof(argv[9]);		// boxWidth
	if (argc > 10) boxHeight = atof(argv[10]);	// boxHeight
	if (argc > 11) windowPosX = atoi(argv[11]);	// window X pos
	if (argc > 12) windowPosY = atoi(argv[12]);	// window Y pos
	if (argc > 13) QType   = atoi(argv[13]);	// PriorityQ Type (random or no)

cout << "after empty tree\n";
	genEmptyTree();		// Initialize the quadtree


	if (interactive > 0) {	// non-interactive
	    // do something...
	    cout << "Non Interactive Run of Disc Robot" << endl;
	    if (noPath)
	    	cout << "No Path Found!" << endl;
	    else
	    	cout << "Path was Found!" << endl;
	    return 0;
	}

	// Else, set up for GLUT/GLUI interactive display:
	
	glutInit(&argc, argv);
	glutInitWindowPosition(windowPosX, windowPosY);
	glutInitWindowSize(boxWidth, boxWidth);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	int windowID = glutCreateWindow("Motion Planning");
	glutDisplayFunc(renderScene);
	GLUI_Master.set_glutIdleFunc( NULL );
	GLUI *glui = GLUI_Master.create_glui( "control", 0, windowPosX + boxWidth + 20, windowPosY );
	
	editInput = glui->add_edittext( "Input file:", GLUI_EDITTEXT_TEXT );
	editInput->set_text((char*)fileName.c_str());
	editRadius = glui->add_edittext( "Radius:", GLUI_EDITTEXT_FLOAT );
	editRadius->set_float_val(R0);
	editEpsilon = glui->add_edittext( "Epsilon:", GLUI_EDITTEXT_FLOAT );
	editEpsilon->set_float_val(epsilon);
	editAlphaX = glui->add_edittext( "alpha.x:", GLUI_EDITTEXT_FLOAT );
	editAlphaX->set_float_val(alpha[0]);
	editAlphaY = glui->add_edittext( "alpha.y:", GLUI_EDITTEXT_FLOAT );
	editAlphaY->set_float_val(alpha[1]);
	editBetaX = glui->add_edittext( "Beta.x:", GLUI_EDITTEXT_FLOAT );
	editBetaX->set_float_val(beta[0]);
	editBetaY = glui->add_edittext( "Beta.y:", GLUI_EDITTEXT_FLOAT );
	editBetaY->set_float_val(beta[1]);

	glui->add_separator();
	radioQType = glui->add_radiogroup();
	glui->add_radiobutton_to_group(radioQType, "Sequential");
	glui->add_radiobutton_to_group(radioQType, "Random");
	glui->add_separator();

	// unused warning: comment out (Chee)
	GLUI_Button* buttonRun = glui->add_button( "Run", -1, (GLUI_Update_CB)run);

	glui->set_main_gfx_window( windowID );

	// run(); // make it do something interesting from the start!
cout << "before run\n";
	run(); // make it do something interesting from the start!!!
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
	QT = new QuadTree(root, epsilon, QType);
}

void run()
{
	//update from glui live variables
	fileName = editInput->get_text();
	R0 = editRadius->get_float_val();
	epsilon = editEpsilon->get_float_val();
	alpha[0] = editAlphaX->get_float_val();
	alpha[1] = editAlphaY->get_float_val();
	beta[0] = editBetaX->get_float_val();
	beta[1] = editBetaY->get_float_val();
    QType = radioQType->get_int_val();

	genEmptyTree();

	noPath = false;

	boxA = QT->getBox(alpha[0], alpha[1]);
	while (boxA && !boxA->isFree())
	{
		if (!QT->expand(boxA))
		{
			noPath = true;
			break;
		}
		boxA = QT->getBox(boxA, alpha[0], alpha[1]);
	}

	boxB = QT->getBox(beta[0], beta[1]);
	while (!noPath && boxB && !boxB->isFree())
	{
		if (!QT->expand(boxB))
		{
			noPath = true;
			break;
		}
		boxB = QT->getBox(boxB, beta[0], beta[1]);
	}

	int ct = 0;
	while(!noPath && !QT->isConnect(boxA, boxB))
	{
		++ct;
		if (!QT->expand())
		{
			noPath = true;
		}
	}

	glutPostRedisplay();

	cout << "path found is " << !noPath << endl;
	cout << "expended " << ct << " times" << endl;
}

void drawPath(vector<Box*>& path)
{
	glColor3f(1, 0, 0);
	glBegin(GL_LINE_STRIP);	
	glVertex2f(beta[0], beta[1]);
	for (int i = 0; i < (int)path.size(); ++i)
	{
		glVertex2f(path[i]->x, path[i]->y);
	}
	glVertex2f(alpha[0], alpha[1]);
	glEnd();
}

void drawQuad(Box* b)
{
	switch(b->status)
	{
	case Box::FREE:
		glColor3f(0, 1, 0);
		break;
	case Box::STUCK:
		glColor3f(1, 0, 0);
		break;
	case Box::MIXED:
		glColor3f(1, 1, 0);
		if (b->height < epsilon || b->width < epsilon)
		{
			glColor3f(0.5, 0.5, 0.5);
		}
		break;
	case Box::UNKNOWN:
		std::cout << "UNKNOWN in drawQuad" << std::endl;
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex2f(b->x - b->width / 2, b->y - b->height / 2);
	glVertex2f(b->x + b->width / 2, b->y - b->height / 2);
	glVertex2f(b->x + b->width / 2, b->y + b->height / 2);
	glVertex2f(b->x - b->width / 2, b->y + b->height / 2);
	glEnd();

	glColor3f(0, 0 , 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POLYGON);
	glVertex2f(b->x - b->width / 2, b->y - b->height / 2);
	glVertex2f(b->x + b->width / 2, b->y - b->height / 2);
	glVertex2f(b->x + b->width / 2, b->y + b->height / 2);
	glVertex2f(b->x - b->width / 2, b->y + b->height / 2);
	glEnd();
}

void drawWalls(Box* b)
{
	glColor3f(1, 1, 1);
	for (list<Wall*>::iterator iter = b->walls.begin(); iter != b->walls.end(); ++iter)
	{
		Wall* w = *iter;
		glBegin(GL_LINES);
		glVertex2f(w->src->x, w->src->y);
		glVertex2f(w->dst->x, w->dst->y);
		glEnd();
	}
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

void drawCircle( float Radius, int numPoints, double x, double y)
{
	glBegin( GL_LINE_STRIP );
	for( int i = 0; i <= numPoints; ++i )
	{
		float Angle = i * (2.0* 3.1415926 / numPoints);  
		float X = cos( Angle )*Radius;  
		float Y = sin( Angle )*Radius;
		glVertex2f( X + x, Y + y);
	}
	glEnd();
}

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glScalef(2.0/boxWidth, 2.0/boxHeight, 0);
	glTranslatef(-boxWidth/2, -boxHeight/2, 0);

	treeTraverse(QT->pRoot);

	glPolygonMode(GL_FRONT, GL_LINE);

	glColor3f(0, 0, 1);
	//glPointSize(R0*2);
	//glEnable( GL_POINT_SMOOTH );
	//glBegin(GL_POINTS);
	//glVertex2f(alpha[0], alpha[1]);
	//glVertex2f(beta[0], beta[1]);
	//glEnd();
	drawCircle(R0, 100, alpha[0], alpha[1]);
	drawCircle(R0, 100, beta[0], beta[1]);

	drawWalls(QT->pRoot);

	if (!noPath)
	{
		vector<Box*> path = Graph::findPath(boxA, boxB);
		drawPath(path);
	}

	glutSwapBuffers();
}

void parseConfigFile(Box* b)
{	
	ifstream ifs(fileName.c_str());
	if (!ifs)
	{
		cerr<< "cannot open input file" << endl;
		exit(1);
	}

	int nPt, nFeature;
	ifs >> nPt;
	vector<double> pts(nPt*2);
	for (int i = 0; i < nPt; ++i)
	{
		ifs >> pts[i*2] >> pts[i*2+1];
	}
	ifs >> nFeature;
	string temp;
	std::getline(ifs, temp);
	for (int i = 0; i < nFeature; ++i)
	{
		string s;
		std::getline(ifs, s);
		stringstream ss(s);
		vector<Corner*> ptVec;
		set<int> ptSet;
		while (ss)
		{
			int pt;
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
