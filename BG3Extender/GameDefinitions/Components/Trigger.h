#pragma once

BEGIN_SE()

struct SoundStateSync
{
    FixedString Group;
    FixedString State;
    int field_8;
};

struct SoundRTPCSync
{
    FixedString Name;
    float Value;
    int field_8;
};

struct ResourceSetTriggerData : public ITriggerData
{
    Array<FixedString> Atmosphere;
    float FadeTime;
};

struct CameraLockTriggerData : public ITriggerData
{
    bool TriggerCameraLockPos;
    uint32_t TriggerGameCameraBehavior;
};

struct EventTriggerData : public ITriggerData
{
    FixedString EnterEvent;
    FixedString LeaveEvent;
};

struct ExplorationTriggerData : public ITriggerData
{
    Guid ExplorationReward;
    int LevelOverride;
};

struct MusicOriginSound
{
    FixedString OriginTheme;
    FixedString OriginOnly;
    bool OriginThemeAddInstrument;
};

struct MusicVolumeEvent
{
    bool TriggerOnEnter;
    bool TriggerOnlyOnce;
    bool IsStinger;
    FixedString MusicEvent;
    FixedString Bansuri;
    FixedString Cello;
    FixedString Tambura;
    FixedString Ud;
    MusicOriginSound OriginSound;
};

struct NavigationCloudTriggerData : public ITriggerData
{
    int GenerateType;
};

struct PointSoundTriggerDummyData : public ITriggerData
{
    FixedString ParentTrigger;
};

struct RegionTriggerData : public ITriggerData
{
    bool RegionCameraLock;
    bool RegionCameraLockPos;
    bool RegionMaskRegionInWorld;
};

struct SpectatorTriggerData : public ITriggerData
{
    float FOV;
    float Speed;
    float DOFFocalDistance;
    float DOFAperature;
    float DOFNearOnly;
    float DOFFarOnly;
    float DOFNearSharpDistance;
    float DOFFarSharpDistance;
    bool UseDOF;
    bool Lerp;
};

struct StartTriggerData : public ITriggerData
{
    uint8_t Player;
    float Angle;
};

struct StatsAreaTriggerData : public ITriggerData
{
    Guid AreaLevelOverrideGuid;
};

struct TeleportTriggerData : public ITriggerData
{
    float Angle;
    bool Zoom;
};

struct CinematicArenaTriggerData : public ITriggerData
{
    HashSet<Guid> Timeline;
    char MainMenuStartEventType;
    char MainMenuResetEventType;
    char MainMenuActivateCameraEventType;
};

struct MusicVolumeTriggerData : public ITriggerData
{
    Array<MusicVolumeEvent> Events;
};

struct PointSoundTriggerData : public ITriggerData
{
    FixedString SoundEvent;
    float AttenuationScale;
    float MixdownDelayTime;
    uint8_t MixdownSpeed;
    int16_t MaxAttenuation;
    int16_t CustomMixRadius;
    bool UseOcclusion;
    Array<glm::vec3> Dummies;
};

struct FloorTriggerData : public ITriggerData
{
    bool IsRoof;
    int Floor;
    FixedString BuildingUUID;
    Array<FixedString> TriggersToHide;
    Array<FixedString> AttachedPortal;
};

struct RoomTriggerData : public ITriggerData
{
    uint8_t RenderMethod;
    Array<FixedString> AttachedPortals;
    bool OneShotTrigger;
    float Border;
};

struct TimelineSceneTriggerData : public ITriggerData
{
    HashSet<Guid> Timelines;
    bool Optional;
    bool IsWorldTimeline;
    bool ShouldPreloadWorldTimelineActors;
    bool TriggerWorldTimelineOnSkillPrepare;
    bool IsUsingCalculatedSceneBounds;
};

struct DefaultSyncedTriggerData : public ITriggerData
{
};

struct AiHintAreaTriggerData : public ITriggerData
{
    bool IsAnchor;
};

struct CrowdCharacterTriggerSpawnTemplate
{
    FixedString CharacterTemplateId;
    float SpawnWeight;
};

struct CrowdCharacterCustomAnimation
{
    FixedString AnimationShortName;
    float Weight;
};

struct CrowdCharacterDynamicAnimationTag
{
    Guid Tag;
    float Weight;
};

