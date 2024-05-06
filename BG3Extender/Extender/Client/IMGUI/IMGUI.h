#pragma once

#include <Extender/Client/IMGUI/Backends.h>
#include <imgui.h>

BEGIN_NS(extui)

using HandleType = uint64_t;
static constexpr HandleType InvalidHandle = 0xffffffffffffffffull;

class IMGUIObjectManager;

class IMGUIManager
{
public:
    IMGUIManager(SDLManager& sdl);
    ~IMGUIManager();

    void EnableHooks();
    void DisableHooks();

    void InitializeUI();
    void DestroyUI();
    void EnableUI(bool enabled);
    void SetObjects(IMGUIObjectManager* objects);
    std::optional<ImTextureID> RegisterTexture(FixedString id);
    void UnregisterTexture(ImTextureID id);

    void OnRenderBackendInitialized();
    void Update();

private:
    SDLManager& sdl_;
    std::unique_ptr<RenderingBackend> renderer_;

    IMGUIObjectManager* objects_{ nullptr };

    bool initialized_{ false };
    bool enableUI_{ false };
};

END_NS()
