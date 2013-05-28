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
sphereradius = -1
epsilon = -1
type = 2
xStart = -1
yStart = -1
zStart = -1
xEnd = -1
yEnd = -1
zEnd = -1

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

help: 
	@echo "Usage: Call Draw eg[x]"
	@echo "[x] can be the following:"
	@echo "1, 1ex, 2, 3, 4, 5, 6"
	@echo "7, 8, 8ex, 9, 9ex, 10"

eg1: Draw
	./Draw cmpr1/ 2 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1

#=================================================
# ./Draw ${filename} ${type} ${spaceradius} ${epsilon} ${sphereradius} \
#	${xStart} ${yStart} ${zStart} ${xEnd} ${yEnd} ${zEnd} \
#	&
#=================================================

eg1ex: Draw
	./Draw cmpr/ ${type} ${spaceradius} ${epsilon} ${sphereradius} -1 -1 -1 -1 -1 -1

eg2: Draw
	./Draw cmpr2basic/ 2 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1

eg3: Draw
	./Draw cmpr3basic/ 2 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1

eg4: Draw
	./Draw cmpr4blank/ 1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1

eg5: Draw
	./Draw cmpr5/ 1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1

eg6: Draw
	./Draw cmpr6piece/ 1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1

eg7: Draw
	./Draw cmpr7/ 2 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1

eg8: Draw
	./Draw cmpr8bc/ 1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1

eg8ex: Draw
	./Draw cmpr8bn/ 1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1

eg9: Draw
	./Draw cmpr9d/ 1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1

eg9ex: Draw
	./Draw cmpr9bc/ 1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1

eg10: Draw
	./Draw cmpr10w/ 1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1

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

