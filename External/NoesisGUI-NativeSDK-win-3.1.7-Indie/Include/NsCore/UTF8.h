////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_UTF8_H__
#define __CORE_UTF8_H__


#include <NsCore/Noesis.h>


namespace Noesis
{
namespace UTF8
{

/// Appends unicode point to UTF8 encoded string
inline char* Append(uint32_t cp, char* text);

/// Returns next unicode point and moves to next position
inline uint32_t Next(const char*& text);
inline uint32_t Next(char*& text);

/// Decreases the pointer until the beginning of an UTF8 encoded code point and returns it
inline uint32_t Prior(const char*& text);
inline uint32_t Prior(char*& text);

/// Returns first unicode point of UTF8 encoded string
inline uint32_t Get(const char* text);

/// Advances an iterator by the specified number of code points within an UTF-8 sequence
inline void Advance(const char*& text, uint32_t n);
inline void Advance(char*& text, uint32_t n);

/// Returns the number of code points between two UTF8 pointers
inline uint32_t Distance(const char* first, const char* last);

/// Returns the number of characters in an UTF8 encoded string
inline uint32_t Length(const char* text);

/// Checks whether given buffer contains valid UTF-8
inline bool IsValid(const char* text);

/// Conversion from UTF8 to UTF16. Returns the numbers of characters needed for the whole text
/// even if they don't fit in the provided buffer.
inline uint32_t UTF8To16(const char* utf8, uint16_t* utf16, uint32_t numUTF16Chars);

/// Conversion from UTF16 to UTF8. Returns the numbers of characters needed for the whole text
/// even if they don't fit in the provided buffer.
inline uint32_t UTF16To8(const uint16_t* utf16, char* utf8, uint32_t numUTF8Chars);

}
}

#include <NsCore/UTF8.inl>

#endif
