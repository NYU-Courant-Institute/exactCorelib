#include <CORE/CORE.h>

typedef BigInt NT;
typedef Polynomial<NT> PolyNT;
typedef Descartes<BigFloat> DescartesNT;

using namespace std;

int main(int argc, char** argv){

cout <<"=============================================================\n"
      << "TEST 6:   Wilkinson Polynomial of degree 6 (roots are 1,..,6)\n" <<
      "=============================================================" << endl;
  NT cs5[] = {720, -1764, 1624, -735, 175, -21, 1};
  PolyNT P5(6, cs5);
  DescartesNT D5(P5);

  BFVecInterval v;
  D5.isolateRoots(BigFloat(0), BigFloat(7), v);

  int i=0;
  for (BFVecInterval::const_iterator it = v.begin();
    it != v.end(); ++it) {
    BFInterval vv = D5.refine(*it, 53);
    std::cout << ++i << "th Root is in ["
    << vv.first << " ; " << vv.second << "]" << std::endl;
  }

  return 0;
}

