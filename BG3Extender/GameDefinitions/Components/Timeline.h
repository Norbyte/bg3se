#pragma once

BEGIN_SE()

struct LightingDetails : public ProtectedGameObject<LightingDetails>
{
    [[bg3::hidden]] void* VMT;
    uint8_t Type;
    bool CastShadow;
    bool VolumetricShadow;
    bool PreExpose;
    float Radius;
    float Intensity;
    float ScatteringScale;
    glm::vec2 SpotLightAngles;
    glm::vec3 Color;
    glm::vec3 Position;
};


struct MaterialReference
{
    FixedString VisualResourceID;
    bool ForceEnabled;
    int SlotIndex;
    int MaterialApplyFlags;
    // Editor only
    // Guid Key;
};


struct MaterialGroup
{
    bool IsReplacement;
    FixedString MaterialResourceID;
    FixedString Name;
    Array<MaterialReference> References;
};


struct MaterialOverlayGroupItem
{
    Guid ID;
    int32_t MaterialOverlayType;
};

struct SoundEventDetails
{
    Guid ID;
    float Threshold;
    uint8_t SoundObjectIndex;
    FixedString EventName;
    bool Force;
    bool TriggerOnIncreasing;
};


struct RTPCDetails
{
    FixedString EventName;
    std::optional<float> SoundDuration;
    std::optional<float> TargetValue;
    uint8_t SoundObjectIndex;
    float Delay;
};


struct MaterialTargetDetails
{
    FixedString ParameterName;
    Guid GroupId;
    bool IsIncreasing;
    float MaterialDuration;
    float MaterialMaxValue;
    float MaterialMinValue;
    uint8_t MaterialBlend;
    Array<SoundEventDetails> SoundEvents;
    Array<RTPCDetails> RTPCs;
};


struct MaterialConfiguration
{
    int32_t Category;
    int32_t Variation;
    Array<MaterialOverlayGroupItem> OverlayGroups;
    Array<MaterialTargetDetails> MaterialDetails;
};


struct TimelineEmotionalMaterials
{
    HashMap<Guid, Array<MaterialConfiguration>> MaterialConfigurations;
    HashMap<Guid, MaterialGroup> MaterialGroups;
};


struct MaterialGroupLookup
{
    HashMap<Guid, MaterialGroup> MaterialGroups;
};


struct CombatCameraParams
{
    bool IsEnabled;
    float HoldTimeAfterEvent;
    float MaxSequenceTime;
    float ObstructionCheckRadius;
    float MinChangeCameraTime;
    float MinAngleDegreesDifferenceToChangeCamera;
    float ObstructionBufferTime;
};


struct TLLookAtAutomationConfig : public ProtectedGameObject<TLLookAtAutomationConfig>
{
    [[bg3::hidden]] void* VMT;
    FixedString Bone;
    uint8_t TurnMode;
    uint8_t TrackingMode;
    float TurnSpeedMultiplier;
    float TorsoTurnSpeedMultiplier;
    float HeadTurnSpeedMultiplier;
    float Weight;
    float SafeZoneAngle;
    float HeadSafeZoneAngle;
    glm::vec3 Offset;
    bool Reset;
    uint8_t LookAtMode;
    uint8_t LookAtInterpMode;
    bool IsEyeLookAtEnabled;
    uint8_t EyeLookAtBone;
    glm::vec3 EyeLookAtOffset;
    float MinDelayTimeSeconds;
    float MaxDelayTimeSeconds;
};


struct TLSystemConfig : public ProtectedGameObject<TLSystemConfig>
{
    [[bg3::hidden]] void* VMT;
    Path FileName;
    float FoVOffset;
    float MinimumComponentLength;
    float FoVOffsetDeviation;
    float PercentageActivationChance;
    [[bg3::hidden]] uint8_t QuestionHoldAutomationParams[32];
    Guid CombatCamerasFallbackSystemicTimeline;
    CombatCameraParams CombatCamera;
    float SteppingBlendTime;
    TLLookAtAutomationConfig LookAt;
    float field_D8;
    uint32_t field_DC;
};


struct TLAutomatedLight
{
    uint8_t TypeId;
    Guid ID;
    STDString Name;
    FixedString AttachBone;
    bool IsDarkVisionLight;
    LightDesc* LightDesc;
};


struct TLAutomatedLightSetup
{
    uint8_t TypeId;
    Guid ID;
    STDString Name;
    Array<TLAutomatedLight*> Lights;
};


struct TimelineActorDataComponent : public BaseComponent
{
    DEFINE_COMPONENT(TimelineActorData, "eoc::TimelineActorDataComponent")

    Guid field_0;
    Guid field_10;
    uint16_t field_20;
    uint8_t field_22;
    uint8_t field_23;
};


END_SE()

BEGIN_NS(ecl)

struct ClientTimelineActorControlComponent : public BaseComponent
{
    DEFINE_COMPONENT(ClientTimelineActorControl, "ecl::ClientTimelineActorControlComponent")

    Guid field_0;
    Guid field_10;
    uint8_t field_20;
    uint8_t field_21;
    uint8_t field_22;
};


