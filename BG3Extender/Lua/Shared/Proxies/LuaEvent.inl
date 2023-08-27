#include <Lua/Shared/Proxies/LuaEvent.h>

BEGIN_NS(lua)

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
