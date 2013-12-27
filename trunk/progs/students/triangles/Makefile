# Makefile for triangles 
#
# Core Library, $Id: Makefile,v 1.4 2010/11/23 18:00:52 exact Exp $

include ../Make.options

LEVEL := 1

#=================================================
# TARGET FILES
#=================================================
TARGETS := code-original_level${LEVEL}
MORETARGETS := code-original_level1 code-original_level2 code-original_level3

#=================================================
# TARGETS
#=================================================

default: $(TARGETS)

all: $(TARGETS) 

test:  $(TARGETS)
	./code-original_level$(LEVEL)

moretest: $(MORETARGETS)
	./code-original_level1
	./code-original_level2
	./code-original_level3

testall: moretest

code-original_level${LEVEL}: code-original_level${LEVEL}.o

#=================================================
# Rules
#=================================================
%_level${LEVEL}: %_level${LEVEL}.o $(DEPEND)
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

%_level${LEVEL}.o : %.cpp
	${CXX} -c $(CXXFLAGS) -DCORE_LEVEL=${LEVEL} $(CORE_INC) $< -o $@

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o

#=================================================
# Remove executable files
#=================================================
ALL=$(TARGETS)
EXEPROGS=$(ALL:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

