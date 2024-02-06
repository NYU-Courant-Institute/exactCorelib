#include "Corner.h"
#include "Wall.h"


bool Corner::isConvex()
{
	Corner* c = this->nextWall->dst;
	return !this->preWall->isRight(c->x, c->y);
}

bool Corner::intersectZone()
{
	return true;  // Chee, feb2024: this SHOULD be implemented to improve
				// the intersection test.  The zone of a corner
				// is the wedge emanating from the corner
				// bounded by rays parallel to the 2 incident walls.
				// Eric, please talk to me about implementing this.
}
