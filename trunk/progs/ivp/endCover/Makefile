# Makefile for radicalIVP
#	Feb2025, Bingwei, Chee
#
# How to use this Makefile:
#
#		> make		-- will give a simple default
#		
#	All output is automatically added to the file "out.txt"
#		Note: you may want to delete this file before calling
#			because it is cumulative.
#
#	Some command line arguments (current CLI order):
#		./radicalIVP-new.exe iflag mode method stepB stepA n <vars...> <funs...> eps order T debug <intervals...>
#
#		iflag: progressive (iflag>=k includes outputs of all smaller k)
#			0+: print total runtime (ms)
#			1+: print Hull(T): the minimal axis-aligned hull of the time-T cover
#			2+: print E0Boxes: number of initial sub-boxes
#			3+: write E0.txt and E1.txt (E1.txt contains ONLY time-T cover)
#			4+: additionally write E_0.txt and E_1.txt for plotting:
#				- E_0.txt contains E0 (and in 2D also the 4 corner points)
#				- E_1.txt contains time-T cover plus propagated images at times 0.1/0.4/0.7 (if <= T)
#				  and in 2D also corner propagation
#		method: values in range -1-3
#					0: our method
#					1: oursimple method # Euler method.
#			   		2: oursimpleT method #Euler method + Transformation
#					4: ourNoEuler method  # our method without EulerTube
#					5: Lohner method
#		stepB: values in range 0-3
#					0:crlohner
#					1:crlohner + lognorm
#					2:direct + lognorm #our stepB
#					3:direct
#					
#		stepA: values in range 0-1
#					0:adaptive stepA
#					1:non-adaptive stepA
#		Debug: values in range 0-1
#					0:turn off debug operation
#					1:turn on debug operation
#		mode: values in {0,1}
#				1: Boundary algorithm (TwoDimEncAlgo / ThreeDimEncAlgo)
#				0: EndCover subdivision of the full initial box

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# SOFTWARE:
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
GCC=g++
GVIM=gvim

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# VARIABLES
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INC=
LIPS=
# Prefer the locally built CAPD (this workspace's build tree) over a system install.
# Override on the command line if needed, e.g. `make CAPD_CONFIG=capd-config ...`
# This Makefile lives in build/examples/Boundarymethod/ourcode, so build/bin is 3 levels up.
CAPD_CONFIG ?= ../../../bin/capd-config
CAPD_FLAGS=$(shell $(CAPD_CONFIG) --cflags --libs)
SYMENGINE=-l:libsymengine.a
ADDITIONAL=-lgmp -lmpfr -lmpc
# Bingwei's path
INCLUDE_PATH=-I/cygdrive/c/Users/69592/OneDrive/Desktop/CAPD/capdAlg/include
INCLUDE_PATH2=-I/usr/local/include
INCLUDE_PATH3=-L/usr/local/lib
FILIB_OVERRIDE=-I/cygdrive/c/Users/69592/OneDrive/Desktop/CAPD/capdExt/filibsrc


EXE=.exe
IVP=coverIVP
p=CoverIVP
TstepA=TestStepA
TestF1=TestF1
TstepB=TstepB

###############################################################################
# Optional: include an example parameter file.
# Usage:
#   make run-eg1Volterra
#   make FILE=examples/eg1Volterra.mk run
###############################################################################
FILE ?=

###############################################################################
# Defaults (can be overridden by command line or included FILE)
###############################################################################
iflag ?= 2
mode  ?= 1
method ?= 5
stepB ?= 0
stepA ?= 0
n ?= 2

# New: comma-separated variable list and function list (used by run/run-%)
var ?= x,y
ff  ?= 2*x-2*x*y,-y+x*y

# Legacy single-name vars (kept for compatibility with older targets)
var1 ?= x
var2 ?= y
var3 ?=

# Legacy single-function defaults
f1 ?= 2*x-2*x*y
f2 ?= -y+x*y
f3 ?=

eps ?= 0.1
order ?= 20
T ?= 1
debug ?= 0

# Explicit lo/hi pairs (legacy / fallback)
lo1 ?= 0.9
hi1 ?= 1.1
lo2 ?= 2.9
hi2 ?= 3.1
lo3 ?=
hi3 ?=

# Center-width form (comma-separated). If provided, lo/hi pairs are computed from these.
cen ?=
wid ?=

