#include "processor.h"
#include <cassert>
#include <iostream>

void Emulator::Processor::runInstruction(Instruction const& instr) {
switch(instr.op) {

case Instruction::NOP:
  break;

case Instruction::ADD:
 registerBank[instr.operandRegNum[0]] = registerBank[instr.operandRegNum[1]] + registerBank[instr.operandRegNum[2]];
  if(registerBank[instr.operandRegNum[0]] == MIN) {
    overflow = true;
  }
  break;

case Instruction::SUB:
  registerBank[instr.operandRegNum[0]] = registerBank[instr.operandRegNum[1]] - registerBank[instr.operandRegNum[2]];
  if(registerBank[instr.operandRegNum[0]] == MAX) {
    underflow = true;
  }
  break;

}

}