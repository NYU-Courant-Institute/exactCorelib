#= 
    @author: Thomas Chen
    @advisors: Yap Chee, Kai Hormann, Bingwei Zhang
    @date: 2025-06-23
    @description: Lagrange range function 

    There are three main functions in this file
        - lagrange_range_function(poly, B, k, method, R): Computes the range of the polynomial at a given box B 
            using the lagrange range function with the given method, term count R, and max degree k
            - term count R applies to method "S" only
        - dx_lagrange(B, method, k): Computes the derivative range w.r.t x of the polynomial at a given box B 
        using the lagrange range function with the given method and max degree k. 
        - dy_lagrange(B, method, k): Computes the derivative range w.r.t y of the polynomial at a given box B 
        using the lagrange range function with the given method and max degree k.
        
        For all three functions, 
        - Currently only support methods:
            - "T": Using taylor interpolation to compute lagrange interpolation function
            - "S": Using our split method to compute lagrange interpolation function
=# 

#= 
    evaluate(poly::Polynomial, B::myBox)

    Evaluates the polynomial and computes derivatives

    # Arguments
    - `poly`: The polynomial to evaluate
    - `B`: The box region where the evaluation is performed
=#
function evaluate(poly::Polynomial, B::myBox, sharing::Bool)
    slp = poly.slp


    global derivatives
    global total_points
    global total_eval
    global max_x
    global max_y
    global total_degree

    for i in range(1, 3)
        for j in range(1,3)
            pt = get_point(B, i, j)

            if sharing
                if haskey(derivatives, pt)
                    continue 
                end
            end

            max_x_multiples = div(max_x, 3) + 1
            max_y_multiples = div(max_y, 3) + 1  

            point_derivatives = [zeros(Float64, max_y_multiples+ 1) for _ in 1:max_x_multiples + 1]

            for x_index in range(1, max_x_multiples)
                for y_index in range(1, max_y_multiples)
                    x_order = (x_index - 1) * 3
                    y_order = (y_index - 1) * 3
                    if x_order + y_order > total_degree
                        continue
                    end
                    order = monomial_string(x_order, y_order)
                    deriv = eval_slp(slp, pt[1], pt[2], order) 
                    point_derivatives[x_index][y_index] = deriv
                    total_eval += 1
                end
            end

            total_points += 1
            derivatives[pt] = point_derivatives
        end
    end
end


#= 
    lagrange_bases(b::myBox)::Tuple{Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function}

    Computes the Lagrange basis functions for a given box region.

    # Arguments
    - `b`: The box region for which to compute the Lagrange basis functions
    
    # Returns
    - `Tuple{Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function}`: A tuple of Eighteen functions representing the Lagrange basis functions
=#
function lagrange_bases(b::myBox)::Tuple{Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function, Function}

    x_1 = b.x_1
    x_2 = b.x_2
    x_3 = b.x_3
    y_1 = b.y_1
    y_2 = b.y_2
    y_3 = b.y_3

    L1 = x -> ((x - x_2) * (x - x_3)) / ((x_1 - x_2) * (x_1 - x_3))
   
    L2 = x -> ((x - x_1) * (x - x_3)) / ((x_2 - x_1) * (x_2 - x_3))
    
    L3 = x -> ((x - x_1) * (x - x_2)) / ((x_3 - x_1) * (x_3 - x_2))

    dL1 = x -> (2x - x_2 - x_3) / ((x_1 - x_2) * (x_1 - x_3))
    dL2 = x -> (2x - x_1 - x_3) / ((x_2 - x_1) * (x_2 - x_3))
    dL3 = x -> (2x - x_1 - x_2) / ((x_3 - x_1) * (x_3 - x_2))

    ddL1 = x -> 2/((x_1-x_2)*(x_1-x_3))
    ddL2 = x -> 2/((x_2-x_1)*(x_2-x_3))
    ddL3 = x -> 2/((x_3-x_1)*(x_3-x_2))

    R1 = y -> ((y - y_2) * (y - y_3)) / ((y_1 - y_2) * (y_1 - y_3))

    R2 = y -> ((y - y_1) * (y - y_3)) / ((y_2 - y_1) * (y_2 - y_3))

    R3 = y -> ((y - y_1) * (y - y_2)) / ((y_3 - y_1) * (y_3 - y_2))

    dR1 = y -> (2y - y_2 - y_3) / ((y_1 - y_2) * (y_1 - y_3))
    dR2 = y -> (2y - y_1 - y_3) / ((y_2 - y_1) * (y_2 - y_3))
    dR3 = y -> (2y - y_1 - y_2) / ((y_3 - y_1) * (y_3 - y_2))

    ddR1 = y ->  2/((y_1-y_2)*(y_1-y_3))
    ddR2 = y -> 2/((y_2-y_1)*(y_2-y_3))
    ddR3 = y -> 2/((y_3-y_1)*(y_3-y_2))

    return L1, L2, L3, R1, R2, R3, dL1, dL2, dL3, dR1, dR2, dR3, ddL1, ddL2, ddL3, ddR1, ddR2, ddR3
