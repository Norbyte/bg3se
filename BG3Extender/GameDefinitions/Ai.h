#pragma once

BEGIN_SE()

using AiSubgridId = uint32_t;
using AiMetaDataId = uint16_t;
using AiSurfaceMetaDataId = uint16_t;
using AiGridLayerId = uint16_t;

static constexpr AiMetaDataId AiNullMetaData = 0xffff;


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
	[[bg3::hidden]] void* PortalData; // AIPortalObjectData*
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
	uint64_t AiFlags;
	uint16_t MinHeight;
	uint16_t MaxHeight;
	AiMetaDataId MetaDataIndex;
	AiSurfaceMetaDataId SurfaceMetaDataIndex;
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


struct AiPath : public ProtectedGameObject<AiPath>
{
	// FIXME - needs to be mapped
};


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


struct AiGrid : public ProtectedGameObject<AiGrid>
{
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
	int NextPathHandle;
	int field_EC;
	Array<AiPath*> FreePaths;
	LegacyRefMap<int, AiPath*> PathMap_int_pAiPath;
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
	bool ToTilePos(AiWorldPos const& pos, AiTilePos& tilePos, AiGridTile const*& tileInfo) const;
};


END_SE()
