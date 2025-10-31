#pragma once
#include <istream>
#include "streambuffs/crc32_streambuf.h"

/**
 * \brief Basic CRC32 output stream. Computes CRC32 of input data.
 */
template <typename ELEM_TYPE, typename TRAITS_TYPE>
class basic_crc32stream
  : public std::basic_istream<ELEM_TYPE, TRAITS_TYPE>
{
  public:
    basic_crc32stream()
      : std::basic_istream<ELEM_TYPE, TRAITS_TYPE>(&_crc32Streambuf)
    {

    }

    basic_crc32stream(std::basic_istream<ELEM_TYPE, TRAITS_TYPE>* stream)
      : std::basic_istream<ELEM_TYPE, TRAITS_TYPE>(&_crc32Streambuf)
      , _crc32Streambuf(stream)
    {

    }

    void init(std::basic_istream<ELEM_TYPE, TRAITS_TYPE>& stream)
    {
      _crc32Streambuf.init(stream);
    }

    size_t get_bytes_read() const
    {
      return _crc32Streambuf.get_bytes_read();
    }

    uint32_t get_crc32() const
    {
      return _crc32Streambuf.get_crc32();
    }

  private:
    crc32_streambuf<ELEM_TYPE, TRAITS_TYPE> _crc32Streambuf;
};

//////////////////////////////////////////////////////////////////////////

typedef basic_crc32stream<uint8_t, std::char_traits<uint8_t>>  byte_crc32stream;
typedef basic_crc32stream<char, std::char_traits<char>>        crc32stream;
typedef basic_crc32stream<wchar_t, std::char_traits<wchar_t>>  wcrc32stream;
