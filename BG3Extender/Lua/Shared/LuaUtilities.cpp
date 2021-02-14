#include <stdafx.h>
#include <OsirisProxy.h>
#include <PropertyMaps.h>
#include <Version.h>
#include <ScriptHelpers.h>
#include <Lua/LuaSerializers.h>

#include <fstream>
#include <json/json.h>

namespace bg3se::lua::utils
{
	void JsonParse(lua_State * L, Json::Value & val);

	void JsonParseArray(lua_State * L, Json::Value & val)
	{
		lua_newtable(L);
		int idx = 1;
		for (auto it = val.begin(), end = val.end(); it != end; ++it) {
			push(L, idx++);
			JsonParse(L, *it);
			lua_settable(L, -3);
		}
	}

	void JsonParseObject(lua_State * L, Json::Value & val)
	{
		lua_newtable(L);
		for (auto it = val.begin(), end = val.end(); it != end; ++it) {
			JsonParse(L, it.key());
			JsonParse(L, *it);
			lua_settable(L, -3);
		}
	}

	void JsonParse(lua_State * L, Json::Value & val)
	{
		switch (val.type()) {
		case Json::nullValue:
			lua_pushnil(L);
			break;

		case Json::intValue:
			push(L, val.asInt64());
			break;

		case Json::uintValue:
			push(L, (int64_t)val.asUInt64());
			break;

		case Json::realValue:
			push(L, val.asDouble());
			break;

		case Json::stringValue:
			push(L, val.asCString());
			break;

		case Json::booleanValue:
			push(L, val.asBool());
			break;

		case Json::arrayValue:
			JsonParseArray(L, val);
			break;

		case Json::objectValue:
			JsonParseObject(L, val);
			break;

		default:
			luaL_error(L, "Attempted to parse unknown Json value");
		}
	}


	int JsonParse(lua_State * L)
	{
		StackCheck _(L, 1);
		size_t length;
		auto json = luaL_checklstring(L, 1, &length);

		Json::CharReaderBuilder factory;
		std::unique_ptr<Json::CharReader> reader(factory.newCharReader());

		Json::Value root;
		std::string errs;
		if (!reader->parse(json, json + length, &root, &errs)) {
			return luaL_error(L, "Unable to parse JSON: %s", errs.c_str());
		}

		JsonParse(L, root);
		return 1;
	}

	Json::Value JsonStringify(lua_State * L, int index, int depth, bool stringifyInternalTypes);

	Json::Value JsonStringifyObject(lua_State * L, int index, int depth, bool stringifyInternalTypes)
	{
		Json::Value arr(Json::objectValue);
		lua_pushnil(L);

		if (index < 0) index--;

		while (lua_next(L, index) != 0) {
			Json::Value val(JsonStringify(L, -1, depth + 1, stringifyInternalTypes));

			if (lua_type(L, -2) == LUA_TSTRING) {
				auto key = lua_tostring(L, -2);
				arr[key] = val;
			} else if (lua_type(L, -2) == LUA_TNUMBER) {
				lua_pushvalue(L, -2);
				auto key = lua_tostring(L, -1);
				arr[key] = val;
				lua_pop(L, 1);
			} else {
				throw std::runtime_error("Can only stringify string or number table keys");
			}

			lua_pop(L, 1);
		}

		return arr;
	}

	Json::Value JsonStringifyArray(lua_State * L, int index, int depth, bool stringifyInternalTypes)
	{
		Json::Value arr(Json::arrayValue);
		lua_pushnil(L);

		if (index < 0) index--;

		while (lua_next(L, index) != 0) {
			arr.append(JsonStringify(L, -1, depth + 1, stringifyInternalTypes));
			lua_pop(L, 1);
		}

		return arr;
	}

	bool JsonCanStringifyAsArray(lua_State * L, int index)
	{
		Json::Value arr(Json::objectValue);
		lua_pushnil(L);

		if (index < 0) index--;

		int next = 1;
		bool isArray = true;
		while (lua_next(L, index) != 0) {
#if LUA_VERSION_NUM > 501
			if (lua_isinteger(L, -2)) {
				auto key = lua_tointeger(L, -2);
				if (key != next++) {
					isArray = false;
				}
			} else {
				isArray = false;
			}
#else
			if (lua_isnumber(L, -2)) {
				auto key = lua_tonumber(L, -2);
				if (abs(key - next++) < 0.0001) {
					isArray = false;
				}
			} else {
				isArray = false;
			}
#endif

			lua_pop(L, 1);
		}

		return isArray;
	}

