# Makefile

# set the core library directory and include Make.config
CORE_DIR=../..
include ${CORE_DIR}/Make.config

# define targets list
TARGETS= \
	t_root \
	t_prec_round \
	t_get_str

# default target
all: ${TARGETS}

#@begin targets

t_root: t_root.o

t_prec_round: t_prec_round.o

t_get_str: t_get_str.o

#@end targets

#include standard rules from Make.rules
include ${CORE_DIR}/Make.rules
