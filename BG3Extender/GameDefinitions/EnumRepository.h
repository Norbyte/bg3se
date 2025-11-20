#pragma once

#include <cstdint>

BEGIN_SE()

template <class T> struct EnumID {};
template <class T> struct BitfieldID {};


// Type used to store enumeration and bitfield values internally.
// Must be a superset of all enum/bitfield types used ingame.
using EnumUnderlyingType = uint64_t;
using EnumTypeId = int32_t;
using BitfieldTypeId = int32_t;


template <class T>
struct BitfieldInfoStoreBase
{
    Vector<FixedString> Labels;
    LegacyMap<FixedString, T> Values;
    T AllowedFlags{ 0 };
    FixedString EnumName;
    FixedString LuaName;
    int RegistryIndex{ -1 };

    BitfieldInfoStoreBase(unsigned sizeHint, FixedString const& enumName, FixedString const& luaName)
    {
        Labels.reserve(sizeHint);
        Values.ResizeHashtable(GetNearestLowerPrime(sizeHint));
        EnumName = enumName;
        LuaName = luaName;
    }

    void __declspec(noinline) Add(T val, char const* label)
    {
        FixedString fs(label);

        // Mapping composite flags (i.e. combination of flags) only supported in one direction
        if ((val & (val - 1)) == 0) {
            DWORD index;
            if (_BitScanForward64(&index, (uint64_t)val)) {
                index++;
            } else {
                index = 0;
            }

            if (Labels.size() <= index) {
                Labels.resize(index + 1);
            }

            // If multiple names are seen with the same value, only keep the last one.
            // (assume older names are deprecated aliases)
            Labels[index] = fs;
        }

        se_assert(Values.find(fs) == Values.end());
        Values.insert(std::move(fs), val);
        AllowedFlags |= val;
    }

    std::optional<T> Find(FixedString const& name) const
    {
        auto val = Values.find(name);
        if (!val) {
            return {};
        } else {
            return val.Value();
        }
    }

    FixedString Find(T val) const
    {
        DWORD index;
        if (_BitScanForward64(&index, val)) {
            index++;
        } else {
            index = 0;
        }

        if (index >= Labels.size()) {
            return FixedString{};
        }

        return Labels[index];
    }
};


template <class T>
struct EnumInfoStoreBase
{
    Vector<FixedString> Labels;
    LegacyMap<FixedString, T> Values;
    FixedString EnumName;
    FixedString LuaName;
    int RegistryIndex{ -1 };

    EnumInfoStoreBase(unsigned sizeHint, FixedString const& enumName, FixedString const& luaName)
    {
        Labels.reserve(sizeHint);
        Values.ResizeHashtable(GetNearestLowerPrime(sizeHint));
        EnumName = enumName;
        LuaName = luaName;
    }

    void __declspec(noinline) Add(T val, char const* label)
    {
        FixedString fs(label);
        auto index = static_cast<uint32_t>(val);

        if (Labels.size() <= index) {
            Labels.resize(index + 1);
        }

        Labels[index] = fs;

        se_assert(Values.find(fs) == Values.end());
        Values.insert(std::move(fs), val);
    }

    std::optional<T> Find(FixedString const& name) const
    {
        auto val = Values.find(name);
        if (val != Values.end()) {
            return (T)val.Value();
        } else {
            return {};
        }
    }

    FixedString Find(T val) const
    {
        if (val >= Labels.size()) {
            return FixedString{};
        }

        return Labels[(uint32_t)val];
    }
};


using BitfieldInfoStore = BitfieldInfoStoreBase<EnumUnderlyingType>;
using EnumInfoStore = EnumInfoStoreBase<EnumUnderlyingType>;


struct EnumRegistry
{
    static EnumRegistry& Get();

    LegacyMap<FixedString, EnumInfoStore*> EnumsByName;
    Array<EnumInfoStore*> EnumsById;

    void Register(EnumInfoStore* ei, EnumTypeId id);
};

struct BitfieldRegistry
{
    static BitfieldRegistry& Get();

    LegacyMap<FixedString, BitfieldInfoStore*> BitfieldsByName;
    Array<BitfieldInfoStore*> BitfieldsById;

    void Register(BitfieldInfoStore* ei, BitfieldTypeId id);
};


template <class T>
struct EnumInfo
{
    inline static EnumInfoStore& GetStore()
    {
        return *EnumRegistry::Get().EnumsById[EnumID<T>::ID];
    }

    inline static std::optional<T> Find(FixedString const& name)
    {
        auto val = GetStore().Find(name);
        if (val) {
            return (T)*val;
        } else {
            return {};
        }
    }

    inline static FixedString Find(T val)
    {
        return GetStore().Find((EnumUnderlyingType)val);
    }
};

template <class T>
struct BitfieldInfo
{
    inline static BitfieldInfoStore& GetStore()
    {
        return *BitfieldRegistry::Get().BitfieldsById[BitfieldID<T>::ID];
    }

    static std::optional<T> Find(FixedString const& name)
    {
        auto val = GetStore().Find(name);
        if (!val) {
            return {};
        } else {
            return (T)*val;
        }
    }

    static FixedString Find(T val)
    {
        return GetStore().Find((EnumUnderlyingType)val);
    }
};


template <class T>
struct IsBitfield {
    static const bool value = false;
};

#define MARK_AS_BITFIELD(T) \
    template<> struct IsBitfield<T> { \
        static const bool value = true; \
    };

template <class T>
constexpr bool IsBitfieldV = IsBitfield<T>::value;

END_SE()


namespace std
{
    template <class T>
    inline std::enable_if_t<std::is_enum_v<T>, ostream&> operator << (ostream& out, T const& v)
    {
        static_assert(!bg3se::IsBitfieldV<T>, "Cannot print bitfields");
        auto label = bg3se::EnumInfo<T>::Find(v);
        if (label) {
            out << label.GetString();
        } else {
            out << "(unknown)";
        }
        return out;
    }
}
