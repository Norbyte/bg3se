#include "ZipArchiveEntry.h"
#include "ZipArchive.h"

#include "detail/ZipLocalFileHeader.h"

#include "methods/ZipMethodResolver.h"

#include "streams/zip_cryptostream.h"
#include "streams/compression_encoder_stream.h"
#include "streams/compression_decoder_stream.h"
#include "streams/nullstream.h"

#include "utils/stream_utils.h"
#include "utils/time_utils.h"

#include <iostream>
#include <cassert>
#include <algorithm>
#include <memory>
#include <sstream>

namespace
{
  bool IsValidFilename(const std::string& fullPath)
  {
    // this function ensures, that the filename will have non-zero
    // length, when the filename will be normalized

    if (fullPath.length() > 0)
    {
      std::string tmpFilename = fullPath;
      std::replace(tmpFilename.begin(), tmpFilename.end(), '\\', '/');

      // if the filename is built only from '/', then it is invalid path
      return tmpFilename.find_first_not_of('/') != std::string::npos;
    }

    return false;
  }

  std::string GetFilenameFromPath(const std::string& fullPath)
  {
    std::string::size_type dirSeparatorPos;

    if ((dirSeparatorPos = fullPath.find_last_of('/')) != std::string::npos)
    {
      return fullPath.substr(dirSeparatorPos + 1);
    }
    else
    {
      return fullPath;
    }
  }

  bool IsDirectoryPath(const std::string& fullPath)
  {
    return (fullPath.length() > 0 && fullPath.back() == '/');
  }
}

ZipArchiveEntry::ZipArchiveEntry()
  : _archive(nullptr)
  , _archiveStream(nullptr)
  , _compressionStream(nullptr)
  , _encryptionStream(nullptr)
  , _rawStream(nullptr)

  , _originallyInArchive(false)
  , _isNewOrChanged(false)
  , _hasLocalFileHeader(false)

  , _offsetOfCompressedData(-1)
  , _offsetOfSerializedLocalFileHeader(-1)

  , _inputStream(nullptr)
{

}

ZipArchiveEntry::~ZipArchiveEntry()
{
  this->CloseRawStream();
  this->CloseDecompressionStream();
}

ZipArchiveEntry::Ptr ZipArchiveEntry::CreateNew(ZipArchive* zipArchive, const std::string& fullPath)
{
  ZipArchiveEntry::Ptr result;

  assert(zipArchive != nullptr);

  if (IsValidFilename(fullPath))
  {
    result.reset(new ZipArchiveEntry());

    result->_archive = zipArchive;
    result->_isNewOrChanged = true;
    result->SetAttributes(Attributes::Archive);
    result->SetVersionToExtract(VERSION_NEEDED_DEFAULT);
    result->SetVersionMadeBy(VERSION_MADEBY_DEFAULT);
    result->SetLastWriteTime(time(nullptr));
  
    result->SetFullName(fullPath);

    result->SetCompressionMethod(StoreMethod::CompressionMethod);
    result->SetGeneralPurposeBitFlag(BitFlag::None);
  }

  return result;
}

ZipArchiveEntry::Ptr ZipArchiveEntry::CreateExisting(ZipArchive* zipArchive, detail::ZipCentralDirectoryFileHeader& cd)
{
  ZipArchiveEntry::Ptr result;

  assert(zipArchive != nullptr);

  if (IsValidFilename(cd.Filename))
  {
    result.reset(new ZipArchiveEntry());

    result->_archive                    = zipArchive;
    result->_centralDirectoryFileHeader = cd;
    result->_originallyInArchive        = true;
    result->CheckFilenameCorrection();

    // determining folder by path has more priority
    // than attributes. however, if attributes
    // does not correspond with path, they will be fixed.
    result->SetAttributes(IsDirectoryPath(result->GetFullName())
                          ? Attributes::Directory
                          : Attributes::Archive);
  }

  return result;
}

//////////////////////////////////////////////////////////////////////////
// public methods & getters & setters

const std::string& ZipArchiveEntry::GetFullName() const
{
  return _centralDirectoryFileHeader.Filename;
}

