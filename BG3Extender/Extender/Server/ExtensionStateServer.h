#pragma once

#include <Extender/Shared/ExtensionState.h>
#include <Lua/Server/LuaBindingServer.h>

namespace Json { class Value; }

namespace bg3se::esv
{
	class ExtensionState;
	using LuaServerPin = LuaStatePin<ExtensionState, lua::ServerState>;

	class ExtensionState : public ExtensionStateBase
	{
	public:
		ExtensionState();
		~ExtensionState() override;

		void Reset() override;
		lua::State * GetLua() override;
		ModManager * GetModManager() override;
		void OnGameSessionLoading() override;

		inline char const * GetBootstrapFileName() override
		{
			return "BootstrapServer.lua";
		}

		inline char const * GetUnconditionalFileName() override
		{
			return "OverrideServer.lua";
		}

		inline std::unordered_set<FixedString> const& GetPersistentStats() const
		{
			return persistentStats_;
		}

		inline std::unordered_set<FixedString> const& GetDynamicStats() const
		{
			return dynamicStats_;
		}

		void MarkPersistentStat(FixedString const& statId);
		void UnmarkPersistentStat(FixedString const& statId);
		void MarkDynamicStat(FixedString const& statId);

		std::optional<STDString> GetModPersistentVars(FixedString const& mod);
		void RestoreModPersistentVars(FixedString const& mod, STDString const& vars);
		std::unordered_set<FixedString> GetPersistentVarMods();
		std::optional<STDString> GetModTable(FixedString const& mod);

		void StoryLoaded();
		void StoryFunctionMappingsUpdated();
		void StorySetMerging(bool isMerging);

		static ExtensionState & Get();

	protected:
		friend LuaStatePin<ExtensionState, lua::ServerState>;
		std::unique_ptr<lua::ServerState> Lua;
		std::unordered_set<FixedString> dynamicStats_;
		std::unordered_set<FixedString> persistentStats_;
		std::unordered_map<FixedString, STDString> cachedPersistentVars_;
		uint32_t nextGenerationId_{ 1 };

		void DoLuaReset() override;
		void LuaStartup() override;
	};
}
