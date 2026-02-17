# file: egVanderPol.mk
#
# This runs the van der Pol System
#		x' = y
#		y' = -x^2*y + y - x
# To run this code, call the Makefile for executables:
#
#		> make run-egVanderPol
#
# Bingwei,Chee (Oct'25)


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# REQUIRED INPUT ARGUMENTS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# Dimension of the problem
n = 2 

# The variable names
var = x,y

# The first order differential system
ff = y,-x^2*y+y-x

# Bound on output enclosure diameter (in the infinity-norm)
eps = 1.0 

# The input box in centered form: 
cen = -3.0,3.0 
wid = 0.1,0.2 

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# OPTIONAL ARGUMENTS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# The target time
T = 1 
# The order of Taylor expansion
order = 20 

# iflag=0 suppresses stage information
iflag = 0 

# 	  0: our method
#	  1: oursimple method # Euler method.
#	  2: oursimpleT method # Euler method + Transformation
#	  3: ourNoEuler method  # our method without EulerTube

method = 0 

#         0:crlohner
#	  1:crlohner + lognorm
#	  2:direct + lognorm #our stepB
#	  3:direct

stepB = 2 

# 	  0:adaptive stepA
#	  1:non-adaptive stepA
stepA = 0 

#
debug = 0 
