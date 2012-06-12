#include "Euler_Ops.h"

int main(){
  Euler_Ops *eo=new Euler_Ops();
  Solid * s=eo->mvfs(1,1,1,0.0,0.0,0.0);

  Vec<Solid *> *ss=eo->solids;
  cout<<ss->size()<<endl;
  cout<<(*ss)[0]<<endl;
  cout<<s<<endl;
  Vec<Face *> *fs=s->sfaces;
  Vec<Edge *> *es=s->sedges;
  Vec<Vertex *> *vs=s->sverts;
  Vec<Loop *> *ls=(*fs)[0]->floops;
  Loop *l=(*fs)[0]->flout;

  
  s->print();

  int check=eo->mev(1,1,1,1,1,1,1,1,0,0);

  if(check!=SUCCESS)
    cout<<"We cannot build the point 1,0,0 and the edge."<<endl;

  s->print();

  cout<<"Solid no"<<s->solidno<<endl;

  cout<<"sfaces ="<<fs<<endl;
  cout<<"sfaces size="<<fs->size()<<endl;
  cout<<"face no="<<(*fs)[0]->faceno<<endl;

  cout<<"sedges ="<<es<<endl;
  cout<<"sedges size="<<es->size()<<endl;

  cout<<"sverts ="<<vs<<endl;
  cout<<"sverts size="<<vs->size()<<endl;
  cout<<"vertex no="<<(*vs)[0]->vertexno<<endl;

  cout<<"Loops size="<<ls->size()<<endl;
  cout<<"Loop 0 = "<<(*ls)[0]<<endl;
  cout<<"Outer Loop="<<l<<endl;
  l->print();
  HalfEdge *he=l->ledg;
  he->print();
  
  he->nxthe->print();
  he->prvhe->print();

  cout<<"he= "<<he<<endl;
  cout<<"nxthe= "<<he->nxthe<<endl;
  cout<<"prvhe= "<<he->prvhe<<endl;
  cout<<"he start vertex"<<he->start<<endl;
  cout<<"nxthe start vertex"<<he->nxthe->start<<endl;
  
  
  

}
