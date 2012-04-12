#pragma once
#include "Box.h"
#include "UnionFind.h"
#include "PriorityQueue.h"
#include <queue>

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
			break;
		case Box::STUCK:
			break;
		case Box::MIXED:
			PQ.push(b);
			break;
		}
	}

public:
	UnionFind* pSets;
	priority_queue<Box*, vector<Box*>, PQCmp> PQ;
	Box* pRoot;
	double epsilon;

	QuadTree(Box* root, double e):pRoot(root), epsilon(e)
	{
		pRoot->updateStatus();
		insertNode(pRoot);
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

	bool expand(Box* b)
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

	bool expand()
	{
		while(!PQ.empty())
		{
			Box* b = PQ.top();
			PQ.pop();
			//b might not be a leaf since it could already be split in expand(Box* b), and PQ is not updated there
			if (b->isLeaf && b->split(epsilon))
			{
				assert(b->status == Box::MIXED);
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

	~QuadTree(void)
	{
	}
};
