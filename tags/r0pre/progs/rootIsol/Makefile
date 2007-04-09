# Makefile for rootIsol
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2006-03-07 04:51:25 exact Exp $

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

real: real.o

real2: real2.o

simple: simple.o

simple2: simple2.o

t: t.o  

tDescartes:tDescartes.o

testSturm:testSturm.o

danielbug:danielbug.o

danielbug1:danielbug1.o

danielbug2:danielbug2.o

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

