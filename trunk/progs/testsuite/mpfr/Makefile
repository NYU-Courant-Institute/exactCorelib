# Makefile

# set the core library directory and include Make.config
CORE_DIR=../..
include ${CORE_DIR}/Make.config

# define targets list
TARGETS=t_prec_round \
	t_returned_value \
	t_divbyzero \
	t_get_str \
	t_prec t_auto \
        t_mul \
	t_add \
	t_io

# default target
all: ${TARGETS}

#@begin targets
t_prec_round: t_prec_round.o

t_returned_value: t_returned_value.o

t_divbyzero: t_divbyzero.o

t_get_str: t_get_str.o

t_prec: t_prec.o

t_auto: t_auto.o

t_mul: t_mul.o

t_add: t_add.o

t: t.o

t_io: t_io.o

#@end targets

#include standard rules from Make.rules
include ${CORE_DIR}/Make.rules
