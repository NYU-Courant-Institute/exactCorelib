/**************************************************
 *	Introduction: 
 *		Command:
 *			//
 *		Keyboard: 
 *			s: Zoom out
 *			w: Zoom in
 *          p: Show/Hide path
 *          b: Show/Hide boxes
 *          t: Toggle types of boxes shown
 *	        r: Findpath();
 *		Mouse:
 *			Change the angle of view
 ***************************************************/
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include <regex>

#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4
#define ESC 27
#define BACKSPACE 8
#define ENTER 13

#define WINDOWS_HEIGHT 600
#define WINDOWS_WIDTH 1000

float c=3.1415926/180.0f; // transform from angle to radian
int du=90, oldmy=-1,oldmx=-1; // 
float r=160.0f,h=0.0f; // radius of turn 

GLfloat Vx = 0.0, Vy = 1.0, Vz = 0.0; //View-up vector.

/***************************************************************************/
// for the display lists
GLuint glistID;  

//glui controls
std::string inputString;

bool isShowBox = 1; bool isShowPath = 1;
bool isInputMode = false;
int nextBoxType = 0;

double alpha[2] = {10, 360};
double beta[2] = {500, 20};
double epsilon = 10;
float maxRadius = 50;

int windowID;

// Prelimenary types

int obstacleCounter;

struct Coord {
	float x, y, z;
};

Coord coord[1000];

struct Face {
	float a, b, c, d;
};

Face faces[1000];

int boxCounter;

struct Box {
	bool isActive;
	float x, y, z, radius;
	int type; //0 = mix, 1 = stuck, -1 = free
	int parent, previous;
	bool isVisited;
};

Box box[1000];

int radius, xStart, yStart, zStart, xEnd, yEnd, zEnd;

int sourceBox, sinkBox;

bool found = false; bool isRand = false; bool isDijk = false;

