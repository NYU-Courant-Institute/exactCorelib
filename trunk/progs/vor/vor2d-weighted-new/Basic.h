//------------------------------------------------------------------------------
//  Copyright 2010-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#pragma once
#ifndef _MATHTOOL_UTILITY
#define _MATHTOOL_UTILITY

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <iostream>   // define C++ stream I/O routines
#include <iomanip>
using namespace std;

/* Miscellaneous Scalar Math */
#define sqr(x)      ((x) * (x))
#define sign(x)     ((x)>=0? 1: -1)
#define applysign(x, y) ((y) >= 0? abs(x): -abs(x))

/* Angle Conversions & Constants */
#ifndef PI
#define PI ((double)3.1415926535897)
#endif

#ifndef PI2
#define PI2 ((double)6.2831853071794)
#endif

#define RAD2DEG (180/PI)
#define DEG2RAD (PI/180)

#define DegToRad(x) ((x)*DEG2RAD)
#define RadToDeg(x) ((x)*RAD2DEG)

#endif //_MATHTOOL_UTILITY
