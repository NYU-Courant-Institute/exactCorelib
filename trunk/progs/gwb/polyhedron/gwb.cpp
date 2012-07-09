/* file: gwb.cpp
 *
 * Author: Kai Cao (Supervised by Professor Yap, July 2012)
 *
 * Since Core 2.1.
 * 
 ***************************************************/


#include "Euler_Ops.h"
#include<math.h>

/***************************************************
 * CONSTANTS:
 ***************************************************/
#define SQRTWO sqrt(2)
#define SQRTFIVE sqrt(5)
#define fi ((1+SQRTFIVE)/2)
#define WIRE
#define DODECAHEDRON

//#################################################
// PARAMETERS:
//#################################################
int interacive=0;                   // mode of interaction
                                    //    =0 means non-interactive, >0 means interactive.
string inputDir("inputs"); 		// Path for input files 
string fileName("cube.txt"); 	      // Input file name
windowWidth = 512		            // initial configuration box size
windowHt = 512
windowPosX = 200	# initial Window position
windowPosY = 200	

GLsizei ww=500;
GLsizei wh=500;
double startX=0;
double startY=0;
double startZ=0;
double  base=0.0;
double theta=0.0;
double rad=0.0;


void pressMouse(int button,int state,int x,int y);
void holdMouse(int x,int y);
Euler_Ops *eo;

##################################################
# GUI STUFF:
##################################################
void MyInit(void){
  glClearColor(1,1,1,0);
  glColor3f(1,0,0);
  glPointSize(10);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluOrtho2D(0,(GLdouble)ww,0.0,(GLdouble)wh);
  glOrtho(-100.0,(GLdouble)ww,-100.0,(GLdouble)wh,-1000,1000);
}//MyInit

void display(void){
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  /*Get Edges and Faces*/
  Solid *s=(*(eo->solids))[0]; 
  Vec<Edge *> *es=s->sedges;
  Vec<Face *> *fs=s->sfaces;
  glRotatef(45,1,1,1);

  /*Draw different Solids*/
  #ifdef CUBEWITHAHOLE
  glScalef(100,100,100);
  #elif defined(CUBE)
  glScalef(300,300,300);
  #elif defined(TETRAHEDRON)
  glScalef(300,300,300);
  #elif defined(OCTAHEDRON)
  glScalef(200,200,200);
  glTranslatef(1,1,1);
  #elif defined (DODECAHEDRON)
  glScalef(200,200,200);
  glTranslatef(1,1,1);
  #elif defined(ICOSAHEDRON)
  glScalef(150,150,150);
  glTranslatef(1,1,1);
  #else
  glScalef(100,100,100);
  glTranslatef(1,1,1);
  #endif

  /*Wire Solids*/
  #ifdef WIRE

  glBegin(GL_LINES);
  for (int i=0;i<es->size();i++){
    Edge *e=(*es)[i];
    Vertex *v1=e->he1->start;
    Vertex *v2=e->he2->start;
   glVertex3f(v1->getX(),v1->getY(),v1->getZ());
   glVertex3f(v2->getX(),v2->getY(),v2->getZ());

  }
  glEnd();
  
  #elif defined(SURFACE)
  for (int i=0;i<fs->size();i++){
    glBegin(GL_POLYGON);
    Face *e=(*es)[i];
    Vertex *v1=e->he1->start;
    Vertex *v2=e->he2->start;
    glVertex3f(v1->getX(),v1->getY(),v1->getZ());
    glVertex3f(v2->getX(),v2->getY(),v2->getZ());
    glEnd();

  }

  #endif


   glFlush();

}//display

void pressMouse(int button,int state,int x ,int y){
if (state==GLUT_DOWN){
  startX=x;
  startY=wh-y;
  }
 if(state==GLUT_UP){
  holdMouse(x,y);
  base=rad;
 }
}//pressMouse

