#include <GameDefinitions/Surface.h>
#include <Lua/LuaBinding.h>

namespace bg3se::lua
{
	/*char const* const ObjectProxy<esv::Surface>::MetatableName = "esv::Surface";

	esv::Surface* ObjectProxy<esv::Surface>::Get(lua_State* L)
	{
		if (obj_) return obj_;

		auto level = GetStaticSymbols().GetCurrentServerLevel();
		if (level) {
			auto surface = level->SurfaceManager->Get(handle_);
			if (surface == nullptr) luaL_error(L, "Surface handle invalid");
			return surface;
		} else {
			return nullptr;
		}
	}

	int ObjectProxy<esv::Surface>::Index(lua_State* L)
	{
		auto surface = Get(L);
		auto prop = checked_get<char const*>(L, 2);

		if (strcmp(prop, "RootTemplate") == 0) {
			auto tmpl = GetStaticSymbols().GetSurfaceTemplate(surface->SurfaceType);
			if (tmpl != nullptr) {
				ObjectProxy<SurfaceTemplate>::New(L, tmpl);
			} else {
				LuaError("Couldn't fetch surface template of type " << (unsigned)surface->SurfaceType);
				push(L, nullptr);
			}
			return 1;
		}

		return GenericGetter(L, gEsvSurfacePropertyMap);
	}

	int ObjectProxy<esv::Surface>::NewIndex(lua_State* L)
	{
		return GenericSetter(L, gEsvSurfacePropertyMap);
	}


	char const* const ObjectProxy<esv::SurfaceAction>::MetatableName = "esv::SurfaceAction";

	esv::SurfaceAction* ObjectProxy<esv::SurfaceAction>::Get(lua_State* L)
	{
		if (obj_) return obj_;
		luaL_error(L, "SurfaceAction object not bound (maybe it was executed already?)");
		return nullptr;
	}

	PropertyMapBase& GetSurfaceActionPropertyMap(esv::SurfaceAction* action)
	{
		switch (action->VMT->GetTypeId(action)) {
		case SurfaceActionType::CreateSurfaceAction:
			return gEsvCreateSurfaceActionPropertyMap;
		case SurfaceActionType::CreatePuddleAction:
			return gEsvCreatePuddleActionPropertyMap;
		case SurfaceActionType::ExtinguishFireAction:
			return gEsvExtinguishFireActionPropertyMap;
		case SurfaceActionType::ZoneAction:
			return gEsvZoneActionPropertyMap;
		case SurfaceActionType::TransformSurfaceAction:
			return gEsvTransformSurfaceActionPropertyMap;
		case SurfaceActionType::ChangeSurfaceOnPathAction:
			return gEsvChangeSurfaceOnPathActionPropertyMap;
		case SurfaceActionType::RectangleSurfaceAction:
			return gEsvRectangleSurfaceActionPropertyMap;
		case SurfaceActionType::PolygonSurfaceAction:
			return gEsvPolygonSurfaceActionPropertyMap;
		case SurfaceActionType::SwapSurfaceAction:
			return gEsvSwapSurfaceActionPropertyMap;
		default:
			OsiError("No property map found for this surface type!");
			return gEsvSurfaceActionPropertyMap;
		}
	}

	int ObjectProxy<esv::SurfaceAction>::Index(lua_State* L)
	{
		auto action = Get(L);
		if (!action) return 0;

		return GenericGetter(L, GetSurfaceActionPropertyMap(action));
	}

	int ObjectProxy<esv::SurfaceAction>::NewIndex(lua_State* L)
	{
		StackCheck _(L, 0);
		auto action = Get(L);

		auto prop = luaL_checkstring(L, 2);

		if (strcmp(prop, "DamageList") == 0) {
			auto& damageList = DamageList::CheckUserData(L, 3)->Get();
			switch (action->VMT->GetTypeId(action)) {
			case SurfaceActionType::RectangleSurfaceAction:
			{
				auto act = static_cast<esv::RectangleSurfaceAction*>(action);
				act->DamageList.CopyFrom(damageList);
				break;
			}

			case SurfaceActionType::PolygonSurfaceAction:
			{
				auto act = static_cast<esv::PolygonSurfaceAction*>(action);
				act->DamageList.CopyFrom(damageList);
				break;
			}

			case SurfaceActionType::ZoneAction:
			{
				auto act = static_cast<esv::ZoneAction*>(action);
				act->DamageList.CopyFrom(damageList);
				break;
			}

			default:
				OsiError("This surface action type doesn't have a DamageList!");
			}

			return 0;
		}

		if (strcmp(prop, "Vertices") == 0) {
			if (action->VMT->GetTypeId(action) == SurfaceActionType::PolygonSurfaceAction) {
				auto act = static_cast<esv::PolygonSurfaceAction*>(action);
				act->PolygonVertices.Clear();
				luaL_checktype(L, 3, LUA_TTABLE);
				for (auto idx : iterate(L, 3)) {
					auto vec2 = checked_get<glm::vec2>(L, idx);
					act->PolygonVertices.Add(vec2);
				}
			} else {
				OsiError("Vertices only supported for surface action type PolygonSurfaceAction!");
			}

			return 0;
		}

		auto const& propertyMap = GetSurfaceActionPropertyMap(action);
		return GenericSetter(L, propertyMap);
	}*/
}


