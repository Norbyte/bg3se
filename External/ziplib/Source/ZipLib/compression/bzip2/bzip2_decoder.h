#pragma once
#include "../compression_interface.h"

#include "bzip2_decoder_properties.h"

#include "../../extlibs/bzip2/bzlib.h"

#include <cstdint>

template <typename ELEM_TYPE, typename TRAITS_TYPE>
class basic_bzip2_decoder
  : public compression_decoder_interface_basic<ELEM_TYPE, TRAITS_TYPE>
{
  public:
    typedef typename compression_interface_basic<ELEM_TYPE, TRAITS_TYPE>::istream_type istream_type;
    typedef typename compression_interface_basic<ELEM_TYPE, TRAITS_TYPE>::ostream_type ostream_type;

    basic_bzip2_decoder()
      : _lastError(BZ_OK)
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

    ~basic_bzip2_decoder()
    {
      if (is_init())
      {
        BZ2_bzDecompressEnd(&_bzstream);
        uninit_buffers();
      }
    }

    void init(istream_type& stream) override
    {
      bzip2_decoder_properties props;
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
      bzip2_decoder_properties& bzip2Props = static_cast<bzip2_decoder_properties&>(props);
      _bufferCapacity = bzip2Props.BufferCapacity;

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
      _bzstream.avail_out = (unsigned int)-1; // force first load of data

      // no verbosity & do not use small memory model
      _lastError = BZ2_bzDecompressInit(&_bzstream, 0, 0);
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
      // do not load any data until there
      // are something left
      if (_bzstream.avail_out != 0)
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
        _bzstream.next_in = reinterpret_cast<char*>(_inputBuffer);
        _bzstream.avail_in = static_cast<unsigned int>(_inputBufferSize);
      }

      // zstream output
      _bzstream.next_out = reinterpret_cast<char*>(_outputBuffer);
      _bzstream.avail_out = static_cast<unsigned int>(_bufferCapacity);

      // inflate stream
      if (!bzip2_suceeded(BZ2_bzDecompress(&_bzstream)))
      {
        return 0;
      }

      // associate output buffer
      size_t bytesProcessed = _bufferCapacity - static_cast<size_t>(_bzstream.avail_out);

      // increase amount of total written bytes
      _bytesWritten += bytesProcessed;

      if (_lastError == BZ_STREAM_END)
      {
        _endOfStream = true;

        // if we read more than we should last time, move pointer to the correct position
        if (_bzstream.avail_in > 0)
        {
          _stream->clear();
          _stream->seekg(-static_cast<typename istream_type::off_type>(_bzstream.avail_in), std::ios::cur);
        }
      }

      _outputBufferSize = bytesProcessed;

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

    bool bzip2_suceeded(int errorCode)
    {
      return ((_lastError = errorCode) >= 0);
    }

    bz_stream   _bzstream;        // internal bzip2 structure
    int         _lastError;       // last error of bzip2 operation

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

typedef basic_bzip2_decoder<uint8_t, std::char_traits<uint8_t>>  byte_bzip2_decoder;
typedef basic_bzip2_decoder<char, std::char_traits<char>>        bzip2_decoder;
typedef basic_bzip2_decoder<wchar_t, std::char_traits<wchar_t>>  wbzip2_decoder;
