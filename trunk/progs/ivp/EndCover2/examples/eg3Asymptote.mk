# file: eg3Asymptote.mk
#
# Asymptote system
#		x' = x^2
#		y' = -y^2 + 7*x
#
# How to run (from build/examples/EndCover_algo):
#		> make run-eg3Asymptote
#		> make run-eg3Asymptote iflag=4          # also writes E_0.txt / E_1.txt for plotting
#
# Current CLI order (for reference):
#		./endcover.exe iflag stepB stepA n <vars...> <funs...> eps order T debug tubedegree <intervals...>
#
# iflag is progressive (iflag>=k includes all smaller outputs):
#		0+: reserved for interactive (prints nothing)
#		1+: print total runtime (ms) and Hull(T)
#		2+: print E0Boxes: number of initial sub-boxes
#		3+: write E0.txt and E1.txt (E1.txt contains ONLY time-T cover)
#		4+: additionally write output/E_0.txt and output/E_1.txt for plotting
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
ff = x^2,-y^2+7*x

# Bound on output enclosure diameter (in the \infinity-norm)
eps = 1.0

# The input box in centered form (comma-separated)
cen = -1.5,8.5
wid = 0.01,0.01

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# OPTIONAL ARGUMENTS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
T = 1
order = 20
iflag = 2
stepB = 2
stepA = 0
debug = 0
