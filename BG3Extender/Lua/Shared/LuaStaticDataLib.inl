#include <Extender/ScriptExtender.h>
#include <GameDefinitions/Resources.h>
#include <GameDefinitions/GuidResources.h>
#include <Lua/LuaSerializers.h>

namespace bg3se::lua::utils
{
	template <class T>
	int GetGuidResourceProxy(lua_State* L, UUID const& resourceGuid)
	{
		auto& helpers = gExtender->GetServerEntityHelpers();
		auto resourceMgr = helpers.GetResourceManager<T>();
		if (!resourceMgr) {
			LuaError("Resource manager not available for this resource type");
			push(L, nullptr);
			return 1;
		}

		auto resource = (*resourceMgr)->Resources.Find(resourceGuid);
		if (resource) {
			ObjectProxy::MakeRef<T>(L, *resource, GetCurrentLifetime());
		} else {
			push(L, nullptr);
		}

		return 1;
	}

	int GetGuidResource(lua_State* L)
	{
		auto resourceGuid = checked_get<UUID>(L, 1);
		auto type = checked_get<ExtResourceManagerType>(L, 2);

		auto& helpers = gExtender->GetServerEntityHelpers();
		switch (type) {
		case ActionResource::ResourceManagerType: return GetGuidResourceProxy<ActionResource>(L, resourceGuid);
		case ClassDescriptionResource::ResourceManagerType: return GetGuidResourceProxy<ClassDescriptionResource>(L, resourceGuid);
		case TagResource::ResourceManagerType: return GetGuidResourceProxy<TagResource>(L, resourceGuid);
		case FactionResource::ResourceManagerType: return GetGuidResourceProxy<FactionResource>(L, resourceGuid);
		case RaceResource::ResourceManagerType: return GetGuidResourceProxy<RaceResource>(L, resourceGuid);
		case OriginResource::ResourceManagerType: return GetGuidResourceProxy<OriginResource>(L, resourceGuid);
		case BackgroundResource::ResourceManagerType: return GetGuidResourceProxy<BackgroundResource>(L, resourceGuid);
		case GodResource::ResourceManagerType: return GetGuidResourceProxy<GodResource>(L, resourceGuid);
		case ProgressionResource::ResourceManagerType: return GetGuidResourceProxy<ProgressionResource>(L, resourceGuid);
		case ProgressionDescriptionResource::ResourceManagerType: return GetGuidResourceProxy<ProgressionDescriptionResource>(L, resourceGuid);
		case GossipDefinition::ResourceManagerType: return GetGuidResourceProxy<GossipDefinition>(L, resourceGuid);
		case ActionResourceGroupDefinition::ResourceManagerType: return GetGuidResourceProxy<ActionResourceGroupDefinition>(L, resourceGuid);
		case ColorDefinitionResource::ResourceManagerType: return GetGuidResourceProxy<ColorDefinitionResource>(L, resourceGuid);
		case EquipmentTypeResource::ResourceManagerType: return GetGuidResourceProxy<EquipmentTypeResource>(L, resourceGuid);
		case FlagResource::ResourceManagerType: return GetGuidResourceProxy<FlagResource>(L, resourceGuid);
		case FeatResource::ResourceManagerType: return GetGuidResourceProxy<FeatResource>(L, resourceGuid);
		case FeatDescriptionResource::ResourceManagerType: return GetGuidResourceProxy<FeatDescriptionResource>(L, resourceGuid);

		default:
			LuaError("Resource type not supported: " << type);
			push(L, nullptr);
			return 1;
		}
	}


	template <class T>
	int GetAllGuidResourcesTyped(lua_State* L)
	{
		auto& helpers = gExtender->GetServerEntityHelpers();
		auto resourceMgr = helpers.GetResourceManager<T>();
		if (!resourceMgr) {
			LuaError("Resource manager not available for this resource type");
			push(L, nullptr);
			return 1;
		}

		lua_newtable(L);
		int i{ 1 };
		for (auto const& k : (*resourceMgr)->Resources.Keys) {
			settable(L, i++, k);
		}

		return 1;
	}

