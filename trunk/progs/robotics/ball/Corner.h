#pragma once
#include <math.h>
class Edge;

class Corner
{
public:
	double x, y, z;
	Edge* preEdge;
	Edge* nextEdge;
	// Constructor from (x,y,z):
	Corner(double xx, double yy, double zz):x(xx), y(yy), z(zz)
	{

	}
	// Constructor from 3 corners (c0, c1, c2):
	//     the resulting corner is given by:
	//     		c0+(c1-c0)+(c2-c0) = c1+c2-c0.
	Corner(Corner *c0, Corner *c1, Corner *c2):
	    	x(c1->x + c2->x - c0->x),
		y(c1->y + c2->y - c0->y),
		z(c1->z + c2->z - c0->z)
	{

	}

	double distance(double x2, double y2, double z2)
	{
		return sqrt( (x-x2)*(x-x2) + (y-y2)*(y-y2) + (z-z2)*(z-z2) );
	}	
};
