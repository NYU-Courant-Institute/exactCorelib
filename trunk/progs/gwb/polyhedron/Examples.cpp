#include "Euler_Ops.h"
#define CUBEWITHAHOLE
GLsizei ww=500;
GLsizei wh=500;
Euler_Ops *eo;
void MyInit(void){
  glClearColor(1,1,1,0);
  glColor3f(1,0,0);
  glPointSize(10);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluOrtho2D(0,(GLdouble)ww,0.0,(GLdouble)wh);
  glOrtho(-100.0,(GLdouble)ww,-100.0,(GLdouble)wh,-1000,1000);
}
void display(void){
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  Solid *s=(*(eo->solids))[0]; 
  vector<Edge *> *es=s->sedges;
  glRotatef(45,1,1,1);
  glScalef(100,100,100);
  glBegin(GL_LINES);
  for (int i=0;i<es->size();i++){
    Edge *e=(*es)[i];
    Vertex *v1=e->he1->start;
    Vertex *v2=e->he2->start;
   glVertex3f(v1->getX(),v1->getY(),v1->getZ());
   glVertex3f(v2->getX(),v2->getY(),v2->getZ());

  }
   glEnd();
   glFlush();

}
int main(int argc,char **argv){
  eo=new Euler_Ops();
  #ifdef CUBE
  /**********************First Step New a Solid with single Face&Vertex, add vertex 1:(0,0,0)*************************************/
  cout<<"**********************First Step New a Solid with single Face&Vertex, add vertex 1:(0,0,0)******************************"<<endl;
  Solid *cube=eo->mvfs(1,1,1,0,0,0);

  Vec<Solid *> *ss=eo->solids;
  /*cout<<scube->size()<<endl;
  cout<<(*ss)[0]<<endl;
  cout<<s<<endl;*/
  Vec<Face *> *fs=cube->sfaces;
  Vec<Edge *> *es=cube->sedges;
  Vec<Vertex *> *vs=cube->sverts;
  Vec<Loop *> *ls=(*fs)[0]->floops;
  Loop *l=(*fs)[0]->flout;

  cube->showInfo();
  cube->print();

  /***********************2nd step : make edge&vertex,add vertex 2(1,0,0)************************************/
  cout<<"***********************2nd step : make edge&vertex,add vertex 2(1,0,0)************************************"<<endl;
  int check=eo->mev(1,1,1,1,1,1,2,1,0,0);/*Solidno, faceno1,faceno2,v1,v2,v3...newV,coordinates*/

  if(check!=SUCCESS)
    cout<<"We cannot build the point 1,0,0 and the edge."<<endl;

  cube->showInfo();
  cube->print();

  /************************3rd step: make edge &vertex,add vertex 3(0,1,0)***********************************/
  cout<<"***********************3rd step : make edge&vertex,add vertex 3(0,1,0)************************************"<<endl;
  check=eo->mev(1,1,1,1,2,2,3,0,1,0);
  if (check!=SUCCESS)
    cout<<"We cannot build the point 0,1,0 and the edge."<<endl;
  cube->showInfo();
  cube->print();
    
  /************************4th step: make edge &vertex,add vertex 4(1,1,0)***********************************/
  cout<<"************************4th step: make edge &vertex,add vertex 4(1,1,0)***********************************"<<endl;
  check=eo->mev(1,1,1,3,1,1,4,1,1,0);
  if (check!=SUCCESS)
    cout<<"We cannot build the point 0,1,0 and the edge."<<endl;
  cube->showInfo();
  cube->print();

  /************************5th step: make edge &face,add edge (1,0,0) - (1,1,0)***********************************/
  cout<<"************************5th step: make edge &face,add edge (1,0,0) - (1,1,0)***********************************"<<endl;
  check=eo->mef(1,1,2,1,4,3,2);
  if (check!=SUCCESS)
    cout<<"We cannot build the point 0,1,0 and the edge."<<endl;
  cube->showInfo();
  cube->print();

  /************************6th step: make edge &vertex,add vertex 5(0,0,1)***********************************/
  cout<<"************************6th step: make edge &vertex,add vertex 5(0,0,1)***********************************"<<endl;
  check=eo->mev(1,2,2,1,3,3,5,0,0,1);/*Solidno, faceno1,faceno2,v1,v2,v3...newV,coordinates*/

  if (check!=SUCCESS)
    cout<<"We cannot build the point 0,1,0 and the edge."<<endl;
  cube->showInfo();
  cube->print();

  /************************7th step: make edge &vertex,add vertex 6(1,0,1)***********************************/
  cout<<"************************7th step: make edge &vertex,add vertex 6(1,0,1)***********************************"<<endl;
  check=eo->mev(1,2,2,2,1,1,6,1,0,1);/*Solidno, faceno1,faceno2,v1,v2,v3...newV,coordinates*/

  if (check!=SUCCESS)
    cout<<"We cannot build the point 0,1,0 and the edge."<<endl;
  cube->showInfo();
  cube->print();

  /************************8th step: make edge &vertex,add vertex 7(0,1,1)***********************************/
  cout<<"************************8th step: make edge &vertex,add vertex 7(0,1,1)***********************************"<<endl;
  check=eo->mev(1,2,2,3,4,4,7,0,1,1);/*Solidno, faceno1,faceno2,v1,v2,v3...newV,coordinates*/

  if (check!=SUCCESS)
    cout<<"We cannot build the point 0,1,0 and the edge."<<endl;
  cube->showInfo();
  cube->print();

  /************************9th step: make edge &vertex,add vertex 8(1,1,1)***********************************/
  cout<<"************************9th step: make edge &vertex,add vertex 8(1,1,1)***********************************"<<endl;
  check=eo->mev(1,2,2,4,2,2,8,1,1,1);/*Solidno, faceno1,faceno2,v1,v2,v3...newV,coordinates*/

  if (check!=SUCCESS)
    cout<<"We cannot build the point 0,1,0 and the edge."<<endl;
  cube->showInfo();
  cube->print();

  /************************10th step: make edge &face,add edge (1,0,1) - (0,0,1)***********************************/
  cout<<"***********************10th step: make edge &face,add edge (1,0,1) - (0,0,1)***********************************"<<endl;
  check=eo->mef(1,2,6,2,5,1,3);/*Solidno, oldfaceno,v1,v2,v3,v4,newfaceno*/
  if (check!=SUCCESS)
    cout<<"We cannot build the point 0,1,0 and the edge."<<endl;
  cube->showInfo();
  cube->print();

  /************************11th step: make edge &face,add edge (0,0,1) - (0,1,1)***********************************/
  cout<<"***********************11th step: make edge &face,add edge (0,0,1) - (0,1,1)***********************************"<<endl;
  check=eo->mef(1,2,5,1,7,3,4);/*Solidno, oldfaceno,v1,v2,v3,v4,newfaceno*/
  if (check!=SUCCESS)
    cout<<"We cannot build the point 0,1,0 and the edge."<<endl;
  cube->showInfo();
  cube->print();

  /************************12th step: make edge &face,add edge (0,1,1) - (1,1,1)***********************************/
  cout<<"***********************12th step: make edge &face,add edge (0,1,1) - (1,1,1)***********************************"<<endl;
  check=eo->mef(1,2,7,3,8,4,5);/*Solidno, oldfaceno,v1,v2,v3,v4,newfaceno*/
  if (check!=SUCCESS)
    cout<<"We cannot build the point 0,1,0 and the edge."<<endl;
  cube->showInfo();
  cube->print();

  /************************13th step: make edge &face,add edge (1,1,1) - (1,0,1)***********************************/
  cout<<"***********************13th step: make edge &face,add edge (1,1,1) - (1,0,1)***********************************"<<endl;
  check=eo->mef(1,2,8,4,6,5,6);/*Solidno, oldfaceno,v1,v2,v3,v4,newfaceno*/
  if (check!=SUCCESS)
    cout<<"We cannot build the point 0,1,0 and the edge."<<endl;
  cube->showInfo();
  cube->print();

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
   //glutMouseFunc(pressMouse);
   //glutMotionFunc(holdMouse);
   glutMainLoop();
  
  

}
