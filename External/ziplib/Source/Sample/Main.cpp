#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "../ZipLib/ZipFile.h"
#include "../ZipLib/streams/memstream.h"

#include "../ZipLib/methods/Bzip2Method.h"

#include <fstream>

struct PrintMethodName
{
  PrintMethodName(const char* methodName) { printf("[*] == %s ==\n", methodName); }
  ~PrintMethodName() { printf("\n\n"); }
};

#define TEST_METHOD(methodName) void methodName(PrintMethodName _ = #methodName) 

static const char* zipFilename  = "archive.zip";
static const char* zipFilename2 = "other_archive.zip";

static const char* fileIn1     = "in1.jpg";
static const char* fileIn2     = "in2.png";
static const char* fileIn2Dest = "dir/in2.png";
static const char* fileIn3     = "in3.txt";

static const char* fileOut1    = "out1.jpg";
static const char* fileOut2    = "out2.png";
static const char* fileOut3    = "out3.txt";

void ListZipArchive(const char* zipArchiveName = zipFilename)
{
  ZipArchive::Ptr archive = ZipFile::Open(zipArchiveName);
  size_t entries = archive->GetEntriesCount();

  printf("[o] Listing archive (comment: '%s'):\n", archive->GetComment().c_str());
  printf("[o] Entries count: %lu\n", entries);

  for (size_t i = 0; i < entries; ++i)
  {
    auto entry = archive->GetEntry(int(i));

    printf("[o] -- %s\n", entry->GetFullName().c_str());
    printf("[o]   >> uncompressed size: %lu\n", entry->GetSize());
    printf("[o]   >> compressed size: %lu\n", entry->GetCompressedSize());
    printf("[o]   >> password protected: %s\n", entry->IsPasswordProtected() ? "yes" : "no");
    printf("[o]   >> compression method: %s\n", entry->GetCompressionMethod() == DeflateMethod::CompressionMethod ? "DEFLATE" : "stored");
    printf("[o]   >> comment: %s\n", entry->GetComment().c_str());
    printf("[o]   >> crc32: 0x%08X\n", entry->GetCrc32());
  }

  printf("\n");
}

TEST_METHOD(Sample_ZipFile)
{
  try
  {
    printf("[+] Compressing '%s'\n", fileIn1);
    ZipFile::AddFile(zipFilename, fileIn1, LzmaMethod::Create());

    ListZipArchive();

    printf("[+] Compressing & encrypting '%s' as '%s'\n", fileIn2, fileIn2Dest);
    ZipFile::AddEncryptedFile(zipFilename, fileIn2, fileIn2Dest, "pass");

    ListZipArchive();

    printf("[+] Extracting '%s' as '%s'\n", fileIn1, fileOut1);
    ZipFile::ExtractFile(zipFilename, fileIn1, fileOut1);

    printf("[+] Extracting & decrypting '%s' as '%s'\n", fileIn2Dest, fileOut2);
    ZipFile::ExtractEncryptedFile(zipFilename, fileIn2Dest, fileOut2, "pass");

    printf("[+] Removing file '%s'\n", fileIn2Dest);
    ZipFile::RemoveEntry(zipFilename, fileIn2Dest);

    ListZipArchive();
  }
  catch (std::exception& e)
  {
    printf("[-] An exception occured: '%s'\n", e.what());
  }
}

TEST_METHOD(Sample_ZipArchive_Stream_Deferred_Comment)
{
  ZipArchive::Ptr archive = ZipFile::Open(zipFilename);
  archive->SetComment("archive comment");

  char content[] = "Content to add";
  imemstream contentStream(content);

  ZipArchiveEntry::Ptr entry = archive->CreateEntry(fileIn3);
  assert(entry != nullptr);

  entry->SetPassword("pass");

  // if this is not set, the input stream would be readen twice
  // this method is only useful for password protected files
  entry->UseDataDescriptor();

  Bzip2Method::Ptr ctx = Bzip2Method::Create();
  ctx->SetBlockSize(Bzip2Method::BlockSize::B600);

  entry->SetCompressionStream(
    contentStream,
    ctx,
    ZipArchiveEntry::CompressionMode::Deferred
  );

  entry->SetComment("entry comment");

  // data from contentStream are pumped here
  ZipFile::SaveAndClose(archive, zipFilename);

  ListZipArchive();
}

