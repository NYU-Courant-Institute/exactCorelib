# Makefile for bugs 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2010/04/30 10:10:56 exact Exp $

include ../Make.options

#=================================================
# variables
#=================================================

verbose=
verbose=1

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= badOutputDigits
MORETARGETS= 

default: $(TARGETS)

all: $(TARGETS) $(MORETARGETS)

test: $(TARGETS)
	./badOutputDigits

moretest: $(MORETARGETS)

badOutputDigits: badOutputDigits.o 

r run: badOutputDigits
	./badOutputDigits

t: t.o

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

