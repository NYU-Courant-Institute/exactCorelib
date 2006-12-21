# Makefile for Curves 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.2 2006-12-21 17:41:14 exact Exp $

#=================================================
# VARIABLES
#=================================================
#CHOOSE THE LINKAGE 
LINKAGE=static
LINKAGE=shared

#CHOOSE THE VARIANT Core Library you need
#VAR=GCC31
#VAR=Debug
#VAR=
#VAR=Opt

CORE_PATH=../..
include ${CORE_PATH}/progs/Make.options

#=================================================
# VARIABLES 
#=================================================

DEPEND=
TARGETS= tCurves
MORE_TARGETS =  parseCurve tIntersect plotCurve
OTHER_TARGETS = 

n=1
n=

p=tt
p=tCurves
p=moreCurves
p=plotCurve
p=tPcurves
p=parseCurve

ifile=plot

verbose=1
verbose=

#=================================================
# TARGETS
#=================================================

default: $(TARGETS)

more: $(MORE_TARGETS)

test:  $(TARGETS)
	./tCurves 0
	cp data/plot data/plot_tCurves0
	./tCurves 1

moretest: $(MORE_TARGETS)
	./parseCurve 
	./tIntersect 1 2 
	cp data/plot2 data/plot_tCurves1
	./plotCurve "y^2 - x^3"

othertest: $(OTHER_TARGETS)

p: $(p).o $(DEPEND)
	${CXX} $(LDFLAGS)  $< $(CORE_LIB) -o ${p}

all: $(TARGETS) $(MORE_TARGETS) 

testall: test moretest

time:  tCurves
	time tCurves

# Compile with different number types
curves: tCurves.cpp
	${CXX} -c $(CXXFLAGS) \
		$(CORE_INC)            tCurves.cpp -o tCurves.o
	${CXX} $(LDFLAGS) tCurves.o $(CORE_LIB) -o tCurves
	${CXX} -c $(CXXFLAGS) \
		$(CORE_INC) -D_POLYINT tCurves.cpp -o tCurvesInt.o
	${CXX} $(LDFLAGS) tCurvesInt.o $(CORE_LIB) -o tCurvesInt

debugNewton: tCurves.cpp
	${CXX} -c -DCORE_DEBUG_NEWTON $(CXXFLAGS) \
		$(CORE_INC)            tCurves.cpp -o tCurves.o
	${CXX} $(LDFLAGS) tCurves.o $(CORE_LIB) -o tCurves

tC tCurves: tCurves${n}.o

tPc tPcurves: tPcurves${n}.o

tI tIntersect: tIntersect${n}.o

mC moreCurves: moreCurves${n}.o

cI curveInput: curveInput.o

pC parseCurve: parseCurve.o

tt:	tt.o 

tt1:	tt1.o

tt2:	tt2.o 

tt3:	tt3.o 

plotCurve pc: plotCurve.o

s show:
	../../ext/graphics/drawCurve$(EXETYPE) $(verbose) < data/${ifile}

s2 show2:
	../../ext/graphics/drawCurve$(EXETYPE) $(verbose) < data/${ifile}2

d debug:
	../../ext/graphics/drawCurve$(EXETYPE) verbose < data/${ifile}

#=================================================
# Rules
#=================================================
%: %.o $(DEPEND)
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
ALL=$(TARGETS) $(MORE_TARGETS)
EXEPROGS=$(ALL:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)
	-@rm -f tCurves${EXETYPE}

#=================================================
# Housekeeping
#=================================================

tar:
	-@tar cvf curves.tar \
		*cpp Makefile README

save:
	-@ci -l -m"make save" *.cpp *.tcc *.h Makefile README Notes


