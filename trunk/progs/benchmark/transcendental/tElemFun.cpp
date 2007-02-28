
// This program MUST be run at CORE_LEVEL 3:
#define CORE_LEVEL 3

#include "CORE.h"
#include <map>

using namespace std;

#define DEFAULT_SIN 

void function_test (string strFun, string strVal, int outDigit, 
                    bool timing = false, bool compatible = false,
                    string strMaple = string(""))
{
  Expr funVal;
  prec_t relPrec = digits2bits(outDigit);

  Timer2 timer;
  timer.start();

  if (strFun == "sin")
     funVal = sin(Expr(strVal));
  if (strFun == "cos")
     funVal = cos(Expr(strVal));
  if (strFun == "tan")
     funVal = tan(Expr(strVal));
  if (strFun == "cot")
     funVal = cot(Expr(strVal));
  if (strFun == "arcsin")
     funVal = arcsin(Expr(strVal));
  if (strFun == "arccos")
     funVal = arccos(Expr(strVal));
  if (strFun == "arctan")
     funVal = arctan(Expr(strVal));
  if (strFun == "exp")
     funVal = exp(Expr(strVal));
  if (strFun == "log2")
     funVal = log_2(Expr(strVal));

  funVal.approx(relPrec, CORE_INFTY);

  timer.stop();

  cout << "compute " << strFun << "(" << strVal << ")" << " to " << relPrec << " relative precision : ";
  cout << funVal << std::endl;

  if (timing)
    cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;

  if (compatible && isCompatible(funVal, outDigit, strMaple))
    cout << "CORRECT!!!" <<   strFun << "(" << strVal << ")" << " is correponding to the same of Maple" << endl;
  else if (compatible) {
    cout << "ERROR!!!" <<   strFun << "(" << strVal << ")" << " is not correponding to the same of Maple" << endl;
    cout << "CORE2 : " << funVal << std::endl;
    cout << "Maple : " << strMaple << std::endl;
  }
}

int main( int argc, char *argv[] ) {

  // default function values with parameter 0.7 and output digits 30
  map<string, string> strMap;

  strMap.insert(pair<string, string>("sin",    "0.644217687237691053672614351399"));
  strMap.insert(pair<string, string>("cos",    "0.764842187284488426255859990192"));
  strMap.insert(pair<string, string>("tan",    "0.842288380463079448128135002213"));
  strMap.insert(pair<string, string>("cot",    "1.18724183212667935367236269369"));
  strMap.insert(pair<string, string>("arcsin", "0.775397496610753063740353352715"));
  strMap.insert(pair<string, string>("arccos", "0.795398830184143555490968338925"));
  strMap.insert(pair<string, string>("arctan", "0.610725964389208616543758876490"));
  strMap.insert(pair<string, string>("exp",    "2.01375270747047652162454938858"));
  strMap.insert(pair<string, string>("log2",   "-0.514573172829758240428350112255"));

  string strFun("sin");
  string strVal("0.7");
  int    outDigit = 30;;
  prec_t relPrec;

  if (argc < 2) {
    cout << "usage : ./tElemFun function [value] [output digits]" << endl;
    cout << "running default test : comparision results with those of Maple" << endl;

    setDefaultOutputDigits(outDigit);

    function_test("sin", strVal, outDigit, false, true, strMap["sin"]);  
    function_test("cos", strVal, outDigit, false, true, strMap["cos"]);  
    function_test("tan", strVal, outDigit, false, true, strMap["tan"]);  
    function_test("cot", strVal, outDigit, false, true, strMap["cot"]);  
    function_test("arcsin", strVal, outDigit, false, true, strMap["arcsin"]);  
    function_test("arccos", strVal, outDigit, false, true, strMap["arccos"]);  
    function_test("arctan", strVal, outDigit, false, true, strMap["arctan"]);  
    function_test("exp", strVal, outDigit, false, true, strMap["exp"]);  
    function_test("log2", strVal, outDigit, false, true, strMap["log2"]);  

    return 0;
  }

  if (argc > 1)
    strFun = argv[1];

  if (argc > 2)
    strVal = argv[2];

  if (argc > 3)
    outDigit = atoi(argv[3]);

  relPrec = digits2bits(outDigit); 

  setDefaultOutputDigits(outDigit);

  function_test(strFun, strVal, relPrec, true);

  return 0;
}
