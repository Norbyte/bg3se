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
#if defined(ENABLE_UI)
#include <Lua/Libs/ClientUI/Module.inl>
#endif
#if defined(ENABLE_IMGUI)
#include <Lua/Libs/ClientIMGUI.inl>
#endif

BEGIN_NS(ecl::lua)

void RegisterClientLibraries()
{
    net::RegisterNetLib();
    tmpl::RegisterTemplateLib();
    audio::RegisterAudioLib();
    input::RegisterInputLib();
#if defined(ENABLE_UI)
    ui::RegisterUILib();
#endif
#if defined(ENABLE_IMGUI)
    imgui::RegisterIMGUILib();
#endif
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
#if defined(ENABLE_IMGUI)
    ImguiObjectProxyMetatable::RegisterMetatable(L);
#endif
    types::RegisterEnumerations(L);
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