void drawScene() 
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glMatrixMode(GL_MODELVIEW);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   // Draw obstacles
	for (int i = 0; i < obstacleCounter; i++) {
		glBegin(GL_TRIANGLES);
	    {
			glColor4f(1.0, 1.0, 1.0, 1.0);
			glVertex3f(coord[i * 3 + 1].x, coord[i * 3 + 1].y, coord[i * 3 + 1].z);
		    glVertex3f(coord[i * 3 + 2].x, coord[i * 3 + 2].y, coord[i * 3 + 2].z);
		    glVertex3f(coord[i * 3 + 3].x, coord[i * 3 + 3].y, coord[i * 3 + 3].z);
		}
		glEnd();

		glBegin(GL_LINES);
		{
			glColor4f(0.0, 0.0, 0.0, 1.0);
			glVertex3f(coord[i * 3 + 1].x, coord[i * 3 + 1].y, coord[i * 3 + 1].z);
		    glVertex3f(coord[i * 3 + 2].x, coord[i * 3 + 2].y, coord[i * 3 + 2].z);
		    glVertex3f(coord[i * 3 + 3].x, coord[i * 3 + 3].y, coord[i * 3 + 3].z);
			glVertex3f(coord[i * 3 + 1].x, coord[i * 3 + 1].y, coord[i * 3 + 1].z);
		    glVertex3f(coord[i * 3 + 2].x, coord[i * 3 + 2].y, coord[i * 3 + 2].z);
		    glVertex3f(coord[i * 3 + 3].x, coord[i * 3 + 3].y, coord[i * 3 + 3].z);
		}
		glEnd();
	}

	// Draw sphere
	glColor4f(0.5, 0.45, 0.8, 1.0);
	GLUquadric *quad = gluNewQuadric();
	glPushMatrix();
	glTranslatef(xStart, yStart, zStart);
	gluSphere(quad, radius, 20, 20);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(xEnd, yEnd, zEnd);
	gluSphere(quad, radius, 20, 20);
	glPopMatrix();

	// Draw route
	if ((isShowPath) && (sinkBox != 0)) {
		glBegin(GL_LINES);
		{
			glVertex3f(box[sourceBox].x, box[sourceBox].y, box[sourceBox].z);
			glVertex3f(xStart, yStart, zStart);
			glVertex3f(box[sinkBox].x, box[sinkBox].y, box[sinkBox].z);
			glVertex3f(xEnd, yEnd, zEnd);
		}
		glEnd();
		int i = sinkBox;
		while (i != sourceBox) {
			glBegin(GL_LINES);
			{
				glVertex3f(box[i].x, box[i].y, box[i].z);
				glVertex3f(box[box[i].previous].x, box[box[i].previous].y, box[box[i].previous].z);
			}
			glEnd();
			i = box[i].previous;
		}
	}

	// Draw boxes
	glDisable(GL_DEPTH_TEST);
	if (isShowBox == 1) for (int i = 0; i < boxCounter; i++)
		if (box[i].isActive)
	{
		box[i].radius -= 0.5;
		glBegin(GL_QUADS);
		{
			switch (box[i].type) {
			case -1: glColor4f(0.1, 0.9, 0.1, 0.1); if ((nextBoxType != 0) && (nextBoxType != 1)) glColor4f(0.0, 0.0, 0.0, 0.0); break;
			case 0: glColor4f(0.8, 0.7, 0.0, 0.1); if ((nextBoxType != 0) && (nextBoxType != 2)) glColor4f(0.0, 0.0, 0.0, 0.0); break;
			case 1: glColor4f(0.8, 0.2, 0.1, 0.1); if ((nextBoxType != 0) && (nextBoxType != 3)) glColor4f(0.0, 0.0, 0.0, 0.0); break;
			}
			if ((nextBoxType == 4) && (box[i].isVisited)) glColor4f(0.0, 0.0, 1.0, 0.2);
			if ((nextBoxType == 4) && (box[i].isVisited)) glColor4f(0.0, 0.0, 1.0, 0.2);
			// face 1
			glVertex3f(box[i].x + box[i].radius, box[i].y + box[i].radius, box[i].z + box[i].radius);
			glVertex3f(box[i].x + box[i].radius, box[i].y - box[i].radius, box[i].z + box[i].radius);
			glVertex3f(box[i].x + box[i].radius, box[i].y - box[i].radius, box[i].z - box[i].radius);
			glVertex3f(box[i].x + box[i].radius, box[i].y + box[i].radius, box[i].z - box[i].radius);
			// face 2
			glVertex3f(box[i].x - box[i].radius, box[i].y + box[i].radius, box[i].z + box[i].radius);
			glVertex3f(box[i].x - box[i].radius, box[i].y - box[i].radius, box[i].z + box[i].radius);
			glVertex3f(box[i].x - box[i].radius, box[i].y - box[i].radius, box[i].z - box[i].radius);
			glVertex3f(box[i].x - box[i].radius, box[i].y + box[i].radius, box[i].z - box[i].radius);
			// face 3
			glVertex3f(box[i].x + box[i].radius, box[i].y + box[i].radius, box[i].z + box[i].radius);
			glVertex3f(box[i].x - box[i].radius, box[i].y + box[i].radius, box[i].z + box[i].radius);
			glVertex3f(box[i].x - box[i].radius, box[i].y + box[i].radius, box[i].z - box[i].radius);
			glVertex3f(box[i].x + box[i].radius, box[i].y + box[i].radius, box[i].z - box[i].radius);
			// face 4
			glVertex3f(box[i].x + box[i].radius, box[i].y - box[i].radius, box[i].z + box[i].radius);
			glVertex3f(box[i].x - box[i].radius, box[i].y - box[i].radius, box[i].z + box[i].radius);
			glVertex3f(box[i].x - box[i].radius, box[i].y - box[i].radius, box[i].z - box[i].radius);
			glVertex3f(box[i].x + box[i].radius, box[i].y - box[i].radius, box[i].z - box[i].radius);
			// face 5
			glVertex3f(box[i].x + box[i].radius, box[i].y + box[i].radius, box[i].z + box[i].radius);
			glVertex3f(box[i].x - box[i].radius, box[i].y + box[i].radius, box[i].z + box[i].radius);
			glVertex3f(box[i].x - box[i].radius, box[i].y - box[i].radius, box[i].z + box[i].radius);
			glVertex3f(box[i].x + box[i].radius, box[i].y - box[i].radius, box[i].z + box[i].radius);
			// face 6
			glVertex3f(box[i].x + box[i].radius, box[i].y + box[i].radius, box[i].z - box[i].radius);
			glVertex3f(box[i].x - box[i].radius, box[i].y + box[i].radius, box[i].z - box[i].radius);
			glVertex3f(box[i].x - box[i].radius, box[i].y - box[i].radius, box[i].z - box[i].radius);
			glVertex3f(box[i].x + box[i].radius, box[i].y - box[i].radius, box[i].z - box[i].radius);
		}
		glEnd();

		/*glBegin(GL_LINES);
		{
			glColor4f(0.1, 0.1, 0.1, 0.8);
			glVertex3f(box[i].x + box[i].radius, box[i].y + box[i].radius, box[i].z - box[i].radius);
			glVertex3f(box[i].x - box[i].radius, box[i].y + box[i].radius, box[i].z - box[i].radius);
			glVertex3f(box[i].x + box[i].radius, box[i].y + box[i].radius, box[i].z + box[i].radius);
			glVertex3f(box[i].x - box[i].radius, box[i].y + box[i].radius, box[i].z + box[i].radius);
			glVertex3f(box[i].x + box[i].radius, box[i].y - box[i].radius, box[i].z - box[i].radius);
			glVertex3f(box[i].x - box[i].radius, box[i].y - box[i].radius, box[i].z - box[i].radius);
			glVertex3f(box[i].x + box[i].radius, box[i].y - box[i].radius, box[i].z + box[i].radius);
			glVertex3f(box[i].x - box[i].radius, box[i].y - box[i].radius, box[i].z + box[i].radius);
		}
		glEnd();*/
		box[i].radius += 0.5;
	}

	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,640,0,480);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();					// Store The Modelview Matrix
    glLoadIdentity();					// Reset The Modelview Matrix
    
    /* Information Display Text List */
    glColor3f(1.0f,1.0f,1.0f);

	glRasterPos2f(0, 0);
	
	if (isInputMode) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ':');

	for (int i = 0; i < inputString.length(); i++) 
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, inputString[i]);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	

}

