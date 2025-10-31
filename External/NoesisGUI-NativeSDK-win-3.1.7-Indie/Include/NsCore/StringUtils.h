////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_STRINGUTILS_H__
#define __CORE_STRINGUTILS_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>


namespace Noesis
{

/// Indicates whether the specified string is nullptr or an empty string
bool StrIsNullOrEmpty(const char* str);

/// Indicates whether the specified string is an empty string
bool StrIsEmpty(const char* str);

/// Performs a case-insensitive comparison of strings
int StrCaseCompare(const char* str1, const char* str2);
int StrCaseCompare(const char* str1, const char* str2, uint32_t count);

/// Determines whether two strings have the same value
bool StrEquals(const char* str1, const char* str2);
bool StrCaseEquals(const char* str1, const char* str2);

/// Determines whether the beginning of a string matches the specified string
bool StrStartsWith(const char* str, const char* value);
bool StrCaseStartsWith(const char* str, const char* value);

/// Determines whether the end of a string matches the specified string
bool StrEndsWith(const char* str, const char* value);
bool StrCaseEndsWith(const char* str, const char* value);

/// Reports the index of the first occurrence of the specified string
int StrFindFirst(const char* str, const char* value, uint32_t offset = 0);
int StrCaseFindFirst(const char* str, const char* value, uint32_t offset = 0);

/// Reports the index position of the last occurrence of a specified string
int StrFindLast(const char* str, const char* value);

/// Copy characters of one string to another
char* StrCopy(char* dst, uint32_t capacity, const char* src);
char* StrCopy(char* dst, uint32_t capacity, const char* src, uint32_t count);

/// Appends characters of a string
char* StrAppend(char* dst, uint32_t capacity, const char* src);

/// Replaces all occurrences of a specified characeter
char* StrReplace(char* str, char oldValue, char newValue);

/// Calculates the hash value of a string
uint32_t StrHash(const char* str);
uint32_t StrCaseHash(const char* str);

/// Parses the string, interpreting its content as a float and returns its value
NS_CORE_KERNEL_API float StrToFloat(const char* str, uint32_t* charParsed);

/// Parses the string, interpreting its content as a double and returns its value
NS_CORE_KERNEL_API double StrToDouble(const char* str, uint32_t* charParsed);

/// Parses the string, interpreting its content as an integer and returns its value
NS_CORE_KERNEL_API int32_t StrToInt32(const char* str, uint32_t* charParsed);
NS_CORE_KERNEL_API uint32_t StrToUInt32(const char* str, uint32_t* charParsed);
NS_CORE_KERNEL_API int64_t StrToInt64(const char* str, uint32_t* charParsed);
NS_CORE_KERNEL_API uint64_t StrToUInt64(const char* str, uint32_t* charParsed);

}

#include <NsCore/StringUtils.inl>

#endif
