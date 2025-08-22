#include <GameDefinitions/Resources.h>
#include <GameDefinitions/GuidResources.h>

BEGIN_NS(lua::res)

using namespace bg3se::resource;

class GuidResourceBankHelperBase
{
public:
    virtual bool Push(lua_State* L, Guid resourceGuid) = 0;
    virtual bool Create(lua_State* L, std::optional<Guid> resourceGuid) = 0;
    virtual Array<Guid> GetAll() = 0;
    virtual HashMap<Guid, Array<Guid>>* GetSources() = 0;
    virtual Array<Guid>* GetByModId(Guid modGuid) = 0;
};

class NullGuidResourceBankHelper : public GuidResourceBankHelperBase
{
public:
    bool Push(lua_State* L, Guid resourceGuid) override
    {
        return false;
    }
    
    bool Create(lua_State* L, std::optional<Guid> resourceGuid) override
    {
        return false;
    }

    virtual Array<Guid> GetAll()
    {
        return {};
    }

    virtual HashMap<Guid, Array<Guid>>* GetSources()
    {
        return &dummySources_;
    }

    virtual Array<Guid>* GetByModId(Guid modGuid)
    {
        return &dummyResources_;
    }

private:
    HashMap<Guid, Array<Guid>> dummySources_;
    Array<Guid> dummyResources_;
};

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

    bool Create(lua_State* L, std::optional<Guid> resourceGuid) override
    {
        // We need at least one resource to copy the VMT pointer
        if (bank_->Resources.empty()) {
            LuaError("Unable to create resource - resource bank is empty");
            return false;
        }
        
        // It's unsafe to add new resources (for now) if we reach internal array size
        if (bank_->Resources.raw_values().size() == bank_->Resources.keys().size()) {
            LuaError("Unable to create resource - no space left in resource bank");
            return false;
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

    virtual Array<Guid> GetAll()
    {
        return bank_->Resources.keys();
    }

    virtual HashMap<Guid, Array<Guid>>* GetSources()
    {
        return &bank_->ResourceGuidsByMod;
    }

    virtual Array<Guid>* GetByModId(Guid modGuid)
    {
        return bank_->ResourceGuidsByMod.try_get(modGuid);
    }

private:
    GuidResourceBank<T>* bank_;
};

#define FOR_RESOURCE_TYPE(ty) case ty::ResourceManagerType: return MakeHelper<ty>(type);

class GuidResourceHelpers
{
public:
    template <class T>
    GuidResourceBankHelperBase* MakeHelper(ExtResourceManagerType type)
    {
        auto& helpers = gExtender->GetServer().GetEntityHelpers();
        auto bank = helpers.GetResourceManager<T>();
        if (!bank) {
            LuaError("Guid resource manager not available for resource type: " << type);
            return  &nullHelper_;
        }

        helpers_[(unsigned)type] = std::make_unique<GuidResourceBankHelper<T>>(*bank);
        return helpers_[(unsigned)type].get();
    }

    GuidResourceBankHelperBase* Get(ExtResourceManagerType type)
    {
        auto helper = helpers_[(unsigned)type].get();
        if (helper) {
            return helper;
        }

        auto& helpers = gExtender->GetServer().GetEntityHelpers();
        switch (type) {
            FOR_EACH_GUID_RESOURCE_TYPE()

        default:
            LuaError("Guid resource type not supported: " << type);
            return &nullHelper_;
        }
    }

private:
    std::array<std::unique_ptr<GuidResourceBankHelperBase>, (unsigned)ExtResourceManagerType::Max + 1> helpers_;
    NullGuidResourceBankHelper nullHelper_;
};

#undef FOR_RESOURCE_TYPE

GuidResourceHelpers gGuidResourceHelpers;


UserReturn GetGuidResource(lua_State* L, Guid resourceGuid, ExtResourceManagerType type)
{
    if (!gGuidResourceHelpers.Get(type)->Push(L, resourceGuid)) {
        push(L, nullptr);
    }

    return 1;
}

Array<Guid> GetAllGuidResources(lua_State* L, ExtResourceManagerType type)
{
    return gGuidResourceHelpers.Get(type)->GetAll();
}

HashMap<Guid, Array<Guid>>* GetGuidResourceSources(lua_State* L, ExtResourceManagerType type)
{
    return gGuidResourceHelpers.Get(type)->GetSources();
}

Array<Guid>* GetGuidResourcesByModId(lua_State* L, ExtResourceManagerType type, Guid modGuid)
{
    return gGuidResourceHelpers.Get(type)->GetByModId(modGuid);
}

UserReturn CreateGuidResource(lua_State* L, ExtResourceManagerType type, std::optional<Guid> resourceGuid)
{
    if (!gGuidResourceHelpers.Get(type)->Create(L, resourceGuid)) {
        push(L, nullptr);
    }

    return 1;
}

#define FOR_RESOURCE_TYPE(ty)                                                            		  \
    case ResourceBankType::ty: MakeObjectRef(L, static_cast<ty##Resource*>(resource)); break;

UserReturn GetResource(lua_State* L, FixedString const& resourceGuid, ResourceBankType type)
{
    auto bank = GetStaticSymbols().GetCurrentResourceBank();
    if (!bank) {
        LuaError("Resource manager not available");
        push(L, nullptr);
        return 1;
    }

    auto resource = bank->GetResource(type, resourceGuid);
    if (resource != nullptr) {
        switch (type)
        {
            FOR_EACH_NONGUID_RESOURCE_TYPE()
        }
    } else {
        push(L, nullptr);
    }
    return 1;
}


Array<FixedString> GetAllResources(ResourceBankType type)
{
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


void RegisterStaticDataLib()
{
    DECLARE_MODULE(StaticData, Both)
    BEGIN_MODULE()
    MODULE_NAMED_FUNCTION("Get", GetGuidResource)
    MODULE_NAMED_FUNCTION("GetAll", GetAllGuidResources)
    MODULE_NAMED_FUNCTION("GetSources", GetGuidResourceSources)
    MODULE_NAMED_FUNCTION("GetByModId", GetGuidResourcesByModId)
    MODULE_NAMED_FUNCTION("Create", CreateGuidResource)
    END_MODULE()

    DECLARE_MODULE(Resource, Both)
    BEGIN_MODULE()
    MODULE_NAMED_FUNCTION("Get", GetResource)
    MODULE_NAMED_FUNCTION("GetAll", GetAllResources)
    END_MODULE()
}

END_NS()
