include("utils.jl")
include("parser.jl")
include("myInterval.jl")
using BenchmarkTools
using DataStructures
using Combinatorics

zero_interval = myInterval(0.0,0.0)


"""
     Define the SLP Struct
     
     Has variable varsConst and codelist that holds a vector of Tuples(Array of tuples)
     Tuples in varsConst have the composition of {Int, Any, Float64}: {Index, Variable or Constant, value}. 
     Tuples in codelist have the composition of {Int, Symbol, Int, Int, Float64}: {Index, operation, arg1, arg2, value}
"""
struct SLP 
    varsConst::Vector{Tuple{Int, Any, Union{Float64,myInterval}}}
    codelist::Vector{Tuple{Int, Symbol, Int, Int, Union{Float64,myInterval}}}
end


"""
     parse_derivative(derivative_str::String)
     
     Parse a derivative string (like "x^2y") into a multi-index dictionary.
     For example, "x^2y" becomes Dict("x" => 2, "y" => 1).
"""
function parse_derivative(derivative_str::String)
     mi = Dict{String,Int}()
     i = 1
     n = lastindex(derivative_str)
     while i <= n
         c = derivative_str[i]
         if isletter(c)
             var = string(c)
             i += 1
             # If the next character is '^', then read the exponent.
             exp = 1
             if i <= n && derivative_str[i] == '^'
                 i += 1
                 exp_str = ""
                 while i <= n && isdigit(derivative_str[i])
                     exp_str *= derivative_str[i]
                     i += 1
                 end
                 exp = parse(Int, exp_str)
             end
             mi[var] = get(mi, var, 0) + exp
         else
             # Skip any non-letter characters (like spaces)
             i += 1
         end
     end
     return mi
 end
 
 

"""
     multi_index_to_string(mi::Dict{String,Int})
     
     Converts a multi-index dictionary back to its string representation. 
     For example, Dict("x" => 2, "y" => 1) becomes "x^2y".
"""
function multi_index_to_string(mi::Dict{String,Int})
    if isempty(mi)
         return ""
    end
    keys_sorted = sort(collect(keys(mi)))
    code = ""
    for k in keys_sorted
        exp = mi[k]
        if exp == 1
            code *= k
        else
            code *= "$k^$exp"
        end
    end
    return code
end

"""
     multi_index_add(mi1::Dict{String,Int}, mi2::Dict{String,Int})
     
     Adds two multi_index dictionaries together returning a new mi dictionary with combined derivative entries
"""
function multi_index_add(mi1::Dict{String,Int}, mi2::Dict{String,Int})
    mi = copy(mi1)
    for (k, v) in mi2
         mi[k] = get(mi, k, 0) + v
    end
    return mi
end

"""
     multi_index_binom(mi::Dict{String,Int}, beta::Dict{String,Int})
     
     Compute the multi-index binomial coefficient.
     Given α (as mi) and a sub-index β, compute ∏₍v₎ binom(α[v], β[v]),
     where missing keys are treated as 0.
"""
function multi_index_binom(mi::Dict{String,Int}, beta::Dict{String,Int})
    result = 1.0
    for v in union(keys(mi), keys(beta))
         total = get(mi, v, 0)
         part = get(beta, v, 0)
         result *= binomial(total, part)
    end
    return result
end

"""
     leaf_series(variable, value)

     generates the multi_index dictinary for leaf nodes in the SLP(variables and constants)
     
     In our multi_index dictionary, the key "" represents the 0th derivative.
     For a leaf node:
          - If the leaf is a variable x with value v, its series is { "" => v, "x" => 1.0 }.
          - For a constant c, the series is { "" => c }.
"""

function leaf_series(variable, value)
    if variable isa Symbol
        if value isa myInterval
            return Dict("" => value,
                        string(variable) => myInterval(1.0, 1.0))
        else
            return Dict("" => value,
                        string(variable) => 1.0)
        end
    else
        return Dict("" => value)
    end
