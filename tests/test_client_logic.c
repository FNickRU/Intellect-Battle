/**
 * Tests for test.c.
 */
#include "CUnit/Basic.h"
#include <string.h>
#include "client_logic.h"

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
 * Test of connect function. Check socket.
 */
void test_connect();

/**
 * Test of send_conf function. Check type.
 */
void test_send_conf_type();

/**
 * Test of wait_for_players function. Check room is not NULL.
 */
void test_wait_for_players_room();

/**
 * Test of get_unit function. Check unit's pointer.
 */
void test_get_unit_u();

/**
 * Test of get_unit function. Check room's pointer.
 */
void test_get_unit_room();

/**
 * Test of send_ans function. Check answer.
 */
void test_send_ans_ans1();
void test_send_ans_ans2();

/**
 * Test of is_loser function.
 */
void test_is_loser_true();

/**
 * Test of is_loser function.
 */
void test_is_loser_false();


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
        "test of connect() [socket]",
        test_connect)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite,
        "test of send_conf() [type]",
        test_send_conf_type)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite,
        "test of wait_for_players() [room]",
        test_wait_for_players_room)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite,
        "test of get_unit() [unit]",
        test_get_unit_u)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite,
        "test of get_unit() [room]",
        test_get_unit_room)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite,
        "test of send_ans() [answer, 1]",
        test_send_ans_ans1)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite,
        "test of send_ans() [answer, 2]",
        test_send_ans_ans2)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite,
        "test of is_loser() [true]",
        test_is_loser_true)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite,
        "test of is_loser() [false]",
        test_is_loser_false)) {
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

void test_connect()
{
    memset(&Client_info, 0, sizeof(Client_info));
    Client_info.socket = -1;
    CU_ASSERT(0 != connect());
}

void test_send_conf_type()
{
    CU_ASSERT(0 != send_conf(-1, 3));
}

void test_wait_for_players_room()
{
    CU_ASSERT(-1 == wait_for_players(NULL));
}

void test_get_unit_u()
{
    struct room_info room;
    CU_ASSERT(-1 == get_unit(NULL, &room));
}

void test_get_unit_room()
{
    struct unit u;
    CU_ASSERT(-1 == get_unit(&u, NULL));
}

void test_send_ans_ans1()
{
    CU_ASSERT(0 != send_ans(-1));
}

void test_send_ans_ans2()
{
    CU_ASSERT(0 != send_ans(10));
}

void test_is_loser_true()
{
    struct room_info room;
    room.occupancy = 2;
    room.room_size = 2;
    room.id = 1;
    strncpy(room.usernames[0], "user1", 6);
    strncpy(room.usernames[1], "user2", 6);
    room.score[0] = -1;
    room.score[1] = 2;
    CU_ASSERT(1 == is_loser(room));
}

void test_is_loser_false()
{
    struct room_info room;
    room.occupancy = 2;
    room.room_size = 2;
    room.id = 1;
    strncpy(room.usernames[0], "user1", 6);
    strncpy(room.usernames[1], "user2", 6);
    room.score[0] = 2;
    room.score[1] = -2;
    CU_ASSERT(0 == is_loser(room));
}
