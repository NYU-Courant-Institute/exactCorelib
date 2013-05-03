/**************************************************
 *  3D sphere motion planning with rigid obstacles
 *  NOTE: This program is an adaptation of 
 *  Author: Xinwei Lin
 *
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

#ifdef __CYGWIN32__
#include "glui.h"
#endif
#ifdef _WIN32
#include <gl/glui.h>
#endif
#ifdef __APPLE__
#include <GL/glui.h>
#include "GL/glui.h"
#endif

#define MAXFLOAT 100000;

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

bool isShowBox = 1; 
bool isShowPath = 1;
bool isInputMode = false;
int nextBoxType = 0;

double epsilon = 10; // Minimum size of a divided box
float maxRadius = 50; // Max size of the whole space

int windowID;

// Prelimenary types

int obstacleCounter; // Total faces of all obstacles

struct Coord {
	float x, y, z;
};

Coord coord[1000]; // Coordinates of the faces of obstacles

struct Face {
	float a, b, c, d;
    Coord coord[3];
};

Face faces[1000];

int boxCounter;

struct Box {
	bool isActive;
	float x, y, z, radius;
	int type; //0 = mix, 1 = stuck, -1 = free
	int parent, previous;
	bool isVisited;
	float distToSink;
};

Box boxes[1000];

int sphereRadius, xStart, yStart, zStart, xEnd, yEnd, zEnd;

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
			glVertex3f(faces[i].coord[0].x, faces[i].coord[0].y, faces[i].coord[0].z);
		    glVertex3f(faces[i].coord[1].x, faces[i].coord[1].y, faces[i].coord[1].z);
			glVertex3f(faces[i].coord[2].x, faces[i].coord[2].y, faces[i].coord[2].z);
		}
		glEnd();

		glBegin(GL_LINES);
		{
			glColor4f(0.0, 0.0, 0.0, 1.0);
			glVertex3f(faces[i].coord[0].x, faces[i].coord[0].y, faces[i].coord[0].z);
		    glVertex3f(faces[i].coord[1].x, faces[i].coord[1].y, faces[i].coord[1].z);
			glVertex3f(faces[i].coord[2].x, faces[i].coord[2].y, faces[i].coord[2].z);
			glVertex3f(faces[i].coord[0].x, faces[i].coord[0].y, faces[i].coord[0].z);
		    glVertex3f(faces[i].coord[1].x, faces[i].coord[1].y, faces[i].coord[1].z);
			glVertex3f(faces[i].coord[2].x, faces[i].coord[2].y, faces[i].coord[2].z);
		}
		glEnd();
	}

	// Draw sphere
	glColor4f(0.5, 0.45, 0.8, 1.0);
	GLUquadric *quad = gluNewQuadric();
	glPushMatrix();
	glTranslatef(xStart, yStart, zStart);
	gluSphere(quad, sphereRadius, 20, 20);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(xEnd, yEnd, zEnd);
	gluSphere(quad, sphereRadius, 20, 20);
	glPopMatrix();

	// Draw route
	if ((isShowPath) && (sinkBox != 0)) {
		glBegin(GL_LINES);
		{
			glVertex3f(boxes[sourceBox].x, boxes[sourceBox].y, boxes[sourceBox].z);
			glVertex3f(xStart, yStart, zStart);
			glVertex3f(boxes[sinkBox].x, boxes[sinkBox].y, boxes[sinkBox].z);
			glVertex3f(xEnd, yEnd, zEnd);
		}
		glEnd();
		int i = sinkBox;
		while (i != sourceBox) {
			glBegin(GL_LINES);
			{
				glVertex3f(boxes[i].x, boxes[i].y, boxes[i].z);
				glVertex3f(boxes[boxes[i].previous].x, boxes[boxes[i].previous].y, boxes[boxes[i].previous].z);
			}
			glEnd();
			i = boxes[i].previous;
		}
	}

	// Draw boxes
	glDisable(GL_DEPTH_TEST);
	if (isShowBox == 1) for (int i = 0; i < boxCounter; i++)
		if (boxes[i].isActive)
	{
		boxes[i].radius -= 0.5;
		glBegin(GL_QUADS);
		{
			switch (boxes[i].type) {
			case -1: glColor4f(0.1, 0.9, 0.1, 0.1); if ((nextBoxType != 0) && (nextBoxType != 1)) glColor4f(0.0, 0.0, 0.0, 0.0); break;
			case 0: glColor4f(0.8, 0.7, 0.0, 0.1); if ((nextBoxType != 0) && (nextBoxType != 2)) glColor4f(0.0, 0.0, 0.0, 0.0); break;
			case 1: glColor4f(0.8, 0.2, 0.1, 0.1); if ((nextBoxType != 0) && (nextBoxType != 3)) glColor4f(0.0, 0.0, 0.0, 0.0); break;
			}
			if ((nextBoxType == 4) && (boxes[i].isVisited)) glColor4f(0.0, 0.0, 1.0, 0.2);
			if ((nextBoxType == 4) && (boxes[i].isVisited)) glColor4f(0.0, 0.0, 1.0, 0.2);
			// face 1
			glVertex3f(boxes[i].x + boxes[i].radius, boxes[i].y + boxes[i].radius, boxes[i].z + boxes[i].radius);
			glVertex3f(boxes[i].x + boxes[i].radius, boxes[i].y - boxes[i].radius, boxes[i].z + boxes[i].radius);
			glVertex3f(boxes[i].x + boxes[i].radius, boxes[i].y - boxes[i].radius, boxes[i].z - boxes[i].radius);
			glVertex3f(boxes[i].x + boxes[i].radius, boxes[i].y + boxes[i].radius, boxes[i].z - boxes[i].radius);
			// face 2
			glVertex3f(boxes[i].x - boxes[i].radius, boxes[i].y + boxes[i].radius, boxes[i].z + boxes[i].radius);
			glVertex3f(boxes[i].x - boxes[i].radius, boxes[i].y - boxes[i].radius, boxes[i].z + boxes[i].radius);
			glVertex3f(boxes[i].x - boxes[i].radius, boxes[i].y - boxes[i].radius, boxes[i].z - boxes[i].radius);
			glVertex3f(boxes[i].x - boxes[i].radius, boxes[i].y + boxes[i].radius, boxes[i].z - boxes[i].radius);
			// face 3
			glVertex3f(boxes[i].x + boxes[i].radius, boxes[i].y + boxes[i].radius, boxes[i].z + boxes[i].radius);
			glVertex3f(boxes[i].x - boxes[i].radius, boxes[i].y + boxes[i].radius, boxes[i].z + boxes[i].radius);
			glVertex3f(boxes[i].x - boxes[i].radius, boxes[i].y + boxes[i].radius, boxes[i].z - boxes[i].radius);
			glVertex3f(boxes[i].x + boxes[i].radius, boxes[i].y + boxes[i].radius, boxes[i].z - boxes[i].radius);
			// face 4
			glVertex3f(boxes[i].x + boxes[i].radius, boxes[i].y - boxes[i].radius, boxes[i].z + boxes[i].radius);
			glVertex3f(boxes[i].x - boxes[i].radius, boxes[i].y - boxes[i].radius, boxes[i].z + boxes[i].radius);
			glVertex3f(boxes[i].x - boxes[i].radius, boxes[i].y - boxes[i].radius, boxes[i].z - boxes[i].radius);
			glVertex3f(boxes[i].x + boxes[i].radius, boxes[i].y - boxes[i].radius, boxes[i].z - boxes[i].radius);
			// face 5
			glVertex3f(boxes[i].x + boxes[i].radius, boxes[i].y + boxes[i].radius, boxes[i].z + boxes[i].radius);
			glVertex3f(boxes[i].x - boxes[i].radius, boxes[i].y + boxes[i].radius, boxes[i].z + boxes[i].radius);
			glVertex3f(boxes[i].x - boxes[i].radius, boxes[i].y - boxes[i].radius, boxes[i].z + boxes[i].radius);
			glVertex3f(boxes[i].x + boxes[i].radius, boxes[i].y - boxes[i].radius, boxes[i].z + boxes[i].radius);
			// face 6
			glVertex3f(boxes[i].x + boxes[i].radius, boxes[i].y + boxes[i].radius, boxes[i].z - boxes[i].radius);
			glVertex3f(boxes[i].x - boxes[i].radius, boxes[i].y + boxes[i].radius, boxes[i].z - boxes[i].radius);
			glVertex3f(boxes[i].x - boxes[i].radius, boxes[i].y - boxes[i].radius, boxes[i].z - boxes[i].radius);
			glVertex3f(boxes[i].x + boxes[i].radius, boxes[i].y - boxes[i].radius, boxes[i].z - boxes[i].radius);
		}
		glEnd();

		/*glBegin(GL_LINES);
		{
			glColor4f(0.1, 0.1, 0.1, 0.8);
			glVertex3f(boxes[i].x + boxes[i].radius, boxes[i].y + boxes[i].radius, boxes[i].z - boxes[i].radius);
			glVertex3f(boxes[i].x - boxes[i].radius, boxes[i].y + boxes[i].radius, boxes[i].z - boxes[i].radius);
			glVertex3f(boxes[i].x + boxes[i].radius, boxes[i].y + boxes[i].radius, boxes[i].z + boxes[i].radius);
			glVertex3f(boxes[i].x - boxes[i].radius, boxes[i].y + boxes[i].radius, boxes[i].z + boxes[i].radius);
			glVertex3f(boxes[i].x + boxes[i].radius, boxes[i].y - boxes[i].radius, boxes[i].z - boxes[i].radius);
			glVertex3f(boxes[i].x - boxes[i].radius, boxes[i].y - boxes[i].radius, boxes[i].z - boxes[i].radius);
			glVertex3f(boxes[i].x + boxes[i].radius, boxes[i].y - boxes[i].radius, boxes[i].z + boxes[i].radius);
			glVertex3f(boxes[i].x - boxes[i].radius, boxes[i].y - boxes[i].radius, boxes[i].z + boxes[i].radius);
		}
		glEnd();*/
		boxes[i].radius += 0.5;
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
    if(state == GLUT_DOWN) {
		oldmx = x;
		oldmy = y;
	}
    if(state == GLUT_UP && button == GLUT_WHEEL_UP) {
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
	if ((boxes[i].x + boxes[i].radius >= x) && (boxes[i].x - boxes[i].radius <= x)
		&& (boxes[i].y + boxes[i].radius >= y) && (boxes[i].y - boxes[i].radius <= y)
		&& (boxes[i].z + boxes[i].radius >= z) && (boxes[i].z - boxes[i].radius <= z))
		return true;
	return false;
}

