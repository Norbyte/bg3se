#pragma once
#include <streambuf>
#include <algorithm>

template <typename ELEM_TYPE, typename TRAITS_TYPE>
class tee_streambuf:
  public std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE>
{
  public:
    typedef std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE> base_type;
    typedef typename std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE>::traits_type traits_type;

    typedef typename base_type::char_type char_type;
    typedef typename base_type::int_type  int_type;
    typedef typename base_type::pos_type  pos_type;
    typedef typename base_type::off_type  off_type;

    tee_streambuf& bind(base_type* sb)
    {
      _sbCollection.push_back(sb);
      return *this;
    }

    tee_streambuf& bind(std::basic_ostream<ELEM_TYPE, TRAITS_TYPE>& stream)
    {
      _sbCollection.push_back(stream.rdbuf());
      return *this;
    }

  protected:
    int_type overflow(int_type c = traits_type::eof()) override
    {
      bool failed = false;
      
      for (auto* sb : _sbCollection)
      {
        if (sb->sputc(c) == traits_type::eof())
        {
          failed = true;
        }
      }

      return failed ? traits_type::eof() : c;
    }
  
    int sync() override
    {
      bool failed = false;

      for (auto* sb : _sbCollection)
      {
        if (sb->pubsync() == -1)
        {
          failed = true;
        }
      }

      return failed ? -1 : 0;
    }
  
  private:
    std::vector<base_type*> _sbCollection;
};
