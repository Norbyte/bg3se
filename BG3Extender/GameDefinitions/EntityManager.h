#pragma once

#include "BaseTypes.h"

namespace dse
{
	struct LevelManager;
	struct EntityWorldBase;

	struct EntityManager : public ProtectedGameObject<EntityManager>
	{
		void* VMT;
		LevelManager* LevelManager;
		Map<FixedString, void*> field_10;
		EntityWorldBase* EntityWorld;
		FixedString FS1;
		uint64_t Unknown[3];
	};

	namespace esv
	{
		struct EntityManager;
		struct Item;
		struct Character;
		struct Trigger;
		struct Projectile;

		struct ItemConversionHelpers : ProtectedGameObject<ItemConversionHelpers>
		{
			void* VMT;
			EntityManager* EntityManager;
			Map<FixedString, ObjectSet<Item*>*> RegisteredItems;
			Map<FixedString, ObjectSet<Item*>*> ActivatedItems;
			FixedString CurrentLevel;
		};

		struct CharacterConversionHelpers : ProtectedGameObject<CharacterConversionHelpers>
		{
			EntityManager* EntityManager;
			ItemConversionHelpers* ItemConversionHelpers;
			Map<FixedString, ObjectSet<Character*>*> RegisteredCharacters;
			Map<FixedString, ObjectSet<Character*>*> ActivatedCharacters;
		};

		struct TriggerConversionHelpers : ProtectedGameObject<TriggerConversionHelpers>
		{
			EntityManager* EntityManager;
			void* TriggerFactory;
			Map<FixedString, ObjectSet<Trigger*>*> RegisteredTriggers;
		};

		struct ProjectileConversionHelpers : ProtectedGameObject<ProjectileConversionHelpers>
		{
			EntityManager* EntityManager;
			Map<FixedString, ObjectSet<Projectile*>*> RegisteredProjectiles;
		};


		struct EntityManager : public dse::EntityManager
		{
			__int64 field_48;
			Array<void*> field_50;
			__int64 field_68;
			Array<void*> field_70;
			__int64 field_88;
			Array<void*> field_90;
			__int64 field_A8;
			Array<void*> field_B0;
			ItemConversionHelpers ItemConversionHelpers;
			CharacterConversionHelpers CharacterConversionHelpers;
			TriggerConversionHelpers TriggerConversionHelpers;
			ProjectileConversionHelpers ProjectileConversionHelpers;
			void* CharacterManager;
			void* ItemManager;
			void* TriggerManager;
			void* ProjectileManager;
			void* field_188;
			void* SomeManager;
			void* TerrainManager;
			void* field_1A0;
			char field_1A8;
			__int64 Unknown4[14];
		};
	}
}
