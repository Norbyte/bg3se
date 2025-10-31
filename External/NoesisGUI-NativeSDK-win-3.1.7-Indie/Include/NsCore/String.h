////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_STRING_H__
#define __CORE_STRING_H__


#include <NsCore/StringFwd.h>
#include <NsCore/StringUtils.h>
#include <NsCore/Memory.h>
#include <NsCore/CompilerTools.h>
#include <NsCore/Error.h>

#include <stdarg.h>
#include <stdio.h>


namespace Noesis
{

template<unsigned N> class FixedString;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Common code factored out of the String class to reduce duplication based on the 'N' parameter.
/// It can be passed around as reference to functions without exposing the parameter 'N'.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BaseString
{
public:
    BaseString(const BaseString&) = delete;

    /// Returns an iterator pointing to the first character in the string
    char* Begin();
    const char* Begin() const;

    /// Returns an iterator referring to the past-the-end character in the string
    char* End();
    const char* End() const;

    /// Returns the length of the string, in terms of bytes.
    uint32_t Size() const;

    /// Resizes the string to a length of n characters
    void Resize(uint32_t n, char c = 0);

    /// Unilaterally moves the string end position to the given location. Useful for when the user
    /// writes into the string via some extenal means such as strcpy or sprintf. This allows for
    /// more efficient use than using resize to achieve this
    void ForceSize(uint32_t n);

    /// Returns the size of the storage space currently allocated for the string
    uint32_t Capacity() const;

    /// Requests that the string capacity be at least enough to contain n characters
    void Reserve(uint32_t n);

    /// Erases the contents of the string
    void Clear();

    /// Returns whether the string is empty
    bool Empty() const;

    /// Returns a reference to the character at specified position
    char& operator[](uint32_t i);
    const char& operator[](uint32_t i) const;

    /// Returns a reference to the last character of the string
    char& Back();
    const char& Back() const;

    /// Returns a reference to the first character of the string
    char& Front();
    const char& Front() const;

    /// Appending operators
    BaseString& operator+=(const BaseString& str);
    BaseString& operator+=(const char* str);
    BaseString& operator+=(char c);

    /// Extends the string by appending additional characters at the end of its current value
    void Append(const char* str);
    void Append(const char* str, uint32_t n);
    void Append(const BaseString& str);
    void Append(uint32_t n, char c);
    void Append(const char* first, const char* last);
    void AppendFormat(NS_FORMAT_PRINTF const char* format, ...) NS_FORMAT_PRINTF_ATTR(2);
    void AppendFormatVA(const char* format, va_list* args);

    /// Appends character c to the end of the string, increasing its length by one
    void PushBack(char c);

    /// Erases the last character of the string
    void PopBack();

    /// Assignment operators
    BaseString& operator=(const BaseString& str);
    BaseString& operator=(const char* str);
    BaseString& operator=(char c);

    /// Assigns a new value to the string, replacing its current contents
    void Assign(const char* str);
    void Assign(const char* str, uint32_t n);
    void Assign(const BaseString& str);
    void Assign(BaseString&& str);
    template<unsigned N> void Assign(FixedString<N>&& str);
    void Assign(uint32_t n, char c);
    void Assign(const char* first, const char* last);

    /// Assigns from printf-style arguments
    void Format(NS_FORMAT_PRINTF const char* format, ...) NS_FORMAT_PRINTF_ATTR(2);

    /// Inserts additional characters into the string
    void Insert(uint32_t pos, const char* str);
    void Insert(uint32_t pos, const char* str, uint32_t n);
    void Insert(uint32_t pos, uint32_t n, char c);

    /// Erases part of the string, reducing its length:
    void Erase(uint32_t pos = 0, uint32_t n = UINT32_MAX);

    /// Replaces the portion of the string
    void Replace(uint32_t pos, uint32_t n, const char* str);

    /// Returns the contents of the string as a null-terminated character array
    const char* Str() const;

    /// Checks if the string begins with the given prefix
    bool StartsWith(const char* str) const;

    /// Find content in string
    int Find(const char* str, uint32_t pos, uint32_t n) const;
    int Find(const char* str, uint32_t pos = 0) const;

    /// Find last occurrence of content in string
    int FindLast(const char* str, uint32_t pos, uint32_t n) const;
    int FindLast(const char* str, uint32_t pos = UINT32_MAX) const;

    /// Trim whitespaces from the string
    void Trim();
    void RTrim();
    void LTrim();

protected:
    BaseString(uint32_t capacity);
    ~BaseString();

    void ResetToSmall(uint32_t capacity);
    void Assign(BaseString&& str, uint32_t capacity);
    void SetStorage(char* ptr);
    void Grow(uint32_t minCapacity);

protected:
    uint32_t mSize;
    uint32_t mCapacity: 24;
    uint32_t mIsSmall: 8;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A string implementation with N bytes of preallocated memory in-place
////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
class FixedString: public BaseString
{
public:
    static_assert(N >= 8, "Incorrect value for N in FixedString<N>");
    struct VarArgs {};

    /// Constructors
    FixedString();
    FixedString(const FixedString& str);
    FixedString(const FixedString& str, uint32_t pos, uint32_t n = UINT32_MAX);
    FixedString(FixedString&& str);
    FixedString(const char* str);
    FixedString(const char* str, uint32_t n);
    FixedString(VarArgs, NS_FORMAT_PRINTF const char* str, ...) NS_FORMAT_PRINTF_ATTR(3);
    FixedString(uint32_t n, char c);
    FixedString(const char* first, const char* last);

    /// Assignment operators
    FixedString& operator=(const FixedString& str);
    FixedString& operator=(FixedString&& str);
    FixedString& operator=(const char* str);
    FixedString& operator=(char c);

    /// Appending operators
    FixedString& operator+=(const FixedString& str);
    FixedString& operator+=(const char* str);
    FixedString& operator+=(char c);

    /// Returns a substring of this object
    FixedString Substr(uint32_t pos = 0, uint32_t n = UINT32_MAX) const;

private:
    /// The inline buffer is overlapped with the pointer to get extra free bytes of storage
    union
    {
        char* mBegin;
        char mSmallString[N];
    };
};

}

#include <NsCore/String.inl>

#endif
