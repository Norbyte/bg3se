#pragma once

#include "ExtensionState.h"
#include <Lua/Server/LuaBindingServer.h>
//#include <Hit.h>

namespace Json { class Value; }

namespace bg3se::esv
{/*
	class DamageHelperPool
	{
	public:
		void Clear();
		DamageHelpers * Create();
		bool Destroy(ObjectHandle handle);
		DamageHelpers * Get(ObjectHandle handle) const;

	private:
		std::unordered_map<uint32_t, std::unique_ptr<DamageHelpers>> helpers_;
		uint32_t nextHelperId_{ 0 };
	};*/

	class ExtensionState;
	using LuaServerPin = LuaStatePin<ExtensionState, lua::ServerState>;

	class ExtensionState : public ExtensionStateBase
	{
	public:
		/*DamageHelperPool DamageHelpers;
		PendingHitManager PendingHits;
		PendingStatuses PendingStatuses;
		std::unique_ptr<ObjectSet<eoc::ItemDefinition>> PendingItemClone;
		std::unique_ptr<ShootProjectileApiHelper> ProjectileHelper;*/

		void Reset() override;
		lua::State * GetLua() override;
		ModManager * GetModManager() override;
		void OnGameSessionLoading() override;

		inline char const * GetBootstrapFileName() override
		{
			return "BootstrapServer.lua";
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

		void DoLuaReset() override;
		void LuaStartup() override;
	};
}