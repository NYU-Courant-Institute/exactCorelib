
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

  string strfunVal = funVal.toString();
  if (compatible && isCompatible(strfunVal, strMaple))
    cout << "CORRECT!!!" <<   strFun << "(" << strVal << ")" << " is correponding to the same of Maple" << endl;
  else if (compatible) {
    cout << "ERROR!!!" <<   strFun << "(" << strVal << ")" << " is not correponding to the same of Maple" << endl;
    cout << "STRING: " << strfunVal << std::endl;
    cout << "CORE2 : " << funVal << std::endl;
    cout << "UPPER : " << funVal.BigFloat2Value().getRight() << std::endl;
    cout << "LOWER : " << funVal.BigFloat2Value().getLeft() << std::endl;
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
  strMap.insert(pair<string, string>("log2",   "-0.514573172829758240428350112258"));
  
  map<string, string> strMap2;

  strMap2.insert(pair<string, string>("sin",    "0.00099999983333334166666646825397100970015131473480865841900"));
  strMap2.insert(pair<string, string>("cos",    "0.99999950000004166666527777780257936480379188921289614586985"));
  strMap2.insert(pair<string, string>("tan",    "0.00100000033333346666672063494250441803431495977419338549240"));
  strMap2.insert(pair<string, string>("cot",    "999.999666666644444442328042116402095024315082134968695793042"));
  strMap2.insert(pair<string, string>("arcsin", "0.00100000016666674166671130955419149062614469765573280956976"));
  strMap2.insert(pair<string, string>("arccos", "1.56979632662822987756461038208555995147244000203182010091770"));
  strMap2.insert(pair<string, string>("arctan", "0.00099999966666686666652380963492054401162093455426801309143"));
  strMap2.insert(pair<string, string>("exp",    "1.00100050016670834166805575399305831156307620058070146022851"));
  strMap2.insert(pair<string, string>("log2",   "-9.9657842846620870436109582884681705275944941790737418361642"));

  string strFun("sin");
  string strVal("0.7");
  int    outDigit = 30;;
  prec_t relPrec;

  if (argc < 2) {
    cout << "usage : ./tElemFun function [value] [output digits]" << endl;
    cout << "running default test : comparision results with those of Maple" << endl;

    setDefaultOutputDigits(outDigit);

    int argPrec = 100;
    function_test("sin", strVal, outDigit, false, true, strMap["sin"]);  
    function_test("cos", strVal, outDigit, false, true, strMap["cos"]);  
    function_test("tan", strVal, outDigit, false, true, strMap["tan"]);  
    function_test("cot", strVal, outDigit, false, true, strMap["cot"]);  
    function_test("arcsin", strVal, outDigit, false, true, strMap["arcsin"]);  
    function_test("arccos", strVal, outDigit, false, true, strMap["arccos"]);  
    function_test("arctan", strVal, outDigit, false, true, strMap["arctan"]);  
    function_test("exp", strVal, outDigit, false, true, strMap["exp"]);  
    function_test("log2", strVal, outDigit, false, true, strMap["log2"]);  

    outDigit = 50;
    setDefaultOutputDigits(outDigit);
    string strVal("0.001");
    
    argPrec = 250;
    function_test("sin", strVal, outDigit, false, true, strMap2["sin"]);  
    function_test("cos", strVal, outDigit, false, true, strMap2["cos"]);  
    function_test("tan", strVal, outDigit, false, true, strMap2["tan"]);  
    function_test("cot", strVal, outDigit, false, true, strMap2["cot"]);  
    function_test("arcsin", strVal, outDigit, false, true, strMap2["arcsin"]);  
    function_test("arccos", strVal, outDigit, false, true, strMap2["arccos"]);  
    function_test("arctan", strVal, outDigit, false, true, strMap2["arctan"]);  
    function_test("exp", strVal, outDigit, false, true, strMap2["exp"]);  
    function_test("log2", strVal, outDigit, false, true, strMap2["log2"]);  
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
