# Makefile for gaussian
#
# Core Library, $Id: Makefile,v 1.3 2007-04-08 02:43:01 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= gaussian_level3 gaussian_level1

MORETARGETS= gaussianX_level3 gaussianX_level1

default: $(TARGETS)


test: $(TARGETS)
	./gaussian_level3 inputs/MatHilbert6 3
	./gaussian_level1 inputs/MatHilbert6 3
	./gaussian_level3 inputs/MatZero4 3
	./gaussian_level1 inputs/MatZero4 3

moretest: $(TARGETS) $(MORETARGETS)
	./gaussian_level3 inputs/gregA 3
	./gaussian_level1 inputs/gregA 3
	./gaussian_level3 inputs/gregE 3
	./gaussian_level1 inputs/gregE 3

simpletest: $(TARGETS)
	./gaussian_level3 inputs/MatHilbert6 1
	./gaussian_level1 inputs/MatHilbert6 1

%_level1: %_level1.o 
	${CXX} $(LDFLAGS) $? $(CORE_LIB) -o $@

%_level3: %_level3.o
	${CXX} $(LDFLAGS) $? $(CORE_LIB) -o $@

#=================================================
# Rules for CORE_LEVEL 1 and CORE_LEVEL 3
#=================================================
%_level1.o: %.cpp
	${CXX} -c -DCORE_LEVEL=1 $(CXXFLAGS) $(CORE_INC) $< -o $@

%_level3.o: %.cpp
	${CXX} -c -DCORE_LEVEL=3 $(CXXFLAGS) $(CORE_INC) $< -o $@

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

