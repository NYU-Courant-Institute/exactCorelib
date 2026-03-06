#!/usr/bin/env julia

# Test the new critical_point_range function

include("check_dep.jl")

using Printf
using BenchmarkTools
using Plots
using DataStructures
using Colors
using Dates
using NLsolve
using ForwardDiff

include("../SLP/myInterval.jl")
include("../SLP/polynomial.jl")
include("../SLP/derivatives.jl")
include("../SLP/eval.jl")
include("../iso_curve/myBox.jl")
include("../iso_curve/range_func.jl")
include("../iso_curve_Taylor/range_funcs.jl")

# Global Variables
max_x, max_y = 0, 0
total_degree = 0
derivatives = Dict{Tuple{Float64, Float64}, Vector{Vector{Float64}}}()
derivatives_taylor = Dict{Tuple{Float64, Float64}, Vector{Vector{Float64}}}()
total_eval = 0
total_points = 0
tol = 1e-10

# Get required functions from range_funcs.jl
include("range_funcs.jl")

# Test the critical_point_range function
println("Testing critical_point_range function...")

# Simple Polynomial: f(x,y) = x^2 + y^2
# On the box [-1, 1] × [-1, 1], the range should be [0, 2] 
poly_str = "x^2 + y^2"
poly = Polynomial(poly_str)
global max_x = get_max_order(poly, :x)
global max_y = get_max_order(poly, :y)
global total_degree = get_total_degree(poly) * 2

compute_third_derivatives_2D!(poly)

box = myBox(myInterval(-1.0, 1.0), myInterval(-1.0, 1.0))

range_result = critical_point_range(box, poly)
println("f(x,y) = $poly_str")
println("Box: [-1, 1] × [-1, 1]")
println("Computed range: [$(range_result.lower), $(range_result.upper)]")
println("Expected range: [0, 2]")
println()

# Test another polynomial: f(x,y) = x*y
# On the box [-1, 1] × [-1, 1], the range should be [-1, 1]
poly_str2 = "x*y"
poly2 = Polynomial(poly_str2)
global max_x = get_max_order(poly2, :x)
global max_y = get_max_order(poly2, :y)
global total_degree = get_total_degree(poly2) * 2

fill!(poly2.slp.FB, nothing)
fill!(poly2.slp.QB, [])
compute_third_derivatives_2D!(poly2)

range_result2 = critical_point_range(box, poly2)
println("f(x,y) = $poly_str2")
println("Box: [-1, 1] × [-1, 1]")
println("Computed range: [$(range_result2.lower), $(range_result2.upper)]")
println("Expected range: [-1, 1]")

println("\n✓ Test completed!")
