/*
*Kai Cao Jun 2012
*/
#include "Constants.h"
int Vertex::counter=1;
Vertex::Vertex(Id no,double x, double y, double z, Solid *s){
  vertexno=no;
  Vertex(x,y,z,s);
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

Vertex::Vertex(){
}

Vertex::Vertex(Solid* s){
  s->sverts->insert(this);
}

HalfEdge::HalfEdge(Vertex* start,Vertex* end){
  this->start=start;
  this->end=end;
}

HalfEdge::HalfEdge(){
}

/*((HalfEdge) *)HalfEdge::mate(){
  if(!this)
    return NULL;

  if(!edg)
    return NULL;

  if(this==edg->he1)
    return edg->he2;
  else
    return edg->he1;
}*/

Vertex *HalfEdge::x(){}

//HalfEdge *HalfEdge::mate(int a){return NULL;};
//int *HalfEdge::x(){return &1;}

Solid::Solid(){
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