end

"""
     add_series(s1::Dict{String,Float64}, s2::Dict{String,Float64})

     Adds the derivative array of two steps of SLP together since addition in the derivatives context is trivial 
"""
function add_series(s1::Dict{String,T}, s2::Dict{String, T}) where {T<:Union{Float64,myInterval}}
    s = Dict{String,T}()
    for (k, v) in s1
         s[k] = v
    end
    for (k, v) in s2
         s[k] = haskey(s, k) ? (s[k] + v) : v
    end
    return s
end

"""
     neg_series(s1::Dict{String,Float64}, s2::Dict{String,Float64})

     Negates the derivative array of a step of SLP for performing subtraction 
"""
function neg_series(s::Dict{String,T}) where {T<:Union{Float64,myInterval}}
     s_new = Dict{String,T}()
     for (k, v) in s
        if v isa myInterval
            s_new[k] = myInterval(-v.upper, -v.lower)
        else
            s_new[k] = -v
        end
     end
     return s_new
 end

"""
     multiply_series(s1::Dict{String,Float64}, s2::Dict{String,Float64})

     Multiplies the derivative array of two steps of SLP together using Leibniz rule(Higher order product rule for derivation)
"""
function multiply_series(s1::Dict{String,T}, s2::Dict{String,T}, max_orders::Dict{String,Int}) where {T<:Union{Float64,myInterval}}
    s = Dict{String,T}()
    for (k1, v1) in s1
        mi1 = (k1 == "" ? Dict{String,Int}() : parse_derivative(k1))

        for (k2, v2) in s2
            mi2 = (k2 == "" ? Dict{String,Int}() : parse_derivative(k2))
            mi_sum = multi_index_add(mi1, mi2)

            skip = false
            for (var, exp) in mi_sum
                allowed = get(max_orders, var, typemax(Int))
                if exp > allowed
                    skip = true
                    break
                end
            end
            if skip
                continue
            end

            k_sum = multi_index_to_string(mi_sum)
            factor = multi_index_binom(mi_sum, mi1)

            if T == myInterval
                factor = myInterval(factor, factor)
                product = factor * v1 * v2
            else
                product = factor * v1 * v2
            end

            s[k_sum] = haskey(s, k_sum) ? (s[k_sum] + product) : product
        end
    end

    return s
end

function power_series(s1::Dict{String,T}, s2::Dict{String,T}, max_orders::Dict{String,Int}) where {T<:Union{Float64,myInterval}}
    if length(s2) != 1 || !haskey(s2, "")
        error("Power operation requires constant exponent")
    end

    n = s2[""]
    if n isa myInterval
        if n.lower != n.upper
            error("Power operation requires constant exponent")
        end
        n_val = Int(n.lower)
    elseif n isa Float64 && isinteger(n)
        n_val = Int(n)
    else 
        error("Power operation requires integer exponent")
    end

    if n_val == 0
        result = Dict{String, T}()
        if T == myInterval
            result[""] = myInterval(1.0, 1.0)
        else
            result[""] = 1.0
        end
        return result
    elseif n_val == 1
        # f^1 = f
        return copy(s1)
    elseif n_val < 0
        error("Negative exponents not supported")
    end

    result = copy(s1)  # Start with f^1
    
    for i in 2:n_val
        result = multiply_series(result, s1, max_orders)
    end
    
    return result
end




    


    


