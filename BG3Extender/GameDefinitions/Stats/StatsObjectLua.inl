#include <GameDefinitions/Stats/Stats.h>
#include <GameDefinitions/Stats/Cache.h>

BEGIN_NS(stats)

using namespace lua;

void Object::TryPushValue(lua_State* L, StatModifierCache const& attribute) const
{
    StackCheck _(L, 1);

    switch (attribute.Type) {
    case RPGEnumerationType::Int:
        push(L, GetInt(attribute));
        break;

    case RPGEnumerationType::Int64:
        push(L, GetInt64(attribute));
        break;

    case RPGEnumerationType::Float:
        push(L, GetFloat(attribute));
        break;

    case RPGEnumerationType::FixedString:
    case RPGEnumerationType::Enumeration:
    {
        auto value = GetFixedStringRef(attribute);
        if (value) {
            push(L, *value);
        } else {
            push(L, "");
        }
        break;
    }

    case RPGEnumerationType::Conditions:
        push(L, GetConditions(attribute).value_or(""));
        break;

    case RPGEnumerationType::GUID:
        push(L, GetGuid(attribute));
        break;

    case RPGEnumerationType::Flags:
    {
        auto value = GetFlags(attribute);
        if (value) {
            SerializeArray(L, &*value);
        } else {
            push(L, nullptr);
        }
        break;
    }

    case RPGEnumerationType::Requirements:
        LuaWrite(L, Requirements);
        break;

    // Deprecated type
    case RPGEnumerationType::MemorizationRequirements:
        push(L, nullptr);
        break;

    case RPGEnumerationType::TranslatedString:
    {
        auto value = GetTranslatedString(attribute);
        if (value) {
            push(L, value->Handle.Handle);
        } else {
            push(L, nullptr);
        }
        break;
    }

    case RPGEnumerationType::RollConditions:
    {
        auto conditions = GetRollConditions(attribute);
        if (conditions && *conditions) {
            lua_newtable(L);
            auto stats = GetStaticSymbols().GetStats();
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
        auto functors = GetFunctors(attribute);
        if (functors) {
            push(L, **functors, lua::GetCurrentLifetime());
        } else {
            push(L, nullptr);
        }
        break;
    }

    case RPGEnumerationType::AIFlags:
        push(L, AIFlags);
        break;

    default:
        OsiError("Don't know how to fetch values of type '" << attribute.TypeName << "'");
        push(L, nullptr);
        break;
    }
}


bool Object::TrySetValue(lua_State* L, StatModifierCache const& attribute, int index)
{
    switch (lua_type(L, index)) {
    case LUA_TSTRING:
    {
        if (attribute.Type == RPGEnumerationType::FixedString
            || attribute.Type == RPGEnumerationType::AIFlags
            || attribute.Type == RPGEnumerationType::Enumeration
            || attribute.Type == RPGEnumerationType::Flags) {
            auto value = get<FixedString>(L, index);
            return SetFixedString(attribute, value);
        } else {
            auto value = luaL_checkstring(L, index);
            return SetString(attribute, value);
        }
    }

    case LUA_TNUMBER:
    {
        switch (attribute.Type) {
        case RPGEnumerationType::Int64:
            return SetInt64(attribute, (int64_t)luaL_checkinteger(L, index));

        case RPGEnumerationType::Float:
            return SetFloat(attribute, (float)luaL_checknumber(L, index));

        default:
            return SetInt(attribute, (int64_t)luaL_checkinteger(L, index));
        }
    }

    case LUA_TTABLE:
    {
        switch (attribute.Type) {
        case RPGEnumerationType::Flags:
        {
            Array<FixedString> flags;
            Unserialize(L, index, &flags);
            return SetFlags(attribute, flags);
        }

        case RPGEnumerationType::RollConditions:
        {
            HashMap<FixedString, STDString> rolls;
            Unserialize(L, index, &rolls);

            Array<RollCondition> conditions;
            auto stats = GetStaticSymbols().GetStats();
            for (auto const& kv : rolls) {
                auto conditionsId = stats->GetOrCreateConditions(kv.Value());
                if (conditionsId >= 0) {
                    RollCondition roll;
                    roll.Name = kv.Key();
                    roll.Conditions.Id = conditionsId;
                    conditions.push_back(roll);
                }
            }

            return SetRollConditions(attribute, conditions);
        }

        /*case RPGEnumerationType::StatsFunctors:
        {
            Functors* functor = stats->ConstructFunctorSet(attributeName);
            lua_pushvalue(L, index);
            LuaRead(L, functor);
            lua_pop(L, 1);

            Array<FunctorInfo> functors;
            if (functor) {
                FunctorInfo functorInfo;
                functorInfo.Name = GFS.strDefault;
                functorInfo.Functor = functor;
                functors.Add(functorInfo);
            }

            return SetFunctors(attribute, functors);
        }*/

        case RPGEnumerationType::Requirements:
        {
            Array<Requirement> requirements;
            lua_pushvalue(L, index);
            LuaRead(L, requirements);
            lua_pop(L, 1);
            Requirements = requirements;
            return true;
        }

        case RPGEnumerationType::TranslatedString:
        {
            TranslatedString ts;
            lua_pushvalue(L, index);
            LuaRead(L, ts);
            lua_pop(L, 1);
            return SetTranslatedString(attribute, ts);
        }

        default:
            LuaError("Cannot use table value for stat property " << attribute.Name << " of type " << attribute.TypeName << "!");
            return false;
        }
    }

    case LUA_TNIL:
    {
        switch (attribute.Type) {
        case RPGEnumerationType::Float:
            return SetFloat(attribute, {});

        case RPGEnumerationType::GUID:
            return SetGuid(attribute, {});

        case RPGEnumerationType::TranslatedString:
            return SetTranslatedString(attribute, {});

        case RPGEnumerationType::StatsFunctors:
            return SetFunctors(attribute, {});

        default:
            LuaError("Cannot use nil value for stat property " << attribute.Name << " of type " << attribute.TypeName << "!");
            return false;
        }
    }

    default:
        LuaError("Lua property values of type '" << GetDebugName(L, index) << "' are not supported");
        return false;
    }
}

END_NS()
