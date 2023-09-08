#include <Extender/ScriptExtender.h>
#include <Extender/Shared/ScriptHelpers.h>

/// <lua_module>IO</lua_module>
BEGIN_NS(lua::io)

std::optional<STDString> LoadFile(char const* path, std::optional<FixedString> context)
{
	if (!context || *context == GFS.struser) {
		return script::LoadExternalFile(path, PathRootType::UserProfile);
	} else if (context == GFS.strdata) {
		return script::LoadExternalFile(path, PathRootType::Data);
	} else {
		LuaError("Unknown file loading context: " << context->GetString());
		return {};
	}
}

bool SaveFile(char const* path, char const* contents)
{
	return script::SaveExternalFile(path, PathRootType::UserProfile, contents);
}

void AddPathOverride(char const* path, char const* overridePath)
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
	MODULE_FUNCTION(AddPathOverride)
	MODULE_FUNCTION(GetPathOverride)
	END_MODULE()
}

END_NS()
