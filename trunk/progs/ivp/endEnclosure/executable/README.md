README file:

This folder contains an executable version of radicalIVP software from 

**A Novel Approach to the Initial Value Problem with
	a Complete Validated Algorithm** 
by
	_Chee Yap and Bingwei Zhang_

**radicalIVP_v0**
	Version 0 of Rigorous ODE Solver with Enclosure Guarantees
	August 2025

Here is the IVP problem which is solved:

IVP Problem Statement
INPUT:
------
- An initial value problem (IVP) defined by:
    n-dimensional ODE system: dx/dt = f(x)
- Initial hyperrectangle B₀ in RR^n
- Time horizon T>0

OUTPUT:
-------
1. Validated subset ulB₀ (a subset of B₀)
2. End enclosure B₁ of IVP(ulB₀,T), containing all solutions at time T.

Command Line Arguments 
----------------------
./radicalIVP_v0.exe \
	[iflag] [method] [stepB] \
	[stepA] [n] [vars...] \
	[fns...] [eps] [order] \
	[T] [debug] [bounds...]

Meaning of Command Line Arguments
---------------------------------
iflag   : Verbosity Level 
			(0=time only, 1=enclosures, 2=step counts, \
				3=all stages, 9=full debug)
method  : 0=Full radicalIVP (default), 1=Euler, \
				2=Euler+Transform, 4=NoEulerTube
stepB   : 0=Crlohner, 1=Crlohner+lognorm, \
				2=Direct+lognorm (default), 3=Direct
stepA   : 0=Adaptive (default), 1=Non-adaptive
n       : State dimension (1,2,3,...)
vars    : a list of n variable names, separated by comma.
			E.g., n=2, vars="x,y" or vars="x1,x2"
			E.g., n=3, vars="x,y,z" or vars="z1,z2,z3"
fs     : a list of n polynomial expressions in the variables of vars
			E.g., n=2, fs="2*x-2*x*y,-x+y^3"
			  (Note: you can use powers like y^3 instead of y*y*y)
			E.g., n=3, fs="10*y-10*x,28*x-x*z-y,x*y-8*z/3" 
			E.g., n=2, fs="(x+y)*(x-y),1.3*x*y"
			  (Note: you can use parentheses, and also decimal numbers)	
eps     : Precision parameter
order   : Taylor order (default=20)
T       : End time (Initial time is always 0)
debug   : 0=Off (default), 1=On
bounds  : Initial Box given as n intervals
				E.g., (lo1 hi1 lo2 hi2...)

Predefined Examples (run with 'make <target>')
--------------------------------------------
eg1		: Volterra system (x'=2x-2xy, y'=-y+xy)
eg2		: Van der Pol oscillator (x'=y, y'=-x²y+y-x)
eg3		: Asymptote system (x'=x², y'=-y²+7x)
eg4	 	: Lorenz system (3D chaotic)
eg5	 	: Rossler system (3D chaotic)
eg6		: Quadratic system (x'=x², y'=y²)
run-input1	: Volterra system (x'=2x-2xy, y'=-y+xy)
run-input2	: Van der Pol oscillator (x'=y, y'=-x²y+y-x)
run-input3	: Asymptote system (x'=x², y'=-y²+7x)
run-input4	: Lorenz system (3D chaotic)



Custom Example
--------------
make eg0 iflag=3 method=2 stepB=1 T=2.0


Output
------
- Results appended to out.txt (delete before new runs)
- Verbosity controlled by iflag:
  iflag=0: Computation time only
  iflag=1: Initial/final enclosures
  iflag=3: All intermediate stages
  iflag=4: Interactive shell

Configuration Notes
-------------------
1. Adjust paths in Makefile for CAPD installation:
   Current: /cygdrive/c/Users/69592/OneDrive/Desktop/CAPD/
2. Default initial sets:
   - Volterra: x₀∈[0.9,1.1], y₀∈[2.9,3.1]
   - Van der Pol: x₀∈[-3.1,-2.9], y₀∈[2.9,3.1]
3. All output is cumulative in out.txt

References
----------
"A Novel Approach to the Initial Value Problem with a Complete Validated Algorithm" by Chee Yap and Bingwei Zhang
