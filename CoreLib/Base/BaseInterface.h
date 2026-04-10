BEGIN_SE()

class Console;

struct CoreLibPlatformInterface
{
    using AllocProc = _Post_writable_byte_size_(size) void* (std::size_t size);
    using FreeProc = void (_Post_ptr_invalid_ _Pre_valid_ void* ptr) noexcept;

    AllocProc* Alloc{ nullptr };
    FreeProc* Free{ nullptr };

    FixedString::CreateFromStringProc* ls__FixedString__CreateFromString{ nullptr };
    FixedString::GetStringProc* ls__FixedString__GetString{ nullptr };
    FixedString::IncRefProc* ls__FixedString__IncRef{ nullptr };
    FixedString::DecRefProc* ls__FixedString__DecRef{ nullptr };
    GlobalStringTable** ls__gGlobalStringTable{ nullptr };
    GlobalStringTable::MainTable::CreateFromStringProc* ls__GlobalStringTable__MainTable__CreateFromString{ nullptr };
    GlobalStringTable::MainTable::DecRefProc* ls__GlobalStringTable__MainTable__DecRef{ nullptr };

    void* StaticSymbols{ nullptr };
    HMODULE ThisModule{ NULL };
    Console* GlobalConsole{ nullptr };
    bool EnableDebugBreak{ true };
};

extern CoreLibPlatformInterface gCoreLibPlatformInterface;

END_SE()
