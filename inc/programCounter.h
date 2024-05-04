#ifndef PROGRAMCOUNTER_H
#define PROGRAMCOUNTER_H

#include <memory>

namespace Processor {
  class ProgramCounter {
    public:
      std::unique_ptr<uint32_t> increment();
    private:
      std::unique_ptr<uint32_t> address;
  };
}

#endif //PROGRAMCOUNTER_H