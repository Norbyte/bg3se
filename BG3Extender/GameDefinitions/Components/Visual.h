#pragma once

#include <GameDefinitions/CharacterCreation.h>

BEGIN_SE()

struct CustomIconComponent : public BaseComponent
{
    DEFINE_COMPONENT(CustomIcon, "eoc::CustomIconComponent")

    [[bg3::hidden]]
    ScratchBuffer Buffer;
    uint8_t Unknown;
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

    uint8_t field_0;
    float field_4;
    int field_8;
    int field_C;
    glm::vec3 field_10;
    uint8_t field_1C;
    uint8_t field_1D;
    uint8_t field_1E;
    glm::vec3 field_20;
    std::optional<float> field_2C;
    bool field_34;
    FixedString field_38;
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


struct AnimationReceivedGameplayEvent
{
    EntityHandle Entity;
    FixedString Event;
    [[bg3::hidden]] gn::GenomeParametrizedEventArgs Args;
};

struct AnimationReceivedTextKeyEvent
{
    EntityHandle Entity;
    TextKeyEvent* Event;
    [[bg3::hidden]] void* Skeleton;
};

struct EffectComponent : public BaseProxyComponent
{
    DEFINE_COMPONENT(Effect, "ls::EffectComponent")

    [[bg3::hidden]] void* VMT;
    [[bg3::hidden]] void* WorkerThreadJob;
    EntityHandle Entity;
    resource::EffectResource* EffectResource;
    aspk::EffectTimeline* Timeline;
    bool field_28;
    bool PreviewEffect;
    // bool OverridingFadeOpacity;
    EffectFlags Flags;
    FixedString EffectName;
    FixedString AnimationName;
    bool UpdateQueued;
    EntityHandle SoundEntity;
    EntityHandle field_48;
    std::array<float, 2> OverrideFadeCapacity;
    std::array<bool, 2> OverrideFadeShadowEnabled;
    [[bg3::hidden]] void* ConstructionJob;
    [[bg3::readonly]] uint16_t ConstructFlags;
    [[bg3::hidden]] uint64_t _Pad;
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

END_NS()

BEGIN_NS(ecl)


struct EquipmentVisualData
{
    Array<FixedString> VisualTemplates;
    FixedString BoneSheathed;
    FixedString SourceBoneSheathed;
    FixedString field_18;
    uint32_t AttachFlags;
    EntityHandle field_20;
    Array<resource::PresetData::ScalarParameter> ScalarParameters;
    Array<resource::PresetData::Vector3Parameter> Vector3Parameters;
    ecs::EntityRef Item;
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
    uint8_t field_8;
};

END_NS()
