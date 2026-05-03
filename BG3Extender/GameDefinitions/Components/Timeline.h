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
    TLLookAtAutomationConfig LookAt;
    float SteppingBlendTime;
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

    Guid Actor;
    Guid Timeline;
    uint8_t TimelineType;
    uint8_t field_21;
    uint8_t Slot;
    bool IsListener;
    uint8_t IKToggle;
    uint8_t IKToggle2;
    uint8_t HandsIKToggle;
    uint8_t HandsIKToggle2;
};


struct TimelineNode
{
    Guid NodeId;
    Guid ContentNodeIdOverride;
    Guid ContentNodeIdOverride1;
    Guid ContentNodeIdOverride2;
    __int64 field_40;
    uint8_t field_48;
    uint8_t field_49;
    uint8_t field_4A;
    uint8_t field_4B;
    uint8_t field_4C;
    uint8_t field_4D;
    uint8_t field_4E;
    uint8_t field_4F;
};


struct TimelineDataComponent : public BaseComponent
{
    DEFINE_COMPONENT(TimelineData, "eoc::TimelineDataComponent")

    EntityHandle Entity;
    Guid InstanceId;
    Guid InitialTimeline;
    Array<Guid> TimelineResources;
    Transform CurrentTransform;
    int DialogInstanceId;
    int field_64;
    Array<TimelineNode> Nodes;
    uint8_t TimelineType;
    uint8_t PlaybackState;
    uint8_t CreationState;
    uint8_t field_7B;
    int CurrentPlayCount;
    int PlayCount;
    float TimeRemaining;
    float SteppingBlendTime;
    uint8_t field_8C;
    uint8_t field_8D;
    uint8_t field_8E;
    uint8_t field_8F;
    Guid CurrentCameraActor;
    EntityHandle Sound;
    // Editor only
    // Transform PreviewTransform;
};


struct SyncedTimelineActorControlComponent : public BaseComponent
{
    DEFINE_COMPONENT(SyncedTimelineActorControl, "eoc::SyncedTimelineActorControlComponent")

    Guid Actor;
    Guid Timeline;
    uint8_t Slot;
    bool IsListener;
    bool IsAdditional;
    uint8_t field_23;
    int field_24;
};


struct SyncedTimelineControlComponent : public BaseComponent
{
    DEFINE_COMPONENT(SyncedTimelineControl, "eoc::SyncedTimelineControlComponent")

    Guid Timeline;
    Array<Guid> TimelineResources;
    Guid CurrentTimeline;
    uint32_t DialogInstanceId;
    uint8_t TimelineType;
    Guid SceneTrigger;
    Guid InitialTimeline;
    uint32_t CharacterCreationUser;
    uint8_t CreationState;
    [[bg3::hidden]] HashMap<Guid, void*> SceneTriggers; // HashMap<Guid, SceneTrigger>
    HashMap<Guid, Guid> Triggers;
    Array<TimelineNode*> Nodes;
    uint8_t PlaybackState;
    bool SmoothExitRequested;
};


struct TimelineContent;
struct TLBaseComponent;
struct TLVoice;


struct PeanutSlotData
{
    int Index;
    uint8_t Slot;
};


struct TimelinePlayerActor
{
    PlayerId Player;
    EntityHandle Actor;
};


struct TimelineNodeData
{
    Guid field_0;
    Guid field_10;
    Guid field_20;
    uint64_t field_30;
    uint16_t field_38;
    uint32_t field_3C;
};


