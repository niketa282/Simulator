#ifndef MEMORY_H
#define MEMORY_H

#include <unordered_map>

// Memory class mimics RAM memory where data can be read and written to.
// Unordered_map used as very efficient for lookup O(1) which is what the Read and Write functions rely on.
namespace Emulator {
class Memory {
 public:
  // Function to check if pointer address is a key in the memory map
  // Checks if address is valid
  bool CheckMemorySet(unsigned char address) {return memory.count(address);}

  void Write8BitMemory(unsigned char address, unsigned char value) {memory[address] = value;}

  unsigned char Read8BitMemory(unsigned char address) { return CheckMemorySet(address)? memory[address] : 0xde;}

 private:
  std::unordered_map<unsigned char, unsigned char> memory;
};
}
#endif //MEMORY_H