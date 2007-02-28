# Makefile for Radical
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.5 2007-02-28 22:18:16 exact Exp $

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

TARGETS= tConst tElemFun tManyDigits 

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

bench: moretest

all: $(TARGETS) 

tConst: tConst.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) tConst.cpp -o tConst.o
	${CXX} $(LDFLAGS) tConst.o $(CORE_LIB) -o tConst

tElemFun: tElemFun.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) tElemFun.cpp -o tElemFun.o
	${CXX} $(LDFLAGS) tElemFun.o $(CORE_LIB) -o tElemFun

cur tElemFunBF: tElemFunBF.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) tElemFunBF.cpp -o tElemFunBF.o
	${CXX} $(LDFLAGS) tElemFunBF.o $(CORE_LIB) -o tElemFunBF

t: t.cpp
t: t.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) t.cpp -o t.o
	${CXX} $(LDFLAGS) t.o $(CORE_LIB) -o t

	
tManyDigits: tManyDigits.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) tManyDigits.cpp -o tManyDigits.o
	${CXX} $(LDFLAGS) tManyDigits.o $(CORE_LIB) -o tManyDigits

time:
	time ./tConst 10000
	time ./tElemFun 10000

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

