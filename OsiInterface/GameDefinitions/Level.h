#pragma once

#include "BaseTypes.h"

namespace dse
{
	struct EntityManager;

	struct LevelDesc : public ProtectedGameObject<LevelDesc>
	{
		void* VMT;
		FixedString LevelName;
		FixedString UniqueKey;
		STDWString CustomDisplayLevelName;
		ObjectSet<Path> Paths;
		uint8_t Type;
	};

	struct Level : public ProtectedGameObject<Level>
	{
		void* VMT;
		void* LevelManager;
		void* LocalTemplateManager;
		LevelDesc* LevelDesc;
		void* LocalTemplateManager2_M;
		void* Scene_M;
		void* SimplePhysXScene;
		ObjectSet<void*> GameObjects_M;
		void* SomeObject;
		void* field_58;
		FixedString LevelGuid;
		ObjectHandle field_68;
		int SomeFlag;
		ObjectSet<void*> ActivePersistentLevelTemplates_M;
		ObjectSet<void*> Unknown2;
		int IsLoaded_M;
		int field_AC;
	};

	struct EoCLevel : public Level
	{
		EntityManager* EntityManager;
		void* AiGrid;
		__int64 field_C0;
		__int64 field_C8;
		// Maybe?
		/*void* LayerManager;
		eoc::AiGrid* AiGrid;
		void* VisionGrid;*/
	};

	struct LevelManagerBase
	{
		uint64_t Unknown[0x10];
	};

	struct LevelManager : public ProtectedGameObject<LevelManager>
	{
		void* VMT;
		LevelManagerBase b;
		void* field_88;
		Level* CurrentLevel;
		void* field_98;
		void* field_A0;
		int field_A8;
		Array<void*> field_B0;
		Array<void*> field_C8;
		void* field_E0;
		int field_E8;
		Map<FixedString, Level*> FSMap_pLevel;
	};


	namespace esv
	{
		struct Level : public EoCLevel
		{
			void* SightManager;
			void* CharacterManager;
			void* ItemManager;
			void* ProjectileManager;
			void* SurfaceManager;
			void* GameActionManager;
			void* EffectManager;
			void* NoiseManager_M;
			void* ShroudManager;
			void* LightingOverrides;
			void* AtmosphereOverrides;
			void* EnvironmentalStatusManager;
			void* StaticPhysicsManager;
			void* TimelineSystem;
			void* TerrainManager;
			void* LevelCacheTemplateManager_M;
		};

		struct LevelManager
		{
			void* VMT;
#if defined(OSI_EOCAPP)
			uint64_t Unknown[15];
#endif
			Level* CurrentLevel;
			VirtualMultiHashMap<FixedString, void*> LevelMetadata_M;
			Map<FixedString, Level*> Levels;
		};

		struct LevelManager : public dse::LevelManager
		{
			__int64 field_108;
			void* VMT2;
			void* field_118;
			ObjectSet<void*> SomeRegistrations;
			FixedString field_138;
			int field_13C;
			STDWString field_140;
		};
	}
}
