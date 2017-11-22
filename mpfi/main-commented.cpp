// COMMENTED CODE 
//
// Original code from Burr (Nov'2017)
// On my computer, this compiles with
// g++ -std=c++11 main.cpp -lmpfi -lmpfr

#include <iostream>
#include <boost/multiprecision/mpfr.hpp>
#include <boost/multiprecision/mpfi.hpp>
#include <boost/numeric/interval.hpp>

using namespace std;
using namespace boost::numeric;
using namespace boost::multiprecision;

int main()
{
    // Create an interval (10,10) using doubles and a few other doubles
    interval<double>
		my_double_interval = 10;
    double my_double,
	   my_double_width,
	   my_double_upper,
	   my_double_lower,
	   y_double_upper_unrounded,
	   my_double_lower_unrounded,
	   y_unrounded_width;
    
    // Create an Boost MPFI interval (10,10) using 50 digits of precision and a few MPFR values
    mpfi_float_50 
		my_second_small_interval,
		my_small_interval = 10;
    mpfr_float_50
		my_small_number,
		my_small_width,
		my_small_number_upper,
		my_small_number_lower;
    
    // Create an Boost MPFI interval (10,10) using 1000 digits of precision and a few MPFR values
    mpfi_float_1000
		my_second_large_interval,
		my_large_interval = 10;
    mpfr_float_1000
		my_large_number,
		my_large_width,
		my_large_number_upper,
		my_large_number_lower;
    
    // Take the square root of all intervals
    my_double_interval = sqrt(my_double_interval);
    my_small_interval = sqrt(my_small_interval);
    my_large_interval = sqrt(my_large_interval);
    
    // Find the widths of all intervals
    my_double =		width(my_double_interval);
    my_small_number =	width(my_small_interval);
    my_large_number =	width(my_large_interval);
    
    // Display the intervals and the corresponding widths.
    // interval<double> does not have a built-in display, so we'll do it manually.
    
    cout << "ORIGINAL VALUES:\n\n";
    
    cout << "The interval and its width using doubles:\n";
    cout << "{" << my_double_interval.lower() << "," << my_double_interval.upper() << "}\n";
    cout << my_double << "\n\n";
    
    cout << "The interval and its width using MPFR with 50 digits of precision:\n";
    cout << my_small_interval << "\n";
    cout << my_small_number << "\n\n";
    
    cout << "The interval and its width using MPFR with 1000 digits of precision:\n";
    cout << my_large_interval << "\n";
    cout << my_large_number << "\n\n";
    
    // Now, we're going to investigate ways to convert to doubles from the MPFR data types.
    
    cout << "CONVERTING FROM 50 DIGITS OF PRECISION TO DOUBLES:\n\n";
    
    // First, convert the upper and lower values to mpfr types (might not be necessary).
    
    my_small_number_upper = upper(my_small_interval);
    my_small_number_lower = lower(my_small_interval);
    my_small_width = my_small_number_upper - my_small_number_lower;
    
    // We check that the upper and lower values are what we expected to get
    
    cout << "Getting the upper and lower limits and the width of the interval:\n";
    
    cout << my_small_number_upper << "\n";
    cout << my_small_number_lower << "\n";
    cout << my_small_width << "\n";
    
    // We would like to check that the width is correct by comparing to the previous width.
    
    if(my_small_width==my_small_number)
        cout << "Widths are the same.\n\n";
    else
        cout << "Widths are not the same.\n\n";
    
    // The basic conversion will not work because it uses "round to nearest"
    // With a high precision situation, the nearest for the upper and lower is the same
    
    my_double_upper_unrounded = (double) my_small_number_upper;
    my_double_lower_unrounded = (double) my_small_number_lower;
    
    // Get the difference between the rounded upper and lower
    
    my_unrounded_width = my_double_upper_unrounded - my_double_lower_unrounded;
    
    cout << "Getting the rounded upper and lower limits and the width of the interval:\n";
    
    cout << my_double_upper_unrounded << "\n";
    cout << my_double_lower_unrounded << "\n";
    cout << my_unrounded_width << "\n";
    
    if(my_unrounded_width == my_double)
        cout << "Widths are same\n\n";
    else
        cout << "Widths are different\n\n";
    
    // Converting the right way means that we round up on the upper endpoint and round down on the lower endpoint
    // This must be done through the backend, otherwise the rounding method is wrong.
    // This might be possible through the MPFI backend access, but I haven't tried it yet.
    
    my_double_upper = mpfr_get_d(my_small_number_upper.backend().data(),MPFR_RNDU);
    my_double_lower = mpfr_get_d(my_small_number_lower.backend().data(),MPFR_RNDD);
    my_double_width = my_double_upper - my_double_lower;
    
    cout << "Getting the correctly rounded upper and lower limits and the width of the interval:\n";
    
    cout << my_double_upper << "\n";
    cout << my_double_lower << "\n";
    cout << my_double_width << "\n";
    
    if(my_double_width == my_double)
        cout << "Widths are same.\n\n";
    else
        cout << "Widths are not the same.\n\n";
    
    // Converting from lower precision is not lossy
    
    my_second_small_interval = mpfi_float_50(my_double_interval.lower(),my_double_interval.upper());
    
    cout << "CONVERTING FROM DOUBLE TO 50 DIGITS OF PRECISION:\n\n";
    
    cout << "Getting the upper and lower limits and the width of the 50 precision interval:\n";
    
    cout << upper(my_second_small_interval) << "\n";
    cout << lower(my_second_small_interval) << "\n";
    cout << width(my_second_small_interval) << "\n";
    
    if((double)width(my_second_small_interval) == my_double)
        cout << "Widths are same.\n\n";
    else
        cout << "Widths are not the same.\n\n";
    
    cout << "CONVERTING FROM 50 TO 1000 DIGITS OF PRECISION:\n\n";
    
    // Converting between MPFI with 50 digits of precision and 1000 digits of precision.
    // This is not lossy, so it doesn't need to be typecast, but it is still good to typecast
    
    my_second_large_interval = (mpfi_float_1000) my_small_interval;
    
    cout << "Getting the upper and lower limits and the width of the 1000 precision interval:\n";
    
    cout << upper(my_second_large_interval) << "\n";
    cout << lower(my_second_large_interval) << "\n";
    cout << width(my_second_large_interval) << "\n";
    
    if(width(my_second_large_interval) == my_small_number)
        cout << "Widths are same.\n\n";
    else
        cout << "Widths are not the same.\n\n";
    
    cout << "CONVERTING FROM 1000 TO 50 DIGITS OF PRECISION:\n\n";
    
    // Converting from MPFI with 50 digits of precision to 1000 digits of precision is lossy.
    // You do need to typecast in this direction
    // This appears to round correctly.
    
    my_second_small_interval = (mpfi_float_50) my_large_interval;
    
    cout << "Getting the upper and lower limits and the width of the 50 precision interval:\n";
    
    cout << upper(my_second_small_interval) << "\n";
    cout << lower(my_second_small_interval) << "\n";
    cout << width(my_second_small_interval) << "\n";
    
    if(width(my_second_small_interval) == my_small_number)
        cout << "Widths are same.\n\n";
    else
        cout << "Widths are not the same.\n\n";

  return 0;
}
