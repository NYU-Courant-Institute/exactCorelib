/****************************************************************************
 * CORE.h -- The main inclusion file for the Core Library
 *
 * Core Library Version 2.0, March 2006
 * Copyright (c) 1995-2006 Exact Computation Project
 * All rights reserved.
 *
 * This file is part of Core Library (http://cs.nyu.edu/exact/core); you 
 * may redistribute it under the terms of the Q Public License version 1.0.
 * See the file LICENSE.QPL distributed with Core Library.
 *
 * Licensees holding a valid commercial license may use this file in
 * accordance with the commercial license agreement provided with the
 * software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * WWW URL: http://cs.nyu.edu/exact/core
 * Email: exact@cs.nyu.edu
 *
 * $Id: CORE.h,v 1.1.1.1 2006-02-09 09:18:04 exact Exp $
 ***************************************************************************/
#ifndef __CORE_H__
#define __CORE_H__

// define version number
#define CORE_VERSION            2
#define CORE_VERSION_MINOR      0
#define CORE_VERSION_PATCHLEVEL 0

// macros for defining namespace
#define CORE_BEGIN_NAMESPACE    namespace CORE {
#define CORE_END_NAMESPACE      };
#define CORE_NS			CORE

// other macros 
#define CORE_OLDNAMES 1

#include <CORE/CoreDefs.h>

// level 1
#include <CORE/BigInt.h>
#include <CORE/BigRat.h>
#include <CORE/Mpfr.h>
// level 2
#include <CORE/BigFloat.h>
// level 3
#include <CORE/CoreAux.h>
#include <CORE/Expr.h>

// some standard library
#include <iostream>
#include <string>
#include <cmath>

typedef double machine_double;
typedef long machine_long;

#ifndef CORE_LEVEL
  #define CORE_LEVEL 3
#endif

#if CORE_LEVEL == 3
  #undef double
  #define double Expr
#endif

#ifndef CORE_NO_AUTOMATIC_NAMESPACE
using namespace CORE;
#endif

#endif /*__CORE_H__*/
