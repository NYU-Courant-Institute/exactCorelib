
/**
 *Author:Kai Cao
*/
#include <iostream>
#include <vector>
using namespace std;
typedef double coords[4];
typedef float matrix[4][4];
typedef short Id;
#define Vec vector 

/*Declaration*/
class Vertex;
class HalfEdge;
class Edge;
class Loop;
class Face;
class Solid;


/*********************Vertex with coordinates*********************/
class Vertex
{
public:
  static  int counter;
  Id vertexno;/*vertex identifier*/
  HalfEdge *vedge;/*pointer to halfedge*/
  vector<double> *vcoord;/*vertext coordinates*/
  Solid *s;/*pointer to the solid*/

  /*Getters and Setters*/
  Id getId(){
    return vertexno;
  }
  /*Getter and Setter for the half edge*/
  HalfEdge *getHe(){
    return vedge;
  }

  void setHe(HalfEdge *he){
    vedge=he;
  }


  /*Getter and Setter for the coordinates*/
  vector<double> *getCoord(){
    return vcoord;
  }

  void setCoord(vector<double> *cod){
    vcoord=cod;
  }
  /*Get coordinates seperately*/
  double getX(){
    return (*vcoord)[0];
  }

  void setX(double x){
    (*vcoord)[0]=x;
  }

  double getY(){
    return (*vcoord)[1];
  }

  void setY(double y){
    (*vcoord)[1]=y;
  }

  double getZ(){
    return (*vcoord)[2];
  }

  void setZ(double z){
    (*vcoord)[2]=z;
  }

  /*Solid Getters and Setters*/
  Solid *getSolid(){
    return s;
  }

  void setSolid(Solid *s){
    this->s=s;
  }
  /*Constructors*/
  Vertex(Id no,double x,double y,double z,Solid *s);
  Vertex(double x, double y,double z,Solid *s);
  /*If we just have a pointer to s, the vcoords should be initialize all 0*/
  Vertex(Solid *s);
  Vertex();
  void print(); /*Print the coordinates of the vertex*/

};

/*************HalfEdge with a start point and a mate funciton*******/
class HalfEdge
{
public:

  Edge *edg;  /*pointer to parent edge*/
  Vertex *start;  /*pointer to starting vertex */
  Loop *wloop;  /*back pointer to loop*/

  /*This is important for the loop, and actually it is a sequence*/
  HalfEdge *nxthe;  /*pointer to next halfedge*/
  HalfEdge *prvhe;/*pointer to previous halfedge*/

  /*Getters and Setters*/
  /*edg*/
  Edge *getEdge(){
    return edg;
  }
  void vectorEdge(Edge *e){
    edg=e;
  }

  /*start*/
  Vertex *getStart(){
    return start;
  }
  void vectorStart(Vertex *v){
    start=v;
  }

  /*wloop*/
  Loop *getLoop(){
    return wloop;
  }
  void getLoop(Loop *l){
    wloop=l;
  }
 
  /*previous and next HalfEdge*/
  HalfEdge *getNxthe(){
    return nxthe;
  }
  void setNxthe(HalfEdge *he){
    nxthe=he;
  }
  HalfEdge *getPrvhe(){
    return prvhe;
  }
  void setPrvhe(HalfEdge *he){
    prvhe=he;
  }


  HalfEdge();/*Default constructor*/
  HalfEdge(Vertex *start);/*HalfEdge from start vertex*/
  HalfEdge(Loop *l);
  HalfEdge(Edge *e,Vertex *v,Loop *wloop,HalfEdge *prv,HalfEdge *nxt);/*Initiate all members*/


  HalfEdge *mate();/*Find its mate*/
  void print();/*Print start -> end*/
};


/*****************Edge with two half edges***********************/
class Edge
{
public:
  HalfEdge *he1;/*pointer to right halfedge*/
  HalfEdge *he2;/*pointer to left halfedge*/
  Solid *s;

  HalfEdge *getHe1(){return he1;}
  void setHe1(HalfEdge *he){he1=he;}
  HalfEdge *getHe2(){return he2;}
  void setHe2(HalfEdge *he){he2=he;}
  Solid *getSolid(){return s;}
  void setSolid(Solid *solid){s=solid;}

  /*Constructors*/
  Edge();
  Edge(Solid *s);
  Edge(HalfEdge *he1,HalfEdge *he2,Solid *s);

  void print(){ if(he1) he1->print(); else if(he2) he2->print(); else cout<<"empty edge without halfedges"<<endl;}; /*print the start end end point*/
};


/******************Loop with leading Edge*************************/
class Loop
{
public:
  HalfEdge *ledg;/*Pointer to ring of half edges*/
  Face *lface;/*back to pointer to face*/
  Loop();/*Default constructor*/
  Loop(Face *f);/*Constructor with a face*/
  Loop(HalfEdge *he,Face *f);/*Constructor wigh face and leading edge*/
  void print();/*print the loop points in a circle*/
};

/*********************Loop and a inner loop vector********************/
class Face
{
public:
  static int counter;
  Id faceno;/*face identifier*/
  Solid *fsolid;/*back pointer to solid*/
  Loop *flout;/*pointer to outer loop*/
  Vec<Loop *> *floops;/*pointer to list of loops*/
  coords feq;/*face equation*/

  Face();
  Face(Solid* s);
  Face(Vec<Loop *> *floops, Loop *flout,Solid * s, double a,double b,double c,double d);
  Face(Id faceno,Vec<Loop *> *floops, Loop *flout,Solid * s, double a,double b,double c,double d);

  void print();
};

/*******************Solid with vertices,edges and faces**********************/
class Solid
{
public:
  Id solidno;/*solid identifer*/
  Vec<Face *> *sfaces;/*pointer to list of faces*/
  Vec<Edge *> *sedges;/*pointer to list of edges*/
  Vec<Vertex *> *sverts;/*pointer to list of vertices*/
  Vec<Solid *> *solids;

  Solid();
  Solid(Vec<Solid *> *solids);
  Solid(vector<Face *> *sfaces, vector<Edge *> *sedges,vector<Vertex *> *sverts,Vec<Solid *> *solids);
  Solid(Id solidno,vector<Face *> *sfaces, vector<Edge *> *sedges,vector<Vertex *> *sverts,Vec<Solid *> *solids);


  void print();

};

