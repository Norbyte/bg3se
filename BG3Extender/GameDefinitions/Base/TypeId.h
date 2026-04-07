#pragma once

BEGIN_SE()

using StructTypeId = int32_t;
using EnumTypeId = int32_t;
using BitfieldTypeId = int32_t;

template <class T> constexpr StructTypeId StructID = -1;
template <class T> constexpr EnumTypeId EnumID = -1;
template <class T> constexpr BitfieldTypeId BitfieldID = -1;

template <class T>
concept IsStruct = StructID<T> >= 0;

template <class T>
concept IsEnum = EnumID<T> >= 0;

template <class T>
concept IsBitfield = BitfieldID<T> >= 0;

END_SE()
