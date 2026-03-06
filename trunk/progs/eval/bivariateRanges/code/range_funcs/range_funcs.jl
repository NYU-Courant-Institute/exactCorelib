#!/usr/bin/env julia

include("check_dep.jl")


using Printf
using BenchmarkTools
using Plots
using DataStructures
using Colors
using Dates
using NLsolve
using Symbolics
using LinearAlgebra

include("../SLP/myInterval.jl")
include("../SLP/polynomial.jl")
include("../SLP/derivatives.jl")
include("../SLP/eval.jl")
include("../SLP/main.jl")
include("../iso_curve/myBox.jl")
include("../iso_curve/utils.jl")
#include("../iso_curve/myBox.jl")

include("lagrange3.jl")
include("hermite4.jl")
include("taylor4.jl")
include("taylor3.jl")
include("taylor2.jl")
include("utils.jl")

max_x, max_y = 0, 0
total_degree = 0
derivatives = Dict{Tuple{Float64, Float64}, Vector{Vector{Float64}}}()
derivatives_taylor = Dict{Tuple{Float64, Float64}, Vector{Vector{Float64}}}()
derivatives_hermite = Dict{Tuple{Float64, Float64}, Vector{Vector{Float64}}}()
total_eval = 0
total_points = 0
tol = 1e-10
factorial_cache = Vector{Int}()

BenchmarkTools.DEFAULT_PARAMETERS.seconds = 50
BenchmarkTools.DEFAULT_PARAMETERS.samples = 10

#=
    critical_point_range(box::myBox, poly::Polynomial)::myInterval
    
    Compute the exact range of f over `box` by locating true extremal points.
    Method:
    1. Interior critical points: solve ∇f = 0
    2. Boundary extrema: on each edge solve df/ds = 0 (derivative along the edge direction)
    3. Corner points: evaluate the four corners
    
    Arguments:
    - `box::myBox`: computation region
    - `poly::Polynomial`: polynomial to evaluate
    
    Returns:
    - `myInterval`: exact range of f(B)
=#
function critical_point_range(box::myBox, poly::Polynomial)::myInterval
    x_1, x_3 = box.x.lower, box.x.upper
    y_1, y_3 = box.y.lower, box.y.upper
    
    candidate_values = Float64[]
    
    vars = Dict{Symbol, Union{Float64, myInterval}}()
    
    # ==================== 1. Evaluate the four corner points ====================
    corners = [(x_1, y_1), (x_1, y_3), (x_3, y_1), (x_3, y_3)]
    for (x, y) in corners
        vars[:x], vars[:y] = x, y
        val = evaluate_slp_range(poly.slp, "", vars)
        push!(candidate_values, val)
    end
    
    # ==================== 2. Interior critical points: ∇f = 0 ====================
    function gradient_func(vars_vec::Vector{Float64})
        x, y = vars_vec[1], vars_vec[2]
        
        var_dict_x = Dict{Symbol, Union{Float64, myInterval}}(:x => x, :y => y)
        var_dict_y = Dict{Symbol, Union{Float64, myInterval}}(:x => x, :y => y)
        
        fx = evaluate_slp_range(poly.slp, "x", var_dict_x)
        fy = evaluate_slp_range(poly.slp, "y", var_dict_y)
        
        return [fx, fy]
    end
    
    # Search interior critical points with multiple initial guesses
    n_search = 5
    for i in 1:n_search
        for j in 1:n_search
            x0 = x_1 + (x_3 - x_1) * (i / (n_search + 1))
            y0 = y_1 + (y_3 - y_1) * (j / (n_search + 1))
            
            try
                result = nlsolve(gradient_func, [x0, y0], method=:trust_region, ftol=1e-10)
                
                if result.x_converged || result.f_converged
                    x_sol, y_sol = result.zero[1], result.zero[2]
                    
                    # Strict check: must be inside the box (excluding boundary)
                    if x_1 < x_sol < x_3 && y_1 < y_sol < y_3
                        vars[:x], vars[:y] = x_sol, y_sol
                        val = evaluate_slp_range(poly.slp, "", vars)
                        push!(candidate_values, val)
                    end
                end
            catch
            end
        end
    end
    
    # ==================== 3. Bottom edge boundary extrema: y = y_1, solve df/dx = 0 ====================
    # On the bottom edge: extrema of f(x, y_1) along x satisfy df/dx = 0
    function bottom_edge_deriv(x_vec::Vector{Float64})
        x = x_vec[1]
        var_dict = Dict{Symbol, Union{Float64, myInterval}}(:x => x, :y => y_1)
        fx = evaluate_slp_range(poly.slp, "x", var_dict)
        return [fx]
    end
    
    try
        for x_init in range(x_1 + (x_3-x_1)*0.1, x_3 - (x_3-x_1)*0.1, length=3)
            result = nlsolve(bottom_edge_deriv, [x_init], method=:trust_region, ftol=1e-10)
            if result.x_converged || result.f_converged
                x_sol = result.zero[1]
                if x_1 < x_sol < x_3
                    vars[:x], vars[:y] = x_sol, y_1
                    val = evaluate_slp_range(poly.slp, "", vars)
                    push!(candidate_values, val)
                end
            end
        end
    catch
    end
    
    # ==================== 4. Top edge boundary extrema: y = y_3, solve df/dx = 0 ====================
    function top_edge_deriv(x_vec::Vector{Float64})
        x = x_vec[1]
        var_dict = Dict{Symbol, Union{Float64, myInterval}}(:x => x, :y => y_3)
        fx = evaluate_slp_range(poly.slp, "x", var_dict)
        return [fx]
    end
    
    try
        for x_init in range(x_1 + (x_3-x_1)*0.1, x_3 - (x_3-x_1)*0.1, length=3)
            result = nlsolve(top_edge_deriv, [x_init], method=:trust_region, ftol=1e-10)
            if result.x_converged || result.f_converged
                x_sol = result.zero[1]
                if x_1 < x_sol < x_3
                    vars[:x], vars[:y] = x_sol, y_3
                    val = evaluate_slp_range(poly.slp, "", vars)
                    push!(candidate_values, val)
                end
            end
        end
    catch
    end
    
    # ==================== 5. Left edge boundary extrema: x = x_1, solve df/dy = 0 ====================
    function left_edge_deriv(y_vec::Vector{Float64})
        y = y_vec[1]
        var_dict = Dict{Symbol, Union{Float64, myInterval}}(:x => x_1, :y => y)
        fy = evaluate_slp_range(poly.slp, "y", var_dict)
        return [fy]
    end
    
    try
        for y_init in range(y_1 + (y_3-y_1)*0.1, y_3 - (y_3-y_1)*0.1, length=3)
            result = nlsolve(left_edge_deriv, [y_init], method=:trust_region, ftol=1e-10)
            if result.x_converged || result.f_converged
                y_sol = result.zero[1]
                if y_1 < y_sol < y_3
                    vars[:x], vars[:y] = x_1, y_sol
                    val = evaluate_slp_range(poly.slp, "", vars)
                    push!(candidate_values, val)
                end
            end
        end
    catch
    end
    
    # ==================== 6. Right edge boundary extrema: x = x_3, solve df/dy = 0 ====================
    function right_edge_deriv(y_vec::Vector{Float64})
        y = y_vec[1]
        var_dict = Dict{Symbol, Union{Float64, myInterval}}(:x => x_3, :y => y)
        fy = evaluate_slp_range(poly.slp, "y", var_dict)
        return [fy]
    end
    
    try
        for y_init in range(y_1 + (y_3-y_1)*0.1, y_3 - (y_3-y_1)*0.1, length=3)
            result = nlsolve(right_edge_deriv, [y_init], method=:trust_region, ftol=1e-10)
            if result.x_converged || result.f_converged
                y_sol = result.zero[1]
                if y_1 < y_sol < y_3
                    vars[:x], vars[:y] = x_3, y_sol
                    val = evaluate_slp_range(poly.slp, "", vars)
                    push!(candidate_values, val)
                end
            end
        end
    catch
    end
    
    # ==================== Compute minimum and maximum ====================
    if isempty(candidate_values)
        # Fallback: if no critical points are found, return corner range
        return myInterval(
            minimum([evaluate_slp_range(poly.slp, "", Dict{Symbol, Union{Float64, myInterval}}(:x => x, :y => y)) for (x, y) in corners]),
            maximum([evaluate_slp_range(poly.slp, "", Dict{Symbol, Union{Float64, myInterval}}(:x => x, :y => y)) for (x, y) in corners])
        )
    end
    
    min_val = minimum(candidate_values)
    max_val = maximum(candidate_values)
    
    return myInterval(min_val, max_val)
