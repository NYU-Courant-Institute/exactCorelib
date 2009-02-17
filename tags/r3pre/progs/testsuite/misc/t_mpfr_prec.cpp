/* this program is used to test the relative precision of IEEE double.

            x = 1.0000000000...0
   +  epsilon = 0.0000000000...01 (52-bits)
   ------------------------------
              = 1.00000000000000

   The program will print out "52" as result.
 */

#include <iostream>
#include <cmath>
#include <CORE/BigFloat.h>

using namespace CORE;
using namespace std;

int main() {
  prec_t prec = 100;

  BigFloat x;
  x.set_prec(prec);
  x.set(1);
  
  BigFloat y;
  y.set_prec(prec);

  BigFloat epsilon;
  for (int i=prec-10; i<prec+10; i++) {
    epsilon.set_prec(i);
    epsilon.set_2exp(1, -i);
    y.add(x, epsilon);
    std::cout << "x=" << x << std::endl;
    std::cout << "y=" << y << std::endl;
    if (y.cmp(x) == 0) {
      cout << "(i-1)=" << (i-1) << endl;
      break;
    }
  }
  return 0;
}