void display(void) 
{
    static unsigned long cnt = 0;
    //    printf("display(%lu)\n",cnt++);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    
    GLfloat vwx = r*cos(c*h)*cos(c*du), vwy = r*sin(c*h), vwz =  r*cos(c*h)*sin(c*du);
    if ( (int)abs(h)%360 >= 90 && (int)abs(h)%360 <= 270)
	Vy = -1.0;
    else
	Vy = 1.0;
    gluLookAt(vwx, vwy, vwz, 0.0 , 0.0, 0.0 , Vx, Vy, Vz); 
    
    glPolygonMode (GL_FRONT, GL_FILL);
    
    drawScene();
    
	glutPostRedisplay();
    glFlush();
    glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) //
{
    if(state == GLUT_DOWN) 
	oldmx=x,oldmy=y;
    if(state == GLUT_UP && button == GLUT_WHEEL_UP)
    {
	r +=0.2;
	glutPostRedisplay();
    }
    if(state == GLUT_UP && button == GLUT_WHEEL_UP)
    {
	if( r > 5 ){
	    r -=0.2;
	    glutPostRedisplay();
	}
    }
}

void onMouseMove(int x,int y) // process the movement 
{
    du += 0.5*(x-oldmx); 
    h += 0.5f*(y-oldmy); 
    oldmx=x;
    oldmy=y; 
}

bool isEqual(float i, float j) {
	if (abs(i - j) < epsilon / 10) return true;
	return false;
}

bool isInBox(int i, float x, float y, float z) {
	if ((box[i].x + box[i].radius >= x) && (box[i].x - box[i].radius <= x)
		&& (box[i].y + box[i].radius >= y) && (box[i].y - box[i].radius <= y)
		&& (box[i].z + box[i].radius >= z) && (box[i].z - box[i].radius <= z))
		return true;
	return false;
}

int getSourceBox() {
	for (int i = 0; i < boxCounter; i++)
		if ((box[i].isActive) && (isInBox(i, xStart, yStart, zStart))) {
			return i;
		}
	return 0;
}

float sqr(float i) {
	return i * i;
}

float getDist(float px, float py, float pz, int face) {
	float baseDist = abs(px * faces[face].a + py * faces[face].b + pz * faces[face].c + faces[face].d) / 
		sqrt(sqr(faces[face].a) + sqr(faces[face].b) + sqr(faces[face].c));
	return baseDist;
}

