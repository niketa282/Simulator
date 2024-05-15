#include "processor.h"
#include <cassert>
#include <iostream>

std::string Processor::runInstruction(Instruction const& instr) {
switch(instr.op) {
case Instruction::NOP:
    return "NOP";

case Instruction::Add:
     assert(instr.operandRegNum[0] < registerBank.size());
     assert(instr.operandRegNum[1] < registerBank.size());
     assert(instr.operandRegNum[2] < registerBank.size());
     registerBank[instr.operandRegNum[0]] = registerBank[instr.operandRegNum[1]] + registerBank[instr.operandRegNum[2]];
     std::cout << "dest reg val is " <<  registerBank[instr.operandRegNum[0]] << "\n";
     return "ADD";
  }
}