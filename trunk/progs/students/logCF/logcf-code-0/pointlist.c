/*
 * =====================================================================================
 *
 *       Filename:  pointlist.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/04/2013 08:48:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"pointlist.h"
#include "memutil.h"

#include	<stdio.h>

#include	<stdlib.h>
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  createElem
 *  Description:  
 * =====================================================================================
 */
PointElem*
createElem ( void *root ){
	PointElem *re;

	re	=(PointElem*) malloc_d ( sizeof(PointElem) );
	
	re->value=root;
	re->next=NULL;
	return re;
	

}		/* -----  end of function createElem  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  setValue
 *  Description:  
 * =====================================================================================
 */
void
setValue ( PointElem *elem, void* root ){

	if(elem->value!=NULL)
	  delRoot(elem->value);
	elem->value=root;

}		/* -----  end of function setValue  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  delElem
 *  Description:  
 * =====================================================================================
 */
void
delElem ( PointElem *elem, void (*delfun)(void *) ){
	if(elem!=NULL){
		if(elem->value!=NULL) delfun(elem->value);
		free( elem);
	}

}		/* -----  end of function delElem  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  clear_E
 *  Description:  
 * =====================================================================================
 */
void
clear_E ( PointElem *elem ){
	if(elem!=NULL){
		free( elem);
	}

}		/* -----  end of function clear_E  ----- */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  createList
 *  Description:  
 * =====================================================================================
 */
PointList*
createList (void (* del)(void *)  ){
	PointList* re;

	re	=(PointList *) malloc_d ( sizeof(PointList) );
	
	re->head=NULL;
	re->tail=NULL;
	re->delfun=del;
	return re;
}		/* -----  end of function createList  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  delList
 *  Description:  
 * =====================================================================================
 */
void
delList ( PointList *list ){
	if(list!=NULL){
		PointElem *temp=list->head;
		PointElem *temp1;
		while(NULL!=temp){
			temp1=temp->next;
			 delElem(temp,list->delfun );
			 temp=temp1;
		}
		if(temp!=NULL) delElem(temp,list->delfun );
		free(list);
	}

}		/* -----  end of function delList  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  isEmpty
 *  Description:  
 * =====================================================================================
 */
int
isEmpty_L ( PointList *list ){

	return (NULL== list->head);
}		/* -----  end of function isEmpty  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  front
 *  Description:  
 * =====================================================================================
 */
void*
front_L (PointList *list  ){
	if(list->head!=NULL)
	  return list->head->value; 
	return NULL;
}		/* -----  end of function front  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  pop_front
 *  Description:  do not delete value
 * =====================================================================================
 */
void
pop_front_L (PointList *list  ){

	if(list->head!=NULL){
		if(list->head==list->tail){
			free(list->head);
			list->head=list->tail=NULL;
			return ;
		}
		else{
			PointElem *temp= list->head;
			list->head=list->head->next;
			free(temp);

		}
	}

}		/* -----  end of function pop_front  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  push_back
 *  Description:  
 * =====================================================================================
 */
void
push_back_L ( PointList *list, void *root ){

	PointElem *elem=createElem(root);
	if(NULL==list->head){                       /* empty */
		list->head=elem;
		list->tail=elem;
	}else{
		list->tail->next=elem;
		list->tail=elem;
	}

}		/* -----  end of function push_back  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  push_front_L
 *  Description:  
 * =====================================================================================
 */
void
push_front_L ( PointList * list, void * v ){
	
	PointElem *elem=createElem(v);
	if(NULL==list->head){                       /* empty */
		list->head=elem;
		list->tail=elem;
	}else{
		elem->next=list->head;
		list->head=elem;
	}

}		/* -----  end of function push_front_L  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sortList
 *  Description: O(n^2) 
 * =====================================================================================
 */
void
sortList ( PointList *list, int (*com) (void *, void* ) ){

	if(list->head==list->tail) return ;

	PointElem *temp;
	PointElem *temp1;
	PointElem *temp2;

	/*-----------------------------------------------------------------------------
	 *  insert sort
	 *-----------------------------------------------------------------------------*/
	temp=list->head;

	while(NULL!= temp->next){

		/*-----------------------------------------------------------------------------
		 *  temp1 isolate
		 *-----------------------------------------------------------------------------*/
		temp1=temp->next;
//		temp->next=temp1->next;             /* temp-> ( jump(temp1) temp->next) ->temp1->next */
//		temp=temp->next;	
//
		if((*com)(temp1->value,list->head->value)<=0){
			temp->next=temp1->next;
			temp1->next=list->head;
			list->head=temp1;

		}else{

			temp2=list->head;
			while(temp2->next!=temp1 && (*com) (temp1->value, temp2->next->value)>0)
			  temp2=temp2->next;

			if(temp2->next!=temp1){

				temp->next=temp1->next;
				temp1->next=temp2->next;
				temp2->next=temp1;

			}else{
				temp=temp->next;
			}
		}

	}
	list->tail=temp;
	list->tail->next=NULL;

}		/* -----  end of function sortList  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  clear_L
 *  Description:  
 * =====================================================================================
 */
void
clear_L ( PointList *list ){
	if(list!=NULL){

		PointElem *temp=list->head;
		PointElem *temp1;
		while(NULL!=temp){
			temp1=temp->next;
			clear_E(temp);
			temp=temp1;
		}
		list->head=list->tail=NULL;

	}

}		/* -----  end of function clear_L  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  size_L
 *  Description:  
 * =====================================================================================
 */
int 
size_L ( PointList *list ){

	int re=0;

	if(list->head==NULL) return re;
	PointElem *elem=list->head;
	while(NULL!= elem){
		re++;
		elem=elem->next;
	}
	return re ;
}		/* -----  end of function size_L  ----- */


