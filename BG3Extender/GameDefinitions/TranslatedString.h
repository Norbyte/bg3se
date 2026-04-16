#pragma once

BEGIN_SE()

struct RuntimeStringHandle
{
    FixedString Handle;
    uint16_t Version{ 0 };

    RuntimeStringHandle();

    inline RuntimeStringHandle(FixedString const& handle)
        : Handle(handle),
        Version(1)
    {}

    inline RuntimeStringHandle(FixedString const& handle, uint16_t version)
        : Handle(handle),
        Version(version)
    {}

    inline bool operator == (RuntimeStringHandle const& fs) const
    {
        return Handle == fs.Handle;
    }

    inline bool operator != (RuntimeStringHandle const& fs) const
    {
        return Handle != fs.Handle;
    }
};

template <>
inline uint64_t HashMapHash<RuntimeStringHandle>(RuntimeStringHandle const& v)
{
    return HashMapHash(v.Handle);
}

struct TranslatedString
{
    RuntimeStringHandle Handle;
    RuntimeStringHandle ArgumentString;

    static TranslatedString FromString(StringView const& sv);
    std::optional<StringView> Get() const;
};

struct TranslatedFSString
{
    RuntimeStringHandle Handle;
    struct TranslatedFSArgumentString* ArgumentString{ nullptr };

    std::optional<StringView> Get() const;
};

struct TranslatedFSArgumentString : private ProtectedGameObject<TranslatedFSArgumentString>
{
    struct Argument
    {
        uint32_t Type;
        TranslatedFSString TranslatedValue;
        STDString StringValue;
    };
    
    struct ArgumentPosition
    {
        uint32_t Start;
        uint32_t End;
    };

    LegacyRefMap<FixedString, Argument>* Arguments;
    RuntimeStringHandle String;
    char* Buffer;
    STDString ParsedString;
    uint32_t BufferLength;
    bool NeedsParsing;
    LegacyRefMap<FixedString, ArgumentPosition> ArgPositions;
};

template <class Pred>
inline void SpinWait(Pred pred)
{
    while (!pred()) {
        unsigned spinCount = 0;
        while (!pred()) {
            _mm_pause();
            if (spinCount++ > 400) {
                Sleep(0);
                break;
            }
        }
    }
}

class SRWSpinLock
{
public:
    void ReadLock();
    void ReadUnlock();
    void WriteLock();
    void WriteUnlock();

private:
    std::atomic<uint32_t> FastLock;
    DWORD OwningThreadId;
    uint32_t WriteEnterCount;

    void WriteWait();
    void ReadWait();
};

struct TranslatedArgumentStringBuffer
{
    char* ArgumentsBuffer2;
    bool NeedsFormatting;
    char* ArgumentBuffer;
    STDString Formatted;
    uint32_t ArgumentsLength;
    uint32_t _Pad;
    RuntimeStringHandle Handle;
};


struct TranslatedStringRepository : public ProtectedGameObject<TranslatedStringRepository>
{
    struct TextEntry
    {
        RuntimeStringHandle* Handle;
        LSStringView* Text;
        __int64 field_10;
        __int64 field_18;
    };

    struct TextPool
    {
        Array<STDString*> Strings;
        HashMap<RuntimeStringHandle, LSStringView> Texts;
    };

    int field_0;
    TextPool* TranslatedStrings[9];
    TextPool* FallbackPool;
    TextPool* VersionedFallbackPool;
    Array<void*> field_60;
    HashMap<FixedString, TranslatedArgumentStringBuffer> ArgumentStrings;
    HashMap<FixedString, RuntimeStringHandle> TextToStringKey;
    SRWSpinLock Lock;
    bool IsLoaded;

    std::optional<StringView> GetTranslatedString(RuntimeStringHandle const& handle);
    void UpdateTranslatedString(RuntimeStringHandle const& handle, StringView translated);
};

END_SE()
