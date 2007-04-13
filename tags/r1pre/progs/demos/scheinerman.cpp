/***************************************************************
 * File: scheinerman.cpp
 * Author: Chee Yap <yap@cs.nyu.edu>
 *
 * Description:  
 * 	Based on a paper by E.R. Scheinerman,
 *		``When Close Enough is Close Enough''
 *	Amer.Math.Monthly, 107(2000)489--499.
 * 	
 * 	There is an imaginary conversation between a student
 * 	who tried to prove an algebraic identity by numerical
 * 	approximation, and teacher who insists that it is the
 * 	wrong way to prove such identities.  
 *
 * 	Their conversation has been paraphrased to suit Core Library:
 *
 * Date:   Apr 14, 2004
 * Since Core Library Version 1.7
 * $Id: scheinerman.cpp,v 1.1 2006-03-07 04:51:23 exact Exp $
 **************************************************************/


#include "CORE/CORE.h"
using namespace std;

int main() {

// ****************************************************************************
// Set the output to show 10 digits (like the calculator)
// ****************************************************************************
  cout << "==============================================" << endl;
  cout << "SCHEINERMAN'S TEST" << endl;
  cout << "==============================================" << endl;
  setDefaultOutputDigits(10);
  cout << "###########   Output Digits = 10" << endl;

// ****************************************************************************
// TEACHER: Show that sqrt(2) + sqrt(5 - 2* sqrt(6))  = sqrt(3)
// ****************************************************************************

  double X = sqrt(Expr(2)) + sqrt(5 - 2* sqrt(Expr(6)));
  double Y = sqrt(double(3));

  cout << "\n X = sqrt(2) + sqrt(5-2 sqrt(6)) \n"
  	<< "     = " << X << endl;
  cout << "\n Y = sqrt(3) \n"
  	<< "     = " << Y << endl;
 
// ****************************************************************************
// STUDENT: They are the same!  My calculator shows that both sides
//	evaluates to 1.732050808.
// TEACHER: That is an accident!  Your calculator display only 10 digits.

// STUDENT: Go away, it cannot be coincidence.
// TEACHER: OK, try this on your calculator:
// 	sqrt(75025) + sqrt(121393) + sqrt(196418) + sqrt(317811) 
// ****************************************************************************
		
  double A = sqrt(Expr(75025))+sqrt(Expr(121393))+sqrt(Expr(196418))+sqrt(Expr(317811));

  cout  << "\n A = sqrt(75025) + sqrt(121393) + sqrt(196418) + sqrt(317811) \n"
  	<< "     = " << A << endl;
	
// ****************************************************************************
// STUDENT: (after a pause) I get 1629.259889.
// TEACHER: Good, now try sqrt(514229) + sqrt(832040)
// ****************************************************************************

  double B = sqrt(Expr(514229)) + sqrt(Expr(832040));
  cout  << "\n B = sqrt(514229) + sqrt(832040) \n"
  	<< "     = " << B << endl;

// ****************************************************************************
// STUDENT: (after another pause) Hey, I get 1629.259889 again!
// 	So they are equal.
// TEACHER:  No, let me show you in Core Library...
// ****************************************************************************

// ****************************************************************************
// Set the output to 20 digits first
// ****************************************************************************
  setDefaultOutputDigits(20);
  cout << "\n\n ###########   Output Digits = 20" << endl;

// ****************************************************************************
// Redisplay A and B at 20 digit
// ****************************************************************************
  cout  << "\n Here is A again:   " << A << endl;
  cout  << "\n Here is B again:   " << B << endl;

// ****************************************************************************
// TEACHER: Notice that these two numbers differ at the 9th decimal place.
// 	   But you see, it would be impossible to numerically determine
// 	   whether 
//
// 	   	X = sqrt(2) + sqrt(5 - 2* sqrt(6))  
// 	   	Y = sqrt(3)
// 	   
// 	   since no many how many digits of a and b you compute, you
// 	   will see that they agree except possibly the last one.
//
// 	   Let us compute X and Y to 70 digits.
// ****************************************************************************

  setDefaultOutputDigits(70);
  cout << "\n\n ###########   Output Digits = 70" << endl;

  cout  << "\n X = sqrt(2) + sqrt(5-2 sqrt(6)) \n"
  	<< "     = " << X << endl;
  // Prints: 1.7320508075688772935274463415058723669428 (41 digits)
  cout  << "\n Y = sqrt(3) \n"
  	<< "     = " << Y << endl;
  // Prints: 1.73205080756887729352744634150587236694 (39 digits)

// ****************************************************************************
// STUDENT: Hey, they seem to agree, but neither one printed 70 digits!
//
// TEACHER: Oh, I know what is going on.  I read in the Tutorial that
// 	Core Library prints the current approximation that it knows
// 	Apparently, the current approximations do not have 70 digits of accuracy.  
//
// 	We can ask Core Library to approximate X and Y to at least 70 digits.  
// 	Let us see: 70 digits is less than 4*70 = 280 bits.  
// 	So if we force evaluation to 280 relative bits of precision,
// 	we will be able to see 70 digits.
// ****************************************************************************

  cout << "\n\n ###########   Approximate to 280 bits of relative precision"
	  << endl;
  X.approx(280,CORE_INFTY);
  Y.approx(280,CORE_INFTY);

  cout << "\n X = " << X << endl;
// X = 1.732050807568877293527446341505872366942805253810380628055806979451933
  cout << "\n Y = " << Y << endl;
// Y = 1.732050807568877293527446341505872366942805253810380628055806979451933

// ****************************************************************************
// STUDENT: Great, both prints exactly 70 digits.  But
// 	why don't we ask Core Library whether X and Y are equal?
// ****************************************************************************

  if (X == Y)
	  cout << "\n Core Library thinks X == Y (CORRECT!)" << endl;
  else 	
	  cout << "\n Core Library thinks X != Y (ERROR!)" << endl;

// ****************************************************************************
// STUDENT: Hey, Core Library got it right! 
//
// TEACHER:  Perhaps that was a fluke. Let us run a few more tests.
//
// 	(After testing a few more identities...)
//
// 	Amazing!  Core Library got it right every time!
//
// 	Now I am convinced that Core Library does some kind of theorem proving.
// 	After all, we know that that it is impossible to decide equality by
// 	numerical approximation.
//
// STUDENT: Thanks, teach!  That was a great object lesson.
// ****************************************************************************
 
  return 0;
}


