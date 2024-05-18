#include <gtest/gtest.h>
#include "processor.h"
#include "memory.h"

using namespace Emulator;

TEST(Processor, CheckInstructionNOP) {
  Instruction i{Instruction::NOP};
  Processor p{};
  auto &registerBank = p.getRegisterBank();
  p.runInstruction(i);
  EXPECT_EQ(registerBank.size(), 32);
  EXPECT_EQ(p.getUnderflowCondition(), false);
  EXPECT_EQ(p.getOverflowCondition(), false);
}

TEST(Processor, CheckInstructionADD) {
  Instruction i{Instruction::ADD,1,2,3};
  Processor p{};
  auto &registerBank = p.getRegisterBank();

  registerBank[i.srcReg1] = 3;
  registerBank[i.srcReg2] = 4;
  registerBank[i.destReg] = 0xde;

  p.runInstruction(i);
  EXPECT_EQ(registerBank[i.destReg], 7);

  registerBank[i.srcReg1] = 254;
  registerBank[i.srcReg2] = 5;
  registerBank[i.destReg] = 0xde;

  p.runInstruction(i);
  EXPECT_EQ(static_cast<int>(registerBank[i.destReg]), 3);
  EXPECT_EQ(p.getOverflowCondition(), true);
}

TEST(Processor, CheckInstructionSUBO) {
  Instruction i{Instruction::SUB, 1,2,3};
  Processor p{};
  auto &registerBank = p.getRegisterBank();

  registerBank[i.srcReg1] = 100;
  registerBank[i.srcReg2] = 44;
  registerBank[i.destReg] = 0xde;
  p.runInstruction(i);
  EXPECT_EQ(registerBank[3], 56);

  registerBank[i.srcReg1] = 3;
  registerBank[i.srcReg2] = 4;
  registerBank[i.destReg] = 0xde;

  p.runInstruction(i);
  EXPECT_EQ(registerBank[i.destReg], 255);
  EXPECT_EQ(p.getUnderflowCondition(), true);

}

TEST(Processor, CheckReadWriteMemory) {
 Emulator::Memory m{};
 unsigned char addr = 0x01;
 unsigned char* address = &addr;
 m.Write8BitMemory(address, 5);
 EXPECT_EQ(m.Read8BitMemory(address), 5);

 // clear memory and test read returns default value of "0xde"
 std::unordered_map<unsigned char*, unsigned char>& memory = m.getMemoryMap();
 memory.clear();
 EXPECT_EQ(m.Read8BitMemory(address), 0xde);
}

TEST(Processor, CheckInstructionLOAD) {
  Processor p{};
  auto &memory = p.getMemory();
  unsigned char addr = 0x04;
  unsigned char* address = &addr;
  memory.Write8BitMemory(address, 12);
  Instruction i{Instruction::LOAD, 0, 0, 3, 0,address};
  auto &registerBank = p.getRegisterBank();
  p.runInstruction(i);
  EXPECT_EQ(registerBank[i.destReg], 12);
}

TEST(Processor, CheckInstructionLDI) {
  Processor p{};
  Instruction i{Instruction::LDI, 0, 0, 3, 0x5, nullptr};
  auto &registerBank = p.getRegisterBank();
  p.runInstruction(i);
  EXPECT_EQ(registerBank[i.destReg], 0x5);
}

TEST(Processor, CheckInstructionSTORE) {
  Processor p{};
  auto &memory = p.getMemory();
  unsigned char addr = 0x11;
  unsigned char* address = &addr;
  Instruction i{Instruction::STORE, 0, 0, 3, 0, address};
  auto &registerBank = p.getRegisterBank();
  registerBank[i.destReg]= 40;
  p.runInstruction(i);
  EXPECT_EQ(memory.Read8BitMemory(address), 40);
}