struct CrowdCharacterTriggerData : public ITriggerData
{
    bool SpawningEnabled;
    bool ProxiesOnly;
    bool ReversePatrolDirection;
    int16_t MaxCharacters;
    int16_t GroupSizeMin;
    int16_t GroupSizeMax;
    float GroupSpawnTimeMin;
    float GroupSpawnTimeMax;
    float PositionPerturbation;
    float YawPerturbationDegrees;
    float CharacterSpacing;
    float AnimationSelectionTimeMin;
    float AnimationSelectionTimeMax;
    float PatrolWalkingPercentage;
    float DetourChance;
    float DetourMinWaitTime;
    float DetourMaxWaitTime;
    Array<CrowdCharacterTriggerSpawnTemplate> SpawnTemplates;
    Array<FixedString> DetourPatrolSplines;
    Array<FixedString> DispersePatrolSplines;
    Array<FixedString> PatrolSplines;
    Array<FixedString> PointsOfInterest;
    Array<CrowdCharacterCustomAnimation> CustomAnimations;
    Array<CrowdCharacterDynamicAnimationTag> DynamicAnimationTags;
};

struct CrimeAreaTriggerData : public ITriggerData
{
    int16_t CrimeArea;
};

struct CrimeRegionTriggerData : public ITriggerData
{
    FixedString Region;
};

struct ChasmSeederTriggerData : public ITriggerData
{
    int32_t MaxChasmTiles;
    float MaxChasmHeightOffset;
    Array<FixedString> Triggers;
};

struct ChasmRegionTriggerData : public ITriggerData
{
    Array<FixedString> Triggers;
};

struct CampRegionTriggerData : public ITriggerData
{
};

struct CampChestTriggerData : public ITriggerData
{
    uint8_t Index;
};

struct Trigger : public ProtectedGameObject<Trigger>
{
    [[bg3::hidden]] void* VMT;
    [[bg3::hidden]] void* LoggableObjectVMT;
    TriggerTemplate* Template;
    uint16_t Flags;
    uint16_t SyncFlags;
    FixedString TriggerType;
    ecs::EntityRef TriggerEntity;
    FixedString Level;
    FixedString PreviousLevel;
    [[bg3::hidden]] void* Protocol;
    bool HasCustomPoint;
    bool IsGlobal;
    Transform CustomPointTransform;
};

struct AreaTrigger : public Trigger
{
};

struct PointTrigger : public Trigger
{
};

struct FloorTrigger : public AreaTrigger
{
    bool IsRoof;
    int Floor;
    FixedString BuildingUUID;
};

struct TimelineSceneTrigger : public AreaTrigger
{
    HashSet<Guid> DialogueResourceIds;
    bool Optional;
    bool DestroyAfterTimeline;
    bool IsWorldTimeline;
    bool ShouldPreloadWorldTimelineActors;
    bool UseSceneBounds;
    bool TriggerWorldTimelineOnSkillPrepare;
    bool FloodSuccess;
};

struct LightingTrigger : public AreaTrigger
{
    Array<FixedString> LightingResourceIDs;
    FixedString CurrentLightingResourceID;
    float field_84;
};

struct SoundVolumeTrigger : public AreaTrigger
{
    [[bg3::hidden]] __int64 field_70;
    Array<SoundStateSync> States;
    [[bg3::hidden]] __int64 field_88;
    Array<SoundRTPCSync> RTPCs;
    [[bg3::hidden]] __int64 field_A0;
};

struct AtmosphereTrigger : public AreaTrigger
{
    Array<FixedString> AtmosphereResourceIDs;
    FixedString CurrentAtmosphereResourceID;
    float field_84;
};

struct CullTrigger : public AreaTrigger
{
};

struct FadeTrigger : public AreaTrigger
{
};

struct RoomTrigger : public AreaTrigger
{
};

struct PortalTrigger : public AreaTrigger
{
};

struct CameraBlockerTrigger : public AreaTrigger
{
    Array<FixedString> field_70;
    FixedString field_80;
    float field_84;
};

END_SE()

BEGIN_NS(trigger)

struct PhysicsBoundsInfo
{
    float AreaXZ;
    float AreaExp;
    uint64_t field_8;
    uint32_t field_10;
    glm::vec3 BoundsMin;
    glm::vec3 BoundsMax;
    float Area;
    float AreaXZExp;
    glm::vec3 Position;
    glm::vec3 field_40;
    glm::vec3 field_4C;
};

struct NonePhysics
{
};

struct SpherePhysics
{
    float Radius;
};

struct BoxPhysics
{
    glm::vec3 Extents;
};

struct PolyPhysics
{
    float Height;
    Array<glm::vec2> Points;
};

struct AreaComponent : public BaseComponent
{
    DEFINE_COMPONENT(TriggerArea, "ls::trigger::AreaComponent")

    std::variant<NonePhysics, BoxPhysics, PolyPhysics, SpherePhysics> Physics;
    PhysicsBoundsInfo Bounds;
    glm::quat RotationInv;
};

