namespace bg3se::esv::lua
{

template <class Predicate>
void GetCharactersGeneric(lua_State* L, FixedString const& requestedLevel, Predicate pred, bool excludeOffStage, bool excludeDeactivated)
{
	int index{ 1 };

	lua_newtable(L);
	FixedString levelName = requestedLevel;
	if (!levelName) {
		levelName = GetStaticSymbols().GetCurrentServerLevel();
		if (!levelName) {
			OsiError("No current level!");
			return;
		}
	}

	auto predicate = [L, levelName, pred, excludeOffStage, excludeDeactivated, &index](Character* ch) {
		if (excludeOffStage && ch->HasFlag((uint64_t)CharacterFlags::OffStage)) return;

		auto level = gOsirisProxy->GetServerEntityHelpers().GetEntityComponent<LevelComponent>(ch->Base.Entity, false);
		if (!level || level->LevelName != levelName) return;
		
		if (excludeDeactivated) {
			auto active = gOsirisProxy->GetServerEntityHelpers().GetEntityComponent<esv::ActiveComponent>(ch->Base.Entity, false);
			if (!active) return;
		}

		if (!pred(ch)) return;

		settable(L, index++, ch->GUID);
	};

	gOsirisProxy->GetServerEntityHelpers().IterateComponents<Character>(predicate);
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
	glm::vec3 pos(
		checked_get<float>(L, 1),
		checked_get<float>(L, 2),
		checked_get<float>(L, 3)
	);
	float distance = checked_get<float>(L, 4);

	bool excludeOffStage{ false };
	bool excludeDeactivated{ false };

	if (numArgs >= 5) {
		excludeOffStage = checked_get<bool>(L, 5);
	}

	if (numArgs >= 6) {
		excludeDeactivated = checked_get<bool>(L, 6);
	}

	GetCharactersGeneric(L, FixedString{}, [pos, distance](esv::Character* c) {
		auto transform = gOsirisProxy->GetServerEntityHelpers().GetEntityComponent<TransformComponent>(c->Base.Entity, false);
		if (!transform) return false;

		return abs(glm::length(pos - transform->Transform.Translate)) < distance;
	}, excludeOffStage, excludeDeactivated);
	return 1;
}

/*
template <class Predicate>
void GetItemsGeneric(lua_State* L, FixedString const& requestedLevel, Predicate pred)
{
	int index{ 1 };

	lua_newtable(L);
	FixedString levelName = requestedLevel;
	if (!levelName) {
		levelName = GetStaticSymbols().GetCurrentServerLevel();
		if (!levelName) {
			OsiError("No current level!");
			return;
		}
	}

	auto& helpers = GetEoCServer()->EntityManager->ItemConversionHelpers;
	auto items = helpers.RegisteredItems.Find(levelName);
	if (items == nullptr) {
		OsiError("No items registered for level: " << levelName);
		return;
	}

	for (auto item : **items) {
		if (pred(item)) {
			settable(L, index++, item->MyGuid);
		}
	}
}

int GetAllItems(lua_State* L)
{
	FixedString levelName;
	if (lua_gettop(L) >= 1) {
		levelName = checked_get<FixedString>(L, 1);
	}

	GetItemsGeneric(L, levelName, [](esv::Item*) { return true; });
	return 1;
}

int GetItemsAroundPosition(lua_State* L)
{
	glm::vec3 pos(
		checked_get<float>(L, 1),
		checked_get<float>(L, 2),
		checked_get<float>(L, 3)
	);
	float distance = checked_get<float>(L, 4);

	GetItemsGeneric(L, FixedString{}, [pos, distance](esv::Item* i) {
		auto transform = gOsirisProxy->GetServerEntityHelpers().GetEntityComponent<TransformComponent>(i->Base.Entity, false);
		if (!transform) return false;

		return abs(glm::length(pos - transform->Transform.Translate)) < distance;
	});
	return 1;
}

*/
}