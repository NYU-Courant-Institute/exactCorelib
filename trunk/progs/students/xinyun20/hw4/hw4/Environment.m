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
			env.readFile( fname )
			env.showEnv( fname )
			env.test( fname )
	
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	%	Intro to Robotics, Spring 2017
	%	Chee Yap (with help of TAs Naman and Rohit)
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%}

classdef Environment < handle
     properties
        Polygons = {};
        X_bag = {};
        Y_bag = {};
        shape;
        %Geometry: 'polygon';
        start = [];
        goal = [];
        epsilon;
        radius;
        BoundingBox = {};
        %corners = []; % should have concave or convex information
        %walls = []; 
        %arr = [];
    end
     methods

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	% readFile( obj, fname )
	%	-- Reads an "env.txt" file with lines in this order:
	%	     radius, eps, BBX, BBY, start, goal, {PX,PY}*
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
         function obj = readFile(obj, filename) %fills all the properties
            flag = 0; flag_radius=0; strt = 0; flag_robot =0;
            fid=fopen(filename);
            tline = fgetl(fid); %reading line by line
            i = 1;
            while ischar(tline)
                if(strt ~= 0)
                    tline = fgetl(fid);
                end
                if(tline == -1)
                    break;
                end
                strt = 1;
                arr = get_numbers(tline);
                X = size(arr);
                if(X(1) == 0)
                    %do nothing
                elseif(X(1) == 1)
                     if(flag_radius == 0)
                         obj.radius = arr(1);
                         flag_radius = 1;
                    else
                         obj.epsilon = arr(1);
                     end
                else
                    if (X(1) == 2) %
                        if(flag_robot == 0)
                            obj.start = arr;
                            flag_robot = 1;
                        else
                            obj.goal = arr;
                        end
                    else % blocks and bounding box
                        if(flag == 0) % Bounding Box
                            A = arr;
                            tline = fgetl(fid);
                            arr = get_numbers(tline);
                            X = size(arr);
                            while(X(1) == 0)
                                 tline = fgetl(fid);
                                 if(ischar(tline))
                                    arr = get_numbers(tline);
                                 end
                                 X = size(arr);
                            end
                            B = arr;
                            obj.BoundingBox{1} = A;
                            obj.BoundingBox{2} = B;
                            flag = 1; 
                        else
                            A = arr;
                            tline = fgetl(fid);
                            arr = get_numbers(tline);
                            X = size(arr);
                            while(X(1) == 0)
                                 tline = fgetl(fid);
                                 if(ischar(tline))
                                    arr = get_numbers(tline);
                                 end
                                 X = size(arr);
                            end
                            B = arr;
                            obj.X_bag{i} = A.';
                            obj.Y_bag{i} = B.';
                            obj.Polygons{i} = horzcat(A,B);
                            i = i+1;
                            %patch(A,B,'g')
                        end
                    end
                end
            end
           fclose(fid);
         end


	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	 % Display Robot at conf(a,b) with color c 
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	function showDisc(obj,a,b,c)
	    t = linspace(0, 2*pi);
	    x = obj.radius*cos(t);
	    y = obj.radius*sin(t);
	    patch(x+a, y+b, c);
	 end

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	 % Display Bounding Box 
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	function showBBox(obj)
	     rectangle('Position',...
		      [obj.BoundingBox{1}(1)...
		       obj.BoundingBox{2}(1)...
		       max(obj.BoundingBox{1})...
		       max(obj.BoundingBox{2})],...
		       'EdgeColor', 'b', 'LineWidth',3);
	       %patch(obj.BoundingBox{1},obj.BoundingBox{2},'r');
	 end

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	 % Output Image to file 
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	function outputFile(obj, fname)
	      if nargin<2
		  fname = 'image.jpg';
	      end
	      axis square tight;
	      alpha(0.3);	% DOES NOT WORK for screen? OK for image...
	      F = getframe(gca);
	      imwrite(F.cdata,fname);

	      % EXPERIMENTAL:
	      J = imresize(F.cdata, [256 256]); 
	      imwrite(J,'image_resized.jpg');
	 end

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	 % Display Environment:
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	  function obj = showEnv(obj)
	     figure(1);
	     clf(1);  % clear fig 1 (or else it overlays previous display)
	      axis square tight;
	      alpha(0.3);	% Transparency (to show overlaps)
	     % Show Bounding Box:
	     obj.showBBox();
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
	      
	      %
	      F = getframe(gca);
	      imwrite(F.cdata,'fig1.jpg');


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
            showEnv(a); % uncomment this to show the entire environment
            shape = mapshape(a.X_bag, a.Y_bag);

	    % ADDITIONAL TEST: show obstacles using "mapshow" instead of 
	    %		patch.  How to do color?
	    figure(2);
	    clf(2);
	    axis tight square;
	    alpha(0.3);
	    a.showBBox();
            mapshow(shape.X,shape.Y,...
	    	'DisplayType','polygon',...
		'FaceColor', [0.8, 0.5, 0]...   % brown
		); 
            %this to show only the obstracles.
        end
    end
end
