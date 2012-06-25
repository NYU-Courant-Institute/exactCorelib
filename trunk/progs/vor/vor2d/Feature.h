
#pragma once

#define CORE_LEVEL 3
#include "CORE.h"

class Set; //defined in "UnionFind.h"

class Feature
{
public:
    Feature() { pSet=NULL; }
    ~Feature(){}

    virtual void dummy(){}
	Set* pSet;   //?
};
