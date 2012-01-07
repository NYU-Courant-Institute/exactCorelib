# Makefile for mesh/ceval 
#
# (template Makefile for simple targets)
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

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = ceval

all: $(TARGETS)

test: $(TARGETS)
	./ceval

moretest: $(TARGETS)
# nothing to do

ceval: ceval.o algorithm.o predicates.o
	${CXX} $(LDFLAGS) ceval.o algorithm.o predicates.o $(CORE_LIB) -o $@

#=================================================
# TCLAP:
#=================================================
tclap: tclap-example
	./tclap-example --help

tclap1: tclap-example
	./tclap-example -n Yap

tclap2: tclap-example
	./tclap-example -n Yap -r

tclap-example: tclap-example.o

#=================================================
# Examples
#=================================================
eg:
	./ceval $(p) $(xmin) $(ymin) $(boxwidth)

eg1:
	./ceval "x^2-x-1" $(xmin) $(ymin) $(boxwidth)

eg2:
	./ceval "x^3-2x+2" $(xmin) $(ymin) $(boxwidth)

# this is the example of Fig.1 of
# "Empirical Study of an Evaluation-based Subdivision..."
# by Kamath, Voiculescu, Yap. (SNC 2011)
eg3: 
	./ceval "x(9x^9 + 7x^8 + 8x^7 + 8x^6 + 6x^4 + 5x^3 + 2x^2 + 1)" \
		$(xmin) $(ymin) $(boxwidth)

#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c -O3 $(CXXFLAGS) $(CORE_INC) $< -o $@

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o

#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)
	-@test -z "tclap-example" || rm -f tclap-example

