BEGIN_SE()

class Console;

struct CoreLibPlatformInterface
{
	using AllocProc = void* (std::size_t size);
	using FreeProc = void(void* ptr);

	AllocProc* Alloc{ nullptr };
	FreeProc* Free{ nullptr };

	FixedString::CreateFromStringProc* ls__FixedString__CreateFromString{ nullptr };
	FixedString::GetStringProc* ls__FixedString__GetString{ nullptr };
	FixedString::IncRefProc* ls__FixedString__IncRef{ nullptr };
	FixedString::DecRefProc* ls__FixedString__DecRef{ nullptr };
	GlobalStringTable** ls__gGlobalStringTable{ nullptr };

	void* StaticSymbols{ NULL };
	HMODULE ThisModule{ NULL };
	Console* GlobalConsole{ NULL };
};

extern CoreLibPlatformInterface gCoreLibPlatformInterface;

END_SE()
