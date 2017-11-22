# Makefile
#
# Nov'2017: testing Michael Burr's code (in work with Juan)
#
# 	for converting between
#		double interval
# 	and
#		MPFI interval
#
# should compile with
#		g++ -std=c++11 main.cpp -lmpfi -lmpfr


#VARIABLES:
p=main

#TARGETS:

default: c

compile c:
	g++ -O3 -o ${p} -std=gnu++0x ${p}.cpp \
	    -I/usr/include -I/usr/local/include \
	    -L/usr/lib/ -L/usr/local/lib \
	    -lgmp -lmpfi -lmpfr -lboost_system



run r:
	./${p}
