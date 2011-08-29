/**
*
* mk-inl.h
* C0 test, C1 test, MK test
*
* July 16, 2011
*/

#ifndef MK_MK_INL_H_
#define MK_MK_INL_H_

#include "mk-defs.h"
#include "box.h"

using namespace std;

template <typename T>
class MKPredicates {
public:
  // Constructor
  MKPredicates(const Curve<T> &fxy, 
	  const Curve<T> &gxy, const T &min_size, const T &max_size,
	  const unsigned int max_gen_id) : fxy_(fxy), gxy_(gxy),
	  jacobian_(MatrixT<Curve<T> >(2)), min_size_(min_size),
	  max_size_(max_size), max_gen_id_(max_gen_id)
  {
    // differentiate is "self-modified" operator
    Curve<T> temp1 = fxy, temp2 = fxy;
    Curve<T> temp3 = gxy, temp4 = gxy;
    jacobian_(0, 0) = temp1.differentiateX();
    jacobian_(0, 1) = temp2.differentiateY();
    jacobian_(1, 0) = temp3.differentiateX();
    jacobian_(1, 1) = temp4.differentiateY();
    debug_ = false;
  }

  // Inner type definition
  typedef BoxT<T> Box;
  typedef IntervalT<T> Interval;

  /*
  // This is essentially th C0 test for F=(f, g) on a box B
  bool Exclude(const Box *box) const {
    // 0 not in f(B) \/ 0 not in g(B)
    const Interval &x_range = box->x_range;
    const Interval &y_range = box->y_range;
    Interval c_f = fxy_.eval<Interval>(x_range, y_range);
    Interval c_g = gxy_.eval<Interval>(x_range, y_range);
    if(!c_f.zero() || !c_g.zero()) 
      return true;
    else
      return false;
  }
  */

  /*
  // This is essentially the C0 test for F=(f, g) on a box B
  // uses centered form to achieve quadratic convergence 
  bool Exclude(const Box *box) const {
    // o not in f(B) \/ 0 not in g(B)
    const Interval &x_range = box->x_range;
    const Interval &y_range = box->y_range;
    Interval c_f = eval2(fxy_, x_range, y_range);
    Interval c_g = eval2(gxy_, x_range, y_range);
    if(!c_f.zero() || !c_g.zero()) 
      return true;
    else
      return false;
  }
  */

  // This is essentially the C0 test for F=(f, g) on a box B
  // uses centered form to achieve quadratic convergence 
  bool Exclude(const Box *box) const {
    // o not in f(B) \/ 0 not in g(B)
    const Interval &x_range = box->x_range;
    const Interval &y_range = box->y_range;
    Interval c_f = fxy_.eval2(x_range, y_range);
    Interval c_g = gxy_.eval2(x_range, y_range);
    if(!c_f.zero() || !c_g.zero()) 
      return true;
    else
      return false;
  }

  // This is the Jacobian Test on a box B
  //   It returns true iff 0 is not in det(JF(B))
  //   If true, then B has at most one root.
  bool JTest(const Box *box) const {
   const Interval &x_range = box->x_range;
   const Interval &y_range = box->y_range;
   Interval a = jacobian_(0, 0).eval<Interval>(x_range, y_range);
   Interval b = jacobian_(0, 1).eval<Interval>(x_range, y_range);
   Interval c = jacobian_(1, 0).eval<Interval>(x_range, y_range);
   Interval d = jacobian_(1, 1).eval<Interval>(x_range, y_range);
   Interval det = a*d - b*c;
   if(det.zero())
     return false;
   else
     return true;
  }

