# ===========================================================================
#  Makefile for the clean EndCover implementation.
#  ---------------------------------------------------------------------------
#  Quick start:
#    make                 -- compile endcover.exe and run the default example
#    make eg              -- run the built-in Volterra example
#    make run-eg7Lorenz   -- run using a shared example parameter file
#    make run FILE=examples/eg8Rossler.mk
#    make eg-lorenz eps=0.05 T=2
#
#  CLI (the leading iflag only controls output; it never changes the result):
#    ./endcover.exe iflag stepB stepA n <vars..> <funs..> eps order T debug <lo hi ..>
#
#    iflag : progressive (iflag>=k includes the output of all smaller k)
#              0+: print total runtime (ms)
#              1+: print Hull(T): the minimal axis-aligned hull of the time-T cover
#              2+: print E0Boxes: number of initial sub-boxes
#              3+: write E0.txt and E1.txt (E1.txt contains ONLY the time-T cover)
#              4+: additionally write E_0.txt and E_1.txt for plotting:
#                    - E_0.txt contains E0 (and in 2D also the 4 corner points)
#                    - E_1.txt contains the time-T cover plus propagated images
#                      at times 0.1/0.4/0.7 (those <= T), 2D also corner propagation
#    stepB : 0 crlohner | 1 crlohner+logNorm | 2 direct+logNorm | 3 direct
#            4 SIAM + logNorm StepB + endpoint tightening + longest-dim Split
#            5 Boundary-compatible endpoint refine + longest-dim Split
#            6 SIAM scaffold + Boundary endpoint-refine hybrid
#    stepA : 0 adaptive  | 1 non-adaptive
#    debug : 0 off | 1 print admissibility diagnostics
#
#  This tool IS the EndCover subdivision method, so the "mode" and "method"
#  fields present in the shared example files are simply ignored here.
# ===========================================================================

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# SOFTWARE / BUILD FLAGS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
GCC = g++

# Prefer the locally built CAPD (this workspace's build tree).
# This Makefile lives in build/examples/EndCover_algo, so build/bin is 2 up.
CAPD_CONFIG ?= ../../bin/capd-config
CAPD_FLAGS   = $(shell $(CAPD_CONFIG) --cflags --libs)

# CAPD is the only dependency.  These two extra include dirs match this
# workspace's CAPD checkout; override or clear them for your own CAPD install
# (e.g. `make INCLUDE_PATH= FILIB_OVERRIDE=`).
INCLUDE_PATH   = -I/cygdrive/c/Users/69592/OneDrive/Desktop/CAPD/capdAlg/include
FILIB_OVERRIDE = -I/cygdrive/c/Users/69592/OneDrive/Desktop/CAPD/capdExt/filibsrc

EXE  = endcover.exe
SRC  = endcover.cpp
HDRS = calD-calQ-new.h stepAB-new.h Extend-new.h Refine-new.h EndCover-new.h

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# INPUT PARAMETERS (override on the command line, or via an included FILE)
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
iflag ?= 2
stepB ?= 0
stepA ?= 0
n     ?= 2
eps   ?= 0.1
order ?= 20
T     ?= 1
debug ?= 0

# Comma-separated variable list and function list (used by run / run-%).
var ?= x,y
ff  ?= 2*x-2*x*y,-y+x*y

# Explicit lo/hi pairs (legacy / fallback when cen,wid are not given).
lo1 ?= 0.9
hi1 ?= 1.1
lo2 ?= 2.9
hi2 ?= 3.1
lo3 ?=
hi3 ?=

# Center-width form (comma-separated).  If provided, the lo/hi pairs are
# computed as [cen_i - wid_i, cen_i + wid_i].
cen ?=
wid ?=

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# EXAMPLE PARAMETER FILES
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# Directory holding the shared example .mk files.  By default we reuse the ones
# that ship with the Boundary-method tool, so `make run-eg7Lorenz` reads
# ../Boundarymethod/ourcode/examples/eg7Lorenz.mk.  Override with
# EXAMPLES_DIR=... to point elsewhere.
EXAMPLES_DIR ?= ./examples

# Optional single parameter file to include, e.g.
#   make run FILE=$(EXAMPLES_DIR)/eg8Rossler.mk
FILE ?=
ifneq ($(strip $(FILE)),)
include $(FILE)
endif

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# ARGUMENT PLUMBING: split var/ff, build LOHI from cen/wid
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
comma := ,
space :=
space +=

ifneq ($(strip $(var)),)
	var_list := $(strip $(subst $(comma), ,$(var)))
	# If the user didn't set n explicitly, derive it from the variable list.
	ifeq ($(origin n), default)
		n := $(words $(var_list))
	endif
