#include "ZipArchive.h"
#include "streams/serialization.h"
#include <algorithm>
#include <cassert>

#define CALL_CONST_METHOD(expression) \
  const_cast<      std::remove_pointer<std::remove_const<decltype(expression)>::type>::type*>( \
  const_cast<const std::remove_pointer<std::remove_const<decltype(this)      >::type>::type*>(this)->expression)

//////////////////////////////////////////////////////////////////////////
// zip archive

ZipArchive::Ptr ZipArchive::Create()
{
  return ZipArchive::Ptr(new ZipArchive());
}

ZipArchive::Ptr ZipArchive::Create(ZipArchive::Ptr&& other)
{
  ZipArchive::Ptr result(new ZipArchive());

  result->_endOfCentralDirectoryBlock = other->_endOfCentralDirectoryBlock;
  result->_entries = std::move(other->_entries);
  result->_zipStream = other->_zipStream;
  result->_owningStream = other->_owningStream;

  // clean "other"
  other->_zipStream = nullptr;
  other->_owningStream = false;

  return result;
}

ZipArchive::Ptr ZipArchive::Create(std::istream& stream)
{
  ZipArchive::Ptr result(new ZipArchive());

  result->_zipStream = &stream;
  result->_owningStream = false;

  result->ReadEndOfCentralDirectory();
  result->EnsureCentralDirectoryRead();

  return result;
}

ZipArchive::Ptr ZipArchive::Create(std::istream* stream, bool takeOwnership)
{
  ZipArchive::Ptr result(new ZipArchive());

  result->_zipStream = stream;
  result->_owningStream = stream != nullptr ? takeOwnership : false;

  // jesus blew up a school bus when this metod has been implemented
  if (stream != nullptr)
  {
    result->ReadEndOfCentralDirectory();
    result->EnsureCentralDirectoryRead();
  }

  return result;
}

ZipArchive::ZipArchive()
  : _zipStream(nullptr)
  , _owningStream(false)
{

}

ZipArchive::~ZipArchive()
{
  this->InternalDestroy();
}

ZipArchive& ZipArchive::operator = (ZipArchive&& other)
{
  _endOfCentralDirectoryBlock = other._endOfCentralDirectoryBlock;
  _entries = std::move(other._entries);
  _zipStream = other._zipStream;
  _owningStream = other._owningStream;

  // clean "other"
  other._zipStream = nullptr;
  other._owningStream = false;
  
  return *this;
}

ZipArchiveEntry::Ptr ZipArchive::CreateEntry(const std::string& fileName)
{
  ZipArchiveEntry::Ptr result = nullptr;

  if (this->GetEntry(fileName) == nullptr)
  {
    if ((result = ZipArchiveEntry::CreateNew(this, fileName)) != nullptr)
    {
      _entries.push_back(result);
    }
  }

  return result;
}

const std::string& ZipArchive::GetComment() const
{
  return _endOfCentralDirectoryBlock.Comment;
}

void ZipArchive::SetComment(const std::string& comment)
{
  _endOfCentralDirectoryBlock.Comment = comment;
}

ZipArchiveEntry::Ptr ZipArchive::GetEntry(int index)
{
  return _entries[index];
}

ZipArchiveEntry::Ptr ZipArchive::GetEntry(const std::string& entryName)
{
  auto it = std::find_if(_entries.begin(), _entries.end(), [&entryName](ZipArchiveEntry::Ptr& value) { return value->GetFullName() == entryName; });

  if (it != _entries.end())
  {
    return *it;
  }

  return nullptr;
}

size_t ZipArchive::GetEntriesCount() const
{
  return _entries.size();
}

void ZipArchive::RemoveEntry(const std::string& entryName)
{
  auto it = std::find_if(_entries.begin(), _entries.end(), [&entryName](ZipArchiveEntry::Ptr& value) { return value->GetFullName() == entryName; });

  if (it != _entries.end())
  {
    _entries.erase(it);
  }
}

void ZipArchive::RemoveEntry(int index)
{
  _entries.erase(_entries.begin() + index);
}

