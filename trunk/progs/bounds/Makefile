# Makefile for Radical
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.5 2010/06/10 16:14:28 exact Exp $

#=================================================
# VARIABLES
#=================================================
#CHOOSE THE LINKAGE 
LINKAGE=static
LINKAGE=shared

CORE_PATH=../..
include ${CORE_PATH}/progs/Make.options

#=================================================
# TARGET FILES
#=================================================

TARGETS= tEscapeBound tCutOffBound

#=================================================
# TARGETS
#=================================================

default: $(TARGETS)

test:  $(TARGETS)
	time ./tEscapeBound 2000
	time ./tEscapeBound 5000
	time ./tCutOffBound 200
	time ./tCutOffBound 700

moretest:

all: $(TARGETS) 

tEscapeBound : tEscapeBound.o

tCutOffBound : tCutOffBound.o

#=================================================
# Rules
#=================================================
%: %.o $(DEPEND)
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

# NOTE: this used to pass -ffriend-injection on non-mac platforms, to work
# around the friend-name-injection issue described in README.  That is no
# longer needed (the workaround now lives in Expr.h, via a static pi(Expr)),
# and the flag was removed from g++ in GCC 8 -- it now only warns, and other
# compilers reject it outright.
.cpp.o:
	${CXX} -c $(CXXFLAGS) $(CORE_INC) $< -o $@

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o

#=================================================
# Remove executable files
#=================================================
ALL=$(TARGETS) $(MORE_TARGETS) tConst tElemFun
EXEPROGS=$(ALL:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

