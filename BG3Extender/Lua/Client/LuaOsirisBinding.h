#pragma once

#include <Lua/LuaBinding.h>
#include <GameDefinitions/Osiris.h>
#include <Osiris/Shared/CustomFunctions.h>
#include <Extender/Shared/ExtensionHelpers.h>
#include <Osiris/Shared/OsirisHelpers.h>

BEGIN_NS(ecl::lua)

using namespace bg3se::lua;

class AsyncOsiFunctionNameProxy : public Userdata<AsyncOsiFunctionNameProxy>, public Callable
{
public:
	static char const * const MetatableName;
	// Maximum number of OUT params that a query can return.
	// (This setting determines how many function arities we'll check during name lookup)
	static constexpr uint32_t MaxQueryOutParams = 6;

	static void PopulateMetatable(lua_State * L);

	AsyncOsiFunctionNameProxy(STDString const & name, class ClientState & state);

	int LuaCall(lua_State * L);

private:
	static std::atomic<uint32_t> currentId;
	STDString name_;
	class ClientState & state_;

	static int LuaGet(lua_State * L);
	static int LuaDelete(lua_State * L);
	static int LuaDeferredNotification(lua_State * L);
	bool BeforeCall(lua_State * L);
};

END_NS()
