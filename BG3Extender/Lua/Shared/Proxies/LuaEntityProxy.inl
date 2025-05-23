#include <Lua/Shared/Proxies/LuaEntityProxy.h>
#include <Lua/Shared/LuaMethodCallHelpers.h>
#include <GameDefinitions/Character.h>
#include <GameDefinitions/Item.h>
#include <GameDefinitions/Components/All.h>
#include <Extender/ScriptExtender.h>

BEGIN_NS(lua)

HashMap<FixedStringUnhashed, lua_CFunction> EntityProxyMetatable::functions_;
HashMap<FixedStringUnhashed, EntityProxyMetatable::PropertyMapEntry> EntityProxyMetatable::propertyMap_;

ecs::EntitySystemHelpersBase* GetEntitySystem(lua_State* L)
{
    return State::FromLua(L)->GetEntitySystemHelpers();
}

template <class T>
void PushComponentType(lua_State* L, ecs::EntitySystemHelpersBase* helpers, EntityHandle const& handle,
    LifetimeHandle lifetime)
{
    auto component = helpers->GetComponent<T>(handle);
    if (component) {
        MakeDirectObjectRef(L, component, lifetime);
    } else {
        push(L, nullptr);
    }
}

#define T(cls) \
case cls::ComponentType: \
{ \
    PushComponentType<cls>(L, helpers, handle, lifetime); \
    break; \
}

void PushComponent(lua_State* L, ecs::EntitySystemHelpersBase* helpers, EntityHandle const& handle, ExtComponentType componentType,
    LifetimeHandle lifetime)
{
    switch (componentType) {

#include <GameDefinitions/Components/AllComponentTypes.inl>

    default:
        OsiError("Don't know how to push component type: " << componentType);
        push(L, nullptr);
        break;
    }
}

#undef T

#define T(cls) case cls::ComponentType: MakeDirectObjectRef(L, reinterpret_cast<cls*>(rawComponent), lifetime); break;

void PushComponent(lua_State* L, void* rawComponent, ExtComponentType componentType, LifetimeHandle lifetime)
{
    switch (componentType) {

#include <GameDefinitions/Components/AllComponentTypes.inl>

    default:
        OsiError("Don't know how to push component type: " << componentType);
        push(L, nullptr);
        break;
    }
}

#undef T


void EntityHelper::PushComponentByType(lua_State* L, ExtComponentType componentType) const
{
    StackCheck _(L, 1);
    auto ecs = GetEntitySystem(L);
    PushComponent(L, ecs, handle_, componentType, GetCurrentLifetime(L));
}

Array<ExtComponentType> EntityHelper::GetAllComponentTypes() const
{
    Array<ExtComponentType> types;

    auto world = ecs_->GetEntityWorld();
    auto storage = world->GetEntityStorage(handle_);
    if (storage != nullptr) {
        for (auto typeInfo : storage->ComponentTypeToIndex) {
            auto extType = ecs_->GetComponentType(typeInfo.Key());
            if (extType) {
                types.push_back(*extType);
            }
        }

        if (storage->HasOneFrameComponents) {
            for (auto pool : storage->OneFrameComponents) {
                auto extType = ecs_->GetComponentType(pool.Key());
                if (extType && pool->Value().find(handle_) != pool->Value().end()) {
                    types.push_back(*extType);
                }
            }
        }
    }

    return types;
}


EntityHandle EntityProxyMetatable::Get(lua_State* L, int index)
{
    auto meta = lua_get_cppvalue(L, index, MetatableTag::Entity);
    return GetHandle(meta);
}

EntityHelper EntityProxyMetatable::GetHelper(lua_State* L, int index)
{
    return EntityHelper(Get(L, index), GetEntitySystem(L));
}

UserReturn EntityProxyMetatable::CreateComponent(lua_State* L, EntityHandle entity, ExtComponentType component)
{
    auto ecs = GetEntitySystem(L);
    auto typeId = *ecs->GetComponentIndex(component);
    auto ops = ecs->GetEntityWorld()->ComponentOps.Get(typeId);

    if (ops != nullptr) {
        ops->AddImmediateDefaultComponent(entity.Handle, 0);
        PushComponent(L, ecs, entity, component, GetCurrentLifetime(L));
        return 1;
    }

    OsiError("Unable to construct components of this type: " << component);
    push(L, nullptr);
    return 1;
}

