#pragma once

#include <Extender/Shared/ExtensionState.h>
#include <Lua/Client/LuaBindingClient.h>

BEGIN_NS(ecl)

using LuaClientPin = LuaStatePin<ExtensionState, lua::ClientState>;

class ExtensionState : public ExtensionStateBase
{
public:
	static ExtensionState & Get();

	ExtensionState();
	~ExtensionState() override;

	lua::State * GetLua() override;
	ModManager * GetModManager() override;

	lua::ClientState* GetClientLua()
	{
		if (Lua) {
			return Lua.get();
		} else {
			return nullptr;
		}
	}

	inline char const * GetBootstrapFileName() override
	{
		return "BootstrapClient.lua";
	}

protected:
	friend LuaStatePin<ExtensionState, lua::ClientState>;
	std::unique_ptr<lua::ClientState> Lua;
	uint32_t nextGenerationId_{ 1 };

	void DoLuaReset() override;
	void LuaStartup() override;
};

END_NS()