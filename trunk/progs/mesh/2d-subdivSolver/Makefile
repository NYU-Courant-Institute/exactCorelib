# Makefile for sub-division solver

CORE_PATH=$(HOME)/corelib-svn/trunk
include ../Make.options

VAR=Opt
CORE_INC=-I. -I$(CORE_PATH)/inc 
CORE_LIB=-L$(CORE_PATH)/lib \
	 -lcore++$(VAR) 
#CORE_LIB=

COREX_LIB_LEVEL=-lcoreex++$(VAR)_level
COREX_LIB_LEVEL=


#=================================================
# Define target files (put all your files here)
#=================================================

TARGETS=main


#=================================================
# Rules
#=================================================

$(TARGETS):	
	g++ -o main main.cpp  \
		 -I/usr/include -I/usr/local/include \
		 $(CORE_INC)$ \
		-L/usr/lib/ -L/usr/local/lib \
		 $(CORE_LIB)$ \
	    -lgmp -lmpfr -lcore++Opt
#	g++ -o main main.cpp  \
#	    -I$(CORE_PATH)/inc  \
#	    -I$(CORE_PATH)/gmp/include  \
#		 -I/usr/include -I/usr/local/include \
#	    -I$(CORE_PATH)/progs/mesh/arrangements-gui \
#	    -L$(CORE_PATH)/lib \
#	    -L$(CORE_PATH)/gmp/lib \
#		-L/usr/lib/ -L/usr/local/lib \
#	    -lgmp -lmpfr -lcore++Opt

test:
	./main

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o
#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORETARGETS:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

#=================================================
# End
#=================================================
