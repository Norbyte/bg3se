#pragma once
#include "../compression_interface.h"

#include "deflate_decoder_properties.h"

#include "../../extlibs/zlib/zlib.h"

#include <cstdint>

template <typename ELEM_TYPE, typename TRAITS_TYPE>
class basic_deflate_decoder
  : public compression_decoder_interface_basic<ELEM_TYPE, TRAITS_TYPE>
{
  public:
    typedef typename compression_interface_basic<ELEM_TYPE, TRAITS_TYPE>::istream_type istream_type;
    typedef typename compression_interface_basic<ELEM_TYPE, TRAITS_TYPE>::ostream_type ostream_type;

    basic_deflate_decoder()
      : _lastError(Z_OK)
      , _stream(nullptr)
      , _endOfStream(false)
      , _bufferCapacity(0)
      , _inputBufferSize(0)
      , _outputBufferSize(0)
      , _inputBuffer(nullptr)
      , _outputBuffer(nullptr)
      , _bytesRead(0)
      , _bytesWritten(0)
    {

    }

    ~basic_deflate_decoder()
    {
      if (is_init())
      {
        inflateEnd(&_zstream);
        uninit_buffers();
      }
    }

    void init(istream_type& stream) override
    {
      deflate_decoder_properties props;
      init(stream, props);
    }

    void init(istream_type& stream, compression_decoder_properties_interface& props) override
    {
      // init stream
      _stream = &stream;
      _endOfStream = false;

      // init values
      _inputBufferSize = _outputBufferSize = 0;
      _bytesRead = _bytesWritten = 0;

      // init buffers
      deflate_decoder_properties& deflateProps = static_cast<deflate_decoder_properties&>(props);
      _bufferCapacity = deflateProps.BufferCapacity;

      uninit_buffers();
      _inputBuffer = new ELEM_TYPE[_bufferCapacity];
      _outputBuffer = new ELEM_TYPE[_bufferCapacity];

      // init deflate
      _zstream.zalloc = nullptr;
      _zstream.zfree = nullptr;
      _zstream.opaque = nullptr;

      _zstream.next_in = nullptr;
      _zstream.next_out = nullptr;
      _zstream.avail_in = 0;
      _zstream.avail_out = uInt(-1); // force first load of data

      inflateInit2(&_zstream, -MAX_WBITS);
    }

    bool is_init() const override
    {
      return (_inputBuffer != nullptr && _outputBuffer != nullptr);
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
      return _outputBuffer;
    }

    ELEM_TYPE* get_buffer_end() override
    {
      return _outputBuffer + _outputBufferSize;
    }

    size_t decode_next() override
    {
      size_t bytesProcessed = 0;
      do
      {
        // do not load any data until there
        // are something left
        if (_zstream.avail_out != 0)
        {
          // if all data has not been fetched and the stream is at the end,
          // it is an error
          if (_endOfStream)
          {
            return 0;
          }

          // read data into buffer
          read_next();

          // set input buffer and its size
          _zstream.next_in = reinterpret_cast<Bytef*>(_inputBuffer);
          _zstream.avail_in = static_cast<uInt>(_inputBufferSize);
        }

        // zstream output
        _zstream.next_out = reinterpret_cast<Bytef*>(_outputBuffer);
        _zstream.avail_out = static_cast<uInt>(_bufferCapacity);

        // inflate stream
        if (!zlib_suceeded(inflate(&_zstream, Z_NO_FLUSH)))
        {
          return 0;
        }

        // associate output buffer
        bytesProcessed += _bufferCapacity - static_cast<size_t>(_zstream.avail_out);

        // increase amount of total written bytes
        _bytesWritten += bytesProcessed;

        if (_lastError == Z_STREAM_END)
        {
          _endOfStream = true;

          // if we read more than we should last time, move pointer to the correct position
          if (_zstream.avail_in > 0)
          {
            _stream->clear();
            _stream->seekg(-static_cast<typename istream_type::off_type>(_zstream.avail_in), std::ios::cur);
          }
        }

        _outputBufferSize = bytesProcessed;
      }
      // Keep consuming input until we are able to produce some output
      while (_zstream.avail_out == static_cast<uInt>(_bufferCapacity));

      // return count of processed bytes from input stream
      return bytesProcessed;
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

    void read_next()
    {
      // read next bytes from input stream
      _stream->read(_inputBuffer, _bufferCapacity);

      // set the size of buffer
      _inputBufferSize = static_cast<size_t>(_stream->gcount());

      // increase amount of total read bytes
      _bytesRead += _inputBufferSize;

      // set lzma buffer pointer to the begin
      _endOfStream = _inputBufferSize != _bufferCapacity;
    }

    bool zlib_suceeded(int errorCode)
    {
      // Z_BUF_ERROR just means zlib filled its output buffer without
      // consuming all of its input buffer.
      return ((_lastError = errorCode) >= 0) || errorCode == Z_BUF_ERROR;
    }

    z_stream    _zstream;         // internal zlib structure
    int         _lastError;       // last error of zlib operation

    istream_type* _stream;
    bool       _endOfStream;

    size_t     _bufferCapacity;
    size_t     _inputBufferSize;  // how many bytes are read in the input buffer
    size_t     _outputBufferSize; // how many bytes are written in the output buffer
    ELEM_TYPE* _inputBuffer;      // pointer to the start of the input buffer
    ELEM_TYPE* _outputBuffer;     // pointer to the start of the output buffer

    size_t _bytesRead;
    size_t _bytesWritten;
};

typedef basic_deflate_decoder<uint8_t, std::char_traits<uint8_t>>  byte_deflate_decoder;
typedef basic_deflate_decoder<char, std::char_traits<char>>        deflate_decoder;
typedef basic_deflate_decoder<wchar_t, std::char_traits<wchar_t>>  wdeflate_decoder;
