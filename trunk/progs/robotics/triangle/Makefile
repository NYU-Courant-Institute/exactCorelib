# Makefile for progs/robotics/triangle
#
#  HOW TO USE THIS Makefile:
#
# (A)	To compile run this demo, you can use this Makefile and just have to type:
#
# 	       > make 		-- compiles the main program into "tri"
# 	       > make help	-- help message
# 	       > make test	-- compiles and runs "tri" non-interactively
# 	       			   (this is Core Library's automatic self-test)
# 	       > make [target]	-- where [target] has the form egX
# 	                     	   and X=1,2,3,4,5,6,100,150,200,300, etc.
#
# (B)	But you may try the various interactive demos of this program:
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
#			$(Qtype) $(seed)  $(inputDir) \
#			$(xtrans) $(ytrans) $(scale)
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
#		ega, egb,
#		eg1, eg1a, eg1b,
#		eg2, eg2a, eg2b,
#		eg3
#		eg4 (= egbug, bug), eg4a (= egbuga, buga),
#		eg5 (= egbug2, bug2), eg5a (= egbug2a, bug2a),
#		eg6 (= kavraki), eg6a (= kavrakia),
#		eg7 (= tunnel), eg7a (= tunnela), eg7b (= tunnelb)
#		eg100, eg100a
#		eg150
#		eg200, eg200a, eg200b
#		eg300
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

startX = 60		# start configuration
startY = 350
startTheta = 0		# in degrees
goalX = 300		# goal configuration
goalY = 60
goalTheta = 90		# in degrees

epsilon = 10		# resolution parameter
epsilon = 5		# resolution parameter

radius = 70		# robot radius
radius = 40		# robot radius

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
xtrans = 0		# x-translation of input file
ytrans = 0		# y-translation of input file
scale = 1		# scaling of input file
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

testa: $(TARGETS)
	./tri 1

testb: $(TARGETS)
	./tri 1 \
		$(startX) $(startY)  $(startTheta) $(goalX) $(goalY) $(startBeta) \
		$(epsilon)  $(radius)  input2.txt  $(boxWidth) $(boxHt) \
       		$(windowPosX) $(windowPosY) $(Qtype) $(seed) $(inputDir) \
		$(xtrans) $(ytrans) $(scale)

help:
	@echo "USAGE:  The main program is called tri.  Demos can be invoked thus:"
	@echo "              > make eg [...optional parameters...]"
	@echo "     where parameters include fileName, radius, startX, startY,"
	@echo "     goalX, goalY, etc.  Best to see the Makefile for examples."
	@echo "     You can also call demos where these these parameters are prechosen:"
	@echo "              > make egX"
	@echo "     where X is replaced by 1, 2, 3, 4, 5, 6, 100, 150, 200, 300, etc."
	@echo "     Demos have variants (such as egXa, egXb or egXc)"
	@echo "     or alternative names (such as bug, bug2, tunnal, etc): "
	@echo "        ega, egb,"
	@echo "        eg1, eg1a, eg1b, [simple environment with bounding box]"
	@echo "        eg2, eg2a, eg2b, [simple environment without bounding box]"
	@echo "        eg3  [simple environment with door]"
	@echo "        eg4 (= egbug, bug), eg4a (= egbuga, buga),"
	@echo "        eg5 (= egbug2, bug2), eg5a (= egbug2a, bug2a),"
	@echo "        eg6 (= kavraki), eg6a (= kavrakia),"
	@echo "        eg7 (= tunnel), eg7a (= tunnela), eg7b (= tunnelb)"
	@echo "        eg100, eg100a"
	@echo "        eg150"
	@echo "        eg200, eg200a, eg200b"
	@echo "        eg300"


#=================================================
# Examples of running tri program
#=================================================
#
# this is default, but every option may be changed at command line:
#
eg:	./tri
	@echo "Target 'eg':  runs tri with the default options of Makefile"
	@echo "--  You can all of them in you 'make' command line, e.g.,"
	@echo "--      > make eg radius=60 goalX=111 goalY=450 startY=200 startX=20"
	./tri $(interactive) $(startX) $(startY)  $(startTheta) \
	    	$(goalX) $(goalY) $(goalTheta) \
		$(epsilon) $(radius) $(fileName) \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# run with default compiled options:
#	The input file is input2.txt.
#	There are 3 combinatorially different paths.
#	Randomized strategy will find all three of them (keep hitting "run").
#
ega:	./tri
	@echo "Target 'ega':  runs tri with default compiled options"
	./tri &

# run with default Makefile options:
#
egb:	./tri
	@echo "Target 'egb':  runs tri with default Makefile options for file input.txt"
	./tri $(interactive) 60 350 -0.17 300 60 0.5 \
		5 40 input.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# Simple NO PATH example:
nopath eg0:	./tri
	@echo "Target 'eg0':  runs tri with default options for file input0.txt"
	@echo "--  this example has no path..."
	./tri $(interactive) 400 400 0 200 200 0\
		20 20 input0.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# eg1:	input2e.txt is like input2, but there is a bounding box
