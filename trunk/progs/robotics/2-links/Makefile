# Makefile for progs/robotics/2-links
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
#			$(verbose) $(title) \
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
# Author: Zhongdi Luo, May 2013
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
startTheta1 = -30	# in degrees
startTheta2 = 120	# in degrees
goalX = 300		# goal configuration
goalY = 60
goalTheta1 = 90		# in degrees
goalTheta2 = 30		# in degrees

epsilon = 10		# resolution parameter
epsilon = 5		# resolution parameter

len1 = 10		# link1 length
len2 = 15		# link2 length

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
			#  (0=random, 1 = BFS, 2 = Greedy Best First, 3 = A-star) 
seed = 11		# seed for random
step = 0		# number of steps to run
			#	(step=0 means run to completion)
xtrans = 0		# x-translation of input file
ytrans = 0		# y-translation of input file
scale = 1		# scaling of input file

verbose = 0		# if true, display statistics
title = Demos		# Title of control panel

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = 2-links

OBJ_FILES = Box.o QuadTree.o Corner.o Wall.o fileProcessor.o Timer.o Polygon.o

all: $(TARGETS)

# note: this target is the standard target that Core Library uses
#       to test its subdirectories.   So the program must run in a
#       non-interactive mode (i.e., the first argument to "2-links" is "1").
#
test: $(TARGETS)

testa: $(TARGETS)
	./2-links 1

testb: $(TARGETS)
	./2-links 1 \
    $(startX) $(startY)  $(startTheta1) $(startTheta2) \
    $(goalX) $(goalY) $(goalTheta1) $(goalTheta2) \
    $(epsilon)  $(len1) $(len2) input2.txt  $(boxWidth) $(boxHt) \
    $(windowPosX) $(windowPosY) $(Qtype) $(seed) $(inputDir) \
    $(xtrans) $(ytrans) $(scale) \
    $(verbose) $(title) \

help:
	@echo "USAGE:  The main program is called 2-links.  Demos can be invoked thus:"
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
	@echo "        eg5 (= egbug2, bug2), eg5a (= egbug2a, bug2a),"
	@echo "        eg6 (= kavraki), eg6a (= kavrakia),"
	@echo "        eg7 (= tunnel), eg7a (= tunnela), eg7b (= tunnelb)"
	@echo "        eg100, eg100a"
	@echo "        eg150"
	@echo "        eg200, eg200a, eg200b"
	@echo "        eg300"


#=================================================
# Examples of running 2-links program
#=================================================
#
# this is default, but every option may be changed at command line:
#
eg:	./2-links
	@echo "Target 'eg':  runs 2-links with the default options of Makefile"
	@echo "--  You can all of them in you 'make' command line, e.g.,"
	@echo "--      > make eg radius=60 goalX=111 goalY=450 goalTheta=90 startY=200 startX=20"
	./2-links $(interactive) $(startX) $(startY)  $(startTheta) \
	    	$(goalX) $(goalY) $(goalTheta) \
		$(epsilon) $(radius) $(fileName) \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&

# run with default compiled options:
#	The input file is input2.txt.
#	There are 3 combinatorially different paths.
#	Randomized strategy will find all three of them (keep hitting "run").
#
ega:	./2-links
	@echo "Target 'ega':  runs 2-links with default compiled options"
	./2-links &

# run with default Makefile options:
#
egb:	./2-links
	@echo "Target 'egb':  runs 2-links with default Makefile options for file input.txt"
	./2-links $(interactive) 60 350 -30 300 60 60 \
		5 40 input.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&

# Simple NO PATH example:
nopath eg0:	./2-links
	@echo "Target 'eg0':  runs 2-links with default options for file input0.txt"
	@echo "--  this example has no path..."
	./2-links $(interactive) 400 400 -60 200 200 90\
		20 20 input0.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&

# eg1:	input2e.txt is like input2, but there is a bounding box
#eg1: ./2-links
#	@echo "Target eg1:  runs 2-links on input2e.txt (which has a bounding box)"
#	./2-links $(interactive) 150 260 90 40 40 -60 \
#		10 25 input2e.txt \
#		512 512 $(windowPosX) $(windowPosY) \
#		1 1111 $(inputDir) \
#		$(xtrans) $(ytrans) $(scale) \
#		$(verbose) $(title) \
#		&

# eg2: input 2.text, no bounding box
#eg2: ./2-links
#	@echo "Target eg2e:  runs 2-links on input2.txt"
#	./2-links $(interactive) 150 260 -45 20 20 30 \
#		10 40 input2.txt \
#		512 512 $(windowPosX) $(windowPosY) \
#		1 1111 $(inputDir) \
#		$(xtrans) $(ytrans) $(scale) \
#		$(verbose) $(title) \
#		&