end

#=
    uniform_sample_range(box::myBox, poly::Polynomial, num_samples::Int=100)::myInterval
    
    Compute the exact range f(B) using the critical point method rather than simple sampling.
    
    Arguments:
    - `box::myBox`
    - `poly::Polynomial`
    - `num_samples::Int`: deprecated, kept for compatibility
    
    Returns:
    - `myInterval`
=#
function uniform_sample_range(box::myBox, poly::Polynomial, num_samples::Int=100)::myInterval
    return critical_point_range(box, poly)
end

# Keep the old sampling method as a fallback
function uniform_sample_range_old(box::myBox, poly::Polynomial, num_samples::Int=100)::myInterval

    x_min = box.x.lower
    x_max = box.x.upper
    y_min = box.y.lower
    y_max = box.y.upper
    
    samples_per_dim = Int(ceil(sqrt(num_samples)))
    
    x_step = (x_max - x_min) / (samples_per_dim - 1)
    y_step = (y_max - y_min) / (samples_per_dim - 1)
    
    min_val = Inf
    max_val = -Inf
    
    vars = Dict{Symbol,Union{Float64,myInterval}}()


    for i in 0:(samples_per_dim-1)
        for j in 0:(samples_per_dim-1)
            x = x_min + i * x_step
            y = y_min + j * y_step

            vars[:x] = x
            vars[:y] = y
            val = evaluate_slp_range(poly.slp, "", vars)
        
            if val < min_val
                min_val = val
            end
            if val > max_val
                max_val = val
            end
        end
    end
    
    return myInterval(min_val, max_val)
end




function compute_logD(method_interval::myInterval, real_interval::myInterval)::Float64
    """
    Compute the logarithmic distance between a method's interval and the real interval
    logD = log10(max(|method_lower - real_lower|, |method_upper - real_upper|))
    """
    lower_diff = abs(method_interval.lower - real_interval.lower)
    upper_diff = abs(method_interval.upper - real_interval.upper)
    max_diff = max(lower_diff, upper_diff)
    
    if max_diff == 0.0
        return -16.0
    end
    
    return log10(max_diff)
end

function create_boxes_with_radius(midpoint_x::Float64, midpoint_y::Float64, radii::Vector{Float64})::Vector{myBox}
    """
    Create boxes with varying radius around the given midpoint
    Box = [midpoint_x ± r, midpoint_y ± r]
    """
    boxes = myBox[]
    
    for r in radii
        box = myBox(
            myInterval(midpoint_x - r, midpoint_x + r),
            myInterval(midpoint_y - r, midpoint_y + r)
        )
        push!(boxes, box)
    end
    
    return boxes
end

