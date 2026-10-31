#include <stdafx.h>
#include <Lua/Shared/Proxies/LuaEvent.h>
#include <GameDefinitions/Generated/EventsMeta.h>

BEGIN_NS(lua)

struct EventRegistrationEntry
{
    EventTypeId Id;
    char const* EngineEventName;
    char const* LuaEventName;
    StructTypeId ParamsType;
    EventContext Context;
    bool CanPreventAction;
    RestrictionFlag Restrictions;
};

#define DECLARE_EVENT(id, engineName, luaName, cls, context, canPreventAction, restrictions) \
        EventRegistrationEntry { \
            .Id = EventTypeId{ id }, \
            .EngineEventName = #engineName "Event", \
            .LuaEventName = #luaName, \
            .ParamsType = StructID<cls>, \
            .Context = lua::EventContext::context, \
            .CanPreventAction = canPreventAction, \
            .Restrictions = restrictions \
        },

#define DECLARE_EVENT_NS(id, engineName, luaName, ns, cls, context, canPreventAction, restrictions) \
        EventRegistrationEntry { \
            .Id = EventTypeId{ id }, \
            .EngineEventName = #engineName "Event", \
            .LuaEventName = #luaName, \
            .ParamsType = StructID<ns::cls>, \
            .Context = lua::EventContext::context, \
            .CanPreventAction = canPreventAction, \
            .Restrictions = restrictions \
        },

static constexpr EventRegistrationEntry const AllEventDefns[] = {
#include <GameDefinitions/Generated/Events.inl>
};

#undef DECLARE_EVENT
#undef DECLARE_EVENT_NS

void InitEventRegistry()
{
    static bool initialized{ false };
    if (initialized) return;

    gEventRegistry.Initialize(EventRegistrySize);
    for (auto const& defn : std::span(AllEventDefns)) {
        gEventRegistry.Register(
            defn.Id,
            EventMetadata{
                .EngineEventName = FixedString(defn.EngineEventName),
                .LuaEventName = FixedString(defn.LuaEventName),
                .ParamsType = defn.ParamsType,
                .Context = defn.Context,
                .CanPreventAction = defn.CanPreventAction,
                .Restrictions = defn.Restrictions,
            }
        );
    }

    initialized = true;
}

END_NS()
