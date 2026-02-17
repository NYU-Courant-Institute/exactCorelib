# file: egLorenz.mk
#
# This runs the Lorenz System
#		x' = 10*y - 10*x
#		y' = 28*x - x*z - y
#		z' = x*y - 8*z/3
# To run this code, call the Makefile for executables:
#
#		> make run-egLorenz
#
# Bingwei,Chee (Oct'25)


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# REQUIRED INPUT ARGUMENTS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# Dimension of the problem
n = 3 

# The variable names
var = x,y,z

# The first order differential system
ff = 10*y-10*x,28*x-x*z-y,x*y-8*z/3

# Bound on output enclosure diameter (in the \infinity-norm)
eps = 9.0 

# The input box in centered form: 
cen = 15.0,15.0,36.0 
wid = 0.002,0.002,0.002 

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
