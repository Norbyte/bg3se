#pragma once
#include <cstdint>
#include <string>
#include <iostream>

class ZipArchive;
class ZipArchiveEntry;

namespace detail {

struct EndOfCentralDirectoryBlockBase
{
  enum : size_t
  {
    SIZE_IN_BYTES = 22
  };

  uint32_t Signature;
  uint16_t NumberOfThisDisk;
  uint16_t NumberOfTheDiskWithTheStartOfTheCentralDirectory;
  uint16_t NumberOfEntriesInTheCentralDirectoryOnThisDisk;
  uint16_t NumberOfEntriesInTheCentralDirectory;
  uint32_t SizeOfCentralDirectory;
  uint32_t OffsetOfStartOfCentralDirectoryWithRespectToTheStartingDiskNumber;
  uint16_t CommentLength;
};

struct EndOfCentralDirectoryBlock
  : EndOfCentralDirectoryBlockBase
{
  enum : uint32_t
  {
    SignatureConstant = 0x06054b50
  };

  std::string Comment;

  EndOfCentralDirectoryBlock();

  private:
    friend class ::ZipArchive;
    friend class ::ZipArchiveEntry;

    bool Deserialize(std::istream& stream);
    void Serialize(std::ostream& stream);
};

}
