# CMakeLists.txt in triangle
#
#	USAGE:
#	This file is in $(COREPATH)/progs/robotics/triangle. 
#	To use this file, assume you are currently
#	in the directory $(COREPATH)/progs/robotics/triangle.  Do this:
#
#		> cd build				-- go to build dir
#		> cmake ../..				-- create Makefile
#		> make					-- create executables
#		> ln -s triangle/triangle ../triangle	-- install executable
#
#	This will deposit all the executable files under triangle/build.
#
# Aug 31, 2013
# Author: Chee Yap
#
# Since Core Library 2.1
# $Id:$
#
# CMake Version
cmake_minimum_required(VERSION 2.8.8)
#
# Project name
project(triangle)

include_directories(${ROBOTICS_SOURCE_DIR}/triangle)
link_directories(${ROBOTICS_BINARY_DIR}/triangle)


# list the object files
set(OBJ_FILES 
	Box.cpp QuadTree.cpp Corner.cpp Wall.cpp fileProcessor.cpp Timer.cpp)


# create the object file library
add_library(triangle_objects OBJECT ${OBJ_FILES}) 	

# list of libraries
set(ALL_LIBS ${GMP_LIBRARY}
	${MPFR_LIBRARY} ${GLUT_LIBRARY} ${GLUI_LIBRARY} ${OPENGL_LIBRARY} m)

#these are all compiled the same way (keep for example only!)
	#set(PROGRAMS oglfirst pointers)
	
	foreach(program ${PROGRAMS})
	  add_executable(${program} ${program}.cpp)
	  target_link_libraries(${program} ${ALL_LIBS})
	endforeach(program)

#building just a library. 
# add_library(geometry.cpp)

# Add executable
add_executable(tri tri.cpp ${OBJ_FILES})
# add_executable(tri tri.cpp $TARGET_OBJECTS:triangle_object)
#
#linking against a custom library
target_link_libraries(tri ${ALL_LIBS})


