#pragma once

#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "Wall.h"
#include "Corner.h"
#include <vector>
#include <list>
#include <set>
#include <float.h>
#include "UnionFind.h"

class Set;
class Box;

using namespace std;

class BoxIter
{
private:
	const Box* b;
	int direction;
	int prev;
	int next;
	int cross;
	Box* neighbor;
public:
	
	BoxIter(const Box* bb, int direc);

	Box* First();

	Box* Next();

	Box* End();
}; //class BoxIter

class BoxNode
{
public:
    BoxNode(){ x=y=clearance=0; }
    list<Corner*> corners;
    list<Wall*> walls;
    double clearance;
    double x, y;
};

class Box
{
private:

    //
	static bool isOverLimit(const Box* base, const Box* nextBox)
	{
		if ((nextBox->x > base->x - base->width / 2 && nextBox->x < base->x + base->width / 2)
			|| (nextBox->y > base->y - base->height / 2 && nextBox->y < base->y + base->height / 2))
		{
			return false;
		}
		return true;
	}

public:
	
	friend class BoxIter;
	friend class QuadTree;
	
	int depth;
	double x, y;
	double width, height;
	int priority;
	bool isLeaf;
	static double r0; //radius of robot
	double rB; //radius of box
	double cl_m; //clearance of the mid point of the box

	static int counter;	// time of expansion (used in BFS strategy)
	
	//Pointers to children, but when no children (i.e., leaf),
	//	the pointers are used as neighbor pointers
	// where
	//	0 = NW, 1 = EN, 2 = SE, 3 = WS
	Box* pChildren[4]; 

//	// box nodes are the 4 corners of the box
//	// the order of the corners are the same as the order of children
//	//    0 = NW, 1 = EN, 2 = SE, 3 = WS
//	BoxNode * pNodes[4];
//	BoxNode * pMid; // the mid point of the box

	Box* pParent; //parent in quadtree
	enum Status { IN, OFF, ON, TRICKY, UNKNOWN };
	                                      // IN: the VOR is inside the box but does not intersect the border of the box
	                                      // OFF: the VOR is outside
	                                      // ON: the VOR is cross the border
	                                      // TRICKY: tricky case (see def below)
	Status status;
	Set* pSet;   //?

	//these are the features contained in this box
	list<Corner*> corners;
	list<Wall*> walls;


	//for shortest path
	double dist2Source;
	int heapId;
	Box* prev;
	bool visited;

	Box(double xx, double yy, double w, double h):
	    	depth(1), x(xx), y(yy), width(w), height(h), isLeaf(true), 
		pParent(0), status(UNKNOWN),
		pSet(0), dist2Source(-1), heapId(-1), prev(0), visited(false)
	{
		for (int i = 0; i < 4; ++i)
		{
			pChildren[i] = 0;
		}
		rB = sqrt(width*width + height*height)/2;
		priority = Box::counter; 
	}
	
	//
	// change this
	//   (1) need clearance
	//   (2) closest feature of m_b and four corners
	//
	void updateStatus()
	{
		if (status != UNKNOWN)
		{
			return;
		}
		
        //
		//int total_feature_size=corners.size()+walls.size();
        int total_feature_size= corners.size()+ walls.size(); //cornerset.size()+wallset.size();

		//C_0
		if(total_feature_size>3){
		    status = IN; //need more split
		    return;
		}

		if(total_feature_size==1){
		    status = OFF;
		    return;
		}

		bool separable=are_features_separable(); //wallset,cornerset);

		//C_1
		if( (cl_m<rB) && separable ){
            status = IN; //need more split
            return;
        }

		//C_2
		if( (cl_m<rB*rB) ){
            status = IN; //need more split
            return;
        }

        //find actual features of the box nodes
        BoxNode UL, LL, UR, LR; //upper left, lower left, upper right, lower right
        UL.x=x-width/2; UL.y=y+height/2;
        LL.x=x-width/2; LL.y=y-height/2;
        UR.x=x+width/2; UR.y=y+height/2;
        LR.x=x+width/2; LR.y=y-height/2;

        determine_clearance(UL);
        determine_clearance(LL);
        determine_clearance(UR);
        determine_clearance(LR);

        //determine the status: either OFF or ON or Tricky
        set<Wall*>   wallset;
        set<Corner*> cornerset;
        wallset.insert(UL.walls.begin(),UL.walls.end());
        wallset.insert(LL.walls.begin(),LL.walls.end());
        wallset.insert(UR.walls.begin(),UR.walls.end());
        wallset.insert(LR.walls.begin(),LR.walls.end());

        cornerset.insert(UL.corners.begin(),UL.corners.end());
        cornerset.insert(LL.corners.begin(),LL.corners.end());
        cornerset.insert(UR.corners.begin(),UR.corners.end());
        cornerset.insert(LR.corners.begin(),LR.corners.end());

		if(!separable)
		{
		    //if(cl_m<rB)  status = ON;
		    //else
		        status=OFF;
		    return;
		}
		else //separable
		{
            //
            if( (cornerset.size()+wallset.size())>1 ){ //there is (some) change of features
                status = ON;
                return;
            }

            //
            // tricky if all nodes have the same wall feature and
            // there is a corner feature in this box
            //
            if(wallset.size()==1 && corners.empty()==false){
                status = TRICKY;
                return;
            }

            status = OFF;
		}

        return;
	}

