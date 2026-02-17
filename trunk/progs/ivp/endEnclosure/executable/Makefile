# Makefile for radicalIVP
#	Feb2025, Bingwei, Chee
#
# How to use this Makefile:
#
#		> make		-- will run a simple default example
#		> make help	-- give some targets to try
#		
#	All output is automatically added to the file "out.txt"
#		Note: you may want to delete this file before calling
#			because it is cumulative.
#
#	Some command line argments:
#		iflag: values in range 0-9 (least verbose to most verbose)
#					0: outputs time
#					1: initial and end-enclosure
#					2: number of stages and number of mini-steps
#					3: output all stages
#					4: interactive shell
#						You can choose one of these subcommands:
#						S.T, S.E, S.F, 
#						mu1, mu2	(these are for the ministep) 
#					5: undefined
#					6: undefined
#					7: undefined
#					8: undefined
#					9: show everything
#		method: values in range -1-3
#					0: our method
#					1: oursimple method # Euler method.
#			   		2: oursimpleT method #Euler method + Transformation
#					3: ourNoEuler method  # our method without EulerTube
#		stepB: values in range 0-3
#					0:crlohner
#					1:crlohner + lognorm
#					2:direct + lognorm #our stepB
#					3:direct
#					
#		stepA: values in range 0-1
#					0:adaptive stepA
#					1:non-adaptive stepA
#		Diagnostics: values in range 0-9
#					0: turn off diagnostics outputs
#					1: print current (stage,phase: eps) of Refine
#					2: print ...
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
CAPD_FLAGS=$(shell capd-config --cflags --libs)
SYMENGINE=-l:libsymengine.a
ADDITIONAL=-lgmp -lmpfr -lmpc
# Bingwei's path
INCLUDE_PATH=-I/cygdrive/c/Users/69592/OneDrive/Desktop/CAPD/capdAlg/include
INCLUDE_PATH2=-I/usr/local/include
INCLUDE_PATH3=-L/usr/local/lib


EXE=.exe
IVP=radicalIVP_v0
p=radicalIVP_v0
TstepA=TestStepA
TestF1=TestF1
TstepB=TstepB


# 
# FILE 
FILE ?=

# 
ifneq ($(FILE),)
include $(FILE)
endif

run-%:
	$(MAKE) FILE=examples/$*.mk run
n := $(strip $(n))

run:
	@echo "Debug: n = '$(n)'"  
	# Choose dynamic argument construction so VAR_ARGS and FF_ARGS are used
	./$(IVP)$(EXE) $(iflag) $(method) $(stepB) $(stepA) $(n) $(VAR_ARGS) $(FF_ARGS) $(eps) $(order) $(T) $(debug) $(LOHI)

#
# Defaults (backwards compatible)
iflag ?= 0
method ?= 0
stepB ?= 2
stepA ?= 0
n ?= 2            # will be overridden by var (if var is provided)
# New: comma-separated variable list and function list (human-friendly) Eg:make eg var="x, y" ff="2*x-3*x*y, -y+x*y" run
var ?= x, y
ff  ?= 2*x-2*x*y, -y+x*y

# legacy single-name vars (kept for compatibility with older targets)
var1 ?= x
var2 ?= y
var3 ?=

# legacy single-function defaults
f1 ?= 2*x-2*x*y
f2 ?= -y+x*y
f3 ?=
fs ?=

eps ?= 1.0
order ?= 20
T ?= 1
debug ?= 0
# explicit lo/hi pairs (can be overridden by cen/wid or by providing explicit lo1 hi1 ...)
lo1 ?= 0.9
hi1 ?= 1.1
lo2 ?= 2.9
hi2 ?= 3.1
lo3 ?=
hi3 ?=

# Center-width form (comma-separated). If provided, lo/hi pairs are computed from these. Eg: make eg cen="1.0,3.0" wid="0.1,0.2" run

cen ?=
wid ?=

#--------------------------------------------------
# parse comma-separated lists: var -> var_list, ff -> ff_list
#--------------------------------------------------
comma := ,
space :=
space +=

