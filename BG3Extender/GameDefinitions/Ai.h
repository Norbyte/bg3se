#pragma once

BEGIN_SE()

using AiSubgridId = uint32_t;
using AiMetaDataId = uint16_t;
using AiSurfaceMetaDataId = uint16_t;
using AiGridLayerId = uint16_t;

static constexpr AiMetaDataId AiNullMetaData = 0xffff;

using AiPathId = int32_t;

struct AIPortalObjectData : public ProtectedGameObject<AIPortalObjectData>
{
    [[bg3::hidden]] void* VMT;
    EntityHandle Target;
    EntityHandle Source;
    EntityHandle PortalID;
    FixedString Level;
    FixedString TargetLevel_M;
    int PortalType;
    glm::vec3 StartPos;
    glm::vec3 StartPosAdjusted;
    glm::vec3 EndPos;
    glm::vec3 EndPosAdjusted;
    float ActionResourceCost;
    bool IsProhibitedToPlayers;
    bool IsAttached_M;
    bool field_62;
};


struct SomeArray2int
{
    int32_t A;
    int32_t B;
};


struct AiIgnoreMask
{
    AiGrid* AiGrid;
    uint64_t AggregateFlags;
    Array<uint64_t> AiFlags;
    bool Restore;
    uint64_t NextId;
};

struct AiGridPortal;

struct AiMetaData
{
    Array<EntityHandle> Entities;
    Array<AiGridPortal*> Portals;
    Array<AiGridPortal*> EndPortals;
    AiGridLayerId LayerId;
    AiTilePos Position;
};


struct AiSurfaceMetaData
{
    std::array<int16_t, 2> PerLayerIndices;
    AiTilePos Position;
};


struct AiGridPortal
{
    AIPortalObjectData* PortalData;
    AiGrid* AiGrid;
    AiTilePos SourceAIPosition;
    AiTilePos TargetAIPosition;
    bool IsAttached;
    bool IsVisited;
};


struct DataGrid : public ProtectedGameObject<DataGrid>
{
    [[bg3::hidden]] void* VMT;
    glm::vec3 Translate;
    float CellSize;
    int SizeX;
    int SizeY;
};


struct AiGridTile
{
    static constexpr float HeightScale = 1.0f/50.0f;

    uint64_t AiFlags;
    uint16_t MaxHeight;
    uint16_t MinHeight;
    AiMetaDataId MetaDataIndex;
    AiSurfaceMetaDataId SurfaceMetaDataIndex;

    inline float GetLocalMinHeight() const
    {
        return (float)MinHeight * HeightScale;
    }

    inline float GetLocalMaxHeight() const
    {
        return (float)MaxHeight * HeightScale;
    }

    inline AiBaseFlags GetFlags() const
    {
        return (AiBaseFlags)(AiFlags & 0xffffff);
    }

    inline SurfaceType GetGroundSurface() const
    {
        return (SurfaceType)((AiFlags >> 24) & 0xff);
    }

    inline SurfaceType GetCloudSurface() const
    {
        return (SurfaceType)((AiFlags >> 32) & 0xff);
    }

    inline uint8_t GetMaterial() const
    {
        return (uint8_t)((AiFlags >> 40) & 0x3f);
    }

    inline uint32_t GetExtraFlags() const
    {
        return (uint32_t)(AiFlags >> 46);
    }
};


struct AiInternalTile
{
    uint8_t Flags;
    uint8_t field_1;
    int16_t field_2;
    int32_t NodeId;
};


struct AiGridTileData : public ProtectedGameObject<AiGridTileData>
{
    [[bg3::hidden]] void* LevelAllocator;
    int Width;
    int Height;
    AiGridTile* Tiles;
    AiGridTile* TilesEnd;

    AiGridTile const* GetTileAt(int x, int y) const;
};


struct AiGridInternalTileData : public ProtectedGameObject<AiGridInternalTileData>
{
    [[bg3::hidden]] void* LevelAllocator;
    int Width;
    int Height;
    AiInternalTile* Tiles;
    AiInternalTile* TilesEnd;
};