int getBoxType(int i) {
	
	bool collided = false;

	// Detect wall collision
	
	float leftSideAD, t1, t2, t3, t4, s1, s2, s3;

	for (int j = 0; j < obstacleCounter; j++) {
		// Face 1
		leftSideAD = faces[j].a * (box[i].x + box[i].radius + radius) + faces[j].d;
		t1 = leftSideAD + faces[j].b * (box[i].y + box[i].radius) + faces[j].c * (box[i].z + box[i].radius);
		t2 = leftSideAD + faces[j].b * (box[i].y + box[i].radius) + faces[j].c * (box[i].z - box[i].radius);
		t3 = leftSideAD + faces[j].b * (box[i].y - box[i].radius) + faces[j].c * (box[i].z - box[i].radius);
		t4 = leftSideAD + faces[j].b * (box[i].y - box[i].radius) + faces[j].c * (box[i].z + box[i].radius);
		s1 = box[i].x + box[i].radius + radius - coord[j * 3 + 1].x;
		s2 = box[i].x + box[i].radius + radius - coord[j * 3 + 2].x;
		s3 = box[i].x + box[i].radius + radius - coord[j * 3 + 3].x;
		if (!(((t1 > 0) && (t2 > 0) && (t3 > 0) && (t4 > 0)) || 
			((t1 < 0) && (t2 < 0) && (t3 < 0) && (t4 < 0)) ||
			((s1 < 0) && (s2 < 0) && (s3 < 0)) ||
			((s1 > 0) && (s2 > 0) && (s3 > 0)))) collided = true;
		// Face 2
		leftSideAD = faces[j].a * (box[i].x - box[i].radius - radius) + faces[j].d;
		t1 = leftSideAD + faces[j].b * (box[i].y + box[i].radius) + faces[j].c * (box[i].z + box[i].radius);
		t2 = leftSideAD + faces[j].b * (box[i].y + box[i].radius) + faces[j].c * (box[i].z - box[i].radius);
		t3 = leftSideAD + faces[j].b * (box[i].y - box[i].radius) + faces[j].c * (box[i].z - box[i].radius);
		t4 = leftSideAD + faces[j].b * (box[i].y - box[i].radius) + faces[j].c * (box[i].z + box[i].radius);
		s1 = box[i].x - box[i].radius - radius - coord[j * 3 + 1].x;
		s2 = box[i].x - box[i].radius - radius - coord[j * 3 + 2].x;
		s3 = box[i].x - box[i].radius - radius - coord[j * 3 + 3].x;
		if (!(((t1 > 0) && (t2 > 0) && (t3 > 0) && (t4 > 0)) || 
			((t1 < 0) && (t2 < 0) && (t3 < 0) && (t4 < 0)) ||
			((s1 < 0) && (s2 < 0) && (s3 < 0)) ||
			((s1 > 0) && (s2 > 0) && (s3 > 0)))) collided = true;
		// Face 3
		leftSideAD = faces[j].b * (box[i].y + box[i].radius + radius) + faces[j].d;
		t1 = leftSideAD + faces[j].a * (box[i].x + box[i].radius) + faces[j].c * (box[i].z + box[i].radius);
		t2 = leftSideAD + faces[j].a * (box[i].x + box[i].radius) + faces[j].c * (box[i].z - box[i].radius);
		t3 = leftSideAD + faces[j].a * (box[i].x - box[i].radius) + faces[j].c * (box[i].z - box[i].radius);
		t4 = leftSideAD + faces[j].a * (box[i].x - box[i].radius) + faces[j].c * (box[i].z + box[i].radius);
		s1 = box[i].y + box[i].radius + radius - coord[j * 3 + 1].y;
		s2 = box[i].y + box[i].radius + radius - coord[j * 3 + 2].y;
		s3 = box[i].y + box[i].radius + radius - coord[j * 3 + 3].y;
		if (!(((t1 > 0) && (t2 > 0) && (t3 > 0) && (t4 > 0)) || 
			((t1 < 0) && (t2 < 0) && (t3 < 0) && (t4 < 0)) ||
			((s1 < 0) && (s2 < 0) && (s3 < 0)) ||
			((s1 > 0) && (s2 > 0) && (s3 > 0)))) collided = true;
		// Face 4
		leftSideAD = faces[j].b * (box[i].y - box[i].radius - radius) + faces[j].d;
		t1 = leftSideAD + faces[j].a * (box[i].x + box[i].radius) + faces[j].c * (box[i].z + box[i].radius);
		t2 = leftSideAD + faces[j].a * (box[i].x + box[i].radius) + faces[j].c * (box[i].z - box[i].radius);
		t3 = leftSideAD + faces[j].a * (box[i].x - box[i].radius) + faces[j].c * (box[i].z - box[i].radius);
		t4 = leftSideAD + faces[j].a * (box[i].x - box[i].radius) + faces[j].c * (box[i].z + box[i].radius);
		s1 = box[i].y - box[i].radius - radius - coord[j * 3 + 1].y;
		s2 = box[i].y - box[i].radius - radius - coord[j * 3 + 2].y;
		s3 = box[i].y - box[i].radius - radius - coord[j * 3 + 3].y;
		if (!(((t1 > 0) && (t2 > 0) && (t3 > 0) && (t4 > 0)) || 
			((t1 < 0) && (t2 < 0) && (t3 < 0) && (t4 < 0)) ||
			((s1 < 0) && (s2 < 0) && (s3 < 0)) ||
			((s1 > 0) && (s2 > 0) && (s3 > 0)))) collided = true;
		// Face 5
		leftSideAD = faces[j].c * (box[i].z + box[i].radius + radius) + faces[j].d;
		t1 = leftSideAD + faces[j].a * (box[i].x + box[i].radius) + faces[j].b * (box[i].y + box[i].radius);
		t2 = leftSideAD + faces[j].a * (box[i].x + box[i].radius) + faces[j].b * (box[i].y - box[i].radius);
		t3 = leftSideAD + faces[j].a * (box[i].x - box[i].radius) + faces[j].b * (box[i].y - box[i].radius);
		t4 = leftSideAD + faces[j].a * (box[i].x - box[i].radius) + faces[j].b * (box[i].y + box[i].radius);
		s1 = box[i].z + box[i].radius + radius - coord[j * 3 + 1].z;
		s2 = box[i].z + box[i].radius + radius - coord[j * 3 + 2].z;
		s3 = box[i].z + box[i].radius + radius - coord[j * 3 + 3].z;
		if (!(((t1 > 0) && (t2 > 0) && (t3 > 0) && (t4 > 0)) || 
			((t1 < 0) && (t2 < 0) && (t3 < 0) && (t4 < 0)) ||
			((s1 < 0) && (s2 < 0) && (s3 < 0)) ||
			((s1 > 0) && (s2 > 0) && (s3 > 0)))) collided = true;
		// Face 6
		leftSideAD = faces[j].c * (box[i].z - box[i].radius - radius) + faces[j].d;
		t1 = leftSideAD + faces[j].a * (box[i].x + box[i].radius) + faces[j].b * (box[i].y + box[i].radius);
		t2 = leftSideAD + faces[j].a * (box[i].x + box[i].radius) + faces[j].b * (box[i].y - box[i].radius);
		t3 = leftSideAD + faces[j].a * (box[i].x - box[i].radius) + faces[j].b * (box[i].y - box[i].radius);
		t4 = leftSideAD + faces[j].a * (box[i].x - box[i].radius) + faces[j].b * (box[i].y + box[i].radius);
		s1 = box[i].z - box[i].radius - radius - coord[j * 3 + 1].z;
		s2 = box[i].z - box[i].radius - radius - coord[j * 3 + 2].z;
		s3 = box[i].z - box[i].radius - radius - coord[j * 3 + 3].z;
		if (!(((t1 > 0) && (t2 > 0) && (t3 > 0) && (t4 > 0)) || 
			((t1 < 0) && (t2 < 0) && (t3 < 0) && (t4 < 0)) ||
			((s1 < 0) && (s2 < 0) && (s3 < 0)) ||
			((s1 > 0) && (s2 > 0) && (s3 > 0)))) collided = true;
	}

	// Detect corner collision

	/*float dist;

	for (int j = 0; j < obstacleCounter; j++) {
		// Vertex 1
		dist = getDist(box[i].x + box[i].radius, box[i].y + box[i].radius, box[i].z + box[i].radius, j);
		if (dist > radius) collided = true;
		// Vertex 2
		dist = getDist(box[i].x + box[i].radius, box[i].y + box[i].radius, box[i].z - box[i].radius, j);
		if (dist > radius) collided = true;
		// Vertex 3
		dist = getDist(box[i].x + box[i].radius, box[i].y - box[i].radius, box[i].z - box[i].radius, j);
		if (dist > radius) collided = true;
		// Vertex 4
		dist = getDist(box[i].x + box[i].radius, box[i].y - box[i].radius, box[i].z + box[i].radius, j);
		if (dist > radius) collided = true;
		// Vertex 5
		dist = getDist(box[i].x - box[i].radius, box[i].y + box[i].radius, box[i].z + box[i].radius, j);
		if (dist > radius) collided = true;
		// Vertex 6
		dist = getDist(box[i].x - box[i].radius, box[i].y + box[i].radius, box[i].z - box[i].radius, j);
		if (dist > radius) collided = true;
		// Vertex 7
		dist = getDist(box[i].x - box[i].radius, box[i].y - box[i].radius, box[i].z - box[i].radius, j);
		if (dist > radius) collided = true;
		// Vertex 8
		dist = getDist(box[i].x - box[i].radius, box[i].y - box[i].radius, box[i].z + box[i].radius, j);
		if (dist > radius) collided = true;
	}*/

	if (!collided) return -1;
	if (box[i].radius > epsilon) return 0;
	return 1;
}

