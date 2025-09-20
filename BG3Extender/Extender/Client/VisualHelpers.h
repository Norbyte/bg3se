#pragma once

#include <GameDefinitions/Render.h>

BEGIN_NS(ecl)

class VisualHelpers
{
public:
    void Setup();
    bool SetWeight(RenderableObject* obj, FixedString const& driver, std::optional<float> weight);
    bool ClearWeights(RenderableObject* obj);
    void OnVisualUpdateBlendshapeWeightsFromSkeleton(Visual* obj);

private:
    HashMap<RenderableObject*, Array<std::optional<float>>> weightOverrides_;
};

END_NS()
