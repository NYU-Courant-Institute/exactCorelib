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
#			$(xtrans) $(ytrans) $(scale) \
#			$(verbose) $(title) $(colonOn) \
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
#		eg8 (= hsu), eg8a (= hsua), eg8b (= hsub)
#		eg10 (= 2chambers)
#		eg100, eg100a
#		eg150
#		eg200, eg200a, eg200b
#		eg300
#
# 	These targets uses different input files, all taken from
# 	the subdirectory "inputs".   
#
#  Disc Demos for SoCG 2013 Talk in Rio:
#  	demo1	= bug:
#  	demo2	= bug2:
#  	demo3	= eg100:
#  			  (Random	14500 ms)
#  			  (BFS 		4800 ms)
#  			  (Greedy	700 ms)
#  			  (Dist+Size	840 ms)
#  			  (Vor 		1540 ms)
#  		  Now, increase the radius by 1 (no path)
#  			  (Random	5200 ms)
#  			  (BFS 		4600 ms)
#  			  (Greedy	460 ms)
#  			  (Dist+Size	540 ms)
#  			  (Vor 		530 ms)
#  	demo4	= eg200  (rad,eps)=(8,4) -- path
#  			  (Random	?? ms)
#  			  (BFS 		6300 ms)
#  			  (Greedy	3100 ms)
#  			  (Dist+Size	2800 ms)
#  			  (Vor 		4800 ms)
#  		  eg200  (rad,eps)=(8,5) -- NO PATH
#  			  (Random	4350 ms)
#  			  (BFS 		3900 ms)
#  			  (Greedy	430 ms)
#  			  (Dist+Size	440  ms)
#  			  (Vor 		460 ms)
#  Triangle Demos for RCV 2013 Talk in Berlin:
#  	demo1	= bug2:
#  			Fastest is Dist+Size (930 ms)
#  			Next is Boundary Tracker (6680 ms)
#  			Last is Greedy (26900 ms)
#  	demo2 	= 100 triangles,
#  			Fastest is Greedy (250 ms)
#  			Next is Boundary Tracker (780 ms)
#  			Last is Dist+Size (3840 ms)
#
#
# April 13, 2012
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
startTheta = -30	# in degrees
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

Qtype = 2		# type of priority queue
			#  (0=random, 1 = BFS, 2 = Greedy Best First, 3 = Dist+Size) 
			#  (4=Vor heuristic)
seed = 11		# seed for random
step = 0		# number of steps to run
			#	(step=0 means run to completion)
xtrans = 0		# x-translation of input file
ytrans = 0		# y-translation of input file
scale = 1		# scaling of input file

verbose = 0		# if 1 or true, display statistics
title = Demos		# Title of control panel
colorOn = 1		# if 1 or true, show color

tri = triangle		# name of executable program
tri = tri		# name of executable program

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = $(tri)

OBJ_FILES = Box.o QuadTree.o Corner.o Wall.o fileProcessor.o Timer.o

all: $(TARGETS)

# note: this target is the standard target that Core Library uses
#       to test its subdirectories.   So the program must run in a
#       non-interactive mode (i.e., the first argument to "tri" is "1").
#
test: $(TARGETS)

testa: $(TARGETS)
	./$(tri) 1

testb: $(TARGETS)
	./$(tri) 1 \
		$(startX) $(startY)  $(startTheta) $(goalX) $(goalY) $(startBeta) \
		$(epsilon)  $(radius)  input2.txt  $(boxWidth) $(boxHt) \
       		$(windowPosX) $(windowPosY) $(Qtype) $(seed) $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \

