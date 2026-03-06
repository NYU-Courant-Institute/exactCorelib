include("ASTTypes.jl")
include("myInterval.jl")
zero_interval = myInterval(0.0,0.0)


"""
    tokenize(input::String)

Parses the input String into tokens for AST Tree.
"""
function tokenize(input::String)
    tokens = String[]
    i = 1
    len = length(input)

    while i <= len
        c = input[i]
        
        #Ignore white spaces
        if c in (' ', '\t', '\n')
            i += 1
            continue
        #Single character tokens
        elseif c in ('+', '-', '(', ')', '^', '*')
            push!(tokens, string(c))
            i += 1
        #Parse integer or float constants
        elseif isdigit(c) || c == '.'
            start = i
            zero_prefix = (c == '.')
            has_decimal = zero_prefix
            i += 1
            if zero_prefix && (i > len || !isdigit(input[i]))
                error("Non-digit following decimal point")
            end

            while i <= len 
                c2 = input[i]
                if isdigit(c2)
                    i += 1
                elseif c2 == '.' && !has_decimal
                    has_decimal = true
                    i += 1

                    if i <= len && (!isdigit(input[i]) && !(input[i] in (' ', '\t', '\n'))) 
                        error("Non-digit following decimal point")
                    end
                elseif c2 == '.' && has_decimal
                    error("Only one decimal point allowed")
                else
                    break
                end
            end
            
            num = zero_prefix ? ("0" * input[start:i-1]) : input[start:i-1]
            num = endswith(num, ".") ? num[1:end-1] : num
            push!(tokens, num)

        #Parse Variable Name
         elseif isletter(c)
            token = string(c)
            i += 1
            while i <= len && isdigit(input[i])
                token *= string(input[i])
                i += 1
            end
            push!(tokens, token)
        else
            error("Unexpected character: $c")
        end 
    end 

    return tokens
end

"""
    Struct that records the state of the parser when parsing to AST

    tokens: List of tokens
    pos: current position(token index) the parser is working on
"""
mutable struct ParserState
    tokens::Vector{String}
    pos::Int
end

"""
    current_token(state::ParserState)

    Retrieves the current_token that the parser is parsing
"""
function current_token(state::ParserState)
    if state.pos > length(state.tokens)
        return nothing
    end
    return state.tokens[state.pos]
end

"""
    consume_token!(state::ParserState)

    Moves the parser to the next token
"""
function consume_token!(state::ParserState)
    state.pos += 1
end


"""
    parse_expr(state::ParserState)::Expr

    The top level parsing function that parses an expr and separates expr into terms separated by + or - signs
"""
function parse_expr(state::ParserState)::Expr
    node  = parse_term(state)

    while true
        tok = current_token(state)
        if tok == "+"
            consume_token!(state)
            right = parse_term(state)
            node = AddNode(node, right)
        elseif tok == "-"
            consume_token!(state)
            right = parse_term(state)
            node = MinusNode(node, right)
        else 
            return node
        end
    end
end

"""
    parse_term(state::ParserState)::Expr

    The second level of parsing that takes a term and separates it into multiplication factors
"""
function parse_term(state::ParserState)::Expr
    node = parse_factor(state)

    while true 
        tok = current_token(state)
        if tok == "*"
            consume_token!(state)
            right = parse_factor(state)
            node = MulNode(node, right)
        elseif tok !== nothing
            if tok == "(" || isletter(tok[1]) || isdigit(tok[1])
                right = parse_factor(state)
                node = MulNode(node, right)
            else
                return node
            end
        else
            return node
        end
    end
end

"""
    parse_factor(state::ParserState)::Expr

    The third level of parsing that takes a multiplication factor and retrieves its power
"""
function parse_factor(state::ParserState)::Expr
    node = parse_base(state)

    while true
        tok = current_token(state)
        if tok == "^"
            consume_token!(state)
            exponent_expr = parse_factor(state)
            if exponent_expr isa FloatNode
                node = PowNode(node, exponent_expr.value) 
            else
                error("Exponent must be a numeric literal, got $exponent_expr")
            end
        else 
            return node
        end
    end
end