TEST_METHOD(Sample_ZipArchive_Stream_Immediate_Store_Own_Save_Password_Protected)
{
  ZipArchive::Ptr archive = ZipFile::Open(zipFilename);

  ZipArchiveEntry::Ptr entry = archive->CreateEntry(fileIn2Dest);
  assert(entry != nullptr);

  {
    std::ifstream contentStream;
    contentStream.open(fileIn2, std::ios::binary);

    assert(contentStream.is_open());

    DeflateMethod::Ptr ctx = DeflateMethod::Create();
    ctx->SetCompressionLevel(DeflateMethod::CompressionLevel::L1);

    entry->SetCompressionStream(   // data from contentStream are pumped here
      contentStream,
      ctx,
      ZipArchiveEntry::CompressionMode::Immediate
    );

    // ifs stream is destroyed here
  }

  std::ofstream archiveToSave;
  archiveToSave.open(zipFilename2, std::ios::binary);
  archive->WriteToStream(archiveToSave);

  archiveToSave.flush();
  archiveToSave.close();

  ListZipArchive(zipFilename2);
}

TEST_METHOD(Sample_ZipArchive_Decompress_Password_Protected)
{
  ZipArchive::Ptr archive = ZipFile::Open(zipFilename);

  std::istream* decompressStream = nullptr;

  printf("[+] Extracting file '%s'\n", fileIn3);

  ZipArchiveEntry::Ptr entry = archive->GetEntry(fileIn3);
  assert(entry != nullptr);

  printf("[+] Trying no pass...\n");
  decompressStream = entry->GetDecompressionStream();
  assert(decompressStream == nullptr);
  
  printf("[+] Trying wrong pass...\n");
  entry->SetPassword("wrongpass");
  decompressStream = entry->GetDecompressionStream();
  assert(decompressStream == nullptr);

  printf("[+] Trying correct password...\n");
  entry->SetPassword("pass");
  decompressStream = entry->GetDecompressionStream();
  assert(decompressStream != nullptr);

  std::string line;
  std::getline(*decompressStream, line);

  printf("[+] Content of a file: '%s'\n", line.c_str());
}

TEST_METHOD(Sample_EXAMPLE)
{
  ZipArchive::Ptr archive = ZipFile::Open(zipFilename);

  std::ifstream contentStream;
  contentStream.open(fileIn3, std::ios::binary);

  ZipArchiveEntry::Ptr entry = archive->CreateEntry(fileIn3);
  assert(entry != nullptr);

  entry->SetPassword("pass");

  // if this is not set, the input stream would be readen twice
  // this method is only useful for password protected files
  entry->UseDataDescriptor();

  StoreMethod::Ptr ctx = StoreMethod::Create();
  entry->SetCompressionStream(
    contentStream,
    ctx,
    ZipArchiveEntry::CompressionMode::Deferred
  );

  // data from contentStream are pumped here
  ZipFile::SaveAndClose(archive, zipFilename);

  ListZipArchive();
}

int main()
{
#ifdef _MSC_VER
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  // delete archive before this sample
  remove(zipFilename);
  remove(zipFilename2);
  remove(fileOut1);
  remove(fileOut2);
  remove(fileOut3);

  Sample_ZipFile();
  Sample_ZipArchive_Stream_Deferred_Comment();
  Sample_ZipArchive_Stream_Immediate_Store_Own_Save_Password_Protected();
  Sample_ZipArchive_Decompress_Password_Protected();
  //Sample_EXAMPLE();
  return 0;
}
