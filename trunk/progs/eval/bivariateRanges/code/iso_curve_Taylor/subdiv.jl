
#= 
    @author: Thomas Chen
    @advisors: Yap Chee, Kai Hormann, Bingwei Zhang
    @date: 2025-06-20
    @description: Subdivision and line connection

    The main function here are:
        - isotopic_curve(polynomial, boxes; tol = 1e-5): Function that subdivides the given boxes 
        and computes C0 and C1 predicates on the given polynomial to determine whether a zero-level 
        curve exists within the box.
        - isotopic_connect(pq): Function that connects the zero-level curve to the given boxes.
            - Uses midpoint directly, no interpolation done
=# 

#=
    split_box(b::myBox)::Vector{myBox}

    Splits the box into four equal parts.

    # Arguments
    - `b::myBox`: The box to split

    # Returns
    - `::Vector{myBox}`: A vector of four boxes, each representing a quarter of the original box
=#
function split_box(b::myBox)::Vector{myBox}
    mx = (b.x.lower + b.x.upper) / 2
    my = (b.y.lower + b.y.upper) / 2

    x1 = myInterval(b.x.lower, mx)
    x2 = myInterval(mx, b.x.upper)
    y1 = myInterval(b.y.lower, my)
    y2 = myInterval(my, b.y.upper)

    d  = b.depth + 1

    children = [
        myBox(x1, y1; depth=d),
        myBox(x1, y2; depth=d),
        myBox(x2, y1; depth=d),
        myBox(x2, y2; depth=d)
    ]

    internal_neighbors!(children)

    external_neighbors!(b, children)

    return children
end

#=
    predicate_C0(poly::Polynomial, b::myBox)::Bool

    Predicate function for checking if the Taylor polynomial interpolation of the polynomial at the center point of the box is zero.

    # Arguments
    - `poly::Polynomial`: The polynomial
    - `b::myBox`: The box

    # Returns
    - `::Bool`: True if the interpolation is zero, false otherwise
=#
function predicate_C0(poly::Polynomial, b::myBox)::Bool
    global total_degree
    result = taylor_interpolation(poly, b, total_degree)
    # if b.x_1 == -0.5 && b.x_3 == 1 && b.y_1 == -7 && b.y_3 == -5.5
    #     println("Result = $result")
    # end
    return result.upper < -tol || result.lower > tol
end

#=
    predicate_C1(b::myBox)::Bool

    Predicate function for checking if the gradient of the Taylor polynomial interpolation of the polynomial at the center point of the box is non-zero.

    # Arguments
    - `b::myBox`: The box

    # Returns
    - `::Bool`: True if the gradient is non-zero, false otherwise
=#
function predicate_C1(b::myBox)::Bool
    global total_degree
    grad_x = taylor_interpolation_dx(b, total_degree)
    grad_y = taylor_interpolation_dy(b, total_degree)

    # Calculate myInterval for grad_x^2 + grad_y^2
    grad_squared_sum = grad_x^2 + grad_y^2

    # if b.x_1 == -0.5 && b.x_3 == 1 && b.y_1 == -7 && b.y_3 == -5.5
    #     println(grad_x)
    #     println(grad_y)
    #     println(grad_squared_sum)
    # end
    
    # Exclude box only if gradient is definitely non-zero
    return grad_squared_sum.lower > tol
end

#=
    precondition: C_1(b) is true 
    smooth_split!(b::myBox, polynomial::Polynomial, Q_small::Vector{myBox}, Q1::PriorityQueue{myBox,Int}, Q2::PriorityQueue{myBox,Int}; Δ::Int = 1) 

    # Arguments
    - `b::myBox`: The box
    - `polynomial::Polynomial`: The polynomial
    - `Q_small::Vector{myBox}`: The small queue
    - `Q1::PriorityQueue{myBox,Int}`: The priority queue 1
    - `Q2::PriorityQueue{myBox,Int}`: The priority queue 2
    - `Δ::Int=1`: The delta value (default: 1)