endif

ifneq ($(strip $(ff)),)
	ff_list := $(strip $(subst $(comma), ,$(ff)))
endif

# Positional variable names, and quoted function strings (quotes protect '*').
VAR_ARGS := $(var_list)
FF_ARGS  := $(foreach F,$(ff_list),"$(F)")

ifneq ($(strip $(cen)),)
	cen_list := $(strip $(subst $(comma), ,$(cen)))
endif
ifneq ($(strip $(wid)),)
	wid_list := $(strip $(subst $(comma), ,$(wid)))
endif

ifneq ($(strip $(cen_list)),)
	# Emit "lo1 hi1 lo2 hi2 ..." from center/width.
	lo_hi_args := $(shell awk 'BEGIN{ split("$(cen_list)",C); split("$(wid_list)",W); n=length(C); for(i=1;i<=n;i++){ if(length(W)<i) W[i]=0; printf "%g %g ", C[i]-W[i], C[i]+W[i] } }')
endif

ifeq ($(strip $(lo_hi_args)),)
	LOHI := $(lo1) $(hi1) $(lo2) $(hi2) $(lo3) $(hi3)
else
	LOHI := $(lo_hi_args)
endif

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# BUILD
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
default: eg

compile: $(EXE)

$(EXE): $(SRC) $(HDRS)
	$(GCC) $(SRC) -o $(EXE) $(FILIB_OVERRIDE) $(CAPD_FLAGS) $(INCLUDE_PATH)

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# RUN
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# Show the resolved arguments (handy when debugging an included FILE).
show-runargs:
	-@echo "	-- FILE=" $(FILE)
	-@echo "	-- iflag=" $(iflag)
	-@echo "	-- stepB, stepA=" $(stepB) $(stepA)
	-@echo "	-- n=" $(n) "  var=" $(var) "  ff=" $(ff)
	-@echo "	-- eps=" $(eps) "  order=" $(order) "  T=" $(T) "  debug=" $(debug)
	-@echo "	-- LOHI=" $(LOHI)

# Run using var / ff / LOHI (supports FILE=examples/*.mk).
run: compile show-runargs
	./$(EXE) $(iflag) $(stepB) $(stepA) $(n) $(VAR_ARGS) $(FF_ARGS) $(eps) $(order) $(T) $(debug) $(LOHI)

# Convenience: `make run-eg7Lorenz` loads $(EXAMPLES_DIR)/eg7Lorenz.mk.
run-%:
	$(MAKE) FILE=$(EXAMPLES_DIR)/$*.mk run

# `make eg`: the default (Volterra) example, no parameter file needed.
eg: compile
	-@echo "Volterra system (default example)"
	./$(EXE) $(iflag) $(stepB) $(stepA) 2 x y "2*x-2*x*y" "-y+x*y" $(eps) $(order) $(T) $(debug) 0.9 1.1 2.9 3.1

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# NAMED EXAMPLES (accept the usual overrides, e.g. `make eg-lorenz eps=0.05 T=2`)
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
eg-volterra: compile
	./$(EXE) $(iflag) $(stepB) $(stepA) 2 x y "2*x-2*x*y" "-y+x*y" $(eps) $(order) $(T) $(debug) 0.9 1.1 2.9 3.1

eg-vanderpol: compile
	./$(EXE) $(iflag) $(stepB) $(stepA) 2 x y "y" "-x^2*y+y-x" $(eps) $(order) $(T) $(debug) -3.1 -2.9 2.9 3.1

eg-asymptote: compile
	./$(EXE) $(iflag) $(stepB) $(stepA) 2 x y "x^2" "-y^2+7*x" $(eps) $(order) $(T) $(debug) -1.51 -1.49 8.49 8.51

eg-lorenz: compile
	./$(EXE) $(iflag) $(stepB) $(stepA) 3 x y z "10*y-10*x" "28*x-x*z-y" "x*y-8*z/3" $(eps) $(order) $(T) $(debug) 14.999 15.001 14.999 15.001 35.999 36.001

eg-rossler: compile
	./$(EXE) $(iflag) $(stepB) $(stepA) 3 x y z "-y-z" "x+0.2*y" "0.2+x*z-5.7*z" $(eps) $(order) $(T) $(debug) 0.9 1.1 1.9 2.1 2.9 3.1

clean:
	rm -f $(EXE) E0.txt E1.txt E_0.txt E_1.txt

.PHONY: default compile run show-runargs eg clean \
	eg-volterra eg-vanderpol eg-asymptote eg-lorenz eg-rossler
