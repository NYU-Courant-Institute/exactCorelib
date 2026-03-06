#!/usr/bin/env julia

include("check_dep.jl")

using Printf
using NLsolve

include("../SLP/myInterval.jl")
include("../SLP/polynomial.jl")
include("../SLP/derivatives.jl")
include("../SLP/eval.jl")
include("../iso_curve/myBox.jl")

# Initialize Global Variables
INSTRUCTION_HASH = Dict{Tuple{Symbol, Union{Int, String}, Union{Int, String}}, Int}()

# Global Variables
max_x, max_y = 0, 0
total_degree = 0
derivatives = Dict{Tuple{Float64, Float64}, Vector{Vector{Float64}}}()
tol = 1e-10

# critical_point_range function from range_funcs.jl
function critical_point_range(box::myBox, poly::Polynomial)::myInterval
    x_1, x_3 = box.x.lower, box.x.upper
    y_1, y_3 = box.y.lower, box.y.upper
    
    candidate_values = Float64[]
    
    vars = Dict{Symbol, Union{Float64, myInterval}}()
    
    # ==================== 1. Evaluate the Function at the Corners ====================
    corners = [(x_1, y_1), (x_1, y_3), (x_3, y_1), (x_3, y_3)]
    for (x, y) in corners
        vars[:x], vars[:y] = x, y
        val = evaluate_slp_range(poly.slp, "", vars)
        push!(candidate_values, val)
        println("Corner ($x, $y): f = $val")
    end
    
    # ==================== 2. Internal Critical Points: ∇f = 0 ==================== 
    function gradient_func(vars_vec::Vector{Float64})
        x, y = vars_vec[1], vars_vec[2]
        
        var_dict_x = Dict{Symbol, Union{Float64, myInterval}}(:x => x, :y => y)
        var_dict_y = Dict{Symbol, Union{Float64, myInterval}}(:x => x, :y => y)
        
        fx = evaluate_slp_range(poly.slp, "x", var_dict_x)
        fy = evaluate_slp_range(poly.slp, "y", var_dict_y)
        
        return [fx, fy]
    end
    
    # Use multiple initial points to search for internal critical points
    n_search = 5
    interior_critical_count = 0
    for i in 1:n_search
        for j in 1:n_search
            x0 = x_1 + (x_3 - x_1) * (i / (n_search + 1))
            y0 = y_1 + (y_3 - y_1) * (j / (n_search + 1))
            
            try
                result = nlsolve(gradient_func, [x0, y0], method=:trust_region, ftol=1e-10)
                
                if result.x_converged || result.f_converged
                    x_sol, y_sol = result.zero[1], result.zero[2]
                    
                    # Strict Check: Must be inside the box (not on the boundary)
                    if x_1 < x_sol < x_3 && y_1 < y_sol < y_3
                        vars[:x], vars[:y] = x_sol, y_sol
                        val = evaluate_slp_range(poly.slp, "", vars)
                        push!(candidate_values, val)
                        interior_critical_count += 1
                        println("Internal Critical Point ($x_sol, $y_sol): f = $val")
                    end
                end
            catch
            end
        end
    end
    
    if interior_critical_count == 0
        println("Did not find any internal critical points.")
    end
    
    # ==================== 3. Bottom Edge Extremum: y = y_1, find solution to df/dx = 0 ====================
    function bottom_edge_deriv(x_vec::Vector{Float64})
        x = x_vec[1]
        var_dict = Dict{Symbol, Union{Float64, myInterval}}(:x => x, :y => y_1)
        fx = evaluate_slp_range(poly.slp, "x", var_dict)
        return [fx]
    end
    
    bottom_count = 0
    try
        for x_init in range(x_1 + (x_3-x_1)*0.1, x_3 - (x_3-x_1)*0.1, length=3)
            result = nlsolve(bottom_edge_deriv, [x_init], method=:trust_region, ftol=1e-10)
            if result.x_converged || result.f_converged
                x_sol = result.zero[1]
                if x_1 < x_sol < x_3
                    vars[:x], vars[:y] = x_sol, y_1
                    val = evaluate_slp_range(poly.slp, "", vars)
                    push!(candidate_values, val)
                    bottom_count += 1
                    println("Bottom Edge Extremum ($x_sol, $y_1): f = $val")
                end
            end
        end
    catch
    end
    
    if bottom_count == 0
        println("Bottom Edge Extremum not found.")
    end
    
    # ==================== 4. Top Edge Extremum: y = y_3, find solution to df/dx = 0 ====================
    function top_edge_deriv(x_vec::Vector{Float64})
        x = x_vec[1]
        var_dict = Dict{Symbol, Union{Float64, myInterval}}(:x => x, :y => y_3)
        fx = evaluate_slp_range(poly.slp, "x", var_dict)
        return [fx]
    end
    
    top_count = 0
    try
        for x_init in range(x_1 + (x_3-x_1)*0.1, x_3 - (x_3-x_1)*0.1, length=3)
            result = nlsolve(top_edge_deriv, [x_init], method=:trust_region, ftol=1e-10)
            if result.x_converged || result.f_converged
                x_sol = result.zero[1]
                if x_1 < x_sol < x_3
                    vars[:x], vars[:y] = x_sol, y_3
                    val = evaluate_slp_range(poly.slp, "", vars)
                    push!(candidate_values, val)
                    top_count += 1
                    println("Top Edge Extremum ($x_sol, $y_3): f = $val")
                end
            end
        end
    catch
    end
    
    if top_count == 0
        println("Top Edge Extremum not found.")
    end
    
    # ==================== 5. Left Edge Extremum: x = x_1, find solution to df/dy = 0 ====================
    function left_edge_deriv(y_vec::Vector{Float64})
        y = y_vec[1]
        var_dict = Dict{Symbol, Union{Float64, myInterval}}(:x => x_1, :y => y)
        fy = evaluate_slp_range(poly.slp, "y", var_dict)
        return [fy]
    end
    
    left_count = 0
    try
        for y_init in range(y_1 + (y_3-y_1)*0.1, y_3 - (y_3-y_1)*0.1, length=3)
            result = nlsolve(left_edge_deriv, [y_init], method=:trust_region, ftol=1e-10)
            if result.x_converged || result.f_converged
                y_sol = result.zero[1]
                if y_1 < y_sol < y_3
                    vars[:x], vars[:y] = x_1, y_sol
                    val = evaluate_slp_range(poly.slp, "", vars)
                    push!(candidate_values, val)
                    left_count += 1
                    println("Left Edge Extremum ($x_1, $y_sol): f = $val")
                end
            end
        end
    catch
    end
    
    if left_count == 0
        println("Left Edge Extremum not found.")
    end
    
    # ==================== 6. Right Edge Extremum: x = x_3, find solution to df/dy = 0 ====================
    function right_edge_deriv(y_vec::Vector{Float64})
        y = y_vec[1]
        var_dict = Dict{Symbol, Union{Float64, myInterval}}(:x => x_3, :y => y)
        fy = evaluate_slp_range(poly.slp, "y", var_dict)
        return [fy]
    end
    
    right_count = 0
    try
        for y_init in range(y_1 + (y_3-y_1)*0.1, y_3 - (y_3-y_1)*0.1, length=3)
            result = nlsolve(right_edge_deriv, [y_init], method=:trust_region, ftol=1e-10)
            if result.x_converged || result.f_converged
                y_sol = result.zero[1]
                if y_1 < y_sol < y_3
                    vars[:x], vars[:y] = x_3, y_sol
                    val = evaluate_slp_range(poly.slp, "", vars)
                    push!(candidate_values, val)
                    right_count += 1
                    println("Right Edge Extremum ($x_3, $y_sol): f = $val")
                end
            end
        end
    catch
    end
    
    if right_count == 0
        println("Right Edge Extremum not found.")
    end
    
    # ==================== Compute Minimum and Maximum ====================
    if isempty(candidate_values)
        println("No candidate values found.")
        return myInterval(
            minimum([evaluate_slp_range(poly.slp, "", Dict{Symbol, Union{Float64, myInterval}}(:x => x, :y => y)) for (x, y) in corners]),
            maximum([evaluate_slp_range(poly.slp, "", Dict{Symbol, Union{Float64, myInterval}}(:x => x, :y => y)) for (x, y) in corners])
        )
    end
    
    min_val = minimum(candidate_values)
    max_val = maximum(candidate_values)
    
    return myInterval(min_val, max_val)
