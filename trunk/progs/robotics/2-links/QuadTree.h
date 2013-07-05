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

//#ifdef __CYGWIN32__
//#include "GL/glui.h"
//#endif
//#ifdef __linux__
//#include <GL/glut.h>
//#include "GL/glui.h"
//#endif
//#ifdef _WIN32
//#include <gl/glui.h>
//#endif
//#ifdef __APPLE__
//#include <GLUI/glui.h>
//#endif

extern vector<Box*> allLeaf;
extern vector<Set*> allSet;

extern int freeCount;
extern int stuckCount;
extern int mixCount;
extern int mixSmallCount;

//extern void glutPostRedisplay(void);

class QuadTree
{
private:
	void insertNode(Box* b)
	{
		switch (b->getStatus())
		{
		case Box::FREE:
			{
				Set* st = new Set(b);
				allSet.push_back(st);
			}
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
			PQ = new dijkstraQueue<DistCmp>();
			break;
        case 3:
            PQ = new dijkstraQueue<DistPlusSizeCmp>();
            break;
        case 4:
            PQ = new dijkstraQueue<VorCmp>();
            break;
		}

		//PQ = new randQueue();

		pRoot->updateStatus();
		insertNode(pRoot);
	}

	//bfs search for alpha/beta
	Box* getBox (double x, double y, double a1, double a2, int& ct)
	{
		std::queue<Box*> q;

		for (int i = 0; i < (int)allLeaf.size(); ++i)
		{
			if ( allLeaf[i]->contains(x, y, a1, a2) )
			{
				q.push(allLeaf[i]);
			}
		}
//		std::cout<<"getBox 124"<< endl;

		while (q.size())
		{
			Box* b = q.front();
			q.pop();
			if (!b->isLeaf)
			{
				continue;
			}
			if (!b->contains(x, y, a1, a2))
			{
//				std::cout<<"getBox 135"<< endl;
				return 0;
			}

			vector<Box*> cldrn;
			if (!expand(b, cldrn))
			{
//				std::cout<<"getBox 142"<< endl;
				return 0;
			}
			++ct;

			for (int i = 0; i < (int)cldrn.size(); ++i)
			{
				if ( cldrn[i]->contains(x, y, a1, a2) )
				{
					if (cldrn[i]->isFree())
					{
//						std::cout<<"getBox 153"<< endl;
						return cldrn[i];
					}
					q.push(cldrn[i]);
				}
			}

		}
//		std::cout<<"getBox 160"<< endl;
		return 0;
	}

	bool expand (Box* b)
	{
		vector<Box*> cldrn;
		return expand(b, cldrn);
	}

	//expand and put children ptr in cldrn
	bool expand (Box* b, vector<Box*>& cldrn)
	{
		if (!b->split(epsilon, cldrn))
		{
//			std::cout<<"expand 177"<<endl;
			return false;
		}

		for (int i = 0; i < (int)cldrn.size(); ++i)
		{
//			glutPostRedisplay();
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
//					glutPostRedisplay();
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
		for (int i = 0; i < 4; ++i)
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
