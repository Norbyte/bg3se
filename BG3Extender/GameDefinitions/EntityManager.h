#pragma once

#include <GameDefinitions/Base/Base.h>

namespace bg3se
{
    struct LevelManager;

    struct EntityManager : public ProtectedGameObject<EntityManager>
    {
        void* VMT;
        LevelManager* LevelManager;
        LegacyMap<FixedString, void*> field_10;
        ecs::EntityWorld* EntityWorld;
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
            FixedString CurrentLevel;
        };

        struct CharacterConversionHelpers : ProtectedGameObject<CharacterConversionHelpers>
        {
            EntityManager* EntityManager;
            ItemConversionHelpers* ItemConversionHelpers;
        };

        struct TriggerConversionHelpers : ProtectedGameObject<TriggerConversionHelpers>
        {
            EntityManager* EntityManager;
            void* TriggerFactory;
            LegacyMap<FixedString, ObjectSet<Trigger*>*> RegisteredTriggers;
        };

        struct ProjectileConversionHelpers : ProtectedGameObject<ProjectileConversionHelpers>
        {
            EntityManager* EntityManager;
            LegacyMap<FixedString, ObjectSet<Projectile*>*> RegisteredProjectiles;
        };


        struct EntityManager : public bg3se::EntityManager
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
