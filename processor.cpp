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
   std::cout << "Value in " << int{instr.Reg1} << " is " << int{registerBank[instr.Reg1]} << "\n";
   std::cout << "Value in " << int{instr.Reg2} << " is " << int{registerBank[instr.Reg2]} << "\n";
   std::cout << " Output into Reg "<< int{instr.Reg3} << "= " << int{registerBank[instr.Reg1]} << "+" << int{registerBank[instr.Reg2]} << "\n";
  }
  auto add = static_cast<int>(registerBank[instr.Reg1]) + static_cast<int>(registerBank[instr.Reg2]);
  if (add > MAX) {
    overflow = true;
  }
  registerBank[instr.Reg3] = registerBank[instr.Reg1] + registerBank[instr.Reg2];
  if (debug) std::cout << "Value in Reg " << int{instr.Reg3} << " is " << int{registerBank[instr.Reg3]} << "\n";
  break;
}

case Instruction::SUB:
{
  if (debug)
  {
    std::cout << "SUB \n";
    std::cout << " Output into Reg " << int{instr.Reg3} << int{registerBank[instr.Reg1]} << "-" << int{registerBank[instr.Reg2]} << "\n";
  }
  auto diff =  static_cast<int>(registerBank[instr.Reg1]) - static_cast<int>(registerBank[instr.Reg2]);
  if (diff < MIN) {
     underflow = true;
  }
  registerBank[instr.Reg3] = registerBank[instr.Reg1] - registerBank[instr.Reg2];
  break;
}

case Instruction::LOAD:
{
  if (debug)
  {
    std::cout << " LOAD \n";
    std::cout << " Load into Reg " << int{instr.Reg3} << " an address of" << int{instr.immediateOrAddress} << "\n";
    std::cout << " Value read is " << int{m.Read8BitMemory(instr.immediateOrAddress)} << "\n";
  }
  registerBank[instr.Reg3] = m.Read8BitMemory(instr.immediateOrAddress);
  break;
}

case Instruction::LDI:
{
  if (debug)
  {
    std::cout << "LDI \n";
    std::cout << "Load into Reg " << int{instr.Reg3} << " an immediate value of" << int{instr.immediateOrAddress} << "\n";
  }
  registerBank[instr.Reg3] = instr.immediateOrAddress;
  break;
}

case Instruction::STORE:
{
  if (debug)
  {
    std::cout << "STORE \n";
    std::cout << "Store from Reg " << int{instr.Reg3} << "into address at " << int{instr.immediateOrAddress} << "\n";
 //   std::cout << "Value being written is " << m.Write8BitMemory(instr.immediateOrAddress, registerBank[instr.Reg3]) << "\n";
  }
  m.Write8BitMemory(instr.immediateOrAddress, registerBank[instr.Reg3]);
  if (debug)
  break;
}

case Instruction::CMP:
{
  if (debug)
  {
    std::cout << "CMP \n";
    std::cout << "Compare Value " << int{registerBank[instr.Reg1]} << " in Reg " << int{instr.Reg1} << " with Value "<< int{registerBank[instr.Reg2]}<< "in Reg" << int{instr.Reg2} << "\n";
  }
  registerBank[instr.Reg1] == registerBank[instr.Reg2] ? equalFlag = true : equalFlag = false;
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