#pragma once

#include <GameDefinitions/Base/Base.h>
#include <Lua/LuaHelpers.h>

BEGIN_NS(lua)

struct ModuleFunction
{
    FixedString Name;
    lua_CFunction Func;
    TypeInformation Signature;
};

enum class ModuleRole
{
    Client,
    Server,
    Both
};

struct ModuleDefinition
{
    ModuleRole Role;
    FixedString Table;
    FixedString SubTable;
    std::vector<ModuleFunction> Functions;
};

class ModuleRegistry
{
public:
    void RegisterModule(ModuleDefinition const& module);
    void ConstructState(lua_State* L, ModuleRole role);
    void RegisterTypeInformation();

    inline std::vector<ModuleDefinition> const& GetModules() const
    {
        return modules_;
    }

private:
    std::vector<ModuleDefinition> modules_;

    void MakeLuaFunctionTable(ModuleDefinition const& module, std::vector<luaL_Reg>& lib);
    void InstantiateModule(lua_State* L, ModuleDefinition const& module);
    void InstantiateModule(lua_State* L, char const* prefix, ModuleDefinition const& module);
    void InstantiateNamedModule(lua_State* L, char const* name, ModuleDefinition const& module);
    void InstantiateNamedModule(lua_State* L, char const* name, char const* subTableName, ModuleDefinition const& module);
    void RegisterModuleTypeInformation(ModuleDefinition const& module);
};

extern ModuleRegistry gModuleRegistry;

END_NS()
