# !/bin/sh

# echo "                                           START"

# check command line argument
if [[ $1 = r ]]; then
    echo "Printing names of real polynomials" $1
    decide=r
elif  [[ $1 = c ]]; then
    echo "Printing names of complex polynomials" $1
    decide=c
else
    echo "Usage " $0 "[c|r]  defaulting to r"
    decide=r
fi


# process every FRISCO polynomial in the frisco subdirectory
cd frisco
for x in *pol  ;
   { 
    # echo "Processing polynomial" $x "..."; 
	
    # Select first few lines of the polynomial file
    # ignore those that are comments
    # look for "c" in complex and for "r" in real polynomials
    # head -5 $x | grep -v "!" | grep -c "c" 

    if [[ `head -5 $x | grep -v "!" | grep -c $decide` -ne 0 ]]; then
           # real polynomial
           echo $x
    fi
   } 

cd ..

# echo "                                           END"
