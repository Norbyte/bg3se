#pragma once
#include <iostream>
#include "streambuffs/zip_crypto_streambuf.h"

template <typename ELEM_TYPE, typename TRAITS_TYPE>
class basic_zip_cryptostream
  : public std::basic_iostream<ELEM_TYPE, TRAITS_TYPE>
{
  public:
    basic_zip_cryptostream()
      : std::basic_iostream<ELEM_TYPE, TRAITS_TYPE>(&_zipCryptoStreambuf)
    {

    }

    basic_zip_cryptostream(std::basic_ostream<ELEM_TYPE, TRAITS_TYPE>& stream, const ELEM_TYPE* password)
      : std::basic_iostream<ELEM_TYPE, TRAITS_TYPE>(&_zipCryptoStreambuf)
      , _zipCryptoStreambuf(stream, password)
    {

    }

    basic_zip_cryptostream(std::basic_istream<ELEM_TYPE, TRAITS_TYPE>& stream, const ELEM_TYPE* password)
      : std::basic_iostream<ELEM_TYPE, TRAITS_TYPE>(&_zipCryptoStreambuf)
      , _zipCryptoStreambuf(stream, password)
    {

    }

    void init(std::basic_ostream<ELEM_TYPE, TRAITS_TYPE>& stream, const ELEM_TYPE* password)
    {
      _zipCryptoStreambuf.init(stream, password);
    }

    void init(std::basic_istream<ELEM_TYPE, TRAITS_TYPE>& stream, const ELEM_TYPE* password)
    {
      _zipCryptoStreambuf.init(stream, password);
    }

    bool is_init() const
    {
      return _zipCryptoStreambuf.is_init();
    }

    void set_final_byte(uint8_t c)
    {
      _zipCryptoStreambuf.set_final_byte(c);
    }

    bool has_correct_password() const
    {
      return _zipCryptoStreambuf.has_correct_password();
    }

    bool prepare_for_decryption()
    {
      return _zipCryptoStreambuf.prepare_for_decryption();
    }

  private:
    zip_crypto_streambuf<ELEM_TYPE, TRAITS_TYPE> _zipCryptoStreambuf;
};

//////////////////////////////////////////////////////////////////////////

typedef basic_zip_cryptostream<uint8_t, std::char_traits<uint8_t>>  byte_zip_cryptostream;
typedef basic_zip_cryptostream<char, std::char_traits<char>>        zip_cryptostream;
