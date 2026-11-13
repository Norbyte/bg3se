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

#define BEGIN_BITMASK_NS(NS, T, luaName, type, ...) { .Name = #NS "::" #T, .LuaName = #luaName, .TypeId = ENUM_TYPE_ID(__VA_ARGS__), .Type = EnumRegistrationType::BitfieldType },
#define BEGIN_ENUM_NS(NS, T, luaName, type, ...) { .Name = #NS "::" #T, .LuaName = #luaName, .TypeId = ENUM_TYPE_ID(__VA_ARGS__), .Type = EnumRegistrationType::EnumType },
#define BEGIN_BITMASK(T, type, ...) { .Name = #T, .LuaName = #T, .TypeId = ENUM_TYPE_ID(__VA_ARGS__), .Type = EnumRegistrationType::BitfieldType },
#define BEGIN_ENUM(T, type, ...) { .Name = #T, .LuaName = #T, .TypeId = ENUM_TYPE_ID(__VA_ARGS__), .Type = EnumRegistrationType::EnumType },
#define E(label) EV(label, 0)
#define EV(label, value) { .Name = #label, .Value = (EnumUnderlyingType)value, .Type = EnumRegistrationType::Value },
#define END_ENUM_NS()
#define END_ENUM()
#include <GameDefinitions/AllEnumerations.inl>
#include <GameDefinitions/AllExternalEnumerations.inl>
#undef BEGIN_BITMASK_NS
#undef BEGIN_ENUM_NS
#undef BEGIN_BITMASK
#undef BEGIN_ENUM
#undef E
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
            EnumRegistry::Get().Register(enumInfo, EnumTypeId(entry.TypeId));
        } else if (entry.Type == EnumRegistrationType::BitfieldType) {
            enumInfo = nullptr;
            bitfieldInfo = new BitfieldInfoStore(61, FixedString(entry.Name), FixedString(entry.LuaName));
            BitfieldRegistry::Get().Register(bitfieldInfo, BitfieldTypeId(entry.TypeId));
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
    Array<TypeInformation*> enumInfos, bitmaskInfos;
    TypeInformation* typeInfo{ nullptr };

    for (auto i = 0; i < std::size(gEnumRegistrationTable); i++) {
        auto const& entry = gEnumRegistrationTable[i];
        if (entry.Type == EnumRegistrationType::EnumType) {
            if (enumInfos.size() <= (uint32_t)entry.TypeId) {
                enumInfos.resize(entry.TypeId + 1);
            }

            typeInfo = &TypeInformationRepository::GetInstance().RegisterType(FixedString(entry.LuaName));
            typeInfo->Kind = LuaTypeId::Enumeration;
            typeInfo->IsBitfield = false;
            se_assert(enumInfos[entry.TypeId] == nullptr);
            enumInfos[entry.TypeId] = typeInfo;
        } else if (entry.Type == EnumRegistrationType::BitfieldType) {
            if (bitmaskInfos.size() <= (uint32_t)entry.TypeId) {
                bitmaskInfos.resize(entry.TypeId + 1);
            }

            typeInfo = &TypeInformationRepository::GetInstance().RegisterType(FixedString(entry.LuaName));
            typeInfo->Kind = LuaTypeId::Enumeration;
            typeInfo->IsBitfield = true;
            se_assert(bitmaskInfos[entry.TypeId] == nullptr);
            bitmaskInfos[entry.TypeId] = typeInfo;
        } else if (entry.Type == EnumRegistrationType::Value) {
            typeInfo->EnumValues.insert(std::make_pair(FixedString(entry.Name), entry.Value));
        }
    }

#define BEGIN_BITMASK_NS(NS, T, luaName, type, ...) GetStaticTypeInfo(Overload<NS::T>{}).Type = bitmaskInfos[ENUM_TYPE_ID(__VA_ARGS__)];
#define BEGIN_ENUM_NS(NS, T, luaName, type, ...) GetStaticTypeInfo(Overload<NS::T>{}).Type = enumInfos[ENUM_TYPE_ID(__VA_ARGS__)];
#define BEGIN_BITMASK(T, type, ...) GetStaticTypeInfo(Overload<T>{}).Type = bitmaskInfos[ENUM_TYPE_ID(__VA_ARGS__)];
#define BEGIN_ENUM(T, type, ...) GetStaticTypeInfo(Overload<T>{}).Type = enumInfos[ENUM_TYPE_ID(__VA_ARGS__)];
#define E(label)
#define EV(label, value)
#define END_ENUM_NS()
#define END_ENUM()

#include <GameDefinitions/AllEnumerations.inl>
#include <GameDefinitions/AllExternalEnumerations.inl>

#undef BEGIN_BITMASK_NS
#undef BEGIN_ENUM_NS
#undef BEGIN_BITMASK
#undef BEGIN_ENUM
#undef E
#undef EV
#undef END_ENUM_NS
#undef END_ENUM
}

END_SE()
