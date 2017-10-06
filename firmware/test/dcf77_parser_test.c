#include "dcf77_parser.h"
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

static void simple_test(void** state) {
    
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(simple_test),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
