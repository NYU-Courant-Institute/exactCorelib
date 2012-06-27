/*
 * file: Feature.h
 *
 * 	This is the main file that controls the CORE_LEVEL...
 * 		Choose CORE_LEVEL to be 1, 2 or 3.
 * 	We generally prefer to default to LEVEL 1,
 * 		as it could be slow for LEVEL 2 or 3.
 *
 * 	Author: Jyh-Ming Lien, Chee Yap
 * 	Since Core 2.1
 *
 * ***************************************************/

#pragma once

#define CORE_LEVEL 1
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