bool EntityProxyMetatable::RemoveComponent(lua_State* L, EntityHandle entity, ExtComponentType component)
{
    auto ecs = GetEntitySystem(L);
    auto typeId = *ecs->GetComponentIndex(component);
    return ecs->GetEntityWorld()->Cache->RemoveComponent(entity, typeId);
}

UserReturn EntityProxyMetatable::GetComponent(lua_State* L, EntityHandle entity, ExtComponentType component)
{
    auto ecs = GetEntitySystem(L);
    PushComponent(L, ecs, entity, component, GetCurrentLifetime(L));
    return 1;
}

bool EntityProxyMetatable::HasRawComponent(lua_State* L, EntityHandle entity, STDString componentName)
{
    auto ecs = GetEntitySystem(L);
    auto index = ecs->GetComponentIndex(componentName);
    if (!index) {
        return false;
    } else {
        auto ptr = ecs->GetEntityWorld()->GetRawComponent(entity, *index, 1, false);
        return ptr != nullptr;
    }
}

template <class F>
void EnumerateComponents(ecs::EntitySystemHelpersBase* ecs, EntityHandle entity, bool warnOnMissing, F f)
{
    auto world = ecs->GetEntityWorld();
    auto storage = world->GetEntityStorage(entity);
    if (storage != nullptr) {
        auto componentPtr = storage->InstanceToPageMap.try_get(entity);
        if (componentPtr) {
            for (auto typeInfo : storage->ComponentTypeToIndex) {
                auto extType = ecs->GetComponentType(typeInfo.Key());
                if (extType) {
                    auto const& meta = ecs->GetComponentMeta(*extType);
                    auto component = storage->GetComponent(*componentPtr, typeInfo.Value(), meta.Size, meta.IsProxy);

                    f(*extType, component);
                } else if (warnOnMissing) {
                    auto name = ecs->GetComponentName(typeInfo.Key());
                    if (name) {
                        OsiWarn("No model found for component: " << *name);
                    } else {
                        OsiWarn("No model found for component ID: " << (unsigned)typeInfo.Key());
                    }
                }
            }

            if (storage->HasOneFrameComponents) {
                for (auto pool : storage->OneFrameComponents) {
                    auto extType = ecs->GetComponentType(pool.Key());
                    if (extType) {
                        auto const& meta = ecs->GetComponentMeta(*extType);
                        auto component = pool->Value().get_or_default(entity);
                        f(*extType, component);
                    } else if (warnOnMissing) {
                        auto name = ecs->GetComponentName(pool.Key());
                        if (name) {
                            OsiWarn("No model found for component: " << *name);
                        } else {
                            OsiWarn("No model found for component ID: " << (unsigned)pool.Key());
                        }
                    }
                }
            }
        }
    }

    auto changes = world->Deferred();
    auto change = changes->Data.GetEntityChange(entity);
    if (change) {
        for (uint32_t i = 0; i < change->Store.size(); i++) {
            auto const& comp = change->Store[i];
            if (comp.PoolIndex) {
                auto extType = ecs->GetComponentType(comp.ComponentTypeId);
                if (extType) {
                    auto component = changes->GetComponentChange(comp.ComponentTypeId, comp.PoolIndex);
                    if (component) {
                        f(*extType, component);
                    }
                } else if (warnOnMissing) {
                    auto name = ecs->GetComponentName(comp.ComponentTypeId);
                    if (name) {
                        OsiWarn("No model found for component: " << *name);
                    } else {
                        OsiWarn("No model found for component ID: " << (unsigned)comp.ComponentTypeId);
                    }
                }
            }
        }
    }
}

UserReturn EntityProxyMetatable::GetAllComponents(lua_State* L, EntityHandle entity, std::optional<bool> warnOnMissing)
{
    StackCheck _(L, 1);

    lua_newtable(L);

    auto ecs = GetEntitySystem(L);
    EnumerateComponents(ecs, entity, warnOnMissing && *warnOnMissing, [L] (ExtComponentType type, void* component) {
        push(L, type);
        PushComponent(L, component, type, GetCurrentLifetime(L));
        lua_rawset(L, -3);
    });

    return 1;
}

