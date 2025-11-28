#pragma once

BEGIN_SE()

struct EffectComponent : public BaseProxyComponent
{
    DEFINE_COMPONENT(Effect, "ls::EffectComponent")

    [[bg3::hidden]] void* VMT;
    [[bg3::hidden]] void* WorkerThreadJob;
    EntityHandle Entity;
    resource::EffectResource* EffectResource;
    aspk::EffectTimeline* Timeline;
    bool Initialized;
    bool OverridingFadeOpacity;
    EffectFlags Flags;
    FixedString EffectName;
    FixedString AnimationName;
    bool UpdateQueued;
    EntityHandle SoundEntity;
    [[bg3::legacy(field_48)]] EntityHandle Parent;
    std::array<float, 2> OverrideFadeCapacity;
    std::array<bool, 2> OverrideFadeShadowEnabled;
    [[bg3::hidden]] void* ConstructionJob;
    [[bg3::readonly]] uint16_t ConstructFlagsAndSalt;
    [[bg3::hidden]] uint64_t _Pad;
};

DEFINE_ONEFRAME_TAG_COMPONENT(ls, EffectCreateOneFrameComponent, EffectCreate);
DEFINE_TAG_COMPONENT(ls, AlwaysUpdateEffectComponent, AlwaysUpdateEffect);

struct GameplayEffectSetTimeFactorRequestsSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(GameplayEffectSetTimeFactorRequests, "ls::GameplayEffectSetTimeFactorRequestsSingletonComponent")

    HashMap<EntityHandle, float> SetTimeFactor;
};

struct VFXSetPlayTimeRequest
{
    uint64_t Phase;
    float Time;
};

struct GameplayVFXSetPlayTimeRequestsSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(GameplayVFXSetPlayTimeRequests, "ls::GameplayVFXSetPlayTimeRequestsSingletonComponent")

    HashMap<EntityHandle, VFXSetPlayTimeRequest> SetPlayTime;
};

struct GameplayVFXInfo
{
    FixedString VFX;
    glm::vec3 Position;
};

struct GameplayVFXSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(GameplayVFX, "ls::GameplayVFXSingletonComponent")

    Array<GameplayVFXInfo> VFX;
};


#pragma pack(push, 4)

struct EffectCreateInvoke {};

struct EffectLoadInvoke {};

struct EffectDestroyInvoke {};

struct EffectReleaseFxInvoke {};

struct EffectStopInvoke {};

struct EffectForceStopInvoke {};

struct EffectRegisterForgetInvoke
{
    float field_0;
    bool field_4;
    bool field_5;
};

struct EffectPlayInvoke
{
    [[bg3::hidden]] Scene* Scene{ nullptr };
};

struct EffectPauseInvoke {};

struct EffectPlayToEndInvoke {};

struct EffectUnregisterForgetInvoke {};

struct EffectRestoreRenderStateInvoke {};

struct EffectSoundActivationInvoke
{
    bool Activate;
};

struct EffectSoundStateInvoke
{
    bool State;
};

struct EffectInputIntInvoke
{
    int32_t InputIndex;
    int32_t Value;
};

struct EffectInputFloatInvoke
{
    int32_t InputIndex;
    float Value;
};

struct EffectInputFloat3Invoke
{
    int32_t InputIndex;
    glm::vec3 Value;
};

struct EffectInputColorInvoke
{
    int32_t InputIndex;
    uint32_t Value;
};

struct EffectTargetVisualInvoke
{
    EntityHandle Visual;
};

struct EffectResetTargetVisualInvoke
{
    EntityHandle Visual;
};

struct EffectUpdateTargetVisualInvoke
{
    EntityHandle Visual;
};

struct EffectInputFixedStringInvoke
{
    int32_t InputIndex;
    FixedString Value;
};

struct EffectInputFastStringInvoke
{
    int32_t InputIndex;
    char const* Value{ nullptr };
    uint32_t Length;
};

struct EffectInputStringInvoke
{
    char const* Value{ nullptr };
    uint32_t Length;
};

struct EffectCallbackNewInvoke
{
    [[bg3::hidden]] void* Callback{ nullptr };
};

struct EffectPhaseInvoke
{
    uint64_t PhaseIndex;
    bool field_8;
};

#pragma pack(pop)

struct EffectInvoke
{
    using InvokeType = std::variant<
        EffectCreateInvoke,
        EffectLoadInvoke,
        EffectDestroyInvoke,
        EffectReleaseFxInvoke,
        EffectStopInvoke,
        EffectForceStopInvoke,
        EffectRegisterForgetInvoke,
        EffectPlayInvoke,
        EffectPauseInvoke,
        EffectPlayToEndInvoke,
        EffectUnregisterForgetInvoke,
        EffectRestoreRenderStateInvoke,
        EffectSoundActivationInvoke,
        EffectSoundStateInvoke,
        EffectInputIntInvoke,
        EffectInputFloatInvoke,
        EffectInputFloat3Invoke,
        EffectInputColorInvoke,
        EffectTargetVisualInvoke,
        EffectResetTargetVisualInvoke,
        EffectUpdateTargetVisualInvoke,
        EffectInputFixedStringInvoke,
        EffectInputFastStringInvoke,
        EffectInputStringInvoke,
        EffectCallbackNewInvoke,
        EffectPhaseInvoke
    >;

