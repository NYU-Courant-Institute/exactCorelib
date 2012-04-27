/* **************************************
   File: QuadTree.h

   Description: 

   	The main routines here are:
		expand()  -- this keeps expanding mixed boxes
				until the priority queue is empty
				or a path is found

   HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang

   Since Core Library  Version 2.1
   $Id: $
 ************************************** */

#pragma once
#include <iostream>
#include "Box.h"
#include "UnionFind.h"
#include "PriorityQueue.h"

extern vector<Box*> allLeaf;

extern int freeCount;
extern int stuckCount;
extern int mixCount;
extern int mixSmallCount;

class QuadTree
{
private:
	void insertNode(Box* b)
	{
		switch (b->getStatus())
		{
		case Box::FREE:
			new Set(b);
			unionAdjacent(b);
			++freeCount;
			break;
		case Box::STUCK:
			++stuckCount;
			break;
		case Box::MIXED:
			++mixCount;
			if (b->height < epsilon || b->width < epsilon)
				++mixSmallCount;
			PQ->push(b);
			break;
		case Box::UNKNOWN:
			std::cout << "UNKNOWN not handled?" << std::endl;
		}
	}

public:
	UnionFind* pSets;
	BoxQueue* PQ;
	Box* pRoot;
	double epsilon;
	int QType;
	int seed;

	QuadTree (Box* root, double e, int qType, int s):
	    pRoot(root), epsilon(e), QType(qType), seed(s)
	{
		switch (QType)
		{
		case 1:
			PQ = new seqQueue();
			break;
		case 0:
			PQ = new randQueue(s);
			break;
		case 2:
			PQ = new dijkstraQueue();
			break;
		}
		
		//PQ = new randQueue();

		pRoot->updateStatus();
		insertNode(pRoot);
	}

	Box* getBox (Box* root, double x, double y)
	{
		if (x > root->x + root->width / 2 || x < root->x - root->width / 2
			|| y > root->y + root->height / 2 || y < root->y - root->height / 2)
		{
			return 0;
		}

		Box* b = root;
		while (!b->isLeaf)
		{
			vector<Box*>::reverse_iterator it = allLeaf.rbegin();
			for (;;)
			{
				Box* c = *it;
				if (c->contains(x, y))
				{
					b = c;
					break;
				}
				++it;
			}
		}
		return b;
	}

	Box* getBox(double x, double y)
	{
		return getBox(pRoot, x, y);
	}

	bool expand (Box* b)
	{
		vector<Box*> cldrn;
		if (!b->split(epsilon, cldrn))
		{
			return false;
		}

		for (int i = 0; i < (int)cldrn.size(); ++i)
		{
			cldrn[i]->updateStatus();
			insertNode(cldrn[i]);
		}

		return true;
	}

	bool expand ()
	{
		while(!PQ->empty())
		{
			Box* b = PQ->extract();
			if (!b->isLeaf)
			{
				continue;
			}

			vector<Box*> cldrn;

			//b might not be a leaf since it could already be split in expand(Box* b), and PQ is not updated there
			if (b->split(epsilon, cldrn))
			{
				assert(b->status == Box::MIXED);

				for (int i = 0; i < (int)cldrn.size(); ++i)
				{
					cldrn[i]->updateStatus();
					insertNode(cldrn[i]);
				}			
				return true;
			}			
		}
		return false;
	}

	bool isConnected (Box* a, Box* b)
	{
		if (pSets->Find(a) == pSets->Find(b))
		{
			return true;
		}
		return false;
	}

	void unionAdjacent (Box* b)
	{
		for (int i = 0; i < 6; ++i)
		{
			for (vector<Box*>::iterator it = b->Nhbrs[i].begin(); it != b->Nhbrs[i].end(); ++it)
			{
				Box* neighbor = *it;
				if (neighbor->status == Box::FREE)
				{
					pSets->Union(b, neighbor);
				}
			}
		}
	}

	~QuadTree (void)
	{
	}
};
