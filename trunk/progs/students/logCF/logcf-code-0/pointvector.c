/*
 * =====================================================================================
 *
 *       Filename:  pointvector.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/05/2013 03:58:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"pointvector.h"

#include	"util.h"

#include "memutil.h"
#include	<stdlib.h>

#include	<stdio.h>
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  createVector
 *  Description:  
 * =====================================================================================
 */
PointVector*
createVector (void (* del)(void *)  ){
	PointVector* re;

	re	=(PointVector*) malloc_d ( sizeof(PointVector) );
	
	re->capacity=DEFAULT_CAP;
	
	re->data=(void **) malloc_d(re->capacity* sizeof(void *));
	
	re->size=-1;
	re->delfun=del;
	return re;
}		/* -----  end of function createVector  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  reScale_V
 *  Description:  
 * =====================================================================================
 */
void
reScale_V ( PointVector *p ){

	p->capacity*=RESCALE;
	p->capacity+=2;
	p->data	=(void **) realloc_d (p->data,p->capacity*sizeof(void*) );
	

}		/* -----  end of function reScale_V  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  delVector
 *  Description:  
 * =====================================================================================
 */
void
delVector ( PointVector *p ){
	int i;
	for ( i = 0; i < p->size; i += 1 ) {
		p->delfun(p->data[i]);

	}
	free(p->data);
	free(p);

}		/* -----  end of function delVector  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  push_back_V
 *  Description:  
 * =====================================================================================
 */
void
push_back_V ( PointVector *p , void * elem ){


	p->size++;
	if(p->size>=p->capacity){ 
		p->capacity=p->size+1;
		reScale_V(p);
	}

	p->data[p->size]=elem;

}		/* -----  end of function push_back_V  ----- */


void insertBack_V(PointVector * lhs, const PointVector *rhs){
	int i;

	for ( i = 0; i < rhs->size+1; i += 1 ) {
		push_back_V(lhs, rhs->data[i]);
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sort_V
 *  Description:  
 * =====================================================================================
 */
void
sortVector ( PointVector * p, int (*com)(void *, void*) ){
	qsort_d(p->data,0, p->size, com);

}		/* -----  end of function sort_V  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  size_V
 *  Description:  
 * =====================================================================================
 */
int
size_V (const  PointVector * p){
	return p->size+1;

}		/* -----  end of function size_V  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  at_V
 *  Description:  
 * =====================================================================================
 */
void*
at_V ( const PointVector *p, const int n ){

	return p->data[n];
}		/* -----  end of function at_V  ----- */

void clear_V(PointVector *vec){
	vec->size=-1;
/* 	int i;
 * 	for ( i = 0; i < vec->size; i += 1 ) {
 * 
 * 		vec->delfun(vec->data[i]);
 * 
 * 	}
 * 	vec->size=-1;
 */

}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  v_insertBack_L
 *  Description:  
 * =====================================================================================
 */
void
v_insertBack_L ( PointVector *vec, const PointList *list ){

	PointElem *elem=list->head;
	while(elem!=list->tail){
		push_back_V(vec, elem->value );
		elem=elem->next;
	}
	if(elem!=NULL){
		push_back_V(vec, elem->value );
	}


}		/* -----  end of function v_insertBack_L  ----- */
