#include <stdafx.h>
#include <Extender/ScriptExtender.h>
#include <Lua/LuaBinding.h>
#include <GameDefinitions/Stats/Stats.h>
#include <Lua/Shared/LuaStats.h>

BEGIN_NS(lua::stats)


FixedString ObjectHelpers::GetModifierList(Object const* obj)
{
	auto stats = GetStaticSymbols().GetStats();
	return stats->ModifierLists.Find(obj->ModifierListIndex)->Name;
}


FixedString ObjectHelpers::GetModId(Object const* obj)
{
	auto mod = gExtender->GetStatLoadOrderHelper().GetStatsEntryMod(obj->Name);
	return mod ? mod->LastMod : FixedString{};
}


FixedString ObjectHelpers::GetOriginalModId(Object const* obj)
{
	auto mod = gExtender->GetStatLoadOrderHelper().GetStatsEntryMod(obj->Name);
	return mod ? mod->FirstMod : FixedString{};
}


FixedString ObjectHelpers::GetUsing(Object const* obj)
{
	auto stats = GetStaticSymbols().GetStats();
	if (obj->Using) {
		auto parent = stats->Objects.Find(obj->Using);
		if (parent != nullptr) {
			return parent->Name;
		}
	}

	return {};
}


void ObjectHelpers::Sync(Object* object, std::optional<bool> persist)
{
	auto stats = GetStaticSymbols().GetStats();

	stats->SyncWithPrototypeManager(object);

	if (gExtender->GetServer().IsInServerThread()) {
		object->BroadcastSyncMessage(false);

		gExtender->GetServer().GetExtensionState().MarkDynamicStat(object->Name);
		if (persist) {
			gExtender->GetServer().GetExtensionState().MarkPersistentStat(object->Name);
		}
	}
}


void ObjectHelpers::SetPersistence(Object* object, bool persist)
{
	if (persist) {
		gExtender->GetServer().GetExtensionState().MarkPersistentStat(object->Name);
	} else {
		gExtender->GetServer().GetExtensionState().UnmarkPersistentStat(object->Name);
	}
}

void ClearStatsFunctors(Object* object, FixedString key, char const* value)
{
	// We need to delete the functors beforehand, otherwise updating them will
	// delete the functor object while inherited stats entries may still use it
	auto stats = GetStaticSymbols().GetStats();
	STDString setName = object->Name.GetString();
	setName += '_';
	setName += key.GetString();
	setName += '_';
	setName += "Default";
	auto it = stats->StatsFunctors.find(FixedString(setName));
	if (it != stats->StatsFunctors.end()) {
		stats->StatsFunctors.erase(it);
	}

	// Try to find cast keys
	STDString functors(value);
	STDString::size_type pos = 0;
	for (;;) {
		auto nextKey = functors.find_first_of('[', pos);
		if (nextKey != STDString::npos) {
			pos = nextKey + 1;
			auto end = nextKey;
			auto start = end;
			while (start > 0 && isalnum(functors[start - 1])) {
				start--;
			}

			auto textKey = functors.substr(start, end - start);
			setName = object->Name.GetString();
			setName += '_';
			setName += key.GetString();
			setName += '_';
			setName += textKey;
			auto it = stats->StatsFunctors.find(FixedString(setName));
			if (it != stats->StatsFunctors.end()) {
				stats->StatsFunctors.erase(it);
			}
		} else {
			break;
		}
	}

	object->Functors.remove(key);
}

bool ObjectHelpers::SetRawAttribute(Object* object, FixedString key, char const* value)
{
	int attrIndex{ 0 };
	auto info = object->GetAttributeInfo(key, attrIndex);
	if (info == nullptr) {
		OsiError("Stats object '" << object->Name << "' has no attribute named '" << key << "'");
		return false;
	}

	if (info->GetPropertyType() == RPGEnumerationType::StatsFunctors) {
		ClearStatsFunctors(object, key, value);
	}

	auto set = GetStaticSymbols().stats__Object__SetPropertyString;
	set(object, key, value);

	return true;
}


bool ObjectHelpers::CopyFrom(Object* object, FixedString parent)
{
	auto stats = GetStaticSymbols().GetStats();
	auto copyFromObject = stats->Objects.Find(parent);
	if (copyFromObject == nullptr) {
		OsiError("Cannot copy stats from nonexistent object: " << parent);
		return false;
	// Self-inheritance should not copy anything
	} else if (copyFromObject != object) {
		return object->CopyFrom(copyFromObject);
	} else {
		return true;
	}
}


