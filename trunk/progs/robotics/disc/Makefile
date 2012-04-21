# Makefile for progs/robotics/2d-disc
#
# April 13, 2012
#
# Since Core2
# $Id:$

CORE_PATH=../../..
include ../Make.options

#=================================================
# Original file
#=================================================
# Box.cpp
# Corner.cpp
# QuadTree.cpp
# Wall.cpp
# disc.cpp
# 
# CC = g++ -I/usr/include/opengl -g -Wall -ansi
# 
# all:
# 	${CC} *.cpp  -lglui -lglut32 -lglu32 -lopengl32  -o output.exe
# 
# clean:
# 	rm *.exe
#=================================================
# User definition (put parameters manually here)
#=================================================
interactive = 0		# 0=interactive, >0 is non-interactive

startX = 10		# start configuration
startY = 350

goalX = 500		# goal configuration
goalY = 20

epsilon = 2		# resolution parameter
radius = 30		# robot radius

fileName = bug0.txt	# input environment file
fileName = input2.txt	# input environment file
fileName = input2c.txt	# input environment file

boxWidth = 512		# initial configuration box size
boxHt = 512

windowPosX = 200	# initial Window position
windowPosY = 200	

Qtype = 0		# type of priority queue
			#	(1 = BFS, 0 = random) // funny switch...
seed = 1111		# seed for random
		
#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = disc

OBJ_FILES = Box.o QuadTree.o Corner.o Wall.o 

all: $(TARGETS)

test: $(TARGETS)
	./disc 1 \
		$(startX) $(startY)  $(goalX) $(goalY) \
		$(epsilon)  $(radius)  input2.txt  $(boxWidth) $(boxHt) \
       		$(windowPosX) $(windowPosY) $(Qtype) $(seed) 

avoidWarning: avoidWarning.o

#=================================================
# Examples of running disc program
#=================================================

eg:	disc
	./disc $(interactive) $(startX) $(startY)  $(goalX) $(goalY) \
		$(epsilon) $(radius) $(fileName) \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed) 

eg1: disc
	./disc $(interactive) 10 360 20 20 \
		$(epsilon) 10 input1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111


eg2: disc
	./disc 

moretest: $(TARGETS)
# nothing to do

disc: $(OBJ_FILES) disc.o


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

