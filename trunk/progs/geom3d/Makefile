# Makefile for geom3d 
#
# Core Library, $Id: Makefile,v 1.1 2006-03-07 04:51:24 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= lines2d_level3 lines3d_level1

MORE_TARGETS= lines2d_level1 lines3d_level3

default: $(TARGETS)

more: $(MORE_TARGETS)

test: $(TARGETS)
	./lines2d_level3 
	./lines3d_level1 

moretest: $(MORE_TARGETS)
	./lines2d_level1 
	./lines3d_level3 

one:
	./lines2d_level3 

two:
	./lines2d_level1 

three:
	./lines3d_level3 

four:
	./lines3d_level1 

%_level1: %_level1.o
	${CXX} $(LDFLAGS) $? -lcorex++$(VAR)_level1 $(CORE_LIB) -o $@

%_level3: %_level3.o
	${CXX} $(LDFLAGS) $? -lcorex++$(VAR)_level3 $(CORE_LIB) -o $@

#=================================================
# Rules for CORE_LEVEL 1 and CORE_LEVEL 3
#=================================================
%_level1.o: %.cpp
	${CXX} -c -DCORE_LEVEL=1 $(CXXFLAGS) $(CORE_INC) $< -o $@

%_level3.o: %.cpp
	${CXX} -c -DCORE_LEVEL=3 $(CXXFLAGS) $(CORE_INC) $< -o $@

.cpp.o:
	${CXX} -c $(CXXFLAGS) $(CORE_INC) $< -o $@

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o

#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORE_TARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