ifneq ($(strip $(var)),)
  var_list := $(strip $(subst $(comma), ,$(var)))
  # if user didn't explicitly set n, derive it from var_list
  ifeq ($(origin n), default)
    n := $(words $(var_list))
  endif
  # set legacy var1,var2,var3 for backward compatibility
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
  # set legacy f1,f2,f3 for backward compatibility
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

# convenience argument snippets
VAR_ARGS := $(var_list)
# quote each function separately so they survive the shell call as single args
FF_ARGS := $(foreach F,$(ff_list),"$(F)")

#--------------------------------------------------
# compute lo/hi interleaved list from cen/wid if provided
# Use awk to handle floating point arithmetic; output is: lo1 hi1 lo2 hi2 ...
#--------------------------------------------------
ifneq ($(strip $(cen)),)
  cen_list := $(strip $(subst $(comma), ,$(cen)))
endif
ifneq ($(strip $(wid)),)
  wid_list := $(strip $(subst $(comma), ,$(wid)))
endif

ifneq ($(strip $(cen_list)),)
  # produce interleaved lo/hi values (may be empty if wid not provided)
  lo_hi_args := $(shell awk 'BEGIN{ split("$(cen_list)",C); split("$(wid_list)",W); n=length(C); for(i=1;i<=n;i++){ if(length(W)<i) W[i]=0; printf "%g %g ", C[i]-W[i], C[i]+W[i] } }')
endif

# fallback LOHI if cen/wid not used
ifeq ($(strip $(lo_hi_args)),)
  LOHI := $(lo1) $(hi1) $(lo2) $(hi2) $(lo3) $(hi3)
else
  LOHI := $(lo_hi_args)
endif

# debug helper to print parsed lists
debug-vars:
	@echo "var      = '$(var)'"
	@echo "var_list = '$(var_list)'"
	@echo "n        = '$(n)'"
	@echo "ff       = '$(ff)'"
	@echo "ff_list  = '$(ff_list)'"
	@echo "VAR_ARGS = $(VAR_ARGS)"
	@echo "FF_ARGS  = $(FF_ARGS)"
	@echo "LOHI     = $(LOHI)"

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# TARGETS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# default: compile
default: eg

help:
	-@echo "+++++++++++++++++ help ++++++++++++++++++++"
	-@echo "Here are systems to try:"
	-@echo "    Volterra:    eg1 egVolterra, egVolterra1"
	-@echo "    Van der Pol: eg2 egVanderpol"
	-@echo "    Asymptote:   eg3 egAsymptote"
	-@echo "    Lorenz:      eg4 egLorenz"
	-@echo "    Rossler:     eg5 egRossler"
	-@echo "To override the builtin parameters for "
	-@echo "+++++++++++++++++ help ++++++++++++++++++++"

compile: $(IVP)$(EXE)

$(IVP)$(EXE): $(p).cpp
	$(GCC) $(p).cpp -o $(IVP)$(EXE) \
		$(CAPD_FLAGS) $(INCLUDE_PATH) \
			$(INCLUDE_PATH2) $(INCLUDE_PATH3) \
				$(SYMENGINE) $(ADDITIONAL)

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
	-@echo "\t-- iflag=" $(iflag)
	-@echo "\t-- method=" $(method)
	-@echo "\t-- stepB=" $(stepB)
	-@echo "\t-- stepB=" $(stepA)
	-@echo "\t-- n=" $(n)
	-@echo "\t-- vars=" $(VAR_ARGS)
	-@echo "\t-- fns=" $(FF_ARGS)
	-@echo "\t-- eps=" $(eps)
	-@echo "\t-- order=" $(order)
	-@echo "\t-- T=" $(T)
	-@echo "\t-- debug=" $(debug)
	-@echo "\t-- LOHI=" $(LOHI)

# interface default example: use VAR_ARGS and FF_ARGS and LOHI
eg: showargs
	./$(IVP)$(EXE) $(iflag) $(method) $(stepB) $(stepA) $(n) $(VAR_ARGS) $(FF_ARGS) $(eps) $(order) $(T) $(debug) $(LOHI)

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# EXAMPLES WITH OVERRIDABLE PARAMETERS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



