%{
#include <iostream>
#include <map>
#include "poly_types.h"

using namespace std;

int yylex();
int yyerror(const char *p) { std::cerr << "error: " << p << std::endl; };
%}

%union {
    double num;
    int pow;
    mon_inside_ mon_inside;
    mon_ mon;
};

%start poly

%token PLUS POW
%token X Y
%token <num> NUM

%type <pow>	   x_pow y_pow
%type <mon>	   mono
%type <mon_inside> mono_inside

%%

poly :  mono           { cout << $1.coeff << " x^" << $1.xpow << " y^" << $1.ypow << "\n"; }
     | poly PLUS mono  { cout << "\n"; }
;

mono : NUM mono_inside    { $$ = {$1, $2.xpow, $2.ypow }; }
       	| 	mono_inside     { $$ = {1.0, $1.xpow, $1.ypow}; }
       	| 	NUM             { $$ = {$1, 0, 0}; }
;

mono_inside : x_pow            { $$ = {$1, 0}; }
	|     	y_pow            { $$ = {0, $1}; }
	|     	x_pow y_pow      { $$ = {$1, $2}; }
        |     	y_pow x_pow      { $$ = {$2, $1}; }
;

x_pow : X POW NUM     { $$ = (int) $3; }
;

y_pow : Y POW NUM     { $$ = (int) $3; }
;

%%

int main()
{
    yyparse();
    return 0;
}

