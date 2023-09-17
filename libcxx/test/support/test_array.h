//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef TEST_ARRAY_H
#define TEST_ARRAY_H

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <compare>

template <typename T, size_t N>
struct TestArray
{
    T m_data[N];

    friend constexpr auto operator<=>(const TestArray &, const TestArray &) = default;

    template <size_t M>
    friend constexpr auto operator<=>(const TestArray<T, N> &lhs, const TestArray<T, M> &rhs)
    {
        return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
    
    // types
    using value_type             = T;
    using pointer                = T*;
    using const_pointer          = const T*;
    using reference              = T&;
    using const_reference        = const T&;
    using size_type              = size_t;
    using difference_type        = ptrdiff_t;
    using iterator               = T*; // see [container.requirements]
    using const_iterator         = const T*; // see [container.requirements]
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // no explicit construct/copy/destroy for aggregate type

    constexpr void fill(const T& u) {std::fill_n(begin(), N, u);}
    constexpr void swap(TestArray&y) noexcept(std::is_nothrow_swappable_v<T>) {std::swap_ranges(begin(), end(), y.begin());}

    // iterators
    constexpr iterator               begin() noexcept {return m_data;}
    constexpr const_iterator         begin() const noexcept {return m_data;}
    constexpr iterator               end() noexcept {return m_data + N;}
    constexpr const_iterator         end() const noexcept {return m_data + N;}

    constexpr reverse_iterator       rbegin() noexcept {return reverse_iterator(end());}
    constexpr const_reverse_iterator rbegin() const noexcept {return const_reverse_iterator(end());}
    constexpr reverse_iterator       rend() noexcept {return reverse_iterator(begin());}
    constexpr const_reverse_iterator rend() const noexcept {return const_reverse_iterator(begin());}

    constexpr const_iterator         cbegin() const noexcept {return m_data;}
    constexpr const_iterator         cend() const noexcept {return m_data + N;}
    constexpr const_reverse_iterator crbegin() const noexcept {return const_reverse_iterator(end());}
    constexpr const_reverse_iterator crend() const noexcept {return const_reverse_iterator(begin());}

    // capacity
    [[nodiscard]] constexpr bool empty() const noexcept {return N==0;}
    constexpr size_type size() const noexcept {return N;}
    constexpr size_type max_size() const noexcept {return N;}

    // element access
    constexpr reference       operator[](size_type n) {return m_data[n];}
    constexpr const_reference operator[](size_type n) const {return m_data[n];}
    //constexpr reference       at(size_type n) {return m_data[n];}
    //constexpr const_reference at(size_type n) const {return m_data[n];}
    constexpr reference       front() {return m_data[0];}
    constexpr const_reference front() const {return m_data[0];}
    constexpr reference       back() {return m_data[N-1];}
    constexpr const_reference back() const {return m_data[N-1];}

    constexpr T *       data() noexcept {return m_data;}
    constexpr const T * data() const noexcept {return m_data;}
};

template<class T, class... U>
TestArray(T, U...) -> TestArray<T, 1 + sizeof...(U)>;

template <typename T>
struct TestArray<T, 0>
{
    friend constexpr auto operator<=>(const TestArray &, const TestArray &) = default;

    template <size_t M>
    friend constexpr auto operator<=>(const TestArray<T, 0> &lhs, const TestArray<T, M> &rhs)
    {
        return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
    
    // types
    using value_type             = T;
    using pointer                = T*;
    using const_pointer          = const T*;
    using reference              = T&;
    using const_reference        = const T&;
    using size_type              = size_t;
    using difference_type        = ptrdiff_t;
    using iterator               = T*; // see [container.requirements]
    using const_iterator         = const T*; // see [container.requirements]
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // no explicit construct/copy/destroy for aggregate type

    constexpr void fill(const T& u) {}
    constexpr void swap(TestArray&y) noexcept(std::is_nothrow_swappable_v<T>) {}

    // iterators
    constexpr iterator               begin() noexcept {return nullptr;}
    constexpr const_iterator         begin() const noexcept {return nullptr;}
    constexpr iterator               end() noexcept {return nullptr;}
    constexpr const_iterator         end() const noexcept {return nullptr;}

    constexpr reverse_iterator       rbegin() noexcept {return reverse_iterator(end());}
    constexpr const_reverse_iterator rbegin() const noexcept {return const_reverse_iterator(end());}
    constexpr reverse_iterator       rend() noexcept {return reverse_iterator(begin());}
    constexpr const_reverse_iterator rend() const noexcept {return const_reverse_iterator(begin());}

    constexpr const_iterator         cbegin() const noexcept {return nullptr;}
    constexpr const_iterator         cend() const noexcept {return nullptr;}
    constexpr const_reverse_iterator crbegin() const noexcept {return const_reverse_iterator(end());}
    constexpr const_reverse_iterator crend() const noexcept {return const_reverse_iterator(begin());}

    // capacity
    [[nodiscard]] constexpr bool empty() const noexcept {return true;}
    constexpr size_type size() const noexcept {return 0;}
    constexpr size_type max_size() const noexcept {return 0;}

    // element access
    constexpr reference       operator[](size_type n) {return reinterpret_cast<reference>(n);}
    constexpr const_reference operator[](size_type n) const {return reinterpret_cast<const_reference>(n);}
    //constexpr reference       at(size_type n) {return m_data[n];}
    //constexpr const_reference at(size_type n) const {return m_data[n];}
    constexpr reference       front() {return reinterpret_cast<reference>(0);}
    constexpr const_reference front() const {return reinterpret_cast<const_reference>(0);}
    constexpr reference       back() {return reinterpret_cast<reference>(0);}
    constexpr const_reference back() const {return reinterpret_cast<const_reference>(0);}

    constexpr T *       data() noexcept {return nullptr;}
    constexpr const T * data() const noexcept {return nullptr;}
};

#endif // TEST_ARRAY_H
