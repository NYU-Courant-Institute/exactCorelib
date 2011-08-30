/**
 *  algorithm.h
 *  major algorithm
 *
 *  This contains a name space called Algorithm:
 *
 *  It contains only one method called Run.
 *
 *  Run takes arguments:
 *  	(1) An instance of the class MKPredicates  (which has 2 polynomials)
 *  	(2) An initial Box 
 *  	(3) 3 Queues: Q_output, Q_ambiguous, Q_exclude
 *  	
 *  Run has several while loops:
 *  	(1) Apply the C0 and JC test to put boxes into a queue called Q_confirm 
 *  	(2) Apply the MK test to boxes in Q_confirm to produce a priority queue
 *  		of root boxes, called Q_roots
 *  	(3) Finally, refine the roots in Q_root until they are pairwise
 *  		disjoint, and placed into Q_output
 *
 * Author: Shang Wang, July 17, 2011
 *
 */

#ifndef MK_ALGORITHM_H_
#define MK_ALGORITHM_H_

#include <vector>
#include <iostream>

#include "mk-inl.h"

using namespace std;

namespace Algorithm {

  template <typename T> void Run(
      const MKPredicates<T> &pred,
      const BoxT<T> *initial,
      vector<const BoxT<T> *> *output,
      vector<const BoxT<T> *> *ambiguous,
      vector<const BoxT<T> *> *exclude) {

    // inner type definition
    typedef BoxT<T> Box;
    // main process queue
    vector<const Box *> Q_tmp;
    // Jacobian queue
    vector<const Box *> Q_confirm;
    // Priority Queue 
    MultiList< MKPredicates<T> >

    if(!pred.Exclude(initial)) { // current box fails C0
      Q_tmp.push_back(initial);
    }
    else {
        exclude->push_back(initial);
    }

    while(!Q_tmp.empty()) { // main loop
      const Box *current = Q_tmp.back();
      Q_tmp.pop_back();
      // (current box)X2 passes the Jacobian test
      const Box *double_current = current->Dilate(2);
      if(pred.JTest(double_current)) {
        Q_confirm.push_back(current);  // wait for further confirmation
      }
      else {
        pred.Split_Exclude(current, &Q_tmp, exclude); // split and test C0
      }
      delete double_current;

      // confirmation loop
      while(!Q_confirm.empty()) {
        const Box *box = Q_confirm.back();
        Q_confirm.pop_back();
        // box too small
        if(pred.Min(box)) {
          ambiguous->push_back(box);
          continue;
        }
        // also do MK test on (box)X2
        const Box *double_box = box->Dilate(2);
        if(pred.MKTest(double_box)) {
          // CHECK IF size of sub is larger than max-size,
          // and if so, refine, else put it in output
output->push_back(box);
//          pred.Refinement(box, output, ambiguous, exclude);

          Q_confirm.clear();  // include area already found in region, clean Q_confirm
          delete double_box;
          break;
        }
        else {
          //cout << endl;
          pred.Split_Exclude(box, &Q_confirm, exclude);
          delete double_box;
        }
      }//while (Q_confirm)
    }//while (Q_tmp)
  }//Run
}  // end of namespace algorithm

#endif
