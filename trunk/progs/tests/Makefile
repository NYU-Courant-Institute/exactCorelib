# Core Library, $Id: Makefile,v 1.3 2006-10-31 16:29:21 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
p= floorLg
p= zero
p= wein_expr_3
p= tExpr
p= test

TARGETS=
MORETARGETS= floor_ceil bench

#CXXFLAGS+= -pg
# CXXFLAGS=-O -Wall
CXXFLAGS=-O -Wall -DCORE_DEBUG_ROOTBOUND -g -DCORE_DEBUG

all: $(TARGETS)

p ${p}: ${p}.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) ${p}.cpp -o ${p}.o
	${CXX} $(LDFLAGS) ${p}.o $(CORE_LIB) -o ${p}
	
	
test: $(TARGETS)

moretest: $(MORETARGETS)
	./floor_ceil
	./bench

floor_ceil: floor_ceil.o

floorLg: floorLg.o

length: length.o

invalid: invalid.o

bench: bench.o

wein: wein_expr_3.o

bench1: bench1.o

tBigFloat: tBigFloat.o

tt: tt.o

itoa: itoa.o

tExpr: tExpr.o

t: t.o

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
EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORETARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

