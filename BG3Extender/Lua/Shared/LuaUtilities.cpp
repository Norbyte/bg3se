#include <stdafx.h>
#include <OsirisProxy.h>
#include <PropertyMaps.h>
#include <Version.h>
#include <ScriptHelpers.h>
#include <Lua/LuaSerializers.h>

#include <fstream>
#include <json/json.h>

#include <Lua/Shared/LuaJson.inl>
#include <Lua/Shared/LuaLocalization.inl>

namespace bg3se::lua::utils
{
	void ArgsToStream(lua_State * L, std::stringstream & ss)
	{
		int nargs = lua_gettop(L);  /* number of arguments */

		lua_getglobal(L, "tostring");
		for (int i = 1; i <= nargs; i++) {
			lua_pushvalue(L, -1);  /* function to be called */
			lua_pushvalue(L, i);   /* value to print */
			lua_call(L, 1, 1);
			const char * str = lua_tostring(L, -1);  /* get result */
			if (str == nullptr)
				luaL_error(L, "'tostring' must return a string to 'print'");
			if (i > 1) ss << "\t";
			ss << str;
			lua_pop(L, 1);  /* pop result */
		}
	}

	int GetExtensionVersion(lua_State* L)
	{
		return CurrentVersion;
	}

	WrapLuaFunction(GetExtensionVersion)

	std::optional<STDString> GetGameVersion(lua_State* L)
	{
		GameVersionInfo gameVersion;
		if (gOsirisProxy->GetLibraryManager().GetGameVersion(gameVersion)) {
			std::stringstream ss;
			ss << "v" << gameVersion.Major << "." << gameVersion.Minor << "." << gameVersion.Revision << "." << gameVersion.Build;
			return ss.str().c_str();
		} else {
			return {};
		}
	}

	WrapLuaFunction(GetGameVersion)

	int MonotonicTime(lua_State* L)
	{
		using namespace std::chrono;
		return (int)time_point_cast<milliseconds>(steady_clock::now()).time_since_epoch().count();
	}

	WrapLuaFunction(MonotonicTime)

	int Include(lua_State * L)
	{
		auto modGuid = luaL_checkstring(L, 1);
		auto fileName = luaL_checkstring(L, 2);

		bool replaceGlobals = !lua_isnil(L, 3);
		auto globalsIdx = lua_gettop(L) + 1;

		if (replaceGlobals) {
			luaL_checktype(L, 3, LUA_TTABLE);
#if LUA_VERSION_NUM > 501
			lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
			lua_pushvalue(L, 3);
			lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
#endif
		}

		auto nret = gOsirisProxy->GetCurrentExtensionState()
			->LuaLoadModScript(modGuid, fileName, true, replaceGlobals ? 3 : 0);

		if (replaceGlobals) {
#if LUA_VERSION_NUM > 501
			lua_pushvalue(L, globalsIdx);
			lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
			lua_remove(L, globalsIdx);
#endif
		}

		if (nret) {
			return *nret;
		} else {
			return 0;
		}
	}

	int Print(lua_State* L)
	{
		std::stringstream ss;
		ArgsToStream(L, ss);
		gOsirisProxy->LogOsirisMsg(ss.str());
		return 0;
	}

	int PrintWarning(lua_State* L)
	{
		std::stringstream ss;
		ArgsToStream(L, ss);
		gOsirisProxy->LogOsirisWarning(ss.str());
		return 0;
	}

	int PrintError(lua_State* L)
	{
		std::stringstream ss;
		ArgsToStream(L, ss);
		gOsirisProxy->LogLuaError(ss.str());
		return 0;
	}

	std::optional<STDString> LoadFile(lua_State* L, char const* path, std::optional<char const*> context)
	{
		if (!context || strcmp(*context, "user") == 0) {
			return script::LoadExternalFile(path, PathRootType::GameStorage);
		} else if (strcmp(*context, "data") == 0) {
			return script::LoadExternalFile(path, PathRootType::Data);
		} else {
			LuaError("Unknown file loading context: " << *context);
			return {};
		}
	}

