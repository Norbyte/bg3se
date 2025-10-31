#pragma once
#include "lzma_alloc.h"

#include "../../../extlibs/lzma/LzmaEnc.h"

namespace detail
{
  class lzma_handle
  {
    public:
      lzma_handle()
        : _handle(nullptr)
      {
        _handle = LzmaEnc_Create(&_alloc);
      }

      ~lzma_handle()
      {
        if (_handle != nullptr)
        {
          LzmaEnc_Destroy(_handle, &_alloc, &_alloc);
        }
      }

      CLzmaEncHandle get_native_handle() const { return _handle; }

    private:
      CLzmaEncHandle  _handle;
      lzma_alloc      _alloc;
  };
}
