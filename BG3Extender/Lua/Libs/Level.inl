#include <GameDefinitions/Ai.h>
#include <Lua/Libs/Level.h>
#include <GameDefinitions/Physics.h>

/// <lua_module>Level</lua_module>
BEGIN_NS(lua::level)


PathfindingSystem::PathfindingSystem(LevelManager& levelManager)
    : levelManager_(levelManager)
{}

PathfindingSystem::~PathfindingSystem()
{
    for (auto& req : pendingRequests_) {
        req.Release(levelManager_);
    }
}

void PathfindingSystem::PathRequest::Release(LevelManager& levelManager)
{
    if (path_->InUse) {
        levelManager.CurrentLevel->AiGrid->FreePath(path_);
    } else {
        ERR("Trying to release path %p that is no longer in use?", path_);
    }
}

AiPath* PathfindingSystem::CreatePathRequestImmediate()
{
    auto path = levelManager_.CurrentLevel->AiGrid->CreatePath();
    if (path) {
        pendingRequests_.push_back(PathRequest{ path, LuaDelegate<void(AiPath*)>{}, true });
        levelManager_.CurrentLevel->AiGrid->Paths.push_back(path);
    }

    return path;
}

AiPath* PathfindingSystem::CreatePathRequest(LuaDelegate<void(AiPath*)>&& callback)
{
    auto path = levelManager_.CurrentLevel->AiGrid->CreatePath();
    if (path) {
        pendingRequests_.push_back(PathRequest{ path, std::move(callback), false });
        levelManager_.CurrentLevel->AiGrid->Paths.push_back(path);
    }

    return path;
}

void PathfindingSystem::ReleasePath(AiPath* path)
{
    for (uint32_t i = 0; i < pendingRequests_.size(); i++) {
        if (pendingRequests_[i].path_ == path) {
            pendingRequests_[i].Release(levelManager_);
            pendingRequests_.ordered_remove_at(i);
        }
    }
}

bool PathfindingSystem::ProcessRequest(PathRequest& request)
{
    if (request.immediate_) {
        // Always release immediate-use paths, even if pathfinding was not performed on them
        if (!request.path_->SearchComplete) {
            WARN("BeginPathfindingImmediate() was called on path %p, but no pathfinding was performed", request.path_);
        }
        return true;
    }
    
    if (request.path_->SearchComplete) {
        // Otherwise wait for async search completion
        eventQueue_.Call(request.callback_, request.path_);
        return true;
    }

    return false;
}

void PathfindingSystem::Update()
{
    Array<PathRequest> pending;

    for (auto& req : pendingRequests_) {
        if (ProcessRequest(req)) {
            req.Release(levelManager_);
        } else {
            pending.push_back(req);
        }
    }

    pendingRequests_ = pending;
    eventQueue_.Flush();
}



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

