# Makefile

# set the core library directory and include Make.config
include ../Make.options

# define targets list
TARGETS= \
	timeFilter \
	testFilter \
	testFilter2 \
	createMatrices

# default target
all: ${TARGETS}

#@begin targets
timeFilter: timeFilter.o

testFilter: testFilter.o

testFilter2: testFilter2.o

createMatrices: createMatrices.o

tt: tt.o

matrix:
	./createMatrices inputs/200x3x10matrices 200 3 10
	./createMatrices inputs/200x4x20matrices 200 4 20
	./createMatrices inputs/200x5x30matrices 200 5 30

morematrix:
	./createMatrices inputs/1000x2x10matrices 1000 2 10
	./createMatrices inputs/1000x3x10matrices 1000 3 10
	./createMatrices inputs/1000x4x10matrices 1000 4 10
	./createMatrices inputs/500x5x10matrices 500 5 10
	./createMatrices inputs/500x6x10matrices 500 6 10
	./createMatrices inputs/500x7x10matrices 500 7 10
	./createMatrices inputs/500x8x10matrices 500 8 10

test:
	./timeFilter inputs/200x3x10matrices
	./timeFilter inputs/200x4x20matrices
	./timeFilter inputs/200x5x30matrices

# timing filter
time: testFilter2
	./testFilter2 inputs/1000x2x10matrices
	./testFilter2 inputs/1000x3x10matrices
	./testFilter2 inputs/1000x4x10matrices
	./testFilter2 inputs/500x5x10matrices
	./testFilter2 inputs/500x6x10matrices
	./testFilter2 inputs/500x7x10matrices
	./testFilter2 inputs/500x8x10matrices

# verify the filter works well
verify: testFilter
	./testFilter inputs/1000x2x10matrices
	./testFilter inputs/1000x3x10matrices
	./testFilter inputs/1000x4x10matrices
	./testFilter inputs/500x5x10matrices
	./testFilter inputs/500x6x10matrices
	./testFilter inputs/500x7x10matrices
	./testFilter inputs/500x8x10matrices

#@end targets

#include standard rules from Make.rules
include ../Make.rules
