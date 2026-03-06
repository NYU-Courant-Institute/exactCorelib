#= 
    @author: Thomas Chen
    @advisors: Yap Chee, Kai Hormann, Bingwei Zhang
    @date: 2025-06-17
    @description: Dependency Check and automatically installation
=# 

using Pkg

# List of required packages
required_packages = [
    "DataStructures",
    "Combinatorics",
    "BenchmarkTools",
    "Printf"
]

# Function to check if a package is installed
function is_installed(pkg::String)::Bool
    deps = Pkg.dependencies()
    return any(p -> p.name == pkg, values(deps))
end

# Check and install packages if they are not already installed
for pkg in required_packages
    if !is_installed(pkg)
        println("Installing $pkg...")
        Pkg.add(pkg)
    else
        println("$pkg is already installed.")
    end
end
