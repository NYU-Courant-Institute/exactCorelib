# file: eg4Quadratic.mk
#
# Quadratic system
#		x' = y
#		y' = x^2
#
# How to run (from build/examples/Boundarymethod/ourcode):
#		> make run-eg4Quadratic
#		> make run-eg4Quadratic iflag=4          # also writes E_0.txt / E_1.txt for plotting
#		> make run-eg4Quadratic mode=0           # use EndCover mode
#
# Authors: Bingwei, Chee (Jan'26)


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# REQUIRED INPUT ARGUMENTS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
n = 2
var = x,y
ff = y,x^2

eps = 1.0

# centered box
cen = 1.0,-1.0
wid = 0.05,0.05

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# OPTIONAL ARGUMENTS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
T = 1
order = 20
iflag = 2
mode = 1
method = 5
stepB = 2
stepA = 0
debug = 0
