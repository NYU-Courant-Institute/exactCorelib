/*
 * =====================================================================================
 *
 *       Filename:  rootvector.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/05/2013 09:51:29 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"config.h"
#include	"pointlist.h"

#ifndef  POINTVECTOR_INC
#define  POINTVECTOR_INC


struct pointVector {

	void** data;
	int capacity;
	int size;
	void (*delfun)(void *);

	
};				/* ----------  end of struct rootVector  ---------- */

typedef struct pointVector PointVector;

PointVector* createVector(void (*)(void *));

void reScale_V(PointVector *);
void delVector(PointVector *);
void push_back_V(PointVector* , void * );
void
sortVector ( PointVector * p, int (*com)(void *, void*) ) ;
int size_V(const PointVector *);
void* at_V(const PointVector *,const int n );


void insertBack_V(PointVector* lhs,const  PointVector* rhs);

void clear_V(PointVector *);

void v_insertBack_L(PointVector *vec, const PointList *list); 
#endif   /* ----- #ifndef POINTVECTOR_INC  ----- */
