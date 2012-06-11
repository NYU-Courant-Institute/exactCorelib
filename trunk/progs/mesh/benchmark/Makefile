# Makefile for root isolation benchmarks.
#
# Core Library, $Id: Makefile,v 1.10 2010/11/09 04:44:05 exact Exp $

CORE_PATH=../../..
include ../Make.options

#=================================================
# Define variables
#=================================================
# -lprofiler has problems on various platforms:
PROFILE=-lprofiler
PROFILE=

DATA=../../data/polynomials
FRISCO=../../data/frisco

VERB=-v
VERB=
v=1
verbose=$(v)
#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = main_ceval main_newt

all: $(TARGETS)

test: $(TARGETS)
	python ./run_tests.py

moretest: $(TARGETS)
# nothing to do

# The following is for visualization and interactive mode:
#
#  NOTE that you can zoom-in (i) or zoom-out (k), pan (w,a,s,d) to
#  view the display.
#
# eg1 (20 roots of unity)
eg1: main_ceval
	main_ceval --poly ./data/nroots20.pol --display --use_root_bounds

# eg2
#
eg2 : main_ceval
	main_ceval --poly ./data/somepoly6.pol --display --use_root_bounds

# 
# eg3 (laguerre20 polynomial) gives explicit bounding box to search for roots:
eg3 : main_ceval
	main_ceval --poly ./data/laguerre20.pol --display \
		--x_max 70 --x_min 0 --y_min -1 --y_max 1

# eg4 (laguerre20 polynomial) uses root bound calculations that
# ensure that all roots are found:
eg4 : main_ceval
	main_ceval --poly ./data/laguerre20.pol --display --use_root_bounds

# eg


main_ceval : main_ceval.o benchmark.o ../ceval/algorithm.o \
		../ceval/predicates.o ../ceval/tmp-display.o 
	${CXX} $(LDFLAGS) benchmark.o main_ceval.o ../ceval/algorithm.o \
		../ceval/predicates.o ../ceval/tmp-display.o $(CORE_LIB) -o $@

main_newt : main_newt.o benchmark.o ../krawczyk/tmp-display.o
	${CXX} $(LDFLAGS) benchmark.o main_newt.o \
		../krawczyk/tmp-display.o $(CORE_LIB) $(PROFILE) -o $@

complexCurves c: complexCurves.o benchmark.o ../ceval/algorithm.o \
		../ceval/predicates.o ../ceval/tmp-display.o 
	${CXX} $(LDFLAGS) benchmark.o complexCurves.o ../ceval/algorithm.o \
		../ceval/predicates.o ../ceval/tmp-display.o $(CORE_LIB) -o $@

# Examples for complexCurves
c1:	complexCurves
	complexCurves --poly $(DATA)/simplePolynomials/rootsOfUnity2.pol $(VERB)

c2:	complexCurves
	complexCurves --poly $(DATA)/simplePolynomials/identity.poL $(VERB)

c3:	complexCurves
	complexCurves --poly $(FRISCO)/hermite20.pol $(VERB)

c4:	complexCurves
	complexCurves --poly $(DATA)/specialPolynomials/goldenratio.pol $(VERB)

c5:	complexCurves
	complexCurves --poly ./data/hermite20.pol  $(VERB)

c6:	complexCurves
	complexCurves --poly $(DATA)/simplePolynomials/rootsOfUnity3.pol $(VERB)

converPoly: convertPoly.o

cv:	convertPoly.o 
	${CXX} $(LDFLAGS) convertPoly.o $(CORE_LIB) -o $@

cv1:	convertPoly
	./convertPoly "D" "(x-1)^5" "output/fifthRootUnity" 1
	cp output/fifthRootUnity* ../../data/complexCurve

cv2:	convertPoly
	./convertPoly "D" "1-x+x^3-x^4+x^5-x^6+x^7-x^9+x^10" "output/lehmer" 1
	cp output/lehmer* ../../data/complexCurve

cv3:	convertPoly
	./convertPoly "H" "data2/fifthRootUnity.poly" "output/fifthRootUnity" 1
	cp output/fifthRootUnity* ../../data/complexCurve

cv4:	convertPoly
	./convertPoly "H" "data2/lehmer.poly" "output/lehmer" 1
	cp output/lehmer* ../../data/complexCurve

cv5:	convertPoly
	./convertPoly "H" "data2/golden.poly" "output/golden" 1
	cp output/golden* ../../data/complexCurve

cv6:	convertPoly
	./convertPoly "F" "data/somepoly6.pol" "output/somepoly6.pol" 1
	cp output/somepoly6* ../../data/complexCurve

cv7:	convertPoly
	./convertPoly "F" "data/hermite20.pol" "output/hermite20.pol" 1
	cp output/hermite20* ../../data/complexCurve

t:	t.o
	${CXX} $(LDFLAGS) t.o $(CORE_LIB) -o $@

t1:	t
	./t "(x-1)^5" "output/fifthRootUnity" 1
	cp output/fifthRootUnity* ../../data/complexCurve

t2:	t
	./t "3x^3 -2 + 2x^2"  "output/cubic1" 1
	cp output/cubic1* ../../data/complexCurve

t3:	t
	./t "1-x+x^3-x^4+x^5-x^6+x^7-x^9+x^10" "data/lehmer" 1
	cp data/lehmer* ../../data/complexCurve

#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c $(CXXFLAGS) -O3 $(CORE_INC) $< -o $@

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o
	-@test -z "../ceval/*.o" || rm -f ../ceval/*.o
	-@test -z "../krawczyk/*.o" || rm -f ../krawczyk/*.o
	-@test -z "t.o" || rm -f t.o

m:
	gvim Makefile

#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)
	-@test -z "t" || rm -f t
	-@test -z "convertPoly" || rm -f convertPoly
	-@test -z "cv" || rm -f cv