void divideBox(int i) {
	box[i].isActive = false;
	// Subbox 1
	box[boxCounter].x = box[i].x + box[i].radius / 2;
	box[boxCounter].y = box[i].y + box[i].radius / 2;
	box[boxCounter].z = box[i].z + box[i].radius / 2;
	box[boxCounter].isActive = true;
	box[boxCounter].parent = i;
	box[boxCounter].radius = box[i].radius / 2;
	box[boxCounter].isVisited = false;
	box[boxCounter].type = getBoxType(boxCounter);
	boxCounter++;
	// Subbox 2
	box[boxCounter].x = box[i].x + box[i].radius / 2;
	box[boxCounter].y = box[i].y + box[i].radius / 2;
	box[boxCounter].z = box[i].z - box[i].radius / 2;
	box[boxCounter].isActive = true;
	box[boxCounter].parent = i;
	box[boxCounter].radius = box[i].radius / 2;
	box[boxCounter].isVisited = false;
	box[boxCounter].type = getBoxType(boxCounter);
	boxCounter++;
	// Subbox 3
	box[boxCounter].x = box[i].x + box[i].radius / 2;
	box[boxCounter].y = box[i].y - box[i].radius / 2;
	box[boxCounter].z = box[i].z + box[i].radius / 2;
	box[boxCounter].isActive = true;
	box[boxCounter].parent = i;
	box[boxCounter].radius = box[i].radius / 2;
	box[boxCounter].isVisited = false;
	box[boxCounter].type = getBoxType(boxCounter);
	boxCounter++;
	// Subbox 4
	box[boxCounter].x = box[i].x + box[i].radius / 2;
	box[boxCounter].y = box[i].y - box[i].radius / 2;
	box[boxCounter].z = box[i].z - box[i].radius / 2;
	box[boxCounter].isActive = true;
	box[boxCounter].parent = i;
	box[boxCounter].radius = box[i].radius / 2;
	box[boxCounter].isVisited = false;
	box[boxCounter].type = getBoxType(boxCounter);
	boxCounter++;
	// Subbox 5
	box[boxCounter].x = box[i].x - box[i].radius / 2;
	box[boxCounter].y = box[i].y + box[i].radius / 2;
	box[boxCounter].z = box[i].z + box[i].radius / 2;
	box[boxCounter].isActive = true;
	box[boxCounter].parent = i;
	box[boxCounter].radius = box[i].radius / 2;
	box[boxCounter].isVisited = false;
	box[boxCounter].type = getBoxType(boxCounter);
	boxCounter++;
	// Subbox 6
	box[boxCounter].x = box[i].x - box[i].radius / 2;
	box[boxCounter].y = box[i].y + box[i].radius / 2;
	box[boxCounter].z = box[i].z - box[i].radius / 2;
	box[boxCounter].isActive = true;
	box[boxCounter].parent = i;
	box[boxCounter].radius = box[i].radius / 2;
	box[boxCounter].isVisited = false;
	box[boxCounter].type = getBoxType(boxCounter);
	boxCounter++;
	// Subbox 7
	box[boxCounter].x = box[i].x - box[i].radius / 2;
	box[boxCounter].y = box[i].y - box[i].radius / 2;
	box[boxCounter].z = box[i].z + box[i].radius / 2;
	box[boxCounter].isActive = true;
	box[boxCounter].parent = i;
	box[boxCounter].radius = box[i].radius / 2;
	box[boxCounter].isVisited = false;
	box[boxCounter].type = getBoxType(boxCounter);
	boxCounter++;
	// Subbox 8
	box[boxCounter].x = box[i].x - box[i].radius / 2;
	box[boxCounter].y = box[i].y - box[i].radius / 2;
	box[boxCounter].z = box[i].z - box[i].radius / 2;
	box[boxCounter].isActive = true;
	box[boxCounter].parent = i;
	box[boxCounter].radius = box[i].radius / 2;
	box[boxCounter].isVisited = false;
	box[boxCounter].type = getBoxType(boxCounter);
	boxCounter++;
}

