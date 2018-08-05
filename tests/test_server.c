/**
 * Tests for server.c.
 */
#include "CUnit/Basic.h"
#include <signal.h>
#include "server.h"


/**
 * Test suit initialization function.
 * @return 0
 */
int init_suite();


/**
 * Test suit finalization function.
 * @return 0
 */
int clean_suite();


/**
 * Test of function "init_server".
 * If init_server() returns the completed structure, it's ok.
 */
void test_init_server();

/**
 * Test of function "loop_recv".
 * If loop_recv() returns -1, it's ok.
 */
void test_loop_recv();

/**
 * Test of function "error_handler".
 * If error_handler() returns 0, it's ok.
 */
void test_error_handler();

/**
 * Test of function "signal_handler".
 * If signal_handler() returns 0 or -1, it's ok.
 */
void test_signal_handler();

/**
 * Test of function "finalize".
 * If finalize() returns 0, it's ok.
 */
void test_finalize();


int main()
{
    CU_pSuite pSuite = NULL;

    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    pSuite = CU_add_suite("Suite", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite,
        "test of init_server()",
        test_init_server)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite,
        "test of loop_recv()",
        test_loop_recv)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite,
        "test of error_handler()",
        test_error_handler)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite,
        "test of signal_handler()",
        testt_signal_handler)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite,
        "test of finalize()",
        test_finalize)) {
        CU_cleanup_registry();
        return CU_get_error();
    }


    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}


int init_suite()
{
   return 0;
}


int clean_suite()
{
   return 0;
}


void test_init_server()
{
    struct server_conf conf;
    int wnum = 3,
        rnum = 5;
    char db_path[] = "./db";

    conf = init_server(db_path, wnum, rnum);

    CU_ASSERT(-1 != conf.socket);
    CU_ASSERT(-1 != conf.msgid);
    CU_ASSERT(wnum == cong.workers.size);
    CU_ASSERT(rnum == conf.rooms.rnum);
}


void test_loop_recv()
{
    struct server_conf conf;
    int wnum = 3,
        rnum = 5;
    char db_path[] = "./db";

    conf = init_server(db_path, wnum, rnum);

    CU_ASSERT(-1 == loop_recv(conf.socket, -1));
    CU_ASSERT(-1 == loop_recv(-1, conf.msgid));
}


void test_error_handler()
{
    int errno = -1;
    CU_ASSERT(0 == error_handler(errno));
}


void test_signal_handler()
{
    CU_ASSERT(0 == signal_handler(SIGINT));
    CU_ASSERT(0 == signal_handler(SIGTERM));
    CU_ASSERT(-1 == signal_handler(SIGPIPE));
}


void test_finalize()
{
    struct server_conf conf;
    int wnum = 3,
        rnum = 5;
    char db_path[] = "./db";

    conf = init_server(db_path, wnum, rnum);

    CU_ASSERT(0 == finalize(&conf));
}
