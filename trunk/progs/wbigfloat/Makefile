# Makefile for gaussian
#
# Core Library, $Id: Makefile,v 1.1 2007-10-22 19:34:37 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= wbigfloat

default: $(TARGETS)

wbigfloat: wbigfloat.o
	${CXX} $(LDFLAGS) $? $(CORE_LIB) $(COREX_LIB_LEVEL3) -o $@

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

