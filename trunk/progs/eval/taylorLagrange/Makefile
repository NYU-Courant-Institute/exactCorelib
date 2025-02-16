# Makefile for C++ programming

###################################################
# VARIABLES
###################################################
# 	THIS SECTION CONTAINS THE ONLY CUSTOMIZATION YOU NEED
# 	See the -include command at end of this section
###################################################

# TARGETS:
TARGETS=interval polynomial eval evalF evalR evalT2min \
	evalT2 evalT3 evalT4 evalL3 evalL3cheap evalL4 evalL4cheap \
	t all table1 table2 table3

# main file
p=eval

# TOP LEVEL CHOICE TASK: edit or compile
d=vi
d=cpp
d=gcc

# compiler flags
c++flags= -std=c++11 -Ofast
lflags= -lstdc++ -lgmp -lgmpxx
CXX=gcc
LDFLAGS= -lstdc++ -lgmp -lgmpxx
CXXFLAGS=
CORE_LIB=
CORE_INC=

# default arguments for testing program
def_file=< test1.txt
def_verb=1

# INCLUDE a local file (perhaps to override above definitions)
-include make.preclude

######################################################
# TRY NOT TO CUSTOMIZE BELOW THIS LINE!
######################################################

# TOP LEVEL Targets
ifeq ($(d),vi) # 1
	ext=.tex
else
ifeq ($(d),cpp) # 2
	ext=.cpp
endif #2
endif #1

pclass := $(p).class
###################################################
# TARGETS
###################################################
default: $(d)

############################### To compile Kai's code
gcc:
	gcc ${p}.cpp ${c++flags} ${lflags} -o ${p}${exe}

r run: ${p}
	${p}${exe} ${def_file}

############################### general Eval with choice
eval:   eval.h eval.cpp evalT2.h evalT3.h evalT4.h evalL3.h \
			interval.h polynomial.h base.h
	make p=eval

t-eval: eval
	./eval t2 < data/test1.txt	# read from file, with quadratic Taylor forms
	./eval t3 < data/test1.txt	# read from file, with cubic Taylor forms
	./eval t4 < data/test1.txt	# read from file, with quartic Taylor forms
	./eval l3 < data/test1.txt	# read from file, with cubic Lagrange forms
	./eval l3c < data/test1.txt	# read from file, with "cheap" cubic Lagrange forms
	./eval l4 < data/test1.txt	# read from file, with quartic Lagrange forms
	./eval l4c < data/test1.txt	# read from file, with "cheap" quartic Lagrange forms

t-eval0: eval
	./eval t2 0			# do unit test, with quadratic Taylor forms
	./eval t3 0	    # do unit test, with cubic Taylor forms
	./eval t4 0  	  # do unit test, with quartic Taylor forms
	./eval l3 0			# do unit test, with cubic Lagrange forms
	./eval l3c 0		# do unit test, with "cheap" cubic Lagrange forms
	./eval l4 0			# do unit test, with quartic Lagrange forms
	./eval l4c 0		# do unit test, with "cheap" quartic Lagrange forms

t-eval1: eval
	./eval t2 1			# eval on some built-in polynomial, with quadratic Taylor forms
	./eval t3 1			# eval on some built-in polynomial, with cubic Taylor forms
	./eval t4 1			# eval on some built-in polynomial, with quartic Taylor forms
	./eval l3 1			# eval on some built-in polynomial, with cubic Lagrange forms
	./eval l3c 1		# eval on some built-in polynomial, with "cheap" cubic Lagrange forms
	./eval l4 1			# eval on some built-in polynomial, with quartic Lagrange forms
	./eval l4c 1		# eval on some built-in polynomial, with "cheap" quartic Lagrange forms

############################### general Eval with choice and floating point arithmetic
evalF: eval.h eval.cpp evalT2.h evalT3.h evalT4.h evalL3.h interval.h polynomial.h baseF.h
	cp baseF.h base.h
	make p=eval
	cp eval.exe evalF.exe

############################### general Eval with choice and rational arithmetic
evalR: eval.h eval.cpp evalT2.h evalT3.h evalT4.h evalL3.h interval.h polynomial.h baseR.h
	cp baseR.h base.h
	make p=eval
	cp eval.exe evalR.exe