Array<STDString> EntityProxyMetatable::GetAllComponentNames(lua_State* L, EntityHandle entity, std::optional<bool> requireMapped)
{
    Array<STDString> names;

    auto ecs = GetEntitySystem(L);
    auto world = ecs->GetEntityWorld();
    auto storage = world->GetEntityStorage(entity);
    if (storage != nullptr) {
        for (auto componentIdx : storage->ComponentTypeToIndex.keys()) {
            auto name = ecs->GetComponentName(componentIdx);
            if (name) {
                auto mapped = ecs->GetComponentType(componentIdx).has_value();
                if (!requireMapped || *requireMapped == mapped) {
                    names.push_back(*name);
                }
            }
        }

        if (storage->HasOneFrameComponents) {
            for (auto it : storage->OneFrameComponents) {
                if (it->Value().find(entity) != it->Value().end()) {
                    auto name = ecs->GetComponentName(it->Key());
                    if (name) {
                        auto mapped = ecs->GetComponentType(it->Key()).has_value();
                        if (!requireMapped || *requireMapped == mapped) {
                            names.push_back(*name);
                        }
                    }
                }
            }
        }
    }

    return names;
}

bool EntityProxyMetatable::IsAlive(lua_State* L, EntityHandle entity)
{
    auto world = GetEntitySystem(L)->GetEntityWorld();
    auto storage = world->GetEntityStorage(entity);
    return storage != nullptr;
}

std::optional<NetId> EntityProxyMetatable::GetNetId(lua_State* L, EntityHandle entity)
{
    auto ecs = GetEntitySystem(L);
    return ecs->EntityToNetId(entity);
}

uint64_t EntityProxyMetatable::GetReplicationFlags(lua_State* L, EntityHandle entity, ExtComponentType component, std::optional<uint32_t> qword)
{
    auto ecs = State::FromLua(L)->GetEntitySystemHelpers();
    auto flagMask = ecs->GetReplicationFlags(entity, component);
    uint64_t flags{ 0 };
    unsigned qw = qword ? *qword : 0;
    if (flagMask && qw < flagMask->NumQwords()) {
        flags = flagMask->GetBuf()[qw];
    }

    return flags;
}

void ReplicateComponent(lua_State* L, EntityHandle entity, ExtComponentType component, uint32_t qword, uint64_t flags)
{
    auto ecs = State::FromLua(L)->GetEntitySystemHelpers();
    if (!ecs->GetEntityWorld()->Replication) {
        OsiError("Changes can only be replicated from server to client");
        return;
    }

    auto replicationFlags = ecs->GetOrCreateReplicationFlags(entity, component);
    if (replicationFlags) {
        replicationFlags->EnsureSize((qword + 1) * 64);
        bool changed = (replicationFlags->GetBuf()[qword] & flags) != flags;
        replicationFlags->GetBuf()[qword] |= flags;
        if (changed) {
            ecs->NotifyReplicationFlagsDirtied();
        }
    } else {
        OsiError("Unable to replicate; " << component << " type cannot be replicated or the replication ID is not mapped");
    }
}

void EntityProxyMetatable::SetReplicationFlags(lua_State* L, EntityHandle entity, ExtComponentType component, uint64_t flags, std::optional<uint32_t> qword)
{
    unsigned qw = qword ? *qword : 0;
    ReplicateComponent(L, entity, component, qw, flags);
}

void EntityProxyMetatable::Replicate(lua_State* L, EntityHandle entity, ExtComponentType component)
{
    ReplicateComponent(L, entity, component, 0, 0xffffffffffffffffull);
}

LuaEntitySubscriptionId EntityProxyMetatable::OnCreate(lua_State* L, EntityHandle entity, ExtComponentType component, 
    FunctionRef func, std::optional<bool> deferred, std::optional<bool> once)
{
    auto flags = ((deferred && *deferred) ? EntityComponentEventFlags::Deferred : (EntityComponentEventFlags)0)
        | ((once && *once) ? EntityComponentEventFlags::Once : (EntityComponentEventFlags)0);
    return EntityEventHelpers::Subscribe(L, entity, component, EntityComponentEvent::Create, flags, func.MakePersistent(L));
}

