//===- NVPTXProxyRegErasure.cpp - NVPTX Proxy Register Instruction Erasure -==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// The pass is needed to remove ProxyReg instructions and restore related
// registers. The instructions were needed at instruction selection stage to
// make sure that callseq_end nodes won't be removed as "dead nodes". This can
// happen when we expand instructions into libcalls and the call site doesn't
// care about the libcall chain. Call site cares about data flow only, and the
// latest data flow node happens to be before callseq_end. Therefore the node
// becomes dangling and "dead". The ProxyReg acts like an additional data flow
// node *after* the callseq_end in the chain and ensures that everything will be
// preserved.
//
//===----------------------------------------------------------------------===//

#include "NVPTX.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

using namespace llvm;

namespace {

struct NVPTXProxyRegErasure : public MachineFunctionPass {
  static char ID;
  NVPTXProxyRegErasure() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

  StringRef getPassName() const override {
    return "NVPTX Proxy Register Instruction Erasure";
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    MachineFunctionPass::getAnalysisUsage(AU);
  }
};

} // namespace

char NVPTXProxyRegErasure::ID = 0;

INITIALIZE_PASS(NVPTXProxyRegErasure, "nvptx-proxyreg-erasure",
                "NVPTX ProxyReg Erasure", false, false)

bool NVPTXProxyRegErasure::runOnMachineFunction(MachineFunction &MF) {
  SmallVector<MachineInstr *, 16> RemoveList;

  // ProxyReg instructions forward a register as another: `%dst = mov.iN %src`.
  // Bulk RAUW the `%dst` registers in two passes over the machine function.
  DenseMap<Register, Register> RAUWBatch;

  for (auto &BB : MF) {
    for (auto &MI : BB) {
      switch (MI.getOpcode()) {
      case NVPTX::ProxyRegB1:
      case NVPTX::ProxyRegB16:
      case NVPTX::ProxyRegB32:
      case NVPTX::ProxyRegB64: {
        auto &InOp = *MI.uses().begin();
        auto &OutOp = *MI.defs().begin();
        assert(InOp.isReg() && "ProxyReg input should be a register.");
        assert(OutOp.isReg() && "ProxyReg output should be a register.");
        RemoveList.push_back(&MI);
        Register replacement = InOp.getReg();
        // Check if the replacement itself has been replaced.
        if (auto it = RAUWBatch.find(replacement); it != RAUWBatch.end())
          replacement = it->second;
        RAUWBatch.try_emplace(OutOp.getReg(), replacement);
        break;
      }
      }
    }
  }

  // If there were no proxy instructions, exit early.
  if (RemoveList.empty())
    return false;

  // Erase the proxy instructions first.
  for (auto *MI : RemoveList) {
    MI->eraseFromParent();
  }

  // Now go replace the registers.
  for (auto &BB : MF) {
    for (auto &MI : BB) {
      for (auto &Op : MI.uses()) {
        if (!Op.isReg())
          continue;
        auto it = RAUWBatch.find(Op.getReg());
        if (it != RAUWBatch.end())
          Op.setReg(it->second);
      }
    }
  }

  return true;
}

MachineFunctionPass *llvm::createNVPTXProxyRegErasurePass() {
  return new NVPTXProxyRegErasure();
}
