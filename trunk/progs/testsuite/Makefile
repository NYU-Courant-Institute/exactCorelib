# Makefile

CORE_DIR=..
include ../Make.config

CXXFLAGS=-O2 -Wall

TARGETS=t_bigint t_bigrat t_bigfloat t_expr kahan t_poly

all: ${TARGETS}

t_bigint: t_bigint.o

t_bigrat: t_bigrat.o

t_bigfloat: t_bigfloat.o

t_expr: t_expr.o

kahan: kahan.o

t_poly: t_poly.o

include ../Make.rules
