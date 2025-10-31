#pragma once
#include <cstdint>
#include <vector>
#include <iostream>

namespace detail {
  
struct ZipGenericExtraField
{
  enum : size_t
  {
    HEADER_SIZE = 4
  };

  uint16_t Tag;
  uint16_t Size;
  std::vector<uint8_t> Data;

  private:
    friend struct ZipLocalFileHeader;
    friend struct ZipCentralDirectoryFileHeader;

    bool Deserialize(std::istream& stream, std::istream::pos_type extraFieldEnd);
    void Serialize(std::ostream& stream);
};

}
