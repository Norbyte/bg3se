#pragma once

#include <GameDefinitions/EnumRepository.h>

#include <cstdint>
#include <SDL_scancode.h>
#include <SDL_keycode.h>
#include <SDL_gamecontroller.h>

#include <NsGui/Enums.h>
#include <NsGui/InputEnums.h>
#include <NsGui/UIElementEvents.h>

#include <imgui.h>

BEGIN_SE()

// Trick to ensure both the generated files (that pass the type ID as the last BEGIN_* argument) and the original ones (that don't) work
#define ENUM_TYPE_ID(...) (__VA_ARGS__ + 0)

#define BEGIN_BITMASK_NS(NS, T, luaName, type, ...) namespace NS { \
    enum class T : type {
#define BEGIN_ENUM_NS(NS, T, luaName, type, ...) namespace NS { \
    enum class T : type {
#define BEGIN_BITMASK(T, type, ...) enum class T : type {
#define BEGIN_ENUM(T, type, ...) enum class T : type {
#define E(label) label,
#define EV(label, value) label = value,
#define END_ENUM_NS() }; };
#define END_ENUM() };
#include <GameDefinitions/AllEnumerations.inl>
#undef BEGIN_BITMASK_NS
#undef BEGIN_ENUM_NS
#undef BEGIN_BITMASK
#undef BEGIN_ENUM
#undef E
#undef EV
#undef END_ENUM_NS
#undef END_ENUM



#define BEGIN_BITMASK_NS(NS, T, luaName, type, ...) template <> constexpr BitfieldTypeId BitfieldID<NS::T> = BitfieldTypeId(ENUM_TYPE_ID(__VA_ARGS__));
#define BEGIN_ENUM_NS(NS, T, luaName, type, ...) template <> constexpr EnumTypeId EnumID<NS::T> = EnumTypeId(ENUM_TYPE_ID(__VA_ARGS__));
#define BEGIN_BITMASK(T, type, ...) template <> constexpr BitfieldTypeId BitfieldID<T> = BitfieldTypeId(ENUM_TYPE_ID(__VA_ARGS__));
#define BEGIN_ENUM(T, type, ...) template <> constexpr EnumTypeId EnumID<T> = EnumTypeId(ENUM_TYPE_ID(__VA_ARGS__));
#define E(label)
#define EV(label, value)
#define END_ENUM_NS()
#define END_ENUM()
#include <GameDefinitions/AllEnumerations.inl>
#if !defined(EXCLUDE_EXTERNALS)
#include <GameDefinitions/AllExternalEnumerations.inl>
#endif
#undef BEGIN_BITMASK_NS
#undef BEGIN_ENUM_NS
#undef BEGIN_BITMASK
#undef BEGIN_ENUM
#undef E
#undef EV
#undef END_ENUM_NS
#undef END_ENUM


void InitializeEnumerations();
void RegisterEnumerationTypeInformation();


#define BEGIN_BITMASK_NS(NS, T, luaName, type, ...)
#define BEGIN_ENUM_NS(NS, T, luaName, type, ...)
#define BEGIN_BITMASK(T, type, ...)
#define BEGIN_ENUM(T, type, ...)
#define E(label)
#define EV(label, value)
#define END_ENUM_NS()
#define END_ENUM()
#include <GameDefinitions/AllEnumerations.inl>
#if !defined(EXCLUDE_EXTERNALS)
#include <GameDefinitions/AllExternalEnumerations.inl>
#endif
#undef BEGIN_BITMASK_NS
#undef BEGIN_ENUM_NS
#undef BEGIN_BITMASK
#undef BEGIN_ENUM
#undef E
#undef EV
#undef END_ENUM_NS
#undef END_ENUM

END_SE()
