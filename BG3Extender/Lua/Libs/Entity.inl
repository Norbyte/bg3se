/// <lua_module>Entity</lua_module>
BEGIN_NS(lua::entity)

std::optional<Guid> HandleToUuid(lua_State* L, EntityHandle entity)
{
    auto uuid = State::FromLua(L)->GetEntitySystemHelpers()->GetComponent<UuidComponent>(entity);
    if (uuid) {
        return uuid->EntityUuid;
    } else {
        return {};
    }
}

EntityHandle UuidToHandle(lua_State* L, Guid uuid)
{
    return State::FromLua(L)->GetEntitySystemHelpers()->GetEntityHandle(uuid);
}

UserReturn Get(lua_State* L, lua::AnyRef entity)
{
    auto type = lua_type(L, entity.Index);
    switch (type) {
    case LUA_TNIL:
    {
        push(L, nullptr);
        break;
    }
    
    case LUA_TSTRING:
    {
        auto uuid = get<Guid>(L, entity.Index);
        auto handle = State::FromLua(L)->GetEntitySystemHelpers()->GetEntityHandle(uuid);
        if (handle) {
            EntityProxyMetatable::Make(L, handle);
        } else {
            push(L, nullptr);
        }
        break;
    }
    
    case LUA_TNUMBER:
    {
        auto netId = get<NetId>(L, entity.Index);
        auto ecs = State::FromLua(L)->GetEntitySystemHelpers();
        auto handle = ecs->NetIdToEntity(netId);
        if (handle) {
            EntityProxyMetatable::Make(L, *handle);
        } else {
            push(L, nullptr);
        }
        break;
    }
    
    case LUA_TLIGHTCPPOBJECT:
    {
        auto handle = get<EntityHandle>(L, entity.Index);
        if (handle) {
            EntityProxyMetatable::Make(L, handle);
        } else {
            push(L, nullptr);
        }
        break;
    }

    default:
        return luaL_error(L, "Expected entity GUID, network ID or entity handle, got %s", GetDebugName(L, entity.Index));
    }

    return 1;
}

HashMap<Guid, EntityHandle> GetAllEntitiesWithUuid(lua_State* L)
{
    auto mappings = State::FromLua(L)->GetEntitySystemHelpers()->GetUuidMappings();
    if (mappings) {
        return mappings->Mappings;
    } else {
        return {};
    }
}

Array<EntityHandle> GetAllEntitiesWithComponent(lua_State* L, ExtComponentType component)
{
    auto ecs = State::FromLua(L)->GetEntitySystemHelpers();
    auto componentType = ecs->GetComponentIndex(component);
    if (!componentType) return {};

    Array<EntityHandle> entities;
    auto world = State::FromLua(L)->GetEntitySystemHelpers()->GetEntityWorld();

    auto const& meta = ecs->GetComponentMeta(component);
    // Disabled for further investigation
    if (false && meta.SingleComponentQuery != ecs::UndefinedQuery) {
        auto& query = world->Queries.Queries[(unsigned)meta.SingleComponentQuery];
        if (ecs::IsOneFrame(*componentType)) {
            for (auto const& storage : query.EntityStorages.values()) {
                auto pool = storage.Storage->OneFrameComponents.try_get(*componentType);
                if (pool) {
                    std::copy(pool->keys().begin(), pool->keys().end(), std::back_inserter(entities));
                }
            }
        } else {
            for (auto const& storage : query.EntityStorages.values()) {
                std::copy(storage.Storage->InstanceToPageMap.keys().begin(), storage.Storage->InstanceToPageMap.keys().end(), std::back_inserter(entities));
            }
        }
    } else {
        if (ecs::IsOneFrame(*componentType)) {
            for (auto cls : world->Storage->Entities) {
                if (cls->HasOneFrameComponents) {
                    auto pool = cls->OneFrameComponents.try_get(*componentType);
                    if (pool) {
                        std::copy(pool->keys().begin(), pool->keys().end(), std::back_inserter(entities));
                    }
                }
            }
        } else {
            for (auto cls : world->Storage->Entities) {
                if (cls->ComponentTypeToIndex.try_get(*componentType)) {
                    std::copy(cls->InstanceToPageMap.keys().begin(), cls->InstanceToPageMap.keys().end(), std::back_inserter(entities));
                }
            }
        }
    }


    return entities;
}

