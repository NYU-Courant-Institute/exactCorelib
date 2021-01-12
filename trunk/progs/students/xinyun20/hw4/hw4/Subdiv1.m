%{
	file: Subdiv1.m

	Implements a class for Subdiv1 of a rootbox.
		Depends on Box1 class.
		There is a register for all created boxes.
		All boxes in register is a descendent of the rootBox.
		The position of a box in this register is its "index".
		The children pointers for each box

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%`
	Robotics Class, Spring 2017
	Chee Yap (with help of TA's Rohit Muthyla and Naman Kumar)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%}

classdef Subdiv1 < handle
    properties
        rootBox;
    end

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    methods
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        function obj = Subdiv1(x,y,w)
	% Constructor for Subdiv1
            obj.rootBox = Box2(x,y,w);
        end

        function split(~,box)
        % SPLIT(box) will split this box into 4 children,
        %	insert the children into the Register,
        %	and store the children's indices into this box.
            %assert(isa(Box1,'Box1'),'Not Box1 object', class(Box1));
            box.split();
        end
        
        function box = findBox(obj,x,y)
        % FINDBOX(x,y) will find the leaf box of the current
        %	Subdiv1 that contains the point (x,y), and
        %	returns the index of this point.  If (x,y) lies
        %	on the boundary of more than one box, we break
        %	ties arbitrarily.
            box = obj.rootBox;
            %assert(isa(Box1,'Box1'),'Not Box1 object', class(Box1));
            while(box.isLeaf == 0)
               box = findChild(obj,box,x,y);
            end
        end

        function childBox = findChild(~,box,x,y)
        % FINDCHILD(box,x,y) is interesting because it
        %	does not depend on the Subdiv1 instance.
        %	Hence the implicit object is specified as "~".
        %	NOTE: the box class has a similar method.
            %assert(isa(Box1,'Box1'),'Not Box1 object', class(Box1));
            if(x < box.x)
                if(y < box.y)
                     childBox = box.child(3);
                else
                     childBox = box.child(2);
                end
            else
                if(y < box.y)
                     childBox = box.child(4);
                else
                     childBox = box.child(1);
                end
            end
        end


	function showBox(~, box)
	% SHOWBOX(box) displays the box with index idx
            box.showBox();		  %  so that fbox is a box, not a cell.
    end
    
    function displayLeaves(obj)
        showAllLeaves(obj, obj.rootBox)
    end
    
    function showAllLeaves(obj, rootBox)
            if(rootBox.isLeaf == 1)
                obj.showBox(rootBox);
            else
                for i = 1:length(rootBox.child)
                    obj.showAllLeaves(rootBox.child(i));
                end
            end
    end
        
    function showSubdiv1(obj)
        showAllBoxes(obj, obj.rootBox);
    end
    
    function showAllBoxes(obj, rootBox)
        obj.showBox(rootBox); 
        if(rootBox.isLeaf == 0)
                for i = 1:length(rootBox.child)
                    obj.showAllBoxes(rootBox.child(i));
                end
        end
    end
    
    end

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    methods (Static = true)
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        function test()
            s = Subdiv1(0,0,1);
            rootBox = s.rootBox;
	    %%%%%%%%%%%%%%%%%%%%%% FIRST SPLIT:
            s.split(rootBox);

	    %%%%%%%%%%%%%%%%%%%%%% SECOND SPLIT:
            box = s.findBox(0.2,-0.7);
            s.split(box);
            %s.showSubdiv1();
	    %%%%%%%%%%%%%%%%%%%%%% THIRD SPLIT:
            %box = s.findBox(0.2,-0.7);
            %s.split(box);
            s.showSubdiv1();	

            box = s.findBox(0.2,-0.7);	
            %s.showBox(box);
        end
    end
end

