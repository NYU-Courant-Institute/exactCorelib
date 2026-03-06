include("utils.jl")
include("parser.jl")
include("myInterval.jl")
using BenchmarkTools
using DataStructures
using Combinatorics
using Printf

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




function prune_varsConst(slp::SLP)
    used = Set{Int}()
    for (_, _, a1, a2, _) in slp.codelist
        push!(used, a1)
        push!(used, a2)
    end

    filtered_varsConst = filter(vc -> vc[1] in used, slp.varsConst)
    
    old_to_new = Dict{Int,Int}()
    new_varsConst = Vector{Tuple{Int, Any, Union{Float64,myInterval}}}()
    
    for (i, (old_idx, var, val)) in enumerate(filtered_varsConst)
        new_idx = -i
        old_to_new[old_idx] = new_idx
        push!(new_varsConst, (new_idx, var, val))
    end
    new_codelist = Vector{Tuple{Int, Symbol, Int, Int, Union{Float64,myInterval}}}()
    for (idx, op, a1, a2, val) in slp.codelist
        new_a1 = a1 < 0 ? get(old_to_new, a1, a1) : a1
        new_a2 = a2 < 0 ? get(old_to_new, a2, a2) : a2
        push!(new_codelist, (idx, op, new_a1, new_a2, val))
    end

    return SLP(new_varsConst, new_codelist)
end