end

#= 
    lagrange_interpolation(fs::Tuple{Function, Function, Function, Function, Function, Function, Function, Function, Function}, B::myBox)::Tuple{Function, Function, Function, Function, Function, Function, Function, Function, Function}

    Computes the Lagrange interpolation function for a given set of functions and box region.

    # Arguments
    - `fs`: A tuple of functions representing the function values at the grid points
    - `B`: The box region for which to compute the Lagrange interpolation function
    
    # Returns
    - `Tuple{Function, Function, Function, Function, Function, Function, Function, Function, Function}`: A tuple of Nine functions representing the Lagrange interpolation function
=#
function lagrange_interpolation(fs::Tuple{Function, Function, Function, Function, Function, Function, Function, Function, Function}, B::myBox)::Tuple{Function, Function, Function, Function, Function, Function, Function, Function, Function}
    f11, f12, f13,f21,f22,f23,f31,f32,f33 = fs

    L1, L2, L3, R1, R2, R3, dL1, dL2, dL3, dR1, dR2, dR3, ddL1, ddL2, ddL3, ddR1, ddR2, ddR3 = lagrange_bases(B)

    lagrange_function = (x, y) -> L1(x) * R1(y) * f11 + L2(x) * R1(y) * f21 + L3(x) * R1(y) * f31 +
    L1(x) * R2(y) * f12 + L2(x) * R2(y) * f22 + L3(x) * R2(y) * f32 +
    L1(x) * R3(y) * f13 + L2(x) * R3(y) * f23 + L3(x) * R3(y) * f33

    lagarange_dx = (x, y) -> dL1(x) * R1(y) * f11 + dL2(x) * R1(y) * f21 + dL3(x) * R1(y) * f31 +
    dL1(x) * R2(y) * f12 + dL2(x) * R2(y) * f22 + dL3(x) * R2(y) * f32 +
    dL1(x) * R3(y) * f13 + dL2(x) * R3(y) * f23 + dL3(x) * R3(y) * f33

    lagarange_dy = (x,y) -> L1(x) * dR1(y) * f11 + L2(x) * dR1(y) * f21 + L3(x) * dR1(y) * f31 +    
    L1(x) * dR2(y) * f12 + L2(x) * dR2(y) * f22 + L3(x) * dR2(y) * f32 +    
    L1(x) * dR3(y) * f13 + L2(x) * dR3(y) * f23 + L3(x) * dR3(y) * f33

    q_xx = (x,y) ->
      ddL1(x)*R1(y)*f11 + ddL2(x)*R1(y)*f21 + ddL3(x)*R1(y)*f31 +
      ddL1(x)*R2(y)*f12 + ddL2(x)*R2(y)*f22 + ddL3(x)*R2(y)*f32 +
      ddL1(x)*R3(y)*f13 + ddL2(x)*R3(y)*f23 + ddL3(x)*R3(y)*f33

    q_xy = (x,y) ->
      dL1(x)*dR1(y)*f11 + dL2(x)*dR1(y)*f21 + dL3(x)*dR1(y)*f31 +
      dL1(x)*dR2(y)*f12 + dL2(x)*dR2(y)*f22 + dL3(x)*dR2(y)*f32 +
      dL1(x)*dR3(y)*f13 + dL2(x)*dR3(y)*f23 + dL3(x)*dR3(y)*f33

    q_yy = (x,y) ->
      L1(x)*ddR1(y)*f11 + L2(x)*ddR1(y)*f21 + L3(x)*ddR1(y)*f31 +
      L1(x)*ddR2(y)*f12 + L2(x)*ddR2(y)*f22 + L3(x)*ddR2(y)*f32 +
      L1(x)*ddR3(y)*f13 + L2(x)*ddR3(y)*f23 + L3(x)*ddR3(y)*f33
    
    q_xxy = (x,y) ->
        ddL1(x)*dR1(y)*f11 + ddL2(x)*dR1(y)*f21 + ddL3(x)*dR1(y)*f31 +
        ddL1(x)*dR2(y)*f12 + ddL2(x)*dR2(y)*f22 + ddL3(x)*dR2(y)*f32 +
        ddL1(x)*dR3(y)*f13 + ddL2(x)*dR3(y)*f23 + ddL3(x)*dR3(y)*f33

    q_xyy = (x,y) ->
      dL1(x)*ddR1(y)*f11 + dL2(x)*ddR1(y)*f21 + dL3(x)*ddR1(y)*f31 +
      dL1(x)*ddR2(y)*f12 + dL2(x)*ddR2(y)*f22 + dL3(x)*ddR2(y)*f32 +
      dL1(x)*ddR3(y)*f13 + dL2(x)*ddR3(y)*f23 + dL3(x)*ddR3(y)*f33

    q_xxyy = (x,y) ->
      ddL1(x)*ddR1(y)*f11 + ddL2(x)*ddR1(y)*f21 + ddL3(x)*ddR1(y)*f31 +
      ddL1(x)*ddR2(y)*f12 + ddL2(x)*ddR2(y)*f22 + ddL3(x)*ddR2(y)*f32 +
      ddL1(x)*ddR3(y)*f13 + ddL2(x)*ddR3(y)*f23 + ddL3(x)*ddR3(y)*f33
    
    return lagrange_function, lagarange_dx, lagarange_dy, q_xx, q_xy, q_yy, q_xxy, q_xyy, q_xxyy
