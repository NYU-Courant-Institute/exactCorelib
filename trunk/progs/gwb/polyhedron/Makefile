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
# 	       > make eg		-- compiles and runs "gwb" interactively
# 	       > make egx rad1=30	-- like "eg" but you can change parameters
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
#include ../Make.options


#=================================================
# User variables (you can change them in the command line)
#=================================================
interactive = 0		# 0=interactive, >0 is non-interactive

inputDir = inputs	# where to find input files
fileName = cube.gwb	# cube
fileName = trefoil.gwb	# trefoil
fileName = tetrahedron.gwb	# tetrahedron

windowWidth = 512
windowHeight = 512 
windowPosX = 200	# initial Window position
windowPosY = 200	
windowPosX = 450	# initial Window position
windowPosY = 20	

mode = face		# show faces of surfaces
mode = wire		# show wire frame of surfaces

model = cube		# name of model to be displayed
model = bullet
model = trefoil

rad = 40		# radius
ht = 10			# height
hor = 30		# horizontal dim
ver = 20		# vertical dim

ht2=40  		# height 2: second part of the model

#alias of the variables
radius=$(rad)
height=$(ht)
nsegments=$(hor)
nsections=$(ver)
height2=$(ht2)

nsegments2=5		# nsegment2: how many segments in the second part
nsections2=1		# nsections2: how many sections in the second part
nparts=10		
nparts2=20

position2=80		# the position of the second part of the polyhedron: Like the distance from the leg to the center
position3=90		# the potition of the third part of the polyhedron: Like how many
thickness3=10
height3=50


rotUnitX=5
rotUnitY=5
rotUnitZ=5

lightRate=0.55
# trefoil
rad1 = 40		# thickness of cross-section of knot
rad2 = 10		# diameter of overall trefoil knotf
nsections = 50		# number of cross-sections in trefoil 
nsegments = 4		# number of segments in each cross-section


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
CXXFLAGS += -MMD -lGL -lGLU -lglui -lglut

all: $(TARGETS)

# note: this target is the standard target that Core Library uses
#       to test its subdirectories.   So the program must run in a
#       non-interactive mode !!!
#
test: $(TARGETS)
	./gwb 0

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
eg:	trefoil	

#eg default (does not work)
egx:	gwb	
	@echo "Target 'eg':  runs gwb with the default options of Makefile"
	@echo "--  You can all of them in you 'make' command line, e.g.,"
	@echo "--      > make eg fileName=cube.txt"
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
	    	$(mode) $(model) \
	    	$(rad1) $(rad2) $(nsegment) $(nsections) \

#eg0 
eg0 facehemi:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
	    	face hemisphere 200 $(height) 20 20 $(ht2) $(rad2) $(rotUnitX) $(rotUnitY) $(rotUnitZ) \
		$(nsegments2) $(nsections2) $(nparts) $(nparts2) $(position2) $(position3) $(thickness3) $(height3) 0.55
eg0x:	gwb
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
	    	$(mode) hemisphere $(rad1) $(ht) $(nsegments) $(nsections) $(ht2) $(rad2) $(rotUnitX) $(rotUnitY) $(rotUnitZ) \
		$(nsegments2) $(nsections2) $(nparts) $(nparts2) $(position2) $(position3) $(thickness3) $(height3) $(lightRate)

#eg1 sphere
eg1 sphere:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
	    	$(mode) sphere 200 $(height) 20 20

#eg2 torus
eg2 donut torus :	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
	    	$(mode) torus 200 $(height) 20 20 $(height2) 50

eg2x torusx:
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
	    	$(mode) torus $(rad) $(ht) $(hor) $(ver)

#eg3 cylinder
eg3 cylinder:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) cylinder 6 20 20 20

eg3x cylinderx:
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) cylinder $(rad) $(ht) $(hor) $(ver)

#eg4 disk 
eg4 disk:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) disk 10 5 20 20

#eg5 bullet 	(40 50 20 20)
eg5 bullet:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) bullet 40 50 30 20

eg5x bulletx:
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) bullet $(rad) $(ht) $(hor) $(ver)

#eg6 cone 
eg6 cone:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) cone 40 70 30 20

#eg7 drum 
eg7 drum:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) drum $(rad) $(ht) $(hor) $(ver)

# THIS IS BADLY CONFUSED!
#eg8 table 	radius, height(thickness), legPos, legNo, legRad, legHor, legHeight
#eg8 table 	radius, height(thickness), nseg,   nsect,   ht2, rad2
eg8 table:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) table 80 10 70 100 100 10

eg8x tablex:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) table $(rad) $(ht) $(hor) $(ver)

#eg9 trefoil (40 10 80 12)
#	rad1 is the diameter of the knot
#	rad2 is the thickness of the knot
#		ASSUMPTION: rad1>rad2
#	interp is the number of interpolation points
#		(more points gives the smoother curves)
eg9 trefoil:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) trefoil 40 10 80 12

eg9x trefoilx:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) trefoil $(rad1) $(rad2) $(nsections) $(nsegments)

# eg10 rocket 	(20 30 20 40 30)
eg10 rocket:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) rocket 10 30 20 40 30

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
	${CXX} $(OBJ_FILES) $(LDFLAGS) $(CORE_LIB) -o $@ $(CXXFLAGS)

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

par params:
	@echo "================ Parameters =================="
	@echo "    interactive = " ${interactive}
	@echo "    inputDir = " ${inputDir}
	@echo "    fileName = " ${fileName}
	@echo "    model = " ${model}
	@echo "    mode = " ${mode}
	@echo "    rad = " ${rad}
	@echo "    ht = " ${ht}
	@echo "    hor = " ${hor}
	@echo "    ver = " ${ver}
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