void ZipArchiveEntry::SetFullName(const std::string& fullName)
{
  std::string filename = fullName;
  std::string correctFilename;

  // unify slashes
  std::replace(filename.begin(), filename.end(), '\\', '/');

  bool isDirectory = IsDirectoryPath(filename);

  // if slash is first char, remove it
  if (filename[0] == '/')
  {
    filename = filename.substr(filename.find_first_not_of('/'));
  }

  // find multiply slashes
  bool prevWasSlash = false;
  for (std::string::size_type i = 0; i < filename.length(); ++i)
  {
    if (filename[i] == '/' && prevWasSlash) continue;
    prevWasSlash = (filename[i] == '/');

    correctFilename += filename[i];
  }

  _centralDirectoryFileHeader.Filename = correctFilename;
  _name = GetFilenameFromPath(correctFilename);

  this->SetAttributes(isDirectory ? Attributes::Directory : Attributes::Archive);
}

const std::string& ZipArchiveEntry::GetName() const
{
  return _name;
}

void ZipArchiveEntry::SetName(const std::string& name)
{
  std::string folder;
  std::string::size_type dirDelimiterPos;

  // search for '/' in path name.
  // if this entry is directory, search up to one character
  // before the last one (which is '/')
  // if this entry is file, just search until last '/'
  // will be found
  dirDelimiterPos = this->GetFullName().find_last_of('/', 
    (uint32_t)this->GetAttributes() & (uint32_t)Attributes::Archive
    ? std::string::npos
    : this->GetFullName().length() - 1);
      
  if (dirDelimiterPos != std::string::npos)
  {
    folder = this->GetFullName().substr(0, dirDelimiterPos);
  }

  this->SetFullName(folder + name);

  if (this->IsDirectory())
  {
    this->SetFullName(this->GetFullName() + '/');
  }
}

const std::string& ZipArchiveEntry::GetComment() const
{
  return _centralDirectoryFileHeader.FileComment;
}

void ZipArchiveEntry::SetComment(const std::string& comment)
{
  _centralDirectoryFileHeader.FileComment = comment;
}

time_t ZipArchiveEntry::GetLastWriteTime() const
{
  return utils::time::datetime_to_timestamp(_centralDirectoryFileHeader.LastModificationDate, _centralDirectoryFileHeader.LastModificationTime);
}

void ZipArchiveEntry::SetLastWriteTime(time_t modTime)
{
  utils::time::timestamp_to_datetime(modTime, _centralDirectoryFileHeader.LastModificationDate, _centralDirectoryFileHeader.LastModificationTime);
}

ZipArchiveEntry::Attributes ZipArchiveEntry::GetAttributes() const
{
  return static_cast<Attributes>(_centralDirectoryFileHeader.ExternalFileAttributes);
}

uint16_t ZipArchiveEntry::GetCompressionMethod() const
{
  return _centralDirectoryFileHeader.CompressionMethod;
}

void ZipArchiveEntry::SetAttributes(Attributes value)
{
  Attributes prevVal = this->GetAttributes();
  Attributes newVal = prevVal | value;

  // if we're changing from directory to file
  if (!!(prevVal & Attributes::Directory) && !!(newVal & Attributes::Archive))
  {
    newVal &= ~Attributes::Directory;

    if (IsDirectoryPath(_centralDirectoryFileHeader.Filename))
    {
      _centralDirectoryFileHeader.Filename.pop_back();
    }
  }

  // if we're changing from file to directory
  else if (!!(prevVal & Attributes::Archive) && !!(newVal & Attributes::Directory))
  {
    newVal &= ~Attributes::Archive;

    if (!IsDirectoryPath(_centralDirectoryFileHeader.Filename))
    {
      _centralDirectoryFileHeader.Filename += '/';
    }
  }

  // if this entry is directory, ensure that crc32 & sizes
  // are set to 0 and do not include any stream
  if (!!(newVal & Attributes::Directory))
  {
    _centralDirectoryFileHeader.Crc32 = 0;
    _centralDirectoryFileHeader.CompressedSize = 0;
    _centralDirectoryFileHeader.UncompressedSize = 0;
  }

  _centralDirectoryFileHeader.ExternalFileAttributes = static_cast<uint32_t>(newVal);
}

