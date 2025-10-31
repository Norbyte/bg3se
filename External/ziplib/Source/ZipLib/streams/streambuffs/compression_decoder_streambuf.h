#pragma once
#include <streambuf>
#include <istream>
#include <cstdint>
#include <memory>

#include "../../compression/compression_interface.h"

template <typename ELEM_TYPE, typename TRAITS_TYPE>
class compression_decoder_streambuf
  : public std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE>
{
  public:
    typedef std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE> base_type;
    typedef typename std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE>::traits_type traits_type;

    typedef typename base_type::char_type char_type;
    typedef typename base_type::int_type  int_type;
    typedef typename base_type::pos_type  pos_type;
    typedef typename base_type::off_type  off_type;

    typedef std::basic_istream<ELEM_TYPE, TRAITS_TYPE> istream_type;
    typedef std::basic_ostream<ELEM_TYPE, TRAITS_TYPE> ostream_type;

    typedef compression_decoder_interface_basic<ELEM_TYPE, TRAITS_TYPE> icompression_decoder_type;
    typedef std::shared_ptr<icompression_decoder_type>                  icompression_decoder_ptr_type;

    compression_decoder_streambuf()
    {

    }

    compression_decoder_streambuf(icompression_decoder_ptr_type compressionDecoder, istream_type& stream)
    {
      init(compressionDecoder, stream);
    }

    compression_decoder_streambuf(icompression_decoder_ptr_type compressionDecoder, compression_decoder_properties_interface& props, istream_type& stream)
    {
      init(compressionDecoder, stream);
    }

    void init(icompression_decoder_ptr_type compressionDecoder, istream_type& stream)
    {
      _compressionDecoder = compressionDecoder;

      // compression decoder init
      _compressionDecoder->init(stream);

      // set stream buffer
      this->setg(_compressionDecoder->get_buffer_end(), _compressionDecoder->get_buffer_end(), _compressionDecoder->get_buffer_end());
    }

    void init(icompression_decoder_ptr_type compressionDecoder, compression_decoder_properties_interface& props, istream_type& stream)
    {
      _compressionDecoder = compressionDecoder;

      // compression decoder init
      _compressionDecoder->init(stream, props);

      // set stream buffer
      this->setg(_compressionDecoder->get_buffer_end(), _compressionDecoder->get_buffer_end(), _compressionDecoder->get_buffer_end());
    }

    bool is_init() const
    {
      return _compressionDecoder->is_init();
    }

    size_t get_bytes_read() const
    {
      return _compressionDecoder->get_bytes_read();
    }

    size_t get_bytes_written() const
    {
      return _compressionDecoder->get_bytes_written();
    }

  protected:
    int_type underflow() override
    {
      // buffer exhausted
      if (this->gptr() >= this->egptr())
      {
        ELEM_TYPE* base = _compressionDecoder->get_buffer_begin();

        // how many bytes has been read
        size_t n = _compressionDecoder->decode_next();

        if (n == 0)
        {
          return traits_type::eof();
        }

        // set buffer pointers
        this->setg(base, base, base + n);
      }

      return traits_type::to_int_type(*this->gptr());
    }

  private:
    icompression_decoder_ptr_type _compressionDecoder;
};
