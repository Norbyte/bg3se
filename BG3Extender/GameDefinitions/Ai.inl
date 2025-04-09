#include <GameDefinitions/Ai.h>
#include <GameDefinitions/Components/Components.h>
#include <GameDefinitions/Components/Combat.h>
#include <GameDefinitions/Components/Data.h>

BEGIN_SE()

AiPathCheckpoint::~AiPathCheckpoint()
{}


void AiPath::Reset()
{
    // CoverManager = ...;
    SearchStarted = false;
    SearchComplete = false;
    GoalFound = false;
    DestinationReached = false;
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
    InteractionRange = .0f;
    SearchHorizon = 32000;
    WorldClimbType = 0;
    WorldDropType = 0;
    DangerousAuras.Auras.clear();
    DangerousAuras.Avoidance = 0;
    CollisionMask.Flags = 0x40000000440094;
    CollisionMaskMove.Flags = 0x40000000000084;
    CollisionMaskStand.Flags = 0x10;
    CloseEnoughMin = .0f;
    CloseEnoughMax = .0f;
    CloseEnoughFloor = .0f;
    CloseEnoughPreference = 0;
    PreciseItemInteraction = false;
    UseSmoothing = true;
    UseSplines = true;
    UseTurning = true;
    IsPlayer = false;

    Portal = EntityHandle{};
    Climbing = false;
    field_154 = 0;

    ClosestFullTileIndex = -1;
    ClosestCollidingCount = 0x7fffffff;
    ClosestCost = 1.0e30f;

    DestinationFunc = {};
    WeightFunc = {};
}

void AiPath::SetSourceEntity(ecs::EntitySystemHelpersBase& helpers, EntityHandle entity)
{
    Source = entity;
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
}

