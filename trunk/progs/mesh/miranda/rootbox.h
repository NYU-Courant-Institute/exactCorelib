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

#include "mk-defs.h"
#include "mk-inl.h"

template <typename NT> 
inline const bool Overlap(const IntervalT<NT> &s, const IntervalT<NT> &t);

template <typename NT>
class RootBoxT {
public:
  // Constructor:
  RootBoxT(const BoxT<NT> *B) : 
    outerBox_(B), innerBox_(B),
    strongIsolFlag_(false),
    failRefineFlag_(false) { }

  ~RootBoxT() {
    delete outerBox_;
  }

  //Inner class type definition
  typedef BoxT<NT> Box;
  typedef IntervalT<NT> Interval;

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
  const bool Refinement(const MKPredicates<NT> &pred) {

//cout << "in refinement" << endl;

    vector<const Box *> Qtmp;
    // split the inner box 
    Cover_Exclude(pred, innerBox_, &Qtmp);
    // loop started
    while(!Qtmp.empty()) {
      const Box *box = Qtmp.back();	
      Qtmp.pop_back();
      // too small
      if(pred.Min(box)) {


//cout << "too small in refinement" << endl;


        innerBox_ = box;  // we cannot refine it anymore, only make it the smallest one
        return false;
      }
      // if box satisfies MK test, we set the new inner box to this one
      // else keep splitting
      if(pred.MKTest(box)) {


//cout << "passed MK test in refinement" << endl;

        innerBox_ = box;
        return true;
      }
      else {


//cout << "split in refinement" << endl;

        Cover_Exclude(pred, box, &Qtmp);
      }
    }

    return false;
  }

  // This routine will make the innerbox a strong
  // rootbox. Return false means that the innerbox
  // cannot be refined anymore
  const bool StrongIsol(const MKPredicates<NT> &pred) {


//cout << "in strong isol " << endl;


    Box *triple_box = innerBox_->Dilate(3);
    while(!pred.JTest(triple_box)) {

//cout << "before refine" << endl;

      if(!Refinement(pred)) { // cannot refine 

//cout << "cannot refine" << endl;
        
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

//cout << "can refine" << endl;

    delete triple_box;
    strongIsolFlag_ = true;
    return true;
  }

  /// Cover(Box B, Queue Q)
  ///   Splits B into 9 regions and push them into Q.
  void Cover(const Box *region,
      vector<const Box*> *queue) {

//cout << "in cover" << endl;

    const NT &x_start = region->x_range.getL();
    const NT &x_end   = region->x_range.getR();
    const NT &x_mid   = region->x_range.mid();
    const NT &y_start = region->y_range.getL();
    const NT &y_end   = region->y_range.getR();
    const NT &y_mid   = region->y_range.mid();
    const NT &x_halfwidth  = region->x_range.width()/4;
    const NT &y_halfwidth  = region->y_range.width()/4;


    // statistic collection only
    extern unsigned int largest_gen;
    const unsigned int gen_id = region->generation_id + 1;
    if(largest_gen < gen_id)
      largest_gen = gen_id;

    queue->push_back(new Box(gen_id, 
      Interval(x_start, x_mid), Interval(y_start, y_mid)));
    queue->push_back(new Box(gen_id, 
      Interval(x_mid, x_end), Interval(y_start, y_mid)));
    queue->push_back(new Box(gen_id, 
      Interval(x_start, x_mid), Interval(y_mid, y_end)));
    queue->push_back(new Box(gen_id, 
      Interval(x_mid, x_end), Interval(y_mid, y_end)));
  
    
for(unsigned haha=0; haha<queue->size(); haha++)
  cout << queue->at(haha)->x_range << " , " << queue->at(haha)->y_range << endl;
  
    
    // next five subregions:
    queue->push_back(new Box(gen_id,    // q_1/2
      Interval(x_mid, x_end), Interval(y_mid-y_halfwidth, y_mid+y_halfwidth)));
      
cout << endl;
for(unsigned haha=0; haha<queue->size(); haha++)
  cout << queue->at(haha)->x_range << " , " << queue->at(haha)->y_range << endl;
    /*
    cout << "create first interval" << endl;
    Interval a(x_mid, x_end);
    cout << "create second interval" << endl;
    Interval b(y_mid-y_halfwidth, y_mid+y_halfwidth);
    cout << "create box" << endl;
    Box *xixi = new Box(gen_id, a, b);
    cout << xixi->x_range << endl;
    cout << xixi->y_range << endl;
    cout << "push"<< endl;
    queue->push_back(xixi);
    cout << "queue.back: " << queue->back()->x_range << endl;
    */

   /* 
    queue->push_back(new Box(gen_id,    // q_3/2
      Interval(x_mid-x_halfwidth, x_mid+x_halfwidth), Interval(y_start, y_mid)));
    */  
cout << "before" << endl;
    Box* bb = new Box(gen_id,    // q_3/2
      Interval(x_mid-x_halfwidth, x_mid+x_halfwidth), Interval(y_start, y_mid));
cout << "after" << endl;
  queue->push_back(bb);
   /* 
    cout << endl;
    cout << "create first interval" << endl;
    Interval c(x_mid-x_halfwidth, x_mid+x_halfwidth);
    cout << "create second interval" << endl;
    Interval d(y_start, y_mid);
    cout << "create box" << endl;
    Box *hehe = new Box(gen_id, c, d);
    
    cout << "push"<< endl;
    queue->push_back(hehe);
*/
    queue->push_back(new Box(gen_id,    // q_5/2
      Interval(x_start, x_mid), Interval(y_mid-y_halfwidth, y_mid+y_halfwidth)));
    queue->push_back(new Box(gen_id,    // q_7/2
      Interval(x_mid-x_halfwidth, x_mid+x_halfwidth), Interval(y_mid, y_end)));
    queue->push_back(new Box(gen_id,    // q_0
      Interval(x_mid-x_halfwidth, x_mid+x_halfwidth),
      Interval(y_mid-y_halfwidth, y_mid+y_halfwidth)));


//    delete region;
  }

  /// Cover_Exclude(Box B, Queue Q, Queue ExcludeQ)
  ///   Splits B into 9 regions and push each child into Q (if it fails C0)
  void Cover_Exclude(const MKPredicates<NT> &pred, 
    const Box *region, vector<const Box *> *queue) {
    // temp queue for processing
    vector<const Box *> temp;
    temp.reserve(10);
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
    if(!queue->empty()) {// some C0 fails, we don't need initial box anymore
cout << "C0 all holds" << endl;
      delete region;
    }
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