struct TLPreviewDummy : public BaseProxyComponent
{
    DEFINE_COMPONENT(TLPreviewDummy, "ecl::TLPreviewDummy")

    TLPreviewDummy() = delete;
    TLPreviewDummy(TLPreviewDummy const&) = delete;

    [[bg3::hidden]] void* VMT;
    [[bg3::hidden]] void* VMT2;
    ecs::EntityRef Entity;
    [[bg3::hidden]] void* field_20;
    FixedString CurrentLevel;
    uint16_t Flags;
    EntityHandle field_30;
    STDString Name;
    FixedString PhysicsResourceId;
    FixedString VisualResourceId;
    Array<FixedString> SpringCollisionResourceIds;
    Array<FixedString> ClothCollisionResourceIds;
    FixedString SourceTemplateOverride;
    FixedString ShapeShiftSourceTemplateOverride;
    FixedString OriginalCharacterTemplate;
    [[bg3::hidden]] void* field_88;
    EntityHandle Listener;
    Array<FixedString> field_98;
    uint32_t VisibilityFlags;
    uint8_t field_AC;
    uint8_t field_AD;
    [[bg3::hidden]] void* VertexColorMaskTexture;
    uint8_t LightChannel;
    uint8_t TimelineType;
    std::optional<ArmorSetState> ArmorSetState;
    uint32_t ComponentLayoutFlags;
    bool ActiveDummy;
    bool ArmorVisibilityControlled;
    uint8_t field_C2;
    bool ArmorVisibilityControlled2;
    uint8_t field_C4;
    bool IsWeaponUnsheathed;
    bool ShouldShowVanityWeapon;
    [[bg3::hidden]] uint64_t _Pad;
#if 0
    //ls::DrawingContext DrawingContext;
    //Array<void*> DebugTexts;
#endif
};


struct TimelineSystem : public BaseSystem
{
    DEFINE_SYSTEM(ClientTimeline, "ecl::TimelineSystem")

