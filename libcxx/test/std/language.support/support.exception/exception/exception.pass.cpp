//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test exception

#include <exception>
#include <type_traits>
#include <cassert>

#include "test_macros.h"

#if BCRAIG_FREESTANDING
// Nopped, never calling, only needed to make virtual dtors work
void operator delete(void * p) noexcept
{}
#endif

constexpr bool test()
{
    std::exception b;
    std::exception b2 = b;
    b2 = b;
    const char* w = b2.what();
    assert(w);
    return true;
}

int main(int, char**)
{
    static_assert(std::is_polymorphic<std::exception>::value,
                 "std::is_polymorphic<std::exception>::value");
    test();
    static_assert(test());
  return 0;
}
