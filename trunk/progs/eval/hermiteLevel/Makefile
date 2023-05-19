# Makefile for C++ programming

###################################################
# VARIABLES
###################################################
# 	THIS SECTION CONTAINS THE ONLY CUSTOMIZATION YOU NEED
# 	See the -include command at end of this section
###################################################

# TARGETS:
TARGETS=interval polynomial eval evalF evalR evalT2min evalT2 evalT3 evalT4 evalL3 evalL3cheap evalL4 evalL4cheap t all table1 table2 table3

# main file
p=eval

# TOP LEVEL CHOICE TASK: edit or compile
d=vi
d=cpp
d=gcc

# compiler flags
dbg=
c++flags= -std=c++11 -Ofast $(dbg)
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
################################################
###
default: $(d)

############################### To compile Kai's code
gcc:
	gcc ${p}.cpp ${c++flags} ${lflags} -o ${p}${exe}

gccd gcc-debug:
	gcc ${p}.cpp ${c++flags} -g ${lflags} -o ${p}${exe}

r run: ${p}
	${p}${exe} ${def_file}

############################### general Eval with choice
eval:   eval.h eval.cpp evalT2.h evalT3.h evalT4.h evalL3.h interval.h polynomial.h base.h
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
tab1: eval
	@cat data/chebyshev020.pol data/chebyshev.int | ./eval at # compute tree sizes for T_20
	@cat data/chebyshev040.pol data/chebyshev.int | ./eval at # compute tree sizes for T_40
	@cat data/chebyshev320.pol data/chebyshev.int | ./eval At # compute tree sizes for T_320
	@cat data/hermite020.pol data/hermite.int | ./eval at # compute tree sizes for H_20
	@cat data/hermite040.pol data/hermite.int | ./eval at # compute tree sizes for H_40

############################### generate all the tables
table: table1 table2 table3 table4 table5 
	@echo "All tables generated"

table1-wilk: eval 
	@cat data/wilk020.pol data/wilk.int | ./eval at # compute tree size for W_20
	@cat data/wilk040.pol data/wilk.int | ./eval at # compute tree size for W_40
	@cat data/wilk080.pol data/wilk.int | ./eval at # compute tree size for W_80
	@cat data/wilk160.pol data/wilk.int | ./eval At # compute tree size for W_160
table2-wilk: evalF
	@cat data/wilk020.pol data/wilk.int | ./evalF ar # compute tree size for W_20
	@cat data/wilk040.pol data/wilk.int | ./evalF ar # compute tree size for W_40
	@cat data/wilk080.pol data/wilk.int | ./evalF ar # compute tree size for W_80
	@cat data/wilk160.pol data/wilk.int | ./evalF Ar # compute tree size for W_160
table3-wilk: evalR
	@cat data/wilk020.pol data/wilk.int | ./evalR ar # compute tree size for W_20
	@cat data/wilk040.pol data/wilk.int | ./evalR ar # compute tree size for W_40
	@cat data/wilk080.pol data/wilk.int | ./evalR ar # compute tree size for W_80
	@cat data/wilk160.pol data/wilk.int | ./evalR Ar # compute tree size for W_160
tables-wilk: table1-wilk table2-wilk table3-wilk

table123: table1 table2 table3
	@echo "Tables 1, 2, and 3 generated"

############################### generate Table 1 with tree sizes
table1: eval
	@echo "Chebyshev"
	@cat data/chebyshev20.pol data/chebyshev.int | ./eval at # compute tree size for T_20
	@cat data/chebyshev40.pol data/chebyshev.int | ./eval at # compute tree size for T_40
	@cat data/chebyshev80.pol data/chebyshev.int | ./eval at # compute tree size for T_80
	@cat data/chebyshev160.pol data/chebyshev.int | ./eval at # compute tree size for T_160
	@cat data/chebyshev320.pol data/chebyshev.int | ./eval At # compute tree size for T_320
	@echo "Hermite" 
	@cat data/hermite20.pol data/hermite.int | ./eval at # compute tree size for H_20
	@cat data/hermite40.pol data/hermite.int | ./eval at # compute tree size for H_40
	@cat data/hermite80.pol data/hermite.int | ./eval at # compute tree size for H_80
	@cat data/hermite160.pol data/hermite.int | ./eval at # compute tree size for H_160
	@cat data/hermite320.pol data/hermite.int | ./eval At # compute tree size for H_320
	@echo "Mignotte"
	@cat data/mignotte21.pol data/mignotte.int | ./eval at # compute tree size for M_21
	@cat data/mignotte41.pol data/mignotte.int | ./eval at # compute tree size for M_41
	@cat data/mignotte81.pol data/mignotte.int | ./eval at # compute tree size for M_81
	@cat data/mignotte161.pol data/mignotte.int | ./eval At # compute tree size for M_161
	@echo "Wilkinsons"
	@cat data/wilk020.pol data/wilk.int | ./eval at # compute tree size for W_20
	@cat data/wilk040.pol data/wilk.int | ./eval at # compute tree size for W_40
	@cat data/wilk080.pol data/wilk.int | ./eval at # compute tree size for W_80
	@cat data/wilk160.pol data/wilk.int | ./eval At # compute tree size for W_160
	@echo "Sparse"
	@cat data/sparse0100.pol data/sparse.int | ./eval at # compute tree sizes for S_100
	@cat data/sparse0200.pol data/sparse.int | ./eval at # compute tree sizes for S_200
	@cat data/sparse0400.pol data/sparse.int | ./eval At # compute tree sizes for S_400

############################### generate Table 2 with running times for floating point arithmetic
tab2-more: evalF
	@cat data/chebyshev020.pol data/chebyshev.int | ./evalF ar 80 # compute running times for T_20
	@cat data/chebyshev040.pol data/chebyshev.int | ./evalF ar 40 # compute running times for T_40
	@cat data/chebyshev080.pol data/chebyshev.int | ./evalF Ar 20 # compute running times for T_80
	@cat data/hermite020.pol data/hermite.int | ./evalF ar 80 # compute running times for H_20
	@cat data/hermite040.pol data/hermite.int | ./evalF ar 40 # compute running times for H_40

############################### generate Table 2 with running times for floating point arithmetic
table2: evalF
	@echo "Chebyshev"
	@cat data/chebyshev20.pol data/chebyshev.int | ./evalF ar # compute tree size for T_20
	@cat data/chebyshev40.pol data/chebyshev.int | ./evalF ar # compute tree size for T_40
	@cat data/chebyshev80.pol data/chebyshev.int | ./evalF ar # compute tree size for T_80
	@cat data/chebyshev160.pol data/chebyshev.int | ./evalF ar # compute tree size for T_160
	@cat data/chebyshev320.pol data/chebyshev.int | ./evalF Ar # compute tree size for T_320
	@echo "Hermite" 
	@cat data/hermite20.pol data/hermite.int | ./evalF ar # compute tree size for H_20
	@cat data/hermite40.pol data/hermite.int | ./evalF ar # compute tree size for H_40
	@cat data/hermite80.pol data/hermite.int | ./evalF ar # compute tree size for H_80
	@cat data/hermite160.pol data/hermite.int | ./evalF ar # compute tree size for H_160
	@cat data/hermite320.pol data/hermite.int | ./evalF Ar # compute tree size for H_320
	@echo "Mignotte"
	@cat data/mignotte21.pol data/mignotte.int | ./evalF ar # compute tree size for M_21
	@cat data/mignotte41.pol data/mignotte.int | ./evalF ar # compute tree size for M_41
	@cat data/mignotte81.pol data/mignotte.int | ./evalF ar # compute tree size for M_81
	@cat data/mignotte161.pol data/mignotte.int | ./evalF Ar # compute tree size for M_161
	@echo "Wilkinsons"
	@cat data/wilk020.pol data/wilk.int | ./evalF ar # compute tree size for W_20
	@cat data/wilk040.pol data/wilk.int | ./evalF ar # compute tree size for W_40
	@cat data/wilk080.pol data/wilk.int | ./evalF ar # compute tree size for W_80
	@cat data/wilk160.pol data/wilk.int | ./evalF Ar # compute tree size for W_160
	@echo "Sparse"
	@cat data/sparse0100.pol data/sparse.int | ./evalF ar # compute tree sizes for S_100
	@cat data/sparse0200.pol data/sparse.int | ./evalF ar # compute tree sizes for S_200
	@cat data/sparse0400.pol data/sparse.int | ./evalF Ar # compute tree sizes for S_400

