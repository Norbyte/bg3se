#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/RootTemplates.h>

BEGIN_NS(ecl::dummy)

struct AnimationState
{
    FixedString field_0;
    FixedString field_4;
    uint8_t field_8;
    int field_C;
    int qwordC;
    uint8_t field_14;
    uint8_t field_15;
    uint8_t field_16;
};

struct AnimationStateComponent : public BaseComponent
{
    DEFINE_COMPONENT(DummyAnimationState, "ecl::dummy::AnimationStateComponent")

    AnimationState State;
};

struct Emote
{
    TranslatedString field_0;
    FixedString field_10;
    int field_14;
    int field_18;
};

struct EmoteCollection
{
    TranslatedString field_0;
    Array<Emote> Emotes;
};

struct FaceExpression
{
    TranslatedString field_0;
    FixedString field_10;
};

struct FaceExpressionCollection
{
    TranslatedString field_0;
    Array<FaceExpression> FaceExpressions;
};

struct AvailableAnimationsComponent : public BaseComponent
{
    DEFINE_COMPONENT(DummyAvailableAnimations, "ecl::dummy::AvailableAnimationsComponent")

    Array<EmoteCollection> EmoteCollections;
    Array<FaceExpressionCollection> FaceExpressionCollections;
};

struct CharacterVFXInitializationComponent : public BaseComponent
{
    DEFINE_COMPONENT(DummyCharacterVFXInitialization, "ecl::dummy::CharacterVFXInitializationComponent")

    Array<EffectHandlerInitInfo> Effects;
};

struct DummiesCreatedSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(DummiesCreatedSingleton, "ecl::dummy::DummiesCreatedSingletonComponent")

    HashMap<EntityHandle, EntityHandle> Dummies;
};

struct DummyComponent : public BaseComponent
{
    DEFINE_COMPONENT(Dummy, "ecl::dummy::DummyComponent")

    EntityHandle Entity;
};

struct EquipmentVisualsStateComponent : public BaseComponent
{
    DEFINE_COMPONENT(DummyEquipmentVisualsState, "ecl::dummy::EquipmentVisualsStateComponent")

    int field_0;
};

struct FootIKStateComponent : public BaseComponent
{
    DEFINE_COMPONENT(DummyFootIKState, "ecl::dummy::FootIKStateComponent")

    __int64 field_0;
    int field_8;
    uint8_t field_C;
    uint8_t field_D;
    uint8_t field_E;
};

struct HasDummyComponent : public BaseComponent
{
    DEFINE_COMPONENT(HasDummy, "ecl::dummy::HasDummyComponent")

    EntityHandle Entity;
};

struct MultiEffectCreateRequestOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(DummyMultiEffectCreateRequest, "ecl::dummy::MultiEffectCreateRequestOneFrameComponent")

    EffectHandlerInitInfo Effect;
};

struct OriginalTransformComponent : public BaseComponent
{
    DEFINE_COMPONENT(DummyOriginalTransform, "ecl::dummy::OriginalTransformComponent")

    Transform Transform;
};

struct SpellVFXInitializationComponent : public BaseComponent
{
    DEFINE_COMPONENT(DummySpellVFXInitialization, "ecl::dummy::SpellVFXInitializationComponent")

    Array<EffectHandlerInitInfo> Effects;
};

struct StatusVFXInitializationComponent : public BaseComponent
{
    DEFINE_COMPONENT(DummyStatusVFXInitialization, "ecl::dummy::StatusVFXInitializationComponent")

    HashMap<EntityHandle, Array<EffectHandlerInitInfo>> Effects;
};

struct SplatterComponent : public BaseComponent
{
    DEFINE_COMPONENT(DummySplatter, "ecl::dummy::SplatterComponent")

    int field_0;
    int field_4;
    int field_8;
    int field_C;
    uint8_t byte10;
};

struct AttachmentClothData
{
    uint32_t field_0;
    FixedString field_4;
    [[bg3::hidden]] HashMap<FixedString, void*> Cloths; // VertexBuffer*
};

struct StoredClothComponent : public BaseComponent
{
    DEFINE_COMPONENT(DummyStoredCloth, "ecl::dummy::StoredClothComponent")

    [[bg3::hidden]] HashMap<FixedString, void*> Cloths; // VertexBuffer*
    Array<AttachmentClothData> Attachments;
};

struct TransformRequestsSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(DummyTransformRequestsSingleton, "ecl::dummy::TransformRequestsSingletonComponent")

    HashMap<EntityHandle, Transform> Requests;
    HashMap<EntityHandle, Transform> Requests2;
};

struct UnsheathComponent : public BaseComponent
{
    DEFINE_COMPONENT(DummyUnsheath, "ecl::dummy::UnsheathComponent")

