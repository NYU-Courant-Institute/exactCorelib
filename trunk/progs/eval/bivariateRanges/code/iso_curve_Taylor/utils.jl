#= 
    @author: Thomas Chen
    @advisors: Yap Chee, Kai Hormann, Bingwei Zhang
    @date: 2025-06-20
    @description: Utility functions
=# 
using DataStructures
using GLMakie




#=
    eval_slp(slp::SLP, x::Float64, y::Float64, order::String)::Float64

    Evaluates the SLP polynomial at a given point with a specified order.

    # Arguments
    - `slp::SLP`: The SLP polynomial
    - `x::Float64`: The x-coordinate
    - `y::Float64`: The y-coordinate
    - `order::String`: The order of the derivative

    # Returns
    - `::Float64`: The evaluated value
=#
function eval_slp(slp::SLP, x::Float64, y::Float64, order::String)::Float64
    global derivatives_taylor 
    point_key = (x, y)

    if haskey(derivatives_taylor , point_key)
        x_order, y_order = parse_monomial_key(order)
        
        cached_derivatives = derivatives_taylor[point_key]
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
    plot_boxes(title::String, slp::SLP, boxes::Vector{myBox}, dim::myBox, segments::Vector{Tuple{Tuple{Float64,Float64},Tuple{Float64,Float64}}}, smooth_boxes::Vector{myBox}, eps_boxes::Vector{myBox}; levels=[0.0])::Figure

    Plots the boxes and segments on a figure.

    # Arguments
    - `title::String`: The title of the plot
    - `slp::SLP`: The SLP polynomial
    - `boxes::Vector{myBox}`: The boxes to plot
    - `dim::myBox`: The dimension of the plot
    - `segments::Vector{Tuple{Tuple{Float64,Float64},Tuple{Float64,Float64}}}`: The segments to plot
    - `smooth_boxes::Vector{myBox}`: The smooth boxes to plot
    - `eps_boxes::Vector{myBox}`: The eps boxes to plot
    
    # Returns
    - `::Figure`: The figure object
=#
function plot_boxes(title::String, slp::SLP, boxes::Vector{myBox}, dim::myBox, segments::Vector{Tuple{Tuple{Float64,Float64},Tuple{Float64,Float64}}}, smooth_boxes::Vector{myBox}, eps_boxes::Vector{myBox}, Q_C0::Vector{myBox}; levels=[0.0])::Figure
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

    Parses a line of input and returns a tuple of polynomial, variables dictionary, and description.

    # Arguments
    - `line::String`: The input line to parse

    # Returns
    - `::Tuple{String, OrderedDict{Symbol, Union{Float64, myInterval}}, String}`: A tuple containing polynomial, variables dictionary, and description
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
    parse_monomial_key(key::String)::Tuple{Int, Int}

    Parses a monomial key and returns the order of x and y.

    # Arguments
    - `key::String`: The monomial key to parse

    # Returns
    - `::Tuple{Int, Int}`: A tuple containing the order of x and y
=#
function parse_monomial_key(key::String)::Tuple{Int, Int}
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

    Generates a monomial string based on the given orders of x and y.

    # Arguments
    - `ordX::Integer`: The order of x
    - `ordY::Integer`: The order of y

    # Returns
    - `::String`: The generated monomial string
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
    get_point(B::myBox, i::Int, j::Int)::Vector{Float64}

    Retrieves the point at the specified indices in the points matrix of a myBox.

    # Arguments
    - `B::myBox`: The myBox object
    - `i::Int`: The index of the point in the first dimension
    - `j::Int`: The index of the point in the second dimension

    # Returns
    - `::Vector{Float64}`: The point at the specified indices
=#
function get_point(B::myBox, i::Int, j::Int)::Vector{Float64}
    return B.pts_matrix[i, j]
end


#=
    reset_global_state!()

    Resets the global state variables.
=#
function reset_global_state!()
    global derivatives_taylor
    global total_boxes
    global total_eval
    global total_points
    global C0_boxes
    derivatives_taylor = Dict{Tuple{Float64, Float64}, Vector{Vector{Float64}}}()
    total_boxes = 0
    total_eval = 0
    total_points = 0
    C0_boxes = 0
end