struct ContainerEvent
{
    bool Left;
    Guid Trigger;
    Guid Object;
};

struct TriggerTypeComponent : public BaseComponent
{
    DEFINE_COMPONENT(TriggerType, "eoc::trigger::TypeComponent")

    uint8_t Type;
};

struct ContainerComponent : public BaseComponent
{
    DEFINE_COMPONENT(TriggerContainer, "ls::trigger::ContainerComponent")

    HashSet<Guid> field_0;
    Array<ContainerEvent> Events;
};

struct IsInsideOfComponent : public BaseComponent
{
    DEFINE_COMPONENT(TriggerIsInsideOf, "ls::trigger::IsInsideOfComponent")

    Array<Guid> InsideOf;
};

struct UpdatedContainerComponent : public BaseComponent
{
    DEFINE_COMPONENT(TriggerUpdatedContainer, "ls::trigger::UpdatedContainerComponent")

    Array<EntityHandle> Updates;
};

struct UpdatedPhysicsComponent : public BaseComponent
{
    DEFINE_COMPONENT(TriggerUpdatedPhysics, "ls::trigger::UpdatedPhysicsComponent")

    Array<EntityHandle> Updates;
};

END_NS()

BEGIN_NS(esv)

struct AreaTriggerBase : public AreaTrigger
{
    EntityHandle Entity;
};

struct PointTriggerBase : public PointTrigger
{
    EntityHandle Entity;
};

struct [[bg3::component]] StatsAreaTrigger : public AreaTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerStatsAreaTrigger, "esv::StatsAreaTrigger")

    Guid AreaLevelOverrideGuid;
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] SoundVolumeTrigger : public bg3se::SoundVolumeTrigger
{
    DEFINE_PROXY_COMPONENT(ServerSoundVolumeTrigger, "esv::SoundVolumeTrigger")

    EntityHandle Entity;
    Guid AreaLevelOverrideGuid;
};

struct [[bg3::component]] AIHintAreaTrigger : public AreaTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerAIHintAreaTrigger, "esv::AIHintAreaTrigger")

    bool IsAnchor;
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] CrowdCharacterTrigger : public AreaTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerCrowdCharacterTrigger, "esv::CrowdCharacterTrigger")

    bool ReversePatrolDirection;
    bool SpawningEnabled;
    bool ProxiesOnly;
    int16_t MaxCharacters;
    int16_t GroupSizeMin;
    int16_t GroupSizeMax;
    float GroupSpawnTimeMin;
    float GroupSpawnTimeMax;
    float PositionPerturbation;
    float YawPerturbationDegrees;
    float CharacterSpacing;
    float AnimationSelectionTimeMin;
    float AnimationSelectionTimeMax;
    float PatrolWalkingPercentage;
    float DetourChance;
    float DetourMinWaitTime;
    float DetourMaxWaitTime;
    Array<CrowdCharacterTriggerSpawnTemplate> SpawnTemplates;
    Array<FixedString> DispersePatrolSplines;
    Array<FixedString> DetourPatrolSplines;
    Array<FixedString> PatrolSplines;
    Array<FixedString> PointsOfInterest;
    Array<CrowdCharacterCustomAnimation> CustomAnimations;
    Array<CrowdCharacterDynamicAnimationTag> DynamicAnimationTags;
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] ChasmSeederTrigger : public PointTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerChasmSeederTrigger, "esv::ChasmSeederTrigger")

    int32_t MaxChasmTiles;
    float MaxChasmHeightOffset;
    Array<FixedString> Triggers;
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] TeleportTrigger : public PointTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerTeleportTrigger, "esv::TeleportTrigger")

    float Angle;
    bool Zoom;
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] CampRegionTrigger : public AreaTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerCampRegionTrigger, "esv::CampRegionTrigger")
    
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] EocAreaTrigger : public AreaTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerEocAreaTrigger, "esv::EocAreaTrigger")

    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] StartTrigger : public PointTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerStartTrigger, "esv::StartTrigger")

    uint8_t Player;
    float Angle;
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] AtmosphereTrigger : public bg3se::AtmosphereTrigger
{
    DEFINE_PROXY_COMPONENT(ServerAtmosphereTrigger, "esv::AtmosphereTrigger")

    EntityHandle Entity;
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] CrimeAreaTrigger : public AreaTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerCrimeAreaTrigger, "esv::CrimeAreaTrigger")

    int MinimumTension;
    FixedString field_7C;
    __int64 field_80;
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] CrimeRegionTrigger : public AreaTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerCrimeRegionTrigger, "esv::CrimeRegionTrigger")

    FixedString Region;
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] EventTrigger : public AreaTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerEventTrigger, "esv::EventTrigger")

    FixedString EnterEvent;
    FixedString LeaveEvent;
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] FloorTrigger : bg3se::FloorTrigger
{
    DEFINE_PROXY_COMPONENT(ServerFloorTrigger, "esv::FloorTrigger")

    EntityHandle Entity;
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] PortalTrigger : public bg3se::PortalTrigger
{
    DEFINE_PROXY_COMPONENT(ServerPortalTrigger, "esv::PortalTrigger")

    EntityHandle Entity;
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] RoomTrigger : public bg3se::RoomTrigger
{
    DEFINE_PROXY_COMPONENT(ServerRoomTrigger, "esv::RoomTrigger")

    EntityHandle Entity;
    uint8_t field_78;
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] AvatarContainerTrigger : public PointTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerAvatarContainerTrigger, "esv::AvatarContainerTrigger")

    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] CampChestTrigger : public PointTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerCampChestTrigger, "esv::CampChestTrigger")

    uint8_t Index;
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] ChasmRegionTrigger : public AreaTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerChasmRegionTrigger, "esv::ChasmRegionTrigger")

    Array<FixedString> Triggers;
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] ExplorationTrigger : public AreaTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerExplorationTrigger, "esv::ExplorationTrigger")

    Guid ExplorationReward;
    int LevelOverride;
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] MusicVolumeTrigger : public AreaTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerMusicVolumeTrigger, "esv::MusicVolumeTrigger")

    Array<MusicVolumeEvent> Events;
    [[bg3::hidden]] void* _PAD;
};

