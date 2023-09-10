#include <GameDefinitions/Resources.h>
#include <GameDefinitions/GuidResources.h>
#include <Lua/LuaSerializers.h>

BEGIN_NS(lua::res)

using namespace bg3se::resource;

#define FOR_EACH_RESOURCE_TYPE() \
	FOR_RESOURCE_TYPE(ActionResource) \
	FOR_RESOURCE_TYPE(ActionResourceGroup) \
	FOR_RESOURCE_TYPE(ClassDescription) \
	FOR_RESOURCE_TYPE(Tag) \
	FOR_RESOURCE_TYPE(Faction) \
	FOR_RESOURCE_TYPE(Race) \
	FOR_RESOURCE_TYPE(Origin) \
	FOR_RESOURCE_TYPE(Background) \
	FOR_RESOURCE_TYPE(God) \
	FOR_RESOURCE_TYPE(Progression) \
	FOR_RESOURCE_TYPE(ProgressionDescription) \
	FOR_RESOURCE_TYPE(Gossip) \
	FOR_RESOURCE_TYPE(Color) \
	FOR_RESOURCE_TYPE(EquipmentType) \
	FOR_RESOURCE_TYPE(Flag) \
	FOR_RESOURCE_TYPE(Feat) \
	FOR_RESOURCE_TYPE(FeatDescription) \
	FOR_RESOURCE_TYPE(PassiveList) \
	FOR_RESOURCE_TYPE(SkillList) \
	FOR_RESOURCE_TYPE(SpellList) \
	FOR_RESOURCE_TYPE(CharacterCreationPreset) \
	FOR_RESOURCE_TYPE(CharacterCreationSkinColor) \
	FOR_RESOURCE_TYPE(CharacterCreationEyeColor) \
	FOR_RESOURCE_TYPE(CharacterCreationHairColor) \
	FOR_RESOURCE_TYPE(CharacterCreationAccessorySet) \
	FOR_RESOURCE_TYPE(CharacterCreationEquipmentIcons) \
	FOR_RESOURCE_TYPE(CharacterCreationIconSettings) \
	FOR_RESOURCE_TYPE(CharacterCreationMaterialOverride) \
	FOR_RESOURCE_TYPE(CharacterCreationAppearanceMaterial) \
	FOR_RESOURCE_TYPE(CharacterCreationPassiveAppearance) \
	FOR_RESOURCE_TYPE(CharacterCreationAppearanceVisual) \
	FOR_RESOURCE_TYPE(CharacterCreationSharedVisual)

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

#define FOR_RESOURCE_TYPE(ty) case ty::ResourceManagerType: return GetGuidResourceProxy<ty>(L, resourceGuid);

UserReturn GetGuidResource(lua_State* L, Guid resourceGuid, ExtResourceManagerType type)
{
	auto& helpers = gExtender->GetServer().GetEntityHelpers();
	switch (type) {
	FOR_EACH_RESOURCE_TYPE()

	default:
		LuaError("Resource type not supported: " << type);
		push(L, nullptr);
		return 1;
	}
}

#undef FOR_RESOURCE_TYPE

template <class T>
Array<Guid> GetAllGuidResourcesTyped()
{
	auto& helpers = gExtender->GetServer().GetEntityHelpers();
	auto resourceMgr = helpers.GetResourceManager<T>();
	if (!resourceMgr) {
		LuaError("Resource manager not available for this resource type");
		return {};
	}

	return (*resourceMgr)->Resources.Keys;
}

#define FOR_RESOURCE_TYPE(ty) case ty::ResourceManagerType: return GetAllGuidResourcesTyped<ty>();

Array<Guid> GetAllGuidResources(lua_State* L, ExtResourceManagerType type)
{
	auto& helpers = gExtender->GetServer().GetEntityHelpers();
	switch (type) {
	FOR_EACH_RESOURCE_TYPE()

	default:
		LuaError("Resource type not supported: " << type);
		return {};
	}
}

#undef FOR_RESOURCE_TYPE


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
	DECLARE_MODULE(StaticData, Both)
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
