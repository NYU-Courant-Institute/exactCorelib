# file: eg2Vanderpol.mk
#
# Van der Pol oscillator
#		x' = y
#		y' = -x^2*y + y - x
#
# How to run (from build/examples/EndCover_algo):
#		> make run-eg2Vanderpol
#		> make run-eg2Vanderpol iflag=4          # also writes E_0.txt / E_1.txt for plotting
#
# Current CLI order (for reference):
#		./endcover.exe iflag stepB stepA n <vars...> <funs...> eps order T debug tubedegree <intervals...>
#
# iflag is progressive (iflag>=k includes all smaller outputs):
#		0+: reserved for interactive (prints nothing)
#		1+: print total runtime (ms) and Hull(T)
#		2+: print E0Boxes: number of initial sub-boxes
#		3+: write E0.txt and E1.txt (E1.txt contains ONLY time-T cover)
#		4+: additionally write output/E_0.txt and output/E_1.txt for plotting:
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
ff = y,-x^2*y+y-x

# Bound on output enclosure diameter (in the \infinity-norm)
eps = 1.0

# The input box in centered form (comma-separated):
# The Makefile converts (cen,wid) -> LOHI as: [cen_i-wid_i, cen_i+wid_i]
cen = -3.0,3.0
wid = 0.1,0.1

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# OPTIONAL ARGUMENTS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# The target time
T = 1

# The order of Taylor expansion
order = 20

# iflag is progressive (iflag>=k includes all smaller outputs)
iflag = 2



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