struct Timeline : public aspk::Timeline
{
    bool field_58;
    bool field_59;
    bool field_5A;
    [[bg3::hidden]] HashMap<uint64_t, void*> ActorOverridePhases; // TimelineActorOverrideCache
    FixedString field_a0;
    Guid Id;
    uint8_t VOState;
    uint8_t InitiatorAnimState;
    Transform InitialTransform;
    Transform CurrentTransform;
    glm::vec3 InitialExtents;
    glm::vec3 Extents;
    bool PositionDirty;
    Guid StartingTimelineResource;
    [[bg3::hidden]] HashMap<Guid, TimelineContent*> CustomContent;
    [[bg3::hidden]] TimelineContent* Content;
    [[bg3::hidden]] TimelineContent* ContentOverride;
    HashSet<Guid> LoadedContent;
    [[bg3::hidden]] void* DialogManager;
    uint8_t ChangeFlags;
    [[bg3::hidden]] Array<TLBaseComponent*> UnprocessedComponentActivations;
    HashMap<Guid, EntityHandle> UnregisteredActors;
    Array<TimelinePlayerActor> JoinedPlayerActors;
    HashMap<Guid, EntityHandle> RegisteredActors;
    HashSet<EntityHandle> RegisteredActorEntities;
    HashMap<Guid, EntityHandle> field_298;
    HashMap<Guid, Guid> ActorToTimeline;
    HashMap<Guid, PeanutSlotData> RegisteredPeanuts;
    HashMap<Guid, PeanutSlotData> InitialPeanuts;
    [[bg3::hidden]] LegacyRefMap<Guid, LegacyRefMap<FixedString, TLBaseComponent*>> RegisteredComponents;
    LegacyRefMap<Guid, EntityHandle> ActorLighting;
    HashMap<Guid, Guid> QuestionHoldCameraActorIdToSceneId;
    HashMap<Guid, Guid> CompanionCameraSceneIdToActorId;
    Guid SceneTriggerId;
    Guid InitiatorId;
    Guid Atmosphere;
    Guid Lighting;
    Guid SceneStageId;
    bool IsPreviewTimeline;
    bool IsGameplayTimeline;
    bool IsSystemicCombatTimeline;
    bool IsAuthoredCombatTimeline;
    bool IsMainMenuTimeline;
    uint32_t SkippingMode;
    bool DeferredRequestOngoing;
    bool NeedsFadeOnPlay;
    bool SoundEventsEnabled;
    bool ArePeanutsVisible;
    bool IsDuringQuestionHoldAutomation;
    bool ShouldForceUpdateCameraBehavior;
    bool AutomaticLightingIdFromAtmosphere;
    bool ShouldUpdateAttachmentTransforms;
    DeltaExposure DeltaExposure;
    bool PausedForInGameControls;
    bool InGameTimelineFreeCamEnabled;
    bool IsHoldingAtZero;
    bool LSDialogStarted;
    bool NestedRemapResetRequested;
    Guid PreviousCamera;
    Guid CurrentCameraActor;
    Guid CurrentCameraActorOverride;
    Guid ActivatedLightingCamera;
    bool ShouldUpdateActiveCustomLighting;
    bool ShouldUpdateCustomLighting;
    bool ShouldUpdateConditionalStaging;
    bool ShouldUpdateAutomatedLighting;
    [[bg3::hidden]] TLVoice *PreviousVoice;
    [[bg3::hidden]] TLVoice *CurrentVoice;
    int32_t DialogInstanceId;
    TimelineNodeData NodeData;
    Array<EntityHandle> VisualStreamerHints;
    ComponentHandle FadeRequest;
    int64_t CurrentPrefetchPhaseIndex;
    [[bg3::hidden]] Array<void*> field_590_Arr_TimelineLocationPrefetch;
    [[bg3::hidden]] Array<void*> field_5A0_Arr_TimelineShotPrefetch;
    uint64_t Random;
    bool ShouldForceUpdateNextJCut;
    bool InitialLimitGroundSnappingHeight;
    bool LimitGroundSnappingHeight;
    [[bg3::hidden]] HashMap<int16_t, void*> PlayerRenderViewData_MHM_i16_PlayerRenderViewData;
    // Editor only
    // ls::GameRenderView *GetRenderViewOverride;
    // FixedString DefaultCameraOverride;
    // Transform PreviewTransform;
    // Transform PreviewBoundSceneWorldTransform;
    // Guid PreviewAtmosphere;
    // float PreviewAtmosphereFadeTime;
    // Guid PreviewLighting;
    // float PreviewLightingFadeTime;
    // bool IsPreviewSoundInitialized;
    // uint64_t PreviewOverridingFlags;
    // float EditorDesiredTime;
    // Guid CurrentLocationTriggerId;
    Guid ActiveTimeline;
    [[bg3::hidden]] HashMap<Guid, PrimitiveSet<aspk::Component*>> CustomComponents;
    HashMap<Guid, aspk::TimelineHeader> CustomHeaders;
    HashSet<EntityHandle> CharacterActors;
    UserId CharacterCreationUser;
    HashMap<EntityHandle, Transform> RootMotions;
    [[bg3::hidden]] void *PhysicsScene;
};


