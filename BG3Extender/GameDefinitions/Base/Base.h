#pragma once

#include <CoreLib/Base/Base.h>
#include <GameDefinitions/Enumerations.h>

#if defined(ENABLE_UI)
#include <NsCore/Symbol.h>
#include <NsCore/String.h>
#include <NsCore/Vector.h>
#endif

#include <GameDefinitions/Base/LuaAnnotations.h>
#include <GameDefinitions/Base/BaseTypeInformation.h>
#include <GameDefinitions/Base/ForwardDeclarations.h>
#include <GameDefinitions/Base/CommonTypes.h>
#include <GameDefinitions/Base/ExposedTypes.h>
#include <GameDefinitions/Base/Serialization.h>
#include <GameDefinitions/TranslatedString.h>
#include <GameDefinitions/GlobalFixedStrings.h>

#define DEFINE_TAG_COMPONENT(ns, name, componentType) \
	struct name : public BaseComponent \
	{ \
		static constexpr ExtComponentType ComponentType = ExtComponentType::componentType; \
		static constexpr auto EngineClass = #ns "::" #name; \
		uint8_t Dummy; \
	};

#define DEFN_BOOST(name, boostType, defn) \
	struct name##BoostComponent_Base : public BaseComponent \
	defn; \
	struct name##BoostComponent : public name##BoostComponent_Base \
	{ \
		static constexpr ExtComponentType ComponentType = ExtComponentType::name##Boost; \
		static constexpr auto BoostType = BoostType::boostType; \
		static constexpr auto EngineClass = "eoc::" #name "BoostComponent"; \
	};
