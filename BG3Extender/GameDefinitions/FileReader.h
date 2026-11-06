#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_SE()

enum class FileMappingType : uint32_t
{
    MetadataOnly,
    MemBuffer,
    MemoryMapped
};

enum class FileOpenFlags : uint32_t
{
    Default = 0,
    SkipPacked = 0x01,
    Overlapped = 0x02,
    NoBuffering = 0x04,
    SequentialScan = 0x08,
    RandomAccess = 0x10,
    SharedHandle = 0x20,
    SkipMerged = 0x40,
    SkipPacked2 = 0x80,
};

struct FileReader : public Noncopyable<FileReader>
{
    bool IsLoaded{ false };
    void* DataPtr{ nullptr };
    void* ReadPtr{ nullptr };
    uint64_t FileSize{ 0 };
    uint64_t FileHandle{ 0 };
    uint64_t FileHandle2{ 0 };
    ScratchBuffer Buffer;
    FileMappingType Type{ FileMappingType::MetadataOnly };
    void* FileObject{ nullptr };
};

class FileReaderPin
{
public:
    inline FileReaderPin(FileReader* reader)
        : reader_(reader)
    {}

    ~FileReaderPin();

    FileReaderPin(FileReaderPin const&) = delete;
    FileReaderPin& operator =(FileReaderPin const&) = delete;
    FileReaderPin& operator =(FileReaderPin&&) = delete;

    inline FileReaderPin(FileReaderPin&& other) noexcept
    {
        reader_ = other.reader_;
        if (&other != this) {
            other.reader_ = nullptr;
        }
    }

    bool IsLoaded() const
    {
        return reader_ != nullptr && reader_->IsLoaded;
    }

    void* Buf() const
    {
        if (IsLoaded()) {
            return reader_->DataPtr;
        } else {
            return nullptr;
        }
    }

    std::size_t Size() const
    {
        if (IsLoaded()) {
            return reader_->FileSize;
        } else {
            return 0;
        }
    }

    STDString ToString() const;

private:
    FileReader* reader_;
};

END_SE()