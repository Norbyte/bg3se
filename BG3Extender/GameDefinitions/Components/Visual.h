#pragma once

#include <GameDefinitions/CharacterCreation.h>

BEGIN_SE()

struct CustomIconComponent : public BaseComponent
{
    DEFINE_COMPONENT(CustomIcon, "eoc::CustomIconComponent")

    ScratchBuffer Icon;
    uint8_t Source;
};

struct IconComponent : public BaseComponent
{
    DEFINE_COMPONENT(Icon, "eoc::IconComponent")

    FixedString Icon;
};

struct ActiveCharacterLightComponent : public BaseComponent
{
    DEFINE_COMPONENT(ActiveCharacterLight, "eoc::light::ActiveCharacterLightComponent")

    FixedString Light;
};

struct VisualComponent : public BaseComponent
{
    DEFINE_COMPONENT(Visual, "ls::VisualComponent")

    Visual* Visual;
    uint8_t field_8;
    uint8_t field_9;
    bool NotClustered;
};

struct GameObjectVisualComponent : public BaseComponent
{
    DEFINE_COMPONENT(GameObjectVisual, "eoc::GameObjectVisualComponent")

    FixedString RootTemplateId;
    uint8_t RootTemplateType;
    FixedString Icon;
    float Scale;
    uint8_t Type;
};

struct AppearanceOverrideComponent : public BaseComponent
{
    DEFINE_COMPONENT(AppearanceOverride, "eoc::object_visual::AppearanceOverrideComponent")

    character_creation::Appearance Visual;
};

struct CharacterCreationTemplateOverrideComponent : public BaseComponent
{
    DEFINE_COMPONENT(CharacterCreationTemplateOverride, "eoc::object_visual::CharacterCreationTemplateOverrideComponent")

    FixedString Template;
};

struct DisplayNameComponent : public BaseComponent
{
    DEFINE_COMPONENT(DisplayName, "eoc::DisplayNameComponent")

    [[bg3::legacy(NameKey)]] TranslatedString Name;
    [[bg3::legacy(UnknownKey)]] TranslatedString Title;
};

struct CustomNameComponent : public BaseComponent
{
    DEFINE_COMPONENT(CustomName, "eoc::CustomNameComponent")

    STDString Name;
};

struct GameplayLightComponent : public BaseComponent
{
    DEFINE_COMPONENT(GameplayLight, "eoc::GameplayLightComponent")

    [[bg3::legacy(field_0)]] bool Active;
    [[bg3::legacy(field_4)]] float Radius;
    [[bg3::legacy(field_8)]] float EdgeSharpening;
    [[bg3::legacy(field_C)]] float SpotlightAngle;
    [[bg3::legacy(field_10)]] glm::vec3 DirectionalDimensions;
    [[bg3::legacy(field_1C)]] uint8_t LightType;
    [[bg3::legacy(field_1D)]] bool IsHalfLit;
    [[bg3::legacy(field_1E)]] bool Sunlight;
    [[bg3::legacy(field_20)]] glm::vec3 AttachAt;
    [[bg3::legacy(field_2C)]] std::optional<float> PointLightVerticalLimit;
    [[bg3::legacy(field_34)]] bool CheckLOS;
    [[bg3::legacy(field_38)]] FixedString LightCookie;
    float field_3C;
    float field_40;
    uint8_t field_44;
};

struct StaticPhysicsComponent : public BaseComponent
{
    DEFINE_COMPONENT(StaticPhysics, "ls::StaticPhysicsComponent")

    // Editor only
    // FixedString field_0;
    // GameObjectTemplate* Template;
    phx::PhysicsObject* Physics;
};

struct PhysicsComponent : public BaseComponent
{
    DEFINE_COMPONENT(Physics, "ls::PhysicsComponent")

    phx::PhysicsObject* Physics;
    uint32_t PhysicsGroup;
    uint32_t CollidesWith;
    uint32_t ExtraFlags;
    uint8_t field_14;
    uint8_t field_15;
    uint8_t field_16;
};