	int GetAllGuidResources(lua_State* L)
	{
		auto type = checked_get<ExtResourceManagerType>(L, 1);

		auto& helpers = gExtender->GetServerEntityHelpers();
		switch (type) {
		case ActionResource::ResourceManagerType: return GetAllGuidResourcesTyped<ActionResource>(L);
		case ClassDescriptionResource::ResourceManagerType: return GetAllGuidResourcesTyped<ClassDescriptionResource>(L);
		case TagResource::ResourceManagerType: return GetAllGuidResourcesTyped<TagResource>(L);
		case FactionResource::ResourceManagerType: return GetAllGuidResourcesTyped<FactionResource>(L);
		case RaceResource::ResourceManagerType: return GetAllGuidResourcesTyped<RaceResource>(L);
		case OriginResource::ResourceManagerType: return GetAllGuidResourcesTyped<OriginResource>(L);
		case BackgroundResource::ResourceManagerType: return GetAllGuidResourcesTyped<BackgroundResource>(L);
		case GodResource::ResourceManagerType: return GetAllGuidResourcesTyped<GodResource>(L);
		case ProgressionResource::ResourceManagerType: return GetAllGuidResourcesTyped<ProgressionResource>(L);
		case ProgressionDescriptionResource::ResourceManagerType: return GetAllGuidResourcesTyped<ProgressionDescriptionResource>(L);
		case GossipDefinition::ResourceManagerType: return GetAllGuidResourcesTyped<GossipDefinition>(L);
		case ActionResourceGroupDefinition::ResourceManagerType: return GetAllGuidResourcesTyped<ActionResourceGroupDefinition>(L);
		case ColorDefinitionResource::ResourceManagerType: return GetAllGuidResourcesTyped<ColorDefinitionResource>(L);
		case EquipmentTypeResource::ResourceManagerType: return GetAllGuidResourcesTyped<EquipmentTypeResource>(L);
		case FlagResource::ResourceManagerType: return GetAllGuidResourcesTyped<FlagResource>(L);
		case FeatResource::ResourceManagerType: return GetAllGuidResourcesTyped<FeatResource>(L);
		case FeatDescriptionResource::ResourceManagerType: return GetAllGuidResourcesTyped<FeatDescriptionResource>(L);

		default:
			LuaError("Resource type not supported: " << type);
			push(L, nullptr);
			return 1;
		}
	}


	ResourceBank* GetCurrentResourceBank()
	{
		auto resMgr = GetStaticSymbols().ResourceMgr;
		if (!resMgr || !*resMgr || !(*resMgr)->ResourceBanks[0]) {
			return nullptr;
		}

		if ((*resMgr)->ResourceBanks[0]->Packages.Count() > 0) {
			return (*resMgr)->ResourceBanks[0];
		} else {
			return (*resMgr)->ResourceBanks[1];
		}
	}

	
	int GetResource(lua_State* L)
	{
		auto resourceGuid = checked_get<FixedString>(L, 1);
		auto type = checked_get<ResourceBankType>(L, 2);

		auto bank = GetCurrentResourceBank();
		if (!bank) {
			LuaError("Resource manager not available");
			push(L, nullptr);
			return 1;
		}

		auto resource = bank->Container.Banks[(unsigned)type]->Resources.Find(resourceGuid);
		if (resource) {
			ObjectProxy::MakeRef<Resource>(L, *resource, GetCurrentLifetime());
		} else {
			push(L, nullptr);
		}

		return 1;
	}


	int GetAllResources(lua_State* L)
	{
		auto type = checked_get<ResourceBankType>(L, 1);

		auto bank = GetCurrentResourceBank();
		if (!bank) {
			LuaError("Resource manager not available");
			push(L, nullptr);
			return 1;
		}

		lua_newtable(L);
		int i{ 1 };
		for (auto const& resource : bank->Container.Banks[(unsigned)type]->Resources) {
			settable(L, i++, resource.Key);
		}

		return 1;
	}


	void RegisterStaticDataLib(lua_State* L)
	{
		static const luaL_Reg staticDataLib[] = {
			{"GetGuidResource", GetGuidResource},
			{"GetAllGuidResources", GetAllGuidResources},

			{"GetResource", GetResource},
			{"GetAllResources", GetAllResources},

			{0,0}
		};

		RegisterLib(L, "StaticData", staticDataLib);
	}
}
