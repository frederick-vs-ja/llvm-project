//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <functional>

//  template <class T> class reference_wrapper {
//  public :
//    using result_type          = see below; // not always defined; deprecated in C++17, removed in C++20
//    using argument_type        = see below; // not always defined; deprecated in C++17, removed in C++20
//    using first_argument_type  = see below; // not always defined; deprecated in C++17, removed in C++20
//    using second_argument_type = see below; // not always defined; deprecated in C++17, removed in C++20
//  };

// C++17 [depr.func.adaptor.typedefs]
// 3. `reference_wrapper<T>` has a weak result type. If `T` is a function type, `result_type` shall be a synonym for the
//   return type of `T`.
// 4. The template specialization `reference_wrapper<T>` shall define a nested type named `argument_type` as a synonym
//   for `T1` only if the type `T` is any of the following:
//   (4.1) a function type or a pointer to function type taking one argument of type `T1`
//   (4.2) a pointer to member function `R T0::f() cv` (where `cv` represents the member function's cv-qualifiers); the
//     type `T1` is `cv T0*`
//   (4.3) [...] (not covered in this test)
// 5. The template instantiation `reference_wrapper<T>` shall define two nested types named first_argument_type and
//   `second_argument_type` as synonyms for `T1` and `T2`, respectively, only if the type `T` is any of the following:
//   (5.1) a function type or a pointer to function type taking two arguments of types `T1` and `T2`
//   (5.2) a pointer to member function `R T0::f(T2) cv` (where cv represents the member function's cv-qualifiers); the
//     type `T1` is `cv T0*`
//   (5.3) [...] (not covered in this test)

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

  static void f37() TEST_NOEXCEPT_COND(IsNoexcept);
  static void* f38(double&) TEST_NOEXCEPT_COND(IsNoexcept);
  static long double f39(int, float) TEST_NOEXCEPT_COND(IsNoexcept);
  static const int& f40(signed char, unsigned char, char) TEST_NOEXCEPT_COND(IsNoexcept);

  static void f41(...) TEST_NOEXCEPT_COND(IsNoexcept);
  static void* f42(double&, ...) TEST_NOEXCEPT_COND(IsNoexcept);
  static long double f43(int, float, ...) TEST_NOEXCEPT_COND(IsNoexcept);
  static const int& f44(signed char, unsigned char, char, ...) TEST_NOEXCEPT_COND(IsNoexcept);
};

template <class Result, class Fn>
void test_fn_result_type_only(Fn&) {
  typedef std::reference_wrapper<Fn> W;
  static_assert(std::is_same<typename W::result_type, Result>::value, "");
  static_assert(has_no_argument_type<W>::value, "");
  static_assert(has_no_first_argument_type<W>::value, "");
  static_assert(has_no_second_argument_type<W>::value, "");
}

template <class Result, class Argument, class Fn>
void test_fn_one_argument(Fn&) {
  typedef std::reference_wrapper<Fn> W;
  static_assert(std::is_same<typename W::result_type, Result>::value, "");
  static_assert(std::is_same<typename W::argument_type, Argument>::value, "");
  static_assert(has_no_first_argument_type<W>::value, "");
  static_assert(has_no_second_argument_type<W>::value, "");
}

template <class Result, class FirstArgument, class SecondArgument, class Fn>
void test_fn_two_arguments(Fn&) {
  typedef std::reference_wrapper<Fn> W;
  static_assert(std::is_same<typename W::result_type, Result>::value, "");
  static_assert(has_no_argument_type<W>::value, "");
  static_assert(std::is_same<typename W::first_argument_type, FirstArgument>::value, "");
  static_assert(std::is_same<typename W::second_argument_type, SecondArgument>::value, "");
}

