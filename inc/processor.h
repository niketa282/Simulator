#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <array>
#include <string>
namespace Emulator{

struct Instruction {
  enum Operation : char {NOP, ADD, SUB} op;                         // one byte
  enum AddrMode : char {RegAddr, ImmAddr} addrMode; // one byte
  // RegAddr doesn't necesarily have effective address just for PC to increment to next instruction. Used for arithmetic operation instructions
  // ImmAddr address present in instruction to be used for branches and load/store
  std::array<unsigned char,3> operandRegNum;           // always 3 bytes
};
class Processor {
 public:
  static constexpr unsigned MIN = 0;
  static constexpr unsigned MAX = 255;

  void runInstruction(Instruction const& instr);
  inline std::array<unsigned char,32>& getRegisterBank() {return registerBank;}
  inline bool getUnderflowCondition() {return underflow;}
  inline bool getOverflowCondition() {return overflow;}
 private:
  // 32 general purpose 8 bit registers
  std::array<unsigned char,32> registerBank;
  bool underflow = false;
  bool overflow = false;
};
} // namespace::Emulator
#endif //PROCESSOR_H