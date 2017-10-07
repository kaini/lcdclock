#pragma once
#include <assert.h>

#ifdef NDEBUG
    #define ASSERT(EXPR) do { (void)(EXPR); } while (0)
#else
    #define ASSERT(EXPR) do { assert(EXPR); } while (0)
#endif
