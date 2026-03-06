#= 
    @author: Thomas Chen
    @advisors: Yap Chee, Kai Hormann, Bingwei Zhang
    @date: 2025-06-20
    @description: Taylor range function 

    There are three main functions in this file
        - taylor_interpolation(poly, B, degree): Computes the range of the polynomial at a given box B 
            using the taylor range function with the given max degree.
        - taylor_interpolation_dx(B, degree): Computes the derivative range w.r.t x of the polynomial at a given box B 
            using the taylor range function with the given max degree.
        - taylor_interpolation_dy(B, degree): Computes the derivative range w.r.t y of the polynomial at a given box B 
            using the taylor range function with the given max degree.
=# 

#=
    evaluate(poly::Polynomial, B::myBox)

    Evaluates the polynomial at the center point of the box.

    # Arguments
    - `poly::Polynomial`: The polynomial
    - `B::myBox`: The box
=#
function evaluate_Taylor(poly::Polynomial, B::myBox, sharing::Bool)

    global derivatives_taylor 
    global total_eval
    global total_points
    global max_x
    global max_y
    global total_degree

    center_pt = (B.x_2, B.y_2)

    if sharing
        if haskey(derivatives_taylor , center_pt)
            return 
        end
    end

    if max_x < 2 || max_y < 2
        point_derivatives = [zeros(Float64, 3) for _ in 1:3]
    else 
        point_derivatives = [zeros(Float64, max_y + 1) for _ in 1:max_x + 1]
    end

    for x_index in range(1, max_x + 1)
        for y_index in range(1, max_y + 1)
            if (x_index-1) + (y_index-1) > total_degree
                continue
            end
            order = monomial_string(x_index-1, y_index-1)
            deriv = eval_slp(poly.slp, center_pt[1], center_pt[2], order) 
            point_derivatives[x_index][y_index] = deriv
            total_eval += 1
        end
    end

    total_points += 1


    derivatives_taylor[center_pt] = point_derivatives
end

#=
    taylor_interpolation(poly::Polynomial, B::myBox, degree::Int)::myInterval

    Performs Taylor interpolation on the polynomial at the center point of the box.

    # Arguments
    - `poly::Polynomial`: The polynomial
    - `B::myBox`: The box
    - `degree::Int`: The degree of the Taylor polynomial

    # Returns
    - `::myInterval`: The interpolated interval
