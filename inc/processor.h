#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <array>
#include <string>
#include "memory.h"
namespace Emulator {

/*struct Instruction{
  enum Operation : char {NOP, ADD, SUB, LOAD} op;                         // one byte
  enum AddrMode : char {RegAddr, ImmAddr} addrMode; // one byte
  // RegAddr doesn't necesarily have effective address just for PC to increment to next instruction. Used for arithmetic operation instructions
  // ImmAddr address present in instruction to be used for branches and load/store
  std::array<unsigned char,3> operandRegNum;           // always 3 bytes
  unsigned char address = 0;
};*/

// incase LDI wants to choose between immediate val and address can add another unsigned char
// to choose in run instruction case statement
struct Instruction {
  enum Operation : char {NOP, ADD, SUB, LOAD, LDI, STORE, CMP, HALT, JMP} op;
  unsigned char srcReg1 = 0;
  unsigned char srcReg2 = 0;
  unsigned char destReg = 0;
  unsigned char immediate = 0;
  unsigned char* address = nullptr;
};

class Processor {
 public:
  static constexpr int MIN = 0;
  static constexpr int MAX = 255;

  void runInstruction(Instruction const& instr);

  inline std::array<unsigned char,32>& getRegisterBank() {return registerBank;}

  inline Memory& getMemory() {return m;}

  inline bool getUnderflowCondition() const {return underflow;}

  inline bool getOverflowCondition() const {return overflow;}

  inline bool getEqualFlag() const {return equalFlag;}

  inline bool getHaltFlag() const {return haltFlag;}

  inline unsigned char& getProgramCounter() {return ProgramCounter;}

 private:
  // 32 general purpose 8 bit registers
  std::array<unsigned char,32> registerBank;
  Memory m;
  bool underflow = false;
  bool overflow = false;
  bool equalFlag = false;
  bool haltFlag = false;
  unsigned char ProgramCounter;
};
} // namespace::Emulator
#endif //PROCESSOR_H