eg1: ./tri
	@echo "Target eg1:  runs tri on input2e.txt (which has a bounding box)"
	./tri $(interactive) 150 260 0.5 40 40 0 \
		10 25 input2e.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir)

# eg2: input 2.text, no bounding box
eg2: ./tri
	@echo "Target eg2e:  runs tri on input2.txt"
	./tri $(interactive) 150 260 0 20 20 0 \
		10 10 input2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# eg3: input3.txt is line input2e, but there is a narrow door
eg3: ./tri
	@echo "Target eg3:  runs tri on input3.txt"
	./tri $(interactive) 150 260 0.5 450 450 0 \
		10 30 input3.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# BUG TRAPS:
# 
# Bugtrap example -- Non-existence of path, but barely!
eg4 egbug bug: ./tri
	@echo "Target 'egbug':  runs tri for bugtrap input, bugtrap.txt"
	@echo "--  this examples shows there is NO PATH, but barely so!"
	@echo "--  you could see a green path that must pass between two green boxes"
	@echo "--  that are NOT adjacent but shares a corner."
	./tri $(interactive) 200 350 0 30 30 0 \
		20 10 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# Bugtrap example -- Existence of path, but barely!
eg4a egbuga buga: ./tri
	@echo "Target 'egbuga':  runs tri for bugtrap input, bugtrap.txt"
	@echo "--  this examples shows there is a PATH, but barely so!"
	@echo "--  the only change from target 'egbug' is that epsilon is now 10, not 20"
	@echo "--  that are NOT adjacent but shares a corner."
	./tri $(interactive) 200 350 0 30 30 0 \
		10 10 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# Bugtrap2 example -- Non-existence of path.
# 	THIS IS TOO SLOW, SO MORE WORK NEEDED.
# 	FOR NOW, WE USE big values for epsilon, etc...
# 	SAMPLE values: Epsilon=18, radius=1 takes 835 ms to report NO PATH.
# 	SAMPLE values: Epsilon=17, radius=2 takes 2800 ms to report NO PATH.
eg5 egbug2 bug2: ./tri
	@echo "Target 'egbug':  runs tri for bugtrap input, bugtrap.txt"
	@echo "--  this examples shows there is NO PATH, but barely so!"
	@echo "--  you could see a green path that must pass between two green boxes"
	@echo "--  that are NOT adjacent but shares a corner."
	./tri $(interactive) 200 350 0 30 30 0 \
		18 2 bugtrap2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# Bugtrap2 example -- Existence of path.
# 	THIS IS TOO SLOW, SO MORE WORK NEEDED.
# 	FOR NOW, WE USE small values for radius, etc...
# 	SAMPLE values: Epsilon=16, radius=1 takes 5700 ms to report PATH.
# 	SAMPLE values: Epsilon=16, radius=2 takes 18000 ms to report PATH.
# 	SAMPLE values: Epsilon=12, radius=2 takes 41000 ms to report PATH.
eg5a egbug2a bug2a: ./tri
	@echo "Target 'egbug':  runs tri for bugtrap input, bugtrap.txt"
	@echo "--  this examples shows there is NO PATH, but barely so!"
	@echo "--  you could see a green path that must pass between two green boxes"
	@echo "--  that are NOT adjacent but shares a corner."
	./tri $(interactive) 200 350 0 30 30 0 \
		16 1 bugtrap2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# eg6: This environment is taken from Kavraki's groups Demo for OOPSMP:
#  NOTE: we must do scaling of the input
#  	(because dimensions came from xfig)
eg6: ./tri
	@echo "Target DemoSE2_PRM (from Kavraki's group)"
	./tri $(interactive) 75 350 0 380 205 0\
		5 15 DemoSE2_PRM.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		250 250 0.04 \
		&

# eg6a:  Variant of eg6a where their "robot" is shown.
eg6a: ./tri
	@echo "Target DemoSE2_PRM (from Kavraki's group) -- their robot is shown"
	@echo "	There is a narrow path... when radius=15, epsilon=2"
	@echo " 	There is No Path... when radius=15, epsilon=3"
	./tri $(interactive) 75 350 0 380 205 0\
		2 15 DemoSE2_PRMa.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		250 250 0.04 \
		&

# eg7, egtunnel:  tunnel example from Salzman et al.
# 	NOTE that the current search strategy seems to miss the direct path!!
# 	TODO: develop a better search (e.g. Best First Search)
eg7 egtunnel tunnel: ./tri
	./tri $(interactive) 200 280 0 450 300 0\
		10 15 tunnel.poly \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		100 200 0.05

# eg7a or tunnela:  Variant of tunnel.  We introduce a confining box to force 
# 	the robot to go through the tunnel.  This one is reasonably fast
# 	(radius=10, epsilon=10).  Path is found.  See tunnelXa for a much harder instance.
eg7a egtunnela tunnela: ./tri
	./tri $(interactive) 200 280 0 450 300 0\
		10 10 tunnelx.poly \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		100 200 0.05

