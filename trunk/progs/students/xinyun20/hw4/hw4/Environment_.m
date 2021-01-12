%{
	Environment class
	
		The environment object defines an instance of the
		path planning problem for a disc robot.
		It needs these data (i.e., properties):
		 
				-- radius of robot
				-- epsilon
				-- bounding box for obstacles
				-- start and goal config of robot
				-- obstacle set (set of polygons)
	
		An environment file (e.g., env0.txt) is a line-based text file
		containing the above information, in THIS STRICT ORDER.
		Comment character (%) indicates that the rest of a line
		are ignored.
	
		Methods:
			showEnv( obj, fname )	-- IMPLEMENTED FOR YOU.
			readFile( obj, fname )
			test( obj, fname )

			showDisc( obj, a, b, c )
			showBoundingBox( obj )
			outputFile( obj, fname )

		HINT: try to provide default arguments for most methods
	
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	%	Intro to Robotics, Spring 2017
	%	Chee Yap (with help of TAs Naman and Rohit)
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%}

classdef Environment < handle
     properties
        radius;
        epsilon;
        BoundingBox = {};
        start = [];
        goal = [];
        Polygons = {};
    end


    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
     methods
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	% readFile( obj, fname )
	%	-- Reads an "env.txt" file with lines in this order:
	%	     radius, eps, BBX, BBY, start, goal, {PX,PY}*
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
         function obj = readFile(obj, filename) %fills all the properties
	  % fill!
         end


	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	 % Display Robot at conf(a,b) with color c 
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	function showDisc(obj,a,b,c)
	  % fill!
	 end

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	 % Display Bounding Box 
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	function showBoundingBox(obj)
	  % fill!
	 end

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	 % Output Image to file 
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	function outputFile(obj, fname)
	  % fill!
	 end

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	 % Display Environment:
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	  function obj = showEnv(obj)
	     figure(1);
	     clf(1);  % clear fig 1
	      axis square tight;
	      alpha(0.3);	% Transparency (to show overlaps)
	     % Show Bounding Box:
	     obj.showBoundingBox();
	     % show start and goal config:
	     bluegreen=[0, 1, 1];
	     redgreen=[1, 1, 0];
	     obj.showDisc(obj.start(1), obj.start(2), bluegreen);
	     obj.showDisc(obj.goal(1), obj.goal(2), redgreen);
	
	      %Display the obstacles in brown:
	      brown = [0.8, 0.5, 0];
	      for C = obj.Polygons
	          patch(C{1}(:,1),C{1}(:,2), brown);
	      end
	      
	      % Output an image file:
	      obj.outputFile('image.jpg');
         end
     end



    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    methods(Static)
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

        function a = test( filename )
	    if nargin<1
		filename = 'env0.txt';
	    end
            a = Environment;
            a = readFile(a, filename);
            showEnv(a); % show the entire environment

	    % ADDITIONAL TEST: show obstacles using "mapshow" instead of 
	    %		patch.  How to do color?
	    figure(2);
	    clf(2);
	    axis tight square;
	    alpha(0.3);
	    a.showBBox();
            % shape = mapshape(a.X_bag, a.Y_bag);
            % mapshow(shape.X,shape.Y,...
	    % 	'DisplayType','polygon',...
	    % 	'FaceColor', [0.8, 0.5, 0]...   % brown
	    % 	); 
        end
    end
end