    int field_0;
    int field_4;
    int field_8;
    int field_C;
};

struct VFXEntitiesComponent : public BaseComponent
{
    DEFINE_COMPONENT(DummyVFXEntities, "ecl::dummy::VFXEntitiesComponent")

    Array<EntityHandle> field_0;
    HashMap<EntityHandle, HashSet<EntityHandle>> field_10;
};


DEFINE_TAG_COMPONENT(ecl::dummy, IsCopyingFullPoseComponent, DummyIsCopyingFullPose)
DEFINE_TAG_COMPONENT(ecl::dummy, LoadedComponent, DummyLoaded)

END_NS()

BEGIN_NS(photo_mode)

struct DummyAnimationStateComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhotoModeDummyAnimationState, "eoc::photo_mode::DummyAnimationStateComponent")

    FixedString field_0;
    FixedString field_4;
    uint8_t field_8;
    int qwordC;
    int field_10;
    uint8_t word14;
    uint8_t field_15;
    uint8_t field_16;
};

struct CameraOffsetComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhotoModeCameraOffset, "ecl::photo_mode::CameraOffsetComponent")

    glm::vec3 Offset;
    glm::vec3 Offset2;
};

struct CameraSavedTransformComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhotoModeCameraSavedTransform, "ecl::photo_mode::CameraSavedTransformComponent")

    Transform Transform;
    glm::vec3 CameraOffset;
    float CameraTilt;
};

struct CameraTiltComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhotoModeCameraTilt, "ecl::photo_mode::CameraTiltComponent")

    float Tilt;
    float Tilt2;
};

struct CameraTrackingComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhotoModeCameraTracking, "ecl::photo_mode::CameraTrackingComponent")

    EntityHandle field_0;
};

struct DummyAnimationUpdateSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhotoModeDummyAnimationUpdateSingleton, "ecl::photo_mode::DummyAnimationUpdateSingletonComponent")

    HashMap<EntityHandle, ecl::dummy::AnimationState> Updates;
};

struct DummyEquipmentVisualVisibility
{
    bool Vanity;
    bool Helmet;
    bool Weapon;
    bool MusicalInstrument;
};

struct DummyEquipmentSetupOneFrameComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhotoModeDummyEquipmentSetupOneFrame, "ecl::photo_mode::DummyEquipmentSetupOneFrameComponent")

    DummyEquipmentVisualVisibility Visibility;
};

struct DummyEquipmentVisualUpdateSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhotoModeDummyEquipmentVisualUpdateSingleton, "ecl::photo_mode::DummyEquipmentVisualUpdateSingletonComponent")

    HashMap<EntityHandle, DummyEquipmentVisualVisibility> Updates;
};

struct DummySplatterUpdateSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhotoModeDummySplatterUpdateSingleton, "ecl::photo_mode::DummySplatterUpdateSingletonComponent")

    HashMap<EntityHandle, bool> Updates;
};

struct DummyTransformUpdateSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhotoModeDummyTransformUpdateSingleton, "ecl::photo_mode::DummyTransformUpdateSingletonComponent")

    HashMap<EntityHandle, Transform> Updates;
};

struct InvisibilityRequest
{
    EntityHandle Entity;
    uint8_t field_8;
    uint8_t field_9;
};

struct InvisibilityRequestSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhotoModeInvisibilityRequestSingleton, "ecl::photo_mode::InvisibilityRequestSingletonComponent")

    Array<InvisibilityRequest> Requests;
};

struct RequestedSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhotoModeRequestedSingleton, "ecl::photo_mode::RequestedSingletonComponent")

    bool Requested;
};

struct SessionComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhotoModeSession, "eoc::photo_mode::SessionComponent")

    uint8_t State;
};

struct DummyComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhotoModeDummy, "eoc::photo_mode::DummyComponent")

    EntityHandle Entity;
    UserId UserId;
};

struct DummyTransformComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhotoModeDummyTransform, "eoc::photo_mode::DummyTransformComponent")

    Transform Transform;
};

struct DummyShowSplatterComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhotoModeDummyShowSplatter, "eoc::photo_mode::DummyShowSplatterComponent")

    uint8_t field_0;
};

struct DummyEquipmentVisualComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhotoModeDummyEquipmentVisual, "eoc::photo_mode::DummyEquipmentVisualComponent")

    int field_0;
};

struct CameraTransformComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhotoModeCameraTransform, "eoc::photo_mode::CameraTransformComponent")

    Transform Transform;
};

END_NS()

BEGIN_NS(esv::photo_mode)

struct CapabilityComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhotoModeCapability, "esv::photo_mode::CapabilityComponent")

    uint8_t field_0;
};

END_NS()
