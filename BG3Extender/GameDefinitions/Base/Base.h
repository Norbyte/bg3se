#pragma once

#include <CoreLib/Base/Base.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/Base/ForwardDeclarations.h>

// The game has a jank SEH handler that silently swallows STATUS_CPP_EH_EXCEPTION without passing it
// to the top-level handler, so we need to have additional checks at the common SE entry points
#define BEGIN_GUARDED() __try {
#define END_GUARDED() } __except (HandleGuardedException(GetExceptionCode(), GetExceptionInformation())) {}

BEGIN_SE()
LONG HandleGuardedException(DWORD code, EXCEPTION_POINTERS* info);
END_SE()

#if defined(ENABLE_UI)
#include <NsCore/Symbol.h>
#include <NsCore/String.h>
#include <NsCore/Vector.h>
#include <NsCore/Interface.h>
#include <NsMath/Vector.h>
#include <NsDrawing/Color.h>
#include <NsDrawing/Point.h>
#include <NsDrawing/Rect.h>
#include <NsGui/BaseCollection.h>
#endif

#if defined(ENABLE_IMGUI)
#include <Extender/Client/IMGUI/IMGUI.h>
#endif

#include <GameDefinitions/Base/LuaAnnotations.h>
#include <GameDefinitions/Base/BaseTypeInformation.h>
#include <GameDefinitions/Base/CommonTypes.h>
#include <GameDefinitions/Base/ExposedTypes.h>
#include <GameDefinitions/Base/Serialization.h>
#include <GameDefinitions/TranslatedString.h>
#include <GameDefinitions/GlobalFixedStrings.h>

#define DEFINE_COMPONENT(componentType, cls) \
	static constexpr ExtComponentType ComponentType = ExtComponentType::componentType; \
	static constexpr auto ComponentName = #componentType; \
	static constexpr auto EngineClass = cls; \
	static constexpr auto OneFrame = false;

#define DEFINE_ONEFRAME_COMPONENT(componentType, cls) \
	static constexpr ExtComponentType ComponentType = ExtComponentType::componentType; \
	static constexpr auto ComponentName = #componentType; \
	static constexpr auto EngineClass = cls; \
	static constexpr auto OneFrame = true;

#define DEFINE_TAG_COMPONENT(ns, name, componentType) \
	struct name : public BaseComponent \
	{ \
		static constexpr ExtComponentType ComponentType = ExtComponentType::componentType; \
		static constexpr auto ComponentName = #componentType; \
		static constexpr auto EngineClass = #ns "::" #name; \
		static constexpr auto OneFrame = false; \
		uint8_t Dummy; \
	};

#define DEFINE_ONEFRAME_TAG_COMPONENT(ns, name, componentType) \
	struct name : public BaseComponent \
	{ \
		static constexpr ExtComponentType ComponentType = ExtComponentType::componentType; \
		static constexpr auto ComponentName = #componentType; \
		static constexpr auto EngineClass = #ns "::" #name; \
		static constexpr auto OneFrame = true; \
		uint8_t Dummy; \
	};

#define DEFN_BOOST(name, boostType, defn) \
	struct name##BoostComponent_Base : public BaseComponent \
	defn; \
	struct name##BoostComponent : public name##BoostComponent_Base \
	{ \
		static constexpr ExtComponentType ComponentType = ExtComponentType::name##Boost; \
		static constexpr auto ComponentName = #name "Boost"; \
		static constexpr auto BoostType = BoostType::boostType; \
		static constexpr auto EngineClass = "eoc::" #name "BoostComponent"; \
		static constexpr auto OneFrame = false; \
	};
