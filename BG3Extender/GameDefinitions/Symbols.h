#pragma once

#include <GameDefinitions/Character.h>
#include <GameDefinitions/EntityManager.h>
#include <GameDefinitions/Item.h>
#include <GameDefinitions/Level.h>
#include <GameDefinitions/Misc.h>
#include <GameDefinitions/Osiris.h>
#include <GameDefinitions/RootTemplates.h>
#include <GameDefinitions/Stats/Stats.h>
#include <GameDefinitions/Status.h>
#include <GameDefinitions/Surface.h>

namespace bg3se
{
	struct GuidResourceDefinitions;
	struct ResourceManager;
	struct App;

	struct StaticSymbols : Noncopyable<StaticSymbols>
	{
		std::unordered_map<int32_t*, char*> SymbolIdToNameMaps;

		struct EoCLibraryInfo
		{
			uint8_t const * initFunc;
			uint8_t const * freeFunc;
			uint32_t refs;
		};

		FixedString::CreateProc* ls__FixedString__Create{ nullptr };
		FixedString::GetStringProc* ls__FixedString__GetString{ nullptr };
		FixedString::IncRefProc* ls__FixedString__IncRef{ nullptr };
		FixedString::DecRefProc* ls__FixedString__DecRef{ nullptr };

		FileReader::CtorProc* ls__FileReader__ctor{ nullptr };
		FileReader::DtorProc* ls__FileReader__dtor{ nullptr };
		STDString** PathRoots{ nullptr };

		ecl::EoCClient** ecl__EoCClient{ nullptr };
		esv::EoCServer** esv__EoCServer{ nullptr };

		ecl::EoCClient::HandleErrorProc* ecl__EoCClient__HandleError{ nullptr };

		ecl::GameStateEventManager__ExecuteGameStateChangedEvent ecl__GameStateEventManager__ExecuteGameStateChangedEvent{ nullptr };
		esv::GameStateEventManager__ExecuteGameStateChangedEvent esv__GameStateEventManager__ExecuteGameStateChangedEvent{ nullptr };
		ecl::GameStateThreaded__GameStateWorker__DoWork ecl__GameStateThreaded__GameStateWorker__DoWork{ nullptr };
		esv::GameStateThreaded__GameStateWorker__DoWork esv__GameStateThreaded__GameStateWorker__DoWork{ nullptr };

		esv::SurfaceActionFactory** esv__SurfaceActionFactory{ nullptr };
		esv::SurfaceActionFactory::CreateActionProc* esv__SurfaceActionFactory__CreateAction{ nullptr };
		esv::SurfaceManager::AddActionProc* esv__SurfaceManager__AddAction{ nullptr };

		esv::StatusMachine::CreateStatusProc* esv__StatusMachine__CreateStatus{ nullptr };
		esv::StatusMachine::ApplyStatusProc* esv__StatusMachine__ApplyStatus{ nullptr };

		DealDamageFunctor::ApplyDamageProc* eoc__DealDamageFunctor__ApplyDamage{ nullptr };

		StatsFunctorSet::ExecuteType1Proc* eoc__StatsFunctorSet__ExecuteType1{ nullptr };
		StatsFunctorSet::ExecuteType2Proc* eoc__StatsFunctorSet__ExecuteType2{ nullptr };
		StatsFunctorSet::ExecuteType3Proc* eoc__StatsFunctorSet__ExecuteType3{ nullptr };
		StatsFunctorSet::ExecuteType4Proc* eoc__StatsFunctorSet__ExecuteType4{ nullptr };
		StatsFunctorSet::ExecuteType5Proc* eoc__StatsFunctorSet__ExecuteType5{ nullptr };
		StatsFunctorSet::ExecuteType6Proc* eoc__StatsFunctorSet__ExecuteType6{ nullptr };
		StatsFunctorSet::ExecuteType7Proc* eoc__StatsFunctorSet__ExecuteType7{ nullptr };
		StatsFunctorSet::ExecuteType8Proc* eoc__StatsFunctorSet__ExecuteType8{ nullptr };

		/*RPGStats::LoadProc RPGStats__Load{ nullptr };
		RPGStats::ParsePropertiesProc* RPGStats__ParseProperties{ nullptr };
		SkillPrototypeManager** eoc__SkillPrototypeManager{ nullptr };
		StatusPrototypeManager** eoc__StatusPrototypeManager{ nullptr };*/

