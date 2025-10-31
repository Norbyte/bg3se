#pragma once
#include "../compression_interface.h"

#include "bzip2_encoder_properties.h"

#include "../../extlibs/bzip2/bzlib.h"

#include <cstdint>

template <typename ELEM_TYPE, typename TRAITS_TYPE>
class basic_bzip2_encoder
  : public compression_encoder_interface_basic<ELEM_TYPE, TRAITS_TYPE>
{
  public:
    typedef typename compression_interface_basic<ELEM_TYPE, TRAITS_TYPE>::istream_type istream_type;
    typedef typename compression_interface_basic<ELEM_TYPE, TRAITS_TYPE>::ostream_type ostream_type;

    basic_bzip2_encoder()
      : _lastError(BZ_OK)
      , _stream(nullptr)
      , _bufferCapacity(0)
      , _inputBuffer(nullptr)
      , _outputBuffer(nullptr)
      , _bytesRead(0)
      , _bytesWritten(0)
    {

    }

    ~basic_bzip2_encoder()
    {
      if (is_init())
      {
        BZ2_bzCompressEnd(&_bzstream);
        uninit_buffers();
      }
    }

    void init(ostream_type& stream) override
    {
      bzip2_encoder_properties props;
      init(stream, props);
    }

    void init(ostream_type& stream, compression_encoder_properties_interface& props) override
    {
      // init stream
      _stream = &stream;

      // init values
      _bytesRead = _bytesWritten = 0;

      // init buffers
      bzip2_encoder_properties& bz2Props = static_cast<bzip2_encoder_properties&>(props);
      _bufferCapacity = bz2Props.BufferCapacity;

      uninit_buffers();
      _inputBuffer = new ELEM_TYPE[_bufferCapacity];
      _outputBuffer = new ELEM_TYPE[_bufferCapacity];

      // init bzip2
      _bzstream.bzalloc = nullptr;
      _bzstream.bzfree = nullptr;
      _bzstream.opaque = nullptr;

      _bzstream.next_in = nullptr;
      _bzstream.next_out = nullptr;
      _bzstream.avail_in = 0;
      _bzstream.avail_out = 0;

      _lastError = BZ2_bzCompressInit(&_bzstream, bz2Props.BlockSize, 0, bz2Props.WorkFactor);
    }

    bool is_init() const override
    {
      return _stream != nullptr;
    }

    size_t get_bytes_read() const override
    {
      return _bytesRead;
    }

    size_t get_bytes_written() const override
    {
      return _bytesWritten;
    }

    ELEM_TYPE* get_buffer_begin() override
    {
      return _inputBuffer;
    }

    ELEM_TYPE* get_buffer_end() override
    {
      return _inputBuffer + _bufferCapacity;
    }

    void encode_next(size_t length) override
    {
      // set the input buffer
      _bzstream.next_in = reinterpret_cast<char*>(_inputBuffer);
      _bzstream.avail_in = static_cast<unsigned int>(length);

      _bytesRead += length;

      bool flush = length < _bufferCapacity;

      // compress data
      do {
        // zstream output
        _bzstream.next_out = reinterpret_cast<char*>(_outputBuffer);
        _bzstream.avail_out = static_cast<unsigned int>(_bufferCapacity);

        // compress stream
        BZ2_bzCompress(&_bzstream, flush ? BZ_FINISH : BZ_RUN);

        size_t have = _bufferCapacity - static_cast<size_t>(_bzstream.avail_out);

        if (have > 0)
        {
          _stream->write(_outputBuffer, have);
          _bytesWritten += have;
        }
      } while (_bzstream.avail_out == 0);
    }

    void sync() override
    {

    }

  private:
    void uninit_buffers()
    {
      if (_inputBuffer != nullptr)
      {
        delete[] _inputBuffer;
      }

      if (_outputBuffer != nullptr)
      {
        delete[] _outputBuffer;
      }
    }

    bool bzip2_suceeded(int errorCode)
    {
      return ((_lastError = errorCode) >= 0);
    }

    bz_stream _bzstream;        // internal bzip2 structure
    int       _lastError;       // last error of bzip2 operation

    ostream_type* _stream;

    size_t     _bufferCapacity;
    ELEM_TYPE* _inputBuffer;      // pointer to the start of the input buffer
    ELEM_TYPE* _outputBuffer;     // pointer to the start of the output buffer

    size_t _bytesRead;
    size_t _bytesWritten;
};

typedef basic_bzip2_encoder<uint8_t, std::char_traits<uint8_t>>  byte_bzip2_encoder;
typedef basic_bzip2_encoder<char, std::char_traits<char>>        bzip2_encoder;
typedef basic_bzip2_encoder<wchar_t, std::char_traits<wchar_t>>  wbzip2_encoder;
