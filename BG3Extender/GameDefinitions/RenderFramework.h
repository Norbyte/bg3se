#pragma once

#include <GameDefinitions/Lighting.h>

BEGIN_NS(rf::sb)

struct Binding
{
    uint8_t ShaderStage;
    uint8_t DxVsIndex;
    uint8_t field_2;
    uint8_t field_3;
    uint8_t field_4;
    uint8_t DxPsIndex;
    uint8_t DxCsIndex;
    uint8_t VkBindingIndex;
    uint8_t VkDescriptorSet;
};

struct CoverageParams
{
    glm::vec2 Offset;
    float HeightMin;
    float HeightScale;
};

struct PerView
{
    glm::vec3 SunLightDirection;
    float WindDirection;
    glm::vec3 SunLight;
    float WindDirectionY;
    glm::vec3 SkyLightColor;
    float WindSpeed;
    glm::mat4 InvertedViewMatrix;
    glm::vec4 field_70;
    glm::vec4 field_80;
    glm::vec4 field_90;
    glm::vec4 LinearClearColor;
    glm::vec4 SunDir;
    float PrevSkyDomeEnabled;
    float PrevSkyLightIntensity;
    float PrevSunYaw;
    float SkyDomeEnabled;
    float SkyLightIntensity;
    float SunYaw;
    float LightingInterpolation;
    float SunLocalCoverageScalar;
    float CloudCoverageWindDir[4];
    float ScatteringIntensity;
    float SunScatteringIntensity;
    float CloudShadowFactor;
    float SunScatteringIntensityScale;
    CoverageParams TargetCoverage;
    glm::vec3 PrevSunRotation;
    float CloudHeightMin;
    glm::vec3 TargetCoverageTransform;
    int TargetCoverageStartHeight;
    CoverageParams PrevCoverage;
    glm::vec3 PrevCoverageTransform;
    float PrevCoverageStartHeight;
    CoverageParams TargetSunCoverage;
    glm::vec3 SunCoverageTransform;
    float SunCoverageStartHeight;
    glm::vec3 SkyLightScatteringSunColor;
    float CastMoonLightActive;
    glm::vec3 SunRotation;
    float FoVBasedLodsDistanceMultiplier;
};

struct PerCamera
{
    glm::mat4 ViewMatrix;
    glm::mat4 JitteredProjectionMatrix;
    glm::vec4 field_80;
    glm::vec4 field_90;
    glm::vec4 field_A0;
    glm::vec4 field_B0;
    float NearPlane;
    float FarPlane;
    float field_C8;
    float field_CC;
    glm::vec3 WorldTranslate;
    float field_DC;
};


END_NS()

BEGIN_NS(rf)

struct [[bg3::hidden]] BufferBindingInfo
{
    __int64 field_0;
    __int64 field_8;
};

struct [[bg3::hidden]] TextureBindingInfo
{
    __int64 field_0;
    int field_8;
    char field_C;
    __int64 field_10;
};

struct RenderView;

struct [[bg3::hidden]] StageGroup : public ProtectedGameObject<StageGroup>
{
    void* SceneGraph;
    RenderView* RenderView;
    Array<void*> AppStages; // IAppStage*
    EntityHandle field_20;
};

template <class T, class TSize, unsigned Num>
struct [[bg3::hidden]] RangeContainer
{
    std::array<T, Num> Values;
    T Dummy;
    TSize Start;
    TSize End;
};

