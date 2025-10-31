#pragma once
#include <ostream>
#include "streambuffs/compression_encoder_streambuf.h"

/**
 * \brief Basic generic compression encoder stream.
 */
template <typename ELEM_TYPE, typename TRAITS_TYPE>
class basic_compression_encoder_stream
  : public std::basic_ostream<ELEM_TYPE, TRAITS_TYPE>
{
  public:
    typedef typename compression_encoder_streambuf<ELEM_TYPE, TRAITS_TYPE>::istream_type istream_type;
    typedef typename compression_encoder_streambuf<ELEM_TYPE, TRAITS_TYPE>::ostream_type ostream_type;

    typedef typename compression_encoder_streambuf<ELEM_TYPE, TRAITS_TYPE>::icompression_encoder_type icompression_encoder_type;
    typedef typename compression_encoder_streambuf<ELEM_TYPE, TRAITS_TYPE>::icompression_encoder_ptr_type icompression_encoder_ptr_type;

    basic_compression_encoder_stream()
      : std::basic_ostream<ELEM_TYPE, TRAITS_TYPE>(&_compressionEncoderStreambuf)
    {

    }

    basic_compression_encoder_stream(icompression_encoder_ptr_type compressionEncoder, ostream_type& stream)
      : std::basic_ostream<ELEM_TYPE, TRAITS_TYPE>(&_compressionEncoderStreambuf)
      , _compressionEncoderStreambuf(compressionEncoder, stream)
    {

    }

    basic_compression_encoder_stream(icompression_encoder_ptr_type compressionEncoder, compression_encoder_properties_interface& props, ostream_type& stream)
      : std::basic_ostream<ELEM_TYPE, TRAITS_TYPE>(&_compressionEncoderStreambuf)
      , _compressionEncoderStreambuf(compressionEncoder, props, stream)
    {

    }

    void init(icompression_encoder_ptr_type compressionEncoder, ostream_type& stream)
    {
      _compressionEncoderStreambuf.init(compressionEncoder, stream);
    }

    void init(icompression_encoder_ptr_type compressionEncoder, compression_encoder_properties_interface& props, ostream_type& stream)
    {
      _compressionEncoderStreambuf.init(compressionEncoder, props, stream);
    }
      
    bool is_init() const
    {
      return _compressionEncoderStreambuf.is_init();
    }

    size_t get_bytes_read() const
    {
      return _compressionEncoderStreambuf.get_bytes_read();
    }

    size_t get_bytes_written() const
    {
      return _compressionEncoderStreambuf.get_bytes_written();
    }

  private:
    compression_encoder_streambuf<ELEM_TYPE, TRAITS_TYPE> _compressionEncoderStreambuf;
};

//////////////////////////////////////////////////////////////////////////

typedef basic_compression_encoder_stream<uint8_t, std::char_traits<uint8_t>>  byte_compression_encoder_stream;
typedef basic_compression_encoder_stream<char, std::char_traits<char>>        compression_encoder_stream;
typedef basic_compression_encoder_stream<wchar_t, std::char_traits<wchar_t>>  wcompression_encoder_stream;
