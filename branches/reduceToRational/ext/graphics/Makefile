#######################################################
# graphics subdirectory
#   whose source is exactly one .cpp file.
#
# To compile the main program, you can simply type "make"
# If you prefer to choose you own file to compile, type
#
#        make p=zzz
#
# where
#       zzz.cc (or zzz.cpp) is the name of the program.
# 
#
#######################################################

#######################################################
# variables/options
#	--use an editor to make sure that your preferred options
#	  is the last one in its list.
#	--You should at least choose the platform variable ("pf").
#######################################################


CORE_PATH=../..
include ${CORE_PATH}/Make.config

AMIYA_HOME=/home/yap/proj/visualhome/proj/bigImages/FromAmiya
HOME=/home/visual/tools/drawpoly
HOME=/home/exact/corelib/ext/graphics/

# PLATFORM is either "unix" or "gnu" or "cyg" or "mac".
# The PLATFORM variable should not be hardcoded
# 	but read from the Make.config file
pf = $(PLATFORM)

CC = g++

# file extension
ext= 
ext=.c
ext=.cc
ext=.cpp

# executable extension
   ifeq ($(pf),cyg)
	exe=.exe
   else
	exe=
   endif

# Runtime libraries (various alternatives are kept around!)

	SRCS = drawCurve.cpp
	OBJS = $(SRCS:.c=.o)

   ifeq ($(pf),cyg)
	COMPILERFLAGS = -Wall -mno-cygwin -DCYGWIN
	LIBS=  -lm -lglut -lglu -lGL 
	LIBS= -L\bin -LC:\WINNT\system32 -lGL -lglu -lglut 
	COMPILERFLAGS = -Wall -DCYGWIN
	SRCS = drawCurve.cpp
	OBJS = $(SRCS:.c=.o)
   else
        ifeq ($(pf),mac)
	@echo PlAtFoRm = $(pf)
	COMPILERFLAGS = -Wall 
	LIBS = -L/opt/local/var/macports/software/mesa/7.4.3_0+hw_render/opt/local/lib -lglut -lGLU -lGL -lm -L/usr/X11R6/lib -lXi -lXmu
        else
	COMPILERFLAGS = -Wall 
	LIBS = -L/usr/X11R6/lib -L/usr/unsupported/installers/chenli/lib \
		-lglut -lGL -lGLU -lXmu -lXi -lXext -lX11 -lm -lsocket -lnsl
	LIBS = -L/usr/X11R6/lib \
		-L/usr/unsupported/packages/opengl/Mesa2.4/lib \
		-lglut -lGL -lGLU -lXmu -lXi -lXext -lX11 -lm -lsocket -lnsl
	LIBS = -L/usr/X11R6/lib \
		-L/usr/unsupported/installers/exact/glut-3.7/lib/glut \
		-lglut -lGL -lGLU -lXmu -lXi -lXext -lX11 -lm -lsocket -lnsl
	LIBS= -lglut -lGLU -lGL -lm -L/usr/X11R6/lib -lXi -lXmu
        endif
   endif

# Include headers files
   ifeq ($(pf),cyg)
	INC= -I\usr\include
   else
      ifeq ($(pf),mac)
	INC = -I/opt/local/var/macports/software/mesa/7.4.3_0+hw_render/opt/local/include
      else
	INC = -I/
	INC = -I/usr/unsupported/installers/chenli/include
	INC = -I/usr/unsupported/packages/opengl/Mesa2.4/include
	INC = -I/usr/unsupported/installers/exact/glut-3.7/include
      endif
   endif

# Load Flags
   ifeq ($(pf),cyg)
   	LDFLAGS =
   else
   	LDFLAGS =
   endif

# program base is "p"
p=drawCurve

# program version is "v"
v=1
v=
# program is "pv"
pv=$(p)$(v)

# data
DATA=/home/visual/tools/drawpoly/data/
DATA=
DATA=data/

ifile=input
ifile=cissoid

# tar files
fn=DRAW

#######################################################
# targets $(COMPILERFLAGS)
#######################################################
p executable default:
	$(CC) $(COMPILERFLAGS) -g $(INC) $(pv)$(ext) \
	    $(LDFLAGS) $(LIBS) -o $(pv)
newdraw: newdraw$(ext)
	$(CC) -g $(INC) newdraw$(ext) \
	    $(LDFLAGS) $(LIBS) -o newdraw

draw: draw.cc
	$(CC) -g $(INC) draw.cc \
	    $(LDFLAGS) $(LIBS) -o draw

poly drawpoly: drawpoly.cc
	$(CC) -g $(INC) drawpoly.cc \
	    $(LDFLAGS) $(LIBS) -o drawpoly

drawCurve1: drawCurve.cpp
	$(CC) $(COMPILERFLAGS) -o $@ $(OBJS) $(LIBS)

curve drawCurve: drawCurve.cpp
	$(CC) $(COMPILERFLAGS) -g $(INC) drawCurve.cpp \
	    $(LDFLAGS) $(LIBS) -o drawCurve

draw.orig: draw.orig.cc
	$(CC) -g $(INC) draw.orig.cc \
	    $(LDFLAGS) $(LIBS) -o draw.orig

# main test
test test0 testcurve:
	./drawCurve < ${DATA}test0

test1:
	./drawCurve < ${DATA}test1

test2 butterfly:
	./drawCurve < ${DATA}butterfly

test3 cissoid:
	./drawCurve < ${DATA}cissoid

t:
	./drawCurve < ${DATA}${ifile}

t1:
	./drawCurve 1 < ${DATA}plot

testpoly:
	./drawpoly < ${DATA}${ifile}

testdraw:
	./draw < ${DATA}${ifile}

testnew:
	./newdraw < ${DATA}${ifile}

but:
	./${pv} < ${DATA}butterfly

ny:
	./${pv} < ${DATA}nylines

teapot: teapot.cpp
	$(CC) $(COMPILERFLAGS) -g $(INC) teapot.cpp \
	    $(LDFLAGS) $(LIBS) -o teapot

teapot2: teapot2.cpp
	$(CC) $(COMPILERFLAGS) -g $(INC) teapot2.cpp \
	    $(LDFLAGS) $(LIBS) -o teapot2

teapot3: teapot3.cpp
	$(CC) $(COMPILERFLAGS) -g $(INC) teapot3.cpp \
	    $(LDFLAGS) $(LIBS) -o teapot3

teapot4: teapot4.cpp
	$(CC) $(COMPILERFLAGS) -g $(INC) teapot4.cpp \
	    $(LDFLAGS) $(LIBS) -o teapot4

#######################################################
# housekeeping
#######################################################
pf platform:
	-@echo Platform is $(PLATFORM)

save:
	ci -l draw.cc drawpoly.cc newdraw.cc Makefile README 

tar:
	tar -cvf DRAW.tar draw*.c* Makefile README input* 

# to use this, you must specify the file name: e.g., make tar fn=1-april-1999
tar1:
	tar -cvf $(fn).tar Makefile README drawpoly.cc data/input*

distrib:
	tar -cvf $(fn).tar Makefile README drawCurve.cpp data/test* data/butterfly

clean:
	-@rm -f *.o *~ 

veryclean: clean
	-@rm -f x* drawCurve


#######################################################
# end of Makefile
#######################################################

