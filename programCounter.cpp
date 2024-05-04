#include "programCounter.h"
#include <sys/types.h>

std::unique_ptr<uint32_t> Processor::ProgramCounter::increment() {
 // uint32_t* pcCurrentAddress = address.get();
 // uint32_t* pcUpdatedAddress = *pcCurrentAddress + 0x04;
  return std::unique_ptr<uint32_t>{(address.get()) + 0x04};
}