//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <cstddef>
#include <type_traits>
#include "test_macros.h"

// REQUIRES: std-at-least-c++17

// std::byte is not an integer type, nor a character type.
// It is a distinct scoped enumeration type for accessing the bits that ultimately make up object storage.

static_assert(std::is_enum_v<std::byte>);
#if TEST_STD_VER >= 23
static_assert(std::is_scoped_enum_v<std::byte>);
#endif
static_assert(std::is_same_v<std::underlying_type_t<std::byte>, unsigned char>);
static_assert(!std::is_convertible_v<std::byte, unsigned char>);
static_assert(sizeof(std::byte) == 1);

static_assert(std::is_trivially_copyable_v<std::byte>);
static_assert(std::is_trivially_default_constructible_v<std::byte>);
#if TEST_STD_VER < 26 // P3247R2
static_assert(std::is_trivial_v<std::byte>);
#endif
#if TEST_STD_VER < 20 // P0767R1
static_assert(std::is_pod_v<std::byte>);
#endif
static_assert(std::is_standard_layout_v<std::byte>);

static_assert(!std::is_arithmetic_v<std::byte>);
static_assert(!std::is_integral_v<std::byte>);

static_assert(!std::is_same_v<std::byte, char>);
static_assert(!std::is_same_v<std::byte, signed char>);
static_assert(!std::is_same_v<std::byte, unsigned char>);

int main(int, char**) { return 0; }
