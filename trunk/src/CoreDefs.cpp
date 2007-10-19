/****************************************************************************
 * CoreDefs.cpp -- Definitions of Global parameters
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
 * $Id: CoreDefs.cpp,v 1.11 2007-10-19 15:40:02 exact Exp $
 ***************************************************************************/
#include <CORE/Config.h>
#include <CORE/CoreDefs.h>
#include <CORE/Mpfr.h>

CORE_BEGIN_NAMESPACE

// filter flag
bool fpFilterFlag = true;

// default output rounding mode
mp_rnd_t def_output_rounding_mode = MPFR_RND;
// default output base
int def_output_base = 0;

long defAbsPrec = CORE_INFTY;
long defRelPrec = 52;
long defInputDigits = CORE_INFTY;

long defBFdivRelPrec = 54;
long defBFradicalRelPrec = 54;

long defWBFPrec = 54;
bool WBFenabled = false;

unsigned long cutOffBound = CORE_INFTY;
// used as transcendental number root bound
unsigned long escapeBound = 10000;

bool coretest_error = false;
bool coretest_verbose = false;

CORE_END_NAMESPACE
