#include <iostream>
#include "Constants.h"
using namespace std;
class Euler_Ops{
  /**********Operations on HalfEdge************/
  HalfEdge *addhe(Edge *e, Vertex *v,HalfEdge *he,int sign);
  HalfEdge *delhe(HalfEdge* he);
  /**********Initialize a solid with a single vertex and a single face*****/
  Solid *mvfs(Id s,Id f,Id v,double x,double y,double z);
  /*Lower level Operations*/
  /*Lower level make edge vertex*/
  void lmev(HalfEdge *he1,HalfEdge *he2,Id v,double x,double y,double z);
  /*Make Edge Face*/
  Face *lmef(HalfEdge *he1, HalfEdge *he2, Id f);
  /*Kill Edge make Ring*/
  void  lkemr(HalfEdge *h1,HalfEdge *h2);
  /*Kill Face make Ring Hole*/
  void lkfmrh(Face *fac1,Face *fac2);
};