	WrapLuaFunction(LoadFile)

	int SaveFile(lua_State* L, char const* path, char const* contents)
	{
		return script::SaveExternalFile(path, PathRootType::GameStorage, contents);
	}

	WrapLuaFunction(SaveFile)

	bool IsModLoaded(lua_State* L, char const* modNameGuid)
	{
		auto modUuid = NameGuidToFixedString(modNameGuid);
		if (modUuid) {
			auto modManager = gOsirisProxy->GetCurrentExtensionState()->GetModManager();
			for (auto const& mod : modManager->BaseModule.LoadOrderedModules) {
				if (mod.Info.ModuleUUIDString == modUuid) {
					return true;
				}
			}
		}

		return false;
	}

	WrapLuaFunction(IsModLoaded)

	int GetModLoadOrder(lua_State* L)
	{
		StackCheck _(L, 1);
		lua_newtable(L);

		auto modManager = gOsirisProxy->GetCurrentExtensionState()->GetModManager();
		auto & mods = modManager->BaseModule.LoadOrderedModules;
		for (uint32_t i = 0; i < mods.Size; i++) {
			auto const & mod = mods[i];
			settable(L, i + 1, mod.Info.ModuleUUIDString.GetString());
		}

		return 1;
	}

	int GetModInfo(lua_State* L)
	{
		StackCheck _(L, 1);
		Module const * module{ nullptr };
		auto modUuid = NameGuidToFixedString(luaL_checkstring(L, 1));
		if (modUuid) {
			auto modManager = gOsirisProxy->GetCurrentExtensionState()->GetModManager();
			for (auto const& mod : modManager->BaseModule.LoadOrderedModules) {
				if (mod.Info.ModuleUUIDString == modUuid) {
					module = &mod;
					break;
				}
			}
		}

		if (module != nullptr) {
			lua_newtable(L);
			setfield(L, "UUID", module->Info.ModuleUUIDString);
			setfield(L, "Name", module->Info.Name);
			setfield(L, "Version", module->Info.ModVersion.Ver);
			setfield(L, "PublishVersion", module->Info.PublishVersion.Ver);
			setfield(L, "Directory", module->Info.Directory);
			setfield(L, "Author", module->Info.Author);
			setfield(L, "Description", module->Info.Description);
			setfield(L, "ModuleType", module->Info.ModuleType);
			
			lua_newtable(L);
			auto & dependents = module->DependentModules;
			for (uint32_t i = 0; i < dependents.Size; i++) {
				auto const & mod = dependents[i];
				settable(L, i + 1, mod.Info.ModuleUUIDString);
			}
			lua_setfield(L, -2, "Dependencies");
		} else {
			push(L, nullptr);
		}

		return 1;
	}

	void DebugBreak(lua_State* L)
	{
#if !defined(OSI_NO_DEBUGGER)
		auto debugger = gOsirisProxy->GetLuaDebugger();
		if (debugger) {
			debugger->DebugBreak(L);
		}
#endif
	}

	WrapLuaFunction(DebugBreak)


	/*int NewDamageList(lua_State * L)
	{
		DamageList::New(L);
		return 1;
	}

	int GetSurfaceTemplate(lua_State* L)
	{
		auto surfaceType = checked_get<SurfaceType>(L, 1);
		auto tmpl = GetStaticSymbols().GetSurfaceTemplate(surfaceType);
		if (tmpl != nullptr) {
			ObjectProxy<SurfaceTemplate>::New(L, tmpl);
		} else {
			LuaError("Couldn't fetch surface template of type " << (unsigned)surfaceType);
			push(L, nullptr);
		}
		return 1;
	}*/

	bool IsDeveloperMode(lua_State * L)
	{
		return gOsirisProxy->GetConfig().DeveloperMode;
	}

