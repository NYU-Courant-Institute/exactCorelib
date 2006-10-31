# Makefile for testIO 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.4 2006-10-31 15:47:40 exact Exp $

include ../Make.options

#=================================================
# variables
#=================================================

verbose=
verbose=1

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= testIO  testMpfr testSqrt
MORETARGETS= 

default: $(TARGETS)

all: $(TARGETS) $(MORETARGETS)

test: $(TARGETS)
	./testIO
	./testMpfr 0
	./testSqrt 997 10 

moretest: $(MORETARGETS)
	./testSqrt 997 10 $(verbose)

testIO: testIO.o

testMpfr: testMpfr.o

testSqrt: testSqrt.o

sqrt: sqrt.o

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

