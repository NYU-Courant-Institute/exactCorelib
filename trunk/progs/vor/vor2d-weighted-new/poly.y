%{
#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

int yylex();
int yyerror(const char *p) { std::cerr << "error: " << p << std::endl; };
%}

%union {
    double coeff;
    int pow;
    tuple<int, int>         mon_inside; // x^b * y^c
    tuple<double, int, int> mon;        // (a, b, c) <-> a * x^b * y^c
};

%start poly

%token PLUS POW
%token X Y
%token <coeff> NUM

%type <pow>	   x_pow y_pow
%type <mon>	   mono
%type <mon_inside> mono_inside

%%

poly : mono       { }
     | poly PLUS mono
;

mono : NUM mono_inside         { $$ = make_tuple($1,  get<0>($2), get<1>($2)); }
	| mono_inside          { $$ = make_tuple(1.0, get<0>($1), get<1>($1)); }
	| NUM                  { $$ = make_tuple($1, 0, 0); }
;

mono_inside : x_pow          { $$ = make_tuple($1, 0); }
	|     y_pow          { $$ = make_tuple(0, $1); }
	|     x_pow y_pow    { $$ = make_tuple($1, $2); }
	|     y_pow x_pow    { $$ = make_tuple($2, $1); }
;

x_pow : X POW NUM     { $$ = (int) $3; }
;

y_pow : Y POW NUM     { $$ = (int) $3; }
;

%%
