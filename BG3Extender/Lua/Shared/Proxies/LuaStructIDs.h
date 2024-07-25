#pragma once

#include <GameDefinitions/Resources.h>
#include <GameDefinitions/GuidResources.h>
#include <GameDefinitions/Components/Spell.h>
#include <GameDefinitions/Progression.h>
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



template <class T> struct StructID {};

// Struct ID values

#define DECLARE_CLS(id, ...) template <> struct StructID<__VA_ARGS__> { static constexpr int32_t ID = id; };
#define DECLARE_CLS_FWD(id, cls) template <> struct StructID<cls> { static constexpr int32_t ID = id; };
#define DECLARE_CLS_NS_FWD(id, ns, cls) template <> struct StructID<ns::cls> { static constexpr int32_t ID = id; };
#define DECLARE_CLS_BARE_NS_FWD(id, ns, cls) template <> struct StructID<ns::cls> { static constexpr int32_t ID = id; };
#define DECLARE_STRUCT_BARE_NS_FWD(id, ns, cls) template <> struct StructID<ns::cls> { static constexpr int32_t ID = id; };
#include <GameDefinitions/Generated/PropertyMapNames.inl>
#undef DECLARE_CLS
#undef DECLARE_CLS_FWD
#undef DECLARE_CLS_NS_FWD
#undef DECLARE_CLS_BARE_NS_FWD
#undef DECLARE_STRUCT_BARE_NS_FWD

END_SE()