bool ZipArchiveEntry::IsPasswordProtected() const
{
  return !!(this->GetGeneralPurposeBitFlag() & BitFlag::Encrypted);
}

const std::string& ZipArchiveEntry::GetPassword() const
{
  return _password;
}

void ZipArchiveEntry::SetPassword(const std::string& password)
{
  _password = password;

  // allow unset password only for empty files
  if (!_originallyInArchive || (_hasLocalFileHeader && this->GetSize() == 0))
  {
    this->SetGeneralPurposeBitFlag(BitFlag::Encrypted, !_password.empty());
  }
}

uint32_t ZipArchiveEntry::GetCrc32() const
{
  return _centralDirectoryFileHeader.Crc32;
}

size_t ZipArchiveEntry::GetSize() const
{
  return static_cast<size_t>(_centralDirectoryFileHeader.UncompressedSize);
}

size_t ZipArchiveEntry::GetCompressedSize() const
{
  return static_cast<size_t>(_centralDirectoryFileHeader.CompressedSize);
}


bool ZipArchiveEntry::CanExtract() const
{
  return (this->GetVersionToExtract() <= VERSION_MADEBY_DEFAULT);
}

bool ZipArchiveEntry::IsDirectory() const
{
  return !!(this->GetAttributes() & Attributes::Directory);
}

bool ZipArchiveEntry::IsUsingDataDescriptor() const
{
  return !!(this->GetGeneralPurposeBitFlag() & BitFlag::DataDescriptor);
}

void ZipArchiveEntry::UseDataDescriptor(bool use)
{
  this->SetGeneralPurposeBitFlag(BitFlag::DataDescriptor, use);
}

std::istream* ZipArchiveEntry::GetRawStream()
{
  if (_rawStream == nullptr)
  {
    if (_originallyInArchive)
    {
      auto offsetOfCompressedData = this->SeekToCompressedData();
      _rawStream = std::make_shared<isubstream>(*_archive->_zipStream, offsetOfCompressedData, this->GetCompressedSize());
    }
    else
    {
      _rawStream = std::make_shared<isubstream>(*_immediateBuffer);
    }
  }

  return _rawStream.get();
}

std::istream* ZipArchiveEntry::GetDecompressionStream()
{
  std::shared_ptr<std::istream> intermediateStream;

  // there shouldn't be opened another stream
  if (this->CanExtract() && _archiveStream == nullptr && _encryptionStream == nullptr)
  {
    auto offsetOfCompressedData = this->SeekToCompressedData();
    bool needsPassword = !!(this->GetGeneralPurposeBitFlag() & BitFlag::Encrypted);
    bool needsDecompress = this->GetCompressionMethod() != StoreMethod::CompressionMethod;

    if (needsPassword && _password.empty())
    {
      // we need password, but we does not have it
      return nullptr;
    }

    // make correctly-ended substream of the input stream
    intermediateStream = _archiveStream = std::make_shared<isubstream>(*_archive->_zipStream, offsetOfCompressedData, this->GetCompressedSize());

    if (needsPassword)
    {
      std::shared_ptr<zip_cryptostream> cryptoStream = std::make_shared<zip_cryptostream>(*intermediateStream, _password.c_str());
      cryptoStream->set_final_byte(this->GetLastByteOfEncryptionHeader());
      bool hasCorrectPassword = cryptoStream->prepare_for_decryption();

      // set it here, because in case the hasCorrectPassword is false
      // the method CloseDecompressionStream() will properly delete the stream
      intermediateStream = _encryptionStream = cryptoStream;

      if (!hasCorrectPassword)
      {
        this->CloseDecompressionStream();
        return nullptr;
      }
    }

    if (needsDecompress)
    {
      ICompressionMethod::Ptr zipMethod = ZipMethodResolver::GetZipMethodInstance(this->GetCompressionMethod());

      if (zipMethod != nullptr)
      {
        intermediateStream = _compressionStream = std::make_shared<compression_decoder_stream>(zipMethod->GetDecoder(), zipMethod->GetDecoderProperties(), *intermediateStream);
      }
    }
  }

  return intermediateStream.get();
}

