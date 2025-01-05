#pragma once

#include <GameDefinitions/Symbols.h>

#include <GameDefinitions/Ai.h>
#include <GameDefinitions/Cursor.h>
#include <GameDefinitions/Dialog.h>
#include <GameDefinitions/Controllers/Controller.h>
#include <GameDefinitions/Controllers/Action.h>
#include <GameDefinitions/Controllers/Behaviour.h>
#include <GameDefinitions/Controllers/Movement.h>
#include <GameDefinitions/Controllers/Steering.h>
#include <GameDefinitions/Controllers/OsirisTask.h>
#include <GameDefinitions/Picking.h>
#include <GameDefinitions/Physics.h>
#include <GameDefinitions/Resources.h>
#include <GameDefinitions/Render.h>
#include <GameDefinitions/Components/All.h>
#include <GameDefinitions/Stats/UseActions.h>
#if defined(ENABLE_UI)
#include <GameDefinitions/UI.h>
#endif

#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>
#include <Lua/Shared/LuaModule.h>
#include <Lua/Shared/LuaMethodCallHelpers.h>
#include <Lua/Shared/Proxies/LuaPropertyMapHelpers.h>
#include <Lua/Shared/Proxies/LuaEvent.h>
#include <Lua/Server/LuaBindingServer.h>
#include <Lua/Client/LuaBindingClient.h>

#include <Extender/ScriptExtender.h>

#if defined(ENABLE_UI)
#include <NsGui/FrameworkElement.h>
#endif

#if defined(ENABLE_IMGUI)
#include <imgui.h>
#include <Extender/Client/IMGUI/Objects.h>
#endif
