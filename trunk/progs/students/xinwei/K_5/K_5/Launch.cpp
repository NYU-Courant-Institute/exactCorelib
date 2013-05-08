#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

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

// Variables Specifics (Mainly visually)

#define windowPosX 100
#define windowPosY 100
#define boxWidth 300
#define boxHeight 300

#define MAXINT 10000

const float squareRadius = 25;
const float squareX[6] = {100, 200, 100, 200, 100, 200};
const float squareY[6] = {50, 50, 150, 150, 250, 250};
const float squareTextX[6] = {100, 200, 100, 200, 100, 200};
const float squareTextY[6] = {10, 10, 110, 110, 210, 210};
const std::string squareText[6] = {"(x, y, 1)", "(x, y, -1)", "(x, 1, z)", "(x, -1, z)", "(1, y, z)", "(-1, y, z)"};

// GLUI Controls

GLUI_EditText* editText;

// Core Variables

std::string fileName = "objectModel.txt";
bool isExist;

float actualTheta = 90.0, actualPhi = 0.0;
float objectWindow_old_y = -1.0 , objectWindow_old_x = -1.0;
float sphere_d_a = 90.0, sphere_d_b = 0.0;
float sphereWindow_old_y = -1.0 , sphereWindow_old_x = -1.0;
float object_d_a = 90.0, object_d_b = 0.0;
float cubeWindow_old_x, cubeWindow_old_y;
int crType;

int facesCounter; // Total faces of the object

struct Coord {
	float x, y, z;
};

Coord coord[MAXINT]; 

struct Face {
    Coord coord[3];
};

Face faces[MAXINT];

float actualX = 1.0, actualY = 0.0, actualZ = 0.0;

// Trad funcs

void reshape(int w,int h)
{
    glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(75.0f, (float)w/h, 1.0f, 1000.0f);
    glMatrixMode( GL_MODELVIEW );
}

// Conversion functions; mapping (theta, phi) -> (x, y, z) or other way around

void recalcXYZ() {

	if (actualPhi >= 45) { // Belongs to plane (x, 1, z)
		float distToCenter = tan(c * (90 - actualPhi));
		actualX = sin(c * actualTheta) * distToCenter;
		actualY = 1;
		actualZ = cos(c * actualTheta) * distToCenter;
	}
	else if (actualPhi <= -45) { // Belongs to plane (x, -1, z)
		float distToCenter = tan(c * (90 + actualPhi));
		actualX = sin(c * actualTheta) * distToCenter;
		actualY = -1;
		actualZ = cos(c * actualTheta) * distToCenter;
	}
	else if ((45 <= actualTheta) && (actualTheta <= 135)) { // Belongs to (1, y, z)
		float distToCenter = tan(c * actualPhi);
		actualX = 1;
		actualY = distToCenter;
		actualZ = cos(c * actualTheta);
	}
	else if ((135 < actualTheta) && (actualTheta <= 225)) { // Belongs to (x, y, -1)
		float distToCenter = tan(c * actualPhi);
		actualX = sin(c * actualTheta);
		actualY = distToCenter;
		actualZ = -1;
	}
	else if ((225 < actualTheta) && (actualTheta <= 315)) { // Belongs to (-1, y, z)
		float distToCenter = tan(c * actualPhi);
		actualX = -1;
		actualY = distToCenter;
		actualZ = cos(c * actualTheta);
	}
	else { // Belongs to (x, y, 1)
		float distToCenter = tan(c * actualPhi);
		actualX = sin(c * actualTheta);
		actualY = distToCenter;
		actualZ = 1;
	}

}

void recalcDP() { // Derived from recalcXYZ();

	if ((actualX == 1) || (actualX == -1)) {
		actualTheta = acos(actualZ) / c;
		actualPhi = atan(actualY) / c;
	}
	else if ((actualZ == 1) || (actualZ == -1)) {
		actualTheta = asin(actualX) / c;
		actualPhi = atan(actualY) / c;
	}
	else if ((actualY == 1) || (actualY == -1)) {

		if (actualZ == 0) {
			actualTheta = 0;
		}
		else {
			actualTheta = atan(actualX / actualZ) / c;
			if (actualZ < 0) {
				actualTheta += 180;
			}
			actualPhi = actualX / sin(c * actualTheta) / c;
		}

		if (actualY == -1) {
			actualPhi -= 90;
		}
		else {
			actualPhi = 90 - actualPhi;
		}
	}

}

// Section: Cube

void drawCubeScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPointSize(3.0);

	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,300,0,300);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();					
    glLoadIdentity();					

    glColor3f(1.0f,1.0f,1.0f);
	
	// Draw squares

	for (int i = 0; i < 6; i++) {

		glColor3f(1.0f,1.0f,1.0f);
		glBegin(GL_QUADS);
		{
			glVertex2f(squareX[i] + squareRadius, squareY[i] + squareRadius);
			glVertex2f(squareX[i] - squareRadius, squareY[i] + squareRadius);
			glVertex2f(squareX[i] - squareRadius, squareY[i] - squareRadius);
			glVertex2f(squareX[i] + squareRadius, squareY[i] - squareRadius);
		}
		glEnd();

		/*glColor3f(0.2f, 1.0f, 0.2f);
		glBegin(GL_LINES);
		{
			glVertex2f(squareX[i], squareY[i]);
			glVertex2f(squareX[i] + 2/3 * squareRadius, squareY[i] + 2/3 * squareRadius);
		}*/

		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glRasterPos2f(squareTextX[i], squareTextY[i]);
		for (int j = 0; j < squareText[i].length(); j++) 
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, squareText[i][j]);
		glPopMatrix();

	}

	// Draw rotation point

	glColor3f(1.0, 0.2, 0.2);

	glPushMatrix();
	if (actualX == 1) {
		glBegin(GL_POINTS);
		glVertex2f(squareX[4] + actualZ * squareRadius, squareY[4] + actualY * squareRadius);
		glEnd();
	}
	if (actualX == -1) {
		glBegin(GL_POINTS);
		glVertex2f(squareX[5] + actualZ * squareRadius, squareY[5] + actualY * squareRadius);
		glEnd();
	}
	if (actualY == 1) {
		glBegin(GL_POINTS);
		glVertex2f(squareX[2] + actualX * squareRadius, squareY[2] + actualZ * squareRadius);
		glEnd();
	}
	if (actualY == -1) {
		glBegin(GL_POINTS);
		glVertex2f(squareX[3] + actualX * squareRadius, squareY[3] + actualZ * squareRadius);
		glEnd();
	}
	if (actualZ == 1) {
		glBegin(GL_POINTS);
		glVertex2f(squareX[0] + actualX * squareRadius, squareY[0] + actualY * squareRadius);
		glEnd();
	}
	if (actualZ == -1) {
		glBegin(GL_POINTS);
		glVertex2f(squareX[1] + actualX * squareRadius, squareY[1] + actualY * squareRadius);
		glEnd();
	}
	glPopMatrix();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
    
	glutPostRedisplay();
    glFlush();
    glutSwapBuffers();

}

void cube_onMouseStatusChange(int button, int state, int x, int y) //
{
	float distX, distY;
	crType = -1;
	y = 300 - y;

    if(state == GLUT_DOWN) {
		for (int i = 0; i < 6; i++) {
			if ((abs(x - squareX[i]) <= squareRadius) && (abs(y - squareY[i]) <= squareRadius)) {
				crType = i;
				distX = (x - squareX[i]) / squareRadius;
				distY = (y - squareY[i]) / squareRadius;
			}
		}

		switch (crType) {
		case 0:
			actualZ = 1;
			actualX = distX;
			actualY = distY;
			break;
		case 1:
			actualZ = -1;
			actualX = distX;
			actualY = distY;
			break;
		case 2:
			actualY = 1;
			actualX = distX;
			actualZ = distY;
			break;
		case 3:
			actualY = -1;
			actualX = distX;
			actualZ = distY;
			break;
		case 4:
			actualX = 1;
			actualZ = distX;
			actualY = distY;
			break;
		case 5:
			actualX = -1;
			actualZ = distX;
			actualY = distY;
			break;
		}
		
		recalcDP();

	}
}

/*void cube_onMouseMove(int x,int y) 
{

	if (x > squareX[crType] + squareRadius) {
		x = squareX[crType] + squareRadius;
	}
	if (x < squareX[crType] - squareRadius) {
		x = squareX[crType] - squareRadius;
	}
	if (y > squareY[crType] + squareRadius) {
		y = squareY[crType] + squareRadius;
	}
	if (y < squareY[crType] - squareRadius) {
		y = squareY[crType] - squareRadius;
	}

	float distX = x - cubeWindow_old_x;
	float distY = y - cubeWindow_old_x;
	cubeWindow_old_x = x;
	cubeWindow_old_y = y;

	if ((crType == 4) || (crType == 5)) { // x == 1 or -1
		actualY = y / squareRadius;
		actualZ = x / squareRadius;
	}
   
	recalcDP();
	
}*/