# eg3: input3.txt is line input2e, but there is a narrow door
#eg3: ./2-links
#	@echo "Target eg3:  runs 2-links on input3.txt"
#	./2-links $(interactive) 150 260 90 450 450 -60 \
#		10 30 input3.txt \
#		512 512 $(windowPosX) $(windowPosY) \
#		1 1111 $(inputDir) \
#		$(xtrans) $(ytrans) $(scale) \
#		$(verbose) $(title) \
#		&

# BUG TRAPS:
# 
# Bugtrap example -- Existence of path, but barely!
eg4 egbug bug: ./2-links
	@echo "Target 'egbug':  runs 2-links for bugtrap input, bugtrap.txt"
	@echo "--  this examples shows there is NO PATH, but barely so!"
	@echo "--  you could see a green path that must pass between two green boxes"
	@echo "--  that are NOT adjacent but shares a corner."
	./2-links $(interactive) 200 350 30 60 30 30 90 120 \
		8 14 10 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&

# Bugtrap example -- Non-existence of path, but barely!
eg4a egbuga buga: ./2-links
	@echo "Target 'eg4a or buga':  runs 2-links for bugtrap input, bugtrap.txt"
	@echo "--  this examples shows there is a PATH, but barely so!"
	@echo "--  the only change from target 'egbug' is that epsilon is now 10, not 20"
	@echo "--  that are NOT adjacent but shares a corner."
	./2-links $(interactive) 200 350 0 30 30 0 \
		9 14 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&


# Bugtrap2 example -- Existance of path.
# 	NEW DATA: Eps=4, radius=11:
# 		with Greedy, takes 0.8 sec to find PATH 
# 		with BFS, takes 2.7 ms to report PATH 
# 		with Random, program hung up.
# 	OLD DATA: Eps=1, radius=7:
# 		with Greedy, takes 7.4 ms to find PATH 
# 		with BFS, takes 3.6 ms to report PATH 
eg5 egbug2 bug2: ./2-links
	@echo "Target 'eg5 or bug2':  runs 2-links on bugtrap2.txt"
	@echo "--  this examples shows there is PATH, but barely so!"
	./2-links $(interactive) 180 210 0 30 30 0 \
		4 11 bugtrap2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&

# Bugtrap2 example -- Non-Existence of path.
# 	NEW DATA: Eps=4, radius=12:
# 		with Greedy, takes 0.9 sec to report NO PATH 
# 		with BFS, process hung up...
# 	OLD DATA: Eps=1, radius=8:
# 		with Greedy, takes <1 ms to report NO PATH 
# 		with BFS, takes 15 ms to report NO PATH
eg5a egbug2a bug2a: ./2-links
	@echo "Target 'eg5a or bug2a':  runs 2-links on bugtrap2.txt"
	@echo "--  this examples shows there is NO PATH, but barely so!"
	./2-links $(interactive) 180 210 0 30 30 0 \
		4 12 bugtrap2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&

# eg6: This environment is taken from Kavraki's groups Demo for OOPSMP:
#  NOTE: we must do scaling of the input
#  	(because dimensions came from xfig)
eg6: ./2-links
	@echo "Target DemoSE2_PRM (from Kavraki's group)"
	./2-links $(interactive) 75 350 0 380 205 0\
		5 15 DemoSE2_PRM.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		250 250 0.04 \
		&

# eg6a:  Variant of eg6a where their "robot" is shown.
eg6a: ./2-links
	@echo "Target DemoSE2_PRM (from Kavraki's group) -- their robot is shown"
	@echo "	There is a narrow path... when radius=15, epsilon=2"
	@echo " 	There is No Path... when radius=15, epsilon=3"
	./2-links $(interactive) 75 350 0 380 205 0\
		2 15 DemoSE2_PRMa.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		250 250 0.04 \
		&

# eg7, egtunnel:  tunnel example from Salzman et al.
# 	NOTE that the current search strategy seems to miss the direct path!!
# 	TODO: develop a better search (e.g. Best First Search)
eg7 egtunnel tunnel: ./2-links
	./2-links $(interactive) 200 280 0 450 300 0\
		10 15 tunnel.poly \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		100 200 0.05

# eg7a or tunnela:  Variant of tunnel.  We introduce a confining box to force 
# 	the robot to go through the tunnel.  This one is reasonably fast
# 	(radius=10, epsilon=10).  Path is found.  See tunnelXa for a much harder instance.
eg7a egtunnela tunnela: ./2-links
	./2-links $(interactive) 200 280 0 450 300 0\
		10 10 tunnelx.poly \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		100 200 0.05

