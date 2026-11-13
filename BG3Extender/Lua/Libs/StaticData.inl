#include <GameDefinitions/Resources.h>
#include <GameDefinitions/GuidResources.h>
#include <Lua/Libs/StaticData.h>
#include <GameDefinitions/Dialog.h>

BEGIN_NS(lua::utils)
dlg::DialogManager* GetDialogManager(lua_State* L);
END_NS()

BEGIN_NS(lua::res)

using namespace bg3se::resource;

bool NullGuidResourceBankHelper::Push(lua_State* L, Guid resourceGuid)
{
    return false;
}
    
bool NullGuidResourceBankHelper::Create(lua_State* L, std::optional<Guid> resourceGuid, bool allowUnsafeExpansion)
{
    return false;
}

Array<Guid> NullGuidResourceBankHelper::GetAll()
{
    return {};
}

HashMap<Guid, Array<Guid>>* NullGuidResourceBankHelper::GetSources()
{
    return &dummySources_;
}

Array<Guid>* NullGuidResourceBankHelper::GetByModId(Guid modGuid)
{
    return &dummyResources_;
}
    
GuidResourceBankBase* NullGuidResourceBankHelper::GetBank() const
{
    return nullptr;
}


template <class T>
class GuidResourceBankHelper : public GuidResourceBankHelperBase
{
public:
    GuidResourceBankHelper(GuidResourceBank<T>* bank)
        : bank_(bank)
    {}

    bool Push(lua_State* L, Guid resourceGuid) override
    {
        auto resource = bank_->Resources.try_get(resourceGuid);
        if (resource) {
            MakeObjectRef(L, resource, LifetimeHandle{});
            return true;
        } else {
            return false;
        }
    }

    bool Create(lua_State* L, std::optional<Guid> resourceGuid, bool allowUnsafeExpansion) override
    {
        // We need at least one resource to copy the VMT pointer
        if (bank_->Resources.empty()) {
            LuaError("Unable to create resource - resource bank is empty");
            return false;
        }
        
        // It's unsafe to add new resources (for now) if we reach internal array size
        if (bank_->Resources.raw_values().size() == bank_->Resources.keys().size()) {
            if (allowUnsafeExpansion) {
                Grow();
            } else {
                LuaError("Unable to create resource - no space left in resource bank");
                return false;
            }
        }

        Guid guid;
        if (resourceGuid) {
            guid = *resourceGuid;
        } else {
            guid = Guid::Generate();
        }

        auto existing = bank_->Resources.try_get(guid);
        if (existing) {
            LuaError("Unable to create resource - a resource with the same GUID already exists");
            return false;
        }

        auto resource = bank_->Resources.add_key(guid);
        resource->VMT = bank_->Resources.values()[0].VMT;
        resource->ResourceUUID = guid;
        MakeObjectRef(L, resource, LifetimeHandle{});

        return true;
    }

    Array<Guid> GetAll() override
    {
        return bank_->Resources.keys();
    }

    HashMap<Guid, Array<Guid>>* GetSources() override
    {
        return &bank_->ResourceGuidsByMod;
    }

    Array<Guid>* GetByModId(Guid modGuid) override
    {
        return bank_->ResourceGuidsByMod.try_get(modGuid);
    }

    GuidResourceBankBase* GetBank() const override
    {
        return bank_;
    }

private:
    GuidResourceBank<T>* bank_;

    void Grow()
    {
        WARN("Growing resource bank %s - this may break resource references!", EnumInfo<ExtResourceManagerType>::Find(T::ResourceManagerType).GetString());
        // The game may still reference the old resources, so ensure that a copy is kept of the old buffer
        // (This leaks memory!)
        auto curSize = bank_->Resources.keys().size();
        auto oldResources = bank_->Resources.raw_values().data();
        T* newResources = GameMemoryAllocator::NewRaw<T>(curSize);
        for (uint32_t i = 0; i < curSize; i++) {
            new (newResources + i) T(oldResources[i]);
        }

        bank_->Resources.raw_values().unsafe_swap_buffer(newResources);

        // Use a more aggressive grow size to ensure we don't need to reallocate too frequently
        auto newCapacity = std::max(curSize, 200u) * 2;
        bank_->Resources.reserve(newCapacity);
    }
};

