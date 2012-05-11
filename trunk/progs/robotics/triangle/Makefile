# Makefile for progs/robotics/triangle
#
#  HOW TO USE THIS Makefile:
#
# (A)	To compile run this demo, you can use this Makefile and just have to type:
#
# 	       > make 		-- compiles the main program into "tri"
# 	       > make test	-- compiles and runs "tri" non-interactively
# 	       			   (this is Core Library's automatic self-test)
#
# (B)	But you probably want to try the various interactive demos of this program:
#
# 	       > make eg	-- compiles and runs "tri" interactively
#
# 	The default search-mode is "random".  We encourage you to just
# 	hit the "Run me" button to see different runs of the program.
#
#	This is equivalent to typing:
#
#	       > tri $(interactive) $(startX) $(startY)  $(startTheta) \
#	       		$(goalX) $(goalY) $(goalTheta) \
#			$(epsilon) $(radius) $(fileName) \
#			$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
#			$(Qtype) $(seed)  $(inputDir)
#
#	where the arguments to tri correspond to the values $(var) of
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
#		eg, ega, egb,
#		eg1, eg1a, eg1b,
#		eg2, eg2a, eg2b,
#		egbug, egbuga,
#		etc.
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
startTheta = 0		# in degrees
goalX = 300		# goal configuration
goalY = 20
goalTheta = 90		# in degrees

epsilon = 10		# resolution parameter
epsilon = 5		# resolution parameter

radius = 40		# robot radius
radius = 70		# robot radius

inputDir = inputs	# where to find input files
fileName = input2d.txt	# input environment file
fileName = input2.txt	# input environment file
fileName = input0.txt	# input environment file
fileName = bugtrap.txt	# input environment file
fileName = input.txt	# input environment file

boxWidth = 512		# initial configuration box size
boxHt = 512

windowPosX = 200	# initial Window position
windowPosY = 200	
windowPosX = 500	# initial Window position
windowPosY = 20	

Qtype = 0		# type of priority queue
			#	(1 = BFS, 0 = random) // funny switch...
seed = 11		# seed for random
step = 0		# number of steps to run
			#	(step=0 means run to completion)
		
#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = tri

OBJ_FILES = Box.o QuadTree.o Corner.o Wall.o fileProcessor.o Timer.o

all: $(TARGETS)

# note: this target is the standard target that Core Library uses
#       to test its subdirectories.   So the program must run in a
#       non-interactive mode (i.e., the first argument to "tri" is "1").
#
test: $(TARGETS)
	./tri 1 \
		$(startX) $(startY)  $(startTheta) $(goalX) $(goalY) $(startBeta) \
		$(epsilon)  $(radius)  input.txt  $(boxWidth) $(boxHt) \
       		$(windowPosX) $(windowPosY) $(Qtype) $(seed) $(inputDir)

testa: $(TARGETS)
	./tri 1

testb: $(TARGETS)
	./tri 1 \
		$(startX) $(startY)  $(startTheta) $(goalX) $(goalY) $(startBeta) \
		$(epsilon)  $(radius)  input2.txt  $(boxWidth) $(boxHt) \
       		$(windowPosX) $(windowPosY) $(Qtype) $(seed) $(inputDir)

#=================================================
# Examples of running tri program
#=================================================
#
# this is default, but every option may be changed at command line:
#
eg:	tri
	@echo "Target 'eg':  runs tri with the default options of Makefile"
	@echo "--  You can all of them in you 'make' command line, e.g.,"
	@echo "--      > make eg radius=60 goalX=111 goalY=450 startY=200 startX=20"
	./tri $(interactive) $(startX) $(startY)  $(startTheta) \
	    	$(goalX) $(goalY) $(goalTheta) \
		$(epsilon) $(radius) $(fileName) \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir)

# run with default compiled options:
#	The input file is input2.txt.
#	There are 3 combinatorially different paths.
#	Randomized strategy will find all three of them (keep hitting "run").
#
ega:	tri
	@echo "Target 'ega':  runs tri with default compiled options"
	./tri 

