#pragma once
#include "Box.h"
#include <queue>
#include <vector>

using namespace std;

class PQCmp
{
public:
	bool operator() (const Box* a, const Box* b)
	{
		//use depth for now
		if (a->depth > b->depth)
		{
			return true;
		}
		//if same depth, expand box created earlier first
		else if (a->depth == b->depth)
		{
			return a->priority > b->priority;
		}
		return false;
	}
};

//class PriorityQueue
//{
//private:
//	priority_queue<Box*, vector<Box*>, PQCmp> q;
//public:
//
//	PriorityQueue(void)
//	{
//	}
//
//	~PriorityQueue(void)
//	{
//	}
//};