  // MK test  (MK stands for Moore-Kioustelides)
  //    Given a box B, it returns true iff MK(B) holds.
  bool MKTest(const Box *box) const {
    const Interval &x_range = box->x_range;
    const Interval &y_range = box->y_range;
    const T &x_l = x_range.getL();
    const T &x_r = x_range.getR();
    const T &y_l = y_range.getL();
    const T &y_r = y_range.getR();
    const T &x_mid = x_range.mid();
    const T &y_mid = y_range.mid();
    // width of the box on x and y directions
    const T &w_x   = x_range.getR() - x_mid;
    const T &w_y   = y_range.getR() - y_mid;

    MatrixT<T> j_center(2); // JF(m(B))
    int sign = JSign(&j_center, box);
    // if det = 0, fail
    if(sign == 0) {
//cout << "sign = 0" << endl;
      return false;
    }
    /*================== MK prediate No.1 ================*/
    /*       
              +x2
           _________
          |    |    |       a = fx(m)   b = fy(m)
      -x1 |____|____| +x1   
          |    |    |       c = gx(m)   d = gy(m)
          |____|____|
              -x2
    */
    T f_left_cen  = fxy_.eval<T>(x_l, y_mid); // f(m(-x1))
    T g_left_cen  = gxy_.eval<T>(x_l, y_mid); // g(m(-x1))
    T f_right_cen = fxy_.eval<T>(x_r, y_mid); // f(m(+x1))
    T g_right_cen = gxy_.eval<T>(x_r, y_mid); // g(m(+x1))

    // ^f- = d*f(m(-x1)) - b*g(m(-x1))
    T f_left  = j_center(1, 1)*f_left_cen -
      j_center(0, 1)*g_left_cen;
    // ^f+ = d*f(m(+x1)) - b*g(m(+x1))
    T f_right = j_center(1, 1)*f_right_cen -
      j_center(0, 1)*g_right_cen;

    T f_down_cen = fxy_.eval<T>(x_mid, y_l);  // f(m(-x2))
    T g_down_cen = gxy_.eval<T>(x_mid, y_l);  // g(m(-x2))
    T f_up_cen   = fxy_.eval<T>(x_mid, y_r);  // f(m(+x2))
    T g_up_cen   = gxy_.eval<T>(x_mid, y_r);  // g(m(+x2))

    // ^g- = a*g(m(-x2)) - c*f(m(-x2))
    T g_left = j_center(0, 0)*g_down_cen - 
      j_center(1, 0)*f_down_cen;
    // ^g+ = a*g(m(+x2)) - c*f(m(+x2))
    T g_right = j_center(0, 0)*g_up_cen - 
      j_center(1, 0)*f_up_cen;

    // print debug information
    if(debug_) {
      cout << "^f- = " << f_left << endl;
      cout << "^f+ = " << f_right << endl;
      cout << "^g- = " << g_left << endl;
      cout << "^g+ = " << g_right << endl;
    }
    // ^f- and ^f+ same sign || ^g- and ^g+ same sign
    if((f_left * f_right >= 0) || (g_left * g_right >= 0))
      return false;

    /*================== MK prediate No.2 ================*/

    Interval fy_l = jacobian_(0, 1).eval<Interval>(
      Interval(x_l), y_range);  // fy(-x1)
    Interval gy_l = jacobian_(1, 1).eval<Interval>(
      Interval(x_l), y_range);  // gy(-x1)
    // ^fy(-x1) = d*fy(-x1) - b*gy(-x1)
    Interval fy_left = fy_l*j_center(1, 1) - gy_l*j_center(0, 1);

    Interval fy_r = jacobian_(0, 1).eval<Interval>(
      Interval(x_r), y_range);  // fy(+x1)
    Interval gy_r = jacobian_(1, 1).eval<Interval>(
      Interval(x_r), y_range);  // gy(+x1)
    // ^fy(+x1) = d*fy(+x1) - b*gy(+x1)
    Interval fy_right = fy_r*j_center(1, 1) - gy_r*j_center(0, 1);

    Interval gx_l = jacobian_(1, 0).eval<Interval>(
      x_range, Interval(y_l));  // gx(-x2)
    Interval fx_l = jacobian_(0, 0).eval<Interval>(
      x_range, Interval(y_l));  // fx(-x2)
    // ^gx(-x1) = a*gx(-x2) - c*fx(-x2)
    Interval gx_left = gx_l*j_center(0, 0) - fx_l*j_center(1, 0);

    Interval gx_r = jacobian_(1, 0).eval<Interval>(
      x_range, Interval(y_r));  // gx(+x2)
    Interval fx_r = jacobian_(0, 0).eval<Interval>(
      x_range, Interval(y_r));  // gx(+x2)
    // ^gx(+x1) = a*gx(+x2) - c*fx(+x2)
    Interval gx_right = gx_r*j_center(0, 0) - fx_r*j_center(1, 0);
    
    if(debug_) {
      cout << "mag(^fy(-x1)) = " << Mag(fy_left) << endl;
      cout << "mag(^fy(+x1)) = " << Mag(fy_right) << endl;
      cout << "mag(^gx(-x1)) = " << Mag(gx_left) << endl;
      cout << "mag(^gx(+x1)) = " << Mag(gx_right) << endl;
    }

    if((absolute(f_left)  <= Mag(fy_left)*w_x)  ||
       (absolute(f_right) <= Mag(fy_right)*w_x) || 
       (absolute(g_left)  <= Mag(gx_left)*w_y)  || 
       (absolute(g_right) <= Mag(gx_right)*w_y)  )
      return false;


    return true;
  }