############################### generate Table 3 with running times for rational arithmetic
table3: evalR
	@echo "Chebyshev"
	@cat data/chebyshev20.pol data/chebyshev.int | ./evalR ar # compute tree size for T_20
	@cat data/chebyshev40.pol data/chebyshev.int | ./evalR ar # compute tree size for T_40
	@cat data/chebyshev80.pol data/chebyshev.int | ./evalR ar # compute tree size for T_80
	@cat data/chebyshev160.pol data/chebyshev.int | ./evalR ar # compute tree size for T_160
	@cat data/chebyshev320.pol data/chebyshev.int | ./evalR Ar # compute tree size for T_320
	@echo "Hermite" 
	@cat data/hermite20.pol data/hermite.int | ./evalR ar # compute tree size for H_20
	@cat data/hermite40.pol data/hermite.int | ./evalR ar # compute tree size for H_40
	@cat data/hermite80.pol data/hermite.int | ./evalR ar # compute tree size for H_80
	@cat data/hermite160.pol data/hermite.int | ./evalR ar # compute tree size for H_160
	@cat data/hermite320.pol data/hermite.int | ./evalR Ar # compute tree size for H_320
	@echo "Mignotte"
	@cat data/mignotte21.pol data/mignotte.int | ./evalR ar # compute tree size for M_21
	@cat data/mignotte41.pol data/mignotte.int | ./evalR ar # compute tree size for M_41
	@cat data/mignotte81.pol data/mignotte.int | ./evalR ar # compute tree size for M_81
	@cat data/mignotte161.pol data/mignotte.int | ./evalR Ar # compute tree size for M_161
	@echo "Wilkinsons"
	@cat data/wilk020.pol data/wilk.int | ./evalR ar # compute tree size for W_20
	@cat data/wilk040.pol data/wilk.int | ./evalR ar # compute tree size for W_40
	@cat data/wilk080.pol data/wilk.int | ./evalR ar # compute tree size for W_80
	@cat data/wilk160.pol data/wilk.int | ./evalR Ar # compute tree size for W_160
	@echo "Sparse"
	@cat data/sparse0100.pol data/sparse.int | ./evalR ar # compute tree sizes for S_100
	@cat data/sparse0200.pol data/sparse.int | ./evalR ar # compute tree sizes for S_200
	@cat data/sparse0400.pol data/sparse.int | ./evalR Ar # compute tree sizes for S_400

############################### generate data for this plot specification:
############################### Plot: x is degree of a family of polynomial, 
############################### y is ratio of speed between L3 and H22. 
############################### Find the point where H22 is faster than L3. 
fig1 figure1 table4: table4-chebyshev table4-hermite table4-mignotte table4-wilk
	@echo "Done"

table4-chebyshev: evalR 
	@cat data/chebyshev2.pol data/chebyshev.int | ./evalR n 80 # compute running times for T_2
	@cat data/chebyshev4.pol data/chebyshev.int | ./evalR n 80 # compute running times for T_4
	@cat data/chebyshev6.pol data/chebyshev.int | ./evalR n 80 # compute running times for T_6
	@cat data/chebyshev8.pol data/chebyshev.int | ./evalR n 80 # compute running times for T_8
	@cat data/chebyshev10.pol data/chebyshev.int | ./evalR n 80 # compute running times for T_10
	@cat data/chebyshev12.pol data/chebyshev.int | ./evalR n 80 # compute running times for T_12
	@cat data/chebyshev14.pol data/chebyshev.int | ./evalR n 80 # compute running times for T_14
	@cat data/chebyshev16.pol data/chebyshev.int | ./evalR n 80 # compute running times for T_16
	@cat data/chebyshev18.pol data/chebyshev.int | ./evalR n 80 # compute running times for T_18
	@cat data/chebyshev20.pol data/chebyshev.int | ./evalR n 80 # compute running times for T_20
	@cat data/chebyshev22.pol data/chebyshev.int | ./evalR n 40 # compute running times for T_22
	@cat data/chebyshev24.pol data/chebyshev.int | ./evalR n 40 # compute running times for T_24
	@cat data/chebyshev26.pol data/chebyshev.int | ./evalR n 40 # compute running times for T_26
	@cat data/chebyshev28.pol data/chebyshev.int | ./evalR n 40 # compute running times for T_28
	@cat data/chebyshev30.pol data/chebyshev.int | ./evalR n 40 # compute running times for T_30
	@cat data/chebyshev32.pol data/chebyshev.int | ./evalR n 40 # compute running times for T_32
	@cat data/chebyshev34.pol data/chebyshev.int | ./evalR n 40 # compute running times for T_34
	@cat data/chebyshev36.pol data/chebyshev.int | ./evalR n 40 # compute running times for T_36
	@cat data/chebyshev38.pol data/chebyshev.int | ./evalR n 40 # compute running times for T_38
	@cat data/chebyshev40.pol data/chebyshev.int | ./evalR n 40 # compute running times for T_40
	@cat data/chebyshev42.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_42
	@cat data/chebyshev44.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_44
	@cat data/chebyshev46.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_46
	@cat data/chebyshev48.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_48
	@cat data/chebyshev50.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_50
	@cat data/chebyshev52.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_52
	@cat data/chebyshev54.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_54
	@cat data/chebyshev56.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_56
	@cat data/chebyshev58.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_58
	@cat data/chebyshev60.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_60
	@cat data/chebyshev62.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_62
	@cat data/chebyshev64.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_64
	@cat data/chebyshev66.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_66
	@cat data/chebyshev68.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_68
	@cat data/chebyshev70.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_70
	@cat data/chebyshev72.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_72
	@cat data/chebyshev74.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_74
	@cat data/chebyshev76.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_76
	@cat data/chebyshev78.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_78
	@cat data/chebyshev80.pol data/chebyshev.int | ./evalR n 20 # compute running times for T_80
	@cat data/chebyshev82.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_82
	@cat data/chebyshev84.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_84
	@cat data/chebyshev86.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_86
	@cat data/chebyshev88.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_88
	@cat data/chebyshev90.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_90
	@cat data/chebyshev92.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_92
	@cat data/chebyshev94.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_94
	@cat data/chebyshev96.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_96
	@cat data/chebyshev98.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_98
	@cat data/chebyshev100.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_100
	@cat data/chebyshev102.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_102
	@cat data/chebyshev104.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_104
	@cat data/chebyshev106.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_106
	@cat data/chebyshev108.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_108
	@cat data/chebyshev110.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_110
	@cat data/chebyshev112.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_112
	@cat data/chebyshev114.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_114
	@cat data/chebyshev116.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_116
	@cat data/chebyshev118.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_118
	@cat data/chebyshev120.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_120
	@cat data/chebyshev122.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_122
	@cat data/chebyshev124.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_124
	@cat data/chebyshev126.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_126
	@cat data/chebyshev128.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_128
	@cat data/chebyshev130.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_130
	@cat data/chebyshev132.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_132
	@cat data/chebyshev134.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_134
	@cat data/chebyshev136.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_136
	@cat data/chebyshev138.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_138
	@cat data/chebyshev140.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_140
	@cat data/chebyshev142.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_142
	@cat data/chebyshev144.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_144
	@cat data/chebyshev146.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_146
	@cat data/chebyshev148.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_148
	@cat data/chebyshev150.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_150
	@cat data/chebyshev152.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_152
	@cat data/chebyshev154.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_154
	@cat data/chebyshev156.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_156
	@cat data/chebyshev158.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_158
	@cat data/chebyshev160.pol data/chebyshev.int | ./evalR n 10 # compute running times for T_160
	@cat data/chebyshev162.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_162
	@cat data/chebyshev164.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_164
	@cat data/chebyshev166.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_166
	@cat data/chebyshev168.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_168
	@cat data/chebyshev170.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_170
	@cat data/chebyshev172.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_172
	@cat data/chebyshev174.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_174
	@cat data/chebyshev176.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_176
	@cat data/chebyshev178.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_178
	@cat data/chebyshev180.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_180
	@cat data/chebyshev182.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_182
	@cat data/chebyshev184.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_184
	@cat data/chebyshev186.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_186
	@cat data/chebyshev188.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_188
	@cat data/chebyshev190.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_190
	@cat data/chebyshev192.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_192
	@cat data/chebyshev194.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_194
	@cat data/chebyshev196.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_196
	@cat data/chebyshev198.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_198
	@cat data/chebyshev200.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_200
	@cat data/chebyshev202.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_202
	@cat data/chebyshev204.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_204
	@cat data/chebyshev206.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_206
	@cat data/chebyshev208.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_208
	@cat data/chebyshev210.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_210
	@cat data/chebyshev212.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_212
	@cat data/chebyshev214.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_214
	@cat data/chebyshev216.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_216
	@cat data/chebyshev218.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_218
	@cat data/chebyshev220.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_220
	@cat data/chebyshev222.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_222
	@cat data/chebyshev224.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_224
	@cat data/chebyshev226.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_226
	@cat data/chebyshev228.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_228
	@cat data/chebyshev230.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_230
	@cat data/chebyshev232.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_232
	@cat data/chebyshev234.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_234
	@cat data/chebyshev236.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_236
	@cat data/chebyshev238.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_238
	@cat data/chebyshev240.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_240
	@cat data/chebyshev242.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_242
	@cat data/chebyshev244.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_244
	@cat data/chebyshev246.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_246
	@cat data/chebyshev248.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_248
	@cat data/chebyshev250.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_250
	@cat data/chebyshev252.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_252
	@cat data/chebyshev254.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_254
	@cat data/chebyshev256.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_256
	@cat data/chebyshev258.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_258
	@cat data/chebyshev260.pol data/chebyshev.int | ./evalR n 4 # compute running times for T_260