PropertyOperationResult ObjectHelpers::FallbackGet(lua_State* L, lua::LifetimeHandle const& lifetime, Object const* object, bg3se::FixedString const& prop)
{
	return LuaStatGetAttribute(L, object, prop);
}


PropertyOperationResult ObjectHelpers::FallbackSet(lua_State* L, Object* object, bg3se::FixedString const& prop, int index)
{
	return LuaStatSetAttribute(L, object, prop, 3);
}


int ObjectHelpers::FallbackNext(lua_State* L, LifetimeHandle const& lifetime, Object const* object, FixedString const& prop)
{
	auto stats = GetStaticSymbols().GetStats();

	auto modifiers = stats->ModifierLists.Find(object->ModifierListIndex);

	Modifier* next;
	if (!prop) {
		if (modifiers->Attributes.Primitives.empty()) {
			return 0;
		}

		next = modifiers->Attributes.Primitives[0];
	} else {
		auto index = modifiers->Attributes.FindIndex(prop);
		if (!index) {
			next = modifiers->Attributes.Primitives[0];
		} else if ((uint32_t)*index < modifiers->Attributes.Primitives.size() - 1) {
			next = modifiers->Attributes.Primitives[*index + 1];
		} else {
			return 0;
		}
	}

	push(L, next->Name);
	auto result = LuaStatGetAttribute(L, object, next->Name);
	if (result != PropertyOperationResult::Success) {
		push(L, nullptr);
	}

	return 2;
}


PropertyOperationResult LuaStatGetAttribute(lua_State* L, stats::Object const* object, FixedString const& attributeName)
{
	StackCheck _(L, 1);
	auto stats = GetStaticSymbols().GetStats();

	int attributeIndex{ -1 };
	auto attrInfo = object->GetAttributeInfo(attributeName, attributeIndex);
	if (!attrInfo) {
		OsiError("Stat object '" << object->Name << "' has no attribute named '" << attributeName << "'");
		return PropertyOperationResult::NoSuchProperty;
	}

	switch (attrInfo->GetPropertyType()) {
	case RPGEnumerationType::Int:
	{
		auto value = object->GetInt(attributeName);
		LuaWrite(L, value);
		break;
	}

	case RPGEnumerationType::Int64:
	{
		auto value = object->GetInt64(attributeName);
		LuaWrite(L, value);
		break;
	}

	case RPGEnumerationType::Float:
	{
		auto value = object->GetFloat(attributeName);
		LuaWrite(L, value);
		break;
	}

	case RPGEnumerationType::FixedString:
	case RPGEnumerationType::Enumeration:
	case RPGEnumerationType::Conditions:
	{
		auto value = object->GetString(attributeName);
		if (value) {
			push(L, *value);
		} else {
			push(L, "");
		}
		break;
	}

	case RPGEnumerationType::GUID:
	{
		auto value = object->GetGuid(attributeName);
		LuaWrite(L, value);
		break;
	}

	case RPGEnumerationType::Flags:
	{
		auto value = object->GetFlags(attributeName);
		LuaWrite(L, value);
		break;
	}

	case RPGEnumerationType::Requirements:
	{
		LuaWrite(L, object->Requirements);
		break;
	}

	// Deprecated type
	case RPGEnumerationType::MemorizationRequirements:
	{
		push(L, nullptr);
		break;
	}

	case RPGEnumerationType::TranslatedString:
	{
		auto value = object->GetTranslatedString(attributeName);
		LuaWrite(L, value);
		break;
	}

	case RPGEnumerationType::RollConditions:
	{
		auto conditions = object->GetRollConditions(attributeName);
		if (conditions && *conditions) {
			lua_newtable(L);
			for (auto const& cond : **conditions) {
				auto condition = stats->GetConditions(cond.Conditions.Id);
				if (condition && *condition) {
					settable(L, cond.Name, **condition);
				}
			}
		} else {
			push(L, nullptr);
		}
		break;
	}

	case RPGEnumerationType::StatsFunctors:
	{
		auto functors = object->GetFunctors(attributeName);
		if (functors) {
			push(L, **functors, lua::GetCurrentLifetime());
		} else {
			push(L, nullptr);
		}
		break;
	}

	default:
		OsiError("Don't know how to fetch values of type '" << attrInfo->Name << "'");
		push(L, nullptr);
		break;
	}

	return PropertyOperationResult::Success;
}