  // compute JF(m(B)), m(B) is the center of box, and return the sign of det(JF(m(B)))
  int JSign(MatrixT<T> *output,
    const Box *box) const {
    // construct mid point for x and y
    const T &x_mid = box->x_mid();
    const T &y_mid = box->y_mid();
    // keep things handy
    MatrixT<T> &temp = *output;
    // evaluate at center of the box
    temp(0, 0) = jacobian_(0, 0).eval<T>(x_mid, y_mid);
    temp(0, 1) = jacobian_(0, 1).eval<T>(x_mid, y_mid);
    temp(1, 0) = jacobian_(1, 0).eval<T>(x_mid, y_mid);
    temp(1, 1) = jacobian_(1, 1).eval<T>(x_mid, y_mid);
    T det = temp(0, 0)*temp(1, 1) - temp(0, 1)*temp(1, 0);
    if(det > 0) 
      return 1;
    else if(det == 0)
      return 0;
    else
      return -1;
  }

  // check region size and generation, put into ambiguous if too small
  bool Min(const Box *region) const {
    if(region->x_range.width() <= min_size_ || 
        region->y_range.width() <= min_size_)
      return true;
    else if(region->generation_id > max_gen_id_)
      return true;
    else
      return false;
  }

  // return true if output box is too big 
  bool Max(const Box *region) const {
    if(((region->x_range.width() >= max_size_) || 
        (region->y_range.width() >= max_size_)) && 
        (region->generation_id < max_gen_id_))
   /* if((region->x_range.width() >= max_size_) || 
        (region->y_range.width() >= max_size_))*/
      return true;
    else
      return false;
  }

  // get absolute value(not sure if core implements this)
  T absolute(const T &input) const {
    if(input <= 0)
      return (T(-1) * input);
    else
      return input;
  }

  // calculate magnitude of interval(should be put into library)
  T Mag(const Interval &interval) const {
    T temp_left = interval.getL();
    T temp_right = interval.getR();
    if(temp_left <= 0)  temp_left  = -temp_left;
    if(temp_right <= 0) temp_right = -temp_right;
    return ((temp_left>=temp_right) ? temp_left : temp_right);
  }

  /// Split(Box B, Queue Q)
  /// 	splits B to 4 children, and put them into the queue Q
  void Split(const Box *region,
      vector<const Box*> *queue) const {
    const T &x_start = region->x_range.getL();
    const T &x_end   = region->x_range.getR();
    const T &x_mid   = region->x_range.mid();
    const T &y_start = region->y_range.getL();
    const T &y_end   = region->y_range.getR();
    const T &y_mid   = region->y_range.mid();

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
    delete region;
  }

  /// Split_Exclude(Box B, Queue Q, Queue ExcludeQ)
  ///   Splits B into 4 children, and push each child into Q if it fails C0 test
  ///	and push into ExcludeQ if C0 holds.
  void Split_Exclude(const Box *region,
    vector<const Box *> *queue,
    vector<const Box *> *exclude) const {

    // temp queue for processing
    vector<const Box *> temp;
    // split box into 4 children 
    Split(region, &temp);
    // Do C0 test for each box in temp queue
    while(!temp.empty()) {
      const Box *box = temp.back();
      temp.pop_back();
      if(!Exclude(box)) {  // C0 fails, put back to queue
        queue->push_back(box);
      }
      else {
          exclude->push_back(box);
      }
    }
  }

  /// Cover(Box B, Queue Q)
  ///   Splits B into 9 regions and push them into Q.
  void Cover(const Box *region,
      vector<const Box*> *queue) const {
    const T &x_start = region->x_range.getL();
    const T &x_end   = region->x_range.getR();
    const T &x_mid   = region->x_range.mid();
    const T &y_start = region->y_range.getL();
    const T &y_end   = region->y_range.getR();
    const T &y_mid   = region->y_range.mid();

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
   
    // next five subregions:

    const T &x_halfwidth  = region->x_range.width()/4;
    const T &y_halfwidth  = region->y_range.width()/4;

    queue->push_back(new Box(gen_id,    // q_1/2
      Interval(x_mid, x_end), Interval(y_mid-y_halfwidth, y_mid+y_halfwidth)));
    queue->push_back(new Box(gen_id,    // q_5/2
      Interval(x_start, x_mid), Interval(y_mid-y_halfwidth, y_mid+y_halfwidth)));
    queue->push_back(new Box(gen_id,    // q_3/2
      Interval(x_mid-x_halfwidth, x_mid+x_halfwidth), Interval(y_mid, y_end)));
    queue->push_back(new Box(gen_id,    // q_7/2
      Interval(x_mid-x_halfwidth, x_mid+x_halfwidth), Interval(y_start, y_mid)));
    queue->push_back(new Box(gen_id,    // q_0
      Interval(x_mid-x_halfwidth, x_mid+x_halfwidth),
      Interval(y_mid-y_halfwidth, y_mid+y_halfwidth)));

    delete region;
  }