struct CharacterCreationAppearanceComponent : public BaseComponent
{
    DEFINE_COMPONENT(CharacterCreationAppearance, "eoc::character_creation::AppearanceComponent")

    Array<Guid> Visuals;
    Array<character_creation::AppearanceMaterialSetting> Elements;
    Array<float> AdditionalChoices;
    Guid SkinColor;
    Guid EyeColor;
    Guid SecondEyeColor;
    Guid HairColor;
};

struct AnimationBlueprintComponent : public BaseProxyComponent
{
    DEFINE_COMPONENT(AnimationBlueprint, "ls::AnimationBlueprintComponent")

    gn::GenomeBlueprintInstance* Instance;
    resource::AnimationBlueprintResource* Resource;
    gn::GenomeBlueprint* Blueprint;
    [[bg3::hidden]] void* LoadCallback;
    [[bg3::readonly]] int InstanceId;
    [[bg3::readonly]] uint8_t Flags;
    [[bg3::readonly]] uint64_t field_40;
};

struct AnimationWaterfallElement
{
    FixedString Slot;
    FixedString Resource;
    FixedString Type;
};

struct AnimationWaterfallOverride
{
    Guid AnimationTag;
    uint8_t OverrideType;
    Array<AnimationWaterfallElement> Overrides;
};

struct AnimationTag
{
    Guid Tag;
    uint8_t field_10;
};

struct AnimationSetEntry
{
    FixedString Slot;
    FixedString Resource;
    FixedString Type;
    [[bg3::hidden]] AnimationTag* TagsStart;
    [[bg3::hidden]] AnimationTag* TagsEnd;
    bool Dynamic;
};

struct AnimationSetComponent : public BaseComponent
{
    DEFINE_PROXY_COMPONENT(AnimationSet, "ls::AnimationSetComponent")

    MiniCompactSet<AnimationSetEntry> Entries;
    FixedString FallbackSubSet;
    [[bg3::hidden]] void* PAD;
};

DEFINE_TAG_COMPONENT(ls, AnimationUpdateComponent, AnimationUpdate)

struct AnimationWaterfallComponent : public BaseComponent
{
    DEFINE_COMPONENT(AnimationWaterfall, "ls::animation::AnimationWaterfallComponent")

    Array<AnimationWaterfallElement> Waterfall;
    Array<AnimationWaterfallOverride> Overrides;
};

struct DynamicAnimationTagsComponent : public BaseComponent
{
    DEFINE_COMPONENT(DynamicAnimationTags, "ls::animation::DynamicAnimationTagsComponent")

    Array<AnimationTag> Tags;
};

struct TemplateAnimationSetOverrideComponent : public BaseComponent
{
    DEFINE_COMPONENT(TemplateAnimationSetOverride, "ls::animation::TemplateAnimationSetOverrideComponent")

    Array<AnimationWaterfallElement> Overrides;
};

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
    [[bg3::readonly]] uint16_t ConstructFlags;
    [[bg3::hidden]] uint64_t _Pad;
};

struct CullComponent : public BaseComponent
{
    DEFINE_COMPONENT(Cull, "ls::CullComponent")

    uint16_t CullFlags;
};

struct SceneAttachComponent : public BaseComponent
{
    DEFINE_COMPONENT(SceneAttach, "ls::SceneAttachComponent")

    [[bg3::hidden]] Scene* Scene;
};

struct CustomIconsStorageSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(CustomIconsStorage, "eoc::CustomIconsStorageSingletonComponent")

    HashMap<Guid, ScratchBuffer> Icons;
};

struct FogVolumeRequestComponent : public MoveableObject
{
    DEFINE_PROXY_COMPONENT(FogVolumeRequest, "eoc::FogVolumeRequestComponent")

    Guid field_0;
};

struct CombinedLightComponent : public MoveableObject
{
    DEFINE_PROXY_COMPONENT(CombinedLight, "eoc::CombinedLightComponent")

