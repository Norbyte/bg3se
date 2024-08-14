#pragma once

template <class T>
constexpr typename std::enable_if_t<std::is_scoped_enum_v<T>, T> operator | (T lhs, T rhs)
{
	using underlying = std::underlying_type_t<T>;
	return static_cast<T>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template <class T>
constexpr typename std::enable_if_t<std::is_scoped_enum_v<T>, T> operator & (T lhs, T rhs)
{
	using underlying = std::underlying_type_t<T>;
	return static_cast<T>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template <class T>
constexpr typename std::enable_if_t<std::is_scoped_enum_v<T>, T> operator ^ (T lhs, T rhs)
{
	using underlying = std::underlying_type_t<T>;
	return static_cast<T>(static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
}

template <class T>
constexpr typename std::enable_if_t<std::is_scoped_enum_v<T>, T> operator ~ (T v)
{
	return static_cast<T>(~static_cast<std::underlying_type_t<T>>(v));
}

template <class T>
constexpr typename std::enable_if_t<std::is_scoped_enum_v<T>, bool> operator ! (T v)
{
	return !static_cast<std::underlying_type_t<T>>(v);
}

template <class T>
constexpr typename std::enable_if_t<std::is_scoped_enum_v<T>, T> operator |= (T& lhs, T rhs)
{
	using underlying = std::underlying_type_t<T>;
	lhs = static_cast<T>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
	return lhs;
}

template <class T>
constexpr typename std::enable_if_t<std::is_scoped_enum_v<T>, T> operator &= (T& lhs, T rhs)
{
	using underlying = std::underlying_type_t<T>;
	lhs = static_cast<T>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
	return lhs;
}

template <class T>
constexpr typename std::enable_if_t<std::is_scoped_enum_v<T>, T> operator ^= (T& lhs, T rhs)
{
	using underlying = std::underlying_type_t<T>;
	lhs = static_cast<T>(static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
	return lhs;
}
