BEGIN_NS(lua::internal)

HashMap<FixedString, uint32_t> GetPublishedEvents(lua_State* L)
{
    HashMap<FixedString, uint32_t> names;
    bool isClient = State::FromLua(L)->IsClient();
    for (auto const& evt : gEventRegistry.EventsById) {
        if (evt.Context == EventContext::Both
            || (!isClient && evt.Context == EventContext::Server)
            || (isClient && evt.Context == EventContext::Client)) {
            names.set(evt.LuaEventName, (uint32_t)evt.RegistryIndex);
        }
    }

    return names;
}

// Signal to start throwing the event as Lua now handles it
void ActivateEvent(lua_State* L, uint32_t eventId)
{
    if (eventId >= EventRegistrySize) {
        luaL_error(L, "Invalid event ID");
    }

    State::FromLua(L)->GetExtensionState().GetEventHints().Activate((EventTypeId)eventId);
}

// Signal to stop throwing the event as there are no Lua subscribers
void DeactivateEvent(lua_State* L, uint32_t eventId)
{
    if (eventId >= EventRegistrySize) {
        luaL_error(L, "Invalid event ID");
    }

    State::FromLua(L)->GetExtensionState().GetEventHints().Deactivate((EventTypeId)eventId);
}

void RegisterInternalLib()
{
    DECLARE_MODULE(Internal, Both)
    BEGIN_MODULE()
    MODULE_FUNCTION(GetPublishedEvents)
    MODULE_FUNCTION(ActivateEvent)
    MODULE_FUNCTION(DeactivateEvent)
    END_MODULE()
}

END_NS()
