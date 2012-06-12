/*
 * file: Euler_Ops.cpp
 *
 *  This is an implementation of Geometric Work Bench from Mantyla's book.
 *
 * Author: Kai Cao
 * June 2012
 * Since Core 2.1.
 *
 ***************************************************/

#include <iostream>
#include "Euler_Ops.h"
using namespace std;

/*Constructors*/
Euler_Ops::Euler_Ops(Vec<Solid *> *solids){
  this->solids=solids;
}

Euler_Ops::Euler_Ops(){
 solids=new Vec<Solid *>();
}


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
Solid *Euler_Ops::mvfs(Id s,Id f,Id v,double x,double y,double z){

/*Initialize*/
  Solid *newsolid;
  Face *newface;
  Vertex *newvertex;
  HalfEdge *newhe;
  Loop *newloop;
  /*Initialize all objects*/
  newsolid=new Solid(solids);
  newface=new Face(newsolid);
  newloop=new Loop(newface);
  newhe=new HalfEdge(newloop);
  newvertex=new Vertex(newsolid);

  /*Assign all values*/
  newsolid->solidno=s;
  newface->faceno=f;
  newvertex->vertexno=v;
  newvertex->setX(x);
  newvertex->setY(y);
  newvertex->setZ(z);

  /*Assign all pointers*/
  /*Connection between vertices and halfedges*/
  newhe->start=newvertex;
  newvertex->vedge=newhe;
  /*Connection between halfedges and Loops*/
  newhe->wloop=newloop;
  newloop->ledg=newhe;
  newhe->nxthe=newhe->prvhe=newhe;
  /*Connection between loops and and faces*/
  newface->flout=newloop;
  newface->floops->push_back(newloop);

  /*We have a half edge but no edge*/  
  newhe->edg=NULL;

  /*newvertex->print();
  newhe->print();
  newloop->print();
  newface->print();
  newsolid->print();*/

  return(newsolid);
}

void Euler_Ops::lmev(HalfEdge *he1,HalfEdge *he2,Id v,double x,double y,double z){
  HalfEdge *he;
  Vertex *newvertex=new Vertex(v,x,y,z,he1->wloop->lface->fsolid);
  Edge *newedge=new Edge(he1->wloop->lface->fsolid);
 
  /*Seperate all points before you reach he2*/
  he=he1;
  while(he!=he2){
    cout<<"In the HalfEdge Loop"<<endl;
    he->start=newvertex;
    he=he->mate()->nxthe;
  }

  addhe(newedge,newvertex,he2,PLUS);
  addhe(newedge,he2->start,he1,MINUS);

  newvertex->vedge=he2->prvhe;
  he2->start->vedge=he2; 
  cout<<newedge->he1<<endl;
  cout<<newedge->he2<<endl;
  cout<<he1<<endl;
  cout<<he2<<endl;
  
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

/*Get solid*/
Solid *Euler_Ops::getsolid(Id sn){
  /*cout<<"begin get solid"<<endl;
  cout<<"solids=NLLL?"<<(solids==NULL)<<endl;
  cout<<"solids size="<<(solids->size())<<endl;
  cout<<"solids before begin"<<solids<<endl;
  cout<<"it points to begin"<<endl;*/
  for (int i=0;i<solids->size();i++){
    Solid *s=(*solids)[i];
    if(s->solidno==sn)
      return s;
  }//for
  return NULL;
}

/*Get face*/
Face *Euler_Ops::fface(Solid *s,Id fn){
  Vec<Face *>::iterator it;
  for (it=s->sfaces->begin();it!=s->sfaces->end();it++){
    if((*it)->faceno==fn)
      return *it;
  }//for
  return NULL;
}

/*Get HalfEdge*/
HalfEdge *Euler_Ops::fhe(Face *f,Id vn1,Id vn2){  

  Vec<Loop *> *ls=f->floops;

  /*Look through all loops*/
  for (int i=0;i<ls->size();i++){
    HalfEdge *he=(*ls)[i]->ledg;

    /*Look through all half edges*/
    do{
      if(he->start->vertexno==vn1&&he->nxthe->start->vertexno==vn2)
        return he;
       he=he->nxthe;
    }while(he!=(*ls)[i]->ledg);
   }//for

   return NULL;
 }

/*Higher level make edge vertex*/
int Euler_Ops::mev(Id s,Id f1,Id f2,Id v1,Id v2,Id v3,Id v4,double x,double y,double z){

  Solid *oldsolid;
  Face *oldface1,*oldface2;
  HalfEdge *he1,*he2;
  
  /*Get solid*/
  oldsolid=getsolid(s);
  if(oldsolid==NULL){
    cout<<"mev: solid "<<s<<"not found\n";
    return(ERROR);
  }

  oldface1=fface(oldsolid,f1);
  if(oldface1==NULL){
    cout<<"mev: face "<<f1<<"not found in solid "<<s<<endl;
    return(ERROR);
  }

  oldface2=fface(oldsolid,f2);
  if(oldface2==NULL){
    cout<<"mev: face "<<f2<<"not found in solid "<<s<<endl;
    return(ERROR);
  }

  he1=fhe(oldface1,v1,v2);
  if(he1==NULL){
    cout<<"mev: HalfEdge"<<v1<<"->"<<v2<<"not found in face"<<f1<<endl;
    return(ERROR);
  }

  he2=fhe(oldface2,v1,v3);
  if(he2==NULL){
    cout<<"mev: HalfEdge"<<v1<<"->"<<v3<<"not found in face"<<f2<<endl;
    return(ERROR);
  }

  cout<<"Solid "<<oldsolid->solidno<<endl;
  oldsolid->print();
  cout<<"oldface1 "<<oldface1->faceno<<endl;
  oldface1->print();
  cout<<"oldface2 "<<oldface2->faceno<<endl;
  oldface2->print();
  cout<<"he1 "<<endl;
  he1->print();
  cout<<"he2 "<<endl;
  he2->print();
  cout<<endl<<endl;

  lmev(he1,he2,v4,x,y,z);
  return SUCCESS;


}//mev

/*Higher level make edge face*/
int Euler_Ops::mef(Id s,Id f1,Id v1,Id v2,Id v3,Id v4,Id f2){
                    
  Solid *oldsolid;
  Face *oldface1;
  HalfEdge *he1,*he2;

  oldsolid=getsolid(s);
  if(oldsolid==NULL){
    cout<<"mev: solid "<<s<<"not found\n";
    return(ERROR);
  }

  oldface1=fface(oldsolid,f1);
  if(oldface1==NULL){
    cout<<"mev: face "<<f1<<"not found in solid "<<s;
    return(ERROR);
  }

  he1=fhe(oldface1,v1,v2);
  if(he1==NULL){
    cout<<"mev: HalfEdge"<<v1<<"->"<<v2<<"not found in face"<<f1;
    return(ERROR);
  }

  he2=fhe(oldface1,v3,v4);
  if(he2==NULL){
    cout<<"mev: HalfEdge"<<v3<<"->"<<v4<<"not found in face"<<f1;
    return(ERROR);
  }

  lmef(he1,he2,f2);
  return SUCCESS;

}