int getSourceBox() {
	for (int i = 0; i < boxCounter; i++)
		if ((boxes[i].isActive) && (isInBox(i, xStart, yStart, zStart))) {
			return i;
		}
	return 0;
}

float sqr(float i) {
	return i * i;
}

float getDist(float px, float py, float pz, int face) {  //Deprecated
	float baseDist = abs(px * faces[face].a + py * faces[face].b + pz * faces[face].c + faces[face].d) / 
		sqrt(sqr(faces[face].a) + sqr(faces[face].b) + sqr(faces[face].c));
	return baseDist;
}

float getDistToPoint(float x, float y, float z, int faceID, int cornerID) {
	return sqrt(sqr(x - faces[faceID].coord[cornerID].x) + sqr(y - faces[faceID].coord[cornerID].y) + sqr(z - faces[faceID].coord[cornerID].z));
}

float getDistToWall(float x, float y, float z, int faceID, int cornerID) {
	// NOT NECCESSARY?
	return 10000; //Nominal big number
}

class Vector {
	public:
		float x, y, z;
		Vector() {
			x = 0;
			y = 0;
			z = 0;
		}

		Vector(float ox, float oy, float oz) {
			x = ox;
			y = oy;
			z = oz;
		}
		
		Vector operator + (Vector other);
		Vector operator - (Vector other);
		//Vector operator = (Vector other);

