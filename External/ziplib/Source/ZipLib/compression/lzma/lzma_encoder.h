#pragma once
#include "../compression_interface.h"

#include "lzma_encoder_properties.h"
#include "detail/lzma_alloc.h"
#include "detail/lzma_handle.h"
#include "detail/lzma_header.h"
#include "detail/lzma_in_stream.h"
#include "detail/lzma_out_stream.h"

#include <ostream>
#include <thread>
#include <cstdint>

template <typename ELEM_TYPE, typename TRAITS_TYPE>
class basic_lzma_encoder
  : public compression_encoder_interface_basic<ELEM_TYPE, TRAITS_TYPE>
{
  public:
    typedef typename compression_interface_basic<ELEM_TYPE, TRAITS_TYPE>::istream_type istream_type;
    typedef typename compression_interface_basic<ELEM_TYPE, TRAITS_TYPE>::ostream_type ostream_type;

    basic_lzma_encoder()
    {

    }

    ~basic_lzma_encoder()
    {
      if (is_init())
      {
        sync();
      }
    }

    void init(ostream_type& stream) override
    {
      lzma_encoder_properties props;
      init(stream, props);
    }

    void init(ostream_type& stream, compression_encoder_properties_interface& props) override
    {
      lzma_encoder_properties& lzmaProps = static_cast<lzma_encoder_properties&>(props);

      _ostream.set_stream(stream);
      lzmaProps.apply(_handle);

      start_compression_thread();
    }

    bool is_init() const override
    {
      return &_ostream.get_stream() != nullptr;
    }

    size_t get_bytes_read() const override
    {
      return _istream.get_bytes_read();
    }

    size_t get_bytes_written() const override
    {
      return _ostream.get_bytes_written();
    }

    ELEM_TYPE* get_buffer_begin() override
    {
      return _istream.get_buffer_begin();
    }

    ELEM_TYPE* get_buffer_end() override
    {
      return _istream.get_buffer_end();
    }

    void encode_next(size_t length) override
    {
      _istream.compress(length);
    }

    void sync() override
    {
      if (_compressionThread.joinable())
      {
        _compressionThread.join();
      }
    }

  private:
    void start_compression_thread()
    {
      detail::lzma_header header;
      header.apply(_handle);
      header.write_to_stream(_ostream);

      _compressionThread = std::thread(&basic_lzma_encoder::encode_threadroutine, this);

      _istream.wait_for_event();
    }

    bool encode_threadroutine()
    {
      return LzmaEnc_Encode(_handle.get_native_handle(), &_ostream, &_istream, nullptr, &_alloc, &_alloc) == SZ_OK;
    }

    detail::lzma_handle _handle;
    detail::lzma_alloc  _alloc;
    detail::lzma_in_stream<ELEM_TYPE, TRAITS_TYPE>  _istream;
    detail::lzma_out_stream<ELEM_TYPE, TRAITS_TYPE> _ostream;

    std::thread _compressionThread;
};

typedef basic_lzma_encoder<uint8_t, std::char_traits<uint8_t>>  byte_lzma_encoder;
typedef basic_lzma_encoder<char, std::char_traits<char>>        lzma_encoder;
typedef basic_lzma_encoder<wchar_t, std::char_traits<wchar_t>>  wlzma_encoder;