=#
function smooth_split!(b::myBox, polynomial::Polynomial, Q_small::Vector{myBox}, Q_C0::Vector{myBox}, Q1::PriorityQueue{myBox,Int}, Q2::PriorityQueue{myBox,Int}; Δ::Int = 1) 
    global leaf_boxes
    enqueue!(Q2, b, -b.depth)  # Add b to Q2 first, like in the original function

    all_neighbors = vcat(b.up, b.down, b.left, b.right)
    
    for neighbor in all_neighbors
        if !haskey(Q1, neighbor) || neighbor === b
            continue
        end

        if abs(b.depth - neighbor.depth) > Δ 
            if b.depth> neighbor.depth
                delete!(Q1, neighbor)
                children = split_box(neighbor) 
                for child in children 
                    if predicate_C0(polynomial, child) 
                        push!(Q_C0, child)
                        continue 
                    elseif prec_limit(child, tol)
                        push!(Q_small, child) 
                    else 
                        enqueue!(Q1, child, -child.depth) 
                    end 
                end
            end
        end 
    end 
end

#=
    isotopic_connect!(pq::PriorityQueue{myBox,Int}, slp::SLP)::Tuple{Vector{Tuple{Tuple{Float64,Float64},Tuple{Float64,Float64}}}, Vector{myBox}}

    Connects the boxes in the priority queue using the SLP polynomial.

    # Arguments
    - `pq::PriorityQueue{myBox,Int}`: The priority queue of boxes
    - `slp::SLP`: The SLP polynomial

    # Returns
    - `::Tuple{Vector{myBox},Vector{Tuple{Tuple{Float64,Float64},Tuple{Float64,Float64}}}}`: A tuple containing the final boxes and segments
