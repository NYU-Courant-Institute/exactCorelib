# Makefile

include ../Make.config

INCS=-I${SFW_DIR}/include -I../inc
LIBS=-L${SFW_DIR}/lib -L../lib -lcore2++ -lgmp -lmpfr
CXXFLAGS=-O2 -Wall

TARGETS=t_bigint t_bigrat t_bigfloat t_expr kahan

all: ${TARGETS}

t_bigint: t_bigint.o

t_bigrat: t_bigrat.o

t_bigfloat: t_bigfloat.o

t_expr: t_expr.o

kahan: kahan.o

#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(LDFLAGS) $< $(LIBS) -o $@

.cpp.o:
	${CXX} -c $(CXXFLAGS) $(INCS) $< -o $@

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o
	rm -f ${TARGETS}