"""
     eval_slp(slp::SLP, vars::OrderedDict{Symbol, Float64})

     Top-level function that intializes values in the varsConst array and performs the evaluation and auto-differentiation simulataneously using the functions defined in this file 
"""
function eval_slp_old(slp::SLP, vars::OrderedDict{Symbol,Union{Float64,myInterval}}; max_orders::Dict{String,Int} = Dict{String,Int}()) 
    use_interval = any(v -> v isa myInterval, values(vars))
    # Initialize Variables in varsConst
    values_dict = Dict{Int, Union{Float64,myInterval}}()

    for (index, variable, _) in slp.varsConst
        if variable isa Symbol && haskey(vars, variable)
            values_dict[index] = vars[variable]
        elseif variable isa Float64
            if use_interval
                values_dict[index] = myInterval(variable, variable)
            else
                values_dict[index] = variable
            end
        else
            error("Variable $variable not defined in varsDict")
        end
    end

    if use_interval
        derivs = Vector{Dict{String,myInterval}}(undef, length(slp.codelist))
    else 
        derivs = Vector{Dict{String,Float64}}(undef, length(slp.codelist))
    end

    # Evaluate codelist properly with intervals
    for (i, (index, op, arg1, arg2, _)) in enumerate(slp.codelist)
        # Retrieve arguments
        s1 = arg1 > 0 ? derivs[arg1] : leaf_series(slp.varsConst[abs(arg1)][2], values_dict[arg1])
        s2 = arg2 > 0 ? derivs[arg2] : leaf_series(slp.varsConst[abs(arg2)][2], values_dict[arg2])

        # Perform interval operations correctly
        new_series = Dict{String,Union{Float64,myInterval}}()

        if op == :+
            new_series = add_series(s1, s2)
        elseif op == :-
            new_series = add_series(s1, neg_series(s2))
        elseif op == :*
            new_series = multiply_series(s1, s2, max_orders)
        elseif op == :^
            new_series = power_series(s1, s2, max_orders)
        else
            error("Unsupported operation $op")
        end

        value = new_series[""]
        slp.codelist[i] = (index, op, arg1, arg2, value)
        derivs[i] = new_series
    end
    return slp, derivs
 end

"""
     main()

     Main function that is responsible for reading the command line argument and connecting the workflow from parsing, convertion to SLP and then evaluation. 
     It is responsible for generating visually clear output and regenerating them to output.txt 
"""
function main()
     #Iterate through each file name in Argument
     for i in 1:length(ARGS)
         file = open(ARGS[i])
         output_file = open("output.txt", "w")
         writeTitle(ARGS[i], output_file)

         #Iterate through each line
         for line in eachline(file)
            #Skipping comment and empty lines
            if startswith(line, "#") || isempty(line)
                continue
            end
            
            poly, varsDict, maxOrders, description = parse_line(line)
            slp = parse_poly(String(poly))

            # Evaluate the SLP at the given variables 


            #For testing
            #slp, derivs = @btime eval_slp($slp, $varsDict)
            slp, derivs = isempty(maxOrders) ? eval_slp_old(slp, varsDict) : eval_slp_old(slp, varsDict; max_orders = maxOrders)

            # Print the results
            write(output_file, string("Polynomial: ", poly, "\n"))
            write(output_file, string("Variable assignments: ", varsDict, "\n"))
            if isempty(maxOrders)
                write(output_file, "Max orders: (all orders)\n")
            else
                # Format as “x=3, y=2, z=3” in sorted order
                sorted_vars = sort(collect(keys(maxOrders)))
                pairs = [ "$(v)=$(maxOrders[v])" for v in sorted_vars ]
                write(output_file, "Max orders: " * join(pairs, ", ") * "\n")
            end
            if description != ""
                write(output_file, string("Description ", description, "\n"))
            end
            write(output_file, string("Final Value: ", slp.codelist[end][5], "\n"))
            write(output_file, string("Computed Taylor Series at Final Node: \n"))
            sorted_keys = sort(collect(keys(derivs[end])), by = x -> (total_degree(x), x))
            
            for key in sorted_keys
                coeff = derivs[end][key]
                write(output_file, string("  Derivative ", key == "" ? "f" : key, " : ", coeff, "\n"))
            end
            
            write(output_file,"--------------------------------------------------\n")
          end
          close(output_file)
          close(file)
     end
end

if abspath(PROGRAM_FILE) == @__FILE__
    main()  # Only run main() if SLP.jl is being executed directly
end