# run with default Makefile options:
#
egb:	tri
	@echo "Target 'egb':  runs tri with default Makefile options for file input.txt"
	./tri $(interactive) 10 350 0 300 20 0\
		10 40 input.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) 1111 $(inputDir)

# Simple NO PATH example:
nopath eg0:	tri
	@echo "Target 'eg0':  runs tri with default options for file input0.txt"
	@echo "--  this example has no path..."
	./tri $(interactive) 400 400 0 200 200 0\
		20 20 input0.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir)

# eg1:
eg1: tri
	@echo "Target eg1:  runs tri on input2e.txt (which has a bounding box)"
	./tri $(interactive) 150 260 0.5 40 40 0 \
		10 25 input2e.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir)

# eg2:
eg2: tri
	@echo "Target eg2e:  runs tri on input2.txt"
	./tri $(interactive) 150 260 0 20 20 0 \
		10 10 input2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir)

# BUG TRAP:
# 
# Bugtrap example -- Non-existence of path, but barely!
egbug: tri
	@echo "Target 'egbug':  runs tri for bugtrap input, bugtrap.txt"
	@echo "--  this examples shows there is NO PATH, but barely so!"
	@echo "--  you could see a green path that must pass between two green boxes"
	@echo "--  that are NOT adjacent but shares a corner."
	./tri $(interactive) 200 350 0 30 30 0 \
		20 10 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir)

# Bugtrap example -- Existence of path, but barely!
egbuga: tri
	@echo "Target 'egbuga':  runs tri for bugtrap input, bugtrap.txt"
	@echo "--  this examples shows there is a PATH, but barely so!"
	@echo "--  the only change from target 'egbug' is that epsilon is now 10, not 20"
	@echo "--  that are NOT adjacent but shares a corner."
	./tri $(interactive) 200 350 0 30 30 0 \
		10 10 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir)

# RANDOMLY GENERATED ENVIRONMENT:
#	This one, generated using our Python Generator, has 200 triangles.
#	Note that these triangles are not disjoint, but our theory remains
#	valid for such inputs.  This is the power of the SSS approach (over
#	the exact algorithms.

# eg100: 100 triangles
eg100: tri
	@echo "Target eg100:  runs tri amidst 100 randomly generated triangles"
	./tri $(interactive) 200 270 0 20 20 0\
		10 5 input100.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir)

# eg100a: 100 triangles
eg100a: tri
	@echo "Target eg100a:  runs tri amidst 100 randomly generated triangles"
	@echo "This one is a bit harder than eg100 (with radius 8 instead of 5)"
	./tri $(interactive) 200 270 0 20 20 0\
		10 8 input100.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir)

# eg200: 200 triangles
eg200: tri
	@echo "Target eg200:  runs tri on randomly generated obstacles"
	@echo "--  This may take a noticeable amount bit of time!!!"
	./tri $(interactive) 150 260 0 20 20 0 \
		5 2 input1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir)

# eg200a: there is an obvious path but probabilistic methods might waste
# 	lots of time checking elsewhere!
#
eg200a: tri
	@echo "Target eg200a:  runs tri on randomly generated obstacles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./tri $(interactive) 10 360 0 20 20 0 \
		2 10 input1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir)

# eg200b: there is no path because eps=10
#
eg200b: tri
	@echo "Target eg200b:  runs tri on randomly generated obstacles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./tri $(interactive) 150 260 0 20 20 0 \
		10 5 input1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir)

# eg300: 
#
eg300: tri
	@echo "Target eg300:  runs tri amidst 300 randomly generated triangles"
	@echo "--  This may take a noticeable amount bit of time!!! "
	./tri $(interactive) 10 400 0 350 20 0\
		3 3 input300.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir)


moretest: $(TARGETS)
# nothing to do

tri: $(OBJ_FILES) tri.o


#=================================================
# Temp
#=================================================
m:
	gvim Makefile
vi:
	gvim tri.cpp

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

