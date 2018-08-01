#include "../src/test.h"
#include "CUnit/Basic.h"

int init_suite()
{
   return 0;
}

int clean_suite()
{
   return 0;
}

void testPROCESS_ARGUMENTS()
{
    int argc = 1;
    char *argv[] = {"out"};
    CU_ASSERT(0 == process_arguments(argc, argv));
}

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
        "test of process_arguments()",
        testPROCESS_ARGUMENTS)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