LuaEntitySubscriptionId EntityProxyMetatable::OnCreateDeferred(lua_State* L, EntityHandle entity, ExtComponentType component, FunctionRef func)
{
    return EntityEventHelpers::Subscribe(L, entity, component, EntityComponentEvent::Create, 
        EntityComponentEventFlags::Deferred, func.MakePersistent(L));
}

LuaEntitySubscriptionId EntityProxyMetatable::OnCreateDeferredOnce(lua_State* L, EntityHandle entity, ExtComponentType component, FunctionRef func)
{
    return EntityEventHelpers::Subscribe(L, entity, component, EntityComponentEvent::Create,
        EntityComponentEventFlags::Deferred | EntityComponentEventFlags::Once, func.MakePersistent(L));
}

LuaEntitySubscriptionId EntityProxyMetatable::OnCreateOnce(lua_State* L, EntityHandle entity, ExtComponentType component, FunctionRef func)
{
    return EntityEventHelpers::Subscribe(L, entity, component, EntityComponentEvent::Create,
        EntityComponentEventFlags::Once, func.MakePersistent(L));
}

LuaEntitySubscriptionId EntityProxyMetatable::OnDestroy(lua_State* L, EntityHandle entity, ExtComponentType component,
    FunctionRef func, std::optional<bool> deferred, std::optional<bool> once)
{
    auto flags = ((deferred && *deferred) ? EntityComponentEventFlags::Deferred : (EntityComponentEventFlags)0)
        | ((once && *once) ? EntityComponentEventFlags::Once : (EntityComponentEventFlags)0);
    return EntityEventHelpers::Subscribe(L, entity, component, EntityComponentEvent::Destroy, flags, func.MakePersistent(L));
}

LuaEntitySubscriptionId EntityProxyMetatable::OnDestroyDeferred(lua_State* L, EntityHandle entity, ExtComponentType component, FunctionRef func)
{
    return EntityEventHelpers::Subscribe(L, entity, component, EntityComponentEvent::Destroy,
        EntityComponentEventFlags::Deferred, func.MakePersistent(L));
}

LuaEntitySubscriptionId EntityProxyMetatable::OnDestroyDeferredOnce(lua_State* L, EntityHandle entity, ExtComponentType component, FunctionRef func)
{
    return EntityEventHelpers::Subscribe(L, entity, component, EntityComponentEvent::Destroy,
        EntityComponentEventFlags::Deferred | EntityComponentEventFlags::Once, func.MakePersistent(L));
}

LuaEntitySubscriptionId EntityProxyMetatable::OnDestroyOnce(lua_State* L, EntityHandle entity, ExtComponentType component, FunctionRef func)
{
    return EntityEventHelpers::Subscribe(L, entity, component, EntityComponentEvent::Destroy,
        EntityComponentEventFlags::Once, func.MakePersistent(L));
}

std::optional<LuaEntitySubscriptionId> EntityProxyMetatable::OnChanged(lua_State* L, EntityHandle entity, ExtComponentType component,
    FunctionRef func, std::optional<uint64_t> flags)
{
    return EntityEventHelpers::SubscribeReplication(L, entity, component, func.MakePersistent(L), flags);
}

