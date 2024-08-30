#pragma once

BEGIN_SE()

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
	Array<void*> field_20;
	uint16_t SomeGlobalId;
	AiTilePos Position;
};


struct AiSurfaceMetaData
{
	std::array<int16_t, 2> PerLayerIndices;
	AiTilePos Position;
};


struct AiGridPortal
{
	void* PortalData; // AIPortalObjectData*
	AiGrid* AiGrid;
	AiTilePos SourceAIPosition;
	AiTilePos TargetAIPosition;
	bool IsAttached;
	bool IsVisited;
};


struct DataGrid : public ProtectedGameObject<DataGrid>
{
	void* VMT;
	glm::vec3 Translate;
	float CellSize;
	int SizeX;
	int SizeY;
};


template <class T>
struct AiGridData : public ProtectedGameObject<AiGridData<T>>
{
	void* LevelAllocator;
	int Width;
	int Height;
	T* Tiles;
	T* TilesEnd;
};


struct AiGridTile
{
	uint64_t AiFlags;
	uint16_t Height;
	int16_t field_A;
	int16_t MetaDataIndex;
	int16_t SurfaceMetaDataIndex;
};


struct AiInternalTile
{
	uint8_t Flags;
	uint8_t field_1;
	int16_t field_2;
	int32_t NodeId;
};


struct AiWorldPos
{
	int Xglobal;
	int Zglobal;
	float Xlocal;
	float Zlocal;
	float Y;
};


using AiSubgridId = uint32_t;

struct AiSubgrid : public DataGrid
{
	DataGrid Grid;
	FixedString SomeGuid;
	void* LevelAllocator;
	AiGridData<AiGridTile>* TileGrid;
	AiGridData<AiInternalTile>* InternalTileGrid;
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
	void* Visual; // AiGridVisual*
};


struct AiGridLayerDelta
{
	uint64_t AiFlags;
	float Height;
};

using AiGridLayerId = uint16_t;

struct AiGridLayer
{
	HashMap<AiTilePos, AiGridLayerDelta> Deltas;
	Guid field_40;
	bool Activated;
	uint16_t MetaDataIndex;
};


struct AiPath : public ProtectedGameObject<AiPath>
{
	// FIXME - needs to be mapped
};


template <class T>
struct AiGridRequestMap
{
	uint16_t NextRequestId;
	Array<T*> Requests;
	LegacyRefMap<uint16_t, T*> IdToRequest;
	Array<T*> Requests2;
};


struct AiSomeFloodObj
{
	__int64 field_0;
	Array<void*> field_8;
	Array<void*> field_18;
	__int64 field_28;
	__int64 field_30;
	__int64 field_38;
	HashMap<AiTilePos, void* /* AiFloodTile */> FloodTiles;
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
	__int64 Flags;
};


struct AiGridChangeLayersTaskDelta
{
	uint16_t Height;
	uint16_t LayerId;
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


struct AiGrid : public ProtectedGameObject<AiGrid>
{
	void* VMT;
	__int64 field_8;
	int MaxIterations;
	Array<AiMetaData*> MetaData;
	Pool AiMetaDataPool;
	Array<AiSurfaceMetaData*> SurfaceMetaData;
	Pool AiSurfaceMetaDataPool;
	Array<BoundComponent*> Objects;
	Array<AiGridPortal*> Portals;
	LegacyRefMap<uint32_t, AiSubgrid*> Subgrids;
	Array<AiGridLayer*> Layers;
	HashMap<Guid, AiGridLayerId> LayerMap;
	ecs::EntityWorld* EntityWorld;
	void* ThothMachine;
	int NextPathHandle;
	int field_EC;
	Array<AiPath*> FreePaths;
	LegacyRefMap<int, AiPath*> PathMap_int_pAiPath;
	Array<AiPath*> Paths;
	AiGridRequestMap<void*> TileStates;
	AiGridRequestMap<void*> Floods;
	AiGridRequestMap<void*> TileHeights;
	uint8_t field_1C8;
	int NumIterations;
	__int64 PreciseClock;
	__int64 field_1D8;
	__int64 field_1E0;
	Array<AiFullTile> FullTiles;
	Array<SomeArray2int> field_1F8_Arr_2xint;
	Array<SomeArray2int> field_208_Arr_2xint;
	AiIgnoreMask IgnoreMask;
	int field_248_SomeNodeId;
	int field_24C_SomeNodeId;
	int PathDistanceToClosestPortal;
	int field_254;
	Array<void*> PathHistory;
	Array<void*> PathHistory2;
	int field_278;
	int field_27C;
	void* LevelAllocator;
	FixedString UUID;
	int Version;
	void* NavigationPortalManager;
	Array<AiTilePos> CellToUpdateTextureFrom;
	bool DynamicsDirty;
	bool PaintedDirty;
	bool SurfacesDirty;
	uint8_t field_2AB;
	bool HeightDirty;
	bool LayersDirty;
	bool ModifiedDirty;
	bool GameplayLightDirty;
	glm::vec3 VisualSourcePos;
	int VisualRadius;
	bool VisualVisible;
	int field_2C4;
	void* Scene;
	int NumAiMetaData;
	int NumAiSurfaceMetaData;
	AiSomeFloodObj SomeFloodObj;
	glm::ivec2 WorldBoundMin;
	glm::ivec2 WorldBoundMax;
	glm::vec3 AiWorldMin;
	LegacyRefMap<uint64_t, Array<AiSubgridId>> Map;
	bool SubgridBoundsDirty;
	AiSomeFloodObj* field_390;
	AiSomeFloodObj* field_398;
	AiGridChangeLayersTask ChangeLayersTask;
	HashMap<AiTilePos, glm::vec2> TileHeightAndStateDirty;
	__int64 field_448;
	__int64 field_450;
};


END_SE()
