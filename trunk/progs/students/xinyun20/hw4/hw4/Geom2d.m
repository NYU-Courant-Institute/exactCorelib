%{
	Geom2d.m

	This class knows about 

		points, edges, polygons

	Moreover, we assume these are all mapshape objects!!!

	This class has ONLY static methods such as

		sep(obj, obj)
		leftOf( p1, p2, p3)

	ALL geometric computation should be done in this class.

	Please add other methods as the need arise.
%}

classdef Geom2d 

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	methods (Static = 'true')
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	% returns the separation between obj1 and obj2
	%	You only need 2 cases:
	%	 (obj1,obj2) = (point, point)
	%	 (obj1,obj2) = (point, edge)
	function s = sep(obj1, obj2)
		% fill!
	end

	% returns true iff (p1,p2,p3) represents a "Left Turn"
	%
	function bool = leftOf(p1, p2, p3)
		% fill!
    	end

end