bool ZipArchive::EnsureCentralDirectoryRead()
{
  detail::ZipCentralDirectoryFileHeader zipCentralDirectoryFileHeader;

  _zipStream->seekg(_endOfCentralDirectoryBlock.OffsetOfStartOfCentralDirectoryWithRespectToTheStartingDiskNumber, std::ios::beg);

  while (zipCentralDirectoryFileHeader.Deserialize(*_zipStream))
  {
    ZipArchiveEntry::Ptr newEntry;

    if ((newEntry = ZipArchiveEntry::CreateExisting(this, zipCentralDirectoryFileHeader)) != nullptr)
    {
      _entries.push_back(newEntry);
    }

    // ensure clearing of the CDFH struct
    zipCentralDirectoryFileHeader = detail::ZipCentralDirectoryFileHeader();
  }

  return true;
}

bool ZipArchive::ReadEndOfCentralDirectory()
{
  const int EOCDB_SIZE     = 22; // sizeof(EndOfCentralDirectoryBlockBase);
  const int SIGNATURE_SIZE = 4;  // sizeof(std::declval<EndOfCentralDirectoryBlockBase>().Signature);
  const int MIN_SHIFT      = (EOCDB_SIZE - SIGNATURE_SIZE);

  _zipStream->seekg(-MIN_SHIFT, std::ios::end);

  if (this->SeekToSignature(detail::EndOfCentralDirectoryBlock::SignatureConstant, SeekDirection::Backward))
  {
    _endOfCentralDirectoryBlock.Deserialize(*_zipStream);
    return true;
  }

  return false;
}

bool ZipArchive::SeekToSignature(uint32_t signature, SeekDirection direction)
{
  std::streampos streamPosition = _zipStream->tellg();
  uint32_t buffer = 0;
  int appendix = static_cast<int>(direction == SeekDirection::Backward ? 0 - 1 : 1);

  while (!_zipStream->eof() && !_zipStream->fail())
  {
    deserialize(*_zipStream, buffer);

    if (buffer == signature)
    {
      _zipStream->seekg(streamPosition, std::ios::beg);
      return true;
    }

    streamPosition += appendix;
    _zipStream->seekg(streamPosition, std::ios::beg);
  }

  return false;
}

void ZipArchive::WriteToStream(std::ostream& stream)
{
  auto startPosition = stream.tellp();

  for (auto& entry : _entries)
  {
    entry->SerializeLocalFileHeader(stream);
  }

  auto offsetOfStartOfCDFH = stream.tellp() - startPosition;
  for (auto& entry : _entries)
  {
    entry->SerializeCentralDirectoryFileHeader(stream);
  }

  _endOfCentralDirectoryBlock.NumberOfThisDisk = 0;
  _endOfCentralDirectoryBlock.NumberOfTheDiskWithTheStartOfTheCentralDirectory = 0;

  _endOfCentralDirectoryBlock.NumberOfEntriesInTheCentralDirectory = static_cast<uint16_t>(_entries.size());
  _endOfCentralDirectoryBlock.NumberOfEntriesInTheCentralDirectoryOnThisDisk = static_cast<uint16_t>(_entries.size());

  _endOfCentralDirectoryBlock.SizeOfCentralDirectory = static_cast<uint32_t>(stream.tellp() - offsetOfStartOfCDFH);
  _endOfCentralDirectoryBlock.OffsetOfStartOfCentralDirectoryWithRespectToTheStartingDiskNumber = static_cast<uint32_t>(offsetOfStartOfCDFH);
  _endOfCentralDirectoryBlock.Serialize(stream);
}

void ZipArchive::Swap(ZipArchive::Ptr other)
{
  //if (this == other) return;
  if (other == nullptr) return;

  std::swap(_endOfCentralDirectoryBlock, other->_endOfCentralDirectoryBlock);
  std::swap(_entries, other->_entries);
  std::swap(_zipStream, other->_zipStream);
  std::swap(_owningStream, other->_owningStream);
}

void ZipArchive::InternalDestroy()
{
  if (_owningStream && _zipStream != nullptr) 
  {
    delete _zipStream;
    _zipStream = nullptr;
  }
}
