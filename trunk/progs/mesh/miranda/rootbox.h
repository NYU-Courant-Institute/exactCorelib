/* ***************************************************
*
* rootbox.h
*
*    This file describes the templated class called RootBoxT.
*
*    	The template parameter DT is some number type,
*    	  typically DoubleWrapper, machine_double, or BigFloat.
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

#include "mk-defs.h"
#include "mk-inl.h"


/*
template <typename DT>
const bool overlap(const IntervalT<DT> &s, const IntervalT<DT> &t) {
  return !(s.getR() <= t.getL() || s.getL() >= t.getR());
}
*/
template <typename DT>
inline const bool Overlap(const IntervalT<DT> &s, const IntervalT<DT> &t); 


template <typename DT,typename NT>
class RootBoxT {
public:
  // Constructor:
  RootBoxT(const BoxT<DT> *B) : 
    outerBox_(B), innerBox_(B),
    strongIsolFlag_(false),
    failRefineFlag_(false) { }

  ~RootBoxT() {
    delete outerBox_;
  }

  //Inner class type definition
  typedef BoxT<DT> Box;
  typedef IntervalT<DT> Interval;


  /***************** Methods:********************/

  /// Refinement(Box B, Queue Q-exc)
  ///	Box B is guaranteed to contain a root in its interior (i.e., it is a "root box")
  /// 	This calls Cover_Exclude repeatedly to generate 9 subboxes.
  /// 	Root refinement will return another root box BB contained in B.
  ///		Now, if BB=B, then the refinement has "failed" in some sense.
  ///		And the only reason it fails it because it has to split boxes
  ///		smaller than the global Min_size (it is a safe-ty feature).  
  ///   We can split the annulus region B - BB into 8 subboxes, and these
  ///		are placed into Q-exc.
  const bool Refinement(const MKPredicates<DT,NT> &pred) {
    vector<const Box *> Qtmp;
    // split the inner box 
    Cover_Exclude(pred, innerBox_, &Qtmp);
    // loop started
    while(!Qtmp.empty()) {
      const Box *box = Qtmp.back();	
      Qtmp.pop_back();
      // too small
      if(pred.Min(box)) {
        delete innerBox_;
        innerBox_ = box;  // we cannot refine it anymore, only make it the smallest one
        return false;
      }
      // if box satisfies MK test, we set the new inner box to this one
      // else keep splitting
      if(pred.MKTest(box)) {
        delete innerBox_;
        innerBox_ = box;
        return true;
      }
      else {
        Cover_Exclude(pred, box, &Qtmp);
      }
    }
    // maybe the queue is empty, which means that non of the box that we splitted can
    // pass C0 test
    return false;
  }

  // This routine will make the innerbox a strong
  // rootbox. Return false means that the innerbox
  // cannot be refined anymore
  const bool StrongIsol(const MKPredicates<DT,NT> &pred) {
    Box *triple_box = innerBox_->Dilate(3);
    while(!pred.JTest(triple_box)) {
      if(!Refinement(pred)) { // cannot refine 
        failRefineFlag_ = true;
        delete triple_box;
        return false;
      }
      else {  // can refine, free previous 3B and create
              // a new one based on the smaller innerBox
        delete triple_box;
        triple_box = innerBox_->Dilate(3);
      }
    }
    delete triple_box;
    strongIsolFlag_ = true;
    return true;
  }

