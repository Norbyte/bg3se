#pragma once
#include <streambuf>
#include <cstdint>
#include <cassert>

template <typename ELEM_TYPE, typename TRAITS_TYPE>
class mem_streambuf
  : public std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE>
{
  public:
    typedef std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE> base_type;
    typedef typename std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE>::traits_type traits_type;

    typedef typename base_type::char_type char_type;
    typedef typename base_type::int_type  int_type;
    typedef typename base_type::pos_type  pos_type;
    typedef typename base_type::off_type  off_type;

    mem_streambuf(ELEM_TYPE* buffer, size_t length)
    {
      // set stream buffer
      ELEM_TYPE* endOfBuffer = buffer + length;
      this->setg(buffer, buffer, endOfBuffer);
      this->setp(buffer, buffer, endOfBuffer);
    }

  protected:
    int_type underflow() override
    {
      // buffer not exhausted
      if (this->gptr() < this->egptr())
      {
        return traits_type::to_int_type(*this->gptr());
      }

      return traits_type::eof();
    }

    int_type overflow(int_type c = traits_type::eof()) override
    {
      assert(this->pptr() != nullptr);

      // check if incoming character is not EOF
      // also, we might not overflow the buffer
      if (!traits_type::eq_int_type(c, traits_type::eof()) &&
          this->pptr() < this->epptr()) 
      {
        *this->pptr() = c;
        this->pbump(1); // shift pptr() by 1
        return c;       // return value must not be EOF
      }

      return traits_type::eof();
    }

    pos_type seekpos(pos_type pos, std::ios::openmode which = std::ios::in | std::ios::out) override
    {
      static const off_type BAD_OFFSET(-1);

      // depending on which pointer we move, it must be set
      assert(which & std::ios::in  ? this->gptr() != nullptr : true);
      assert(which & std::ios::out ? this->pptr() != nullptr : true);

      // do not seek over the length of buffer
      if (pos <= pos_type(this->egptr() - this->eback()))
      {
        // position of read buffer
        if (which & std::ios::in)
        {
          // move gptr to the right position
          this->gbump(static_cast<int>(this->eback() - this->gptr() + pos));

          if (which & std::ios::out)
          {
            // change write position to match
            this->setp(this->pbase(), this->gptr(), this->epptr());
          }
        }

        // position of write buffer
        else if (which & std::ios::out)
        {
          // move pptr to the right position
          this->pbump(static_cast<int>(this->eback() - this->pptr() + pos));
        }

        return pos;
      }

      // something went wrong
      return pos_type(BAD_OFFSET);
    }

    pos_type seekoff(off_type off, std::ios::seekdir dir, std::ios::openmode which = std::ios::in | std::ios::out) override
    {
      static const off_type BAD_OFFSET(-1);

      // depending on which pointer we move, it must be set
      assert(which & std::ios::in  ? this->gptr() != nullptr : true);
      assert(which & std::ios::out ? this->pptr() != nullptr : true);

      if (which & std::ios::in)
      {
        // position within read buffer
        if (dir == std::ios::end)
        {
          off += off_type(this->egptr() - this->eback());
        }
        else if (dir == std::ios::cur && (which & std::ios::out) == 0)
        {
          off += off_type(this->gptr() - this->eback());
        }
        else if (dir != std::ios::beg)
        {
          off = BAD_OFFSET;
        }

        if (off >= 0 && off <= off_type(this->egptr() - this->eback()))
        {
          // move gptr to the right position
          this->gbump(static_cast<int>(this->eback() - this->gptr() + off));
          if (which & std::ios::out)
          {
            // change write position to match
            this->setp(this->pbase(), this->gptr(), this->epptr());
          }
        }
        else
        {
          off = BAD_OFFSET;
        }
      }
      else if (which & std::ios::out)
      {
        // position within write buffer
        if (dir == std::ios::end)
        {
          off += off_type(this->egptr() - this->eback());
        }
        else if (dir == std::ios::cur)
        {
          off += off_type(this->pptr() - this->eback());
        }
        else if (dir != std::ios::beg)
        {
          // if dir is ios::beg, just leave the offset set as is
          // if dir is not neither of the valid directions, set bad offset
          off = BAD_OFFSET;
        }

        if (off >= 0 && off <= off_type(this->egptr() - this->eback()))
        {
          // change write position
          this->pbump(static_cast<int>(this->eback() - this->pptr() + off));
        }
        else
        {
          off = BAD_OFFSET;
        }
      }
      else
      {
        // neither read nor write buffer selected, fail
        off = BAD_OFFSET;
      }
      return (pos_type(off));
    }

    int_type pbackfail(int_type c = traits_type::eof()) override
    {
      assert(this->gptr() != nullptr);

      // put an element back to stream
      if (this->gptr() <= this->eback() ||  // do not underflow
          (!traits_type::eq_int_type(traits_type::eof(), c) &&
           !traits_type::eq(traits_type::to_char_type(c), this->gptr()[-1])))
      {
        // can't put back, fail
        return (traits_type::eof());
      }
      else
      {
        // back up one position and store put-back character
        this->gbump(-1);

        if (!traits_type::eq_int_type(traits_type::eof(), c))
        {
          *this->gptr() = traits_type::to_char_type(c);
        }

        return (traits_type::not_eof(c));
      }
    }

  private:
    void setp(ELEM_TYPE* first, ELEM_TYPE* next, ELEM_TYPE* last)
    {
      this->std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE>::setp(first, last);
      this->pbump(static_cast<int>(next - first));
    }
};