# eg7b or tunnelb:  Variant of tunnel.  We introduce a confining box to force 
# 	the robot to go through the tunnel.  
#	CAREFUL, THIS DEMO (with epsilon=10, radius=15)
#	TAKES A LONG TIME BUT WILL EVENTUALLY FIND A PATH (takes about 25 minutes)
#
	#	./2-links 0		 200 280 0 450 300 0\
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
eg7b tunnelb: ./2-links
	./2-links $(interactive) 200 280 0 450 300 0\
		10 15 tunnelx.poly \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		100 200 0.05

# eg8: from Paper of Hsu-Lathombe-Kurniawati
eg8 hsu: ./2-links
	@echo "Target eg8: example from the paper of Hsu, Latombe, Kurniawati (2006)"
	@echo "	There is a path passage... when radius=6, epsilon=1"
	@echo " 	There is No Path... when radius=7, epsilon=1"
	./2-links $(interactive) 10 490 0 270 495 180 270 0\
		8 40 60 input200.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&

eg8a hsua: ./2-links
	@echo "Target eg8a: example from the paper of Hsu, Latombe, Kurniawati (2006)"
	@echo "	There is a path passage... when radius=6, epsilon=1"
	@echo " 	There is No Path... when radius=7, epsilon=1"
	./2-links $(interactive) 75 350 0 380 205 0\
		1 7 hsu-latombe-kurniawati.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		-70 -70 0.065 \
		&

eg8b hsub: ./2-links
	@echo "Target eg8b: example from the paper of Hsu, Latombe, Kurniawati (2006)"
	@echo "	There is a path passage... when radius=6, epsilon=1"
	@echo " 	There is No Path... when radius=7, epsilon=1"
	./2-links $(interactive) 75 350 0 90 380 205 270 0\
		8 5 10 hsu-latombe-kurniawati2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		-70 -70 0.065 \
		&


eg9 narrow: ./2-links
	@echo "Target eg9: 1st test example from Chee and Zhongdi"
#	@echo "	There is a path passage... when radius=6, epsilon=1"
#	@echo " 	There is No Path... when radius=7, epsilon=1"
	./2-links $(interactive) 195 370 0 90 195 160 270 0\
		4 25 40 narrowCorridor.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		0 0 0.065 \
		&
		
eg10 twoChambers: ./2-links
	@echo "Target eg10: two Chamber environment using BFS"
#	@echo "	There is a path passage... when radius=6, epsilon=1"
#	@echo " 	There is No Path... when radius=7, epsilon=1"
	./2-links $(interactive) 415 400 180 270 105 90 90 0\
		4 65 80 2chambers.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 $(seed)  $(inputDir) \
		-220 0 0.065 \
		&

eg10a twoChambers: ./2-links
	@echo "Target eg10: two Chamber environment using Random Search"
#	@echo "	There is a path passage... when radius=6, epsilon=1"
#	@echo " 	There is No Path... when radius=7, epsilon=1"
	./2-links $(interactive) 415 400 180 270 105 90 90 0\
		4 8 10 2chambers.txt \
		512 512 $(windowPosX) $(windowPosY) \
		0 $(seed)  $(inputDir) \
		-220 0 0.065 \
		&

eg11 nestedRectangle: ./2-links
	@echo "Target eg11: 3nd test example from Chee and Zhongdi"
#	@echo "	There is a path passage... when radius=6, epsilon=1"
#	@echo " 	There is No Path... when radius=7, epsilon=1"
	./2-links $(interactive) 425 390 180 270 95 100 0 90\
		2 60 40 nestedRectangle.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		-220 0 0.065 \
		&

basic1 eg1 : ./2-links
	@echo "Target basic1: basic test 1 from Zhongdi"
#	@echo "	There is a path passage... when radius=6, epsilon=1"
#	@echo " 	There is No Path... when radius=7, epsilon=1"
	./2-links $(interactive) 195 370 0 150 195 160 270 0\
		32 50 80 basic1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		0 0 1 \
		&
		
basic2 eg2: ./2-links
	@echo "Target basic1: basic test 2 from Zhongdi"
#	@echo "	There is a path passage... when radius=6, epsilon=1"
#	@echo " 	There is No Path... when radius=7, epsilon=1"
	./2-links $(interactive) 195 370 0 150 195 160 270 0\
		32 50 80 basic2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		0 0 1 \
		&
		
basic3 eg3: ./2-links
	@echo "Target basic3: basic test 2 from Zhongdi"