Array<EntityHandle> GetAllEntities(lua_State* L)
{
    Array<EntityHandle> entities;

    auto world = State::FromLua(L)->GetEntitySystemHelpers()->GetEntityWorld();
    for (auto cls : world->Storage->Entities) {
        std::copy(cls->InstanceToPageMap.keys().begin(), cls->InstanceToPageMap.keys().end(), std::back_inserter(entities));
    }

    return entities;
}

EntityHandle Create(lua_State* L)
{
    auto world = State::FromLua(L)->GetEntitySystemHelpers()->GetEntityWorld();
    return world->Deferred()->CreateEntityImmediate();
}

bool Destroy(lua_State* L, EntityHandle entity)
{
    auto world = State::FromLua(L)->GetEntitySystemHelpers()->GetEntityWorld();
    return world->Deferred()->DestroyEntity(entity);
}

std::optional<LuaEntitySubscriptionId> Subscribe(lua_State* L, ExtComponentType type, FunctionRef func, std::optional<EntityHandle> entity, std::optional<uint64_t> flags)
{
    return EntityEventHelpers::SubscribeReplication(L, entity ? *entity : EntityHandle{}, type, func.MakePersistent(L), flags);
}

LuaEntitySubscriptionId OnCreate(lua_State* L, ExtComponentType type, FunctionRef func, std::optional<EntityHandle> entity, 
    std::optional<bool> deferred, std::optional<bool> once)
{
    auto flags = ((deferred && *deferred) ? EntityComponentEventFlags::Deferred : (EntityComponentEventFlags)0)
        | ((once && *once) ? EntityComponentEventFlags::Once : (EntityComponentEventFlags)0);
    return EntityEventHelpers::Subscribe(L, entity ? *entity : EntityHandle{}, type, EntityComponentEvent::Create, 
        flags, func.MakePersistent(L));
}

LuaEntitySubscriptionId OnCreateDeferred(lua_State* L, ExtComponentType type, FunctionRef func, std::optional<EntityHandle> entity)
{
    return EntityEventHelpers::Subscribe(L, entity ? *entity : EntityHandle{}, type, EntityComponentEvent::Create, 
        EntityComponentEventFlags::Deferred, func.MakePersistent(L));
}

LuaEntitySubscriptionId OnCreateOnce(lua_State* L, ExtComponentType type, FunctionRef func, std::optional<EntityHandle> entity)
{
    return EntityEventHelpers::Subscribe(L, entity ? *entity : EntityHandle{}, type, EntityComponentEvent::Create, 
        EntityComponentEventFlags::Once, func.MakePersistent(L));
}

LuaEntitySubscriptionId OnCreateDeferredOnce(lua_State* L, ExtComponentType type, FunctionRef func, std::optional<EntityHandle> entity)
{
    return EntityEventHelpers::Subscribe(L, entity ? *entity : EntityHandle{}, type, EntityComponentEvent::Create, 
        EntityComponentEventFlags::Once | EntityComponentEventFlags::Deferred, func.MakePersistent(L));
}

LuaEntitySubscriptionId OnDestroy(lua_State* L, ExtComponentType type, FunctionRef func, std::optional<EntityHandle> entity, 
    std::optional<bool> deferred, std::optional<bool> once)
{
    auto flags = ((deferred && *deferred) ? EntityComponentEventFlags::Deferred : (EntityComponentEventFlags)0)
        | ((once && *once) ? EntityComponentEventFlags::Once : (EntityComponentEventFlags)0);
    return EntityEventHelpers::Subscribe(L, entity ? *entity : EntityHandle{}, type, EntityComponentEvent::Destroy, 
        flags, func.MakePersistent(L));
}

LuaEntitySubscriptionId OnDestroyDeferred(lua_State* L, ExtComponentType type, FunctionRef func, std::optional<EntityHandle> entity)
{
    return EntityEventHelpers::Subscribe(L, entity ? *entity : EntityHandle{}, type, EntityComponentEvent::Destroy, 
        EntityComponentEventFlags::Deferred, func.MakePersistent(L));
}

LuaEntitySubscriptionId OnDestroyOnce(lua_State* L, ExtComponentType type, FunctionRef func, std::optional<EntityHandle> entity)
{
    return EntityEventHelpers::Subscribe(L, entity ? *entity : EntityHandle{}, type, EntityComponentEvent::Destroy, 
        EntityComponentEventFlags::Once, func.MakePersistent(L));
}

