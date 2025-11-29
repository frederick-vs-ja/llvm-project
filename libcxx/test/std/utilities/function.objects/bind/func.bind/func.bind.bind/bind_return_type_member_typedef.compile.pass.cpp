//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <functional>

// template<class F, class... BoundArgs>
//   constexpr unspecified bind(F&& f, BoundArgs&&... bound_args); // constexpr since C++20
// template<class R, class F, class... BoundArgs>
//   constexpr unspecified bind(F&& f, BoundArgs&&... bound_args); // constexpr since C++20

// C++17 [depr.func.adaptor.typedefs]
// 7. The forwarding call wrapper `g` returned by a call to `bind(f, bound_args...)` ([func.bind.bind]) shall have a
//   weak result type.
// 8. The forwarding call wrapper `g` returned by a call to `bind<R>(f, bound_args...)` ([func.bind.bind]) shall have a
//   nested type `result_type` defined as a synonym for `R`.

// REQUIRES: c++11 || c++14 || c++17

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

#include <functional>
#include <type_traits>

template <class...>
using void_t = void;

template <class T, class = void>
struct has_no_result_type : std::true_type {};
template <class T>
struct has_no_result_type<T, void_t<typename T::result_type>> : std::false_type {};

struct functor1 {
  template <class... Args>
  int operator()(Args&&...) const;
};

struct functor2 {
  using result_type = const int&;

  template <class... Args>
  int& operator()(Args&&...) const;
};

struct functor3 {
  using result_type = void() volatile&;

  template <class... Args>
  char* operator()(Args&&...) const;
};

static_assert(has_no_result_type<decltype(std::bind(functor1{}))>::value, "");
static_assert(has_no_result_type<decltype(std::bind(functor1{}, '*', 1729))>::value, "");
static_assert(std::is_same<decltype(std::bind(functor2{}))::result_type, const int&>::value, "");
static_assert(std::is_same<decltype(std::bind(functor2{}, '*', 1729))::result_type, const int&>::value, "");
static_assert(std::is_same<decltype(std::bind(functor3{}))::result_type, void() volatile&>::value, "");
static_assert(std::is_same<decltype(std::bind(functor3{}, '*', 1729))::result_type, void() volatile&>::value, "");

static_assert(std::is_same<decltype(std::bind<long>(functor1{}))::result_type, long>::value, "");
static_assert(std::is_same<decltype(std::bind<long>(functor1{}, 17.2f, 9.42))::result_type, long>::value, "");
static_assert(std::is_same<decltype(std::bind<void>(functor2{}))::result_type, void>::value, "");
static_assert(std::is_same<decltype(std::bind<void>(functor2{}, 17.2f, 9.42))::result_type, void>::value, "");
static_assert(std::is_same<decltype(std::bind<const char*>(functor2{}))::result_type, const char*>::value, "");
static_assert(std::is_same<decltype(std::bind<const char*>(functor2{}, 17.2f, 9.42))::result_type, const char*>::value,
              "");

template <bool IsNoexcept>
struct test_class {
  void f1() noexcept(IsNoexcept);
  int f2() const noexcept(IsNoexcept);
  int* f3() volatile noexcept(IsNoexcept);
  int& f4() const volatile noexcept(IsNoexcept);

  void f5(int) noexcept(IsNoexcept);
  int f6(int*) const noexcept(IsNoexcept);
  int* f7(long&) volatile noexcept(IsNoexcept);
  const int& f8(const short&) const volatile noexcept(IsNoexcept);

  void f9(...) noexcept(IsNoexcept);
  int f10(...) const noexcept(IsNoexcept);
  int* f11(...) volatile noexcept(IsNoexcept);
  volatile int& f12(...) const volatile noexcept(IsNoexcept);

  void f13(int, ...) noexcept(IsNoexcept);
  int f14(int*, ...) const noexcept(IsNoexcept);
  int* f15(long&, ...) volatile noexcept(IsNoexcept);
  const volatile int& f16(const int&, ...) const volatile noexcept(IsNoexcept);

  void f17(int, char, long) noexcept(IsNoexcept);
  int f18(char*, long*, const int*) const noexcept(IsNoexcept);
  int* f19(long&, const int&, char&) volatile noexcept(IsNoexcept);
  int& f20(const int&, volatile long&, const volatile char&) const volatile noexcept(IsNoexcept);

  void f21() & noexcept(IsNoexcept);
  int f22() const& noexcept(IsNoexcept);
  int* f23() volatile& noexcept(IsNoexcept);
  int& f24() const volatile& noexcept(IsNoexcept);