	WrapLuaFunction(IsDeveloperMode)

	void AddPathOverride(lua_State * L, char const* path, char const* overridePath)
	{
		gOsirisProxy->AddPathOverride(path, overridePath);
	}

	WrapLuaFunction(AddPathOverride)

	// Variation of Lua builtin math_random() with custom RNG
	int LuaRandom(lua_State *L)
	{
		auto state = gOsirisProxy->GetCurrentExtensionState();

		lua_Integer low, up;
		switch (lua_gettop(L)) {  /* check number of arguments */
		case 0: {  /* no arguments */
			std::uniform_real_distribution<double> dist(0.0, 1.0);
			push(L, (lua_Number)dist(state->OsiRng));  /* Number between 0 and 1 */
			return 1;
		}
		case 1: {  /* only upper limit */
			low = 1;
			up = luaL_checkinteger(L, 1);
			break;
		}
		case 2: {  /* lower and upper limits */
			low = luaL_checkinteger(L, 1);
			up = luaL_checkinteger(L, 2);
			break;
		}
		default: return luaL_error(L, "wrong number of arguments");
		}
		/* random integer in the interval [low, up] */
		luaL_argcheck(L, low <= up, 1, "interval is empty");
#if LUA_VERSION_NUM > 501
		luaL_argcheck(L, low >= 0 || up <= LUA_MAXINTEGER + low, 1,
			"interval too large");
#endif

		std::uniform_int_distribution<int64_t> dist(low, up);
		push(L, dist(state->OsiRng));
		return 1;
	}

	int64_t LuaRound(lua_State *L, double val)
	{
		return (int64_t)round(val);
	}

	WrapLuaFunction(LuaRound)

	char const * OsiToLuaTypeName(ValueType type)
	{
		auto typeMap = *gOsirisProxy->GetWrappers().Globals.Types;
		auto typeId = typeMap->ResolveAlias((uint32_t)type);

		switch (typeId) {
		case ValueType::Integer:
		case ValueType::Integer64:
			return "integer";

		case ValueType::Real:
			return "number";

		case ValueType::String:
			return "string";

		case ValueType::GuidString:
			return "string GUID";

		default:
			return "any";
		}
	}

	STDString GenerateIdeHelpers(bool builtinOnly)
	{
		STDString helpers;
		helpers.reserve(0x20000);

		STDString functionComment, functionDefn;

		auto functions = gOsirisProxy->GetGlobals().Functions;

		(*functions)->Iterate([&helpers, &functionComment, &functionDefn, builtinOnly](STDString const & key, Function const * func) {
			if (builtinOnly
				&& func->Type != FunctionType::Event
				&& func->Type != FunctionType::Call
				&& func->Type != FunctionType::Query
				&& func->Type != FunctionType::SysCall
				&& func->Type != FunctionType::SysQuery) {
				return;
			}

			functionComment.clear();
			functionDefn.clear();
			auto const & outParams = func->Signature->OutParamList;
			auto numOutParams = outParams.numOutParams();

			auto types = func->Signature->Params->Params.Head;
			for (auto i = 0; i < func->Signature->Params->Params.Size; i++) {
				types = types->Next;

				if (!outParams.isOutParam(i)) {
					functionComment += "--- @param arg";
					functionComment += std::to_string(i + 1);
					functionComment += " ";
					functionComment += OsiToLuaTypeName((ValueType)types->Item.Type);
					functionComment += "\r\n";
				}
			}

			if (numOutParams > 0) {
				auto types = func->Signature->Params->Params.Head;
				for (auto i = 0; i < func->Signature->Params->Params.Size; i++) {
					types = types->Next;

					if (outParams.isOutParam(i)) {
						functionComment += "--- @return ";
						functionComment += OsiToLuaTypeName((ValueType)types->Item.Type);
						functionComment += "\r\n";
					}
				}
			} else {
				if (func->Type == FunctionType::SysQuery
					|| func->Type == FunctionType::Query
					|| func->Type == FunctionType::UserQuery) {
					functionComment += "--- @return boolean Did the query succeed?\r\n";
				}
			}

			functionDefn += func->Signature->Name;
			functionDefn += " = function (";

			auto pendingInParams = func->Signature->Params->Params.Size - numOutParams;
			for (auto i = 0; i < func->Signature->Params->Params.Size; i++) {
				if (!outParams.isOutParam(i)) {
					functionDefn += "arg";
					functionDefn += std::to_string(i + 1);
					if (--pendingInParams > 0) {
						functionDefn += ", ";
					}
				}
			}

			functionDefn += ") end\r\n\r\n";

			helpers += functionComment;
			helpers += "Osi.";
			helpers += functionDefn;

			// Export global name if function is a builtin
			if (func->Type == FunctionType::Event
				|| func->Type == FunctionType::Call
				|| func->Type == FunctionType::Query
				|| func->Type == FunctionType::SysCall
				|| func->Type == FunctionType::SysQuery) {
				helpers += functionComment;
				helpers += functionDefn;
			}
		});

		return helpers;
	}

