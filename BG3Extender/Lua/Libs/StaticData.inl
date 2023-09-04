#include <GameDefinitions/Resources.h>
#include <GameDefinitions/GuidResources.h>
#include <Lua/LuaSerializers.h>

BEGIN_NS(lua::res)

using namespace bg3se::resource;

template <class T>
int GetGuidResourceProxy(lua_State* L, Guid const& resourceGuid)
{
	auto& helpers = gExtender->GetServer().GetEntityHelpers();
	auto resourceMgr = helpers.GetResourceManager<T>();
	if (!resourceMgr) {
		LuaError("Resource manager not available for this resource type");
		push(L, nullptr);
		return 1;
	}

	auto resource = (*resourceMgr)->Resources.Find(resourceGuid);
	if (resource) {
		MakeObjectRef(L, *resource);
	} else {
		push(L, nullptr);
	}

	return 1;
}

int GetGuidResource(lua_State* L, Guid resourceGuid, ExtResourceManagerType type)
{
	auto& helpers = gExtender->GetServer().GetEntityHelpers();
	switch (type) {
	case Action::ResourceManagerType: return GetGuidResourceProxy<Action>(L, resourceGuid);
	case ClassDescription::ResourceManagerType: return GetGuidResourceProxy<ClassDescription>(L, resourceGuid);
	case Tag::ResourceManagerType: return GetGuidResourceProxy<Tag>(L, resourceGuid);
	case Faction::ResourceManagerType: return GetGuidResourceProxy<Faction>(L, resourceGuid);
	case Race::ResourceManagerType: return GetGuidResourceProxy<Race>(L, resourceGuid);
	case Origin::ResourceManagerType: return GetGuidResourceProxy<Origin>(L, resourceGuid);
	case Background::ResourceManagerType: return GetGuidResourceProxy<Background>(L, resourceGuid);
	case God::ResourceManagerType: return GetGuidResourceProxy<God>(L, resourceGuid);
	case Progression::ResourceManagerType: return GetGuidResourceProxy<Progression>(L, resourceGuid);
	case ProgressionDescription::ResourceManagerType: return GetGuidResourceProxy<ProgressionDescription>(L, resourceGuid);
	case Gossip::ResourceManagerType: return GetGuidResourceProxy<Gossip>(L, resourceGuid);
	case ActionResourceGroup::ResourceManagerType: return GetGuidResourceProxy<ActionResourceGroup>(L, resourceGuid);
	case Color::ResourceManagerType: return GetGuidResourceProxy<Color>(L, resourceGuid);
	case EquipmentType::ResourceManagerType: return GetGuidResourceProxy<EquipmentType>(L, resourceGuid);
	case Flag::ResourceManagerType: return GetGuidResourceProxy<Flag>(L, resourceGuid);
	case Feat::ResourceManagerType: return GetGuidResourceProxy<Feat>(L, resourceGuid);
	case FeatDescription::ResourceManagerType: return GetGuidResourceProxy<FeatDescription>(L, resourceGuid);

	default:
		LuaError("Resource type not supported: " << type);
		push(L, nullptr);
		return 1;
	}
}


template <class T>
Array<Guid> GetAllGuidResourcesTyped()
{
	auto& helpers = gExtender->GetServer().GetEntityHelpers();
	auto resourceMgr = helpers.GetResourceManager<T>();
	if (!resourceMgr) {
		LuaError("Resource manager not available for this resource type");
		return {};
	}

	Array<Guid> keys;
	for (auto const& k : (*resourceMgr)->Resources.Keys) {
		keys.Add(k);
	}

	return keys;
}

Array<Guid> GetAllGuidResources(lua_State* L, ExtResourceManagerType type)
{
	auto& helpers = gExtender->GetServer().GetEntityHelpers();
	switch (type) {
	case Action::ResourceManagerType: return GetAllGuidResourcesTyped<Action>();
	case ClassDescription::ResourceManagerType: return GetAllGuidResourcesTyped<ClassDescription>();
	case Tag::ResourceManagerType: return GetAllGuidResourcesTyped<Tag>();
	case Faction::ResourceManagerType: return GetAllGuidResourcesTyped<Faction>();
	case Race::ResourceManagerType: return GetAllGuidResourcesTyped<Race>();
	case Origin::ResourceManagerType: return GetAllGuidResourcesTyped<Origin>();
	case Background::ResourceManagerType: return GetAllGuidResourcesTyped<Background>();
	case God::ResourceManagerType: return GetAllGuidResourcesTyped<God>();
	case Progression::ResourceManagerType: return GetAllGuidResourcesTyped<Progression>();
	case ProgressionDescription::ResourceManagerType: return GetAllGuidResourcesTyped<ProgressionDescription>();
	case Gossip::ResourceManagerType: return GetAllGuidResourcesTyped<Gossip>();
	case ActionResourceGroup::ResourceManagerType: return GetAllGuidResourcesTyped<ActionResourceGroup>();
	case Color::ResourceManagerType: return GetAllGuidResourcesTyped<Color>();
	case EquipmentType::ResourceManagerType: return GetAllGuidResourcesTyped<EquipmentType>();
	case Flag::ResourceManagerType: return GetAllGuidResourcesTyped<Flag>();
	case Feat::ResourceManagerType: return GetAllGuidResourcesTyped<Feat>();
	case FeatDescription::ResourceManagerType: return GetAllGuidResourcesTyped<FeatDescription>();

	default:
		LuaError("Resource type not supported: " << type);
		return {};
	}
}


ResourceBank* GetCurrentResourceBank()
{
	auto resMgr = GetStaticSymbols().ResourceMgr;
	if (!resMgr || !*resMgr || !(*resMgr)->ResourceBanks[0]) {
		return nullptr;
	}

	if ((*resMgr)->ResourceBanks[0]->Packages.size() > 0) {
		return (*resMgr)->ResourceBanks[0];
	} else {
		return (*resMgr)->ResourceBanks[1];
	}
}

	
RefReturn<Resource> GetResource(FixedString const& resourceGuid, ResourceBankType type)
{
	auto bank = GetCurrentResourceBank();
	if (!bank) {
		LuaError("Resource manager not available");
		return {};
	}

	auto resource = bank->Container.Banks[(unsigned)type]->Resources.find(resourceGuid);
	if (resource != bank->Container.Banks[(unsigned)type]->Resources.end()) {
		return resource.Value();
	} else {
		return {};
	}
}


Array<FixedString> GetAllResources(ResourceBankType type)
{
	auto bank = GetCurrentResourceBank();
	if (!bank) {
		LuaError("Resource manager not available");
		return {};
	}

	Array<FixedString> ids;
	for (auto const& resource : bank->Container.Banks[(unsigned)type]->Resources) {
		ids.Add(resource.Key);
	}

	return ids;
}


void RegisterStaticDataLib()
{
	DECLARE_MODULE(Definition, Both)
	BEGIN_MODULE()
	MODULE_NAMED_FUNCTION("Get", GetGuidResource)
	MODULE_NAMED_FUNCTION("GetAll", GetAllGuidResources)
	END_MODULE()

	DECLARE_MODULE(Resource, Both)
	BEGIN_MODULE()
	MODULE_NAMED_FUNCTION("Get", GetResource)
	MODULE_NAMED_FUNCTION("GetAll", GetAllResources)
	END_MODULE()
}

END_NS()
