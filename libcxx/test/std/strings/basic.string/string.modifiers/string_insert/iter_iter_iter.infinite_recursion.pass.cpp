//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// Make sure basic_string::insert does not cause infinite recursion.
// This is a regression test for a bug that had been introduced in D98573.

#include <algorithm>
#include <cassert>
#include <string>

#include "test_macros.h"

struct char_ascii {
    char char_;

    constexpr char_ascii() = default;
    constexpr char_ascii(char ch) : char_(ch) {
        assert(ch <= 0x7f);
    }
};

template <>
struct std::char_traits<char_ascii> {
    using char_type = char_ascii;
    using int_type = typename std::char_traits<char>::int_type;
    using off_type = typename std::char_traits<char>::off_type;
    using pos_type = typename std::char_traits<char>::pos_type;
    using state_type = typename std::char_traits<char>::state_type;

    static constexpr void assign(char_type& r, char_type const& a) TEST_NOEXCEPT {
        r = a;
    }

    static constexpr char_type* assign(char_type* p, std::size_t count, char_type a) {
        std::fill(p, p + count, a);
        return p;
    }

    static constexpr bool eq(char_type a, char_type b) TEST_NOEXCEPT {
        return a.char_ == b.char_;
    }

    static constexpr bool lt(char_type a, char_type b) TEST_NOEXCEPT {
        return a.char_ < b.char_;
    }

    static constexpr std::size_t length(char_type const* s) {
        std::size_t n = 0;
        if (s) {
            while (s->char_)
                ++n;
        }
        return n;
    }

    static constexpr const char_type* find(char_type const* p, std::size_t count, char_type const& ch) {
        while (count > 0) {
            if (p->char_ == ch.char_) {
                return p;
            } else {
                ++p;
                --count;
            }
        }
        return nullptr;
    }

    static constexpr int compare(char_type const* s1, char_type const* s2, std::size_t count) {
        for (std::size_t i = 0; i < count; ++i) {
            if (s1->char_ < s2->char_)
                return -1;
            else if (s2->char_ < s1->char_)
                return 1;
        }
        return 0;
    }

    static constexpr char_type* move(char_type* dest, char_type const* src, std::size_t count) {
        if (src <= dest && dest < src + count) {
            std::copy_backward(src, src + count, dest + count);
        } else {
            std::copy(src, src + count, dest);
        }
        return dest;
    }

    static constexpr char_type* copy(char_type* dest, char_type const* src, std::size_t count) {
        return char_traits::move(dest, src, count);
    }
};

consteval bool test()
{
    std::basic_string<char_ascii> str;

    char_ascii ch('A');
    str.insert(str.begin(), &ch, &ch + 1);
    assert(str.size() == 1);
    assert(str[0].char_ == 'A');
    return true;
}

int main(int, char**) {
    static_assert(test());
    return 0;
}