#define FOR_RESOURCE_TYPE(ty) case resource::ty::ResourceManagerType: return MakeHelper<resource::ty>(type);

template <class T>
GuidResourceBankHelperBase* GuidResourceHelpers::MakeHelper(ExtResourceManagerType type)
{
    auto& ecs = state_.GetEntitySystemHelpers();
    auto bank = ecs.GetResourceManager<T>();
    if (!bank) {
        LuaError("Guid resource manager not available for resource type: " << type);
        return  &nullHelper_;
    }

    helpers_[(unsigned)type] = std::make_unique<GuidResourceBankHelper<T>>(*bank);
    return helpers_[(unsigned)type].get();
}

GuidResourceBankHelperBase* GuidResourceHelpers::Get(ExtResourceManagerType type)
{
    auto helper = helpers_[(unsigned)type].get();
    if (helper) {
        return helper;
    }

    switch (type) {
        FOR_EACH_GUID_RESOURCE_TYPE()

    default:
        LuaError("Guid resource type not supported: " << type);
        return &nullHelper_;
    }
}

#undef FOR_RESOURCE_TYPE


UserReturn GetGuidResource(lua_State* L, Guid resourceGuid, ExtResourceManagerType type)
{
    if (!State::FromLua(L)->GetGuidResourceHelpers().Get(type)->Push(L, resourceGuid)) {
        push(L, nullptr);
    }

    return 1;
}

Array<Guid> GetAllGuidResources(lua_State* L, ExtResourceManagerType type)
{
    OPTICK_EVENT();
    return State::FromLua(L)->GetGuidResourceHelpers().Get(type)->GetAll();
}

HashMap<Guid, Array<Guid>>* GetGuidResourceSources(lua_State* L, ExtResourceManagerType type)
{
    return State::FromLua(L)->GetGuidResourceHelpers().Get(type)->GetSources();
}

Array<Guid>* GetGuidResourcesByModId(lua_State* L, ExtResourceManagerType type, Guid modGuid)
{
    return State::FromLua(L)->GetGuidResourceHelpers().Get(type)->GetByModId(modGuid);
}

void ClearResourceBank(lua_State* L, ExtResourceManagerType type)
{
    auto bank = State::FromLua(L)->GetGuidResourceHelpers().Get(type)->GetBank();
    if (bank) {
        bank->ClearInternal();
    }
}

void SyncResourceBank(lua_State* L, ExtResourceManagerType type)
{
    auto bank = State::FromLua(L)->GetGuidResourceHelpers().Get(type)->GetBank();
    if (bank) {
        bank->PostLoad();
    }
}

UserReturn CreateGuidResource(lua_State* L, ExtResourceManagerType type, std::optional<Guid> resourceGuid)
{
    if (!State::FromLua(L)->GetGuidResourceHelpers().Get(type)->Create(L, resourceGuid, true)) {
        push(L, nullptr);
    }

    return 1;
}

Resource* GetResource(lua_State* L, FixedString const& resourceGuid, ResourceBankType type)
{
    auto bank = GetStaticSymbols().GetCurrentResourceBank();
    if (!bank) {
        LuaError("Resource manager not available");
        return nullptr;
    }

    return bank->GetResource(type, resourceGuid);
}


Array<FixedString> GetAllResources(ResourceBankType type)
{
    OPTICK_EVENT();
    auto bank = GetStaticSymbols().GetCurrentResourceBank();
    if (!bank) {
        LuaError("Resource manager not available");
        return {};
    }

    Array<FixedString> ids;
    for (auto const& resource : bank->Container.Banks[(unsigned)type]->Resources) {
        ids.push_back(resource.Key);
    }

    return ids;
}


