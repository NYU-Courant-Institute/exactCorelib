
/**
 *Author:Kai Cao
*/
#include <iostream>
#include <vector>
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
  Id vertexno;/*vertex identifier*/
  HalfEdge *vedge;/*pointer to halfedge*/
  vector<double> *vcoord;/*vertext coordinates*/
};

class HalfEdge
{
  Edge *edg;/*pointer to parent edge*/
  Vertex *vtx;/*pointer to starting vertex */
  Loop *wloop;/*back pointer to loop*/
  HalfEdge *nxthe;/*pointer to next halfedge*/
  HalfEdge *prvhe;/*pointer to previous halfedge*/
};

class Hdge
{
  HalfEdge *he1;/*pointer to right halfedge*/
  HalfEdge *he2;/*pointer to left halfedge*/
  Edge *nxte;/*pointer to next edge*/
  Edge *prve;/*pointer to previous edge*/
};
class Loop
{
  HalfEdge *ledg;/*Pointer to ring of half edges*/
  Face *lface;/*back to pointer to face*/
};

class Face
{
  Id faceno;/*face identifier*/
  Solid *fsolid;/*back pointer to solid*/
  Loop *flout;/*pointer to outer loop*/
  Loop *floops;/*pointer to list of loops*/
  vector<double> *feq;/*face equation*/
};
class Solid
{
  Id solidno;/*solid identifer*/
  Face *sfaces;/*pointer to list of faces*/
  Edge *sedges;/*pointer to list of edges*/
  Vertex *sverts;/*pointer to list of vertices*/
};