// Section: Sphere

void drawSphereScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();

	GLfloat vwx = r*cos(c*sphere_d_b)*cos(c*sphere_d_a), vwy = r*sin(c*sphere_d_b), vwz =  r*cos(c*sphere_d_b)*sin(c*sphere_d_a);
    if ( (int)abs(sphere_d_b)%360 >= 90 && (int)abs(sphere_d_b)%360 <= 270)
	Vy = -1.0;
    else
	Vy = 1.0;
    gluLookAt(vwx, vwy, vwz, 0.0 , 0.0, 0.0 , Vx, Vy, Vz); 

	glPolygonMode (GL_FRONT, GL_FILL);

	GLUquadric *quad = gluNewQuadric();
    
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glMatrixMode(GL_MODELVIEW);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	// Draw the location sphere

	glPushMatrix();
	float dy = 50 * sin(c * actualPhi), dxpre = 50 * cos(c * actualPhi);
	float dx = dxpre * sin(c * actualTheta), dz = dxpre * cos(c * actualTheta);
	glTranslatef(dx, dy, dz);
	glColor4f(1.0, 0.2, 0.2, 1.0);
	gluSphere(quad, 2, 5, 5);
	glPopMatrix();

	// Draw the outer sphere

	glColor4f(1.0, 1.0, 1.0, 0.5);
	gluSphere(quad, 50, 40, 40);

	// Draw the x-z plane

	glColor4f(0.2, 0.9, 0.2, 0.2);
	glBegin(GL_QUADS);
	{
		glVertex3f(650.0, 0.1, 650.0);
		glVertex3f(650.0, 0.0, -650.0);
		glVertex3f(-650.0, 0.0, -650.0);
		glVertex3f(-650.0, 0.0, 650.0);
	}
	glEnd();

	glutPostRedisplay();
    glFlush();
    glutSwapBuffers();

}

void sphere_onMouseStatusChange(int button, int state, int x, int y) //
{
    if(state == GLUT_DOWN) {
		sphereWindow_old_x = x;
		sphereWindow_old_y = y;
	}
}

void sphere_onMouseMove(int x,int y) 
{
    sphere_d_a += 0.5f*(x - sphereWindow_old_x); 
    sphere_d_b += 0.5f*(y - sphereWindow_old_y); 
    sphereWindow_old_x = x;
    sphereWindow_old_y = y; 
}

// Section: Object

void drawObjectScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();

	GLfloat vwx = r*cos(c*object_d_b)*cos(c*object_d_a), vwy = r*sin(c*object_d_b), vwz =  r*cos(c*object_d_b)*sin(c*object_d_a);
    if ( (int)abs(object_d_b)%360 >= 90 && (int)abs(object_d_b)%360 <= 270)
	Vy = -1.0;
    else
	Vy = 1.0;
    gluLookAt(vwx, vwy, vwz, 0.0 , 0.0, 0.0 , Vx, Vy, Vz); 

	glPolygonMode (GL_FRONT, GL_FILL);
    
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glMatrixMode(GL_MODELVIEW);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw the axises

	glColor4f(0.2, 0.2, 1.0, 1.0);
	glBegin(GL_LINES);
	{
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(75.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 75.0, 0.0);
	}
	glEnd();

	glPushMatrix();
	glTranslatef(80.0, -5.0, 0.0);
	glRasterPos3f(0.0, 0.0, 0.0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'T');
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.0, 80.0, 0.0);
	glRasterPos3f(0.0, 0.0, 0.0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'P');
	glPopMatrix();

	// Rotate first
	
	glPushMatrix();
	glRotatef(actualTheta, 1.0, 0.0, 0.0);
	glRotatef(actualPhi, 0.0, 1.0, 0.0);

	// Draw the actual object (plane?)

	for (int i = 0; i < facesCounter; i++) {
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

	glPopMatrix();

	// Bottom Display Text

	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,300,0,300);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();					
    glLoadIdentity();					

    glColor3f(1.0f,1.0f,1.0f);

	if (isExist) {

		glRasterPos2f(0, 0);

		std::ostringstream ss1, ss2;
		ss1 << actualTheta;
		ss2 << actualPhi;
	
		std::string infoString = "Angles: <" +  ss1.str() + ", " + ss2.str() + ">";

		for (int i = 0; i < infoString.length(); i++) 
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, infoString[i]);

		glRasterPos2f(0, 20);

		std::ostringstream ss3, ss4, ss5;
		ss3 << actualX;
		ss4 << actualY;
		ss5 << actualZ;

		std::string infoString2 = "RV: <" + ss3.str() + ", " + ss4.str() + ", " + ss5.str() + ">";

		for (int i = 0; i < infoString2.length(); i++) 
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, infoString2[i]);
	}
	else {

		glRasterPos2f(0, 0);
	
		std::string infoString = "File does not exist or file is blank.";

		for (int i = 0; i < infoString.length(); i++) 
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, infoString[i]);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
    
	glutPostRedisplay();
    glFlush();
    glutSwapBuffers();

}

