#pragma once
#include <math.h>
#include "Feature.h"

class Wall;
class Box;

class Corner : public Feature
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

	bool inZone(Box * box);
	bool inZone_star(Box * box);

	bool isConvex();
};
