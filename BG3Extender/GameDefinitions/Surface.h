#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Misc.h>
#include <GameDefinitions/ObjectFactory.h>

BEGIN_SE()

struct SurfaceMetaData
{
    Guid TeamId;
    ComponentHandle SurfaceHandle;
    EntityHandle Owner;
    float LifeTime;
    uint8_t Level;
};

END_SE()

BEGIN_NS(esv)

struct SubSurface : ProtectedGameObject<SubSurface>
{
    Surface* Surface;
    uint64_t SurfaceStateFlags;
    Array<AiTilePos> Cells;
    Array<AiTilePos> GrowCells;
    [[bg3::readonly]] uint16_t SurfaceIndex;
    AiTilePos Pos;
};

struct Surface : ProtectedGameObject<Surface>
{
    ComponentHandle SurfaceHandle;
    EntityHandle SurfaceEntity;
    SurfaceType Type;
    uint8_t Flags;
    SurfaceMetaData Meta;
    bool HasLifeTime;
    bool IsControlledByConcentration;
    Array<AiTilePos> NeighbourMergeCells;
    std::array<EntityHandle, 14> TransformActions;
    [[bg3::readonly]] int16_t Index;
    [[bg3::readonly]] int16_t SurfaceConcentrationTarget;
    [[bg3::hidden]] SurfaceManager* SurfaceManager;
    bool NeedsSplitEvaluation;
    float OwnershipTimer;
    int StoryActionID;
    ActionOriginator Originator;
    AbilityId SpellCastingAbility;
    Guid SpellCastSourceUuid;
    Guid Parent;
    Array<SubSurface*> SubSurfaces;
};

struct SurfaceAction : ProtectedGameObject<SurfaceAction>
{
    virtual ~SurfaceAction() = 0;
    virtual SurfaceActionType GetTypeId() = 0;
    virtual bool Visit(ObjectVisitor* visitor) = 0;
    virtual void Unknown4() = 0;
    virtual SurfaceType GetSurfaceType() = 0;
    virtual EntityHandle GetOwner() = 0;
    virtual glm::vec3 const& GetPosition() = 0;
    virtual Guid* GetTeam(Guid& teamId, bool) = 0;
    virtual Guid* GetCombat(Guid& combatId, bool) = 0;
    virtual std::optional<uint16_t> GetSurfaceConcentrationTarget() = 0;
    virtual void BreakConcentration() = 0;
    virtual void Enter() = 0;
    virtual void Update(GameTime const& time) = 0;
    virtual void Exit() = 0;
    virtual bool IsFinished() = 0;
    virtual void SetPosition(glm::vec3 const&) = 0;
    virtual void Unknown16() = 0;
    virtual void ApplySurfaceChanges() = 0;

    [[bg3::hidden]] Level* Level;
    int StoryActionID;
    ActionOriginator Originator;
    [[bg3::hidden]] resource::GuidResourceBankBase* ClassDescriptionMgr;
    EntityHandle Handle;
};

struct CreateSurfaceActionBase : public SurfaceAction
{
    EntityHandle Owner;
    float Duration;
    bool IsControlledByConcentration;
    glm::vec3 Position;
    SurfaceType SurfaceType;
    std::array<ComponentHandle, (unsigned)SurfaceType::Sentinel> SurfaceHandlesByType;
    std::array<Array<AiTilePos>*, (unsigned)SurfaceType::Sentinel> SurfaceChanges;
    std::array<Array<AiTilePos>*, 2> SurfaceCellsByLayer;
};

struct CreateSurfaceAction : public CreateSurfaceActionBase
{
    static constexpr SurfaceActionType Type = SurfaceActionType::CreateSurface;

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
    Array<AiTilePos> SurfaceCells;
    SurfaceLayer8 SurfaceLayer;
};

struct CreatePuddleAction : public CreateSurfaceActionBase
{
    static constexpr SurfaceActionType Type = SurfaceActionType::CreatePuddle;

    int SurfaceCells;
    int Step;
    float GrowSpeed;
    int field_454;
    bool IsFinished;
    bool IgnoreIrreplacableSurfaces;
    Array<AiTilePos> CellAtGrow;
    Array<AiTilePos> ClosedCells;
    float GrowTimer;
};

struct ForceCreateSurfaceAction : public CreateSurfaceActionBase
{
    static constexpr SurfaceActionType Type = SurfaceActionType::ForceCreateSurface;
};

struct CapsuleSurfaceAction : public CreateSurfaceActionBase
{
    static constexpr SurfaceActionType Type = SurfaceActionType::CapsuleSurface;

    glm::vec3 AnimationCenter;
    glm::vec3 End;
    float Radius;
    float ElapsedTime;
    float AnimationDuration;
    HashSet<AiTilePos> Cells;
};

struct RemoveSurfaceAction : public CreateSurfaceActionBase
{
    static constexpr SurfaceActionType Type = SurfaceActionType::RemoveSurface;

    bg3se::SurfaceType RemoveSurfaceType;
    glm::vec3 RemovePosition;
    float Radius;
    float Percentage;
    float GrowTimer;
    float Step;
    int TotalCellCount;
    float CurrentGrowTimer;
    Array<AiTilePos> Cells;
    Array<AiTilePos> GrowCells;
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
    static constexpr SurfaceActionType Type = SurfaceActionType::Zone;

