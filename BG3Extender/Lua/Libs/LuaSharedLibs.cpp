#include <stdafx.h>
#include <Lua/Libs/LibraryRegistrationHelpers.h>
#include <Lua/Shared/LuaModule.h>
#include <Lua/Shared/LuaMethodCallHelpers.h>
#include <Lua/Osiris/FunctionProxy.h>
#include <Lua/Libs/Debug.inl>
#include <Lua/Libs/Entity.inl>
#include <Lua/Libs/IO.inl>
#include <Lua/Libs/Json.inl>
#include <Lua/Libs/Level.inl>
#include <Lua/Libs/Localization.inl>
#include <Lua/Libs/Log.inl>
#include <Lua/Libs/Math.inl>
#include <Lua/Libs/Mod.inl>
#include <Lua/Libs/StatAttributes.inl>
#include <Lua/Libs/StatMisc.inl>
#include <Lua/Libs/Stats.inl>
#include <Lua/Libs/StaticData.inl>
#include <Lua/Libs/Table.inl>
#include <Lua/Libs/Timer.inl>
#include <Lua/Libs/Types.inl>
#include <Lua/Libs/Utils.inl>
#include <Lua/Libs/Vars.inl>
#include <Lua/Libs/Net.inl>
#include <Lua/Libs/ClientNet.inl>
#include <Lua/Libs/ServerNet.inl>
#include <Lua/Libs/ServerTemplate.inl>
#include <Lua/Libs/ClientTemplate.inl>
#include <Lua/Libs/ClientAudio.inl>
#include <Lua/Libs/ClientInput.inl>
#include <Lua/Libs/ClientUI/Module.inl>
#include <Lua/Libs/ClientIMGUI.inl>

BEGIN_NS(ecl::lua)

void RegisterClientLibraries()
{
    net::RegisterNetLib();
    tmpl::RegisterTemplateLib();
    audio::RegisterAudioLib();
    input::RegisterInputLib();
    ui::RegisterUILib();
    imgui::RegisterIMGUILib();
}

END_NS()

BEGIN_NS(esv::lua)

void RegisterServerLibraries()
{
    net::RegisterNetLib();
    tmpl::RegisterTemplateLib();
}

void RegisterServerMetatables(lua_State* L)
{
    OsiFunctionNameMetatable::RegisterMetatable(L);
}

END_NS()

BEGIN_NS(lua)

void InitObjectProxyPropertyMaps();

void RegisterSystemProxy(lua_State* L)
{
    StackCheck _(L);
    lua_getglobal(L, "Ext");
    SystemMapMetatable::Make(L);
    lua_setfield(L, -2, "System");
    lua_pop(L, 1);
}

void RegisterConfig(lua_State* L)
{
    StackCheck _(L);
    lua_getglobal(L, "Ext");

    lua_newtable(L);

    setfield(L, "ProfilerEnabled", gExtender->GetConfig().Optick);
    setfield(L, "PerfMessagesEnabled", gExtender->GetConfig().EnablePerfMessages);
    setfield(L, "ProfilerLoadCallbackErrorThreshold", gExtender->GetConfig().ProfilerLoadCallbackThreshold.Error);

    if (State::FromLua(L)->IsClient()) {
        setfield(L, "ProfilerCallbackErrorThreshold", gExtender->GetConfig().ProfilerClientCallbackThreshold.Error);
    } else {
        setfield(L, "ProfilerCallbackErrorThreshold", gExtender->GetConfig().ProfilerCallbackThreshold.Error);
    }

    lua_setfield(L, -2, "Config");

    lua_pop(L, 1);
}

void RegisterSharedMetatables(lua_State* L)
{
    ArrayProxyMetatable::RegisterMetatable(L);
    SetProxyMetatable::RegisterMetatable(L);
    MapProxyMetatable::RegisterMetatable(L);
    LightObjectProxyMetatable::RegisterMetatable(L);
    EnumValueMetatable::RegisterMetatable(L);
    BitfieldValueMetatable::RegisterMetatable(L);
    UserVariableHolderMetatable::RegisterMetatable(L);
    ModVariableHolderMetatable::RegisterMetatable(L);
    EntityProxyMetatable::RegisterMetatable(L);
    EntityProxyMetatable::StaticInitialize();
    ImguiObjectProxyMetatable::RegisterMetatable(L);
    SystemMapMetatable::RegisterMetatable(L);
    RegisterSystemProxy(L);
    types::RegisterEnumerations(L);
    RegisterConfig(L);
}

void RegisterSharedLibraries()
{
    utils::RegisterUtilsLib();
    entity::RegisterEntityLib();
    json::RegisterJsonLib();
    types::RegisterTypesLib();
    io::RegisterIOLib();
    loca::RegisterLocalizationLib();
    log::RegisterLogLib();
    math::RegisterMathLib();
    mod::RegisterModLib();
    debug::RegisterDebugLib();
    stats::RegisterStatsLib();
    res::RegisterStaticDataLib();
    vars::RegisterVarsLib();
    table::RegisterTableLib();
    timer::RegisterTimerLib();
    level::RegisterLevelLib();
}

void RegisterLibraries()
{
    if (!gModuleRegistry.GetModules().empty()) return;

    RegisterSharedLibraries();
    esv::lua::RegisterServerLibraries();
    ecl::lua::RegisterClientLibraries();
}

END_NS()
