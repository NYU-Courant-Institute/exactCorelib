using namespace std;
#include <iostream>
#include <vector>
#include <set>
#include "x.h"
A::A(int a){
  this->a=a;
}
class B
{
  vector<int> v;
  B(int a){
  }
  
};
/*int main(){
  vector<int> v;
  v.insert(v.begin(),3);

  vector<int> v2(10);
  for (int i=0;i<v2.size();i++)
    v2[i]=i;
   
  v2.insert(v2.end(),100);
  for (int i=0;i<v2.size();i++)
    cout<<v2[i]<<endl;
  vector<int>::iterator I;
  for (I=v2.begin();I!=v2.end();++I)
  cout<<*I<<endl;
  vector<int> vx(20);
  cout<<"vx=";
  for (int i=0;i<vx.size();i++)
    cout<<vx[i]<<' ';
    cout<<"\n\n"; 

  vx.swap(v2);
  cout<<"\n vx apfter swapping";
  for (int i=0;i<vx.size();i++)
    cout<<vx[i]<<' ';
  cout<<endl;
  for (int i=0;i<v2.size();i++)
    cout<<v2[i]<<' ';
    cout<<endl;

  set<int> set1;
  set<int>::iterator iter;
  int i;
  set1.insert(3);
  set1.insert(6);
  set1.insert(8);
  for (int i=0;i<10;i++)
  {
    iter=set1.find(i);
    if(iter !=set1.end())
      cout<<i<< " is in the set\n";
  }
};*/