template <class Result, class PF>
void test_pf_pmf_result_type_only(PF) {
  {
    typedef std::reference_wrapper<PF> W;
    static_assert(std::is_same<typename W::result_type, Result>::value, "");
    static_assert(has_no_argument_type<W>::value, "");
    static_assert(has_no_first_argument_type<W>::value, "");
    static_assert(has_no_second_argument_type<W>::value, "");
  }
  {
    typedef std::reference_wrapper<const PF> W;
    static_assert(std::is_same<typename W::result_type, Result>::value, "");
    static_assert(has_no_argument_type<W>::value, "");
    static_assert(has_no_first_argument_type<W>::value, "");
    static_assert(has_no_second_argument_type<W>::value, "");
  }
  {
    typedef std::reference_wrapper<volatile PF> W;
    static_assert(std::is_same<typename W::result_type, Result>::value, "");
    static_assert(has_no_argument_type<W>::value, "");
    static_assert(has_no_first_argument_type<W>::value, "");
    static_assert(has_no_second_argument_type<W>::value, "");
  }
  {
    typedef std::reference_wrapper<const volatile PF> W;
    static_assert(std::is_same<typename W::result_type, Result>::value, "");
    static_assert(has_no_argument_type<W>::value, "");
    static_assert(has_no_first_argument_type<W>::value, "");
    static_assert(has_no_second_argument_type<W>::value, "");
  }
}

template <class Result, class Argument, class PF>
void test_pf_pmf_one_argument(PF) {
  {
    typedef std::reference_wrapper<PF> W;
    static_assert(std::is_same<typename W::result_type, Result>::value, "");
    static_assert(std::is_same<typename W::argument_type, Argument>::value, "");
    static_assert(has_no_first_argument_type<W>::value, "");
    static_assert(has_no_second_argument_type<W>::value, "");
  }
  {
    typedef std::reference_wrapper<const PF> W;
    static_assert(std::is_same<typename W::result_type, Result>::value, "");
    static_assert(std::is_same<typename W::argument_type, Argument>::value, "");
    static_assert(has_no_first_argument_type<W>::value, "");
    static_assert(has_no_second_argument_type<W>::value, "");
  }
  {
    typedef std::reference_wrapper<volatile PF> W;
    static_assert(std::is_same<typename W::result_type, Result>::value, "");
    static_assert(std::is_same<typename W::argument_type, Argument>::value, "");
    static_assert(has_no_first_argument_type<W>::value, "");
    static_assert(has_no_second_argument_type<W>::value, "");
  }
  {
    typedef std::reference_wrapper<const volatile PF> W;
    static_assert(std::is_same<typename W::result_type, Result>::value, "");
    static_assert(std::is_same<typename W::argument_type, Argument>::value, "");
    static_assert(has_no_first_argument_type<W>::value, "");
    static_assert(has_no_second_argument_type<W>::value, "");
  }
}

template <class Result, class FirstArgument, class SecondArgument, class PF>
void test_pf_pmf_two_arguments(PF) {
  {
    typedef std::reference_wrapper<PF> W;
    static_assert(std::is_same<typename W::result_type, Result>::value, "");
    static_assert(has_no_argument_type<W>::value, "");
    static_assert(std::is_same<typename W::first_argument_type, FirstArgument>::value, "");
    static_assert(std::is_same<typename W::second_argument_type, SecondArgument>::value, "");
  }
  {
    typedef std::reference_wrapper<const PF> W;
    static_assert(std::is_same<typename W::result_type, Result>::value, "");
    static_assert(has_no_argument_type<W>::value, "");
    static_assert(std::is_same<typename W::first_argument_type, FirstArgument>::value, "");
    static_assert(std::is_same<typename W::second_argument_type, SecondArgument>::value, "");
  }
  {
    typedef std::reference_wrapper<volatile PF> W;
    static_assert(std::is_same<typename W::result_type, Result>::value, "");
    static_assert(has_no_argument_type<W>::value, "");
    static_assert(std::is_same<typename W::first_argument_type, FirstArgument>::value, "");
    static_assert(std::is_same<typename W::second_argument_type, SecondArgument>::value, "");
  }
  {
    typedef std::reference_wrapper<const volatile PF> W;
    static_assert(std::is_same<typename W::result_type, Result>::value, "");
    static_assert(has_no_argument_type<W>::value, "");
    static_assert(std::is_same<typename W::first_argument_type, FirstArgument>::value, "");
    static_assert(std::is_same<typename W::second_argument_type, SecondArgument>::value, "");
  }
}