    EntityHandle Entity;
    BYTE TemplateType;
    FixedString Template;
    FixedString InstanceGuid;
    Transform Transform;
};

struct StandardGameObject : public ProtectedGameObject<StandardGameObject>
{
    [[bg3::hidden]] void* VMT;
    [[bg3::hidden]] void* VMT2;
    ecs::EntityRef GameObjectEntity;
    [[bg3::hidden]] void* field_20;
    FixedString Level;
    uint16_t GameObjectFlags;
};

struct ConstructionTile : public StandardGameObject
{
    DEFINE_COMPONENT(ConstructionTile, "ls::ConstructionTile")

    Guid InstanceId;
    EntityHandle Entity;
    EntityHandle field_48;
    ConstructionTileTemplate* Template;
    FixedString Construction;
    FixedString field_5C;
    float Scale;
    uint8_t Flags;
};

struct ConstructionFilling : public StandardGameObject
{
    DEFINE_COMPONENT(ConstructionFilling, "ls::ConstructionFilling")

    struct Guid InstanceId;
    EntityHandle Entity;
    ConstructionFillingTemplate* Template;
    RenderableObject* Renderable;
    RenderableObject* Renderable2;
    FixedString Construction;
    FixedString FadeGroup;
    FixedString Material;
    FixedString Physics;
    FixedString Id;
    float Tiling;
    bool WalkOn;
    bool SeeThrough;
    bool Fadeable;
    bool HierarchyOnlyFade;
};

struct Construction : public ProtectedGameObject<Construction>
{
    DEFINE_COMPONENT(Construction, "ls::Construction")

    [[bg3::hidden]] void* field_0;
    [[bg3::hidden]] UnknownSignal field_8;
    Array<EntityHandle> Tiles;
    Array<EntityHandle> Filling;
    Guid InstanceId;
    ConstructionTemplate* Template;
};


DEFINE_TAG_COMPONENT(ls, IsSeeThroughComponent, IsSeeThrough)

END_SE()

BEGIN_NS(animation)

struct ReceivedEvent
{
    FixedString Event;
    Array<gn::GenomeVariant> Args;
};


struct TextKeyEventValue
{
    FixedString field_0;
    uint8_t field_4;
};


struct TextKeyEventInfo
{
    FixedString TextKey;
    uint8_t field_4;
    std::optional<TextKeyEventValue> field_8;
};


struct TextKeyEventsSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(AnimationTextKeyEventsSingleton, "ls::animation::TextKeyEventsSingletonComponent")

    Array<AnimationReceivedTextKeyEvent> Events;
};

struct GameplayEventsSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(AnimationGameplayEventsSingleton, "ls::animation::GameplayEventsSingletonComponent")

    Array<AnimationReceivedGameplayEvent> Events;
};

struct GameplayEventsOneFrameComponent : public BaseComponent
{
    DEFINE_COMPONENT(AnimationGameplayEvents, "eoc::animation::GameplayEventsOneFrameComponent")

    HashMap<EntityHandle, Array<ReceivedEvent>> Events;
};


struct TextKeyEventsOneFrameComponent : public BaseComponent
{
    DEFINE_COMPONENT(AnimationTextKeyEvents, "eoc::animation::TextKeyEventsOneFrameComponent")

    HashMap<EntityHandle, Array<TextKeyEventInfo>> Events;
};


struct TriggeredEventsOneFrameComponent : public BaseComponent
{
    DEFINE_COMPONENT(AnimationTriggeredEvents, "eoc::animation::TriggeredEventsOneFrameComponent")

    HashMap<EntityHandle, Array<FixedString>> Events;
};

END_NS()


BEGIN_NS(esv)

struct DisplayName
{
    TranslatedString NameKey;
    uint8_t field_10;
    STDString Name;
};

struct DisplayTitle
{
    TranslatedString NameKey;
    uint8_t field_10;
};

struct DisplayNameListComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerDisplayNameList, "esv::DisplayNameListComponent")

    Array<DisplayName> Names;
    [[bg3::legacy(TranslatedStrings)]] Array<DisplayTitle> Titles;
    [[bg3::legacy(Unknown)]] uint8_t HideTitle;
};

