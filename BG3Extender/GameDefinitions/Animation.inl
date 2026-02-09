#pragma once

#include <GameDefinitions/Animation.h>
#include <GameDefinitions/Symbols.h>
#include <Lua/LuaHelpers.h>

BEGIN_NS(gn)

using namespace bg3se::lua;


GenomeVariant::GenomeVariant()
{}

GenomeVariant::GenomeVariant(GenomeVariant&& o)
{
    Value = o.Value;
    Type = o.Type;
    o.Value = nullptr;
    o.Type = nullptr;
}

GenomeVariant::GenomeVariant(GenomeVariant const& o)
{
    if (o.Type) {
        o.Type->Init(this, o);
    }
}

GenomeVariant::~GenomeVariant()
{
    Release();
}

GenomeVariant& GenomeVariant::operator = (GenomeVariant const& o)
{
    if (Type && (!o.Type || Type->TypeName != o.Type->TypeName)) {
        ERR("Tried to change type of GenomeVariant from %s to %s!", Type->TypeName.GetString(), (o.Type ? o.Type->TypeName.GetString() : "(Untyped)"));
    } else {
        if (o.Type) {
            o.Type->Init(this, o);
        } else {
            Release();
        }
    }

    return *this;
}

GenomeVariant& GenomeVariant::operator = (GenomeVariant&& o)
{
    if (Type && (!o.Type || Type->TypeName != o.Type->TypeName)) {
        ERR("Tried to change type of GenomeVariant from %s to %s!", Type->TypeName.GetString(), (o.Type ? o.Type->TypeName.GetString() : "(Untyped)"));
    } else {
        Release();
        Value = o.Value;
        Type = o.Type;
        o.Value = nullptr;
        o.Type = nullptr;
    }

    return *this;
}

bool GenomeVariant::operator == (GenomeVariant const& o) const
{
    if (Type && Type == o.Type) {
        if (Type->Size <= sizeof(Value)) {
            return Value == o.Value;
        } else {
            return memcmp(Value, o.Value, Type->Size);
        }
    } else {
        return Type == o.Type;
    }
}

GenomeVariant::GenomeVariant(lua_State* L, int index)
{
    LuaSetValue(L, index);
}

void GenomeVariant::Release()
{
    if (Type) {
        if (Type->Destroy) {
            Type->Destroy(&Value);
        }
        if (Type->Size > sizeof(Value)) {
            GameFree(Value);
        }
    }
}

FixedString GenomeVariant::GetTypeName() const
{
    return Type ? Type->TypeName : FixedString{};
}

UserReturn GenomeVariant::LuaGetValue(lua_State* L) const
{
    if (Type == nullptr) {
        push(L, nullptr);
        return 1;
    }

    if (Type->TypeName == GFS.strBool) {
        push(L, GetValue<bool>());
    } else if (Type->TypeName == GFS.strFloat) {
        push(L, GetValue<float>());
    } else if (Type->TypeName == GFS.strInt 
        || Type->TypeName == GFS.strEnum) {
        push(L, GetValue<int32_t>());
    } else if (Type->TypeName == GFS.strGuid) {
        push(L, GetValue<Guid>());
    } else if (Type->TypeName == GFS.strFixedString 
        || Type->TypeName == GFS.strAnimationSimpleName
        || Type->TypeName == GFS.strAnimationSetID
        || Type->TypeName == GFS.strAnimationComplexName) {
        push(L, GetValue<FixedString>());
    } else if (Type->TypeName == GFS.strString 
        || Type->TypeName == GFS.strPath) {
        push(L, GetValue<STDString>());
    } else if (Type->TypeName == GFS.strVector3) {
        push(L, GetValue<glm::vec3>());
    } else if (Type->TypeName == GFS.strRotator3) {
        push(L, GetValue<glm::quat>());
    } else if (Type->TypeName == GFS.strFloat3x3) {
        push(L, GetValue<glm::mat3>());
    } else if (Type->TypeName == GFS.strFloat4x4) {
        push(L, GetValue<glm::mat4>());
    } else if (Type->TypeName == GFS.strFloatSet) {
        Serialize(L, &GetValue<TGenomeSet<float>>());
    } else if (Type->TypeName == GFS.strIntSet) {
        Serialize(L, &GetValue<TGenomeSet<int32_t>>());
    } else if (Type->TypeName == GFS.strShortNameSet) {
        Serialize(L, &GetValue<TGenomeSet<FixedString>>());
    } else if (Type->TypeName == GFS.strStringSet) {
        Serialize(L, &GetValue<TGenomeSet<STDString>>());
    } else if (Type->TypeName == GFS.strFixedStringSet) {
        Serialize(L, &GetValue<TGenomeSet<FixedString>>());
    } else {
        // TODO - TimelineData (= ls::GenomeTimelineData*)
        WARN_ONCE("Unsupported Genome variant type: %s", Type->TypeName.GetString());
        push(L, nullptr);
    }

    return 1;
}

