%{
	SSS class:
		this is the main class encoding the
		"Soft Subdivision Search" technique.

	The Main Method:
		Setup Environment
			(read from file and initialize data structures)
%}

classdef SSS < handle

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	properties
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		fname;		% filename
		sdiv;		% subdivision
		path=[];	% path 
		startbox=[];	% box containing the start config
		goalbox=[];	% box containing the goal config
	end

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	methods
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    % Constructor
	    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    function sss = SSS(fname)
	    	if (nargin < 1)
		    fname = 'env0.txt';
		end
		setup(fname);
	    end

	    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    % run (arg)
	    %		if no arg, run default example
	    %		if with arg, do
	    %			interactive loop (ignores arg)
	    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    function run(obj, arg)
	    	if nargin<1
			obj.mainLoop();
			obj.showPath();
			return;
		end

		disp(['Welcome to SSS!']);
		while true
			option = input(
				['Choose an options:',
				'0=quit, 1=mainLoop, 2=showEnv, 3=showPath',
				'4=new setup']);
	          switch option
		    case 0,
			return;
		    case 1,
			obj.mainLoop();
			obj.showPath();
		    case 2,
			obj.showEnv();
		    case 3,
			obj.showPath();
		    case 4,
			obj.fname=input('input environment file');
			obj.setup(obj.fname);
			obj.showEnv();
		    otherwise
			disp('invalid option');
	          end % switch
	        end % while
	    end


	    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    % mainLoop
	    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    function flag = mainLoop(obj)

	        flag = false;
	    	obj.startBox = obj.makeFree(startConfig);
		if isempty(startBox)
		    obj.display('NOPATH: start is not free');
		    return;
		end

	    	obj.goalBox = obj.makeFree(goalConfig);
		if isempty(goalBox)
		    display('NOPATH: goal is not free');
		    return;
		end

		if (!obj.makeConnected(obj.startBox, obj.goalBox))
		    display('NOPATH: start and goal not connected');
		    return;
		end
		flag = true;
	    end


	    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    % Setup
	    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    function setup(obj, fname)
	    	obj.env = Environment.readFile(fname);

		obj.sdiv = Subdiv2(obj.env.rootbox.x,
				obj.env.rootbox.y,
				obj.env.rootbox.w);	% 
		obj.unionF = UnionFind();
	    end


	    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    % makeFree(config)
	    %		keeps splitting until we find
	    %		a FREE box containing the config.
	    %		If we fail, we return [] (empty array)
	    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    function box = makeFree(config)
	    end


	    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    % makeConnected(startBox, goalBox)
	    %		keeps splitting until we find
	    %		a FREE path from startBox to goalBox.
	    %		Returns true if path found, else false.
	    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    function flag = makeConnected(startBox, goalBox)
	    end

	    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    % showPath(path)
	    %		Animation of the path
	    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    function showPath(path)
	    end

	    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    % showEnv()
	    %		Display the environment
	    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    function showEnv()
	    end

	end

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	methods (Static = true)
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


	    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    % test()
	    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    function flag = test()
	    end

	end


end % SSS class

