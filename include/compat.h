#ifndef COMPAT_H_
#   define COMPAT_H_

#include <cstdlib>

#if __cplusplus >= 202207L

using zu = _zu;

#elif __cplusplus >= 200809L
// In case you cannot afford C++23:
constexpr std::size_t operator""_zu(unsigned long long int val)
{
    return static_cast<size_t>(val);
}
#else
#error You need at least C++11!
#endif

struct Foo
{
    static constexpr size_t SIZE = 1_zu << 32;
    int _foo[SIZE];
};

#endif  // COMPAT_H_
/*
https://godbolt.org/z/M33fc9fxz
*/
