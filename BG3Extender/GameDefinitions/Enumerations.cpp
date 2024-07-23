#include <stdafx.h>

#include <GameDefinitions/Base/Base.h>
#include <Extender/Shared/ExtensionHelpers.h>

namespace bg3se
{
	enum class EnumRegistrationType
	{
		EnumType,
		BitfieldType,
		Value,
		End
	};

	struct EnumRegistrationEntry
	{
		char const* Name;
		char const* LuaName;
		union {
			EnumUnderlyingType Value;
			int32_t TypeId;
		};
		EnumRegistrationType Type;
	};



	static constexpr EnumRegistrationEntry gEnumRegistrationTable[] = {

#define BEGIN_BITMASK_NS(NS, T, luaName, type, id) { .Name = #NS "::" #T, .LuaName = #luaName, .TypeId = id, .Type = EnumRegistrationType::BitfieldType },
#define BEGIN_ENUM_NS(NS, T, luaName, type, id) { .Name = #NS "::" #T, .LuaName = #luaName, .TypeId = id, .Type = EnumRegistrationType::EnumType },
#define BEGIN_BITMASK(T, type, id) { .Name = #T, .LuaName = #T, .TypeId = id, .Type = EnumRegistrationType::BitfieldType },
#define BEGIN_ENUM(T, type, id) { .Name = #T, .LuaName = #T, .TypeId = id, .Type = EnumRegistrationType::EnumType },
#define EV(label, value) { .Name = #label, .Value = (EnumUnderlyingType)value, .Type = EnumRegistrationType::Value },
#define END_ENUM_NS()
#define END_ENUM()
#include <GameDefinitions/Enumerations.inl>
#include <GameDefinitions/ExternalEnumerations.inl>
#undef BEGIN_BITMASK_NS
#undef BEGIN_ENUM_NS
#undef BEGIN_BITMASK
#undef BEGIN_ENUM
#undef EV
#undef END_ENUM_NS
#undef END_ENUM

		{ .Type = EnumRegistrationType::End },
	};

	void InitializeEnumerations()
	{
		EnumInfoStore* enumInfo{ nullptr };
		BitfieldInfoStore* bitfieldInfo{ nullptr };

		for (auto i = 0; i < std::size(gEnumRegistrationTable); i++) {
			auto const& entry = gEnumRegistrationTable[i];
			if (entry.Type == EnumRegistrationType::EnumType) {
				enumInfo = new EnumInfoStore(61, FixedString(entry.Name), FixedString(entry.LuaName));
				bitfieldInfo = nullptr;
				EnumRegistry::Get().Register(enumInfo, entry.TypeId);
			} else if (entry.Type == EnumRegistrationType::BitfieldType) {
				enumInfo = nullptr;
				bitfieldInfo = new BitfieldInfoStore(61, FixedString(entry.Name), FixedString(entry.LuaName));
				BitfieldRegistry::Get().Register(bitfieldInfo, entry.TypeId);
			} else if (entry.Type == EnumRegistrationType::Value) {
				if (enumInfo != nullptr) {
					enumInfo->Add(entry.Value, entry.Name);
				} else {
					bitfieldInfo->Add(entry.Value, entry.Name);
				}
			}
		}
	}
}
