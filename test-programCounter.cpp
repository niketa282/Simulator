#include <cstdint>
#include <gtest/gtest.h>
#include "programCounter.h"

TEST(Calculator, ProgramCounter)
{
  Processor::ProgramCounter pc{};
  auto updatedPC = pc.increment();
  uint32_t *updatedPCVal = updatedPC.get();
  EXPECT_EQ(0x04, *updatedPCVal);
}