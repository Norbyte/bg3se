#include <Extender/ScriptExtender.h>
#include <Extender/Shared/ScriptHelpers.h>

/// <lua_module>IO</lua_module>
BEGIN_NS(lua::io)

PathRootType ContextToRoot(lua_State* L, std::optional<FixedString> context)
{
    if (!context || *context == GFS.struser) {
        return PathRootType::UserProfile;
    } else if (context == GFS.strdata) {
        return PathRootType::Data;
    } else {
        luaL_error(L, "Unknown file loading context: %s", context->GetString());
        return PathRootType::Data;
    }
}

std::optional<STDString> LoadFile(lua_State* L, char const* path, std::optional<FixedString> context)
{
    OPTICK_EVENT();
    return script::LoadExternalFile(path, ContextToRoot(L, context));
}

bool SaveFile(char const* path, StringView contents)
{
    OPTICK_EVENT();
    return script::SaveExternalFile(path, PathRootType::UserProfile, contents);
}

bool AppendFile(char const* path, StringView contents)
{
    OPTICK_EVENT();
    return script::SaveExternalFile(path, PathRootType::UserProfile, contents, true);
}

Array<STDString> FindFiles(lua_State* L, StringView path, StringView glob, std::optional<FixedString> context, 
    std::optional<bool> recursive, std::optional<bool> checkPackedFiles)
{
    OPTICK_EVENT();
    return script::FindFiles(path, glob, ContextToRoot(L, context), recursive.value_or(false), checkPackedFiles.value_or(true));
}

void AddPathOverride(char const* path, char const* overridePath, std::optional<FixedString> context)
{
    gExtender->AddPathOverride(path, overridePath);
}

std::optional<STDString> GetPathOverride(char const* path)
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