=#
function taylor_interpolation(poly::Polynomial, B::myBox, degree::Int; sharing::Bool=true)::myInterval
    x_2, y_2 = B.x_2, B.y_2
    rx, ry = B.rx, B.ry

    evaluate_Taylor(poly, B, sharing)

    center_pt = (x_2, y_2)
    if !haskey(derivatives_taylor, center_pt)
        return myInterval(0.0, 0.0)
    end

    D = derivatives_taylor[center_pt]
    numX = length(D)
    numY = length(D[1])

    # quadratic Taylor coefficients (centered coordinates: x,y are offsets)
    c_f  = D[1][1]
    c_x  = D[2][1]
    c_y  = D[1][2]
    c_xx = D[3][1]
    c_yy = D[1][3]
    c_xy = D[2][2]

    @inline quad_eval(c0,c1,c2,t) = (c2*t + c1)*t + c0

    # corners in centered coords: (±rx, ±ry)
    p_aa = c_f - c_x*rx - c_y*ry + c_xy*rx*ry + 0.5*c_xx*rx^2 + 0.5*c_yy*ry^2
    p_ab = c_f - c_x*rx + c_y*ry - c_xy*rx*ry + 0.5*c_xx*rx^2 + 0.5*c_yy*ry^2
    p_ba = c_f + c_x*rx - c_y*ry - c_xy*rx*ry + 0.5*c_xx*rx^2 + 0.5*c_yy*ry^2
    p_bb = c_f + c_x*rx + c_y*ry + c_xy*rx*ry + 0.5*c_xx*rx^2 + 0.5*c_yy*ry^2

    min_val = min(p_aa,p_ab,p_ba,p_bb)
    max_val = max(p_aa,p_ab,p_ba,p_bb)

    # boundary extrema: x fixed to ±rx, vary y in [-ry,ry]
    c2y = 0.5 * c_yy
    for dx in (-rx, rx)
        c0 = c_f + c_x*dx + 0.5*c_xx*dx^2
        c1 = c_y + c_xy*dx

        # endpoints
        min_val = min(min_val, quad_eval(c0,c1,c2y,-ry), quad_eval(c0,c1,c2y, ry))
        max_val = max(max_val, quad_eval(c0,c1,c2y,-ry), quad_eval(c0,c1,c2y, ry))

        if c2y != 0 && abs(c1) < 2*abs(c2y)*ry
            t0 = -c1/(2*c2y)
            if abs(t0) <= ry
                v = quad_eval(c0,c1,c2y,t0)
                min_val = min(min_val, v)
                max_val = max(max_val, v)
            end
        end
    end

    # boundary extrema: y fixed to ±ry, vary x in [-rx,rx]
    c2x = 0.5 * c_xx
    for dy in (-ry, ry)
        c0 = c_f + c_y*dy + 0.5*c_yy*dy^2
        c1 = c_x + c_xy*dy

        min_val = min(min_val, quad_eval(c0,c1,c2x,-rx), quad_eval(c0,c1,c2x, rx))
        max_val = max(max_val, quad_eval(c0,c1,c2x,-rx), quad_eval(c0,c1,c2x, rx))

        if c2x != 0 && abs(c1) < 2*abs(c2x)*rx
            t0 = -c1/(2*c2x)
            if abs(t0) <= rx
                v = quad_eval(c0,c1,c2x,t0)
                min_val = min(min_val, v)
                max_val = max(max_val, v)
            end
        end
    end

    # interior critical point (if Hessian is non-singular)
    Dp = c_xx*c_yy - c_xy^2
    if Dp != 0
        x0 = (-c_x*c_yy + c_y*c_xy) / Dp
        y0 = (-c_y*c_xx + c_x*c_xy) / Dp
        if abs(x0) <= rx && abs(y0) <= ry
            v = c_f + c_x*x0 + c_y*y0 + c_xy*x0*y0 + 0.5*c_xx*x0^2 + 0.5*c_yy*y0^2
            min_val = min(min_val, v)
            max_val = max(max_val, v)
        end
    end

    # fast remainder bound for total degree k >= 3
    invfact = Vector{Float64}(undef, degree + 1)
    invfact[1] = 1.0
    for k in 1:degree
        invfact[k+1] = invfact[k] / k
    end

    powx = Vector{Float64}(undef, degree + 1)
    powy = Vector{Float64}(undef, degree + 1)
    powx[1] = 1.0
    powy[1] = 1.0
    for k in 1:degree
        powx[k+1] = powx[k] * rx
        powy[k+1] = powy[k] * ry
    end

    S = 0.0
    @inbounds for k in 3:degree
        for i in 0:k
            j = k - i
            if (i + 1) <= numX && (j + 1) <= numY
                d = D[i+1][j+1]
                if d != 0.0
                    S += abs(d) * powx[i+1] * powy[j+1] * invfact[i+1] * invfact[j+1]
                end
            end
        end
    end

    return myInterval(min_val - S, max_val + S)
end

#=
    taylor_interpolation_dx(B::myBox, degree::Int)::myInterval

    Performs Taylor interpolation on the SLP polynomial at the center point of the box.

    # Arguments
    - `B::myBox`: The box
    - `degree::Int`: The degree of the Taylor polynomial

    # Returns
    - `::myInterval`: The interpolated interval
=#
function taylor_interpolation_dx(B::myBox, degree::Int)::myInterval
    x_2 = B.x_2
    y_2 = B.y_2
    rx = B.rx
    ry = B.ry


    center_pt = (x_2, y_2)

    center_derivatives = derivatives_taylor[center_pt]
    numX = length(center_derivatives)
    numY = length(center_derivatives[1])


    initial = myInterval(center_derivatives[2][1], center_derivatives[2][1])

    # if B.x_1 == -0.5 && B.x_3 == 1 && B.y_1 == -7 && B.y_3 == -5.5
    #     println("dx_intial = $initial")
    # end

    for k in 2:degree
        for i in 1:k
            j = k - i

            if i + 1 > numX || j + 1 > numY
                continue
            end

            #c = factorial(i-1) * factorial(j)
            if factorial_cache[i] == 0
                factorial_cache[i] = factorial(BigInt(i-1))
            end

            if factorial_cache[j+1] == 0
                factorial_cache[j+1] = factorial(BigInt(j))
            end

            d = center_derivatives[i+1][j+1]
            d = d/factorial_cache[i]
            d = d/factorial_cache[j+1]

            x_bound = rx^(i-1)
            y_bound = ry^j
            x_n = myInterval(-x_bound, x_bound)
            y_n = myInterval(-y_bound, y_bound)
            initial = initial + d * x_n * y_n 
        end
    end
    return initial
end

# function taylor_interpolation_dx(B, degree)

