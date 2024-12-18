#ifndef BASE_ASSERT_H
#define BASE_ASSERT_H

#include <debug.hpp>

#include <string>

#ifdef DEBUG
#define assertTrap() __builtin_trap()
#define assert(EXP, MESSAGE)        \
{                                   \
    if (EXP) {                      \
    } else {                        \
        Debug::log(                 \
            DEBUG_FATAL,            \
            "Assertion failed: " +  \
            std::string(MESSAGE) +  \
            " file: " +             \
            std::string(__FILE__) + \
            " line: " +             \
            std::to_string(__LINE__)\
        );                          \
        assertTrap();               \
    }                               \
}

#else
#define assert(EXP, MESSAGE)
#endif
#endif