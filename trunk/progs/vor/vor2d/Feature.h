/*
 * file: Feature.h
 *
 * 	This is the main file that controls the CORE_LEVEL...
 * 		Choose CORE_LEVEL to be 1, 2 or 3.
 * 	We generally prefer to default to LEVEL 1,
 * 		as it could be slow for LEVEL 2 or 3.
 *
 * 	Author: Jyh-Ming Lien
 * 		Chee Yap
 * 	Since Core 2.1
 * 	$Id:$
 * ***************************************************/

#pragma once

#ifndef CORE_LEVEL
	#define CORE_LEVEL 1
#endif

#include "CORE.h"

class Set; //defined in "UnionFind.h"

class Feature
{
public:
    static int LEVEL;

    /*
    int corelevel() {
#if CORE_LEVEL=1
    LEVEL=1;
#elif CORE_LEVEL=2
    LEVEL=2;
#elif CORE_LEVEL=3
    LEVEL=3;
#else
    LEVEL=4;
#endif
    }
    */

    Feature() { pSet=NULL; }
    ~Feature(){}

    void static showLevel(){
	std::cout << "Core Level = " << LEVEL << std::endl;
    }

    virtual void dummy(){}
	Set* pSet;   //?
};
