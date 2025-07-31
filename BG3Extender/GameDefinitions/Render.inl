#pragma once

#include <GameDefinitions/RootTemplates.h>
#include <Extender/ScriptExtender.h>

BEGIN_SE()

void MoveableObject::LuaSetWorldTranslate(glm::vec3 const& translate)
{
    SetWorldTranslate(translate);
}

void MoveableObject::LuaSetWorldRotate(glm::quat const& rotate)
{
    SetWorldRotate(rotate);
}

void MoveableObject::LuaSetWorldScale(glm::vec3 const& scale)
{
    SetWorldScale(scale);
}

END_SE()

BEGIN_NS(lua)

void LuaPolymorphic<MoveableObject>::MakeRef(lua_State* L, MoveableObject* value, LifetimeHandle lifetime)
{
#define V(type) if ((rtti & type::StaticRTTI) == type::StaticRTTI) \
            MakeDirectObjectRef(L, static_cast<type*>(value), lifetime); return;

    auto rtti = value->GetRTTI();

    // Types must be ordered from more specific to less specific, as we only check the presence
    // of required RTTI flags, but not the absence of all other flags
    V(LightComponent)
    V(Effect)
    V(Visual)
    V(InstancingObject)
    V(InstancingRenderableObject)
    V(Shape)
    V(DecalObject)
    V(AnimatableObject)
    V(RenderableObject)
    V(MoveableObject)

    MakeDirectObjectRef(L, value, lifetime);

#undef V
}

void LuaPolymorphic<RenderableObject>::MakeRef(lua_State* L, RenderableObject* value, LifetimeHandle lifetime)
{
    LuaPolymorphic<MoveableObject>::MakeRef(L, static_cast<MoveableObject*>(value), lifetime);
}

void LuaPolymorphic<AnimatableObject>::MakeRef(lua_State* L, AnimatableObject* value, LifetimeHandle lifetime)
{
    LuaPolymorphic<MoveableObject>::MakeRef(L, static_cast<MoveableObject*>(value), lifetime);
}

void LuaPolymorphic<Visual>::MakeRef(lua_State* L, Visual* value, LifetimeHandle lifetime)
{
    LuaPolymorphic<MoveableObject>::MakeRef(L, static_cast<MoveableObject*>(value), lifetime);
}

END_NS()
