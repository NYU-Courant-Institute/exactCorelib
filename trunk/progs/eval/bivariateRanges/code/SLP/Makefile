# Define the Julia executable
JULIA = julia
# The name of the ZIP file
ZIP_FILE = SLP.zip

# The source directory containing the files to zip (current directory)
SRC_DIR = .

# The folder to remove from the zip file
REMOVE_FOLDER = SLP

# The default input file
p = input.txt

# The default mode
m = 0

d = 5

c = 1

# The default target to run your Julia program
default: run

# A target to run your Julia program
run: checkDep 
	$(JULIA) main.jl $(or $(ARGS), $(p)) $(m) $(d) $(c)

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
