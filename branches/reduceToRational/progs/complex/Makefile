# Makefile for bareiss 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2010/06/22 16:25:15 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = tComplex

all: $(TARGETS)

test: $(TARGETS)
	./tComplex

moretest: $(TARGETS)
# nothing to do

tComplex: tComplex.o


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

