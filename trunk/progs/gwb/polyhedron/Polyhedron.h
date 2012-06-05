
/**
 *Author:Kai Cao
*/
#include <iostream>
#include <vector>
#include <set>
using namespace std;
//typedef float vector<double>[4];
typedef float matrix[4][4];
typedef short Id;

class Vertex;
class HalfEdge;
class Edge;
class Loop;
class Face;
class Solid;



class Vertex
{
public:
  static  int counter;
  Id vertexno;/*vertex identifier*/
  HalfEdge *vedge;/*pointer to halfedge*/
  vector<double> *vcoord;/*vertext coordinates*/
  Solid *s;
  Vertex(Id no,double x,double y,double z,Solid *s);
  Vertex(double x, double y,double z,Solid *s);
  Vertex();
  Vertex(Solid *s);
};
class HalfEdge
{
public:
  Edge *edg;  /*pointer to parent edge*/
  Vertex *start;  /*pointer to starting vertex */
  Vertex *end;  /*pointer to the end vertex*/
  Loop *wloop;  /*back pointer to loop*/
  HalfEdge *nxthe;  /*pointer to next halfedge*/
  HalfEdge *prvhe;/*pointer to previous halfedge*/
  HalfEdge();/*Default constructor*/
  HalfEdge(Vertex *start, Vertex *end);/*HalfEdge from start to end*/
  HalfEdge *mate(int a);/*Find its mate*/
  Vertex *x();
};

class Edge
{
public:
  HalfEdge *he1;/*pointer to right halfedge*/
  HalfEdge *he2;/*pointer to left halfedge*/
  Edge *nxte;/*pointer to next edge*/
  Edge *prve;/*pointer to previous edge*/
  Edge();
  Edge(Solid *s);
};
class Loop
{
public:
  HalfEdge *ledg;/*Pointer to ring of half edges*/
  Face *lface;/*back to pointer to face*/
  Loop(Face *f);
  Loop();
};

class Face
{
public:
  static int counter;
  Id faceno;/*face identifier*/
  Solid *fsolid;/*back pointer to solid*/
  Loop *flout;/*pointer to outer loop*/
  set<Loop *> *floops;/*pointer to list of loops*/
  vector<double> *feq;/*face equation*/

  Face(Solid* s);
  Face(set<Loop *> *floops, Loop *flout,Solid * s, double a,double b,double c,double d);
  Face();
};
class Solid
{
public:
  Id solidno;/*solid identifer*/
  set<Face *> *sfaces;/*pointer to list of faces*/
  set<Edge *> *sedges;/*pointer to list of edges*/
  set<Vertex *> *sverts;/*pointer to list of vertices*/

  Solid(set<Face *> *sfaces, set<Edge *> sedges,set<Vertex *> *sverts);
  Solid();
};