function build_partial_derivative_slp(slp::SLP, var::Symbol)
    new_varsConst = deepcopy(slp.varsConst)
    copied_codelist = deepcopy(slp.codelist) 
    
    deriv_codelist = Vector{Tuple{Int, Symbol, Int, Int, Union{Float64,myInterval}}}()
    deriv_map = Dict{Int,Int}()
    next_reg = maximum(first.(slp.codelist)) + 1

    function lookup_const(idx)
        return findfirst(vc->vc[1]==idx, new_varsConst)
    end

    function fresh_reg()
        r = next_reg
        next_reg += 1
        return r
    end

    function get_const_reg(val::Float64)
        for (idx,sym,raw) in new_varsConst
          if !(sym isa Symbol) && raw == myInterval(val, val)
            return idx
          end
        end
        new_idx = -(length(new_varsConst) + 1)
        push!(new_varsConst, (new_idx, val, myInterval(val, val)))
        return new_idx
    end

    function is_constant(idx)
        if idx < 0
            i = lookup_const(idx)
            return i !== nothing && !(new_varsConst[i][2] isa Symbol)
        end
        return false
    end

    function get_constant_value(idx)
        if idx < 0
            i = lookup_const(idx)
            if i !== nothing && !(new_varsConst[i][2] isa Symbol)
                return new_varsConst[i][3]
            end
        end
        return nothing
    end

    zero_idx = get_const_reg(0.0)
    one_idx = get_const_reg(1.0)

    for (leafidx, sym, _) in slp.varsConst
        if sym === var
            deriv_map[leafidx] = one_idx
        else
            deriv_map[leafidx] = zero_idx
        end
    end

    for (idx, op, a1, a2, _) in slp.codelist
        d1 = deriv_map[a1]
        d2 = deriv_map[a2]

        if op == :+
            if is_constant(d1) && is_constant(d2)
                val1 = get_constant_value(d1)
                val2 = get_constant_value(d2)
                result_val = val1 + val2
                deriv_map[idx] = get_const_reg(result_val.lower)
            elseif d1 == zero_idx
                deriv_map[idx] = d2
            elseif d2 == zero_idx
                deriv_map[idx] = d1
            else
                new_idx = fresh_reg()
                push!(deriv_codelist, (new_idx, :+, d1, d2, zero_interval))
                deriv_map[idx] = new_idx
            end
        elseif op == :-
            if is_constant(d1) && is_constant(d2)
                val1 = get_constant_value(d1)
                val2 = get_constant_value(d2)
                result_val = val1 - val2
                deriv_map[idx] = get_const_reg(result_val.lower)
            elseif d2 == zero_idx
                deriv_map[idx] = d1
            else
                new_idx = fresh_reg()
                push!(deriv_codelist, (new_idx, :-, d1, d2, zero_interval))
                deriv_map[idx] = new_idx
            end
        elseif op == :*
            has_f = (d1 != zero_idx)
            has_g = (d2 != zero_idx)

            if !has_f && !has_g
                deriv_map[idx] = zero_idx
            elseif has_f && !has_g
                if d1 == one_idx
                    deriv_map[idx] = a2
                else
                    new_idx = fresh_reg()
                    push!(deriv_codelist, (new_idx, :*, d1, a2, zero_interval))
                    deriv_map[idx] = new_idx
                end
            elseif !has_f && has_g
                if d2 == one_idx
                    deriv_map[idx] = a1
                else
                    new_idx = fresh_reg()
                    push!(deriv_codelist, (new_idx, :*, a1, d2, zero_interval))
                    deriv_map[idx] = new_idx
                end
            else
                t1_idx = if d1 == one_idx
                    a2 
                else
                    new_idx = fresh_reg()
                    push!(deriv_codelist, (new_idx, :*, d1, a2, zero_interval))
                    new_idx
                end
                
                t2_idx = if d2 == one_idx
                    a1
                else
                    new_idx = fresh_reg()
                    push!(deriv_codelist, (new_idx, :*, a1, d2, zero_interval))
                    new_idx
                end

                final_idx = fresh_reg()
                push!(deriv_codelist, (final_idx, :+, t1_idx, t2_idx, zero_interval))
                deriv_map[idx] = final_idx
            end
        elseif op == :^
            if is_constant(a2)
                exponent_val = get_constant_value(a2)
                if exponent_val isa myInterval
                    n = Int(exponent_val.lower)
                else
                    n = Int(exponent_val)
                end
                
                if n == 0
                    deriv_map[idx] = zero_idx
                elseif n == 1
                    deriv_map[idx] = d1
                elseif d1 == zero_idx
                    deriv_map[idx] = zero_idx
                else
                    n_idx = get_const_reg(Float64(n))
                    
                    if n == 2
                        coeff_idx = fresh_reg()
                        push!(deriv_codelist, (coeff_idx, :*, n_idx, a1, zero_interval))
                    else
                        n_minus_1_idx = get_const_reg(Float64(n-1))
                        f_power_idx = fresh_reg()
                        push!(deriv_codelist, (f_power_idx, :^, a1, n_minus_1_idx, zero_interval))
                        
                        coeff_idx = fresh_reg()
                        push!(deriv_codelist, (coeff_idx, :*, n_idx, f_power_idx, zero_interval))
                    end
                    
                    if d1 == one_idx
                        deriv_map[idx] = coeff_idx
                    else
                        final_idx = fresh_reg()
                        push!(deriv_codelist, (final_idx, :*, coeff_idx, d1, zero_interval))
                        deriv_map[idx] = final_idx
                    end
                end
            else
                error("Power operation with non-constant exponent not supported")
            end
        else
            error("Unsupported op $op")
        end
    end

    combined_codelist = vcat(copied_codelist, deriv_codelist)
    
    if isempty(deriv_codelist)
        final_result_idx = maximum(keys(deriv_map))
        final_result = deriv_map[final_result_idx]
        
        if final_result != zero_idx
            dummy_instruction = (1, :+, final_result, zero_idx, zero_interval)
            new_codelist = [dummy_instruction]
            return prune_varsConst(SLP(new_varsConst, new_codelist))
        else
            return SLP(Vector{Tuple{Int, Any, Union{Float64,myInterval}}}(), Vector{Tuple{Int, Symbol, Int, Int, Union{Float64,myInterval}}}())
        end
    end
    
    final_deriv_idx = maximum(keys(deriv_map))
    final_deriv_result = deriv_map[final_deriv_idx]
    
    needed_indices = Set{Int}()
    to_process = [final_deriv_result]
    
    while !isempty(to_process)
        current = pop!(to_process)
        if current in needed_indices || current < 0 
            continue
        end
        push!(needed_indices, current)
    
        for (idx, op, a1, a2, _) in combined_codelist
            if idx == current
                if a1 > 0 && !(a1 in needed_indices)
                    push!(to_process, a1)
                end
                if a2 > 0 && !(a2 in needed_indices)
                    push!(to_process, a2)
                end
                break
            end
        end
    end
    
    filtered_codelist = filter(inst -> first(inst) in needed_indices, combined_codelist)
    
    remap = Dict{Int, Int}()
    renumbered = Vector{Tuple{Int,Symbol,Int,Int,Union{Float64,myInterval}}}()
    
    for (i, (idx, op, a1, a2, val)) in enumerate(filtered_codelist)
        remap[idx] = i
        push!(renumbered, (i, op, a1, a2, val))
    end
    
    for i in 1:length(renumbered)
        (idx, op, a1, a2, val) = renumbered[i]
        new_a1 = a1 > 0 ? get(remap, a1, a1) : a1
        new_a2 = a2 > 0 ? get(remap, a2, a2) : a2
        renumbered[i] = (idx, op, new_a1, new_a2, val)
    end

    return prune_varsConst(SLP(new_varsConst, renumbered))