struct AiWorldPos
{
    int Xglobal;
    int Zglobal;
    float Xlocal;
    float Zlocal;
    float Y;
};

struct AiSubgrid : public DataGrid
{
    FixedString SomeGuid;
    [[bg3::hidden]] void* LevelAllocator;
    AiGridTileData* TileGrid;
    AiGridInternalTileData* InternalTileGrid;
    AiWorldPos WorldPos;
    std::array<uint16_t, 2> TraversedTiles0;
    std::array<uint16_t, 2> TraversedTiles1;
    FixedString GUID;
    glm::vec3 RelativePlatformPosition;
    bool LoadedExternally;
    bool IsActive;
    HashMap<EntityHandle, uint64_t> AttachedObjectRefCounts;
    Array<glm::ivec2> field_B0;
    bool PatchesDirty;
    [[bg3::hidden]] void* Visual; // AiGridVisual*

    bool WorldToTilePos(AiWorldPos const& pos, glm::ivec2& localPos) const;
};


struct AiGridLayerDelta
{
    uint64_t AiFlags;
    float Height;
};


struct AiGridLayer
{
    HashMap<AiTilePos, AiGridLayerDelta> Deltas;
    Guid field_40;
    bool Activated;
    AiMetaDataId MetaDataIndex;
};


struct AiPath;


template <class T>
struct [[bg3::hidden]] AiGridRequestMap
{
    uint16_t NextRequestId;
    Array<T*> Requests;
    LegacyRefMap<uint16_t, T*> IdToRequest;
    Array<T*> Requests2;
};


struct AiSomeFloodObj
{
    __int64 field_0;
    [[bg3::hidden]] StaticArray<void*> field_8;
    [[bg3::hidden]] StaticArray<void*> field_18;
    __int64 field_28;
    __int64 field_30;
    __int64 field_38;
    [[bg3::hidden]] HashMap<AiTilePos, void* /* AiFloodTile */> FloodTiles;
};


struct AiFullTile
{
    AiTilePos Position;
    int AoOCost;
    int field_C;
    int CollidingCount;
    int field_14;
    EntityHandle Portal;
    int field_20;
    int Sibling;
    //__int64 Flags;
};


struct AiGridChangeLayersTaskDelta
{
    uint16_t Height;
    AiGridLayerId LayerId;
    __int64 AiFlags;
    __int64 AiFlags2;
    __int64 field_18;
};


struct AiGridChangeLayersTask
{
    __int64 field_0;
    Array<Guid> Layers;
    Array<Guid> Layers2;
    HashMap<AiTilePos, AiGridChangeLayersTaskDelta> Deltas;
};


struct AoERangeFilterTargetData
{
    glm::vec3 Position;
    float Radius;
    float HeightRange;
};


struct DangerousAuras
{
    AoERangeFilterTargetData Target;
    int field_14;
    Array<AoERangeFilterTargetData> Auras;
    uint8_t Avoidance;
};


struct AiPathAoOPosition
{
    int field_0;
    int field_4;
    int field_8;
    int field_C;
};


struct AiPathEntityPosition
{
    int field_0;
    int field_4;
    int field_8;
    int field_C;
};


struct AiPathNode
{
    glm::vec3 Position;
    EntityHandle Portal;
    float Distance;
    float DistanceModifier;
    uint8_t Flags;
};


struct AiPathCheckpoint : public ProtectedGameObject<AiPathCheckpoint>
{
    [[bg3::hidden]] void* field_0;
    glm::vec3 Position;
    EntityHandle Portal;
    EntityHandle field_20;
    uint8_t Flags;
};



