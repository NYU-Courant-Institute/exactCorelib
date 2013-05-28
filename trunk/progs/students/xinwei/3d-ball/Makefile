# Makefile for students/xinwei/K_3/K_3
#
# May 28, 2013
#
# Since Core2
# $Id:$

include Make.options

#=================================================
# Original file
#=================================================
# Draw.cpp
# 
# 
# all:
# 	${CC} *.cpp  -lglui -lglut32 -lglu32 -lopengl32  -o output.exe
# 
# clean:
# 	rm *.exe
#=================================================
# User variables (you can change them in the command line)
#=================================================

spaceradius = 50
sphereradius = 5
epsilon = 2
type = 2

#=================================================
# Define target files (put all your files here)
#=================================================



#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = Draw

OBJ_FILES =

CC = g++

all: $(TARGETS)

test: Draw
	./Draw

eg1: Draw
	./Draw cmpr1/ 2 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1

eg1ex: Draw
	./Draw cmpr1/ ${type} ${spaceradius} ${epsilon} ${sphereradius} -1 -1 -1 -1 -1 -1

eg2: Draw
	./Draw cmpr2/ 2 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1

eg3: Draw
	./Draw cmpr3/ 2 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1

eg4: Draw
	./Draw cmpr4/ 2 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1

eg5: Draw
	./Draw cmpr5/ 2 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1

Draw: $(OBJ_FILES) Draw.o

#=================================================
# Temp
#=================================================
m:
	gvim Makefile

vi:
	gvim Draw.cpp

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

