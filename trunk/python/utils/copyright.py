import os, string

copyright = """\
/****************************************************************************
 * %s -- %s
 *
 * %s Version %s, %s
 * Copyright (c) %s-%s %s
 * All rights reserved.
 *
 * This file is part of %s (%s); you 
 * may redistribute it under the terms of the Q Public License version 1.0.
 * See the file LICENSE.QPL distributed with %s.
 *
 * Licensees holding a valid commercial license may use this file in
 * accordance with the commercial license agreement provided with the
 * software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * WWW URL: %s
 * Email: %s
 *
 * $Id: copyright.py,v 1.2 2006-02-28 18:05:11 exact Exp $
 ***************************************************************************/
"""

def gen_copyright(filename, description, name, version, time, 
                  start_year, end_year, project, url, email):
    return copyright % (filename, description, name, version, time, 
           start_year, end_year, project, name, url, name, url, email)

def gen_header(filename, description):
    name = "Core Library"
    version = "2.0"
    time = "March 2006"
    start_year = "1995"
    end_year = "2006"

    project = "Exact Computation Project"
    url = "http://cs.nyu.edu/exact/core"
    email = "exact@cs.nyu.edu"

    return gen_copyright(filename, description, name, version, time, 
           start_year, end_year, project, url, email) 

def add_header(path, filename, description):
    src_fn = path + filename
    tmp_fn = src_fn + ".tmp"

    print "handling %s..." % (src_fn)

    src_f = open(src_fn, "r")
    tmp_f = open(tmp_fn, "w")
    lines = src_f.readlines()

    # find the start line
    startline = 0
    if (lines[0].startswith("/*****")):
      for i in range(1, len(lines)):
        if lines[i].find("*****/") >= 0:
          startline = i+1
          break;
    # write header
    tmp_f.write(gen_header(filename, description))
    # write the rest line
    for i in range(startline, len(lines)):
      tmp_f.write(lines[i])

    tmp_f.close()
    src_f.close()
    os.system("mv %s %s" % (tmp_fn, src_fn))
         
def handle_header_files(path):
    filelist = { 
                 "CORE.h" : "The main inclusion file for the Core Library",
                 "Config.h" : "Configuration Macros",
                 "CoreAux.h" : "Auxilliary functions for the Core Library",
                 "CoreDefs.h" : "Global parameters for the Core Library",
                 "Gmpz.h" : "A C++ wrapper class for GMP mpz",
                 "BigInt.h" : "A C++ wrapper class for GMP mpz",
                 "Gmpq.h" : "A C++ wrapper class for GMP mpq",
                 "BigRat.h" : "A C++ wrapper class for GMP mpq",
                 "Mpfr.h" : "A C++ wrapper class for MPFR mpfr",
                 "BigFloat.h" : "A C++ wrapper class for MPFR mpfr",
                 "BigFloat2.h" : "A C++ number class providing arbitrary precision",
                 "BigFloat2.inl" : "Inline functions for BigFloat",
                 "ExprRep.h" : "Internal Representation for Expr",
                 "Expr.h" : "A C++ number class providing guarranteed precision",
                 "Filters.h" : "Floating-point filters",
                 "Policies.h" : "Computation policies for BigFloat",
                 "RootBounds.h" : "Constructive root bounds",
                 "Timer.h" : "A C++ class providing timing",
               }
    for key in filelist.keys():
        add_header(path+"inc/CORE/", key, filelist[key])

    add_header(path+"inc/", "CORE.h", "The main inclusion file for the Core Library")

def handle_src_files(path):
    filelist = { 
                 "CoreDefs.cpp" : "Definitions of Global parameters",
                 "MpfrIO.cpp" : "I/O functions for MPFR",
               }
    for key in filelist.keys():
        add_header(path+"src/", key, filelist[key])

def handle_python_files(path):
    filelist = { 
                 "core2.sip" : "Python Interface for Core Library 2",
                 "BigInt.sip" : "Python Interface for BigInt",
                 "BigRat.sip" : "Python Interface for BigRat",
                 "Mpfr.sip" : "Python Interface for Mpfr",
                 "BigFloat.sip" : "Python Interface for BigFloat",
                 "string.sip" : "Python Interface for std::string",
               }
    for key in filelist.keys():
        add_header(path+"python/", key, filelist[key])


if __name__ == "__main__":
    #print gen_header("CORE.h", "The main inclusion file for the Core Library")
    #handle_header_files("../../")
    #handle_src_files("../../")
    handle_python_files("../../")
