//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <clc/opencl/workitem/get_global_id.h>
#include <clc/opencl/workitem/get_group_id.h>
#include <clc/opencl/workitem/get_local_id.h>
#include <clc/opencl/workitem/get_local_size.h>

_CLC_DEF _CLC_OVERLOAD size_t get_global_id(uint dim) {
  return get_group_id(dim) * get_local_size(dim) + get_local_id(dim);
}
