# Makefile for progs/robotics/2d-vor2d
#
#  HOW TO USE THIS Makefile:
#
# (A)	To compile run this demo, you can use this Makefile and just have to type:
#
# 	       > make 		-- compiles the main program into "vor2d"
# 	       > make test	-- compiles and runs "vor2d" non-interactively
# 	       			   (this is Core Library's automatic self-test)
#
# (B)	But you probably want to try the various interactive demos of this program:
#
# 	       > make eg	-- compiles and runs "vor2d" interactively
#
# 	The default search-mode is "random".  We encourage you to just
# 	hit the "Run me" button to see different runs of the program.
#
#	This is equivalent to typing:
#
#	       > vor2d $(interactive) $(startX) $(startY)  $(goalX) $(goalY) \
#			$(epsilon) $(radius) $(fileName) \
#			$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
#			$(Qtype) $(seed)  $(inputDir)
#
#	where the arguments to vor2d correspond to the values $(var) of
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
#		eg0,
#		eg1, eg1a, eg1b,
#		eg2, eg2a, eg2b,
#		egbug, egbuga,
#		egbug2, egbug2a,
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

VAR=Debug

CORE_PATH=../../..
include ../Make.options

#=================================================
# User variables (you can change them in the command line)
#=================================================
interactive = 0		# 0=interactive, >0 is non-interactive

epsilon = 2		# resolution parameter
epsilon = 10		# resolution parameter

pseudo = 0		# boolean: true => show pseudo Vor objects
interior = 0	# boolean: true => show interior Vor objects

inputDir = inputs	# where to find input files
fileName = input2d.txt	# input environment file
fileName = input2.txt	# input environment file
fileName = input0.txt	# input environment file
fileName = input.txt	# input environment file
fileName = input100.txt	# input environment file
fileName = input150.txt	# input environment file
fileName = input200.txt	# input environment file
fileName = input300.txt	# input environment file
fileName = bugtrap.txt	# input environment file

boxWidth = 512		# initial configuration box size
boxHt = 512

windowPosX = 200	# initial Window position
windowPosY = 200	
windowPosX = 450	# initial Window position
windowPosY = 20	

Qtype = 0		# type of priority queue
			#	(0 = random, 1 = BFS, 3 = astar) 
seed = 11		# seed for random
step = 0		# number of steps to run
			#	(step=0 means run to completion)
		
