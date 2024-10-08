; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py UTC_ARGS: --version 4
; RUN: llc -force-streaming -verify-machineinstrs < %s | FileCheck %s

target triple = "aarch64-linux"

define void @mopa_bf16(<vscale x 8 x i1> %pn, <vscale x 8 x i1> %pm, <vscale x 8 x bfloat> %zn, <vscale x 8 x bfloat> %zm) #0 {
; CHECK-LABEL: mopa_bf16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    bfmopa za0.h, p0/m, p1/m, z0.h, z1.h
; CHECK-NEXT:    ret
  call void @llvm.aarch64.sme.mopa.nxv8bf16(i32 0, <vscale x 8 x i1> %pn, <vscale x 8 x i1> %pm, <vscale x 8 x bfloat> %zn, <vscale x 8 x bfloat> %zm)
  ret void
}

define void @mopa_f16(<vscale x 8 x i1> %pn, <vscale x 8 x i1> %pm, <vscale x 8 x half> %zn, <vscale x 8 x half> %zm) #0 {
; CHECK-LABEL: mopa_f16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    fmopa za1.h, p0/m, p1/m, z0.h, z1.h
; CHECK-NEXT:    ret
  call void @llvm.aarch64.sme.mopa.nxv8f16(i32 1, <vscale x 8 x i1> %pn, <vscale x 8 x i1> %pm, <vscale x 8 x half> %zn, <vscale x 8 x half> %zm)
  ret void
}

define void @mops_bf16(<vscale x 8 x i1> %pn, <vscale x 8 x i1> %pm, <vscale x 8 x bfloat> %zn, <vscale x 8 x bfloat> %zm) #0 {
; CHECK-LABEL: mops_bf16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    bfmops za0.h, p0/m, p1/m, z0.h, z1.h
; CHECK-NEXT:    ret
  call void @llvm.aarch64.sme.mops.nxv8bf16(i32 0, <vscale x 8 x i1> %pn, <vscale x 8 x i1> %pm, <vscale x 8 x bfloat> %zn, <vscale x 8 x bfloat> %zm)
  ret void
}

define void @mops_f16(<vscale x 8 x i1> %pn, <vscale x 8 x i1> %pm, <vscale x 8 x half> %zn, <vscale x 8 x half> %zm) #0 {
; CHECK-LABEL: mops_f16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    fmops za1.h, p0/m, p1/m, z0.h, z1.h
; CHECK-NEXT:    ret
  call void @llvm.aarch64.sme.mops.nxv8f16(i32 1, <vscale x 8 x i1> %pn, <vscale x 8 x i1> %pm, <vscale x 8 x half> %zn, <vscale x 8 x half> %zm)
  ret void
}

attributes #0 = {nounwind "target-features" = "+sme-b16b16,+sme2p1,+bf16,+sme-f16f16" }