  /// Cover(Box B, Queue Q)
  ///   Splits B into 9 regions and push them into Q.
  void Cover(const Box *region,
             vector<const Box*> *queue) {

    const DT &x_start = region->x_range.getL();
    const DT &x_end   = region->x_range.getR();
    const DT &x_mid   = region->x_range.mid();
    const DT &y_start = region->y_range.getL();
    const DT &y_end   = region->y_range.getR();
    const DT &y_mid   = region->y_range.mid();
    const DT &x_halfwidth  = region->x_range.width()/4;
    const DT &y_halfwidth  = region->y_range.width()/4;


    // statistic collection only
    extern unsigned int largest_gen;
    const unsigned int gen_id = region->generation_id + 1;
    if(largest_gen < gen_id)
      largest_gen = gen_id;

//cout << "first" << endl;
    queue->push_back(new Box(gen_id, 
      Interval(x_start, x_mid), Interval(y_start, y_mid)));
//cout << "second" << endl;
    queue->push_back(new Box(gen_id, 
      Interval(x_mid, x_end), Interval(y_start, y_mid)));
//cout << "third" << endl;
    queue->push_back(new Box(gen_id, 
      Interval(x_start, x_mid), Interval(y_mid, y_end)));
//cout << "fourth" << endl;
    queue->push_back(new Box(gen_id, 
      Interval(x_mid, x_end), Interval(y_mid, y_end)));
    // next five subregions:
//cout << "fifth" << endl;
/*
    queue->push_back(new Box(gen_id,    // q_1/2
      Interval(x_mid, x_end), Interval(y_mid-y_halfwidth, y_mid+y_halfwidth)));
*/
//cout << "sixth" << endl;
    queue->push_back(new Box(gen_id,    // q_3/2
      Interval(x_mid-x_halfwidth, x_mid+x_halfwidth), Interval(y_start, y_mid)));
//cout << "seventh" << endl;
    queue->push_back(new Box(gen_id,    // q_5/2
      Interval(x_start, x_mid), Interval(y_mid-y_halfwidth, y_mid+y_halfwidth)));
//cout << "eighth" << endl;
    queue->push_back(new Box(gen_id,    // q_7/2
      Interval(x_mid-x_halfwidth, x_mid+x_halfwidth), Interval(y_mid, y_end)));
//cout << "nineth" << endl;
    queue->push_back(new Box(gen_id,    // q_0
      Interval(x_mid-x_halfwidth, x_mid+x_halfwidth),
      Interval(y_mid-y_halfwidth, y_mid+y_halfwidth)));

//cout << "done" << endl;
/*
    for(unsigned int i=0; i<queue->size(); i++) {
      cout << i << ": " << queue->at(i)->x_range << " , " << queue->at(i)->y_range << endl;
    }
*/
   
//    delete region;
  }

  /// Cover_Exclude(Box B, Queue Q, Queue ExcludeQ)
  ///   Splits B into 9 regions and push each child into Q (if it fails C0)
  void Cover_Exclude(const MKPredicates<DT,NT> &pred, 
    const Box *region, vector<const Box *> *queue) {
    // temp queue for processing
    vector<const Box *> temp;
    // cover box into 9 children 
    Cover(region, &temp);
    // Do C0 test for each box in temp queue
    while(!temp.empty()) {
      const Box *box = temp.back();
      temp.pop_back();
      if(!pred.Exclude(box)) {  // C0 fails, put back to queue
        queue->push_back(box);
      }
      else {
        delete box;
      }
    }
    /*
    if(!queue->empty()) {// some C0 fails, we don't need initial box anymore
      delete region;
    }
    */
  }//Cover_Exclude

  // check if two inner boxes are disjoint
  // the parameter is not RootBox because we are comparing with another 
  // inner box in Q_output(see algorithm.h for detail)
  const bool Disjoint(const Box *other) {
    const Interval &inner_x = innerBox_->x_range;
    const Interval &inner_y = innerBox_->y_range;
    const Interval &other_x = other->x_range;
    const Interval &other_y = other->y_range;
    return (!Overlap(inner_x, other_x) || !Overlap(inner_y, other_y));
  }

  /***************** end of Methods:********************/

  /****************** Members: ************************/
  const Box    *outerBox_;
  const Box    *innerBox_;
  mutable bool strongIsolFlag_;
  mutable bool failRefineFlag_;
};

#endif /* ROOTBOX_H_ */
