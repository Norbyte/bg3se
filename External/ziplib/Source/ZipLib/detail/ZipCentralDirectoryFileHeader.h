#pragma once
#include "ZipGenericExtraField.h"

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

class ZipArchive;
class ZipArchiveEntry;

namespace detail {

struct ZipLocalFileHeader;

struct ZipCentralDirectoryFileHeaderBase
{
  enum : size_t
  {
    SIZE_IN_BYTES = 46
  };

  uint32_t Signature;
  uint16_t VersionMadeBy;
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
  uint16_t FileCommentLength;
  uint16_t DiskNumberStart;
  uint16_t InternalFileAttributes;
  uint32_t ExternalFileAttributes;
   int32_t RelativeOffsetOfLocalHeader;
};

struct ZipCentralDirectoryFileHeader
  : ZipCentralDirectoryFileHeaderBase
{
  enum : uint32_t
  {
    SignatureConstant = 0x02014b50
  };

  std::string Filename;
  std::vector<ZipGenericExtraField> ExtraFields;
  std::string FileComment;

  ZipCentralDirectoryFileHeader();

  private:
    friend class ::ZipArchive;
    friend class ::ZipArchiveEntry;

    void SyncWithLocalFileHeader(ZipLocalFileHeader& lfh);

    bool Deserialize(std::istream& stream);
    void Serialize(std::ostream& stream);
};

}