PropertyOperationResult LuaStatSetAttribute(lua_State* L, stats::Object* object, FixedString const& attributeName, int valueIdx)
{
	StackCheck _(L);
	auto lua = State::FromLua(L);

	if (lua->RestrictionFlags & State::ScopeModulePreLoad) {
		luaL_error(L, "Stat functions unavailable during module preload");
		return PropertyOperationResult::Unknown;
	}

	if (!(lua->RestrictionFlags & State::ScopeModuleLoad)) {
		static bool syncWarningShown{ false };
		if (!syncWarningShown) {
			OsiWarn("Stats edited after ModuleLoad must be synced manually; make sure that you call Sync() on it when you're finished!");
			syncWarningShown = true;
		}
	}

	auto stats = GetStaticSymbols().GetStats();
		
	int index;
	auto attrInfo = object->GetAttributeInfo(attributeName, index);
	if (!attrInfo) {
		LuaError("Object '" << object->Name << "' has no attribute named '" << attributeName << "'");
		return PropertyOperationResult::NoSuchProperty;
	}

	auto attrType = attrInfo->GetPropertyType();

	switch (lua_type(L, valueIdx)) {
	case LUA_TSTRING:
	{
		auto value = luaL_checkstring(L, valueIdx);
		object->SetString(attributeName, value);
		break;
	}

	case LUA_TNUMBER:
	{
		switch (attrType) {
		case RPGEnumerationType::Int64:
			object->SetInt64(attributeName, (int64_t)luaL_checkinteger(L, valueIdx));
			break;

		case RPGEnumerationType::Float:
			object->SetFloat(attributeName, (float)luaL_checknumber(L, valueIdx));
			break;

		default:
			object->SetInt(attributeName, (int32_t)luaL_checkinteger(L, valueIdx));
			break;
		}
		break;
	}

	case LUA_TTABLE:
	{
		switch (attrType) {
		case RPGEnumerationType::Flags:
		{
			Array<STDString> flags;
			lua_pushvalue(L, valueIdx);
			LuaRead(L, flags);
			lua_pop(L, 1);
			object->SetFlags(attributeName, flags);
			break;
		}

		case RPGEnumerationType::RollConditions:
		{
			HashMap<FixedString, STDString> rolls;
			lua_pushvalue(L, valueIdx);
			LuaRead(L, rolls);
			lua_pop(L, 1);

			Array<stats::Object::RollCondition> conditions;
			for (auto const& kv : rolls) {
				auto conditionsId = stats->GetOrCreateConditions(kv.Value());
				if (conditionsId >= 0) {
					stats::Object::RollCondition roll;
					roll.Name = kv.Key();
					roll.Conditions.Id = conditionsId;
					conditions.Add(roll);
				}
			}

			object->SetRollConditions(attributeName, conditions);
			break;
		}

		/*case RPGEnumerationType::StatsFunctors:
		{
			Functors* functor = stats->ConstructFunctorSet(attributeName);
			lua_pushvalue(L, valueIdx);
			LuaRead(L, functor);
			lua_pop(L, 1);

			Array<stats::Object::FunctorInfo> functors;
			if (functor) {
				stats::Object::FunctorInfo functorInfo;
				functorInfo.Name = GFS.strDefault;
				functorInfo.Functor = functor;
				functors.Add(functorInfo);
			}

			object->SetFunctors(attributeName, functors);
			break;
		}*/

		case RPGEnumerationType::Requirements:
		{
			Array<stats::Requirement> requirements;
			lua_pushvalue(L, valueIdx);
			LuaRead(L, requirements);
			lua_pop(L, 1);
			object->Requirements = requirements;
			break;
		}

		case RPGEnumerationType::TranslatedString:
		{
			TranslatedString ts;
			lua_pushvalue(L, valueIdx);
			LuaRead(L, ts);
			lua_pop(L, 1);
			object->SetTranslatedString(attributeName, ts);
			break;
		}

		default:
			LuaError("Cannot use table value for stat property " << attributeName << " of type " << (unsigned)attrType << "!");
			return PropertyOperationResult::UnsupportedType;
		}
		break;
	}

	case LUA_TNIL:
	{
		switch (attrType) {
		case RPGEnumerationType::Float:
			object->SetFloat(attributeName, {});
			break;

		case RPGEnumerationType::GUID:
			object->SetGuid(attributeName, {});
			break;

		case RPGEnumerationType::TranslatedString:
			object->SetTranslatedString(attributeName, {});
			break;

		case RPGEnumerationType::StatsFunctors:
			object->SetFunctors(attributeName, {});
			break;

		default:
			LuaError("Cannot use nil value for stat property " << attributeName << " of type " << (unsigned)attrType << "!");
			return PropertyOperationResult::UnsupportedType;
		}
		break;
	}

	default:
		LuaError("Lua property values of type '" << lua_typename(L, lua_type(L, valueIdx)) << "' are not supported");
		return PropertyOperationResult::UnsupportedType;
	}

	return PropertyOperationResult::Success;
}

END_NS()
