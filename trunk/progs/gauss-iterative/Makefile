# Makefile for bcir
#
# Core Library, $Id: Makefile,v 1.1 2007-10-18 16:04:45 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= bcir_level1

# MORETARGETS= 

default: $(TARGETS)


test: $(TARGETS)
	./bcir_level1 inputs/matrix_d4_1 inputs/vector_d4_1 200
#	./bcir_level1 inputs/matrix_d10_1 inputs/vector_d10_1 200
#	./bcir_level1 inputs/matrix_d10_2 inputs/vector_d10_2 200

moretest: $(TARGETS) $(MORETARGETS)

simpletest: $(TARGETS)

%_level1: %_level1.o 
	${CXX} $(LDFLAGS) $? $(CORE_LIB) $(COREX_LIB_LEVEL1) -o $@


#=================================================
# Rules for CORE_LEVEL 1 and CORE_LEVEL 3
#=================================================
%_level1.o: %.cpp
	${CXX} -c -DCORE_LEVEL=1 $(CXXFLAGS) $(CORE_INC) $< -o $@


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

