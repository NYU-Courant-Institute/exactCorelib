#include <assert.h>
#include <iostream>
#include "../BiPoly.h"

void test_creation() {
  BiPoly p;
  p.add_monomial(1, 0, 0);
}

void test_mul() {
  BiPoly p;
  p.add_monomial(1, 1, 0);
  p.add_monomial(1, 0, 0);

  BiPoly q;
  q.add_monomial(1, 0, 1);
  
  BiPoly r;
  r.add_monomial(1, 0, 1);
  r.add_monomial(1, 1, 1);

  BiPoly pq = p * q;
  
  assert(r == pq);
}

int main(int argc, char** argv) {
  test_creation();
  test_mul();
  cout << "All tests passed.\n";
  return 0;
}
