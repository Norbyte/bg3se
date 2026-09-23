#include <stdafx.h>
#include <Extender/ScriptExtender.h>
#include <Lua/LuaBinding.h>
#include <GameDefinitions/Stats/Stats.h>
#include <GameDefinitions/Stats/Cache.h>
#include <Lua/Shared/LuaStats.h>

BEGIN_NS(lua::stats)


FixedString ObjectHelpers::GetModifierList(Object const* obj)
{
    auto stats = GetStaticSymbols().GetStats();
    auto modifierList = stats->ModifierLists.GetByHandle(obj->ModifierListIndex);
    return modifierList ? modifierList->Name : FixedString{};
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
        auto parent = stats->Objects.GetByHandle(obj->Using);
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

    if (persist) {
        WARN_ONCE("The 'persist' argument to Ext.Stats.Sync() is deprecated");
    }
}

void ObjectHelpers::SetPersistence(Object* obj, bool persist)
{
    WARN_ONCE("Ext.Stats.SetPersistence() is deprecated");
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
    auto info = gStatStructureCache.GetCachedAttribute(object->ModifierListIndex, key);
    if (info == nullptr) {
        OsiError("Stats object '" << object->Name << "' has no attribute named '" << key << "'");
        return false;
    }

    if (info->Type == RPGEnumerationType::StatsFunctors) {
        ClearStatsFunctors(object, key, value);
    }

    auto set = GetStaticSymbols().stats__Object__SetPropertyString;
    set(object, key, value);

    return true;
}


bool ObjectHelpers::CopyFrom(Object* object, FixedString parent)
{
    auto stats = GetStaticSymbols().GetStats();
    auto copyFromObject = stats->Objects.GetByName(parent);
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


PropertyOperationResult ObjectHelpers::FallbackGet(lua_State* L, lua::LifetimeHandle lifetime, Object const* object, bg3se::FixedString const& prop)
{
    return LuaStatGetAttribute(L, object, prop);
}


PropertyOperationResult ObjectHelpers::FallbackSet(lua_State* L, Object* object, bg3se::FixedString const& prop, int index)
{
    return LuaStatSetAttribute(L, object, prop, 3);
}


int ObjectHelpers::FallbackNext(lua_State* L, LifetimeHandle lifetime, Object const* object, FixedString const& prop)
{
    auto stats = GetStaticSymbols().GetStats();

    auto modifiers = stats->ModifierLists.GetByHandle(object->ModifierListIndex);

    Modifier* next;
    if (!prop) {
        if (modifiers->Attributes.Values.empty()) {
            return 0;
        }

        next = modifiers->Attributes.Values[0];
    } else {
        auto index = modifiers->Attributes.GetHandleByName(prop);
        if (index == -1) {
            next = modifiers->Attributes.Values[0];
        } else if ((uint32_t)index < modifiers->Attributes.Values.size() - 1) {
            next = modifiers->Attributes.Values[index + 1];
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
    auto attrInfo = gStatStructureCache.GetCachedAttribute(object->ModifierListIndex, attributeName);
    if (!attrInfo) {
        OsiError("Stat object '" << object->Name << "' has no attribute named '" << attributeName << "'");
        return PropertyOperationResult::NoSuchProperty;
    }

    object->TryPushValue(L, *attrInfo);
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

    auto attrInfo = gStatStructureCache.GetCachedAttribute(object->ModifierListIndex, attributeName);
    if (!attrInfo) {
        LuaError("Object '" << object->Name << "' has no attribute named '" << attributeName << "'");
        return PropertyOperationResult::NoSuchProperty;
    }

    if (object->TrySetValue(L, *attrInfo, valueIdx)) {
        return PropertyOperationResult::Success;
    } else {
        return PropertyOperationResult::UnsupportedType;
    }
}

END_NS()
