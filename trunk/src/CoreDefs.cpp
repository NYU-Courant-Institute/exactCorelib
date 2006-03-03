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
 * $Id: CoreDefs.cpp,v 1.4 2006-03-03 17:58:10 exact Exp $
 ***************************************************************************/
#include <CORE/Config.h>
#include <CORE/BigFloat.h>

CORE_BEGIN_NAMESPACE

// filter flag
bool fpFilterFlag = true;

// default output rounding mode
mp_rnd_t def_output_rounding_mode = MPFR_RND;
// default output base
int def_output_base = 0;

long defAbsPrec = LONG_MAX;
long defRelPrec = 52;

CORE_END_NAMESPACE