# eg7b or tunnelb:  Variant of tunnel.  We introduce a confining box to force 
# 	the robot to go through the tunnel.  
#	CAREFUL, THIS DEMO (with epsilon=10, radius=15)
#	TAKES A LONG TIME BUT WILL EVENTUALLY FIND A PATH (takes about 25 minutes)
#
	#	./tri 0		 200 280 0 450 300 0\
	#		10 15 tunnelx.poly \
	#		512 512 500	20 \
	#		1 1111 inputs	 \
	#		100 200 0.05
	#	before interactive, Qtype= 1
	#	inside run:  Qtype= 0
	#	input file name = inputs/tunnelx.poly
	#	nPt=36
	#	nPolygons=11
	#	done genEmptyTree 
	#	          --------------------->>  PATH FOUND !
	#	Expanded 48769 times
	#	Time used: 1.39074e+06 ms
	#	total Free boxes: 78886
	#	total Stuck boxes: 2757
	#	total Mixed boxes smaller than epsilon: 250988
	#	total Mixed boxes bigger than epsilon: 8781
	#	####################### END of RUN ######################
eg7b tunnelb: ./tri
	./tri $(interactive) 200 280 0 450 300 0\
		10 15 tunnelx.poly \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		100 200 0.05

# RANDOMLY GENERATED ENVIRONMENT:
#	This one, generated using our Python Generator, has 200 triangles.
#	Note that these triangles are not disjoint, but our theory remains
#	valid for such inputs.  This is the power of the SSS approach (over
#	the exact algorithms.

# eg100 100: 100 random triangles
eg100 100: ./tri
	@echo "Target eg100:  runs tri amidst 100 randomly generated triangles"
	./tri $(interactive) 200 270 0 20 20 0\
		10 5 input100.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# eg100a: 100 random triangles
eg100a 100a: ./tri
	@echo "Target eg100a:  runs tri amidst 100 randomly generated triangles"
	@echo "This one is a bit harder than eg100 (with radius 8 instead of 5)"
	./tri $(interactive) 200 270 0 20 20 0\
		10 8 input100.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# eg150: 150 random triangles (use radius=1)
eg150 150: ./tri
	@echo "Target eg150:  runs tri amidst 100 randomly generated triangles"
	./tri $(interactive) 200 270 0 20 20 0\
		10 1 input150.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# eg200: 200 random triangles
eg200 200: ./tri
	@echo "Target eg200:  runs tri on randomly generated obstacles"
	@echo "--  This may take a noticeable amount bit of time!!!"
	./tri $(interactive) 150 260 0 20 20 0 \
		5 2 input1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# eg200a: there is an obvious path but probabilistic methods might waste
# 	lots of time checking elsewhere!
#
eg200a 200a: ./tri
	@echo "Target eg200a:  runs tri on randomly generated obstacles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./tri $(interactive) 10 360 0 20 20 0 \
		2 10 input1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# eg200b: there is no path because eps=10
#
eg200b: ./tri
	@echo "Target eg200b:  runs tri on randomly generated obstacles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./tri $(interactive) 150 260 0 20 20 0 \
		10 5 input1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# eg300: 300 random triangles
#
eg300: ./tri
	@echo "Target eg300:  runs tri amidst 300 randomly generated triangles"
	@echo "--  This may take a noticeable amount bit of time!!! "
	./tri $(interactive) 10 400 0 350 20 0\
		3 3 input300.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&


moretest: $(TARGETS)
# nothing to do

tri: $(OBJ_FILES) tri.o


#=================================================
# Experiments for our paper 
#=================================================
#bugtrap: radius 40, other setting same as buga
#200: alpha 130, 460, beta 20, 20, other setting same as eg200
#300: alpha 230, 210, beta 480, 10, other setting unchanged
#150: radius 7, epsilon 1, other setting unchanged

ex-all: ex-bug ex-150 ex-200 ex-300

# Bugtrap example 
ex-bug experiment-bug: ./tri
	@echo "Target 'ex-bug':  runs tri for bugtrap input, bugtrap.txt"
	./tri 1 200 350 0 30 30 0 \
		10 40 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# ex-150: 150 random triangles (use radius=1)
ex-150: ./tri
	@echo "Target ex-150:  runs tri amidst 150 randomly generated triangles"
	./tri 1 200 270 0 20 20 0\
		1 7 input150.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# ex-200: 200 random triangles
ex-200: ./tri
	@echo "Target 2x-200:  runs tri on randomly generated obstacles"
	./tri 1 130 460 0 20 20 0 \
		5 2 input1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&
	#
# ex-300: 300 random triangles
#
ex-300: ./tri
	@echo "Target ex-300:  runs tri amidst 300 randomly generated triangles"
	./tri 1 230 210 0 480 10 0\
		3 3 input300.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&


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

