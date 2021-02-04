#pragma once

#include <GameDefinitions/Character.h>
#include <GameDefinitions/Item.h>
#include <GameDefinitions/Misc.h>
#include <GameDefinitions/Osiris.h>
#include <GameDefinitions/RootTemplates.h>
#include <GameDefinitions/Stats/Stats.h>

namespace bg3se
{
	/*enum class EsvGlobalEoCApp {
		EsvLSDialogEventManager = 0,
		EsvStoryDialogEventManager = 1,
		EsvStoryItemEventManager = 2,
		EsvStoryCharacterEventManager = 3,
		// 4
		// 5-6 event visitor
		// 7-8 game event visitor
		ServerLevelManager = 9,
		PartyManager = 10,
		EsvCharacterFactory = 11,
		// 12
		EsvProjectileFactory = 13,
		EsvEoCTriggerFactory = 14,
		EsvItemFactory = 15,
		EsvSkillFactory = 16,
		EsvSkillStatePool = 17,
		EsvInventoryFactory = 18,
		EsvInventoryViewFactory = 19,
		StoryImplementation = 20,
		AlignmentFactory = 21,
		AlignmentContainer = 22,
		OsirisVariableHelper = 23,
		// 24
		OverheadTextFactory = 25,
		SurfaceActionFactory = 26,
		CustomBookManager = 27,
		// 28
		GameControl = 29,
		// 30
		CharacterCreationManager = 31,
		MarkerManager = 32,
		OsirisTaskFactory = 33,
		EnvironmentalInfluencesPool = 34,
		CombatFormationManager = 35,
		DivinityStats = 36,
		PlanManager = 37,
		// 38
		ActivationManager = 39
	};

	enum class EsvGlobalEoCPlugin {
		EsvSavegameManager = 0,
		EsvEoCServer = 1,
		EsvCacheTemplateManager = 2,
		EsvAiFactory = 3,
		EsvGameObjectFactory = 4,
		EsvLSDialogEventManager = 5,
		EsvStoryDialogEventManager = 6,
		EsvStoryItemEventManager = 7,
		EsvStoryCharacterEventManager = 8,
		// 9, 10-11, 12-13 event visitors
		ServerLevelManager = 14,
		PartyManager = 15,
		EsvCharacterFactory = 16,
		EsvProjectileFactory = 17,
		EsvEoCTriggerFactory = 18,
		EsvItemFactory = 19,
		EsvSkillFactory = 20,
		EsvSkillStatePool = 21,
		EsvInventoryFactory = 22,
		EsvInventoryViewFactory = 23,
		StoryImplementation = 24,
		AlignmentFactory = 25,
		OsirisVariableHelper = 26,
		// 27
		OverheadTextFactory = 28,
		SurfaceActionFactory = 29,
		CustomBookManager = 30,
		// 31
		GameControl = 32,
		// 33
		CharacterCreationManager = 34,
		MarkerManager = 35,
		OsirisTaskFactory = 36,
		EnvironmentalInfluencesPool = 37,
		CombatFormationManager = 38,
		DivinityStats = 39,
		PlanManager = 40,
		ActivationManager = 41
	};*/

	struct ResourceDefinitions;

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

		/*esv::StatusMachine::CreateStatusProc* esv__StatusMachine__CreateStatus{ nullptr };
		esv::StatusMachine::ApplyStatusProc* esv__StatusMachine__ApplyStatus{ nullptr };

		GlobalStringTable const ** GlobalStrings{ nullptr };
		ls__Path__GetPrefixForRoot GetPrefixForRoot{ nullptr };*/
		/*
		RPGStats::LoadProc RPGStats__Load{ nullptr };
		RPGStats::ParsePropertiesProc* RPGStats__ParseProperties{ nullptr };
		SkillPrototypeManager** eoc__SkillPrototypeManager{ nullptr };
		StatusPrototypeManager** eoc__StatusPrototypeManager{ nullptr };*/

		void * ModuleSettingsHasCustomMods{ nullptr };
		RPGStats** Stats{ nullptr };

		std::map<uint8_t const *, EoCLibraryInfo> Libraries;

		EoCAllocFunc EoCAlloc{ nullptr };
		EoCFreeFunc EoCFree{ nullptr };
		CrtAllocFunc CrtAlloc{ nullptr };
		CrtFreeFunc CrtFree{ nullptr };

		//CharacterStatsGetters CharStatsGetters;

		ResourceDefinitions** ResourceDefns;

		inline StaticSymbols() {}
		StaticSymbols(StaticSymbols const &) = delete;
		StaticSymbols & operator = (StaticSymbols const &) = delete;

		/*inline GlobalStringTable const * GetGlobalStringTable() const
		{
			if (GlobalStrings) {
				return *GlobalStrings;
			} else {
				return nullptr;
			}
		}*/

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

		void CanonicalizePath(STDString & path) const;
		STDString ToPath(StringView path, PathRootType root, bool canonicalize = true) const;
		bool FileExists(StringView path, PathRootType root = PathRootType::Data, bool canonicalize = true) const;
	};

	StaticSymbols & GetStaticSymbols();
}