bool isBoxConnected(int i, int j) {
	if ((isEqual(box[i].radius + box[j].radius, abs(box[i].x - box[j].x))) &&
		(box[i].y <= box[j].y + box[j].radius) && (box[i].y >= box[j].y - box[j].radius) &&
		(box[i].z <= box[j].z + box[j].radius) && (box[i].z >= box[j].z - box[j].radius)) return true;
	if ((isEqual(box[i].radius + box[j].radius, abs(box[i].y - box[j].y))) &&
		(box[i].x <= box[j].x + box[j].radius) && (box[i].x >= box[j].x - box[j].radius) &&
		(box[i].z <= box[j].z + box[j].radius) && (box[i].z >= box[j].z - box[j].radius)) return true;
	if ((isEqual(box[i].radius + box[j].radius, abs(box[i].z - box[j].z))) &&
		(box[i].y <= box[j].y + box[j].radius) && (box[i].y >= box[j].y - box[j].radius) &&
		(box[i].x <= box[j].x + box[j].radius) && (box[i].x >= box[j].x - box[j].radius)) return true;
	int t = i; i = j; j = t;
	if ((isEqual(box[i].radius + box[j].radius, abs(box[i].x - box[j].x))) &&
		(box[i].y <= box[j].y + box[j].radius) && (box[i].y >= box[j].y - box[j].radius) &&
		(box[i].z <= box[j].z + box[j].radius) && (box[i].z >= box[j].z - box[j].radius)) return true;
	if ((isEqual(box[i].radius + box[j].radius, abs(box[i].y - box[j].y))) &&
		(box[i].x <= box[j].x + box[j].radius) && (box[i].x >= box[j].x - box[j].radius) &&
		(box[i].z <= box[j].z + box[j].radius) && (box[i].z >= box[j].z - box[j].radius)) return true;
	if ((isEqual(box[i].radius + box[j].radius, abs(box[i].z - box[j].z))) &&
		(box[i].y <= box[j].y + box[j].radius) && (box[i].y >= box[j].y - box[j].radius) &&
		(box[i].x <= box[j].x + box[j].radius) && (box[i].x >= box[j].x - box[j].radius)) return true;
	return false;
}

