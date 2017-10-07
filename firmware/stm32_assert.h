#pragma once
#include "my_assert.h"

#ifdef NDEBUG
    #define assert_param(EXPR) do { } while(0)
#else
    #define assert_param(EXPR) do { ASSERT(EXPR); } while (0)
#endif