=#
function isotopic_connect!(pq::PriorityQueue{myBox,Int}, slp::SLP)::Tuple{Vector{Tuple{Tuple{Float64,Float64},Tuple{Float64,Float64}}}, Vector{myBox}}
    final_boxes = Vector{myBox}()
    segments = Vector{Tuple{Tuple{Float64,Float64},Tuple{Float64,Float64}}}()

    while !isempty(pq)
        b = dequeue!(pq)

        pts_matrix = b.pts_matrix
        x_1 = b.x_1
        x_2 = b.x_2
        x_3 = b.x_3
        y_1 = b.y_1
        y_2 = b.y_2
        y_3 = b.y_3


        cross = Vector{Tuple{Float64, Float64}}()

        sides = Vector{Int}()

        neighbor_bottom_cross = false
        if !isempty(b.down)
            for neighbor in b.down
                if neighbor.depth > b.depth 
                    edge_point = pts_matrix[2, 1]
                    e1 = eval_slp(slp, edge_point[1], edge_point[2], "")
                    if neighbor.x.upper <= x_2
                        vertex = pts_matrix[1,1]
                        e2 = eval_slp(slp, vertex[1], vertex[2], "")
                    else
                        vertex = pts_matrix[3,1]
                        e2 = eval_slp(slp, vertex[1], vertex[2], "")
                    end
                    if (abs(e1) <= tol)
                        neighbor_bottom_cross = true
                        push!(cross, edge_point)
                        push!(sides, 1)
                    end
                    if (abs(e2) <= tol)
                        neighbor_bottom_cross = true
                        push!(cross, vertex)
                        push!(sides, 1)
                    end
                    if (e1 * e2 < -tol) && !neighbor_bottom_cross
                        neighbor_bottom_cross = true
                        mid = ((vertex[1] + x_2) / 2 , y_1)
                        push!(cross, mid)
                        push!(sides, 1)
                    end
                end 
            end
        end

        if !neighbor_bottom_cross
            bottom_cross = false
            edge_1 = pts_matrix[1,1]
            edge_2 = pts_matrix[3,1]
            e1 = eval_slp(slp, edge_1[1], edge_1[2], "")
            e2 = eval_slp(slp, edge_2[1], edge_2[2], "")
            if (abs(e1) <= tol)
                bottom_cross = true
                push!(cross, edge_1)
                push!(sides, 1)
            end
            if (abs(e2) <= tol)
                bottom_cross = true
                push!(cross, edge_2)
                push!(sides, 1)
            end
            if (e1 * e2 < -tol) && !bottom_cross
                push!(cross, pts_matrix[2,1])
                push!(sides, 1)
            end
        end

        
        neighbor_top_cross = false
        if !isempty(b.up)
            for neighbor in b.up
                if neighbor.depth > b.depth 
                    #&& neighbor in smooth_boxes
                    edge_point = pts_matrix[2, 3]
                    e1 = eval_slp(slp, edge_point[1], edge_point[2], "")
                    if neighbor.x.upper <= x_2      
                        vertex = pts_matrix[1,3]
                        e2 = eval_slp(slp, vertex[1], vertex[2], "")
                    else
                        vertex = pts_matrix[3,3]
                        e2 = eval_slp(slp, vertex[1], vertex[2], "")
                    end
                    if (abs(e1) <= tol)
                        neighbor_top_cross = true
                        push!(cross, edge_point)
                        push!(sides, 2)
                    end
                    if (abs(e2) <= tol)
                        neighbor_top_cross = true
                        push!(cross, vertex)
                        push!(sides, 2)
                    end
                    if (e1 * e2 < -tol) && !neighbor_top_cross
                        neighbor_top_cross = true
                        mid = ((vertex[1] + x_2) / 2 , y_3)
                        push!(cross, mid)
                        push!(sides, 2)
                    end
                end
            end
        end
        if !neighbor_top_cross
            top_cross = false
            edge_1 = pts_matrix[1,3]
            edge_2 = pts_matrix[3,3]
            e1 = eval_slp(slp, edge_1[1], edge_1[2], "")
            e2 = eval_slp(slp, edge_2[1], edge_2[2], "")
            if (abs(e1) <= tol)
                top_cross = true
                push!(cross, edge_1)
                push!(sides, 2)
            end
            if (abs(e2) <= tol)
                top_cross = true
                push!(cross, edge_2)
                push!(sides, 2)
            end
            if (e1 * e2 < -tol) && !top_cross
                push!(cross, pts_matrix[2, 3])
                push!(sides, 2)
            end
        end

        neighbor_left_cross = false
        if !isempty(b.left)
            for neighbor in b.left
                if neighbor.depth > b.depth 
                    #&& neighbor in smooth_boxes
                    edge_point = pts_matrix[1, 2]
                    e1 = eval_slp(slp, edge_point[1], edge_point[2], "")
                    if neighbor.y.upper <= y_2      
                        vertex = pts_matrix[1,1]
                        e2 = eval_slp(slp, vertex[1], vertex[2], "")
                    else
                        vertex = pts_matrix[1,3]
                        e2 = eval_slp(slp, vertex[1], vertex[2], "")
                    end
                    if (abs(e1) <= tol)
                        neighbor_left_cross = true
                        push!(cross, edge_point)
                        push!(sides, 3)
                    end
                    if (abs(e2) <= tol)
                        neighbor_left_cross = true
                        push!(cross, vertex)
                        push!(sides, 3)
                    end
                    if (e1 * e2 < -tol) && !neighbor_left_cross
                        neighbor_left_cross = true
                        mid = (x_1, (vertex[2] + y_2) / 2)
                        push!(cross, mid)
                        push!(sides, 3)
                    end
                end
            end
        end

        if !neighbor_left_cross
            left_cross = false
            edge_1 = pts_matrix[1,1]
            edge_2 = pts_matrix[1,3]
            e1 = eval_slp(slp, edge_1[1], edge_1[2], "")
            e2 = eval_slp(slp, edge_2[1], edge_2[2], "")
            if (abs(e1) <= tol)
                left_cross = true
                push!(cross, edge_1)
                push!(sides, 3)
            end
            if (abs(e2) <= tol)
                left_cross = true
                push!(cross, edge_2)
                push!(sides, 3)
            end
            if (e1 * e2 < -tol) && !left_cross
                push!(cross, pts_matrix[1, 2])
                push!(sides, 3)
            end
        end

        neighbor_right_cross = false
        if !isempty(b.right)
            for neighbor in b.right
                if neighbor.depth > b.depth 
                    #&& neighbor in smooth_boxes
                    edge_point = pts_matrix[3, 2]
                    e1 = eval_slp(slp, edge_point[1], edge_point[2], "")
                    if neighbor.y.upper <= y_2      
                        vertex = pts_matrix[3,1]
                        e2 = eval_slp(slp, vertex[1], vertex[2], "")
                    else
                        vertex = pts_matrix[3,3]
                        e2 = eval_slp(slp, vertex[1], vertex[2], "")
                    end
                    if (abs(e1) <= tol)
                        neighbor_right_cross = true
                        push!(cross, edge_point)
                        push!(sides, 4)
                    end
                    if (abs(e2) <= tol)
                        neighbor_right_cross = true
                        push!(cross, vertex)
                        push!(sides, 4)
                    end
                    if (e1 * e2 < -tol) && !neighbor_right_cross
                        neighbor_right_cross = true
                        mid = (x_3, (vertex[2] + y_2) / 2)
                        push!(cross, mid)
                        push!(sides, 4)
                    end
                end
            end
        end

        if !neighbor_right_cross
            right_cross = false
            edge_1 = pts_matrix[3,1]
            edge_2 = pts_matrix[3,3]
            e1 = eval_slp(slp, edge_1[1], edge_1[2], "")
            e2 = eval_slp(slp, edge_2[1], edge_2[2], "")
            if (abs(e1) <= tol)
                right_cross = true
                push!(cross, edge_1)
                push!(sides, 4)
            end
            if (abs(e2) <= tol)
                right_cross = true
                push!(cross, edge_2)
                push!(sides, 4)
            end
            if (e1 * e2 < -tol) && !right_cross
                push!(cross, pts_matrix[3, 2])
                push!(sides, 4)
            end
        end

        n = length(cross)

        if n == 2
            push!(segments, (cross[1], cross[2]))
            push!(final_boxes, b)
        else
            side_counts = [0,0,0,0]
            for side in sides
                side_counts[side] += 1
            end

            double_side = findfirst(x -> x == 2, side_counts)

            if double_side !== nothing
                clockwise_points = []
                side_sequence = []
                
                if double_side == 1      
                    side_sequence = [1, 3, 2, 4]  
                elseif double_side == 2  
                    side_sequence = [2, 4, 1, 3]  
                elseif double_side == 3  
                    side_sequence = [3, 2, 4, 1]  
                elseif double_side == 4 
                    side_sequence = [4, 1, 3, 2]  
                end
                
                for side in side_sequence
                    side_points = [cross[i] for i in 1:length(cross) if sides[i] == side]
                    if !isempty(side_points)
                        if side == 1   
                            sort!(side_points, by = p -> -p[1]) 
                        elseif side == 2  
                            sort!(side_points, by = p -> p[1])   
                        elseif side == 3  
                            sort!(side_points, by = p -> p[2])  
                        elseif side == 4  
                            sort!(side_points, by = p -> -p[2])   
                        end
                        
                        
                        append!(clockwise_points, side_points)
                    end
                end

                push!(segments, (clockwise_points[1], clockwise_points[4])) 
                push!(segments, (clockwise_points[2], clockwise_points[3]))
                push!(final_boxes, b)
            end
        end
    end
    return segments, final_boxes