END_SE()

BEGIN_NS(ecl)

struct ClientTimelineActorControlComponent : public BaseComponent
{
    DEFINE_COMPONENT(ClientTimelineActorControl, "ecl::ClientTimelineActorControlComponent")

    [[bg3::legacy(field_0)]] Guid Actor;
    [[bg3::legacy(field_10)]] Guid Timeline;
    [[bg3::legacy(field_20)]] uint8_t Slot;
    [[bg3::legacy(field_21)]] bool IsAdditional;
    [[bg3::legacy(field_22)]] bool IsListener;
};


struct ClientTimelineControlComponent : public BaseComponent
{
    DEFINE_COMPONENT(ClientTimelineControl, "ecl::ClientTimelineControlComponent")

    Guid InstanceId;
    Guid Timeline;
    Array<Guid> TimelineResources;
    int DialogInstanceId;
    uint8_t TimelineType;
    uint8_t CreationState;
    uint8_t field_36;
    uint8_t field_37;
    Transform Transform;
    glm::vec3 Extents;
    int field_6C;
    Array<TimelineNode> Nodes;
    uint8_t PlaybackState;
    uint8_t field_81;
    uint8_t field_82;
    uint8_t field_83;
    int field_84;
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
    [[bg3::legacy(field_98)]] Array<FixedString> Speakers;
    uint32_t VisibilityFlags;
    [[bg3::legacy(field_AC)]] VisualLoadFlags VisualLoadFlags;
    [[bg3::hidden]] void* VertexColorMaskTexture;
    uint8_t LightChannel;
    uint8_t TimelineType;
    std::optional<ArmorSetState> ArmorSetState;
    uint32_t ComponentLayoutFlags;
    bool ActiveDummy;
    bool ArmorVisibilityControlled;
    [[bg3::legacy(field_C2)]] bool WeaponsLoaded;
    [[bg3::legacy(ArmorVisibilityControlled2)]] bool EquipmentVisibilityControlled;
    [[bg3::legacy(field_C4)]] bool IsPlayer;
    bool IsWeaponUnsheathed;
    bool ShouldShowVanityWeapon;
    [[bg3::hidden]] uint64_t _Pad;
#if 0
    //ls::DrawingContext DrawingContext;
    //Array<void*> DebugTexts;
#endif
};


struct TimelineActorDataCreateDestroyRequest
{
    EntityHandle Entity;
    Guid Timeline;
    bool Synced;
    uint8_t field_19;
};


struct TimelineActorJoinedEventRequest
{
    EntityHandle Timeline;
    EntityHandle Actor;
    PlayerId PlayerIndex;
};


struct TimelineActorLeaveRequest
{
    EntityHandle Entity;
    uint8_t TimelineType;
    Guid Actor;
    Guid Timeline;
    bool IsDummy;
    bool IsListening;
};


struct TimelineActorLeftEventRequest
{
    EntityHandle Timeline;
    PlayerId PlayerIndex;
    EntityHandle Actor;
    Guid ActorGuid;
    int DialogInstanceId;
};


