
include("myInterval.jl")

"""
    writeTitle(title, file)
     
    Function to write file name for each file read for visual clarity
"""
function writeTitle(title, file)
    n = length(title)+1
    write(file, string(repeat("-", n), "\n"))
    write(file, "$(title):\n")
    write(file, string(repeat("-", n), "\n"))
end


"""
    parse_line(line::String)
     
    Function to parse the input file line by line and extract the desired parts.
    This version supports variable substitutions that are either numbers (integers or floats)
    or intervals in the form "[a,b]".
"""
function parse_line(line::String)
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

    while i <= length(tokens) && !startswith(tokens[i], "[")
        if occursin("=", tokens[i])
            parts = split(tokens[i], "=")
            var = Symbol(strip(parts[1]))
            valstr = strip(parts[2])

            if startswith(valstr, "[") && endswith(valstr, "]")
                inner = valstr[2:end-1]
                splitVals = split(inner, ",")
                if length(splitVals) != 2
                    error("Interval for variable $(var) must have two endpoints")
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

    maxOrders = Dict{String,Int}()
    if i <= length(tokens) && startswith(tokens[i], "[") && occursin("=", tokens[i])
        der_str = strip(tokens[i], ['[', ']'])
        for pair in split(der_str, ",")
            kv = split(pair, "=")
            if length(kv) != 2
                error("Bad max_orders entry: $pair")
            end
            varname = strip(kv[1])
            deg = parse(Int, strip(kv[2]))
            maxOrders[varname] = deg
        end
        i += 1
    end

    description = ""
    if i <= length(tokens)
        description = join(tokens[i:end], ",")
        description = replace(description, "\"" => "")
    end

    return (poly, varsDict, maxOrders, description)
end


"""
    total_degree(key::String)
     
    Calculates the total degree of a derivative string for ordering of the output for visual clarity.
"""
function total_degree(key::String)
    # The empty string represents the function value, so degree is 0.
    if key == ""
        return 0
    else
        mi = parse_derivative(key)
        return sum(values(mi))
    end
end
