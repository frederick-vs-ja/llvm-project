// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___FUNCTIONAL_WEAK_RESULT_TYPE_H
#define _LIBCPP___FUNCTIONAL_WEAK_RESULT_TYPE_H

#include <__config>
#include <__functional/binary_function.h>
#include <__functional/unary_function.h>
#include <__type_traits/invoke.h>
#include <__type_traits/is_same.h>
#include <__type_traits/void_t.h>
#include <__utility/declval.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp, class = void>
inline const bool __has_result_type_v = false;

template <class _Tp>
inline const bool __has_result_type_v<_Tp, __void_t<typename _Tp::result_type> > = true;

// __weak_result_type

template <class _Tp>
struct __derives_from_unary_function {
private:
  static void __find_base(...);
  template <class _Ap, class _Rp>
  static __unary_function<_Ap, _Rp> __find_base(const volatile __unary_function<_Ap, _Rp>*);

public:
  using type              = decltype(__find_base(static_cast<_Tp*>(nullptr)));
  static const bool value = !is_same<type, void>::value;
};

template <class _Tp>
struct __derives_from_binary_function {
private:
  static void __find_base(...);
  template <class _A1, class _A2, class _Rp>
  static __binary_function<_A1, _A2, _Rp> __find_base(const volatile __binary_function<_A1, _A2, _Rp>*);

public:
  using type              = decltype(__find_base(static_cast<_Tp*>(nullptr)));
  static const bool value = !is_same<type, void>::value;
};

template <class _Tp, bool = __derives_from_unary_function<_Tp>::value>
struct __maybe_derive_from_unary_function // bool is true
    : public __derives_from_unary_function<_Tp>::type {};

template <class _Tp>
struct __maybe_derive_from_unary_function<_Tp, false> {};

_LIBCPP_SUPPRESS_DEPRECATED_PUSH
template <class _Tp, bool = __derives_from_binary_function<_Tp>::value>
struct __maybe_derive_from_binary_function // bool is true
    : public __derives_from_binary_function<_Tp>::type {};

template <class _Tp>
struct __maybe_derive_from_binary_function<_Tp, false> {};

template <class _Tp, bool = __has_result_type_v<_Tp> >
struct __weak_result_type_imp // bool is true
    : public __maybe_derive_from_unary_function<_Tp>,
      public __maybe_derive_from_binary_function<_Tp> {
#if _LIBCPP_STD_VER <= 17 || defined(_LIBCPP_ENABLE_CXX20_REMOVED_BINDER_TYPEDEFS)
  using result_type _LIBCPP_NODEBUG _LIBCPP_DEPRECATED_IN_CXX17 = typename _Tp::result_type;
#endif
};

template <class _Tp>
struct __weak_result_type_imp<_Tp, false>
    : public __maybe_derive_from_unary_function<_Tp>, public __maybe_derive_from_binary_function<_Tp> {};

template <class _Tp>
struct __weak_result_type : public __weak_result_type_imp<_Tp> {};
_LIBCPP_SUPPRESS_DEPRECATED_POP

#if _LIBCPP_STD_VER >= 17
#  define _LIBCPP_TEMPLATE_IS_NOEXCEPT_PARAM , bool _IsNoexcept
#  define _LIBCPP_TEMPALTE_NOEXCEPT_SPEC noexcept(_IsNoexcept)
#else
#  define _LIBCPP_TEMPLATE_IS_NOEXCEPT_PARAM
#  define _LIBCPP_TEMPALTE_NOEXCEPT_SPEC
#endif

#if _LIBCPP_STD_VER <= 17 || defined(_LIBCPP_ENABLE_CXX20_REMOVED_BINDER_TYPEDEFS)
#  define _LIBCPP_DECL_MEM_ARGUMENT_TYPE(t) using argument_type _LIBCPP_DEPRECATED_IN_CXX17 = t
#  define _LIBCPP_DECL_MEM_FIRST_ARGUMENT_TYPE(t) using first_argument_type _LIBCPP_DEPRECATED_IN_CXX17 = t
#  define _LIBCPP_DECL_MEM_SECOND_ARGUMENT_TYPE(t) using second_argument_type _LIBCPP_DEPRECATED_IN_CXX17 = t
#  define _LIBCPP_DECL_MEM_RESULT_TYPE(t) using result_type _LIBCPP_DEPRECATED_IN_CXX17 = t
#else
#  define _LIBCPP_DECL_MEM_ARGUMENT_TYPE(t)
#  define _LIBCPP_DECL_MEM_FIRST_ARGUMENT_TYPE(t)
#  define _LIBCPP_DECL_MEM_SECOND_ARGUMENT_TYPE(t)
#  define _LIBCPP_DECL_MEM_RESULT_TYPE(t)
#endif

