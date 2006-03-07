# Makefile for testIO 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2006-03-07 04:51:26 exact Exp $

include ../Make.options

#=================================================
# variables
#=================================================

verbose=
verbose=1

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= testIO 
MORETARGETS= testSqrt

default: $(TARGETS)

all: $(TARGETS) $(MORETARGETS)

test: $(TARGETS)
	./testIO

moretest: $(MORETARGETS)
	./testSqrt 997 10 $(verbose)

testIO: testIO.o

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

