# Define the Julia executable
JULIA = julia
# The name of the ZIP file
ZIP_FILE = iso_curve.zip

# The source directory containing the files to zip (current directory)
SRC_DIR = .

REMOVE_FOLDER = iso_curve

p = input.txt

method = S

DEBUG_LEVEL = 4

BENCHMARK = 0

# The default target to run your Julia program
default: run

# A target to run your Julia program
run: checkDep 
	$(JULIA) main.jl $(or $(ARGS), $(p)) $(method) $(DEBUG_LEVEL) $(if $(filter 1,$(BENCHMARK)),--benchmark)

# Target to run with benchmarking enabled
benchmark: BENCHMARK=1
benchmark: run

compileTime ct: checkDep
	@echo "Measuring Julia compilation/loading time..."
	@start_time=$$(date +%s.%N); \
	$(JULIA) --compile=yes -e "include(\"subdiv.jl\"); println(\"Script loaded successfully\")" > /dev/null 2>&1; \
	end_time=$$(date +%s.%N); \
	compile_time=$$(echo "$$end_time - $$start_time" | bc -l); \
	printf "Compilation/Loading time: %.3f seconds\n" $$compile_time

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