void AiPath::SetSourceTemplate(CharacterTemplate* tmpl)
{
    StepHeight = tmpl->MovementStepUpHeight.Value;
    WorldClimbingHeight = 0.0f;
    if (tmpl->IsWorldClimbingEnabled.Value) {
        if (tmpl->WorldClimbingHeight.Value >= 0) {
            WorldClimbingHeight = tmpl->WorldClimbingHeight.Value;
        } else {
            WorldClimbingHeight = MovingBound;
        }
    }

    WorldClimbingRadius = tmpl->WorldClimbingRadius.Value;
    TurningNodeAngle = tmpl->TurningNodeAngle.Value;
    TurningNodeOffset = tmpl->TurningNodeOffset.Value;
    UseStandAtDestination = tmpl->UseStandAtDestination.Value;

    WorldClimbType = 1;
    WorldDropType = 1;
    CheckLockedDoors = true;
    CloseEnoughMin = 0.5f;
    CloseEnoughMax = 3.5f;
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
    TargetPosition = position;
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

int CalculateInfluenceCost(Array<SurfacePathInfluence> const& influences, AiFlags const& flags, int damagingSurfacesThreshold)
{
    int cost = 0;

    for (auto const& influence : influences) {
        if (flags.GetSurface(influence.IsCloud) == influence.SurfaceType) {
            if (influence.Influence < damagingSurfacesThreshold) {
                cost += std::min(influence.Influence, 12);
            } else {
                cost += influence.Influence;
            }
        }
    }

    return cost;
}

bool CheckPlayerWeightCell(AiPlayerWeightFuncData const& data, AiGrid* aiGrid, AiTilePos const& pos, int& pathScore)
{
    auto tile = aiGrid->GetTileAt(pos);
    auto area = aiGrid->GetStateInArea(pos, 1);

    AiFlags charBound;
    if (data.CharacterBounds == 0) {
        charBound = tile->Flags;
    } else if (data.CharacterBounds == 1) {
        charBound = area;
    } else {
        area = aiGrid->GetStateInArea(pos, data.CharacterBounds);
        charBound = area;
    }

    pathScore = 0;
    if (data.IsAvoidingObstacles && area.IsObstacle()) {
        pathScore += 20;
    }

    if (data.IsAvoidingDynamics) {
        if (tile->Flags.Flags & (uint32_t)AiBaseFlags::WalkBlockCharacter) {
            pathScore += 200;
        } else if (charBound.Flags & (uint32_t)AiBaseFlags::WalkBlockCharacter) {
            pathScore += 20;
        }
    }

    if (data.UseSurfaceInfluences && area.HasAnySurface()) {
        auto surfaceCost = 2 * CalculateInfluenceCost(*data.SurfacePathInfluences, tile->Flags, data.DamagingSurfacesThreshold);
        if (!surfaceCost) {
            surfaceCost = CalculateInfluenceCost(*data.SurfacePathInfluences, area, data.DamagingSurfacesThreshold) / 5;
        }
    }

    if (data.IsAvoidingTraps) {
        if (tile->Flags.Flags & (uint32_t)AiBaseFlags::Trap) {
            pathScore += 500;
        } else if (charBound.Flags & (uint32_t)AiBaseFlags::Trap) {
            pathScore += 100;
        }
    }

    return true;
}

void AiPath::SetPlayerWeightFunction(AiPlayerWeightFuncData const& params)
{
    WeightFunc = MakeFunction(&CheckPlayerWeightCell, params);
}

void AiPath::UsePlayerWeighting(lua_State* L, std::optional<bool> avoidObstacles, std::optional<bool> avoidDynamics)
{
    auto helpers = lua::State::FromLua(L)->GetEntitySystemHelpers();

    bool inCombat{ false };
    if (Source) {
        auto combat = helpers->GetComponent<combat::ParticipantComponent>(Source);
        if (combat) {
            inCombat = (bool)combat->CombatHandle;
        }
    }

    auto damagingSurfacesThreshold = GetStaticSymbols().GetStats()->ExtraData->get_or_default(FixedString{ "DamagingSurfacesThreshold" }, 0.0f);

    SetPlayerWeightFunction(AiPlayerWeightFuncData{
        .CharacterBounds = StandingBoundTiles,
        .IsAvoidingDynamics = avoidDynamics.value_or(true),
        .IsAvoidingObstacles = avoidObstacles.value_or(true),
        .UseSurfaceInfluences = !inCombat,
        .IsAvoidingTraps = (CollisionMask.Flags & (uint32_t)AiBaseFlags::Trap) != 0,
        .SurfacePathInfluences = &SurfacePathInfluences,
        .DamagingSurfacesThreshold = (int)damagingSurfacesThreshold
    });
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

AiFlags AiSubgrid::GetStateInArea(glm::ivec2 const& pos, int radius) const
{
    AiFlags flags = 0;

    glm::ivec2 topLeft, bottomRight;
    GetCornerTiles(pos, radius, topLeft, bottomRight);

    for (int y = topLeft.y; y <= bottomRight.y; y++) {
        for (int x = topLeft.x; x <= bottomRight.x; x++) {
            flags.Flags |= TileGrid->GetTileAt(x, y)->Flags.Flags;
        }
    }

    return flags;
}

void AiSubgrid::GetCornerTiles(glm::ivec2 const& pos, int radius, glm::ivec2& topLeft, glm::ivec2& bottomRight) const
{
    topLeft = glm::ivec2(
        std::max(0, pos.x - radius),
        std::max(0, pos.y - radius)
    );
    bottomRight = glm::ivec2(
        std::min(SizeX - 1, pos.x + radius),
        std::min(SizeY - 1, pos.y + radius)
    );
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
        auto subgrid = Subgrids.try_get(subgridId);
        if (subgrid) {
            if (subgrid->WorldToTilePos(pos, localPos)) {
                auto tile = subgrid->TileGrid->GetTileAt(localPos.x, localPos.y);
                if (!tile->Flags.IsBlocker()) {
                    auto minY = subgrid->Translate.y + tile->GetLocalMinHeight();
                    auto maxY = subgrid->Translate.y + tile->GetLocalMaxHeight();

                    auto curYdiff = fabs(pos.Y - minY);
                    if (curYdiff < ydiff) {
                        ydiff = curYdiff;
                        tilePos.SubgridId = subgridId;
                        tilePos.X = localPos.x;
                        tilePos.Y = localPos.y;
                        tileInfo = tile;
                        pSubgrid = subgrid;
                    }
                }
            }
        }
    }

    return ydiff < 3.40282347e+38f;
}

AiGridTile const* AiGrid::GetTileAt(AiTilePos const& pos) const
{
    auto subgrid = Subgrids.try_get(pos.SubgridId);
    if (subgrid) {
        return subgrid->TileGrid->GetTileAt(pos.X, pos.Y);
    }

    return nullptr;
}

AiFlags AiGrid::GetStateInArea(AiTilePos const& pos, int radius) const
{
    auto subgrid = Subgrids.try_get(pos.SubgridId);
    if (subgrid) {
        return subgrid->GetStateInArea(glm::ivec2(pos.X, pos.Y), radius);
    }

    return 0;
}

Array<float> AiGrid::GetHeightsAt(AiWorldPos const& pos) const
{
    glm::ivec2 localPos;
    Array<float> heights;

    auto subgrids = GetSubgridsAt(pos);
    for (auto subgridId : subgrids) {
        auto subgrid = Subgrids.try_get(subgridId);
        if (subgrid) {
            if (subgrid->WorldToTilePos(pos, localPos)) {
                auto tile = subgrid->TileGrid->GetTileAt(localPos.x, localPos.y);
                if (!tile->Flags.IsBlocker()) {
                    heights.push_back(subgrid->Translate.y + tile->GetLocalMaxHeight());
                }
            }
        }
    }

    return heights;
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
        ERR("No free AiPath available; make sure you released paths that are no longer in use");
        return nullptr;
    }

    auto handle = NextPathHandle++;
    path->Reset();
    path->InUse = true;
    PathMap.insert(handle, path);

    return path;
}

std::optional<AiPathId> AiGrid::GetPathId(AiPath* path)
{
    for (auto const& it : PathMap) {
        if (it.Value == path) {
            return it.Key;
        }
    }

    return {};
}

void AiGrid::FreePath(AiPath* path)
{
    if (!path->InUse) {
        WARN("Trying to free path that is not in use?");
        return;
    }

    auto pathId = GetPathId(path);
    if (!pathId) {
        ERR("Trying to free path that has no ID?");
        return;
    }

    for (uint32_t i = 0; i < Paths.size(); i++) {
        if (Paths[i] == path) {
            Paths.ordered_remove_at(i);
            break;
        }
    }

    PathMap.erase(PathMap.find(*pathId));
    path->InUse = false;
}

END_SE()