    InvokeType Invoke;
    uint32_t Salt;
    EntityHandle Effect;
};

struct MaterialInfo : ProtectedGameObject<MaterialInfo>
{
    [[bg3::hidden]] void* VMT;
    EntityHandle Entity;
    FixedString MaterialID;
    float OverlayOffset;
    OverlayMaterialApplyFlags ApplyFlags;
    ApplyMaterialMapFlags MapFlags;
    MaterialInfoFlags Flags;
};

struct OverlayMaterialRequest : ProtectedGameObject<OverlayMaterialRequest>
{
    MaterialInfo** Material{ nullptr };
    bool* CompletionFlag{ nullptr };
    float FadingParam;
    EntityHandle Entity;
    FixedString MaterialID;
    float OverlayOffset;
    OverlayMaterialApplyFlags ApplyFlags;
    ApplyMaterialMapFlags MapFlags;
    OverlayMaterialRequestFlags Flags;
    [[bg3::hidden]] UnknownFunction Callback;
};

struct DestroyEffectStruct
{
    EntityHandle Effect;
    float field_8;
};

struct EffectSoundObjectInfo
{
    FixedString field_0;
    EntityHandle field_8;
};

struct EffectsManager : public BaseSystem
{
    DEFINE_SYSTEM(Effect, "ls::EffectsManager")

    [[bg3::hidden]] void* VMT_IWorkerThreadListener;
    [[bg3::hidden]] UnknownSignal OverlayMaterialRequestSignal;
    bool CanUpdate;
    [[bg3::readonly]] uint32_t FxCreateThreadId;
    Array<DestroyEffectStruct> DestroyEffectData;
    // FIXME - implement proper Queue grow support!
    [[bg3::hidden]] Queue<EffectInvoke> Invokes;
    [[bg3::hidden]] CRITICAL_SECTION InvokesCS;
    HashMap<EntityHandle, EffectSoundObjectInfo> SoundObjectsOnLevel;
    [[bg3::hidden]] Array<void*> Listeners; // IEffectsManagerListener*
    // Editor only?
    // TrackedCompactSet<Path> UnloadEffects;
    // uint8_t field_E8;
    [[bg3::hidden]] CRITICAL_SECTION RCB_CS;
    [[bg3::hidden]] void* RCB; // rf::RendererCommandBuffer*
    [[bg3::hidden]] std::array<void*, 3> GPUFences;
    [[bg3::hidden]] Array<void*> RCBs; // rf::RendererCommandBuffer*
    [[bg3::hidden]] CRITICAL_SECTION AddOverlayMaterialRequestsCS;
    Array<OverlayMaterialRequest> OverlayMaterialRequests;
    [[bg3::hidden]] CRITICAL_SECTION NonConstructedEffectsCS;
    HashSet<EntityHandle> NonConstructedEffects;
    [[bg3::hidden]] void* RenderBatchesGlobalVB;
    [[bg3::hidden]] void* qword1E0;
    [[bg3::hidden]] void* RibbonGlobalVB;
    [[bg3::hidden]] void* VisualSystem;
    [[bg3::hidden]] void* VisualChangedSystem;
};

END_SE()

BEGIN_NS(ecl::effect)

struct HandlerComponent : public BaseComponent
{
    DEFINE_COMPONENT(ClientEffectHandler, "ecl::effect::HandlerComponent")

    EffectHandler* Handler{ nullptr };
};

struct [[bg3::hidden]] HandlerComponentView
{
    HandlerComponent* EffectHandler;
    uint64_t* MutationData{ nullptr };
    void* field_10{ nullptr };
    EntityHandle Entity;
    uint32_t ComponentIndex{ 0 };
    uint8_t field_24{ 0 };
};

struct [[bg3::hidden]] HandlerSystemHelper
{
    virtual ~HandlerSystemHelper() = 0;
    virtual void InitMultiEffect(HandlerComponentView const& entity, struct ecl::EffectHandlerInitInfo const& info, Scene* scene) const = 0;
    virtual void UpdateMultiEffect(HandlerComponentView const& entity) const = 0;
    virtual void DestroyMultiEffect(HandlerComponentView const& entity) const = 0;
    virtual void HideMultiEffect(HandlerComponentView const& entity) const = 0;
    virtual void UnhideMultiEffect(HandlerComponentView const& entity) const = 0;
};

struct HandlerSystem : public BaseSystem
{
    DEFINE_SYSTEM(ClientEffectHandler, "ecl::effect::HandlerSystem")

    [[bg3::hidden]] HandlerSystemHelper* Helper;
};

END_NS()