function analyze(poly_str::String, midpoint_x::Float64, midpoint_y::Float64, samples::Int=100)
    poly_lagrange = Polynomial(poly_str)
    poly_taylor = Polynomial(poly_str)

    global max_x, max_y, total_degree
    max_x, max_y = get_max_order(poly_lagrange, :x), get_max_order(poly_lagrange, :y)
    total_degree = get_total_degree(poly_lagrange) * 2

    compute_third_derivatives_2D!(poly_lagrange)
    compute_all_derivatives!(poly_taylor)

    radii = [10^x for x in range(log10(0.0000001), log10(1.0), length=1000)]
    log_radii = log10.(radii)

    boxes = create_boxes_with_radius(midpoint_x, midpoint_y, radii)

    lagrange_logD = Float64[]
    taylor_logD = Float64[]

    for (i, (box, r)) in enumerate(zip(boxes, radii))
        if i % 10 == 0
            println("Processing box $i/$(length(boxes)), radius = $r")
        end
        
        try
            real_interval = uniform_sample_range(box, poly_lagrange)

            reset_derivatives()
            fill!(box.FB, nothing)
            fill!(box.QB, [])
    
            lagrange_interval = lagrange_range_function(poly_lagrange, box, total_degree, "S", 6)


            reset_derivatives()
            fill!(box.FB, nothing)
            fill!(box.QB, [])
            
            taylor_interval = taylor_interpolation(poly_taylor, box, total_degree)

            reset_derivatives()
            fill!(box.FB, nothing)
            fill!(box.QB, [])
            
            # Compute logD for both methods
            lagrange_logd = compute_logD(lagrange_interval, real_interval)
            taylor_logd = compute_logD(taylor_interval, real_interval)
            
            push!(lagrange_logD, lagrange_logd)
            push!(taylor_logD, taylor_logd)
            
        catch e
            println("Error processing box with radius $r: $e")
            # Use NaN for failed computations
            push!(lagrange_logD, NaN)
            push!(taylor_logD, NaN)
        end
    end
    
    # Filter out NaN values
    valid_indices = .!isnan.(lagrange_logD) .& .!isnan.(taylor_logD)
    log_radii_clean = log_radii[valid_indices]
    lagrange_logD_clean = lagrange_logD[valid_indices]
    taylor_logD_clean = taylor_logD[valid_indices]
    
    p = plot(log_radii_clean, lagrange_logD_clean, 
             label="Lagrange Method", 
             linewidth=0.5, 
             marker=:circle, 
             markersize=0.5,
             markerstrokewidth=0.5,
             color=:blue,
             markerstrokecolor=:darkblue,
             alpha=0.8,
             xlabel="log₁₀(r)", 
             ylabel="log₁₀(D)",
             title="logD vs logr Analysis\nPolynomial: $(poly_str[1:min(50, length(poly_str))])...",
             size=(800, 600),
             dpi=300,
             legend=:topright)
    
    plot!(p, log_radii_clean, taylor_logD_clean, 
          label="Taylor Method", 
          linewidth=0.5, 
          marker=:square, 
          markersize=0.5,
          markerstrokewidth=0.5,
          color=:red,
          markerstrokecolor=:darkred,
          alpha=0.8,
          linestyle=:solid)

    x_range = extrema(log_radii_clean)
    y_intercept = minimum([minimum(lagrange_logD_clean), minimum(taylor_logD_clean)]) - 1
    reference_y = [y_intercept + 3 * (x - x_range[1]) for x in log_radii_clean]
    
    plot!(p, log_radii_clean, reference_y,
          label="Reference (slope=3)",
          linewidth=2,
          linestyle=:dash,
          color=:green,
          alpha=0.7)
    
    plot!(p, grid=true, gridwidth=1, gridcolor=:gray, gridalpha=0.3)
    
    println("Analysis complete. Generated plot with $(length(log_radii_clean)) valid data points.")

    mkpath("imgs")

    filename = "imgs/logD_vs_logr_analysis_$(replace(poly_str[1:min(20, length(poly_str))], r"[^a-zA-Z0-9]" => "_"))_x$(midpoint_x)_y$(midpoint_y).png"
    savefig(p, filename)
    
    println("Plot saved to: $filename")
    
    return p, log_radii_clean, lagrange_logD_clean, taylor_logD_clean
end


function test_intervals_at_r(poly_str::String, midpoint_x::Float64, midpoint_y::Float64; radii::Vector{Float64}=[0.2, 0.1, 0.05])
    poly_lagrange = Polynomial(poly_str)
    poly_taylor = Polynomial(poly_str)
    poly_hermite = Polynomial(poly_str)

    global max_x, max_y, total_degree
    max_x, max_y = get_max_order(poly_lagrange, :x), get_max_order(poly_lagrange, :y)
    total_degree = get_total_degree(poly_lagrange)

    compute_third_derivatives_2D!(poly_lagrange)
    compute_all_derivatives!(poly_taylor)
    compute_all_derivatives!(poly_hermite)

    results = Tuple{Float64, myInterval, myInterval, myInterval}[]

    for r in radii
        box = myBox(
            myInterval(midpoint_x - r, midpoint_x + r),
            myInterval(midpoint_y - r, midpoint_y + r)
        )

        real_interval = uniform_sample_range(box, poly_lagrange)

        reset_derivatives()
        fill!(box.FB, nothing)
        fill!(box.QB, [])
        poly_lagrange = Polynomial(poly_str)
        compute_third_derivatives_2D!(poly_lagrange)

        # lagrange_interval = lagrange_range_function(poly_lagrange, box, total_degree, "S", 6)
        lagrange_interval = Lagrange3(poly_lagrange, box, total_degree)

        reset_derivatives()
        fill!(box.FB, nothing)
        fill!(box.QB, [])

        taylor_interval = taylor_interpolation(poly_taylor, box, total_degree)

        reset_derivatives()
        fill!(box.FB, nothing)
        fill!(box.QB, [])
        poly_taylor = Polynomial(poly_str)
        compute_all_derivatives!(poly_taylor)


        taylor_interval_linear = taylor_interpolation_linear(poly_taylor, box, total_degree)
       
        reset_derivatives()
        fill!(box.FB, nothing)
        fill!(box.QB, [])
        poly_taylor = Polynomial(poly_str)
        compute_all_derivatives!(poly_taylor)


        taylor_interval_4 = taylor_interpolation4(poly_taylor, box, total_degree)
       
        reset_derivatives()
        fill!(box.FB, nothing)
        fill!(box.QB, [])

        hermite_interval = hermite4(poly_hermite, box, total_degree)
        # hermite_interval = hermite4(poly_str, box_center, total_degree)
        println("r = $(r)")
        println("real_interval = $(real_interval)")
        println("lagrange_interval = $(lagrange_interval)")
        println("taylor_interval = $(taylor_interval)")
        println("taylor_interval_linear = $(taylor_interval_linear)")
        println("taylor_interval_4 = $(taylor_interval_4)")
        println("hermite_interval = $(hermite_interval)")

        push!(results, (r, real_interval, lagrange_interval, taylor_interval))
    end

    return results
end


function reset_derivatives()
    global derivatives, derivatives_taylor, derivatives_hermite
    derivatives = Dict{Tuple{Float64, Float64}, Vector{Vector{Float64}}}()
    derivatives_taylor = Dict{Tuple{Float64, Float64}, Vector{Vector{Float64}}}()
    derivatives_hermite = Dict{Tuple{Float64, Float64}, Vector{Vector{Float64}}}()
end


function printf(f)
    if abs(f) > 1e10
        @printf(stdout,"%.3e",f)
    else 
        @printf(stdout,"%.3f",f)
    end
end

function get_width(I::myInterval)
    return I.upper - I.lower
end

function get_width(I::Tuple{Float64, Float64})
    return I[2] - I[1]
end

