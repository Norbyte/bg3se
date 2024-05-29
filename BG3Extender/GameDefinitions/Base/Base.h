#pragma once

#include <CoreLib/Base/Base.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/Base/ForwardDeclarations.h>

#if defined(ENABLE_UI)
#include <NsCore/Symbol.h>
#include <NsCore/String.h>
#include <NsCore/Vector.h>
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
	static constexpr auto EngineClass = cls;

#define DEFINE_TAG_COMPONENT(ns, name, componentType) \
	struct name : public BaseComponent \
	{ \
		static constexpr ExtComponentType ComponentType = ExtComponentType::componentType; \
		static constexpr auto ComponentName = #componentType; \
		static constexpr auto EngineClass = #ns "::" #name; \
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
	};
