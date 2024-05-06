#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_NS(extui)

class RenderingBackend
{
public:
    virtual ~RenderingBackend();
    virtual void EnableHooks() = 0;
    virtual void DisableHooks() = 0;
    virtual void InitializeUI() = 0;
    virtual void DestroyUI() = 0;
    virtual void NewFrame() = 0;
    virtual void FinishFrame() = 0;
    virtual void ClearFrame() = 0;
    virtual std::optional<ImTextureID> RegisterTexture(FixedString id) = 0;
    virtual void UnregisterTexture(ImTextureID id) = 0;
};

END_NS()