struct AiPath : public ProtectedGameObject<AiPath>
{
    Array<SurfacePathInfluence> SurfacePathInfluences;
    EntityHandle Source;
    EntityHandle Target;
    float MovingBound;
    float StandingBound;
    uint64_t CollisionMask;
    uint64_t CollisionMaskMove;
    uint64_t CollisionMaskStand;
#if 0
    // STDString CallerLocation;
    // int CallerLine;
    // __int64 field_60;
    // __int64 field_68;
#endif
    float MovingBound2;
    float CloseEnoughMin;
    float CloseEnoughMax;
    float CloseEnoughFloor;
    float CloseEnoughCeiling;
    float CloseEnoughPreference;
    int PathType;
    uint16_t CoverFlags;
    int field_90;
    glm::vec3 SourceAdjusted;
    glm::vec3 SourceOriginal;
    glm::vec3 TargetAdjusted;
    glm::vec3 ProjectileTarget;
    float Height;
    int16_t SearchHorizon;
    int16_t SearchHorizon2;
#if 0
    // int field_CC;
#endif
    bool IsPlayer;
    bool CanUseLadders;
    bool CanUsePortals;
    bool CanUseCombatPortals;
    bool CheckLockedDoors;
    HashSet<FixedString> AvailableKeys;
    bool UseSmoothing;
    bool AddBoundsToMargin;
    bool AddSourceBoundsToMargin;
    float StepHeight;
    float WorldClimbingHeight;
    float WorldClimbingRadius;
    float TurningNodeAngle;
    float TurningNodeOffset;
    float FallDamageMinimumDistance;
    float FallDamageDistanceEstimate;
    float FallMinDamagePathfindingCost;
    float FallMaxDamagePathfindingCost;
    int FallDeadPathfindingCost;
    uint8_t WorldClimbType;
    uint8_t WorldDropType;
    uint8_t field_136;
    bool IsBidirectionalSearch;
    bool UseTurning;
    bool PreciseItemInteraction;
    bool UseSplines;
    bool UseStandAtDestination;
    bool PickUpTarget;
    [[bg3::hidden]] void* CoverManager;
    EntityHandle field_148;
    uint8_t field_150;
    int field_154;
    Array<AiPathEntityPosition> MovedEntities;
    Array<EntityHandle> IgnoreEntities;
    [[bg3::legacy(field_178)]] glm::vec3 TargetPosition;
    [[bg3::hidden]] UnknownFunction* pDestinationFunc;
    [[bg3::hidden]] UnknownFunction DestinationFunc;
    [[bg3::hidden]] UnknownFunction* pWeightFunc;
    [[bg3::hidden]] UnknownFunction WeightFunc;
    HashMap<AiTilePos, uint64_t> AoOTiles;
    Array<AiPathAoOPosition> AoOPositions;
    DangerousAuras DangerousAuras;
    int MovingBoundTiles;
    int StandingBoundTiles;
    int MovingBoundTiles2;
    int field_294;
    int32_t field_298;
    float field_29C;
    bool SearchStarted;
    bool SearchComplete;
    bool GoalFound;
    uint8_t field_2A3;
    bool InUse;
    Array<AiPathNode> Nodes;
    Array<AiPathCheckpoint> Checkpoints;
    uint64_t LimitNodeIndex;
    bool HasLimitNode;
    uint32_t ErrorCause;

    void Reset();
    void SetSourceEntity(ecs::EntitySystemHelpersBase& helpers, EntityHandle entity);
    void SetSourceTemplate(CharacterTemplate* tmpl);
    void SetSource(glm::vec3 position);
    void SetTargetEntity(EntityHandle entity);
    void SetTarget(glm::vec3 position);
    void SetBounds(float movingBound, float standingBound);
};


struct AiGrid : public ProtectedGameObject<AiGrid>
{
    static constexpr float PatchSize = 25.0f;

    using FindPathProc = void (AiGrid* self, AiPathId pathId);
    using FindPathImmediateProc = bool (AiGrid* self, AiPathId pathId);

