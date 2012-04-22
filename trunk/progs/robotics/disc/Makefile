# Makefile for progs/robotics/2d-disc
#
#  HOW TO USE THIS Makefile:
#
# (A)	To compile run this demo, you can use this Makefile and just have to type:
#
# 	       > make 		-- compiles the main program into "disc"
# 	       > make test	-- compiles and runs "disc" non-interactively
# 	       			   (this is Core Library's automatic self-test)
#
# (B)	But you probably want to try the various interactive demos of this program:
#
# 	       > make eg	-- compiles and runs "disc" interactively
#
#	This is equivalent to typing:
#
#	       > disc $(interactive) $(startX) $(startY)  $(goalX) $(goalY) \
#			$(epsilon) $(radius) $(fileName) \
#			$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
#			$(Qtype) $(seed)  $(inputDir)
#
#	where the arguments to disc correspond to the values $(var) of
#	various Makefile variables.   
#
# (C)	You can override any of these values at the command line.
#	For instance, to change the radius of the robot
#	to 55, and to change the start position of the robot to (20,200), you can type:
#
#		> make eg radius=55 startY=200 startX=20
#
# (D)	Instead of the target "eg" you can use any of these targets:
#
#		ega, egb, egc, eg1, eg1a, eg1b, eg2, eg2a, eg2b, etc
#
# 	These targets uses different input files, all taken from
# 	the subdirectory "inputs".   
#
#
# April 13, 2012
#
# Since Core Library 2.1
# $Id:$

CORE_PATH=../../..
include ../Make.options

#=================================================
# User variables (you can change them in the command line)
#=================================================
interactive = 0		# 0=interactive, >0 is non-interactive

startX = 10		# start configuration
startY = 350
goalX = 300		# goal configuration
goalY = 20

epsilon = 2		# resolution parameter
epsilon = 10		# resolution parameter
radius = 40		# robot radius
radius = 30		# robot radius

inputDir = inputs	# where to find input files
fileName = bug0.txt	# input environment file
fileName = input2d.txt	# input environment file
fileName = input2.txt	# input environment file
fileName = input0.txt	# input environment file
fileName = input.txt	# input environment file

boxWidth = 512		# initial configuration box size
boxHt = 512

windowPosX = 200	# initial Window position
windowPosY = 200	
windowPosX = 700	# initial Window position
windowPosY = 20	

Qtype = 0		# type of priority queue
			#	(1 = BFS, 0 = random) // funny switch...
seed = 11		# seed for random
step = 0		# number of steps to run
			#	(step=0 means run to completion)
		
#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = disc

OBJ_FILES = Box.o QuadTree.o Corner.o Wall.o 

all: $(TARGETS)

# note: this target is the standard target that Core Library uses
#       to test its subdirectories.   So the program must run in a
#       non-interactive mode (i.e., the first argument to "disc" is "1").
#
test: $(TARGETS)
	./disc 1 \
		$(startX) $(startY)  $(goalX) $(goalY) \
		$(epsilon)  $(radius)  input.txt  $(boxWidth) $(boxHt) \
       		$(windowPosX) $(windowPosY) $(Qtype) $(seed) $(inputDir)

testa: $(TARGETS)
	./disc 1

testb: $(TARGETS)
	./disc 1 \
		$(startX) $(startY)  $(goalX) $(goalY) \
		$(epsilon)  $(radius)  input2.txt  $(boxWidth) $(boxHt) \
       		$(windowPosX) $(windowPosY) $(Qtype) $(seed) $(inputDir)

#=================================================
# Examples of running disc program
#=================================================
#
# this is default, but every option may be changed at command line:
#
eg:	disc
	@echo "Target 'eg':  runs disc with the default options of Makefile"
	@echo "--  You can all of them in you 'make' command line, e.g.,"
	@echo "--      > make eg radius=60 goalX=111 goalY=450 startY=200 startX=20"
	./disc $(interactive) $(startX) $(startY)  $(goalX) $(goalY) \
		$(epsilon) $(radius) $(fileName) \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir)

# run with default compiled options:
#
ega:	disc
	@echo "Target 'ega':  runs disc with default compiled options"
	./disc 

# run with default Makefile options:
#
egb:	disc
	@echo "Target 'egb':  runs disc with default Makefile options for file input.txt"
	./disc $(interactive) 10 350 300 20 \
		10 30 input.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) 1111 $(inputDir)

# Simple NO PATH example:
nopath eg0:	disc
	@echo "Target 'eg0':  runs disc with default options for file input0.txt"
	@echo "--  this example has no path..."
	./disc $(interactive) 400 400  200 200 \
		10 20 input0.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir)

# BUG TRAP:
egbug: disc
	@echo "Target 'egbug':  runs disc for bugtrap input, bugtrap.txt"
	./disc $(interactive) 30 30 200 200 \
		$(epsilon) 10 bugtrap0.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir)

# RANDOMLY GENERATED ENVIRONMENT:
#
# eg1:
#
eg1: disc
	@echo "Target eg1:  runs disc on randomly generated obstacles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./disc $(interactive) 150 260 20 20 \
		2 5 input1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir)

# eg1a: there is an obvious path but probabilistic methods might waste
# 	lots of time checking elsewhere!
#
eg1a: disc
	@echo "Target eg1a:  runs disc on randomly generated obstacles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./disc $(interactive) 10 360 20 20 \
		2 10 input1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir)

# eg1b: there is no path because eps=10
#
# 	The display also shows some current bugs in the
# 	code.   THIS IS A GOOD PLACE TO RUN IN LEVEL 2 to CHECK!!!
#
eg1b: disc
	@echo "Target eg1a:  runs disc on randomly generated obstacles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./disc $(interactive) 150 260 20 20 \
		10 5 input1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir)


eg2: disc
	./disc $(interactive) 10 350  500 20 \
		10 20 input2.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir)


moretest: $(TARGETS)
# nothing to do

disc: $(OBJ_FILES) disc.o


#=================================================
# Temp
#=================================================
m:
	gvim Makefile
vi:
	gvim disc.cpp

#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(OBJ_FILES) $(LDFLAGS) $< $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c -O3 $(CXXFLAGS) $(CORE_INC) $< -o $@

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o

#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

#=================================================
# END Makefile 
#=================================================

