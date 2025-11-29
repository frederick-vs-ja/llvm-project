//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <functional>

// template<class R, class T> constexpr unspecified mem_fn(R T::*) noexcept;       // constexpr in C++20

// C++17 [depr.func.adaptor.typedefs]
// 9. The simple call wrapper returned from a call to `mem_fn(pm)` shall have a nested type `result_type` that is a
//   synonym for the return type of `pm` when `pm` is a pointer to member function.
// 10. The simple call wrapper returned from a call to `mem_fn(pm)` shall define two nested types named
//   `argument_type` and `result_type` as synonyms for `cv T*` and `Ret`, respectively, when `pm` is a pointer to
//   member function with cv-qualifier `cv` and taking no arguments, where `Ret` is `pm`'s return type.
// 11. The simple call wrapper returned from a call to `mem_fn(pm)` shall define three nested types named
//   `first_argument_type`, `second_argument_type`, and `result_type` as synonyms for `cv T*`, `T1`, and `Ret`,
//   respectively, when `pm` is a pointer to member function with cv-qualifier `cv` and taking one argument of type
//   `T1`, where `Ret` is `pm`'s return type.

// When `pm` is pointer to member function taking variadic arguments, there should be no member type `argument_type`,
// `first_argument_type`, or `second_argument_type`.

// REQUIRES: c++03 || c++11 || c++14 || c++17

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

#include <functional>
#include <type_traits>

#include "test_macros.h"

template <class>
struct make_void {
  typedef void type;
};

template <class T, class = void>
struct has_no_argument_type : std::true_type {};
template <class T>
struct has_no_argument_type<T, typename make_void<typename T::argument_type>::type> : std::false_type {};

template <class T, class = void>
struct has_no_first_argument_type : std::true_type {};
template <class T>
struct has_no_first_argument_type<T, typename make_void<typename T::first_argument_type>::type> : std::false_type {};

template <class T, class = void>
struct has_no_second_argument_type : std::true_type {};
template <class T>
struct has_no_second_argument_type<T, typename make_void<typename T::second_argument_type>::type> : std::false_type {};

template <bool IsNoexcept>
struct test_class {
  void f1() TEST_NOEXCEPT_COND(IsNoexcept);
  int f2() const TEST_NOEXCEPT_COND(IsNoexcept);
  int* f3() volatile TEST_NOEXCEPT_COND(IsNoexcept);
  int& f4() const volatile TEST_NOEXCEPT_COND(IsNoexcept);

  void f5(int) TEST_NOEXCEPT_COND(IsNoexcept);
  int f6(int*) const TEST_NOEXCEPT_COND(IsNoexcept);
  int* f7(long&) volatile TEST_NOEXCEPT_COND(IsNoexcept);
  const int& f8(const short&) const volatile TEST_NOEXCEPT_COND(IsNoexcept);

  void f9(...) TEST_NOEXCEPT_COND(IsNoexcept);
  int f10(...) const TEST_NOEXCEPT_COND(IsNoexcept);
  int* f11(...) volatile TEST_NOEXCEPT_COND(IsNoexcept);
  volatile int& f12(...) const volatile TEST_NOEXCEPT_COND(IsNoexcept);

  void f13(int, ...) TEST_NOEXCEPT_COND(IsNoexcept);
  int f14(int*, ...) const TEST_NOEXCEPT_COND(IsNoexcept);
  int* f15(long&, ...) volatile TEST_NOEXCEPT_COND(IsNoexcept);
  const volatile int& f16(const int&, ...) const volatile TEST_NOEXCEPT_COND(IsNoexcept);

  void f17(int, char, long) TEST_NOEXCEPT_COND(IsNoexcept);
  int f18(char*, long*, const int*) const TEST_NOEXCEPT_COND(IsNoexcept);
  int* f19(long&, const int&, char&) volatile TEST_NOEXCEPT_COND(IsNoexcept);
  int& f20(const int&, volatile long&, const volatile char&) const volatile TEST_NOEXCEPT_COND(IsNoexcept);

#if TEST_STD_VER >= 11
  void f21() & TEST_NOEXCEPT_COND(IsNoexcept);
  int f22() const& TEST_NOEXCEPT_COND(IsNoexcept);
  int* f23() volatile& TEST_NOEXCEPT_COND(IsNoexcept);
  int& f24() const volatile& TEST_NOEXCEPT_COND(IsNoexcept);

  void f25(int) & TEST_NOEXCEPT_COND(IsNoexcept);
  int f26(int*) const& TEST_NOEXCEPT_COND(IsNoexcept);
  int* f27(long&) volatile& TEST_NOEXCEPT_COND(IsNoexcept);
  const int& f28(const char&) const volatile& TEST_NOEXCEPT_COND(IsNoexcept);

