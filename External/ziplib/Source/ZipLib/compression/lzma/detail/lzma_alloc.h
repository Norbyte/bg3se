#pragma once
#include "../../../extlibs/lzma/Types.h"

namespace detail
{
  class lzma_alloc
    : public ISzAlloc
  {
    public:
      lzma_alloc()
      {
        this->Alloc = [](void*, size_t size)   { return malloc(size); };
        this->Free  = [](void*, void* address) { free(address); };
      }
  };
}
