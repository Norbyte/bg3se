#pragma once

#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/Symbols.h>

#include <GameDefinitions/Resources.h>
#include <GameDefinitions/Components/Combat.h>
#include <GameDefinitions/Components/Components.h>
#include <GameDefinitions/Components/Passives.h>
#include <GameDefinitions/Components/Projectile.h>
#include <GameDefinitions/Components/Stats.h>

#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>
#include <Lua/Shared/LuaModule.h>
#include <Lua/Shared/LuaMethodCallHelpers.h>
#include <Lua/Shared/Proxies/LuaPropertyMapHelpers.h>
#include <Lua/Shared/Proxies/LuaEvent.h>
#include <Lua/Server/LuaBindingServer.h>
#include <Lua/Client/LuaBindingClient.h>

#include <Extender/ScriptExtender.h>
