#pragma once
#include <streambuf>

template <typename ELEM_TYPE, typename TRAITS_TYPE>
class null_streambuf
  : public std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE>
{
  public:
    typedef std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE> base_type;
    typedef typename std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE>::traits_type traits_type;

    typedef typename base_type::char_type char_type;
    typedef typename base_type::int_type  int_type;
    typedef typename base_type::pos_type  pos_type;
    typedef typename base_type::off_type  off_type;

    null_streambuf()
    {

    }

  protected:
    std::streamsize xsgetn(char_type* s, std::streamsize count) override
    {
      return 0;
    }

    std::streamsize xsputn(const ELEM_TYPE* ptr, std::streamsize count) override
    {
      return count;
    }
};
