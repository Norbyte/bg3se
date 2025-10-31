#pragma once
#include <iostream>
#include <cstdint>
#include "streambuffs/null_streambuf.h"

/**
 * \brief Basic null stream.
 */
template <typename ELEM_TYPE, typename TRAITS_TYPE>
class basic_nullstream
  : public std::basic_iostream<ELEM_TYPE, TRAITS_TYPE>
{
  public:
    basic_nullstream()
      : std::basic_iostream<ELEM_TYPE, TRAITS_TYPE>(&_nullStreambuf)
    {

    }

  private:
    null_streambuf<ELEM_TYPE, TRAITS_TYPE> _nullStreambuf;
};

//////////////////////////////////////////////////////////////////////////

typedef basic_nullstream<uint8_t, std::char_traits<uint8_t>> byte_nullstream;
typedef basic_nullstream<char, std::char_traits<char>>       nullstream;
typedef basic_nullstream<wchar_t, std::char_traits<wchar_t>> wnullstream;