#	@echo "	There is a path passage... when radius=6, epsilon=1"
#	@echo " 	There is No Path... when radius=7, epsilon=1"
	./2-links $(interactive) 195 370 0 150 345 180 270 0\
		8 50 80 basic3.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		0 0 1 \
		&

# RANDOMLY GENERATED ENVIRONMENT:
#	This one, generated using our Python Generator, has 200 2-links.
#	Note that these 2-links are not disjoint, but our theory remains
#	valid for such inputs.  This is the power of the SSS approach (over
#	the exact algorithms.

# eg100 100: 100 random 2-links
eg100 100: ./2-links
	@echo "Target eg100:  runs 2-links amidst 100 randomly generated 2-links"
	./2-links $(interactive) 10 490 0 270 495 180 270 0\
		8 40 60 input100.txt \
		512 512 $(windowPosX) $(windowPosY) \
		3 $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&

# eg100a: 100 random 2-links
eg100a 100a: ./2-links
	@echo "Target eg100a:  runs 2-links amidst 100 randomly generated 2-links"
	@echo "This one is a bit harder than eg100 (with radius 8 instead of 5)"
	./2-links $(interactive) 200 270 0 20 20 0\
		10 8 input100.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&

# eg150: 150 random 2-links (use radius=1)
eg150 150: ./2-links
	@echo "Target eg150:  runs 2-links amidst 100 randomly generated 2-links"
	./2-links $(interactive) 10 490 0 270 495 180 270 0\
		8 40 60 input150.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&

# eg200: 200 random 2-links
eg200 200: ./2-links
	@echo "Target eg200:  runs 2-links on randomly generated obstacles"
	@echo "--  This may take a noticeable amount bit of time!!!"
	./2-links $(interactive) 10 490 0 270 495 180 270 0\
		8 40 60 input200.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&

# eg200a: there is an obvious path but probabilistic methods might waste
# 	lots of time checking elsewhere!
#
eg200a 200a: ./2-links
	@echo "Target eg200a:  runs 2-links on randomly generated obstacles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./2-links $(interactive) 10 360 0 20 20 0 \
		2 10 input1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&

# eg200b: there is no path because eps=10
#
eg200b: ./2-links
	@echo "Target eg200b:  runs 2-links on randomly generated obstacles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./2-links $(interactive) 150 260 0 20 20 0 \
		10 5 input1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&

# eg300: 300 random 2-links
#
eg300: ./2-links
	@echo "Target eg300:  runs 2-links amidst 300 randomly generated 2-links"
	@echo "--  This may take a noticeable amount bit of time!!! "
	./2-links $(interactive) 10 400 0 350 20 0\
		3 3 input300.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&


moretest: $(TARGETS)
# nothing to do

2-links: $(OBJ_FILES) 2-links.o


# under development...
# (1) Run all series of examples using Qtypes=0,1,2 and compare timing


#=================================================
# Experiments for our paper 
#=================================================
#bugtrap: radius 40, other setting same as buga
#200: alpha 130, 460, beta 20, 20, other setting same as eg200
#300: alpha 230, 210, beta 480, 10, other setting unchanged
#150: radius 7, epsilon 1, other setting unchanged

ex-all: ex-bug ex-150 ex-200 ex-300

# Bugtrap example 
ex-bug experiment-bug: ./2-links
	@echo "Target 'ex-bug':  runs 2-links for bugtrap input, bugtrap.txt"
	./2-links 1 200 350 0 30 30 0 \
		17 40 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&

# Bugtrap example 
ex-buga: ./2-links
	@echo "Target 'ex-bug':  runs 2-links for bugtrap input, bugtrap.txt"
	./2-links 1 200 350 0 30 30 0 \
		1 15 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&

# ex-150: 150 random 2-links (use radius=1)
ex-150: ./2-links
	@echo "Target ex-150:  runs 2-links amidst 150 randomly generated 2-links"
	./2-links 1 200 270 0 20 20 0\
		1 7 input150.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&

# ex-200: 200 random 2-links
ex-200: ./2-links
	@echo "Target 2x-200:  runs 2-links on randomly generated obstacles"
	./2-links 0 130 460 0 20 20 0 \
		5 2 input1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&
	#
# ex-300: 300 random 2-links
#
ex-300: ./2-links
	@echo "Target ex-300:  runs 2-links amidst 300 randomly generated 2-links"
	./2-links 0 230 210 0 480 10 0\
		3 3 input300.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&


#=================================================
# Temp
#=================================================
m:
	gvim Makefile
vi:
	gvim 2-links.cpp

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

