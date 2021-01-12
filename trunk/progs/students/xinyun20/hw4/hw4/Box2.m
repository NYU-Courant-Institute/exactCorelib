%{
 file: Box2.m
	
 	The Box2 class is derived from Box class,
		designed to be used by the Subdivision class,
		and for robot motiom planning.
	
	What is new in Box2 class over Box1?
		box has features (set of corners and edges)
		box has classification (FREE/MIXED/STUCK)
		box has principal neighbors (N,S,E,W)

	In particular
	What is NOT implemented in box.m are information related to
	the subdivision tree, such as the neighbor properties.
	
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	Robotics Class, Spring 2017
	Chee Yap (with help of TA's Rohit Muthyla and Naman Kumar)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%}
classdef Box2 < Box1
    properties
        classification;		% FREE/MIXED/STUCK
        features = {};		% set of features
        N, E, S, W;		% principal Nbrs
        visited = false;	% for Graph Search use later
    end
    methods
        function obj = Box2(xx, yy, ww, pp)
            if(nargin == 3)
                super_args{1} = xx;
                super_args{2} = yy;
                super_args{3} = ww;
            elseif(nargin == 4)
                super_args{4} = pp;
            else
                error('Wrong number of input args');
            end
            obj = obj@Box1(super_args{:});
            obj.classifyRandom();
        end
        
	% Temporarily: randomly classify
	% This should be replaced by the real classification later.
	%
        function classifyRandom(box)
            random = rand(1);
            if(random > 0.6)
                box.classification = BoxType.FREE;
            elseif(random < 0.3)
                box.classification = BoxType.STUCK;
            else
                box.classification = BoxType.MIXED;
            end
        end
        
%	THIS IS A KEY METHOD:
%		It is best to call sub-methods.
%
%         function child = split(obj)
%		1. call Box2.split
%		2. For each child:
%			2.1 compute its features
%			2.2 classify the child
%			2.3 if FREE, add child to UnionFind structure
%		3. For each child:
%			If it is FREE, do UNION with all its neighbors
%			%	NOTE that this is NOT done as step 2.4! Why?
%
%         end
    end
    
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    methods (Static = true)
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	function test()
	% Testing some basic functions of box class.
	    b = Box2(0,0,1);
            b.showBox();
            disp(['-->> Is root a leaf? ',num2str(b.isLeaf)]);
            children = b.split();
            disp('-->> child 1 = '); children(1).showBox();
            disp('-->> child 2 = '); children(2).showBox();
            disp('-->> child 3 = '); children(3).showBox();
            disp(['-->> Is root a leaf? ',num2str(b.isLeaf)]);
            disp(['-->> Is child(1) a leaf? ',num2str(children(1).isLeaf)]);
	    disp(['-->> Which child contains (-0.2, 0.6)  ? ', ...
	    	num2str(b.findQuad(-.2,.6))]);
	    disp(['-->> Is (0.5, -2) inside box(0,0,1)? ', ...
	    	num2str(b.isIn(0.5,-2))]);
% 
% 	    corner = children(4).SW;
% 	    disp(['-->> SW corner of child(4) = (', ...
% 	    	num2str(corner(1)), ', ', num2str(corner(2)), ')']);
        end
    end
end