struct [[bg3::hidden]] D3D11RendererCommandBufferState : public ProtectedGameObject<D3D11RendererCommandBufferState>
{
    std::array<ComponentHandle, 16> VSSamplerHandles;
    std::array<ComponentHandle, 16> PSSamplerHandles;
    std::array<ComponentHandle, 16> CSSamplerHandles;
    RangeContainer<ID3D11SamplerState*, uint8_t, 16> VSSamplers;
    RangeContainer<ID3D11SamplerState*, uint8_t, 16> PSSamplers;
    RangeContainer<ID3D11SamplerState*, uint8_t, 16> CSSamplers;
    rf::Texture* RasterizerState;
    RangeContainer<ID3D11ShaderResourceView*, uint8_t, 128> VSTexturesSRV;
    RangeContainer<ID3D11ShaderResourceView*, uint8_t, 128> PSTexturesSRV;
    RangeContainer<ID3D11ShaderResourceView*, uint8_t, 128> CSTexturesSRV;
    RangeContainer<ID3D11UnorderedAccessView*, uint8_t, 14> CSUAV;
    RangeContainer<ID3D11UnorderedAccessView*, uint8_t, 14> UAV;
    ComponentHandle BlendState;
    uint32_t SampleMask;
    ComponentHandle DepthState;
    uint32_t DepthFlag;
    uint8_t PrimitiveTopology;
    glm::ivec2 ViewportSize;
    glm::ivec2 ViewportOffset;
    glm::ivec2 ScissorSize;
    glm::ivec2 ScissorOffset;
    ComponentHandle VertexFormat;
    std::array<ID3D11Buffer*, 8> VertexBuffers;
    __int64 field_10F0;
    std::array<uint32_t, 8> VertexBufferOffsets;
    ID3D11Buffer* IndexBuffer;
    int IndexBufferOffset;
    ComponentHandle ShaderHandle;
    void* Shader;
    void* Shader2;
    ID3D11VertexShader* VertexShader;
    ID3D11PixelShader* PixelShader;
    ID3D11ComputeShader* ComputeShader;
    uint64_t field_1158[12];
    void* RenderPass;
    void* Framebuffer;
    void* SubPassIndex;
    uint8_t RenderPassFlag;
};

struct [[bg3::hidden]] CommandDispatcherBuffer : public ProtectedGameObject<CommandDispatcherBuffer>
{
    void* FirstPage;
    void* LastPage;
    uint64_t PageSize;
};

struct [[bg3::hidden]] CommandDispatcher : public ProtectedGameObject<CommandDispatcher>
{
    void* VMT;
    CommandDispatcherBuffer Buffer;
    CommandDispatcherBuffer Buffer2;
};

struct [[bg3::hidden]] D3D11RendererCommandBuffer : public CommandDispatcher
{
    void* Renderer;
    void* GPUDevice;
    ID3D11DeviceContext* DeviceContext;
    void* qword50;
    D3D11RendererCommandBufferState State;
    uint32_t CommandBufferType;
    bool NeedsSubmit;
    bool HasCommands;
    ID3DUserDefinedAnnotation* UserDefinedAnnotation;
    void* DynamicBufferAllocator;
    HashSet<Texture*> field_1240_MHS_pRfTexture;
    STDString DebugName;
    STDString DebugNameSubmit;
};

struct [[bg3::hidden]] RCBRollingBuffer
{
    __int64 field_0;
    __int64 field_8;
    __int64 field_10;
};

struct [[bg3::hidden]] RendererCommandBuffer : public ProtectedGameObject<RendererCommandBuffer>
{
    void* VMT;
    D3D11RendererCommandBuffer API;
    void* RendererBase;
    __int64 field_2008;
    __int64 field_2010;
    std::array<RCBRollingBuffer, 3> field_2018;
    std::array<RCBRollingBuffer, 3> field_2060;
    std::array<RCBRollingBuffer, 3> field_20A8;
};

struct CameraController;

struct Viewport
{
    float field_0;
    float field_4;
    float field_8;
    float field_C;
    int Width;
    int Height;
    int OffsetX;
    int OffsetY;
};

struct RenderViewState : public ProtectedGameObject<RenderViewState>
{
    bool Enabled;
    int FullscreenUIRefCount;
    glm::vec2 Scale;
    glm::vec2 Offset;
    Viewport RenderResolution;
    Viewport UpscaledResolution;
    Viewport DisplayResolution;
    uint8_t OwnerInputPlayerIndex;
    uint8_t field_79;
    uint8_t field_7A;
    uint8_t field_7B;
    int field_7C;
    CameraController* CameraController;
};

