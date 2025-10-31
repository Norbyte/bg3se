#pragma once
#include <istream>
#include "streambuffs/compression_decoder_streambuf.h"

/**
 * \brief Basic generic compression decoder stream.
 */
template <typename ELEM_TYPE, typename TRAITS_TYPE>
class basic_compression_decoder_stream
  : public std::basic_istream<ELEM_TYPE, TRAITS_TYPE>
{
  public:
    typedef typename compression_decoder_streambuf<ELEM_TYPE, TRAITS_TYPE>::istream_type istream_type;
    typedef typename compression_decoder_streambuf<ELEM_TYPE, TRAITS_TYPE>::ostream_type ostream_type;

    typedef typename compression_decoder_streambuf<ELEM_TYPE, TRAITS_TYPE>::icompression_decoder_type icompression_decoder_type;
    typedef typename compression_decoder_streambuf<ELEM_TYPE, TRAITS_TYPE>::icompression_decoder_ptr_type icompression_decoder_ptr_type;

    basic_compression_decoder_stream()
      : std::basic_istream<ELEM_TYPE, TRAITS_TYPE>(&_compressionDecoderStreambuf)
    {

    }

    basic_compression_decoder_stream(icompression_decoder_ptr_type compressionDecoder, istream_type& stream)
      : std::basic_istream<ELEM_TYPE, TRAITS_TYPE>(&_compressionDecoderStreambuf)
      , _compressionDecoderStreambuf(compressionDecoder, stream)
    {

    }

    basic_compression_decoder_stream(icompression_decoder_ptr_type compressionDecoder, compression_decoder_properties_interface& props, istream_type& stream)
      : std::basic_istream<ELEM_TYPE, TRAITS_TYPE>(&_compressionDecoderStreambuf)
      , _compressionDecoderStreambuf(compressionDecoder, props, stream)
    {

    }

    bool init(icompression_decoder_ptr_type compressionDecoder, istream_type& stream)
    {
      return _compressionDecoderStreambuf.init(compressionDecoder, stream);
    }

    bool init(icompression_decoder_ptr_type compressionDecoder, compression_decoder_properties_interface& props, istream_type& stream)
    {
      return _compressionDecoderStreambuf.init(compressionDecoder, props, stream);
    }

    bool is_init() const
    {
      return _compressionDecoderStreambuf.is_init();
    }

    size_t get_bytes_read() const
    {
      return _compressionDecoderStreambuf.get_bytes_read();
    }

    size_t get_bytes_written() const
    {
      return _compressionDecoderStreambuf.get_bytes_written();
    }

  private:
    compression_decoder_streambuf<ELEM_TYPE, TRAITS_TYPE> _compressionDecoderStreambuf;
};

//////////////////////////////////////////////////////////////////////////

typedef basic_compression_decoder_stream<uint8_t, std::char_traits<uint8_t>>  byte_compression_decoder_stream;
typedef basic_compression_decoder_stream<char, std::char_traits<char>>        compression_decoder_stream;
typedef basic_compression_decoder_stream<wchar_t, std::char_traits<wchar_t>>  wcompression_decoder_stream;