namespace bg3se::esv::lua
{
	using namespace bg3se::lua;

	int GetSurface(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*
		LuaServerPin lua(ExtensionState::Get());
		if (lua->RestrictionFlags & State::RestrictHandleConversion) {
			return luaL_error(L, "Attempted to resolve item handle in restricted context");
		}

		auto handle = checked_get<ObjectHandle>(L, 1);

		auto level = GetStaticSymbols().GetCurrentServerLevel();
		if (!level || !level->SurfaceManager) {
			OsiError("Current level not available yet!");
			return 0;
		}

		auto surface = level->SurfaceManager->Get(handle);
		if (surface != nullptr) {
			ObjectProxy<esv::Surface>::New(L, handle);
			return 1;
		} else {
			return 0;
		}*/
	}

	int GetSurfaceTransformRules(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*
		auto rules = GetStaticSymbols().eoc__SurfaceTransformActionsFromType;
		if (rules == nullptr) {
			OsiError("Surface transform rules not available!");
			return 0;
		}

		StackCheck _(L, 1);
		lua_newtable(L);

		for (auto i = 0; i < std::size(rules->SurfaceTypes); i++) {
			auto const& interactionSet = rules->SurfaceTypes[i].Interactions;

			if ((uint64_t)interactionSet[0].ActionableSurfaces[0] == 0) continue;

			lua_newtable(L);

			auto interactionIdx = 1;
			for (auto const& interaction : interactionSet) {
				if ((uint64_t)interaction.ActionableSurfaces[0] == 0) break;

				push(L, interactionIdx++);
				lua_newtable(L);

				setfield(L, "TransformType", interaction.ActionType);

				lua_newtable(L);
				int surfIdx = 1;
				for (auto const& surface : interaction.ActionableSurfaces) {
					if ((uint64_t)surface == 0) break;

					push(L, surfIdx++);
					push_flags(L, surface);
					lua_settable(L, -3);
				}

				lua_setfield(L, -2, "ActionableSurfaces");

				lua_settable(L, -3);
			}

			lua_setfield(L, -2, EnumInfo<SurfaceType>::Find((SurfaceType)i).Str);
		}

		return 1;*/
	}

	/*void UpdateSurfaceTransformInteraction(lua_State* L, int idx, eoc::SurfaceTransformInteractions::Interaction& interaction)
	{
		luaL_checktype(L, idx, LUA_TTABLE);
		auto actionType = checked_getfield<SurfaceTransformActionType>(L, "TransformType", idx);
		interaction.ActionType = actionType;

		lua_getfield(L, idx, "ActionableSurfaces");
		luaL_checktype(L, -1, LUA_TTABLE);
		int curActionIdx = 0;
		for (auto surfaceIdx : iterate(L, -1)) {
			if (curActionIdx >= std::size(interaction.ActionableSurfaces) - 1) {
				luaL_error(L, "At most %d actionable surfaces per transform type are supported", std::size(interaction.ActionableSurfaces) - 1);
			}

			auto surfaceFlags = checked_get_flags<ESurfaceFlag>(L, surfaceIdx);
			interaction.ActionableSurfaces[curActionIdx++] = surfaceFlags;
		}

		lua_pop(L, 1);
	}*/

