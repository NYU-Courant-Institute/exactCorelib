# file: eg5FitzHugh.mk
#
# FitzHugh system
#		x' = x - x^3/3 - y + 0.5
#		y' = 0.08*x + 0.056 - 0.064*y
#
# How to run (from build/examples/Boundarymethod/ourcode):
#		> make run-eg5FitzHugh
#		> make run-eg5FitzHugh iflag=4
#		> make run-eg5FitzHugh mode=0
#
# Authors: Bingwei, Chee (Jan'26)


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# REQUIRED INPUT ARGUMENTS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
n = 2
var = x,y
ff = x-x^3/3-y+0.5,0.08*x+0.056-0.064*y

eps = 1.0

cen = 1.0,0.0
wid = 0.1,0.1

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
