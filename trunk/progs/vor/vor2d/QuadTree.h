/* **************************************
   File: QuadTree.h

   Description: 

   HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang

   Since Core Library  Version 2.1
   $Id: $
 ************************************** */

#pragma once
#include <iostream>
#include "Box.h"
//#include "UnionFind.h"
#include "PriorityQueue.h"

extern int freeCount;
extern int stuckCount;
extern int trickyCount;
extern int mixCount;
extern int mixSmallCount;

class QuadTree
{
private:


	//insert Node to PQ
	void insertNode(Box* b)
	{
		switch (b->getStatus())
		{
		case Box::OFF:
			//new Set(b);
			//unionAdjacent(b);
			++freeCount;
			break;
		case Box::ON:
			++stuckCount;
			break;
        case Box::TRICKY:
            ++trickyCount;
            break;
		case Box::IN:
			++mixCount;
			if (b->height < epsilon || b->width < epsilon)
				++mixSmallCount;
			PQ->push(b);
			break;
		case Box::UNKNOWN:
			std::cout << "UNKNOWN not handled?" << std::endl;
			break;
		}
	}

public:

	//UnionFind* pSets;
	BoxQueue* PQ;
	Box* pRoot;
	double epsilon;
	int QType;
	int seed;

	QuadTree(Box* root, double e, int qType, int s):
	    pRoot(root), epsilon(e), QType(qType), seed(s)
	{
	/*
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
		*/
		
		PQ = new seqQueue();

		pRoot->updateStatus();
		insertNode(pRoot);
	}

	void subdividePhase()
	{
	    int ct = 0;
	    while(PQ->empty()==false)
	    {
	        expand();
	        ct++;
	    }
	}

	//
	void balancePhase()
	{
	    list<Box*> leaves;
	    pRoot->getLeaves(leaves);

	    vector< pair<int,Box*> > box_pq;
	    for(list<Box*>::iterator i=leaves.begin();i!=leaves.end();i++)
	    {
	        Box * box=*i;
	        pair<int,Box*> tmp(box->depth,box);
	        box_pq.push_back(tmp);
	        push_heap(box_pq.begin(),box_pq.end());
	    }


	    while(box_pq.empty()==false)
	    {

	        pair<int,Box*> tmp=box_pq.front();
	        Box * box=tmp.second;
	        pop_heap(box_pq.begin(),box_pq.end());
	        box_pq.pop_back();

	        //
	        for(int i=0;i<4;i++){

                Box * nei=box->pChildren[i];

	            if(nei==NULL) continue;

	            //visiting all neighbors
	            if(nei->depth < box->depth-1)
	            {
	                Box::Status backup_nei_status=nei->status;
	                nei->status=Box::IN; //force to split
	                bool results=nei->split(epsilon); //ask the neighbor to slip
	                nei->status=backup_nei_status;

	                if(results)
	                {
                        for(int k=0;k<4;k++){ //enqueue neighbors' kid
                            Box * nei_kid=nei->pChildren[k];

                            //
                            nei_kid->updateStatus();
                            if(nei_kid->status==Box::IN)
                            {
                                PQ->push(nei_kid);
                            }
                            //    nei_kid->status=nei->status;
                            //

                            pair<int,Box*> tmp(nei_kid->depth,nei_kid);
                            box_pq.push_back(tmp);
                            push_heap(box_pq.begin(),box_pq.end());
                        }//end for k
	                }
	                else{
	                    cout<<"Split failed"<<endl;
	                }
	            }
	        }//end for i
	    }//end while
	}

	void constructPhase()
	{
        list<Box*> leaves;
        pRoot->getLeaves(leaves);

        for(list<Box*>::iterator i=leaves.begin();i!=leaves.end();i++)
        {
            Box * box=*i;
            box->buildVor();
        }
	}

	Box* getBox(Box* root, double x, double y)
	{
		if (x > root->x + root->width / 2 || x < root->x - root->width / 2
			|| y > root->y + root->height / 2 || y < root->y - root->height / 2)
		{
			return 0;
		}

		Box* b = root;
		while (!b->isLeaf)
		{
			double dx = x - b->x;
			double dy = y - b->y;
			if (dx <= 0 && dy >= 0)
			{
				b = b->pChildren[0];
			}
			else if (dx >= 0 && dy >= 0)
			{
				b = b->pChildren[1];
			}
			else if (dx >= 0 && dy <= 0)
			{
				b = b->pChildren[2];
			}
			else if (dx <= 0 && dy <= 0)
			{
				b = b->pChildren[3];
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
		if (!b->split(epsilon))
		{
			return false;
		}
	
		for (int i = 0; i < 4; ++i)
		{
			b->pChildren[i]->updateStatus();
			insertNode(b->pChildren[i]);
		}

		return true;
	}

	bool expand ()
	{
		while(!PQ->empty())
		{
			Box* b = PQ->extract();
			//b might not be a leaf since it could already be split in expand(Box* b), and PQ is not updated there
			if (b->isLeaf && b->split(epsilon))
			{
				assert(b->status == Box::IN);
				for (int i = 0; i < 4; ++i)
				{
					b->pChildren[i]->updateStatus();
					insertNode(b->pChildren[i]);
				}			
				return true;
			}			
		}
		return false;
	}

	/*
	bool isConnect(Box* a, Box* b)
	{
		if (pSets->Find(a) == pSets->Find(b))
		{
			return true;
		}
		return false;
	}


	void unionAdjacent(Box* b)
	{
		for (int i = 0; i < 4; ++i)
		{
			BoxIter* iter = new BoxIter(b, i);
			Box* neighbor = iter->First();
			while(neighbor && neighbor != iter->End())
			{
				if (neighbor->status == Box::FREE)
				{
					pSets->Union(b, neighbor);
				}
				neighbor = iter->Next();
			}
		}
	}
	*/
	
	~QuadTree(void)
	{
	}
};