"""
    parse_base(state::ParserState)::Expr

    The base level of parsing that takes a token and checks for its basic type(Variable, Float, or parenthesis) and converts them to ASTNode accordingly
"""
function parse_base(state::ParserState)::Expr
    tok = current_token(state)
    if tok === nothing
        error("Unexpected end of input in parse_base")
    end


    if tok == "-"
        consume_token!(state)
        right = parse_base(state)
        return MinusNode(FloatNode(0.0), right)
    end
    
    # variable
    if isletter(tok[1])
        consume_token!(state)
        return VarNode(Symbol(tok))
    # float64
    elseif isdigit(tok[1])
        consume_token!(state)
        return FloatNode(parse(Float64, tok))
    elseif tok == "("
        consume_token!(state)
        node = parse_expr(state)
        if current_token(state) != ")"
            error("Missing closing parenthesis")
        end
        consume_token!(state)
        return node
    else 
        error("Unexpected token in parse_base: $tok")
    end    
end

"""
    ast_to_slp(root::Expr)::SLP

    The function that takes an ast and converts it to a SLP. Mainly achieved using tree traversal technique. 
"""
function ast_to_slp(root::Expr)::SLP
    var_const_dict= Dict{Any,Int}()

    varsConst = Vector{Tuple{Int, Any, myInterval}}()
    codeList = Vector{Tuple{Int, Symbol, Int, Int, myInterval}}()

    nextConstIndex = Ref(-1)  
    nextTempIndex  = Ref(1)  

    """
    get_index_for_val(val::Union{Symbol, Float64})::Int

    Helper function that retrieves the index of a given input variable or constant in the varsConst array or appends it to the array if not already present
    """
    function get_index_for_val(val::Union{Symbol, Float64})::Int
        if haskey(var_const_dict, val)
            return var_const_dict[val]
        else
            idx = nextConstIndex[]
            if val isa Symbol
                push!(varsConst, (idx, val, zero_interval))
            else
                push!(varsConst, (idx, val, myInterval(val, val)))
            end
            var_const_dict[val] = idx
            nextConstIndex[] -= 1
            return idx
        end
    end

    """
    build(node::Expr)::Int

    Helper function that essentially builds the the codelist array by recursive traversal of the AST Tree
    """
    function build(node::Expr)::Int
        if node isa VarNode
            return get_index_for_val(node.name)

        elseif node isa FloatNode
            return get_index_for_val(node.value)

        elseif node isa AddNode
            left_idx  = build(node.left)
            right_idx = build(node.right)
            out_idx  = nextTempIndex[]
            nextTempIndex[] += 1
            push!(codeList, (out_idx, :+, left_idx, right_idx, zero_interval))
            return out_idx

        elseif node isa MinusNode
            left_idx  = build(node.left)
            right_idx = build(node.right)
            out_idx   = nextTempIndex[]
            nextTempIndex[] += 1
            push!(codeList, (out_idx, :-, left_idx, right_idx, zero_interval))
            return out_idx

        elseif node isa MulNode
            left_idx  = build(node.left)
            right_idx = build(node.right)
            out_idx   = nextTempIndex[]
            nextTempIndex[] += 1
            push!(codeList, (out_idx, :*, left_idx, right_idx, zero_interval))
            return out_idx

        elseif node isa PowNode
            n = node.exponent
            if isinteger(n)
                n = Int(n)
                if n == 0
                    return get_index_for_val(1.0)
                elseif n == 1
                    return build(node.base)
                else
                    idx = build(node.base)
                    exp_idx = get_index_for_val(Float64(n))
                    out_idx = nextTempIndex[]
                    nextTempIndex[] += 1
                    push!(codeList, (out_idx, :^, idx, exp_idx, zero_interval))
                    return out_idx
                end
            else
                error("Exponent is not an integer constant: $n")
            end
        else
            error("Unsupported AST node type: $(typeof(node))")
        end
    end

    build(root)

    return SLP(varsConst, codeList)
end

 """
    parse_poly(poly::String)::SLP

    Top-level function that combines functions defined in this file and converts an input polynomial in string form to SLP form
"""
function parse_poly(poly::String)::SLP
    tokenized_poly = tokenize(poly)
    state = ParserState(tokenized_poly, 1)
    poly_ast = parse_expr(state)
    slp = ast_to_slp(poly_ast)
    if isempty(slp.codelist) && !isempty(slp.varsConst)
        idx = slp.varsConst[1][1]
        const_idx = -(length(slp.varsConst) + 1)
        push!(slp.varsConst, (const_idx, 0.0, zero_interval))
        push!(slp.codelist, (1, :+, const_idx, idx, slp.varsConst[1][3]))
    end
    return slp
end

