//
// test all functions of BigInt
//
#include <CORE/BigInt.h>
#include <iostream>

typedef BigInt NT;

int main() {
  // testing constructors
  NT x0;
  NT xx(x0);
  NT x1(char(1));
  NT x2((unsigned char)(1));
  NT x3(short(1));
  NT x4((unsigned short)(1));
  NT x5(int(1));
  NT x6((unsigned int)(1));
  NT x7(long(1));
  NT x8((unsigned long)(1));
  NT x9(1.0f);
  NT x10(1.0);
  NT x11("12345");
  NT x12(std::string("12345"));
  
  // test assignment
  x0.set(xx);
  x1.set(char(1));
  x2.set((unsigned char)(1));
  x3.set(short(1));
  x4.set((unsigned short)(1));
  x5.set(int(1));
  x6.set((unsigned int)(1));
  x7.set(long(1));
  x8.set((unsigned long)(1));
  x9.set(1.0f);
  x10.set(1.0);
  x11.set("12345");
  x12.set(std::string("12345"));

  // test conversion functions

  // test arithmetic functions
  xx.add(x0, char(1));
  xx.add(x0, (unsigned char)(1));
  xx.add(x0, short(1));
  xx.add(x0, (unsigned short)(1));
  xx.add(x0, int(1));
  xx.add(x0, (unsigned int)(1));
  xx.add(x0, long(1));
  xx.add(x0, (unsigned long)(1));
  xx.add(x0, 1.0f);
  xx.add(x0, 2.3);

  xx.add(char(1), x0);
  xx.add((unsigned char)(1), x0);
  xx.add(short(1), x0);
  xx.add((unsigned short)(1), x0);
  xx.add(int(1), x0);
  xx.add((unsigned int)(1), x0);
  xx.add(long(1), x0);
  xx.add((unsigned long)(1), x0);
  xx.add(1.0f, x0);
  xx.add(2.3, x0);

  // test c++ operator
  NT x(210);
  NT y(10);
  NT z;

  y++;
  std::cout << "y=" << y << std::endl;
  ++y;
  std::cout << "y=" << y << std::endl;
  x += 2000;
  std::cout << "x=" << x << std::endl;
  x += y;
  std::cout << "x=" << x << std::endl;

  z = x + y;
  std::cout << "z=" << z << std::endl;
  z = x + 100;
  std::cout << "z=" << z << std::endl;
  z = 100 + x;
  std::cout << "z=" << z << std::endl;
  std::cout << "x=" << x << std::endl;
  std::cout << "y=" << y << std::endl;
  
  // test comparison operator
  bool b;
  b = x == y;
  b = x == 100;
  b = 100 == x;

  return 0;
}

