#pragma once
#include "lzma_handle.h"
#include "lzma_out_stream.h"

#include "../../../extlibs/lzma/7zVersion.h"
#include "../../../extlibs/lzma/LzmaEnc.h"

namespace detail
{
  class lzma_header
  {
    public:
      enum : size_t
      {
        HEADER_SIZE = LZMA_PROPS_SIZE + 4
      };

      lzma_header()
      {
        _header[0] = MY_VER_MAJOR;
        _header[1] = MY_VER_MINOR;
        _header[2] = LZMA_PROPS_SIZE & 0xFF;
        _header[3] = (LZMA_PROPS_SIZE >> 8) & 0xFF;
      }

      void apply(lzma_handle& handle)
      {
        size_t headerSize = LZMA_PROPS_SIZE;
        LzmaEnc_WriteProperties(handle.get_native_handle(), &_header[4], &headerSize);
      }

      template <typename ELEM_TYPE, typename TRAITS_TYPE>
      void write_to_stream(detail::lzma_out_stream<ELEM_TYPE, TRAITS_TYPE>& stream)
      {
        stream.write(&_header, HEADER_SIZE);
      }

    private:
      Byte _header[HEADER_SIZE];
  };
}
