#include <Extender/ScriptExtender.h>
#include <Extender/Client/IMGUI/Objects.h>

/// <lua_module>IMGUI</lua_module>
BEGIN_NS(ecl::lua::imgui)

ImguiHandle NewWindow(lua_State* L, char const* name)
{
    auto& ui = ClientState::FromLua(L)->IMGUI();
    auto window = ui.CreateRenderable<extui::Window>();
    window->Label = name;

    if (window->Label.empty()) {
        WARN_ONCE("Creating window with empty label - window will not be drawn until a Label or IDContext is assigned!");
    }

    return window;
}

void EnableDemo(lua_State* L, bool enabled)
{
    ClientState::FromLua(L)->IMGUI().EnableDemo(enabled);
}

bool LoadFont(FixedString const& name, char const* path, float size)
{
    if (!gExtender->IMGUI().WasUIInitialized()) {
        ERR("LoadFont() called before IMGUI was initialized!");
        return false;
    }

    OPTICK_EVENT();
    return gExtender->IMGUI().LoadFont(name, path, size);
}

void SetScale(float scale)
{
    WARN("Ext.IMGUI.SetScale() is deprecated; UI scaling is managed by SE");
}

void SetUIScaleMultiplier(float scale)
{
    gExtender->IMGUI().SetUIScaleMultiplier(scale);
}

void SetFontScaleMultiplier(float scale)
{
    gExtender->IMGUI().SetFontScaleMultiplier(scale);
}

glm::ivec2 GetViewportSize()
{
    return gExtender->IMGUI().GetViewportSize();
}

void RegisterIMGUILib()
{
    DECLARE_MODULE(IMGUI, Client)
    BEGIN_MODULE()
    MODULE_FUNCTION(NewWindow)
    MODULE_FUNCTION(EnableDemo)
    MODULE_FUNCTION(LoadFont)
    MODULE_FUNCTION(SetScale)
    MODULE_FUNCTION(SetUIScaleMultiplier)
    MODULE_FUNCTION(SetFontScaleMultiplier)
    MODULE_FUNCTION(GetViewportSize)
    END_MODULE()
}

END_NS()
