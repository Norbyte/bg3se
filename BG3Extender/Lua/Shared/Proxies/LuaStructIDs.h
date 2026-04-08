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

// Struct ID values

#define DECLARE_CLS(id, ...) template <> constexpr StructTypeId StructID<__VA_ARGS__> = StructTypeId(id);
#define DECLARE_CLS_FWD(id, cls) template <> constexpr StructTypeId StructID<cls> = StructTypeId(id);
#define DECLARE_CLS_NS_FWD(id, ns, cls) template <> constexpr StructTypeId StructID<ns::cls> = StructTypeId(id);
#define DECLARE_CLS_BARE_NS_FWD(id, ns, cls) template <> constexpr StructTypeId StructID<ns::cls> = StructTypeId(id);
#define DECLARE_STRUCT_BARE_NS_FWD(id, ns, cls) template <> constexpr StructTypeId StructID<ns::cls> = StructTypeId(id);
#include <GameDefinitions/Generated/PropertyMapNames.inl>
#undef DECLARE_CLS
#undef DECLARE_CLS_FWD
#undef DECLARE_CLS_NS_FWD
#undef DECLARE_CLS_BARE_NS_FWD
#undef DECLARE_STRUCT_BARE_NS_FWD

END_SE()