	//
	void distribute_features2child(Box * child)
	{
	    typedef list<Wall*>::iterator   WIT;
	    typedef list<Corner*>::iterator CIT;

	    //center x,y
	    double x=child->x;
	    double y=child->y;
	    double cl2r=child->rB*2+child->cl_m;
	    //

        //compute the separation to walls
        for (WIT iterW=walls.begin(); iterW != walls.end(); ++iterW)
        {
            Wall* w = *iterW;
            double dist = w->distance_star(x, y); //w->distance_star(x, y); //w->distance(x, y);
            if (dist < cl2r)
            {
                child->walls.push_back(w);
            }
        }

        //compute the separation to corners
        for (CIT iterC=corners.begin(); iterC != corners.end(); ++iterC)
        {
            Corner* c = *iterC;
            double dist = c->distance(x, y);
            if (dist < cl2r)
            {
                child->corners.push_back(c);
            }
        }
	}

	//
	// find the nearest feature and the clearance
	// results are stored in "node"
	//
	void determine_clearance(BoxNode& node)
	{

	    double x=node.x;
	    double y=node.y;

	    //compute the closest wall
		Wall* nearestWall=NULL;
		double mindistW=FLT_MAX;
		for (list<Wall*>::iterator iterW = walls.begin(); iterW != walls.end(); ++iterW)
		{
			Wall* w = *iterW;
			double dist = w->distance(x, y); //w->distance_star(x, y); //w->distance(x, y);
			if (dist < mindistW)
			{
				mindistW = dist;
				nearestWall = *iterW;
			}
		}

		//
		// compute a closest corner that is closer than  (mindistW +1) (?? why +1)
		//
		double mindistC = mindistW; //mindistC may not exist, so init to a bigger number
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

		if (mindistW < mindistC)
		{
		    node.clearance=mindistW;
		    node.walls.push_back(nearestWall);
		}
		else{
		    node.clearance=mindistC;
		    node.corners.push_back(nearestCorner);
		}
	}

	void addCorner(Corner* c)
	{
		corners.push_back(c);
	}

	void addWall(Wall* w)
	{
		walls.push_back(w);
	}

//	bool isFree()
//	{
//		if (status == FREE)
//		{
//			return true;
//		}
//		return false;
//	}

	Status getStatus()
	{
		updateStatus();
		return status;
	}

	// split (eps)
	// 	returns false if we fail to split for some reason
	//
	bool split(double epsilon)
	{
		if (this->height < epsilon || this->width < epsilon)
		{
			return false;
		}

		if (!this->isLeaf || this->status != IN)
		{
			return false; 
		}

		//record the time of this split event, will be used to set priority of children
		++Box::counter;

		Box* children[4];
		children[0] = new Box(x - width / 4, y + height / 4, width / 2, height / 2);
		children[1] = new Box(x + width / 4, y + height / 4, width / 2, height / 2);
		children[2] = new Box(x + width / 4, y - height / 4, width / 2, height / 2);
		children[3] = new Box(x - width / 4, y - height / 4, width / 2, height / 2);
		for (int i = 0; i < 4; ++i)
		{
			children[i]->depth = this->depth + 1;
			BoxNode node;
			node.x=children[i]->x;
			node.y=children[i]->y;
			determine_clearance(node);
			children[i]->cl_m=node.clearance;

			//distribute the feature
			distribute_features2child(children[i]);
		}

		//
		// for internal nodes, children [i] is the i-th child
		// for leaves, children [i] is the pointer to first node in i-th adj list
		//
		for (int i = 0; i < 4; ++i)
		{
			//find three other directions
			int prev = (i + 3) % 4;
			int next = (i + 1) % 4;
			int cross = (i + 2) % 4;

			//update easy cases
			children[i]->pChildren[i] = pChildren[i];
			children[i]->pChildren[next] = children[next];
			children[i]->pChildren[cross] = children[prev];

			//init box neighbor iterator for direction i
			BoxIter* iter = new BoxIter(this, i);
			Box* neighbor = iter->First();

			if (!neighbor)
			{
				continue;
			}

			// if neighbor are no smaller
			if (neighbor->depth <= this->depth)
			{
                //after split child 'next' should also point to
                //neighbor in direction i
				children[next]->pChildren[i] = neighbor;

                //if neighbor's cross direction point to this, it should
                //instead point to child 'next' after split
				if (neighbor->pChildren[cross] == this)
				{
					neighbor->pChildren[cross] = children[next];
				}
				continue;
			}

			Box* prevNeighbor = neighbor;

			//indicate if we go across the boundary between child 'i'
			//and 'next' the first time
			bool firstTimeCrossBetweenChildren = true;

			//if neighbor smaller
			while(neighbor != iter->End())
			{
                //assert( abs(abs(neighbor->x - this->x)
                //	- (neighbor->width/2 + this->width/2)) < 1e-8
                // ||  abs(abs(neighbor->y - this->y)
                // 	- (neighbor->height/2 + this->height/2)) < 1e-8);
                //within the strip of child next, neighbor's cross direction
                //should point to next
				if (!isOverLimit(children[next], neighbor))
				{
					neighbor->pChildren[cross] = children[next];
				} 

				//within the strip of child i, neighbor's cross
				//direction should point to i
				else if (!isOverLimit(children[i], neighbor))
				{
					neighbor->pChildren[cross] = children[i];

					//first time cross between child i and next,
					//should update next's i direction pointer
					if (firstTimeCrossBetweenChildren)
					{
						firstTimeCrossBetweenChildren = false;
						children[next]->pChildren[i] = prevNeighbor;
					}
				}
				else
				{
					assert(0);
				}
				prevNeighbor = neighbor;
				neighbor = iter->Next();
			}
		}


		for (int i = 0; i < 4; ++i)
		{
			this->pChildren[i] = children[i];
			this->pChildren[i]->pParent = this;
			
//			//add all of parent's walls and corners to each child,
//			//will be filtered later in updatestatus()
//			this->pChildren[i]->walls.insert(this->pChildren[i]->walls.begin(), this->walls.begin(), this->walls.end());
//			this->pChildren[i]->corners.insert(
//				this->pChildren[i]->corners.begin(),
//				this->corners.begin(), this->corners.end());
		}

		this->isLeaf = false;

		return true;
	} 