#define ADD_FUNC(fun) \
    functions_.set( \
        FixedString(#fun), \
        [](lua_State* L) -> int { return CallFunction(L, &fun); })

void EntityProxyMetatable::StaticInitialize()
{
    ADD_FUNC(CreateComponent);
    ADD_FUNC(RemoveComponent);
    ADD_FUNC(GetComponent);
    ADD_FUNC(HasRawComponent);
    ADD_FUNC(GetAllComponents);
    ADD_FUNC(GetAllComponentNames);

    ADD_FUNC(IsAlive);
    ADD_FUNC(GetNetId);

    ADD_FUNC(GetReplicationFlags);
    ADD_FUNC(SetReplicationFlags);
    ADD_FUNC(Replicate);

    ADD_FUNC(OnCreate);
    ADD_FUNC(OnCreateDeferred);
    ADD_FUNC(OnCreateDeferredOnce);
    ADD_FUNC(OnCreateOnce);

    ADD_FUNC(OnDestroy);
    ADD_FUNC(OnDestroyDeferred);
    ADD_FUNC(OnDestroyDeferredOnce);
    ADD_FUNC(OnDestroyOnce);

    ADD_FUNC(OnChanged);

    auto const& components = EnumInfo<ExtComponentType>::GetStore();
    for (auto const& component : components.Values) {
        propertyMap_.set(component.Key, PropertyMapEntry{
            .Function = nullptr,
            .Component = (ExtComponentType)component.Value
        });
    }

    for (auto const& func : functions_) {
        propertyMap_.set(func.Key(), PropertyMapEntry{
            .Function = func.Value(),
            .Component = {}
        });
    }
}

int EntityProxyMetatable::Index(lua_State* L, CppValueOpaque* self)
{
    StackCheck _(L, 1);
    EntityHandle handle(lua_get_opaque_value(self));
    auto key = get<FixedStringNoRef>(L, 2);
    auto& keyAH = reinterpret_cast<FixedStringUnhashed&>(key);

    auto entry = propertyMap_.try_get(keyAH);
    if (entry) {
        if (entry->Function) {
            push(L, entry->Function);
            return 1;
        } else {
            auto ecs = GetEntitySystem(L);
            auto rawComponent = ecs->GetRawComponent(handle, *entry->Component);
            if (rawComponent != nullptr) {
                PushComponent(L, rawComponent, *entry->Component, GetCurrentLifetime(L));
            } else {
                push(L, nullptr);
            }
            return 1;
        }
    }

    if (key == GFS.strVars) {
        UserVariableHolderMetatable::Make(L, handle);
        return 1;
    }

    auto componentTypeName = get<char const*>(L, 2);
    return luaL_error(L, "Not a valid EntityProxy method or component type: %s", componentTypeName);
}

bool EntityProxyMetatable::IsEqual(lua_State* L, CppObjectMetadata& self, int otherIndex)
{
    CppObjectMetadata other;
    return lua_try_get_cppvalue(L, otherIndex, MetatableTag::Entity, other)
        && GetHandle(other) == GetHandle(self);
}

bool EntityProxyMetatable::IsLessThan(lua_State* L, CppObjectMetadata& self, int otherIndex)
{
    CppObjectMetadata other;
    return lua_try_get_cppvalue(L, otherIndex, MetatableTag::Entity, other)
        && GetHandle(self).Handle < GetHandle(other).Handle;
}

bool EntityProxyMetatable::IsLessThan(lua_State* L, int selfIndex, CppObjectMetadata& other)
{
    CppObjectMetadata self;
    return lua_try_get_cppvalue(L, selfIndex, MetatableTag::Entity, self)
        && GetHandle(self).Handle < GetHandle(other).Handle;
}

char const* EntityProxyMetatable::GetTypeName(lua_State* L, CppObjectMetadata& self)
{
    return "EntityProxy";
}

int EntityProxyMetatable::ToString(lua_State* L, CppObjectMetadata& self)
{
    StackCheck _(L, 1);
    char entityName[100];
    auto handle = GetHandle(self);
    sprintf_s(entityName, "Entity (%016llx)", handle.Handle);
    push(L, entityName);
    return 1;
}

EntityHandle do_get(lua_State* L, int index, Overload<EntityHandle>)
{
    if (lua_type(L, index) == LUA_TNIL) {
        return NullEntityHandle;
    } else {
        return EntityProxyMetatable::Get(L, index);
    }
}

ecs::EntityRef do_get(lua_State* L, int index, Overload<ecs::EntityRef>)
{
    if (lua_type(L, index) == LUA_TNIL) {
        return ecs::EntityRef{ NullEntityHandle, State::FromLua(L)->GetEntityWorld() };
    } else {
        return ecs::EntityRef{ EntityProxyMetatable::Get(L, index), State::FromLua(L)->GetEntityWorld() };
    }
}

TypeInformationRef do_get(lua_State* L, int index, Overload<TypeInformationRef>)
{
    luaL_error(L, "TypeInformationRef is an engine-only type");
    return {};
}

EntityHelper do_get(lua_State* L, int index, Overload<EntityHelper>)
{
    return EntityProxyMetatable::GetHelper(L, index);
}

END_NS()