LuaEntitySubscriptionId OnSystemUpdate(lua_State* L, ExtSystemType type, FunctionRef func, std::optional<bool> once)
{
    return EntityEventHelpers::SubscribeSystemUpdate(L, type, func.MakePersistent(L), false, once ? *once : false);
}

LuaEntitySubscriptionId OnSystemPostUpdate(lua_State* L, ExtSystemType type, FunctionRef func, std::optional<bool> once)
{
    return EntityEventHelpers::SubscribeSystemUpdate(L, type, func.MakePersistent(L), true, once ? *once : false);
}

LuaEntitySubscriptionId OnDestroyDeferredOnce(lua_State* L, ExtComponentType type, FunctionRef func, std::optional<EntityHandle> entity)
{
    return EntityEventHelpers::Subscribe(L, entity ? *entity : EntityHandle{}, type, EntityComponentEvent::Destroy, 
        EntityComponentEventFlags::Once | EntityComponentEventFlags::Deferred, func.MakePersistent(L));
}

bool Unsubscribe(lua_State* L, LuaEntitySubscriptionId handle)
{
    return EntityEventHelpers::Unsubscribe(L, handle);
}

void EnableTracing(lua_State* L, bool enable)
{
    if (!gExtender->GetConfig().DeveloperMode) {
        ERR("Entity tracing is only available in developer mode");
        return;
    }

    static bool DevelWarningShown = false;

    if (!DevelWarningShown) {
        DevelWarningShown = true;
        WARN("Entity tracing is a development tool designed for tracking entity changes; it should not be used in production!");
    }

    State::FromLua(L)->GetEntitySystemHelpers()->EnableLogging(enable);
}

ecs::ECSChangeLog* GetTrace(lua_State* L)
{
    return &State::FromLua(L)->GetEntitySystemHelpers()->GetLog();
}

void ClearTrace(lua_State* L)
{
    State::FromLua(L)->GetEntitySystemHelpers()->GetLog().Clear();
}

Array<StringView> GetRegisteredComponentTypes(lua_State* L, std::optional<bool> oneFrame, std::optional<bool> mapped)
{
    Array<StringView> types;
    
    auto helpers = State::FromLua(L)->GetEntitySystemHelpers();
    auto const& registry = helpers->GetEntityWorld()->ComponentRegistry_;
    for (unsigned i = 0; i < registry.Bitmask.Size; i++) {
        if (registry.Bitmask[i]) {
            auto const& component = registry.Types[i];
            auto name = helpers->GetComponentName(component.TypeId);
            if (name 
                && (!oneFrame || IsOneFrame(component.TypeId) == *oneFrame)
                && (!mapped || (bool)helpers->GetComponentType(component.TypeId) == *mapped)) {
                types.push_back(*name);
            }
        }
    }

    return types;
}

void RegisterEntityLib()
{
    DECLARE_MODULE(Entity, Both)
    BEGIN_MODULE()
    MODULE_FUNCTION(HandleToUuid)
    MODULE_FUNCTION(UuidToHandle)

    MODULE_FUNCTION(Get)
    MODULE_FUNCTION(GetAllEntitiesWithUuid)
    MODULE_FUNCTION(GetAllEntitiesWithComponent)
    MODULE_FUNCTION(GetAllEntities)

    MODULE_FUNCTION(Create)
    MODULE_FUNCTION(Destroy)

    MODULE_FUNCTION(Subscribe)
    MODULE_NAMED_FUNCTION("OnChange", Subscribe)
    MODULE_FUNCTION(OnCreate)
    MODULE_FUNCTION(OnCreateDeferred)
    MODULE_FUNCTION(OnCreateOnce)
    MODULE_FUNCTION(OnCreateDeferredOnce)
    MODULE_FUNCTION(OnDestroy)
    MODULE_FUNCTION(OnDestroyDeferred)
    MODULE_FUNCTION(OnDestroyOnce)
    MODULE_FUNCTION(OnDestroyDeferredOnce)
    MODULE_FUNCTION(OnSystemUpdate)
    MODULE_FUNCTION(OnSystemPostUpdate)
    MODULE_FUNCTION(Unsubscribe)

    MODULE_FUNCTION(EnableTracing)
    MODULE_FUNCTION(GetTrace)
    MODULE_FUNCTION(ClearTrace)

    MODULE_FUNCTION(GetRegisteredComponentTypes)
    END_MODULE()
}

END_NS()