end  


#=
    prec_limit(b::myBox, tol::Float64)::Bool

    Determines if the box is within the precision limit.

    # Arguments
    - `b::myBox`: The box
    - `tol::Float64`: The precision limit

    # Returns
    - `::Bool`: True if the box is within the precision limit, false otherwise
=#
function prec_limit(b::myBox, tol::Float64)::Bool
    (b.x.upper - b.x.lower < tol) && (b.y.upper - b.y.lower < tol)
end
    

#=
    isotopic_curve(poly::Polynomial, boxes::Vector{myBox})::Tuple{PriorityQueue{myBox,Int}, Vector{myBox}, Vector{myBox}}

    Computes the isotopic curve of a polynomial using a given set of boxes.

    # Arguments
    - `poly::Polynomial`: The polynomial
    - `boxes`: The initial boxes

    # Returns
    - `::Tuple{PriorityQueue{myBox,Int}, Vector{myBox}, Vector{myBox}}`: A tuple containing the priority queue of boxes, the final boxes, and the boxes that reach the precision limit
=#
function isotopic_curve(poly::Polynomial, boxes::Union{Vector{myBox}, myBox})::Tuple{PriorityQueue{myBox,Int}, Vector{myBox}, Vector{myBox}, Vector{myBox}}
    global total_boxes
    global C0_boxes

    #Initial boxes
    Q0 = isa(boxes, Vector) ? copy(boxes) : [boxes]
    #Stores boxes that reaches tol 
    Q_small = myBox[]

    Q_C0 = myBox[]

    #Boxes that pass C1(A PriorityQueue with priority based on depth)
    Q1 = PriorityQueue{myBox, Int}() 


    # Phase 1: Q0 -> Q1 + Q_small
    while !isempty(Q0)
        b = popfirst!(Q0)

        # Skip if C0 predicate is true (no roots possible)
        if predicate_C0(poly, b)
            push!(Q_C0, b)
            continue
        end 

        if prec_limit(b, tol)                
            push!(Q_small, b)         
        elseif predicate_C1(b)            
            enqueue!(Q1, b, -b.depth)
        else                       
            append!(Q0, split_box(b))
        end
    end

    #Phase 2: Q1 -> Q2 + Q_small(additional entries)
    #Q2 stores smooth boxes
    Q2 = PriorityQueue{myBox,Int}()

    while(!isempty(Q1))
        b = dequeue!(Q1)
        smooth_split!(b, poly, Q_small, Q_C0, Q1, Q2)
    end

    smooth_boxes = collect(keys(Q2))

    total_boxes = length(smooth_boxes)

    C0_boxes = length(Q_C0)

    return Q2, smooth_boxes, Q_small, Q_C0