template <bool IsNoexcept>
void test_func_noexcept() {
  typedef test_class<IsNoexcept> C;

  test_pf_pmf_one_argument<void, C*>(&C::f1);
  test_pf_pmf_one_argument<int, const C*>(&C::f2);
  test_pf_pmf_one_argument<int*, volatile C*>(&C::f3);
  test_pf_pmf_one_argument<int&, const volatile C*>(&C::f4);

  test_pf_pmf_two_arguments<void, C*, int>(&C::f5);
  test_pf_pmf_two_arguments<int, const C*, int*>(&C::f6);
  test_pf_pmf_two_arguments<int*, volatile C*, long&>(&C::f7);
  test_pf_pmf_two_arguments<const int&, const volatile C*, const short&>(&C::f8);

  test_pf_pmf_result_type_only<void>(&C::f9);
  test_pf_pmf_result_type_only<int>(&C::f10);
  test_pf_pmf_result_type_only<int*>(&C::f11);
  test_pf_pmf_result_type_only<volatile int&>(&C::f12);

  test_pf_pmf_result_type_only<void>(&C::f13);
  test_pf_pmf_result_type_only<int>(&C::f14);
  test_pf_pmf_result_type_only<int*>(&C::f15);
  test_pf_pmf_result_type_only<const volatile int&>(&C::f16);

  test_pf_pmf_result_type_only<void>(&C::f17);
  test_pf_pmf_result_type_only<int>(&C::f18);
  test_pf_pmf_result_type_only<int*>(&C::f19);
  test_pf_pmf_result_type_only<int&>(&C::f20);

#if TEST_STD_VER >= 11
  test_pf_pmf_one_argument<void, C*>(&C::f21);
  test_pf_pmf_one_argument<int, const C*>(&C::f22);
  test_pf_pmf_one_argument<int*, volatile C*>(&C::f23);
  test_pf_pmf_one_argument<int&, const volatile C*>(&C::f24);

  test_pf_pmf_two_arguments<void, C*, int>(&C::f25);
  test_pf_pmf_two_arguments<int, const C*, int*>(&C::f6);
  test_pf_pmf_two_arguments<int*, volatile C*, long&>(&C::f27);
  test_pf_pmf_two_arguments<const int&, const volatile C*, const char&>(&C::f28);

  test_pf_pmf_one_argument<void, C*>(&C::f29);
  test_pf_pmf_one_argument<int, const C*>(&C::f30);
  test_pf_pmf_one_argument<int*, volatile C*>(&C::f31);
  test_pf_pmf_one_argument<volatile int&&, const volatile C*>(&C::f32);

  test_pf_pmf_two_arguments<void, C*, int>(&C::f33);
  test_pf_pmf_two_arguments<int, const C*, int*>(&C::f34);
  test_pf_pmf_two_arguments<int*, volatile C*, long&&>(&C::f35);
  test_pf_pmf_two_arguments<const volatile int&&, const volatile C*, const long&>(&C::f36);
#endif // TEST_STD_VER >= 11

  test_pf_pmf_result_type_only<void>(&C::f37);
  test_pf_pmf_one_argument<void*, double&>(&C::f38);
  test_pf_pmf_two_arguments<long double, int, float>(&C::f39);
  test_pf_pmf_result_type_only<const int&>(&C::f40);

  test_pf_pmf_result_type_only<void>(&C::f41);
  test_pf_pmf_result_type_only<void*>(&C::f42);
  test_pf_pmf_result_type_only<long double>(&C::f43);
  test_pf_pmf_result_type_only<const int&>(&C::f44);

  test_fn_result_type_only<void>(C::f37);
  test_fn_one_argument<void*, double&>(C::f38);
  test_fn_two_arguments<long double, int, float>(C::f39);
  test_fn_result_type_only<const int&>(C::f40);

  test_fn_result_type_only<void>(C::f41);
  test_fn_result_type_only<void*>(C::f42);
  test_fn_result_type_only<long double>(C::f43);
  test_fn_result_type_only<const int&>(C::f44);
}

void test_func() {
  test_func_noexcept<false>();
  test_func_noexcept<true>();
}