		static float dot(Vector v1, Vector v2);
	};

Vector Vector::operator+(Vector other) {
	return Vector(x + other.x, y + other.y, z + other.z);
}

Vector Vector::operator-(Vector other) {
	return Vector(x - other.x, y - other.y, z - other.z);
}

float Vector::dot(Vector v1, Vector v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Readapted from http://www.mathworks.com/matlabcentral/fileexchange/22857-distance-between-a-point-and-a-triangle-in-3d
float getDistToFace(float x, float y, float z, int faceID) {

	// Init

	Vector p0(x, y, z), p1(faces[faceID].coord[0].x, faces[faceID].coord[0].y, faces[faceID].coord[0].z),
		p2(faces[faceID].coord[1].x, faces[faceID].coord[1].y, faces[faceID].coord[1].z),
		p3(faces[faceID].coord[2].x, faces[faceID].coord[2].y, faces[faceID].coord[2].z);

	// Normalize Triangle

	Vector B, e0, e1;
	B = p1;
	e0 = p2 - B;
	e1 = p3 - B;

	Vector D;
	D = B - p0;

	float a, b, c, d, e, f;
	a = Vector::dot(e0, e0);
	b = Vector::dot(e0, e1);
	c = Vector::dot(e1, e1);
	d = Vector::dot(e0, D);
	e = Vector::dot(e1, D);
	f = Vector::dot(D, D);

	float det, s, t;
	det = a * c - b * b;
	s = b * e - c * d;
	t = b * d - a * e;

	// Conditioning

	if (s + t <= det) {
		if (s < 0) {
			if (t < 0) {
				// Region 4
				if (d < 0) {
					t = 0;
					if (-d >= a) {
						s = 1;
						return a + 2 * d + f;
					}
					else {
						s = -d / a;
						return d * s + f;
					}
				}
				else {
					s = 0;
					if (e >= 0) {
						t = 0;
						return f;
					}
					else {
						t = -e / c;
						return e * t + f;
					}
				}
			} // End of Region 4
			else {
				// Region 3
				s = 0;
				if (e >= 0) {
					t = 0;
					return f;
				}
				else {
					if (-e >= c) {
						t = 1;
						return c + 2 * e + f;
					}
					else {
						t = -e / c;
						return e * t + f;
					}
				}
			} // End of Region 3
		}
		else {
			if (t < 0) {
				// Region 5
				t = 0;
				if (d >= 0) {
					s = 0;
					return f;
				}
				else {
					if (-d >= a) {
						s = 1;
						return a + 2 * d + f;
					}
					else {
						s = -d / a;
						return d * s + f;
					}
				}
				// End of Region 5
			}
			else {
				// Region 0
				float invDet = 1 / det;
				s = s * invDet;
				t = t * invDet;
				return s * (a * s + b * t + 2 * d) + t * (b * s + c * t + 2 * e) + f;
				// End of Region 0
			}
		}
	}
	else {
		if (s < 0) {
			// Region 2
			float tmp0, tmp1;
			tmp0 = b + d;
			tmp1 = c + e;
			if (tmp1 > tmp0) {
				float numer = tmp1 - tmp0;
				float denom = a - 2 * b + c;
				if (numer >= denom) {
					s = 1;
					t = 0;
					return a + 2 * d + f;
				}
				else {
					s = numer / denom;
					t = 1 - s;
					return s * (a * s + b * t + 2 * d) + t * (b * s + c * t + 2 * e) + f;
				}
			}
			else {
				s = 0;
				if (tmp1 <= 0) {
					t = 1;
					return c + 2 * e + f;
				}
				else {
					if (e >= 0) {
						t = 0;
						return f;
					}
					else {
						t = -e / c;
						return e * t + f;
					}
				}
			}
			// End of Region 2
		}
		else {
			if (t < 0) {
				// Region 6
				float tmp0, tmp1;
				tmp0 = b + e;
				tmp1 = a + d;
				if (tmp1 > tmp0) {
					float numer = tmp1 - tmp0;
					float denom = a - 2 * b + c;
					if (numer >= denom) {
						t = 1;
						s = 0;
						return c + 2 * e + f;
					}
					else {
						t = numer / denom;
						s = 1 - t;
						return s * (a * s + b * t + 2 * d) + t * (b * s + c * t + 2 * e) + f;
					}
				}
				else {
					t = 0;
					if (tmp1 <= 0) {
						s = 1;
						return a + 2 * d + f;
					}
					else {
						if (d >= 0) {
							s = 0;
							return f;
						}
						else {
							s = -d / a;
							return d * s + f;
						}
					}
				}
				// End of Region 6
			}
			else {
				// Region 1
				float numer = c + e - b - d;
				if (numer <= 0) {
					s = 0;
					t = 1;
					return c + 2 * e + f;
				}
				else {
					float denom = a - 2 * b + c;
					if (numer >= denom) {
						s = 1;
						t = 0;
						return a + 2 * d + f;
					}
					else {
						s = numer / denom;
						t = 1 - s;
						return s * (a * s + b * t + 2 * d) + t * (b * s + c * t + 2 * e) + f;
					}
				}
				// End of Region 1
			}
		}
	}

	return 0;
}

int getBoxType(int boxID) {
	
	bool collided = false; // indicates whether being mixed

	float innerDomain = sphereRadius;
	float outerDomain = boxes[boxID].radius * sqrt(2.0) + sphereRadius;

	for (int i = 0; i < obstacleCounter; i++) {
		
		// Check Coreners

		for (int j = 0; j < 3; j++) {
			float distToCorner = getDistToPoint(boxes[boxID].x, boxes[boxID].y, boxes[boxID].z, i, j);
			if (distToCorner < innerDomain) return 1;
			if (distToCorner < outerDomain) collided = true;
		}

		// Check Walls

		for (int j = 0; j < 3; j++) {
			float distToWall = getDistToWall(boxes[boxID].x, boxes[boxID].y, boxes[boxID].z, i, j);
			if (distToWall < innerDomain) return 1;
			if (distToWall < outerDomain) collided = true;
		}

		// Check Faces

		float distToFace = sqrt(getDistToFace(boxes[boxID].x, boxes[boxID].y, boxes[boxID].z, i));
		if (distToFace < innerDomain) return 1;
		if (distToFace < outerDomain) collided = true;

	}
	
	if (collided) return 0;
		else return -1;
}

void divideBox(int i) {
	boxes[i].isActive = false;
	// Subbox 1
	boxes[boxCounter].x = boxes[i].x + boxes[i].radius / 2;
	boxes[boxCounter].y = boxes[i].y + boxes[i].radius / 2;
	boxes[boxCounter].z = boxes[i].z + boxes[i].radius / 2;
	boxes[boxCounter].isActive = true;
	boxes[boxCounter].parent = i;
	boxes[boxCounter].radius = boxes[i].radius / 2;
	boxes[boxCounter].isVisited = false;
	boxes[boxCounter].type = getBoxType(boxCounter);
	boxCounter++;
	// Subbox 2
	boxes[boxCounter].x = boxes[i].x + boxes[i].radius / 2;
	boxes[boxCounter].y = boxes[i].y + boxes[i].radius / 2;
	boxes[boxCounter].z = boxes[i].z - boxes[i].radius / 2;
	boxes[boxCounter].isActive = true;
	boxes[boxCounter].parent = i;
	boxes[boxCounter].radius = boxes[i].radius / 2;
	boxes[boxCounter].isVisited = false;
	boxes[boxCounter].type = getBoxType(boxCounter);
	boxCounter++;
	// Subbox 3
	boxes[boxCounter].x = boxes[i].x + boxes[i].radius / 2;
	boxes[boxCounter].y = boxes[i].y - boxes[i].radius / 2;
	boxes[boxCounter].z = boxes[i].z + boxes[i].radius / 2;
	boxes[boxCounter].isActive = true;
	boxes[boxCounter].parent = i;
	boxes[boxCounter].radius = boxes[i].radius / 2;
	boxes[boxCounter].isVisited = false;
	boxes[boxCounter].type = getBoxType(boxCounter);
	boxCounter++;
	// Subbox 4
	boxes[boxCounter].x = boxes[i].x + boxes[i].radius / 2;
	boxes[boxCounter].y = boxes[i].y - boxes[i].radius / 2;
	boxes[boxCounter].z = boxes[i].z - boxes[i].radius / 2;
	boxes[boxCounter].isActive = true;
	boxes[boxCounter].parent = i;
	boxes[boxCounter].radius = boxes[i].radius / 2;
	boxes[boxCounter].isVisited = false;
	boxes[boxCounter].type = getBoxType(boxCounter);
	boxCounter++;
	// Subbox 5
	boxes[boxCounter].x = boxes[i].x - boxes[i].radius / 2;
	boxes[boxCounter].y = boxes[i].y + boxes[i].radius / 2;
	boxes[boxCounter].z = boxes[i].z + boxes[i].radius / 2;
	boxes[boxCounter].isActive = true;
	boxes[boxCounter].parent = i;
	boxes[boxCounter].radius = boxes[i].radius / 2;
	boxes[boxCounter].isVisited = false;
	boxes[boxCounter].type = getBoxType(boxCounter);
	boxCounter++;
	// Subbox 6
	boxes[boxCounter].x = boxes[i].x - boxes[i].radius / 2;
	boxes[boxCounter].y = boxes[i].y + boxes[i].radius / 2;
	boxes[boxCounter].z = boxes[i].z - boxes[i].radius / 2;
	boxes[boxCounter].isActive = true;
	boxes[boxCounter].parent = i;
	boxes[boxCounter].radius = boxes[i].radius / 2;
	boxes[boxCounter].isVisited = false;
	boxes[boxCounter].type = getBoxType(boxCounter);
	boxCounter++;
	// Subbox 7
	boxes[boxCounter].x = boxes[i].x - boxes[i].radius / 2;
	boxes[boxCounter].y = boxes[i].y - boxes[i].radius / 2;
	boxes[boxCounter].z = boxes[i].z + boxes[i].radius / 2;
	boxes[boxCounter].isActive = true;
	boxes[boxCounter].parent = i;
	boxes[boxCounter].radius = boxes[i].radius / 2;
	boxes[boxCounter].isVisited = false;
	boxes[boxCounter].type = getBoxType(boxCounter);
	boxCounter++;
	// Subbox 8
	boxes[boxCounter].x = boxes[i].x - boxes[i].radius / 2;
	boxes[boxCounter].y = boxes[i].y - boxes[i].radius / 2;
	boxes[boxCounter].z = boxes[i].z - boxes[i].radius / 2;
	boxes[boxCounter].isActive = true;
	boxes[boxCounter].parent = i;
	boxes[boxCounter].radius = boxes[i].radius / 2;
	boxes[boxCounter].isVisited = false;
	boxes[boxCounter].type = getBoxType(boxCounter);
	boxCounter++;
}

bool isBoxConnected(int i, int j) {
	// NEED TO BE REWRITTEN, TEMPORARY
	if ((isEqual(boxes[i].radius + boxes[j].radius, abs(boxes[i].x - boxes[j].x))) &&
		(boxes[i].y <= boxes[j].y + boxes[j].radius) && (boxes[i].y >= boxes[j].y - boxes[j].radius) &&
		(boxes[i].z <= boxes[j].z + boxes[j].radius) && (boxes[i].z >= boxes[j].z - boxes[j].radius)) return true;
	if ((isEqual(boxes[i].radius + boxes[j].radius, abs(boxes[i].y - boxes[j].y))) &&
		(boxes[i].x <= boxes[j].x + boxes[j].radius) && (boxes[i].x >= boxes[j].x - boxes[j].radius) &&
		(boxes[i].z <= boxes[j].z + boxes[j].radius) && (boxes[i].z >= boxes[j].z - boxes[j].radius)) return true;
	if ((isEqual(boxes[i].radius + boxes[j].radius, abs(boxes[i].z - boxes[j].z))) &&
		(boxes[i].y <= boxes[j].y + boxes[j].radius) && (boxes[i].y >= boxes[j].y - boxes[j].radius) &&
		(boxes[i].x <= boxes[j].x + boxes[j].radius) && (boxes[i].x >= boxes[j].x - boxes[j].radius)) return true;
	int t = i; i = j; j = t;
	if ((isEqual(boxes[i].radius + boxes[j].radius, abs(boxes[i].x - boxes[j].x))) &&
		(boxes[i].y <= boxes[j].y + boxes[j].radius) && (boxes[i].y >= boxes[j].y - boxes[j].radius) &&
		(boxes[i].z <= boxes[j].z + boxes[j].radius) && (boxes[i].z >= boxes[j].z - boxes[j].radius)) return true;
	if ((isEqual(boxes[i].radius + boxes[j].radius, abs(boxes[i].y - boxes[j].y))) &&
		(boxes[i].x <= boxes[j].x + boxes[j].radius) && (boxes[i].x >= boxes[j].x - boxes[j].radius) &&
		(boxes[i].z <= boxes[j].z + boxes[j].radius) && (boxes[i].z >= boxes[j].z - boxes[j].radius)) return true;
	if ((isEqual(boxes[i].radius + boxes[j].radius, abs(boxes[i].z - boxes[j].z))) &&
		(boxes[i].y <= boxes[j].y + boxes[j].radius) && (boxes[i].y >= boxes[j].y - boxes[j].radius) &&
		(boxes[i].x <= boxes[j].x + boxes[j].radius) && (boxes[i].x >= boxes[j].x - boxes[j].radius)) return true;
	return false;
}

bool isBoxDividable(int boxID) {
	if (boxes[boxID].radius < epsilon) return false;
	return true;
}

bool findPathIter(int curBox) {

	if (isInBox(curBox, xEnd, yEnd, zEnd)) {
		sinkBox = curBox; 
		return true;
	}
	boxes[curBox].isVisited = true;
	for (int prochBox = 0; prochBox < boxCounter; prochBox++) {
		if ((boxes[prochBox].isActive) && (!boxes[prochBox].isVisited) && (isBoxConnected(curBox, prochBox))) {
			if (boxes[prochBox].type == 0) { // MIXED
				boxes[prochBox].isVisited = true;
				if (isBoxDividable(prochBox)) divideBox(prochBox);
			} 
			else if (boxes[prochBox].type == -1) { // FREE
				boxes[prochBox].previous = curBox;
				bool found = findPathIter(prochBox);
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
			if (boxes[nextRandomUnvisitedConnectedBoxCounter - 1].type != 1) {
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

int getNextClosestUnvisitedConnectedBox(int boxID) {
	// NOT OPTIMIZED
	int id = -1;
	float lowestDist = MAXFLOAT;
	for (int i = 0; i < boxCounter; i++) {
		if ((boxes[i].isActive) && (!boxes[i].isVisited) && (isBoxConnected(boxID, i))) {
			float dist = sqr(boxes[i].x - xEnd) + sqr(boxes[i].y - yEnd) + sqr(boxes[i].z - zEnd);
			if (dist < lowestDist) {
				lowestDist = dist;
				id = i;
			}
		}
	}
	return id;
}

bool findPathIterRand(int curBox) {

	if (isInBox(curBox, xEnd, yEnd, zEnd)) {
		sinkBox = curBox; 
		return true;
	}
	boxes[curBox].isVisited = true;
	int prochBox;
	do {

		prochBox = getNextRandomUnvisitedConnectedBox(curBox);
		
		if (boxes[prochBox].type == 0) {
			boxes[prochBox].isVisited = true;
			if (isBoxDividable(prochBox)) divideBox(prochBox);
		} else if (boxes[prochBox].type == -1) {
			boxes[prochBox].previous = curBox;
			bool found = findPathIter(prochBox);
			if (found) return found;
		}

	} while (prochBox != -1);

	return false;

}

bool findPathIterDijk(int curBox) {

	if (isInBox(curBox, xEnd, yEnd, zEnd)) {
		sinkBox = curBox; 
		return true;
	}
	boxes[curBox].isVisited = true;
	int prochBox;
	do {

		prochBox = getNextClosestUnvisitedConnectedBox(curBox);
		
		if (boxes[prochBox].type == 0) {
			boxes[prochBox].isVisited = true;
			if (isBoxDividable(prochBox)) divideBox(prochBox);
		} else if (boxes[prochBox].type == -1) {
			boxes[prochBox].previous = curBox;
			bool found = findPathIter(prochBox);
			if (found) return found;
		}

	} while (prochBox != -1);

	return false;

}


void findPath() {
	// Init
	boxCounter = 1;
	boxes[0].x = 0;
	boxes[0].y = 0;
	boxes[0].z = 0;
	boxes[0].isActive = true;
	boxes[0].type = 0;
	boxes[0].radius = maxRadius;
	boxes[0].isVisited = true;
	sourceBox = 0;
	sinkBox = 0;
	randomUnvisitedConnectedBoxCounter = 0;
	nextRandomUnvisitedConnectedBoxCounter = 2;
	// Divide boxes in the starting loc
	while (boxes[sourceBox].type == 0) {
		divideBox(sourceBox);
		sourceBox = getSourceBox();
	}
	if (isRand) {
		found = findPathIterRand(sourceBox);
	}
	else if (isDijk) {
		found = findPathIterDijk(sourceBox);
	}
	else found = findPathIter(sourceBox);
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
		faces[obstacleCounter].coord[0].x = x1;
		faces[obstacleCounter].coord[1].x = x2;
		faces[obstacleCounter].coord[2].x = x3;
		faces[obstacleCounter].coord[0].y = y1;
		faces[obstacleCounter].coord[1].y = y2;
		faces[obstacleCounter].coord[2].y = y3;
		faces[obstacleCounter].coord[0].z = z1;
		faces[obstacleCounter].coord[1].z = z2;
		faces[obstacleCounter].coord[2].z = z3;
		faces[obstacleCounter].a = y1 * z2 - y1 * z3 - y2 * z1 + y2 * z3 + y3 * z1 - y3 * z2;
		faces[obstacleCounter].b = -x1 * z2 + x1 * z3 + x2 * z1 - x2 * z3 - x3 * z1 + x3 * z2;
		faces[obstacleCounter].c = x1 * y2 - x1 * y3 - x2 * y1 + x2 * y3 + x3 * y1 - x3 * y2;
		faces[obstacleCounter].d = x1 * y2 * z3 - x1 * y3 * z2 - x2 * y1 * z3 + x2 * y3 * z1 + x3 * y1 * z2 - x3 * y2 * z1;
		obstacleCounter++;
	}

	std::ifstream jFile(fileName + "sphereloc.txt");
	jFile>>sphereRadius>>xStart>>yStart>>zStart>>xEnd>>yEnd>>zEnd;

	inputString = "";

}

void runCommand(std::string s) {

	bool q = false;

	if (std::regex_match(s, std::regex("run( s(eq(uential)?)?)?"))) {
		q = true;
		isRand = false;
		isDijk = false;
		findPath();
		if (found) { inputString = "Path found.";} else { inputString = "No path found.";}
	}
	if (std::regex_match(s, std::regex("run r(and(om)?)?"))) {
		q = true;
		isRand = true;
		isDijk = false;
		findPath();
		if (found) { inputString = "Path found.";} else { inputString = "No path found.";}
	}
	if (std::regex_match(s, std::regex("run l(owd(istance)?)?"))) {
		q = true;
		isRand = false;
		isDijk = true;
		findPath();
		if (found) { inputString = "Path found.";} else { inputString = "No path found.";}
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