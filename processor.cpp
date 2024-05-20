#include <cassert>
#include <iostream>
#include <sstream>
#include "processor.h"

void Emulator::Processor::runInstruction(Instruction const& instr, bool debug) {
switch(instr.op) {

case Instruction::NOP:
  if (debug) std::cout << "NOP \n";
  break;

case Instruction::ADD:
{
  if (debug)
  {
   std::cout << "ADD \n";
   std::cout << "Value in Reg " << int{instr.operandRegNum[0]} << " is " << int{registerBank[instr.operandRegNum[0]]} << "\n";
   std::cout << "Value in Reg " << int{instr.operandRegNum[1]} << " is " << int{registerBank[instr.operandRegNum[1]]} << "\n";
   std::cout << "Output into Reg "<< int{instr.operandRegNum[2]} << " = " << int{registerBank[instr.operandRegNum[0]]} << "+" << int{registerBank[instr.operandRegNum[1]]} << "\n";
  }
  auto add = static_cast<int>(registerBank[instr.operandRegNum[0]]) + static_cast<int>(registerBank[instr.operandRegNum[1]]);
  if (add > MAX) {
    overflow = true;
  }
  registerBank[instr.operandRegNum[2]] = registerBank[instr.operandRegNum[0]] + registerBank[instr.operandRegNum[1]];
  if (debug) std::cout << "Value in Reg " << int{instr.operandRegNum[2]} << " is " << int{registerBank[instr.operandRegNum[2]]} << "\n";
  break;
}

case Instruction::SUB:
{
  if (debug)
  {
    std::cout << "SUB \n";
    std::cout << "Output in Reg " << int{instr.operandRegNum[2]} << " = " << int{registerBank[instr.operandRegNum[0]]} << " - " << int{registerBank[instr.operandRegNum[1]]} << "\n";
  }
  auto diff =  static_cast<int>(registerBank[instr.operandRegNum[0]]) - static_cast<int>(registerBank[instr.operandRegNum[1]]);
  if (diff < MIN) {
     underflow = true;
  }
  registerBank[instr.operandRegNum[2]] = registerBank[instr.operandRegNum[0]] - registerBank[instr.operandRegNum[1]];
  if (debug) std::cout << "Value in Reg " << int{instr.operandRegNum[2]} << " is " << int{registerBank[instr.operandRegNum[2]]} << "\n";
  break;
}

case Instruction::LOAD:
{
  if (debug)
  {
    std::cout << "LOAD \n";
    std::cout << "Load into Reg " << int{instr.operandRegNum[2]} << " an address of " << int{instr.immediateOrAddress} << "\n";
    std::cout << "Value read is " << int{m.Read8BitMemory(instr.immediateOrAddress)} << "\n";
  }
  registerBank[instr.operandRegNum[2]] = m.Read8BitMemory(instr.immediateOrAddress);
  break;
}

case Instruction::LDI:
{
  if (debug)
  {
    std::cout << "LDI \n";
    std::cout << "Load into Reg " << int{instr.operandRegNum[2]} << " an immediate value of " << int{instr.immediateOrAddress} << "\n";
  }
  registerBank[instr.operandRegNum[2]] = instr.immediateOrAddress;
  break;
}

case Instruction::STORE:
{
  if (debug)
  {
    std::cout << "STORE \n";
    std::cout << "Store from Reg " << int{instr.operandRegNum[2]} << " into address at " << int{instr.immediateOrAddress} << "\n";
  }
  m.Write8BitMemory(instr.immediateOrAddress, registerBank[instr.operandRegNum[2]]);
  if (debug)
  break;
}

case Instruction::CMP:
{
  if (debug)
  {
    std::cout << "CMP \n";
    std::cout << "Compare Value " << int{registerBank[instr.operandRegNum[0]]} << " in Reg " << int{instr.operandRegNum[0]} << " with Value "<< int{registerBank[instr.operandRegNum[1]]}<< "in Reg" << int{instr.operandRegNum[1]} << "\n";
  }
  registerBank[instr.operandRegNum[0]] == registerBank[instr.operandRegNum[1]] ? equalFlag = true : equalFlag = false;
  break;
}

case Instruction::HALT:
{
  if (debug) std::cout << "HALT \n";
  haltFlag = true;
  break;
}

case Instruction::JMP:
{
  if (debug)
  {
    std::cout << "JMP " << "Program counter jumps to address " << int{instr.immediateOrAddress} << "\n";
  }
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

void Emulator::Processor::Execute(std::string const& filename)
{
  populateInstructionMemory(filename);
  while (ProgramCounter < InstructionMemory.size() && haltFlag != true) {
    std::string Instruction = fetchInstruction(ProgramCounter);
    Emulator::Instruction i = decodeInstruction(Instruction);
    runInstruction(i, true);
    ++ProgramCounter;
  }
}

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
      i.operandRegNum[0] = convertStringToChar(Instruction.substr(12,4)).second;
      i.operandRegNum[1] = convertStringToChar(Instruction.substr(8,4)).second;
      i.operandRegNum[2] = convertStringToChar(Instruction.substr(4,4)).second;
      break;

    case Instruction::LOAD:
    case Instruction::STORE:
    case Instruction::LDI:
      i.operandRegNum[2] = convertStringToChar(Instruction.substr(4,4)).second;
      i.immediateOrAddress = convertStringToChar(Instruction.substr(8,8)).second;
      break;

    case Instruction::CMP:
      i.operandRegNum[0] = convertStringToChar(Instruction.substr(4,4)).second;
      i.operandRegNum[1] = convertStringToChar(Instruction.substr(8,4)).second;
      break;

    case Instruction::HALT:
      break;

    case Instruction::JMP:
      i.immediateOrAddress = convertStringToChar(Instruction.substr(4,8)).second;
      break;
  }
  return i;
}