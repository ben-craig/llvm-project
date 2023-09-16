//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <algorithm>

// template<RandomAccessIterator Iter, StrictWeakOrder<auto, Iter::value_type> Compare>
//   requires ShuffleIterator<Iter>
//         && CopyConstructible<Compare>
//   void
//   sort(Iter first, Iter last, Compare comp);

#include <algorithm>
#include <functional>
#include <array>
#include <cassert>
#include <cstddef>
#include <memory>

#include "test_macros.h"
#include "MoveOnly.h"

struct indirect_less
{
    template <class P>
    bool operator()(const P& x, const P& y)
        {return x.get() < y.get();}
};

int main(int, char**)
{
    {
    static std::array<int, 1000> v;
    for (int i = 0; static_cast<std::size_t>(i) < v.size(); ++i)
        v[i] = i;
    std::sort(v.begin(), v.end(), std::greater<int>());
    std::reverse(v.begin(), v.end());
    assert(std::is_sorted(v.begin(), v.end()));
    }

#if TEST_STD_VER >= 11
    {
    static std::array<MoveOnly, 1000> v;
    for (int i = 0; static_cast<std::size_t>(i) < v.size(); ++i)
        v[i] = MoveOnly(i);
    std::sort(v.begin(), v.end(), indirect_less());
    assert(std::is_sorted(v.begin(), v.end(), indirect_less()));
    assert(v[0].get() == MoveOnly(0));
    assert(v[1].get() == MoveOnly(1));
    assert(v[2].get() == MoveOnly(2));
    }
#endif

  return 0;
}
