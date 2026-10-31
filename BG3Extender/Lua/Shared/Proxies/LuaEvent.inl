#include <Lua/Shared/Proxies/LuaEvent.h>

BEGIN_NS(lua)

EventRegistry gEventRegistry;

void EventRegistry::Initialize(int32_t size)
{
    se_assert(EventsById.empty());
    EventsById.resize(size);
}

void EventRegistry::Register(EventTypeId id, EventMetadata&& meta)
{
    se_assert((uint32_t)id < EventsById.size());
    se_assert(!EventsById[(int32_t)id].Initialized);

    meta.Initialized = true;
    meta.RegistryIndex = id;
    EventsById[(int32_t)id] = std::move(meta);
}


void EventBase::StopPropagation()
{
    Stopped = true;
}

void EventBase::PreventAction()
{
    if (CanPreventAction) {
        ActionPrevented = true;
    } else {
        OsiErrorS("Can't prevent action");
    }
}

END_NS()
