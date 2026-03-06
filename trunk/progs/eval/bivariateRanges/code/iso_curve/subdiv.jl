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
    predicate_C0(polynomial::Polynomial, b::myBox)::Bool

    Checks the C0 predicate for isotopic curve computation.
    Determines if the polynomial has a sign change within the given box,
    which indicates potential presence of the zero-level curve.

    # Arguments
    - `polynomial::Polynomial`: The polynomial to evaluate
    - `b::myBox`: The box region to check

    # Returns
    - `Bool`: true if the polynomial maintains consistent sign (no zero crossing), false otherwise
=#
function predicate_C0(polynomial::Polynomial, b::myBox)::Bool
    global total_degree
    result = lagrange_range_function(polynomial, b, total_degree, method, 6)
    # if b.x_1 == -0.3 && b.x_3 == 0.3 && b.y_1 == 0.2 && b.y_3 == 0.8
    #     benchmark_res = @benchmark lagrange_range_function($polynomial, $b, 18, $method, 6)
    #     display(benchmark_res)
    #     println(result)
    # end
    return result.upper < tol || result.lower > tol
end

#=
    predicate_C1(b::myBox)::Bool

    Checks the C1 predicate for isotopic curve computation.
    Determines if the gradient of the polynomial is greater than the tolerance
    within the given box, indicating potential presence of a zero-level curve.

    # Arguments
    - `b::myBox`: The box region to check

    # Returns
    - `Bool`: true if the gradient exceeds the tolerance, false otherwise
=#
function predicate_C1(b::myBox)::Bool
    grad_x = dx_lagrange(b, method)
    grad_y = dy_lagrange(b, method)

    # if b.x_1 == -0.75 && b.x_3 == -0.5 && b.y_1 == -0.25 && b.y_3 == 0.0
    #     println(grad_x)
    #     println(grad_y)
    # end

    grad_squared_sum = grad_x^2 + grad_y^2
    
    return grad_squared_sum.lower > tol
end

#=
    smooth_split!(b::myBox, polynomial::Polynomial, Q_small::Vector{myBox}, Q1::PriorityQueue{myBox,Int}, Q2::PriorityQueue{myBox,Int}; Δ = 1)

    Splits a box into four children and enqueues them based on the C0 and C1 predicates.
    If the box depth difference exceeds Δ, it moves the box to Q2; otherwise, it enqueues
    it in Q1 or Q_small based on the C0 predicate.

    # Arguments
    - `b`: The box to split
    - `polynomial`: The polynomial defining the curve
    - `Q_small`: Vector to store boxes violating the C0 predicate
    - `Q1`: Priority queue for boxes violating the C1 predicate
    - `Q2`: Priority queue for boxes violating both C0 and C1 predicates
    - `Δ`: Depth difference threshold for box movement (default: 1)
=#
function smooth_split!(b::myBox, polynomial::Polynomial, Q_small::Vector{myBox}, Q_C0::Vector{myBox}, Q1::PriorityQueue{myBox,Int}, Q2::PriorityQueue{myBox,Int}; Δ = 1)
    enqueue!(Q2, b, -b.depth) 

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
    isotopic_connect!(pq::PriorityQueue{myBox,Int})::Tuple{Vector{myBox}, Vector{Tuple{Tuple{Float64,Float64},Tuple{Float64,Float64}}}}

    Connects boxes in the priority queue based on isotopic curve properties.
    Iterates through boxes, identifies intersections, and constructs curve segments.

    # Arguments
    - `pq::PriorityQueue{myBox,Int}`: Priority queue containing boxes to connect

    # Returns
    - `Tuple{Vector{myBox}, Vector{Tuple{Tuple{Float64,Float64},Tuple{Float64,Float64}}}}`: Tuple containing a vector of connected boxes and a vector of curve segments
