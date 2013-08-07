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
# (B)	But you probably want to try the various interactive demos of this program:
#
# 	       > make eg	-- compiles and runs "gwb" interactively
#
#	This is equivalent to typing:
#
#		> ./gwb $(interactive) \
#			$(pseudo) $(interior) $(fileName) \
#			$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
#			$(inputDir) \
#			$(xtrans) $(ytrans) $(scale) \
#			$(closePoly) $(c1) $(c2) 
#
#	where the meaning of these arguments are described below when we
#	define these parameters.
#
#	To see what these default parameters are, you can type
#
#		> make params
#
# (C)	You can override any of these values at the command line.
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
#		eg4 (line)	-- or eg4a, eg4b (linea, lineb) for variants
#		eg5 (boxes)	-- or eg5a (boxesa) for a various with no translation
#		eg6 (boundedstar)
#		eg7 (bug)	-- a bug trap
#		eg8 (overlap)
# 		eg9 (2color)	-- or eg9a, eg9b
# 		eg10 (2tri)
# 		eg11 (star) 	-- or eg11a (stara) for variant
# 		eg12 (bug2) 	-- a double bug trap
# 		eg150		-- 150 random overlapping triangles 
# 		eg200		-- 200 random overlapping triangles 
#
# 	These targets uses different input files, all taken from
# 	the subdirectory "inputs".   
#
################################################################################
# 	Chee Yap, Jyh-Ming Lien, Vikram Sharma
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

epsilon = 2		# resolution parameter
epsilon = 10		# resolution parameter

pseudo = 0		# boolean: 0 or true => show pseudo Vor objects
pseudo = 1		# boolean: 1 or false => do not show pseudo Vor objects
interior = 0		# boolean: 0 or true => show interior Vor objects
interior = 1		# boolean: 1 or false => do not show interior Vor objects

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

seed = 11		# seed for random
step = 0		# number of steps to run
			#	(step=0 means run to completion)
		
xtrans = 5		# x-translation of the input data 
ytrans = 5		# y-translation of the input data
scale  = 1		# scaling of input data 
closePoly  = 1  # if true (non zero) then do not attempt to close input polygons.
c1  = 0			# if 0 (false) then do not use c1 filter
c2  = 0			# if 0 (false) then do not use c2 filter

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
#       non-interactive mode (i.e., the first argument to "gwb" is "1").
#
test: $(TARGETS)
	./gwb 1 \
		1 $(pseudo) $(interior) input.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2)

testall: $(TARGETS)
	make eg0 interactive=1
	make eg1 interactive=1

testa: $(TARGETS)
	./gwb 1 &

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

#eg0 hemisphere
eg eg0 facehemi:	gwb	
	./gwb face hemisphere 6 20 20

#eg1 sphere
eg1 sphere wiresphere:	gwb	
	./gwb wire sphere 6 20 20

#eg2 torus
eg2 donut torus wiretorus:	gwb	
	./gwb wire torus 6 4 20 20

#eg3 facecylinder
eg3 facecylinder:	gwb	
	./gwb face cylinder 6 20 20 20

#eg4 wirecylinder
eg4 cylinder wirecylinder:	gwb	
	./gwb wire cylinder 6 20 20 20

#eg5 disk 
eg5 disk wiredisk:	gwb	
	./gwb wire disk 10 5 20 20

#eg6 facedisk 
eg6 facedisk:	gwb	
	./gwb face disk 10 5 20 20
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