struct RenderView : public ProtectedGameObject<RenderView>
{
    [[bg3::hidden]] void* VMT;
    [[bg3::hidden]] __int64 field_8;
    [[bg3::hidden]] __int64 RenderFrame;
    [[bg3::hidden]] StageGroup StageGroup;
    [[bg3::hidden]] void* Worker;
    [[bg3::hidden]] void* Rasterizer; // rf::OcclusionRasterizer*
    [[bg3::hidden]] RendererCommandBuffer RCB;
    bool IsRecording;
    int field_2144;
    RenderViewState ReadState;
    RenderViewState WriteState;
    uint8_t SplitScreenIndex;
    int ViewIndex;
    STDString DebugName;
    int32_t RenderOrder;
    bool DisableShadowFading;
};


END_NS()

BEGIN_SE()

struct AtmosphericFogKey
{
    FixedString field_0;
    int32_t field_4;

    inline constexpr bool operator == (AtmosphericFogKey const& o) const
    {
        return field_0 == o.field_0
            && field_4 == o.field_4;
    }
};

inline constexpr uint64_t Hash(AtmosphericFogKey const& v)
{
    return Hash(v.field_0) | ((uint64_t)v.field_4 << 32);
}

struct AtmosphericFog
{
    AtmosphericFogKey Key;
    Fog Fog;
    int32_t field_A0;
};

struct RenderSettings
{
    int64_t field_0;
    int64_t field_8;
    int64_t field_10;
    int64_t field_18;
    int64_t field_20;
    int64_t field_28;
    int64_t field_30;
    int64_t field_38;
    int64_t field_40;
    int64_t field_48;
    int64_t field_50;
    int64_t field_58;
    int field_60;
};

