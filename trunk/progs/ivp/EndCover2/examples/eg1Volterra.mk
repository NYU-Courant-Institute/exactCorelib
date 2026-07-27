# file: eg1Volterra.mk
#
# Volterra system
#		x' = 2*x-2*x*y
#		y' = -y+x*y
#
# How to run (from build/examples/Boundarymethod/ourcode):
#		> make run-eg1Volterra
#		> make run-eg1Volterra iflag=4          # also writes E_0.txt / E_1.txt for plotting
#		> make run-eg1Volterra mode=0           # use EndCover mode
#
# Current CLI order (for reference):
#		./radicalIVP-new.exe iflag mode method stepB stepA n <vars...> <funs...> eps order T debug <intervals...>
#
# iflag is progressive (iflag>=k includes all smaller outputs):
#		0+: print total runtime (ms)
#		1+: print Hull(T): minimal axis-aligned hull of the time-T cover
#		2+: print E0Boxes: number of initial sub-boxes
#		3+: write E0.txt and E1.txt (E1.txt contains ONLY time-T cover)
#		4+: additionally write E_0.txt and E_1.txt for plotting:
#			- E_0.txt contains E0 (and in 2D also the 4 corner points)
#			- E_1.txt contains time-T cover plus propagated images at times 0.1/0.4/0.7 (if <= T)
#			  and in 2D also corner propagation
#
# Authors: Bingwei, Chee (Jan'26)


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# REQUIRED INPUT ARGUMENTS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# Dimension of the problem
n = 2 

# The variable names (comma-separated)
var = x,y

# The first order differential system (comma-separated, each entry is one ODE RHS)
ff = 2*x-2*x*y,-y+x*y 

# Bound on output enclosure diameter (in the \infinity-norm)
eps = 1.0 

# The input box in centered form (comma-separated):
# The Makefile converts (cen,wid) -> LOHI as: [cen_i-wid_i, cen_i+wid_i]
cen = 1.0,3.0
wid = 0.1,0.2

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# OPTIONAL ARGUMENTS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# The target time
T = 1 
# The order of Taylor expansion
order = 20 

# iflag is progressive (iflag>=k includes all smaller outputs):
#	0+: print total runtime (ms)
#	1+: print Hull(T): minimal axis-aligned hull of the time-T cover
#	2+: print E0Boxes: number of initial sub-boxes
#	3+: write E0.txt and E1.txt (E1.txt contains ONLY time-T cover)
#	4+: additionally write E_0.txt and E_1.txt for plotting (adds 0.1/0.4/0.7 tracking if <=T)
iflag = 2

# mode: 1 = Boundary algorithm (TwoDimEncAlgo / ThreeDimEncAlgo), 0 = EndCover subdivision
mode = 1

# method (see Makefile header / radicalIVP-new.cpp):
#	0: our method
#	1: oursimple method (Euler)
#	2: oursimpleT method (Euler + transformation)
#	4: ourNoEuler method
#	5: Lohner method

method = 5 

# stepB:
#	0: crlohner
#	1: crlohner + lognorm
#	2: direct + lognorm
#	3: direct

stepB = 2 

# stepA:
#	0: adaptive stepA
#	1: non-adaptive stepA
stepA = 0 

# debug:
#	0: off
#	1: on
debug = 0 
