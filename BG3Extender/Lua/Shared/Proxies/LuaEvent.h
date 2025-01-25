#pragma once

#include <Lua/LuaHelpers.h>

BEGIN_SE()

enum class EventResult
{
    Successful,
    Failed,
    ActionPrevented
};

END_SE()

BEGIN_NS(lua)

struct EventBase
{
    FixedString Name;
    bool CanPreventAction{ false };
    bool ActionPrevented{ false };
    bool Stopped{ false };

    void StopPropagation();
    void PreventAction();
};

struct EmptyEvent : public EventBase {};

END_NS()
