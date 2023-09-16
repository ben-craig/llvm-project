//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <algorithm>

// template<InputIterator InIter1, InputIterator InIter2, typename OutIter,
//          CopyConstructible Compare>
//   requires OutputIterator<OutIter, InIter1::reference>
//         && OutputIterator<OutIter, InIter2::reference>
//         && Predicate<Compare, InIter1::value_type, InIter2::value_type>
//         && Predicate<Compare, InIter2::value_type, InIter1::value_type>
//   OutIter
//   set_union(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2,
//             OutIter result, Compare comp);

// UNSUPPORTED: c++03

#include <algorithm>
#include <cassert>
#include <iterator>
#include <array>

#include "test_macros.h"
#include "MoveOnly.h"


int main(int, char**)
{
    std::array<MoveOnly, 1> lhs({2}), rhs({2});

    std::array<MoveOnly, 2> res({99,99});
    std::set_union(std::make_move_iterator(lhs.begin()),
                   std::make_move_iterator(lhs.end()),
                   std::make_move_iterator(rhs.begin()),
                   std::make_move_iterator(rhs.end()), std::begin(res));

    assert(res[0].get() == 2);
    assert(res[1].get() == 99);

  return 0;
}
