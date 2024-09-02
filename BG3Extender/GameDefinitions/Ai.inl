#include <GameDefinitions/Ai.h>

BEGIN_SE()

bool AiSubgrid::WorldToTilePos(AiWorldPos const& pos, glm::ivec2& localPos) const
{
	int gX, gZ;
	if (fabs(pos.Xlocal) < 0.00000011920929f && fabs(pos.Zlocal) < 0.00000011920929f) {
		gX = pos.Xglobal - WorldPos.Xglobal;
		gZ = pos.Zglobal - WorldPos.Zglobal;
	} else {
		auto Xcalc = (int)floor(((pos.Xglobal * CellSize + pos.Xlocal) - WorldPos.Xlocal) / CellSize);
		auto Zcalc = (int)floor(((pos.Zglobal * CellSize + pos.Zlocal) - WorldPos.Zlocal) / CellSize);
		gX = Xcalc - WorldPos.Xglobal;
		gZ = Zcalc - WorldPos.Zglobal;
	}

	if (gX < 0 || gZ < 0 || gX >= SizeX || gZ >= SizeY) {
		return false;
	}

	localPos = glm::ivec2(gX, gZ);
	return true;
}

AiGridTile const* AiGridTileData::GetTileAt(int x, int y) const
{
	assert(x >= 0 && x < Width);
	assert(y >= 0 && y < Height);
	return &Tiles[x + y * Width];
}

AiWorldPos AiGrid::ToWorldPos(glm::vec3 pos)
{
	if (pos.x > 9999999.0f || pos.x < -9999999.0f
		|| pos.z > 9999999.0f || pos.z < -9999999.0f) {
		return AiWorldPos{
			.Xglobal = 0x7fff,
			.Zglobal = 0x7fff,
			.Xlocal = .0f,
			.Zlocal = .0f,
			.Y = 3.40282347e+38f
		};
	} else {
		auto xG = floorf(pos.x + pos.x);
		auto zG = floorf(pos.z + pos.z);
		return AiWorldPos{
			.Xglobal = (int)xG,
			.Zglobal = (int)zG,
			.Xlocal = pos.x - (xG * 0.5f),
			.Zlocal = pos.z - (zG * 0.5f),
			.Y = pos.y
		};
	}
}

std::span<AiSubgridId const> AiGrid::GetSubgridsAt(AiWorldPos const& pos) const
{
	uint64_t gridKey = (uint32_t)(int32_t)floor(pos.Xglobal / 25.0f)
		| ((uint64_t)(uint32_t)(int32_t)floor(pos.Zglobal / 25.0f) << 32);
	auto subgrids = SubgridsAtPatch.find(gridKey);
	if (subgrids != SubgridsAtPatch.end()) {
		return std::span<AiSubgridId const>(subgrids.Value().raw_buf(), subgrids.Value().raw_buf() + subgrids.Value().size());
	} else {
		return {};
	}
}

bool AiGrid::ToTilePos(AiWorldPos const& pos, AiTilePos& tilePos, AiGridTile const*& tileInfo) const
{
	glm::ivec2 localPos;
	float ydiff = 3.40282347e+38f;

	auto subgrids = GetSubgridsAt(pos);
	for (auto subgridId : subgrids) {
		auto subgrid = Subgrids.find(subgridId);
		if (subgrid != Subgrids.end()) {
			if (subgrid.Value()->WorldToTilePos(pos, localPos)) {
				auto tile = subgrid.Value()->TileGrid->GetTileAt(localPos.x, localPos.y);
				if ((tile->AiFlags & 1) == 0) {
					auto minY = subgrid.Value()->Translate.y + ((float)tile->MinHeight / 50.0f);
					auto maxY = subgrid.Value()->Translate.y + ((float)tile->MaxHeight / 50.0f);

					auto curYdiff = fabs(pos.Y - minY);
					if (curYdiff < ydiff) {
						ydiff = curYdiff;
						tilePos.SubgridId = subgridId;
						tilePos.X = localPos.x;
						tilePos.Y = localPos.y;
						tileInfo = tile;
					}
				}
			}
		}
	}

	return ydiff < 3.40282347e+38f;
}

END_SE()
