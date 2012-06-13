# Makefile for mesh/ceval 
#
# 	This directory contains the main implementation of the
# 	CEVAL Algorithm of Sagraloff and Yap.
# 	The implementation is part of Narayan Kamath's MSc Thesis
# 	at Oxford, supervised by Irina Voiculescu and Chee Yap.
#
# 	The visualation and benchmarking of CEVAL is implemented
# 	in the parallel directory mesh/benchmark.
#
# Chee Yap (June 2012)
# Since CORE 2.1
#
# Core Library, $Id: Makefile,v 1.11 2011/05/05 16:20:23 exact Exp $

CORE_PATH=../../..
include ../Make.options

#=================================================
# Variables
#=================================================
p="x^3-5"
xmin=-2.0
ymin=-2.0
boxwidth=4.0

# compiling the two versions of predicates.cpp:
ORIG=0
#=================================================
# TARGETS
#=================================================
TARGETS = ceval
DEPENDS = algorithm.o predicates.o ceval.o

all: $(TARGETS)

test: $(TARGETS)
	@echo "\n Test: runnng 'ceval' with all default arguments"
	@echo " ==============================================="
	./ceval

moretest: $(TARGETS)
# nothing to do

ceval: $(DEPENDS)
	${CXX} $(LDFLAGS) $(CORE_LIB) $(DEPENDS) -o $@
#	${CXX} $(LDFLAGS) ceval.o algorithm.o predicates.o $(CORE_LIB) -o $@

#ceval.o:
#	${CXX} -c -O3 -DORIGINAL=$(ORIG) $(CXXFLAGS) $(CORE_INC) $< -o $@

#predicate.o:
#	${CXX} -c -O3 -DORIGINAL=$(ORIG) $(CXXFLAGS) $(CORE_INC) $< -o $@
#=================================================
# TCLAP:
#=================================================
tclap: ./tclap-example
	./tclap-example --help

tclap1: ./tclap-example
	./tclap-example -n Yap

tclap2: ./tclap-example
	./tclap-example -n Yap -r

tclap-example: tclap-example.o

#=================================================
# EXAMPLES
#=================================================
eg: ./ceval
	@echo "\n============== USING Makefile DEFAULTS ============="
	./ceval $(p) $(xmin) $(ymin) $(boxwidth)

eg1: ./ceval
	@echo "\n============== THE GOLDEN RATIO POLYNOMIAL ============="
	./ceval "x^2-x-1" $(xmin) $(ymin) $(boxwidth)

eg2: ./ceval
	@echo "\n============== A DEGREE 10 POLYNOMIAL ============="
	@echo "     This is example of Fig.1 in the paper"
	@echo "     'Empirical Study of an Evaluation-based Subdivision...'"
	@echo "     by Kamath, Voiculescu, Yap (SNC 2011) "
	@echo "====================================================\n"
	./ceval "x(9x^9 + 7x^8 + 8x^7 + 8x^6 + 6x^4 + 5x^3 + 2x^2 + 1)" \
		$(xmin) $(ymin) $(boxwidth)

eg3: ./ceval
	@echo "\n============== 7th ROOTS OF UNITY ============="
	./ceval "x^7-1" $(xmin) $(ymin) $(boxwidth)

#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

#ceval.o:
#	${CXX} -c -O3 -DORIGINAL=$(ORIG) $(CXXFLAGS) $(CORE_INC) $< -o $@

.cpp.o:
	${CXX} -c -DORIGINAL=$(ORIG) -O3 $(CXXFLAGS) $(CORE_INC) $< -o $@

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o
	-@test -z "*~" || rm -f *~

m:
	gvim Makefile

options:
	cd ../../../; make options

#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)
	-@test -z "tclap-example" || rm -f tclap-example

#=================================================
# End
#=================================================

