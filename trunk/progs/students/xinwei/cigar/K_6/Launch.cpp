#include <stdio.h>
#include <iostream>
#include <fstream>
#include <regex>

#ifdef __CYGWIN32__
#include "glui.h"
#endif
#ifdef _WIN32
#include <gl/glui.h>
#endif
#ifdef __APPLE__
#include "glui.h"
#endif

// Display Contants

#define MAXFLOAT_t 100000;

#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4
#define ESC 27
#define BACKSPACE 8
#define ENTER 13

float c=3.1415926/180.0f; // transform from angle to radian
float r=160.0f;

GLfloat Vx = 0.0, Vy = 1.0, Vz = 0.0; //View-up vector.

// Data struct and preset variables

const int MAXINT_t = 100000;
bool isExist;

struct Coord {
	float x, y, z;
};

Coord coord[MAXINT_t]; 

struct Face {
    Coord coord[3];
};

Face faces[MAXINT_t];

std::string fileName = "objectModel.txt";
int facesCounter;
float theta, phi;
float sphereWindow_old_y = -1.0 , sphereWindow_old_x = -1.0;
float d_a = 90.0, d_b = 0.0;

float radius, length, platfRadius = 100;

void drawScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();

	GLfloat vwx = r*cos(c*d_b)*cos(c*d_a), vwy = r*sin(c*d_b), vwz =  r*cos(c*d_b)*sin(c*d_a);
    if ( (int)abs(d_b)%360 >= 90 && (int)abs(d_b)%360 <= 270)
	Vy = -1.0;
    else
	Vy = 1.0;
    gluLookAt(vwx, vwy, vwz, 0.0 , 0.0, 0.0 , Vx, Vy, Vz); 

	glPolygonMode (GL_FRONT, GL_FILL);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Draw the axises

	glColor4f(0.2, 0.2, 1.0, 1.0);
	glBegin(GL_LINES);
	{
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(75.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 75.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 75.0);
	}
	glEnd();

	glPushMatrix();
	glTranslatef(80.0, -5.0, 0.0);
	glRasterPos3f(0.0, 0.0, 0.0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'X');
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.0, 80.0, 0.0);
	glRasterPos3f(0.0, 0.0, 0.0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'Y');
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.0, 0.0, 80.0);
	glRasterPos3f(0.0, 0.0, 0.0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'Z');
	glPopMatrix();

	// Rotation

	glRotatef(theta, 1.0f, 0.0f, 0.0f);
	glRotatef(phi, 0.0f, 0.0f, 1.0f);

	// Draw the plane

	/*glColor4f(0.2, 0.9, 0.2, 0.6);
	glPushMatrix();
	glTranslatef(0, length / 2, 0);
	glBegin(GL_QUADS);
	{
		glVertex3f(-platfRadius, 0.0f, -platfRadius);
		glVertex3f(-platfRadius, 0.0f, platfRadius);
		glVertex3f(platfRadius, 0.0f, platfRadius);
		glVertex3f(platfRadius, 0.0f, -platfRadius);
	}
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, - length / 2, 0);
	glBegin(GL_QUADS);
	{
		glVertex3f(-platfRadius, 0.0f, -platfRadius);
		glVertex3f(-platfRadius, 0.0f, platfRadius);
		glVertex3f(platfRadius, 0.0f, platfRadius);
		glVertex3f(platfRadius, 0.0f, -platfRadius);
	}
	glEnd();
	glPopMatrix();*/

	// Draw the cigar

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	GLUquadric *quad = gluNewQuadric();
	glPushMatrix();
	glTranslatef(0, length / 2, 0);
	gluSphere(quad, radius, 25, 25);
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, -length / 2);
	gluCylinder(quad, radius, radius, length, 25, 25);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -length / 2, 0);
	gluSphere(quad, radius, 25, 25);
	glPopMatrix();

}

void sphere_onMouseMove(int x,int y) 
{
    d_a += 0.5f*(x - sphereWindow_old_x); 
    d_b += 0.5f*(y - sphereWindow_old_y); 
    sphereWindow_old_x = x;
    sphereWindow_old_y = y; 
}

void onMouseStatusChange(int button, int state, int x, int y) {
	 if(state == GLUT_DOWN) {
		sphereWindow_old_x = x;
		sphereWindow_old_y = y;
	}
}

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

void reshape(int w,int h)
{
    glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(75.0f, (float)w/h, 1.0f, 1000.0f);
    glMatrixMode( GL_MODELVIEW );
}

void progQuit() {
	exit(0);
}

void resetScene() {
	theta = 0;
	phi = 0;
	radius = 15;
	length = 45;
}

int main(int argc, char *argv[]) {
	
	initFromFile(fileName);

	resetScene();

	glutInit(&argc, argv);

	int windowID = glutCreateWindow("");
	glutPositionWindow(250,200);
	glutReshapeFunc(reshape);
	glutDisplayFunc(drawScene);
	//glutIdleFunc(drawScene);
	glutMouseFunc(onMouseStatusChange);
	glutMotionFunc(sphere_onMouseMove);

	// GLUI

	GLUI_Master.set_glutIdleFunc( drawScene );
	GLUI *glui = GLUI_Master.create_glui( "", 0, 50, 200);
	
	//editText = glui->add_edittext( "Input:", GLUI_EDITTEXT_TEXT );
	//editText->set_text((char*)fileName.c_str());
	//glui->add_separator();
	//GLUI_Button* buttonRun = glui->add_button( "Run", -1, (GLUI_Update_CB)update_GLUI_Variables);
	GLUI_Button* buttonReset = glui->add_button( "Reset", -1, (GLUI_Update_CB)resetScene);
	GLUI_Button* buttonExit = glui->add_button( "Exit", -1, (GLUI_Update_CB)progQuit);

	glui->set_main_gfx_window( windowID );
	//update_GLUI_Variables();

	glutMainLoop();

	return 0;

}