void holdMouse(int x, int y){
    /*glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBegin(GL_POINTS);
    glPointSize(50);
    glVertex2i(x,wh-y);
    glEnd();
    glFlush();*/

   Solid *s=(*(eo->solids))[0]; 
   vector<Edge *> *es=s->sedges;
   double centerX=(*(s->center()))[0];
   double  centerY=(*(s->center()))[1];
   double  centerZ=(*(s->center()))[2];
   //cout<<"CENTERx="<<centerX<<endl;
   //cout<<"CENTERy="<<centerY<<endl;
   y=wh-y;
   double radius=sqrt((startX-centerX)*(startX-centerX)+(startY-centerY)*(startY-centerY)+(startZ-centerZ)*(startZ-centerZ));
   double distance=sqrt((x-startX)*(x-startX)+(y-startY)*(y-startY));
   theta=acos((2.0*radius*radius-distance*distance)/(2.0*radius*radius))/PI*180.0;
   double rotX=(startY-centerY)*(0-centerZ)-(startZ-centerZ)*(y-centerY);
   double rotY=(startZ-centerZ)*(x-centerX)-(startX-centerX)*(0-centerZ);
   double rotZ=(startX-centerX)*(y-centerY)-(startY-centerY)*(x-centerX);
   //int direction=(startX-centerX)*(y-centerY)-(startY-centerY)*(y-centerY)>0?1:-1;

   glClear(GL_COLOR_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glRotatef(45,1,1,1);
  #ifdef CUBEWITHAHOLE
  glScalef(100,100,100);
  #elif defined(CUBE)
  glScalef(300,300,300);
  #elif defined(TETRAHEDRON)
  glScalef(300,300,300);
  #elif defined(OCTAHEDRON)
  glScalef(200,200,200);
  glTranslatef(1,1,1);
  #elif defined (DODECAHEDRON)
  glScalef(200,200,200);
  glTranslatef(1,1,1);
  #elif defined(ICOSAHEDRON)
  glScalef(150,150,150);
  glTranslatef(1,1,1);
  #else
  glScalef(100,100,100);
  glTranslatef(1,1,1);
  #endif
   //glTranslatef(x-startX,y-startY,0);
   glTranslatef(centerX,centerY,centerZ);
   //rad=base+theta*direction;
   glRotatef(theta,rotX,rotY,rotZ);
   glTranslatef(-centerX,-centerY,-centerZ);
   /*glBegin(GL_LINE_LOOP);
   glVertex2f(150,150);
   glVertex2f(350,150);
   glVertex2f(350,350);
   glVertex2f(150,350);
   glEnd();
   
   glBegin(GL_POINTS);
   glPointSize(50);
   glVertex2f(250,250);
   glEnd();
   glFlush();*/

  glBegin(GL_LINES);
  for (int i=0;i<es->size();i++){
    Edge *e=(*es)[i];
    Vertex *v1=e->he1->start;
    Vertex *v2=e->he2->start;
   glVertex3f(v1->getX(),v1->getY(),v1->getZ());
   glVertex3f(v2->getX(),v2->getY(),v2->getZ());

  }
   glEnd();
   glBegin(GL_POINTS);
   glPointSize(50);
   glVertex3f(centerX,centerY,centerZ);
   glEnd();
   glFlush();
  
}//holdMouse

##################################################
# MAIN:
##################################################

int main(int argc,char **argv){

	if (argc > 1) interactive = atoi(argv[1]);	// Interactive (0) or no (>0)
	if (argc > 2) inputDir  = argv[2];		      // path for input files
	if (argc > 3) fileName = argv[3]; 		      // Input file name
	if (argc > 4) windowWidth = atof(argv[4]);		// windowWidth
	if (argc > 5) windowHeight = atof(argv[5]);	      // windowHeight
	if (argc > 6) windowPosX = atoi(argv[6]);	      // window X pos
	if (argc > 7) windowPosY = atoi(argv[7]);	      // window Y pos

  eo=new Euler_Ops();
  Vec<Solid *> *ss=eo->solids;
  #ifdef CUBE
	  Solid *cube=eo->mvfs(1,1,1,0,0,0);
	  eo->mev(1,1,1,1,1,1,2,1,0,0);
	  eo->mev(1,1,1,1,2,2,3,0,1,0);
	  eo->mev(1,1,1,3,1,1,4,1,1,0);
	  eo->mef(1,1,2,1,4,3,2);
	  eo->mev(1,2,2,1,3,3,5,0,0,1);
	  eo->mev(1,2,2,2,1,1,6,1,0,1);
	  eo->mev(1,2,2,3,4,4,7,0,1,1);
	  eo->mev(1,2,2,4,2,2,8,1,1,1);
	  eo->mef(1,2,6,2,5,1,3);
	  eo->mef(1,2,5,1,7,3,4);
	  eo->mef(1,2,7,3,8,4,5);
	  eo->mef(1,2,8,4,6,5,6);

  #elif defined(TETRAHEDRON) 

  /*************************************************************************************************************************/
  /*************************************************************************************************************************/
  /****************************************************Tetrahedron*************************************************/

  Solid *tetra=eo->mvfs(2,1,1,0,0,0);
  eo->mev(2,1,1,1,1,1,2,1,0,0);
  eo->mev(2,1,1,1,2,2,3,0,1,0);
  eo->mef(2,1,2,1,3,1,2);
  eo->mev(2,2,2,1,3,3,4,0,0,1);
  eo->mef(2,2,2,1,4,1,3);
  eo->mef(2,2,3,2,4,1,4);
  tetra->showInfo();
  tetra->print();


  for (int i=0;i<ss->size();i++){
    cout<<"*********************************************************************************************************************"<<endl;
    (*ss)[i]->showInfo();
    (*ss)[i]->print();
  }

  #elif defined(CUBEWITHAHOLE)
  
  Solid *holecube=eo->mvfs(1,1,1,0,0,0);
  int check=eo->mev(1,1,1,1,1,1,2,3,0,0);/*Solidno, faceno1,faceno2,v1,v2,v3...newV,coordinates*/
  check=eo->mev(1,1,1,2,1,1,3,3,3,0);
  check=eo->mev(1,1,1,3,2,2,4,0,3,0);
  check=eo->mef(1,1,4,3,1,2,2);
  check=eo->mev(1,2,2,1,4,4,5,0,0,3);/*Solidno, faceno1,faceno2,v1,v2,v3...newV,coordinates*/
  check=eo->mev(1,2,2,2,1,1,6,3,0,3);/*Solidno, faceno1,faceno2,v1,v2,v3...newV,coordinates*/
  check=eo->mev(1,2,2,3,2,2,7,3,3,3);/*Solidno, faceno1,faceno2,v1,v2,v3...newV,coordinates*/
  check=eo->mev(1,2,2,4,3,3,8,0,3,3);/*Solidno, faceno1,faceno2,v1,v2,v3...newV,coordinates*/
  check=eo->mef(1,2,6,2,5,1,3);/*Solidno, oldfaceno,v1,v2,v3,v4,newfaceno*/
  check=eo->mef(1,2,5,1,8,4,4);/*Solidno, oldfaceno,v1,v2,v3,v4,newfaceno*/
  check=eo->mef(1,2,8,4,7,3,5);/*Solidno, oldfaceno,v1,v2,v3,v4,newfaceno*/
  check=eo->mef(1,2,7,3,6,5,6);/*Solidno, oldfaceno,v1,v2,v3,v4,newfaceno*/
  /*Step 14-17 : 4*mev*/
  check=eo->mev(1,1,1,1,2,2,9,1,1,0);
  check=eo->mev(1,1,1,9,1,1,10,2,1,0);
  check=eo->mev(1,1,1,10,9,9,11,2,2,0);
  check=eo->mev(1,1,1,11,10,10,12,1,2,0);

  /*Step 18 : mef*/
  check=eo->mef(1,1,9,10,12,11,7);

  /*Step 19 : kemr*/
  check=eo->kemr(1,1,1,9);

  /*Step 20-23 : 4*mev*/
  check=eo->mev(1,7,9,13,1,1,3);
  check=eo->mev(1,7,10,14,2,1,3);
  check=eo->mev(1,7,11,15,2,2,3);
  check=eo->mev(1,7,12,16,1,2,3);

  /*Step 24-27 : 4*mef*/
  check=eo->mef(1,7,13,9,14,10,8);
  if(check!=SUCCESS)
    cout<<"ERROR step 24"<<endl;
  check=eo->mef(1,7,14,10,15,11,9);
  check=eo->mef(1,7,15,11,16,12,10);
  check=eo->mef(1,7,16,12,13,14,11);

  /*Step 28 : kfmrh */
  check=eo->kfmrh(1,2,7);

  if (check!=SUCCESS)
    cout<<"ERROR"<<endl;
  holecube->print();
  holecube->showInfo();
  #elif defined(OCTAHEDRON)
  Solid *octa=eo->mvfs(4,1,1,1,1,0);
  /*Lemina Square*/
  eo->mev(4,1,1,1,1,1,2,-1,1,0);
  eo->mev(4,1,1,2,1,1,3,-1,-1,0);
  eo->mev(4,1,1,3,2,2,4,1,-1,0);
  /*Face 2 is upper face with Face 1 is lower face*/
  eo->mef(4,1,4,3,1,2,2);

  /*Build upper point*/
  eo->mev(4,2,2,1,4,4,5,0,0,SQRTWO/2);
  /*Build 4 upper triangles*/
  eo->mef(4,2,2,1,5,1,3);
  eo->mef(4,2,3,2,5,1,4);
  eo->mef(4,2,4,3,5,1,5);

  /*Build lower point*/
  eo->mev(4,1,1,1,2,2,6,0,0,-SQRTWO/2);
  /*Build 4 lower triangles*/
  eo->mef(4,1,6,1,2,3,6);
  eo->mef(4,1,6,2,3,4,7);
  eo->mef(4,1,6,3,4,1,8);
  octa->showInfo();
  octa->print();
  
  #elif defined(DODECAHEDRON)
  Solid *dodeca=eo->mvfs(1,1,1,-1,-1,-1);
  eo->mev(1,1,1,1,1,1,2,1,-1,-1);
  eo->mev(1,1,1,2,1,1,3,1,1,-1);
  eo->mev(1,1,1,3,2,2,4,-1,1,-1);
  /*Upper part is 4-3-2-1*/
  eo->mef(1,1,4,3,1,2,2);

  /*Build four danlging edges*/
  eo->mev(1,2,2,1,4,4,5,-1,-1,1);
  eo->mev(1,2,2,2,1,1,6,1,-1,1);
  eo->mev(1,2,2,3,2,2,7,1,1,1);
  eo->mev(1,2,2,4,3,3,8,-1,1,1);

  /*Build four edges */
  eo->mef(1,2,8,4,7,3,5);
  eo->mef(1,2,7,3,6,2,6);
  eo->mef(1,2,6,2,5,1,3);
  eo->mef(1,2,5,1,8,7,4);
  /************************************NOW WE HAVE A CUBE*********************************/
  /*Build other 12 points*/
  /*On face 1 separate it into 4 faces*/
  eo->mev(1,1,1,1,2,2,9,0,-1/fi,-fi);
  eo->mef(1,1,9,1,2,3,7);
  eo->mev(1,1,1,9,2,2,10,0,1/fi,-fi);
  eo->mef(1,1,10,9,3,4,8);
  eo->mef(1,1,10,3,4,1,9);

  /*On face 2 separate it into 4 faces*/
  eo->mev(1,2,2,6,5,5,11,0,-1/fi,fi);
  eo->mef(1,2,11,6,5,8,10);
  eo->mev(1,2,2,11,5,5,12,0,1/fi,fi);
  eo->mef(1,2,12,11,8,7,11);
  eo->mef(1,2,12,8,7,6,12);

  /*On face 3 separate it into 4 faces*/
  eo->mev(1,3,3,1,5,5,13,-1/fi,-fi,0);
  eo->mef(1,3,13,1,5,6,13);
  eo->mev(1,3,3,13,5,5,14,1/fi,-fi,0);
  eo->mef(1,3,14,13,6,2,14);
  eo->mef(1,3,14,6,2,1,15);


  
  /*On face 4 separate it into 4 faces*/
  eo->mev(1,4,4,1,4,4,15,-fi,0,-1/fi);
  eo->mef(1,4,15,1,4,8,16);
  eo->mev(1,4,4,15,4,4,16,-fi,0,1/fi);
  eo->mef(1,4,16,15,8,5,17);
  eo->mef(1,4,16,8,5,1,18);

  /*On face 5 separate it into 4 faces*/
  eo->mev(1,5,5,8,4,4,17,-1/fi,fi,0);
  eo->mef(1,5,17,8,4,3,19);
  eo->mev(1,5,5,17,4,4,18,1/fi,fi,0);
  eo->mef(1,5,18,17,3,7,20);
  eo->mef(1,5,18,3,7,8,21);

  /*On face 6 separate it into 4 faces*/
  eo->mev(1,6,6,3,2,2,19,fi,0,-1/fi);
  eo->mef(1,6,19,3,2,6,22);
  eo->mev(1,6,6,19,2,2,20,fi,0,1/fi);
  eo->mef(1,6,20,19,6,7,23);
  eo->mef(1,6,20,6,7,3,24);

  /************************************NOW WE HAVE 24 faces,42 edges 20 points*********************************/
  /************************************WE NEED 12 KEF ,so Kill the CUBE*********************************/
  eo->kef(1,7,1,2);
  eo->kef(1,8,2,3);
  eo->kef(1,9,3,4);
  eo->kef(1,1,4,1);

  eo->kef(1,14,5,6);
  eo->kef(1,24,6,7);
  eo->kef(1,5,7,8);
  eo->kef(1,18,8,5);

  eo->kef(1,13,1,5);
  eo->kef(1,23,2,6);
  eo->kef(1,21,3,7);
  eo->kef(1,17,4,8);
  

  dodeca->showInfo();
  dodeca->print();

/******************************************ICOSAHEDRON*************************************/
  #elif defined(ICOSAHEDRON)
  /*1st step: build an regular hexagon*/
  Solid * icosa=eo->mvfs(1,1,1,0,-1,-fi);
  eo->mev(1,1,1,1,1,1,2,fi,0,-1);
  eo->mev(1,1,1,2,1,1,3,1,fi,0);
  eo->mev(1,1,1,3,2,2,4,0,1,fi);
  eo->mev(1,1,1,4,3,3,5,-fi,0,1);
  eo->mev(1,1,1,5,4,4,6,-1,-fi,0);
  eo->mef(1,1,6,5,1,2,2);

  /*2nd step: build upper part*/
  /*2.a:mev from Vertex 1 and build two triangle from 6 and 2*/
  eo->mev(1,2,2,1,6,6,7,1,-fi,0);
  eo->mef(1,2,7,1,6,5,3);
  eo->mef(1,2,2,1,7,6,4);

  /*2.b:mev from Vertex 3 and build two triangle from 2 and 4*/
  eo->mev(1,2,2,3,2,2,8,fi,0,1);
  eo->mef(1,2,8,3,2,7,5);
  eo->mef(1,2,4,3,8,2,6);

  /*2.c:mev from Vertex 5 and build two triangle from 4 and 6*/
  eo->mev(1,2,5,9,0,-1,fi);
  eo->mef(1,2,9,4,7);
  eo->mef(1,2,6,9,8);

  /*2.d:3*mef : build a triangle from Vertex 7 ,8 ,9*/
  eo->mef(1,2,8,7,9);
  eo->mef(1,2,9,8,10);
  eo->mef(1,2,7,9,11);

  /***************************Now we have an upper 10 faces**************************************/
  /*3nd:Same Progress, build the lower part*/
  /*3.a:mev from Vertex 1 and build two triangle from 2 and 6*/
  eo->mev(1,1,2,10,0,1,-fi);
  eo->mef(1,1,10,3,12);
  eo->mef(1,1,1,10,13);

  /*3.b:mev from Vertex 3 and build two triangle from 4 and 2*/
  eo->mev(1,1,4,11,-1,fi,0);
  eo->mef(1,1,11,5,14);
  eo->mef(1,1,3,11,15);

  /*3.c:mev from Vertex 5 and build two triangle from 6 and 4*/
  eo->mev(1,1,6,12,-fi,0,-1);
  eo->mef(1,1,12,1,16);
  eo->mef(1,1,5,12,17);

  /*3.d:3*mef : build a triangle from Vertex 10,11,12*/
  eo->mef(1,1,10,11,18);
  eo->mef(1,1,11,12,19);
  eo->mef(1,1,12,10,20);

  icosa->print();
  icosa->showInfo();




  #endif



 
  
  /*cout<<"Solid no"<<cube->solidno<<endl;

  cout<<"sfaces ="<<fs<<endl;
  cout<<"sfaces size="<<fcube->size()<<endl;
  cout<<"face no="<<(*fs)[0]->faceno<<endl;

  cout<<"sedges ="<<es<<endl;
  cout<<"sedges size="<<ecube->size()<<endl;

  cout<<"sverts ="<<vs<<endl;
  cout<<"sverts size="<<vcube->size()<<endl;
  cout<<"vertex no="<<(*vs)[0]->vertexno<<endl;

  cout<<"Loops size="<<lcube->size()<<endl;
  cout<<"Loop 0 = "<<(*ls)[0]<<endl;
  cout<<"Outer Loop="<<l<<endl;
  l->print();
  HalfEdge *he=l->ledg;
  he->print();
  
  he->nxthe->print();
  he->prvhe->print();

  cout<<"he= "<<he<<endl;
  cout<<"nxthe= "<<he->nxthe<<endl;
  cout<<"prvhe= "<<he->prvhe<<endl;
  cout<<"he start vertex"<<he->start<<endl;
  cout<<"nxthe start vertex"<<he->nxthe->start<<endl;*/
  
   glutInit(&argc,argv);
   glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
   glutInitWindowSize(ww,wh);
   glutInitWindowPosition(150,150);
   glutCreateWindow("Cube");

   MyInit();

   glutDisplayFunc(display);
   glutMouseFunc(pressMouse);
   glutMotionFunc(holdMouse);
   glutMainLoop();
  

}//main

##################################################
# END
##################################################