void GenomeVariant::LuaSetValue(lua_State* L, int index)
{
    switch (lua_type(L, index)) {
    case LUA_TNIL:
        return LuaSetValue(GFS.strEmpty, L, index);
    case LUA_TBOOLEAN:
        return LuaSetValue(GFS.strBool, L, index);
    case LUA_TNUMBER:
        if (lua_isinteger(L, index)) {
            return LuaSetValue(GFS.strInt, L, index);
        } else {
            return LuaSetValue(GFS.strFloat, L, index);
        }
    case LUA_TSTRING:
        return LuaSetValue(GFS.strFixedString, L, index);
    case LUA_TTABLE:
    {
        auto typeName = checked_getfield<FixedString>(L, "Type", index);
        lua_getfield(L, index, "Value");
        LuaSetValue(typeName, L, -1);
        lua_pop(L, 1);
        break;
    }
    default:
        luaL_error(L, "Unsupported Lua type for Genome variant assignment: %s", lua_typename(L, lua_type(L, index)));
    }
}

void GenomeVariant::LuaSetValue(FixedString const& typeName, lua_State* L, int index)
{
    Release();
    Value = nullptr;
    Type = nullptr;

    if (!typeName) {
        return;
    }

    auto types = (*GetStaticSymbols().ls__gGlobalResourceManager)->GenomeTypeManager;
    auto type = types->VarTypes.try_get_ptr(typeName);
    if (!type) {
        luaL_error(L, "Genome type does not exist: %s", typeName.GetString());
        return;
    }

    LuaSetValue(type, L, index);
}

void GenomeVariant::LuaSetValue(GenomeVarTypeDesc* type, lua_State* L, int index)
{
    if (type->TypeName == GFS.strBool) {
        SetValue<bool>(L, index);
    } else if (type->TypeName == GFS.strFloat) {
        SetValue<float>(L, index);
    } else if (type->TypeName == GFS.strInt
        || type->TypeName == GFS.strEnum) {
        SetValue<int32_t>(L, index);
    } else if (type->TypeName == GFS.strGuid) {
        SetValue<Guid>(L, index);
    } else if (type->TypeName == GFS.strFixedString
        || type->TypeName == GFS.strAnimationSimpleName
        || type->TypeName == GFS.strAnimationSetID
        || type->TypeName == GFS.strAnimationComplexName) {
        SetValue<FixedString>(L, index);
    } else if (type->TypeName == GFS.strString
        || type->TypeName == GFS.strPath) {
        SetValue<STDString>(L, index);
    } else if (type->TypeName == GFS.strVector3) {
        SetValue<glm::vec3>(L, index);
    } else if (type->TypeName == GFS.strRotator3) {
        SetValue<glm::quat>(L, index);
    } else if (type->TypeName == GFS.strFloat3x3) {
        SetValue<glm::mat3>(L, index);
    } else if (type->TypeName == GFS.strFloat4x4) {
        SetValue<glm::mat4>(L, index);
    } else if (type->TypeName == GFS.strFloatSet) {
        SetValue<TGenomeSet<float>>(L, index);
    } else {
        // TODO - unsupported: FloatSet, IntSet, ShortNameSet, StringSet, FixedStringSet, TimelineData
        luaL_error(L, "Assignment not supported for this Genome type: %s", type->TypeName.GetString());
        return;
    }

    Type = type;
}

GenomeEventArgs::~GenomeEventArgs()
{}

GenomeParametrizedEventArgs::~GenomeParametrizedEventArgs()
{}

END_NS()

BEGIN_NS(lua)

gn::GenomeVariant do_get(lua_State* L, int index, Overload<gn::GenomeVariant>)
{
    return gn::GenomeVariant(L, index);
}

template <>
PropertyOperationResult Unserialize(lua_State* L, int index, gn::GenomeVariant* val)
{
    *val = gn::GenomeVariant(L, index);
    return PropertyOperationResult::Success;
}

END_NS()

BEGIN_SE()

void AnimationBlueprintSystem::LuaQueueGameplayEventToGenome(EntityHandle entity, FixedString const& event, Array<gn::GenomeVariant> args)
{
    AnimationReceivedGameplayEvent evt{
        .Entity = entity,
        .Event = event,
    };

    for (unsigned i = 0; i < std::min(args.size(), (uint32_t)std::size(evt.Args.Args)); i++) {
        evt.Args.Args[i] = args[i];
    }

    EnterCriticalSection(&GameplayEventsToGenomeLock);
    GameplayEventsToGenome.push_back(std::move(evt));
    LeaveCriticalSection(&GameplayEventsToGenomeLock);
}

void AnimationBlueprintSystem::LuaQueueGameplayEventFromGenome(EntityHandle entity, FixedString const& event, Array<gn::GenomeVariant> args)
{
    AnimationReceivedGameplayEvent evt{
        .Entity = entity,
        .Event = event,
    };

    for (unsigned i = 0; i < std::min(args.size(), (uint32_t)std::size(evt.Args.Args)); i++) {
        evt.Args.Args[i] = args[i];
    }

    EnterCriticalSection(&GameplayEventsFromGenomeLock);
    GameplayEventsFromGenome.push_back(std::move(evt));
    LeaveCriticalSection(&GameplayEventsFromGenomeLock);
}

END_SE()
