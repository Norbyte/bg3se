#include <stdafx.h>
#include <Extender/ScriptExtender.h>
#include <Extender/Shared/ExtensionState.h>
#include <Version.h>
#include <fstream>
#include "json/json.h"

namespace bg3se
{
	std::unordered_set<std::string_view> ExtensionStateBase::sAllFeatureFlags = {
		"Osiris",
		"Lua",
		"Preprocessor"
	};

	char const* sContextNames[] = {
		"Uninitialized",
		"Game",
		"Load"
	};

	static_assert(std::size(sContextNames) == (unsigned)ExtensionStateContext::Max + 1);

	char const* ContextToString(ExtensionStateContext ctx)
	{
		return sContextNames[(unsigned)ctx];
	}

	ExtensionStateBase::ExtensionStateBase(bool isServer)
		/* : userVariables_(isServer),
		modVariables_(isServer)*/
	{}

	ExtensionStateBase::~ExtensionStateBase()
	{}

	void ExtensionStateBase::Reset()
	{
		time_t tm;
		OsiRng.seed(time(&tm));
	}

	void ExtensionStateBase::LoadConfigs()
	{
		auto modManager = GetModManager();
		if (modManager == nullptr) {
			OsiErrorS("Mod manager not available");
			return;
		}

		unsigned numConfigs{ 0 };
		for (auto const& mod : modManager->BaseModule.LoadOrderedModules) {
			auto dir = mod.Info.Directory;
			auto configFile = "Mods/" + dir + "/ScriptExtender/Config.json";
			auto reader = GetStaticSymbols().MakeFileReader(configFile);

			if (reader.IsLoaded()) {
				ExtensionModConfig config;
				if (LoadConfig(mod, reader.ToString(), config)) {
					std::stringstream featureFlags;
					for (auto const & flag : config.FeatureFlags) {
						featureFlags << flag << " ";
					}

					INFO("Configuration for '%s':\r\n\tMinVersion %d; Feature flags: %s", mod.Info.Name.c_str(),
						config.MinimumVersion, featureFlags.str());

					if (config.MinimumVersion == 0) {
						OsiError("Module '" << mod.Info.Name << ":");
						OsiError("Specifying RequiredVersion in ScriptExtender/Config.json is mandatory.");
						continue;
					}

					if (config.ModTable.empty()) {
						OsiError("Module '" << mod.Info.Name << ":");
						OsiError("Module must specify a ModTable in ScriptExtender/Config.json.");
						continue;
					}

					if (config.MinimumVersion > CurrentVersion) {
						OsiError("Module '" << mod.Info.Name << " is targeting version v" << config.MinimumVersion << " that' doesn't exist's more recent than the current version!");
					}

					if (config.MinimumVersion != 0 && config.MinimumVersion > MergedConfig.MinimumVersion) {
						MergedConfig.MinimumVersion = config.MinimumVersion;
						HighestVersionMod = &mod;
					}

					for (auto const & flag : config.FeatureFlags) {
						MergedConfig.FeatureFlags.insert(flag);
					}

					numConfigs++;

					modConfigs_.insert(std::make_pair(mod.Info.ModuleUUIDString.GetString(), config));
				}
			}
		}

		if (numConfigs > 0) {
			INFO("%d mod configuration(s) loaded.", numConfigs);
			std::stringstream featureFlags;
			for (auto const & flag : MergedConfig.FeatureFlags) {
				featureFlags << flag << " ";
			}

			INFO(L"Merged configuration:\r\n\tMinVersion %d; Feature flags: %s",
				MergedConfig.MinimumVersion, FromUTF8(featureFlags.str()).c_str());
		}

		if (CurrentVersion < MergedConfig.MinimumVersion && HighestVersionMod != nullptr) {
			std::stringstream msg;
			msg << "Module \"" << HighestVersionMod->Info.Name << "\" requires extension version "
				<< MergedConfig.MinimumVersion << "; current version is v" << CurrentVersion;
			gExtender->GetLibraryManager().ShowStartupError(msg.str().c_str(), false, true);
		}
	}

	bool ExtensionStateBase::LoadConfig(Module const & mod, STDString const & configText, ExtensionModConfig & config)
	{
		Json::CharReaderBuilder factory;
		auto reader = std::unique_ptr<Json::CharReader>(factory.newCharReader());

		Json::Value root;
		std::string errs;
		if (!reader->parse(configText.c_str(), configText.c_str() + configText.size(), &root, &errs)) {
			OsiError("Unable to parse configuration for mod '" << mod.Info.Name << "': " << errs);
			return false;
		}

		return LoadConfig(mod, root, config);
	}

