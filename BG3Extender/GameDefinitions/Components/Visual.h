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

END_NS()