Array<FixedString> GetPackages()
{
    OPTICK_EVENT();
    auto bank = GetStaticSymbols().GetCurrentResourceBank();
    if (!bank) {
        LuaError("Resource manager not available");
        return {};
    }

    Array<FixedString> ids;
    for (auto const& package : bank->Packages) {
        ids.push_back(package.Key);
    }

    return ids;
}


Array<FixedString> GetPackageResources(FixedString const& packageName, std::optional<ResourceBankType> type)
{
    OPTICK_EVENT();
    auto bank = GetStaticSymbols().GetCurrentResourceBank();
    if (!bank) {
        LuaError("Resource manager not available");
        return {};
    }

    auto package = bank->Packages.try_get(packageName);
    if (!package) {
        return {};
    }

    Array<FixedString> ids;
    for (auto const& resource : (*package)->Resources) {
        if (!type || *type == (ResourceBankType)resource.Value->GetType()) {
            ids.push_back(resource.Key);
        }
    }

    return ids;
}


Resource* GetPackageResource(FixedString const& packageName, FixedString const& resourceId, std::optional<ResourceBankType> type)
{
    OPTICK_EVENT();
    auto bank = GetStaticSymbols().GetCurrentResourceBank();
    if (!bank) {
        LuaError("Resource manager not available");
        return nullptr;
    }

    auto package = bank->Packages.try_get(packageName);
    if (!package) {
        return nullptr;
    }

    auto resource = (*package)->Resources.try_get(resourceId);
    if (resource) {
        if (type && type != (ResourceBankType)(*resource)->GetType()) {
            return nullptr;
        }

        return *resource;
    } else {
        return nullptr;
    }
}

TextureAtlasMap* GetTextureAtlasManager()
{
    return *GetStaticSymbols().ls__gTextureAtlasMap;
}

TextureAtlas* GetTextureAtlas(STDString path)
{
    return (*GetStaticSymbols().ls__gTextureAtlasMap)->AtlasMap.get_or_default(path);
}

TextureAtlas* GetIconAtlas(FixedString icon)
{
    return (*GetStaticSymbols().ls__gTextureAtlasMap)->IconMap.get_or_default(icon);
}

UVValues* GetIconUVs(FixedString icon)
{
    auto atlas = (*GetStaticSymbols().ls__gTextureAtlasMap)->IconMap.get_or_default(icon);
    if (atlas) {
        return atlas->Icons.get_or_default(icon);
    }

    return nullptr;
}

// Dialogues loaded with LoadResource: the state holds one reference on each until ReleaseResource,
// or until it shuts down. They are kept by the UUID of the cached dialogue rather than by pointer,
// as the game may free its cache (e.g. on a level change).
dlg::Dialog* FindCachedDialog(dlg::DialogManager* manager, FixedString const& uuid)
{
    for (auto dialog : manager->CachedDialogs) {
        if (dialog->UUID == uuid) return dialog;
    }

    return nullptr;
}

// Releases a reference the way the game does: once the dialogue is no longer used, the time it was
// released at starts the delay after which the dialogue manager unloads it.
bool ReleaseDialog(lua_State* L, dlg::DialogManager* manager, FixedString const& uuid)
{
    auto dialog = manager ? FindCachedDialog(manager, uuid) : nullptr;
    if (dialog == nullptr || dialog->RefCount <= 0) return false;

    if (--dialog->RefCount == 0) {
        dialog->UnloadedTimeStamp = State::FromLua(L)->GetExtensionState().Time().Time;
    }

    return true;
}

void ReleaseHeldDialogs(lua_State* L)
{
    auto& held = State::FromLua(L)->GetHeldDialogs();
    if (held.empty()) return;

    auto manager = utils::GetDialogManager(L);
    for (auto const& dialog : held) {
        ReleaseDialog(L, manager, dialog.Value());
    }

    held.clear();
}