    [[bg3::hidden]] void* GameEventListenerVMT;
    [[bg3::hidden]] void* field_18;
    [[bg3::hidden]] void* GameStateEventListenerVMT;
    [[bg3::hidden]] void* DialogEventListenerAdapterVMT;
    [[bg3::hidden]] UnknownSignal TimelineCreatedSignal;
    [[bg3::hidden]] UnknownSignal AutoContinueSignal;
    [[bg3::hidden]] UnknownSignal TakeActorControlSignal;
    [[bg3::hidden]] UnknownSignal qword78;
    [[bg3::hidden]] UnknownSignal TimelineUnloadedSignal;
    [[bg3::hidden]] UnknownSignal NodeDataChangedFlagSignal;
    [[bg3::hidden]] UnknownSignal qwordC0;
    [[bg3::hidden]] UnknownSignal VOChangedFlagSignal;
    [[bg3::hidden]] UnknownSignal DialogHistoryUpdatedSignal;
    [[bg3::hidden]] UnknownSignal UINodeDataChangedSignal1;
    [[bg3::hidden]] UnknownSignal UINodeUpdatedSignal1;
    [[bg3::hidden]] UnknownSignal UINodeUpdatedSignal2;
    [[bg3::hidden]] UnknownSignal UINodeDataChangedSignal2;
    [[bg3::hidden]] UnknownSignal UIVOChangedSignal;
    [[bg3::hidden]] UnknownSignalSubscriber TimelinePlaybackStateChangeSignal;
    [[bg3::hidden]] UnknownSignalSubscriber TimelineCreationStateChangeSignal;
    [[bg3::hidden]] UnknownSignalSubscriber ClientTimelineControlChangeSignal;
    [[bg3::hidden]] UnknownSignalSubscriber DialogStateChangeSignal;
    [[bg3::hidden]] void* SoundSystem;
    [[bg3::hidden]] void* SoundRoutingSystem;
    [[bg3::hidden]] void* SpeakerManager;
    [[bg3::hidden]] void* ResourceManager;
    [[bg3::hidden]] void* AnimationBlueprintSystem;
    [[bg3::hidden]] void* SoundAttachmentSystem;
    [[bg3::hidden]] void* TransformSystem;
    [[bg3::hidden]] void* VisualSystem;
    [[bg3::hidden]] void* MusicManager;
    [[bg3::hidden]] void* TimelineInstanceManager;
    [[bg3::hidden]] void* NetworkManager;
    [[bg3::hidden]] void* PlayerManager;
    [[bg3::hidden]] void* DialogManager;
    [[bg3::hidden]] void* CameraManager;
    [[bg3::hidden]] void* AspkComponentFactoryCustom;
    [[bg3::hidden]] UnknownFunction SignalConnectionCollection;
    LightingDetails Lighting;
    MaterialGroupLookup MaterialGroups;
    TimelineEmotionalMaterials EmotionalMaterials;
    [[bg3::hidden]] Array<void*> Arr_ActorDataCreateDestroyRequest;
    [[bg3::hidden]] Array<void*> Arr_ActorLeaveRequest;
    [[bg3::hidden]] Array<void*> Arr_TimelineControlCreateDestroyRequest;
    [[bg3::hidden]] Array<void*> Arr_ClientOnlyTimelineDestroyRequest;
    [[bg3::hidden]] Array<void*> Arr_TimelineDataCreateDestroyRequest;
    HashMap<Guid, int32_t> SyncedTimelineControl;
    [[bg3::hidden]] SynchronizedMPMCQueueBounded<void*> TimelineDataCreateDestroyRequestQueue;
    [[bg3::hidden]] Array<void*> Arr_CinematicAnimationRequest;
    [[bg3::hidden]] Array<void*> Arr_AutoContinueRequest;
    [[bg3::hidden]] Array<void*> Arr_AnimationTextKeyRequest;
    [[bg3::hidden]] Array<void*> Arr_CharacterAssignedRequest;
    [[bg3::hidden]] Array<void*> Arr_TimelineVisibilityRequest;
    [[bg3::hidden]] Array<void*> SetClientControlPlaybackState_Arr_PlaybackStateChangeRequest;
    [[bg3::hidden]] Array<void*> SetClientControlCreationState_Arr_CreationStateChangeRequest;
    [[bg3::hidden]] Array<void*> LevelUnloaded_Arr_UnloadRequest;
    [[bg3::hidden]] Array<void*> Arr_ReactivateCameraRequest;
    [[bg3::hidden]] Array<void*> Arr_TakeActorControlRequest;
    [[bg3::hidden]] Array<void*> Arr_FastForwardToCurrentNodeRequest;
    [[bg3::hidden]] Array<void*> Arr_JoinLeaveTransitionRequest;
    [[bg3::hidden]] Array<void*> Arr_RegisterPeanutRequest;
    [[bg3::hidden]] Array<void*> Arr_UnloadActorsRequest;
    [[bg3::hidden]] Array<void*> Arr_UnloadTimelineRequest;
    [[bg3::hidden]] Array<void*> Arr_TimelineTransitionFadeCompletionRequest;
    [[bg3::hidden]] Array<void*> Arr_ActorJoinedEventRequest;
    [[bg3::hidden]] Array<void*> Arr_ActorLeftEventRequest;
    [[bg3::hidden]] Array<void*> DialogTimelineRequested_Arr_u32;
    [[bg3::hidden]] SynchronizedMPMCQueueBounded<void*> field_740;
    [[bg3::hidden]] SynchronizedMPMCQueueBounded<void*> field_840;
    [[bg3::hidden]] HashMap<Guid, Array<void*>> MHM_Guid_Arr_PreviewNodeRequest;
    [[bg3::hidden]] HashMap<Guid, Array<void*>> NodeRequests_MHM_Guid_Arr_NodeRequest;
    [[bg3::hidden]] HashMap<EntityHandle, void*> MHM_EH_DialogueEffectRequest;
    [[bg3::hidden]] HashMap<EntityHandle, void*> MHM_EH_DialogueEffectRequest2;
    std::array<HashMap<EntityHandle, bool>, 2> ActiveView;
    std::array<HashSet<EntityHandle>, 2> ActiveViewPrev;
    [[bg3::hidden]] HashMap<Guid, Timeline*> Timelines;
    HashMap<Guid, EntityHandle> TimelineData;
    HashMap<uint32_t, Guid> DialogIdToTimeline;
    HashSet<uint32_t> DialogStarted;
    HashMap<EntityHandle, bool> VisualSwapped;
    std::array<HashMap<Guid, bool>, 2> MHM_Guid_bool;
    HashSet<Guid> CombatTimelines;
    Guid SessionGuid;
    Guid GlobalTimelineGuid;
    [[bg3::hidden]] HashMap<Guid, Array<void*>> MHM_Guid_Arr_BackgroundEntityRequest;
    [[bg3::hidden]] HashMap<Guid, Array<void*>> DeferredNodeRequestState_MHM_Guid_Arr_DeferredNodeRequest;
    [[bg3::hidden]] HashMap<Guid, HashMap<Guid, void*>> MHM_Guid_MHM_Guid_DeferredDummyCreation;
    HashMap<Guid, float> TimeAdjustments;
    HashMap<Guid, int32_t> SyncedTimelinePlayedNodesChanged;
    [[bg3::hidden]] HashMap<Guid, void*> MHM_Guid_TimelineToUpdate;
    TLSystemConfig SystemConfig;
    Path ConfigPath;
    [[bg3::hidden]] uint8_t TimelineCombatCameraEvaluator[168];
    [[bg3::hidden]] CRITICAL_SECTION SyncedTimelineControlCS;
    bool RequestVisualUpdate;
    bool InGameTimelineControlsVisible;
    bool ClientSessionLoaded;
    bool ClientSessionUnloaded;
    bool LevelUnloaded;
    bool LevelStartingTimelineFinished;
    bool LevelDeactivated;
    // Editor only
    // HashSet<uint64_t> DisconnectTimelineCreatedSignal;
    // bool HideUserReservedCharacters;
    // [[bg3::hidden]] void* TimelineMetadataContainer;
    // HashSet<Guid> MHS2_Guid;
};


END_NS()