struct IconInfo
{
    FixedString Icon;
    uint32_t field_4;
};

struct IconListComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerIconList, "esv::IconListComponent")

    Array<IconInfo> Icons;
};

struct VisualSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerVisual, "esv::VisualSystem")

    [[bg3::hidden]] void* ThothMachine;
    [[bg3::hidden]] void* AnimationBlueprintSystem;
    [[bg3::hidden]] void* ShapeshiftSystem;
    [[bg3::hidden]] void* ResourceManager;
    HashSet<EntityHandle> UpdateVisuals;
    HashSet<EntityHandle> BoostsRemoved;
    [[bg3::hidden]] HashMap<EntityHandle, std::optional<TemplateInfo>> CharacterSetVisualResource;
    HashMap<EntityHandle, FixedString> ItemSetVisualResource;
    // Editor only?
    // [[bg3::hidden]] void* field_110;
    // HashSet<EntityHandle> ScaleChange;
    // HashMap<EntityHandle, FixedString> TemplateIDChange;
    [[bg3::hidden]] void* GlobalTemplateManager;
    [[bg3::hidden]] void* CacheTemplateManager;
    [[bg3::hidden]] void* LevelManager;
    [[bg3::hidden]] UnknownFunction field_1A0;
    [[bg3::hidden]] UnknownFunction field_1E0;
};


END_NS()

BEGIN_NS(ecl)


struct EquipmentVisualData
{
    Array<FixedString> VisualTemplates;
    [[bg3::legacy(BoneSheathed)]] FixedString Bone;
    [[bg3::legacy(SourceBoneSheathed)]] FixedString SourceBone;
    FixedString field_18;
    uint32_t AttachFlags;
    EntityHandle field_20;
    Array<resource::PresetData::ScalarParameter> ScalarParameters;
    Array<resource::PresetData::Vector3Parameter> Vector3Parameters;
    EntityHandle Item;
    uint16_t VisualFlags;
    uint8_t HairType;
    uint8_t Flags_63;
    uint32_t SlotAndFlags;
};


struct EquipmentSubVisualRequest
{
    FixedString VisualTemplate;
    EntityHandle VisualEntity;
    [[bg3::hidden]] void* LoadRequest_M;
    bool Processed;
};


struct EquipmentVisualRequest
{
    Array<EntityHandle> Item;
    Array<EquipmentSubVisualRequest> SubRequests;
    EquipmentVisualData Data;
    EntityHandle field_90;
};

struct EquipmentVisual
{
    EntityHandle Item;
    Array<EntityHandle> SubVisuals;
    EquipmentVisualRequest* VisualRequest;
    std::optional<EquipmentVisualData> VisualData;
    bool field_20;
};

struct EquipmentVisualsComponent : public BaseComponent
{
    DEFINE_COMPONENT(ClientEquipmentVisuals, "ecl::EquipmentVisualsComponent")

    EntityHandle Entity;
    HashMap<ItemSlot, EquipmentVisual> Equipment;
};

struct PaperdollComponent : public BaseComponent
{
    DEFINE_COMPONENT(ClientPaperdoll, "ecl::PaperdollComponent")

    EntityHandle Entity;
    [[bg3::legacy(field_8)]] bool Combat;
};

struct LoadAnimationFromVisualRequestData
{
    FixedString field_0;
    FixedString field_4;
    FixedString field_8;
    bool field_C;
};

struct EquipmentRemoveMaterialRequest
{
    EntityHandle Entity;
    FixedString Material;
    bool field_C;
    bool RemoveOverlayMaterial;
    uint8_t CheckFlags;
};

struct EquipmentUnloadRequest
{
    EntityHandle Parent;
    EntityHandle Visual;
    EntityHandle Item;
    ItemSlot Slot;
};

struct EquipmentVisualsSystem : public BaseSystem
{
    DEFINE_SYSTEM(ClientEquipmentVisuals, "ecl::EquipmentVisualsSystem")

