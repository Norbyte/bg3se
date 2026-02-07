#include <stdafx.h>

#include <GameDefinitions/Base/Base.h>
#include <Extender/Shared/ExtensionHelpers.h>

BEGIN_SE()

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
#include <GameDefinitions/Generated/Enumerations.inl>
#include <GameDefinitions/Generated/ExternalEnumerations.inl>
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

void RegisterEnumerationTypeInformation()
{
    Array<TypeInformation*> typeInfos;
    TypeInformation* typeInfo{ nullptr };

    for (auto i = 0; i < std::size(gEnumRegistrationTable); i++) {
        auto const& entry = gEnumRegistrationTable[i];
        if (entry.Type == EnumRegistrationType::EnumType) {
            if (typeInfos.size() <= entry.TypeId) {
                typeInfos.resize(entry.TypeId + 1);
            }

            typeInfo = &TypeInformationRepository::GetInstance().RegisterType(FixedString(entry.LuaName));
            typeInfo->Kind = LuaTypeId::Enumeration;
            typeInfo->IsBitfield = false;
            typeInfos[entry.TypeId] = typeInfo;
        } else if (entry.Type == EnumRegistrationType::BitfieldType) {
            typeInfo = &TypeInformationRepository::GetInstance().RegisterType(FixedString(entry.LuaName));
            typeInfo->Kind = LuaTypeId::Enumeration;
            typeInfo->IsBitfield = true;
        } else if (entry.Type == EnumRegistrationType::Value) {
            typeInfo->EnumValues.insert(std::make_pair(FixedString(entry.Name), entry.Value));
        }
    }

#define BEGIN_BITMASK_NS(NS, T, luaName, type, id) GetStaticTypeInfo(Overload<NS::T>{}).Type = typeInfos[id];
#define BEGIN_ENUM_NS(NS, T, luaName, type, id) GetStaticTypeInfo(Overload<NS::T>{}).Type = typeInfos[id];
#define BEGIN_BITMASK(T, type, id) GetStaticTypeInfo(Overload<T>{}).Type = typeInfos[id];
#define BEGIN_ENUM(T, type, id) GetStaticTypeInfo(Overload<T>{}).Type = typeInfos[id];
#define EV(label, value)
#define END_ENUM_NS()
#define END_ENUM()

#include <GameDefinitions/Generated/Enumerations.inl>
#include <GameDefinitions/Generated/ExternalEnumerations.inl>

#undef BEGIN_BITMASK_NS
#undef BEGIN_ENUM_NS
#undef BEGIN_BITMASK
#undef BEGIN_ENUM
#undef EV
#undef END_ENUM_NS
#undef END_ENUM
}

END_SE()
