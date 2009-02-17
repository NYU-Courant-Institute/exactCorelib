# Makefile for testPoint 
#
# Core Library, $Id: Makefile,v 1.1 2006-03-07 04:51:23 exact Exp $

include ../Make.options

#=================================================
# Define variables 
#=================================================

p=pointOnPlane
p=simple
p=golden
p=scheinerman

ifile=i
arg= < inputs/${ifile}
arg=

LEV=1
LEV=3
TARGETS= scheinerman${LEV} 
MORETARGETS= simple${LEV} pointOnPlane${LEV} golden

#=================================================
# Define targets
#=================================================

default: $(TARGETS)
	
test: $(TARGETS)
	./scheinerman${LEV}

moretest: $(MORETARGETS)
	./simple${LEV}
	./pointOnPlane${LEV}
	./golden

all: $(TARGETS) $(MORETARGETS)

p: ${p}
	./${p}${LEV} ${arg}

${p}: ${p}.o

pointOnCircle: pointOnCircle.o

pointOnPlane${LEV}: pointOnPlane.o

scheinerman${LEV}: scheinerman.o

golden: golden.o

simple: simple.o

t: t.o

#=================================================
# Rules
#=================================================
%${LEV}: %.o
	${CXX} $(LDFLAGS) $< -lcorex++$(VAR)_level${LEV} $(CORE_LIB) -o $@

%: %.o
	${CXX} $(LDFLAGS) $< -lcorex++$(VAR)_level${LEV} $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c $(CXXFLAGS) -DCORE_LEVEL=${LEV} $(CORE_INC) $< -o $@

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