############################### generate Table 1 with tree sizes
table1: eval
	@cat data/chebyshev020.pol data/chebyshev.int | ./eval at # compute tree sizes for T_20
	@cat data/chebyshev040.pol data/chebyshev.int | ./eval at # compute tree sizes for T_40
	@cat data/chebyshev080.pol data/chebyshev.int | ./eval At # compute tree sizes for T_80
	@cat data/chebyshev160.pol data/chebyshev.int | ./eval At # compute tree sizes for T_160
	@cat data/chebyshev320.pol data/chebyshev.int | ./eval At # compute tree sizes for T_320
	@cat data/hermite020.pol data/hermite.int | ./eval at # compute tree sizes for H_20
	@cat data/hermite040.pol data/hermite.int | ./eval at # compute tree sizes for H_40
	@cat data/hermite080.pol data/hermite.int | ./eval At # compute tree sizes for H_80
	@cat data/hermite160.pol data/hermite.int | ./eval At # compute tree sizes for H_160
	@cat data/hermite320.pol data/hermite.int | ./eval At # compute tree sizes for H_320
	@cat data/wilk020.pol data/wilk.int | ./eval at # compute tree sizes for W_20
	@cat data/wilk040.pol data/wilk.int | ./eval At # compute tree sizes for W_40
	@cat data/wilk080.pol data/wilk.int | ./eval At # compute tree sizes for W_80
	@cat data/wilk160.pol data/wilk.int | ./eval At # compute tree sizes for W_160
	@cat data/wilk320.pol data/wilk.int | ./eval At # compute tree sizes for W_320
	@cat data/sparse0100.pol data/sparse.int | ./eval at # compute tree sizes for S_100
	@cat data/sparse0200.pol data/sparse.int | ./eval at # compute tree sizes for S_200
	@cat data/sparse0400.pol data/sparse.int | ./eval at # compute tree sizes for S_400
	@cat data/sparse0800.pol data/sparse.int | ./eval at # compute tree sizes for S_800

############################### generate Table 2 with running times for floating point arithmetic
table2: evalF
	@cat data/chebyshev020.pol data/chebyshev.int | ./evalF ar 80 # compute running times for T_20
	@cat data/chebyshev040.pol data/chebyshev.int | ./evalF ar 40 # compute running times for T_40
	@cat data/chebyshev080.pol data/chebyshev.int | ./evalF Ar 20 # compute running times for T_80
	@cat data/chebyshev160.pol data/chebyshev.int | ./evalF Ar 10 # compute running times for T_160
	@cat data/chebyshev320.pol data/chebyshev.int | ./evalF Ar 5 # compute running times for T_320
	@cat data/hermite020.pol data/hermite.int | ./evalF ar 80 # compute running times for H_20
	@cat data/hermite040.pol data/hermite.int | ./evalF ar 40 # compute running times for H_40
	@cat data/hermite080.pol data/hermite.int | ./evalF Ar 20 # compute running times for H_80
	@cat data/hermite160.pol data/hermite.int | ./evalF Ar 10 # compute running times for H_160
	@cat data/hermite320.pol data/hermite.int | ./evalF Ar 5 # compute running times for H_320
	@cat data/wilk020.pol data/wilk.int | ./evalF ar 80 # compute running times for W_20
	@cat data/wilk040.pol data/wilk.int | ./evalF Ar 40 # compute running times for W_40
	@cat data/wilk080.pol data/wilk.int | ./evalF Ar 20 # compute running times for W_80
	@cat data/wilk160.pol data/wilk.int | ./evalF Ar 10 # compute running times for W_160
	@cat data/wilk320.pol data/wilk.int | ./evalF Ar 5 # compute running times for W_320
	@cat data/sparse0100.pol data/sparse.int | ./evalF ar 40 # compute running times for S_100
	@cat data/sparse0200.pol data/sparse.int | ./evalF ar 20 # compute running times for S_200
	@cat data/sparse0400.pol data/sparse.int | ./evalF ar 10 # compute running times for S_400
	@cat data/sparse0800.pol data/sparse.int | ./evalF ar 5 # compute running times for S_800