	//
	// determine if the corners and walls in this class are separable
	// return true if separable
	// otherwise return false
	//
	bool are_features_separable() //(set<Wall*>& wallset, set<Corner*>& cornerset)
	{
	    //list<Wall*> walls(wallset.begin(),wallset.end());
	    //list<Corner*> corners(cornerset.begin(),cornerset.end());

	    int feature_size=walls.size()+corners.size();

	    assert(feature_size<=3);

	    if(feature_size<=1) return false; //nothing to separate

	    //so, there are 2 or 3 features now...
        if(walls.empty()){
            return true;   //multiple disjoint corners
        }
        if(corners.empty()){
            return true; //multiple disjoint walls
        }

        bool separable=true;
	    if(feature_size==3)
	    {
	        if(walls.size()==2) //two walls and one corner
	        {
	            Corner* c=corners.front();
	            Wall* w1=walls.front();
	            Wall* w2=walls.back();
	            if( (c->nextWall==w1 || c->preWall==w1) &&  (c->nextWall==w2 || c->preWall==w2) )
	                separable=false; //adjacent, so inseparable
	        }
	        else{ //two conrners and one wall
                Corner* c1=corners.front();
                Corner* c2=corners.back();
                Wall* w=walls.front();
                if( (c1->nextWall==w || c1->preWall==w) &&  (c2->nextWall==w || c2->preWall==w) )
                    separable=false; //adjacent, so inseparable
	        }
	    }
	    else{ //feature_size==2
	        Corner* c=corners.front();
	        Wall* w=walls.front();
	        if(c->nextWall==w || c->preWall==w)
	            separable=false; //adjacent, so inseparable
	    }

	    return separable;


//	    list<Set*> features;
//	    typedef list<Wall*>::iterator WIT;
//	    typedef list<Corner*>::iterator CIT;
//
//	    for (WIT it = walls.begin(); it != walls.end(); it++){
//	        Set* set=new Set((Feature*)*it);
//	        assert(set);
//	        features.push_back(set);
//	    }//end for
//
//        for (CIT it = corners.begin(); it != corners.end(); it++){
//            Set* set=new Set((Feature*)*it);
//            assert(set);
//            features.push_back(set);
//        }//end for
//
//        UnionFind UF;
//
//        //merge
//        for (WIT it = walls.begin(); it != walls.end(); it++){
//            Wall* w=*it;
//            if(w->dst->pSet!=NULL) UF.Union(w,w->dst);
//            if(w->src->pSet!=NULL) UF.Union(w,w->src);
//        }//end for
//
//        for (CIT it = corners.begin(); it != corners.end(); it++){
//            Corner* c=*it;
//            if(c->preWall->pSet!=NULL) UF.Union(c,c->preWall);
//            if(c->nextWall->pSet!=NULL) UF.Union(c,c->nextWall);
//        }//end for
//
//        //now check the number of Sets
//        Set * firstSet=walls.front()->pSet;
//        for (WIT it = walls.begin(); it != walls.end(); it++){
//            if( (*it)->pSet!=firstSet ) return true; //more than one set, so separable
//        }//end for
//
//        for (CIT it = corners.begin(); it != corners.end(); it++){
//            if( (*it)->pSet!=firstSet ) return true; //more than one set, so separable
//        }//end for
//
//        //done
//        return false; //there is only one set, so unseparable!
	}

};//class Box
