#pragma once

#include <Extender/Client/IMGUI/Backends.h>
#include <imgui.h>

BEGIN_NS(extui)

inline ImVec2 ToImVec(glm::vec2 const& v)
{
    return ImVec2(v.x, v.y);
}

inline ImVec4 ToImVec(glm::vec4 const& v)
{
    return ImVec4(v.x, v.y, v.z, v.w);
}

inline ImVec2 ToImVec(std::optional<glm::vec2> const& v, ImVec2 defaultVal = ImVec2(0.0f, 0.0f))
{
    return v ? ImVec2(v->x, v->y) : defaultVal;
}

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
    HashMap<FixedString, FontData> fonts_;
    float scale_{ 1.0f };
    float requestedScale_{ 1.0f };

    IMGUIObjectManager* objects_{ nullptr };

    bool reducedFontAtlas_{ false };
    bool initialized_{ false };
    bool enableUI_{ false };
    unsigned frameNo_{ 0 };

    bool LoadFont(FontData& request);
    void OnViewportUpdated();
    void UpdateStyle();
};

END_NS()
