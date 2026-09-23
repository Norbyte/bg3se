#include "stdafx.h"
#include <CoreLib/Base/Base.h>
#include <combaseapi.h>

#include <CoreLib/Base/BaseMemory.inl>
#include <CoreLib/Base/BaseString.inl>
#include <CoreLib/Base/BaseMap.inl>

BEGIN_SE()

CoreLibPlatformInterface gCoreLibPlatformInterface;

__declspec(noinline) void AssertionFailed(char const* expr)
{
    if (gCoreLibPlatformInterface.GlobalConsole) {
        gCoreLibPlatformInterface.GlobalConsole->Print(DebugMessageType::Error, "Assertion failed:");
        gCoreLibPlatformInterface.GlobalConsole->Print(DebugMessageType::Error, expr);
    }

    DebugBreak();
    std::terminate();
}

const Guid Guid::Null{};

inline uint32_t HexToByte(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0xffffffff; // Error indicator
}

#define PARSE_BYTE(b, off) \
    uint32_t b; { uint32_t val = (HexToByte(s[off]) << 4) | HexToByte(s[off+1]); accum |= val; b = val; }

std::optional<Guid> Guid::Parse(StringView s)
{
    if (s.size() != Guid::StringLength
        || s[8] != '-'
        || s[13] != '-'
        || s[18] != '-'
        || s[23] != '-') {
        return {};
    }

    // Accumulates error flags from all parsed bytes
    uint32_t accum{ 0 };

    Guid uuid;

    // First nibble
    PARSE_BYTE(b0, 0)
    PARSE_BYTE(b1, 2)
    PARSE_BYTE(b2, 4)
    PARSE_BYTE(b3, 6)
    uint32_t data1 = b3
        | (b2 << 8)
        | (b1 << 16)
        | (b0 << 24);

    // 2nd nibble
    PARSE_BYTE(b4, 9)
    PARSE_BYTE(b5, 11)
    uint32_t data2 = b5
        | (b4 << 8);

    // 3rd nibble
    PARSE_BYTE(b6, 14)
    PARSE_BYTE(b7, 16)
    uint32_t data3 = b7
        | (b6 << 8);

    uuid.Val[0] = data1
        | ((uint64_t)data2 << 32)
        | ((uint64_t)data3 << 48);

    // 4th nibble
    PARSE_BYTE(b8, 19)
    PARSE_BYTE(b9, 21)
    uint32_t data4 = b9
        | (b8 << 8);

    // 5th nibble
    PARSE_BYTE(b10, 24)
    PARSE_BYTE(b11, 26)
    PARSE_BYTE(b12, 28)
    PARSE_BYTE(b13, 30)
    PARSE_BYTE(b14, 32)
    PARSE_BYTE(b15, 34)
    uint64_t data5 = b11
        | ((uint64_t)b10 << 8)
        | ((uint64_t)b13 << 16)
        | ((uint64_t)b12 << 24)
        | ((uint64_t)b15 << 32)
        | ((uint64_t)b14 << 40);

    uuid.Val[1] = data4
        | (data5 << 16);

    if (accum & ~0xffu) {
        return {};
    } else {
        return uuid;
    }
}

#undef PARSE_BYTE

std::optional<Guid> Guid::ParseGuidString(StringView nameGuid)
{
    if (nameGuid.size() < Guid::StringLength) {
        return {};
    }

    auto guid = nameGuid.data() + nameGuid.size() - Guid::StringLength;
    return Parse(std::string_view(guid, Guid::StringLength));
}

void CharToHex(uint8_t c, char* dest)
{
    static constexpr char const* HexChars = "0123456789abcdef";
    dest[0] = HexChars[c >> 4];
    dest[1] = HexChars[c & 0x0f];
}

void Guid::ToString(std::span<char> str) const
{
    se_assert(str.size() >= Guid::StringLength);
    auto s = str.data();

    uint8_t const* p = reinterpret_cast<uint8_t const*>(this);

    CharToHex(p[3], s + 0);
    CharToHex(p[2], s + 2);
    CharToHex(p[1], s + 4);
    CharToHex(p[0], s + 6);
    s[8] = '-';

    CharToHex(p[5], s + 9);
    CharToHex(p[4], s + 11);
    s[13] = '-';

    CharToHex(p[7], s + 14);
    CharToHex(p[6], s + 16);
    s[18] = '-';

    CharToHex(p[9], s + 19);
    CharToHex(p[8], s + 21);
    s[23] = '-';

    CharToHex(p[11], s + 24);
    CharToHex(p[10], s + 26);
    CharToHex(p[13], s + 28);
    CharToHex(p[12], s + 30);
    CharToHex(p[15], s + 32);
    CharToHex(p[14], s + 34);
}

STDString Guid::ToString() const
{
    STDString str;
    str.resize(Guid::StringLength);
    ToString(str);
    return str;
}

Guid Guid::Generate()
{
    UUID uuid;
    UuidCreate(&uuid);
    return *reinterpret_cast<Guid*>(&uuid);
}

void LSAcquireSRWLockExclusive(PSRWLOCK SRWLock)
{
    if (!TryAcquireSRWLockExclusive(SRWLock))
    {
        auto loops = 0;
        while (loops++ < 4000)
        {
            if (TryAcquireSRWLockExclusive(SRWLock))
                return;
        }

        AcquireSRWLockExclusive(SRWLock);
    }
}

END_SE()
