# file: Makefile (for COREX)
#
#  Makefile for CORE Library Extensions (libcorex++.a)
#
#  	We create the Level 1, 2 and 3 versions
#	of each library file, so that users can
#	run their CORE programs in either level.
# $Id: Makefile,v 1.1 2006-04-03 18:55:31 exact Exp $


# =============================================================
# PATHS =======================================================
# =============================================================

CORE_PATH=..
INCLUDE=-I${CORE_PATH}/inc -I${CORE_PATH}/gmp/include \
	-I${CORE_PATH}/mpfr/include

MODULES := linearAlgebra geometry2d geometry3d

L1_OBJS := $(foreach mod, ${MODULES}, $(mod)$(VAR)_level1.o)
L2_OBJS := $(foreach mod, ${MODULES}, $(mod)$(VAR)_level2.o)
L3_OBJS := $(foreach mod, ${MODULES}, $(mod)$(VAR)_level3.o)

# =============================================================
# VARIABLES, FLAGS ============================================
# =============================================================
include ${CORE_PATH}/Make.config

COREX_LIB_LEVEL3=${CORE_PATH}/lib/libcorex++$(VAR)_level3.a
COREX_LIB_LEVEL2=${CORE_PATH}/lib/libcorex++$(VAR)_level2.a
COREX_LIB_LEVEL1=${CORE_PATH}/lib/libcorex++$(VAR)_level1.a
COREX_SHARED_LIB_LEVEL3=${CORE_PATH}/lib/libcorex++$(VAR)_level3.so
COREX_SHARED_LIB_LEVEL2=${CORE_PATH}/lib/libcorex++$(VAR)_level2.so
COREX_SHARED_LIB_LEVEL1=${CORE_PATH}/lib/libcorex++$(VAR)_level1.so

TARGET=${COREX_LIB_LEVEL3} ${COREX_LIB_LEVEL2} ${COREX_LIB_LEVEL1}

ifeq (${LINKAGE}, shared)
  TARGET += $(COREX_SHARED_LIB_LEVEL3) $(COREX_SHARED_LIB_LEVEL2) \
            $(COREX_SHARED_LIB_LEVEL1)
endif

# =============================================================
# TARGETS =====================================================
# =============================================================

default: $(TARGET) 

graphicsTools: 
	cd graphics; ${MAKE}

# =============================================================
# RULES  ======================================================
# =============================================================

${COREX_LIB_LEVEL3}: ${L3_OBJS}
	-@rm -rf $@ 
	$(AR) $@ $?

${COREX_SHARED_LIB_LEVEL3}: ${L3_OBJS}
	-@rm -rf $@ 
	$(CXX) $(CORE_LDFLAGS) -o $@ $?

${COREX_LIB_LEVEL2}: ${L2_OBJS}
	-@rm -rf $@ 
	$(AR) $@ $?

${COREX_SHARED_LIB_LEVEL2}: ${L2_OBJS}
	-@rm -rf $@ 
	$(CXX) $(CORE_LDFLAGS) -o $@ $?

${COREX_LIB_LEVEL1}: ${L1_OBJS}
	-@rm -rf $@ 
	$(AR) $@ $?

${COREX_SHARED_LIB_LEVEL1}: ${L1_OBJS}
	-@rm -rf $@ 
	$(CXX) $(CORE_LDFLAGS) -o $@ $?

%_level3.o:	%.cpp
	${CXX} $(CORE_CXXFLAGS) -DCORE_LEVEL=3 $(INCLUDE) -c $< -o $@

%_level2.o:	%.cpp
	${CXX} $(CORE_CXXFLAGS) -DCORE_LEVEL=2 $(INCLUDE) -c $< -o $@

%_level1.o:	%.cpp
	${CXX} $(CORE_CXXFLAGS) -DCORE_LEVEL=1 $(INCLUDE) -c $< -o $@

clean:
	-@test -z "*.o" || rm -f *.o
	-${MAKE} -C geom2d clean
	-${MAKE} -C geom3d clean

veryclean: clean
	-@test -z "$(COREX_LIB_LEVEL3)" || rm -rf "$(COREX_LIB_LEVEL3)"
	-@test -z "$(COREX_LIB_LEVEL2)" || rm -rf "$(COREX_LIB_LEVEL2)"
	-@test -z "$(COREX_LIB_LEVEL1)" || rm -rf "$(COREX_LIB_LEVEL1)"
	-@test -z "$(COREX_SHARED_LIB_LEVEL1)"||rm -rf "$(COREX_SHARED_LIB_LEVEL1)"
	-@test -z "$(COREX_SHARED_LIB_LEVEL2)"||rm -rf "$(COREX_SHARED_LIB_LEVEL2)"
	-@test -z "$(COREX_SHARED_LIB_LEVEL3)"||rm -rf "$(COREX_SHARED_LIB_LEVEL3)"
	-${MAKE} -C geom2d veryclean
	-${MAKE} -C geom3d veryclean

