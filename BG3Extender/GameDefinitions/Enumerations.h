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

#define BEGIN_BITMASK_NS(NS, T, luaName, type, id) namespace NS { \
    enum class T : type {
#define BEGIN_ENUM_NS(NS, T, luaName, type, id) namespace NS { \
    enum class T : type {
#define BEGIN_BITMASK(T, type, id) enum class T : type {
#define BEGIN_ENUM(T, type, id) enum class T : type {
#define EV(label, value) label = value,
#define END_ENUM_NS() }; };
#define END_ENUM() };
#include <GameDefinitions/Generated/Enumerations.inl>
#undef BEGIN_BITMASK_NS
#undef BEGIN_ENUM_NS
#undef BEGIN_BITMASK
#undef BEGIN_ENUM
#undef EV
#undef END_ENUM_NS
#undef END_ENUM



#define BEGIN_BITMASK_NS(NS, T, luaName, type, id) template <> struct BitfieldID<NS::T> { static constexpr int32_t ID = id; };
#define BEGIN_ENUM_NS(NS, T, luaName, type, id) template <> struct EnumID<NS::T> { static constexpr int32_t ID = id; };
#define BEGIN_BITMASK(T, type, id) template <> struct BitfieldID<T> { static constexpr int32_t ID = id; };
#define BEGIN_ENUM(T, type, id) template <> struct EnumID<T> { static constexpr int32_t ID = id; };
#define EV(label, value)
#define END_ENUM_NS()
#define END_ENUM()
#include <GameDefinitions/Generated/Enumerations.inl>
#include <GameDefinitions/Generated/ExternalEnumerations.inl>
#undef BEGIN_BITMASK_NS
#undef BEGIN_ENUM_NS
#undef BEGIN_BITMASK
#undef BEGIN_ENUM
#undef EV
#undef END_ENUM_NS
#undef END_ENUM


void InitializeEnumerations();


#define BEGIN_BITMASK_NS(NS, T, luaName, type, id) \
    template<> struct IsBitfield<NS::T> { \
        static const bool value = true; \
    };
#define BEGIN_ENUM_NS(NS, T, luaName, type, id)
#define BEGIN_BITMASK(T, type, id) \
    template<> struct IsBitfield<T> { \
        static const bool value = true; \
    };
#define BEGIN_ENUM(T, type, id)
#define EV(label, value)
#define END_ENUM_NS()
#define END_ENUM()
#include <GameDefinitions/Generated/Enumerations.inl>
#include <GameDefinitions/Generated/ExternalEnumerations.inl>
#undef BEGIN_BITMASK_NS
#undef BEGIN_ENUM_NS
#undef BEGIN_BITMASK
#undef BEGIN_ENUM
#undef EV
#undef END_ENUM_NS
#undef END_ENUM

END_SE()