end

#= 
    bicentric_lagrange_interpolation(fs::Vector{Float64}, B::myBox)::Tuple{Float64, Float64, Float64, Float64, Float64, Float64, Float64, Float64, Float64}

    Computes the bicentric Lagrange interpolation for a given set of function values and box region.

    # Arguments
    - `fs`: A vector of 9 function values corresponding to the corners of the box region
    - `B`: The box region for which to compute the bicentric Lagrange interpolation
    
    # Returns
    - `Tuple{Float64, Float64, Float64, Float64, Float64, Float64, Float64, Float64, Float64}`: A tuple of 9 coefficients representing the bicentric Lagrange interpolation
=#
function bicentric_lagrange_interpolation(fs::Vector{Float64}, B::myBox)::Tuple{Float64, Float64, Float64, Float64, Float64, Float64, Float64, Float64, Float64}
    rx = B.rx
    ry = B.ry

    f11, f12, f13,f21,f22,f23,f31,f32,f33 = fs

    q_00 = f22 
    q_10 = (f32 - f12)/(2*rx)
    q_01 = (f23 - f21)/(2*ry)
    q_20 = (f12 - 2*f22 + f32)/(2*rx^2)
    q_11 = (f11 - f13 - f31 + f33)/(4*rx*ry)
    q_02 = (f21 - 2*f22 + f23)/(2*ry^2)
    q_21 = (f33 - 2*f23 + f13 - f31 + 2*f21 - f11)/(4*rx^2*ry)
    q_12 = (f33 - 2*f32 + f31 - f13 + 2*f12 - f11)/(4*rx*ry^2)
    q_22 = (f11 - 2*f12 + f13 - 2*f21 + 4*f22 - 2*f23 + f31 - 2*f32 + f33)/(4*rx^2*ry^2)

    return q_00, q_10, q_01, q_20, q_11, q_02, q_21, q_12, q_22
end

#=
    lagrange_range_function(poly::Polynomial, B::myBox, k::Int, method::String, R::Int)::myInterval

    Computes the range function for a given polynomial, box region, and parameters.

    # Arguments
    - `poly`: The polynomial for which to compute the range function
    - `B`: The box region for which to compute the range function
    - `k`: The number of recursive steps to perform
    - `method`: The method to use for range computation (e.g., "T" for Taylor series)
    - `R`: The number of terms in the Taylor series
    
    # Returns
    - `myInterval`: The range interval computed using the specified method
