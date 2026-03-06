#=
    @author: Thomas Chen
    @advisors: Yap Chee, Kai Hormann, Bingwei Zhang
    @date: 2025-06-30
    @description: Main file for the iso_curve_Taylor program.
    
    This is the main entry point to the SLP programs.
        The main function is
        - main()
            command line arguments (i.e., ARGS) are
            - p: input file[s] (default is "input.txt")
            - DEBUG_LEVEL: debug level (default is 4)
            - benchmark flag (default is false)
            The output is written into the folder outputs and images
            - If benchmark flag is true, then the output is written into the folder outputs
                - benchmark_results_$now_str.txt: benchmark results and detailed information about computation
            - If DEBUG_LEVEL > 4 and < 7, then a graph will be generated for each polynomial in images
                - iso_curve_$now_str.png: graph of the polynomial
=# 

using BenchmarkTools
using Dates 
using DataStructures
using GLMakie
using Printf

include("../SLP/SLP.jl")
include("../SLP/parser.jl")
include("../SLP/polynomial.jl")
include("../SLP/derivatives.jl")
include("../SLP/main.jl")
include("../SLP/myInterval.jl")
include("myBox.jl")
include("subdiv.jl")
include("utils.jl")
include("range_funcs.jl")

tol = 1e-14

derivatives_taylor  = Dict{Tuple{Float64, Float64}, Vector{Vector{Float64}}}()

total_boxes = 0

DEBUG_LEVEL = 4

factorial_cache = Vector{Int}()

total_eval = 0

total_points = 0

C0_boxes = 0

max_x = 0

max_y = 0

total_degree = 0

BenchmarkTools.DEFAULT_PARAMETERS.seconds = 50
BenchmarkTools.DEFAULT_PARAMETERS.samples = 10

