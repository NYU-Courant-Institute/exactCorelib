#= 
    @author: Thomas Chen
    @advisors: Yap Chee, Kai Hormann, Bingwei Zhang
    @date: 2025-06-18
    @description: main function 
        This is the main entry point to the SLP programs.
        The main function is
        - main()
            command line arguments (i.e., ARGS) are
            - p: input file[s] (default is "input.txt")
            - m: mode is either 0 or 3 (default is 0)
            - d: digit precision (default is 5)
            - c: Result display mode. 0 for interval notation, 1 for centered notation(center +/- radius) (default is 1)
            The output is written into the file "output.txt"
            - the output includes the polynomial description
            - assignment of values to variable
            - results of evaluating the function and its derivatives
=# 
using BenchmarkTools


include("SLP.jl")
include("myInterval.jl")
include("polynomial.jl")
include("derivatives.jl")
include("eval.jl")
include("utils.jl")

# BenchmarkTools.DEFAULT_PARAMETERS.seconds = 500
# BenchmarkTools.DEFAULT_PARAMETERS.samples = 1

INSTRUCTION_HASH = Dict{Tuple{Symbol, Union{Int, String}, Union{Int, String}}, Int}()

#= 
Main function to read through input files and write to output file
=#
function main()

    if length(ARGS) < 1
        error("Missing arguments. Please provide input file, mode and digit precision")
    end

    c = parse(Int, ARGS[end])

    digits = parse(Int, ARGS[end-1])

    mode = parse(Int, ARGS[end-2])

    if mode != 0 && mode != 3
        error("Invalid mode. Please use 0 or 3.")
    end

    if c == 1
        centered = true
    elseif c == 0
        centered = false
    else
        error("Invalid centered boolean inputted. Please use 0 or 1")
    end

    file_args = ARGS[1:end-3]

    # Iterate through each input filename
    for fname in file_args
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
                    poly, assignments, description = parse_line(line)
                    
                    polynomial = Polynomial(poly)

                    
                    benchmark_result = @benchmark compute_all_derivatives!($polynomial)
                    display(benchmark_result)
                    if mode == 0
                        compute_all_derivatives!(polynomial)
                    elseif mode == 3
                        compute_third_derivatives_2D!(polynomial)
                    end

                    println(polynomial.slp)

                    println(length(polynomial.slp.codelist))

                    println(polynomial.slp.slp_ranges[""])

                    # header
                    write(output_file, "Polynomial: $poly\n")
                    write(output_file, "Variables: $assignments\n")
                    if description != ""
                        write(output_file, "Description: $description\n")
                    end
                    write(output_file, "────────────────────────────────────────\n\n")

                    # benchmark_res = @benchmark evaluate_slp_range($polynomial.slp, "x^2y", $assignments)
                    # display(benchmark_res)

                    for (key, _) in sort(collect(polynomial.slp.slp_ranges); by=first)
                        let deriv_name = (key == "" ? "f" : key)
                            result = evaluate_slp_range(polynomial.slp, key, assignments)
                            formatted_result = format_result(result, digits, 6, centered)
                            write(output_file, "$deriv_name = $formatted_result\n")
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
    main()  
end