void object_onMouseStatusChange(int button, int state, int x, int y) //
{
    if(state == GLUT_DOWN) {
		objectWindow_old_x = x;
		objectWindow_old_y = y;
	}
}

void object_onMouseMove(int x,int y) 
{
    object_d_a += 0.5f*(x - objectWindow_old_x); 
    object_d_b += 0.5f*(y - objectWindow_old_y); 
    objectWindow_old_x = x;
    objectWindow_old_y = y; 
	
}

void object_onKeyPress (GLubyte moveKey, GLint xMouse, GLint yMouse) {

	switch (moveKey) {
	case 'w': actualPhi += 2; break;
	case 's': actualPhi -= 2; break;
	case 'a': actualTheta += 2; break;
	case 'd': actualTheta -= 2; break;
	}

	while (actualPhi > 180) {
		actualPhi -= 360;
	}
	while (actualPhi < -180) {
		actualPhi += 360;
	}
	if (actualPhi > 90) {
		actualPhi = 180 - actualPhi;
		actualTheta += 180;
	}
	if (actualPhi < -90) {
		actualPhi = -180 - actualPhi;
		actualTheta += 180;
	}
	while (actualTheta > 360) {
		actualTheta -= 360;
	}
	while (actualTheta < 0) {
		actualTheta += 360;
	}

	recalcXYZ();

}

// Init

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

	// Reset
	
	actualTheta = 90.0, actualPhi = 0.0;
	object_d_a = 90.0, object_d_b = 0.0;
	objectWindow_old_y = -1.0 , objectWindow_old_x = -1.0;
	sphere_d_a = 55.0, sphere_d_b = 25.0;
	sphereWindow_old_y = -1.0 , sphereWindow_old_x = -1.0;
	actualX = 1.0, actualY = 0.0, actualZ = 0.0;

}

// GLUI Callback

void update_GLUI_Variables() {

	fileName = editText->get_text();
	initFromFile(fileName);

}

void resetScene() {

	initFromFile(fileName);

}

int main(int argc, char *argv[]) {

	initFromFile(fileName);

	glutInit(&argc, argv);
	//glutInitWindowPosition(windowPosX, windowPosY);
	glutInitWindowSize(boxWidth, boxHeight);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	// View: Cube in CSpace

	int windowCubeID = glutCreateWindow("");
	glutPositionWindow(250,200);
	glutReshapeFunc(reshape);
	glutDisplayFunc(drawCubeScene);
	glutIdleFunc(drawCubeScene);
	glutMouseFunc(cube_onMouseStatusChange);
    //glutMotionFunc(cube_onMouseMove);

	// View: Sphere in CSpace

	glutCreateWindow("");
	glutPositionWindow(600,200);
	glutReshapeFunc(reshape);
	glutDisplayFunc(drawSphereScene);
	glutIdleFunc(drawSphereScene);
	glutMouseFunc(sphere_onMouseStatusChange);
    glutMotionFunc(sphere_onMouseMove);

	// View: Object in Physcial Space

	glutCreateWindow("");
	glutPositionWindow(950,200);
	glutReshapeFunc(reshape);
	glutDisplayFunc(drawObjectScene);
	glutIdleFunc(drawObjectScene);
	glutMouseFunc(object_onMouseStatusChange);
    glutMotionFunc(object_onMouseMove);
	glutKeyboardFunc(object_onKeyPress);

	// GLUI

	GLUI_Master.set_glutIdleFunc( NULL );
	GLUI *glui = GLUI_Master.create_glui( "", 0, 50, 200);
	
	editText = glui->add_edittext( "Input:", GLUI_EDITTEXT_TEXT );
	editText->set_text((char*)fileName.c_str());
	glui->add_separator();
	GLUI_Button* buttonRun = glui->add_button( "Run", -1, (GLUI_Update_CB)update_GLUI_Variables);
	GLUI_Button* buttonReset = glui->add_button( "Reset", -1, (GLUI_Update_CB)resetScene);

	glui->set_main_gfx_window( windowCubeID );
	update_GLUI_Variables();

	glutMainLoop();

}