	int UpdateSurfaceTransformRules(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*
		StackCheck _(L, 0);
		auto rules = GetStaticSymbols().eoc__SurfaceTransformActionsFromType;
		if (rules == nullptr) {
			OsiError("Surface transform rules not available!");
			return 0;
		}

		auto interactions = std::make_unique<eoc::SurfaceTransformInteractions>();
		memset(interactions.get(), 0, sizeof(eoc::SurfaceTransformInteractions));

		luaL_checktype(L, 1, LUA_TTABLE);
		for (auto idx : iterate(L, 1)) {
			auto surfaceType = checked_get<SurfaceType>(L, idx - 1);
			auto& surfaceRules = interactions->SurfaceTypes[(unsigned)surfaceType].Interactions;

			luaL_checktype(L, -1, LUA_TTABLE);
			int curInteractionIdx = 0;
			for (auto interactionIdx : iterate(L, -1)) {
				if (curInteractionIdx >= std::size(surfaceRules) - 1) {
					luaL_error(L, "At most %d rules per surface are supported", std::size(surfaceRules) - 1);
				}

				UpdateSurfaceTransformInteraction(L, interactionIdx, surfaceRules[curInteractionIdx++]);
			}

		}

		*rules = *interactions;
		return 0;*/
	}

	SurfaceAction* CreateSurfaceAction(SurfaceActionType type)
	{
		auto const& sym = GetStaticSymbols();
		auto factory = sym.GetSurfaceActionFactory();
		if (!factory || !sym.esv__SurfaceActionFactory__CreateAction) {
			OsiError("esv::SurfaceActionFactory not mapped!");
			return nullptr;
		}

		auto resourceMgr = gExtender->GetServerEntityHelpers().GetResourceManager<ClassDescriptionResource>();
		if (!resourceMgr) {
			OsiError("ClassDescriptionResource resource manager not available");
			return nullptr;
		}

		auto action = sym.esv__SurfaceActionFactory__CreateAction(factory, type, *resourceMgr, ObjectHandle::NullHandle);
		if (!action) {
			OsiError("Couldn't create surface action for some reason.");
			return nullptr;
		}

		return action;
	}

#define TY(cls) case SurfaceActionType::cls: ObjectProxy::MakeRef<esv::cls>(L, static_cast<esv::cls*>(action), GetServerLifetime()); break;

	void CreateSurfaceActionRef(lua_State* L, SurfaceAction* action)
	{
		switch (action->VMT->GetTypeId(action)) {
		TY(CreateSurfaceAction)
		TY(CreatePuddleAction)
		TY(ExtinguishFireAction)
		TY(ZoneAction)
		TY(TransformSurfaceAction)
		TY(ChangeSurfaceOnPathAction)
		TY(RectangleSurfaceAction)
		TY(PolygonSurfaceAction)
		default:
			OsiError("Can't push surface actions of type " << action->VMT->GetTypeId(action));
			push(L, nullptr);
			break;
		}
	}

	int LuaCreateSurfaceAction(lua_State* L)
	{
		StackCheck _(L, 1);
		auto type = checked_get<SurfaceActionType>(L, 1);

		auto action = CreateSurfaceAction(type);
		if (action) {
			CreateSurfaceActionRef(L, action);
		} else {
			push(L, nullptr);
		}

		return 1;
	}

