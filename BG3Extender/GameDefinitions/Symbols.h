#pragma once

#include <GameDefinitions/Character.h>
#include <GameDefinitions/EntityManager.h>
#include <GameDefinitions/Item.h>
#include <GameDefinitions/Level.h>
#include <GameDefinitions/Misc.h>
#include <GameDefinitions/Osiris.h>
#include <GameDefinitions/RootTemplates.h>
#include <GameDefinitions/Stats/Stats.h>
#include <GameDefinitions/Passives.h>
#include <GameDefinitions/Status.h>
#include <GameDefinitions/Surface.h>
#include <GameDefinitions/EntitySystemHelpers.h>

namespace bg3se
{
	struct GuidResourceDefinitions;
	struct ResourceManager;
	struct App;

	struct StaticSymbols : Noncopyable<StaticSymbols>
	{
		std::unordered_map<int32_t*, ecs::IndexSymbolInfo> IndexSymbolToNameMaps;
		std::unordered_map<void const*, char const*> StaticStringRegistrantMaps;

		struct EoCLibraryInfo
		{
			uint8_t const * initFunc;
			uint8_t const * freeFunc;
			uint32_t refs;
		};

		FixedString::CreateFromStringProc* ls__FixedString__CreateFromString{ nullptr };
		FixedString::GetStringProc* ls__FixedString__GetString{ nullptr };
		FixedString::IncRefProc* ls__FixedString__IncRef{ nullptr };
		FixedString::DecRefProc* ls__FixedString__DecRef{ nullptr };
		GlobalStringTable** ls__gGlobalStringTable{ nullptr };

		FileReader::CtorProc* ls__FileReader__ctor{ nullptr };
		FileReader::DtorProc* ls__FileReader__dtor{ nullptr };
		STDString** ls__PathRoots{ nullptr };
		CoreLibInitProc* CoreLibSDM__Init{ nullptr };
		AppUpdatePathsProc* App__UpdatePaths{ nullptr };

		ecl::EoCClient** ecl__EoCClient{ nullptr };
		esv::EoCServer** esv__EoCServer{ nullptr };

		ecl::EoCClient::HandleErrorProc* ecl__EoCClient__HandleError{ nullptr };

		TranslatedStringRepository** ls__gTranslatedStringRepository{ nullptr };

		void** ecl__gGameStateEventManager{ nullptr };
		ecl::GameStateEventManager__ExecuteGameStateChangedEvent ecl__GameStateEventManager__ExecuteGameStateChangedEvent{ nullptr };
		ecl::GameStateThreaded__GameStateWorker__DoWork ecl__GameStateThreaded__GameStateWorker__DoWork{ nullptr };
		esv::GameStateThreaded__GameStateWorker__DoWork esv__GameStateThreaded__GameStateWorker__DoWork{ nullptr };
		ecl::GameStateMachine__Update ecl__GameStateMachine__Update{ nullptr };
		esv::GameStateMachine__Update esv__GameStateMachine__Update{ nullptr };

		/*esv::SurfaceActionFactory** esv__SurfaceActionFactory{nullptr};
		esv::SurfaceActionFactory::CreateActionProc* esv__SurfaceActionFactory__CreateAction{ nullptr };
		esv::SurfaceManager::AddActionProc* esv__SurfaceManager__AddAction{ nullptr };*/

		stats::SpellPrototypeManager** eoc__SpellPrototypeManager{ nullptr };
		stats::SpellPrototype::InitProc* eoc__SpellPrototype__Init{ nullptr };

		stats::StatusPrototypeManager** eoc__StatusPrototypeManager{ nullptr };
		stats::StatusPrototype::InitProc* eoc__StatusPrototype__Init{ nullptr };

		stats::PassiveManager** eoc__PassiveManager{ nullptr };
		stats::Passive::InitProc* eoc__Passive__Init{ nullptr };

		esv::StatusMachine::CreateStatusProc* esv__StatusMachine__CreateStatus{ nullptr };
		esv::StatusMachine::ApplyStatusProc* esv__StatusMachine__ApplyStatus{ nullptr };

		stats::DealDamageFunctor::ApplyDamageProc* stats__DealDamageFunctor__ApplyDamage{ nullptr };

		stats::Functors::ExecuteType1Proc* stats__Functors__ExecuteType1{ nullptr };
		stats::Functors::ExecuteType2Proc* stats__Functors__ExecuteType2{ nullptr };
		stats::Functors::ExecuteType3Proc* stats__Functors__ExecuteType3{ nullptr };
		stats::Functors::ExecuteType4Proc* stats__Functors__ExecuteType4{ nullptr };
		stats::Functors::ExecuteType5Proc* stats__Functors__ExecuteType5{ nullptr };
		stats::Functors::ExecuteType6Proc* stats__Functors__ExecuteType6{ nullptr };
		stats::Functors::ExecuteType7Proc* stats__Functors__ExecuteType7{ nullptr };
		stats::Functors::ExecuteType8Proc* stats__Functors__ExecuteType8{ nullptr };

