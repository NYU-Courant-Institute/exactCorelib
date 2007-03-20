# Makefile for Radical
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.2 2007-03-20 14:36:27 exact Exp $

#=================================================
# VARIABLES
#=================================================
#CHOOSE THE LINKAGE 
LINKAGE=static
LINKAGE=shared

#CHOOSE THE VARIANT Core Library you need
VAR=GCC31
VAR=Debug
VAR=

CORE_PATH=../../..
include ${CORE_PATH}/progs/Make.options

#=================================================
# TARGET FILES
#=================================================

TARGETS= tSum2 
MORE_TARGETS= tSumALL

#=================================================
# TARGETS
#=================================================

default: $(TARGETS)

more: $(MORE_TARGETS)

test:  $(TARGETS)
	./tSum2 1000 

moretest: $(MORE_TARGETS)
	./tSum2 1000
	./tSum2 10000
	./tSum2 100000
	./tSum2 1000000
	./tSum1 1000
	./tSum1 10000
	./tSum1 100000
	./tSum1 1000000

bench: moretest

all: $(TARGETS) $(MORE_TARGETS) 


# Compiling tSum with(without) using summation operator various number types

tSumALL: tSum1 tSum2

tSum1: tSum.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC)  tSum.cpp -o tSum1.o
	${CXX} $(LDFLAGS) tSum1.o $(CORE_LIB) -o tSum1

tSum2: tSum.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_USE_SUM tSum.cpp -o tSum2.o
	${CXX} $(LDFLAGS) tSum2.o $(CORE_LIB) -o tSum2

#=================================================
# Rules
#=================================================
%: %.o $(DEPEND)
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

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
ALL=$(TARGETS) $(MORE_TARGETS) tSum1 tSum2
EXEPROGS=$(ALL:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

