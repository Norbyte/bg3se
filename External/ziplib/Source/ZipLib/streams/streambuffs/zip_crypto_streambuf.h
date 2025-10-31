#pragma once
#include <streambuf>
#include <iostream>
#include <cstdint>
#include <chrono>
#include <random>
#include <cassert>

#include "../../extlibs/zlib/zlib.h"

template <typename ELEM_TYPE, typename TRAITS_TYPE>
class zip_crypto_streambuf
  : public std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE>
{
  public:
    typedef std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE> base_type;
    typedef typename std::basic_streambuf<ELEM_TYPE, TRAITS_TYPE>::traits_type traits_type;

    typedef typename base_type::char_type char_type;
    typedef typename base_type::int_type  int_type;
    typedef typename base_type::pos_type  pos_type;
    typedef typename base_type::off_type  off_type;

    zip_crypto_streambuf()
      : _internalBuffer(nullptr)
      , _inputStream(nullptr)
      , _outputStream(nullptr)
      , _finalByte(-1)
      , _encryptionHeaderRead(false)
      , _encryptionHeaderWritten(false)
    {
      static_assert(sizeof(ELEM_TYPE) == 1, "size of ELEM_TYPE must be 1");
    }

    zip_crypto_streambuf(std::basic_ostream<ELEM_TYPE, TRAITS_TYPE>& stream, const ELEM_TYPE* password)
      : zip_crypto_streambuf()
    {
      static_assert(sizeof(ELEM_TYPE) == 1, "size of ELEM_TYPE must be 1");
      init(stream, password);
    }

    zip_crypto_streambuf(std::basic_istream<ELEM_TYPE, TRAITS_TYPE>& stream, const ELEM_TYPE* password)
      : zip_crypto_streambuf()
    {
      static_assert(sizeof(ELEM_TYPE) == 1, "size of ELEM_TYPE must be 1");
      init(stream, password);
    }

    ~zip_crypto_streambuf()
    {
      if (_internalBuffer != nullptr)
      {
        delete[] _internalBuffer;
      }
    }

    void init(std::basic_ostream<ELEM_TYPE, TRAITS_TYPE>& stream, const ELEM_TYPE* password)
    {
      _inputStream = nullptr;
      _outputStream = &stream;
      _finalByte = -1;
      _encryptionHeaderRead = false;
      _encryptionHeaderWritten = false;

      init_internal(password);
    }

    void init(std::basic_istream<ELEM_TYPE, TRAITS_TYPE>& stream, const ELEM_TYPE* password)
    {
      _inputStream = &stream;
      _outputStream = nullptr;
      _finalByte = -1;
      _encryptionHeaderRead = false;
      _encryptionHeaderWritten = false;

      init_internal(password);
    }

    bool is_init() const
    {
      return (_inputStream != nullptr || _outputStream != nullptr);
    }

    void set_final_byte(uint8_t c)
    {
      _finalByte = int(c);
    }

    bool has_correct_password() const
    {
      return (uint8_t(_finalByte) == _encryptionHeader.u8[11]);
    }

    bool prepare_for_decryption()
    {
      if (_inputStream == nullptr)
      {
        return false;
      }

      _inputStream->read(reinterpret_cast<ELEM_TYPE*>(&_encryptionHeader), sizeof(_encryptionHeader));
      finish_decryption_header();
      _encryptionHeaderRead = true;

      return has_correct_password();
    }

  protected:
    int_type overflow(int_type c = traits_type::eof()) override
    {
      bool is_eof = traits_type::eq_int_type(c, traits_type::eof());

      // buffering would be great, maybe?
      if (!is_eof)
      {
        if (!_encryptionHeaderWritten)
        {
          finish_encryption_header();
          _outputStream->write(reinterpret_cast<ELEM_TYPE*>(&_encryptionHeader), sizeof(_encryptionHeader));
          _encryptionHeaderWritten = true;
        }

        uint8_t encryptedByte = encrypt_byte(uint8_t(c));
        _outputStream->write(reinterpret_cast<ELEM_TYPE*>(&encryptedByte), sizeof(encryptedByte));

        return encryptedByte;
      }

      return traits_type::eof();
    }

    int_type underflow() override
    {
      // if we're going to decrypt,
      // the call to prepare_for_decryption() is needed
      if (!_encryptionHeaderRead)
      {
        return traits_type::eof();
      }

      // buffer exhausted
      if (this->gptr() >= this->egptr())
      {
        ELEM_TYPE* base = _internalBuffer;
        _inputStream->read(_internalBuffer, INTERNAL_BUFFER_SIZE);
      
        size_t n = static_cast<size_t>(_inputStream->gcount());

        if (n == 0)
        {
          return traits_type::eof();
        }

        decrypt_internal_buffer(n);

        // set buffer pointers
        this->setg(base, base, base + n);
      }

      return traits_type::to_int_type(*this->gptr());
    }

    int sync() override
    {
      return _outputStream->rdbuf()->pubsync();
    }

  private:
    static uint32_t crc32_byte(uint32_t prevCrc32, uint8_t c)
    {
      return uint32_t(get_crc_table()[(prevCrc32 ^ c) & 0xff] ^ (prevCrc32 >> 8));
    }
    
    bool init_internal(const ELEM_TYPE* password)
    {
      assert(password != nullptr);

      _keys.u32[0] = 0x12345678;
      _keys.u32[1] = 0x23456789;
      _keys.u32[2] = 0x34567890;

      do 
      {
        update_keys(uint8_t(*password++));
      } while (*password != '\0');

      // make encryption header
      auto seed = std::chrono::system_clock::now().time_since_epoch().count();
      std::mt19937 generator(static_cast<uint32_t>(seed));

      _encryptionHeader.u32[0] = generator();
      _encryptionHeader.u32[1] = generator();
      _encryptionHeader.u32[2] = generator();

      // set stream buffer
      _internalBuffer = new ELEM_TYPE[INTERNAL_BUFFER_SIZE];
      ELEM_TYPE* endOfInternalBuffer = _internalBuffer + INTERNAL_BUFFER_SIZE;
      this->setg(endOfInternalBuffer, endOfInternalBuffer, endOfInternalBuffer);

      return true;
    }

    void finish_encryption_header()
    {
      assert(_finalByte != -1);

      _encryptionHeader.u8[11] = uint8_t(_finalByte);

      for (uint8_t& c : _encryptionHeader.u8)
      {
        c = encrypt_byte(c);
      }
    }

    void finish_decryption_header()
    {
      for (uint8_t& c : _encryptionHeader.u8)
      {
        c = decrypt_byte(c);
      }
    }

    uint8_t encrypt_byte(uint8_t c)
    {
      uint8_t result = uint8_t(c ^ get_magic_byte());
      
      update_keys(c);

      return result;
    }

    uint8_t decrypt_byte(uint8_t c)
    {
      uint8_t result = uint8_t(c ^ get_magic_byte());

      update_keys(result);

      return result;
    }

    void decrypt_internal_buffer(size_t length)
    {
      for (size_t i = 0; i < length; ++i)
      {
        _internalBuffer[i] = decrypt_byte(_internalBuffer[i]);
      }
    }

    void update_keys(uint8_t c)
    {
      _keys.u32[0] = crc32_byte(_keys.u32[0], c);
      _keys.u32[1] = _keys.u32[1] + uint8_t(_keys.u32[0] & 0x000000ff);
      _keys.u32[1] = _keys.u32[1] * 0x08088405 + 1;
      _keys.u32[2] = crc32_byte(_keys.u32[2], _keys.u32[1] >> 24);
    }

    uint8_t get_magic_byte() const
    {
      uint16_t t = uint16_t(uint16_t(_keys.u32[2] & 0xFFFF) | 2);
      return uint8_t((t * (t ^ 1)) >> 8);
    }

    union encryption_header
    {
      uint8_t  u8[12];
      uint32_t u32[3];
    };

    //////////////////////////////////////////////////////////////////////////

    enum : size_t
    {
      INTERNAL_BUFFER_SIZE = 1 << 15
    };

    ELEM_TYPE* _internalBuffer;

    std::basic_istream<ELEM_TYPE, TRAITS_TYPE>* _inputStream;
    std::basic_ostream<ELEM_TYPE, TRAITS_TYPE>* _outputStream;
    encryption_header _keys;
    encryption_header _encryptionHeader;
    int _finalByte;
    bool _encryptionHeaderRead;
    bool _encryptionHeaderWritten;
};
