#pragma once
#include "ZipGenericExtraField.h"

#include <iostream>
#include <vector>
#include <cstdint>

class ZipArchiveEntry;

namespace detail {

struct ZipCentralDirectoryFileHeader;

struct ZipLocalFileHeaderBase
{
  enum : size_t
  {
    SIZE_IN_BYTES = 30
  };

  uint32_t Signature;
  uint16_t VersionNeededToExtract;
  uint16_t GeneralPurposeBitFlag;
  uint16_t CompressionMethod;
  uint16_t LastModificationTime;
  uint16_t LastModificationDate;
  uint32_t Crc32;
  uint32_t CompressedSize;
  uint32_t UncompressedSize;
  uint16_t FilenameLength;
  uint16_t ExtraFieldLength;
};

struct ZipLocalFileHeader
  : ZipLocalFileHeaderBase
{
  enum : uint32_t
  {
    SignatureConstant       = 0x04034b50,
    DataDescriptorSignature = 0x08074b50
  };

  std::string Filename;
  std::vector<ZipGenericExtraField> ExtraFields;

  ZipLocalFileHeader();

  private:
    friend class ::ZipArchiveEntry;

    void SyncWithCentralDirectoryFileHeader(ZipCentralDirectoryFileHeader& cdfh);

    bool Deserialize(std::istream& stream);
    void Serialize(std::ostream& stream);

    void DeserializeAsDataDescriptor(std::istream& stream);
    void SerializeAsDataDescriptor(std::ostream& stream);
};

}
