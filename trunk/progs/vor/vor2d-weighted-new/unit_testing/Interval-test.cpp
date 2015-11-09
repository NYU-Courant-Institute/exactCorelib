#include <assert.h>
#include <iostream>
#include "../Interval.h"

const Interval p(1, 2);
const Interval n(-5, -3);
const Interval m(-7, 11);

void test_un_minus() {
  Interval i = -m;
  assert(i.a_ == -11 && i.b_ == 7);
}

void test_mult() {
  Interval i = n * m;
  assert(i.a_ == -55 && i.b_ == 35);
}

int main(int argc, char** argv) {
  test_un_minus();
  test_mult();
  cout << "All tests passed.\n";
  return 0;
}
