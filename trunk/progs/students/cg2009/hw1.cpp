#ifndef CORE_LEVEL
#define CORE_LEVEL 1
#endif
#include <CORE.h>
#include <CORE/geometry2d.h>

using namespace std;

int main() {
  int ansCorr(0), ansIncorr(0);
  Line2d line1(1,1,-1);

  for(double a = 1.0; a <= 50.0; ++a) {
    for(double b = 1.0; b <= 50.0; ++b) {

      Line2d line2(a, b, 0);

      Point2d* ptIntersect(NULL);
      ptIntersect = dynamic_cast<Point2d*>(line1.intersection(line2));

      if (ptIntersect == NULL) {
        ansCorr++;
        continue;
      }

      if (line1.contains(*ptIntersect) && line2.contains(*ptIntersect)) 
        ansCorr++;
      else
        ansIncorr++;
    }
  }

  cout << "Correct Tests = " << ansCorr << endl;
  cout << "Incorrect Tests = " << ansIncorr << endl;

  return 0;
}

