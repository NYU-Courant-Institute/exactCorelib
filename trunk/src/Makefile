# file: Makefile (for src)
#$Id: Makefile,v 1.6 2006-04-03 19:56:03 exact Exp $

# =============================================================
# PATHS ======================================================
# =============================================================

CORE_PATH=..

INCLUDE=-I${CORE_PATH}/inc -I${CORE_PATH}/mpfr/include -I${CORE_PATH}/gmp/include

# VAR indicates variants of the Core library
#	E.g., if VAR=(empty) then we compile the release version.
#	If VAR=Debug, then we compile a debugging version.

EXPROBJS=MpfrIO${VAR}.o CoreDefs${VAR}.o CoreAux${VAR}.o

ifdef USE_GMPXX
  LIBS+= -lgmpxx
else
  EXPROBJS+= gmpxx/isfuns${VAR}.o gmpxx/ismpz${VAR}.o \
	 gmpxx/ismpq${VAR}.o gmpxx/ismpf${VAR}.o
endif

# =============================================================
# FLAGS =======================================================
# =============================================================

# read in the appropriate compilation settings from the
# global Make.config.
include ${CORE_PATH}/Make.config


# =============================================================
# TARGETS =====================================================
# =============================================================

# The following library will be built:
CORE_LIB=$(CORE_PATH)/lib/libcore2++${VAR}.a
CORE_SHARED_LIB=$(CORE_PATH)/lib/libcore2++${VAR}.so

TARGET=$(CORE_LIB)

ifeq (${LINKAGE}, shared)
  TARGET+=$(CORE_SHARED_LIB)
endif

# =============================================================
# RULES  ======================================================
# =============================================================

all:	${TARGET}

${CORE_LIB}:	$(EXPROBJS)
	$(AR) $@ $^

${CORE_SHARED_LIB}:	$(EXPROBJS)
	$(CXX) $(CORE_LDFLAGS) -o $@ $^

%${VAR}.o:	%.cpp
	${CXX} $(CORE_CXXFLAGS) $(INCLUDE) -c $< -o $@

%${VAR}.o:	%.cc
	${CXX} $(CORE_CXXFLAGS) $(INCLUDE) -c $< -o $@

# =============================================================
# HOUSE KEEPING  ==============================================
# =============================================================

clean:
	-@test -z "*${VAR}.o" || rm -rf *${VAR}.o

veryclean: clean
	-@test -z "${CORE_LIB}" || rm -rf "${CORE_LIB}"
	-@test -z "${CORE_SHARED_LIB}" || rm -rf "${CORE_SHARED_LIB}"

