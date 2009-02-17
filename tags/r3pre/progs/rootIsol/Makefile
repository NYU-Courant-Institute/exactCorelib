# Makefile for rootIsol
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.2 2009-01-04 18:16:05 sharma Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= t 

all: $(TARGETS)

test: $(TARGETS)
	./tRobust.sh

moretest:

# nothing to do

t: t.o  

tDescartes:tDescartes.o

testSturm:testSturm.o

tRootIsol:tRootIsol.o
#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(LDFLAGS) $? $(CORE_LIB) -o $@

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