	void GenerateIdeHelpers(lua_State * L, std::optional<bool> builtinOnly)
	{
#if defined(OSI_EOCAPP)
		if (gOsirisProxy->GetConfig().DeveloperMode) {
#endif
			esv::LuaServerPin lua(esv::ExtensionState::Get());
			if (lua->RestrictionFlags & State::RestrictOsiris) {
				luaL_error(L, "GenerateIdeHelpers() can only be called when Osiris is available");
			}

			auto helpers = GenerateIdeHelpers(builtinOnly && *builtinOnly);

			auto path = GetStaticSymbols().ToPath("", PathRootType::Data);
			path += "Mods/";
			//path += ToUTF8(GetModManagerServer()->BaseModule.Info.Directory);
			path += "/Story/RawFiles/Lua/OsiIdeHelpers.lua";

			std::ofstream f(path.c_str(), std::ios::out | std::ios::binary);
			if (!f.good()) {
				OsiError("Could not open file to save IDE helpers: '" << path << "'");
				return;
			}

			f.write(helpers.c_str(), helpers.size());
#if defined(OSI_EOCAPP)
		} else {
			OsiError("GenerateIdeHelpers() only supported in developer mode");
		}
#endif
	}

	WrapLuaFunction(GenerateIdeHelpers)

	void DumpStack(lua_State* L)
	{
		auto top = lua_gettop(L);
		for (int idx = 1; idx <= top; idx++) {
			switch (lua_type(L, idx)) {
			case LUA_TNIL:
				INFO("<%d> nil");
				break;

			case LUA_TBOOLEAN:
				INFO("<%d> %s", idx, lua_toboolean(L, idx) ? "true" : "false");
				break;

			case LUA_TLIGHTUSERDATA:
				INFO("<%d> Handle %ull", idx, (uint64_t)lua_touserdata(L, idx));
				break;

			case LUA_TNUMBER:
				INFO("<%d> %f", idx, (float)lua_tonumber(L, idx));
				break;

			case LUA_TSTRING:
				INFO("<%d> String '%s'", idx, lua_tostring(L, idx));
				break;

			default:
				INFO("<%d> %s", idx, lua_typename(L, lua_type(L, idx)));
				break;
			}
		}
	}

	WrapLuaFunction(DumpStack)


	template <class T>
	int GetResourceProxy(lua_State* L, UUID const& resourceGuid)
	{
		auto helpers = gOsirisProxy->GetServerEntityHelpers();
		auto resourceMgr = helpers.GetResourceManager<T>();
		if (!resourceMgr) {
			LuaError("Resource manager not available for this resource type");
			push(L, nullptr);
			return 1;
		}

		auto resource = (*resourceMgr)->Resources.Find(resourceGuid);
		if (resource) {
			ObjectProxy2<T>::New(L, *resource);
		} else {
			push(L, nullptr);
		}

		return 1;
	}

