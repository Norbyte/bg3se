#pragma once

BEGIN_SE()

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

END_NS()