  void f25(int) & noexcept(IsNoexcept);
  int f26(int*) const& noexcept(IsNoexcept);
  int* f27(long&) volatile& noexcept(IsNoexcept);
  const int& f28(const char&) const volatile& noexcept(IsNoexcept);

  void f29() && noexcept(IsNoexcept);
  int f30() const&& noexcept(IsNoexcept);
  int* f31() volatile&& noexcept(IsNoexcept);
  volatile int&& f32() const volatile&& noexcept(IsNoexcept);

  void f33(int) && noexcept(IsNoexcept);
  int f34(int*) const&& noexcept(IsNoexcept);
  int* f35(long&&) volatile&& noexcept(IsNoexcept);
  const volatile int&& f36(const long&) const volatile&& noexcept(IsNoexcept);

  static void f37() noexcept(IsNoexcept);
  static void* f38(double&) noexcept(IsNoexcept);
  static long double f39(int, float) noexcept(IsNoexcept);
  static const int& f40(signed char, unsigned char, char) noexcept(IsNoexcept);

  static void f41(...) noexcept(IsNoexcept);
  static void* f42(double&, ...) noexcept(IsNoexcept);
  static long double f43(int, float, ...) noexcept(IsNoexcept);
  static volatile int&& f44(signed char, unsigned char, char, ...) noexcept(IsNoexcept);
};

template <class Result, class BindFn>
void test_member_result_type(BindFn) {
  static_assert(std::is_same<typename BindFn::result_type, Result>::value, "");
}

template <bool IsNoexcept>
void test_pf_pmf_noexcept() {
  using C = test_class<IsNoexcept>;

  test_member_result_type<void>(std::bind(&C::f1));
  test_member_result_type<int>(std::bind(&C::f2));
  test_member_result_type<int*>(std::bind(&C::f3));
  test_member_result_type<int&>(std::bind(&C::f4));

  test_member_result_type<void>(std::bind(&C::f5));
  test_member_result_type<int>(std::bind(&C::f6));
  test_member_result_type<int*>(std::bind(&C::f7));
  test_member_result_type<const int&>(std::bind(&C::f8));

  test_member_result_type<void>(std::bind(&C::f9));
  test_member_result_type<int>(std::bind(&C::f10));
  test_member_result_type<int*>(std::bind(&C::f11));
  test_member_result_type<volatile int&>(std::bind(&C::f12));

  test_member_result_type<void>(std::bind(&C::f13));
  test_member_result_type<int>(std::bind(&C::f14));
  test_member_result_type<int*>(std::bind(&C::f15));
  test_member_result_type<const volatile int&>(std::bind(&C::f16));

  test_member_result_type<void>(std::bind(&C::f17));
  test_member_result_type<int>(std::bind(&C::f18));
  test_member_result_type<int*>(std::bind(&C::f19));
  test_member_result_type<int&>(std::bind(&C::f20));

  test_member_result_type<void>(std::bind(&C::f21));
  test_member_result_type<int>(std::bind(&C::f22));
  test_member_result_type<int*>(std::bind(&C::f23));
  test_member_result_type<int&>(std::bind(&C::f24));

  test_member_result_type<void>(std::bind(&C::f25));
  test_member_result_type<int>(std::bind(&C::f6));
  test_member_result_type<int*>(std::bind(&C::f27));
  test_member_result_type<const int&>(std::bind(&C::f28));

  test_member_result_type<void>(std::bind(&C::f29));
  test_member_result_type<int>(std::bind(&C::f30));
  test_member_result_type<int*>(std::bind(&C::f31));
  test_member_result_type<volatile int&&>(std::bind(&C::f32));

  test_member_result_type<void>(std::bind(&C::f33));
  test_member_result_type<int>(std::bind(&C::f34));
  test_member_result_type<int*>(std::bind(&C::f35));
  test_member_result_type<const volatile int&&>(std::bind(&C::f36));

  test_member_result_type<void>(std::bind(&C::f37));
  test_member_result_type<void*>(std::bind(&C::f38));
  test_member_result_type<long double>(std::bind(&C::f39));
  test_member_result_type<const int&>(std::bind(&C::f40));

  test_member_result_type<void>(std::bind(&C::f41));
  test_member_result_type<void*>(std::bind(&C::f42));
  test_member_result_type<long double>(std::bind(&C::f43));
  test_member_result_type<volatile int&&>(std::bind(&C::f44));
}

void test_pf_pmf() {
  test_pf_pmf_noexcept<false>();
  test_pf_pmf_noexcept<true>();
}
