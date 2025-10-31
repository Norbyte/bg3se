////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Error.h>
#include <NsCore/Math.h>

#include <string.h>


namespace Noesis
{


////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool StrIsNullOrEmpty(const char* str)
{
    return str == nullptr || *str == 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool StrIsEmpty(const char* str)
{
    NS_ASSERT(str != nullptr);
    return *str == 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline char ToLower(char c)
{
    return (c >= 'A' && c <= 'Z') ? c + 'a' - 'A' : c;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int StrCaseCompare(const char* str1, const char* str2)
{
    while (true)
    {
        char a = ToLower(*str1++);
        char b = ToLower(*str2++);

        if (a != b)
        {
            return a < b ? -1 : 1;
        }

        if (a == 0)
        {
            return 0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int StrCaseCompare(const char* str1, const char* str2, uint32_t count)
{
    while (count-- > 0)
    {
        char a = ToLower(*str1++);
        char b = ToLower(*str2++);

        if (a != b)
        {
            return a < b ? -1 : 1;
        }

        if (a == 0)
        {
            return 0;
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool StrEquals(const char* str1, const char* str2)
{
    NS_ASSERT(str1 != nullptr);
    NS_ASSERT(str2 != nullptr);
    return strcmp(str1, str2) == 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool StrCaseEquals(const char* str1, const char* str2)
{
    NS_ASSERT(str1 != nullptr);
    NS_ASSERT(str2 != nullptr);
    return StrCaseCompare(str1, str2) == 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool StrStartsWith(const char* str, const char* value)
{
    NS_ASSERT(str != nullptr);
    NS_ASSERT(value != nullptr);
    return strncmp(str, value, strlen(value)) == 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool StrCaseStartsWith(const char* str, const char* value)
{
    NS_ASSERT(str != nullptr);
    NS_ASSERT(value != nullptr);
    return StrCaseCompare(str, value, (uint32_t)strlen(value)) == 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool StrEndsWith(const char* str, const char* value)
{
    NS_ASSERT(str != nullptr);
    NS_ASSERT(value != nullptr);

    size_t len = strlen(str);
    size_t valueLen = strlen(value);

    if (len >= valueLen)
    {
        return strcmp(str + len - valueLen, value) == 0;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool StrCaseEndsWith(const char* str, const char* value)
{
    NS_ASSERT(str != nullptr);
    NS_ASSERT(value != nullptr);

    size_t len = strlen(str);
    size_t valueLen = strlen(value);

    if (len >= valueLen)
    {
        return StrCaseCompare(str + len - valueLen, value) == 0;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int StrFindFirst(const char* str, const char* value, uint32_t offset)
{
    NS_ASSERT(str != nullptr);
    NS_ASSERT(value != nullptr);

    size_t valueLen = strlen(value);
    size_t len = strlen(str);

    for (uint32_t i = offset; i + valueLen <= len; ++i)
    {
        if (strncmp(str + i, value, valueLen) == 0)
        {
            return i;
        }
    }

    return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int StrCaseFindFirst(const char* str, const char* value, uint32_t offset)
{
    NS_ASSERT(str != nullptr);
    NS_ASSERT(value != nullptr);

    size_t valueLen = strlen(value);
    size_t len = strlen(str);

    for (uint32_t i = offset; i + valueLen <= len; ++i)
    {
        if (StrCaseCompare(str + i, value, (uint32_t)valueLen) == 0)
        {
            return i;
        }
    }

    return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int StrFindLast(const char* str, const char* value)
{
    NS_ASSERT(str != nullptr);
    NS_ASSERT(value != nullptr);

    uint32_t valueLen = (uint32_t)strlen(value);
    uint32_t len = (uint32_t)strlen(str);

    for (int i = len - valueLen; i >= 0; i--)
    {
        if (strncmp(str + i, value, valueLen) == 0)
        {
            return i;
        }
    }

    return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline char* StrCopy(char* dst, uint32_t capacity, const char* src)
{
    NS_ASSERT(dst != nullptr);
    NS_ASSERT(src != nullptr);
    NS_ASSERT(capacity != 0);

    uint32_t n = Min((uint32_t)strlen(src), capacity - 1);
    memcpy(dst, src, n);
    dst[n] = 0;

    return dst;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline char* StrCopy(char* dst, uint32_t capacity, const char* src, uint32_t count)
{
    NS_ASSERT(dst != nullptr);
    NS_ASSERT(src != nullptr);
    NS_ASSERT(capacity != 0);

    uint32_t n = Min(Min((uint32_t)strlen(src), count), capacity - 1);
    memcpy(dst, src, n);
    dst[n] = 0;

    return dst;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline char* StrAppend(char* dst, uint32_t capacity, const char* src)
{
    NS_ASSERT(dst != nullptr);
    NS_ASSERT(src != nullptr);
    NS_ASSERT(capacity != 0);

    uint32_t len = (uint32_t)strlen(dst);
    NS_ASSERT(len < capacity);

    uint32_t n = Min((uint32_t)strlen(src), capacity - len - 1);
    memcpy(dst + len, src, n);
    dst[len + n] = 0;

    return dst;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline char* StrReplace(char* str, char oldValue, char newValue)
{
    NS_ASSERT(str != nullptr);

    while (*str != 0)
    {
        if (*str == oldValue)
        {
            *str = newValue;
        }

        ++str;
    }

    return str;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t StrHash(const char* str)
{
    NS_ASSERT(str != nullptr);

    char c;
    uint32_t result = 2166136261U;

    while ((c = *str++) != 0)
    {
        result = (result * 16777619) ^ c;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t StrCaseHash(const char* str)
{
    NS_ASSERT(str != nullptr);

    char c;
    uint32_t result = 2166136261U;

    while ((c = *str++) != 0)
    {
        result = (result * 16777619) ^ ToLower(c);
    }

    return result;
}

}