    [[bg3::hidden]] UnknownSignal field_10;
    [[bg3::hidden]] UnknownSignal field_28;
    [[bg3::hidden]] void* EocClient;
    [[bg3::hidden]] void* LevelManager;
    [[bg3::hidden]] void* MaterialManager;
    [[bg3::hidden]] void* LevelInstanceAttachRequestSystem;
    [[bg3::hidden]] void* EquipmentVisualsUnitTestHelper;
    [[bg3::hidden]] UnknownFunction field_68;
    [[bg3::hidden]] UnknownSignalSubscriber field_A8;
    [[bg3::hidden]] UnknownSignalSubscriber field_F8;
    Array<EquipmentRemoveMaterialRequest> RemoveMaterials;
    HashMap<EntityHandle, uint64_t> PendingSlotLoads;
    HashMap<EntityHandle, HashMap<uint16_t, bg3se::inventory::ContainerSlotData>> InventoryEvents;
    Array<EquipmentUnloadRequest> UnloadRequests;
    HashSet<EntityHandle> DyeUpdates;
    [[bg3::readonly]] int StreamingCount;
    [[bg3::readonly]] int UnloadVisualRequestCount;
    [[bg3::hidden]] void* Dispatcher;
    HashMap<EntityHandle, Array<LoadAnimationFromVisualRequestData>> AnimationSetRequest;
    HashMap<EntityHandle, HashSet<FixedString>> RemoveAnimationSetRequest;
    HashSet<EntityHandle> UpdateSplatter;
    HashMap<EntityHandle, FixedString> InitVisualLevel;
    HashMap<EntityHandle, EntityHandle> VFXTargetRequest;
    HashSet<EntityHandle> DestroyVisuals;
};

struct VisualSystem : public BaseSystem
{
    DEFINE_SYSTEM(ClientVisual, "ecl::VisualSystem")

    [[bg3::hidden]] UnknownSignal field_10;
    [[bg3::hidden]] UnknownSignalSubscriber field_28;
    [[bg3::hidden]] UnknownSignalSubscriber field_78;
    [[bg3::hidden]] UnknownSignalSubscriber field_C8;
    [[bg3::hidden]] UnknownSignalSubscriber field_118;
    [[bg3::hidden]] UnknownFunction field_168;
    HashMap<EntityHandle, bool> ReloadVisuals;
    HashMap<EntityHandle, uint32_t> ChangeSplatterState;
    HashMap<EntityHandle, uint32_t> SplatterType;
    [[bg3::hidden]] __int64 field_268[6];
    bool ReloadAllVisuals;
    [[bg3::hidden]] void* LSVisualSystem;
    [[bg3::hidden]] void* CacheTemplateManager;
    [[bg3::hidden]] void* LevelManager;
    [[bg3::hidden]] void* GlobalTemplateManager;
    [[bg3::hidden]] void* ResourceManager;
};


struct MaterialParameterFloat
{
    FixedString Parameter;
    float Value;
    bool Override{ false };
    uint8_t field_9{ 0 };
    bool Preset{ false };
};

struct MaterialParameterVec2
{
    FixedString Parameter;
    glm::vec2 Value;
    bool Override{ false };
    uint8_t field_9{ 0 };
    bool Preset{ false };
};

struct MaterialParameterVec3
{
    FixedString Parameter;
    glm::vec3 Value;
    bool Override{ false };
    uint8_t field_9{ 0 };
    bool Preset{ false };
};

struct MaterialParameterVec4
{
    FixedString Parameter;
    glm::aligned_highp_vec4 Value;
    bool Override{ false };
    uint8_t field_9{ 0 };
    bool Preset{ false };
};

struct MaterialParameterTexture
{
    FixedString Parameter;
    FixedString Value;
    bool Override{ false };
    uint8_t field_9{ 0 };
    bool Preset{ false };
};

struct MaterialParameterPresetSlot
{
    FixedString GroupName;
    FixedString CCPreset;
    uint32_t field_8;
};