############################### generate Table 3 with running times for rational arithmetic
table3: evalR
	@cat data/chebyshev020.pol data/chebyshev.int | ./evalR ar 80 # compute running times for T_20
	@cat data/chebyshev040.pol data/chebyshev.int | ./evalR ar 40 # compute running times for T_40
	@cat data/chebyshev080.pol data/chebyshev.int | ./evalR Ar 20 # compute running times for T_80
	@cat data/chebyshev160.pol data/chebyshev.int | ./evalR Ar 10 # compute running times for T_160
	@cat data/chebyshev320.pol data/chebyshev.int | ./evalR Ar 5 # compute running times for T_320
	@cat data/hermite020.pol data/hermite.int | ./evalR ar 80 # compute running times for H_20
	@cat data/hermite040.pol data/hermite.int | ./evalR ar 40 # compute running times for H_40
	@cat data/hermite080.pol data/hermite.int | ./evalR Ar 20 # compute running times for H_80
	@cat data/hermite160.pol data/hermite.int | ./evalR Ar 10 # compute running times for H_160
	@cat data/hermite320.pol data/hermite.int | ./evalR Ar 5 # compute running times for H_320
	@cat data/wilk020.pol data/wilk.int | ./evalR ar 80 # compute running times for W_20
	@cat data/wilk040.pol data/wilk.int | ./evalR Ar 40 # compute running times for W_40
	@cat data/wilk080.pol data/wilk.int | ./evalR Ar 20 # compute running times for W_80
	@cat data/wilk160.pol data/wilk.int | ./evalR Ar 10 # compute running times for W_160
	@cat data/wilk320.pol data/wilk.int | ./evalR Ar 5 # compute running times for W_320
	@cat data/sparse0100.pol data/sparse.int | ./evalR ar 40 # compute running times for S_100
	@cat data/sparse0200.pol data/sparse.int | ./evalR ar 20 # compute running times for S_200
	@cat data/sparse0400.pol data/sparse.int | ./evalR ar 10 # compute running times for S_400
	@cat data/sparse0800.pol data/sparse.int | ./evalR ar 5 # compute running times for S_800

############################### Eval using minimal Taylor form with quadratic convergence
evalT2min:   evalT2min.h evalT2min.cpp eval.h interval.h polynomial.h base.h
	make p=evalT2min

t-evalT2min: evalT2min
	./evalT2min < data/test1.txt	# read from file

t-evalT2min0: evalT2min
	./evalT2min 0			# do unit test

t-evalT2min1: evalT2min
	./evalT2min 1			# eval on some built-in polynomial

############################### Eval using Taylor form with quadratic convergence
evalT2:   evalT2.h evalT2.cpp eval.h interval.h polynomial.h base.h
	make p=evalT2

t-evalT2: evalT2
	./evalT2 < data/test1.txt	# read from file

t-evalT20: evalT2
	./evalT2 0			# do unit test

t-evalT21: evalT2
	./evalT2 1			# eval on some built-in polynomial

############################### Eval using Taylor form with cubic convergence
evalT3:   evalT3.h evalT3.cpp eval.h interval.h polynomial.h base.h
	make p=evalT3

t-evalT3: evalT3
	./evalT3 < data/test1.txt	# read from file

t-evalT30: evalT3
	./evalT3 0			# do unit test

t-evalT31: evalT3
	./evalT3 1			# eval on some built-in polynomial

############################### Eval using Taylor form with quartic convergence
evalT4:   evalT4.h evalT4.cpp eval.h interval.h polynomial.h base.h
	make p=evalT4

t-evalT4: evalT4
	./evalT4 < data/test1.txt	# read from file

t-evalT40: evalT4
	./evalT4 0			# do unit test

t-evalT41: evalT4
	./evalT4 1			# eval on some built-in polynomial

############################### Eval using recursive Lagrange form with cubic convergence
evalL3:   evalL3.h evalL3.cpp eval.h interval.h polynomial.h base.h
	make p=evalL3

t-evalL3: evalL3
	./evalL3 < data/test1.txt	# read from file

t-evalL30: evalL3
	./evalL3 0			# do unit test

t-evalL31: evalL3
	./evalL3 1			# eval on some built-in polynomial

############################### Eval using recursive Lagrange form with cubic convergence
###############################            and "cheap" estimate of the remainder
evalL3cheap:   evalL3cheap.h evalL3cheap.cpp eval.h interval.h polynomial.h base.h
	make p=evalL3cheap