table4-hermite: evalR 
	@cat data/hermite2.pol data/hermite.int | ./evalR n 60 # compute running times for H_2
	@cat data/hermite4.pol data/hermite.int | ./evalR n 60 # compute running times for H_4
	@cat data/hermite6.pol data/hermite.int | ./evalR n 60 # compute running times for H_6
	@cat data/hermite8.pol data/hermite.int | ./evalR n 60 # compute running times for H_8
	@cat data/hermite10.pol data/hermite.int | ./evalR n 60 # compute running times for H_10
	@cat data/hermite12.pol data/hermite.int | ./evalR n 60 # compute running times for H_12
	@cat data/hermite14.pol data/hermite.int | ./evalR n 60 # compute running times for H_14
	@cat data/hermite16.pol data/hermite.int | ./evalR n 60 # compute running times for H_16
	@cat data/hermite18.pol data/hermite.int | ./evalR n 60 # compute running times for H_18
	@cat data/hermite20.pol data/hermite.int | ./evalR n 60 # compute running times for H_20
	@cat data/hermite22.pol data/hermite.int | ./evalR n 30 # compute running times for H_22
	@cat data/hermite24.pol data/hermite.int | ./evalR n 30 # compute running times for H_24
	@cat data/hermite26.pol data/hermite.int | ./evalR n 30 # compute running times for H_26
	@cat data/hermite28.pol data/hermite.int | ./evalR n 30 # compute running times for H_28
	@cat data/hermite30.pol data/hermite.int | ./evalR n 30 # compute running times for H_30
	@cat data/hermite32.pol data/hermite.int | ./evalR n 30 # compute running times for H_32
	@cat data/hermite34.pol data/hermite.int | ./evalR n 30 # compute running times for H_34
	@cat data/hermite36.pol data/hermite.int | ./evalR n 30 # compute running times for H_36
	@cat data/hermite38.pol data/hermite.int | ./evalR n 30 # compute running times for H_38
	@cat data/hermite40.pol data/hermite.int | ./evalR n 30 # compute running times for H_40
	@cat data/hermite42.pol data/hermite.int | ./evalR n 20 # compute running times for H_42
	@cat data/hermite44.pol data/hermite.int | ./evalR n 20 # compute running times for H_44
	@cat data/hermite46.pol data/hermite.int | ./evalR n 20 # compute running times for H_46
	@cat data/hermite48.pol data/hermite.int | ./evalR n 20 # compute running times for H_48
	@cat data/hermite50.pol data/hermite.int | ./evalR n 20 # compute running times for H_50
	@cat data/hermite52.pol data/hermite.int | ./evalR n 20 # compute running times for H_52
	@cat data/hermite54.pol data/hermite.int | ./evalR n 20 # compute running times for H_54
	@cat data/hermite56.pol data/hermite.int | ./evalR n 20 # compute running times for H_56
	@cat data/hermite58.pol data/hermite.int | ./evalR n 20 # compute running times for H_58
	@cat data/hermite60.pol data/hermite.int | ./evalR n 20 # compute running times for H_60
	@cat data/hermite62.pol data/hermite.int | ./evalR n 20 # compute running times for H_62
	@cat data/hermite64.pol data/hermite.int | ./evalR n 20 # compute running times for H_64
	@cat data/hermite66.pol data/hermite.int | ./evalR n 20 # compute running times for H_66
	@cat data/hermite68.pol data/hermite.int | ./evalR n 20 # compute running times for H_68
	@cat data/hermite70.pol data/hermite.int | ./evalR n 20 # compute running times for H_70
	@cat data/hermite72.pol data/hermite.int | ./evalR n 20 # compute running times for H_72
	@cat data/hermite74.pol data/hermite.int | ./evalR n 20 # compute running times for H_74
	@cat data/hermite76.pol data/hermite.int | ./evalR n 20 # compute running times for H_76
	@cat data/hermite78.pol data/hermite.int | ./evalR n 20 # compute running times for H_78
	@cat data/hermite80.pol data/hermite.int | ./evalR n 20 # compute running times for H_80
	@cat data/hermite82.pol data/hermite.int | ./evalR n 10 # compute running times for H_82
	@cat data/hermite84.pol data/hermite.int | ./evalR n 10 # compute running times for H_84
	@cat data/hermite86.pol data/hermite.int | ./evalR n 10 # compute running times for H_86
	@cat data/hermite88.pol data/hermite.int | ./evalR n 10 # compute running times for H_88
	@cat data/hermite90.pol data/hermite.int | ./evalR n 10 # compute running times for H_90
	@cat data/hermite92.pol data/hermite.int | ./evalR n 10 # compute running times for H_92
	@cat data/hermite94.pol data/hermite.int | ./evalR n 10 # compute running times for H_94
	@cat data/hermite96.pol data/hermite.int | ./evalR n 10 # compute running times for H_96
	@cat data/hermite98.pol data/hermite.int | ./evalR n 10 # compute running times for H_98
	@cat data/hermite100.pol data/hermite.int | ./evalR n 10 # compute running times for H_100
	@cat data/hermite102.pol data/hermite.int | ./evalR n 10 # compute running times for H_102
	@cat data/hermite104.pol data/hermite.int | ./evalR n 10 # compute running times for H_104
	@cat data/hermite106.pol data/hermite.int | ./evalR n 10 # compute running times for H_106
	@cat data/hermite108.pol data/hermite.int | ./evalR n 10 # compute running times for H_108
	@cat data/hermite110.pol data/hermite.int | ./evalR n 10 # compute running times for H_110
	@cat data/hermite112.pol data/hermite.int | ./evalR n 10 # compute running times for H_112
	@cat data/hermite114.pol data/hermite.int | ./evalR n 10 # compute running times for H_114
	@cat data/hermite116.pol data/hermite.int | ./evalR n 10 # compute running times for H_116
	@cat data/hermite118.pol data/hermite.int | ./evalR n 10 # compute running times for H_118
	@cat data/hermite120.pol data/hermite.int | ./evalR n 10 # compute running times for H_120
	@cat data/hermite122.pol data/hermite.int | ./evalR n 10 # compute running times for H_122
	@cat data/hermite124.pol data/hermite.int | ./evalR n 10 # compute running times for H_124
	@cat data/hermite126.pol data/hermite.int | ./evalR n 10 # compute running times for H_126
	@cat data/hermite128.pol data/hermite.int | ./evalR n 10 # compute running times for H_128
	@cat data/hermite130.pol data/hermite.int | ./evalR n 10 # compute running times for H_130
	@cat data/hermite132.pol data/hermite.int | ./evalR n 10 # compute running times for H_132
	@cat data/hermite134.pol data/hermite.int | ./evalR n 10 # compute running times for H_134
	@cat data/hermite136.pol data/hermite.int | ./evalR n 10 # compute running times for H_136
	@cat data/hermite138.pol data/hermite.int | ./evalR n 10 # compute running times for H_138
	@cat data/hermite140.pol data/hermite.int | ./evalR n 10 # compute running times for H_140
	@cat data/hermite142.pol data/hermite.int | ./evalR n 10 # compute running times for H_142
	@cat data/hermite144.pol data/hermite.int | ./evalR n 10 # compute running times for H_144
	@cat data/hermite146.pol data/hermite.int | ./evalR n 10 # compute running times for H_146
	@cat data/hermite148.pol data/hermite.int | ./evalR n 10 # compute running times for H_148
	@cat data/hermite150.pol data/hermite.int | ./evalR n 10 # compute running times for H_150
	@cat data/hermite152.pol data/hermite.int | ./evalR n 10 # compute running times for H_152
	@cat data/hermite154.pol data/hermite.int | ./evalR n 10 # compute running times for H_154
	@cat data/hermite156.pol data/hermite.int | ./evalR n 10 # compute running times for H_156
	@cat data/hermite158.pol data/hermite.int | ./evalR n 10 # compute running times for H_158
	@cat data/hermite160.pol data/hermite.int | ./evalR n 10 # compute running times for H_160
	@cat data/hermite162.pol data/hermite.int | ./evalR n 4 # compute running times for H_162
	@cat data/hermite164.pol data/hermite.int | ./evalR n 4 # compute running times for H_164
	@cat data/hermite166.pol data/hermite.int | ./evalR n 4 # compute running times for H_166
	@cat data/hermite168.pol data/hermite.int | ./evalR n 4 # compute running times for H_168
	@cat data/hermite170.pol data/hermite.int | ./evalR n 4 # compute running times for H_170
	@cat data/hermite172.pol data/hermite.int | ./evalR n 4 # compute running times for H_172
	@cat data/hermite174.pol data/hermite.int | ./evalR n 4 # compute running times for H_174
	@cat data/hermite176.pol data/hermite.int | ./evalR n 4 # compute running times for H_176
	@cat data/hermite178.pol data/hermite.int | ./evalR n 4 # compute running times for H_178
	@cat data/hermite180.pol data/hermite.int | ./evalR n 4 # compute running times for H_180
	@cat data/hermite182.pol data/hermite.int | ./evalR n 4 # compute running times for H_182
	@cat data/hermite184.pol data/hermite.int | ./evalR n 4 # compute running times for H_184
	@cat data/hermite186.pol data/hermite.int | ./evalR n 4 # compute running times for H_186
	@cat data/hermite188.pol data/hermite.int | ./evalR n 4 # compute running times for H_188
	@cat data/hermite190.pol data/hermite.int | ./evalR n 4 # compute running times for H_190
	@cat data/hermite192.pol data/hermite.int | ./evalR n 4 # compute running times for H_192
	@cat data/hermite194.pol data/hermite.int | ./evalR n 4 # compute running times for H_194
	@cat data/hermite196.pol data/hermite.int | ./evalR n 4 # compute running times for H_196
	@cat data/hermite198.pol data/hermite.int | ./evalR n 4 # compute running times for H_198
	@cat data/hermite200.pol data/hermite.int | ./evalR n 4 # compute running times for H_200
	@cat data/hermite202.pol data/hermite.int | ./evalR n 4 # compute running times for H_202
	@cat data/hermite204.pol data/hermite.int | ./evalR n 4 # compute running times for H_204
	@cat data/hermite206.pol data/hermite.int | ./evalR n 4 # compute running times for H_206
	@cat data/hermite208.pol data/hermite.int | ./evalR n 4 # compute running times for H_208
	@cat data/hermite210.pol data/hermite.int | ./evalR n 4 # compute running times for H_210
	@cat data/hermite212.pol data/hermite.int | ./evalR n 4 # compute running times for H_212
	@cat data/hermite214.pol data/hermite.int | ./evalR n 4 # compute running times for H_214
	@cat data/hermite216.pol data/hermite.int | ./evalR n 4 # compute running times for H_216
	@cat data/hermite218.pol data/hermite.int | ./evalR n 4 # compute running times for H_218
	@cat data/hermite220.pol data/hermite.int | ./evalR n 4 # compute running times for H_220
	@cat data/hermite222.pol data/hermite.int | ./evalR n 4 # compute running times for H_222
	@cat data/hermite224.pol data/hermite.int | ./evalR n 4 # compute running times for H_224
	@cat data/hermite226.pol data/hermite.int | ./evalR n 4 # compute running times for H_226
	@cat data/hermite228.pol data/hermite.int | ./evalR n 4 # compute running times for H_228
	@cat data/hermite230.pol data/hermite.int | ./evalR n 4 # compute running times for H_230
	@cat data/hermite232.pol data/hermite.int | ./evalR n 4 # compute running times for H_232
	@cat data/hermite234.pol data/hermite.int | ./evalR n 4 # compute running times for H_234
	@cat data/hermite236.pol data/hermite.int | ./evalR n 4 # compute running times for H_236
	@cat data/hermite238.pol data/hermite.int | ./evalR n 4 # compute running times for H_238
	@cat data/hermite240.pol data/hermite.int | ./evalR n 4 # compute running times for H_240
	@cat data/hermite242.pol data/hermite.int | ./evalR n 4 # compute running times for H_242
	@cat data/hermite244.pol data/hermite.int | ./evalR n 4 # compute running times for H_244
	@cat data/hermite246.pol data/hermite.int | ./evalR n 4 # compute running times for H_246
	@cat data/hermite248.pol data/hermite.int | ./evalR n 4 # compute running times for H_248
	@cat data/hermite250.pol data/hermite.int | ./evalR n 4 # compute running times for H_250
	@cat data/hermite252.pol data/hermite.int | ./evalR n 4 # compute running times for H_252
	@cat data/hermite254.pol data/hermite.int | ./evalR n 4 # compute running times for H_254
	@cat data/hermite256.pol data/hermite.int | ./evalR n 4 # compute running times for H_256
	@cat data/hermite258.pol data/hermite.int | ./evalR n 4 # compute running times for H_258
	@cat data/hermite260.pol data/hermite.int | ./evalR n 4 # compute running times for H_260