// 0 argument case

template <class _Rp _LIBCPP_TEMPLATE_IS_NOEXCEPT_PARAM>
struct __weak_result_type<_Rp() _LIBCPP_TEMPALTE_NOEXCEPT_SPEC> {
  _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);
};

#define _LIBCPP_DEF_WEAK_RESULT_TYPE_PF_0_TOPCV(topcv)                                                                 \
  template <class _Rp _LIBCPP_TEMPLATE_IS_NOEXCEPT_PARAM>                                                              \
  struct __weak_result_type<_Rp (*topcv)() _LIBCPP_TEMPALTE_NOEXCEPT_SPEC> {                                           \
    _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);                                                                                 \
  };

_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_0_TOPCV()
_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_0_TOPCV(const)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_0_TOPCV(volatile)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_0_TOPCV(const volatile)

#undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PF_0_TOPCV

// 1 argument case
// For ABI compatibility, the partial specialization is derived from __unary_function when there is no top-level
// cv-qualifiers, no ref-qualifies, and no noexcept.

template <class _Rp, class _A1>
struct __weak_result_type<_Rp(_A1)> : public __unary_function<_A1, _Rp> {};

template <class _Rp, class _A1>
struct __weak_result_type<_Rp (*)(_A1)> : public __unary_function<_A1, _Rp> {};

#define _LIBCPP_DEF_WEAK_RESULT_TYPE_PF_1_NONNOEXCEPT_TOPCV(topcv)                                                     \
  template <class _Rp, class _A1>                                                                                      \
  struct __weak_result_type<_Rp (*topcv)(_A1)> {                                                                       \
    _LIBCPP_DECL_MEM_ARGUMENT_TYPE(_A1);                                                                               \
    _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);                                                                                 \
  };

_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_1_NONNOEXCEPT_TOPCV(const)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_1_NONNOEXCEPT_TOPCV(volatile)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_1_NONNOEXCEPT_TOPCV(const volatile)

#undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PF_1_NONNOEXCEPT_TOPCV

#define _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_LEGACY(cv)                                                                  \
  template <class _Rp, class _Cp>                                                                                      \
  struct __weak_result_type<_Rp (_Cp::*)() cv> : public __unary_function<cv _Cp*, _Rp> {};

_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_LEGACY()
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_LEGACY(const)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_LEGACY(volatile)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_LEGACY(const volatile)

#undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_LEGACY

#define _LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_1_NONNOEXCEPT_TOPCV(topcv, cv)                                                \
  template <class _Rp, class _Cp>                                                                                      \
  struct __weak_result_type<_Rp (_Cp::*topcv)() cv> {                                                                  \
    _LIBCPP_DECL_MEM_ARGUMENT_TYPE(cv _Cp*);                                                                           \
    _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);                                                                                 \
  };

#define _LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_1_NONNOEXCEPT(cv)                                                             \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_1_NONNOEXCEPT_TOPCV(const, cv)                                                      \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_1_NONNOEXCEPT_TOPCV(volatile, cv)                                                   \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_1_NONNOEXCEPT_TOPCV(const volatile, cv)

_LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_1_NONNOEXCEPT()
_LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_1_NONNOEXCEPT(const)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_1_NONNOEXCEPT(volatile)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_1_NONNOEXCEPT(const volatile)

#undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_1_NONNOEXCEPT
#undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_1_NONNOEXCEPT_TOPCV

#if _LIBCPP_STD_VER >= 17
template <class _Rp, class _A1>
struct __weak_result_type<_Rp(_A1) noexcept> {
  _LIBCPP_DECL_MEM_ARGUMENT_TYPE(_A1);
  _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);
};

#  define _LIBCPP_DEF_WEAK_RESULT_TYPE_PF_1_NOEXCEPT_TOP_CV(topcv)                                                     \
    template <class _Rp, class _A1>                                                                                    \
    struct __weak_result_type<_Rp (*topcv)(_A1) noexcept> {                                                            \
      _LIBCPP_DECL_MEM_ARGUMENT_TYPE(_A1);                                                                             \
      _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);                                                                               \
    };

_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_1_NOEXCEPT_TOP_CV()
_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_1_NOEXCEPT_TOP_CV(const)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_1_NOEXCEPT_TOP_CV(volatile)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_1_NOEXCEPT_TOP_CV(const volatile)