	std::optional<bool> GetConfigBool(Json::Value & config, std::string const & key)
	{
		auto value = config[key];
		if (!value.isNull()) {
			if (value.isBool()) {
				return value.asBool();
			} else {
				OsiError("Config option '" << key << "' should be a boolean.");
			}
		}

		return {};
	}

	std::optional<int32_t> GetConfigInt(Json::Value & config, std::string const & key)
	{
		auto value = config[key];
		if (!value.isNull()) {
			if (value.isInt()) {
				return value.asInt();
			} else {
				OsiError("Config option '" << key << "' should be an integer.");
			}
		}

		return {};
	}

	std::optional<STDString> GetConfigString(Json::Value& config, std::string const& key)
	{
		auto value = config[key];
		if (!value.isNull()) {
			if (value.isString()) {
				return value.asString().c_str();
			} else {
				OsiError("Config option '" << key << "' should be a string.");
			}
		}

		return {};
	}

	bool ExtensionStateBase::LoadConfig(Module const & mod, Json::Value & json, ExtensionModConfig & config)
	{
		auto version = GetConfigInt(json, "RequiredVersion");
		if (version) {
			config.MinimumVersion = (uint32_t)*version;
		}

		auto modTable = GetConfigString(json, "ModTable");
		if (modTable) {
			config.ModTable = *modTable;
		}

		auto featureFlags = json["FeatureFlags"];
		if (featureFlags.isArray()) {
			for (auto const & flag : featureFlags) {
				if (flag.isString()) {
					auto flagStr = flag.asString();
					if (sAllFeatureFlags.find(flagStr) != sAllFeatureFlags.end()) {
						config.FeatureFlags.insert(STDString(flagStr));
					} else {
						ERR("Feature flag '%s' not supported!", flagStr.c_str());
					}
				} else {
					ERR("Garbage found in FeatureFlags array");
				}
			}
		}

		return true;
	}

	bool ExtensionStateBase::HasFeatureFlag(char const * flag) const
	{
		return MergedConfig.FeatureFlags.find(flag) != MergedConfig.FeatureFlags.end();
	}

	void ExtensionStateBase::OnGameSessionLoading()
	{
		LuaVirtualPin lua(*this);
		if (lua) {
			lua->OnGameSessionLoading();
		}
	}

	void ExtensionStateBase::OnGameSessionLoaded()
	{
		LuaVirtualPin lua(*this);
		if (lua) {
			lua->OnGameSessionLoaded();
		}
	}

	void ExtensionStateBase::OnModuleLoadStarted()
	{
		LuaVirtualPin lua(*this);
		if (lua) {
			lua->OnModuleLoadStarted();
		}
	}

	void ExtensionStateBase::OnModuleLoading()
	{
		StatLoadTriggered = true;
		LuaVirtualPin lua(*this);
		if (lua) {
			lua->OnModuleLoading();
		}
	}

	void ExtensionStateBase::OnStatsLoaded()
	{
		LuaVirtualPin lua(*this);
		if (lua) {
			lua->OnStatsLoaded();
		}
	}

	void ExtensionStateBase::OnModuleResume()
	{
		LuaVirtualPin lua(*this);
		if (lua) {
			lua->OnModuleResume();
		}
	}

	void ExtensionStateBase::OnResetCompleted()
	{
		LuaVirtualPin lua(*this);
		if (lua) {
			lua->OnResetCompleted();
		}
	}

	void ExtensionStateBase::OnUpdate(GameTime const& time)
	{
		LuaVirtualPin lua(*this);
		if (lua) {
			lua->OnUpdate(time);
		}

		// userVariables_.Update();
		// modVariables_.Update();
	}


	void ExtensionStateBase::IncLuaRefs()
	{
		luaMutex_.lock();
		luaRefs_++;
	}

	void ExtensionStateBase::DecLuaRefs()
	{
		assert(luaRefs_ > 0);
		luaRefs_--;
		luaMutex_.unlock();

		if (luaRefs_ == 0 && LuaPendingDelete) {
			LuaResetInternal();
		}
	}

	void ExtensionStateBase::LuaReset(ExtensionStateContext nextContext, bool startup)
	{
		nextContext_ = nextContext;
		LuaReset(startup);
	}

	void ExtensionStateBase::LuaReset(bool startup)
	{
		if (LuaPendingDelete) {
			OsiWarn("State delete is already pending!");
		}

		LuaPendingDelete = true;
		if (startup) {
			LuaPendingStartup = true;
		}

		if (luaRefs_ == 0) {
			LuaResetInternal();
		} else {
			OsiWarn("Lua state deletion deferred (" << luaRefs_ << " references still alive)");
		}
	}

