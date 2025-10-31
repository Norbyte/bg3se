#include "ZipLocalFileHeader.h"
#include "ZipCentralDirectoryFileHeader.h"

#include "../streams/serialization.h"

#include <cstring>

namespace detail {

ZipLocalFileHeader::ZipLocalFileHeader()
{
  memset(this, 0, sizeof(ZipLocalFileHeaderBase));
  Signature = SignatureConstant;
}

void ZipLocalFileHeader::SyncWithCentralDirectoryFileHeader(ZipCentralDirectoryFileHeader& cdfh)
{
  VersionNeededToExtract = cdfh.VersionNeededToExtract;
  GeneralPurposeBitFlag = cdfh.GeneralPurposeBitFlag;
  CompressionMethod = cdfh.CompressionMethod;
  LastModificationTime = cdfh.LastModificationTime;
  LastModificationDate = cdfh.LastModificationDate;
  Crc32 = cdfh.Crc32;
  CompressedSize = cdfh.CompressedSize;
  UncompressedSize = cdfh.UncompressedSize;

  Filename = cdfh.Filename;
  FilenameLength = static_cast<uint16_t>(Filename.length());
}

bool ZipLocalFileHeader::Deserialize(std::istream& stream)
{
  if (sizeof(ZipLocalFileHeaderBase) == ZipLocalFileHeaderBase::SIZE_IN_BYTES)
  {
    deserialize<ZipLocalFileHeaderBase>(stream, *this);
  }
  else
  {
    deserialize(stream, Signature);
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
  }

  // If there is not any other entry.
  if (stream.fail() || Signature != SignatureConstant)
  {
    stream.clear();
    stream.seekg(static_cast<std::ios::off_type>(static_cast<std::ios::streamoff>(stream.tellg()) - stream.gcount()), std::ios::beg);
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

    // Some archives do not store extra field in the form of tag, size and data tuples.
    // That may cause the above while cycle exit prior to reaching the extra field end,
    // which causes wrong data offset returned by ZipArchiveEntry::GetOffsetOfCompressedData().
    // Seek forcefully to the end of extra field to mitigate that problem.
    stream.seekg(extraFieldEnd, std::ios::beg);
  }

  return true;
}

void ZipLocalFileHeader::Serialize(std::ostream& stream)
{
  FilenameLength = static_cast<uint16_t>(Filename.length());
  ExtraFieldLength = 0;

  for (auto& extraField : ExtraFields)
  {
    ExtraFieldLength += static_cast<uint16_t>(ZipGenericExtraField::HEADER_SIZE + extraField.Data.size());
  }

  if (sizeof(ZipLocalFileHeaderBase) == ZipLocalFileHeaderBase::SIZE_IN_BYTES)
  {
    serialize<ZipLocalFileHeaderBase>(stream, *this);
  }
  else
  {
    serialize(stream, Signature);
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
  }

  serialize(stream, Filename);

  if (ExtraFieldLength > 0)
  {
    for (auto& extraField : ExtraFields)
    {
      extraField.Serialize(stream);
    }
  }
}

void ZipLocalFileHeader::DeserializeAsDataDescriptor(std::istream& stream)
{
  uint32_t firstWord;
  deserialize(stream, firstWord);

  // the signature is optional, if it's missing,
  // we're starting with crc32
  if (firstWord != DataDescriptorSignature)
  {
    deserialize(stream, Crc32);
  }
  else
  {
    Crc32 = firstWord;
  }

  deserialize(stream, CompressedSize);
  deserialize(stream, UncompressedSize);
}

void ZipLocalFileHeader::SerializeAsDataDescriptor(std::ostream& stream)
{
  serialize(stream, DataDescriptorSignature);
  serialize(stream, Crc32);
  serialize(stream, CompressedSize);
  serialize(stream, UncompressedSize);
}

}
