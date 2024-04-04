#include <Extender/ScriptExtender.h>
#include <Extender/Client/IMGUI/Objects.h>

/// <lua_module>IMGUI</lua_module>
BEGIN_NS(ecl::lua::imgui)

ImguiHandle NewWindow(lua_State* L, char const* name)
{
	auto& ui = ClientState::FromLua(L)->IMGUI();
	auto window = ui.CreateRenderable<extui::Window>();
	window->Label = name;
	return window;
}

void RegisterIMGUILib()
{
	DECLARE_MODULE(IMGUI, Client)
	BEGIN_MODULE()
	MODULE_FUNCTION(NewWindow)
	END_MODULE()
}

END_NS()
