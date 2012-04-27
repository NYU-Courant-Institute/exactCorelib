#include "Box.h"
#include <assert.h>
#include <math.h>

extern double triRobo[3];

double Box::r0 = 0;
double Box::THETA_MIN = 0;
int Box::counter = 0;

vector<Box*>* Box::pAllLeaf = 0;

int Box::oppositeDir[6] = {2, 3, 0, 1, 5, 4};


void Box::getRoundTriVerts(double& v01x, double& v01y, double& v02x, double& v02y, double& v11x, double& v11y,
					  double& v12x, double& v12y, double& v21x, double& v21y, double& v22x, double& v22y)
{
	double r = r0;
	double theta1 = triRobo[0];
	double theta2 = triRobo[1];
	double xi1 = this->xi[0];
	double xi2 = this->xi[1];

	v01x = r * cos(xi1 * PI);
	v01y = r * sin(xi1 * PI);
	v02x = r * cos(xi2 * PI);
	v02y = r * sin(xi2 * PI);

	v11x = r * cos((xi1 + theta1) * PI);
	v11y = r * sin((xi1 + theta1) * PI);
	v12x = r * cos((xi2 + theta1) * PI);
	v12y = r * sin((xi2 + theta1) * PI);

	v21x = r * cos((xi1 + theta2) * PI);
	v21y = r * sin((xi1 + theta2) * PI);
	v22x = r * cos((xi2 + theta2) * PI);
	v22y = r * sin((xi2 + theta2) * PI);
}
