# Makefile

include ../Make.config

AR=ar rcs
INCS=-I${SFW_DIR}/include -I../inc
LIBS=-L${SFW_DIR}/lib -L../lib -lgmp -lmpfr
CXXFLAGS=-O2 -Wall -DNDEBUG

OBJS=MpfrIO.o CoreDefs.o

ifdef USE_GMPXX
  LIBS+= -lgmpxx
else
  OBJS+= gmpxx/isfuns.o gmpxx/ismpz.o gmpxx/ismpq.o gmpxx/ismpf.o
endif

CORE2_LIB=../lib/libcore2++.a

all: ${CORE2_LIB}

${CORE2_LIB}: ${OBJS}
	$(AR) $@ $?

#=================================================
# Rules
#=================================================
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

