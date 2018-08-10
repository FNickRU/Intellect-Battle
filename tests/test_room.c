/**
 * Tests for room.c.
 */
#include <string.h>
#include "CUnit/Basic.h"
#include "room.h"

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
 * Test of sendto_user function. Check data is not NULL.
 */
void test_sendto_user_data();


/**
 * Test of sendto_user function. Check user's socket.
 */
void test_sendto_user_socket();


/**
 * Test of recvfrom_user function. Check data is not NULL.
 */
void test_recvfrom_user_data();


/**
 * Test of recvfrom_user function. Check user's socket.
 */
void test_recvfrom_user_socket();


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
        "test of sendto_user() [data]",
        test_sendto_user_data)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite,
        "test of sendto_user() [socket]",
        test_sendto_user_socket)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite,
        "test of recvfrom_user() [data]",
        test_recvfrom_user_data)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite,
        "test of recvfrom_user() [socket]",
        test_recvfrom_user_socket)) {
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

void test_sendto_user_data()
{
    struct player user;
    char *data = NULL;
    unsigned int size = 2;
    CU_ASSERT(0 != sendto_user(user, (void *) data, size));
}

void test_sendto_user_socket()
{
    struct player user;
    user.socket = 0;
    char data[] = "NULL";
    unsigned int size = 5;
    CU_ASSERT(0 != sendto_user(user, (void *) data, size));
}

void test_recvfrom_user_data()
{
    struct player user;
    char *data = NULL;
    unsigned int size = 2;
    CU_ASSERT(0 != recvfrom_user(user, (void *) data, size));
}

void test_recvfrom_user_socket()
{
    struct player user;
    user.socket = 0;
    char data[] = "NULL";
    unsigned int size = 5;
    CU_ASSERT(0 != recvfrom_user(user, (void *) data, size));
}
