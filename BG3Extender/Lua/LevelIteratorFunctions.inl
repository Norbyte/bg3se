namespace bg3se::esv::lua
{

template <class Predicate>
void GetCharactersGeneric(lua_State* L, FixedString const& requestedLevel, Predicate pred, bool excludeOffStage, bool excludeDeactivated)
{
	int index{ 1 };

	lua_newtable(L);
	FixedString levelName = requestedLevel;
	if (!levelName) {
		levelName = GetStaticSymbols().GetCurrentServerLevelName();
		if (!levelName) {
			OsiError("No current level!");
			return;
		}
	}

	auto predicate = [L, levelName, pred, excludeOffStage, excludeDeactivated, &index](Character* ch) {
		if (excludeOffStage && ch->HasFlag((uint64_t)CharacterFlags::OffStage)) return;

		auto level = gExtender->GetServerEntityHelpers().GetEntityComponent<LevelComponent>(ch->Base.Entity, false);
		if (!level || level->LevelName != levelName) return;
		
		if (excludeDeactivated) {
			auto active = gExtender->GetServerEntityHelpers().GetEntityComponent<esv::ActiveComponent>(ch->Base.Entity, false);
			if (!active) return;
		}

		if (!pred(ch)) return;

		settable(L, index++, ch->GUID);
	};

	gExtender->GetServerEntityHelpers().IterateComponents<Character>(predicate);
}

int GetAllCharacters(lua_State* L)
{
	FixedString levelName;
	bool excludeOffStage{ false };
	bool excludeDeactivated{ false };

	auto numArgs = lua_gettop(L);
	if (numArgs >= 1) {
		levelName = checked_get<FixedString>(L, 1);
	}
	
	if (numArgs >= 2) {
		excludeOffStage = checked_get<bool>(L, 2);
	}
	
	if (numArgs >= 3) {
		excludeDeactivated = checked_get<bool>(L, 3);
	}

	GetCharactersGeneric(L, levelName, [](esv::Character*) { return true; }, excludeOffStage, excludeDeactivated);
	return 1;
}

int GetCharactersAroundPosition(lua_State* L)
{
	auto numArgs = lua_gettop(L);
	glm::vec3 pos = checked_get<glm::vec3>(L, 1);
	float distance = checked_get<float>(L, 2);

	bool excludeOffStage{ false };
	bool excludeDeactivated{ false };

	if (numArgs >= 3) {
		excludeOffStage = checked_get<bool>(L, 3);
	}

	if (numArgs >= 4) {
		excludeDeactivated = checked_get<bool>(L, 4);
	}

	GetCharactersGeneric(L, FixedString{}, [pos, distance](esv::Character* c) {
		auto transform = gExtender->GetServerEntityHelpers().GetEntityComponent<TransformComponent>(c->Base.Entity, false);
		if (!transform) return false;

		return abs(glm::length(pos - transform->Transform.Translate)) < distance;
	}, excludeOffStage, excludeDeactivated);
	return 1;
}


template <class Predicate>
void GetItemsGeneric(lua_State* L, FixedString const& requestedLevel, Predicate pred, bool excludeOffStage, bool excludeDeactivated)
{
	int index{ 1 };

	lua_newtable(L);
	FixedString levelName = requestedLevel;
	if (!levelName) {
		levelName = GetStaticSymbols().GetCurrentServerLevelName();
		if (!levelName) {
			OsiError("No current level!");
			return;
		}
	}

	auto predicate = [L, levelName, pred, excludeOffStage, excludeDeactivated, &index](Item* it) {
		if (excludeOffStage && it->HasFlag((uint64_t)ItemFlags::OffStage)) return;

		auto level = gExtender->GetServerEntityHelpers().GetEntityComponent<LevelComponent>(it->Base.Entity, false);
		if (!level || level->LevelName != levelName) return;

		if (excludeDeactivated) {
			auto active = gExtender->GetServerEntityHelpers().GetEntityComponent<esv::ActiveComponent>(it->Base.Entity, false);
			if (!active) return;
		}

		if (!pred(it)) return;

		settable(L, index++, it->GUID);
	};

	gExtender->GetServerEntityHelpers().IterateComponents<Item>(predicate);
}

int GetAllItems(lua_State* L)
{
	FixedString levelName;
	bool excludeOffStage{ false };
	bool excludeDeactivated{ false };

	auto numArgs = lua_gettop(L);
	if (numArgs >= 1) {
		levelName = checked_get<FixedString>(L, 1);
	}

	if (numArgs >= 2) {
		excludeOffStage = checked_get<bool>(L, 2);
	}

	if (numArgs >= 3) {
		excludeDeactivated = checked_get<bool>(L, 3);
	}

	GetItemsGeneric(L, levelName, [](esv::Item*) { return true; }, excludeOffStage, excludeDeactivated);
	return 1;
}

int GetItemsAroundPosition(lua_State* L)
{
	auto numArgs = lua_gettop(L);
	glm::vec3 pos = checked_get<glm::vec3>(L, 1);
	float distance = checked_get<float>(L, 2);

	bool excludeOffStage{ false };
	bool excludeDeactivated{ false };

	if (numArgs >= 3) {
		excludeOffStage = checked_get<bool>(L, 3);
	}

	if (numArgs >= 4) {
		excludeDeactivated = checked_get<bool>(L, 4);
	}

	GetItemsGeneric(L, FixedString{}, [pos, distance](esv::Item* it) {
		auto transform = gExtender->GetServerEntityHelpers().GetEntityComponent<TransformComponent>(it->Base.Entity, false);
		if (!transform) return false;

		return abs(glm::length(pos - transform->Transform.Translate)) < distance;
	}, excludeOffStage, excludeDeactivated);
	return 1;
}

}
