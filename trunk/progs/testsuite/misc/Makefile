# Makefile

include ../../Make.config

CORE2_DIR=../..
INCS=-I. -I${SFW_DIR}/include -I${CORE2_DIR}/inc
LIBS=-L${SFW_DIR}/lib -L${CORE2_DIR}/lib -lcore2++ -lgmp -lmpfr

TARGETS=t_auto_ptr t_policy t_infty t_double_prec

all: ${TARGETS}

t_auto_ptr: t_auto_ptr.o

t_policy: t_policy.o

t_infty: t_infty.o

t_double_prec: t_double_prec.o

t_mpfr_prec: t_mpfr_prec.o

t_realib: t_realib.o

t_int: t_int.o

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
