/***************************************************
 * Chee,
 * 
 *   I see what you're trying to do, I'll try to point you in the right
 *   direction (but note that there are some potential drawbacks as well).
 *
 *    How to do this:
 *
 *       I've prepared another sample document to illustrate how to change
 *       precision on the fly.  Note that MPFR seems to have a better interface
 *       than MPFI.  You can change the MPFR precision on the fly, while you
 *       need to do a little more work for MPFI's (but it can be done).  You
 *       should be able to compile the attached document with the command:
 *
 *        g++ -std=c++11 main2.cpp -lmpfi -lmpfr
 *
 *        It shows some examples of what works and what doesn't work in
 *        changing precision.  You can get the current precision of an interval
 *        by "<interval>.precision()".
 *    Comments on your approach:
 *
 *        The reason that you don't have variable precision is that you're
 *        working in fixed precision (mpfi_float_50 is fixed precision).  You
 *        need to use mpfi_float to have variable precision.  MPFR/MPFI are
 *        slow, so if you're always maintaining these high-precision objects,
 *        the resulting code may be quite slow.  Perhaps it would be better to
 *        have a union where the second type is a pointer to an MPFI so that it
 *        can be null?
 *
 *    I'll take a look at the other document that you sent, but I don't have
 *    any immediate comments on a soft T_1.
 *    Best,
 *
 *    Michael
 *	Nov 22, 2017 
 *************************************************** */


#include<iostream>
#include<boost/multiprecision/mpfr.hpp>
#include<boost/multiprecision/mpfi.hpp>

using namespace std;
using namespace boost::multiprecision;

int main()
{
    
    // Data1 and Data2 have low precision (accuracy to 2 decimal digits)
    mpfi_float::default_precision(2);
    mpfi_float int_data1 = 10,
	       int_data2 = 10,
	       int_data3 = 10;
    
    // Data3 and Data4 have high precision (accuracy to 50 decimal digits)
    mpfi_float_50 int_data4,
		  int_data5=10;
    
    // Data1 and Data2 have low precision (accuracy to 2 decimal digits)
    mpfr_float::default_precision(2);
    mpfr_float data1 = 10,
	       data2 = 10,
	       data3 = 10;
    
    // Data3 and Data4 have high precision (accuracy to 50 decimal digits)
    mpfr_float_50 data4,
		  data5=10;
    
    cout << "WITH MPFR DATA:\n\n";
    
    // Originally, all data has value 10.
    cout << "ORIGINAL DATA:\n\n";
    cout << "Initial Values of Data:\n";
    cout << data1 << "\n";
    cout << data2 << "\n";
    cout << data3 << "\n";
    cout << data5 << "\n\n";
    
    cout << "SQARE ROOT:\n\n";
    cout << "Square root without changing precision:\n";
    
    // Taking the square root of low precision remains low precision.
    cout << "Square root of low precision:\n";
    data1 = sqrt(data1);
    cout << data1 << "\n";
    // Even if stored into a high precision data type (of course).
    cout << "Square root of low precision stored into high precision:\nObserve: computation is done in low precision:\n";
    data4 = sqrt(data2);
    cout << data4 << "\n";
    // Taking sqare root of high precision retains high precision.
    cout << "Square root of high precision (right answer):\n";
    data5 = sqrt(data5);
    cout << data5 << "\n\n";
    
    cout << "Square root with changing precision:\n";
    
    // Changing the default precision.
    cout << "Changing the default precision to high precision:\n";
    mpfr_float::default_precision(50);
    
    // Computation is done in high precision
    cout << "Square root of low precision stored into high precision:\nObserve: computation is done in high precision:\n";
    data4 = sqrt(data2);
    cout << data4 << "\n";
    // Computation is done and stored in high precision (precision is bumped up).
    cout << "Square root of low precision stored into low precision:\nObserve: computation is done in low precision:\n";
    cout << "Data precision: " << data2.precision() << "\n";
    data2 = sqrt(data2);
    cout << data2 << "\n\n";
    
    cout << "Changing precision directly on data:\n";
    cout << "Changing default precision back to low:\n";
    mpfr_float::default_precision(2);
    // Computation is done in high precision because the data has high precision.
    cout << "Changing the precision of a low precision data type to high precision:\nObserve: computation is done in high precision:\n";
    data3.precision(50);
    data3 = sqrt(data3);
    cout << data3 << "\n";
    // The data is rounded to nearest when the precision is reduced.  This is lossy.
    cout << "Changing the precision of the output back to low precision:\nObserve: we get the low precision output (rounded):\n";
    data3.precision(2);
    cout << data3 << "\n\n\n";

    cout << "WITH MPFI DATA:\n\n";
    
    // Originally, all data has value 10.
    cout << "ORIGINAL DATA:\n\n";
    cout << "Initial Values of Data:\n";
    cout << int_data1 << "\n";
    cout << int_data2 << "\n";
    cout << int_data3 << "\n";
    cout << int_data5 << "\n\n";
    
    cout << "SQARE ROOT:\n\n";
    cout << "Square root without changing precision:\n";
    
    // Taking the square root of low precision remains low precision.
    cout << "Square root of low precision:\n";
    int_data1 = sqrt(int_data1);
    cout << int_data1 << "\n";
    // Even if stored into a high precision data type (of course).
    cout << "Square root of low precision stored into high precision:\nObserve: computation is done in low precision:\n";
    int_data4 = sqrt(int_data2);
    cout << int_data4 << "\n";
    // Taking sqare root of high precision retains high precision.
    cout << "Square root of high precision (right answer):\n";
    int_data5 = sqrt(int_data5);
    cout << int_data5 << "\n\n";
    
    cout << "Square root with changing precision:\n";
    
    // Changing the default precision.
    cout << "Changing the default precision to high precision:\n";
    mpfi_float::default_precision(50);
    
    // Computation is done in high precision
    cout << "Square root of low precision stored into high precision:\nObserve: computation is done in high precision:\n";
    int_data4 = sqrt(int_data2);
    cout << int_data4 << "\n";
    // Computation is done and stored in high precision (precision is bumped up).
    cout << "Square root of low precision stored into low precision:\nObserve: computation is done in low precision:\n";
    cout << "Data precision: " << int_data2.precision() << "\n";
    int_data2 = sqrt(int_data2);
    cout << int_data2 << "\n\n";
    
    cout << "Changing precision directly on data:\n";
    cout << "Changing default precision back to low:\n";
    mpfi_float::default_precision(2);
    // Canging precision destroys data
    cout << "Changing the precision of a low precision data type to high precision destroys the data:\n";
    cout << int_data3 << "\n";
    mpfr_float::default_precision(50);
    mpfr_float my_upper, my_lower;
    my_upper = upper(int_data3);
    my_lower = lower(int_data3);
    int_data3.precision(50);
    cout << int_data3 << "\n";
    
    // However, if the data is stored in other types, we can change the precision.
    cout << "We can change the precision by temporarily storing the data elsewhere:\n";
    mpfi_float::default_precision(50);
    int_data3 = (mpfi_float)(my_lower,my_upper);
    int_data3 = sqrt(int_data3);
    cout << int_data3 << "\n";
    
    return 0;
}
