#  snowflake.rob
#
#  	This file describe a star-shaped robot.	
#
#	The general file format for a star-shaped is as follows:
#
#	The first number is
#
#		N = number of vertices of the star-shaped robot
#
#	followed by the list of the vertices (one vertex per line),
#
#		v_1, v_2, ..., v_N
#	
#	We assume 
#		(1) v_0 = (0,0) is NOT a vertex of the robot
#		(2) the vertices are given in CCW order
#		(3) each of the triangles    T_i = (v_0, v_i, v_{i+1})
#				for i=1,2,...,N  is nice.
#			where T_N = (v_0, v_N, v_1)
#		
#	The snowflake robot is decomposed into a central
#		regular hexagon with side length 2,
#	with 6 rectangles of dimensions 2xL  (the L's may be different
#		for some rectangles). 
#
#	Assume L>2 so that the ``short sides'' of the rectangles
#	have length 2.  The ``inner'' short side of each rectangle 
#	abuts a side of the hexagon.  This suggests that the snowflake
#	has 6 + (2x6) = 18 vertices.   However, we will need to 
#	to introduce a vertex
#	in the middle of the ``outer'' short side of each rectangle because
#	of the requirement that T_i is nice.
#	Thus, the snowflake is actually has 24 vertices.
#		
#	We design the snowflake so that only one irrational number,
#			a = sqrt(3)=1.732
#	is needed to describe the coordinates:

24

10	0
10	1
a	1
2+a	1+2a
2+a/2	3/2 + 2a
2	2+2a
0	2
-2	2+2a
-2-a/2	3/2 + 2a
-2-a	1+2a
-a	1
-10	1
-10	0
-10	-1
-a	-1
-2-a	-1-2a
-2-a/2	-3/2-2a
-2	-2-2a
0	-2
2	-2-2a
2+a/2	-3/2-2a
2+a	-1-2a
a	-1
10	-1