		void * ModuleSettingsHasCustomMods{ nullptr };
		RPGStats** Stats{ nullptr };
		esv::SavegameManager** esv__SavegameManager{ nullptr };

		App** AppInstance{ nullptr };

		std::map<uint8_t const *, EoCLibraryInfo> Libraries;

		EoCAllocFunc EoCAlloc{ nullptr };
		EoCFreeFunc EoCFree{ nullptr };
		CrtAllocFunc CrtAlloc{ nullptr };
		CrtFreeFunc CrtFree{ nullptr };

		//CharacterStatsGetters CharStatsGetters;

		GuidResourceDefinitions** ResourceDefns{ nullptr };
		ResourceManager** ResourceMgr{ nullptr };

		inline StaticSymbols() {}
		StaticSymbols(StaticSymbols const &) = delete;
		StaticSymbols & operator = (StaticSymbols const &) = delete;

		inline RPGStats* GetStats() const
		{
			if (Stats && *Stats) {
				return *Stats;
			} else {
				return nullptr;
			}
		}

		inline std::optional<ecl::GameState> GetClientState() const
		{
			if (ecl__EoCClient != nullptr
				&& *ecl__EoCClient != nullptr
				&& (*ecl__EoCClient)->GameStateMachine != nullptr) {
				return (*ecl__EoCClient)->GameStateMachine->State;
			} else {
				return {};
			}
		}

		inline std::optional<esv::GameState> GetServerState() const
		{
			if (esv__EoCServer != nullptr
				&& *esv__EoCServer != nullptr
				&& (*esv__EoCServer)->StateMachine != nullptr) {
				return (*esv__EoCServer)->StateMachine->State;
			} else {
				return {};
			}
		}

		inline ecl::EntityWorld* GetClientEntityWorld() const
		{
			if (ecl__EoCClient != nullptr
				&& *ecl__EoCClient != nullptr
				&& (*ecl__EoCClient)->EntityWorld != nullptr) {
				return (*ecl__EoCClient)->EntityWorld;
			} else {
				return {};
			}
		}

		inline esv::EntityWorld* GetServerEntityWorld() const
		{
			if (esv__EoCServer != nullptr
				&& *esv__EoCServer != nullptr
				&& (*esv__EoCServer)->EntityWorld != nullptr) {
				return (*esv__EoCServer)->EntityWorld;
			} else {
				return {};
			}
		}

		inline esv::SurfaceActionFactory* GetSurfaceActionFactory() const
		{
			/*if (esv__SavegameManager != nullptr
				&& *esv__SavegameManager != nullptr
				&& (*esv__SavegameManager)->ComponentFactories.Size > 21) {
				return reinterpret_cast<esv::SurfaceActionFactory*>((*esv__SavegameManager)->ComponentFactories[21]);*/
			if (esv__SurfaceActionFactory != nullptr) {
				return *esv__SurfaceActionFactory;
			} else {
				return {};
			}
		}

		inline esv::LevelManager* GetServerLevelManager() const
		{
			if (esv__EoCServer != nullptr
				&& *esv__EoCServer != nullptr
				&& (*esv__EoCServer)->EntityManager != nullptr
				&& (*esv__EoCServer)->EntityManager->LevelManager != nullptr) {
				return reinterpret_cast<esv::LevelManager *>((*esv__EoCServer)->EntityManager->LevelManager);
			} else {
				return {};
			}
		}

		inline esv::Level* GetCurrentServerLevel() const
		{
			auto levelMgr = GetServerLevelManager();
			if (levelMgr) {
				return static_cast<esv::Level*>(levelMgr->CurrentLevel);
			} else {
				return nullptr;
			}
		}

		inline FixedString GetCurrentServerLevelName() const
		{
			auto levelMgr = GetServerLevelManager();
			if (levelMgr && levelMgr->CurrentLevel) {
				return levelMgr->CurrentLevel->LevelData->LevelDesc->LevelName;
			} else {
				return {};
			}
		}

		void CanonicalizePath(STDString & path) const;
		STDString ToPath(StringView path, PathRootType root, bool canonicalize = true) const;
		bool FileExists(StringView path, PathRootType root = PathRootType::Data, bool canonicalize = true) const;
	};

	StaticSymbols & GetStaticSymbols();
}
