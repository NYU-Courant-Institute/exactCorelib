// robotics2D.cpp : Defines the entry point for the console application.
//
#include "QuadTree.h"
#include "PriorityQueue.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <gl/glut.h>
#include <set>

using namespace std;

QuadTree* QT;
double alpha[2] = {50, 312};
double beta[2] = {256, 62};
double epsilon = 10;
Box* boxA;
Box* boxB;
bool noPath = false;
double boxWidth = 512;
double boxHeight = 512;
double R0 = 50;

void renderScene(void);
void parseConfigFile(Box*);

int main(int argc, char* argv[])
{
	Box* root = new Box(boxWidth/2, boxHeight/2, boxWidth, boxHeight);
	Box::r0 = R0;

	parseConfigFile(root);

	root->updateStatus();

	QT = new QuadTree(root, epsilon);
	//root->split(0);
	//root->pChildren[1]->split(0);
	//root->pChildren[1]->pChildren[3]->split(0);
	//root->pChildren[1]->pChildren[3]->pChildren[0]->split(0);
	//root->pChildren[0]->split(0);

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
	while (boxB && !boxB->isFree())
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

	cout << "path found is " << !noPath << endl;
	cout << "expended " << ct << " times" << endl;

	glutInit(&argc, argv);
	glutInitWindowPosition(400, 200);
	glutInitWindowSize(boxWidth, boxWidth);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Motion Planning");
	glutDisplayFunc(renderScene);
	glutMainLoop();

	return 0;
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
	for (int i = 0; i < 4; ++i)
	{
		if (b->pChildren[i]->isLeaf)
		{
			drawQuad(b->pChildren[i]);
			//cout << b->pChildren[i]->x << b->pChildren[i]->y << endl;
		}
		else
		{
			treeTraverse(b->pChildren[i]);
		}
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

	glutSwapBuffers();
}

void parseConfigFile(Box* b)
{
	ifstream ifs("input2.txt");
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

}