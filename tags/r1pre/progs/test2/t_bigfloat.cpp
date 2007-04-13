#define CORE_LEVEL 4
#include <CORE/CORE.h>
#include <iostream>

typedef BigFloat2 NT;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

void show_msg(const char* msg)
{ std::cout << msg << std::endl; }

template <typename T>
void test_constructor(const T& v, const char* type) {
  NT x(v);
  std::cout << "constructor for " << type << ": x=" << x << std::endl; 
  NT xx(v, 10);
  std::cout << "constructor for " << type << " with prec=10: xx=" << xx << std::endl; 
}

template <typename T>
void test_set(const T& v, const char* type) {
  NT x, xx;
  x.set(v);
  std::cout << "set for " << type << ": x=" << x << std::endl; 
  xx.set(v, 10);
  std::cout << "set with prec=10 for " << type << ": xx=" << xx << std::endl; 
}

void test_constructors() {
  show_msg("test constructors");
  NT x;
  std::cout << "default constructor: x=" << x << std::endl; 
  test_constructor<char>(1, "char");
  test_constructor<uchar>(2, "uchar");
  test_constructor<short>(3, "short");
  test_constructor<ushort>(4, "ushort");
  test_constructor<int>(5, "int");
  test_constructor<uint>(6, "uint");
  test_constructor<long>(7, "long");
  test_constructor<ulong>(8, "ulong");
  test_constructor<float>(0.1, "float");
  test_constructor<double>(0.3, "double");
  test_constructor<BigInt>(1234567, "BigInt");
  test_constructor<BigRat>(BigRat(1, 3), "BigRat");
  test_constructor<BigFloat>(0.01, "BigFloat");

  show_msg("test assignment functions");
  test_set<char>(1, "char");
  test_set<uchar>(2, "uchar");
  test_set<short>(3, "short");
  test_set<ushort>(4, "ushort");
  test_set<int>(5, "int");
  test_set<uint>(6, "uint");
  test_set<long>(7, "long");
  test_set<ulong>(8, "ulong");
  test_set<float>(0.1, "float");
  test_set<double>(0.3, "double");
  test_set<BigInt>(1234567, "BigInt");
  test_set<BigRat>(BigRat(1, 3), "BigRat");
  test_set<BigFloat>(0.01, "BigFloat");
  x.set_prec(40);
  x.set("1234.567890");
  std::cout << "set for const char*: x=" << x << std::endl; 
  x.set("1234.567890", 10, 10);
  std::cout << "set with prec=10 for const char*: x=" << x << std::endl; 
}

int main() {
  NT f(5);
  NT ff;
  ff.sqrt(f);
  std::cout <<  "sqrt(5)=" << ff << std::endl;
  std::cout <<  "left=" << ff.getLeft() << std::endl;
  std::cout <<  "right=" << ff.getRight() << std::endl;
  if (ff.is_exact()) {
    std::cout <<  "is_exact=" << ff.is_exact() << std::endl;
  } else {
    std::cout << " inexact" << std::endl;
  }
  Expr x(5);
  Expr xx = sqrt(x);
  Expr e = xx * xx - 5;
  if (e == 0)
    std::cout << "ok" << std::endl;
  else
    std::cout << "no" << std::endl;

/*
  BigInt zz(1234567);
  BigFloat f(zz, 10);
  std::cout << "f=" << f << std::endl;

  test_constructors();

  NT x(100);
  NT y(200);
  NT z(100);

  std::cout << "test auto version" << std::endl;
  z.add(x, y);
  std::cout << "z=" << z << std::endl;
  z.add(x, 100);
  std::cout << "z=" << z << std::endl;
  z.add(100, y);
  std::cout << "z=" << z << std::endl;

  std::cout << "test fixed version" << std::endl;
  z.add(x, y, 100);
  std::cout << "z=" << z << std::endl;
  z.add(x, 100, 100);
  std::cout << "z=" << z << std::endl;
  z.add(100, y, 100);
  std::cout << "z=" << z << std::endl;

  std::cout << "test raw version" << std::endl;
  z.set_prec(100);
  z.r_add(x, y);
  std::cout << "z=" << z << std::endl;
  z.r_add(x, 100);
  std::cout << "z=" << z << std::endl;
  z.r_add(100, y);
  std::cout << "z=" << z << std::endl;
  */

/*
  Mpfr ff;
  //ff.set_prec(Mpfr::count_prec("1234.567890", 10));
  for (int p = 5; p<20; p++) {
  ff.set_prec(p);
  ff.set("1234.567890", 10, GMP_RNDD);
  std::cout << ff << std::endl;
  std::cout << ff.get_str(0, 2) << std::endl;
  ff.set("1234.567890", 10, GMP_RNDU);
  std::cout << ff << std::endl;
  std::cout << ff.get_str(0, 2) << std::endl;
  }
*/

  return 0;
}