ifneq ($(strip $(FILE)),)
include $(FILE)
endif

###############################################################################
# Helpers for run/run-%: parse comma-separated lists and build LOHI from cen/wid
###############################################################################
comma := ,
space :=
space +=

ifneq ($(strip $(var)),)
	var_list := $(strip $(subst $(comma), ,$(var)))
	# if user didn't explicitly set n, derive it from var_list
	ifeq ($(origin n), default)
		n := $(words $(var_list))
	endif
	_var1 := $(word 1,$(var_list))
	_var2 := $(word 2,$(var_list))
	_var3 := $(word 3,$(var_list))
	ifneq ($(_var1),)
		var1 := $(_var1)
	endif
	ifneq ($(_var2),)
		var2 := $(_var2)
	endif
	ifneq ($(_var3),)
		var3 := $(_var3)
	endif
endif

ifneq ($(strip $(ff)),)
	ff_list := $(strip $(subst $(comma), ,$(ff)))
	_f1 := $(word 1,$(ff_list))
	_f2 := $(word 2,$(ff_list))
	_f3 := $(word 3,$(ff_list))
	ifneq ($(_f1),)
		f1 := $(_f1)
	endif
	ifneq ($(_f2),)
		f2 := $(_f2)
	endif
	ifneq ($(_f3),)
		f3 := $(_f3)
	endif
endif

VAR_ARGS := $(var_list)
FF_ARGS  := $(foreach F,$(ff_list),"$(F)")

ifneq ($(strip $(cen)),)
	cen_list := $(strip $(subst $(comma), ,$(cen)))
endif
ifneq ($(strip $(wid)),)
	wid_list := $(strip $(subst $(comma), ,$(wid)))
endif

ifneq ($(strip $(cen_list)),)
	# output format: lo1 hi1 lo2 hi2 ...
	lo_hi_args := $(shell awk 'BEGIN{ split("$(cen_list)",C); split("$(wid_list)",W); n=length(C); for(i=1;i<=n;i++){ if(length(W)<i) W[i]=0; printf "%g %g ", C[i]-W[i], C[i]+W[i] } }')
endif

ifeq ($(strip $(lo_hi_args)),)
	LOHI := $(lo1) $(hi1) $(lo2) $(hi2) $(lo3) $(hi3)
else
	LOHI := $(lo_hi_args)
endif

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# TARGETS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# default: compile
default: eg

compile: $(IVP)$(EXE)

$(IVP)$(EXE): $(p).cpp
	$(GCC) $(p).cpp -o $(IVP)$(EXE) $(FILIB_OVERRIDE) $(CAPD_FLAGS) $(INCLUDE_PATH) $(INCLUDE_PATH2) $(INCLUDE_PATH3) $(SYMENGINE) $(ADDITIONAL)

compilestepA:  $(TstepA)$(EXE)

$(TstepA)$(EXE): $(TstepA).cpp
	$(GCC) $(TstepA).cpp -o $(TstepA)$(EXE) $(CAPD_FLAGS) $(INCLUDE_PATH) $(INCLUDE_PATH2) $(INCLUDE_PATH3) $(SYMENGINE) $(ADDITIONAL)

compileTestF1:  $(TestF1)$(EXE)

$(TestF1)$(EXE): $(TestF1).cpp
	$(GCC) $(TestF1).cpp -o $(TestF1)$(EXE) $(CAPD_FLAGS) $(INCLUDE_PATH) $(INCLUDE_PATH2) $(INCLUDE_PATH3) $(SYMENGINE) $(ADDITIONAL)

compilestepB:  $(TstepB)$(EXE)

$(TstepB)$(EXE): $(TstepB).cpp
	$(GCC) $(TstepB).cpp -o $(TstepB)$(EXE) $(CAPD_FLAGS) $(INCLUDE_PATH) $(INCLUDE_PATH2) $(INCLUDE_PATH3) $(SYMENGINE) $(ADDITIONAL)



