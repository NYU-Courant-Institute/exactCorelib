# Makefile

CORE_DIR=..
include ../Make.config

AR=ar rcs
CXXFLAGS=-O2 -Wall -DNDEBUG
LDFLAGS=-O2 -Wall

OBJS=MpfrIO.o CoreDefs.o

ifdef USE_GMPXX
  LIBS+= -lgmpxx
else
  OBJS+= gmpxx/isfuns.o gmpxx/ismpz.o gmpxx/ismpq.o gmpxx/ismpf.o
endif

CORE2_LIB=${CORE_DIR}/lib/libcore2++.a

all: ${CORE2_LIB}

${CORE2_LIB}: ${OBJS}
	$(AR) $@ $^

# show flags
echo:
	@echo "inc  path:" ${INCS} 
	@echo "lib  path:" ${LIBS} 
	@echo "cxx flags:" ${CXXFLAGS} 
	@echo "ld  flags:" ${LDFLAGS} 

#====================================================================
# Rules
#
# $@ -- The file name of the target of the rule
# $< -- The name of the first prerequisite 
# $^ -- The names of all the prerequisites, with spaces between them.
# $? -- The names of all the prerequisites that are newer than the 
#       target, with spaces between them.
#===================================================================
%: %.o
	${CXX} $(LDFLAGS) $< $(LIBS) -o $@

.cpp.o:
	${CXX} -c $(CXXFLAGS) $(INCS) $< -o $@

.cc.o:
	${CXX} -c $(CXXFLAGS) $(INCS) $< -o $@

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f ${OBJS}

