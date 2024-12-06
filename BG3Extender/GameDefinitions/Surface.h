#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Misc.h>

BEGIN_SE()

struct SurfaceMetaData
{
    Guid TeamId;
    EntityHandle field_10;
    EntityHandle field_18;
    float LifeTime;
    uint8_t Level;
};

END_SE()

namespace bg3se::esv
{
    struct Level;
    struct Surface;
    struct SurfaceManager;

    struct SurfaceCell
    {
        glm::i16vec2 Position;
    };

    struct SubSurface : ProtectedGameObject<SubSurface>
    {
        Surface* Surface;
        uint64_t SurfaceStateFlags;
        PrimitiveSmallSet<SurfaceCell> Cells;
        PrimitiveSmallSet<SurfaceCell> GrowCells;
        uint16_t SurfaceIndex;
        int16_t SurfaceConcentrationTarget_M;
        AiTilePos Pos;
    };

    struct Surface : ProtectedGameObject<Surface>
    {
        EntityHandle field_0;
        EntityHandle field_8;
        SurfaceType Type;
        uint8_t Flags;
        SurfaceMetaData Meta;
        bool HasLifeTime;
        bool IsControlledByConcentration;
        Array<AiTilePos> NeighbourMergeCells;
        std::array<EntityHandle, 14> TransformActions;
        int16_t Index;
        int16_t SurfaceConcentrationTarget;
        SurfaceManager* SurfaceManager;
        bool NeedsSplitEvaluation;
        float OwnershipTimer;
        int StoryActionID;
        ActionOriginator Originator;
        AbilityId SpellCastingAbility;
        Guid SpellCastSourceUuid;
        Guid Parent;
        Array<SubSurface*> SubSurfaces;
    };

    struct SurfaceAction;

    struct SurfaceActionVMT : ProtectedGameObject<SurfaceActionVMT>
    {
        void (*Destroy)(SurfaceAction*, bool);
        SurfaceActionType(*GetTypeId)(SurfaceAction*);
        bool (*Visit)(SurfaceAction* self, ObjectVisitor* visitor);
        int* (*Unknown)(SurfaceAction* self, int*, bool);
        void (*Enter)(SurfaceAction* self);
        void (*Update)(SurfaceAction* self, void* time);
        void (*Unknown2)(SurfaceAction* self);
        void (*Reset)(SurfaceAction* self);
        bool (*IsFinished)(SurfaceAction* self);
        bool (*Unknown3)(SurfaceAction* self);
    };

    struct SurfaceAction : ProtectedGameObject<SurfaceAction>
    {
        SurfaceActionVMT* VMT;
        Level* Level;
        int StoryActionID;
        ActionOriginator Originator;
        resource::GuidResourceBankBase* ClassDescriptionMgr;
        EntityHandle Handle;
    };

    struct CreateSurfaceActionBase : public SurfaceAction
    {
        ecs::EntityRef Owner;
        float Duration;
        bool IsControlledByConcentration;
        glm::vec3 Position;
        SurfaceType SurfaceType;
        EntityHandle SurfaceHandlesByType[(unsigned)SurfaceType::Sentinel];
        Array<SurfaceCell>* SurfaceChanges[(unsigned)SurfaceType::Sentinel];
        Array<SurfaceCell>* SurfaceCellsByLayer[2];
    };

    struct CreateSurfaceAction : public CreateSurfaceActionBase
    {
        bool InitialChangesPushed;
        float Radius;
        float ExcludeRadius;
        float MaxHeight;
        bool IgnoreIrreplacableSurfaces;
        bool CheckExistingSurfaces;
        uint64_t LineCheckBlock;
        float Timer;
        float GrowTimer;
        int GrowStep;
        int CurrentCellCount;
        Array<SurfaceCell> SurfaceCells;
        SurfaceLayer8 SurfaceLayer;
        uint8_t field_491;
        SpellId SpellId;
        uint16_t SurfaceConcentrationTarget;
    };

    struct CreatePuddleAction : public CreateSurfaceActionBase
    {
        int SurfaceCells;
        int Step;
        float GrowSpeed;
        int field_454;
        bool IsFinished;
        bool IgnoreIrreplacableSurfaces;
        __int64 CellAtGrow[3];
        __int64 ClosedCells[3];
        float GrowTimer;
    };

    struct ExtinguishFireAction : public CreateSurfaceActionBase
    {
        glm::vec3 ExtinguishPosition;
        float Radius;
        float Percentage;
        float ExtinguishGrowTimer;
        float Step;
        Array<void*> field_468;
        __int64 SomeFixedArray[3];
    };