end

# Main Test
println("=" ^ 80)
println("Test critical_point_range with real_interval")
println("=" ^ 80)

poly_1 = "50*x^10+(249*y^2-57)*x^8+(498*y^4-227*y^2-1)*x^6+(498*y^6-341*y^4-3*y^2+16)*x^4+(249*y^8-227*y^6-3*y^4-102*y^2-1)*x^2+50*y^10-57*y^8-y^6+16*y^4-y^2-1"

poly = Polynomial(poly_1)
global max_x = get_max_order(poly, :x)
global max_y = get_max_order(poly, :y)
global total_degree = get_total_degree(poly) * 2

compute_third_derivatives_2D!(poly)

# Box: x ∈ [-0.3, 0.3], y ∈ [0.2, 0.8]
B = myBox(myInterval(-0.1, -0.09), myInterval(-0.2, -0.19))

println("\nPolynomial: $poly_1")
println("\nBox: x ∈ [$(B.x.lower), $(B.x.upper)], y ∈ [$(B.y.lower), $(B.y.upper)]")
println("\n" ^ 1)
println("Find Critical Points:")
println("-" ^ 80)

real_interval = critical_point_range(B, poly)

println("-" ^ 80)
println("\nFinal Result:")
println("=" ^ 80)
println("real_interval = [$(@sprintf("%.16e", real_interval.lower)), $(@sprintf("%.16e", real_interval.upper))]")
println("Interval Width = $(@sprintf("%.16e", real_interval.upper - real_interval.lower))")
println("=" ^ 80)
