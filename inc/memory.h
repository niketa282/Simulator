#ifndef MEMORY_H
#define MEMORY_H

#include <unordered_map>

namespace Emulator {
class Memory {
 public:
// function to check if pointer address is a key in the memory map .. Ie checks if
// address is valid
// count method returns 1 if keys exists false otherwise
  bool CheckMemorySet(unsigned char* address) {return memory.count(address);}

  void Write8BitMemory(unsigned char* address, unsigned char value) {memory[address] = value;}

  unsigned char Read8BitMemory(unsigned char* address) { return CheckMemorySet(address)? memory[address] : 0xde;}

  inline std::unordered_map<unsigned char*, unsigned char>& getMemoryMap() {return memory;}

 private:
  std::unordered_map<unsigned char*, unsigned char> memory;
};
}
#endif //MEMORY_H