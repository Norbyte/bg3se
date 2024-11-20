#include <GameDefinitions/Ai.h>

/// <lua_module>Level</lua_module>
BEGIN_NS(lua::level)

AiGrid* GetAiGrid(lua_State* L)
{
	auto levelManager = State::FromLua(L)->GetExtensionState().GetLevelManager();
	return levelManager->CurrentLevel->AiGrid;
}

Array<EntityHandle> GetEntitiesOnTile(lua_State* L, glm::vec3 pos)
{
	auto aiGrid = GetAiGrid(L);
	auto worldPos = AiGrid::ToWorldPos(pos);

	Array<EntityHandle> entities;
	AiTilePos tilePos;
	AiGridTile const* tileInfo;
	AiSubgrid* subgrid;
	if (aiGrid->ToTilePos(worldPos, subgrid, tilePos, tileInfo)) {
		if (tileInfo->MetaDataIndex != AiNullMetaData) {
			auto meta = aiGrid->MetaData[tileInfo->MetaDataIndex];
			entities = meta->Entities;
		}
	}

	return entities;
}

AiGridLuaTile gTestTile;

AiGridLuaTile* GetTile(lua_State* L, glm::vec3 pos)
{
	auto aiGrid = GetAiGrid(L);
	auto worldPos = AiGrid::ToWorldPos(pos);

	AiGridLuaTile& tile = gTestTile;
	AiTilePos tilePos;
	AiGridTile const* tileInfo;
	AiSubgrid* subgrid;
	if (aiGrid->ToTilePos(worldPos, subgrid, tilePos, tileInfo)) {
		tile.Flags = tileInfo->GetFlags();
		tile.GroundSurface = tileInfo->GetGroundSurface();
		tile.CloudSurface = tileInfo->GetCloudSurface();
		tile.Material = tileInfo->GetMaterial();
		tile.UnmappedFlags = (uint32_t)(tileInfo->AiFlags & 0x200001);
		tile.ExtraFlags = (uint32_t)(tileInfo->AiFlags >> 46) & 0xffffc;

		tile.SubgridId = tilePos.SubgridId;
		tile.TileX = tilePos.X;
		tile.TileY = tilePos.Y;
		tile.MinHeight = subgrid->Translate.y + tileInfo->GetLocalMinHeight();
		tile.MaxHeight = subgrid->Translate.y + tileInfo->GetLocalMaxHeight();

		tile.MetaDataIndex = tileInfo->MetaDataIndex;
		tile.SurfaceMetaDataIndex = tileInfo->SurfaceMetaDataIndex;

		if (tileInfo->MetaDataIndex != AiNullMetaData) {
			auto meta = aiGrid->MetaData[tileInfo->MetaDataIndex];
			tile.Entities = meta->Entities;
		}
	}

	return &gTestTile;
}

AiPath* BeginPath(lua_State* L, EntityHandle source, glm::vec3 target)
{
	auto aiGrid = GetAiGrid(L);

	auto path = aiGrid->CreatePath();
	path->SetSourceEntity(*State::FromLua(L)->GetEntitySystemHelpers(), source);
	path->SetTarget(target);

	return path;
}

void RegisterLevelLib()
{
	DECLARE_MODULE(Level, Both)
	BEGIN_MODULE()
	MODULE_FUNCTION(GetEntitiesOnTile)
	MODULE_FUNCTION(GetTile)
	MODULE_FUNCTION(BeginPath)
	END_MODULE()
}

END_NS()
