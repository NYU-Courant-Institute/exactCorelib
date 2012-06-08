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
  vertexno=counter++;
  this->s=s;
}

Vertex::Vertex(double x,double y,double z,Solid* s){
  vcoord=new vector<double>(4);
  (*vcoord)[0]=x;
  (*vcoord)[1]=y;
  (*vcoord)[2]=z;
  (*vcoord)[3]=1.0;
  vertexno=counter++;
  this->s=s;
  this->s->sverts->insert(this);
}


Vertex::Vertex(Solid* s){
  vcoord=new vector<double>(4);
  (*vcoord)[0]=0;
  (*vcoord)[1]=0;
  (*vcoord)[2]=0;
  (*vcoord)[3]=1.0;
  vertexno=counter++;
  this->s=s;
  this->s->sverts->insert(this);
}
/*Vertex default constructor*/
Vertex::Vertex(){
  cout<<"Get into constructor"<<endl;
  vcoord=new vector<double>(4);
  cout<<"successfully new the coordinates"<<endl;
  (*vcoord)[0]=0;
  (*vcoord)[1]=0;
  (*vcoord)[2]=0;
  (*vcoord)[3]=1.0;
  cout<<"successfully assign the coordinates"<<endl;
  vertexno=counter++;
  cout<<"no problem"<<endl;
  s=new Solid;
  s->sverts->insert(this);
}

void Vertex::print(){
  cout<<"("<<(*vcoord)[0]<<","<<(*vcoord)[1]<<","<<(*vcoord)[2]<<")"<<endl;
}

/******************************************/
/*HalfEdge constructors*/
HalfEdge::HalfEdge(Vertex* start){
  this->start=start;
} 
HalfEdge::HalfEdge(){
}

HalfEdge *HalfEdge::mate(){
  if(!this)
    return NULL;

  if(!edg)
    return NULL;

  if(this==edg->he1)
    return edg->he2;
  else
    return edg->he1;
}


//HalfEdge *HalfEdge::mate(int a){return NULL;};
//int *HalfEdge::x(){return &1;}

/*Solid Constructors*/
Solid::Solid(){
  sfaces=new set<Face *>();/*pointer to list of faces*/
  sedges=new set<Edge *>();/*pointer to list of edges*/
  sverts=new set<Vertex *>();/*pointer to list of vertices*/
  cout<<"sfaces="<<sfaces<<endl;

}

Face::Face(){
}

Face::Face(Solid* s){
  s->sfaces->insert(this);
}

Edge::Edge(Solid *s){
  s->sedges->insert(this);
}

Edge::Edge(){
}

Loop::Loop(Face *f){
  lface=f;
  f->floops->insert(this);
}

Loop::Loop(){
}