function main()
    global DEBUG_LEVEL, max_x, max_y, total_degree

    benchmark_mode = "--benchmark" in ARGS
    
    file_args = filter(arg -> arg != "--benchmark", ARGS)
    
    now_str = Dates.format(now(), "yyyy-mm-dd_HHMMSS")

    if length(file_args) < 2
        error("Invalid number of arguments. Please provide at least 2 arguments.")
    end
    
    try
        DEBUG_LEVEL = parse(Int, file_args[end])
        DEBUG_LEVEL = clamp(DEBUG_LEVEL, -2, 7)
    catch
        DEBUG_LEVEL = 4
        println("Invalid debug level, using default: $DEBUG_LEVEL")
    end

    file_args = file_args[1:end-1]

    if benchmark_mode
        mkpath("outputs")
        benchmark_filename = joinpath("outputs", "benchmark_results_$now_str.txt")
        benchmark_file = open(benchmark_filename, "w")
        
        println(benchmark_file, "Benchmark Results - $(now_str)")
        println(benchmark_file, "="^60)
    end

    if DEBUG_LEVEL == -1
        table_rows = NamedTuple{(:curve, :min_ms, :median_ms, :mean_ms, :memory_mb, :efficacy), Tuple{String, Float64, Float64, Float64, Float64, Float64}}[]
    end

    for i in 1:length(file_args)
        file = open(file_args[i])
        print("Processing file: $file\n")
        

        #Iterate through each line
        for line in eachline(file)

            reset_global_state!()
            now_str = Dates.format(now(), "yyyy-mm-dd_HHMMSS")

            #Skipping comment and empty lines
            if startswith(line, "#") || isempty(line)
                continue
            end
            
            poly, varsDict, description = parse_line(line)
            # Parse the polynomial to create an SLP

            polynomial = Polynomial(poly)
            println("Processing Polynomial: $poly")
            
            max_x, max_y = get_max_order(polynomial, :x), get_max_order(polynomial, :y)

            total_degree = get_total_degree(polynomial) * 2

            compute_all_derivatives!(polynomial)

            # Create initial box
            x_interval = varsDict[:x]
            y_interval = varsDict[:y]
            initial = myBox(x_interval, y_interval)

            boxes = collect(keys(uniform_split(initial, 1024)))

            if benchmark_mode && DEBUG_LEVEL >= 0
                println(benchmark_file, "\nPolynomial: $poly")
                println(benchmark_file, "Description: $description")  
                println(benchmark_file, "Debug Level: $DEBUG_LEVEL")
                println(benchmark_file, "Total Degree: $total_degree")
                println(benchmark_file, "Domain: x ∈ $x_interval, y ∈ $y_interval")
                println(benchmark_file, "-"^40)

                benchmark_result = @benchmark isotopic_curve($polynomial, $initial) teardown=(reset_global_state!())
                #benchmark_result = @benchmark isotopic_curve($polynomial, $boxes) teardown=(reset_global_state!())
                display(benchmark_result)

                println(benchmark_file, "Minimum time: $(minimum(benchmark_result.times) / 1e6) ms")
                println(benchmark_file, "Median time: $(median(benchmark_result.times) / 1e6) ms")
                println(benchmark_file, "Mean time: $(mean(benchmark_result.times) / 1e6) ms")
                println(benchmark_file, "Maximum time: $(maximum(benchmark_result.times) / 1e6) ms")
                println(benchmark_file, "Memory allocated: $(benchmark_result.memory) bytes")
                println(benchmark_file, "Allocations: $(benchmark_result.allocs)")
                println(benchmark_file, "Samples: $(length(benchmark_result.times))")
                reset_global_state!()
            end


            if DEBUG_LEVEL == -1
                # initial_copy = myBox(x_interval, y_interval)
                box_count = 1024

                q = uniform_split(initial, box_count)

                total_width = evaluate_boxes(q, polynomial)

                efficacy = total_width / box_count

                reset_global_state!()

                benchmark_result = @benchmark evaluate_boxes($q, $polynomial) teardown=(reset_global_state!())

                min_ms    = minimum(benchmark_result.times) / 1e6
                median_ms = median(benchmark_result.times) / 1e6
                mean_ms   = mean(benchmark_result.times) / 1e6
                memory_mb = benchmark_result.memory / 1024^2

                println(description)

                push!(table_rows, (; curve=description, min_ms, median_ms, mean_ms, memory_mb, efficacy))

                display(benchmark_result)

                println("Total width: $total_width")
                println("Efficacy: $efficacy")

                # q2 = uniform_split(initial_copy, 65536)
                # benchmark_result2 = @benchmark evaluate_boxes($q2, $polynomial) teardown=(reset_global_state!())
                # display(benchmark_result2)

                if benchmark_mode
                    println(benchmark_file, "\nPolynomial: $poly")
                    println(benchmark_file, "Description: $description")
                    println(benchmark_file, "Debug Level: $DEBUG_LEVEL")
                    println(benchmark_file, "Total Degree: $total_degree")
                    println(benchmark_file, "Domain: x ∈ $x_interval, y ∈ $y_interval")
                    println(benchmark_file, "-"^40)

                    println(benchmark_file, "Division boxes: $(length(q))")
                    println(benchmark_file, "Minimum time: $min_ms ms")
                    println(benchmark_file, "Median time: $median_ms ms")
                    println(benchmark_file, "Mean time: $mean_ms ms")
                    println(benchmark_file, "Memory allocated: $memory_mb MB")
                    println(benchmark_file, "Allocations: $(benchmark_result.allocs)")
                    println(benchmark_file, "Samples: $(length(benchmark_result.times))")
                    println()

                    # println(benchmark_file, "Division boxes: $(length(q2))")
                    # println(benchmark_file, "Minimum time: $(minimum(benchmark_result2.times) / 1e6) ms")
                    # println(benchmark_file, "Median time: $(median(benchmark_result2.times) / 1e6) ms")
                    # println(benchmark_file, "Mean time: $(mean(benchmark_result2.times) / 1e6) ms")
                    # println(benchmark_file, "Maximum time: $(maximum(benchmark_result2.times) / 1e6) ms")
                    # println(benchmark_file, "Memory allocated: $(benchmark_result2.memory) bytes")
                    # println(benchmark_file, "Allocations: $(benchmark_result2.allocs)")
                    # println(benchmark_file, "Samples: $(length(benchmark_result2.times))")
                end
                reset_global_state!()
            end

            # if DEBUG_LEVEL == -2: 
            #     println(taylor_interpolation(polynomial, initial, ))
            # end
            if DEBUG_LEVEL == -2
                test_box = myBox(myInterval(-10^-5, 10^-5),myInterval(-10^-5, 10^-5))
                println(taylor_interpolation(polynomial, test_box, total_degree))
            end

            if DEBUG_LEVEL >= 0

                pq, smooth_boxes, eps_boxes, Q_C0 = isotopic_curve(polynomial, initial) 
                #pq, smooth_boxes, eps_boxes, Q_C0 = isotopic_curve(polynomial, boxes) 
                println("Total Eval: $total_eval")
                println("Total Points: $total_points")
                println("Eval per Point: $(total_eval / total_points)")
                println()

                if benchmark_mode
                    println(benchmark_file, "Total Eval: $total_eval")
                    println(benchmark_file, "Total Points: $total_points")
                    println(benchmark_file, "Eval per Point: $(total_eval / total_points)")
                    println()
                end
            end

            if DEBUG_LEVEL >= 1
                println("Total Boxes Count: $total_boxes")
                println("Leaf Boxes: $(C0_boxes + total_boxes)")
                println("Boxes Thrown away by C0: $(C0_boxes)")
                if benchmark_mode
                    println(benchmark_file, "Total Boxes Count: $total_boxes")
                    println(benchmark_file, "Leaf Boxes: $(C0_boxes + total_boxes)")
                    println(benchmark_file, "Boxes Thrown away by C0: $(C0_boxes)")
                end
            end

            if DEBUG_LEVEL >= 2
                println("Epsilon Small Boxes: $(length(eps_boxes))")
                if benchmark_mode
                    println(benchmark_file, "Epsilon Small Boxes: $(length(eps_boxes))")
                end
            end
            
            if DEBUG_LEVEL >= 3 
                segments, boxes = isotopic_connect!(pq, polynomial.slp)
                
                println("Final Boxes Count: $(length(boxes))")
                if benchmark_mode
                    println(benchmark_file, "Final Boxes Count: $(length(boxes))")
                end

                if DEBUG_LEVEL >= 4 && DEBUG_LEVEL < 7
                    mkpath("images")

                    filename = joinpath("images", " iso_curve_$now_str.png")

                    fig = plot_boxes(poly, polynomial.slp, boxes, initial, segments, smooth_boxes, eps_boxes, Q_C0; levels = [0.0])
                    save(filename, fig)
                    println("Saved $(poly) to $filename\n")
                end

                if DEBUG_LEVEL == 7
                    println("Total points in derivatives dictionary: $(length(derivatives_taylor ))")
                    for (point) in derivatives_taylor 
                        println("Point $point")
                    end
                    println("\n=== DEBUG_LEVEL 8: Entering Interactive Mode ===")
                    println("\nEnter all parameters (space-separated): i j p_x p_y")
                    print("Input: ")
                    input_line = readline()
                    inputs = split(input_line)
                    if length(inputs) != 4
                        println("Error: Expected 4 parameters, got $(length(inputs))")
                        println("Usage: i j p_x p_y")
                    else
                        try
                            i = parse(Int, debug_query_args[1]) 
                            j = parse(Int, debug_query_args[2]) 
                            p_x = parse(Float64, debug_query_args[3])  
                            p_y = parse(Float64, debug_query_args[4])  
                            p = (p_x, p_y)
                            
                            if haskey(derivatives_taylor, p)
                                point_derivatives = derivatives_taylor[p]
                                
                                if i >= 1 && i <= length(point_derivatives) && 
                                j >= 1 && j <= length(point_derivatives[i])
                                    derivative_value = point_derivatives[i][j]
                                    println("Found derivative at point $p")
                                    println("dx^$(i)dy^$(j) = $derivative_value")
                                else
                                    println("Error: Derivative order ($i, $j) not available for point $p")
                                    println("Available orders: $(length(point_derivatives)) x $(length(point_derivatives[1]))")
                                end
                            else
                                println("Error: Point $p not found in derivatives dictionary")
                                println("Available points: $(collect(keys(derivatives_taylor )))")
                            end
                        catch e
                            println("Error parsing DEBUG_LEVEL=7 arguments: $e")
                            println("Usage: i j p_x p_y where i,j are integers and p_x, p_y are floats")
                        end
                    end
                end
            end
        end
    end

    if DEBUG_LEVEL == -1 && !isempty(table_rows)
        println()
        println(rpad("Curve", 12), rpad("Method", 10), rpad("Min (ms)", 12),
                rpad("Median (ms)", 14), rpad("Mean (ms)", 12), rpad("Memory (MB)", 13), rpad("Efficacy", 12))

        for row in table_rows
            println(rpad(row.curve, 12),
                    rpad("Taylor", 10),
                    @sprintf("%-11.2f", row.min_ms),
                    @sprintf("%-13.2f", row.median_ms),
                    @sprintf("%-11.2f", row.mean_ms),
                    @sprintf("%-12.2f", row.memory_mb),
                    @sprintf("%-12.2f", row.efficacy))
        end
        println()

        empty!(table_rows)
    end
end

if abspath(PROGRAM_FILE) == @__FILE__
    main() 
end