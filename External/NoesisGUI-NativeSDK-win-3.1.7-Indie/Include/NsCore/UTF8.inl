////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Error.h>


namespace Noesis
{
namespace UTF8
{

////////////////////////////////////////////////////////////////////////////////////////////////////
char* Append(uint32_t cp, char* text)
{
    NS_ASSERT(text != 0);

    if (cp < 0x80)
    {
        // one octet
        *(text++) = static_cast<char>(cp);
    }
    else if (cp < 0x800)
    {
        // two octets
        *(text++) = static_cast<char>((cp >> 6) | 0xc0);
        *(text++) = static_cast<char>((cp & 0x3f) | 0x80);
    }
    else if (cp < 0x10000)
    {
        // three octets
        *(text++) = static_cast<char>((cp >> 12) | 0xe0);
        *(text++) = static_cast<char>(((cp >> 6) & 0x3f) | 0x80);
        *(text++) = static_cast<char>((cp & 0x3f) | 0x80);
    }
    else 
    {
        // four octets
        *(text++) = static_cast<char>((cp >> 18) | 0xf0);
        *(text++) = static_cast<char>(((cp >> 12) & 0x3f) | 0x80);
        *(text++) = static_cast<char>(((cp >> 6) & 0x3f) | 0x80);
        *(text++) = static_cast<char>((cp & 0x3f) | 0x80);
    }
    
    return text;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint8_t Mask8(char c)
{
    return static_cast<uint8_t>(0xff & c);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint16_t Mask16(uint16_t c)
{
    return static_cast<uint16_t>(0xffff & c);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t SequenceLength(const char* text)
{
    NS_ASSERT(text != 0);
    uint8_t lead = Mask8(*text);

    if (lead < 0x80)
    {
        return 1;
    }
    else if ((lead >> 5) == 0x6)
    {
        return 2;
    }
    else if ((lead >> 4) == 0xe)
    {
        return 3;
    }
    else if ((lead >> 3) == 0x1e)
    {
        return 4;
    }

    NS_ASSERT_UNREACHABLE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t Next(const char*& text)
{
    NS_ASSERT(text != 0);
    uint32_t cp = Mask8(*text);

    switch (SequenceLength(text)) 
    {
        case 1:
        {
            ++text;
            break;
        }
        case 2:
        {
            NS_ASSERT(*text != 0);
            ++text;
            cp = ((cp << 6) & 0x7ff) + ((*text) & 0x3f);
            NS_ASSERT(*text != 0);
            ++text;
            break;
        }
        case 3:
        {
            NS_ASSERT(*text != 0);
            ++text; 
            cp = ((cp << 12) & 0xffff) + ((Mask8(*text) << 6) & 0xfff);
            NS_ASSERT(*text != 0);
            ++text;
            cp += (*text) & 0x3f;
            NS_ASSERT(*text != 0);
            ++text;
            break;
        }
        case 4:
        {
            NS_ASSERT(*text != 0);
            ++text;
            cp = ((cp << 18) & 0x1fffff) + ((Mask8(*text) << 12) & 0x3ffff);
            NS_ASSERT(*text != 0);
            ++text;
            cp += (Mask8(*text) << 6) & 0xfff;
            NS_ASSERT(*text != 0);
            ++text;
            cp += (*text) & 0x3f;
            NS_ASSERT(*text != 0);
            ++text;
            break;
        }
        default:
            NS_ASSERT_UNREACHABLE;
    }

    return cp; 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t Next(char*& text)
{
    NS_ASSERT(text != 0);
    const char* text_ = text;
    uint32_t cp = Next(text_);
    text = const_cast<char*>(text_);
    return cp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t Prior(const char*& text)
{
    NS_ASSERT(text != 0);
    while ((Mask8(*(--text)) >> 6) == 0x2) {}
    return Get(text);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t Prior(char*& text)
{
    NS_ASSERT(text != 0);
    const char* text_ = text;
    uint32_t cp = Prior(text_);
    text = const_cast<char*>(text_);
    return cp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t Get(const char* text)
{
    NS_ASSERT(text != 0);
    return Next(text);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Advance(const char*& text, uint32_t n)
{
    NS_ASSERT(text != 0);
    for (uint32_t i = 0; i < n; i++)
    {
        Next(text);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Advance(char*& text, uint32_t n)
{
    NS_ASSERT(text != 0);
    const char* text_ = text;
    Advance(text_, n);
    text = const_cast<char*>(text_);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t Distance(const char* first, const char* last)
{
    NS_ASSERT(first != 0);
    NS_ASSERT(last != 0);

    uint32_t len = 0;

    while (first != last)
    {
        NS_ASSERT(first < last);
        first += SequenceLength(first);
        len++;
    }

    return len;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t Length(const char* text)
{
    NS_ASSERT(text != 0);

    uint32_t len = 0;

    while (*text != 0)
    {
        uint32_t numBytes = SequenceLength(text);

        switch (numBytes)
        {
            case 4:
                NS_ASSERT(*(text + 3) != 0);
            case 3:
                NS_ASSERT(*(text + 2) != 0);
            case 2:
                NS_ASSERT(*(text + 1) != 0);
                NS_ASSERT(*(text + 0) != 0);
        }

        text += numBytes;
        len++;
    }

    return len;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool IsValid(const char* text)
{
    NS_ASSERT(text != 0);

    while (*text != 0)
    {
        uint8_t lead = Mask8(*text);

        if (lead < 0x80)
        {
            text++;
        }
        else if ((lead >> 5) == 0x6)
        {
            if (*(text) == 0 || *(text + 1) == 0)
            {
                return false;
            }

            text += 2;
        }
        else if ((lead >> 4) == 0xe)
        {
            if (*(text) == 0 || *(text + 1) == 0 || *(text + 2) == 0)
            {
                return false;
            }

            text += 3;
        }
        else if ((lead >> 3) == 0x1e)
        {
            if (*(text) == 0 || *(text + 1) == 0 || *(text + 2) == 0 || *(text + 3) == 0)
            {
                return false;
            }

            text += 4;
        }
        else
        {
            return false;
        }
    }

    return true;
}

#define LEAD_SURROGATE_MIN 0xd800u
#define LEAD_SURROGATE_MAX 0xdbffu
#define TRAIL_SURROGATE_MIN 0xdc00u
#define TRAIL_SURROGATE_MAX 0xdfffu
#define LEAD_OFFSET (LEAD_SURROGATE_MIN - (0x10000 >> 10))
#define SURROGATE_OFFSET (0x10000u - (LEAD_SURROGATE_MIN << 10) - TRAIL_SURROGATE_MIN)

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t UTF8To16(const char* utf8, uint16_t* utf16, uint32_t numUTF16Chars)
{
    NS_ASSERT(utf8 != 0);
    uint32_t totalChars = 0;
    uint32_t writtenChars = 0;

    for (;;)
    {
        uint32_t c = UTF8::Next(utf8);

        if (c <= 0xffff)
        {
            if (totalChars + 1 < numUTF16Chars)
            {
                utf16[totalChars] = static_cast<uint16_t>(c);
                writtenChars++;
            }

            totalChars++;
        }
        else
        {
            if (totalChars + 2 < numUTF16Chars)
            {
                utf16[totalChars] = static_cast<uint16_t>((c >> 10) + LEAD_OFFSET);
                utf16[totalChars + 1] = static_cast<uint16_t>((c & 0x3ff) + TRAIL_SURROGATE_MIN);
                writtenChars += 2;
            }

            totalChars += 2;
        }

        if (c == 0)
        {
            if (writtenChars < numUTF16Chars)
            {
                utf16[writtenChars] = 0;
            }

            return totalChars;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t UTF16To8(const uint16_t* utf16, char* utf8, uint32_t numUTF8Chars)
{
    NS_ASSERT(utf16 != 0);
    uint32_t totalChars = 0;
    uint32_t writtenChars = 0;

    for (;;)
    {
        uint32_t c = Mask16(*utf16++);
        bool isLeadSurrogate = c >= LEAD_SURROGATE_MIN && c <= LEAD_SURROGATE_MAX;

        if (isLeadSurrogate)
        {
            uint32_t trail = Mask16(*utf16++);
            c = (c << 10) + trail + SURROGATE_OFFSET;
        }

        uint32_t numOctets = c < 0x80 ? 1 : c < 0x800 ? 2 : c < 0x10000 ? 3 : 4;

        if (totalChars + numOctets + 1 <= numUTF8Chars)
        {
            utf8 = UTF8::Append(c, utf8);
            writtenChars += numOctets;
        }

        totalChars += numOctets;

        if (c == 0)
        {
            if (writtenChars < numUTF8Chars)
            {
                *utf8 = 0;
            }

            return totalChars;
        }
    }
}

#undef LEAD_SURROGATE_MIN
#undef LEAD_SURROGATE_MAX
#undef TRAIL_SURROGATE_MIN
#undef TRAIL_SURROGATE_MAX
#undef LEAD_OFFSET
#undef SURROGATE_OFFSET

}
}