	void ExtensionStateBase::AddPostResetCallback(PostResetCallback callback)
	{
		luaPostResetCallbacks_.push_back(callback);
	}

	std::optional<STDString> ExtensionStateBase::ResolveModScriptPath(STDString const& modNameGuid, STDString const& fileName)
	{
		auto mod = GetModManager()->FindModByNameGuid(modNameGuid.c_str());
		if (mod == nullptr) {
			OsiError("Mod does not exist or is not loaded: " << modNameGuid);
			return {};
		}

		return ResolveModScriptPath(*mod, fileName);
	}

	STDString ExtensionStateBase::ResolveModScriptPath(Module const& mod, STDString const& fileName)
	{
		STDString path("Mods/");
		path += mod.Info.Directory;
		path += "/ScriptExtender/Lua/";
		path += fileName;
		return path;
	}

	std::optional<int> ExtensionStateBase::LuaLoadExternalFile(STDString const & path)
	{
		std::ifstream f(path.c_str(), std::ios::in | std::ios::binary);
		if (!f.good()) {
			OsiError("File does not exist: " << path);
			return {};
		}

		f.seekg(0, std::ios::end);
		auto length = f.tellg();
		f.seekg(0, std::ios::beg);
		STDString s((uint32_t)length, '\0');
		f.read(const_cast<char *>(s.data()), length);
		f.close();

		LuaVirtualPin lua(*this);
		if (!lua) {
			OsiErrorS("Called when the Lua VM has not been initialized!");
			return {};
		}

		return lua->LoadScript(s, path);
	}

	std::optional<int> ExtensionStateBase::LuaLoadGameFile(FileReaderPin & reader, STDString const & scriptName, int globalsIdx)
	{
		if (!reader.IsLoaded()) {
			OsiErrorS("Attempted to load script from invalid file reader");
			return {};
		}

		LuaVirtualPin lua(*this);
		if (!lua) {
			OsiErrorS("Called when the Lua VM has not been initialized!");
			return {};
		}

		return lua->LoadScript(reader.ToString(), scriptName, globalsIdx);
	}

	std::optional<int> ExtensionStateBase::LuaLoadGameFile(STDString const & path, STDString const & scriptName, 
		bool warnOnError, int globalsIdx)
	{
		auto reader = GetStaticSymbols().MakeFileReader(path);
		if (!reader.IsLoaded()) {
			if (warnOnError) {
				OsiError("Script file could not be opened: " << path);
			}
			return {};
		}

		auto result = LuaLoadGameFile(reader, scriptName.empty() ? path : scriptName, globalsIdx);
		if (result) {
			loadedFiles_.insert(std::make_pair(scriptName, path));
			auto fullPath = GetStaticSymbols().ToPath(path, PathRootType::Data);
			if (!fullPath.empty()) {
				fullPath[0] = std::tolower(fullPath[0]);
			}
			loadedFileFullPaths_.insert(std::make_pair(scriptName, fullPath));
		}

		return result;
	}

	std::optional<int> ExtensionStateBase::LuaLoadModScript(STDString const & modNameGuid, STDString const & fileName, 
		bool warnOnError, int globalsIdx)
	{
		auto mod = GetModManager()->FindModByNameGuid(modNameGuid.c_str());
		if (mod == nullptr) {
			OsiError("Mod does not exist or is not loaded: " << modNameGuid);
			return {};
		}

		auto path = ResolveModScriptPath(*mod, fileName);

		STDString scriptName = mod->Info.Directory;
		if (scriptName.length() > 37) {
			// Strip GUID from end of dir
			scriptName = scriptName.substr(0, scriptName.length() - 37);
		}
		scriptName += "/" + fileName;

		return LuaLoadGameFile(path, scriptName, warnOnError, globalsIdx);
	}

	std::optional<int> ExtensionStateBase::LuaLoadBuiltinFile(STDString const & path, bool warnOnError, int globalsIdx)
	{
		auto file = gExtender->GetLuaBuiltinBundle().GetResource(path);
		if (!file) {
			if (warnOnError) {
				OsiError("Builtin Lua script file could not be opened: " << path);
			}
			return {};
		}

		LuaVirtualPin lua(*this);
		if (!lua) {
			OsiErrorS("Called when the Lua VM has not been initialized!");
			return {};
		}

		auto scriptName = STDString("builtin://") + path;
		return lua->LoadScript(*file, scriptName, globalsIdx);
	}