end

function build_derivative_slp(slp::SLP, alpha::Dict{String, Int}, deriv_dict::Dict{String, SLP} = Dict{String, SLP}())
    alpha_str = multi_index_to_string(alpha)

    if haskey(deriv_dict, alpha_str)
        return deriv_dict[alpha_str], deriv_dict
    end

    if all(v == 0 for v in values(alpha))
        deriv_dict[alpha_str] = slp
        return slp, deriv_dict
    end

    var_to_diff = nothing
    for (var, degree) in alpha
        if degree > 0
            var_to_diff = var
            break
        end
    end

    if var_to_diff === nothing
        deriv_dict[alpha_str] = slp
        return slp, deriv_dict
    end

    reduced_alpha = copy(alpha)
    reduced_alpha[var_to_diff] -= 1

    reduced_alpha = Dict(k => v for (k, v) in reduced_alpha if v > 0)

    reduced_slp, _ = build_derivative_slp(slp, reduced_alpha, deriv_dict)

    if isempty(reduced_slp.varsConst) && isempty(reduced_slp.codelist)
        zero_slp = SLP(Vector{Tuple{Int, Any, Union{Float64,myInterval}}}(), 
                      Vector{Tuple{Int, Symbol, Int, Int, Union{Float64,myInterval}}}())
        deriv_dict[alpha_str] = zero_slp
        return zero_slp, deriv_dict
    end

    partial_slp = build_partial_derivative_slp(reduced_slp, Symbol(var_to_diff))

    deriv_dict[alpha_str] = partial_slp
    return partial_slp, deriv_dict
end

function eval_slp(slp::SLP, vars::OrderedDict{Symbol,T}) where T<:Union{Float64,myInterval}
    use_interval = any(v -> v isa myInterval, values(vars))
    
    # Update varsConst values directly
    for i in 1:length(slp.varsConst)
        index, variable, _ = slp.varsConst[i]
        if variable isa Symbol && haskey(vars, variable)
            computed_value = vars[variable]
            slp.varsConst[i] = (index, variable, computed_value)
        elseif variable isa Float64
            if use_interval
                computed_value = myInterval(variable, variable)
            else
                computed_value = variable
            end
            slp.varsConst[i] = (index, variable, computed_value)
        else
            error("Variable $variable not defined in varsDict")
        end
    end
    
    function get_value(idx)
        if idx < 0
            for (index, _, value) in slp.varsConst
                if index == idx
                    return value
                end
            end
        else
            for (index, _, _, _, value) in slp.codelist
                if index == idx
                    return value
                end
            end
        end
        error("Index $idx not found")
    end

    for i in 1:length(slp.codelist)
        index, op, arg1, arg2, _ = slp.codelist[i]
        val1 = get_value(arg1)
        val2 = get_value(arg2)
        
        computed_value = if op == :+
            val1 + val2
        elseif op == :-
            val1 - val2
        elseif op == :*
            val1 * val2
        elseif op == :^
            if val2 isa myInterval
                if val2.lower != val2.upper
                    error("Power operation requires constant exponent")
                end
                exponent = Int(val2.lower)
            elseif val2 isa Float64 && isinteger(val2)
                exponent = Int(val2)
            else
                error("Power operation requires integer exponent")
            end
            val1 ^ exponent
        else
            error("Unsupported operation: $op")
        end
        
        slp.codelist[i] = (index, op, arg1, arg2, computed_value)
    end
    
    if isempty(slp.codelist)
        if use_interval
            return zero_interval
        else 
            return 0.0
        end
    else
        return slp.codelist[end][5]
    end