=#
function lagrange_range_function(poly::Polynomial, B::myBox, k::Int, method::String, R::Int; sharing::Bool = true)::myInterval
    evaluate(poly, B, sharing)
    return recurse_lagrange(B, 1, 1, k, method, R, sharing)
end

#=
    recurse_lagrange(B::myBox, i::Int, j::Int, k::Int, method::String, R::Int, sharing::Bool)::myInterval

    Recursively computes the range function using the specified method.

    # Arguments
    - `B`: The box region for which to compute the range function
    - `i`: The index of the x variable in the box region
    - `j`: The index of the y variable in the box region
    - `k`: The number of recursive steps to perform
    - `method`: The method to use for range computation (e.g., "T" for Taylor series)
    - `R`: The number of terms in the Taylor series
    
    # Returns
    - `myInterval`: The range interval computed using the specified method
=#
function recurse_lagrange(B::myBox, i::Int, j::Int, k::Int, method::String, R::Int, sharing::Bool)::myInterval

    x_1 = B.x_1
    x_2 = B.x_2
    x_3 = B.x_3
    y_1 = B.y_1
    y_2 = B.y_2
    y_3 = B.y_3
    rx = B.rx
    ry = B.ry

    if !isempty(derivatives)
        first_point_derivatives = first(values(derivatives))
        numX = length(first_point_derivatives)           
        numY = length(first_point_derivatives[1])   
    else
        numX = 0
        numY = 0
    end
   
    if i < 1 || i > numX || j< 1 || j > numY
        return myInterval(0.0, 0.0)
    end

    if k <= 0
        return myInterval(0.0, 0.0)
    else 
        nine_derivatives = get_deriv_nine(B, i, j)
        if all(abs(v) <= tol for v in nine_derivatives)
            return myInterval(0.0, 0.0)
        end
        
        if method == "T"
            q, q_x, q_y, q_xx, q_xy, q_yy, q_xxy, q_xyy, q_xxyy = lagrange_interpolation(nine_derivatives, B)
            B.QB[i,j] = [q_x, q_y, q_xx, q_xy, q_yy, q_xxy, q_xyy, q_xxyy]
            initial = q(x_2, y_2)
            partials = Dict{Tuple{Int,Int}, Function}(
                (0,0) => q,
                (1,0) => q_x,
                (0,1) => q_y,
                (2,0) => q_xx,
                (1,1) => q_xy,
                (0,2) => q_yy,
                (2,1) => q_xxy,
                (1,2) => q_xyy,
                (2,2) => q_xxyy,
            )

            for n in 1:4
                for i_index in 0:n
                    j_index = n - i_index
                    c = factorial(i_index) * factorial(j_index)
                    d = get(partials, (i_index,j_index), (x,y)->0)

                    x_bound = rx^i_index
                    y_bound = ry^j_index
                    x_n = myInterval(-x_bound, x_bound)
                    y_n = myInterval(-y_bound, y_bound)

                    initial = initial + 1/c * d(x_2, y_2) * x_n * y_n 
                end
            end
        elseif method == "S"
            ex_pts = [(x_1, y_1), (x_3, y_3), (x_1, y_3), (x_3, y_1)]
            if sharing
                if B.QB[i,j] != []
                    q_00, q_10, q_01, q_20, q_11, q_02, q_21, q_12, q_22 = B.QB[i,j]
                else 
                    q_00, q_10, q_01, q_20, q_11, q_02, q_21, q_12, q_22 = bicentric_lagrange_interpolation(nine_derivatives, B)
                    B.QB[i,j] = [q_00, q_10, q_01, q_20, q_11, q_02, q_21, q_12, q_22]
                end
            else
                q_00, q_10, q_01, q_20, q_11, q_02, q_21, q_12, q_22 = bicentric_lagrange_interpolation(nine_derivatives, B)
                B.QB[i,j] = [q_00, q_10, q_01, q_20, q_11, q_02, q_21, q_12, q_22]
            end

            p1 = (x,y) -> q_00 + q_10 * (x-x_2) + q_01 * (y-y_2) + q_20 * (x-x_2)^2 + q_11 * (x-x_2) * (y-y_2) + q_02 * (y-y_2)^2
            p2 = (x,y) -> q_21 * (x-x_2)^2 * (y-y_2) + q_12 * (x-x_2) * (y-y_2)^2 + q_22 * (x-x_2)^2 * (y-y_2)^2

            x_inner_ex = (4*x_2*q_02*q_20 - x_2*q_11^2 + q_01*q_11 - 2*q_02*q_10)/(4*q_02*q_20 - q_11^2) 
            y_inner_ex = (4*y_2*q_02*q_20 - y_2*q_11^2 + q_10*q_11 - 2*q_01*q_20)/(4*q_02*q_20 - q_11^2) 

            if x_inner_ex <= x_3 && x_inner_ex >= x_1 && y_inner_ex <= y_3 && y_inner_ex >= y_1
                push!(ex_pts, (x_inner_ex, y_inner_ex))
            end

            x_bound_ex = (2*x_2*q_20 + y_2*q_11 - y_1*q_11 - q_10)/(2*q_20)

            if x_bound_ex <= x_3 && x_bound_ex >= x_1
                push!(ex_pts, (x_bound_ex, y_1))
            end

            x_bound_ex = (2*x_2*q_20 + y_2*q_11 - y_3*q_11 - q_10)/(2*q_20)

            if x_bound_ex <= x_3 && x_bound_ex >= x_1
                push!(ex_pts, (x_bound_ex, y_3))
            end

            y_bound_ex = (2*y_2*q_02 + x_2*q_11 - x_1*q_11 - q_01)/(2*q_02)

            if y_bound_ex <= y_3 && y_bound_ex >= y_1
                push!(ex_pts, (x_1, y_bound_ex))
            end

            y_bound_ex = (2*y_2*q_02 + x_2*q_11 - x_3*q_11 - q_01)/(2*q_02)

            if y_bound_ex <= y_3 && y_bound_ex >= y_1
                push!(ex_pts, (x_3, y_bound_ex))
            end
            
            max_val, min_val = -Inf, Inf
            for pt in ex_pts
                max_val = max(max_val, p1(pt[1], pt[2]))
                min_val = min(min_val, p1(pt[1], pt[2]))
            end

            p1_interval = myInterval(min_val, max_val)

            ex_pts = [(x_1, y_1), (x_3, y_3), (x_1, y_3), (x_3, y_1), (x_2, y_2)]
            
            x_inner_ex = (2*x_2*q_22 - 3*q_12)/(2*q_22) 
            y_inner_ex = (2*y_2*q_22 - 3*q_21)/(2*q_22)

            if x_inner_ex <= x_3 && x_inner_ex >= x_1 && y_inner_ex <= y_3 && y_inner_ex >= y_1
                push!(ex_pts, (x_inner_ex, y_inner_ex))
            end

            x_bound_ex = (2*x_2*y_2*q_22 - 2*x_2*y_1*q_22 - 2*x_2*q_21 - y_2*q_12 + y_1*q_12)/(2*(y_2*q_22 - y_1*q_22 - q_21))

            if x_bound_ex <= x_3 && x_bound_ex >= x_1
                push!(ex_pts, (x_bound_ex, y_1))
            end

            x_bound_ex = (2*x_2*y_2*q_22 - 2*x_2*y_3*q_22 - 2*x_2*q_21 - y_2*q_12 + y_3*q_12)/(2*(y_2*q_22 - y_3*q_22 - q_21))

            if x_bound_ex <= x_3 && x_bound_ex >= x_1
                push!(ex_pts, (x_bound_ex, y_3))
            end

            y_bound_ex = (2*x_2*y_2*q_22 - 2*y_2*x_1*q_22 - x_2*q_21 - 2*y_2*q_12 + x_1*q_21)/(2*(x_2*q_22 - x_1*q_22 - q_12))

            if y_bound_ex <= y_3 && y_bound_ex >= y_1
                push!(ex_pts, (x_1, y_bound_ex))
            end

            y_bound_ex = (2*x_2*y_2*q_22 - 2*y_2*x_3*q_22 - x_2*q_21 - 2*y_2*q_12 + x_3*q_21)/(2*(x_2*q_22 - x_3*q_22 - q_12))

            if y_bound_ex <= y_3 && y_bound_ex >= y_1
                push!(ex_pts, (x_3, y_bound_ex))
            end
            
            max_val, min_val = -Inf, Inf
            for pt in ex_pts
                max_val = max(max_val, p2(pt[1], pt[2]))
                min_val = min(min_val, p2(pt[1], pt[2]))
            end

            p2_interval = myInterval(min_val, max_val)

            initial = p1_interval + p2_interval
        # elseif method == "H"
        #     q= lagrange_interpolation_horner(nine_derivatives)
        #     initial = q(x_interval, y_interval)
        else 
            error("Method not recognized")
        end

        exact = myInterval(initial.lower, initial.upper)

        x_term = sqrt(3)/27 * rx^3
        y_term = sqrt(3)/27 * ry^3


        if sharing
            if isnothing(B.FB[i+1, j])
                B.FB[i+1, j] = recurse_lagrange(B, i+1, j, k - 3, method, R, sharing)
            end
            if isnothing(B.FB[i, j+1])
                B.FB[i, j+1] = recurse_lagrange(B, i, j+1, k - 3, method, R, sharing)
            end
        else 
            B.FB[i+1, j] = recurse_lagrange(B, i+1, j, k - 3, method, R, sharing)
            B.FB[i, j+1] = recurse_lagrange(B, i, j+1, k - 3, method, R, sharing)
        end

        if R < 4
            x_term = sqrt(3)/24 * rx^3
            y_term = sqrt(3)/24 * ry^3
            B.FB[i,j] = exact + myInterval(-x_term, x_term) * B.FB[i+1, j] + myInterval(-y_term, y_term) * B.FB[i, j+1]
            return B.FB[i,j]
        elseif R > 5
            if sharing 
                if isnothing(B.FB[i+1, j+1])
                    B.FB[i+1, j+1] = recurse_lagrange(B, i+1, j+1, k - 6, method, R, sharing)
                end
            else 
                B.FB[i+1, j+1] = recurse_lagrange(B, i+1, j+1, k - 6, method, R, sharing)
            end
            B.FB[i,j] = exact + myInterval(-x_term, x_term) * B.FB[i+1, j] + myInterval(-y_term, y_term) * B.FB[i, j+1] + myInterval(-x_term, x_term) * myInterval(-y_term, y_term) * B.FB[i+1, j+1]
            return B.FB[i,j]
        else
            error("R not recognized")
        end
    end
