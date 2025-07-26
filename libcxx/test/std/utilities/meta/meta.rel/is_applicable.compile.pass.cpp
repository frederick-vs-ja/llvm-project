//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: std-at-least-c++26

// <type_traits>

// template<class Fn, class Tuple> struct is_applicable;

// template<class Fn, class Tuple>
// constexpr bool is_applicable_v = is_applicable<T, U>::value;

#include <cassert>
#include <array>
#include <complex>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>

template <class Fn, class Tuple, bool Expected>
void test_is_applicable() {
  static_assert(std::is_applicable<Fn, Tuple>::value == Expected);
  static_assert(std::is_applicable_v<Fn, Tuple> == Expected);
}

void test_valid_cases() {
  // test array
  // test complex
  // test subrange

  // test tuple
  test_is_applicable<void, std::tuple<>, false>();
  test_is_applicable<void*, std::tuple<>, false>();
  test_is_applicable<int, std::tuple<int>, false>();
  test_is_applicable<int&, std::tuple<int>, false>();
  test_is_applicable<int() const&, std::tuple<int>, false>();

  test_is_applicable<int(), std::tuple<>, true>();
  test_is_applicable<int(), std::tuple<>&, true>();
  test_is_applicable<int(), const std::tuple<>, true>();
  test_is_applicable<int(), const std::tuple<>&, true>();
  test_is_applicable<int (&)(), std::tuple<>, true>();
  test_is_applicable<int (&)(), std::tuple<>, true>();
  test_is_applicable<int (*)(), std::tuple<>, true>();
  test_is_applicable<int(), std::tuple<int>, false>();

  // test pair
  test_is_applicable<void, std::pair<int, long>, false>();
  test_is_applicable<void*, std::pair<int, long>, false>();
  test_is_applicable<int, std::pair<int, long>, false>();
  test_is_applicable<int&, std::pair<int, long>, false>();
  test_is_applicable<int(int, long) const&, std::pair<int, long>, false>();

  // test invalid types
  test_is_applicable<void(int), int(&)[1], false>();
  test_is_applicable<void(int), int(&&)[1], false>();
}

void test_volatile() {}

void test_invalid_nontuple_types() {}

void test_invalid_invocations() {}
