#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_NS(extui)

struct TextureLoadResult
{
    ImTextureID Id;
    uint32_t Width;
    uint32_t Height;
};

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
    virtual std::optional<TextureLoadResult> RegisterTexture(TextureDescriptor* descriptor) = 0;
    virtual void UnregisterTexture(ImTextureID id) = 0;
    virtual bool IsInitialized() = 0;
    virtual void ReloadFonts() = 0;
    virtual glm::ivec2 GetViewportSize() = 0;
};

END_NS()
