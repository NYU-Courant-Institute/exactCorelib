# Makefile for progs/gwb/polyhedron/
#
#  HOW TO USE THIS Makefile:
#
# (A)	To compile run this demo, you can use this Makefile and just have to type:
#
# 	       > make 		-- compiles the main program into "gwb"
# 	       > make test	-- compiles and runs "gwb" non-interactively
# 	       			   (this is Core Library's automatic self-test)
#
# (B)	See the default demo:
#
# 	       > make eg	-- compiles and runs "gwb" interactively
#
#	To see what these default parameters are, you can type
#
#		> make params
#
# (C)	You can override any of these default parameters at the command line.
#	For instance, to change the input file to "input2.txt", you may type:
#
#		> make eg fineName=input2.txt
#
# (D)	Instead of the target "eg" you can try these targets (alternative
# 	names are in parenthesis):
#
#		eg0 (annulus)
#		eg1 (simple)
#		eg2 (simple2)
#		eg3 (dog)
#
# 	These targets uses different input files, all taken from
# 	the subdirectory "inputs".   
#
################################################################################
# 	Kai Cao and Chee Yap
# 	May 20, 2012
# 	Since Core Library 2.1
# 	$Id:$
################################################################################

#=================================================
#	Core Library Options
#=================================================
VAR=Opt
VAR=Debug
CORE_PATH=../../..
include ../Make.options

#=================================================
# User variables (you can change them in the command line)
#=================================================
interactive = 0		# 0=interactive, >0 is non-interactive

inputDir = inputs	# where to find input files
fileName = cube.txt	# cube
fileName = cube.txt	# cube

shape = bullet		# builtin shape to display

mode = face		# show faces of surfaces
mode = wire		# show wire frame of surfaces

rad = 20		# radius
ht = 40			# height
hor = 30		# horizontal dim
ver = 20		# vertical dim

## unused:
xtrans = 5		# x-translation of the input data 
ytrans = 5		# y-translation of the input data
scale  = 1		# scaling of input data 

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = gwb
SUBDIRS = . 
SRCS=$(wildcard $(addsuffix /*.cpp,$(SUBDIRS)))
OBJS=${SRCS:.cpp=.o}
OBJ_FILES = $(OBJS)
CXXFLAGS += -MMD

all: $(TARGETS)

# note: this target is the standard target that Core Library uses
#       to test its subdirectories.   So the program must run in a
#       non-interactive mode !!!
#
test: $(TARGETS)

testall: $(TARGETS)

#=================================================
# Examples of running gwb program
#=================================================
#
# This is default, but every option may be changed at command line:

##################################################
# SIMPLE MAKEFILE:
##################################################
#compiling...
#gwb:	*.h *.cpp
#	g++ *.h *.cpp -o gwb -LGL -lglut -lglui

#eg default (does not work)
eg:	gwb	
	./gwb $(mode) $(shape) $(rad) $(ht) $(hor) $(ver)

#eg0 
eg0 facehemi:	gwb	
	./gwb $(mode) hemisphere 6 20 20

#eg1 sphere
eg1 sphere wiresphere:	gwb	
	./gwb $(mode) sphere 6 20 20

#eg2 torus
eg2 donut torus wiretorus:	gwb	
	./gwb $(mode) torus 6 4 20 20

#eg3 facecylinder
eg3 facecylinder:	gwb	
	./gwb $(mode) cylinder 6 20 20 20

#eg4 wirecylinder
eg4 cylinder wirecylinder:	gwb	
	./gwb $(mode) cylinder 6 20 20 20

#eg5 disk 
eg5 disk wiredisk:	gwb	
	./gwb $(mode) disk 10 5 20 20

#eg6 facedisk 
eg6 facedisk:	gwb	
	./gwb $(mode) facedisk 10 5 20 20

#eg7 bullet 
eg7 bullet:	gwb	
	./gwb $(mode) bullet 10 5 20 20

#eg8 cone 
eg8 cone:	gwb	
	./gwb $(mode) cone 10 5 20 20
##################################################
		
moretest: $(TARGETS)
# nothing to do

$(TARGETS): $(OBJ_FILES)

#=================================================
# Temp
#=================================================
m:
	gvim Makefile
vi:
	gvim vor2d.cpp

#=================================================
# Rules
#=================================================

.SUFFIXES: .cpp

%: %.o
	${CXX} $(OBJ_FILES) $(LDFLAGS) $(CORE_LIB) -o $@

.cpp.o:
	${CXX} $(CXXFLAGS) $(CORE_INC) -c $< -o $@
	cat $*.d >> Dependencies
	rm -f $*.d 

Dependencies:
	touch Dependencies

include Dependencies

#=================================================
# Misc
#=================================================

params:
	@echo "================ Parameters =================="
	@echo "    interactive = " ${interactive}
	@echo "    epsilon = " ${epsilon}
	@echo "    pseudo = " ${pseudo}
	@echo "    interior = " ${interior}
	@echo "    inputDir = " ${inputDir}
	@echo "    fileName = " ${fileName}
	@echo "    boxWidth = " ${boxWidth}
	@echo "    boxHt = " ${boxHt}
	@echo "    windowPosX = " ${windowPosX}
	@echo "    windowPosY = " ${windowPosY}
	@echo "    seed = " ${seed}
	@echo "    step = " ${step}
	@echo "    xtrans = " ${xtrans}
	@echo "    ytrans = " ${ytrans}
	@echo "    scale = " ${scale}
	@echo "    closePoly = " ${closePoly}
	@echo "=============================================="

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o Dependencies $(TARGETS)

#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS) Dependencies

#=================================================
# END Makefile 
#=================================================

