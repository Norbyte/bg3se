#include <stdafx.h>
#include <Extender/ScriptExtender.h>
#include <Extender/Client/VisualHelpers.h>

BEGIN_NS(ecl)

void VisualHelpers::Setup()
{
    gExtender->GetEngineHooks().ls__Visual__UpdateBlendshapeWeightsFromSkeleton
        .SetPostHook(&VisualHelpers::OnVisualUpdateBlendshapeWeightsFromSkeleton, this);
}

bool VisualHelpers::SetWeight(RenderableObject* obj, FixedString const& driver, std::optional<float> weight)
{
    if (obj->BlendShape == nullptr) return false;

    auto const& drivers = static_cast<TransformingVertexModel*>(obj->Model)->BlendShapeModelData->Data->MorphTargetDrivers;
    auto driverIt = drivers.find(driver);
    if (driverIt == drivers.end()) return false;
    auto driverIdx = (uint32_t)(driverIt - drivers.begin());

    auto overrides = weightOverrides_.get_or_add(obj);
    while (driverIdx >= overrides->size()) {
        overrides->push_back({});
    }

    (*overrides)[driverIdx] = weight;
    return true;
}

bool VisualHelpers::ClearWeights(RenderableObject* obj)
{
    if (obj->BlendShape == nullptr) return false;

    weightOverrides_.remove(obj);
    return true;
}

void VisualHelpers::OnVisualUpdateBlendshapeWeightsFromSkeleton(Visual* obj)
{
    if (obj->BlendShapeBase == 0xffff) return;

    for (auto const& child : obj->ObjectDescs) {
        if (child.Renderable->BlendShape != nullptr) {
            auto overrides = weightOverrides_.try_get(child.Renderable);

            if (overrides != nullptr) {
                auto blendShape = child.Renderable->LuaGetBlendShape();

                for (uint32_t i = 0; i < std::min(overrides->size(), blendShape->Weights.size()); i++) {
                    if ((*overrides)[i]) {
                        blendShape->Weights[i] = *(*overrides)[i];
                    }
                }
            }
        }
    }
}

END_NS()
