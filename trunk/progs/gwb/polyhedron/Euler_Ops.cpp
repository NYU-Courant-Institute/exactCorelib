#include <iostream>
#include "Euler_Ops.h"
using namespace std;

HalfEdge *Euler_Ops::addhe(Edge *e, Vertex *v,HalfEdge *followhe,int sign){
 HalfEdge *he; 
 if(!followhe->edg)
  he=followhe;
 else{
  he=new HalfEdge();
  /*Set previous he*/
  followhe->prvhe->nxthe=he;
  he->prvhe=followhe->prvhe;
  /*set next he*/
  he->nxthe=followhe;
  followhe->prvhe=he;
 }

 he->edg=e;
 he->start=v;
 he->wloop=followhe->wloop;
 if(sign==PLUS)
  e->he1=he;
 else
  e->he2=he;
 
 return he;
}

HalfEdge *Euler_Ops::delhe(HalfEdge *he){
  if (!he->edg){
    delete he;
  }
  else if (he->nxthe==he){
    he->edg=NULL;
    return(he);
  }
  else{
  /*Delete one direction*/
    he->prvhe->nxthe=he->nxthe;
    he->nxthe->prvhe=he->prvhe;

    HalfEdge *rhe=he->prvhe;
    delete he;
    return (rhe);
  }
}

/*Higher level make vertex face solid*/
Solid *mvfs(Id s,Id f,Id v,double x,double y,double z){

/*Initialize*/
  Solid *newsolid;
  Face *newface;
  Vertex *newvertex;
  HalfEdge *newhe;
  Loop *newloop;
  newsolid=new Solid;
  newface=new Face;
  newvertex=new Vertex;
  newhe=new HalfEdge;

  newsolid->solidno=s;
  newface->faceno=f;
  newface->flout=newloop;
  newloop->ledg=newhe;
  newhe->wloop=newloop;
  newhe->nxthe=newhe->prvhe=newhe;
  newhe->start=newvertex;
  newhe->edg=NULL;

  newvertex->vertexno=v;
  (*(newvertex->vcoord))[0]=x;
  (*(newvertex->vcoord))[1]=y; 
  (*(newvertex->vcoord))[2]=z; 
  (*(newvertex->vcoord))[3]=1.0;

  return(newsolid);
}

void Euler_Ops::lmev(HalfEdge *he1,HalfEdge *he2,Id v,double x,double y,double z){
  HalfEdge *he;
  Vertex *newvertex=new Vertex(v,x,y,z,he1->wloop->lface->fsolid);
  Edge *newedge=new Edge(he1->wloop->lface->fsolid);
 
  /*Seperate all points before you reach he2*/
  he=he1;
  while(he!=he2){
    he->start=newvertex;
    //he=he->mate();//->nxthe;
  }

  addhe(newedge,newvertex,he2,PLUS);
  addhe(newedge,he2->start,he1,MINUS);

  newvertex->vedge=he2->prvhe;
  he2->start->vedge=he2; 
  
}

Face *Euler_Ops::lmef(HalfEdge *he1, HalfEdge *he2,Id f){

  Face *newface=new Face(he1->wloop->lface->fsolid);
  Loop *newloop=new Loop(newface);
  Edge *newedge=new Edge(he1->wloop->lface->fsolid);
  newface->faceno=f;
  newface->flout=newloop;

  /*The HalfEdges in [he1,he2) is in the newloop*/
  HalfEdge *he=he1;

  while(he!=he2){
    he->wloop=newloop;
    he=he->nxthe;
  }

  HalfEdge *nhe1=addhe(newedge,he2->start,he1,MINUS);
  HalfEdge *nhe2=addhe(newedge,he1->start,he2,PLUS);

  nhe2->prvhe->nxthe=nhe1;
  nhe1->prvhe->nxthe=nhe2;

  HalfEdge *temp=nhe1->prvhe;
  nhe1->prvhe=nhe2->prvhe;
  nhe2->prvhe=temp;

  newloop->ledg=nhe1;
  he2->wloop->ledg=nhe2;

  return(newface);
  
}
int main(){
}
