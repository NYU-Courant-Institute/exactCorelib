/*
 * =====================================================================================
 *
 *       Filename:  myassert.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  04/22/2013 11:28:12 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include    <stdio.h>
#include    <time.h>

#ifndef  MYASSERT_INC
#define  MYASSERT_INC

#ifdef DEBUG

#define RETURN_IF_FAIL(expr)      do {                                  \
        if (!(expr))                                                    \
        {                                                               \
            fprintf(stderr,                                             \
                    "file %s: line %d (%s): precondition `%s' failed.", \
                    __FILE__,                                           \
                    __LINE__,                                           \
                    __PRETTY_FUNCTION__,                                \
                    #expr);                                             \
            print_stack_trace(2);                                       \
            return;                                                     \
        };               } while(0)
#define RETURN_VAL_IF_FAIL(expr, val)  do {                             \
        if (!(expr))                                                    \
        {                                                               \
            fprintf(stderr,                                             \
                    "file %s: line %d (%s): precondition `%s' failed.", \
                    __FILE__,                                           \
                    __LINE__,                                           \
                    __PRETTY_FUNCTION__,                                \
                    #expr);                                             \
            print_stack_trace(2);                                       \
            return val;                                                 \
        };               } while(0)


#define PRINT_IF_FAIL(expr)      do {                                   \
        if (!(expr))                                                    \
        {                                                               \
            time_t rawtime;                                             \
            struct tm * timeinfo;                                       \
            time (&rawtime);                                            \
            timeinfo = localtime (&rawtime);                            \
            fprintf(stderr,                                             \
                    "%s: file %s: line %d (%s): precondition `%s' failed.", \
                    asctime(timeinfo),                                  \
                    __FILE__,                                           \
                    __LINE__,                                           \
                    __PRETTY_FUNCTION__,                                \
                    #expr);                                             \
            fflush(stderr);                                             \
            print_stack_trace(2);                                       \
        };               } while(0)


#define PRINT_IF_TRUE(expr)      do {                                   \
        if ((expr))                                                     \
        {                                                               \
            time_t rawtime;                                             \
            struct tm * timeinfo;                                       \
            time (&rawtime);                                            \
            timeinfo = localtime (&rawtime);                            \
            fprintf(stderr,                                             \
                    "%s: file %s: line %d (%s): precondition `%s' failed.", \
                    asctime(timeinfo),                                  \
                    __FILE__,                                           \
                    __LINE__,                                           \
                    __PRETTY_FUNCTION__,                                \
                    #expr);                                             \
            fflush(stderr);                                             \
            print_stack_trace(2);                                       \
        };               } while(0)



#define PRINT_VALUE(expr)      do {                                     \
        time_t rawtime;                                                 \
        struct tm * timeinfo;                                           \
        time (&rawtime);                                                \
        timeinfo = localtime (&rawtime);                                \
        fprintf(stderr,                                                 \
                "%s: file %s: line %d (%s): precondition `%s' failed. %s", \
                asctime(timeinfo),                                      \
                __FILE__,                                               \
                __LINE__,                                               \
                __PRETTY_FUNCTION__,                                    \
                #expr,expr);                                            \
        fflush(stderr);                                                 \
        print_stack_trace(2);                                           \
    } while(0)

#else


#define RETURN_IF_FAIL(expr)
#define RETURN_VAL_IF_FAIL(expr, val)
#define PRINT_IF_FAIL(expr)
#define PRINT_IF_TRUE(expr)
#define PRINT_VALUE(expr)

#endif

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif

    void print_stack_trace(int fd);

#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif

#endif   /* ----- #ifndef MYASSERT_INC  ----- */
