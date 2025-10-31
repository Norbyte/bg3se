#pragma once
#include "../../../extlibs/lzma/Types.h"

namespace detail
{
  template <typename ELEM_TYPE, typename TRAITS_TYPE>
  class lzma_out_stream
    : public ISeqOutStream
  {
    public:
      typedef std::basic_ostream<ELEM_TYPE, TRAITS_TYPE> stream_t;

      lzma_out_stream()
        : _bytesWritten(0)
        , _stream(nullptr)
      {
        this->Write = [](void* p, const void* buf, size_t size)
        {
          lzma_out_stream* pthis = static_cast<lzma_out_stream*>(p);
          return pthis->write(buf, size);
        };
      }

      size_t write(const void* buf, size_t size)
      {
        auto currentPosition = _stream->tellp();
        _stream->write(reinterpret_cast<const ELEM_TYPE*>(buf), size);

        size_t delta = static_cast<size_t>(_stream->tellp()) - static_cast<size_t>(currentPosition);
        _bytesWritten += delta;

        return delta;
      }

      size_t get_bytes_written() const { return _bytesWritten; }

      const stream_t& get_stream() const { return *_stream; }
      stream_t& get_stream() { return *_stream; }
      void set_stream(stream_t& stream) { _stream = &stream; }

    private:
      stream_t* _stream;
      size_t _bytesWritten;
  };
}
