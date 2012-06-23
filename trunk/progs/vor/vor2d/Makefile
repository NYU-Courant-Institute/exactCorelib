# Makefile for progs/robotics/vor/vor2d
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
#	This is equivalent to typing:
#
#		> ./vor2d $(interactive) \
#			$(pseudo) $(interior) $(fileName) \
#			$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
#			$(inputDir) \
#			$(xtrans) $(ytrans) $(scale) \
#			$(closePoly) $(c1) $(c2) \
#			$(maxEps)
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
# 		eg13 (monkey)
# 		eg14 (mammoth)	
# 		eg15 	 	-- Vikram's degenerate example
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

maxEps = 200		# maximum size of a box containing an Voronoi point

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
		1 $(pseudo) $(interior) input.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps)

testall: $(TARGETS)
	make eg0 interactive=1
	make eg1 interactive=1

testa: $(TARGETS)
	./vor2d 1 &

help:
	@echo "USAGE: Type the following"
	@echo "  > make eg                 -- to see an example"
	@echo "  > make params             -- to see default parameters of eg"
	@echo "  > make eg epsilon=7 [...] -- change the default epsilon [...] of eg"
	@echo "  > make egN                -- where N = 0, 1, 2, ..., 14, 15, 150, 200"
	@echo "                               to see various examples "

#=================================================
# Examples of running vor2d program
#=================================================
#
# This is default, but every option may be changed at command line:
#
eg:	vor2d
	@echo "Target 'eg':  runs vor2d with the default options of Makefile"
	./vor2d $(interactive) \
		1 $(pseudo) $(interior) $(fileName) \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg0: Simple NO PATH example (an triangular annulus region)
nopath eg0:	vor2d
	./vor2d $(interactive) \
		1 $(pseudo) $(interior) input0.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg1: Simple example with 3 obstacles (2 Voronoi vertices, one concave verteix)
# 	REMARK: input.txt and input2.txt are really the same.
# 		But input2.txt has comments in it.
simple eg1:	vor2d
	./vor2d $(interactive) \
		1 $(pseudo) $(interior) input.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg2: Same as simple, but with bounding box
simple2 eg2: vor2d
	./vor2d $(interactive)\
		1 $(pseudo) $(interior) input5.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg3: dog example
eg3 dog:	vor2d
	@echo "Target 'eg3' or 'dog': "
	./vor2d $(interactive) \
		0.1 $(pseudo) $(interior) dog.poly \
		512 512 $(windowPosX) $(windowPosY) \
		$(inputDir) \
		100 256 1.4 $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg4: Example of a non-regularized input (set of 4 lines) with closed polygons
eg4 line:	vor2d
	@echo "Target 'eg4' or 'line': "
	./vor2d $(interactive) \
		0.5 $(pseudo) $(interior) line.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) 1 \
		0 $(c2) $(maxEps) \
		&
	
# Eg4a: Example of a non-regularized input (set of 4 lines) but using C1 filter
eg4a linea:	vor2d
	@echo "Target 'eg4a' or 'linea': "
	./vor2d $(interactive) \
		0.5 $(pseudo) $(interior) line.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) 1 \
		1 $(c2) $(maxEps) \
		&
	
# Eg4a: Example of a non-regularized input (set of 4 lines) but open polygons 
eg4b lineb:	vor2d
	@echo "Target 'eg4b' or 'lineb': "
	./vor2d $(interactive) \
		0.5 $(pseudo) $(interior) line.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) 0 \
		0 $(c2) $(maxEps) \
		&
	
# Eg5: A degenerate input with 5 boxes
eg5 boxes: vor2d
	./vor2d $(interactive)\
		1 $(pseudo) $(interior) boxes.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) 100 100 $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
# Eg5a: A degenerate input with 5 boxes (but with no translations, our current
# 	code shows some discontinuous curves)
eg5a boxesa: vor2d
	./vor2d $(interactive)\
		1 $(pseudo) $(interior) boxes.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) 0 0 $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
# Eg6: A triangular star within a bounding frame
eg6 boundedstar: vor2d
	./vor2d $(interactive)\
		1 $(pseudo) $(interior) input6.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg7: bugtrap
eg7 bug: vor2d
	./vor2d $(interactive) \
		1 $(pseudo) $(interior) bugtrap.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg8: Overlapping polygons: 2 triangles + 2 concentric squares (with funny orientation)
eg8 overlap: vor2d
	./vor2d $(interactive) \
		1 $(pseudo) $(interior) overlap.poly \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
# Eg9x: Box containing Voronoi Vertex whose vertices has only two colors
# 	CORRECT trans/scale params:   xtrans=-50  ytrans=80,   scale = 0.1
eg9x 2colorx: vor2d
	./vor2d $(interactive) \
		1 $(pseudo) $(interior) vorBoxWith2colorX.poly \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		-50 80 0.1 $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
# Eg9: Box containing Voronoi Vertex whose vertices has only two colors
eg9 2color: vor2d
	./vor2d $(interactive) \
		1 $(pseudo) $(interior) vorBoxWith2color.poly \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		-360 -150 0.1 $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
# Eg9a: Box containing Voronoi Vertex whose vertices has only two colors
eg9a 2colora: vor2d
	./vor2d $(interactive) \
		1 $(pseudo) $(interior) vorBoxWith2color.poly \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		-350 -150 0.1 $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
# Eg9b: Box containing Voronoi Vertex whose vertices has only two colors
eg9b 2colorb: vor2d
	./vor2d $(interactive) \
		1 $(pseudo) $(interior) vorBoxWith2color.poly \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		-340 -150 0.1 $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
# Eg10: Two triangles generating a Voronoi curve
eg10 2tri: vor2d
	./vor2d $(interactive) \
		1 $(pseudo) $(interior) input10.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
# Eg11: Star example
eg11 star:	vor2d
	./vor2d $(interactive) 1 $(pseudo) $(interior) star.poly \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		100 100 10 $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
	#
# Eg11a: with pseudo=1(false) and interior=1(false)
eg11a stara:	vor2d
	./vor2d $(interactive) 1 1 1 star.poly \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		100 100 4 $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg12: bugtrap2  (one bug trap inside another)
eg12 bug2: vor2d
	./vor2d $(interactive) \
		1 $(pseudo) $(interior) bugtrap2.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg13: monkey example
eg13 monkey:	vor2d
	@echo "Target 'eg13' or 'monkey': "
	./vor2d $(interactive) \
		0.1 $(pseudo) $(interior) nazca_monkey.poly \
		512 512 $(windowPosX) $(windowPosY) \
		$(inputDir) \
		220 220 2 $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg14: mammoth example
eg14 mammoth:	vor2d
	@echo "Target 'eg14' or 'mammoth': "
	./vor2d $(interactive) \
		0.1 $(pseudo) $(interior) mammoth.poly \
		512 512 $(windowPosX) $(windowPosY) \
		$(inputDir) \
		220 220 20 $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
			
# Eg15: Vikram example
eg15 :	vor2d
	@echo "Target 'eg15' :"
	./vor2d $(interactive) \
		0.1 $(pseudo) $(interior) Vikram-1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg15a: Vikram example
eg15a :	vor2d
	@echo "Target 'eg15a' :"
	./vor2d $(interactive) \
		0.1 $(pseudo) $(interior) Vikram-2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
								
# Eg150: Example with 150 randomly generated, overlapping triangles
eg150: vor2d
	./vor2d $(interactive) \
		4 $(pseudo) $(interior) input150.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
eg200: vor2d
	./vor2d $(interactive) \
		4 $(pseudo) $(interior) input200.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
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

