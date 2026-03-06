# Iso Curve Taylor

## Description

This project implements an isotopic curve computation algorithm for polynomial zero-level sets using interval arithmetic and adaptive subdivision. This program is based on the pure Taylor method. The program computes and visualizes curves where polynomials equal zero within specified 2D regions.

### Key Features

- **Isotopic curve computation** using adaptive box subdivision
- **Interval arithmetic** for guaranteed numerical accuracy
- **SLP (Straight Line Program)** representation for efficient polynomial evaluation
- **Interactive debugging modes** for analysis and visualization
- **Benchmark capabilities** for performance analysis
- **Visualization support** with plot generation

## Requirements

- Julia
- Required Julia packages (automatically checked via `check_dep.jl`)
  -BenchmarkTools
  - DataStructures
  - GLMakie
  - Dates

## Usage

### Input Structure

- Input file is read line by line
  - Each line corresponds to a polynomial expression
- Each line is separated by commas
  - First, input the polynomial expression
    - ^ as power, * as multiplication, + as addition, - as subtraction, ( and ) as parentheses
    - Only supports integer powers
  - Then, input the value to evaluate at for each variable, separated by commas.
    - Ex. x = 2, y = 3
    - Can also input intervals using []
      - Ex. x = [1.0, 2.0]
  - At the end of the line, takes an optional input called description
    - For identification, does not affect computation
- Ex. ((x+2)^2 + 1)^2, x = 2, "test"  

### Basic Usage

```bash
make run 
```

This will process the default input file with debugging level 4

### Custom Input File

```bash
make run p=my_input.txt
```

Replace `my_input.txt` with the path to your input file.
You can also include multiple input files, separated by spaces. For example:

```bash
make run p=my_input1.txt my_input2.txt
```

### Custom Debugging Level

The DEBUG_LEVEL goes from 0 to 7. The higher the level, the more verbose the output. The default is 4.

```bash
make run DEBUG_LEVEL = 7
```

This will run the program with debugging level 7.

#### Debugging Level Options

- 0: Number of evaluations and and points
- 1: Total number of boxes produced and boxes that retain(leaf_boxes)
- 2: Number of boxes that reach epsilon limit
- 3: Number of boxes that have segment crossing it(final boxes)
- 4: Show the graph of the isotopic curve and final boxes
- 5: Show the graph of the boxes that passed predicate C1
- 6: Show the graph of the boxes that reach epsilon limit
- 7: Enter interactive mode to check values stored inside derivatives of a given point and given derivative index i, j

Except for 7, every Debug level would include its preceding Debug levels.

### benchmark flag

The benchmark flag is set to false by default. Set it to true to enable benchmarking.

```bash
make run benchmark
```

### Convention

Please first list all input files, then the debugging level. Keep the benchmark flag at the end.

### Output

Results are written to outputs folder and include:

- input information
- benchmark results

- Also creates folder images containing plots of the computed curves
  - Activated when debugging level is between 4 and 6

## Project Structure

- main.jl - Main entry point
- utils.jl - Utility functions
- check_dep.jl - Dependency checker
- myBox.jl - Box data structure
- subdiv.jl - Subdivision logic
- range_funcs.jl - Range function computation
- images/ - Folder for generated images
- outputs/ - Folder for output files
- input.txt - Default input file

## Building

```bash
make zip    # Create distribution package
make clean  # Clean build artifacts
```

## Dependencies

Dependencies are automatically checked when running the program. To manually check:

```bash
julia check_dep.jl
```

## Authors

- [Thomas Chen (Author)](https://github.com/ThomasChen0717)
- Bingwei Zhang(Collaborator)
- Yap Chee(Supervisor)
- Kai Hormann(Supervisor)
