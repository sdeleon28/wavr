#pragma once
#include <iostream>

#define ASSERT(condition)                                 \
    if (! (condition))                                    \
    {                                                     \
        printf ("%s:%d: expression is false -> %s\n", \
                __FILE__,                                 \
                __LINE__,                                 \
                #condition);                              \
    }

#define EXPECT_EQUALS(actual, expected) ASSERT (actual == expected)

#define EXPECT_STR_EQUALS(actual, expected)                 \
    if (actual != expected)                                 \
    {                                                       \
        printf ("%s:%d: expected '%s' but was '%s' \n", \
                __FILE__,                                   \
                __LINE__,                                   \
                expected.c_str(),                           \
                actual.c_str());                            \
    }

#define EXPECT_INT_EQUALS(actual, expected)                 \
    if (actual != expected)                                 \
    {                                                       \
        printf ("%s:%d: expected '%d' but was '%d' \n", \
                __FILE__,                                   \
                __LINE__,                                   \
                expected,                                   \
                actual);                                    \
    }

#define EXPECT_LONG_EQUALS(actual, expected)                  \
    if (actual != expected)                                   \
    {                                                         \
        printf ("%s:%d: expected '%ld' but was '%ld' \n", \
                __FILE__,                                     \
                __LINE__,                                     \
                expected,                                     \
                actual);                                      \
    }

#define EXPECT_FLOAT_EQUALS(actual, expected)               \
    if (actual != expected)                                 \
    {                                                       \
        printf ("%s:%d: expected '%f' but was '%f' \n", \
                __FILE__,                                   \
                __LINE__,                                   \
                expected,                                   \
                actual);                                    \
    }

#define EXPECT_LT(actual, expected)                                  \
    if (actual > expected)                                           \
    {                                                                \
        printf ("%s:%d: expected '%f' to be lower than '%f' \n", \
                __FILE__,                                            \
                __LINE__,                                            \
                (float) actual,                                      \
                (float) expected);                                   \
    }

#define EXPECT_NEAR(actual, expected, precision)            \
    if (! (std::abs (actual - expected) < precision))       \
    {                                                       \
        printf ("%s:%d: expected '%f' but was '%f' \n", \
                __FILE__,                                   \
                __LINE__,                                   \
                expected,                                   \
                actual);                                    \
    }