#     x_2 = B.x_2
#     y_2 = B.y_2
#     rx = B.rx
#     ry = B.ry


#     center_pt = (x_2, y_2)
#     if !haskey(derivatives_taylor , center_pt)
#         return myInterval(0.0, 0.0)
#     end

#     center_derivatives = derivatives_taylor [center_pt]
#     numX = length(center_derivatives)
#     numY = length(center_derivatives[1])

#     initial = myInterval(center_derivatives[2][1], center_derivatives[2][1])

#     for n in 2:degree
#         for i in 1:n
#             j = n - i

#             if i+1 > numX || j+1 > numY
#                 continue
#             end
        
#             if i == 0 && j == 0
#                 continue
#             end


#             c = factorial(i-1) * factorial(j)
#             d = center_derivatives[i+1][j+1]

#             x_bound = rx^i
#             y_bound = ry^j
#             x_n = myInterval(-x_bound, x_bound)
#             y_n = myInterval(-y_bound, y_bound)

#             initial = initial + 1/c * d * x_n * y_n 
#         end
#     end
#     return initial
# end

#=
    taylor_interpolation_dy(B::myBox, degree::Int)::myInterval

    Performs Taylor interpolation on the SLP polynomial at the center point of the box.

    # Arguments
    - `B::myBox`: The box
    - `degree::Int`: The degree of the Taylor polynomial

    # Returns
    - `::myInterval`: The interpolated interval
=#
function taylor_interpolation_dy(B::myBox, degree::Int)::myInterval

    x_2 = B.x_2
    y_2 = B.y_2
    rx = B.rx
    ry = B.ry


    center_pt = (x_2, y_2)

    center_derivatives = derivatives_taylor[center_pt]
    numX = length(center_derivatives)
    numY = length(center_derivatives[1])

    initial = myInterval(center_derivatives[1][2], center_derivatives[1][2])

    # println("dy_intial = $initial")

    for k in 2:degree
        for i in 0:k-1
            j = k - i

            if i + 1 > numX || j + 1 > numY
                continue
            end

            if factorial_cache[i+1] == 0
                factorial_cache[i+1] = factorial(BigInt(i))
            end

            if factorial_cache[j] == 0
                factorial_cache[j] = factorial(BigInt(j-1))
            end

            # c = factorial(i) * factorial(j-1)
            d = center_derivatives[i+1][j+1]
            d = d/factorial_cache[i+1]
            d = d/factorial_cache[j]
            x_bound = rx^i
            y_bound = ry^(j-1)
            x_n = myInterval(-x_bound, x_bound)
            y_n = myInterval(-y_bound, y_bound)
            initial = initial + d * x_n * y_n 
        end
    end
    
    return initial
end


function taylor_interpolation_linear(poly::Polynomial, B::myBox, degree::Int; sharing::Bool = true)::myInterval
    x_2, y_2 = B.x_2, B.y_2
    rx, ry = B.rx, B.ry

    evaluate_Taylor(poly, B, sharing)

    center_pt = (x_2, y_2)
    if !haskey(derivatives_taylor, center_pt)
        return myInterval(0.0, 0.0)
    end

    D = derivatives_taylor[center_pt]
    numX = length(D)
    numY = length(D[1])

    c_f = D[1][1]
    c_x = D[2][1]
    c_y = D[1][2]

    # Exact linear range over centered box: f0 + c_x*x + c_y*y, with x∈[-rx,rx], y∈[-ry,ry]
    min_val = c_f - abs(c_x)*rx - abs(c_y)*ry
    max_val = c_f + abs(c_x)*rx + abs(c_y)*ry

    invfact = Vector{Float64}(undef, degree + 1)
    invfact[1] = 1.0
    for k in 1:degree
        invfact[k+1] = invfact[k] / k
    end

    powx = Vector{Float64}(undef, degree + 1)
    powy = Vector{Float64}(undef, degree + 1)
    powx[1] = 1.0
    powy[1] = 1.0
    for k in 1:degree
        powx[k+1] = powx[k] * rx
        powy[k+1] = powy[k] * ry
    end

    # remainder for total degree k >= 2
    S = 0.0
    @inbounds for k in 2:degree
        for i in 0:k
            j = k - i
            if (i + 1) <= numX && (j + 1) <= numY
                d = D[i+1][j+1]
                if d != 0.0
                    S += abs(d) * powx[i+1] * powy[j+1] * invfact[i+1] * invfact[j+1]
                end
            end
        end
    end

    return myInterval(min_val - S, max_val + S)
end