#pragma once

#include <GameDefinitions/Animation.h>
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
    Type = o.Type;
    if (Type) {
        Type->AssignFromRawValue(this, o.Value);
    }
}

GenomeVariant::~GenomeVariant()
{
    Release();
}

GenomeVariant& GenomeVariant::operator = (GenomeVariant const& o)
{
    Release();
    Type = o.Type;
    if (Type) {
        Type->AssignFromRawValue(this, o.Value);
    }
    return *this;
}

GenomeVariant& GenomeVariant::operator = (GenomeVariant&& o)
{
    Release();
    Value = o.Value;
    Type = o.Type;
    o.Value = nullptr;
    o.Type = nullptr;
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

GenomeVariant::GenomeVariant(lua_State* L, FixedString const& typeName, int index, GenomeTypeManager* types)
{
    if (!typeName) {
        return;
    }
    
    auto type = types->VarTypes.try_get_ptr(typeName);
    if (!type) {
        ERR("Genome type does not exist: %s", typeName.GetString());
        return;
    }
    
    if (typeName == GFS.strBool) {
        SetValue<bool>(L, index);
    } else if (Type->TypeName == GFS.strFloat) {
        SetValue<float>(L, index);
    } else if (Type->TypeName == GFS.strInt 
        || Type->TypeName == GFS.strEnum) {
        SetValue<int32_t>(L, index);
    } else if (Type->TypeName == GFS.strGuid) {
        SetValue<Guid>(L, index);
    } else if (Type->TypeName == GFS.strFixedString 
        || Type->TypeName == GFS.strAnimationSimpleName
        || Type->TypeName == GFS.strAnimationSetID
        || Type->TypeName == GFS.strAnimationComplexName) {
        SetValue<FixedString>(L, index);
    } else if (Type->TypeName == GFS.strString 
        || Type->TypeName == GFS.strPath) {
        SetValue<STDString>(L, index);
    } else if (Type->TypeName == GFS.strVector3) {
        SetValue<glm::vec3>(L, index);
    } else if (Type->TypeName == GFS.strRotator3) {
        SetValue<glm::quat>(L, index);
    } else if (Type->TypeName == GFS.strFloat3x3) {
        SetValue<glm::mat3>(L, index);
    } else if (Type->TypeName == GFS.strFloat4x4) {
        SetValue<glm::mat4>(L, index);
    } else if (Type->TypeName == GFS.strFloatSet) {
        SetValue<TGenomeSet<float>>(L, index);
    } else {
        // TODO - unsupported: FloatSet, IntSet, ShortNameSet, StringSet, FixedStringSet, TimelineData
        ERR("Assignment not supported for this Genome type: %s", typeName.GetString());
        return;
    }

    Type = type;
}

void GenomeVariant::Release()
{
    if (Type) {
        Type->Destroy(this);
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
        WARN("Unsupported Genome variant type: %s", Type->TypeName.GetString());
        push(L, nullptr);
    }

    return 1;
}

GenomeEventArgs::~GenomeEventArgs()
{}

GenomeParametrizedEventArgs::~GenomeParametrizedEventArgs()
{}

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