table4-mignotte: evalR
	@cat data/mignotte3.pol data/mignotte.int | ./evalR n 60 # compute running times for M_3
	@cat data/mignotte5.pol data/mignotte.int | ./evalR n 60 # compute running times for M_5
	@cat data/mignotte7.pol data/mignotte.int | ./evalR n 60 # compute running times for M_7
	@cat data/mignotte9.pol data/mignotte.int | ./evalR n 60 # compute running times for M_9
	@cat data/mignotte11.pol data/mignotte.int | ./evalR n 60 # compute running times for M_11
	@cat data/mignotte13.pol data/mignotte.int | ./evalR n 60 # compute running times for M_13
	@cat data/mignotte15.pol data/mignotte.int | ./evalR n 60 # compute running times for M_15
	@cat data/mignotte17.pol data/mignotte.int | ./evalR n 60 # compute running times for M_17
	@cat data/mignotte19.pol data/mignotte.int | ./evalR n 60 # compute running times for M_19
	@cat data/mignotte21.pol data/mignotte.int | ./evalR n 60 # compute running times for M_21
	@cat data/mignotte23.pol data/mignotte.int | ./evalR n 30 # compute running times for M_23
	@cat data/mignotte25.pol data/mignotte.int | ./evalR n 30 # compute running times for M_25
	@cat data/mignotte27.pol data/mignotte.int | ./evalR n 30 # compute running times for M_27
	@cat data/mignotte29.pol data/mignotte.int | ./evalR n 30 # compute running times for M_29
	@cat data/mignotte31.pol data/mignotte.int | ./evalR n 30 # compute running times for M_31
	@cat data/mignotte33.pol data/mignotte.int | ./evalR n 30 # compute running times for M_33
	@cat data/mignotte35.pol data/mignotte.int | ./evalR n 30 # compute running times for M_35
	@cat data/mignotte37.pol data/mignotte.int | ./evalR n 30 # compute running times for M_37
	@cat data/mignotte39.pol data/mignotte.int | ./evalR n 30 # compute running times for M_39
	@cat data/mignotte41.pol data/mignotte.int | ./evalR n 30 # compute running times for M_41
	@cat data/mignotte43.pol data/mignotte.int | ./evalR n 20 # compute running times for M_43
	@cat data/mignotte45.pol data/mignotte.int | ./evalR n 20 # compute running times for M_45
	@cat data/mignotte47.pol data/mignotte.int | ./evalR n 20 # compute running times for M_47
	@cat data/mignotte49.pol data/mignotte.int | ./evalR n 20 # compute running times for M_49
	@cat data/mignotte51.pol data/mignotte.int | ./evalR n 20 # compute running times for M_51
	@cat data/mignotte53.pol data/mignotte.int | ./evalR n 20 # compute running times for M_53
	@cat data/mignotte55.pol data/mignotte.int | ./evalR n 20 # compute running times for M_55
	@cat data/mignotte57.pol data/mignotte.int | ./evalR n 20 # compute running times for M_57
	@cat data/mignotte59.pol data/mignotte.int | ./evalR n 20 # compute running times for M_59
	@cat data/mignotte61.pol data/mignotte.int | ./evalR n 20 # compute running times for M_61
	@cat data/mignotte63.pol data/mignotte.int | ./evalR n 20 # compute running times for M_63
	@cat data/mignotte65.pol data/mignotte.int | ./evalR n 20 # compute running times for M_65
	@cat data/mignotte67.pol data/mignotte.int | ./evalR n 20 # compute running times for M_67
	@cat data/mignotte69.pol data/mignotte.int | ./evalR n 20 # compute running times for M_69
	@cat data/mignotte71.pol data/mignotte.int | ./evalR n 20 # compute running times for M_71
	@cat data/mignotte73.pol data/mignotte.int | ./evalR n 20 # compute running times for M_73
	@cat data/mignotte75.pol data/mignotte.int | ./evalR n 20 # compute running times for M_75
	@cat data/mignotte77.pol data/mignotte.int | ./evalR n 20 # compute running times for M_77
	@cat data/mignotte79.pol data/mignotte.int | ./evalR n 20 # compute running times for M_79
	@cat data/mignotte81.pol data/mignotte.int | ./evalR n 20 # compute running times for M_81
	@cat data/mignotte83.pol data/mignotte.int | ./evalR n 10 # compute running times for M_83
	@cat data/mignotte85.pol data/mignotte.int | ./evalR n 10 # compute running times for M_85
	@cat data/mignotte87.pol data/mignotte.int | ./evalR n 10 # compute running times for M_87
	@cat data/mignotte89.pol data/mignotte.int | ./evalR n 10 # compute running times for M_89
	@cat data/mignotte91.pol data/mignotte.int | ./evalR n 10 # compute running times for M_91
	@cat data/mignotte93.pol data/mignotte.int | ./evalR n 10 # compute running times for M_93
	@cat data/mignotte95.pol data/mignotte.int | ./evalR n 10 # compute running times for M_95
	@cat data/mignotte97.pol data/mignotte.int | ./evalR n 10 # compute running times for M_97
	@cat data/mignotte99.pol data/mignotte.int | ./evalR n 10 # compute running times for M_99
	@cat data/mignotte101.pol data/mignotte.int | ./evalR n 10 # compute running times for M_101
	@cat data/mignotte103.pol data/mignotte.int | ./evalR n 10 # compute running times for M_103
	@cat data/mignotte105.pol data/mignotte.int | ./evalR n 10 # compute running times for M_105
	@cat data/mignotte107.pol data/mignotte.int | ./evalR n 10 # compute running times for M_107
	@cat data/mignotte109.pol data/mignotte.int | ./evalR n 10 # compute running times for M_109
	@cat data/mignotte111.pol data/mignotte.int | ./evalR n 10 # compute running times for M_111
	@cat data/mignotte113.pol data/mignotte.int | ./evalR n 10 # compute running times for M_113
	@cat data/mignotte115.pol data/mignotte.int | ./evalR n 10 # compute running times for M_115
	@cat data/mignotte117.pol data/mignotte.int | ./evalR n 10 # compute running times for M_117
	@cat data/mignotte119.pol data/mignotte.int | ./evalR n 10 # compute running times for M_119
	@cat data/mignotte121.pol data/mignotte.int | ./evalR n 10 # compute running times for M_121
	@cat data/mignotte123.pol data/mignotte.int | ./evalR n 10 # compute running times for M_123
	@cat data/mignotte125.pol data/mignotte.int | ./evalR n 10 # compute running times for M_125
	@cat data/mignotte127.pol data/mignotte.int | ./evalR n 10 # compute running times for M_127
	@cat data/mignotte129.pol data/mignotte.int | ./evalR n 10 # compute running times for M_129
	@cat data/mignotte131.pol data/mignotte.int | ./evalR n 10 # compute running times for M_131
	@cat data/mignotte133.pol data/mignotte.int | ./evalR n 10 # compute running times for M_133
	@cat data/mignotte135.pol data/mignotte.int | ./evalR n 10 # compute running times for M_135
	@cat data/mignotte137.pol data/mignotte.int | ./evalR n 10 # compute running times for M_137
	@cat data/mignotte139.pol data/mignotte.int | ./evalR n 10 # compute running times for M_139
	@cat data/mignotte141.pol data/mignotte.int | ./evalR n 10 # compute running times for M_141
	@cat data/mignotte143.pol data/mignotte.int | ./evalR n 10 # compute running times for M_143
	@cat data/mignotte145.pol data/mignotte.int | ./evalR n 10 # compute running times for M_145
	@cat data/mignotte147.pol data/mignotte.int | ./evalR n 10 # compute running times for M_147
	@cat data/mignotte149.pol data/mignotte.int | ./evalR n 10 # compute running times for M_149
	@cat data/mignotte151.pol data/mignotte.int | ./evalR n 10 # compute running times for M_151
	@cat data/mignotte153.pol data/mignotte.int | ./evalR n 10 # compute running times for M_153
	@cat data/mignotte155.pol data/mignotte.int | ./evalR n 10 # compute running times for M_155
	@cat data/mignotte157.pol data/mignotte.int | ./evalR n 10 # compute running times for M_157
	@cat data/mignotte159.pol data/mignotte.int | ./evalR n 10 # compute running times for M_159
	@cat data/mignotte161.pol data/mignotte.int | ./evalR n 10 # compute running times for M_161
	@cat data/mignotte163.pol data/mignotte.int | ./evalR n 4 # compute running times for M_163
	@cat data/mignotte165.pol data/mignotte.int | ./evalR n 4 # compute running times for M_165
	@cat data/mignotte167.pol data/mignotte.int | ./evalR n 4 # compute running times for M_167
	@cat data/mignotte169.pol data/mignotte.int | ./evalR n 4 # compute running times for M_169
	@cat data/mignotte171.pol data/mignotte.int | ./evalR n 4 # compute running times for M_171
	@cat data/mignotte173.pol data/mignotte.int | ./evalR n 4 # compute running times for M_173
	@cat data/mignotte175.pol data/mignotte.int | ./evalR n 4 # compute running times for M_175
	@cat data/mignotte177.pol data/mignotte.int | ./evalR n 4 # compute running times for M_177
	@cat data/mignotte179.pol data/mignotte.int | ./evalR n 4 # compute running times for M_179
	@cat data/mignotte181.pol data/mignotte.int | ./evalR n 4 # compute running times for M_181
	@cat data/mignotte183.pol data/mignotte.int | ./evalR n 4 # compute running times for M_183
	@cat data/mignotte185.pol data/mignotte.int | ./evalR n 4 # compute running times for M_185
	@cat data/mignotte187.pol data/mignotte.int | ./evalR n 4 # compute running times for M_187
	@cat data/mignotte189.pol data/mignotte.int | ./evalR n 4 # compute running times for M_189
	@cat data/mignotte191.pol data/mignotte.int | ./evalR n 4 # compute running times for M_191
	@cat data/mignotte193.pol data/mignotte.int | ./evalR n 4 # compute running times for M_193
	@cat data/mignotte195.pol data/mignotte.int | ./evalR n 4 # compute running times for M_195
	@cat data/mignotte197.pol data/mignotte.int | ./evalR n 4 # compute running times for M_197
	@cat data/mignotte199.pol data/mignotte.int | ./evalR n 4 # compute running times for M_199
	@cat data/mignotte201.pol data/mignotte.int | ./evalR n 4 # compute running times for M_201
	@cat data/mignotte203.pol data/mignotte.int | ./evalR n 4 # compute running times for M_203
	@cat data/mignotte205.pol data/mignotte.int | ./evalR n 4 # compute running times for M_205
	@cat data/mignotte207.pol data/mignotte.int | ./evalR n 4 # compute running times for M_207
	@cat data/mignotte209.pol data/mignotte.int | ./evalR n 4 # compute running times for M_209
	@cat data/mignotte211.pol data/mignotte.int | ./evalR n 4 # compute running times for M_211
	@cat data/mignotte213.pol data/mignotte.int | ./evalR n 4 # compute running times for M_213
	@cat data/mignotte215.pol data/mignotte.int | ./evalR n 4 # compute running times for M_215
	@cat data/mignotte217.pol data/mignotte.int | ./evalR n 4 # compute running times for M_217
	@cat data/mignotte219.pol data/mignotte.int | ./evalR n 4 # compute running times for M_219
	@cat data/mignotte221.pol data/mignotte.int | ./evalR n 4 # compute running times for M_221
	@cat data/mignotte223.pol data/mignotte.int | ./evalR n 4 # compute running times for M_223
	@cat data/mignotte225.pol data/mignotte.int | ./evalR n 4 # compute running times for M_225
	@cat data/mignotte227.pol data/mignotte.int | ./evalR n 4 # compute running times for M_227
	@cat data/mignotte229.pol data/mignotte.int | ./evalR n 4 # compute running times for M_229
	@cat data/mignotte231.pol data/mignotte.int | ./evalR n 4 # compute running times for M_231
	@cat data/mignotte233.pol data/mignotte.int | ./evalR n 4 # compute running times for M_233
	@cat data/mignotte235.pol data/mignotte.int | ./evalR n 4 # compute running times for M_235
	@cat data/mignotte237.pol data/mignotte.int | ./evalR n 4 # compute running times for M_237
	@cat data/mignotte239.pol data/mignotte.int | ./evalR n 4 # compute running times for M_239
	@cat data/mignotte241.pol data/mignotte.int | ./evalR n 4 # compute running times for M_241
	@cat data/mignotte243.pol data/mignotte.int | ./evalR n 4 # compute running times for M_243
	@cat data/mignotte245.pol data/mignotte.int | ./evalR n 4 # compute running times for M_245
	@cat data/mignotte247.pol data/mignotte.int | ./evalR n 4 # compute running times for M_247
	@cat data/mignotte249.pol data/mignotte.int | ./evalR n 4 # compute running times for M_249
	@cat data/mignotte251.pol data/mignotte.int | ./evalR n 4 # compute running times for M_251
	@cat data/mignotte253.pol data/mignotte.int | ./evalR n 4 # compute running times for M_253
	@cat data/mignotte255.pol data/mignotte.int | ./evalR n 4 # compute running times for M_255
	@cat data/mignotte257.pol data/mignotte.int | ./evalR n 4 # compute running times for M_257
	@cat data/mignotte259.pol data/mignotte.int | ./evalR n 4 # compute running times for M_259
	@cat data/mignotte261.pol data/mignotte.int | ./evalR n 4 # compute running times for M_261

