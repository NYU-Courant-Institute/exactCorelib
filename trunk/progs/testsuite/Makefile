# Makefile

# set the core library directory and include Make.config
CORE_DIR=..
include ${CORE_DIR}/Make.config

# define targets list
TARGETS= \
	t_bigint \
	t_bigrat \
	t_bigfloat \
	t_expr \
	t_poly \
	kahan

# default target
all: ${TARGETS}

#@begin targets

t_bigint: t_bigint.o

t_bigrat: t_bigrat.o

t_bigfloat: t_bigfloat.o

t_expr: t_expr.o

t_poly: t_poly.o

kahan: kahan.o

#@end targets

#include standard rules from Make.rules
include ${CORE_DIR}/Make.rules
