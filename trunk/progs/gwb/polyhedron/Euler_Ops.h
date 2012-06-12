#include <iostream>
#include "Constants.h"
using namespace std;
class Euler_Ops{
public:
  Vec<Solid *> *solids;
  /*Constructors*/
  Euler_Ops();
  Euler_Ops(Vec<Solid *> *solids);
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

  /*************Higher Leve Opeartions**************/
  /*In higher level,we need some search for objects*/
  /*Get solid we want*/
  Solid *getsolid(Id sn);
  /*Get face*/
  Face *fface(Solid *s,Id fn);
  /*Get HalfEdge*/
  HalfEdge *fhe(Face *f,Id vn1,Id vn2);

  /*Actural higher level Euler Operators*/
  /*Higher Level make edge vertex*/
  int mev(Id s,Id f1,Id f2,Id v1,Id v2,Id v3,Id v4,double x,double y,double z);
  int smev(Id s,Id f1,Id v1,Id v4,double x,double y,double z);

  /*Higher level make edge face*/
  int mef(Id s,Id f1,Id v1,Id v2,Id v3,Id v4,Id f2);
  int smev(Id s,Id f1,Id v1,Id v3,Id f2);
};
