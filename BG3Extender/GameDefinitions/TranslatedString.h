#pragma once

BEGIN_SE()

struct RuntimeStringHandle
{
	FixedString Handle;
	uint16_t Version{ 0 };

	inline RuntimeStringHandle() {}

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

class ThreadedFastLock
{
public:
	inline void Enter()
	{
		auto tid = GetCurrentThreadId();
		if (CurrentThreadId == 0xffffffffu || CurrentThreadId != tid) {
			for (;;) {
				SpinWait([&]() { return (FastLock & 0xfff00000u) == 0; });

				if ((FastLock.fetch_add(0x100000u) & 0xfff00000u) == 0) {
					break;
				}

				FastLock -= 0x100000u;
			}

			SpinWait([&]() { return (FastLock & 0x000fffffu) == 0; });
			CurrentThreadId = tid;
		}

		++EnterCount;
	}
	
	inline void Leave()
	{
		if (--EnterCount == 0) {
			CurrentThreadId = 0xffffffffu;
			FastLock -= 0x100000u;
		}
	}

private:
	std::atomic<uint32_t> FastLock;
	DWORD CurrentThreadId;
	DWORD EnterCount;
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
	ThreadedFastLock Lock;
	bool IsLoaded;

	std::optional<StringView> GetTranslatedString(RuntimeStringHandle const& handle);
	void UpdateTranslatedString(RuntimeStringHandle const& handle, StringView translated);
};

END_SE()
