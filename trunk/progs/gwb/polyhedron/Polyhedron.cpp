/*
*Kai Cao Jun 2012
*/
#include "Constants.h"
int Vertex::counter=1;

/*Vertex constructors*/
Vertex::Vertex(Id no,double x, double y, double z, Solid *s){
  vertexno=no;
  vcoord=new vector<double>(4);
  (*vcoord)[0]=x;
  (*vcoord)[1]=y;
  (*vcoord)[2]=z;
  (*vcoord)[3]=1.0;
  vertexno=no;
  counter++;
  this->s=s;
  s->sverts->push_back(this);
}

Vertex::Vertex(double x,double y,double z,Solid* s){
  vcoord=new vector<double>(4);
  (*vcoord)[0]=x;
  (*vcoord)[1]=y;
  (*vcoord)[2]=z;
  (*vcoord)[3]=1.0;
  vertexno=counter++;
  this->s=s;
  this->s->sverts->push_back(this);
}


Vertex::Vertex(Solid* s){
  vcoord=new vector<double>(4);
  (*vcoord)[0]=0;
  (*vcoord)[1]=0;
  (*vcoord)[2]=0;
  (*vcoord)[3]=1.0;
  vertexno=counter++;
  this->s=s;
  this->s->sverts->push_back(this);
}
/*Vertex default constructor*/
Vertex::Vertex(){
  vcoord=new vector<double>(4);
  (*vcoord)[0]=0;
  (*vcoord)[1]=0;
  (*vcoord)[2]=0;
  (*vcoord)[3]=1.0;
  vertexno=counter++;
  //s->sverts->push_back(this);
}

/*Print coordinates*/

void Vertex::print(){
  cout<<vertexno<<"("<<(*vcoord)[0]<<","<<(*vcoord)[1]<<","<<(*vcoord)[2]<<")"<<endl;
}

/******************************************/
/*HalfEdge constructors*/
HalfEdge::HalfEdge(){
  start=new Vertex();
  prvhe=this;
  nxthe=this;
}
HalfEdge::HalfEdge(Vertex* start){
  this->start=start;
  prvhe=this;
  nxthe=this;
} 
HalfEdge::HalfEdge(Loop *l){
  wloop=l;
}
HalfEdge::HalfEdge(Edge *e,Vertex *v,Loop *l,HalfEdge *prv,HalfEdge *nxt){
  edg=e;
  start=v;
  wloop=l;
  prvhe=prv;
  nxthe=nxt;  
}

/*To find the mate of a half edge*/
HalfEdge *HalfEdge::mate(){
  if(!this)
    return NULL;

  if(edg==NULL)
    return NULL;

  if(this==edg->he1)
    return edg->he2;
  else
    return edg->he1;
}


void HalfEdge::print(){
  if(start==NULL){
    cout<<"Wrong!No start point!"<<endl;
    return;
  }

  if (nxthe==NULL){
    cout<<"No next HalfEdge!"<<endl;
    nxthe=this;
    return;
  }

  cout<<(start->vertexno)<<"("<<start->getX()<<","<<start->getY()<<","<<start->getZ()<<")"
  <<" -> "<<(nxthe->getStart()->getId())<<"("<<nxthe->getStart()->getX()<<","<<nxthe->getStart()->getY()<<","<<nxthe->getStart()->getZ()<<")"<<endl;

}
/********************Edge***************************/
Edge::Edge(){
  he1=new HalfEdge();
  he2=new HalfEdge();
}
Edge::Edge(Solid *solid){
  s=solid;
  he1=new HalfEdge();
  he2=new HalfEdge();
  s->sedges->push_back(this);

}
Edge::Edge(HalfEdge *half1,HalfEdge *half2,Solid *solid){
  he1=half1;
  he2=half2;
  s=solid;
  s->sedges->push_back(this);
}
/*********************Loop*********************/

Loop::Loop(){ledg=new HalfEdge;}/*Default constructor*/
Loop::Loop(Face *f){ledg=new HalfEdge;lface=f;}/*Constructor with a face*/
Loop::Loop(HalfEdge *he,Face *f){ledg=he;lface=f;}/*Constructor wigh face and leading edge*/
void Loop::print(){
  HalfEdge *he=ledg;
  if (he==NULL)
    cout<<"No Half Edge!"<<endl;
  else do{
    Vertex *v=he->getStart();
    cout<<v->getId()<<"("<<v->getX()<<","<<v->getY()<<","<<v->getZ()<<") -> ";
    he=he->nxthe;
  }while(he!=ledg);
  cout<<endl;

}
/****************Face*************************/
/*Constructors of Face*/
Face::Face(){
  floops=new Vec<Loop *>();
  flout=new Loop(this);
  //fsolid->sfaces->push_back(this);

}

Face::Face(Solid* s){
  floops=new Vec<Loop *>();
  flout=new Loop();
  fsolid=s;
  fsolid->sfaces->push_back(this);

}

Face::Face(Vec<Loop *> *floops, Loop *flout,Solid * s, double a,double b,double c,double d){
  this->floops=floops;
  this->flout=flout;
  fsolid=s;
  fsolid->sfaces->push_back(this);
  feq[0]=a;
  feq[1]=b;
  feq[2]=c;
  feq[3]=d;
}


Face::Face(Id faceno,Vec<Loop *> *floops, Loop *flout,Solid * s, double a,double b,double c,double d){
  
  this->faceno=faceno;
  this->floops=floops;
  this->flout=flout;
  fsolid=s;
  fsolid->sfaces->push_back(this);
  feq[0]=a;
  feq[1]=b;
  feq[2]=c;
  feq[3]=d;
}

void Face::print(){
  cout<<"Outer Loop:"<<endl;
  flout->print();
  cout<<"All Loops:"<<endl;
  for (int i=0;i<floops->size();i++){
    (*floops)[i]->print();
  }
  cout<<endl;
}
//HalfEdge *HalfEdge::mate(int a){return NULL;};
//int *HalfEdge::x(){return &1;}

/*Solid Constructors*/
Solid::Solid(){
  sfaces=new Vec<Face *>();/*pointer to list of faces*/
  sedges=new Vec<Edge *>();/*pointer to list of edges*/
  sverts=new Vec<Vertex *>();/*pointer to list of vertices*/

}
Solid::Solid(Vec<Solid *> *s){
  sfaces=new Vec<Face *>();/*pointer to list of faces*/
  sedges=new Vec<Edge *>();/*pointer to list of edges*/
  sverts=new Vec<Vertex *>();/*pointer to list of vertices*/
  solids=s;
  solids->push_back(this);
}
Solid::Solid(vector<Face *> *sfaces, vector<Edge *> *sedges,vector<Vertex *> *sverts,Vec<Solid *> *solids){
  this->sfaces=sfaces;
  this->sedges=sedges;
  this->sverts=sverts;
  this->solids=solids;
  solids->push_back(this);
}

Solid::Solid(Id solidno,vector<Face *> *sfaces, vector<Edge *> *sedges,vector<Vertex *> *sverts,Vec<Solid *> *solids){
  this->solidno=solidno;
  this->sfaces=sfaces;
  this->sedges=sedges;
  this->sverts=sverts;
  this->solids=solids;
  solids->push_back(this);
}

void Solid::print(){
  for (int i=0;i<sfaces->size();i++){
    cout<<"Face Id: "<<(*sfaces)[i]->faceno<<endl;
    (*sfaces)[i]->print();
  }
}



