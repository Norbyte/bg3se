#pragma once

#include <GameDefinitions/Base/Base.h>

namespace bg3se
{
    struct EntityManager;

    struct LevelDesc : public ProtectedGameObject<LevelDesc>
    {
        void* VMT;
        FixedString LevelName;
        FixedString UniqueKey;
        STDString CustomDisplayLevelName;
        ObjectSet<Path> Paths;
        uint8_t Type;
    };

    struct LevelData
    {
        LevelDesc* LevelDesc;
        void* SomeDesc;
        LevelManager* LevelManager;
        LegacyRefMap<FixedString, void*> field_18;
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


    struct LevelBase : public ProtectedGameObject<LevelBase>
    {
        void* VMT;
        void* LevelManager;
        void* LevelAllocator;
        LevelData* LevelData;
        LocalTemplateManager* LocalTemplateManager;
        void* Scene;
        phx::PhysicsSceneBase* PhysicsScene;
        FixedString LevelGuid;
    };


    struct LSLevel : public LevelBase
    {
        EntityHandle Root;
        uint32_t field_48;
        Array<void*> ActiveLevelTemplates;
        Array<void*> LocalTemplateTraces;
        // void* LayerManager;
        uint32_t LoadState;
    };


    struct EoCLevel : public LSLevel
    {
        void* EntityManager;
        AiGrid* AiGrid;
        void* CoverManager;
    };

    struct LevelManagerBase
    {
        uint64_t Unknown[0x10];
    };

    struct LevelManager : public ProtectedGameObject<LevelManager>
    {
        void* VMT;
        LevelManagerBase b;
        LocalTemplateManager* LocalTemplateManager;
        EoCLevel* CurrentLevel;
        LegacyMap<FixedString, EoCLevel*> Levels;
        void* field_B0;
    };


    namespace esv
    {
        struct SurfaceManager;

        struct Level : public EoCLevel
        {
            void* SightManager;
            void* CharacterManager;
            void* ItemManager;
            void* SurfaceManager;
            void* GameActionManager;
            void* EffectManager;
            void* ShroudManager;
            void* LightingOverrides;
            void* AtmosphereOverrides;
            void* EnvironmentalStatusManager;
            void* StaticPhysicsManager;
            CacheTemplateManagerBase* CacheTemplateManager;
        };

        struct LevelManager : public bg3se::LevelManager
        {
            Array<void*> field_B8;
            ecs::EntityWorld* EntityWorld;
        };
    }


    namespace ecl
    {
        struct Level : public EoCLevel
        {
            // TODO
        };

        struct LevelManager : public bg3se::LevelManager
        {
        };
    }
}