help:
	@echo "USAGE:  The main program is called tri.  Demos can be invoked thus:"
	@echo "              > make eg [...optional parameters...]"
	@echo "     where parameters include fileName, radius, startX, startY,"
	@echo "     goalX, goalY, etc.  See the Makefile for examples."
	@echo "     You can also call demos where these these parameters are prechosen:"
	@echo "              > make egX"
	@echo "     where X is replaced by 1, 2, 3, 4, 5, 6, 100, 150, 200, 300, etc."
	@echo "     Some demos have variants (such as egXa, egXb or egXc)"
	@echo "     or alternative names (such as bug, bug2, tunnal, etc): "
	@echo "        ega, egb,"
	@echo "        eg1, eg1a, eg1b, [simple environment with bounding box]"
	@echo "        eg2, eg2a, eg2b, [simple environment without bounding box]"
	@echo "        eg3  [simple environment with door]"
	@echo "        eg4 (= egbug, bug), eg4a (= egbuga, buga),"
	@echo "        eg5 (= egbug2, bug2), eg5a (= egbug2a, bug2a), eg5b, eg5c"
	@echo "        eg6 (= kavraki), eg6a (= kavrakia)"
	@echo "        eg7 (= tunnel), eg7a (= tunnela), eg7b (= tunnelb)"
	@echo "        eg8 (= hsu), eg8a (= hsua), eg8b (=hsub) [Hsu-Lathombe-Kurniawati]"
	@echo "        eg9 (= corridor)"
	@echo "        eg100, eg100a"
	@echo "        eg150"
	@echo "        eg200, eg200a, eg200b"
	@echo "        eg300"
	@echo "========"
	@echo "	You may also try demoX (X=1,2,3) for a quick walk through" 

#=================================================
# Examples of running tri program
#=================================================
#
# this is default, but every option may be changed at command line:
#
eg:	./tri
	@echo "Target 'eg':  runs tri with the default options of Makefile"
	@echo "-- You can all of them in you 'make' command line, e.g.,"
	@echo "--   > make eg radius=60 goalX=111 goalY=450 goalTheta=90 startY=200 startX=20"
	./tri $(interactive) $(startX) $(startY)  $(startTheta) \
	    	$(goalX) $(goalY) $(goalTheta) \
		$(epsilon) $(radius) $(fileName) \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
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
	./tri $(interactive) 60 350 -30 300 60 60 \
		5 40 input.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# Simple NO PATH example:
nopath eg0:	./tri
	@echo "Target 'eg0':  runs tri with default options for file input0.txt"
	@echo "--  this example has no path..."
	./tri $(interactive) 400 400 -60 200 200 90\
		20 20 input0.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# eg1:	input2e.txt is like input2, but there is a bounding box
eg1: ./tri
	@echo "Target eg1:  runs tri on input2e.txt (which has a bounding box)"
	./tri $(interactive) 150 260 90 40 40 -60 \
		10 25 input2e.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# eg2: input 2.text, no bounding box
eg2: ./tri
	@echo "Target eg2e:  runs tri on input2.txt"
	./tri $(interactive) 150 260 -45 20 20 30 \
		10 40 input2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# eg3: input3.txt is line input2e, but there is a narrow door
eg3: ./tri
	@echo "Target eg3:  runs tri on input3.txt"
	./tri $(interactive) 150 260 90 450 450 -60 \
		10 30 input3.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# eg3a: input3a.txt is line input2e, but there is a narrow door
eg3a: ./tri
	@echo "Target eg3:  runs tri on input3.txt"
	./tri $(interactive) 150 260 90 450 450 -60 \
		10 30 input3a.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# BUG TRAPS:
# 
# Bugtrap example -- Existence of path, but barely!
eg4 egbug bug: ./tri
	@echo "Target 'egbug':  runs tri for bugtrap input, bugtrap.txt"
	@echo "--  this examples shows there is NO PATH, but barely so!"
	@echo "--  you could see a green path that must pass between two green boxes"
	@echo "--  that are NOT adjacent but shares a corner."
	./tri $(interactive) 200 350 0 30 30 90 \
		8 14 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# Bugtrap example -- Non-existence of path, but barely!
eg4a egbuga buga: ./tri
	@echo "Target 'eg4a or buga':  runs tri for bugtrap input, bugtrap.txt"
	@echo "--  this examples shows there is a PATH, but barely so!"
	@echo "--  the only change from target 'egbug' is that epsilon is now 10, not 20"
	@echo "--  that are NOT adjacent but shares a corner."
	./tri $(interactive) 200 350 0 30 30 0 \
		9 14 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# Bugtrap example -- Interesting because robot is about as big as can get through
# 	The Boundary Tracker is fastest heuristic (dispite its funny search)
# 	taking 780 ms on my MacAir.