struct MaterialParameterPresetsContainer
{
    Array<MaterialParameterFloat> FloatOverrides;
    Array<MaterialParameterVec2> Vec2Overrides;
    Array<MaterialParameterVec3> Vec3Overrides;
    Array<MaterialParameterVec4> Vec4Overrides;
    Array<MaterialParameterTexture> TextureOverrides;
    Array<MaterialParameterTexture> VirtualTextureOverrides;
    FixedString field_60;
    HashMap<FixedString, MaterialParameterPresetSlot> Presets;
};

struct VisualLocatorAttachment
{
    FixedString DisplayName;
    FixedString LocatorName;
};

struct VisualSetSlot
{
    FixedString Slot;
    FixedString Visual;
    FixedString field_8;
};

struct VisualSetSlots
{
    // Editor only
    // FixedString StatsColorPresetResource;
    FixedString BodySetVisual;
    Array<VisualSetSlot> VisualSlots;
    Array<VisualLocatorAttachment> LocatorAttachments;
    MaterialParameterPresetsContainer MaterialParameters;
    LegacyMap<FixedString, MaterialParameterPresetsContainer> Materials;
    HashMap<FixedString, FixedString> MaterialOverrides;
    HashMap<FixedString, FixedString> MaterialRemaps;
    FixedString VisualSet;
    bool ShowEquipmentVisuals;
};


struct CharacterIconRequestComponent : public BaseComponent
{
    DEFINE_COMPONENT(ClientCharacterIconRequest, "ecl::CharacterIconRequestComponent")

    FixedString Visual;
    VisualSetSlots VisualSet;
    Array<FixedString> Equipment;
    FixedString Template;
    std::optional<ArmorSetState> ArmorSetState;
    EntityHandle field_190;
    STDString Trigger;
    int field_1B0;
};

struct CharacterIconResultComponent : public BaseComponent
{
    DEFINE_COMPONENT(ClientCharacterIconResult, "ecl::CharacterIconResultComponent")

    ScratchBuffer Icon;
};

struct CharacterIconRenderSystem : public BaseSystem
{
    DEFINE_SYSTEM(ClientCharacterIconRender, "ecl::CharacterIconRenderSystem")

    [[bg3::hidden]] UnknownFunction qword10;
    HashSet<EntityHandle> IconRequests;
    HashSet<EntityHandle> DeletedIconRequests;
    int RenderState;
    float RenderTimeRemaining;
    uint32_t ProcessingStage;
    [[bg3::hidden]] void* IconRenderer;
    uint32_t PollResult;
    int field_CC;
    int field_D0;
    int field_D4;
    bool IconRendererSetUp;
    uint8_t field_D9;
    uint8_t field_DA;
    uint8_t field_DB;
    uint16_t field_DC;
    uint8_t field_DE;
    ScratchBuffer Icon;
    EntityHandle EntityBeingRendered;
    EntityHandle field_140;
    uint32_t field_148;
    EntityHandle RenderCallback;
    uint32_t SessionCount;
    [[bg3::hidden]] void* EquipmentVisualsSystem;
};

struct TradeModeChangeRequest
{
    uint16_t field_0;
    uint8_t field_2;
};

struct CharacterManager : public BaseSystem
{
    DEFINE_SYSTEM(ClientCharacterManager, "ecl::CharacterManager")