#  undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PF_1_NOEXCEPT_TOP_CV

#  define _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_NOEXCEPT_TOPCV(topcv, cv)                                                 \
    template <class _Rp, class _Cp>                                                                                    \
    struct __weak_result_type<_Rp (_Cp::*topcv)() cv noexcept> {                                                       \
      _LIBCPP_DECL_MEM_ARGUMENT_TYPE(cv _Cp*);                                                                         \
      _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);                                                                               \
    };

#  define _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_NOEXCEPT(cv)                                                              \
    _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_NOEXCEPT_TOPCV(, cv)                                                            \
    _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_NOEXCEPT_TOPCV(const, cv)                                                       \
    _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_NOEXCEPT_TOPCV(volatile, cv)                                                    \
    _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_NOEXCEPT_TOPCV(const volatile, cv)

_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_NOEXCEPT()
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_NOEXCEPT(const)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_NOEXCEPT(volatile)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_NOEXCEPT(const volatile)

#  undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_NOEXCEPT
#  undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_NOEXCEPT_TOPCV
#endif

#define _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_TOP_CV(topcv, cv, ref)                                                      \
  template <class _Rp _LIBCPP_TEMPLATE_IS_NOEXCEPT_PARAM, class _Cp>                                                   \
  struct __weak_result_type<_Rp (_Cp::*topcv)() cv ref _LIBCPP_TEMPALTE_NOEXCEPT_SPEC> {                               \
    _LIBCPP_DECL_MEM_ARGUMENT_TYPE(cv _Cp*);                                                                           \
    _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);                                                                                 \
  };

#define _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1(cv, ref)                                                                    \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_TOP_CV(, cv, ref)                                                                 \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_TOP_CV(const, cv, ref)                                                            \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_TOP_CV(volatile, cv, ref)                                                         \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_TOP_CV(const volatile, cv, ref)

_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1(, &)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1(, &&)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1(const, &)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1(const, &&)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1(volatile, &)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1(volatile, &&)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1(const volatile, &)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1(const volatile, &&)

#undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1
#undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_1_TOP_CV

// 2 argument case
// For ABI compatibility, the partial specialization is derived from __binary_function when there is no top-level
// cv-qualifiers, no ref-qualifies, and no noexcept.

template <class _Rp, class _A1, class _A2>
struct __weak_result_type<_Rp(_A1, _A2)> : public __binary_function<_A1, _A2, _Rp> {};

template <class _Rp, class _A1, class _A2>
struct __weak_result_type<_Rp (*)(_A1, _A2)> : public __binary_function<_A1, _A2, _Rp> {};

#define _LIBCPP_DEF_WEAK_RESULT_TYPE_PF_2_NONNOEXCEPT_TOPCV(topcv)                                                     \
  template <class _Rp, class _A1, class _A2>                                                                           \
  struct __weak_result_type<_Rp (*topcv)(_A1, _A2)> {                                                                  \
    _LIBCPP_DECL_MEM_FIRST_ARGUMENT_TYPE(_A1);                                                                         \
    _LIBCPP_DECL_MEM_SECOND_ARGUMENT_TYPE(_A2);                                                                        \
    _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);                                                                                 \
  };

_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_2_NONNOEXCEPT_TOPCV(const)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_2_NONNOEXCEPT_TOPCV(volatile)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_2_NONNOEXCEPT_TOPCV(const volatile)

#undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PF_2_NONNOEXCEPT_TOPCV

#define _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_LEGACY(cv)                                                                  \
  template <class _Rp, class _Cp, class _A1>                                                                           \
  struct __weak_result_type<_Rp (_Cp::*)(_A1) cv> : public __binary_function<cv _Cp*, _A1, _Rp> {};

_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_LEGACY()
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_LEGACY(const)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_LEGACY(volatile)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_LEGACY(const volatile)

#undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_LEGACY

#define _LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_2_NONNOEXCEPT_TOPCV(topcv, cv)                                                \
  template <class _Rp, class _Cp, class _A1>                                                                           \
  struct __weak_result_type<_Rp (_Cp::*topcv)(_A1) cv> {                                                               \
    _LIBCPP_DECL_MEM_FIRST_ARGUMENT_TYPE(cv _Cp*);                                                                     \
    _LIBCPP_DECL_MEM_SECOND_ARGUMENT_TYPE(_A1);                                                                        \
    _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);                                                                                 \
  };