eg4b egbugb bugb: ./tri
	@echo "Target 'eg4b or bugb':  runs tri for bugtrap input, bugtrap.txt"
	@echo "--  this example is fastest for Boundary Tracker Heuristic"
	@echo "--  the only change from target 'egbug' is that epsilon is now 10, not 20"
	@echo "--  that are NOT adjacent but shares a corner."
	./tri $(interactive) 200 350 0 30 30 0 \
		8 20 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		4 $(seed) $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# Bugtrap example -- Interesting because robot is about as big as can get through
# 	Moreover, the Boundary Tracker is fastest heuristic (dispite its funny search)


# Bugtrap2 example -- Existance of path.
# 	NEW DATA: Eps=4, radius=11 (has Path)
# 		Random=too long
# 		BFS= 3200 ms,
# 		Greedy=5360 ms, Dist+size= 2200* ms, Vor=5400 ms, 
eg5 egbug2 bug2: ./tri
	@echo "Target 'eg5 or bug2':  runs tri on bugtrap2.txt"
	@echo "--  this example shows there is PATH, but barely so!"
	./tri $(interactive) 180 210 90 30 30 -30 \
		4 11 bugtrap2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		3 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&
# same as eg5, but we reverse the role of start and finish position!
eg5a egbug2a bug2a: ./tri
	@echo "Target 'eg5a or bug2a':  runs tri on bugtrap2.txt"
	@echo "--  this example is like eg5, but exchanges the GOAL/START of eg5"
	./tri $(interactive) 30 30 -30  180 210 90 \
		4 11 bugtrap2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# Bugtrap2 example -- Non-Existence of path.
# 	NEW DATA: Eps=4, radius=12:
# 		with Greedy, takes 0.9 sec to report NO PATH 
# 		with BFS, process hung up...
# 	OLD DATA: Eps=1, radius=8:
# 		with Greedy, takes <1 ms to report NO PATH 
# 		with BFS, takes 15 ms to report NO PATH
eg5b egbug2b bug2b: ./tri
	@echo "Target 'eg5b or bug2b':  runs tri on bugtrap2.txt"
	@echo "--  this examples shows there is NO PATH, but barely so!"
	./tri $(interactive) 180 210 0 30 30 0 \
		4 12 bugtrap2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
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
		$(verbose) $(title) $(colorOn) \
		&

# eg6a:  Variant of eg6a where their "robot" is shown.
# 	 I THINK THIS EXAMPLE IS INSTRUCTIVE: too slow with Greedy or Boundary Tracker
# 	 MUST FIX THIS.  (If you change to Dist+Size heuristic, it is actually quite fast)
eg6a: ./tri
	@echo "Target DemoSE2_PRM (from Kavraki's group) -- their robot is shown"
	@echo "	 THIS EXAMPLE IS INSTRUCTIVE!"
	@echo "   TOO slow with Greedy Heuristic (6 s) or Boundary Tracker Heuristic!"
	@echo "	  But with Dist+Size Heuristic, it is fast (10 ms)!"
	./tri $(interactive) 75 350 0 380 205 0\
		2 15 DemoSE2_PRMa.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		3 $(seed)  $(inputDir) \
		250 250 0.04 \
		$(verbose) $(title) $(colorOn) \
		&

# eg7, egtunnel:  tunnel example from Salzman et al.
# 	NOTE that the current search strategy seems to miss the direct path!!
# 	TODO: develop a better search (e.g. Best First Search)
eg7 egtunnel tunnel: ./tri
	@echo "Target tunnel (from paper of Salzman et al.)"
	@echo "Very fast with boundary tracker heuristic"
	./tri $(interactive) 200 280 0 450 300 0\
		10 15 tunnel.poly \
		512 512 $(windowPosX) $(windowPosY) \
		4 1111 $(inputDir) \
		100 200 0.05 \
		$(verbose) $(title) 0 \
		&

# eg7a or tunnela:  Variant of tunnel.  We introduce a confining box to force 
# 	the robot to go through the tunnel.  This one is reasonably fast
# 	(radius=10, epsilon=10).  Path is found.  See tunnelXa for a much harder instance.
# 	TODO: Bug (penetrate obstacle?) (Chee March 2014)
eg7a egtunnela tunnela: ./tri
	./tri $(interactive) 200 280 0 450 300 0\
		10 10 tunnelx.poly \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		100 200 0.05 \
		$(verbose) $(title) $(colorOn) \
		&

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
		100 200 0.05 \
		$(verbose) $(title) $(colorOn) \
		&