#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# TESTING:
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
show showargs:
	-@echo "	-- iflag=" $(iflag)
	-@echo "	-- mode=" $(mode)
	-@echo "	-- method=" $(method) 
	-@echo "	-- stepB=" $(stepB) 
	-@echo "	-- stepA=" $(stepA)
	-@echo "	-- n=" $(n)
	-@echo "	-- var1, var2=" $(var1) $(var2)
	-@echo "	-- f1, f2=" $(f1) $(f2)
	-@echo "	-- eps=" $(eps) 
	-@echo "	-- order=" $(order)
	-@echo "	-- T=" $(T) 
	-@echo "	-- debug=" $(debug)
	-@echo "	-- lo1, hi1=" $(lo1) $(hi1) 
	-@echo "	-- lo2, hi2=" $(lo2) $(hi2)

# Extended debug for run/run-% (works with included examples/*.mk)
show-runargs:
	-@echo "\t-- FILE=" $(FILE)
	-@echo "\t-- iflag=" $(iflag)
	-@echo "\t-- mode=" $(mode)
	-@echo "\t-- method=" $(method)
	-@echo "\t-- stepB=" $(stepB)
	-@echo "\t-- stepA=" $(stepA)
	-@echo "\t-- n=" $(n)
	-@echo "\t-- var=" $(var)
	-@echo "\t-- var_list=" $(var_list)
	-@echo "\t-- ff=" $(ff)
	-@echo "\t-- ff_list=" $(ff_list)
	-@echo "\t-- eps=" $(eps)
	-@echo "\t-- order=" $(order)
	-@echo "\t-- T=" $(T)
	-@echo "\t-- debug=" $(debug)
	-@echo "\t-- cen=" $(cen)
	-@echo "\t-- wid=" $(wid)
	-@echo "\t-- LOHI=" $(LOHI)

# Run using var/ff/LOHI (supports FILE=examples/*.mk)
run: compile show-runargs
	./$(IVP)$(EXE) $(iflag) $(mode) $(method) $(stepB) $(stepA) $(n) $(VAR_ARGS) $(FF_ARGS) $(eps) $(order) $(T) $(debug) $(LOHI)

# Convenience: run-eg1Volterra loads examples/eg1Volterra.mk
run-%:
	$(MAKE) FILE=examples/$*.mk run

# interface default example:
eg: showargs
	./$(IVP)$(EXE) $(iflag) $(mode) $(method) $(stepB) $(stepA) $(n) \
	$(var1) $(var2) $(f1) $(f2) $(eps) $(order) $(T) $(debug) \
	$(lo1) $(hi1) $(lo2) $(hi2)

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# EXAMPLES WITH OVERRIDABLE PARAMETERS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

input1:
	@echo "input file"
	@./$(IVP)$(EXE) -1 $(mode) $(method) $(stepB) $(stepA) 2 x y \
		"2*x-2*x*y" "-y+x*y" $(eps) $(order) $(T) $(debug) \
		0.9 1.1 2.9 3.1



# eg1: Volterra (default parameters)
eg1 egVolterra:
	-@echo "Volterra system"
	./$(IVP)$(EXE) \
		$(iflag) \
		$(mode) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"2*x-2*x*y" "-y+x*y" \
		$(eps) \
		$(order) \
		$(T) \
		$(debug) \
		0.9 1.1 \
		2.9 3.1

# Backward-compatible alias
eg0: eg1

# Volterra with parameter override support
eg1a egVolterra1:
	-@echo "Volterra system"
	./$(IVP)$(EXE) $(iflag) $(mode) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"2*x-2*x*y" "-y+x*y" \
		$(eps) \
		$(order) \
		1.0 \
		$(debug) \
		0.9 1.1 \
		2.9 3.1
# be careful: this may take about 5 minutes...
eg1b egVolterra2:
	-@echo "Volterra system"
	./$(IVP)$(EXE) $(iflag) $(mode) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"2*x-2*x*y" "-y+x*y" \
		$(eps) \
		$(order) \
		5.5 \
		$(debug) \
		0.9 1.1 \
		2.9 3.1
# Van der Pol oscillator
eg2 egVanderpol:
	-@echo "Van der Pol system"
	./$(IVP)$(EXE) $(iflag) $(mode) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"y" "-x^2*y+y-x" \
		$(eps) \
		$(order) \
		$(T) \
		$(debug) \
		-3.1 -2.9 \
		2.9 3.1



eg22 eg2-b egVanderpol-b:
	-@echo "Van der Pol"
	./$(IVP)$(EXE) $(iflag) $(mode) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"y" "-x^2*y+y-x" \
		$(eps) \
		$(order) \
		$(T) \
		$(debug) \
		2.9 3.1 \
		-3.1 -2.9
		

