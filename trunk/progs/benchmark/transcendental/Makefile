# Makefile for Radical
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.8 2008-10-31 20:08:48 exact Exp $

#=================================================
# VARIABLES
#=================================================
#CHOOSE THE LINKAGE 
LINKAGE=static
LINKAGE=shared

CORE_PATH=../../..
include ${CORE_PATH}/progs/Make.options

#=================================================
# TARGET FILES
#=================================================

TARGETS= tConst tElemFun tElemFunBF tManyDigits 

#=================================================
# TARGETS
#=================================================

default: $(TARGETS)

test:  $(TARGETS)
	./tConst 1000 1
	./tElemFun 
	./tElemFunBF 

moretest: $(TARGETS)
	./tConst 10000
	./tConst 100000
	./tElemFun 
	./tElemFunBF 

time:
	time ./tConst 10000
	time ./tElemFun 

all: $(TARGETS) 

t: t.o
	
bench: moretest

tConst: tConst.o

tElemFun: tElemFun.o

tElemFunBF2: tElemFunBF.o

tManyDigits: tManyDigits.o

tZimmerman : tZimmerman.o

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

