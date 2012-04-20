/* **************************************
   File: Corner.h

   Description: 

   HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang

   Since Core Library  Version 2.1
   $Id: $
 ************************************** */

#pragma once
#include <math.h>
class Wall;

class Corner
{
public:
	double x, y;
	Wall* preWall;
	Wall* nextWall;

	Corner(double xx, double yy):x(xx), y(yy)
	{

	}

	double distance(double x2, double y2)
	{
		return sqrt( (x-x2)*(x-x2) + (y-y2)*(y-y2) );
	}

};