struct MusicVolumeTriggerState
{
    uint16_t PeerId;
    uint32_t Index;
};

struct MusicVolumeTriggerStateComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerMusicVolumeTriggerState, "esv::MusicVolumeTriggerStateComponent")

    Array<MusicVolumeTriggerState> Triggered;
};

struct [[bg3::component]] TimelineSceneTrigger : public bg3se::TimelineSceneTrigger
{
    DEFINE_PROXY_COMPONENT(ServerTimelineSceneTrigger, "esv::TimelineSceneTrigger")

    [[bg3::hidden]] void* field_A8;
    EntityHandle Entity;
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] BlockBronzeTimelinePlacementTrigger : public AreaTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerBlockBronzeTimelinePlacementTrigger, "esv::BlockBronzeTimelinePlacementTrigger")

    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] AiGridAreaTrigger : public AreaTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerAiGridAreaTrigger, "esv::AiGridAreaTrigger")

    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] RegionTrigger : public AreaTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerRegionTrigger, "esv::RegionTrigger")

    bool RegionMaskRegionInWorld;
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] LightingTrigger : public bg3se::LightingTrigger
{
    DEFINE_PROXY_COMPONENT(ServerLightingTrigger, "esv::LightingTrigger")

    EntityHandle Entity;
    [[bg3::hidden]] void* _PAD;
};

struct [[bg3::component]] EocPointTrigger : public PointTriggerBase
{
    DEFINE_PROXY_COMPONENT(ServerEocPointTrigger, "esv::EocPointTrigger")

    [[bg3::hidden]] void* _PAD;
};

END_NS()

BEGIN_NS(esv::trigger)

struct EventConfigComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerTriggerEventConfig, "esv::trigger::EventConfigComponent")

    uint8_t Config;
};

struct RegisteredForComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerTriggerRegisteredFor, "esv::trigger::RegisteredForComponent")

    HashSet<Guid> RegisteredFor;
};

struct RegistrationSettingsComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerRegistrationSettings, "esv::trigger::RegistrationSettingsComponent")

    bool Registered;
};

struct UpdatedRegisteredForComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerUpdatedRegisteredFor, "esv::trigger::UpdatedRegisteredForComponent")

    Array<EntityHandle> Updated;
};

struct UpdatedRegisteredForItemsComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerUpdatedRegisteredForItems, "esv::trigger::UpdatedRegisteredForItemsComponent")

    Array<EntityHandle> Updated;
};

struct CachedLeaveEventData
{
    Guid Object;
    FixedString Event;
    uint8_t field_14;
};

struct CachedLeaveEventsComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerTriggerCachedLeaveEvents, "esv::trigger::CachedLeaveEventsComponent")

    Array<CachedLeaveEventData> Updated;
};

DEFINE_TAG_COMPONENT(esv::trigger, LoadedHandledComponent, ServerTriggerLoadedHandled)
DEFINE_TAG_COMPONENT(esv::trigger, TriggerWorldAutoTriggeredComponent, ServerTriggerWorldAutoTriggered)

END_NS()
