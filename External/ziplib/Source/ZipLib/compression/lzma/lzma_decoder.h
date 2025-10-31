#pragma once
#include "../compression_interface.h"

#include "detail/lzma_alloc.h"
#include "lzma_decoder_properties.h"

#include "../../extlibs/lzma/LzmaDec.h"

#include <cstdint>

template <typename ELEM_TYPE, typename TRAITS_TYPE>
class basic_lzma_decoder
  : public compression_decoder_interface_basic<ELEM_TYPE, TRAITS_TYPE>
{
  public:
    typedef typename compression_interface_basic<ELEM_TYPE, TRAITS_TYPE>::istream_type istream_type;
    typedef typename compression_interface_basic<ELEM_TYPE, TRAITS_TYPE>::ostream_type ostream_type;

    basic_lzma_decoder()
      : _inPos(0)
      , _inProcessed(0)
      , _outProcessed(0)
      , _stream(nullptr)
      , _bufferCapacity(0)
      , _inputBufferSize(0)
      , _outputBufferSize(0)
      , _inputBuffer(nullptr)
      , _outputBuffer(nullptr)
      , _bytesRead(0)
      , _bytesWritten(0)
    {
      LzmaDec_Construct(&_handle);
    }

    ~basic_lzma_decoder()
    {
      if (is_init())
      {
        LzmaDec_Free(&_handle, &_alloc);
        uninit_buffers();
      }
    }

    void init(istream_type& stream) override
    {
      lzma_decoder_properties props;
      init(stream, props);
    }

    void init(istream_type& stream, compression_decoder_properties_interface& props) override
    {
      // init stream
      _stream = &stream;

      // init values
      _inPos = _inProcessed = _outProcessed = 0;
      _inputBufferSize = 0;

      // init buffers
      lzma_decoder_properties& lzmaProps = static_cast<lzma_decoder_properties&>(props);
      _bufferCapacity = lzmaProps.BufferCapacity;

      uninit_buffers();
      _inputBuffer = new ELEM_TYPE[_bufferCapacity];
      _outputBuffer = new ELEM_TYPE[_bufferCapacity];

      // read lzma header
      Byte header[LZMA_PROPS_SIZE + 4];
      _stream->read(reinterpret_cast<ELEM_TYPE*>(header), sizeof(header) / sizeof(ELEM_TYPE));

      // init lzma
      LzmaDec_Allocate(&_handle, &header[4], LZMA_PROPS_SIZE, &_alloc);
      LzmaDec_Init(&_handle);
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
      if (_inPos == _inputBufferSize)
      {
        read_next();
      }

      _inProcessed = _inputBufferSize - _inPos;
      _outProcessed = _bufferCapacity;

      ELzmaFinishMode finishMode = LZMA_FINISH_ANY;
      ELzmaStatus status;
      SRes res;
      res = LzmaDec_DecodeToBuf(
        &_handle,
        reinterpret_cast<Byte*>(_outputBuffer),
        &_outProcessed,
        reinterpret_cast<Byte*>(_inputBuffer) + _inPos,
        &_inProcessed,
        finishMode,
        &status);

      _inPos += _inProcessed;
      _bytesWritten += _outProcessed;

      _outputBufferSize = _outProcessed;

      return _outputBufferSize;

      // if (res != SZ_OK)
      //   return _lzmaInProcessed;
      // 
      // if (_lzmaInProcessed == 0 && _lzmaOutProcessed == 0)
      // {
      //   if (status != LZMA_STATUS_FINISHED_WITH_MARK)
      //     return _lzmaOutProcessed;
      //   return _lzmaOutProcessed;
      // }
    }

  private:
    void uninit_buffers()
    {
      if (_inputBuffer != nullptr)
      {
        delete [] _inputBuffer;
      }

      if (_outputBuffer != nullptr)
      {
        delete [] _outputBuffer;
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
      _inPos = 0;
    }

    CLzmaDec _handle;
    detail::lzma_alloc _alloc;

    SizeT _inPos;
    SizeT _inProcessed;
    SizeT _outProcessed;

    istream_type* _stream;

    size_t     _bufferCapacity;
    size_t     _inputBufferSize;  // how many bytes are read in the input buffer
    size_t     _outputBufferSize; // how many bytes are written in the output buffer
    ELEM_TYPE* _inputBuffer;      // pointer to the start of the input buffer
    ELEM_TYPE* _outputBuffer;     // pointer to the start of the output buffer

    size_t _bytesRead;
    size_t _bytesWritten;
};

typedef basic_lzma_decoder<uint8_t, std::char_traits<uint8_t>>  byte_lzma_decoder;
typedef basic_lzma_decoder<char, std::char_traits<char>>        lzma_decoder;
typedef basic_lzma_decoder<wchar_t, std::char_traits<wchar_t>>  wlzma_decoder;