    [[bg3::hidden]] void* GameEventListenerVMT;
    [[bg3::hidden]] void* field_18;
    [[bg3::hidden]] void* GameStateEventListenerVMT;
    [[bg3::hidden]] UnknownSignal field_28;
    [[bg3::hidden]] UnknownSignal field_40;
    [[bg3::hidden]] UnknownSignal field_58;
    [[bg3::hidden]] UnknownSignal field_70;
    [[bg3::hidden]] void* EntityManager;
    [[bg3::hidden]] void* AnimationBlueprintSystem;
    [[bg3::hidden]] void* LSVisualSystem;
    [[bg3::hidden]] void* LEDSystem;
    [[bg3::hidden]] void* FactionContainer;
    [[bg3::hidden]] void* EoCGlobalSwitches;
    [[bg3::hidden]] void* ResourceManager;
    [[bg3::hidden]] void* GameAnalyticsSystem;
    [[bg3::hidden]] void* StatusSystem;
    [[bg3::hidden]] void* TransformSystem;
    [[bg3::hidden]] void* StatsSystem;
    [[bg3::hidden]] void* LevelManager;
    [[bg3::hidden]] void* HUB;
    [[bg3::hidden]] void* CharacterSoundSystem;
    [[bg3::hidden]] void* GameActionManager;
    [[bg3::hidden]] void* PlayerManager;
    [[bg3::hidden]] void* SpellPrototypeManager;
    [[bg3::hidden]] Array<void*> field_110;
    [[bg3::hidden]] Array<void*> field_120;
    HashSet<EntityHandle> RefreshLights;
    HashSet<EntityHandle> UpdatePhysicsGroup;
    HashMap<EntityHandle, HashSet<ItemSlot>> RefreshEquipmentSlot;
    HashSet<EntityHandle> UpdateObscurity;
    HashSet<EntityHandle> ReloadVisuals;
    HashSet<EntityHandle> ReloadPhysics;
    HashMap<EntityHandle, TradeModeChangeRequest> TradingModeChanges;
    HashSet<EntityHandle> ChangedTemplates;
    [[bg3::hidden]] UnknownSignalSubscriber field_2D0;
    Array<EntityHandle> UpdateCombatCamera;
    std::array<HashSet<EntityHandle>, 3> UpdateCharacterHighlights;
    std::array<HashSet<EntityHandle>, 3> UpdateCharacterHighlights2;
    HashSet<EntityHandle> UpdateRepose;
    [[bg3::hidden]] UnknownSignalSubscriber field_480;
    [[bg3::hidden]] UnknownSignalSubscriber field_4D0;
    [[bg3::hidden]] UnknownSignalSubscriber field_520;
    [[bg3::hidden]] UnknownSignalSubscriber field_570;
    [[bg3::hidden]] UnknownSignalSubscriber field_5C0;
    [[bg3::hidden]] UnknownSignalSubscriber field_610;
    [[bg3::hidden]] UnknownSignalSubscriber field_660;
    [[bg3::hidden]] UnknownFunction field_6B0;
    bool PauseState;
    bool RestoreCharacters;
    bool ReloadVisualSets;
    uint8_t ColorBlindMode;
};

END_NS()

BEGIN_NS(ecl::equipment)

struct EquipmentSlotVisualState
{
    EntityHandle Item;
    uint8_t field_8;
    uint8_t field_9;
};

struct VisualsVisibilityStateComponent : public BaseComponent
{
    DEFINE_COMPONENT(ClientEquipmentVisibilityState, "ecl::equipment::VisualsVisibilityStateComponent")

    HashMap<ItemSlot, EquipmentSlotVisualState> Equipment;
    uint8_t field_40;
    uint8_t field_41;
};

struct VisualsDesiredStateComponent : public BaseComponent
{
    DEFINE_COMPONENT(ClientVisualsDesiredState, "ecl::equipment::VisualsDesiredStateComponent")

    HashMap<ItemSlot, EquipmentVisualData> Slots;
};

struct VisualsVisibilityStateSystem : public BaseSystem
{
    DEFINE_SYSTEM(ClientVisualsVisibilityState, "ecl::equipment::VisualsVisibilityStateSystem")

    HashMap<EntityHandle, bool> SetVisualState;
    HashSet<EntityHandle> UnloadVisuals;
    [[bg3::hidden]] void* CacheTemplateManager;
    [[bg3::hidden]] void* LevelManager;
    [[bg3::hidden]] void* GlobalTemplateManager;
    [[bg3::hidden]] void* ResourceManager;
    [[bg3::hidden]] void* VisualsVisibilityStateUnitTestHelper;
};

END_NS()
