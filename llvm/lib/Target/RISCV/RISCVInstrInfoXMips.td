//===-- RISCVInstrInfoXMips.td -----------------------------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes the vendor extensions defined by MIPS.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Operand definitions.
//===----------------------------------------------------------------------===//

// A 7-bit unsigned immediate where the least significant three bits are zero.
def uimm7_lsb000 : RISCVOp,
                   ImmLeaf<XLenVT, [{return isShiftedUInt<4, 3>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<7, "Lsb000">;
  let EncoderMethod = "getImmOpValue";
  let DecoderMethod = "decodeUImmOperand<7>";
  let OperandType = "OPERAND_UIMM7_LSB000";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (!MCOp.evaluateAsConstantImm(Imm))
      return false;
    return isShiftedUInt<4, 3>(Imm);
  }];
}

//===----------------------------------------------------------------------===//
// MIPS custom instruction formats
//===----------------------------------------------------------------------===//

// Load double pair format.
class LDPFormat<dag outs, dag ins, string opcodestr, string argstr>
    : RVInst<outs, ins, opcodestr, argstr, [], InstFormatI> {
  bits<7> imm7;
  bits<5> rs1;
  bits<5> rd1;
  bits<5> rd2;

  let Inst{31-27} = rd2;
  let Inst{26-23} = imm7{6-3};
  let Inst{22-20} = 0b000;
  let Inst{19-15} = rs1;
  let Inst{14-12} = 0b100;
  let Inst{11-7} = rd1;
  let Inst{6-0} = OPC_CUSTOM_0.Value;
}

// Load word pair format.
class LWPFormat<dag outs, dag ins, string opcodestr, string argstr>
    : RVInst<outs, ins, opcodestr, argstr, [], InstFormatI> {
  bits<7> imm7;
  bits<5> rs1;
  bits<5> rd1;
  bits<5> rd2;

  let Inst{31-27} = rd2;
  let Inst{26-22} = imm7{6-2};
  let Inst{21-20} = 0b01;
  let Inst{19-15} = rs1;
  let Inst{14-12} = 0b100;
  let Inst{11-7} = rd1;
  let Inst{6-0} = OPC_CUSTOM_0.Value;
}

// Store double pair format.
class SDPFormat<dag outs, dag ins, string opcodestr, string argstr>
    : RVInst<outs, ins, opcodestr, argstr, [], InstFormatI> {
  bits<7> imm7;
  bits<5> rs3;
  bits<5> rs2;
  bits<5> rs1;

  let Inst{31-27} = rs3;
  let Inst{26-25} = imm7{6-5};
  let Inst{24-20} = rs2;
  let Inst{19-15} = rs1;
  let Inst{14-12} = 0b101;
  let Inst{11-10} = imm7{4-3};
  let Inst{9-7} = 0b000;
  let Inst{6-0} = OPC_CUSTOM_0.Value;
}

// Store word pair format.
class SWPFormat<dag outs, dag ins, string opcodestr, string argstr>
    : RVInst<outs, ins, opcodestr, argstr, [], InstFormatI> {
  bits<7> imm7;
  bits<5> rs3;
  bits<5> rs2;
  bits<5> rs1;

  let Inst{31-27} = rs3;
  let Inst{26-25} = imm7{6-5};
  let Inst{24-20} = rs2;
  let Inst{19-15} = rs1;
  let Inst{14-12} = 0b101;
  let Inst{11-9} = imm7{4-2};
  let Inst{8-7} = 0b01;
  let Inst{6-0} = OPC_CUSTOM_0.Value;
}

//===----------------------------------------------------------------------===//
// MIPS extensions
//===----------------------------------------------------------------------===//

let Predicates = [HasVendorXMIPSCMov], hasSideEffects = 0, mayLoad = 0, mayStore = 0,
                 DecoderNamespace = "Xmipscmov" in {
def MIPS_CCMOV : RVInstR4<0b11, 0b011, OPC_CUSTOM_0, (outs GPR:$rd),
                          (ins GPR:$rs1, GPR:$rs2, GPR:$rs3),
                          "mips.ccmov", "$rd, $rs2, $rs1, $rs3">,
                 Sched<[]>;
}

let Predicates = [UseCCMovInsn] in {
def : Pat<(select (riscv_setne (XLenVT GPR:$rs2)),
                  (XLenVT GPR:$rs1), (XLenVT GPR:$rs3)),
          (MIPS_CCMOV GPR:$rs1, GPR:$rs2, GPR:$rs3)>;
def : Pat<(select (riscv_seteq (XLenVT GPR:$rs2)),
                  (XLenVT GPR:$rs3), (XLenVT GPR:$rs1)),
          (MIPS_CCMOV GPR:$rs1, GPR:$rs2, GPR:$rs3)>;

def : Pat<(select (XLenVT GPR:$rs2), (XLenVT GPR:$rs1), (XLenVT GPR:$rs3)),
          (MIPS_CCMOV GPR:$rs1, GPR:$rs2, GPR:$rs3)>;
}

let Predicates = [HasVendorXMIPSLSP], hasSideEffects = 0,
                 DecoderNamespace = "Xmipslsp" in {
let mayLoad = 1, mayStore = 0 in {
def MIPS_LWP : LWPFormat<(outs GPR:$rd1, GPR:$rd2), (ins GPR:$rs1, uimm7_lsb00:$imm7),
                         "mips.lwp", "$rd1, $rd2, ${imm7}(${rs1})">,
               Sched<[WriteLDW, WriteLDW, ReadMemBase]>;
def MIPS_LDP : LDPFormat<(outs GPR:$rd1, GPR:$rd2), (ins GPR:$rs1, uimm7_lsb000:$imm7),
                         "mips.ldp", "$rd1, $rd2, ${imm7}(${rs1})">,
               Sched<[WriteLDD, WriteLDD, ReadMemBase]>;
} // mayLoad = 1, mayStore = 0

let mayLoad = 0, mayStore = 1 in {
def MIPS_SWP : SWPFormat<(outs), (ins GPR:$rs2, GPR:$rs3, GPR:$rs1, uimm7_lsb00:$imm7),
                         "mips.swp", "$rs2, $rs3, ${imm7}(${rs1})">,
               Sched<[WriteSTW, ReadStoreData, ReadStoreData, ReadMemBase]>;
def MIPS_SDP : SDPFormat<(outs), (ins GPR:$rs2, GPR:$rs3, GPR:$rs1, uimm7_lsb000:$imm7),
                         "mips.sdp", "$rs2, $rs3, ${imm7}(${rs1})">,
               Sched<[WriteSTD, ReadStoreData, ReadStoreData, ReadMemBase]>;
} // mayLoad = 0, mayStore = 1
} // Predicates = [HasVendorXMIPSLSP], hasSideEffects = 0, DecoderNamespace = "Xmipslsp"
