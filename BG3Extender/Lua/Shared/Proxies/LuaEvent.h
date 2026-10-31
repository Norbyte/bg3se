#pragma once

#include <Lua/LuaHelpers.h>
#include <Lua/Shared/Proxies/LuaEventTypes.h>

BEGIN_NS(lua)

enum class EventResult
{
    Successful,
    Failed,
    ActionPrevented,
    Skipped
};

enum class EventContext
{
    Client = 1,
    Server = 2,
    Both = 3
};

struct EventMetadata
{
    bool Initialized{ false };
    EventTypeId RegistryIndex;
    FixedString EngineEventName;
    FixedString LuaEventName;
    StructTypeId ParamsType;
    EventContext Context;
    bool CanPreventAction;
    RestrictionFlag Restrictions;
};

struct EventRegistry
{
    Array<EventMetadata> EventsById;

    void Initialize(int32_t size);
    void Register(EventTypeId id, EventMetadata&& ei);

    inline EventMetadata const* Get(EventTypeId id) const
    {
        assert((int)id >= 0 && (int)id < (int)EventsById.size());
        return EventsById.data() + (int32_t)id;
    }
};

extern EventRegistry gEventRegistry;

struct EventBase
{
    FixedString Name;
    bool CanPreventAction{ false };
    bool ActionPrevented{ false };
    bool Stopped{ false };

    void StopPropagation();
    void PreventAction();
};

END_NS()
