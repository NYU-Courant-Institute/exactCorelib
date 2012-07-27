
#pragma once

#ifndef CORE_LEVEL
	#define CORE_LEVEL 2
#endif

#include "CORE.h"

class Set; //defined in "UnionFind.h"

class Feature
{
public:

    virtual void dummy(){}
	Set* pSet;   //?
};
