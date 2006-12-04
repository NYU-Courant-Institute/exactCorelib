# Makefile for Radical
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2006-12-04 22:04:29 exact Exp $

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

TARGETS= tConst tElemFun 

#=================================================
# TARGETS
#=================================================

default: $(TARGETS)

test:  $(TARGETS)
	./tConst 1000 1
	./tElemFun 1000 1

moretest: $(TARGETS)
	./tConst 1000
	./tConst 10000
	./tConst 100000
	./tElemFun 1000
	./tElemFun 10000
	./tElemFun 100000

bench: moretest

all: $(TARGETS) 

tConst: tConst.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) tConst.cpp -o tConst.o
	${CXX} $(LDFLAGS) tConst.o $(CORE_LIB) -o tConst

tElemFun: tElemFun.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) tElemFun.cpp -o tElemFun.o
	${CXX} $(LDFLAGS) tElemFun.o $(CORE_LIB) -o tElemFun
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
ALL=$(TARGETS) $(MORE_TARGETS) tConst tElemFun
EXEPROGS=$(ALL:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

