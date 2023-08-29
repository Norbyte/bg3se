#pragma once

#include <CoreLib/Base/Base.h>

BEGIN_SE()

struct RuntimeStringHandle
{
	FixedString Handle;
	uint16_t Version{ 0 };

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
inline uint64_t MultiHashMapHash<RuntimeStringHandle>(RuntimeStringHandle const& v)
{
	return MultiHashMapHash(v.Handle);
}

struct TranslatedString
{
	RuntimeStringHandle Handle;
	RuntimeStringHandle ArgumentString;
};

struct TranslatedStringRepository : public ProtectedGameObject<TranslatedStringRepository>
{
	struct TextEntry
	{
		RuntimeStringHandle* Handle;
		StringView* Text;
		__int64 field_10;
		__int64 field_18;
	};

	struct TextPool
	{
		Array<STDString*> Strings;
		MultiHashMap<RuntimeStringHandle, StringView> Texts;
	};

	int field_0;
	TextPool* TranslatedStrings[9];
	TextPool* TranslatedArgumentStrings;
	TextPool* StringHandles;
	Array<void*> field_60;
	MultiHashMap<FixedString, void*> field_70;
	MultiHashMap<FixedString, void*> field_B0;
	DWORD FastLock;
	DWORD CurrentThreadId;
	int field_F8;
	char field_FC;
};

END_SE()
