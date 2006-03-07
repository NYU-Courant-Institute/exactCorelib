# Makefile for compare 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2006-03-07 04:51:23 exact Exp $

VAR=Bfms
VAR=Bfmss
VAR=
include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= compare

all: $(TARGETS)

test: $(TARGETS)
	./compare 10 1

moretest:
# nothing to do

testslow: $(TARGETS)
	./compare 1000 2

compare: compare.o

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
EXEPROGS=$(TARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

