/****************************************************************************
 * CoreAux.cpp -- Definitions of some Auxiliary routines.
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
 * $Id: CoreAux.cpp,v 1.7 2007-02-06 22:58:51 exact Exp $
 ***************************************************************************/
#include <CORE/Config.h>
#include <CORE/CoreDefs.h>
#include <CORE/BigInt.h>
#include <iostream>
#include <fstream>

CORE_BEGIN_NAMESPACE

msb_t ceillg(size_t v)
{ return BigInt(v).ceillg(); }
unsigned long gcd(unsigned long x, unsigned long y)
{ return gcd(BigInt(x), BigInt(y)).get_ui(); }

msb_t ceillg(long v)
{ return BigInt(v).ceillg(); }
msb_t floorlg(long v)
{ return BigInt(v).floorlg(); }

long gcd(long x, long y)
{ return gcd(BigInt(x), BigInt(y)).get_si(); }

#if defined ((gnu) || (cygwin))
// it produces error in linux and mac os x
msb_t ceillg(unsigned long v)
{ return BigInt(v).ceillg(); }
msb_t floorlg(unsigned long v)
{ return BigInt(v).floorlg(); }
#endif

int gcd(int x, int y)
{ return gcd(BigInt(x), BigInt(y)).get_si(); }


/// CORE_DIAGFILE is file name for core_error(..) output.
char* CORE_DIAGFILE = "Core_Diagnostics";  // global file name
  
/// core_error is the method to write Core Library warning or error messages
/**     Both warnings and errors are written to a file called CORE_DIAGFILE.
 *      But errors are also written on std:cerr (similar to std::perror()).
 * */
// Usage: core_error(message, file_with_error, line_number, err_type)
//   where err_type=false means WARNING, error_type=true means ERROR
void core_error(std::string msg, std::string file, int lineno, bool err) {
  std::ofstream outFile(CORE_DIAGFILE, std::ios::app);  // open to append
  if (!outFile) {               
     std::cerr << "CORE ERROR: can't open Core Diagnostics file"<<std::endl;
     exit(1); //Note: do not call abort()
  }
  outFile << "CORE " << (err? "ERROR" : "WARNING")
     << " (at " << file << ": " << lineno << "): "
     << msg << std::endl; 
  outFile.close();
  if (err) { // ERROR!
    std::cerr << "CORE ERROR: file " << file << ", line "
        << lineno << "):" << msg << std::endl;
     exit(1); //Note: do not call abort()
  }
}

CORE_END_NAMESPACE
