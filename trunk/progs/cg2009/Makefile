# Makefile for testPoint 
#
# Core Library, $Id: Makefile,v 1.1 2009/02/12 18:49:33 exact Exp $

include ../Make.options

#=================================================
# Define variables 
#=================================================

CORE_LEVEL=1

TARGETS= hw1${CORE_LEVEL} 
MORETARGETS= simple${LEV} pointOnPlane${LEV} golden

#=================================================
# Define targets
#=================================================

default: $(TARGETS)
	
test: $(TARGETS)
	./hw1${CORE_LEVEL}

moretest: $(MORETARGETS)
	./simple${LEV}
	./pointOnPlane${LEV}
	./golden

all: $(TARGETS) $(MORETARGETS)

p: ${p}
	./${p}${LEV} ${arg}

${p}: ${p}.o

hw1 : hw1.o

#=================================================
# Rules
#=================================================
%${CORE_LEVEL}: %.o
	${CXX} $(LDFLAGS) $< -lcorex++$(VAR)_level${CORE_LEVEL} $(CORE_LIB) -o $@

%: %.o
	${CXX} $(LDFLAGS) $< -lcorex++$(VAR)_level${CORE_LEVEL} $(CORE_LIB) -o $@

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
	echo "EXEPROGS=" $(EXEPROGS)
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)
	-@rm -f simple1$(EXETYPE) simple3$(EXETYPE) simple$(EXETYPE)
	-@rm -f golden1$(EXETYPE) golden3$(EXETYPE) golden$(EXETYPE)
	-@rm -f scheinerman1$(EXETYPE) scheinerman3$(EXETYPE) scheinerman$(EXETYPE)
	-@rm -f pointOnPlane1$(EXETYPE) pointOnPlane3$(EXETYPE) pointOnPlane$(EXETYPE)

save: 
	ci -l -m"Make save" Makefile *.cpp README