end


#=
    max_abs(I::myInterval)::Float64

    Computes the maximum absolute value of an interval.

    # Arguments
    - `I::myInterval`: The interval for which to compute the maximum absolute value

    # Returns
    - `Float64`: The maximum absolute value of the interval
=#
function max_abs(I::myInterval)::Float64
    return max(abs(I.lower), abs(I.upper))
end

#= 
    dx_lagrange(B::myBox, method::String, k::Int)::myInterval

    Recursively computes the range function along the x-axis using the specified method.

    # Arguments
    - `B`: The box region for which to compute the range function
    - `method`: The method to use for range function computation
    - `k`: The number of recursive steps to perform

    # Returns
    - `myInterval`: The range interval computed using the specified method
=#
function dx_lagrange(B::myBox, method::String)::myInterval

    x_1 = B.x_1
    x_2 = B.x_2
    x_3 = B.x_3
    y_1 = B.y_1
    y_2 = B.y_2
    y_3 = B.y_3
    rx = B.rx
    ry = B.ry

    if method == "T" 
        q_x, _, q_xx, q_xy, _, q_xxy, q_xyy, q_xxyy = B.QB[1,1]
        initial = q_x(x_2, y_2)
        partials = Dict{Tuple{Int,Int}, Function}(
            (0,0) => q_x,
            (1,0) => q_xx,
            (0,1) => q_xy,
            (1,1) => q_xxy,
            (0,2) => q_xyy,
            (1,2) => q_xxyy,
        )

        for n in 1:3
            for i_index in 0:n
                j_index = n - i_index
                c = factorial(i_index) * factorial(j_index)
                d = get(partials, (i_index,j_index), (x,y)->0)

                x_bound = rx^i_index
                y_bound = ry^j_index
                x_n = myInterval(-x_bound, x_bound)
                y_n = myInterval(-y_bound, y_bound)

                initial = initial + 1/c * d(x_2, y_2) * x_n * y_n 
            end
        end
    elseif method == "S"
        q_00, q_10, q_01, q_20, q_11, q_02, q_21, q_12, q_22 = B.QB[1,1]

        ex_pts = [(x_1, y_1), (x_3, y_3), (x_1, y_3), (x_3, y_1)]
        qx = (x,y) -> q_10 + 2*q_20 * (x-x_2) + q_11 * (y-y_2) + 2 * q_21 * (x-x_2) * (y-y_2) + q_12 * (y-y_2)^2 + 2*q_22*(x-x_2)*(y-y_2)^2

        delta_arg = -4 * q_20 * q_22 + q_21^2
        if delta_arg <= 0
            delta = 0
        else
            delta = sqrt(delta_arg)
        end

        if delta != 0
            x_inner_ex = (-2 * delta * x_2 * q_22 + delta * q_12 - q_11 * q_22 + q_12 * q_21)/(-2 * q_22 * delta)
            y_inner_ex = (-2* y_2 * q_22 + q_21 + delta)/(-2 * q_22)

            if x_inner_ex <= x_3 && x_inner_ex >= x_1 && y_inner_ex <= y_3 && y_inner_ex >= y_1
                push!(ex_pts, (x_inner_ex, y_inner_ex))
            end

            x_inner_ex = (-2 * delta * x_2 * q_22 + delta * q_12 + q_11 * q_22 - q_12 * q_21)/(-2 * q_22 * delta)
            y_inner_ex = (2* y_2 * q_22 - q_21 + delta)/(2 * q_22)

            if x_inner_ex <= x_3 && x_inner_ex >= x_1 && y_inner_ex <= y_3 && y_inner_ex >= y_1
                push!(ex_pts, (x_inner_ex, y_inner_ex))
            end
        end

        y_bound_ex = (4*x_2*y_2*q_22 - 4*y_2*x_1*q_22 - 2*x_2*q_21 - 2*y_2*q_12 + 2*x_1*q_21 + q_11)/(2*(2*x_2*q_22 - 2*x_1*q_22 - q_12))

        if y_bound_ex <= y_3 && y_bound_ex >= y_1
            push!(ex_pts, (x_1, y_bound_ex))
        end

        y_bound_ex = (4*x_2*y_2*q_22 - 4*y_2*x_3*q_22 - 2*x_2*q_21 - 2*y_2*q_12 + 2*x_3*q_21 + q_11)/(2*(2*x_2*q_22 - 2*x_3*q_22 - q_12))

        if y_bound_ex <= y_3 && y_bound_ex >= y_1
            push!(ex_pts, (x_3, y_bound_ex))
        end
        
        max_val, min_val = -Inf, Inf
        for pt in ex_pts
            max_val = max(max_val, qx(pt[1], pt[2]))
            min_val = min(min_val, qx(pt[1], pt[2]))
        end

        initial = myInterval(min_val, max_val)
        # if B.x_1 == -0.75 && B.x_3 == -0.5 && B.y_1 == -0.25 && B.y_3 == 0.0
        #     println("Initial = $(initial)")
        # end
    else 
        error("Method not recognized")
    end

    exact = myInterval(initial.lower, initial.upper)

    x_term = (rx^2)/6
    y_term = sqrt(3)/27 * ry^3

    if isnothing(B.FB[2, 1])
        dx = myInterval(0.0, 0.0)
    else
        dx = B.FB[2, 1]
    end

    if isnothing(B.FB[1, 2])
        dy = myInterval(0.0, 0.0)
    else
        dy = B.FB[1, 2]
    end

    return exact + myInterval(-x_term, 2 * x_term) * dx + dy * (4/3) * myInterval(-y_term, y_term)
