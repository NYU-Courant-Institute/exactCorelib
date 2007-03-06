# Makefile for Radical
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.6 2007-03-06 23:49:24 exact Exp $

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
	./tElemFun 1000 1
	./tElemFunBF 1000 1

moretest: $(TARGETS)
	./tConst 10000
	./tConst 100000
	./tElemFun 10000
	./tElemFunBF 100000

time:
	time ./tConst 10000
	time ./tElemFun 10000

all: $(TARGETS) 

t: t.o
	
bench: moretest

tConst: tConst.o

tElemFun: tElemFun.o

tElemFunBF2: tElemFunBF.o

tManyDigits: tManyDigits.o

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

