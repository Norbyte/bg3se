#include <GameDefinitions/Resources.h>
#include <GameDefinitions/GuidResources.h>

BEGIN_NS(lua::res)

using namespace bg3se::resource;

#define FOR_EACH_GUID_RESOURCE_TYPE() \
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
	FOR_RESOURCE_TYPE(CharacterCreationSharedVisual) \
	FOR_RESOURCE_TYPE(LevelMap)

#define FOR_EACH_NONGUID_RESOURCE_TYPE() \
	FOR_RESOURCE_TYPE(Visual) \
	FOR_RESOURCE_TYPE(VisualSet) \
	FOR_RESOURCE_TYPE(Animation) \
	FOR_RESOURCE_TYPE(AnimationSet) \
	FOR_RESOURCE_TYPE(Texture) \
	FOR_RESOURCE_TYPE(Material) \
	FOR_RESOURCE_TYPE(Physics) \
	FOR_RESOURCE_TYPE(Effect) \
	FOR_RESOURCE_TYPE(Script) \
	FOR_RESOURCE_TYPE(Sound) \
	FOR_RESOURCE_TYPE(Lighting) \
	FOR_RESOURCE_TYPE(Atmosphere) \
	FOR_RESOURCE_TYPE(AnimationBlueprint) \
	FOR_RESOURCE_TYPE(MeshProxy) \
	FOR_RESOURCE_TYPE(MaterialSet) \
	FOR_RESOURCE_TYPE(BlendSpace) \
	FOR_RESOURCE_TYPE(FCurve) \
	FOR_RESOURCE_TYPE(Timeline) \
	FOR_RESOURCE_TYPE(Dialog) \
	FOR_RESOURCE_TYPE(VoiceBark) \
	FOR_RESOURCE_TYPE(TileSet) \
	FOR_RESOURCE_TYPE(IKRig) \
	FOR_RESOURCE_TYPE(Skeleton) \
	FOR_RESOURCE_TYPE(VirtualTexture) \
	FOR_RESOURCE_TYPE(TerrainBrush) \
	FOR_RESOURCE_TYPE(ColorList) \
	FOR_RESOURCE_TYPE(CharacterVisual) \
	FOR_RESOURCE_TYPE(MaterialPreset) \
	FOR_RESOURCE_TYPE(SkinPreset) \
	FOR_RESOURCE_TYPE(ClothCollider) \
	FOR_RESOURCE_TYPE(DiffusionProfile) \
	FOR_RESOURCE_TYPE(LightCookie) \
	FOR_RESOURCE_TYPE(TimelineScene) \
	FOR_RESOURCE_TYPE(SkeletonMirrorTable)

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
	FOR_EACH_GUID_RESOURCE_TYPE()

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
	FOR_EACH_GUID_RESOURCE_TYPE()

	default:
		LuaError("Resource type not supported: " << type);
		return {};
	}
}

#undef FOR_RESOURCE_TYPE


ResourceBank* GetCurrentResourceBank()
{
	auto resMgr = GetStaticSymbols().ls__gGlobalResourceManager;
	if (!resMgr || !*resMgr || !(*resMgr)->ResourceBanks[0]) {
		return nullptr;
	}

	if ((*resMgr)->ResourceBanks[0]->Packages.size() > 0) {
		return (*resMgr)->ResourceBanks[0];
	} else {
		return (*resMgr)->ResourceBanks[1];
	}
}

#define FOR_RESOURCE_TYPE(ty)																	  \
	case ResourceBankType::ty: MakeObjectRef(L, (ty##Resource*)resource.Value()); break;

UserReturn GetResource(lua_State* L, FixedString const& resourceGuid, ResourceBankType type)
{
	auto bank = GetCurrentResourceBank();
	if (!bank) {
		LuaError("Resource manager not available");
		push(L, nullptr);
		return 1;
	}

	auto resource = bank->Container.Banks[(unsigned)type]->Resources.find(resourceGuid);
	if (resource != bank->Container.Banks[(unsigned)type]->Resources.end()) {
		switch (type)
		{
			FOR_EACH_NONGUID_RESOURCE_TYPE();
		}
	} else {
		push(L, nullptr);
	}
	return 1;
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
