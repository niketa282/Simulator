#include <gtest/gtest.h>
#include "processor.h"

TEST(Processor, CheckVarsSize) {
  Processor p{};
  EXPECT_EQ(p.getRegisterBank().size(), 32);
  Instruction i{};
}

TEST(Processor, CheckInstructionNOP) {
  Instruction i{Instruction::NOP, Instruction::ImmAddr,{}};
  Processor p{};
  EXPECT_EQ(p.runInstruction(i), "NOP");
}

TEST(Processor, CheckInstructionADD) {
  Instruction i = Instruction{Instruction::Add, Instruction::ImmAddr, {3,1,2}};
  Processor p{};
  auto registerBank = p.getRegisterBank();
 /* registerBank[2] = 5;
  registerBank[5] = 7;
  registerBank[8] = 0xffa;*/

  registerBank[1] = 3;
  registerBank[2] = 4;
  registerBank[3] = 0xdeadbeef;

  EXPECT_EQ(p.runInstruction(i), "ADD");
  EXPECT_EQ(registerBank[3], 7);
}