# Basic Volterra example with default parameters
eg1 egVolterra: 
	-@echo "Volterra system"
	./$(IVP)$(EXE) \
		$(iflag) \
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

# Volterra with parameter override support
eg1-a egVolterra-a:
	-@echo "Volterra system"
	./$(IVP)$(EXE) $(iflag) \
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
eg1-b egVolterra-b:
	-@echo "Volterra system"
	./$(IVP)$(EXE) $(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"2*x-2*x*y" "-y+x*y" \
		3.3 \
		$(order) \
		5.5 \
		$(debug) \
		0.9 1.1 \
		2.9 3.1
# Van der Pol oscillator
eg2 egVanderpol:
	-@echo "Van der Pol system"
	./$(IVP)$(EXE) $(iflag) \
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

eg2-a egVanderpol-a:
	-@echo "Van der Pol"
	./$(IVP)$(EXE) $(iflag) \
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
eg3 egAsymptote:
	-@echo "Asymptote system"
	./$(IVP)$(EXE) $(iflag) \
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
		-1.51 -1.49 \
		8.49 8.51

eg3-b egAsymptote-b:
	-@echo "Asymptote system"
	./$(IVP)$(EXE) $(iflag) \
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

# Lorenz system
eg4 egLorenz:
	-@echo "Lorenz system"
	./$(IVP)$(EXE) $(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		3 \
		x y z \
		"10*y-10*x" "28*x-x*z-y" "x*y-8*z/3" \
		9.0 \
		$(order) \
		$(T) \
		$(debug) \
		14.999 15.001 \
		14.999 15.001 \
		35.999 36.001
eg4-b egLorenz-b:
	-@echo "Lorenz system"
	./$(IVP)$(EXE) $(iflag) \
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

eg5 egRossler:
	-@echo "Rossler chaotic system"
	./$(IVP)$(EXE) $(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		3 \
		x y z \
		"-y-z" "x+0.2*y" "0.2+x*z-5.7*z" \
		$(eps) \
		$(order)  \
		1 \
		$(debug) \
		0.9 1.1 \
		1.9 2.1 \
		2.9 3,1
eg6 expA1:
	-@echo "(x^2,y^2) system"
	./$(IVP)$(EXE) $(iflag) \
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

eg6-b expA1b:
	-@echo "(x^2) system"
	./$(IVP)$(EXE) $(iflag) \
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


egmo:
	./$(IVP)$(EXE) \
		$(iflag) \
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

table2: 
	-@echo "TABLE 2"
	-@echo -e "\n\n >>>>>>>>>>>>>>> Volterra1 >>>>>>>> \n\n"
	-@make egVolterra1 
	-@echo -e "\n\n >>>>>>>>>>>>>>> vanDerPol >>>>>>>> \n\n"
	-@make egVanderpol
	-@echo -e "\n\n >>>>>>>>>>>>>>> eg3 >>>>>>>> \n\n"
	-@make eg3
	-@echo -e "\n\n >>>>>>>>>>>>>>> Lorenz >>>>>>>> \n\n"
	-@make egLorenz

tablestepAG: 
	-@echo "TABLE Step AG"
	-@echo -e "\n\n >>>>>>>>>>>>>>> Volterra1 >>>>>>>> \n\n"
	-@make egVolterra1 stepA=0 
	-@make egVolterra1 stepA=1 
	-@echo -e "\n\n >>>>>>>>>>>>>>> vanDerPol >>>>>>>> \n\n"
	-@make egVanderpol stepA=0 
	-@make egVanderpol stepA=1 
	-@echo -e "\n\n >>>>>>>>>>>>>>> eg3 >>>>>>>> \n\n"
	-@make eg3 stepA=0
	-@make eg3 stepA=1
	-@echo -e "\n\n >>>>>>>>>>>>>>> Lorenz >>>>>>>> \n\n"
	-@make egLorenz stepA=0
	-@make egLorenz stepA=1



#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# BOOKKEEPING
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# VERBOSITY LEVEL EXAMPLES
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

expV3:
	./$(IVP)$(EXE) \
		3 \
		$(method) \
		2 \
		x y \
		"y" "-x^2*y+y-x" \
		0.001 \
		3 \
		1 \
		$(debug) \
		-3.1 -2.9 \
		2.9 3.1

expV9:
	./$(IVP)$(EXE) \
		9 \
		$(method) \
		2 \
		x y \
		"y" "-x^2*y+y-x" \
		0.01 \
		3 \
		1 \
		$(debug) \
		-3.1 -2.9 \
		2.9 3.1


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# EXPERIMENTS TO SHOW ADVANTAGES OF radicalTransform
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

# x'=x^2, E_0=[0.5,0.8], using Euler


# x' = x^2, E_0 = [0.5, 0.8], using Euler
expA11-b:
	./$(IVP)$(EXE) \
		$(iflag) \
		$(method) \
		1 \
		x \
		x^2 \
		$(eps) \
		$(order) \
		$(T) \
		$(debug) \
		0.8 0.9 \
				

# System with two variables demonstrating radicalTransform
expA2-b:
	./$(IVP)$(EXE) \
		$(iflag) \
		$(method) \
		2 \
		x y \
		"x^3 + 2*x^2 - y^2" \
		"x^2 + y^2 - 1" \
		$(eps) \
		$(order) \
		$(T) \
		$(debug) \
		-0.1 0.1 \
		-1.1 -0.9






#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# TESINGstepA:
# One step test for adaptive stepA with non-adaptive:
# output: (time1,stepsize1) of  adaptive stepA  (time2,stepsize2) of  non-adaptive stepA.
# rho: time1/time2 tau: stepsize1/stepsize2.

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

# interface default example:
egstepA: 	
	./$(TstepA)$(EXE) \
	$(iflag) \
	$(method) \
	$(stepB) \
	$(stepA) \
	$(n) \
	$(var1) $(var2) \
	$(f1) $(f2) \
	$(eps) \
	$(order) \
	$(T) \
	$(lo1) $(hi1) \
	$(lo2) $(hi2)

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# EXAMPLES WITH OVERRIDABLE PARAMETERS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

# Basic Volterra example with default parameters
eg1stepA0:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"2*x-2*x*y" "-y+x*y" \
		0.1 \
		$(order) \
		0.1 \
		0.9 1.1 \
		2.9 3.1
eg1stepA1:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"2*x-2*x*y" "-y+x*y" \
		0.1 \
		$(order) \
		1.0 \
		0.9 1.1 \
		2.9 3.1

eg1stepA2:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"2*x-2*x*y" "-y+x*y" \
		0.1 \
		$(order) \
		10.0 \
		0.9 1.1 \
		2.9 3.1

eg1stepA3:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"2*x-2*x*y" "-y+x*y" \
		0.0001 \
		$(order) \
		0.1 \
		0.9 1.1 \
		2.9 3.1

eg1stepA4:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"2*x-2*x*y" "-y+x*y" \
		0.0001 \
		$(order) \
		1.0 \
		0.9 1.1 \
		2.9 3.1

eg1stepA5:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"2*x-2*x*y" "-y+x*y" \
		0.0001 \
		$(order) \
		10.0 \
		0.9 1.1 \
		2.9 3.1

eg1stepA6:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"2*x-2*x*y" "-y+x*y" \
		0.1 \
		$(order) \
		0.1 \
		2.0 4.0 \
		3.0 5.0

eg1stepA7:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"2*x-2*x*y" "-y+x*y" \
		0.1 \
		$(order) \
		1.0 \
		2.0 4.0 \
		3.0 5.0

eg1stepA8:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"2*x-2*x*y" "-y+x*y" \
		0.1 \
		$(order) \
		10.0 \
		2.0 4.0 \
		3.0 5.0

eg1stepA9:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"2*x-2*x*y" "-y+x*y" \
		0.0001 \
		$(order) \
		0.1 \
		2.0 4.0 \
		3.0 5.0

eg1stepA10:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"2*x-2*x*y" "-y+x*y" \
		0.0001 \
		$(order) \
		1.0 \
		2.0 4.0 \
		3.0 5.0

eg1stepA11:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"2*x-2*x*y" "-y+x*y" \
		0.0001 \
		$(order) \
		10.0 \
		2.0 4.0 \
		3.0 5.0


# Van der Pol oscillator
eg2stepA0:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"y" "-x^2*y+y-x" \
		0.1 \
		$(order) \
		0.1 \
		-3.1 -2.9 \
		2.9 3.1

eg2stepA1:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"y" "-x^2*y+y-x" \
		0.1 \
		$(order) \
		1.0 \
		-3.1 -2.9 \
		2.9 3.1

eg2stepA2:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"y" "-x^2*y+y-x" \
		0.1 \
		$(order) \
		10.0 \
		-3.1 -2.9 \
		2.9 3.1

eg2stepA3:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"y" "-x^2*y+y-x" \
		0.0001 \
		$(order) \
		0.1 \
		-3.1 -2.9 \
		2.9 3.1

eg2stepA4:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"y" "-x^2*y+y-x" \
		0.0001 \
		$(order) \
		1.0 \
		-3.1 -2.9 \
		2.9 3.1

eg2stepA5:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"y" "-x^2*y+y-x" \
		0.0001 \
		$(order) \
		10.0 \
		-3.1 -2.9 \
		2.9 3.1

eg2stepA6:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"y" "-x^2*y+y-x" \
		0.1 \
		$(order) \
		0.1 \
		-4.0 -2.0 \
		3.0 5.0

eg2stepA7:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"y" "-x^2*y+y-x" \
		0.1 \
		$(order) \
		1.0 \
		-4.0 -2.0 \
		3.0 5.0

eg2stepA8:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"y" "-x^2*y+y-x" \
		0.1 \
		$(order) \
		10.0 \
		-4.0 -2.0 \
		3.0 5.0

eg2stepA9:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"y" "-x^2*y+y-x" \
		0.0001 \
		$(order) \
		0.1 \
		-4.0 -2.0 \
		3.0 5.0

eg2stepA10:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"y" "-x^2*y+y-x" \
		0.0001 \
		$(order) \
		1.0 \
		-4.0 -2.0 \
		3.0 5.0

eg2stepA11:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"y" "-x^2*y+y-x" \
		0.0001 \
		$(order) \
		10.0 \
		-4.0 -2.0 \
		3.0 5.0

# Custom example 3
eg3stepA0:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"x^2" "-y^2+7*x" \
		0.1 \
		$(order) \
		0.1 \
		-1.51 -1.49 \
		8.49 8.51

eg3stepA1:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"x^2" "-y^2+7*x" \
		0.1 \
		$(order) \
		1.0 \
		-1.51 -1.49 \
		8.49 8.51

eg3stepA2:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"x^2" "-y^2+7*x" \
		0.1 \
		$(order) \
		10.0 \
		-1.51 -1.49 \
		8.49 8.51

eg3stepA3:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"x^2" "-y^2+7*x" \
		0.0001 \
		$(order) \
		0.1 \
		-1.51 -1.49 \
		8.49 8.51

eg3stepA4:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"x^2" "-y^2+7*x" \
		0.0001 \
		$(order) \
		1.0 \
		-1.51 -1.49 \
		8.49 8.51

eg3stepA5:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"x^2" "-y^2+7*x" \
		0.0001 \
		$(order) \
		10.0 \
		-1.51 -1.49 \
		8.49 8.51

eg3stepA6:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"x^2" "-y^2+7*x" \
		0.1 \
		$(order) \
		0.1 \
		-3.5 -3.4 \
		8.4 8.5

eg3stepA7:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"x^2" "-y^2+7*x" \
		0.1 \
		$(order) \
		1.0 \
		-3.5 -3.4 \
		8.4 8.5

eg3stepA8:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"x^2" "-y^2+7*x" \
		0.1 \
		$(order) \
		10.0 \
		-3.5 -3.4 \
		8.4 8.5

eg3stepA9:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"x^2" "-y^2+7*x" \
		0.0001 \
		$(order) \
		0.1 \
		-3.5 -3.4 \
		8.4 8.5

eg3stepA10:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"x^2" "-y^2+7*x" \
		0.0001 \
		$(order) \
		1.0 \
		-3.5 -3.4 \
		8.4 8.5

eg3stepA11:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"x^2" "-y^2+7*x" \
		0.0001 \
		$(order) \
		10.0 \
		-3.5 -3.4 \
		8.4 8.5

# Lorenz system
eg4stepA0:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		3 \
		x y z \
		"10*y-10*x" "28*x-x*z-y" "x*y-8*z/3" \
		0.1 \
		$(order) \
		0.1 \
		14.999 15.001 \
		14.999 15.001 \
		35.999 36.001

eg4stepA1:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		3 \
		x y z \
		"10*y-10*x" "28*x-x*z-y" "x*y-8*z/3" \
		0.1 \
		$(order) \
		1.0 \
		14.999 15.001 \
		14.999 15.001 \
		35.999 36.001

eg4stepA2:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		3 \
		x y z \
		"10*y-10*x" "28*x-x*z-y" "x*y-8*z/3" \
		0.1 \
		$(order) \
		10.0 \
		14.999 15.001 \
		14.999 15.001 \
		35.999 36.001

eg4stepA3:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		3 \
		x y z \
		"10*y-10*x" "28*x-x*z-y" "x*y-8*z/3" \
		0.0001 \
		$(order) \
		0.1 \
		14.999 15.001 \
		14.999 15.001 \
		35.999 36.001

eg4stepA4:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		3 \
		x y z \
		"10*y-10*x" "28*x-x*z-y" "x*y-8*z/3" \
		0.0001 \
		$(order) \
		1.0 \
		14.999 15.001 \
		14.999 15.001 \
		35.999 36.001

eg4stepA5:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		3 \
		x y z \
		"10*y-10*x" "28*x-x*z-y" "x*y-8*z/3" \
		0.0001 \
		$(order) \
		10.0 \
		14.999 15.001 \
		14.999 15.001 \
		35.999 36.001

eg4stepA6:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		3 \
		x y z \
		"10*y-10*x" "28*x-x*z-y" "x*y-8*z/3" \
		0.1 \
		$(order) \
		0.1 \
		12.0 15.0 \
		13.0 15.0 \
		34.0 36.0

eg4stepA7:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		3 \
		x y z \
		"10*y-10*x" "28*x-x*z-y" "x*y-8*z/3" \
		0.1 \
		$(order) \
		1.0 \
		12.0 15.0 \
		13.0 15.0 \
		34.0 36.0

eg4stepA8:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		3 \
		x y z \
		"10*y-10*x" "28*x-x*z-y" "x*y-8*z/3" \
		0.1 \
		$(order) \
		10.0 \
		12.0 15.0 \
		13.0 15.0 \
		34.0 36.0

eg4stepA9:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		3 \
		x y z \
		"10*y-10*x" "28*x-x*z-y" "x*y-8*z/3" \
		0.0001 \
		$(order) \
		0.1 \
		12.0 15.0 \
		13.0 15.0 \
		34.0 36.0

eg4stepA10:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		3 \
		x y z \
		"10*y-10*x" "28*x-x*z-y" "x*y-8*z/3" \
		0.0001 \
		$(order) \
		1.0 \
		12.0 15.0 \
		13.0 15.0 \
		34.0 36.0

eg4stepA11:
	./$(TstepA)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		3 \
		x y z \
		"10*y-10*x" "28*x-x*z-y" "x*y-8*z/3" \
		0.0001 \
		$(order) \
		10.0 \
		12.0 15.0 \
		13.0 15.0 \
		34.0 36.0

tablestepA: 
	-@echo -e "\n\n >>>>>>>>>>>>>>> Volterra1 >>>>>>>> \n\n"
	-@make eg1stepA0
	-@make eg1stepA1
	-@make eg1stepA2
	-@make eg1stepA3
	-@make eg1stepA4
	-@make eg1stepA5
	-@make eg1stepA6
	-@make eg1stepA7
	-@make eg1stepA8
	-@make eg1stepA9
	-@make eg1stepA10
	-@make eg1stepA11

	-@echo -e "\n\n >>>>>>>>>>>>>>> vanDerPol >>>>>>>> \n\n"
	-@make eg2stepA0
	-@make eg2stepA1
	-@make eg2stepA2
	-@make eg2stepA3
	-@make eg2stepA4
	-@make eg2stepA5
	-@make eg2stepA6
	-@make eg2stepA7
	-@make eg2stepA8
	-@make eg2stepA9
	-@make eg2stepA10
	-@make eg2stepA11

	-@echo -e "\n\n >>>>>>>>>>>>>>> eg3 >>>>>>>> \n\n"
	-@make eg3stepA0
	-@make eg3stepA1
	-@make eg3stepA2
	-@make eg3stepA3
	-@make eg3stepA4
	-@make eg3stepA5
	-@make eg3stepA6
	-@make eg3stepA7
	-@make eg3stepA8
	-@make eg3stepA9
	-@make eg3stepA10
	-@make eg3stepA11

	-@echo -e "\n\n >>>>>>>>>>>>>>> Lorenz >>>>>>>> \n\n"
	-@make eg4stepA0
	-@make eg4stepA1
	-@make eg4stepA2
	-@make eg4stepA3
	-@make eg4stepA4
	-@make eg4stepA5
	-@make eg4stepA6
	-@make eg4stepA7
	-@make eg4stepA8
	-@make eg4stepA9
	-@make eg4stepA10
	-@make eg4stepA11
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# EXAMPLES WITH OVERRIDABLE PARAMETERS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#Test EulerTube: One step test: output: an admissible pair(H1,B1) for E0(input).
# compute Jaccobian matrix, max eigenvalue, mu
# heuler, len1: length of full-enclosure by Taylor method. len2: length of full-enclosure by EulerTube.
# Basic Volterra example with default parameters
eg1testF1A0:
	./$(TestF1)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"2*x-2*x*y" "-y+x*y" \
		0.1 \
		$(order) \
		0.1 \
		0.9 1.1 \
		2.9 3.1
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# Test stepB: One step test: output: an admissible pair(H1,B1) for E0(input).
# compute Jaccobian matrix, max eigenvalue, mu
# len1: length of E1 by Crlohner + lognorm
# len2 and len3: length of E1 by direct + lognorm(two different method to compute it for debuging) 
# len4: length of E1 by direct method
# len22: length of E1 by crlohner
# sigma: len2/len1
# sigma: len2/len3
# Basic Volterra example with default parameters
eg1TstepB:
	./$(TstepB)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"2*x-2*x*y" "-y+x*y" \
		0.1 \
		$(order) \
		$(T) \
		0.59 0.61 \
		1.19 1.21

eg1TstepB1:
	./$(TstepB)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"2*x-2*x*y" "-y+x*y" \
		0.1 \
		$(order) \
		$(T) \
		0.5999 0.6001 \
		1.1999 1.2001


eg2TstepB:
	./$(TstepB)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"y" "-x^2*y+y-x" \
		$(eps) \
		$(order) \
		$(T) \
		-3.1 -2.9 \
		2.9 3.1

eg2TstepB1:
	./$(TstepB)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"y" "-x^2*y+y-x" \
		$(eps) \
		$(order) \
		$(T) \
		-3.0001 -2.9999 \
		2.9999 3.0001



eg3TstepB:
	./$(TstepB)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"x^2" "-y^2+7*x" \
		$(eps) \
		$(order) \
		$(T) \
		-1.51 -1.49 \
		8.49 8.51

eg3TstepB1:
	./$(TstepB)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		2 \
		x y \
		"x^2" "-y^2+7*x" \
		$(eps) \
		$(order) \
		$(T) \
		-1.5001 -1.4999 \
		8.4999 8.5001


eg4TstepB:
	./$(TstepB)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		3 \
		x y z \
		"10*y-10*x" "28*x-x*z-y" "x*y-8*z/3" \
		0.1 \
		$(order) \
		$(T)\
		14.999 15.001 \
		14.999 15.001 \
		35.999 36.001


eg4TstepB1:
	./$(TstepB)$(EXE) \
		$(iflag) \
		$(method) \
		$(stepB) \
		$(stepA) \
		3 \
		x y z \
		"10*y-10*x" "28*x-x*z-y" "x*y-8*z/3" \
		0.1 \
		$(order) \
		$(T)\
		14.9999 15.0001 \
		14.9999 15.0001 \
		35.9999 36.0001
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# END
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
