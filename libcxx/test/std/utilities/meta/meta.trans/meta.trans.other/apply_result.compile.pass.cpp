//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: std-at-least-c++26

// <type_traits>

// template<class Fn, class Tuple> struct apply_result;

// template<class Fn, class Tuple>
// using apply_result_t = typename apply_result<T, U>::type;

#include <cassert>
#include <array>
#include <complex>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>

template <class Fn, class Tuple>
concept apply_result_has_member_type = requires { typename std::apply_result<Fn, Tuple>::type; };

template <class Fn, class Tuple>
concept apply_result_t_is_valid = requires { typename std::apply_result_t<Fn, Tuple>; };

template <class Fn, class Tuple, class Expected>
void test_valid_apply_result() {
  static_assert(apply_result_has_member_type<Fn, Tuple>);
  static_assert(apply_result_t_is_valid<Fn, Tuple>);
  static_assert(std::is_same_v<typename std::apply_result<Fn, Tuple>::type, Expected>);
  static_assert(std::is_same_v<std::apply_result_t<Fn, Tuple>, Expected>);
}

template <class Fn, class Tuple>
void test_invalid_apply_result() {
  static_assert(!apply_result_has_member_type<Fn, Tuple>);
  static_assert(!is_apply_result_t_valid<Fn, Tuple>);
}

void test_valid_cases() {
  // test array
  // test complex
  // test subrange

  // test tuple
  test_valid_apply_result<int(), std::tuple<>, int>();
  test_valid_apply_result<char(), std::tuple<>&, char>();
  test_valid_apply_result<long&(), const std::tuple<>, long&>();
  test_valid_apply_result<void(), const std::tuple<>&, void>();
  test_valid_apply_result<int && (&)(), std::tuple<>, int&&>();
  test_valid_apply_result<char* (&)(), std::tuple<>, char*>();
  test_valid_apply_result<std::tuple<> (*)(), std::tuple<>, std::tuple<>>();

  // test pair
}

void test_volatile() {}

void test_invalid_nontuple_types() {
  test_invalid_apply_result<void(int), int(&)[1]>();
  test_invalid_apply_result<void(int), int(&&)[1]>();
}

void test_invalid_invocations() {
  // test array
  // test complex
  // test subrange

  // test tuple
  test_invalid_apply_result<void, std::tuple<>>();
  test_invalid_apply_result<void*, std::tuple<>>();
  test_invalid_apply_result<int, std::tuple<int>>();
  test_invalid_apply_result<int&, std::tuple<int>>();
  test_invalid_apply_result<int() const&, std::tuple<int>>();

  test_invalid_apply_result<int(), std::tuple<int>>();

  // test pair
  test_invalid_apply_result<void, std::pair<int, long>>();
  test_invalid_apply_result<void*, std::pair<int, long>>();
  test_invalid_apply_result<int, std::pair<int, long>>();
  test_invalid_apply_result<int&, std::pair<int, long>>();
  test_invalid_apply_result<int(int, long) const&, std::pair<int, long>>();
}
