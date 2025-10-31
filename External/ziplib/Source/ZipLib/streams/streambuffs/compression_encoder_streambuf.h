#pragma once
#include <streambuf>
#include <ostream>
#include <cstdint>
#include <thread>
#include <memory>
#include <condition_variable>

#include "../../compression/compression_interface.h"

template <typename ELEM_TYPE, typename TRAITS_TYPE>
class compression_encoder_streambuf
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

    typedef compression_encoder_interface_basic<ELEM_TYPE, TRAITS_TYPE> icompression_encoder_type;
    typedef std::shared_ptr<icompression_encoder_type>                  icompression_encoder_ptr_type;

    compression_encoder_streambuf()
    {

    }

    compression_encoder_streambuf(icompression_encoder_ptr_type compressionEncoder, ostream_type& stream)
    {
      init(compressionEncoder, stream);
    }

    compression_encoder_streambuf(icompression_encoder_ptr_type compressionEncoder, compression_encoder_properties_interface& props, ostream_type& stream)
    {
      init(compressionEncoder, props, stream);
    }

    virtual ~compression_encoder_streambuf()
    {
      sync();
    }

    void init(icompression_encoder_ptr_type compressionEncoder, ostream_type& stream)
    {
      _compressionEncoder = compressionEncoder;

      // compression encoder init
      _compressionEncoder->init(stream);

      // set stream buffer
      this->setp(_compressionEncoder->get_buffer_begin(), _compressionEncoder->get_buffer_end() - 1);
    }

    void init(icompression_encoder_ptr_type compressionEncoder, compression_encoder_properties_interface& props, ostream_type& stream)
    {
      _compressionEncoder = compressionEncoder;

      // compression encoder init
      _compressionEncoder->init(stream, props);

      // set stream buffer
      this->setp(_compressionEncoder->get_buffer_begin(), _compressionEncoder->get_buffer_end() - 1);
    }

    bool is_init() const
    {
      return _compressionEncoder->is_init();
    }

    size_t get_bytes_read() const
    {
      return _compressionEncoder->get_bytes_read();
    }

    size_t get_bytes_written() const
    {
      return _compressionEncoder->get_bytes_written();
    }

  protected:
    int_type overflow(int_type c = traits_type::eof()) override
    {
      bool is_eof = traits_type::eq_int_type(c, traits_type::eof());

      // if the input is not EOF, just put it into the buffer
      // and increment current pointer
      if (!is_eof)
      {
        *this->pptr() = c;
        this->pbump(1);
      }

      // if input buffer is full or input is EOF,
      // compress the buffer
      if (this->pptr() >= this->epptr() || is_eof)
      {
        process();
      }

      // not eof
      return ~traits_type::eof();
    }

    int sync() override
    {
      process();
      _compressionEncoder->sync();

      return 0;
    }

  private:
    void process()
    {
      std::ptrdiff_t inputLength = this->pptr() - this->pbase();
      _compressionEncoder->encode_next(inputLength);

      // set pointers for new buffer
      this->setp(_compressionEncoder->get_buffer_begin(), _compressionEncoder->get_buffer_end() - 1);
    }

    icompression_encoder_ptr_type _compressionEncoder;
};