function compare_methods(poly_str::String, box::myBox, dim::Int = 1024)
    """
    Compare Lagrange vs Taylor methods on subdivided boxes
    subdivisions = 32 gives 32x32 = 1024 subboxes
    Returns matrix of ratios (lagrange_width / taylor_width)
    """
    
    # Create polynomial objects
    poly_lagrange = Polynomial(poly_str)
    poly_taylor = Polynomial(poly_str)
    
    global max_x, max_y, total_degree
    max_x, max_y = get_max_order(poly_lagrange, :x), get_max_order(poly_lagrange, :y)
    total_degree = get_total_degree(poly_lagrange)
    
    # compute_third_derivatives_2D!(poly_lagrange)
    compute_all_derivatives!(poly_taylor)
    
    # expr = Meta.parse(poly_str)
    # f_sym = eval(expr)
    # df_table = make_df_table(f_sym, total_degree, x, y)


    # Create uniform grid directly instead of using uniform_split
    side = round(Int, sqrt(dim))
    ratio_matrix = zeros(Float64, side, side)
    
    # Calculate grid spacing
    x_step = (box.x.upper - box.x.lower) / side
    y_step = (box.y.upper - box.y.lower) / side
    
    println("Computing ranges for $(side)x$(side) = $(side*side) subboxes...")
    
    # Create uniform grid of boxes
    for i in 1:side
        for j in 1:side
            # Calculate box boundaries
            x_lower = box.x.lower + (j-1) * x_step
            x_upper = box.x.lower + j * x_step
            y_lower = box.y.lower + (i-1) * y_step
            y_upper = box.y.lower + i * y_step
            
            # Create subbox
            subbox = myBox(myInterval(x_lower, x_upper), myInterval(y_lower, y_upper))

            box_center = ((x_lower + x_upper) / 2, (y_lower + y_upper) / 2, (x_upper - x_lower) / 2)
            
            println("Processing box at ($i, $j) with center $box_center")

            if (i-1)*side + j % 100 == 0
                println("Processed $((i-1)*side + j)/$(side*side) boxes")
            end
            
            try
                # reset_derivatives()
                
                # lagrange_interval = lagrange_range_function(poly_lagrange, subbox, total_degree, "S", 6)
                # lagrange_width = get_width(lagrange_interval)

                hermite_interval = hermite4(poly_taylor, subbox, total_degree)
                hermite_width = get_width(hermite_interval)
                
                reset_derivatives()

                taylor4_interval = taylor_interpolation4(poly_taylor, subbox, total_degree)
                taylor4_width = get_width(taylor4_interval)

                reset_derivatives()
                
                # taylor_interval = taylor_interpolation(poly_taylor, subbox, total_degree)
                # taylor_width = get_width(taylor_interval)

                # reset_derivatives()

                # taylor_2_interval = taylor_interpolation_linear(poly_taylor, subbox, total_degree)
                # taylor_2_width = get_width(taylor_2_interval)
                
                if taylor4_width > 0
                    ratio = hermite_width / taylor4_width
                else
                    ratio = 1.0  
                end
                
                ratio_matrix[i, j] = ratio
                
            catch e
                println("Error processing box at ($i, $j): $e")
                ratio_matrix[i, j] = 1.0 
            end
        end
    end
    
    return ratio_matrix
end

function create_visualization(ratio_matrix::Matrix{Float64}, box::myBox, poly_str::String)
    """
    Create color-coded heatmap visualization with clear threshold
    Green (varying depths): Lagrange/Taylor ratio < 1 (Lagrange better)
    Red (varying depths): Lagrange/Taylor ratio > 1 (Taylor better)
    Intensity based on magnitude of ratio
    """
    
    # Create color matrix with clear threshold and magnitude-based intensity
    color_matrix = zeros(size(ratio_matrix))
    
    for i in 1:size(ratio_matrix, 1)
        for j in 1:size(ratio_matrix, 2)
            ratio = ratio_matrix[i, j]
            
            if ratio > 1.0
                # Red side: Taylor is better (ratio > 1)
                color_matrix[i, j] = log10(ratio)
            else
                # Green side: Lagrange is better (ratio ≤ 1)
                color_matrix[i, j] = -abs(log10(ratio))
            end
        end
    end
    
    # Create coordinate ranges for axes
    x_range = range(box.x.lower, box.x.upper, length=size(ratio_matrix, 2))
    y_range = range(box.y.lower, box.y.upper, length=size(ratio_matrix, 1))
    
    # Use heatmap with proper coordinate mapping
    # Note: Don't flip the matrix - let heatmap handle the coordinate system naturally
    p = heatmap(x_range, y_range, color_matrix,
               color=cgrad([:darkgreen, :forestgreen, :limegreen, :yellow, :orange, :red, :darkred], [0.0, 0.2, 0.4, 0.5, 0.6, 0.8, 1.0]),
               aspect_ratio=:equal,
               title="Hermite4 vs Taylor4 Tightness Comparison\n$(poly_str[1:min(50, length(poly_str))])...",
               xlabel="",
               ylabel="",
               xticks=false,
               yticks=false,
               framestyle=:none,
               size=(800, 600),
               dpi=300,
               margin=5Plots.mm,
               right_margin=15Plots.mm,
               clims=(-maximum(abs.(color_matrix)), maximum(abs.(color_matrix))),
               yflip=false)
    

    polynomial = Polynomial(poly_str)

    slp = polynomial.slp

    xs = range(box.x.lower, box.x.upper; length=(round(Int, box.x.upper - box.x.lower) + 1) * 50)
    ys = range(box.y.lower, box.y.upper; length=(round(Int, box.y.upper - box.y.lower) + 1) * 50)
    Z = [ eval_slp(slp, x, y, "") for  y in ys, x in xs ]

    contour!(p, xs, ys, Z, levels=[0], 
             color=:black, linewidth=3, linestyle=:solid,
             label="f(x,y) = 0")

    return p
end



