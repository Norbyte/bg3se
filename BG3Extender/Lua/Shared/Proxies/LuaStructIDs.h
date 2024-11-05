#pragma once

#include <GameDefinitions/Resources.h>
#include <GameDefinitions/GuidResources.h>
#include <GameDefinitions/Components/Spell.h>
#include <GameDefinitions/Progression.h>
#include <GameDefinitions/AllSpark.h>
#include <GameDefinitions/Render.h>
#include <GameDefinitions/Stats/Common.h>

// Non-SE forward declarations

#define DECLARE_CLS(id, ...)
#define DECLARE_CLS_FWD(id, cls)
#define DECLARE_CLS_NS_FWD(id, ns, cls)
#define DECLARE_CLS_BARE_NS_FWD(id, ns, cls) namespace ns { class cls; }
#define DECLARE_STRUCT_BARE_NS_FWD(id, ns, cls) namespace ns { struct cls; }
#include <GameDefinitions/Generated/PropertyMapNames.inl>
#undef DECLARE_CLS
#undef DECLARE_CLS_FWD
#undef DECLARE_CLS_NS_FWD
#undef DECLARE_CLS_BARE_NS_FWD
#undef DECLARE_STRUCT_BARE_NS_FWD

BEGIN_SE()

// SE forward declarations

#define DECLARE_CLS(id, ...)
#define DECLARE_CLS_FWD(id, cls) struct cls;
#define DECLARE_CLS_NS_FWD(id, ns, cls) namespace ns { struct cls; }
#define DECLARE_CLS_BARE_NS_FWD(id, ns, cls)
#define DECLARE_STRUCT_BARE_NS_FWD(id, ns, cls)
#include <GameDefinitions/Generated/PropertyMapNames.inl>
#undef DECLARE_CLS
#undef DECLARE_CLS_FWD
#undef DECLARE_CLS_NS_FWD
#undef DECLARE_CLS_BARE_NS_FWD
#undef DECLARE_STRUCT_BARE_NS_FWD


using StructTypeId = int32_t;

template <class T> struct StructID {
	static constexpr bool Valid = false;
};

// Struct ID values

#define DECLARE_CLS(id, ...) template <> struct StructID<__VA_ARGS__> { static constexpr bool Valid = true; static constexpr StructTypeId ID = id; };
#define DECLARE_CLS_FWD(id, cls) template <> struct StructID<cls> { static constexpr bool Valid = true; static constexpr StructTypeId ID = id; };
#define DECLARE_CLS_NS_FWD(id, ns, cls) template <> struct StructID<ns::cls> { static constexpr bool Valid = true; static constexpr StructTypeId ID = id; };
#define DECLARE_CLS_BARE_NS_FWD(id, ns, cls) template <> struct StructID<ns::cls> { static constexpr bool Valid = true; static constexpr StructTypeId ID = id; };
#define DECLARE_STRUCT_BARE_NS_FWD(id, ns, cls) template <> struct StructID<ns::cls> { static constexpr bool Valid = true; static constexpr StructTypeId ID = id; };
#include <GameDefinitions/Generated/PropertyMapNames.inl>
#undef DECLARE_CLS
#undef DECLARE_CLS_FWD
#undef DECLARE_CLS_NS_FWD
#undef DECLARE_CLS_BARE_NS_FWD
#undef DECLARE_STRUCT_BARE_NS_FWD

END_SE()