  void f29() && TEST_NOEXCEPT_COND(IsNoexcept);
  int f30() const&& TEST_NOEXCEPT_COND(IsNoexcept);
  int* f31() volatile&& TEST_NOEXCEPT_COND(IsNoexcept);
  volatile int&& f32() const volatile&& TEST_NOEXCEPT_COND(IsNoexcept);

  void f33(int) && TEST_NOEXCEPT_COND(IsNoexcept);
  int f34(int*) const&& TEST_NOEXCEPT_COND(IsNoexcept);
  int* f35(long&&) volatile&& TEST_NOEXCEPT_COND(IsNoexcept);
  const volatile int&& f36(const long&) const volatile&& TEST_NOEXCEPT_COND(IsNoexcept);
#endif // TEST_STD_VER >= 11
};

template <class Result, class W>
void test_result_type_only(W) {
  static_assert(std::is_same<typename W::result_type, Result>::value, "");
  static_assert(has_no_argument_type<W>::value, "");
  static_assert(has_no_first_argument_type<W>::value, "");
  static_assert(has_no_second_argument_type<W>::value, "");
}

template <class Result, class Argument, class W>
void test_one_argument(W) {
  static_assert(std::is_same<typename W::result_type, Result>::value, "");
  static_assert(std::is_same<typename W::argument_type, Argument>::value, "");
  static_assert(has_no_first_argument_type<W>::value, "");
  static_assert(has_no_second_argument_type<W>::value, "");
}

template <class Result, class FirstArgument, class SecondArgument, class W>
void test_two_arguments(W) {
  static_assert(std::is_same<typename W::result_type, Result>::value, "");
  static_assert(has_no_argument_type<W>::value, "");
  static_assert(std::is_same<typename W::first_argument_type, FirstArgument>::value, "");
  static_assert(std::is_same<typename W::second_argument_type, SecondArgument>::value, "");
}

template <bool IsNoexcept>
void test_noexcept() {
  typedef test_class<IsNoexcept> C;

  test_one_argument<void, C*>(std::mem_fn(&C::f1));
  test_one_argument<int, const C*>(std::mem_fn(&C::f2));
  test_one_argument<int*, volatile C*>(std::mem_fn(&C::f3));
  test_one_argument<int&, const volatile C*>(std::mem_fn(&C::f4));

  test_two_arguments<void, C*, int>(std::mem_fn(&C::f5));
  test_two_arguments<int, const C*, int*>(std::mem_fn(&C::f6));
  test_two_arguments<int*, volatile C*, long&>(std::mem_fn(&C::f7));
  test_two_arguments<const int&, const volatile C*, const short&>(std::mem_fn(&C::f8));

  test_result_type_only<void>(std::mem_fn(&C::f9));
  test_result_type_only<int>(std::mem_fn(&C::f10));
  test_result_type_only<int*>(std::mem_fn(&C::f11));
  test_result_type_only<volatile int&>(std::mem_fn(&C::f12));

  test_result_type_only<void>(std::mem_fn(&C::f13));
  test_result_type_only<int>(std::mem_fn(&C::f14));
  test_result_type_only<int*>(std::mem_fn(&C::f15));
  test_result_type_only<const volatile int&>(std::mem_fn(&C::f16));

  test_result_type_only<void>(std::mem_fn(&C::f17));
  test_result_type_only<int>(std::mem_fn(&C::f18));
  test_result_type_only<int*>(std::mem_fn(&C::f19));
  test_result_type_only<int&>(std::mem_fn(&C::f20));

#if TEST_STD_VER >= 11
  test_one_argument<void, C*>(std::mem_fn(&C::f21));
  test_one_argument<int, const C*>(std::mem_fn(&C::f22));
  test_one_argument<int*, volatile C*>(std::mem_fn(&C::f23));
  test_one_argument<int&, const volatile C*>(std::mem_fn(&C::f24));

  test_two_arguments<void, C*, int>(std::mem_fn(&C::f25));
  test_two_arguments<int, const C*, int*>(std::mem_fn(&C::f6));
  test_two_arguments<int*, volatile C*, long&>(std::mem_fn(&C::f27));
  test_two_arguments<const int&, const volatile C*, const char&>(std::mem_fn(&C::f28));

  test_one_argument<void, C*>(std::mem_fn(&C::f29));
  test_one_argument<int, const C*>(std::mem_fn(&C::f30));
  test_one_argument<int*, volatile C*>(std::mem_fn(&C::f31));
  test_one_argument<volatile int&&, const volatile C*>(std::mem_fn(&C::f32));

  test_two_arguments<void, C*, int>(std::mem_fn(&C::f33));
  test_two_arguments<int, const C*, int*>(std::mem_fn(&C::f34));
  test_two_arguments<int*, volatile C*, long&&>(std::mem_fn(&C::f35));
  test_two_arguments<const volatile int&&, const volatile C*, const long&>(std::mem_fn(&C::f36));
#endif // TEST_STD_VER >= 11
}

void test() {
  test_noexcept<false>();
  test_noexcept<true>();
}