bool ZipArchiveEntry::IsRawStreamOpened() const
{
  return _rawStream != nullptr;
}

bool ZipArchiveEntry::IsDecompressionStreamOpened() const
{
  return _compressionStream != nullptr;
}

void ZipArchiveEntry::CloseRawStream()
{
  _rawStream.reset();
}

void ZipArchiveEntry::CloseDecompressionStream()
{
  _compressionStream.reset();
  _encryptionStream.reset();
  _archiveStream.reset();
  _immediateBuffer.reset();
}

bool ZipArchiveEntry::SetCompressionStream(std::istream& stream, ICompressionMethod::Ptr method /* = DeflateMethod::Create() */, CompressionMode mode /* = CompressionMode::Deferred */)
{
  // if _inputStream is set, we already have some stream to compress
  // so we discard it
  if (_inputStream != nullptr)
  {
    this->UnloadCompressionData();
  }

  _isNewOrChanged = true;

  _inputStream = &stream;
  _compressionMethod = method;
  _compressionMode = mode;
  this->SetCompressionMethod(method->GetZipMethodDescriptor().GetCompressionMethod());

  if (_inputStream != nullptr && _compressionMode == CompressionMode::Immediate)
  {
    _immediateBuffer = std::make_shared<std::stringstream>();
    this->InternalCompressStream(*_inputStream, *_immediateBuffer);

    // we have everything we need, let's act like we were loaded from archive :)
    _isNewOrChanged = false;
    _inputStream = nullptr;
  }

  return true;
}

void ZipArchiveEntry::UnsetCompressionStream()
{
  if (!this->HasCompressionStream())
  {
    this->FetchLocalFileHeader();
  }

  this->UnloadCompressionData();
  this->SetPassword(std::string());
}

void ZipArchiveEntry::Remove()
{
  auto it = std::find(_archive->_entries.begin(), _archive->_entries.end(), this->shared_from_this());

  if (it != _archive->_entries.end())
  {
    _archive->_entries.erase(it);
    delete this;
  }
}

//////////////////////////////////////////////////////////////////////////
// private getters & setters

void ZipArchiveEntry::SetCompressionMethod(uint16_t value)
{
  _centralDirectoryFileHeader.CompressionMethod = value;
}

ZipArchiveEntry::BitFlag ZipArchiveEntry::GetGeneralPurposeBitFlag() const
{
  return static_cast<BitFlag>(_centralDirectoryFileHeader.GeneralPurposeBitFlag);
}

void ZipArchiveEntry::SetGeneralPurposeBitFlag(BitFlag value, bool set)
{
  if (set)
  {
    _centralDirectoryFileHeader.GeneralPurposeBitFlag |= static_cast<uint16_t>(value);
  }
  else
  {
    _centralDirectoryFileHeader.GeneralPurposeBitFlag &= static_cast<uint16_t>(~value);
  }
}

uint16_t ZipArchiveEntry::GetVersionToExtract() const
{
  return _centralDirectoryFileHeader.VersionNeededToExtract;
}

void ZipArchiveEntry::SetVersionToExtract(uint16_t value)
{
  _centralDirectoryFileHeader.VersionNeededToExtract = value;
}

uint16_t ZipArchiveEntry::GetVersionMadeBy() const
{
  return _centralDirectoryFileHeader.VersionMadeBy;
}

void ZipArchiveEntry::SetVersionMadeBy(uint16_t value)
{
  _centralDirectoryFileHeader.VersionMadeBy = value;
}

int32_t ZipArchiveEntry::GetOffsetOfLocalHeader() const
{
  return _centralDirectoryFileHeader.RelativeOffsetOfLocalHeader;
}

void ZipArchiveEntry::SetOffsetOfLocalHeader(int32_t value)
{
  _centralDirectoryFileHeader.RelativeOffsetOfLocalHeader = static_cast<int32_t>(value);
}

