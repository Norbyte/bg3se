#pragma once

#include <Lua/LuaHelpers.h>
#include <Lua/Shared/Proxies/LuaEventTypes.h>
#include <GameDefinitions/Generated/EventsMeta.h>

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

// Stores hints to indicate whether an event is currently handled by any Lua subscribers or not.
// Checked before acquiring the lua VM lock to avoid unnecessary waits.
struct EventSubscriberHints
{
    std::array<std::atomic<uint32_t>, EventRegistrySize> Subscribers;

    inline bool ShouldThrowEvent(EventTypeId id) const
    {
        return Subscribers[(uint32_t)id] > 0;
    }

    inline void Activate(EventTypeId id)
    {
        Subscribers[(uint32_t)id] = 1;
    }

    inline void Deactivate(EventTypeId id)
    {
        Subscribers[(uint32_t)id] = 0;
    }

    inline void DeactivateAll()
    {
        for (auto& sub : Subscribers) {
            sub = 0;
        }
    }
};

END_NS()
