/* ***************************************************
*
* rootbox.h
*
*    This file describes the templated class called RootBoxT.
*
*    	The template parameter NT is some number type,
*    	typically DoubleWrapper, machine_double, or BigFloat.
*    	This type is used for the dimension of the boxes.
*
*    The class has the following members:
*    	--Boxes: OuterBox and InnerBox
*    	--Boolean: StrongIsolFlag, FailRefineFlag
*
*    OuterBox is known to contain a unique root of fxy, gxy in its interior
*    InnerBox is contained in Outerbox, and also
*    	known to contain a unique root of fxy, gxy.
*    	Thus, the annulus region between Outerbox and InnterBox has no roots.
*
*    Initially, InnerBox=OuterBox.
*    	We will preserve the OuterBox, but the InnerBox can shrink
*    	by calling the Refine Procedure.
*
*    StrongIsolFlag is true if we know that 3*InnerBox contains a unique root.
*    	Initially, StrongIsolFlag = false.
*    FailRefineFlag is true if our attempt to refine the InnerBox fails.
*    	Note that this can only happen because our Min_Size global variable
*    	prevents further subdivision during refinement.
*    	Initially, FailRefineFlag = false.
*
* Author: Shang Wang and Chee Yap, Aug 30, 2011
*
* Since Core 2.1.
************************************************** */

#ifndef MK_ROOTBOX_H_
#define MK_ROOTBOX_H_

#include "box.h"

using namespace std;


template <typename NT> class RootBoxT {
public:
  // Constructor:
  RootBoxT( Box<NT> B) {
      OuterBox_ = B;
      InnerBox_ = B;
      StrongIsolFlag = false;
      FailRefineFlag = false;
  }

  ~RootBoxT() { }

  // Method:
  bool Refine(){
      
  }

  // Members:
  Box <NT> OuterBox_;
  Box <NT> InnerBox_;
  bool StrongIsolFlag;
  bool FailRefineFlag;
};

#endif /* ROOTBOX_H_ */