bool ZipArchiveEntry::HasCompressionStream() const
{
  return _inputStream != nullptr;
}

//////////////////////////////////////////////////////////////////////////
// private working methods

void ZipArchiveEntry::FetchLocalFileHeader()
{
  if (!_hasLocalFileHeader && _originallyInArchive && _archive != nullptr)
  {
    _archive->_zipStream->seekg(this->GetOffsetOfLocalHeader(), std::ios::beg);
    _localFileHeader.Deserialize(*_archive->_zipStream);

    _offsetOfCompressedData = _archive->_zipStream->tellg();
  }

  // sync data
  this->SyncLFH_with_CDFH();
  _hasLocalFileHeader = true;
}

void ZipArchiveEntry::CheckFilenameCorrection()
{
  // this forces recheck of the filename.
  // this is useful when the check is needed after
  // deserialization
  this->SetFullName(this->GetFullName());
}

void ZipArchiveEntry::FixVersionToExtractAtLeast(uint16_t value)
{
  if (this->GetVersionToExtract() < value)
  {
    this->SetVersionToExtract(value);
  }
}

void ZipArchiveEntry::SyncLFH_with_CDFH()
{
  _localFileHeader.SyncWithCentralDirectoryFileHeader(_centralDirectoryFileHeader);
}

void ZipArchiveEntry::SyncCDFH_with_LFH()
{
  _centralDirectoryFileHeader.SyncWithLocalFileHeader(_localFileHeader);

  this->FixVersionToExtractAtLeast(this->IsDirectory()
    ? VERSION_NEEDED_EXPLICIT_DIRECTORY
    : _compressionMethod->GetZipMethodDescriptor().GetVersionNeededToExtract());
}

std::ios::pos_type ZipArchiveEntry::GetOffsetOfCompressedData()
{
  if (!_hasLocalFileHeader)
  {
    this->FetchLocalFileHeader();
  }

  return _offsetOfCompressedData;
}

std::ios::pos_type ZipArchiveEntry::SeekToCompressedData()
{
  // check for fail bit?
  _archive->_zipStream->seekg(this->GetOffsetOfCompressedData(), std::ios::beg);
  return this->GetOffsetOfCompressedData();
}

void ZipArchiveEntry::SerializeLocalFileHeader(std::ostream& stream)
{
  // ensure opening the stream
  std::istream* compressedDataStream = nullptr;

  if (!this->IsDirectory())
  {
    if (_inputStream == nullptr)
    {
      if (!_isNewOrChanged)
      {
        // the file was either compressed in immediate mode,
        // or was in previous archive
        compressedDataStream = this->GetRawStream();
      }

      // if file is new and empty or stream has been set to nullptr,
      // just do not set any compressed data stream
    }
    else
    {
      assert(_isNewOrChanged);
      compressedDataStream = _inputStream;
    }
  }

  if (!_hasLocalFileHeader)
  {
    this->FetchLocalFileHeader();
  }

  // save offset of stream here
  _offsetOfSerializedLocalFileHeader = stream.tellp();      

  if (this->IsUsingDataDescriptor())
  {
    _localFileHeader.CompressedSize = 0;
    _localFileHeader.UncompressedSize = 0;
    _localFileHeader.Crc32 = 0;
  }

  _localFileHeader.Serialize(stream);

  // if this entry is a directory, it should not contain any data
  // nor crc.
  assert(
    this->IsDirectory()
    ? !GetCrc32() && !GetSize() && !GetCompressedSize() && !_inputStream
    : true
  );

  if (!this->IsDirectory() && compressedDataStream != nullptr)
  {
    if (_isNewOrChanged)
    {
      this->InternalCompressStream(*compressedDataStream, stream);

      if (this->IsUsingDataDescriptor())
      {
        _localFileHeader.SerializeAsDataDescriptor(stream);
      }
      else
      {
        // actualize local file header
        // make non-seekable version?
        stream.seekp(_offsetOfSerializedLocalFileHeader);
        _localFileHeader.Serialize(stream);
        stream.seekp(this->GetCompressedSize(), std::ios::cur);
      }
    }
    else
    {
      utils::stream::copy(*compressedDataStream, stream);
    }
  }
}