table4-wilk: evalR 
	@cat data/wilk002.pol data/wilk.int | ./evalR n 60 # compute running times for W_2
	@cat data/wilk004.pol data/wilk.int | ./evalR n 60 # compute running times for W_4
	@cat data/wilk006.pol data/wilk.int | ./evalR n 60 # compute running times for W_6
	@cat data/wilk008.pol data/wilk.int | ./evalR n 60 # compute running times for W_8
	@cat data/wilk010.pol data/wilk.int | ./evalR n 60 # compute running times for W_10
	@cat data/wilk012.pol data/wilk.int | ./evalR n 60 # compute running times for W_12
	@cat data/wilk014.pol data/wilk.int | ./evalR n 60 # compute running times for W_14
	@cat data/wilk016.pol data/wilk.int | ./evalR n 60 # compute running times for W_16
	@cat data/wilk018.pol data/wilk.int | ./evalR n 60 # compute running times for W_18
	@cat data/wilk020.pol data/wilk.int | ./evalR n 60 # compute running times for W_20
	@cat data/wilk022.pol data/wilk.int | ./evalR n 30 # compute running times for W_22
	@cat data/wilk024.pol data/wilk.int | ./evalR n 30 # compute running times for W_24
	@cat data/wilk026.pol data/wilk.int | ./evalR n 30 # compute running times for W_26
	@cat data/wilk028.pol data/wilk.int | ./evalR n 30 # compute running times for W_28
	@cat data/wilk030.pol data/wilk.int | ./evalR n 30 # compute running times for W_30
	@cat data/wilk032.pol data/wilk.int | ./evalR n 30 # compute running times for W_32
	@cat data/wilk034.pol data/wilk.int | ./evalR n 30 # compute running times for W_34
	@cat data/wilk036.pol data/wilk.int | ./evalR n 30 # compute running times for W_36
	@cat data/wilk038.pol data/wilk.int | ./evalR n 30 # compute running times for W_38
	@cat data/wilk040.pol data/wilk.int | ./evalR n 30 # compute running times for W_40
	@cat data/wilk042.pol data/wilk.int | ./evalR n 20 # compute running times for W_42
	@cat data/wilk044.pol data/wilk.int | ./evalR n 20 # compute running times for W_44
	@cat data/wilk046.pol data/wilk.int | ./evalR n 20 # compute running times for W_46
	@cat data/wilk048.pol data/wilk.int | ./evalR n 20 # compute running times for W_48
	@cat data/wilk050.pol data/wilk.int | ./evalR n 20 # compute running times for W_50
	@cat data/wilk052.pol data/wilk.int | ./evalR n 20 # compute running times for W_52
	@cat data/wilk054.pol data/wilk.int | ./evalR n 20 # compute running times for W_54
	@cat data/wilk056.pol data/wilk.int | ./evalR n 20 # compute running times for W_56
	@cat data/wilk058.pol data/wilk.int | ./evalR n 20 # compute running times for W_58
	@cat data/wilk060.pol data/wilk.int | ./evalR n 20 # compute running times for W_60
	@cat data/wilk062.pol data/wilk.int | ./evalR n 20 # compute running times for W_62
	@cat data/wilk064.pol data/wilk.int | ./evalR n 20 # compute running times for W_64
	@cat data/wilk066.pol data/wilk.int | ./evalR n 20 # compute running times for W_66
	@cat data/wilk068.pol data/wilk.int | ./evalR n 20 # compute running times for W_68
	@cat data/wilk070.pol data/wilk.int | ./evalR n 20 # compute running times for W_70
	@cat data/wilk072.pol data/wilk.int | ./evalR n 20 # compute running times for W_72
	@cat data/wilk074.pol data/wilk.int | ./evalR n 20 # compute running times for W_74
	@cat data/wilk076.pol data/wilk.int | ./evalR n 20 # compute running times for W_76
	@cat data/wilk078.pol data/wilk.int | ./evalR n 20 # compute running times for W_78
	@cat data/wilk080.pol data/wilk.int | ./evalR n 20 # compute running times for W_80
	@cat data/wilk082.pol data/wilk.int | ./evalR n 10 # compute running times for W_82
	@cat data/wilk084.pol data/wilk.int | ./evalR n 10 # compute running times for W_84
	@cat data/wilk086.pol data/wilk.int | ./evalR n 10 # compute running times for W_86
	@cat data/wilk088.pol data/wilk.int | ./evalR n 10 # compute running times for W_88
	@cat data/wilk090.pol data/wilk.int | ./evalR n 10 # compute running times for W_90
	@cat data/wilk092.pol data/wilk.int | ./evalR n 10 # compute running times for W_92
	@cat data/wilk094.pol data/wilk.int | ./evalR n 10 # compute running times for W_94
	@cat data/wilk096.pol data/wilk.int | ./evalR n 10 # compute running times for W_96
	@cat data/wilk098.pol data/wilk.int | ./evalR n 10 # compute running times for W_98
	@cat data/wilk100.pol data/wilk.int | ./evalR n 10 # compute running times for W_100
	@cat data/wilk102.pol data/wilk.int | ./evalR n 10 # compute running times for W_102
	@cat data/wilk104.pol data/wilk.int | ./evalR n 10 # compute running times for W_104
	@cat data/wilk106.pol data/wilk.int | ./evalR n 10 # compute running times for W_106
	@cat data/wilk108.pol data/wilk.int | ./evalR n 10 # compute running times for W_108
	@cat data/wilk110.pol data/wilk.int | ./evalR n 10 # compute running times for W_110
	@cat data/wilk112.pol data/wilk.int | ./evalR n 10 # compute running times for W_112
	@cat data/wilk114.pol data/wilk.int | ./evalR n 10 # compute running times for W_114
	@cat data/wilk116.pol data/wilk.int | ./evalR n 10 # compute running times for W_116
	@cat data/wilk118.pol data/wilk.int | ./evalR n 10 # compute running times for W_118
	@cat data/wilk120.pol data/wilk.int | ./evalR n 10 # compute running times for W_120
	@cat data/wilk122.pol data/wilk.int | ./evalR n 10 # compute running times for W_122
	@cat data/wilk124.pol data/wilk.int | ./evalR n 10 # compute running times for W_124
	@cat data/wilk126.pol data/wilk.int | ./evalR n 10 # compute running times for W_126
	@cat data/wilk128.pol data/wilk.int | ./evalR n 10 # compute running times for W_128
	@cat data/wilk130.pol data/wilk.int | ./evalR n 10 # compute running times for W_130
	@cat data/wilk132.pol data/wilk.int | ./evalR n 10 # compute running times for W_132
	@cat data/wilk134.pol data/wilk.int | ./evalR n 10 # compute running times for W_134
	@cat data/wilk136.pol data/wilk.int | ./evalR n 10 # compute running times for W_136
	@cat data/wilk138.pol data/wilk.int | ./evalR n 10 # compute running times for W_138
	@cat data/wilk140.pol data/wilk.int | ./evalR n 10 # compute running times for W_140
	@cat data/wilk142.pol data/wilk.int | ./evalR n 10 # compute running times for W_142
	@cat data/wilk144.pol data/wilk.int | ./evalR n 10 # compute running times for W_144
	@cat data/wilk146.pol data/wilk.int | ./evalR n 10 # compute running times for W_146
	@cat data/wilk148.pol data/wilk.int | ./evalR n 10 # compute running times for W_148
	@cat data/wilk150.pol data/wilk.int | ./evalR n 10 # compute running times for W_150
	@cat data/wilk152.pol data/wilk.int | ./evalR n 10 # compute running times for W_152
	@cat data/wilk154.pol data/wilk.int | ./evalR n 10 # compute running times for W_154
	@cat data/wilk156.pol data/wilk.int | ./evalR n 10 # compute running times for W_156
	@cat data/wilk158.pol data/wilk.int | ./evalR n 10 # compute running times for W_158
	@cat data/wilk160.pol data/wilk.int | ./evalR n 10 # compute running times for W_160
	@cat data/wilk162.pol data/wilk.int | ./evalR n 4 # compute running times for W_162
	@cat data/wilk164.pol data/wilk.int | ./evalR n 4 # compute running times for W_164
	@cat data/wilk166.pol data/wilk.int | ./evalR n 4 # compute running times for W_166
	@cat data/wilk168.pol data/wilk.int | ./evalR n 4 # compute running times for W_168
	@cat data/wilk170.pol data/wilk.int | ./evalR n 4 # compute running times for W_170
	@cat data/wilk172.pol data/wilk.int | ./evalR n 4 # compute running times for W_172
	@cat data/wilk174.pol data/wilk.int | ./evalR n 4 # compute running times for W_174
	@cat data/wilk176.pol data/wilk.int | ./evalR n 4 # compute running times for W_176
	@cat data/wilk178.pol data/wilk.int | ./evalR n 4 # compute running times for W_178
	@cat data/wilk180.pol data/wilk.int | ./evalR n 4 # compute running times for W_180
	@cat data/wilk182.pol data/wilk.int | ./evalR n 4 # compute running times for W_182
	@cat data/wilk184.pol data/wilk.int | ./evalR n 4 # compute running times for W_184
	@cat data/wilk186.pol data/wilk.int | ./evalR n 4 # compute running times for W_186
	@cat data/wilk188.pol data/wilk.int | ./evalR n 4 # compute running times for W_188
	@cat data/wilk190.pol data/wilk.int | ./evalR n 4 # compute running times for W_190
	@cat data/wilk192.pol data/wilk.int | ./evalR n 4 # compute running times for W_192
	@cat data/wilk194.pol data/wilk.int | ./evalR n 4 # compute running times for W_194
	@cat data/wilk196.pol data/wilk.int | ./evalR n 4 # compute running times for W_196
	@cat data/wilk198.pol data/wilk.int | ./evalR n 4 # compute running times for W_198
	@cat data/wilk200.pol data/wilk.int | ./evalR n 4 # compute running times for W_200
	@cat data/wilk202.pol data/wilk.int | ./evalR n 4 # compute running times for W_202
	@cat data/wilk204.pol data/wilk.int | ./evalR n 4 # compute running times for W_204
	@cat data/wilk206.pol data/wilk.int | ./evalR n 4 # compute running times for W_206
	@cat data/wilk208.pol data/wilk.int | ./evalR n 4 # compute running times for W_208
	@cat data/wilk210.pol data/wilk.int | ./evalR n 4 # compute running times for W_210
	@cat data/wilk212.pol data/wilk.int | ./evalR n 4 # compute running times for W_212
	@cat data/wilk214.pol data/wilk.int | ./evalR n 4 # compute running times for W_214
	@cat data/wilk216.pol data/wilk.int | ./evalR n 4 # compute running times for W_216
	@cat data/wilk218.pol data/wilk.int | ./evalR n 4 # compute running times for W_218
	@cat data/wilk220.pol data/wilk.int | ./evalR n 4 # compute running times for W_220
	@cat data/wilk222.pol data/wilk.int | ./evalR n 4 # compute running times for W_222
	@cat data/wilk224.pol data/wilk.int | ./evalR n 4 # compute running times for W_224
	@cat data/wilk226.pol data/wilk.int | ./evalR n 4 # compute running times for W_226
	@cat data/wilk228.pol data/wilk.int | ./evalR n 4 # compute running times for W_228
	@cat data/wilk230.pol data/wilk.int | ./evalR n 4 # compute running times for W_230
	@cat data/wilk232.pol data/wilk.int | ./evalR n 4 # compute running times for W_232
	@cat data/wilk234.pol data/wilk.int | ./evalR n 4 # compute running times for W_234
	@cat data/wilk236.pol data/wilk.int | ./evalR n 4 # compute running times for W_236
	@cat data/wilk238.pol data/wilk.int | ./evalR n 4 # compute running times for W_238
	@cat data/wilk240.pol data/wilk.int | ./evalR n 4 # compute running times for W_240
	@cat data/wilk242.pol data/wilk.int | ./evalR n 4 # compute running times for W_242
	@cat data/wilk244.pol data/wilk.int | ./evalR n 4 # compute running times for W_244
	@cat data/wilk246.pol data/wilk.int | ./evalR n 4 # compute running times for W_246
	@cat data/wilk248.pol data/wilk.int | ./evalR n 4 # compute running times for W_248
	@cat data/wilk250.pol data/wilk.int | ./evalR n 4 # compute running times for W_250
	@cat data/wilk252.pol data/wilk.int | ./evalR n 4 # compute running times for W_252
	@cat data/wilk254.pol data/wilk.int | ./evalR n 4 # compute running times for W_254
	@cat data/wilk256.pol data/wilk.int | ./evalR n 4 # compute running times for W_256
	@cat data/wilk258.pol data/wilk.int | ./evalR n 4 # compute running times for W_258
	@cat data/wilk260.pol data/wilk.int | ./evalR n 4 # compute running times for W_260
	