AiGridLuaTile* GetTileDebugInfo(lua_State* L, glm::vec3 pos)
{
    auto aiGrid = GetAiGrid(L);
    auto worldPos = AiGrid::ToWorldPos(pos);

    AiGridLuaTile& tile = gTestTile;
    AiTilePos tilePos;
    AiGridTile const* tileInfo;
    AiSubgrid* subgrid;
    if (aiGrid->ToTilePos(worldPos, subgrid, tilePos, tileInfo)) {
        tile.Flags = tileInfo->Flags.GetFlags();
        tile.GroundSurface = tileInfo->Flags.GetGroundSurface();
        tile.CloudSurface = tileInfo->Flags.GetCloudSurface();
        tile.Material = tileInfo->Flags.GetMaterial();
        tile.UnmappedFlags = (uint32_t)(tileInfo->Flags.Flags & 0x200001);
        tile.ExtraFlags = (uint32_t)(tileInfo->Flags.Flags >> 46) & 0xffffc;

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

Array<float> GetHeightsAt(lua_State* L, float x, float z)
{
    auto aiGrid = GetAiGrid(L);
    auto worldPos = AiGrid::ToWorldPos(glm::vec3(x, 0.0f, z));
    return aiGrid->GetHeightsAt(worldPos);
}

AiPath* BeginPathfinding(lua_State* L, EntityHandle source, glm::vec3 target, LuaDelegate<void(AiPath*)> callback)
{
    auto path = State::FromLua(L)->GetPathfinding().CreatePathRequest(std::move(callback));
    if (path) {
        path->SetSourceEntity(*State::FromLua(L)->GetEntitySystemHelpers(), source);
        path->SetTarget(target);
    }

    return path;
}

AiPath* BeginPathfindingImmediate(lua_State* L, EntityHandle source, glm::vec3 target)
{
    auto path = State::FromLua(L)->GetPathfinding().CreatePathRequestImmediate();
    if (path) {
        path->SetSourceEntity(*State::FromLua(L)->GetEntitySystemHelpers(), source);
        path->SetTarget(target);
    }

    return path;
}

bool FindPath(lua_State* L, AiPath* path)
{
    if (!path->InUse) {
        ERR("Trying to pathfind on released path %p?", path);
        return false;
    }

    if (!path->SearchComplete) {
        auto aiGrid = GetAiGrid(L);
        auto pathId = aiGrid->GetPathId(path);
        if (!pathId) {
            ERR("Couldn't find PathId mapping for path %p?", path);
            return false;
        }

        auto find = GetStaticSymbols().eoc__AiGrid__FindPathImmediate;
        find(aiGrid, *pathId);
    }

    return path->GoalFound;
}

void ReleasePath(lua_State* L, AiPath* path)
{
    if (path->InUse) {
        State::FromLua(L)->GetPathfinding().ReleasePath(path);
    }
}

AiPath* GetPathById(lua_State* L, AiPathId id)
{
    auto aiGrid = GetAiGrid(L);
    return aiGrid->PathMap.try_get(id);
}

Array<AiPath*> GetActivePathfindingRequests(lua_State* L)
{
    Array<AiPath*> paths;
    auto aiGrid = GetAiGrid(L);
    for (auto path : aiGrid->PathPool) {
        if (path->InUse) {
            paths.push_back(path);
        }
    }

    return paths;
}

bool filterfunc(phx::PhysicsShape const* obj)
{
    return true;
}

phx::PhysicsHit gHit;
phx::PhysicsHitAll gHits;

phx::PhysicsHit* RaycastClosest(lua_State* L, glm::vec3 const& source, glm::vec3 const& destination, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context)
{
    auto levelManager = State::FromLua(L)->GetExtensionState().GetLevelManager();
    auto phys = levelManager->CurrentLevel->PhysicsScene;
    auto callback = MakeFunction(&filterfunc);
    gHit = phx::PhysicsHit{};
    auto xhit = phys->RaycastClosest(source, destination, gHit, physicsType, includePhysicsGroup, excludePhysicsGroup, context , -1, -1, &callback);
    if (xhit) {
        return &gHit;
    } else {
        return nullptr;
    }
}

phx::PhysicsHitAll* RaycastAll(lua_State* L, glm::vec3 const& source, glm::vec3 const& destination, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context)
{
    auto levelManager = State::FromLua(L)->GetExtensionState().GetLevelManager();
    auto phys = levelManager->CurrentLevel->PhysicsScene;
    auto xhit = phys->RaycastAll(source, destination, gHits, physicsType, includePhysicsGroup, excludePhysicsGroup, context, -1, -1, {});
    return &gHits;
}

bool RaycastAny(lua_State* L, glm::vec3 const& source, glm::vec3 const& destination, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context)
{
    auto levelManager = State::FromLua(L)->GetExtensionState().GetLevelManager();
    auto phys = levelManager->CurrentLevel->PhysicsScene;
    return phys->RaycastAny(source, destination, physicsType, includePhysicsGroup, excludePhysicsGroup, context, -1, -1, {});
}

phx::PhysicsHit* SweepSphereClosest(lua_State* L, glm::vec3 const& source, glm::vec3 const& destination, float radius, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context)
{
    auto levelManager = State::FromLua(L)->GetExtensionState().GetLevelManager();
    auto phys = levelManager->CurrentLevel->PhysicsScene;
    gHit = phx::PhysicsHit{};
    auto xhit = phys->SweepSphereClosest(radius, source, destination, gHit, physicsType, includePhysicsGroup, excludePhysicsGroup, context, -1, -1);
    if (xhit) {
        return &gHit;
    } else {
        return nullptr;
    }
}

phx::PhysicsHit* SweepCapsuleClosest(lua_State* L, glm::vec3 const& source, glm::vec3 const& destination, float radius, float halfHeight, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context)
{
    auto levelManager = State::FromLua(L)->GetExtensionState().GetLevelManager();
    auto phys = levelManager->CurrentLevel->PhysicsScene;
    gHit = phx::PhysicsHit{};
    auto xhit = phys->SweepCapsuleClosest(radius, halfHeight, source, destination, gHit, physicsType, includePhysicsGroup, excludePhysicsGroup, context, -1, -1);
    if (xhit) {
        return &gHit;
    } else {
        return nullptr;
    }
}

phx::PhysicsHit* SweepBoxClosest(lua_State* L, glm::vec3 const& source, glm::vec3 const& destination, glm::vec3 const& extents, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context)
{
    auto levelManager = State::FromLua(L)->GetExtensionState().GetLevelManager();
    auto phys = levelManager->CurrentLevel->PhysicsScene;
    gHit = phx::PhysicsHit{};
    auto xhit = phys->SweepBoxClosest(extents, source, destination, gHit, physicsType, includePhysicsGroup, excludePhysicsGroup, context, -1, -1);
    if (xhit) {
        return &gHit;
    } else {
        return nullptr;
    }
}

phx::PhysicsHitAll* SweepSphereAll(lua_State* L, glm::vec3 const& source, glm::vec3 const& destination, float radius, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context)
{
    auto levelManager = State::FromLua(L)->GetExtensionState().GetLevelManager();
    auto phys = levelManager->CurrentLevel->PhysicsScene;
    gHits = phx::PhysicsHitAll{};
    auto xhit = phys->SweepSphereAll(radius, source, destination, gHits, physicsType, includePhysicsGroup, excludePhysicsGroup, context, -1, -1);
    if (xhit) {
        return &gHits;
    } else {
        return nullptr;
    }
}

phx::PhysicsHitAll* SweepCapsuleAll(lua_State* L, glm::vec3 const& source, glm::vec3 const& destination, float radius, float halfHeight, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context)
{
    auto levelManager = State::FromLua(L)->GetExtensionState().GetLevelManager();
    auto phys = levelManager->CurrentLevel->PhysicsScene;
    gHits = phx::PhysicsHitAll{};
    auto xhit = phys->SweepCapsuleAll(radius, halfHeight, source, destination, gHits, physicsType, includePhysicsGroup, excludePhysicsGroup, context, -1, -1);
    if (xhit) {
        return &gHits;
    } else {
        return nullptr;
    }
}

phx::PhysicsHitAll* SweepBoxAll(lua_State* L, glm::vec3 const& source, glm::vec3 const& destination, glm::vec3 const& extents, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context)
{
    auto levelManager = State::FromLua(L)->GetExtensionState().GetLevelManager();
    auto phys = levelManager->CurrentLevel->PhysicsScene;
    gHits = phx::PhysicsHitAll{};
    auto xhit = phys->SweepBoxAll(extents, source, destination, gHits, physicsType, includePhysicsGroup, excludePhysicsGroup, context, -1, -1);
    if (xhit) {
        return &gHits;
    } else {
        return nullptr;
    }
}

phx::PhysicsHitAll* TestBox(lua_State* L, glm::vec3 const& position, glm::vec3 const& extents, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup)
{
    auto levelManager = State::FromLua(L)->GetExtensionState().GetLevelManager();
    auto phys = levelManager->CurrentLevel->PhysicsScene;
    gHits = phx::PhysicsHitAll{};
    auto xhit = phys->TestBox(extents, position, gHits, physicsType, includePhysicsGroup, excludePhysicsGroup);
    if (xhit) {
        return &gHits;
    } else {
        return nullptr;
    }
}

phx::PhysicsHitAll* TestSphere(lua_State* L, glm::vec3 const& position, float radius, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup)
{
    auto levelManager = State::FromLua(L)->GetExtensionState().GetLevelManager();
    auto phys = levelManager->CurrentLevel->PhysicsScene;
    gHits = phx::PhysicsHitAll{};
    auto xhit = phys->TestSphere(position, radius, gHits, physicsType, includePhysicsGroup, excludePhysicsGroup);
    if (xhit) {
        return &gHits;
    } else {
        return nullptr;
    }
}

void RegisterLevelLib()
{
    DECLARE_MODULE(Level, Both)
    BEGIN_MODULE()
    MODULE_FUNCTION(GetEntitiesOnTile)
    MODULE_FUNCTION(GetTileDebugInfo)
    MODULE_FUNCTION(GetHeightsAt)
    MODULE_FUNCTION(BeginPathfinding)
    MODULE_FUNCTION(BeginPathfindingImmediate)
    MODULE_FUNCTION(FindPath)
    MODULE_FUNCTION(ReleasePath)
    MODULE_FUNCTION(GetPathById)
    MODULE_FUNCTION(GetActivePathfindingRequests)

    MODULE_FUNCTION(RaycastClosest)
    MODULE_FUNCTION(RaycastAny)
    MODULE_FUNCTION(RaycastAll)

    MODULE_FUNCTION(SweepSphereClosest)
    MODULE_FUNCTION(SweepCapsuleClosest)
    MODULE_FUNCTION(SweepBoxClosest)
    MODULE_FUNCTION(SweepSphereAll)
    MODULE_FUNCTION(SweepCapsuleAll)
    MODULE_FUNCTION(SweepBoxAll)

    MODULE_FUNCTION(TestBox)
    MODULE_FUNCTION(TestSphere)
    END_MODULE()
}

END_NS()
