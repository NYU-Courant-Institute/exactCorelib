ls# Makefile for students/xinwei/K_5/K_5
#
# May 8, 2013
#
# Since Core2
# $Id:$

include Make.options

#=================================================
# Original file
#=================================================
# Launch.cpp
# 
# 
# all:
# 	${CC} *.cpp  -lglui -lglut32 -lglu32 -lopengl32  -o output.exe
# 
# clean:
# 	rm *.exe
#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = Launch

OBJ_FILES =

CC = g++

all: $(TARGETS)

test: Launch
	Launch

Launch: $(OBJ_FILES) Launch.o

#=================================================
# Temp
#=================================================
m:
	gvim Makefile

vi:
	gvim Launch.cpp

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

