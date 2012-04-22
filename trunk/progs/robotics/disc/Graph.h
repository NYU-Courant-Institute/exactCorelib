#pragma once

#include <vector>
#include <algorithm>
#include <math.h>
#include "Box.h"

using namespace std;

// min heap
class distHeap
{
private:
	static bool distLess(Box* a, Box* b)
	{
		return a->dist2Source < b->dist2Source;
	}
public:
	static void makeHeap(vector<Box*>& bv)
	{
		if (bv.size() <= 1)
		{
			return;
		}
		for (int i = 0; i < (int)bv.size(); ++i)
		{
			bv[i]->heapId = i;
		}
		for (int i = (bv.size() -2) / 2; i >= 0; --i)
		{
			siftDown(bv, i);
		}
	}

	static void siftDown(vector<Box*>& bv, int i)
	{
		unsigned int l = 2*i + 1;
		unsigned int r = 2*i + 2;
		int smallest;
		if (l < bv.size() && distLess(bv[l], bv[i]))
		{
			smallest = l;
		}
		else
		{
			smallest = i;
		}
		if (r < bv.size() && distLess(bv[r], bv[smallest]))
		{
			smallest = r;
		}
		if (smallest != i)
		{
			Box* tmp = bv[smallest];
			bv[smallest] = bv[i];
			bv[i] = tmp;
			bv[smallest]->heapId = smallest;
			bv[i]->heapId = i;

			siftDown(bv, smallest);
		}

	}

	static void insert(vector<Box*>& bv, Box* b)
	{
		bv.push_back(b);
		int bid = bv.size() - 1;
		b->heapId = bid;
		int pid = (bid - 1) / 2;
		while (bid > 0 && distLess(bv[bid], bv[pid]))
		{
			Box* tmp = bv[bid];
			bv[bid] = bv[pid];
			bv[pid] = tmp;
			bv[bid]->heapId = bid;
			bv[pid]->heapId = pid;

			bid = pid;
			pid = (bid - 1) / 2;
		}
	}

	static void decreaseKey(vector<Box*>& bv, Box* b, double dist)
	{
		assert(bv[b->heapId] == b);
		assert(b->dist2Source > dist);

		b->dist2Source = dist;
		int bid = b->heapId;
		int pid = (bid - 1) / 2;
		while (bid > 0 && distLess(bv[bid], bv[pid]))
		{
			Box* tmp = bv[bid];
			bv[bid] = bv[pid];
			bv[pid] = tmp;
			bv[bid]->heapId = bid;
			bv[pid]->heapId = pid;

			bid = pid;
			pid = (bid - 1) / 2;
		}
	}

	static Box* extractMin(vector<Box*>& bv)
	{
		Box* minB = bv[0];
		bv[0] = bv.back();
		bv[0]->heapId = 0;
		minB->heapId = -1;
		bv.pop_back();
		siftDown(bv, 0);
		return minB;
	}


};

class Graph
{
public:
	static vector<Box*> findPath(Box* a, Box* b)
	{
		a->dist2Source = 0;
		vector<Box*> bv;
		distHeap::insert(bv, a);
		while(bv.size())
		{
			Box* current = distHeap::extractMin(bv);
			current->visited = true;
			if (current == b)
			{				
				break;
			}
			for (int i = 0; i < 4; ++i)
			{
				BoxIter* iter = new BoxIter(current, i);
				Box* neighbor = iter->First();
				while (neighbor && neighbor != iter->End())
				{
					if (!neighbor->visited && neighbor->status == Box::FREE)
					{
						double dist2pre = sqrt( (current->x - neighbor->x)*(current->x - neighbor->x) + (current->y - neighbor->y)*(current->y - neighbor->y) );
						double dist2src = dist2pre + current->dist2Source;

						if (neighbor->dist2Source == -1)
						{
							neighbor->prev = current;
							neighbor->dist2Source = dist2src;
							distHeap::insert(bv, neighbor);
						}
						else
						{
							if (neighbor->dist2Source > dist2src)
							{
								neighbor->prev = current;
								distHeap::decreaseKey(bv, neighbor, dist2src);
							}
						}
					}
					neighbor = iter->Next();
				}
			}
		}
		vector<Box*> path;
		path.push_back(b);
		while (path.back()->prev)
		{
			path.push_back(path.back()->prev);
		}
		return path;
	}

};
