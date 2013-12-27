/*
 * =====================================================================================
 *
 *       Filename:  lcf.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  01/05/2013 09:27:28 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include    <gmp.h>
#include    "root.h"
#include    "pointlist.h"
#include    "pointvector.h"
#include    "pointstack.h"
#include    <stdio.h>
#include    <assert.h>
#include    <stdlib.h>
#ifndef  LCF_INC
#define  LCF_INC

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif

    void initVars();


    void clearVars();

    int readMpz(mpz_t v,const  char *str, int * start);


/*void nroot_1(mpq_t re, mpq_t b, unsigned long used, unsigned long  int n);*/


/*void nroot_2(mpz_t re, mpz_t b, unsigned long int used, unsigned long int n);

//void nroot_3(mpq_t re, mpq_t  initRoot, unsigned long int aused, unsigned long int n1,
//          mpz_t  b, unsigned long int bused, unsigned long int n2, mpz_t *c);
*/

    typedef  mpz_t* MPZ_P ;
    struct SearchPoly {

        MPZ_P coeff;
        mpz_t a,b,c,d;
        int size;
        int deg;
        int ch;
        int times;
        int *index;
        int *sign;

    };              /* ----------  end of struct SearchPoly  ---------- */

    typedef struct SearchPoly SearchPoly;


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  copySearchPoly
 *  Description:
 * =====================================================================================
 */
    SearchPoly*
    copySearchPoly (const  SearchPoly * );


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  SearchPoly
 *  Description:
 * =====================================================================================
 */
    SearchPoly*
    createSearchPoly (const int n, const int* exp, const char * cf  );



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  delSearchPoly
 *  Description:
 * =====================================================================================
 */
    void
    delSearchPoly ( SearchPoly * );

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  printPoly
 *  Description:
 * =====================================================================================
 */
    void
    printPoly (const SearchPoly *p );


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  oneSepZeros
 *  Description:
 * =====================================================================================
 */
    int
    oneSepZeros (const  SearchPoly* );


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  hasPosRoot
 *  Description:
 * =====================================================================================
 */
    int
    hasPosRoot (const  SearchPoly * );

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  mapOne
 *  Description:
 * =====================================================================================
 */
    void
    mapOne ( SearchPoly * );

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  stepOne
 *  Description:
 * =====================================================================================
 */
    void
    stepOne (SearchPoly *  );

    void negOrder(SearchPoly * );

    void decrease(SearchPoly *);

    void negsubs(SearchPoly *);

    int  value(const SearchPoly *, const mpq_t v);

/*int getDeg(SearchPoly *);*/

    int hasZeroRoot(const SearchPoly *);

    int isRoot( SearchPoly*,  const unsigned long int value);

    Root * zeroRoot(const SearchPoly *);

    Root* root_1(const SearchPoly * );

    Root* root_2(const SearchPoly *,const  unsigned long int );

    void
    root (mpq_t re, mpq_t q, unsigned long int mul, unsigned long int n, int r );

    void
    pow_Q (mpq_t re, mpq_t q,unsigned long int n );


    Root* leftOneRoot( SearchPoly *);

    void subsAdd( SearchPoly*, mpz_t z,const  int shift);

    void subsAddOne(SearchPoly*);

    void update(SearchPoly *);

    void delFac_1(SearchPoly* , const unsigned long int fac );

    void delFac_2(SearchPoly*, const mpq_t v);

    void subsTimes(SearchPoly *,const mpz_t m, const int shift);

    void subsDiv(SearchPoly *,const  mpz_t d);

    int lmt_lb(SearchPoly *);
    int lmt_up(const SearchPoly *);

    void kiou_up( const SearchPoly *);

    int getChange(SearchPoly *);

    int getTimes(SearchPoly *);

    void vas_cont(PointList *roots, PointVector *ratRoot, SearchPoly *);


    CLOGLIB char * lcf( const int n, const int *exp, const char * cf ,int * ,const char *);
    CLOGLIB void lcfbyf(const char* file, const char* preStr);
    

#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif
#endif   /* ----- #ifndef LCF_INC  ----- */
