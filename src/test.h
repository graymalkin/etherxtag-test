
#define PASS 1
#define FAIL 0
#define TEST_NAME_PREFIX "  - "

/**
 * Unit test define. You need to provide local variables `count` and `tot`
 * It may then be used as:
 * RUN_TEST("Test name", test_function())
 *
 * Where test_function() returns 1 for sucess and 0 for failiure (defined as 
 * PASS and FAIL above)
 */
#define RUN_TEST(name, x) printf(TEST_NAME_PREFIX "%-60s :: ", name); fflush(stdout);\
        tot++; count += x ? 1 + (printf("PASS\n")*0) : 0 +(printf("FAIL\n")*0);

// Print test results.
#define PRINT_TEST_RESULTS                              \
    printf("\n==================\n");                   \
    printf("Passed Tests  : %d\n", count);              \
    printf("Total Tests   : %d\n", tot);

extern int quiet_mode;
extern int no_spinner;

#ifndef __test_h_s
#define __test_h_


#endif //__test_h_