# file: eg8Rossler.mk
#
# Rossler chaotic system
#		x' = -y - z
#		y' = x + 0.2*y
#		z' = 0.2 + x*z - 5.7*z
#
# How to run (from build/examples/EndCover_algo):
#		> make run-eg8Rossler
#		> make run-eg8Rossler iflag=4
#		> make run-eg8Rossler mode=0
#
# Authors: Bingwei, Chee (Jan'26)


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# REQUIRED INPUT ARGUMENTS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
n = 3
var = x,y,z
ff = -y-z,x+0.2*y,0.2+x*z-5.7*z

eps = 1.0

cen = 1.0,2.0,3.0
wid = 0.1,0.1,0.1

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# OPTIONAL ARGUMENTS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
T = 1
order = 20
iflag = 2
stepB = 2
stepA = 0
debug = 0