# eg8: from Paper of Hsu-Lathombe-Kurniawati
eg8 hsu: ./tri
	@echo "Target eg8: example from the paper of Hsu, Latombe, Kurniawati (2006)"
	@echo "	There is a path passage... when radius=6, epsilon=1"
	@echo " 	There is No Path... when radius=7, epsilon=1"
	./tri $(interactive) 75 350 0 380 205 0\
		1 5 hsu-latombe-kurniawati.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		-70 -70 0.065 \
		$(verbose) $(title) $(colorOn) \
		&

eg8a hsua: ./tri
	@echo "Target eg8a: example from the paper of Hsu, Latombe, Kurniawati (2006)"
	@echo "	There is a path passage... when radius=6, epsilon=1"
	@echo " 	There is No Path... when radius=7, epsilon=1"
	./tri $(interactive) 75 350 0 380 205 0\
		1 7 hsu-latombe-kurniawati.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		-70 -70 0.065 \
		$(verbose) $(title) $(colorOn) \
		&

eg8b hsub: ./tri
	@echo "Target eg8b: example from the paper of Hsu, Latombe, Kurniawati (2006)"
	@echo "	There is a path passage... when radius=6, epsilon=1"
	@echo " 	There is No Path... when radius=7, epsilon=1"
	./tri $(interactive) 75 350 0 380 205 0\
		1 5 hsu-latombe-kurniawati2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		-70 -70 0.065 \
		$(verbose) $(title) $(colorOn) \
		&
# eg9: narrow corridor
eg9: ./tri
	@echo "Target eg9: narror corridor"
	./tri $(interactive) 250 370 -60 220 150 30\
		5 30 narrowCorridor.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		0 0 0.065 \
		$(verbose) $(title) $(colorOn) \
		&

# eg10: 2-chamber
eg10 2chamber: ./tri
	@echo "Target eg10: example using 2-chambers"
	@echo "	There is a path passage... when radius=6, epsilon=1"
	@echo " 	There is No Path... when radius=7, epsilon=1"
	./tri $(interactive) 415 400 -60 120 100 30\
		5 22 2chambers.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		-220 0 .065 \ #$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# eg11: maze
eg11 maze: ./tri
	@echo "Target eg11: maze"
	./tri $(interactive) 385 400 -30 105 60 30\
		5 10 input4.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		0 0 1 \ #$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

eg20: ./tri
	@echo "Target eg20: example using 2-chambers"
	@echo "	There is a path passage... when radius=6, epsilon=1"
	@echo " 	There is No Path... when radius=7, epsilon=1"
	./tri $(interactive) 415 400 180 105 90 90 \
		4 22 2chambers.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		-220 0 .065 \ #$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# RANDOMLY GENERATED ENVIRONMENT:
#	This one, generated using our Python Generator, has 200 triangles.
#	Note that these triangles are not disjoint, but our theory remains
#	valid for such inputs.  This is the power of the SSS approach (over
#	the exact algorithms.

# eg100 100: 100 random triangles
# 	Random = ? ms, BFS=14500 ms, Greedy = 950 ms, Dist+Size = 16000 ms, Vor = 2900 ms
eg100 100: ./tri
	@echo "Target eg100:  runs tri amidst 100 randomly generated triangles"
	./tri $(interactive) 20 20 45 200 270 0 \
		3 20 input100.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# eg100a: 100 random triangles
eg100a 100a: ./tri
	@echo "Target eg100a:  runs tri amidst 100 randomly generated triangles"
	@echo "This one is a bit harder than eg100 (with radius 8 instead of 5)"
	./tri $(interactive) 20 20 45 310 310 -45\
		5 28 input100.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# eg150: 150 random triangles (use radius=15)
eg150 150: ./tri
	@echo "Target eg150:  runs tri amidst 100 randomly generated triangles"
	./tri $(interactive) 200 320 0 20 20 0\
		3 18 input150.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# eg200: 200 random triangles