############################### Plotting table 5: fix degree of polnomial, find optimal level 
fig2 figure2 table5: table5-125 table5-250
	@echo "Table 5 Printed: Deg 125 then Deg 250"

table5-250: evalF 
	@echo "Lagrange"
	@cat data/chebyshev250.pol data/chebyshev.int | ./evalF ml 2 83 1
	@cat data/hermite250.pol data/hermite.int | ./evalF ml 2 83 1
	@cat data/wilk250.pol data/wilk.int | ./evalF ml 28 83 1
	@cat data/mignotte251.pol data/mignotte.int | ./evalF ml 18 83 1
	@echo "Hermite"
	@cat data/chebyshev250.pol data/chebyshev.int | ./evalF mh 2 62 1
	@cat data/hermite250.pol data/hermite.int | ./evalF mh 2 62 1
	@cat data/wilk250.pol data/wilk.int | ./evalF mh 22 62 1
	@cat data/mignotte251.pol data/mignotte.int | ./evalF mh 14 62 1
	@echo "Done"

table5-125: evalF 
	@echo "Lagrange"
	@cat data/chebyshev124.pol data/chebyshev.int | ./evalF ml 1 42 1
	@cat data/hermite124.pol data/hermite.int | ./evalF ml 1 42 1
	@cat data/wilk124.pol data/wilk.int | ./evalF ml 6 42 1
	@cat data/mignotte125.pol data/mignotte.int | ./evalF ml 4 42 1
	@echo "Hermite"
	@cat data/chebyshev124.pol data/chebyshev.int | ./evalF mh 1 31 1
	@cat data/hermite124.pol data/hermite.int | ./evalF mh 1 31 1
	@cat data/wilk124.pol data/wilk.int | ./evalF mh 6 31 1
	@cat data/mignotte125.pol data/mignotte.int | ./evalF mh 4 31 1
	@echo "Done"

