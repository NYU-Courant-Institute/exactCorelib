#include <stdio.h>
#include <iostream>
#include <fstream>

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

// Display Contants

#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4
#define ESC 27
#define BACKSPACE 8
#define ENTER 13

float c=3.1415926/180.0f; // transform from angle to radian
float r=160.0f;

GLfloat Vx = 0.0, Vy = 1.0, Vz = 0.0; //View-up vector.

// Data struct and preset variables

const int MAXINT = 100000;

struct Coord {
	float x, y, z;
};

Coord coord[MAXINT]; 

struct Face {
    Coord coord[3];
};

Face faces[MAXINT];

std::string fileName = "objectModel.txt";
bool isExist;
int facesCounter;

void initFromFile(std::string fileName) {

	facesCounter = 0;

	float x1, x2, x3, y1, y2, y3, z1, z2, z3;
	
	std::ifstream iFile(fileName);

	isExist = false;

	while (iFile>>x1>>y1>>z1>>x2>>y2>>z2>>x3>>y3>>z3) {
		faces[facesCounter].coord[0].x = x1;
		faces[facesCounter].coord[1].x = x2;
		faces[facesCounter].coord[2].x = x3;
		faces[facesCounter].coord[0].y = y1;
		faces[facesCounter].coord[1].y = y2;
		faces[facesCounter].coord[2].y = y3;
		faces[facesCounter].coord[0].z = z1;
		faces[facesCounter].coord[1].z = z2;
		faces[facesCounter].coord[2].z = z3;
		facesCounter ++;
		isExist = true;
	}

}

int main(int argc, char *argv[]) {
	
	initFromFile(fileName);

	glutInit(&argc, argv);

	int windowID = glutCreateWindow("");

	return 0;

}