/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/05/2013 09:33:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	"lcf.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  test1
 *  Description: meory bug 
 * =====================================================================================
 */
void
test1 ( void ){
	char *str="28,-12,-57,-97,-80";
	int exp[]={5, 4, 3, 2, 1};

	int num;
	printf ( "%s \n",lcf(5,exp, str,&num  ,"1") );

}		/* -----  end of function test1  ----- */
/*-----------------------------------------------------------------------------
 *oneSepZeros bug
 *-----------------------------------------------------------------------------*/
void test2(void){
	char *str="87,26,18,-15,-23,-6";
	int exp[]={5, 4, 3, 2, 1,0};

	int num;
	printf ( "%s \n",lcf(6,exp, str,&num  ,"1"));

	
}
/*-----------------------------------------------------------------------------
 *  
 *-----------------------------------------------------------------------------*/
void test3(void ){
	printf ( "test3\n" );
	char *str="-35,16,-23,4,78";
	int exp[]={83, 63, 47, 25, 1};

	int num;
	printf ( "%s \n",lcf(5,exp, str,&num  ,"1"));

}

void test4(void){
	printf ( "test4\n" );
	char *str="-50,42,-33,27,-16,-40";
	int exp[]={90, 88, 56, 42, 34,0};
	int num;
	printf ( "%s \n",lcf(6,exp, str,&num ,"1"));

}
void test5(void){
	printf ( "test5\n" );
	char *str="-17,-75,-10,-7,-40,42";
	int exp[]={5,4,3,2,1,0};

	int num;
	printf ( "%s \n",lcf(6,exp, str,&num ,"1/2222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222"));
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int
main ( int argc, char *argv[] ){
//	test1();
//	test2();
//	test3();
//	test4();
//	test5();
//	createSearchPoly2f("test.txt");
//

	if(argc<2)
	  printf ( "add data file\n" );
	lcfbyf(argv[1],"0");
	return 0;
    
	char *str="32,-48,18,-1";
	int exp[]={6, 4, 2, 0};
	int num;
    // lcfbyf("test.txt","1");
    
     printf ( "%s \n",lcf(4,exp, str,&num,"1" ));
	clearVars();
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
