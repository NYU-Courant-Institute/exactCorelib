# Makefile for progs/gwb/polyhedron/
#
#  HOW TO USE THIS Makefile:
#
# (A)	To compile run this demo, you can use this Makefile and just
#     have to type:
#
# 	       > make 		-- compiles the main program into "vor2d"
# 	       > make test	-- compiles and runs "vor2d" non-interactively
# 	       			   (this is Core Library's automatic self-test)
#
# (B)	But you probably want to try the various interactive demos of
#     this program:
#
# 	       > make eg	-- compiles and runs "vor2d" interactively
#
#	This is equivalent to typing:
#
#		> ./vor2d $(interactive) \
#			$(pseudo) $(interior) $(fileName) \
#			$(boxWidth) $(boxHeight) $(windowPosX) $(windowPosY) \
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
#
################################################################################
# 	Kai Cao
# 	July, 2012
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

inputDir = inputs	# where to find input files
fileName = input300.txt	# input environment file
fileName = bugtrap.txt	# input environment file

winWid = 512		# initial configuration box size
winHt = 512

winX = 200	# initial Window position
winY = 450	# initial Window position

seed = 11		# seed for random
step = 0		# number of steps to run
			#	(step=0 means run to completion)
		
#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = gwb
SUBDIRS = . 
SRCS=$(wildcard $(addsuffix /*.cpp,$(SUBDIRS)))
OBJS=${SRCS:.cpp=.o}
OBJ_FILES = $(OBJS)
# Dependencies:
OBJ_FILES = Euler_Ops.o Polyhedron.o 
MAIN_FILE = gwb.cpp
CXXFLAGS += -MMD

default: gwb

all: $(TARGETS)

# note: this target is the standard target that Core Library uses
#       to test its subdirectories.   So the program must run in a
#       non-interactive mode (i.e., the first argument to "vor2d" is "1").
#
test: $(TARGETS)
	./vor2d 1 \
		1 $(pseudo) $(interior) input.txt \
		$(boxWidth) $(boxHeight) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2)

#=================================================
# Examples of running vor2d program
#=================================================
#
# This is default, but every option may be changed at command line:
#
eg eg0 cube:	./gwb
	@echo "Target 'eg':  runs examples with the default options of Makefile"
	./gwb \

eg2 tetra:	./gwb
	@echo "Target 'eg2':  runs examples with the tetrahedron"
	./gwb \
		0 $(inputDir) tetra.gwb \
            $(winHt) $(winWid) $(winX) $(winY) \
            &

eg3 cube-with-hole:	./gwb
	@echo "Target 'eg2':  runs examples with the tetrahedron"
	./gwb \
		0 $(inputDir) cube-with-hole.gwb \
            $(winHt) $(winWid) $(winX) $(winY) \
            &

eg4 octa:	./gwb
	@echo "Target 'eg2':  runs examples with the tetrahedron"
	./gwb \
		0 $(inputDir) octahedron.gwb \
            $(winHt) $(winWid) $(winX) $(winY) \
            &

eg5 dodeca:	./gwb
	@echo "Target 'eg2':  runs examples with the tetrahedron"
	./gwb \
		0 $(inputDir) dodecahedron.gwb \
            $(winHt) $(winWid) $(winX) $(winY) \
            &

eg6 icosa:	./gwb
	@echo "Target 'eg2':  runs examples with the tetrahedron"
	./gwb \
		0 $(inputDir) icosahedron.gwb \
            $(winHt) $(winWid) $(winX) $(winY) \
            &

help:
	@echo "HELP"

opt options:
	@echo "Compiler Options:"
	@echo "    PLATFORM = " $(PLATFORM)
	@echo "    CXX= " $(CXX)
	@echo "    CORE_INC = " $(CORE_INC)
	@echo "    CORE_LIB = " $(CORE_LIB)
	@echo "    LDFLAGS  = " $(LDFLAGS)
	@echo "    OBJ_FILES= " $(OBJ_FILES)
	@echo "    @= " $@
	@echo "    CXXFLAGS= " $(CXXFLAGS)


#=================================================
# Temp
#=================================================
m:
	gvim Makefile
vi:
	gvim gwb.cpp

gwb: $(OBJ_FILES)
	${CXX} -o $@ $(MAIN_FILE) $(OBJ_FILES) $(LDFLAGS) $(CORE_LIB)

#=================================================
# Rules
#=================================================
.SUFFIXES:
.SUFFIXES: .cpp .o

%:%.o
	${CXX} -o $@ $(OBJ_FILES) $(LDFLAGS) $(CORE_LIB)

.cpp.o:
	${CXX} $(CXXFLAGS) $(CORE_INC) $(CORE_LIB) -c $< -o $@
	cat $*.d >> Dependencies
	rm -f $*.d 

Dependencies:
	touch Dependencies

# Dependencies not used:
# include Dependencies

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
	@echo "    boxHeight = " ${boxHeight}
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
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS) Dependencies $@

#=================================================
# END Makefile 
#=================================================