# Custom example 3
eg3:
	-@echo "Asymptote system"
	./$(IVP)$(EXE) $(iflag) $(mode) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"x^2" "-y^2+7*x" \
		$(eps) \
		$(order) \
		$(T) \
		$(debug) \
		-1.51 -1.49 \
		8.49 8.51

eg32 eg3-b:
	-@echo "Asymptote system"
	./$(IVP)$(EXE) $(iflag) $(mode) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"x^2" "-y^2+7*x" \
		$(eps) \
		$(order) \
		1 \
		$(debug) \
		-1.55 -1.45 \
		8.45 8.55

# eg7: Lorenz system
eg7 egLorenz:
	-@echo "Lorenz system"
	./$(IVP)$(EXE) $(iflag) $(mode) \
		$(method) \
		$(stepB) \
		$(stepA) \
		3 \
		x y z \
		"10*y-10*x" "28*x-x*z-y" "x*y-8*z/3" \
		$(eps) \
		$(order) \
		$(T) \
		$(debug) \
		14.999 15.001 \
		14.999 15.001 \
		35.999 36.001
eg7-b:
	-@echo "Lorenz system"
	./$(IVP)$(EXE) $(iflag) $(mode) \
		$(method) \
		$(stepB) \
		$(stepA) \
		3 \
		x y z \
		"10*y-10*x" "28*x-x*z-y" "x*y-8*z/3" \
		9.0 \
		$(order) \
		1 \
		$(debug) \
		14.999922 15.000078 \
		14.999922 15.000078 \
		35.999922 36.000078

# eg8: Rossler chaotic system
eg8 egrossler:
	-@echo "Rossler chaotic system"
	./$(IVP)$(EXE) $(iflag) $(mode) \
		$(method) \
		$(stepB) \
		$(stepA) \
		3 \
		x y z \
		"-y-z" "x+0.2*y" "0.2+x*z-5.7*z" \
		$(eps) \
		$(order)  \
		$(T) \
		$(debug) \
		0.9 1.1 \
		1.9 2.1 \
		2.9 3.1

# Other examples (kept, but not in eg1..eg8 numbering)
egExpA1 expA1:
	-@echo "(x^2,y^2) system"
	./$(IVP)$(EXE) $(iflag) $(mode) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"x^2" "y^2" \
		$(eps) \
		$(order) \
		$(T) \
		$(debug) \
		0.98 0.99 \
		0.98 0.99

egExpA1b expA1b:
	-@echo "(x^2) system"
	./$(IVP)$(EXE) $(iflag) $(mode) \
		$(method) \
		$(stepB) \
		$(stepA) \
		1 \
		x \
		"x^2" \
		$(eps) \
		$(order) \
		$(T) \
		$(debug) \
		0.98 0.99 

# eg4: quadratic example (previously eg7)
eg4 quadratic:
	-@echo "(y,x^2) system"
	./$(IVP)$(EXE) $(iflag) $(mode) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"y" "x^2" \
		$(eps) \
		$(order) \
		$(T) \
		$(debug) \
		0.95 1.05 \
		-1.05 -0.95

# eg5: FitzHugh system
eg5 FitzHugh:
	-@echo "FitzHugh system"
	./$(IVP)$(EXE) $(iflag) $(mode) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"x-x^3/3-y+0.5" "0.08*x+0.056-0.064*y" \
		$(eps) \
		$(order) \
		$(T) \
		$(debug) \
		0.9 1.1 \
		-0.1 0.1

# eg6: Robertson system
eg6 Reduced-Robertson:
	-@echo "Robertson system"
	./$(IVP)$(EXE) $(iflag) $(mode) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"-0.04*x+10000*y*(1.0-x-y)" "0.04*x-10000*y*(1.0-x-y)-30000000*y^2" \
		$(eps) \
		$(order) \
		$(T) \
		$(debug) \
		0.999999 1.000001 \
		-0.000001 0.000001

# Backward-compatible alias
eg9: eg6

egmo:
	./$(IVP)$(EXE) \
		$(iflag) \
		$(mode) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"y" "-x" \
		$(eps) \
		$(order) \
		$(T) \
		$(debug) \
		-0.1 -0.1 \
		0.9 0.9
