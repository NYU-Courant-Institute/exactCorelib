#include "Box.h"
#include <assert.h>
#include <math.h>

#define OVERRIDE_NEW_DELETE
#include "MemProCpp\MemPro.cpp"

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

bool Box::split2D( double epsilon, vector<Box*>& chldn )
{
	//todo do sth to return false
	if (this->height < epsilon || this->width < epsilon)
	{
		return 0;
	}

	if (!this->isLeaf || this->status == FREE || this->status == STUCK)
	{
		return 0; 
	}

	Box* children[4];
	//same as quadrants I, II, III, IV
	children[2] = new Box(x - width / 4, y + height / 4, width / 2, height / 2);
	children[1] = new Box(x + width / 4, y + height / 4, width / 2, height / 2);
	children[0] = new Box(x + width / 4, y - height / 4, width / 2, height / 2);
	children[3] = new Box(x - width / 4, y - height / 4, width / 2, height / 2);
	for (int i = 0; i < 4; ++i)
	{
		children[i]->depth = this->depth + 1;
		children[i]->isBig = this->isBig;
		children[i]->pParent = this;
		children[i]->tChildID = i;

		children[i]->xi[0] = this->xi[0];
		children[i]->xi[1] = this->xi[1];
	}

	children[0]->Nhbrs[1].push_back(children[1]);
	children[0]->Nhbrs[2].push_back(children[3]);
	children[1]->Nhbrs[2].push_back(children[2]);
	children[1]->Nhbrs[3].push_back(children[0]);
	children[2]->Nhbrs[3].push_back(children[3]);
	children[2]->Nhbrs[0].push_back(children[1]);
	children[3]->Nhbrs[0].push_back(children[0]);
	children[3]->Nhbrs[1].push_back(children[2]);

	for (int i = 0; i < 6; ++i)
	{
		for (vector<Box*>::iterator iter = this->Nhbrs[i].begin(); iter != this->Nhbrs[i].end(); ++iter)
		{
			Box* b = *iter;
			int foundDst = 0;
			for (int j = 0; j < 4; ++j)
			{
				int idx = isNhbr(children[j], b);
				if (idx != -1)
				{
					children[j]->Nhbrs[idx].push_back(b);
					if (foundDst == 0)
					{
						for (vector<Box*>::iterator it = b->Nhbrs[oppositeDir[idx]].begin();
							it != b->Nhbrs[oppositeDir[idx]].end(); ++it)
						{
							if (*it == this)
							{
								*it = children[j]; 
							}
						}
					} 
					else
					{
						b->Nhbrs[oppositeDir[idx]].push_back(children[j]);
					}						
					++foundDst;
				}					
			}
			assert(foundDst > 0);
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		//add all of parent's walls and corners to each child,
		//will be filtered later in updatestatus()
		children[i]->walls.insert(children[i]->walls.begin(),				 
			this->walls.begin(), this->walls.end());
		children[i]->corners.insert(
			children[i]->corners.begin(),
			this->corners.begin(), this->corners.end());			
	}

	this->isLeaf = false;

	for (int i = 0; i < 4; ++i)
	{
		pAllLeaf->push_back(children[i]);
		chldn.push_back(children[i]);
	}
	//list<Box*>::iterator it = find(pAllLeaf->begin(), pAllLeaf->end(), this);
	//pAllLeaf->erase(it);

	return true;
}

bool Box::split3D( double epsilon, vector<Box*>& chldn )
{
	vector<Box*> bv;
	if (this->splitAngle(epsilon, bv))
	{
		for (int i = 0; i < 2; ++i)
		{
			if (!bv[i]->split2D(epsilon, chldn))
			{
				return false;
			}				
		}	
		return true;
	}
	return 0;
}

bool Box::splitAngle( double epsilon, vector<Box*>& chldn )
{
	//todo do sth to return false
	if ( this->xi[1] > this->xi[0] )
	{
		//todo, 0.028 is somehow random
		if ( (this->xi[1] - this->xi[0]) * PI < 0.014)
		{
			return 0;
		}
	} 
	else
	{
		if ( (2 + this->xi[1] - this->xi[0]) * PI < 0.014)
		{
			return 0;
		}
	}

	//if (!this->isLeaf || this->status != MIXED)
	if (!this->isLeaf || this->status == FREE || this->status == STUCK)
	{
		return 0; 
	}

	Box* children[2];
	children[1] = new Box(x, y, width, height);
	children[0] = new Box(x, y, width, height);
	for (int i = 0; i < 2; ++i)
	{
		//children[i]->depth = this->depth + 1;
		children[i]->isBig = false;
		children[i]->pParent = this;
		children[i]->rChildID = i;
	}
	//todo, 0(2) is within [xi[0], xi[1]] shouldn't happen
	children[0]->xi[0] = this->xi[0];
	children[0]->xi[1] = this->xi[0] + (this->xi[1] - this->xi[0]) / 2;
	children[1]->xi[0] = this->xi[0] + (this->xi[1] - this->xi[0]) / 2;
	children[1]->xi[1] = this->xi[1];

	//simple case
	children[0]->Nhbrs[4].push_back(children[1]);
	children[1]->Nhbrs[5].push_back(children[0]);

	for (int i = 0; i < 6; ++i)
	{
		for (vector<Box*>::iterator iter = this->Nhbrs[i].begin(); iter != this->Nhbrs[i].end(); ++iter)
		{
			Box* b = *iter;
			int foundDst = 0;
			for (int j = 0; j < 2; ++j)
			{
				int idx = isNhbr(children[j], b);
				if (idx != -1)
				{
					children[j]->Nhbrs[idx].push_back(b);
					if (foundDst == 0)
					{
						for (vector<Box*>::iterator it = b->Nhbrs[oppositeDir[idx]].begin();
							it != b->Nhbrs[oppositeDir[idx]].end(); ++it)
						{
							if (*it == this)
							{
								*it = children[j]; 
								break;
							}
						}
					} 
					else
					{
						b->Nhbrs[oppositeDir[idx]].push_back(children[j]);
					}						
					++foundDst;
				}					
			}
			assert(foundDst > 0 && foundDst < 3);
		}
	}

	for (int i = 0; i < 2; ++i)
	{
		//add all of parent's walls and corners to each child,
		//will be filtered later in updatestatus()
		children[i]->walls.insert(children[i]->walls.begin(),				 
			this->walls.begin(), this->walls.end());
		children[i]->corners.insert(
			children[i]->corners.begin(),
			this->corners.begin(), this->corners.end());			
	}

	this->isLeaf = false;

	for (int i = 0; i < 2; ++i)
	{
		pAllLeaf->push_back(children[i]);
		chldn.push_back(children[i]);
	}
	//list<Box*>::iterator it = find(pAllLeaf->begin(), pAllLeaf->end(), this);
	//pAllLeaf->erase(it);

	return true;
}

bool Box::split( double epsilon, vector<Box*>& chldn )
{
	if (!this->isBig)
	{
		return split3D(epsilon, chldn);
	} 
	else if( rB / 2 < r0 * 2 / THETA_MIN / 20 )
	{
		//int n = ceil( 2 / THETA_MIN );
		//int m = 1;
		//for (int i = 0; i < n; ++i)
		//{
		//	splitAngle(epsilon);
		//}
		//return splitAngle(epsilon);

		int n = ceil( 2 / THETA_MIN );
		//n = 64;
		int m = 1;
		recursiveSplitAngle(epsilon, chldn, n, m);
		return true;
	}
	else
	{
		return split2D(epsilon, chldn);
	}
}

void Box::recursiveSplitAngle( double epsilon, vector<Box*>& chldn, const int n, int m )
{
	if (m >= n)
	{
		chldn.push_back(this);
		return;
	}
	vector<Box*> bv;
	splitAngle(epsilon, bv);
	for (int i = 0; i < (int)bv.size(); ++i)
	{
		bv[i]->recursiveSplitAngle(epsilon, chldn, n, 2*m);
	}
}

Box::Status Box::checkChildStatus( double x, double y )
{
	//assert(walls.size());

	Wall* nearestWall;
	list<Wall*>::iterator iterW = walls.begin();
	double mindistW = (*iterW)->distance(x, y);
	nearestWall = *iterW;
	++iterW;
	for (; iterW != walls.end(); ++iterW)
	{
		Wall* w = *iterW;
		double dist = w->distance(x, y);
		if (dist < mindistW)
		{
			mindistW = dist;
			nearestWall = *iterW;
		}
	}

	double mindistC = mindistW +1; //mindistC may not exist, so init to a bigger number
	Corner* nearestCorner = NULL;
	if (corners.size())
	{			
		list<Corner*>::iterator iterC = corners.begin();
		mindistC = (*iterC)->distance(x, y);
		nearestCorner = *iterC;
		++iterC;
		for (; iterC != corners.end(); ++iterC)
		{
			Corner* c = *iterC;
			double dist = c->distance(x, y);
			if (dist < mindistC)
			{
				mindistC = dist;
				nearestCorner = *iterC;
			}
		}
	}

	bool isFree = false;

	//nearest feature is a wall
	if (mindistW < mindistC)
	{
		if (nearestWall->isRight(x, y))
		{
			isFree = true;
		}
	}		
	//otherwise check the corner's convexity
	//if convex, out; if concave, in
	//note that if a wall and a corner are the same dist, 
	//only need to take care of the corner
	else
	{
		if (nearestCorner->isConvex())
		{
			isFree = true;
		}
	}

	if (isFree)
	{
		return FREE;
	}
	return STUCK;
}

int Box::isNhbr( Box* b1, Box* b2 )
{
	double xmin1 = b1->x - b1->width / 2;
	double xmax1 = b1->x + b1->width / 2;
	double ymin1 = b1->y - b1->height / 2;
	double ymax1 = b1->y + b1->height / 2;
	double xmin2 = b2->x - b2->width / 2;
	double xmax2 = b2->x + b2->width / 2;
	double ymin2 = b2->y - b2->height / 2;
	double ymax2 = b2->y + b2->height / 2;

	if (isEq(xmin1, xmax2) && isOneContainAnotherY(b1, b2) && isOneContainAnotherA(b1, b2))
	{
		return 2;
	}
	if (isEq(xmax1, xmin2) && isOneContainAnotherY(b1, b2) && isOneContainAnotherA(b1, b2))
	{
		return 0;
	}
	if (isEq(ymin1, ymax2) && isOneContainAnotherX(b1, b2) && isOneContainAnotherA(b1, b2))
	{
		return 3;
	}
	if (isEq(ymax1, ymin2) && isOneContainAnotherX(b1, b2) && isOneContainAnotherA(b1, b2))
	{
		return 1;
	}
	//down
	if (isEq(b1->xi[0], b2->xi[1]) && isOneContainAnotherX(b1, b2) && isOneContainAnotherY(b1, b2))
	{
		return 5;
	}
	//up
	if (isEq(b1->xi[1], b2->xi[0]) && isOneContainAnotherX(b1, b2) && isOneContainAnotherY(b1, b2))
	{
		return 4;
	}

	return -1;
}

void Box::updateStatusBig()
{
	if (status != UNKNOWN)
	{
		return;
	}

	double outerDomain = r0 + rB;
	double innerDomain = r0 > rB ? r0 - rB : 0;
	for (list<Corner*>::iterator it = corners.begin(); it != corners.end(); )
	{
		Corner* c = *it;
		if (c->distance(this->x, this->y) <= innerDomain)
		{
			status = STUCK;
			return;
		}
		else if( c->distance(this->x, this->y) <= outerDomain ) {
			status = MIXED;
			++it;
		}
		else {
			it = corners.erase(it);
		}
	}

	for (list<Wall*>::iterator it = walls.begin(); it != walls.end(); )
	{
		Wall* w = *it;
		double distWall = w->distance(this->x, this->y);
		if (distWall < innerDomain)
		{
			status = STUCK;
			return;				
		} 
		else if (distWall <= outerDomain)
		{
			status = MIXED;
			++it;
		}
		else
		{
			it = walls.erase(it);
		}
	}

	if (corners.size() == 0 && walls.size() == 0)
	{
		if (!pParent)
		{
			status = FREE;
		} 
		else
		{
			status = pParent->checkChildStatus(this->x, this->y);
		}			
	}
}

void Box::updateStatusSmall()
{
	if (status != UNKNOWN)
	{
		return;
	}

	double v01x, v01y, v02x, v02y, v11x, v11y, v12x, v12y, v21x, v21y, v22x, v22y;

	getRoundTriVerts(v01x, v01y, v02x, v02y, v11x, v11y, v12x, v12y, v21x, v21y, v22x, v22y);

	Line2d L1(v02x, v02y, v11x, v11y);
	Line2d L2(v12x, v12y, v21x, v21y);
	Line2d L3(v22x, v22y, v01x, v01y);
	bool expandSuccess = L1.expand(rB, L2, L3) && L2.expand(rB, L1, L3) && L3.expand(rB, L1, L2) && !L1.isNegative(L2, L3);
	assert(expandSuccess);


	Line2d L1a(v01x, v01y, v12x, v12y);
	Line2d L2a(v11x, v11y, v22x, v22y);
	Line2d L3a(v21x, v21y, v02x, v02y);
	bool shrinkSuccess = L1a.expand(-rB, L2a, L3a) 
		&& L2a.expand(-rB, L1a, L3a) && L3a.expand(-rB, L1a, L2a) && !L1a.isNegative(L2a, L3a);

	double x23, y23;
	L2a.intersection(L3a, x23, y23);


	double X12x, X12y;
	L1.intersection(L2, X12x, X12y);
	double X23x, X23y;
	L2.intersection(L3, X23x, X23y);
	double X31x, X31y;
	L3.intersection(L1, X31x, X31y);

	for (list<Corner*>::iterator it = corners.begin(); it != corners.end(); )
	{
		Corner* c = *it;
		double cx = c->x - this->x;
		double cy = c->y - this->y;

		if (shrinkSuccess && !L1a.isRight(cx, cy) && !L2a.isRight(cx, cy) && !L3a.isRight(cx, cy))
		{
			status = STUCK;
			return;
		}

		if ( !(sqrt(cx*cx + cy*cy) < r0 + rB
			&& !Line2d::isRight(X31x, X31y, X12x, X12y, cx, cy)
			&& !Line2d::isRight(X12x, X12y, X23x, X23y, cx, cy)
			&& !Line2d::isRight(X23x, X23y, X31x, X31y, cx, cy)
			) )
		{
			it = corners.erase(it);
		}
		else
		{
			status = MIXED;
			++it;
		}
	}

	for (list<Wall*>::iterator it = walls.begin(); it != walls.end(); )
	{
		Wall* w = *it;
		double distWall = w->distance(this->x, this->y);
		if (distWall < r0 + rB)
		{
			double srcx = w->src->x - this->x;
			double srcy = w->src->y - this->y;
			double dstx = w->dst->x - this->x;
			double dsty = w->dst->y - this->y;

			//quick stuck detection
			if (shrinkSuccess) //inner triangle (before shrink rB) is not negative
			{
				double x23s, y23s;
				L2a.intersection(L3a, x23s, y23s);
				double x13s, y13s;
				L1a.intersection(L3a, x13s, y13s);
				double x12s, y12s;
				L1a.intersection(L2a, x12s, y12s);


				//if src or dst is in triangle
				if ( (!L1a.isRight(srcx, srcy)
					&& !L2a.isRight(srcx, srcy)
					&& !L3a.isRight(srcx, srcy)) ||
					(!L1a.isRight(dstx, dsty)
					&& !L2a.isRight(dstx, dsty)
					&& !L3a.isRight(dstx, dsty))
					)
				{
					status = STUCK;
					return;
				}
				// or line seg (src,dst) intersects any edge of triangle
				else if ( Line2d::lineSegIntsct(x23s, y23s, x13s, y13s, srcx, srcy, dstx, dsty)
					|| Line2d::lineSegIntsct(x13s, y13s, x12s, y12s, srcx, srcy, dstx, dsty)
					|| Line2d::lineSegIntsct(x12s, y12s, x23s, y23s, srcx, srcy, dstx, dsty) )
				{
					status = STUCK;
					return;
				}					
			}

			//if src or dst is in triangle
			if ( (!Line2d::isRight(X31x, X31y, X12x, X12y, srcx, srcy)
				&& !Line2d::isRight(X12x, X12y, X23x, X23y, srcx, srcy)
				&& !Line2d::isRight(X23x, X23y, X31x, X31y, srcx, srcy)) ||
				(!Line2d::isRight(X31x, X31y, X12x, X12y, dstx, dsty)
				&& !Line2d::isRight(X12x, X12y, X23x, X23y, dstx, dsty)
				&& !Line2d::isRight(X23x, X23y, X31x, X31y, dstx, dsty))
				)
			{
				status = MIXED;
				++it;
			}
			// or line seg (src,dst) intersects any edge of triangle
			else if ( Line2d::lineSegIntsct(X31x, X31y, X12x, X12y, srcx, srcy, dstx, dsty)
				|| Line2d::lineSegIntsct(X12x, X12y, X23x, X23y, srcx, srcy, dstx, dsty)
				|| Line2d::lineSegIntsct(X23x, X23y, X31x, X31y, srcx, srcy, dstx, dsty) )
			{
				status = MIXED;
				++it;
			}
			else
			{
				it = walls.erase(it);
			}								
		} 
		else
		{
			it = walls.erase(it);
		}
	}

	if (corners.size() == 0 && walls.size() == 0)
	{
		if (!pParent)
		{
			status = FREE;
		} 
		else
		{
			status = pParent->checkChildStatus(this->x, this->y);
		}			
	}
}