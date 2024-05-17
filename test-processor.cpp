#include <gtest/gtest.h>
#include "processor.h"

using namespace Emulator;

TEST(Processor, CheckInstructionNOP) {
  Instruction i{Instruction::NOP, Instruction::ImmAddr,{}};
  Processor p{};
  auto &registerBank = p.getRegisterBank();
  p.runInstruction(i);
  EXPECT_EQ(registerBank.size(), 32);
  EXPECT_EQ(p.getUnderflowCondition(), false);
  EXPECT_EQ(p.getOverflowCondition(), false);
}

TEST(Processor, CheckInstructionADD) {
  Instruction i = Instruction{Instruction::ADD, Instruction::ImmAddr, {3,1,2}};
  Processor p{};
  auto &registerBank = p.getRegisterBank();

 /*registerBank[1] = 3;
  registerBank[2] = 4;
  registerBank[3] = 0xde;

  p.runInstruction(i,registerBank);
  EXPECT_EQ(registerBank[3], 7);*/

  registerBank[1] = 255;
  registerBank[2] = 2;
  registerBank[3] = 0xde;

  p.runInstruction(i);
  EXPECT_EQ(registerBank[3], 0);
  EXPECT_EQ(p.getOverflowCondition(), true);
}

TEST(Processor, CheckInstructionSUBO) {
  Instruction i = Instruction{Instruction::SUB, Instruction::ImmAddr, {3,1,2}};
  Processor p{};
  auto &registerBank = p.getRegisterBank();

  registerBank[1] = 100;
  registerBank[2] = 44;
  registerBank[3] = 0xde;
  p.runInstruction(i);

  EXPECT_EQ(registerBank[3], 56);

  registerBank[1] = 3;
  registerBank[2] = 4;
  registerBank[3] = 0xde;

  p.runInstruction(i);
  EXPECT_EQ(registerBank[3], 255);
  EXPECT_EQ(p.getUnderflowCondition(), true);

}

/*TEST(Processor, CheckInstructionLOAD) {
  Instruction i = Instruction{Instruction::LOAD, Instruction::ImmAddr, {3,1,2}};
  Processor p{};
  Registers r{};

}*/