		stats::RPGStats** gRPGStats{ nullptr };
		stats::RPGStats::LoadProc* RPGStats__Load{ nullptr };
		stats::RPGStats::ParseStructureFolderProc* RPGStats__ParseStructureFolder{ nullptr };
		/*RPGStats::ParsePropertiesProc* RPGStats__ParseProperties{nullptr};
		SkillPrototypeManager** eoc__SkillPrototypeManager{ nullptr };
		StatusPrototypeManager** eoc__StatusPrototypeManager{ nullptr };*/

		esv::SavegameManager** esv__SavegameManager{ nullptr };

		App** AppInstance{ nullptr };

		std::map<uint8_t const *, EoCLibraryInfo> Libraries;

		ls__GlobalAllocator__AllocProc* ls__GlobalAllocator__Alloc{ nullptr };
		ls__GlobalAllocator__FreeProc* ls__GlobalAllocator__Free{ nullptr };

		GuidResourceDefinitions** ResourceDefns{ nullptr };
		ResourceManager** ResourceMgr{ nullptr };

		GlobalSwitches** ls__GlobalSwitches{ nullptr };

		inline StaticSymbols() {}
		StaticSymbols(StaticSymbols const &) = delete;
		StaticSymbols & operator = (StaticSymbols const &) = delete;

		inline GlobalSwitches * GetGlobalSwitches() const
		{
			if (ls__GlobalSwitches && *ls__GlobalSwitches) {
				return *ls__GlobalSwitches;
			} else {
				return nullptr;
			}
		}

		inline ecl::EoCClient * GetEoCClient() const
		{
			if (ecl__EoCClient == nullptr || *ecl__EoCClient == nullptr) {
				return nullptr;
			} else {
				return *ecl__EoCClient;
			}
		}

		inline esv::EoCServer* GetEoCServer() const
		{
			if (esv__EoCServer == nullptr || *esv__EoCServer == nullptr) {
				return nullptr;
			} else {
				return *esv__EoCServer;
			}
		}

		inline ModManager* GetModManagerClient() const
		{
			auto client = GetEoCClient();
			if (client == nullptr || client->ModManager == nullptr) {
				return nullptr;
			} else {
				return client->ModManager;
			}
		}

		inline ModManager* GetModManagerServer() const
		{
			auto server = GetEoCServer();
			if (server == nullptr) {
				return nullptr;
			} else {
				return &server->ModManager;
			}
		}

		inline stats::RPGStats* GetStats() const
		{
			if (gRPGStats && *gRPGStats) {
				return *gRPGStats;
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
				&& (*esv__EoCServer)->GameStateMachine != nullptr) {
				return (*esv__EoCServer)->GameStateMachine->State;
			} else {
				return {};
			}
		}

		inline ecs::EntityWorld* GetClientEntityWorld() const
		{
			if (ecl__EoCClient != nullptr
				&& *ecl__EoCClient != nullptr
				&& (*ecl__EoCClient)->EntityWorld != nullptr) {
				return (*ecl__EoCClient)->EntityWorld;
			} else {
				return {};
			}
		}

		inline ecs::EntityWorld* GetServerEntityWorld() const
		{
			if (esv__EoCServer != nullptr
				&& *esv__EoCServer != nullptr
				&& (*esv__EoCServer)->EntityWorld != nullptr) {
				return (*esv__EoCServer)->EntityWorld;
			} else {
				return {};
			}
		}

		/*inline esv::SurfaceActionFactory* GetSurfaceActionFactory() const
		{
			if (esv__SurfaceActionFactory != nullptr) {
				return *esv__SurfaceActionFactory;
			} else {
				return {};
			}
		}*/

		inline esv::LevelManager* GetServerLevelManager() const
		{
			/*if (esv__EoCServer != nullptr
				&& *esv__EoCServer != nullptr
				&& (*esv__EoCServer)->EntityManager != nullptr
				&& (*esv__EoCServer)->EntityManager->LevelManager != nullptr) {
				return reinterpret_cast<esv::LevelManager *>((*esv__EoCServer)->EntityManager->LevelManager);
			} else {
				return {};
			}*/
			return nullptr;
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
		FileReaderPin MakeFileReader(StringView path, PathRootType root = PathRootType::Data, bool canonicalize = true) const;
		void DestroyFileReader(FileReader* reader);
		bool FileExists(StringView path, PathRootType root = PathRootType::Data, bool canonicalize = true) const;
	};

	extern StaticSymbols* gStaticSymbols;

	void InitStaticSymbols();

	inline StaticSymbols& GetStaticSymbols()
	{
		return *gStaticSymbols;
	}
}