=#
function isotopic_connect!(pq::PriorityQueue{myBox,Int})::Tuple{Vector{Tuple{Tuple{Float64,Float64},Tuple{Float64,Float64}}}, Vector{myBox}}
    smooth_boxes = Vector{myBox}()

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
                    #&& neighbor in smooth_boxes
                    edge_point = pts_matrix[2, 1]
                    e1 = derivatives[edge_point][1][1]
                    if neighbor.x.upper <= x_2
                        vertex = pts_matrix[1,1]
                        e2 = derivatives[vertex][1][1]
                    else
                        vertex = pts_matrix[3,1]
                        e2 = derivatives[vertex][1][1]
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
            e1 = derivatives[edge_1][1][1]
            e2 = derivatives[edge_2][1][1]
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
                    e1 = derivatives[edge_point][1][1]
                    if neighbor.x.upper <= x_2      
                        vertex = pts_matrix[1,3]
                        e2 = derivatives[vertex][1][1]
                    else
                        vertex = pts_matrix[3,3]
                        e2 = derivatives[vertex][1][1]
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
            e1 = derivatives[edge_1][1][1]
            e2 = derivatives[edge_2][1][1]
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
                    e1 = derivatives[edge_point][1][1]
                    if neighbor.y.upper <= y_2      
                        vertex = pts_matrix[1,1]
                        e2 = derivatives[vertex][1][1]
                    else
                        vertex = pts_matrix[1,3]
                        e2 = derivatives[vertex][1][1]
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
            e1 = derivatives[edge_1][1][1]
            e2 = derivatives[edge_2][1][1]
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
                    e1 = derivatives[edge_point][1][1]
                    if neighbor.y.upper <= y_2      
                        vertex = pts_matrix[3,1]
                        e2 = derivatives[vertex][1][1]
                    else
                        vertex = pts_matrix[3,3]
                        e2 = derivatives[vertex][1][1]
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
            e1 = derivatives[edge_1][1][1]
            e2 = derivatives[edge_2][1][1]
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
            push!(smooth_boxes, b)
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
                elseif double_side == 4  # Right
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
                push!(smooth_boxes, b)
            end
        end
    end
    return segments, smooth_boxes
end  


#= 
    prec_limit(b::myBox, tol::Float64)::Bool

    Checks if the box dimensions are within the tolerance limit.

    # Arguments
    - `b::myBox`: The box to check
    - `tol::Float64`: Tolerance value

    # Returns
    - `Bool`: true if box dimensions are within tolerance, false otherwise
=#
function prec_limit(b::myBox, tol::Float64)::Bool
    (b.x.upper - b.x.lower < tol) && (b.y.upper - b.y.lower < tol)
end
    
#= 
    isotopic_curve(polynomial::Polynomial, boxes::Union{Vector{myBox}, myBox})::Tuple{PriorityQueue{myBox,Int}, Vector{myBox}, Vector{myBox}}

    Generates an isotopic curve using subdivision.

    # Arguments
    - `polynomial::Polynomial`: The polynomial defining the curve
    - `boxes`: Initial boxes or a single box
    - `tol::Float64`: Tolerance for precision limit (default: 1e-5)

    # Returns
    - `Tuple{PriorityQueue{myBox,Int}, Vector{myBox}, Vector{myBox}}`: Tuple containing the priority queue, smooth boxes, and small boxes
=#
function isotopic_curve(polynomial::Polynomial, boxes::Union{Vector{myBox}, myBox})::Tuple{PriorityQueue{myBox,Int}, Vector{myBox}, Vector{myBox}, Vector{myBox}}
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
        if predicate_C0(polynomial, b)
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
    Q2 = PriorityQueue{myBox,Int}()

    while(!isempty(Q1))
        b = dequeue!(Q1)
        smooth_split!(b, polynomial, Q_small, Q_C0, Q1, Q2)
    end

    smooth_boxes = collect(keys(Q2))

    total_boxes = length(smooth_boxes)
    C0_boxes = length(Q_C0)

    return Q2, smooth_boxes, Q_small, Q_C0
end

#= 
    (For testing)
    uniform_split(b::myBox, poly::Polynomial)::PriorityQueue{myBox, Int}

    Splits a box into 1024 equal parts and enqueues them with depth priority.

    # Arguments
    - `b::myBox`: The box to split
    - `poly::Polynomial`: The polynomial defining the curve

    # Returns
    - `PriorityQueue{myBox, Int}`: Priority queue containing the split boxes
=#
function uniform_split(b::myBox, dim::Int)::PriorityQueue{myBox, Int}
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
    (For testing)
    evaluate_boxes(q::PriorityQueue{myBox, Int}, poly::Polynomial)

    Evaluates the boxes in the priority queue using the lagrange_range_function.

    # Arguments
    - `q::PriorityQueue{myBox, Int}`: Priority queue containing the boxes to evaluate
    - `poly::Polynomial`: The polynomial defining the curve
=#
function evaluate_boxes(q::PriorityQueue{myBox, Int}, poly::Polynomial; sharing::Bool = true)
    global total_degree
    global method

    total_width = 0.0
    for b in keys(q)
        if !sharing
            reset_global_state!()
        end
        range = lagrange_range_function(poly, b, total_degree, method, 6; sharing=sharing)
        curr_width = range.upper - range.lower
        total_width += curr_width
    end

    return total_width
end

