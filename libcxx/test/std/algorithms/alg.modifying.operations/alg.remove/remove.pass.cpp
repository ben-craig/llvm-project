//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <algorithm>

// template<ForwardIterator Iter, class T>
//   requires OutputIterator<Iter, RvalueOf<Iter::reference>::type>
//         && HasEqualTo<Iter::value_type, T>
//   constexpr Iter         // constexpr after C++17
//   remove(Iter first, Iter last, const T& value);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "MoveOnly.h"

#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
    int ia[] = {1, 3, 5, 2, 5, 6};

    auto it = std::remove(std::begin(ia), std::end(ia), 5);

    return (std::begin(ia) + std::size(ia) - 2) == it  // we removed two elements
        && std::none_of(std::begin(ia), it, [](int a) {return a == 5; })
           ;
    }
#endif

template <class Iter>
void
test()
{
    int ia[] = {0, 1, 2, 3, 4, 2, 3, 4, 2};
    const unsigned sa = sizeof(ia)/sizeof(ia[0]);
    Iter r = std::remove(Iter(ia), Iter(ia+sa), 2);
    assert(base(r) == ia + sa-3);
    assert(ia[0] == 0);
    assert(ia[1] == 1);
    assert(ia[2] == 3);
    assert(ia[3] == 4);
    assert(ia[4] == 3);
    assert(ia[5] == 4);
}

#if TEST_STD_VER >= 11
template <class Iter>
void
test1()
{
    const unsigned sa = 9;
    MoveOnly ia[sa];
    ia[0] = MoveOnly(0);
    ia[1] = MoveOnly(1);
    ia[2] = MoveOnly(86);
    ia[3] = MoveOnly(3);
    ia[4] = MoveOnly(4);
    ia[5] = MoveOnly(86);
    ia[6] = MoveOnly(3);
    ia[7] = MoveOnly(4);
    ia[8] = MoveOnly(86);
    Iter r = std::remove(Iter(ia), Iter(ia+sa), MoveOnly{86});
    assert(base(r) == ia + sa-3);
    assert(ia[0].get() == 0);
    assert(ia[1].get() == 1);
    assert(ia[2].get() == 3);
    assert(ia[3].get() == 4);
    assert(ia[4].get() == 3);
    assert(ia[5].get() == 4);
}
#endif // TEST_STD_VER >= 11

int main(int, char**)
{
    test<forward_iterator<int*> >();
    test<bidirectional_iterator<int*> >();
    test<random_access_iterator<int*> >();
    test<int*>();

#if TEST_STD_VER >= 11
    test1<forward_iterator<MoveOnly*> >();
    test1<bidirectional_iterator<MoveOnly*> >();
    test1<random_access_iterator<MoveOnly*> >();
    test1<MoveOnly*>();
#endif // TEST_STD_VER >= 11

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  return 0;
}
