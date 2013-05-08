# Makefile for students/xinwei/K_5/K_5
#
# May 8, 2013
#
# Since Core2
# $Id:$

CORE_PATH=../../..
# include ../Make.options

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

CC = g++

all:
	$(CC) -o Launch.cpp

test:
	$(CC) Launch.cpp -o Launch

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o

#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

#=================================================
# END Makefile 
#=================================================