end

#= 
    dy_lagrange(B::myBox, method::String, k::Int)::myInterval

    Recursively computes the range function along the y-axis using the specified method.

    # Arguments
    - `B`: The box region for which to compute the range function
    - `method`: The method to use for range function computation
    - `k`: The number of recursive steps to perform

    # Returns
    - `myInterval`: The range interval computed using the specified method
=#
function dy_lagrange(B::myBox, method::String)::myInterval

    x_1 = B.x_1
    x_2 = B.x_2
    x_3 = B.x_3
    y_1 = B.y_1
    y_2 = B.y_2
    y_3 = B.y_3
    rx = B.rx
    ry = B.ry

    if method == "T"
        _, q_y, _, q_xy, q_yy, q_xxy, q_xyy, q_xxyy = B.QB[1,1]
        initial = q_y(x_2, y_2)
        partials = Dict{Tuple{Int,Int}, Function}(
            (0,0) => q_y,
            (0,1) => q_yy,
            (1,0) => q_xy,
            (2,0) => q_xxy,
            (1,1) => q_xyy,
            (2,1) => q_xxyy,
        )

        for n in 1:3
            for i_index in 0:n
                j_index = n - i_index
                c = factorial(i_index) * factorial(j_index)
                d = get(partials, (i_index,j_index), (x,y)->0)


                x_bound = rx^i_index
                y_bound = ry^j_index
                x_n = myInterval(-x_bound, x_bound)
                y_n = myInterval(-y_bound, y_bound)

                initial = initial + 1/c * d(x_2, y_2) * x_n * y_n 
            end
        end
    elseif method == "S"
        q_00, q_10, q_01, q_20, q_11, q_02, q_21, q_12, q_22 = B.QB[1,1]

        ex_pts = [(x_1, y_1), (x_3, y_3), (x_1, y_3), (x_3, y_1)]
        qy = (x,y) -> q_01 + 2*q_02 * (y-y_2) + q_11 * (x-x_2) + q_21 * (x-x_2)^2 + 2* q_12 * (x-x_2) * (y-y_2) + 2*q_22*(x-x_2)^2*(y-y_2)

        delta_arg = -4 * q_02 * q_22 + q_12^2
        if delta_arg <= 0
            delta = 0
        else
            delta = sqrt(delta_arg)
        end

        if delta != 0
            y_inner_ex = (-2 * delta * y_2 * q_22 + delta * q_21 - q_11 * q_22 + q_12 * q_21)/(-2 * q_22 * delta)
            x_inner_ex = (-2* x_2 * q_22 + q_12 + delta)/(-2 * q_22)

            if x_inner_ex <= x_3 && x_inner_ex >= x_1 && y_inner_ex <= y_3 && y_inner_ex >= y_1
                push!(ex_pts, (x_inner_ex, y_inner_ex))
            end

            y_inner_ex = (-2 * delta * y_2 * q_22 + delta * q_21 + q_11 * q_22 - q_12 * q_21)/(-2 * q_22 * delta)
            x_inner_ex = (2* x_2 * q_22 - q_12 + delta)/(2 * q_22)

            if x_inner_ex <= x_3 && x_inner_ex >= x_1 && y_inner_ex <= y_3 && y_inner_ex >= y_1
                push!(ex_pts, (x_inner_ex, y_inner_ex))
            end
        end

        x_bound_ex = (4*x_2*y_2*q_22 - 4*x_2*y_1*q_22 - 2*x_2*q_21 - 2*y_2*q_12 + 2*y_1*q_12 + q_11)/(2*(2*y_2*q_22 - 2*y_1*q_22 - q_21))

        if x_bound_ex <= x_3 && x_bound_ex >= x_1
            push!(ex_pts, (x_bound_ex, y_1))
        end

        x_bound_ex = (4*x_2*y_2*q_22 - 4*x_2*y_3*q_22 - 2*x_2*q_21 - 2*y_2*q_12 + 2*y_3*q_12 + q_11)/(2*(2*y_2*q_22 - 2*y_3*q_22 - q_21))

        if x_bound_ex <= x_3 && x_bound_ex >= x_1
            push!(ex_pts, (x_bound_ex, y_3))
        end
        
        max_val, min_val = -Inf, Inf
        for pt in ex_pts
            max_val = max(max_val, qy(pt[1], pt[2]))
            min_val = min(min_val, qy(pt[1], pt[2]))
        end

        initial = myInterval(min_val, max_val)
    else 
        error("Method not recognized")
    end

    exact = myInterval(initial.lower, initial.upper)

    x_term = sqrt(3)/27 * rx^3
    y_term = (ry^2)/6

    if isnothing(B.FB[2, 1])
        dx = myInterval(0.0, 0.0)
    else
        dx = B.FB[2, 1]
    end

    if isnothing(B.FB[1, 2])
        dy = myInterval(0.0, 0.0)
    else
        dy = B.FB[1, 2]
    end

    return exact + myInterval(-y_term, 2 * y_term) * dy * myInterval(-1, 1) + dx * (4/3) * myInterval(-x_term, x_term)
end