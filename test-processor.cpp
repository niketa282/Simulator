#include <gtest/gtest.h>
#include "processor.h"
#include "memory.h"

using namespace Emulator;

TEST(Processor, CheckInstructionNOP) {
  Instruction i{Instruction::NOP};
  Processor p{};
  auto &registerBank = p.getRegisterBank();
  p.runInstruction(i);
  EXPECT_EQ(registerBank.size(), 16);
  EXPECT_EQ(p.getUnderflowCondition(), false);
  EXPECT_EQ(p.getOverflowCondition(), false);
}

TEST(Processor, CheckInstructionADD) {
  Instruction i{Instruction::ADD,1,2,3};
  Processor p{};
  auto &registerBank = p.getRegisterBank();

  registerBank[i.Reg1] = 3;
  registerBank[i.Reg2] = 4;
  registerBank[i.Reg3] = 0xde;

  p.runInstruction(i);
  EXPECT_EQ(registerBank[i.Reg3], 7);

  registerBank[i.Reg1] = 254;
  registerBank[i.Reg2] = 5;
  registerBank[i.Reg3] = 0xde;

  p.runInstruction(i);
  EXPECT_EQ(static_cast<int>(registerBank[i.Reg3]), 3);
  EXPECT_EQ(p.getOverflowCondition(), true);
}

TEST(Processor, CheckInstructionSUBO) {
  Instruction i{Instruction::SUB, 1,2,3};
  Processor p{};
  auto &registerBank = p.getRegisterBank();

  registerBank[i.Reg1] = 100;
  registerBank[i.Reg2] = 44;
  registerBank[i.Reg3] = 0xde;
  p.runInstruction(i);
  EXPECT_EQ(registerBank[3], 56);

  registerBank[i.Reg1] = 3;
  registerBank[i.Reg2] = 4;
  registerBank[i.Reg3] = 0xde;

  p.runInstruction(i);
  EXPECT_EQ(registerBank[i.Reg3], 255);
  EXPECT_EQ(p.getUnderflowCondition(), true);

}

TEST(Processor, CheckReadWriteMemory) {
 Emulator::Memory m{};
 unsigned char addr = 0x01;
 m.Write8BitMemory(0x01, 5);
 EXPECT_EQ(m.Read8BitMemory(addr), 5);

 // clear memory and test read returns default value of "0xde"
 std::unordered_map<unsigned char, unsigned char>& memory = m.getMemoryMap();
 memory.clear();
 EXPECT_EQ(m.Read8BitMemory(addr), 0xde);
}

TEST(Processor, CheckInstructionLOAD) {
  Processor p{};
  auto &memory = p.getMemory();
  unsigned char addr = 0x04;
  memory.Write8BitMemory(addr, 12);
  Instruction i{Instruction::LOAD, 0, 0, 3, addr};
  auto &registerBank = p.getRegisterBank();
  p.runInstruction(i);
  EXPECT_EQ(registerBank[i.Reg3], 12);
}

TEST(Processor, CheckInstructionLDI) {
  Processor p{};
  Instruction i{Instruction::LDI, 0, 0, 3, 0x5};
  auto x = static_cast<int>(i.op);
  std::cout << "checking val of instr " << x << "\n";
  auto &registerBank = p.getRegisterBank();
  p.runInstruction(i);
  EXPECT_EQ(registerBank[i.Reg3], 0x5);
}

TEST(Processor, CheckInstructionSTORE) {
  Processor p{};
  auto &memory = p.getMemory();
  unsigned char addr = 0x11;
  Instruction i{Instruction::STORE, 0, 0, 3, addr};
  auto &registerBank = p.getRegisterBank();
  registerBank[i.Reg3]= 40;
  p.runInstruction(i);
  EXPECT_EQ(memory.Read8BitMemory(addr), 40);
}

TEST(Processor, CheckInstructionCMP) {
  Processor p{};
  Instruction i{Instruction::CMP, 1, 2, 0};
  auto &registerBank = p.getRegisterBank();
  registerBank[i.Reg1]= 5;
  registerBank[i.Reg2] = 12;
  p.runInstruction(i);
  EXPECT_EQ(p.getEqualFlag(), false);
}

TEST(Processor, CheckInstructionHALT) {
  Processor p{};
  Instruction i{Instruction::HALT};
  p.runInstruction(i);
  EXPECT_EQ(p.getHaltFlag(), true);
}

TEST(Processor, CheckInstructionJMP) {
  Processor p{};
  unsigned char addr = 0x11;
  Instruction i{Instruction::JMP,0, 0, 0, addr};
  p.runInstruction(i);
  EXPECT_EQ(p.getProgramCounter(), 0x11);
}

TEST(Processor, populateInstructionMemory) {
  Processor p{};
  auto &instructionMemory = p.getInstructionMemory();
  EXPECT_EQ(instructionMemory.size(), 0);
  p.populateInstructionMemory("binaryDataTest.txt");
  EXPECT_EQ(instructionMemory.size(), 3);
  for(auto& iMem: instructionMemory)
  {
    EXPECT_EQ(iMem.size(), 16);
  }
}

TEST(Processor, fetchInstruction) {
  Processor p{};
  auto &ProgramCounter = p.getProgramCounter();
  ProgramCounter = 2;
  p.populateInstructionMemory("binaryDataTest.txt");
  p.fetchInstruction(ProgramCounter);
  EXPECT_EQ(p.fetchInstruction(ProgramCounter), "0011000101110000");
}

TEST(Processor, decodeInstructions) {
  Processor p{};
  Instruction i{};
  auto &ProgramCounter = p.getProgramCounter();
  p.populateInstructionMemory("InstructionsDecodingData.txt");
  std::string Instruction = p.fetchInstruction(ProgramCounter);
  EXPECT_EQ(i.op, Instruction::NOP);

  ProgramCounter = 1;
  Instruction = p.fetchInstruction(ProgramCounter);
  i = p.decodeInstruction(Instruction);
  EXPECT_EQ(i.op, Instruction::LDI);
  EXPECT_EQ(i.Reg3, 2);
  EXPECT_EQ(i.immediateOrAddress, 10);

  ProgramCounter = 2;
  Instruction = p.fetchInstruction(ProgramCounter);
  i = p.decodeInstruction(Instruction);
  EXPECT_EQ(i.op, Instruction::LOAD);
  EXPECT_EQ(i.Reg3, 4);
  EXPECT_EQ(i.immediateOrAddress, 2);

  ProgramCounter = 3;
  Instruction = p.fetchInstruction(ProgramCounter);
  i = p.decodeInstruction(Instruction);
  EXPECT_EQ(i.op, Instruction::ADD);
  EXPECT_EQ(i.Reg1, 2);
  EXPECT_EQ(i.Reg2, 3);
  EXPECT_EQ(i.Reg3, 5);

  ProgramCounter = 4;
  Instruction = p.fetchInstruction(ProgramCounter);
  i = p.decodeInstruction(Instruction);
  EXPECT_EQ(i.op, Instruction::CMP);
  EXPECT_EQ(i.Reg1, 6);
  EXPECT_EQ(i.Reg2, 7);

  ProgramCounter = 5;
  Instruction = p.fetchInstruction(ProgramCounter);
  i = p.decodeInstruction(Instruction);
  EXPECT_EQ(i.op, Instruction::JMP);
  EXPECT_EQ(i.immediateOrAddress, 2);
}

TEST(Processor, ExecuteInstruction) {
  Processor p{};
  p.Execute("SimpleAddTest.txt");
  auto &instructionMemory = p.getInstructionMemory();
  EXPECT_EQ(instructionMemory.size(), 4);
}