bool findPathIter(int i) {

	if (isInBox(i, xEnd, yEnd, zEnd)) {
		sinkBox = i; 
		return true;
	}
	box[i].isVisited = true;
	for (int j = 0; j < boxCounter; j++) {
		if ((box[j].isVisited == false) && (isBoxConnected(i, j))) {
			if (box[j].type == 0) {
				box[j].isVisited = true;
				divideBox(j);
			} else if (box[j].type == -1) {
				box[j].previous = i;
				bool found = findPathIter(j);
				if (found) return found;
			}
		}
	}

	return false;

}

int nextRandomUnvisitedConnectedBoxes[1000];

int randomUnvisitedConnectedBoxCounter = 0, nextRandomUnvisitedConnectedBoxCounter = 2;

int getNextRandomUnvisitedConnectedBox(int i) {
	while (nextRandomUnvisitedConnectedBoxCounter <= boxCounter) {
			if (box[nextRandomUnvisitedConnectedBoxCounter - 1].type != 1) {
				nextRandomUnvisitedConnectedBoxes[randomUnvisitedConnectedBoxCounter] = 
					nextRandomUnvisitedConnectedBoxCounter - 1;
				randomUnvisitedConnectedBoxCounter++;
				// Random swapping order
				int rn = rand() % randomUnvisitedConnectedBoxCounter;
				int t = nextRandomUnvisitedConnectedBoxes[rn];
				nextRandomUnvisitedConnectedBoxes[rn] = nextRandomUnvisitedConnectedBoxes[randomUnvisitedConnectedBoxCounter - 1];
				nextRandomUnvisitedConnectedBoxes[randomUnvisitedConnectedBoxCounter - 1] = t;
			}
			nextRandomUnvisitedConnectedBoxCounter++;
	}
			
	for (int j = 0; j < randomUnvisitedConnectedBoxCounter; j++) {
		if (isBoxConnected(i, nextRandomUnvisitedConnectedBoxes[j])) {
				int r = nextRandomUnvisitedConnectedBoxes[j];
				randomUnvisitedConnectedBoxCounter--;
				nextRandomUnvisitedConnectedBoxes[j] = nextRandomUnvisitedConnectedBoxes[randomUnvisitedConnectedBoxCounter];
				return r;	
		}
	}
	return -1;
}

bool findPathIterRand(int i) {

	if (isInBox(i, xEnd, yEnd, zEnd)) {
		sinkBox = i; 
		return true;
	}
	box[i].isVisited = true;
	int j;
	do {
		j = getNextRandomUnvisitedConnectedBox(i);
		
			if (box[j].type == 0) {
				box[j].isVisited = true;
				divideBox(j);
			} else if (box[j].type == -1) {
				box[j].previous = i;
				bool found = findPathIter(j);
				if (found) return found;
			}

	} while (j != -1);

	return false;

}

void findPath() {
	// Reset
	boxCounter = 1;
	box[0].x = 0;
	box[0].y = 0;
	box[0].z = 0;
	box[0].isActive = true;
	box[0].type = 0;
	box[0].radius = maxRadius;
	box[0].isVisited = true;
	sourceBox = 0;
	sinkBox = 0;
	randomUnvisitedConnectedBoxCounter = 0;
	nextRandomUnvisitedConnectedBoxCounter = 2;
	// Divide box in the starting loc
	while (box[sourceBox].type == 0) {
		divideBox(sourceBox);
		sourceBox = getSourceBox();
	}
	if (!isRand) {
		found = findPathIter(sourceBox);
	}
	else found = findPathIterRand(sourceBox);
}

void showNextBoxType() {
	nextBoxType += 1;
	if (nextBoxType > 4) nextBoxType = 0;
	switch (nextBoxType) {
		case 0: inputString = "Showing all boxes"; break;
		case 1: inputString = "Showing FREE boxes"; break;
		case 2: inputString = "Showing MIXED boxes"; break;
		case 3: inputString = "Showing STUCK boxes"; break;
		case 4: inputString = "Showing VISTED boxes during the search"; break;
	}
}

void reshape(int w,int h)
{
    glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(75.0f, (float)w/h, 1.0f, 1000.0f);
    glMatrixMode( GL_MODELVIEW );
}