#define _LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_2_NONNOEXCEPT(cv)                                                             \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_2_NONNOEXCEPT_TOPCV(const, cv)                                                      \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_2_NONNOEXCEPT_TOPCV(volatile, cv)                                                   \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_2_NONNOEXCEPT_TOPCV(const volatile, cv)

_LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_2_NONNOEXCEPT()
_LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_2_NONNOEXCEPT(const)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_2_NONNOEXCEPT(volatile)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_2_NONNOEXCEPT(const volatile)

#undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_2_NONNOEXCEPT
#undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PFM_2_NONNOEXCEPT_TOPCV

#if _LIBCPP_STD_VER >= 17
template <class _Rp, class _A1, class _A2>
struct __weak_result_type<_Rp(_A1, _A2) noexcept> {
  _LIBCPP_DECL_MEM_FIRST_ARGUMENT_TYPE(_A1);
  _LIBCPP_DECL_MEM_SECOND_ARGUMENT_TYPE(_A2);
  _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);
};

#  define _LIBCPP_DEF_WEAK_RESULT_TYPE_PF_2_NOEXCEPT_TOP_CV(topcv)                                                     \
    template <class _Rp, class _A1, class _A2>                                                                         \
    struct __weak_result_type<_Rp (*topcv)(_A1, _A2) noexcept> {                                                       \
      _LIBCPP_DECL_MEM_FIRST_ARGUMENT_TYPE(_A1);                                                                       \
      _LIBCPP_DECL_MEM_SECOND_ARGUMENT_TYPE(_A2);                                                                      \
      _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);                                                                               \
    };

_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_2_NOEXCEPT_TOP_CV()
_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_2_NOEXCEPT_TOP_CV(const)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_2_NOEXCEPT_TOP_CV(volatile)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_2_NOEXCEPT_TOP_CV(const volatile)

#  undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PF_2_NOEXCEPT_TOP_CV

#  define _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_NOEXCEPT_TOP_CV(topcv, cv)                                                \
    template <class _Rp, class _Cp, class _A1>                                                                         \
    struct __weak_result_type<_Rp (_Cp::*topcv)(_A1) cv noexcept> {                                                    \
      _LIBCPP_DECL_MEM_FIRST_ARGUMENT_TYPE(cv _Cp*);                                                                   \
      _LIBCPP_DECL_MEM_SECOND_ARGUMENT_TYPE(_A1);                                                                      \
      _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);                                                                               \
    };

#  define _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_NOEXCEPT(cv)                                                              \
    _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_NOEXCEPT_TOP_CV(, cv)                                                           \
    _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_NOEXCEPT_TOP_CV(const, cv)                                                      \
    _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_NOEXCEPT_TOP_CV(volatile, cv)                                                   \
    _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_NOEXCEPT_TOP_CV(const volatile, cv)

_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_NOEXCEPT();
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_NOEXCEPT(const);
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_NOEXCEPT(volatile);
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_NOEXCEPT(const volatile);

#  undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_NOEXCEPT
#  undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_NOEXCEPT_TOP_CV
#endif

#define _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_TOP_CV(topcv, cv, ref)                                                      \
  template <class _Rp _LIBCPP_TEMPLATE_IS_NOEXCEPT_PARAM, class _Cp, class _A1>                                        \
  struct __weak_result_type<_Rp (_Cp::*topcv)(_A1) cv ref _LIBCPP_TEMPALTE_NOEXCEPT_SPEC> {                            \
    _LIBCPP_DECL_MEM_FIRST_ARGUMENT_TYPE(cv _Cp*);                                                                     \
    _LIBCPP_DECL_MEM_SECOND_ARGUMENT_TYPE(_A1);                                                                        \
    _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);                                                                                 \
  };

#define _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2(cv, ref)                                                                    \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_TOP_CV(, cv, ref)                                                                 \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_TOP_CV(const, cv, ref)                                                            \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_TOP_CV(volatile, cv, ref)                                                         \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_TOP_CV(const volatile, cv, ref)

_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2(, &);
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2(, &&);
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2(const, &);
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2(const, &&);
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2(volatile, &);
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2(volatile, &&);
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2(const volatile, &);
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2(const volatile, &&);

#undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2
#undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_2_TOP_CV

// 3 or more arguments

template <class _Rp _LIBCPP_TEMPLATE_IS_NOEXCEPT_PARAM, class _A1, class _A2, class _A3, class... _A4>
struct __weak_result_type<_Rp(_A1, _A2, _A3, _A4...) _LIBCPP_TEMPALTE_NOEXCEPT_SPEC> {
  _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);
};