struct TimelineBackgroundEntityRequest
{
    Guid Entity;
    Guid Timeline;
};


struct ClientOnlyTimelineDestroyRequest
{
    EntityHandle Entity;
};


struct TimelineDataCreateDestroyRequest
{
    EntityHandle Entity;
    Guid Timeline;
    bool Synced;
    char Create;
};


struct TimelineUnloadActorsRequest
{
    Guid Actor;
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
    Array<TimelineActorDataCreateDestroyRequest> ActorDataCreateDestroyRequests;
    Array<TimelineActorLeaveRequest> ActorLeaveRequests;
    [[bg3::hidden]] Array<void*> Arr_TimelineControlCreateDestroyRequest;
    Array<ClientOnlyTimelineDestroyRequest> ClientOnlyTimelineDestroyRequests;
    Array<TimelineDataCreateDestroyRequest> TimelineDataCreateDestroyRequests;
    HashMap<Guid, int32_t> RequestContentForTimelines;
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
    Array<TimelineUnloadActorsRequest> UnloadActorsRequests;
    [[bg3::hidden]] Array<void*> Arr_UnloadTimelineRequest;
    [[bg3::hidden]] Array<void*> Arr_TimelineTransitionFadeCompletionRequest;
    Array<TimelineActorJoinedEventRequest> ActorJoinedEventRequests;
    Array<TimelineActorLeftEventRequest> ActorLeftEventRequests;
    Array<uint32_t> DialogTimelineRequests;
    [[bg3::hidden]] SynchronizedMPMCQueueBounded<void*> SystemicTimelineCreationRequests;
    [[bg3::hidden]] SynchronizedMPMCQueueBounded<void*> CombatTimelineCreationRequests;
    [[bg3::hidden]] HashMap<Guid, Array<void*>> MHM_Guid_Arr_PreviewNodeRequest;
    [[bg3::hidden]] HashMap<Guid, Array<void*>> NodeRequests_MHM_Guid_Arr_NodeRequest;
    [[bg3::hidden]] HashMap<EntityHandle, void*> MHM_EH_DialogueEffectRequest;
    [[bg3::hidden]] HashMap<EntityHandle, void*> MHM_EH_DialogueEffectRequest2;
    std::array<HashMap<EntityHandle, bool>, 2> FadeRequests;
    std::array<HashSet<EntityHandle>, 2> ChangeVisibilityRequests;
    HashMap<Guid, Timeline*> Timelines;
    HashMap<Guid, EntityHandle> TimelineData;
    HashMap<uint32_t, Guid> DialogIdToTimeline;
    HashSet<uint32_t> DialogStarted;
    HashMap<EntityHandle, bool> MaterialSwapped;
    std::array<HashMap<Guid, bool>, 2> FadeStates;
    HashSet<Guid> CombatTimelines;
    Guid SessionGuid;
    Guid GlobalTimelineGuid;
    HashMap<Guid, Array<TimelineBackgroundEntityRequest>> BackgroundEntityRequests;
    [[bg3::hidden]] HashMap<Guid, Array<void*>> DeferredNodeRequestState_MHM_Guid_Arr_DeferredNodeRequest;
    [[bg3::hidden]] HashMap<Guid, HashMap<Guid, void*>> MHM_Guid_MHM_Guid_DeferredDummyCreation;
    HashMap<Guid, float> SoundSyncs;
    HashMap<Guid, int32_t> SyncedTimelinePlayedNodesChanged;
    [[bg3::hidden]] HashMap<Guid, void*> MHM_Guid_TimelineToUpdate;
    TLSystemConfig SystemConfig;
    Path ConfigPath;
    [[bg3::hidden]] uint8_t TimelineCombatCameraEvaluator[168];
    [[bg3::hidden]] CRITICAL_SECTION RequestContentForTimelinesCS;
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
