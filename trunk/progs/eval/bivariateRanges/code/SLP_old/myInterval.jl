# interval.jl
struct myInterval
    lower::Float64
    upper::Float64
end

# myInterval addition
Base.:+(a::myInterval, b::myInterval) = myInterval(a.lower + b.lower, a.upper + b.upper)
Base.:+(a::Real, b::myInterval) = myInterval(a + b.lower, a + b.upper)
Base.:+(a::myInterval, b::Real) = myInterval(a.lower + b, a.upper + b)

# myInterval subtraction
Base.:-(a::myInterval, b::myInterval) = myInterval(a.lower - b.upper, a.upper - b.lower)
Base.:-(a::Real, b::myInterval) = myInterval(a - b.lower, a - b.upper)
Base.:-(a::myInterval, b::Real) = myInterval(a.lower - b, a.upper - b)


# myInterval multiplication
Base.:*(a::myInterval, b::myInterval) = myInterval(
    minimum([a.lower * b.lower, a.lower * b.upper, a.upper * b.lower, a.upper * b.upper]),
    maximum([a.lower * b.lower, a.lower * b.upper, a.upper * b.lower, a.upper * b.upper])
)
Base.:*(a::Real, I::myInterval) = myInterval(a,a) * I
Base.:*(I::myInterval, a::Real) = myInterval(a,a) * I



# myInterval exponentiation (integer power)
function Base.:^(a::myInterval, n::Int)
    if n == 0
        return myInterval(1.0, 1.0)
    elseif n > 0
        result = a
        for _ in 2:n
            result *= a
        end
        return result
    else
        error("Negative interval exponentiation not supported")
    end
end