/// <summary>
/// Loads a resource the game only loads when it needs it, and holds a reference on it until
/// `ReleaseResource`, or until the Lua state is reset. Only dialogues (`Dialog`) are supported:
/// they are loaded into the dialogue manager of the current context (see
/// `Ext.Utils.GetDialogManager`) the way the game does before playing them; the client only loads
/// the ones it plays.
/// </summary>
dlg::Dialog* LoadResource(lua_State* L, FixedString const& resourceGuid, ResourceBankType type)
{
    if (type != ResourceBankType::Dialog) {
        luaL_error(L, "LoadResource only supports Dialog resources");
    }

    auto load = GetStaticSymbols().dlg__DialogManager__PerformDialogLoad;
    if (load == nullptr) {
        LuaError("dlg::DialogManager::PerformDialogLoad not mapped");
        return nullptr;
    }

    auto manager = utils::GetDialogManager(L);
    auto bank = GetStaticSymbols().GetCurrentResourceBank();
    if (manager == nullptr || bank == nullptr) {
        LuaError("Dialogue manager or resource bank not available");
        return nullptr;
    }

    auto& held = State::FromLua(L)->GetHeldDialogs();
    auto uuid = held.try_get(resourceGuid);
    if (uuid != nullptr) {
        auto dialog = FindCachedDialog(manager, *uuid);
        if (dialog != nullptr) return dialog;
        held.remove(resourceGuid);
    }

    auto resource = static_cast<resource::DialogResource const*>(bank->GetResource(ResourceBankType::Dialog, resourceGuid));
    if (resource == nullptr) return nullptr;

    auto dialog = load(manager, resource, nullptr, nullptr);
    if (dialog != nullptr) {
        held.set(resourceGuid, dialog->UUID);
    }

    return dialog;
}

/// <summary>
/// Releases the reference `LoadResource` holds on a resource, if it still holds it.
/// Returns whether it did.
/// </summary>
bool ReleaseResource(lua_State* L, FixedString const& resourceGuid, ResourceBankType type)
{
    if (type != ResourceBankType::Dialog) {
        luaL_error(L, "ReleaseResource only supports Dialog resources");
    }

    auto& held = State::FromLua(L)->GetHeldDialogs();
    auto uuid = held.try_get(resourceGuid);
    if (uuid == nullptr) return false;

    auto dialogUuid = *uuid;
    held.remove(resourceGuid);
    return ReleaseDialog(L, utils::GetDialogManager(L), dialogUuid);
}

void RegisterStaticDataLib()
{
    DECLARE_MODULE(StaticData, Both)
    BEGIN_MODULE()
    MODULE_NAMED_FUNCTION("Get", GetGuidResource)
    MODULE_NAMED_FUNCTION("GetAll", GetAllGuidResources)
    MODULE_NAMED_FUNCTION("GetSources", GetGuidResourceSources)
    MODULE_NAMED_FUNCTION("GetByModId", GetGuidResourcesByModId)
    MODULE_NAMED_FUNCTION("Create", CreateGuidResource)
    MODULE_NAMED_FUNCTION("GetTextureAtlasManager", GetTextureAtlasManager)
    MODULE_NAMED_FUNCTION("GetIconAtlas", GetIconAtlas)
    MODULE_NAMED_FUNCTION("GetIconUVs", GetIconUVs)
    MODULE_NAMED_FUNCTION("ClearResourceBank", ClearResourceBank)
    MODULE_NAMED_FUNCTION("SyncResourceBank", SyncResourceBank)
    MODULE_NAMED_FUNCTION("LoadResource", LoadResource)
    MODULE_NAMED_FUNCTION("ReleaseResource", ReleaseResource)
    END_MODULE()

    DECLARE_MODULE(Resource, Both)
    BEGIN_MODULE()
    MODULE_NAMED_FUNCTION("Get", GetResource)
    MODULE_NAMED_FUNCTION("GetAll", GetAllResources)
    MODULE_NAMED_FUNCTION("GetPackages", GetPackages)
    MODULE_NAMED_FUNCTION("GetPackageResources", GetPackageResources)
    MODULE_NAMED_FUNCTION("GetPackageResource", GetPackageResource)
    END_MODULE()
}

END_NS()