    SpellId Spell;
    FixedString TextKey;
    glm::vec3 Target;
    ZoneActionParams Params;
    float GrowTimer;
    int GrowStep;
    [[bg3::hidden]] void* SpellProperties;
    float CurrentGrowTimer;
    Array<AiTilePos> Cells;
    Array<EntityHandle> Targets;
    Array<HitDesc> Hits;
    int64_t CurrentCellCount;
    uint8_t Flags;
};

struct TransformSurfaceAction : public SurfaceAction
{
    static constexpr SurfaceActionType Type = SurfaceActionType::TransformSurface;

    float Timer;
    uint8_t SurfaceTransformAction; // FIXME enum
    uint8_t OriginSurface; // FIXME enum
    SurfaceLayer8 SurfaceLayer;
    float GrowCellPerSecond;
    bool Finished;
    [[bg3::hidden]] void* CellSearcher;
    EntityHandle OwnerHandle;
    glm::vec3 Position;
    float SurfaceLifetime;
    LegacyRefMap<SurfaceType, ComponentHandle> SurfaceMap;
    LegacyRefMap<SurfaceType, Array<AiTilePos>> SurfaceCellMap;
    Array<AiTilePos> SurfaceRemoveGroundCellMap;
    Array<AiTilePos> SurfaceRemoveCloudCellMap;
    bool PlayerCharacterNearby;
    Array<SurfaceType> SurfaceTypes;
    LegacyRefMap<SurfaceType, SurfaceType> SurfaceTypeMap;
};

struct ChangeSurfaceOnPathAction : public CreateSurfaceActionBase
{
    static constexpr SurfaceActionType Type = SurfaceActionType::ChangeSurfaceOnPath;

    EntityHandle FollowHandle;
    float Radius;
    bool IsFinished;
    Array<AiTilePos> Cells;
    bool IgnoreIrreplacableSurfaces;
    bool CheckExistingSurfaces;
    bool IgnoreOwnerCells;
    bool field_5C3;
};

struct RectangleSurfaceAction : public CreateSurfaceActionBase
{
    static constexpr SurfaceActionType Type = SurfaceActionType::RectangleSurface;

    glm::vec3 Target;
    float SurfaceArea_M;
    float Width;
    float Length;
    float GrowTimer;
    float MaxHeight;
    int GrowStep;
    [[bg3::hidden]] ObjectSet<void*> DamageList;
    stats::DeathType DeathType;
    uint64_t LineCheckBlock;
    Array<AiTilePos> Cells;
    ObjectSet<EntityHandle> Characters;
    ObjectSet<EntityHandle> Items;
    int CurrentCellCount;
};

struct PolygonSurfaceAction : public CreateSurfaceActionBase
{
    static constexpr SurfaceActionType Type = SurfaceActionType::PolygonSurface;

    FixedString field_448;
    Array<glm::vec3> PolygonVertices;
    int field_468;
    [[bg3::hidden]] Array<void*> DamageList;
    uint8_t field_488;
    __int64 field_490;
    float CurrentGrowTimer;
    float GrowTimer;
    int GrowStep;
    int LastSurfaceCellCount;
    Array<AiTilePos> SurfaceCells;
    Array<EntityHandle> Characters;
    Array<EntityHandle> Items;
    uint8_t field_500;
    bool PolygonCreated;
};

struct [[bg3::hidden]] SurfaceAiFlood : public AiFlood
{
    AiTilePos field_68;
    __int64 field_70;
    __int64 field_78;
    __int16 field_80;
    Array<AiTilePos> Tiles;
};

struct SurfaceCreatedEventRequest : ProtectedGameObject<SurfaceCreatedEventRequest>
{
    Surface* Surface;
    bool NewlyCreated;
};

struct SurfaceRemovedEventRequest
{
    EntityHandle field_0;
    EntityHandle field_8;
    Guid SpellCastUuid;
};

struct SurfacePosition
{
    ComponentHandle Surface;
    glm::vec3 Position;
};

struct SurfaceConcentrationTarget
{
    HashSet<ComponentHandle> Surfaces;
    uint16_t Index;
};

struct SurfaceTransformActionRequirement
{
    uint8_t Type; // SurfaceTransformRequirementType
    std::variant<SurfaceType, uint8_t /*SurfaceModifierType*/, int8_t /*SurfaceCategory*/> Requirement;
};

struct SurfaceTransformActionRequirements
{
    uint8_t byte0;
    Array<SurfaceTransformActionRequirement> Requirements;
};


struct [[bg3::hidden]] SurfaceManager : public ObjectFactory<Surface>
{
    void* VMT2;
    __int64 field_68;
    std::array<Array<SurfaceTransformActionRequirements>, 16> TransformActionRequirements;
    Level* Level;
    Array<SurfaceAction*> Actions;
    LegacyArray<Surface*> Surfaces;
    LegacyArray<SurfaceConcentrationTarget*> SurfaceConcentrationTargets;
    Array<AiTilePos> RemoveCells;
    SurfaceAiFlood AiFlood;
    Array<Array<AiTilePos>> HandledCellTransitions;
    LegacyRefMap<FixedString, Array<AiTilePos>> Chasms;
    Array<SurfacePosition> CheckFTBTeamRequests;
    Array<SurfaceCreatedEventRequest> CreatedEvents;
    Array<SurfaceRemovedEventRequest> RemovedEvents;
};

END_NS()

BEGIN_NS(lua)

LUA_POLYMORPHIC(esv::SurfaceAction)

END_NS()