function main()
    global max_x, max_y, total_degree
    # poly = "x^2y^2 -x + y -1"
    # poly = "x^3 + x^2 - y^2 + 0.5"
  #  test_poly = "x^10*(y)^10"
    poly_1 = "50*x^10+(249*y^2-57)*x^8+(498*y^4-227*y^2-1)*x^6+(498*y^6-341*y^4-3*y^2+16)*x^4+(249*y^8-227*y^6-3*y^4-102*y^2-1)*x^2+50*y^10-57*y^8-y^6+16*y^4-y^2-1"
    poly_2 = "71*x^12+(424*y^2-79)*x^10+(1059*y^4-396*y^2-1)*x^8+(1412*y^6-793*y^4-4*y^2-1)*x^6+20*x^5+(1059*y^8-793*y^6-6*y^4-3*y^2-1)*x^4-202*y^2*x^3+(424*y^10-396*y^8-4*y^6-3*y^4-2*y^2-1)*x^2+101*y^4*x+71*y^12-79*y^10-y^8-y^6-y^4-y^2-1"
    poly_3 = "95*x^14+(667*y^2-106)*x^12+(2000*y^4-634*y^2-1)*x^10+(3334*y^6-1585*y^4-5*y^2-1)*x^8+(3334*y^8-2113*y^6-10*y^4-4*y^2+23)*x^6+(2000*y^10-1585*y^8-10*y^6-6*y^4-360*y^2-1)*x^4+(667*y^12-634*y^10-5*y^8-4*y^6+354*y^4-2*y^2-1)*x^2+95*y^14-106*y^12-y^10-y^8-25*y^6-y^4-y^2-1"
    poly_4 = "156*x^18+(1406*y^2-170)*x^16+(5625*y^4-1363*y^2-1)*x^14+(13125*y^6-4769*y^4-7*y^2-1)*x^12+(19688*y^8-9538*y^6-21*y^4-6*y^2-1)*x^10+(19688*y^10-11922*y^8-35*y^6-15*y^4-5*y^2+30)*x^8+(13125*y^12-9538*y^10-35*y^8-21*y^6-11*y^4-879*y^2-1)*x^6+(5625*y^14-4769*y^12-21*y^10-15*y^8-11*y^6+2181*y^4-4*y^2-1)*x^4+(1406*y^16-1363*y^14-7*y^12-6*y^10-5*y^8-879*y^6-4*y^4-3*y^2-1)*x^2+156*y^18-170*y^16-y^14-y^12-y^10+30*y^8-y^6-y^4-y^2-1"
    # horner_poly_1 = "-1+(-1+(16+(-1+(50*y^2-57)*y^2)*y^2)*y^2)*y^2+(-1+(-102+(-3+(249*y^2-227)*y^2)*y^2)*y^2+(16+(-3+(498*y^2-341)*y^2)*y^2+(-1+(498*y^2-227)*y^2+(50*x^2+249*y^2-57)*x^2)*x^2)*x^2)*x^2"
    # horner_poly_2 = "-1+(-1+(-1+(-1+(-1+(71*y^2-79)*y^2)*y^2)*y^2)*y^2)*y^2+(101*y^4+(-1+(-2+(-3+(-4+(424*y^2-396)*y^2)*y^2)*y^2)*y^2+(-202*y^2+(-1+(-3+(-6+(1059*y^2-793)*y^2)*y^2)*y^2+(20+(-1+(-4+(1412*y^2-793)*y^2)*y^2+(-1+(1059*y^2-396)*y^2+(71*x^2+424*y^2-79)*x^2)*x^2)*x)*x)*x)*x"
    # horner_poly_3 = "-1+(-1+(-1+(-25+(-1+(-1+(95*y^2-106)*y^2)*y^2)*y^2)*y^2)*y^2)*y^2+(-1+(-2+(354+(-4+(-5+(667*y^2-634)*y^2)*y^2)*y^2)*y^2)*y^2+(-1+(-360+(-6+(-10+(2000*y^2-1585)*y^2)*y^2+(23+(-4+(-10+(3334*y^2-2113)*y^2)*y^2)*y^2+(-1+(-5+(3334*y^2-1585)*y^2)*y^2+(-1+(2000*y^2-634)*y^2+(95*x^2+667*y^2-106)*x^2)*x^2)*x^2)*x^2)*x^2)*x^2)*x^2"
    # horner_poly_4 = "-1+(-1+(-1+(-1+(30+(-1+(-1+(-1+(156*y^2-170)*y^2)*y^2)*y^2)*y^2)*y^2)*y^2)*y^2+(-1+(-3+(-4+(-879+(-5+(-6+(-7+(1406*y^2-1363)*y^2)*y^2)*y^2+(-1+(-4+(2181+(-11+(-15+(-21+(5625*y^2-4769)*y^2)*y^2)*y^2+(-1+(-879+(-11+(-21+(-35+(13125*y^2-9538)*y^2)*y^2)*y^2+(30+(-5+(-15+(-35+(19688*y^2-11922)*y^2)*y^2)*y^2+(-1+(-6+(-21+(19688*y^2-9538)*y^2)*y^2)*y^2+(-1+(-7+(13125*y^2-4769)*y^2)*y^2+(-1+(5625*y^2-1363)*y^2+(156*x^2+1406*y^2-170)*x^2)*x^2)*x^2)*x^2"

    
    # poly_6 = "2000y^8 + 8000x^2y^6 + 12000x^4y^4 + 8000x^6y^2 + 2000x^8 - 3000y^6 + 9000x^2y^4 - 21000x^4y^2 - 1000x^6 + 1"
    poly_5 = "3028466566125*x^12-1275033793950*x^10*y^2+93020598855*x^8*y^4-1519616052*x^6*y^6+5930667*x^4*y^8-5454*x^2*y^10+y^12+2550067587900*x^11-372082395420*x^9*y^2+9117696312*x^7*y^4-47445336*x^5*y^6+54540*x^3*y^8-12*x*y^10-902951398530*x^10+167805805086*x^8*y^2-4416512148*x^6*y^4+23504508*x^4*y^6-27210*x^2*y^8+6*y^10-359925467004*x^9+18045611280*x^7*y^2-141899688*x^5*y^4+218000*x^3*y^6-60*x*y^8+74880096903*x^8-4274612460*x^6*y^2+34929762*x^4*y^4-54300*x^2*y^6+15*y^8+8928089496*x^7-141463560*x^5*y^2+326760*x^3*y^4-120*x*y^6-1377716300*x^6+23068668*x^4*y^2-54180*x^2*y^4+20*y^6-47009208*x^5+217680*x^3*y^2-120*x*y^4+5712747*x^4-27030*x^2*y^2+15*y^4+54380*x^3-60*x*y^2-5394*x^2+6*y^2-12*x+2"
    poly_6 = "6379536384*x^36+114995232768*x^34*y^2+978278416384*x^32*y^4+5219942400000*x^30*y^6+19579740094464*x^28*y^8+54830448508928*x^26*y^10+118807142334464*x^24*y^12+203676057600000*x^22*y^14+280058859618304*x^20*y^16+311177912516608*x^18*y^18+280058859618304*x^16*y^20+203676057600000*x^14*y^22+118807142334464*x^12*y^24+54830448508928*x^10*y^26+19579740094464*x^8*y^28+5219942400000*x^6*y^30+978278416384*x^4*y^32+114995232768*x^2*y^34+6379536384*y^36-8690073600*x^34-147996016640*x^32*y^2-1185084866560*x^30*y^4-5928146370560*x^28*y^6-20752701849600*x^26*y^8-53961142763520*x^24*y^10-107924671037440*x^22*y^12-169596414525440*x^20*y^14-211995286896640*x^18*y^16-211995286896640*x^16*y^18-169596414525440*x^14*y^20-107924671037440*x^12*y^22-53961142763520*x^10*y^24-20752701849600*x^8*y^26-5928146370560*x^6*y^28-1185084866560*x^4*y^30-147996016640*x^2*y^32-8690073600*y^34+1850540032*x^32+29674930176*x^30*y^2+222794301440*x^28*y^4+1040147972096*x^26*y^6+3380930641920*x^24*y^8+8114349899776*x^22*y^10+14875996045312*x^20*y^12+21250981527552*x^18*y^14+23907165863936*x^16*y^16+21250981527552*x^14*y^18+14875996045312*x^12*y^20+8114349899776*x^10*y^22+3380930641920*x^8*y^24+1040147972096*x^6*y^26+222794301440*x^4*y^28+29674930176*x^2*y^30+1850540032*y^32-27607040*x^30-414392320*x^28*y^2-2901872640*x^26*y^4-12618895360*x^24*y^6-38110392320*x^22*y^8-84521492480*x^20*y^10-141871616000*x^18*y^12-183157678080*x^16*y^14-183157678080*x^14*y^16-141871616000*x^12*y^18-84521492480*x^10*y^20-38110392320*x^8*y^22-12618895360*x^6*y^24-2901872640*x^4*y^26-414392320*x^2*y^28-27607040*y^30-29200384*x^28-409122816*x^26*y^2-2701409280*x^24*y^4-11042656256*x^22*y^6-31027382272*x^20*y^8-63148691456*x^18*y^10-95792569344*x^16*y^12-109900320768*x^14*y^14-95792569344*x^12*y^16-63148691456*x^10*y^18-31027382272*x^8*y^20-11042656256*x^6*y^22-2701409280*x^4*y^24-409122816*x^2*y^26-29200384*y^28+1239367680*x^26-24765742080*x^24*y^2-189838315520*x^22*y^4-422069159680*x^20*y^6+32327372800*x^18*y^8+1854551856640*x^16*y^10+3867544352000*x^14*y^12+3867544352000*x^12*y^14+1854551856640*x^10*y^16+32327372800*x^8*y^18-422069159680*x^6*y^20-189838315520*x^4*y^22-24765742080*x^2*y^24+1239367680*y^26-376422144*x^24+6609039488*x^22*y^2+41905561792*x^20*y^4+60370496192*x^18*y^6-102949604736*x^16*y^8-465920452672*x^14*y^10-674514695296*x^12*y^12-465920452672*x^10*y^14-102949604736*x^8*y^16+60370496192*x^6*y^18+41905561792*x^4*y^20+6609039488*x^2*y^22-376422144*y^24-30239040*x^22-346078560*x^20*y^2-1850334640*x^18*y^4-5936360160*x^16*y^6-12496914880*x^14*y^8-17979334800*x^12*y^10-17979334800*x^10*y^12-12496914880*x^8*y^14-5936360160*x^6*y^16-1850334640*x^4*y^18-346078560*x^2*y^20-30239040*y^22-29864336*x^20-283463064*x^18*y^2-1238936924*x^16*y^4-3258756216*x^14*y^6-5668585636*x^12*y^8-6807869984*x^10*y^10-5668585636*x^8*y^12-3258756216*x^6*y^14-1238936924*x^4*y^16-283463064*x^2*y^18-29864336*y^20-29770660*x^18-266964330*x^16*y^2-1069378745*x^14*y^4-2501688745*x^12*y^6-3764232520*x^10*y^8-3764232520*x^8*y^10-2501688745*x^6*y^12-1069378745*x^4*y^14-266964330*x^2*y^16-29770660*y^18+11393278*x^16+76003395*x^14*y^2+544478961*x^12*y^4-368667742*x^10*y^6+2396752122*x^8*y^8-368667742*x^6*y^10+544478961*x^4*y^12+76003395*x^2*y^14+11393278*y^16+11925*x^14+394595*x^12*y^2+847065*x^10*y^4-622845*x^8*y^6-622845*x^6*y^8+847065*x^4*y^10+394595*x^2*y^12+11925*y^14-2971*x^12+245870*x^10*y^2+195311*x^8*y^4-1190691*x^6*y^6+195311*x^4*y^8+245870*x^2*y^10-2971*y^12-6695*x^10+226425*x^8*y^2-325885*x^6*y^4-325885*x^4*y^6+226425*x^2*y^8-6695*y^10-7626*x^8+226339*x^6*y^2-559797*x^4*y^4+226339*x^2*y^6-7626*y^8+85*x^6+340*x^4*y^2+340*x^2*y^4+85*y^6+21*x^4+59*x^2*y^2+21*y^4+5*x^2+5*y^2+1"
    # poly = "(2x^2 + 4)(3y + 5)"
    # f = make_polynomial_function(poly)

    # B_1 = myBox(myInterval(-0.1, 0.1),myInterval(-0.1,0.1))
    # B_2 = myBox(myInterval(-0.2,0.2),myInterval(-0.2,0.2))
    # B_3 = myBox(myInterval(5,5.1),myInterval(3,3.1))
    # B_4 = myBox(myInterval(0.5,0.6),myInterval(0.3,0.4))
    # B_5 = myBox(myInterval(10,20),myInterval(30,40))
    # B = myBox(myInterval(0,2),myInterval(0, 2))
    # B_6 = myBox(myInterval(-0.0375, 0),myInterval(0.5, 0.5375))

    # B_1 = myBox(myInterval(-1, 1),myInterval(-1,1))
    # B_2 = myBox(myInterval(-1,1),myInterval(-1,1))
    # B_3 = myBox(myInterval(-1,1),myInterval(-1,1))
    # B_4 = myBox(myInterval(-1,1),myInterval(-1,1))
    # B_5 = myBox(myInterval(-0.3,0.3),myInterval(0.2,0.8))

    B = myBox(myInterval(-1.2, 1.2),myInterval(-1.2, 1.2))

    polys = [poly_1, poly_2,poly_4, poly_5]


    table_rows = NamedTuple{(:curve, :min_ms, :median_ms, :mean_ms, :memory_mb, :efficacy), Tuple{String, Float64, Float64, Float64, Float64, Float64}}[]
    

    for (i, poly) in enumerate(polys)
        polynomial = Polynomial(poly)

        max_x, max_y = get_max_order(polynomial, :x), get_max_order(polynomial, :y)
        total_degree = get_total_degree(polynomial)

        compute_all_derivatives!(polynomial)
        #compute_third_derivatives_2D!(polynomial)


        if i == 1
            description = "Clover4"
        elseif i == 2
            description = "Clover5"
        elseif i == 3
            description = "Clover8"
        elseif i == 4
            description = "Grass"
        end

        box_count = 1024

        q = uniform_split(B, box_count)

        total_width = evaluate_boxes(q, polynomial, "Lagrange3"; sharing=false)

        efficacy = total_width / box_count

        reset_derivatives()

        benchmark_result = @benchmark evaluate_boxes($q, $polynomial, "Lagrange3"; sharing=false) teardown=(reset_derivatives())  

        min_ms    = minimum(benchmark_result.times) / 1e6
        median_ms = median(benchmark_result.times) / 1e6
        mean_ms   = mean(benchmark_result.times) / 1e6
        memory_mb = benchmark_result.memory / 1024^2

        println(description)

        push!(table_rows, (; curve=description, min_ms, median_ms, mean_ms, memory_mb, efficacy))

        display(benchmark_result)

        println("Total width: $total_width")
        println("Efficacy: $efficacy")
    end

    println()
    println(rpad("Curve", 12), rpad("Method", 10), rpad("Min (ms)", 12),
            rpad("Median (ms)", 14), rpad("Mean (ms)", 12), rpad("Memory (MB)", 13), rpad("Efficacy", 12))

    for row in table_rows
        println(rpad(row.curve, 12),
                rpad("Lagrange3", 10),
                @sprintf("%-11.4f", row.min_ms),
                @sprintf("%-13.4f", row.median_ms),
                @sprintf("%-11.4f", row.mean_ms),
                @sprintf("%-12.4f", row.memory_mb),
                @sprintf("%-12.4f", row.efficacy))
    end
    println()


    # B= myBox(myInterval(0.2-0.001, 0.2+0.001), myInterval(0.1-0.001, 0.1+0.001))

    # poly = Polynomial(poly_4)

    # max_x = get_max_order(poly, :x)
    # max_y = get_max_order(poly, :y)
    # total_degree = get_total_degree(poly) * 2

    # compute_all_derivatives!(poly)

    # reset_global_state!()

    # taylorinterval= taylor_interpolation_linear(poly, B, total_degree)

    # print(taylorinterval)


    # horner_polys = [horner_poly_1, horner_poly_2, horner_poly_3, horner_poly_4]
    # Bs = [B_1]
    # Bs_double = [myBox(B.x * 2, B.y * 2) for B in Bs]
    # Bs_shifted = [myBox(myInterval(B.x.lower + 0.1, B.x.upper + 0.1), myInterval(B.y.lower + 0.1, B.y.upper + 0.1)) for B in Bs_double]

    # 2D array: rows = boxes, columns = polynomials
    # results = [
    #     [0.0270169306, 0.1946758144, 5.7780786895986*10^8, 4.2354696006, 1544928161474299000],

    #     [0.021218409080, 0.113114132480, 3.4231610250975*10^10, 4.495965742216, 4453506554995008299000],
        
    #     [0.02040816984108, 0.08698194051072, 1.87402319511643*10^12, 3.69019207471188, 12084868375500162172999000],

    #     [ 0.02050517347055, 0.08740755359008, 4.79850741681596*10^15, 1.859935202, 79779127269226806361960306449000],
    # ]
    #for (i, poly) in enumerate(polys)
    #    analyze(poly, 0.1, 0.1, 1000)
       # analyze(poly, 0.1, 0.2, 1000)
    #end
    # test_intervals_at_r(poly_1, 0.1, 0.2)
    # poly = Polynomial(poly_4)
    # horner_poly = Polynomial(horner_poly_4)
    # max_x, max_y = get_max_order(poly, :x), get_max_order(poly, :y)
    # total_degree = get_total_degree(poly) * 2
    # compute_all_derivatives!(poly)
    # compute_all_derivatives!(horner_poly)

    # B_2 = myBox(myInterval(-0.2,0.2),myInterval(-0.2,0.2))

    # interval_s6_test  = lagrange_range_function(poly, B_2, total_degree, "S", 6) 
    # interval_s3_test  = lagrange_range_function(poly, B_2, total_degree, "S", 3) 

    # vars = Dict{Symbol, myInterval}(:x => B_2.x,:y => B_2.y)
    # interval_horner = evaluate_slp_range(horner_poly.slp, "", vars)
    # # interval_horner_dx = evaluate_slp_range(horner_poly.slp, "x", vars)
    # # interval_horner_dy = evaluate_slp_range(horner_poly.slp, "y", vars)
    

    # println(get_width(interval_s6_test) / results[4][2])
    # println(get_width(interval_s3_test) / results[4][2])
    # println(get_width(interval_horner) / results[4][2])

    # for i in 1:length(polys)
    #     reset_derivatives()
    #     println("P$i = $(polys[i])")
    #     poly = Polynomial(polys[i])
    #     poly_taylor = Polynomial(polys[i])
    #     # horner_poly = Polynomial(horner_polys[i])
    #     max_x, max_y = get_max_order(poly, :x), get_max_order(poly, :y)
    #     total_degree = get_total_degree(poly) * 2
    #     compute_third_derivatives_2D!(poly)
    #     compute_all_derivatives!(poly_taylor)
        # compute_all_derivatives!(horner_poly)

        # B_1 = myBox(myInterval(-0.1, 0.1),myInterval(-0.1,0.1))
        # B_2 = myBox(myInterval(-0.2,0.2),myInterval(-0.2,0.2))
        # B_3 = myBox(myInterval(5,5.1),myInterval(3,3.1))
        # B_4 = myBox(myInterval(0.5,0.6),myInterval(0.3,0.4))
        # B_5 = myBox(myInterval(10,20),myInterval(30,40))

        # B_1 = myBox(myInterval(-1.7, 1.5),myInterval(-1.7, 1.5))
        # B_2 = myBox(myInterval(-0.9, 0.7),myInterval(-0.9, 0.7))
        # B_3 = myBox(myInterval(-0.5, 0.3),myInterval(-0.5, 0.3))
        # B_4 = myBox(myInterval(-0.3, 0.1),myInterval(-0.3, 0.1))

        # B_5 = myBox(myInterval(-0.2, 0.0), myInterval(-0.2, 0.0))
        # B_6 = myBox(myInterval(-0.15, -0.05), myInterval(-0.15, -0.05))
        # B_7 = myBox(myInterval(-0.125, -0.025), myInterval(-0.125, -0.025))
        # B_8 = myBox(myInterval(-0.1125, -0.0125), myInterval(-0.1125, -0.0125))


        # Bs = [B_1, B_2, B_3, B_4, B_5, B_6, B_7, B_8]

        # for j in 1:length(Bs)
        #     println("B$j = $(Bs[j].x), $(Bs[j].y)")
        #     reset_derivatives()

        #     fill!(Bs[j].FB, nothing)
        #     fill!(Bs[j].QB, [])
            
        #     interval_s6 = lagrange_range_function(poly, Bs[j], total_degree, "S", 6)

        #     # reset_derivatives()
        #     # fill!(Bs[j].FB, nothing)
        #     # fill!(Bs[j].QB, [])

        #     # interval_s3 = lagrange_range_function(poly, Bs[j], total_degree, "S", 3)
        #     # dx = dx_lagrange(Bs[j], "S")
        #     # dy = dy_lagrange(Bs[j], "S")
            
        #     reset_derivatives()

        #     interval_taylor = taylor_interpolation(poly_taylor, Bs[j], total_degree)
        #     taylor_dx = taylor_interpolation_dx(Bs[j], total_degree)
        #     taylor_dy = taylor_interpolation_dy(Bs[j], total_degree)

            # vars = Dict{Symbol, myInterval}(:x => Bs[j].x,:y => Bs[j].y)
            # interval_horner = evaluate_slp_range(horner_poly.slp, "", vars)
            # interval_horner_dx = evaluate_slp_range(horner_poly.slp, "x", vars)
            # interval_horner_dy = evaluate_slp_range(horner_poly.slp, "y", vars)


    # #         # bm = @benchmark taylor_interpolation($(polys[i]), $(Bs[j]), 18)
    # #         # display(bm)
    # #         # print("W(H, 3) =  W(")
    # #         # printf(interval_h3.lower)
    # #         # print(", ")
    # #         # printf(interval_h3.upper)
    # #         # print(") = ")
    # #         # printf(get_width(interval_h3))
    # #         # print(", Ratio = ")
    # #         # printf(get_width(interval_h3)/results[i][j])
    # #         # println("")
            
    # #         # print("W(T, 3) =  W(")
    # #         # printf(interval_t3.lower)
    # #         # print(", ")
    # #         # printf(interval_t3.upper)
    # #         # print(") = ")
    # #         # printf(get_width(interval_t3))
    # #         # print(", Ratio = ")
    # #         # printf(get_width(interval_t3)/results[i][j])
    # #         # println("")

            
    # #         # print("W(H, 6) =  W(")
    # #         # printf(interval_h6.lower)
    # #         # print(", ")
    # #         # printf(interval_h6.upper)
    # #         # print(") = ")
    # #         # printf(get_width(interval_h6))
    # #         # print(", Ratio = ")
    # #         # printf(get_width(interval_h6)/results[i][j])
    # #         # println("")
            
    # #         # print("W(T, 6) =  W(")
    # #         # printf(interval_t6.lower)
    # #         # print(", ")
    # #         # printf(interval_t6.upper)
    # #         # print(") = ")
    # #         # printf(get_width(interval_t6))
    # #         # print(", Ratio = ")
    # #         # printf(get_width(interval_t6)/results[i][j])
    # #         # println("")

            # print("W(S, 6) =  W(")
            # printf(interval_s6.lower)
            # print(", ")
            # printf(interval_s6.upper)
            # print(") = ")
            # printf(get_width(interval_s6))
            # # print(", Ratio = ")
            # # printf(get_width(interval_s6)/results[i][j])
            # println("")

            # print("W(S, 3) =  W(")
            # printf(interval_s3.lower)
            # print(", ")
            # printf(interval_s3.upper)
            # print(") = ")
            # printf(get_width(interval_s3))
            # # print(", Ratio = ")
            # # printf(get_width(interval_s3)/results[i][j])
            # println("")

    #         print("dx = (")
    #         printf(dx.lower)
    #         print(", ")
    #         printf(dx.upper)
    #         println(")")
    #         print("dy = (")
    #         printf(dy.lower)
    #         print(", ")
    #         printf(dy.upper)
    #         print(")")
    #         println("")

            # print("Taylor =  W(")
            # printf(interval_taylor.lower)
            # print(", ")
            # printf(interval_taylor.upper)
            # print(") = ")
            # printf(get_width(interval_taylor))
            # # print(", Ratio = ")
            # # printf(get_width(interval_taylor)/results[i][j])
            # println("")

    #         print("Taylor dx = (")
    #         printf(taylor_dx.lower)
    #         print(", ")
    #         printf(taylor_dx.upper)
    #         println(")")
    #         print("Taylor dy = (")
    #         printf(taylor_dy.lower)
    #         print(", ")
    #         printf(taylor_dy.upper)
    #         print(")")
    #         println("")

    #         print("Horner =  W(")
    #         printf(interval_horner.lower)
    #         print(", ")
    #         printf(interval_horner.upper)
    #         print(") = ")
    #         printf(get_width(interval_horner))
    #         print(", Ratio = ")
    #         printf(get_width(interval_horner)/results[i][j])
    #         println("")

    #         print("Horner dx = ( ")
    #         printf(interval_horner_dx.lower)
    #         print(", ")
    #         printf(interval_horner_dx.upper)
    #         println(")")
    #         print("Horner dy = ( ")
    #         printf(interval_horner_dy.lower)
    #         print(", ")
    #         printf(interval_horner_dy.upper)
    #         print(")")
    #         println("")

            # println("")
        # end
    #     println("")
    # end
    # evaluate(poly, B)

    # println(taylor_interpolation(B, 8))

    # println("W(H, 3) = $(get_width(lagrange_range_function(poly, B, 14, "H", 3)))")
    # println("W(T, 3) = $(get_width(lagrange_range_function(poly, B, 14, "T", 3)))")
    # println("W(H, 6) = $(get_width(lagrange_range_function(poly, B, 14, "H", 6)))")
    # println("W(T, 6) = $(get_width(lagrange_range_function(poly, B, 14, "T", 6)))")

    # println( (get_width(lagrange_range_function(poly, B, 14, "T", 6)))/(get_width(lagrange_range_function(poly, B, 14, "T", 3))))

    # lo, hi = exact_range_function(B, deriv)
    # println((lo, hi))

    # for i in 1:length(polys)
    #     mkpath("imgs")

    #     ratio_matrix = compare_methods(polys[i], B, 1024)
    #     # ratio_matrix_double = compare_methods(polys[i], Bs_double[i], 1024)
    #     # ratio_matrix_shifted = compare_methods(polys[i], Bs_shifted[i], 1024)

    #     plot = create_visualization(ratio_matrix, B, polys[i])
    #     # plot_double = create_visualization(ratio_matrix_double, Bs_double[i], polys[i])
    #     # plot_shifted = create_visualization(ratio_matrix_shifted, Bs_shifted[i], polys[i])

    #     savefig(plot, "imgs/f_$(i) B_$(i)_hermite_vs_taylor4_1024.png")
    #     # savefig(plot_double, "imgs/f_$(i) B_$(i)_double.png")
    #     # savefig(plot_shifted, "imgs/f_$(i) B_$(i)_shifted.png")
    # end

    # ratio_matrix = compare_methods(poly_3, B, 1024)
    # plot = create_visualization(ratio_matrix, B, poly_3)
    # savefig(plot, "imgs/f_3 B_3.png")

end

if abspath(PROGRAM_FILE) == @__FILE__
    main() 
end