	int GetResource(lua_State* L)
	{
		auto resourceGuid = checked_get<UUID>(L, 1);
		auto type = checked_get<ExtResourceManagerType>(L, 2);

		auto helpers = gOsirisProxy->GetServerEntityHelpers();
		switch (type) {
		case ActionResource::ResourceManagerType: return GetResourceProxy<ActionResource>(L, resourceGuid);
		case ClassDescriptionResource::ResourceManagerType: return GetResourceProxy<ClassDescriptionResource>(L, resourceGuid);
		case TagResource::ResourceManagerType: return GetResourceProxy<TagResource>(L, resourceGuid);
		case FactionResource::ResourceManagerType: return GetResourceProxy<FactionResource>(L, resourceGuid);
		case RaceResource::ResourceManagerType: return GetResourceProxy<RaceResource>(L, resourceGuid);
		case OriginResource::ResourceManagerType: return GetResourceProxy<OriginResource>(L, resourceGuid);
		case BackgroundResource::ResourceManagerType: return GetResourceProxy<BackgroundResource>(L, resourceGuid);
		case GodResource::ResourceManagerType: return GetResourceProxy<GodResource>(L, resourceGuid);
		case ProgressionResource::ResourceManagerType: return GetResourceProxy<ProgressionResource>(L, resourceGuid);

		default:
			LuaError("Resource type not supported: " << EnumInfo<ExtResourceManagerType>::Find(type));
			push(L, nullptr);
			return 1;
		}
	}

	void RegisterUtilsLib(lua_State* L)
	{
		static const luaL_Reg utilsLib[] = {
			{"Version", GetExtensionVersionWrapper},
			{"GameVersion", GetGameVersionWrapper},
			{"MonotonicTime", MonotonicTimeWrapper},
			{"Include", Include},
			{"Print", Print},
			{"PrintWarning", PrintWarning},
			{"PrintError", PrintError},
			{"DebugBreak", DebugBreakWrapper},

			{"IsModLoaded", IsModLoadedWrapper},
			{"GetModLoadOrder", GetModLoadOrder},
			{"GetModInfo", GetModInfo},

			{"IsDeveloperMode", IsDeveloperModeWrapper},
			{"GenerateIdeHelpers", GenerateIdeHelpersWrapper},

			// FIXME - move somewhere else later!
			{"GetResource", GetResource},

			// EXPERIMENTAL FUNCTIONS
			{"DumpStack", DumpStackWrapper},
			//{"DumpNetworking", DumpNetworking},

			//{"GetGameState", GetGameState},
			{"AddPathOverride", AddPathOverrideWrapper},
			{0,0}
		};

		lua_getglobal(L, "Ext"); // stack: Ext
		luaL_newlib(L, utilsLib); // stack: ext, lib
		lua_setfield(L, -2, "Utils");
		lua_pop(L, 1);
	}

	void RegisterIOLib(lua_State* L)
	{
		static const luaL_Reg ioLib[] = {
			{"SaveFile", SaveFileWrapper},
			{"LoadFile", LoadFileWrapper},
			{0,0}
		};

		lua_getglobal(L, "Ext"); // stack: Ext
		luaL_newlib(L, ioLib); // stack: ext, lib
		lua_setfield(L, -2, "IO");
		lua_pop(L, 1);
	}

	void RegisterMathLib(lua_State * L)
	{
		static const luaL_Reg mathLib[] = {
			{"Random", LuaRandom},
			{"Round", LuaRoundWrapper},
			{0,0}
		};

		lua_getglobal(L, "Ext"); // stack: Ext
		luaL_newlib(L, mathLib); // stack: ext, lib
		lua_setfield(L, -2, "Math");
		lua_pop(L, 1);
	}
}