	Json::Value JsonStringifyTable(lua_State * L, int index, int depth, bool stringifyInternalTypes)
	{
		if (JsonCanStringifyAsArray(L, index)) {
			return JsonStringifyArray(L, index, depth, stringifyInternalTypes);
		} else {
			return JsonStringifyObject(L, index, depth, stringifyInternalTypes);
		}
	}


	Json::Value JsonStringify(lua_State * L, int index, int depth, bool stringifyInternalTypes)
	{
		if (depth > 64) {
			throw std::runtime_error("Recursion depth exceeded while stringifying JSON");
		}

		switch (lua_type(L, index)) {
		case LUA_TNIL:
			return Json::Value(Json::nullValue);

		case LUA_TBOOLEAN:
			return Json::Value(lua_toboolean(L, index) == 1);

		case LUA_TNUMBER:
#if LUA_VERSION_NUM > 501
			if (lua_isinteger(L, index)) {
				return Json::Value(lua_tointeger(L, index));
			} else {
				return Json::Value(lua_tonumber(L, index));
			}
#else
			return Json::Value(lua_tonumber(L, index));
#endif

		case LUA_TSTRING:
			return Json::Value(lua_tostring(L, index));

		case LUA_TTABLE:
			return JsonStringifyTable(L, index, depth, stringifyInternalTypes);

		case LUA_TLIGHTUSERDATA:
		case LUA_TFUNCTION:
		case LUA_TUSERDATA:
		case LUA_TTHREAD:
			if (stringifyInternalTypes) {
				auto val = Json::Value(luaL_tolstring(L, index, NULL));
				lua_pop(L, 1);
				return val;
			} else {
				throw std::runtime_error("Attempted to stringify a lightuserdata, userdata, function or thread value");
			}

		default:
			throw std::runtime_error("Attempted to stringify an unknown type");
		}
	}


	int JsonStringify(lua_State * L)
	{
		StackCheck _(L, 1);
		int nargs = lua_gettop(L);
		if (nargs < 1) {
			return luaL_error(L, "JsonStringify expects at least one parameter.");
		}

		if (nargs > 3) {
			return luaL_error(L, "JsonStringify expects at most three parameters.");
		}

		bool beautify{ true };
		if (nargs >= 2) {
			beautify = lua_toboolean(L, 2) == 1;
		}

		bool stringifyInternalTypes{ false };
		if (nargs >= 3) {
			stringifyInternalTypes = lua_toboolean(L, 3) == 1;
		}

		Json::Value root;
		try {
			root = JsonStringify(L, 1, 0, stringifyInternalTypes);
		} catch (std::runtime_error & e) {
			return luaL_error(L, "%s", e.what());
		}

		Json::StreamWriterBuilder builder;
		if (beautify) {
			builder["indentation"] = "\t";
		}
		std::stringstream ss;
		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		writer->write(root, &ss);

		push(L, ss.str());
		return 1;
	}


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

	void AddVoiceMetaData(lua_State * L, char const* speakerGuid, char const* translatedStringKey, char const* source, 
		float length, std::optional<int> priority)
	{
		/*auto speakerMgr = GetStaticSymbols().eoc__SpeakerManager;
		if (speakerMgr == nullptr || *speakerMgr == nullptr) {
			OsiError("Speaker manager not initialized!");
			return;
		}

		auto speaker = (*speakerMgr)->SpeakerMetaDataHashMap->Insert(MakeFixedString(speakerGuid));
		auto voiceMeta = speaker->Insert(MakeFixedString(translatedStringKey));
		voiceMeta->CodecID = 4;
		voiceMeta->IsRecorded = true;
		voiceMeta->Length = (float)length;
		voiceMeta->Priority = priority ? *priority : 0;

		auto path = GetStaticSymbols().ToPath(source, PathRootType::Data);
		voiceMeta->Source.Name = path;*/
	}

	WrapLuaFunction(AddVoiceMetaData)

