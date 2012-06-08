#include <iostream>
#include "Euler_Ops.h"
using namespace std;

/*Constructors*/
Euler_Ops::Euler_Ops(set<Solid *> *solids){
  this->solids=solids;
}

Euler_Ops::Euler_Ops(){
 solids=new set<Solid *>();
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
  cout<<"begin mvfs"<<endl;
/*Initialize*/
  Solid *newsolid;
  Face *newface;
  Vertex *newvertex;
  HalfEdge *newhe;
  Loop *newloop;
  newsolid=new Solid();
  newface=new Face;
  newloop=new Loop;
  cout<<"successfully before vertex"<<endl;
  newvertex=new Vertex();
  cout<<"successfully after vertex"<<endl;
  cout<<"solid sverts"<<(newsolid->sverts)<<endl;
  cout<<"solid sedges"<<(newsolid->sedges)<<endl;
  cout<<"solid sfaces"<<(newsolid->sfaces)<<endl;
  newhe=new HalfEdge;
  cout<<"successfully new all objects needed"<<endl;

  newsolid->solidno=s;
  newface->faceno=f;
  newface->flout=newloop;
  cout<<"1/4"<<endl;
  cout<<"newloop="<<newloop<<endl;
  cout<<"newloop->ledg"<<endl;
  cout<<newloop->ledg<<endl;
  cout<<"newhe"<<endl;
  cout<<newhe<<endl;
  cout<<"after newloop->ledg"<<endl;
  newloop->ledg=newhe;
  cout<<"3/8"<<endl;
  newhe->wloop=newloop;
  cout<<"1/2"<<endl;
  
  newhe->nxthe=newhe->prvhe=newhe;
  cout<<"3/4"<<endl;
  newhe->start=newvertex;
  cout<<"7/8"<<endl;
  newhe->edg=NULL;
  cout<<"successfully set up all members"<<endl;


  newvertex->vertexno=v;
  cout<<"newvertex->vertexno"<<endl;
  (*(newvertex->vcoord))[0]=x;
  cout<<"vcoord"<<endl;
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
    he=he->mate()->nxthe;
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

/*Get solid*/
Solid *Euler_Ops::getsolid(Id sn){
  cout<<"begin get solid"<<endl;
  set<Solid *>::iterator it;
  cout<<"solids=NLLL?"<<(solids==NULL)<<endl;
  cout<<"solids size="<<(solids->size())<<endl;
  cout<<"solids before begin"<<solids<<endl;
  solids->begin();
  cout<<"it points to begin";
  for (;it!=solids->end();it++){
    cout<<"in the loop";
    if((*it)->solidno==sn)
      return *it;
  }//for
  return NULL;
}

/*Get face*/
Face *Euler_Ops::fface(Solid *s,Id fn){
  set<Face *>::iterator it;
  for (it=s->sfaces->begin();it!=s->sfaces->end();it++){
    if((*it)->faceno==fn)
      return *it;
  }//for
  return NULL;
}

/*Get HalfEdge*/
HalfEdge *Euler_Ops::fhe(Face *f,Id vn1,Id vn2){  
  set<Loop *>::iterator it;
  for (it=f->floops->begin();it!=f->floops->end();it++){
    HalfEdge *he=(*it)->ledg;
    do{
      if(he->start->vertexno==vn1&&he->nxthe->start->vertexno==vn2)
        return he;
       he=he->nxthe;
    }while(he!=(*it)->ledg);
   }//for

   return NULL;
 }

/*Higher level make edge vertex*/
int Euler_Ops::mev(Id s,Id f1,Id f2,Id v1,Id v2,Id v3,Id v4,double x,double y,double z){
  cout<<"begin mev"<<endl;
  Solid *oldsolid;
  Face *oldface1,*oldface2;
  HalfEdge *he1,*he2;
  
  cout<<"check mev 1: before getsolid"<<endl;
  /*Get solid*/
  cout<<"solids before get solid:"<<solids<<endl;
  oldsolid=getsolid(s);
  if(oldsolid==NULL){
    cout<<"mev: solid "<<s<<"not found\n";
    return(ERROR);
  }
  cout<<"check mev 2: after getsolid"<<endl;

  oldface1=fface(oldsolid,f1);
  if(oldface1==NULL){
    cout<<"mev: face "<<f1<<"not found in solid "<<s;
    return(ERROR);
  }

  oldface2=fface(oldsolid,f2);
  if(oldface2==NULL){
    cout<<"mev: face "<<f2<<"not found in solid "<<s;
    return(ERROR);
  }

  he1=fhe(oldface1,v1,v2);
  if(he1==NULL){
    cout<<"mev: HalfEdge"<<v1<<"->"<<v2<<"not found in face"<<f1;
    return(ERROR);
  }

  he2=fhe(oldface2,v1,v3);
  if(he2==NULL){
    cout<<"mev: HalfEdge"<<v1<<"->"<<v3<<"not found in face"<<f2;
    return(ERROR);
  }

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

