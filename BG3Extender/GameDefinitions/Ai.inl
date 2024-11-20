#include <GameDefinitions/Ai.h>
#include <GameDefinitions/Components/Components.h>
#include <GameDefinitions/Components/Data.h>

BEGIN_SE()

void AiPath::Reset()
{
	// CoverManager = ...;
	SearchStarted = false;
	SearchComplete = false;
	GoalFound = false;
	CanUseLadders = false;
	CanUsePortals = false;
	CanUseCombatPortals = false;

	UseSmoothing = true;
	AddBoundsToMargin = true;
	AddSourceBoundsToMargin = true;

	StepHeight = 0;
	WorldClimbingHeight = 0;

	WorldClimbType = 1;
	WorldDropType = 0;

	Nodes.clear();
	Checkpoints.clear();

	Source = EntityHandle{};
	Target = EntityHandle{};
	IgnoreEntities.clear();
	MovedEntities.clear();
	PathType = 3;
	CoverFlags = 0;
	SearchHorizon = 0x7D00;
	WorldClimbType = 1;
	WorldDropType = 0;
	DangerousAuras.Auras.clear();
	DangerousAuras.Avoidance = 1;
	CollisionMask = 0x40000000400084;
	CollisionMaskMove = CollisionMask;
	CollisionMaskStand = 0x10;
	CloseEnoughMin = 0i64;
	CloseEnoughMax = 0i64;
	CloseEnoughFloor = 0i64;
	CloseEnoughPreference = 0;
	AddSourceBoundsToMargin = false;
	PreciseItemInteraction = false;
	UseSmoothing = true;
	UseSplines = true;
	UseTurning = true;
	IsPlayer = false;
}

void AiPath::SetSourceEntity(ecs::EntitySystemHelpersBase& helpers, EntityHandle entity)
{
	Source = entity;
	auto esvCharacter = helpers.GetComponent<esv::Character>(entity);
	if (esvCharacter) {
		SetSourceTemplate(esvCharacter->Template);
		IsPlayer = (esvCharacter->Flags & esv::CharacterFlags::IsPlayer) == esv::CharacterFlags::IsPlayer;
	} else {
		auto eclCharacter = helpers.GetComponent<ecl::Character>(entity);
		if (eclCharacter) {
			SetSourceTemplate(eclCharacter->Template);
			IsPlayer = (eclCharacter->Flags & ecl::CharacterFlags::IsPlayer) == ecl::CharacterFlags::IsPlayer;
		}
	}

	auto transform = helpers.GetComponent<TransformComponent>(entity);
	auto bounds = helpers.GetComponent<BoundComponent>(entity);

	BoundData* standingBound{ nullptr };
	BoundData* movingBound{ nullptr };

	if (bounds) {
		movingBound = bounds->Bound.AIBounds.try_get_ptr(AIBoundType::Move);
		standingBound = bounds->Bound.AIBounds.try_get_ptr(AIBoundType::Stand);
	}

	if (transform) {
		SetSource(transform->Transform.Translate);
	}

	if (transform && standingBound) {
		Height = transform->Transform.Scale.x * standingBound->Height * 0.65f;
	}

	if (movingBound && standingBound) {
		SetBounds(movingBound->Radius, standingBound->Radius);
	} else {
		SetBounds(0.5f, 0.5f);
	}
}

void AiPath::SetSourceTemplate(CharacterTemplate* tmpl)
{
	StepHeight = tmpl->MovementStepUpHeight.Value;
	WorldClimbingHeight = 0.0f;
	if (tmpl->IsWorldClimbingEnabled.Value) {
		WorldClimbingHeight = tmpl->WorldClimbingHeight.Value;
	}

	WorldClimbingRadius = tmpl->WorldClimbingRadius.Value;
	TurningNodeAngle = tmpl->TurningNodeAngle.Value;
	TurningNodeOffset = tmpl->TurningNodeOffset.Value;
	UseStandAtDestination = tmpl->UseStandAtDestination.Value;
}

void AiPath::SetSource(glm::vec3 position)
{
	SourceOriginal = position;
	SourceAdjusted = position;
}

void AiPath::SetTargetEntity(EntityHandle entity)
{
	Target = entity;
}

void AiPath::SetTarget(glm::vec3 position)
{
	TargetAdjusted = position;
}

void AiPath::SetBounds(float movingBound, float standingBound)
{
	MovingBound = movingBound;
	StandingBound = standingBound;
	MovingBound2 = movingBound;

	MovingBoundTiles = (int)floor(2.0f * movingBound + 0.5f - 0.001f);
	StandingBoundTiles = (int)floor(2.0f * standingBound + 0.5f - 0.001f);
	MovingBoundTiles2 = (int)floor(2.0f * movingBound + 0.5f - 0.001f);
}

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
	uint64_t gridKey = (uint32_t)(int32_t)floor(pos.Xglobal / PatchSize)
		| ((uint64_t)(uint32_t)(int32_t)floor(pos.Zglobal / PatchSize) << 32);
	auto subgrids = SubgridsAtPatch.find(gridKey);
	if (subgrids != SubgridsAtPatch.end()) {
		return std::span<AiSubgridId const>(subgrids.Value().raw_buf(), subgrids.Value().raw_buf() + subgrids.Value().size());
	} else {
		return {};
	}
}

bool AiGrid::ToTilePos(AiWorldPos const& pos, AiSubgrid*& pSubgrid, AiTilePos& tilePos, AiGridTile const*& tileInfo) const
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
					auto minY = subgrid.Value()->Translate.y + tile->GetLocalMinHeight();
					auto maxY = subgrid.Value()->Translate.y + tile->GetLocalMaxHeight();

					auto curYdiff = fabs(pos.Y - minY);
					if (curYdiff < ydiff) {
						ydiff = curYdiff;
						tilePos.SubgridId = subgridId;
						tilePos.X = localPos.x;
						tilePos.Y = localPos.y;
						tileInfo = tile;
						pSubgrid = subgrid.Value();
					}
				}
			}
		}
	}

	return ydiff < 3.40282347e+38f;
}

AiPath* AiGrid::CreatePath()
{
	AiPath* path{ nullptr };
	for (auto p : PathPool) {
		if (!p->InUse) {
			path = p;
			break;
		}
	}

	if (!path) {
		ERR("Failed to find a free AiPath");
		return nullptr;
	}

	auto handle = NextPathHandle++;
	path->Reset();
	PathMap.insert(handle, path);

	return path;

}

END_SE()
