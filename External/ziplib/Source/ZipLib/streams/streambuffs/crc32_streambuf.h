#pragma once
#include <streambuf>
#include <cstdint>

#include "../substream.h"
#include "../../extlibs/zlib/zlib.h"

template <typename ELEM_TYPE, typename TRAITS_TYPE>
class crc32_streambuf
  : public std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE>
{
  public:
    typedef std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE> base_type;
    typedef typename std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE>::traits_type traits_type;

    typedef typename base_type::char_type char_type;
    typedef typename base_type::int_type  int_type;
    typedef typename base_type::pos_type  pos_type;
    typedef typename base_type::off_type  off_type;

    crc32_streambuf()
      : _inputStream(nullptr)
      , _internalBufferPosition(_internalBuffer + INTERNAL_BUFFER_SIZE)
      , _internalBufferEnd(_internalBuffer + INTERNAL_BUFFER_SIZE)
      , _bytesRead(0)
      , _crc32(0)
    {

    }

    crc32_streambuf(std::basic_istream<ELEM_TYPE, TRAITS_TYPE>& input)
      : crc32_streambuf()
    {
      init(input);
    }

    void init(std::basic_istream<ELEM_TYPE, TRAITS_TYPE>& input)
    {
      _inputStream = &input;

      ELEM_TYPE* endOfBuffer = _internalBuffer + INTERNAL_BUFFER_SIZE;
      this->setg(endOfBuffer, endOfBuffer, endOfBuffer);
    }

    bool is_init() const
    {
      return (_inputStream != nullptr);
    }

    size_t get_bytes_read() const
    {
      return _bytesRead;
    }

    uint32_t get_crc32() const
    {
      return _crc32;
    }

  protected:
    int_type underflow() override
    {
      // buffer exhausted
      if (this->gptr() >= _internalBufferEnd)
      {
        _inputStream->read(_internalBuffer, static_cast<std::streamsize>(INTERNAL_BUFFER_SIZE));
        size_t n = static_cast<size_t>(_inputStream->gcount());

        _bytesRead += n;

        _internalBufferPosition = _internalBuffer;
        _internalBufferEnd = _internalBuffer + n;

        if (n == 0)
        {
          return traits_type::eof();
        }
      }

      // set buffer pointers, increment current position
      ELEM_TYPE* base = _internalBufferPosition++;

      // setting all pointers to the same position forces calling of this method each time,
      // so crc32 really represents the checksum of what really has been read
      this->setg(base, base, base + 1);

      _crc32 = crc32(_crc32, reinterpret_cast<Bytef*>(this->gptr()), static_cast<uInt>(sizeof(ELEM_TYPE)));

      return traits_type::to_int_type(*this->gptr());
    }
    
  private:
    enum : size_t
    {
      INTERNAL_BUFFER_SIZE = 1 << 15
    };

    ELEM_TYPE  _internalBuffer[INTERNAL_BUFFER_SIZE];
    ELEM_TYPE* _internalBufferPosition;
    ELEM_TYPE* _internalBufferEnd;

    std::basic_istream<ELEM_TYPE, TRAITS_TYPE>* _inputStream;
    size_t _bytesRead;
    uint32_t _crc32;
};
