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

TARGETS= bm_bigfloat 

#=================================================
# TARGETS
#=================================================

default: $(TARGETS)

test:  $(TARGETS)
	./bm_bigfloat 1 10000 

bench:  $(TARGETS)
	./bm_bigfloat 1 1000 
	./bm_bigfloat 1 10000 
	./bm_bigfloat 1 100000 

bm_bigfloat: bm_bigfloat.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC)  bm_bigfloat.cpp -o bm_bigfloat.o
	${CXX} $(LDFLAGS) bm_bigfloat.o $(CORE_LIB) -o bm_bigfloat

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
ALL=$(TARGETS) $(MORE_TARGETS) bm_bigfloat
EXEPROGS=$(ALL:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

