#= 
    @author: Thomas Chen
    @advisors: Yap Chee, Kai Hormann, Bingwei Zhang
    @date: 2025-06-03
    @description: Utility functions
=# 

#= 
    eval_slp(slp::SLP, x::Float64, y::Float64, order::String)::Float64

    Evaluates the polynomial with the given order at the specified point (x, y).

    # Arguments
    - `slp`: The polynomial to evaluate
    - `x`: The x-coordinate at which to evaluate the polynomial
    - `y`: The y-coordinate at which to evaluate the polynomial
    - `order`: The order of the derivative to evaluate (e.g., "d2xdy2", "d2ydx2")

    # Returns
    - `Float64`: The result of the polynomial evaluation
=#
function eval_slp(slp::SLP, x::Float64, y::Float64, order::String)::Float64
    global derivatives
    point_key = (x, y)

    if haskey(derivatives, point_key)
        x_order, y_order = parse_monomial_key(order)
        
        cached_derivatives = derivatives[point_key]
        if x_order + 1 <= length(cached_derivatives) && y_order + 1 <= length(cached_derivatives[x_order + 1])
            return cached_derivatives[x_order + 1][y_order + 1]
        end
    end


    vars = Dict{Symbol,Union{Float64,myInterval}}()
    vars[:x] = x
    vars[:y] = y
    return evaluate_slp_range(slp, order, vars)
end

#= 
    plot_boxes(title::String, slp::SLP, boxes::Vector{myBox}, dim::myBox, segments::Vector{Tuple{Tuple{Float64,Float64},Tuple{Float64,Float64}}}, smooth_boxes, eps_boxes; levels=[0.0])::Figure

    Plots the boxes and the given segments on a 2D axis.

    # Arguments
    - `title`: The title of the plot
    - `slp`: The polynomial to plot
    - `boxes`: A vector of myBox objects representing the boxes to plot
    - `dim`: The dimension of the plot
    - `segments`: A vector of segments to plot
    - `smooth_boxes`: A vector of myBox objects representing the smooth boxes to plot
    - `eps_boxes`: A vector of myBox objects representing the eps boxes to plot
    - `levels`: A vector of levels for the contour plot (default: [0.0])

    # Returns
    - `Figure`: The resulting figure object
=#
function plot_boxes(title::String, slp::SLP, boxes::Vector{myBox}, dim::myBox, segments::Vector{Tuple{Tuple{Float64,Float64},Tuple{Float64,Float64}}}, smooth_boxes, eps_boxes, Q_C0; levels=[0.0])::Figure
    fig = Figure(size = (800, 800))
    ax  = Axis(fig[1, 1]; title = title, xlabel = "x", ylabel = "y", aspect = DataAspect())

    if DEBUG_LEVEL >= 5
        for b in smooth_boxes
            x1, x2 = b.x.lower, b.x.upper
            y1, y2 = b.y.lower, b.y.upper
            xs_rect = [x1, x1, x2, x2, x1]
            ys_rect = [y1, y2, y2, y1, y1]
            poly!(ax, xs_rect, ys_rect;
            color       = (:green, 0.5),
            strokecolor = :black,
            strokewidth = 1)
        end
    end

    if DEBUG_LEVEL >= 6
        for b in eps_boxes
            x1, x2 = b.x.lower, b.x.upper
            y1, y2 = b.y.lower, b.y.upper
            xs_rect = [x1, x1, x2, x2, x1]
            ys_rect = [y1, y2, y2, y1, y1]
            poly!(ax, xs_rect, ys_rect;
                color       = (:black, 0.5),
                strokecolor = :black,
                strokewidth = 1)
        end
        for b in Q_C0
            x1, x2 = b.x.lower, b.x.upper
            y1, y2 = b.y.lower, b.y.upper
            xs_rect = [x1, x1, x2, x2, x1]
            ys_rect = [y1, y2, y2, y1, y1]
            poly!(ax, xs_rect, ys_rect;
                color       = (:gray, 0.15),
                strokecolor = :black,
                strokewidth = 1)
        end
    end

    if DEBUG_LEVEL >= 4
        for b in boxes
            x1, x2 = b.x.lower, b.x.upper
            y1, y2 = b.y.lower, b.y.upper
            xs_rect = [x1, x1, x2, x2, x1]
            ys_rect = [y1, y2, y2, y1, y1]
            poly!(ax, xs_rect, ys_rect;
            color       = (:yellow, 0.5),
            strokecolor = :black,
            strokewidth = 1)
        end

        xs = range(dim.x.lower, dim.x.upper; length=(round(Int, dim.x.upper - dim.x.lower) + 1) * 50)
        ys = range(dim.y.lower, dim.y.upper; length=(round(Int, dim.y.upper - dim.y.lower) + 1) * 50)
        Z = [ eval_slp(slp, x, y, "") for x in xs, y in ys ]
        
        contour!(ax, xs, ys, Z; levels = levels, linewidth = 2, color = :red)

        for segment in segments
            start_point, end_point = segment
            x_coords = [start_point[1], end_point[1]]
            y_coords = [start_point[2], end_point[2]]
            lines!(ax, x_coords, y_coords; color = :blue, linewidth = 3)
        end
    end

    fig 
end

#=
    parse_line(line::String)::Tuple{String, OrderedDict{Symbol, Union{Float64, myInterval}}, String}
     
    Function to parse the input file line by line and extract the desired parts.
    This version supports variable substitutions that are either numbers (integers or floats)
    or intervals in the form "[a,b]".

    # Arguments
    - `line`: The input line to parse

    # Returns
    - `Tuple{String, OrderedDict{Symbol, Union{Float64, myInterval}}, String}`: A tuple containing the polynomial, a dictionary of variable substitutions, and a description