# 	Parameters: (rad,eps)=(8,4)
# 			Time:	Random= slow! ms, BFS=6300 ms, Greedy=3100 ms,
# 				Dist+Size 2800 ms, Vor 4800 ms
eg200 200: ./tri
	@echo "Target eg200:  runs tri on randomly generated obstacles"
	@echo "--  This may take a noticeable amount bit of time!!!"
	@echo "--  Using Greedy Strategy"
	./tri $(interactive) 150 260 0 20 20 90 \
		4 8 input1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# eg200a: there is an obvious path but probabilistic methods might waste
# 	lots of time checking elsewhere!
# 	Using Qtype=4 (Vor Heuristic)
eg200a 200a: ./tri
	@echo "Target eg200a:  runs tri on randomly generated obstacles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	@echo "--  Using Vor Heuristic"
	./tri $(interactive) 10 360 0 320 20 90 \
		5 7 input1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		4 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
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
		$(verbose) $(title) $(colorOn) \
		&

# eg300: 300 random triangles
#
eg300: ./tri
	@echo "Target eg300:  runs tri amidst 300 randomly generated triangles"
	@echo "--  This may take a noticeable amount bit of time!!! "
	./tri $(interactive) 10 400 0 350 20 0\
		2 3 input300.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&


moretest: $(TARGETS)
# nothing to do

tri: $(OBJ_FILES) tri.o


# under development...
# (1) Run all series of examples using Qtypes=0,1,2 and compare timing


#=================================================
# Experiments for our SoCG 2013 paper 
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
		17 40 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# Bugtrap example 
ex-buga: ./tri
	@echo "Target 'ex-bug':  runs tri for bugtrap input, bugtrap.txt"
	./tri 1 200 350 0 30 30 0 \
		1 15 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# ex-150: 150 random triangles (use radius=1)
ex-150: ./tri
	@echo "Target ex-150:  runs tri amidst 150 randomly generated triangles"
	./tri 1 200 270 0 20 20 0\
		1 7 input150.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# ex-200: 200 random triangles
ex-200: ./tri
	@echo "Target 2x-200:  runs tri on randomly generated obstacles"
	./tri 0 130 460 0 20 20 0 \
		5 2 input1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&
	#
# ex-300: 300 random triangles
#
ex-300: ./tri
	@echo "Target ex-300:  runs tri amidst 300 randomly generated triangles"
	./tri 0 230 210 0 480 10 0\
		3 3 input300.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

#=================================================
# Demos (from Talk at RCV 2013 in Berlin RSS Conference)
#=================================================
# Demo1 is same as "bug2"  or "eg5"
# Arguments:
#	./tri $(interactive) 180 210 90 30 30 -30 \
#		5 11 bugtrap2.txt \
#		512 512 $(windowPosX) $(windowPosY) \
#		3 11 $(inputDir) \
#		$(xtrans) $(ytrans) $(scale) \
#		$(verbose) $(title) \
#		&
# Using Dist+Size Strategy with radius 11, eps=5, this takes less than 800 ms.
# 	Boundary tracker is a bit slower (1230 ms), Greedy takes 2670 ms.
# 	Random tend to take over 3000 ms.
#
demo1: ./tri
	@echo "Target 'eg5 or bug2':  runs tri on bugtrap2.txt"
	@echo "--  this example shows there is PATH, but barely so!"
	./tri $(interactive) 180 210 90 30 30 -30 \
		5 11 bugtrap2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		3 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

# Demo2 is same as eg100 (the greedy strategy is fastest, but try
# 	also Dist+size, Boundary tracker strategy)
# Using Radius 20, eps=3, this takes less than 250 ms with Greedy strategy.
# 	Boundary tracker is slightly slower at 850 ms.
# 	Dist+Size strategy is a 40 times slower (10000 ms),
#
demo2: ./tri
	@echo "Target eg100:  runs tri amidst 100 randomly generated triangles"
	./tri $(interactive) 20 20 45 200 270 0 \
		3 20 input100.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) $(colorOn) \
		&

#=================================================
# End of Demos (from Talk at RCV 2013 in Berlin RSS Conference)
#=================================================



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