	STDString GetTranslatedString(lua_State* L, char const* translatedStringKey, std::optional<char const*> fallbackText)
	{
		STDWString translated;
		if (script::GetTranslatedString(translatedStringKey, translated)) {
			return ToUTF8(translated);
		} else {
			return fallbackText ? *fallbackText : "";
		}
	}

	WrapLuaFunction(GetTranslatedString)

	std::tuple<STDString, FixedString> GetTranslatedStringFromKey(lua_State* L, FixedString key)
	{
		/*TranslatedString translated;
		if (script::GetTranslatedStringFromKey(key, translated)) {
			return { ToUTF8(translated.Handle.ReferenceString), translated.Handle.Handle };
		} else {*/
			return {};
		//}
	}

	WrapLuaFunction(GetTranslatedStringFromKey)

	unsigned NextDynamicStringHandleId{ 1 };

	std::optional<STDString> CreateTranslatedString(lua_State* L, char const* keyStr, char const* value)
	{
		auto key = FixedString(keyStr);

		STDString handleStr = "ExtStr_";
		handleStr += std::to_string(NextDynamicStringHandleId++);
		FixedString handle(handleStr.c_str());

		if (script::CreateTranslatedStringKey(key, handle)) {
			STDWString str(FromUTF8(value));
			if (script::CreateTranslatedString(handle, str)) {
				return handleStr;
			}
		}

		return {};
	}

	WrapLuaFunction(CreateTranslatedString)

	bool CreateTranslatedStringKey(lua_State* L, char const* keyStr, char const* handleStr)
	{
		FixedString key(keyStr);
		FixedString handle(handleStr);
		return script::CreateTranslatedStringKey(key, handle);
	}

	WrapLuaFunction(CreateTranslatedStringKey)

	bool CreateTranslatedStringHandle(lua_State* L, char const* handleStr, char const* value)
	{
		FixedString handle(handleStr);

		STDWString str(FromUTF8(value));
		return script::CreateTranslatedString(handle, str);
	}

	WrapLuaFunction(CreateTranslatedStringHandle)

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

			{"SaveFile", SaveFileWrapper},
			{"LoadFile", LoadFileWrapper},

			{"JsonParse", JsonParse},
			{"JsonStringify", JsonStringify},

			{"IsModLoaded", IsModLoadedWrapper},
			{"GetModLoadOrder", GetModLoadOrder},
			{"GetModInfo", GetModInfo},

			{"IsDeveloperMode", IsDeveloperModeWrapper},
			{"GenerateIdeHelpers", GenerateIdeHelpersWrapper},

			// EXPERIMENTAL FUNCTIONS
			{"DumpStack", DumpStackWrapper},
			//{"DumpNetworking", DumpNetworking},

			//{"GetGameState", GetGameState},
			{"AddPathOverride", AddPathOverrideWrapper},
			{"AddVoiceMetaData", AddVoiceMetaDataWrapper},
			{"GetTranslatedString", GetTranslatedStringWrapper},
			{"GetTranslatedStringFromKey", GetTranslatedStringFromKeyWrapper},
			{"CreateTranslatedString", CreateTranslatedStringWrapper},
			{"CreateTranslatedStringKey", CreateTranslatedStringKeyWrapper},
			{"CreateTranslatedStringHandle", CreateTranslatedStringHandleWrapper},
			{0,0}
		};

		lua_getglobal(L, "Ext"); // stack: Ext
		luaL_newlib(L, utilsLib); // stack: ext, lib
		lua_setfield(L, -2, "Utils");
		lua_pop(L, 1);
	}

	void RegisterLocalizationLib(lua_State * L)
	{
		static const luaL_Reg locaLib[] = {
			{"AddVoiceMetaData", AddVoiceMetaDataWrapper},
			{"GetTranslatedString", GetTranslatedStringWrapper},
			{"GetTranslatedStringFromKey", GetTranslatedStringFromKeyWrapper},
			{"CreateTranslatedString", CreateTranslatedStringWrapper},
			{"CreateTranslatedStringKey", CreateTranslatedStringKeyWrapper},
			{"CreateTranslatedStringHandle", CreateTranslatedStringHandleWrapper},
			{0,0}
		};

		lua_getglobal(L, "Ext"); // stack: Ext
		luaL_newlib(L, locaLib); // stack: ext, lib
		lua_setfield(L, -2, "Localization");
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
