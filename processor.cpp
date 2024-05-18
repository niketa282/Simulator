#include <cassert>
#include <iostream>
#include "processor.h"

void Emulator::Processor::runInstruction(Instruction const& instr) {
switch(instr.op) {

case Instruction::NOP:
  break;

case Instruction::ADD:
{
  auto add = static_cast<int>(registerBank[instr.srcReg1]) + static_cast<int>(registerBank[instr.srcReg2]);
  if(add > MAX) {
    overflow = true;
  }
  registerBank[instr.destReg] = registerBank[instr.srcReg1] + registerBank[instr.srcReg2];
  break;
}

case Instruction::SUB:
{
  auto diff =  static_cast<int>(registerBank[instr.srcReg1]) - static_cast<int>(registerBank[instr.srcReg2]);
  if(diff < MIN) {
     underflow = true;
  }
  registerBank[instr.destReg] = registerBank[instr.srcReg1] - registerBank[instr.srcReg2];
  break;
}

case Instruction::LOAD:
{
  registerBank[instr.destReg] = m.Read8BitMemory(instr.address);
  break;
}

case Instruction::LDI:
{
  registerBank[instr.destReg] = instr.immediate;
  break;
}

case Instruction::STORE:
{
  m.Write8BitMemory(instr.address, registerBank[instr.destReg]);
  break;
}


}
}