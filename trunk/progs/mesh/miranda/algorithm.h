/**
 *  algorithm.h
 *  major algorithm
 *
 * July 17, 2011
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
    vector<const Box *> mainq;
    // Jacobian queue
    vector<const Box *> jqueue;

    if(!pred.Exclude(initial)) { // current box fails C0
      mainq.push_back(initial);
    }
    else {
        exclude->push_back(initial);
    }

    while(!mainq.empty()) { // main loop
      const Box *current = mainq.back();
      mainq.pop_back();
      // (current box)X2 passes the Jacobian test
      const Box *double_current = current->Dilate(2);
      if(pred.JTest(double_current)) {
        jqueue.push_back(current);  // wait for further confirmation
      }
      else {
        pred.Cover_Exclude(current, &mainq, exclude); // split and test C0
      }
      delete double_current;

      // confirmation loop
      while(!jqueue.empty()) {
        const Box *box = jqueue.back();
        jqueue.pop_back();
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
          pred.Refinement(box, output, ambiguous, exclude);
          jqueue.clear();  // include area already found in region, clean jqueue
          delete double_box;
          break;
        }
        else {
          //cout << endl;
          pred.Cover_Exclude(box, &jqueue, exclude);
          delete double_box;
        }
      }//while (jqueue)
    }//while (mainq)
  }//Run
}  // end of namespace algorithm

#endif