t-evalL3cheap: evalL3cheap
	./evalL3cheap < data/test1.txt	# read from file

t-evalL3cheap0: evalL3cheap
	./evalL3cheap 0			# do unit test

t-evalL3cheap1: evalL3cheap
	./evalL3cheap 1			# eval on some built-in polynomial

############################### Eval using recursive Lagrange form with cubic convergence
evalL4:   evalL4.h evalL4.cpp eval.h interval.h polynomial.h base.h
	make p=evalL4

t-evalL4: evalL4
	./evalL4 < data/test1.txt	# read from file

t-evalL40: evalL4
	./evalL4 0			# do unit test

t-evalL41: evalL4
	./evalL4 1			# eval on some built-in polynomial

############################### Eval using recursive Lagrange form with cubic convergence
###############################            and "cheap" estimate of the remainder
evalL4cheap:   evalL4cheap.h evalL4cheap.cpp eval.h interval.h polynomial.h base.h
	make p=evalL4cheap

t-evalL4cheap: evalL4cheap
	./evalL4cheap < data/test1.txt	# read from file

t-evalL4cheap0: evalL4cheap
	./evalL4cheap 0			# do unit test

t-evalL4cheap1: evalL4cheap
	./evalL4cheap 1			# eval on some built-in polynomial

############################### Mignotte polynomials
mignotte: mignotte.cpp
	make p=mignotte

t-mignotte: evalT2 evalT3 evalT4 evalL3 evalL3cheap evalL4 evalL4cheap mignotte
	./mignotte 15 4 0.0 1.5 | ./evalT2
	./mignotte 15 4 0.0 1.5 | ./evalT3
	./mignotte 15 4 0.0 1.5 | ./evalT4
	./mignotte 15 4 0.0 1.5 | ./evalL3
	./mignotte 15 4 0.0 1.5 | ./evalL3cheap
	./mignotte 15 4 0.0 1.5 | ./evalL4
	./mignotte 15 4 0.0 1.5 | ./evalL3cheap

############################### all
all: eval evalT2 evalT3 evalT4 evalL3 evalL3cheap evalL4 evalL4cheap mignotte

############################### The interval class:
interval: interval.h interval.cpp
	make p=interval

t-interval: interval
	./interval

############################### The poly class:
polynomial: polynomial.h polynomial.cpp interval.h interval.cpp
	make p=polynomial

t-polynomial: polynomial
	./polynomial

############################### Testing
t test:	t.cpp
	make p=t

t-t: test
	./t

t1 test1:
	$(a.out) d1.txt ${def_verb}

###################################################
# Rules
###################################################
%: %.o
	# ${CXX} $(c++flags) $(LDFLAGS) $< $(CORE_LIB) -o $@
	gcc ${c++flags} ${lflags} $< -o ${p}${exe}

.cpp.o:
	# ${CXX} -c $(CXXFLAGS) $(c++flags) $(CORE_INC) $< -o $@
	gcc -c $< ${c++flags} ${lflags} -o $@

.cpp.${exe}:
	gcc $< ${c++flags} ${lflags} -o ${p}${exe}


# Phony targets
.PHONY: t test gcc run

###################################################
# Clean object/executable files
###################################################
clean:
	-@test -z "*.o" || rm -f *.o
	-@test -z "*~" || rm -f *~

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)
	-@test -z "$(MORE_EXEPROGS)" || rm -f $(MORE_EXEPROGS)

#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE))
MORE_EXEPROGS=$(MORE_TARGETS:=$(EXETYPE))

###################################################
# UTILITIES (misc)
###################################################

vi:
	gvim ${p}${ext} &

b:
	-@echo \" this \"

param par:
	@echo "Default values:"
	@echo "d = " ${d}
	@echo "p = " ${p}
	@echo "pp = " ${pp}
	@echo "ext = " ${ext}
	@echo "PATH = " $$PATH

m:
	gvim Makefile &

tar:
	cd ..; tar cvf evalProgs.tar \
	    hormann/Makefile \
	    hormann/README \
	    hormann/*.h \
	    hormann/*.cpp \
	    hormann/data \
	    hormann/frKai

###################################################
# END
###################################################
