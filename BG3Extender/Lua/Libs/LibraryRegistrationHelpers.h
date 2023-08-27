#pragma once

#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>
#include <Extender/ScriptExtender.h>
#include <Lua/Shared/LuaModule.inl>
#include <Lua/Shared/LuaReference.inl>
#include <Lua/Shared/Proxies/LuaPropertyMap.inl>
#include <Lua/Shared/Proxies/LuaCppClass.inl>
#include <Lua/Shared/Proxies/LuaArrayProxy.inl>
#include <Lua/Shared/Proxies/LuaEntityProxy.inl>
#include <Lua/Shared/Proxies/LuaMapProxy.inl>
#include <Lua/Shared/Proxies/LuaObjectProxy.inl>
#include <Lua/Shared/Proxies/LuaSetProxy.inl>
#include <Lua/Shared/Proxies/LuaEvent.inl>

BEGIN_SE()

template <class Fun>
TypeInformation DoConstructFunctionSignature(Fun f)
{
	TypeInformation sig;
	ConstructFunctionSignature(sig, f);
	return sig;
}

#define DECLARE_MODULE(name, role) { \
	ModuleDefinition mod; \
	mod.Role = ModuleRole::role; \
	mod.Table = FixedString{#name};

#define DECLARE_SUBMODULE(name, sub, role) { \
	ModuleDefinition mod; \
	mod.Role = ModuleRole::role; \
	mod.Table = FixedString{#name}; \
	mod.SubTable = FixedString{#sub};

#define BEGIN_MODULE() mod.Functions = {
#define MODULE_FUNCTION(fun) { FixedString{#fun}, LuaWrapFunction(&fun), DoConstructFunctionSignature(&fun) },
#define MODULE_NAMED_FUNCTION(name, fun){ FixedString{name}, LuaWrapFunction(&fun), DoConstructFunctionSignature(&fun) },
#define END_MODULE() }; \
	gModuleRegistry.RegisterModule(mod); \
}

END_SE()
