# Makefile

include ../../Make.config

CORE2_DIR=../..
INCS=-I${SFW_DIR}/include -I${CORE2_DIR}/inc
LIBS=-L${SFW_DIR}/lib -L${CORE2_DIR}/lib -lcore2++ -lgmp -lmpfr

TARGETS=t_prec_round t_returned_value t_divbyzero t_get_str t_prec t_auto

all: ${TARGETS}

t_prec_round: t_prec_round.o

t_returned_value: t_returned_value.o

t_divbyzero: t_divbyzero.o

t_get_str: t_get_str.o

t_prec: t_prec.o

t_auto: t_auto.o

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

