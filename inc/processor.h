#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <array>

struct Instruction {
  enum Operation : char {NOP, Add} op;                         // one byte
  enum AddrMode : char {ImmAddr, RegAddr} addrMode; // one byte
  std::array<unsigned char,3> operandRegNum;           // always 3 bytes
};

class Processor {
 public:
  Processor() = default;
  std::string runInstruction(Instruction const& instr);
  inline std::array<unsigned,32> getRegisterBank() const { return registerBank;}
 private:
  std::array<unsigned,32> registerBank;   // R0->31.
};
#endif //PROCESSOR_H