#include <Lua/Shared/LuaModule.h>

BEGIN_NS(lua)

ModuleRegistry gModuleRegistry;

void ModuleRegistry::RegisterModule(ModuleDefinition const& module)
{
	modules_.push_back(module);
}

void ModuleRegistry::ConstructState(lua_State* L, ModuleRole role)
{
	for (auto const& module : modules_) {
		if (role == module.Role || module.Role == ModuleRole::Both) {
			InstantiateModule(L, module);
		}
	}
}

void ModuleRegistry::RegisterTypeInformation()
{
	assert(!modules_.empty());
	for (auto const& module : modules_) {
		RegisterModuleTypeInformation(module);
	}
}

void ModuleRegistry::RegisterModuleTypeInformation(ModuleDefinition const& module)
{
	STDString name;
	name = module.Table.GetString();
	if (module.SubTable) {
		name += ".";
		name += module.SubTable.GetString();
	}

	STDString exportName = "Module_";
	switch (module.Role) {
	case ModuleRole::Both: break;
	case ModuleRole::Client: exportName += "Client"; break;
	case ModuleRole::Server: exportName += "Server"; break;
	}

	exportName += name;

	TypeInformation& mod = TypeInformationRepository::GetInstance().RegisterType(FixedString{ exportName });
	mod.Kind = LuaTypeId::Module;
	mod.NativeName = FixedString(name);
	switch (module.Role) {
	case ModuleRole::Both: mod.ModuleRole = FixedString("Both"); break;
	case ModuleRole::Client: mod.ModuleRole = FixedString("Client"); break;
	case ModuleRole::Server: mod.ModuleRole = FixedString("Server"); break;
	}

	for (auto const& func : module.Functions) {
		mod.Methods.insert(std::make_pair(func.Name, func.Signature));
	}
}

void ModuleRegistry::MakeLuaFunctionTable(ModuleDefinition const& module, std::vector<luaL_Reg>& lib)
{
	lib.reserve(module.Functions.size() + 1);
	for (auto const& fun : module.Functions) {
		lib.push_back({ fun.Name.GetString(), fun.Func });
	}
	lib.push_back({ nullptr, nullptr });
}

void ModuleRegistry::InstantiateNamedModule(lua_State* L, char const* name, ModuleDefinition const& module)
{
	std::vector<luaL_Reg> lib;
	MakeLuaFunctionTable(module, lib);

	lua_getglobal(L, "Ext"); // stack: Ext
	lua_createtable(L, 0, 0); // stack: ext, lib
	luaL_setfuncs(L, lib.data(), 0);
	lua_setfield(L, -2, name);
	lua_pop(L, 1);
}

void ModuleRegistry::InstantiateNamedModule(lua_State* L, char const* name, char const* subTableName, ModuleDefinition const& module)
{
	std::vector<luaL_Reg> lib;
	MakeLuaFunctionTable(module, lib);

	lua_getglobal(L, "Ext"); // stack: Ext
	lua_getfield(L, -1, name); // stack: Ext, parent
	lua_createtable(L, 0, 0); // stack: ext, parent, lib
	luaL_setfuncs(L, lib.data(), 0);
	lua_setfield(L, -2, subTableName);
	lua_pop(L, 2);
}

void ModuleRegistry::InstantiateModule(lua_State* L, char const* prefix, ModuleDefinition const& module)
{
	if (!module.SubTable) {
		InstantiateNamedModule(L, (STDString(prefix) + module.Table.GetString()).c_str(), module);
	} else {
		InstantiateNamedModule(L, (STDString(prefix) + module.Table.GetString()).c_str(), module.SubTable.GetString(), module);
	}
}

void ModuleRegistry::InstantiateModule(lua_State* L, ModuleDefinition const& module)
{
	InstantiateModule(L, "", module);

	if (module.Role == ModuleRole::Both || module.Role == ModuleRole::Server) {
		InstantiateModule(L, "Server", module);
	}

	if (module.Role == ModuleRole::Both || module.Role == ModuleRole::Client) {
		InstantiateModule(L, "Client", module);
	}
}

END_NS()
