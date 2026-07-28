# file: eg6Robertson.mk
#
# Robertson system
#		x' = -0.04*x + 10000*y*(1.0-x-y)
#		y' = 0.04*x - 10000*y*(1.0-x-y) - 30000000*y^2
#
# How to run (from build/examples/EndCover_algo):
#		> make run-eg6Robertson
#		> make run-eg6Robertson iflag=4
#		> make run-eg6Robertson mode=0
#
# Authors: Bingwei, Chee (Jan'26)


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# REQUIRED INPUT ARGUMENTS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
n = 2
var = x,y
ff = -0.04*x+10000*y*(1.0-x-y),0.04*x-10000*y*(1.0-x-y)-30000000*y^2

eps = 1.0

cen = 1.0,0.0
wid = 0.000001,0.000001

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# OPTIONAL ARGUMENTS
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
T = 1
order = 20
iflag = 2
stepB = 2
stepA = 0
debug = 0
