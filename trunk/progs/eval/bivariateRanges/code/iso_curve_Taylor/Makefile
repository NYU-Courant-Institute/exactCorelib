# Define the Julia executable
JULIA = julia
# The name of the ZIP file
ZIP_FILE = iso_curve_Taylor.zip

# The source directory containing the files to zip (current directory)
SRC_DIR = .

REMOVE_FOLDER = iso_curve_Taylor

p = input.txt

BENCHMARK = 0

DEBUG_LEVEL = 4

# The default target to run your Julia program
default: run

# A target to run your Julia program
run: checkDep 
	$(JULIA) main.jl $(or $(ARGS), $(p)) $(DEBUG_LEVEL) $(if $(filter 1,$(BENCHMARK)),--benchmark)

# Target to run with benchmarking enabled
benchmark: BENCHMARK=1
benchmark: run

# Target to create the zip file
zip: clean $(ZIP_FILE)


# Command to create the zip file
$(ZIP_FILE): 
	zip -r $@ $(SRC_DIR) -x "*.vscode/*" -x "*.DS_Store"

# Clean target to remove the zip file
clean:
	rm -f $(ZIP_FILE)
	rm -rf $(REMOVE_FOLDER)

checkDep:
	$(JULIA) check_dep.jl

# Phony targets
.PHONY: default clean run zip checkDep
