#include "stdafx.h"
#include <CoreLib/Base/Base.h>
#include <combaseapi.h>

#include <CoreLib/Base/BaseMemory.inl>
#include <CoreLib/Base/BaseString.inl>
#include <CoreLib/Base/BaseMap.inl>

BEGIN_SE()

CoreLibPlatformInterface gCoreLibPlatformInterface;

std::optional<Guid> Guid::Parse(StringView s)
{
	Guid uuid;
	if (UuidFromStringA((RPC_CSTR)s.data(), (UUID*)&uuid) == RPC_S_OK) {
		// BG3 parses UUIDs into a weird byte swapped format
		auto v1 = uuid.Val[1];
		uuid.Val[1] = (((v1 >> 56) & 0xff) << 48) |
			(((v1 >> 48) & 0xff) << 56) |
			(((v1 >> 40) & 0xff) << 32) |
			(((v1 >> 32) & 0xff) << 40) |
			(((v1 >> 24) & 0xff) << 16) |
			(((v1 >> 16) & 0xff) << 24) |
			(((v1 >> 8) & 0xff) << 0) |
			(((v1 >> 0) & 0xff) << 8);

		return uuid;
	} else {
		return {};
	}
}

std::optional<Guid> Guid::ParseGuidString(StringView nameGuid)
{
	if (nameGuid.size() < 36) {
		return {};
	}

	auto guid = nameGuid.data() + nameGuid.size() - 36;
	return Parse(std::string_view(guid, 36));
}

STDString Guid::Print() const
{
	uint8_t const* p = reinterpret_cast<uint8_t const*>(this);
	char s[100];
	sprintf_s(s, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		p[3], p[2], p[1], p[0],
		p[5], p[4],
		p[7], p[6],
		p[9], p[8],
		p[11], p[10], p[13], p[12], p[15], p[14]
	);
	return s;
}

END_SE()
