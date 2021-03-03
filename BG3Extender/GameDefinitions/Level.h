#pragma once

#include "BaseTypes.h"

namespace bg3se
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

	struct LevelData
	{
		LevelDesc* LevelDesc;
		void* SomeDesc;
		LevelManager* LevelManager;
		RefMap<FixedString, void*> field_18;
		int LockCount;
		int field_2C;
		__int64 field_30;
		__int64 field_38;
		int field_40;
		Array<void*> field_48;
		Array<void*> field_60;
		CRITICAL_SECTION CriticalSection;
		__int64 field_A0;
		HANDLE EventHandle1;
		HANDLE EventHandle2;
	};


	struct Level : public ProtectedGameObject<Level>
	{
		void* VMT;
		void* LevelManager;
		void* LocalTemplateManager;
		LevelData* LevelData;
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
		VirtualMultiHashMap<FixedString, Level*> LevelData;
		Map<FixedString, Level*> Levels;
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
			void* ShroudManager;
			void* LightingOverrides;
			void* AtmosphereOverrides;
			void* EnvironmentalStatusManager;
			void* StaticPhysicsManager;
			void* TimelineSystem;
			void* TerrainManager;
			void* LevelCacheTemplateManager_M;
		};

		struct LevelManager : public bg3se::LevelManager
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
