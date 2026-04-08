#pragma once

BEGIN_SE()

enum class BuiltinTypeId : int32_t {};
enum class StructTypeId : int32_t{};
enum class EnumTypeId : int32_t{};
enum class BitfieldTypeId : int32_t{};
enum class DynamicTypeId : int32_t{};

template <class T> constexpr BuiltinTypeId BuiltinID = BuiltinTypeId(-1);
template <class T> constexpr StructTypeId StructID = StructTypeId(-1);
template <class T> constexpr EnumTypeId EnumID = EnumTypeId(-1);
template <class T> constexpr BitfieldTypeId BitfieldID = BitfieldTypeId(-1);

template <class T>
concept IsBuiltin = BuiltinID<T> >= BuiltinTypeId(0);

template <class T>
concept IsStruct = StructID<T> >= StructTypeId(0);

template <class T>
concept IsEnum = EnumID<T> >= EnumTypeId(0);

template <class T>
concept IsBitfield = BitfieldID<T> >= BitfieldTypeId(0);

END_SE()
