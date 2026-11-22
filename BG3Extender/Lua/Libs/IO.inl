#include <Extender/ScriptExtender.h>

/// <lua_module>IO</lua_module>
BEGIN_NS(lua::io)

UserPathRootType ContextToRoot(lua_State* L, std::optional<FixedString> context)
{
    if (!context || *context == GFS.struser) {
        return UserPathRootType::UserProfileScriptExtender;
    } else if (context == GFS.strdata) {
        return UserPathRootType::Data;
    } else {
        luaL_error(L, "Unknown file loading context: %s", context->GetString());
        return UserPathRootType::Data;
    }
}

std::optional<STDString> LoadFile(lua_State* L, StringView path, std::optional<FixedString> context)
{
    OPTICK_EVENT();
    return LoadExternalFile(path, ContextToRoot(L, context));
}

bool SaveFile(StringView path, StringView contents)
{
    OPTICK_EVENT();
    return SaveExternalFile(path, UserPathRootType::UserProfileScriptExtender, contents);
}

bool AppendFile(StringView path, StringView contents)
{
    OPTICK_EVENT();
    return SaveExternalFile(path, UserPathRootType::UserProfileScriptExtender, contents, true);
}

Array<STDString> FindFiles(lua_State* L, StringView path, StringView glob, std::optional<FixedString> context, 
    std::optional<bool> recursive, std::optional<bool> checkPackedFiles)
{
    OPTICK_EVENT();
    return FindFiles(path, glob, ContextToRoot(L, context), recursive.value_or(false), checkPackedFiles.value_or(true));
}

bool AddPathOverride(StringView path, StringView overridePath, std::optional<FixedString> context)
{
    return gExtender->AddPathOverride(path, overridePath);
}

std::optional<STDString> GetPathOverride(StringView path)
{
    return gExtender->GetPathOverride(path);
}

void RegisterIOLib()
{
    DECLARE_MODULE(IO, Both)
    BEGIN_MODULE()
    MODULE_FUNCTION(LoadFile)
    MODULE_FUNCTION(SaveFile)
    MODULE_FUNCTION(AppendFile)
    MODULE_FUNCTION(FindFiles)
    MODULE_FUNCTION(AddPathOverride)
    MODULE_FUNCTION(GetPathOverride)
    END_MODULE()
}

END_NS()