end

#=

    uniform_split(b::myBox)::PriorityQueue{myBox,Int}

    Splits a box into 1024 equal parts.

    # Arguments
    - `b::myBox`: The box to split

    # Returns
    - `::PriorityQueue{myBox,Int}`: A priority queue containing the 4 new boxes
=#
function uniform_split(b::myBox, dim::Int)::PriorityQueue{myBox,Int}
    box_queue = PriorityQueue{myBox, Int}()
    enqueue!(box_queue, b, b.depth)

    while length(box_queue) < dim
        box_to_split = dequeue!(box_queue)
        children = split_box(box_to_split)

        for child in children 
            enqueue!(box_queue, child, child.depth)
        end
    end
    return box_queue
end

#=
    evaluate_boxes(q::PriorityQueue{myBox, Int}, poly::Polynomial)

    Evaluates the boxes in a priority queue using a given polynomial.

    # Arguments
    - `q::PriorityQueue{myBox, Int}`: The priority queue of boxes
    - `poly::Polynomial`: The polynomial to evaluate
=#
function evaluate_boxes(q::PriorityQueue{myBox, Int}, poly::Polynomial; sharing::Bool = true)
    global total_degree

    total_width = 0.0
    for b in keys(q)
        if !sharing
            reset_global_state!()
        end
        range = taylor_interpolation(poly, b, total_degree, sharing=sharing)
        curr_width = range.upper - range.lower
        total_width += curr_width
    end

    return total_width
end