=#
function parse_line(line::String)::Tuple{String, OrderedDict{Symbol, Union{Float64, myInterval}}, String}
    tokens = String[]
    current = IOBuffer()
    in_brackets = false
    in_quotes = false

    for c in line
        if c == '"'
            in_quotes = !in_quotes
            write(current, c)
        elseif c == '[' && !in_quotes
            in_brackets = true
            write(current, c)
        elseif c == ']' && !in_quotes
            in_brackets = false
            write(current, c)
        elseif c == ',' && !in_brackets && !in_quotes
            token = String(take!(current))
            push!(tokens, strip(token))
        else
            write(current, c)
        end
    end

    token = String(take!(current))
    if !isempty(strip(token))
        push!(tokens, strip(token))
    end

    if isempty(tokens)
        error("No tokens found in input line")
    end

    poly = tokens[1]

    varsDict = OrderedDict{Symbol,Union{Float64,myInterval}}()
    i = 2

    while i <= length(tokens) && occursin("=", tokens[i])
        if occursin("=", tokens[i])
            parts = split(tokens[i], "=")
            var = Symbol(strip(parts[1]))
            valstr = strip(parts[2])

            if startswith(valstr, "[") && endswith(valstr, "]")
                inner = valstr[2:end-1]
                splitVals = split(inner, ",")
                if length(splitVals) != 2
                    error("myInterval for variable $(var) must have two endpoints")
                end
                a = parse(Float64, strip(splitVals[1]))
                b = parse(Float64, strip(splitVals[2]))
                varsDict[var] = myInterval(a, b)
            else
                numVal = parse(Float64, valstr)
                varsDict[var] = numVal
            end
        end
        i += 1
    end

    description = ""
    if i <= length(tokens)
        description = join(tokens[i:end], ",")
        description = replace(description, "\"" => "")
    end

    return (poly, varsDict, description)
end

#= 
    parse_monomial_key(key::String)::Tuple{Integer, Integer}

    Function to parse the monomial key and extract the degree in x and y directions.

    # Arguments
    - `key`: The input monomial key to parse

    # Returns
    - `Tuple{Integer, Integer}`: A tuple containing the degree in x and y directions
=#
function parse_monomial_key(key::String)::Tuple{Integer, Integer}
    ordX = 0
    ordY = 0
    if occursin(r"\bx\^(\d+)", key)
        m = match(r"\bx\^(\d+)", key)
        ordX = parse(Int, m.captures[1])
    elseif occursin(r"\bx\b", key)
        ordX = 1
    end
    if occursin(r"\by\^(\d+)", key)
        m = match(r"\by\^(\d+)", key)
        ordY = parse(Int, m.captures[1])
    elseif occursin(r"\by\b", key)
        ordY = 1
    end
    return ordX, ordY
end

#= 
    monomial_string(ordX::Integer, ordY::Integer)::String

    Function to generate a string representation of a monomial based on the given degrees in x and y directions.

    # Arguments
    - `ordX`: The degree in x direction
    - `ordY`: The degree in y direction

    # Returns
    - `String`: The string representation of the monomial
=#
function monomial_string(ordX::Integer, ordY::Integer)::String
    parts = String[]

    if ordX == 1
        push!(parts, "x")
    elseif ordX > 1
        push!(parts, "x^" * string(ordX))
    end

    if ordY == 1
        push!(parts, "y")
    elseif ordY > 1
        push!(parts, "y^" * string(ordY))
    end

    return isempty(parts) ? "" : join(parts, "")
end

#=
    get_deriv_nine(B::myBox, i::Int, j::Int)::Vector{Float64}

    Function to compute the nine derivatives of a given point in a box region.

    # Arguments
    - `B`: The box region
    - `i`: The index of the point in the x direction
    - `j`: The index of the point in the y direction

    # Returns
    - `Vector{Float64}`: A vector containing the nine derivative values
=#
function get_deriv_nine(B::myBox, i::Int, j::Int)::Vector{Float64}
    derivative_values = Float64[]
    for pt_i in 1:3
        for pt_j in 1:3
            pt = get_point(B, pt_i, pt_j)
            if haskey(derivatives, pt)
                if i <= length(derivatives[pt]) && j <= length(derivatives[pt][i])
                    push!(derivative_values, derivatives[pt][i][j])
                else
                    push!(derivative_values, 0.0)
                end
            else
                push!(derivative_values, 0.0)
            end
        end
    end

    return derivative_values
end

#=
    get_point(B::myBox, i::Int, j::Int)::Tuple{Float64, Float64}

    Function to retrieve a specific point from the box region.

    # Arguments
    - `B`: The box region
    - `i`: The index of the point in the x direction
    - `j`: The index of the point in the y direction

    # Returns
    - `Tuple{Float64, Float64}`: A tuple containing the coordinates of the point
=#
function get_point(B::myBox, i::Int, j::Int)::Tuple{Float64, Float64}
    return B.pts_matrix[i, j]
end

#= 
    reset_global_state!()

    Resets the global state by clearing the derivatives dictionary.
=#
function reset_global_state!()
    global derivatives
    global total_boxes
    global total_points
    global total_eval
    global C0_boxes
    derivatives = Dict{Tuple{Float64, Float64}, Vector{Vector{Float64}}}()
    total_boxes = 0
    total_points = 0
    total_eval = 0
    C0_boxes = 0
end

function clear_box_caches!(box::myBox)
    fill!(box.FB, nothing)
    for i in eachindex(box.QB)
        box.QB[i] = []
    end
end
