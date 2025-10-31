#pragma once
#include "../../../extlibs/lzma/Types.h"

#include <condition_variable>
#include <mutex>

// forward declaration
template <typename ELEM_TYPE_, typename TRAITS_TYPE_>
class basic_lzma_encoder;

namespace detail
{
  template <typename ELEM_TYPE, typename TRAITS_TYPE>
  class lzma_in_stream
    : public ISeqInStream
  {
    public:
      template <typename ELEM_TYPE_, typename TRAITS_TYPE_>
      friend class ::basic_lzma_encoder;

      typedef std::condition_variable event_t;
      typedef std::mutex              mutex_t;

      lzma_in_stream()
        : _bytesRead(0)
        , _internalBufferSize(0)
        , _internalInputBuffer(nullptr)
        , _endOfStream(false)
      {
        this->Read = [](void* p, void* buf, size_t* size) -> SRes
        {
          lzma_in_stream* pthis = static_cast<lzma_in_stream*>(p);
          return pthis->read(buf, size);
        };
      }

      SRes read(void* buf, size_t* size)
      {
        size_t lastBytesRead = _bytesRead;

        // set buffer pointer and get required size
        _internalInputBuffer = static_cast<ELEM_TYPE*>(buf);
        _internalBufferSize = *size / sizeof(ELEM_TYPE);

        // give control back to the main thread
        set_event();

        // wait for buffer fill
        if (!_endOfStream)
        {
          wait_for_event();
        }

        // copy the data
        if ((_bytesRead - lastBytesRead) < *size)
        {
          _endOfStream = true;
        }

        *size = _bytesRead - lastBytesRead;

        return SZ_OK;
      }

      size_t get_bytes_read() const { return _bytesRead; }

    private:
      size_t      _bytesRead;
      size_t      _internalBufferSize;
      ELEM_TYPE*  _internalInputBuffer;
      event_t     _event;
      mutex_t     _mutex;
      bool        _endOfStream;

      ELEM_TYPE* get_buffer_begin() { return _internalInputBuffer; }
      ELEM_TYPE* get_buffer_end() { return _internalInputBuffer + _internalBufferSize; }

      void set_event()
      {
        _event.notify_one();
      }

      void wait_for_event()
      {
        std::unique_lock<std::mutex> lk(_mutex);
        _event.wait(lk);
      }

      void compress(size_t length)
      {
        if (_endOfStream)
        {
          return;
        }

        _bytesRead += length;

        // set event in "read" method -> continue compression
        set_event();

        // wait until compression of the buffer is done
        wait_for_event();
      }
  };
}
