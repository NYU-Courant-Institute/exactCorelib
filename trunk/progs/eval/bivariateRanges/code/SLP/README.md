# SLP - Straight Line Program

A Julia implementation for polynomial evaluation and derivative computation using Straight Line Program (SLP).

## Description

This project provides tools for:

- Parsing polynomial expressions into tokens and then into AST
- Converting polynomials to SLP representation
- Computing derivatives efficiently
- Interval arithmetic support

## Requirements

- Julia
- Required Julia packages (automatically checked via `check_dep.jl`)

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

This will process the default input file with default mode 0 and digit precision 5, centered interval display mode.

### Custom Input File

```bash
make run p=my_input.txt
```

Replace `my_input.txt` with the path to your input file.
You can also include multiple input files, separated by spaces. For example:

```bash
make run p=my_input1.txt my_input2.txt
```

### Custom Mode

```bash
make run m=0
```

This will run the program in mode 0, which computes the all non-zero derivative of the polynomial.

#### Available Modes

Mode 0: Computes the all non-zero derivative of the polynomial.
Mode 3: Computes the all non-zeroderivatives of only x and y, as well any mixed partial derivatives that are powers of 3.
    - Only works for 2D polynomials

### Custom Digit Precision

```bash
make run d=10
```

This will set the digit precision to 10.

### Custom Centered Interval Display Mode

```bash
make run c=0
```

This will set the centered interval display mode to 0, which means the interval notation will be used.

### Convention

Please first list all input files, followed by the mode, digit precision, and centered interval display mode.

### Output

Results are written to output.txt and include:

- input information
- Evaluation results  
  - function value
  - requested derivative values

## Project Structure

- main.jl - Main entry point
- parser.jl - Polynomial parsing logic
- SLP.jl - Straight Line Program implementation
- derivatives.jl - Derivative computation
- polynomial.jl - Polynomial data structures
- myInterval.jl - Interval arithmetic
- utils.jl - Utility functions
- ASTTypes.jl - Abstract syntax tree definitions
- check_dep.jl - Dependency checker

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