	int LuaExecuteSurfaceAction(lua_State* L)
	{
		StackCheck _(L, 0);
		auto action = checked_get_proxy<SurfaceAction>(L, 1);
		if (!action) {
			OsiError("Attempted to execute surface action more than once!");
			return 0;
		}

		auto level = GetStaticSymbols().GetCurrentServerLevel();
		if (!level) {
			OsiError("Attempted to execute surface action while no level is loaded!");
			return 0;
		}

		auto exec = GetStaticSymbols().esv__SurfaceManager__AddAction;
		if (!exec) {
			OsiError("esv::SurfaceManager::AddAction not mapped!");
			return 0;
		}

		action->Level = level;

		if (action->VMT->GetTypeId(action) == SurfaceActionType::TransformSurfaceAction) {
			OsiError("TransformSurfaceAction not supported yet!");
			return 0;
			/*FIXME
			auto transform = static_cast<TransformSurfaceAction*>(action);
			GetStaticSymbols().esv__TransformSurfaceAction__Init(
				transform, transform->SurfaceTransformAction, transform->SurfaceLayer, transform->OriginSurface
			);
			*/
		}

		exec(level->SurfaceManager, action);
		return 0;
	}

	int CancelSurfaceAction(lua_State* L)
	{
		StackCheck _(L, 0);
		auto handle = checked_get<ObjectHandle>(L, 1);
		
		auto factory = GetStaticSymbols().GetSurfaceActionFactory();
		if (!factory) {
			OsiError("SurfaceActionFactory not available!");
			return 0;
		}

		auto xaction = factory->Get(handle);
		if (!xaction) {
			OsiWarn("No surface action found with handle " << std::hex << handle.Handle << "; maybe it already expired?");
			return 0;
		}

		auto action = reinterpret_cast<SurfaceAction*>(xaction);

		switch (action->VMT->GetTypeId(action)) {
		case SurfaceActionType::CreateSurfaceAction:
		{
			auto act = static_cast<esv::CreateSurfaceAction*>(action);
			act->CurrentCellCount = act->SurfaceCells.Size;
			break;
		}
		case SurfaceActionType::CreatePuddleAction:
		{
			auto act = static_cast<esv::CreatePuddleAction*>(action);
			act->IsFinished = true;
			break;
		}
		case SurfaceActionType::ExtinguishFireAction:
		{
			auto act = static_cast<esv::ExtinguishFireAction*>(action);
			act->Percentage = 0.0f;
			break;
		}
		case SurfaceActionType::ZoneAction:
		{
			auto act = static_cast<esv::ZoneAction*>(action);
			act->CurrentCellCount = act->SurfaceCells.Size;
			break;
		}
		case SurfaceActionType::ChangeSurfaceOnPathAction:
		{
			auto act = static_cast<esv::ChangeSurfaceOnPathAction*>(action);
			act->IsFinished = true;
			break;
		}
		case SurfaceActionType::RectangleSurfaceAction:
		{
			auto act = static_cast<esv::RectangleSurfaceAction*>(action);
			act->CurrentCellCount = act->SurfaceCells.Size;
			break;
		}
		case SurfaceActionType::PolygonSurfaceAction:
		{
			auto act = static_cast<esv::PolygonSurfaceAction*>(action);
			act->LastSurfaceCellCount = act->SurfaceCells.Size;
			break;
		}
		case SurfaceActionType::TransformSurfaceAction:
		{
			auto act = static_cast<esv::TransformSurfaceAction*>(action);
			act->Finished = true;
			break;
		}
		default:
			OsiError("CancelSurfaceAction() not implemented for this surface action type!");
		}

		return 0;
	}

	void RegisterSurfaceLibrary(lua_State* L)
	{
		static const luaL_Reg jsonLib[] = {
			{"GetSurface", GetSurface},
			{"GetTransformRules", GetSurfaceTransformRules},
			{"UpdateTransformRules", UpdateSurfaceTransformRules},
			{"CreateAction", LuaCreateSurfaceAction},
			{"ExecuteAction", LuaExecuteSurfaceAction},
			{"CancelAction", CancelSurfaceAction},
			{0,0}
		};

		RegisterLib(L, "Surface", jsonLib);
	}
}
