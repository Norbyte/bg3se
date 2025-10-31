#pragma once
#include <ostream>
#include <cstdint>
#include "streambuffs/tee_streambuff.h"

/**
 * \brief Basic teestream. Distributes the input data into every bound output stream.
 */
template <typename ELEM_TYPE, typename TRAITS_TYPE>
class basic_teestream
  : public std::basic_ostream<ELEM_TYPE, TRAITS_TYPE>
{
  public:
    basic_teestream()
      : std::basic_ostream<ELEM_TYPE, TRAITS_TYPE>(&_teeStreambuf)
    {

    }

    basic_teestream(basic_teestream<ELEM_TYPE, TRAITS_TYPE>&& other)
      : basic_teestream()
    {
      _teeStreambuf = std::move(other._teeStreambuf);
      this->swap(other);
    }

    basic_teestream& bind(std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE>* sb)
    {
      _teeStreambuf.bind(sb);
      return *this;
    }

    basic_teestream& bind(std::basic_ostream<ELEM_TYPE, TRAITS_TYPE>& stream)
    {
      _teeStreambuf.bind(stream);
      return *this;
    }

    basic_teestream move()
    {
      return std::move(*this);
    }

  private:
    basic_teestream(const basic_teestream&);
    basic_teestream& operator = (const basic_teestream&);

    tee_streambuf<ELEM_TYPE, TRAITS_TYPE> _teeStreambuf;
};

//////////////////////////////////////////////////////////////////////////

typedef basic_teestream<uint8_t, std::char_traits<uint8_t>>  byte_teestream;
typedef basic_teestream<char, std::char_traits<char>>        teestream;
typedef basic_teestream<wchar_t, std::char_traits<wchar_t>>  wteestream;
