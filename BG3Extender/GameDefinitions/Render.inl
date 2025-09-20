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

BlendShapeWeights* RenderableObject::LuaGetBlendShape() const
{
    if (!BlendShape) return nullptr;

    if (gExtender->GetLibraryManager().IsDX11()) {
        auto blend = reinterpret_cast<BlendShapeObjectDataDX11*>(BlendShape);
        return &blend->Weights;
    } else {
        auto blend = reinterpret_cast<BlendShapeObjectDataVK*>(BlendShape);
        return &blend->Weights;
    }
}

bool RenderableObject::SetBlendShapeWeight(FixedString const& param, std::optional<float> weight)
{
    return gExtender->GetClient().GetVisualHelpers().SetWeight(this, param, weight);
}

bool RenderableObject::ClearBlendShapeWeights(FixedString const& param)
{
    return gExtender->GetClient().GetVisualHelpers().ClearWeights(this);
}

END_SE()

BEGIN_NS(lua)

void LuaPolymorphic<MoveableObject>::MakeRef(lua_State* L, MoveableObject* value, LifetimeHandle lifetime)
{
#define V(type) if ((rtti & type::StaticRTTI) == type::StaticRTTI) { \
            MakeDirectObjectRef(L, static_cast<type*>(value), lifetime); return; }

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

void LuaPolymorphic<BasicModel>::MakeRef(lua_State* L, BasicModel* value, LifetimeHandle lifetime)
{
    auto rtti = value->GetRTTI();
    if ((rtti & ModelProxy::StaticRTTI) == ModelProxy::StaticRTTI) {
        MakeDirectObjectRef(L, static_cast<ModelProxy*>(value), lifetime);
        return;
    }
    
    if ((rtti & Model::StaticRTTI) == Model::StaticRTTI) {
        auto model = static_cast<Model*>(value);
        if (model->IsSkinned() || model->IsBlendShape() || model->IsCloth()) {
            MakeDirectObjectRef(L, static_cast<TransformingVertexModel*>(model), lifetime);
        } else {
            MakeDirectObjectRef(L, model, lifetime);
        }
        return;
    }

    MakeDirectObjectRef(L, value, lifetime);
}

void LuaPolymorphic<Model>::MakeRef(lua_State* L, Model* value, LifetimeHandle lifetime)
{
    LuaPolymorphic<BasicModel>::MakeRef(L, static_cast<BasicModel*>(value), lifetime);
}

END_NS()