void ZipArchiveEntry::SerializeCentralDirectoryFileHeader(std::ostream& stream)
{
  _centralDirectoryFileHeader.RelativeOffsetOfLocalHeader = static_cast<int32_t>(_offsetOfSerializedLocalFileHeader);
  _centralDirectoryFileHeader.Serialize(stream);
}

void ZipArchiveEntry::UnloadCompressionData()
{
  // unload stream
  _immediateBuffer->clear();
  _inputStream = nullptr;

  _centralDirectoryFileHeader.CompressedSize = 0;
  _centralDirectoryFileHeader.UncompressedSize = 0;
  _centralDirectoryFileHeader.Crc32 = 0;
}

void ZipArchiveEntry::InternalCompressStream(std::istream& inputStream, std::ostream& outputStream)
{
  std::ostream* intermediateStream = &outputStream;

  std::unique_ptr<zip_cryptostream> cryptoStream;
  if (!_password.empty())
  {
    this->SetGeneralPurposeBitFlag(BitFlag::Encrypted);

    // std::make_unique<zip_cryptostream>();
    cryptoStream = std::unique_ptr<zip_cryptostream>(new zip_cryptostream());

    cryptoStream->init(outputStream, _password.c_str());
    cryptoStream->set_final_byte(this->GetLastByteOfEncryptionHeader());
    intermediateStream = cryptoStream.get();
  }

  crc32stream crc32Stream;
  crc32Stream.init(inputStream);

  compression_encoder_stream compressionStream(
    _compressionMethod->GetEncoder(),
    _compressionMethod->GetEncoderProperties(),
    *intermediateStream);
  intermediateStream = &compressionStream;
  utils::stream::copy(crc32Stream, *intermediateStream);

  intermediateStream->flush();

  _localFileHeader.UncompressedSize = static_cast<uint32_t>(compressionStream.get_bytes_read());
  _localFileHeader.CompressedSize   = static_cast<uint32_t>(compressionStream.get_bytes_written() + (!_password.empty() ? 12 : 0));
  _localFileHeader.Crc32 = crc32Stream.get_crc32();

  this->SyncCDFH_with_LFH();
}

void ZipArchiveEntry::FigureCrc32()
{
  if (this->IsDirectory() || _inputStream == nullptr || !_isNewOrChanged)
  {
    return;
  }

  // stream must be seekable
  auto position = _inputStream->tellg();

  // compute crc32
  crc32stream crc32Stream;
  crc32Stream.init(*_inputStream);

  // just force to read all from crc32stream
  nullstream nulldev;
  utils::stream::copy(crc32Stream, nulldev);

  // seek back
  _inputStream->clear();
  _inputStream->seekg(position);

  _centralDirectoryFileHeader.Crc32 = crc32Stream.get_crc32();
}

uint8_t ZipArchiveEntry::GetLastByteOfEncryptionHeader()
{
  if (!!(this->GetGeneralPurposeBitFlag() & BitFlag::DataDescriptor))
  {
    // In the case that bit 3 of the general purpose bit flag is set to
    // indicate the presence of a 'data descriptor' (signature
    // 0x08074b50), the last byte of the decrypted header is sometimes
    // compared with the high-order byte of the lastmodified time,
    // rather than the high-order byte of the CRC, to verify the
    // password.
    //
    // This is not documented in the PKWare Appnote.txt.
    // This was discovered this by analysis of the Crypt.c source file in the
    // InfoZip library
    // http://www.info-zip.org/pub/infozip/

    // Also, winzip insists on this!
    return uint8_t((_centralDirectoryFileHeader.LastModificationTime >> 8) & 0xff);
  }
  else
  {
    // When bit 3 is not set, the CRC value is required before
    // encryption of the file data begins. In this case there is no way
    // around it: must read the stream in its entirety to compute the
    // actual CRC before proceeding.
    this->FigureCrc32();
    return uint8_t((this->GetCrc32() >> 24) & 0xff);
  }
}
