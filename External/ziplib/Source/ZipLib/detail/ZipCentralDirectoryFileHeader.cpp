#include "ZipCentralDirectoryFileHeader.h"
#include "ZipLocalFileHeader.h"

#include "../streams/serialization.h"

#include <cstring>
#include <ctime>

namespace detail {

ZipCentralDirectoryFileHeader::ZipCentralDirectoryFileHeader()
{
  memset(this, 0, sizeof(ZipCentralDirectoryFileHeaderBase));
  Signature = SignatureConstant;
}

void ZipCentralDirectoryFileHeader::SyncWithLocalFileHeader(ZipLocalFileHeader& lfh)
{
  Crc32 = lfh.Crc32;
  CompressedSize = lfh.CompressedSize;
  UncompressedSize = lfh.UncompressedSize;

  FilenameLength = static_cast<uint16_t>(Filename.length());
  FileCommentLength = static_cast<uint16_t>(FileComment.length());
}

bool ZipCentralDirectoryFileHeader::Deserialize(std::istream& stream)
{
  if (sizeof(ZipCentralDirectoryFileHeaderBase) == ZipCentralDirectoryFileHeaderBase::SIZE_IN_BYTES)
  {
    deserialize<ZipCentralDirectoryFileHeaderBase>(stream, *this);
  }
  else
  {
    deserialize(stream, Signature);
    deserialize(stream, VersionMadeBy);
    deserialize(stream, VersionNeededToExtract);
    deserialize(stream, GeneralPurposeBitFlag);
    deserialize(stream, CompressionMethod);
    deserialize(stream, LastModificationTime);
    deserialize(stream, LastModificationDate);
    deserialize(stream, Crc32);
    deserialize(stream, CompressedSize);
    deserialize(stream, UncompressedSize);
    deserialize(stream, FilenameLength);
    deserialize(stream, ExtraFieldLength);
    deserialize(stream, FileCommentLength);
    deserialize(stream, DiskNumberStart);
    deserialize(stream, InternalFileAttributes);
    deserialize(stream, ExternalFileAttributes);
    deserialize(stream, RelativeOffsetOfLocalHeader);
  }

  // If there is not any other entry.
  if (stream.fail() || Signature != SignatureConstant)
  {
    stream.clear();
    stream.seekg(static_cast<std::ios::off_type>(static_cast<std::ios::streamoff>(stream.tellg()) - stream.gcount()), std::istream::beg);
    return false;
  }

  deserialize(stream, Filename, FilenameLength);

  if (ExtraFieldLength > 0)
  {
    ZipGenericExtraField extraField;

    auto extraFieldEnd = ExtraFieldLength + stream.tellg();

    while (extraField.Deserialize(stream, extraFieldEnd))
    {
      ExtraFields.push_back(extraField);
    }
  }

  deserialize(stream, FileComment, FileCommentLength);

  return true;
}

void ZipCentralDirectoryFileHeader::Serialize(std::ostream& stream)
{
  FilenameLength = static_cast<uint16_t>(Filename.length());
  FileCommentLength = static_cast<uint16_t>(FileComment.length());
  ExtraFieldLength = 0;

  for (auto& extraField : ExtraFields)
  {
    ExtraFieldLength += static_cast<uint16_t>(ZipGenericExtraField::HEADER_SIZE + extraField.Data.size());
  }

  if (sizeof(ZipCentralDirectoryFileHeaderBase) == ZipCentralDirectoryFileHeaderBase::SIZE_IN_BYTES)
  {
    serialize<ZipCentralDirectoryFileHeaderBase>(stream, *this);
  }
  else
  {
    serialize(stream, Signature);
    serialize(stream, VersionMadeBy);
    serialize(stream, VersionNeededToExtract);
    serialize(stream, GeneralPurposeBitFlag);
    serialize(stream, CompressionMethod);
    serialize(stream, LastModificationTime);
    serialize(stream, LastModificationDate);
    serialize(stream, Crc32);
    serialize(stream, CompressedSize);
    serialize(stream, UncompressedSize);
    serialize(stream, FilenameLength);
    serialize(stream, ExtraFieldLength);
    serialize(stream, FileCommentLength);
    serialize(stream, DiskNumberStart);
    serialize(stream, InternalFileAttributes);
    serialize(stream, ExternalFileAttributes);
    serialize(stream, RelativeOffsetOfLocalHeader);
  }

  serialize(stream, Filename);

  if (ExtraFieldLength > 0)
  {
    for (auto& extraField : ExtraFields)
    {
      extraField.Serialize(stream);
    }
  }

  serialize(stream, FileComment);
}

}