#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = vor2d
SUBDIRS = . 
SRCS=$(wildcard $(addsuffix /*.cpp,$(SUBDIRS)))
OBJS=${SRCS:.cpp=.o}
OBJ_FILES = $(OBJS)
CXXFLAGS += -MMD

all: $(TARGETS)

# note: this target is the standard target that Core Library uses
#       to test its subdirectories.   So the program must run in a
#       non-interactive mode (i.e., the first argument to "vor2d" is "1").
#
test: $(TARGETS)
	./vor2d 1 \
		$(startX) $(startY)  $(goalX) $(goalY) \
		$(epsilon)  $(radius)  input.txt  $(boxWidth) $(boxHt) \
       		$(windowPosX) $(windowPosY) $(Qtype) $(seed) $(inputDir)

testall: $(TARGETS)
	make ega interactive=1
	make egb interactive=1

testa: $(TARGETS)
	./vor2d 1

testb: $(TARGETS)
	./vor2d 1 \
		$(startX) $(startY)  $(goalX) $(goalY) \
		$(epsilon)  $(radius)  input2.txt  $(boxWidth) $(boxHt) \
       		$(windowPosX) $(windowPosY) $(Qtype) $(seed) $(inputDir)

#=================================================
# Examples of running vor2d program
#=================================================
#
# this is default, but every option may be changed at command line:
#
eg:	vor2d
	@echo "Target 'eg':  runs vor2d with the default options of Makefile"
	./vor2d $(interactive) \
		$(epsilon) $(pseudo) $(interior)$(fileName) \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir)

star:	vor2d
	@echo "Target 'eg':  runs vor2d with the default options of Makefile"
	@echo "--  You can all of them in you 'make' command line, e.g.,"
	@echo "--      > make eg radius=60 goalX=111 goalY=450 startY=200 startX=20"
	./vor2d $(interactive) $(startX) $(startY)  $(goalX) $(goalY) \
		0.1 $(radius) star.poly \
		10 10 $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir)

dog:	vor2d
	@echo "Target 'eg':  runs vor2d with the default options of Makefile"
	@echo "--  You can all of them in you 'make' command line, e.g.,"
	@echo "--      > make eg radius=60 goalX=111 goalY=450 startY=200 startX=20"
	./vor2d $(interactive) $(startX) $(startY)  $(goalX) $(goalY) \
		0.1 $(radius) dog.poly \
		1000 1000 $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir)
		
line:	vor2d
	@echo "Target 'eg':  runs vor2d with the default options of Makefile"
	@echo "--  You can all of them in you 'make' command line, e.g.,"
	@echo "--      > make eg radius=60 goalX=111 goalY=450 startY=200 startX=20"
	./vor2d $(interactive) $(startX) $(startY)  $(goalX) $(goalY) \
		0.1 $(radius) line.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir)
		
# run with default compiled options:
#	The input file is input2.txt.
#	There are 3 combinatorially different paths.
#	Randomized strategy will find all three of them (keep hitting "run").
#
ega:	vor2d
	@echo "Target 'ega':  runs vor2d with default compiled options"
	./vor2d $(interactive)

# run with default Makefile options:
#
egb:	vor2d
	@echo "Target 'egb':  runs vor2d with default Makefile options for file input.txt"
	./vor2d $(interactive) 10 350 300 20 \
		10 30 input.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) 1111 $(inputDir)

# Simple NO PATH example:
nopath eg0:	vor2d
	@echo "Target 'eg0':  runs vor2d with random strategy for file input0.txt"
	@echo "--  this example has no path..."
	./vor2d $(interactive) 400 400  200 200 \
		1 20 input0.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		0 $(seed)  $(inputDir)

nopatha eg0a:	vor2d
	@echo "Target 'eg0a':  runs vor2d with astar strategy for file input0.txt"
	@echo "--  this example has no path..."
	./vor2d $(interactive) 400 400  200 200 \
		1 20 input0.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir)

# BUG TRAP:
###################################################
# Bugtrap example -- Non-existence of path, but barely!
egbug bug: vor2d
	@echo "Target 'egbug':  runs vor2d for bugtrap input, bugtrap.txt"
	@echo "--  this examples shows there is NO PATH, but barely so!"
	@echo "--  you could see a green path that must pass between two green boxes"
	@echo "--  that are NOT adjacent but shares a corner."
	./vor2d $(interactive) 200 350 30 30 \
		5 10 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir)

# Bugtrap example -- Existence of path, but barely!
egbuga buga: vor2d
	@echo "Target 'egbuga':  runs vor2d for bugtrap input, bugtrap.txt"
	@echo "--  this examples shows there is a PATH, but barely so!"
	@echo "--  the only change from target 'egbug' is that epsilon is now 4, not 5"
	@echo "--  that are NOT adjacent but shares a corner."
	./vor2d $(interactive) 200 350 30 30 \
		4 10 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir)

# BUG2 TRAP:
###################################################
# Bugtrap2 example -- Existence of path, but barely! (eps=1)
egbug2 bug2: vor2d
	@echo "Target 'egbug2':  runs vor2d for bugtrap input, bugtrap2.txt"
	@echo "--  this example shows a PATH, but barely so!"
	./vor2d $(interactive) 180 220 20 20 \
		1 8 bugtrap2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir)

# Bugtrap2 example -- Non-existence of path, but barely! (eps=2)
egbug2a bug2a: vor2d
	@echo "Target 'egbug2':  runs vor2d for bugtrap input, bugtrap2.txt"
	@echo "--  this example shows NO PATH, but barely so!"
	./vor2d $(interactive) 180 220 20 20 \
		2 8 bugtrap2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir)


eg2: vor2d
	@echo "Target eg2:  runs vor2d on simples obstacles"
	./vor2d $(interactive) 10 350  500 20 \
		10 20 input2.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir)

eg3: vor2d
	@echo "Target eg3:  runs vor2d amidst 100 randomly generated obstacles"
	./vor2d $(interactive) 10 350  280 220 \
		1 10 input3.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir)

eg4: vor2d
	@echo "Target eg4:  runs vor2d amidst 100 randomly generated obstacles"
	./vor2d $(interactive) 10 350  280 220 \
		1 10 input4.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir)

eg6: vor2d
	@echo "Target eg6:  runs vor2d amidst 100 randomly generated obstacles"
	./vor2d $(interactive) 10 350  280 220 \
		1 10 input6.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir)

# RANDOMLY GENERATED ENVIRONMENT:

eg10: vor2d
	./vor2d $(interactive) \
		1 $(pseudo) $(interior) input10.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir)

# eg100:
#
eg100: vor2d
	@echo "Target eg100:  runs vor2d amidst 100 randomly generated triangles"
	./vor2d $(interactive) 200 270 20 20 \
		2 5 input100.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir)
	#
# eg200:
#	This one, generated using our Python Generator, has 200 triangles.
#	Note that these triangles are not disjoint, but our theory remains
#	valid for such inputs.  This is the power of the SSS approach (over
#	the exact algorithms. RESULTS:
#	    			input file name = inputs/input200.txt
#	    			nPt=600
#	    			nPolygons=200
#	    			Time used: 1726.62 ms.
#	    			Path found !
#	    			Expanded 13720 times
#	    			total Free boxes: 9370
#	    			total Stuck boxes: 16404
#	    			total Mixed boxes smaller than epsilon: 13599
#	    			total Mixed boxes bigger than epsilon: 1788
#
#
eg200: vor2d
	@echo "Target eg200:  runs vor2d amidst 200 randomly generated triangles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./vor2d $(interactive) 150 260 20 20 \
		2 5 input200.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir)

# eg200a: there is an obvious path but probabilistic methods might waste
# 	lots of time checking elsewhere!
#
eg200a: vor2d
	@echo "Target eg200a:  runs vor2d amidst 200 randomly generated triangles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./vor2d $(interactive) 10 360 20 20 \
		2 10 input200.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir)

# eg200b: there is no path because eps=10
#
# 	The display seems to show some current bugs in the
# 	code.   THIS IS A GOOD PLACE TO RUN IN LEVEL 2 to CHECK!!!
#
eg200b: vor2d
	@echo "Target eg200b:  runs vor2d amidst 200 randomly generated triangles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./vor2d $(interactive) 150 260 20 20 \
		10 5 input200.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir)

# eg300: 
# 	RESULTS:
#				input file name = inputs/input300.txt
#				nPt=900
#				nPolygons=300
#				Time used: 28243.1 ms.
#				Path found !
#				Expanded 64220 times
#				total Free boxes: 37999
#				total Stuck boxes: 87069
#				total Mixed boxes smaller than epsilon: 67198
#				total Mixed boxes bigger than epsilon: 395
#
eg300: vor2d
	@echo "Target eg300:  runs vor2d amidst 300 randomly generated triangles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./vor2d $(interactive) 10 400 350 20 \
		1 3 input300.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir)


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
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o Dependencies

#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS) Dependencies

#=================================================
# END Makefile 
#=================================================

