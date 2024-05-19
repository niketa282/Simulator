#include <cassert>
#include <iostream>
#include <sstream>
#include "processor.h"

void Emulator::Processor::runInstruction(Instruction const& instr) {
switch(instr.op) {

case Instruction::NOP:
  break;

case Instruction::ADD:
{
  auto add = static_cast<int>(registerBank[instr.Reg1]) + static_cast<int>(registerBank[instr.Reg2]);
  if(add > MAX) {
    overflow = true;
  }
  registerBank[instr.Reg3] = registerBank[instr.Reg1] + registerBank[instr.Reg2];
  break;
}

case Instruction::SUB:
{
  auto diff =  static_cast<int>(registerBank[instr.Reg1]) - static_cast<int>(registerBank[instr.Reg2]);
  if(diff < MIN) {
     underflow = true;
  }
  registerBank[instr.Reg3] = registerBank[instr.Reg1] - registerBank[instr.Reg2];
  break;
}

case Instruction::LOAD:
{
 registerBank[instr.Reg3] = m.Read8BitMemory(instr.immediateOrAddress);
  break;
}

case Instruction::LDI:
{
  registerBank[instr.Reg3] = instr.immediateOrAddress;
  break;
}

case Instruction::STORE:
{
  m.Write8BitMemory(instr.immediateOrAddress, registerBank[instr.Reg3]);
  break;
}

case Instruction::CMP:
{
  registerBank[instr.Reg1] == registerBank[instr.Reg2] ? equalFlag = true : equalFlag = false;
  break;
}

case Instruction::HALT:
{
  haltFlag = true;
  break;
}

case Instruction::JMP:
{
  ProgramCounter = instr.immediateOrAddress;
  break;
}

}
}

void Emulator::Processor::populateInstructionMemory(std::string const& filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Error opening file!" << "\n";
  }
  std::string str;
  while (getline(file, str))
  {
      size_t commentPos = str.find("//");
      if (commentPos != std::string::npos)
      {
          InstructionMemory.push_back(str.substr(0, commentPos));
      }
      else
      {
          InstructionMemory.push_back(str);
      }
  }
  file.close();

  for(auto& iMem: InstructionMemory)
  {
    assert(iMem.size() == BITS);
  }
}

/*void Emulator::Processor::Execute()
{
  while (ProgramCounter < InstructionMemory.size() && haltFlag != true) {
    // fetch instruction
    // decode
    // run
    // ++ProgramCounter
  }
}*/

std::string Emulator::Processor::fetchInstruction(unsigned char const& ProgramCounter) const {
  return InstructionMemory[ProgramCounter];
}

std::pair<Emulator::Instruction::Operation, unsigned char> Emulator::Processor::convertStringToChar(std::string const& str)
{
  int byteValue = 0;
  for (int i = 0; i < str.length(); ++i) {
    byteValue <<= 1;
    if (str[i] == '1') {
      byteValue |= 1;
    }
  }
  return {static_cast<Emulator::Instruction::Operation>(byteValue), byteValue};
}

Emulator::Instruction Emulator::Processor::decodeInstruction(std::string const& Instruction) {
  Emulator::Instruction i{};
  i.op = convertStringToChar(Instruction.substr(0,4)).first;
  switch(i.op) {
    case Instruction::NOP:
      break;

    case Instruction::ADD:
    case Instruction::SUB:
      i.Reg1 = convertStringToChar(Instruction.substr(12,4)).second;
      i.Reg2 = convertStringToChar(Instruction.substr(8,4)).second;
      i.Reg3 = convertStringToChar(Instruction.substr(4,4)).second;
      break;

    case Instruction::LOAD:
    case Instruction::STORE:
    case Instruction::LDI:
      i.Reg3 = convertStringToChar(Instruction.substr(4,4)).second;
      i.immediateOrAddress = convertStringToChar(Instruction.substr(8,8)).second;
      break;

    case Instruction::CMP:
      i.Reg1 = convertStringToChar(Instruction.substr(4,4)).second;
      i.Reg2 = convertStringToChar(Instruction.substr(8,4)).second;
      break;

    case Instruction::HALT:
      break;

    case Instruction::JMP:
      i.immediateOrAddress = convertStringToChar(Instruction.substr(4,8)).second;
      break;
  }
  return i;
}