    [[bg3::hidden]] void* VMT;
    __int64 field_8;
    int MaxIterations;
    Array<AiMetaData*> MetaData;
    [[bg3::hidden]] Pool AiMetaDataPool;
    Array<AiSurfaceMetaData*> SurfaceMetaData;
    [[bg3::hidden]] Pool AiSurfaceMetaDataPool;
    Array<BoundComponent*> Objects;
    Array<AiGridPortal*> Portals;
    LegacyRefMap<AiSubgridId, AiSubgrid*> Subgrids;
    Array<AiGridLayer*> Layers;
    HashMap<Guid, AiGridLayerId> LayerMap;
    [[bg3::hidden]] ecs::EntityWorld* EntityWorld;
    [[bg3::hidden]] void* ThothMachine;
    AiPathId NextPathHandle;
    int field_EC;
    Array<AiPath*> PathPool;
    LegacyRefMap<AiPathId, AiPath*> PathMap;
    Array<AiPath*> Paths;
    [[bg3::hidden]] AiGridRequestMap<void*> TileStates;
    [[bg3::hidden]] AiGridRequestMap<void*> Floods;
    [[bg3::hidden]] AiGridRequestMap<void*> TileHeights;
    uint8_t field_1C8;
    int NumIterations;
    __int64 PreciseClock;
    __int64 field_1D8;
    __int64 field_1E0;
    Array<AiFullTile> FullTiles;
    StaticArray<SomeArray2int> field_1F8_Arr_2xint;
    StaticArray<SomeArray2int> field_208_Arr_2xint;
    AiIgnoreMask IgnoreMask;
    int field_248_SomeNodeId;
    int field_24C_SomeNodeId;
    int PathDistanceToClosestPortal;
    int field_254;
    //[[bg3::hidden]] Array<void*> PathHistory;
    //[[bg3::hidden]] Array<void*> PathHistory2;
    //int field_278;
    //int field_27C;
    [[bg3::hidden]] void* LevelAllocator;
    FixedString UUID;
    int Version;
    [[bg3::hidden]] void* NavigationPortalManager;
    //Array<AiTilePos> CellToUpdateTextureFrom;
    //bool DynamicsDirty;
    //bool PaintedDirty;
    //bool SurfacesDirty;
    //uint8_t field_2AB;
    //bool HeightDirty;
    //bool LayersDirty;
    //bool ModifiedDirty;
    //bool GameplayLightDirty;
    int field_298;
    int field_29C;
    int field_2A0;
    int field_2A4;
    glm::vec3 VisualSourcePos;
    int VisualRadius;
    //bool VisualVisible;
    //int field_2C0;
    //int field_2C4;
    //[[bg3::hidden]] void* Scene;
    //int NumAiMetaData;
    //int NumAiSurfaceMetaData;
    //AiSomeFloodObj SomeFloodObj;
    //glm::ivec2 WorldBoundMin;
    //glm::ivec2 WorldBoundMax;
    //glm::vec3 AiWorldMin;
    LegacyRefMap<uint64_t, Array<AiSubgridId>> SubgridsAtPatch;
    bool SubgridBoundsDirty;
    AiSomeFloodObj* field_390;
    AiSomeFloodObj* field_398;
    AiGridChangeLayersTask ChangeLayersTask;
    HashMap<AiTilePos, glm::vec2> TileHeightAndStateDirty;
    __int64 field_448;
    __int64 field_450;

    static AiWorldPos ToWorldPos(glm::vec3 pos);
    std::span<AiSubgridId const> GetSubgridsAt(AiWorldPos const& pos) const;
    bool ToTilePos(AiWorldPos const& pos, AiSubgrid*& pSubgrid, AiTilePos& tilePos, AiGridTile const*& tileInfo) const;
    Array<float> GetHeightsAt(AiWorldPos const& pos) const;
    AiPath* CreatePath();
    std::optional<AiPathId> GetPathId(AiPath* path);
    void FreePath(AiPath* path);
};


struct AiGridLuaTile
{
    AiBaseFlags Flags;
    SurfaceType GroundSurface;
    SurfaceType CloudSurface;
    uint8_t Material;
    uint32_t UnmappedFlags;
    uint32_t ExtraFlags;
    uint32_t SubgridId;
    int16_t TileX;
    int16_t TileY;
    float MinHeight;
    float MaxHeight;
    uint16_t MetaDataIndex;
    uint16_t SurfaceMetaDataIndex;
    Array<EntityHandle> Entities;
};


END_SE()
