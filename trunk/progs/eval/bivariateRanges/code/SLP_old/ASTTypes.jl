

abstract type Expr end

"""
    VarNode(name)

Represents a variable (like `x`, `y`, etc.).
"""
struct VarNode <: Expr
    name::Symbol
end

"""
    FloatNode(value)

Represents an float Constant
"""
struct FloatNode <: Expr
    value::Float64
end


"""

    AddNode(left, right)

Represents addition: `left + right`.
"""
struct AddNode <: Expr
    left::Expr
    right::Expr
end

"""
    MinusNode(left, right)

Represents subtraction: `left - right`.
"""
struct MinusNode <: Expr
    left::Expr
    right::Expr
end

"""
    MulNode(left, right)

Represents multiplication: `left * right`.
"""
struct MulNode <: Expr
    left::Expr
    right::Expr
end

"""
    PowNode(base, exponent)

Represents exponentiation: `base^exponent`.
"""
struct PowNode <: Expr
    base::Expr
    exponent::Float64
end



