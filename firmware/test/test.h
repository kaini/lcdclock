#include <stdbool.h>

#define TEST_RESULT bool
#define TEST_SUCCESS true
#define TEST_FALURE false

#define TEST_ASSERT(expr) do { \
    if (!(result)) { \
        puts("Test assertion failed in " __func__ ": " #__expr__ " @" __file__ ":" __line__ "\n"); \
        return TEST_FAILURE; \
    } \
} while (0)


