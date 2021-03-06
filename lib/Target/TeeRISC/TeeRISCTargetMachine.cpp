//===-- TeeRISCTargetMachine.cpp - Define TargetMachine for TeeRISC -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#include "TeeRISCTargetMachine.h"
#include "TeeRISC.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/PassManager.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

extern "C" void LLVMInitializeTeeRISCTarget() {
  // Register the target.
  RegisterTargetMachine<TeeRISCTargetMachine> X(TheTeeRISCTarget);
}

/// TeeRISCTargetMachine ctor - Create an ILP32 architecture model
///
TeeRISCTargetMachine::TeeRISCTargetMachine(const Target &T, StringRef TT,
                                       StringRef CPU, StringRef FS,
                                       const TargetOptions &Options,
                                       Reloc::Model RM, CodeModel::Model CM,
                                       CodeGenOpt::Level OL)
  : LLVMTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL),
    Subtarget(TT, CPU, FS),
    DL(Subtarget.getDataLayout()),
    InstrInfo(Subtarget),
    TLInfo(*this), TSInfo(*this),
    FrameLowering(Subtarget) {
  initAsmInfo();
}

namespace {
/// TeeRISC Code Generator Pass Configuration Options.
class TeeRISCPassConfig : public TargetPassConfig {
public:
  TeeRISCPassConfig(TeeRISCTargetMachine *TM, PassManagerBase &PM)
    : TargetPassConfig(TM, PM) {}

  TeeRISCTargetMachine &getTeeRISCTargetMachine() const {
    return getTM<TeeRISCTargetMachine>();
  }

  virtual bool addInstSelector();
  virtual bool addPreEmitPass();
};
} // namespace

TargetPassConfig *TeeRISCTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new TeeRISCPassConfig(this, PM);
}

bool TeeRISCPassConfig::addInstSelector() {
  addPass(createTeeRISCISelDag(getTeeRISCTargetMachine()));
  return false;
}

/// addPreEmitPass - This pass may be implemented by targets that want to run
/// passes immediately before machine code is emitted.  This should return
/// true if -print-machineinstrs should print out the code after the passes.
bool TeeRISCPassConfig::addPreEmitPass(){
  return true;
}