    struct ZoneActionParams
    {
        int Shape;
        float Radius;
        float ZoneParam;
        float FrontOffset;
        float MaxHeight;
        float Height;
        uint8_t Flags;
    };

    struct ZoneAction : public CreateSurfaceActionBase
    {
        SpellId Spell;
        FixedString TextKey;
        glm::vec3 Target;
        ZoneActionParams Params;
        int field_49C;
        int GrowStep;
        __int64 field_4A8;
        int field_4B0;
        Array<SurfaceCell> SurfaceCells;
        Array<ecs::EntityRef> Targets;
        Array<void*> field_4E8;
        int64_t CurrentCellCount;
        uint8_t Flags;
    };

    struct TransformSurfaceAction : public SurfaceAction
    {
        float Timer;
        uint8_t SurfaceTransformAction; // FIXME enum
        uint8_t OriginSurface; // FIXME enum
        SurfaceLayer8 SurfaceLayer;
        float GrowCellPerSecond;
        bool Finished;
        void* CellSearcher;
        ecs::EntityRef OwnerHandle;
        glm::vec3 Position;
        float SurfaceLifetime;
        LegacyRefMap<SurfaceType, ComponentHandle> SurfaceMap;
        LegacyRefMap<SurfaceType, PrimitiveSet<SurfaceCell>> SurfaceCellMap;
        ObjectSet<SurfaceCell> SurfaceRemoveGroundCellMap;
        ObjectSet<SurfaceCell> SurfaceRemoveCloudCellMap;
        bool PlayerCharacterNearby;
        __int64 field_C0;
        __int64 field_C8;
        __int64 field_D0;
        int field_D8;
        int field_DC;
        __int64 field_E0;
    };

    struct ChangeSurfaceOnPathAction : public CreateSurfaceActionBase
    {
        ecs::EntityRef FollowHandle;
        float Radius;
        bool IsFinished;
        Array<SurfaceCell> SurfaceCells;
        bool IgnoreIrreplacableSurfaces;
        bool CheckExistingSurfaces;
        bool IgnoreOwnerCells;
    };

    struct RectangleSurfaceAction : public CreateSurfaceActionBase
    {
        glm::vec3 Target;
        int field_458;
        float SurfaceArea_M;
        float Width;
        float Length;
        float GrowTimer;
        float MaxHeight;
        int GrowStep;
        ObjectSet<void*> DamageList;
        stats::DeathType DeathType;
        uint64_t LineCheckBlock;
        void* SkillProperties_M;
        int CurrentGrowTimer_M;
        Array<SurfaceCell> SurfaceCells;
        ObjectSet<ecs::EntityRef> Characters;
        ObjectSet<ecs::EntityRef> Items;
        int CurrentCellCount;
    };

    struct PolygonSurfaceAction : public CreateSurfaceActionBase
    {
        FixedString field_448;
        int field_44C;
        ObjectSet<glm::vec2> PolygonVertices;
        int field_468;
        ObjectSet<void*> DamageList;
        uint8_t field_488;
        __int64 field_490;
        float CurrentGrowTimer;
        float GrowTimer;
        glm::vec3 SomePosition;
        int GrowStep;
        int LastSurfaceCellCount;
        int field_4B4;
        Array<SurfaceCell> SurfaceCells;
        ObjectSet<EntityHandle> Characters;
        ObjectSet<EntityHandle> Items;
        uint8_t field_500;
    };

    /*struct SurfaceActionFactory : public ComponentFactoryBase
    {
        using CreateActionProc = SurfaceAction * (SurfaceActionFactory* self, SurfaceActionType actionType, 
            resource::GuidResourceBankBase* classDefMgr, uint64_t actionHandle);
    };

    struct SurfaceManager : public ComponentFactoryBase
    {
        using AddActionProc = void(SurfaceManager* self, SurfaceAction* action);

        struct Unkn
        {
            int64_t field_0[3];
        };

        int64_t field_78;
        int64_t field_80;
        Unkn field_88[17];
        esv::Level* Level;
        Array<SurfaceAction*> SurfaceActions;
        int64_t field_240;
        Array<Surface*> Surfaces;
        int64_t field_260;
        ObjectSet<void*> SurfaceConcentrationTargets;
        int64_t field_280[16];
        ObjectSet<void*> pAiTilePosSets;
        LegacyRefMap<void*, void*> field_318;
        LegacyRefMap<void*, void*> field_328;
        int64_t field_338[10];
    };*/
}
