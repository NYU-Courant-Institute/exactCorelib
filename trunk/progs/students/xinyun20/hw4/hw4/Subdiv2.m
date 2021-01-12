%{ WARNING: it is VERY rough, and probably wrong!

	Subdiv2 
		-- derived from Subdiv1

%}


classdef Subdiv2 < Subdiv1
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    methods
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        function obj = Subdiv2(x,y,w)
            % Constructor for Subdiv
            obj = obj@Subdiv1(x,y,w);
        end
        
        function children = split(obj,box)
            children = split@split(box);
            obj.setPrincipalNeighbour(children);
        end
        
        function setPrincipalNeighbour(obj, boxes)
            % child(1) = quad1, child2 = quad2, child3= quad3, child4 =
            % quad4
            parent = obj.getBox(boxes(1).parentIdx);
            
            %quad(1) W principal neighbour is quad(2) and hence quad(2)
            %E principal neighbour is quad(1)
            boxes(1).W = cell2mat(parent.child(2));
            boxes(2).E = cell2mat(parent.child(1));
            
            %quad(1) S principal neighbour is quad(4) and hence quad(4)
            %N principal neighbour is quad(1)
            boxes(1).S = cell2mat(parent.child(4));
            boxes(4).N = cell2mat(parent.child(1));
            
            %quad(3) E principal neighbour is quad(4) and hence quad(4)
            %W principal neighbour is quad(3)
            boxes(3).E = cell2mat(parent.child(4));
            boxes(4).W = cell2mat(parent.child(3));
            
            %quad(3) N principal neighbour is quad(2) and hence quad(2)
            %S principal neighbour is quad(3)
            boxes(3).N = cell2mat(parent.child(2));
            boxes(2).S = cell2mat(parent.child(3));
            
            for i = 1:4
                box = boxes(i);
                if(box.x > parent.x && box.y > parent.y )
                    box.E = parent.E;
                    box.N = parent.N;
                    %Principal East Neighbour
                    if(box.E ~=0)
                        PEN = obj.getBox(box.E);
                        if(length(PEN.child) ~= 0)
                            box.E = PEN.child(2);
                        end
                    end
                    %Principal North Neighbour
                    if(box.N ~=0)
                        PNN = obj.getBox(box.N);
                        if(length(PNN.child) ~= 0)
                            box.N = PNN.child(4);
                        end
                    end
                end
                
               if(box.x < parent.x && box.y > parent.y )
                    box.W = parent.W;
                    box.N = parent.N;
                    %Principal West Neighbour
                    if(box.W ~= 0)
                        PWN = obj.getBox(box.W);
                        if(length(PWN.child) ~= 0)
                            box.W = PWN.child(1);
                        end
                    end
                    %Principal North Neighbour
                    if(box.N ~= 0)
                        PNN = obj.getBox(box.N);
                        if(length(PNN.child) ~= 0)
                            box.N = PNN.child(3);
                        end
                    end
               end
                
               if(box.x < parent.x && box.y < parent.y )
                    box.W = parent.W;
                    box.S = parent.S;
                    %Principal West Neighbour
                    if(box.W ~= 0)
                        PWN = obj.getBox(box.W);
                        if(length(PWN.child) ~= 0)
                            box.W = PWN.child(4);
                        end
                    end
                    %Principal South Neighbour
                    if(box.S ~= 0)
                        PSN = obj.getBox(box.S);
                        if(length(PSN.child) ~= 0)
                            box.S = PSN.child(2);
                        end
                    end
               end
               
               if(box.x > parent.x && box.y < parent.y )
                    box.E = parent.E;
                    box.S = parent.S;
                    if(box.E ~= 0)
                        %Principal East Neighbour
                        PEN = obj.getBox(box.E);
                        if(length(PEN.child) ~= 0)
                            box.E = PEN.child(3);
                        end
                    end
                    
                    if(box.S ~= 0)
                        %Principal South Neighbour
                        PSN = obj.getBox(box.S);
                        if(length(PSN.child) ~= 0)
                            box.S = PSN.child(1);
                        end
                    end
                end
            end
        end
        
        function neighbour = findNeighbours(obj, box)
            idx = 0;
            obj.getAllNegihbours(idx,obj.getBox(box.N),box);
            obj.getAllNegihbours(length(box.Neighbour),obj.getBox(box.S),box);
            obj.getAllNegihbours(length(box.Neighbour),obj.getBox(box.E),box);
            obj.getAllNegihbours(length(box.Neighbour),obj.getBox(box.W),box);
            neighbour = box.Neighbour
        end
        
        function getAllNegihbours(obj,idx,rootBox,box)
            child = rootBox.child;
            if(length(child) == 0)
                if((rootBox.N ~= 0 && obj.getBox(rootBox.N) == box) || (rootBox.E ~= 0 && obj.getBox(rootBox.E) == box) || (rootBox.S ~= 0 && obj.getBox(rootBox.S) == box) || (rootBox.W ~= 0 && obj.getBox(rootBox.W) == box))
                    idx = idx+1;
                    box.Neighbour{idx} = rootBox;
                end
            else
                for i = 1:length(child)
                    rootBox = obj.getBox(cell2mat(child(i)));
                    getAllNeighbours(obj,idx,rootBox,box);
                end
            end
        end
        
        function plotLeaf(obj)
             y = [1 1 0];
             r = [1 0 0];
             g = [0 1 0];
             colo = [r; y; g];
            for boxIndex = 1:obj.registerLength
                box = obj.getBox(boxIndex);
                assert(isa(box,'box'),'Not box object', class(box));
                if(box.isLeaf == 1) 
                    x1 = box.x - box.w;
                    x2 = box.x + box.w; 
                    y1 = box.y - box.w;
                    y2 = box.y + box.w;
                    a = [x1, x2, x2, x1, x1];
                    b = [y1, y1, y2, y2, y1];
                    fill(a,b,colo(box.classification + 2,:))
                    plot(a,b,'b-');
                    hold on;
                end
            end
            xlim([obj.RootBox.x - obj.RootBox.w, obj.RootBox.x + obj.RootBox.w]);
            ylim([obj.RootBox.y - obj.RootBox.w, obj.RootBox.y + obj.RootBox.w]);
        end
        function s = testSplit(obj)
            s = Subdiv2(0,0,1);

	    %%%%%%%%%%%%%%%%%%%%%% FIRST SPLIT:
            s.split(1);
            %s.showSubdiv();	% 5 boxes total
            %s.showBox(boxIndex);

	    %%%%%%%%%%%%%%%%%%%%%% SECOND SPLIT:
            boxIndex = s.findBox(0.2,-0.7);    
            s.split(boxIndex);
            %s.showSubdiv();	% 9 boxes total

	    %%%%%%%%%%%%%%%%%%%%%% THIRD SPLIT:
            boxIndex = s.findBox(0.2,-0.7);
            s.split(boxIndex);
            %s.showSubdiv();	% 13 boxes total

            boxIndex = s.findBox(0.1,-0.6);	% boxIndex is 15
            
            neighbours = s.findNeighbours(s.getBox(boxIndex))
            %disp(['-->> findBox(0.2,-0.7) = ', num2str(boxIndex)]);
            %s.showBox(boxIndex);
        end
    end
    methods (Static = true)
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        function test()
            ss = Subdiv2(0,0,1);
            s = ss.testSplit();
            s.plotLeaf();
        end
    
    end
end