#define _LIBCPP_DEF_WEAK_RESULT_TYPE_PF_3_OR_MORE_TOP_CV(topcv)                                                        \
  template <class _Rp _LIBCPP_TEMPLATE_IS_NOEXCEPT_PARAM, class _A1, class _A2, class _A3, class... _A4>               \
  struct __weak_result_type<_Rp (*topcv)(_A1, _A2, _A3, _A4...) _LIBCPP_TEMPALTE_NOEXCEPT_SPEC> {                      \
    _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);                                                                                 \
  };

_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_3_OR_MORE_TOP_CV()
_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_3_OR_MORE_TOP_CV(const)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_3_OR_MORE_TOP_CV(volatile)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_3_OR_MORE_TOP_CV(const volatile)

#undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PF_3_OR_MORE_TOP_CV

#define _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE_TOP_CV(topcv, cvref)                                                \
  template <class _Rp _LIBCPP_TEMPLATE_IS_NOEXCEPT_PARAM, class _Cp, class _A1, class _A2, class... _A3>               \
  struct __weak_result_type<_Rp (_Cp::*topcv)(_A1, _A2, _A3...) cvref _LIBCPP_TEMPALTE_NOEXCEPT_SPEC> {                \
    _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);                                                                                 \
  };

#define _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE(cvref)                                                              \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE_TOP_CV(, cvref)                                                           \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE_TOP_CV(const, cvref)                                                      \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE_TOP_CV(volatile, cvref)                                                   \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE_TOP_CV(const volatile, cvref)

_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE()
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE(&)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE(&&)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE(const)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE(const&)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE(const&&)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE(volatile)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE(volatile&)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE(volatile&&)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE(const volatile)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE(const volatile&)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE(const volatile&&)

#undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE
#undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_3_OR_MORE_TOP_CV

// variadic arguments

template <class _Rp _LIBCPP_TEMPLATE_IS_NOEXCEPT_PARAM, class... _As>
struct __weak_result_type<_Rp(_As..., ...) _LIBCPP_TEMPALTE_NOEXCEPT_SPEC> {
  _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);
};

#define _LIBCPP_DEF_WEAK_RESULT_TYPE_PF_VARIADIC_TOP_CV(topcv)                                                         \
  template <class _Rp _LIBCPP_TEMPLATE_IS_NOEXCEPT_PARAM, class... _As>                                                \
  struct __weak_result_type<_Rp (*topcv)(_As..., ...) _LIBCPP_TEMPALTE_NOEXCEPT_SPEC> {                                \
    _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);                                                                                 \
  };

_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_VARIADIC_TOP_CV()
_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_VARIADIC_TOP_CV(const)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_VARIADIC_TOP_CV(volatile)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PF_VARIADIC_TOP_CV(const volatile)

#undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PF_VARIADIC_TOP_CV

#define _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC_TOP_CV(topcv, cvref)                                                 \
  template <class _Rp _LIBCPP_TEMPLATE_IS_NOEXCEPT_PARAM, class _Cp, class... _As>                                     \
  struct __weak_result_type<_Rp (_Cp::*topcv)(_As..., ...) cvref _LIBCPP_TEMPALTE_NOEXCEPT_SPEC> {                     \
    _LIBCPP_DECL_MEM_RESULT_TYPE(_Rp);                                                                                 \
  };

#define _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC(cvref)                                                               \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC_TOP_CV(, cvref)                                                            \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC_TOP_CV(const, cvref)                                                       \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC_TOP_CV(volatile, cvref)                                                    \
  _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC_TOP_CV(const volatile, cvref)

_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC()
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC(&)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC(&&)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC(const)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC(const&)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC(const&&)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC(volatile)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC(volatile&)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC(volatile&&)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC(const volatile)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC(const volatile&)
_LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC(const volatile&&)

#undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC
#undef _LIBCPP_DEF_WEAK_RESULT_TYPE_PMF_VARIADIC_TOP_CV

#undef _LIBCPP_DECL_MEM_RESULT_TYPE
#undef _LIBCPP_DECL_MEM_SECOND_ARGUMENT_TYPE
#undef _LIBCPP_DECL_MEM_FIRST_ARGUMENT_TYPE
#undef _LIBCPP_DECL_MEM_ARGUMENT_TYPE

#undef _LIBCPP_TEMPALTE_NOEXCEPT_SPEC
#undef _LIBCPP_TEMPLATE_IS_NOEXCEPT_PARAM

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___FUNCTIONAL_WEAK_RESULT_TYPE_H
