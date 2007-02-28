# Makefile for testIO 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.6 2007-02-28 19:47:06 exact Exp $

include ../Make.options

#=================================================
# variables
#=================================================

verbose=
verbose=1

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= testIO  testMpfr testSqrt testCompatible
MORETARGETS= 

default: $(TARGETS)

all: $(TARGETS) $(MORETARGETS)

test: $(TARGETS)
	./testIO
	./testMpfr 0
	./testSqrt 997 10 
	./testCompatible 

moretest: $(MORETARGETS)
	./testSqrt 997 10 $(verbose)
	./testCompatible 777.888 777888e-3 1

testIO: testIO.o

testMpfr: testMpfr.o

testSqrt: testSqrt.o

sqrt: sqrt.o

testCompatible: testCompatible.o

cur: testCompatible

#=================================================
# Rules
#=================================================
%: %.o
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
EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORETARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)
	-@test -z Core_Diagnostics || rm -f Core_Diagnostics