end





"""
     main()

     Main function that is responsible for reading the command line argument and connecting the workflow from parsing, convertion to SLP and then evaluation. 
     It is responsible for generating visually clear output and regenerating them to output.txt 
"""
function main()
    # slp = parse_poly("-12x^4y^4 + 78x^2y^6 + 5y^8 + 65x^2y^3 + 81x^3y -91xy^2")

    # deriv, deriv_list = build_derivative_slp(slp, Dict("x"=>4, "y"=>3))


    # println("SLP:")
    # println(slp)
    # println("Derivative SLP:")
    # println(deriv)
    # println("Derivative List:")
    # println(deriv_list)

    
    # slp = SLP(Tuple{Int64, Any, Union{Float64, myInterval}}[(-1, 0.0, 0.0), (-2, 1.0, 1.0)], Tuple{Int64, Symbol, Int64, Int64, Union{Float64, myInterval}}[(1, :+, -2, -1, 1.0)])
    # # println(eval_slp(deriv, OrderedDict(:x => myInterval(0.3, 0.309), :y => myInterval(2.1, 2.109))))
    # println(eval_slp(slp, OrderedDict(:x => myInterval(0.3, 0.309), :y => myInterval(2.1, 2.109))))

    # Iterate through each input filename
    for fname in ARGS
        open(fname) do file
            open("output.txt", "w") do output_file
                writeTitle(fname, output_file)

                for line in eachline(file)
                    println("Processing line: $line")
                    # skip empty/comment lines
                    if isempty(line) || startswith(line, "#")
                        continue
                    end

                    # parse the line
                    poly, varsDict, maxOrders, description = parse_line(line)
                    slp = parse_poly(String(poly))
                    
                    deriv_dict::Dict{String, SLP} = Dict{String, SLP}()

                    if isempty(maxOrders)
                        error("Please specify derivative orders.") 
                    else 
                        build_derivative_slp(slp, maxOrders, deriv_dict)
                    end 

                    # header
                    write(output_file, "Polynomial: $poly\n")
                    write(output_file, "Variables: $varsDict\n")
                    pairs = [ "$(v)=$(maxOrders[v])" for v in sort(collect(keys(maxOrders))) ]
                    write(output_file, "Max orders: " * join(pairs, ", ") * "\n")

                    if !isempty(description)
                        write(output_file, "Description: $description\n")
                    end

                    # now visualize each derivative-SLP
                    write(output_file, "\nGenerated derivatives:\n\n")
                    for (key, dslp) in sort(collect(deriv_dict); by=first)
                        # bench = @benchmark eval_slp($dslp, $varsDict)
                        # display(bench)  
                        val = eval_slp(dslp, varsDict)
                        let deriv_name = (key == "" ? "f" : key)
                            write(output_file, "$deriv_name = $val\n")
                        end
                        write(output_file, "\n")
                    end

                    write(output_file, "────────────────────────────────────────\n\n")
                end
            end
        end
    end
end


if abspath(PROGRAM_FILE) == @__FILE__
    main()  # Only run main() if SLP.jl is being executed directly
end