void initFromFile(std::string fileName) {

	obstacleCounter = 0;
	
	float x1, x2, x3, y1, y2, y3, z1, z2, z3;
	
	std::ifstream iFile(fileName + "obstacles.txt");
	while (iFile>>x1>>y1>>z1>>x2>>y2>>z2>>x3>>y3>>z3) {
		coord[obstacleCounter * 3 + 1].x = x1;
		coord[obstacleCounter * 3 + 2].x = x2;
		coord[obstacleCounter * 3 + 3].x = x3;
		coord[obstacleCounter * 3 + 1].y = y1;
		coord[obstacleCounter * 3 + 2].y = y2;
		coord[obstacleCounter * 3 + 3].y = y3;
		coord[obstacleCounter * 3 + 1].z = z1;
		coord[obstacleCounter * 3 + 2].z = z2;
		coord[obstacleCounter * 3 + 3].z = z3;
		faces[obstacleCounter].a = y1 * z2 - y1 * z3 - y2 * z1 + y2 * z3 + y3 * z1 - y3 * z2;
		faces[obstacleCounter].b = -x1 * z2 + x1 * z3 + x2 * z1 - x2 * z3 - x3 * z1 + x3 * z2;
		faces[obstacleCounter].c = x1 * y2 - x1 * y3 - x2 * y1 + x2 * y3 + x3 * y1 - x3 * y2;
		faces[obstacleCounter].d = x1 * y2 * z3 - x1 * y3 * z2 - x2 * y1 * z3 + x2 * y3 * z1 + x3 * y1 * z2 - x3 * y2 * z1;
		obstacleCounter++;
	}

	std::ifstream jFile(fileName + "sphereloc.txt");
	jFile>>radius>>xStart>>yStart>>zStart>>xEnd>>yEnd>>zEnd;

	inputString = "";

}

void runCommand(std::string s) {

	bool q = false;

	if (std::regex_match(s, std::regex("run"))) {
		q = true;
		isRand = false;
		isDijk = false;
		findPath();
		if (found) { inputString = "Path found.";} else { inputString = "No path found.";}
	}
	if (std::regex_match(s, std::regex("run s(equential)?"))) {
		q = true;
		isRand = false;
		isDijk = false;
		findPath();
		if (found) { inputString = "Path found.";} else { inputString = "No path found.";}
	}
	if (std::regex_match(s, std::regex("run r(andom)?"))) {
		q = true;
		isRand = true;
		isDijk = false;
		findPath();
		if (found) { inputString = "Path found.";} else { inputString = "No path found.";}
	}
	if (std::regex_match(s, std::regex("run l(owd)?"))) {
		/*q = true;
		isRand = false;
		isDijk = true;
		findPath();
		if (found) { inputString = "Path found.";} else { inputString = "No path found.";}*/
	}
	if (std::regex_match(s, std::regex("hidebox"))) {
		q = true;
		isShowBox = false;
		inputString = "";
	}
	if (std::regex_match(s, std::regex("showbox"))) {
		q = true;
		isShowBox = true;
		inputString = "";
	}
	if (std::regex_match(s, std::regex("hidepath"))) {
		q = true;
		isShowPath = false;
		inputString = "";
	}
	if (std::regex_match(s, std::regex("showpath"))) {
		q = true;
		isShowPath = true;
		inputString = "";
	}
	if (std::regex_match(s, std::regex("set epsilon ([0-9])+"))) {
		int epsilonI = std::atoi(s.substr(12, s.length() - 12).c_str());
		if (epsilonI != 0) {
			q = true;
			epsilon = epsilonI;
			inputString = "Epsilon set to " + epsilonI;
		}
	}
	if (std::regex_match(s, std::regex("set filepath (.)+"))) {
		std::string fileName = s.substr(13, s.length() - 13);
		initFromFile(fileName + "/");
	}
	if (std::regex_match(s, std::regex("reset filepath"))) {
		initFromFile("");
	}
	
	if (!q) inputString = "Invalid command or parameter(s).";
}

void keyMoveObj (GLubyte moveKey, GLint xMouse, GLint yMouse)
{
	if (isInputMode) {
		switch (moveKey) {
		case ESC: isInputMode = false; inputString = ""; break;
		case BACKSPACE: if (inputString.length() > 0) inputString.erase(inputString.length() - 1, 1); break;
		case ENTER: runCommand(inputString); isInputMode = false; break;
		default: inputString += moveKey; break;
		}
	} else
	{ 
		switch (moveKey) {
			case ':': isInputMode = !isInputMode; inputString = ""; break;
			case 'r': findPath(); break;
			case 'b': isShowBox = !isShowBox; break;
			case 'p': isShowPath = !isShowPath; break;
			case 't': showNextBoxType(); break;
			case 'w': if (r > 5) r -= 2; break;
			case 's': r += 2; break;
		}
	}

}

int main_t(int argc, char *argv[]) 
{ 

    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH ); 
    glutInitWindowPosition(100, 100); 
    glutInitWindowSize(WINDOWS_WIDTH, WINDOWS_HEIGHT); 
    windowID = glutCreateWindow("Motion Planning"); 
    glutReshapeFunc(reshape);
    glutDisplayFunc(display); 
	glutIdleFunc(display);
    glutMouseFunc(Mouse);
    glutMotionFunc(onMouseMove);
    glutKeyboardFunc (keyMoveObj);
    glutMainLoop();

    return 0; 
}  

int main(int argc, char *argv[]) {

	initFromFile("");

	//testUseOnlyInit();

	inputString = "Started.";

	main_t(argc, argv);
}