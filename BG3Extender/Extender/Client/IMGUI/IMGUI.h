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
    struct FontData
    {
        FixedString Key;
        STDString Path;
        float SizePixels;
        ImFont* Font{ nullptr };
    };

    IMGUIManager(SDLManager& sdl);
    ~IMGUIManager();

    void EnableHooks();
    void DisableHooks();

    void InitializeUI();
    void DestroyUI();
    void EnableUI(bool enabled);
    void SetObjects(IMGUIObjectManager* objects);
    std::optional<TextureLoadResult> RegisterTexture(FixedString const& textureGuid);
    void UnregisterTexture(ImTextureID id, FixedString const& textureGuid);
    bool LoadFont(FixedString const& name, char const* path, float size);
    FontData* GetFont(FixedString const& name);
    void SetScale(float scale);
    glm::ivec2 GetViewportSize();

    void OnRenderBackendInitialized();
    void Update();

private:
    SDLManager& sdl_;
    std::unique_ptr<RenderingBackend> renderer_;
    MultiHashMap<FixedString, FontData> fonts_;
    float scale_{ 1.0f };
    float requestedScale_{ 1.0f };

    IMGUIObjectManager* objects_{ nullptr };

    bool reducedFontAtlas_{ false };
    bool initialized_{ false };
    bool enableUI_{ false };

    bool LoadFont(FontData& request);
    void OnViewportUpdated();
    void UpdateStyle();
};

END_NS()
