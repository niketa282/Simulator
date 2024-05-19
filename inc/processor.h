#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <array>
#include <cstddef>
#include <vector>
#include <string>
#include <fstream>
#include "memory.h"
namespace Emulator {

// incase LDI wants to choose between immediate val and address can add another unsigned char
// to choose in run instruction case statement
struct Instruction {
  enum Operation : char {NOP, ADD, SUB, LOAD, LDI, STORE, CMP, HALT, JMP} op;
  unsigned char Reg1 = 0;
  unsigned char Reg2 = 0;
  unsigned char Reg3 = 0;
  unsigned char immediateOrAddress = 0;
  // unsigned char* address = nullptr;
};

class Processor {
 public:
  static constexpr int MIN = 0;
  static constexpr int MAX = 255;
  static constexpr std::size_t BITS = 16;
  void runInstruction(Instruction const& instr);

  inline std::array<unsigned char, 16>& getRegisterBank() {return registerBank;}

  inline Memory& getMemory() {return m;}

  inline bool getUnderflowCondition() const {return underflow;}

  inline bool getOverflowCondition() const {return overflow;}

  inline bool getEqualFlag() const {return equalFlag;}

  inline bool getHaltFlag() const {return haltFlag;}

  inline unsigned char& getProgramCounter() {return ProgramCounter;}

  inline std::vector<std::string>& getInstructionMemory() {return InstructionMemory;}

  void populateInstructionMemory(std::string const& filename);

  std::string fetchInstruction(unsigned char const& ProgramCounter) const;

  // Instruction::Operation convertStringToChar(std::string const& str);

  Emulator::Instruction decodeInstruction(std::string const& Instruction);

 private:
  // 32 general purpose 8 bit registers
  std::array<unsigned char,16> registerBank;
  // Instruction memory (2 bytes)
  std::vector<std::string> InstructionMemory;
  Memory m;
  bool underflow = false;
  bool overflow = false;
  bool equalFlag = false;
  bool haltFlag = false;
  unsigned char ProgramCounter;

  std::pair<Instruction::Operation, unsigned char> convertStringToChar(std::string const& str);
};
} // namespace::Emulator
#endif //PROCESSOR_H