	std::optional<int> ExtensionStateBase::LuaLoadFile(STDString const & path, STDString const & scriptName, 
		bool warnOnError, int globalsIdx)
	{
		if (path.starts_with("builtin://")) {
			return LuaLoadBuiltinFile(path.substr(10), warnOnError, globalsIdx);
		} else {
			return LuaLoadGameFile(path, scriptName, warnOnError, globalsIdx);
		}
	}

	void ExtensionStateBase::LuaResetInternal()
	{
		std::lock_guard _(luaMutex_);
		if (gExtender->GetClient().IsInClientThread()) {
			gExtender->GetClient().UpdateClientProgress("Lua Init");
		} else {
			gExtender->GetClient().UpdateServerProgress("Lua Init");
		}

		assert(LuaPendingDelete);
		assert(luaRefs_ == 0);

		LuaPendingDelete = false;

		// Destroy previous instance first to make sure that no dangling
		// references are made to the old state while constructing the new
		DoLuaReset();
		OsiWarn("LUA VM reset.");

		// Make sure that we won't get destroyed during startup
		IncLuaRefs();

		if (LuaPendingStartup) {
			LuaPendingStartup = false;
			LuaStartup();
		}

		for (auto const& cb : luaPostResetCallbacks_) {
			cb();
		}

		luaPostResetCallbacks_.clear();

		// Prevent Lua state deletion during startup; it would most likely lead to an infinite delete loop
		if (LuaPendingDelete) {
			LuaPendingDelete = false;
			OsiError("Lua state deletion requested during startup? Weird");
		}

		DecLuaRefs();
	}

	void ExtensionStateBase::LuaStartup()
	{
		LuaVirtualPin lua(*this);
		if (!lua) {
			OsiErrorS("Called when the Lua VM has not been initialized!");
			return;
		}

		auto modManager = GetModManager();
		if (modManager == nullptr) {
			OsiErrorS("Could not bootstrap Lua modules - mod manager not available");
			return;
		}

		lua::Restriction restriction(*lua, lua::State::RestrictAll);
		for (auto const& mod : modManager->BaseModule.LoadOrderedModules) {
			auto configIt = modConfigs_.find(mod.Info.ModuleUUIDString);
			if (configIt != modConfigs_.end()) {
				auto const & config = configIt->second;
				if (config.FeatureFlags.find("Lua") != config.FeatureFlags.end()) {
					if (gExtender->GetClient().IsInClientThread()) {
						gExtender->GetClient().UpdateClientProgress(mod.Info.Name);
					} else {
						gExtender->GetClient().UpdateServerProgress(mod.Info.Name);
					}

					if (context_ == ExtensionStateContext::Game) {
						LuaLoadGameBootstrap(config, mod);
					} else if (context_ == ExtensionStateContext::Load) {
						LuaLoadPreinitBootstrap(config, mod);
					} else {
						ERR("Bootstrap request with Uninitialized extension context?");
					}
				}
			}
		}

		lua->FinishStartup();
	}

	void ExtensionStateBase::LuaLoadGameBootstrap(ExtensionModConfig const& config, Module const& mod)
	{
		auto bootstrapFileName = GetBootstrapFileName();
		auto const& sym = GetStaticSymbols();

		auto bootstrapPath = ResolveModScriptPath(mod, bootstrapFileName);
		if (!bootstrapPath.empty() && sym.FileExists(bootstrapPath)) {
			LuaVirtualPin lua(*this);
			auto L = lua->GetState();
			lua::push(L, mod.Info.ModuleUUIDString);
			lua_setglobal(L, "ModuleUUID");

			OsiMsg("Loading bootstrap script: " << bootstrapPath);
			lua->LoadBootstrap(bootstrapFileName, config.ModTable);

			lua::push(L, nullptr);
			lua_setglobal(L, "ModuleUUID");
		}
	}

	void ExtensionStateBase::LuaLoadPreinitBootstrap(ExtensionModConfig const& config, Module const& mod)
	{
		auto bootstrapFileName = "BootstrapModule.lua";
		auto const& sym = GetStaticSymbols();

		auto path = ResolveModScriptPath(mod, bootstrapFileName);
		if (!sym.FileExists(path)) {
			return;
		}

		LuaVirtualPin lua(*this);
		auto L = lua->GetState();
		lua::push(L, mod.Info.ModuleUUID);
		lua_setglobal(L, "ModuleUUID");

		OsiMsg("Loading preinit bootstrap script: " << path);
		lua->LoadBootstrap(bootstrapFileName, config.ModTable);

		lua::push(L, nullptr);
		lua_setglobal(L, "ModuleUUID");
	}

	ExtensionStateBase* GetCurrentExtensionState()
	{
		return gExtender->GetCurrentExtensionState();
	}
}
