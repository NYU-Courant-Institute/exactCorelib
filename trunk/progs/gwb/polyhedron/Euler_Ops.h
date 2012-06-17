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

  /***************************************Vertex, Face Solid****************************************************/
  /**********Initialize a solid with a single vertex and a single face*****/
  Solid *mvfs(Id s,Id f,Id v,double x,double y,double z);
  /********Kill a solid with a single face & a single vertex***************/
  void lkvfs(Solid *s);
  /********Kill a solid with a single face & a single vertex***************/
  void kvfs(Id s);  

  /*Lower level Operations*/
  /********************************Lower Level Edge & Vertex****************************************************/
  /*Lower level make edge vertex*/
  void lmev(HalfEdge *he1,HalfEdge *he2,Id v,double x,double y,double z);
  /*Lower level kill edge vertex*/
  void lkev(HalfEdge *he1,HalfEdge *he2);
  
  /********************************Lower Level Edge & Ring******************************************************/
  /*Make Edge Face*/
  Face *lmef(HalfEdge *he1, HalfEdge *he2, Id f);
  /*Kill Edge Face*/
  void lkef(HalfEdge *he1,HalfEdge *he2);

  /********************************Lower Level Edge & Ring******************************************************/
  /*Kill Edge make Ring*/
  void lkemr(HalfEdge *h1,HalfEdge *h2);
  /*Make Edge kill Ring*/
  void lmekr(HalfEdge *he1,HalfEdge *he2);

  /*********************************Lower level Face & Ring,Hole*************************************************/
  /*Kill Face make Ring Hole*/
  void lkfmrh(Face *fac1,Face *fac2);
  /*Make Face kill Ring Hole*/
  void lmfkrh(Loop *l,Id f);
  /*Move the loop from its parent face to another(*tofac)*/
  void lringmv(Loop *l,Face *tofac,int inout);

  /*************Higher Leve Opeartions**************/
  /*In higher level,we need some search for objects*/
  /*Get solid we want*/
  Solid *getsolid(Id sn);
  /*Get face*/
  Face *fface(Solid *s,Id fn);
  /*Get HalfEdge*/
  HalfEdge *fhe(Face *f,Id vn1,Id vn2);
  /*Simple get HalfEdge*/
  HalfEdge *fhe(Face *f,Id vn);

  /*****************************************************Actural higher level Euler Operators*********************************************************/
  /**************************Edge & Vertex***************************************************/
  /*Higher level make Edge Vertex*/
  int mev(Id s,Id f1,Id f2,Id v1,Id v2,Id v3,Id v4,double x,double y,double z);
  int mev(Id s,Id f1,Id v1,Id v4,double x,double y,double z);
  /*Higher level kill Edge Vertex*/
  int kev(Id s,Id f,Id v1,Id v2);

  /**************************Edge & Face*****************************************************/
  /*Higher level make edge face*/
  int mef(Id s,Id f1,Id v1,Id v2,Id v3,Id v4,Id f2);
  int mef(Id s,Id f1,Id v1,Id v3,Id f2);
  /*Higher lever kill Edge Face*/
  int kef(Id s,Id f,Id v1,Id v2);

  /***************************Edge & Ring*****************************************************/
  /*Higher level kill edge make ring*/
  int kemr(Id s, Id f, Id v1, Id v2);

  /*Higher level make edge kill ring*/
  /*We build an edge from the start vertex of He(v1,v2) to He (v3,v4)*/
  int mekr(Id s,Id f,Id v1,Id v2,Id v3,Id v4);
  int mekr(Id s,Id f,Id v1,Id v3);

  /**************************face & ring,hole****************************************/
  int kfmrh(Id s,Id f1,Id f2);
  int mfkrh(Id s,Id f1,Id v1,Id v2,Id f2);

  /**************************Move a ring from one face to another face****************/
  int ringmv(Id s,Id f1,Id f2,Id v1,Id v2,Id inout);







};