struct GameRenderView : public rf::RenderView
{
    [[bg3::hidden]] void* InputListenerVMT;
    glm::ivec2 RenderViewSize;
    uint8_t UseSplitscreen_M;
    uint8_t field_2291;
    uint8_t field_2292;
    uint8_t field_2293;
    int field_2294;
    int field_2298;
    float field_229C;
    float field_22A0;
    int field_22A4;
    __int64 field_22A8;
    __int64 field_22B0;
    __int64 field_22B8;
    __int64 field_22C0;
    __int64 field_22C8;
    __int64 field_22D0;
    __int64 field_22D8;
    [[bg3::hidden]] UnknownSignal field_22E0;
    [[bg3::hidden]] UnknownSignal OnAtmosphereChanged;
    [[bg3::hidden]] UnknownSignal OnCameraChanged;
    [[bg3::hidden]] void* CullTrigger; // CullTrigger*
    std::array<rf::sb::PerView, 2> PerView;
    std::array<rf::sb::PerCamera, 2> PerCamera;
    [[bg3::hidden]] std::array<void*, 3> PerViewBuffer1;
    [[bg3::hidden]] std::array<void*, 3> PerViewBuffer2;
    [[bg3::hidden]] std::array<void*, 3> PerCameraBuffer2;
    [[bg3::hidden]] std::array<void*, 3> PerCameraBuffer1;
    Lighting* TargetLighting;
    FixedString TargetLightingTrigger;
    Lighting* PrevLighting;
    FixedString PreviousLightingTrigger;
    Lighting* CurLighting;
    float LightingInterpolationValue;
    float LightingInterpolationScale;
    bool LightingFading;
    resource::Atmosphere* TargetAtmosphere;
    FixedString TargetAtmosphereTrigger;
    resource::Atmosphere* PrevAtmosphere;
    FixedString PreviousAtmosphereTrigger;
    resource::Atmosphere* CurAtmosphere;
    float AtmosphereInterpolationValue;
    float AtmosphereFadeTime;
    bool AtmosphereFading;
    Lighting* RequestTargetLighting;
    FixedString RequestTargetLightingID;
    float RequestTargetLightingFadeTime;
    resource::Atmosphere* RequestTargetAtmosphere;
    FixedString RequestTargetAtmosphereID2;
    float RequestTargetAtmosphereFadeTime;
    HashMap<AtmosphericFogKey, float> FogKeys;
    Array<AtmosphericFog> CurrentFog;
    bool IsInputEnabled;
    bool IsCinematicMode;
    bool IsInCombatCameraMode;
    std::array<EntityHandle, 4> EnvironmentEffects;
    [[bg3::hidden]] rf::Texture* PreviousSkydomeTexture;
    [[bg3::hidden]] rf::Texture* TargetSkydomeTexture;
    glm::vec3 DefaultLocalLightSourceColor;
    float DefaultLocalLightSourceIntensity;
    glm::vec2 ClusteredGridSize;
    glm::vec2 VolumetricGridSize;
    int field_29A8;
    int field_29AC;
    [[bg3::hidden]] void* DefaultExposureBuffer;
    [[bg3::hidden]] rf::BufferBindingInfo PreviousExposureBufferSRV;
    int LightingPhysicalModel;
    [[bg3::hidden]] rf::Texture* Transmittance;
    [[bg3::hidden]] rf::TextureBindingInfo TransmittanceSRV;
    [[bg3::hidden]] rf::Texture* Irradiance;
    [[bg3::hidden]] rf::TextureBindingInfo IrradianceSRV;
    [[bg3::hidden]] rf::Texture* Inscatter;
    [[bg3::hidden]] rf::TextureBindingInfo InscatterSRV;
    glm::vec3 SunLightDirection;
    float SunLightFactor;
    bool IsCastMoonLightActive;
    [[bg3::hidden]] rf::Texture *PrevVolumetricCloudCoverageTexture;
    [[bg3::hidden]] rf::Texture *VolumetricCloudCoverageTexture;
    glm::vec2 CoverageWindDir;
    [[bg3::hidden]] rf::Texture *PrevLocalCoverageTexture;
    [[bg3::hidden]] rf::TextureBindingInfo PrevLocalCoverageTextureSRV;
    [[bg3::hidden]] rf::Texture *TargLocalCoverageTexture;
    [[bg3::hidden]] rf::TextureBindingInfo TargLocalCoverageTextureSRV;
    int field_2AA0;
    int field_2AA4;
    float field_2AA8;
    glm::vec3 LevelTransform;
    [[bg3::hidden]] rf::Texture *MoonAlbedoTexture;
    [[bg3::hidden]] rf::Texture *MoonNormalTexture;
    [[bg3::hidden]] rf::Texture *TearsAlbedoTexture;
    [[bg3::hidden]] rf::Texture *TearsNormalTexture;
    bool LightingIsLocked;
    bool AtmosphereIsLocked;
    bool IsSkyHidden;
    bool LockAtmosphereIdChange;
    bool LockLightingIdChange;
    bool ShouldDiscardPreviousFrames;
    FixedString DiscardAtmosphereID;
    FixedString DiscardLightingParentGUID;
    FixedString DiscardLightingTrigger;
    rf::CameraController *CameraController;
    bool NotifyStateChanged;
    Scene* Scene;
    int ViewMode;
    [[bg3::hidden]] rf::Texture *BlackbodyLut;
    [[bg3::hidden]] rf::TextureBindingInfo BlackbodyLutSRV;
    [[bg3::hidden]] rf::Texture *ColorGradingLut;
    [[bg3::hidden]] rf::TextureBindingInfo ColorGradingLutSRV;
    [[bg3::hidden]] rf::TextureBindingInfo ColorGradingLutUAV;
    RenderSettings RenderSettings;
};

END_SE()
