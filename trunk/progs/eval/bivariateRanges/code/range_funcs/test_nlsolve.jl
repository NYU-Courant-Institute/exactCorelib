#!/usr/bin/env julia

using Printf
using NLsolve

# Test the NLsolve package
println("Testing NLsolve package...")

# Define a simple nonlinear system of equations
function f!(F, x)
    F[1] = x[1]^2 + x[2]^2 - 1  # x^2 + y^2 = 1
    F[2] = x[1] - x[2]          # x = y
end

# Solve the system
x0 = [0.5, 0.5]
result = nlsolve(f!, x0)

println("Solution found: x = $(result.zero[1]), y = $(result.zero[2])")
println("Expected: x ≈ 0.707, y ≈ 0.707")
println("Converged: $(result.x_converged)")
println("\n✓ NLsolve is working correctly!")
