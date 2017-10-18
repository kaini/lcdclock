#pragma once
#include <assert.h>

#define CONCAT_IMPL(A, B) A##B
#define CONCAT(A, B) CONCAT_IMPL(A, B)

#ifdef NDEBUG
    #define ASSERT(EXPR) do { (void)(EXPR); } while (0)
#else
    #define ASSERT(EXPR) do { assert(EXPR); } while (0)
#endif

#define ASSERT_HAL(EXPR) do { \
    HAL_StatusTypeDef CONCAT(status, __LINE__) = (EXPR); \
    ASSERT(CONCAT(status, __LINE__) == HAL_OK); \
} while (0)
