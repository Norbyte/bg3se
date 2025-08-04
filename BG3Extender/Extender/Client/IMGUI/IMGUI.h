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

class IMGUITextureLoader
{
public:
    // Number of frames to wait before releasing rendering resources
    static constexpr unsigned DeleteAfterFrames = 3;

    void BindRenderer(RenderingBackend*);
    void Update();
    std::optional<TextureLoadResult> IncTextureRef(FixedString const& textureGuid);
    bool DecTextureRef(TextureOpaqueHandle id, FixedString const& textureGuid);

private:
    struct TextureRefCount
    {
        TextureDescriptor* Descriptor{ nullptr };
        TextureLoadResult LoadResult;
        uint32_t RefCount{ 0 };
    };
    
    struct TextureUnloadRequest
    {
        TextureOpaqueHandle Id;
        uint32_t WaitForFrames{ 0 };
    };

    RenderingBackend* renderer_{ nullptr };
    HashMap<FixedString, TextureRefCount> refCounts_;
    HashMap<FixedString, TextureUnloadRequest> pendingUnloads_;
};

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

    inline bool WasUIInitialized() const
    {
        return initialized_;
    }

    void EnableHooks();
    void DisableHooks();

    void InitializeUI();
    void DestroyUI();
    void EnableUI(bool enabled);
    void SetObjects(IMGUIObjectManager* objects);
    std::optional<TextureLoadResult> RegisterTexture(FixedString const& textureGuid);
    void UnregisterTexture(TextureOpaqueHandle id, FixedString const& textureGuid);
    std::optional<ImTextureID> BindTexture(TextureOpaqueHandle opaqueHandle);
    bool LoadFont(FixedString const& name, char const* path, float size);
    FontData* GetFont(FixedString const& name);
    void SetScale(float scale);
    void SetUIScaleMultiplier(float scale);
    void SetFontScaleMultiplier(float scale);
    glm::ivec2 GetViewportSize();

    void OnRenderBackendInitialized();
    void Update();

private:
    SDLManager& sdl_;
    std::unique_ptr<RenderingBackend> renderer_;
    HashMap<FixedString, FontData> fonts_;
    float scale_{ 1.0f };
    float uiScaleMultiplier_{ 1.0f };
    float fontScaleMultiplier_{ 1.0f };
    float requestedScale_{ 1.0f };
    float requestedUiScaleMultiplier_{ 1.0f };
    float requestedFontScaleMultiplier_{ 1.0f };
    IMGUITextureLoader textureLoader_;

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