############################### testing evalL3ncheap
t-tab5: evalF
	@echo "Deg 250"
	@cat data/chebyshev250.pol data/chebyshev.int | ./evalF ml 2 2 1
	@cat data/hermite250.pol data/hermite.int | ./evalF ml 3 3 1
	@cat data/wilk250.pol data/wilk.int | ./evalF mh 22 22 1
	@cat data/mignotte251.pol data/mignotte.int | ./evalF mh 2 2 1
	@echo "Deg 125"
	@cat data/chebyshev124.pol data/chebyshev.int | ./evalF mh 1 1 1
	@cat data/hermite124.pol data/hermite.int | ./evalF mh 1 1 1
	@cat data/wilk124.pol data/wilk.int | ./evalF mh 6 6 1
	@cat data/mignotte125.pol data/mignotte.int | ./evalF mh 4 4 1	

t-tab4: evalR 
	@cat data/chebyshev40.pol data/chebyshev.int | ./evalR n 80

tab3: evalR
	@cat data/sparse0400.pol data/sparse.int | ./evalR ar 5
      
tab2: evalF
	@cat data/sparse0400.pol data/sparse.int | ./evalF ar 5

debugging:
	gdb

run-table2-once: evalR
	@cat data/sparse0800.pol data/sparse.int | ./evalR ar 3

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
