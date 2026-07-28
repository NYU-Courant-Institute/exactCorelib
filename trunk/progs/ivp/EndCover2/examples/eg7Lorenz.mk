# file: eg7Lorenz.mk
#
# Lorenz system
#		x' = 10*y - 10*x
#		y' = 28*x - x*z - y
#		z' = x*y - 8*z/3
#
# How to run (from build/examples/EndCover_algo):
#		> make run-eg7Lorenz
#		> make run-eg7Lorenz iflag=4
#		> make run-eg7Lorenz mode=0
#
# Authors: Bingwei, Chee (Jan'26)


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# REQUIRED INPUT ARGUMENTS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
n = 3
var = x,y,z
ff = 10*y-10*x,28*x-x*z-y,x*y-8*z/3

eps = 1.0

cen = 15.0,15.0,36.0
wid = 0.001,0.001,0.001

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# OPTIONAL ARGUMENTS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
T = 1
order = 20
iflag = 2
stepB = 2
stepA = 0
debug = 0
