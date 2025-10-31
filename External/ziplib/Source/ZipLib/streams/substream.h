#pragma once
#include <istream>
#include "streambuffs/sub_streambuf.h"

/**
 * \brief Basic input substream. Creates a virtual stream over an existing input stream.
 *        The substream starts at the position 0 and continues until EOF or the specified length.
 */
template <typename ELEM_TYPE, typename TRAITS_TYPE>
class basic_isubstream
  : public std::basic_istream<ELEM_TYPE, TRAITS_TYPE>
{
  public:
    typedef typename std::basic_istream<ELEM_TYPE, TRAITS_TYPE>::pos_type pos_type;

    basic_isubstream()
      : std::basic_istream<ELEM_TYPE, TRAITS_TYPE>(&_subStreambuf)
    {

    }

    basic_isubstream(std::basic_istream<ELEM_TYPE, TRAITS_TYPE>& input, pos_type startOffset = 0)
      : std::basic_istream<ELEM_TYPE, TRAITS_TYPE>(&_subStreambuf)
      , _subStreambuf(input, startOffset, static_cast<size_t>(-1))
    {

    }

    basic_isubstream(std::basic_istream<ELEM_TYPE, TRAITS_TYPE>& input, pos_type startOffset, size_t length)
      : std::basic_istream<ELEM_TYPE, TRAITS_TYPE>(&_subStreambuf)
      , _subStreambuf(input, startOffset, length)
    {

    }

    void init(std::basic_istream<ELEM_TYPE, TRAITS_TYPE>& input, pos_type startOffset = 0)
    {
      _subStreambuf.init(input, startOffset, static_cast<size_t>(-1));
    }

    void init(std::basic_istream<ELEM_TYPE, TRAITS_TYPE>& input, pos_type startOffset, size_t length)
    {
      _subStreambuf.init(input, startOffset, length);
    }

    bool is_init() const
    {
      return _subStreambuf.is_init();
    }

  private:
    sub_streambuf<ELEM_TYPE, TRAITS_TYPE> _subStreambuf;
};

//////////////////////////////////////////////////////////////////////////

typedef basic_isubstream<uint8_t, std::char_traits<uint8_t>>  byte_isubstream;
typedef basic_isubstream<char, std::char_traits<char>>        isubstream;
typedef basic_isubstream<wchar_t, std::char_traits<wchar_t>>  wisubstream;
