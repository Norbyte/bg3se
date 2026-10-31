#pragma once

BEGIN_SE()

// Forward declarations

#define DECLARE_EVENT(id, engineName, luaName, cls, ...) struct cls;
#define DECLARE_EVENT_NS(id, engineName, luaName, ns, cls, ...) namespace ns { struct cls; }
#include <GameDefinitions/Generated/Events.inl>
#undef DECLARE_EVENT
#undef DECLARE_EVENT_NS

// Event ID values

#define DECLARE_EVENT(id, engineName, luaName, cls, ...) \
	struct engineName##Event { using ParamsType = cls; }; \
	template <> constexpr EventTypeId EventID<engineName##Event> = EventTypeId(id); \

#define DECLARE_EVENT_NS(id, engineName, luaName, ns, cls, ...) \
	struct engineName##Event { using ParamsType = ns::cls; }; \
	template <> constexpr EventTypeId EventID<engineName##Event> = EventTypeId(id); \

#include <GameDefinitions/Generated/Events.inl>

#undef DECLARE_EVENT
#undef DECLARE_EVENT_NS

END_SE()