  /// Cover_Exclude(Box B, Queue Q, Queue ExcludeQ)
  ///   Splits B into 9 regions and push each child into Q (if it fails C0)
  ///	and push into Exclude Q (if it passes C0)
  void Cover_Exclude(const Box *region,
    vector<const Box *> *queue,
    vector<const Box *> *exclude) const {

    // temp queue for processing
    vector<const Box *> temp;
    // cover box into 9 children 
    Cover(region, &temp);
    // Do C0 test for each box in temp queue
    while(!temp.empty()) {
      const Box *box = temp.back();
      temp.pop_back();
      if(!Exclude(box)) {  // C0 fails, put back to queue
        queue->push_back(box);
      }
      else {
          exclude->push_back(box);
      }
    }
  }//Cover_Exclude

  /// Refinement(Box B, Queue Q-exc)
  ///	Box B is guaranteed to contain a root in its interior (i.e., it is a "root box")
  /// 	This calls Cover_Exclude repeatedly to generate 9 subboxes.
  /// 	Root refinement will return another root box BB contained in B.
  ///		Now, if BB=B, then the refinement has "failed" in some sense.
  ///		And the only reason it fails it because it has to split boxes
  ///		smaller than the global Min_size (it is a safe-ty feature).  
  ///   We can split the annulus region B - BB into 8 subboxes, and these
  ///		are placed into Q-exc.
  Box Refinement(const Box *B,
     vector<const Box *> *Qexclude) const
  {
    vector<const Box *> Qtmp;
    // NOTE: The next statement is commented out because we do not
    // 		want Max to limit our intention to do refinement here.
    //
    // if the original already smaller than the max_size, output it
    // if(!Max(B)) {
    //	  output->push_back(B);
    // }
      // Return box is called BB:
      Box *BB = B;    // initialize BB
      // split the box which already passed MK test but too big 
      Cover_Exclude(B, &Qtmp, Qexclude);
      // loop started
      while(true) {
        const Box *box = Qtmp.back(); 	// this queue can never be empty according to our theory
        Qtmp.pop_back();
        // too small
        if(Min(box)) {
          // ambiguous->push_back(box);  // discard this box!  It is wrong to put it into the global ambigous
          continue;
        }
        // if box satisfies MK test, and small enough, put into output
        // else keep splitting
        if(MKTest(box) && !Max(box)) {
          BB= box;
	  Split_Complement(B, BB, Qexclude)
          break;
        }
        else {
          Cover_Exclude(box, &Qtmp, Qexclude);
        }
      }
      return BB;
  }

 /*
 // eval1
 Interval eval1(const Curve<T> &poly, 
     const Interval &x_range, const Interval &y_range) const {
   Curve<T> poly_x = poly, poly_y = poly;
   poly_x.differentiateX();
   poly_y.differentiateY();

   const T &xmid = x_range.mid();
   const T &ymid = y_range.mid();
   const Interval I_m = x_range - Interval(xmid);
   const Interval J_m = y_range - Interval(ymid);
   Interval f_m = poly.eval<Interval>(Interval(xmid), Interval(ymid));
   Interval fx_term = poly_x.eval<Interval>(x_range, y_range) * I_m;
   Interval fy_term = poly_y.eval<Interval>(x_range, y_range) * J_m;

   return (f_m + fx_term + fy_term);
 }

 // eval2
 Interval eval2(const Curve<T> &poly, 
     const Interval &x_range, const Interval &y_range) const {
   Curve<T> poly_x = poly, poly_y = poly;
   poly_x.differentiateX();
   poly_y.differentiateY();
   const T &xmid = x_range.mid();
   const T &ymid = y_range.mid();
   const Interval I_m = x_range - Interval(xmid);
   const Interval J_m = y_range - Interval(ymid);

   Interval f_m = poly.eval<Interval>(Interval(xmid), Interval(ymid));
   Interval fx_term = eval1(poly_x, x_range, y_range) * I_m;
   Interval fy_term = eval1(poly_y, x_range, y_range) * J_m;

   return (f_m + fx_term + fy_term);
 }
*/


private:
  Curve<T>           fxy_;       // f(x, y)
  Curve<T>           gxy_;       // g(x, y)
  MatrixT<Curve<T> > jacobian_;  // jacobian matrix of F = (f, g)
  const T            min_size_;  // minimum size of a box
  const T            max_size_;  // if output box greater that max, keep splitting
  const unsigned int max_gen_id_;// box generation
  bool               debug_;     // debug bit
};

#endif
