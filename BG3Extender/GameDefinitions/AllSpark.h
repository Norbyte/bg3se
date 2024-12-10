#pragma once

#include <GameDefinitions/AllSparkShared.h>

BEGIN_SE()

struct Timeline;

END_SE()

BEGIN_NS(aspk)

struct Property : public ProtectedGameObject<Property>
{
    struct Parameter
    {
        STDString Name;
        float Value;
    };

    virtual ~Property() = 0;
    virtual void Clone(Property const& other) = 0;
    virtual bool Visit(ObjectVisitor* visitor) = 0;
    virtual PropertyType GetPropertyType() = 0;
    virtual void SetInputValueString(FixedString const&) = 0;
    virtual void SetInputValueVec4(glm::vec4 const&) = 0;
    virtual void SetInputValueVec3(glm::vec3 const&) = 0;
    virtual void SetInputValueVec2(glm::vec2 const&) = 0;
    virtual void SetInputValueFloat(float) = 0;

    FixedString FullName;
    FixedString AttributeName;
    LegacyArray<Parameter*> Parameters;
    FixedString Input;
};

struct Input : public ProtectedGameObject<Input>
{
    virtual ~Input() = 0;
    virtual PropertyType GetInputType() = 0;

    FixedString Name;
    Property* Input;
};

struct BooleanProperty : public Property
{
    bool Value;
};

struct IntegerProperty : public Property
{
    int32_t Value;
};

struct IntegerRangeProperty : public Property
{
    int32_t Min;
    int32_t Max;
};

struct ColorARGBKeyFrameData
{
    float Time;
    [[bg3::hidden]] __int32 field_4;
    [[bg3::hidden]] __int64 field_8;
    glm::fvec4 Color;
};

struct ColorARGBKeyFrameProperty : public Property
{
    LegacyArray<ColorARGBKeyFrameData>* Frames;
};

struct FloatProperty : public Property
{
    float Value;
};

struct FloatRangeProperty : public Property
{
    float Min;
    float Max;
};

struct FloatKeyFrame : public ProtectedGameObject<FloatKeyFrame>
{
    virtual ~FloatKeyFrame() = 0;
    virtual void Clone(FloatKeyFrame const&) = 0;
    virtual bool Visit(ObjectVisitor*) = 0;
};

struct LinearFloatKeyFrame : public FloatKeyFrame
{
    float Time;
    [[bg3::hidden]] __int32 field_c; // Padding
    float Value;
};


struct CubicFloatKeyFrame : public FloatKeyFrame
{
    float Time;
    [[bg3::hidden]] __int32 field_c; // Padding
    float A;
    float B;
    float C;
    float D;
};


struct KeyFrameData : public ProtectedGameObject<KeyFrameData>
{
    virtual ~KeyFrameData() = 0;
    virtual int GetType() = 0;
};

struct LinearFloatKeyFrameData : public KeyFrameData
{
    LegacyArray<LinearFloatKeyFrame> Frames;
};

struct CubicFloatKeyFrameData : public KeyFrameData
{
    LegacyArray<CubicFloatKeyFrame> Frames;
};

struct FloatKeyFrameProperty : public Property
{
    LegacyArray<KeyFrameData*> KeyFrames;
};

struct StringProperty : public Property
{
    STDString Value;
};

struct Vector3Property : public Property
{
    Vector3 Value;
    [[bg3::hidden]] __int32 field_44;
};

// Types: 0, 1, 2, 3 is null
struct FixedFunction : public ProtectedGameObject<FixedFunction>
{
    virtual ~FixedFunction() = 0;
    virtual void unknown1() = 0;
    virtual void unknown2() = 0;
    virtual void unknown3() = 0;
    virtual int GetFunctionType() = 0;
};

struct FixedFunction0 : public FixedFunction
{
    float A;
    float B;
};

struct FixedFunction1 : public FixedFunction
{
    float A;
    float B;
    float C;
};

struct FixedFunction2 : public FixedFunction
{
    float A;
    float B;
    float C;
    float D;
};

struct FixedFunctionProperty : public Property
{
    FixedFunction* Function;
};

struct FixedStringProperty : public Property
{
    FixedString Value;
};

struct BaseProperty : public Property
{
};

struct CustomModules
{
    [[bg3::hidden]] Array<void*> CustomFloatModules;
    [[bg3::hidden]] Array<void*> CustomFloat3Modules;
};

struct Component : public ProtectedGameObject<Component>
{
    virtual ~Component() = 0;
    virtual void Clone(Component const&) = 0;
    virtual bool Visit(ObjectVisitor*) = 0;
    virtual uint32_t GetPriorityIndex() const = 0;
    virtual FixedString const& GetTypeName() const = 0;
    virtual bool SetStayAlive(float) = 0;
    virtual void Update(float) = 0;
    virtual void PostUpdate(float) = 0;
    virtual void Preload(void*, double) = 0;
    virtual void Activate() = 0;
    virtual void Deactivate() = 0;
    virtual void Reset() = 0;
    virtual void Reload() = 0;
    virtual void OnPause(bool) = 0;
    virtual void SetTimelineOwner(Timeline*) = 0;
    virtual void SetPreRollData(Component const&) = 0;
    virtual void SetTargetVisual(uint64_t) = 0;
    virtual void ResetTargetVisual(uint64_t) = 0;
    virtual void UpdateTargetVisual() = 0;
    virtual bool HasResource(Guid const&) = 0;
    virtual void OnPreviewUpdated() = 0;
    virtual void ResetState() = 0;
    virtual bool VisitProperties(ObjectVisitor*) = 0;
    virtual void CreateVisual() = 0;
    virtual void DestroyVisual() = 0;
    virtual void Unknown01() = 0;
    virtual void FillSnapshotValidChannels(Array<int>&) const = 0;
    virtual void UpdateInternal(float) = 0;
    virtual void PostUpdateInternal(float) = 0;
    virtual void Unknown02() = 0;
    virtual void AttachMaterial() = 0;
    virtual void DetachMaterial() = 0;
    virtual void Unknown05() = 0;

    // Lua getter helpers
    FixedString LuaGetTypeName() const;

    //# P_GETTER(TypeName, LuaGetTypeName)

    Guid ID;
    [[bg3::hidden]] Timeline* Timeline;
    uint32_t TrackGroupIndex;
    float StartTime;
    float EndTime;
    bool IsActive;
    bool StayAlive;
};

struct Module
{
    STDString Name;
    Array<FixedString> FullName;
    Array<Property*> Properties;
};

// Note: There are a LOT of different EffectComponents, all seemingly over 0x100 in size
// The polymorphic choice should be made based on vtable[4]'s FixedString return value
struct FxBaseComponent : public Component
{
    float LastUpdateTimeStep;
    float LastUpdate;
    float TimeStepEdge0;
    float TimeStep;
    float TimeStepEdge1;
    ecs::EntityRef Entity;
    LegacyMap<FixedString, Property*> Properties;
    Array<Module*> Modules;
    [[bg3::hidden]] Array<Property*> PropertyList;
    uint32_t FxType;
    [[bg3::hidden]] resource::EffectResource* EffectResource;
};

struct TLActor
{
    [[bg3::hidden]] void* vtable;
    bg3se::Guid UUID;
    bool PeanutOverride;
};

struct TLBaseComponent : public Component
{
    TLActor Actor;
    uint8_t field_68x;
    uint32_t TypeId;
    int64_t PhaseIndex;
    bool IsSnappedToEnd;
    bool IsMimicry;
};

BEGIN_BARE_NS(keys)
    struct KeyBase : public ProtectedGameObject<KeyBase>
    {
        virtual ~KeyBase() = 0;
        virtual void Clone(KeyBase const&) = 0;
        virtual bool Visit(ObjectVisitor*) = 0;

        float Time;
        uint8_t InterpolationType;
    };
END_BARE_NS()

template<typename Key>
struct TLKeyBaseComponent : public TLBaseComponent
{
    float field_68;
    [[bg3::hidden]] Array<Key*> Keys;
    bool HasTargetKey;
    Key* TargetKey;
    bool NeedsKeyUpdate;
    bool field_91;
    std::array<uint32_t, 2> Random;
};

template struct TLKeyBaseComponent<keys::KeyBase>;

template<typename Key>
struct TLInterpolationKeyComponent : public TLKeyBaseComponent<Key>
{
};

template struct TLInterpolationKeyComponent<keys::KeyBase>;

template<typename Key>
struct TLEventKeyComponent : public TLKeyBaseComponent<Key>
{
};

template struct TLEventKeyComponent<keys::KeyBase>;

BEGIN_BARE_NS(keys)
    template <typename T>
    struct ValueKey : public KeyBase
    {
        T Value;
    };

    struct AtmosphereAndLightingChannelKey : public KeyBase
    {
        bg3se::Guid Id;
        float FadeTime;
    };

    struct AttachToEventKey : public KeyBase
    {
        bg3se::Guid Target;
        FixedString Bone;
        bool IsPersistent;
        glm::fvec3 offset;
    };

    struct AttitudeEventKey : public KeyBase
    {
        FixedString Pose;
        FixedString Transition;
    };

    struct CameraDoFChannelKey : public KeyBase
    {
        float Value;
    };

    struct CameraFoVKey : public KeyBase
    {
        float FoV;
    };

    struct CameraLookAtKey : public KeyBase
    {
        bg3se::Guid Target;
        FixedString Bone;
        glm::fvec2 Framing;
        glm::fvec2 FramingOffset;
        glm::fvec2 FreeBounds;
        glm::fvec2 SoftBounds;
        float DampingStrength;
        float FreeZoneDelay;
        float SoftZoneDelay;
        float SoftZoneRampTime;
    };

    struct EffectPhaseEventKey : public KeyBase
    {
        int32_t EffectPhase;
    };

    struct EmotionEventKey : public KeyBase
    {
        int32_t Emotion;
        int32_t Variation;
        bool IsSustainedEmotion;
        bool AppliesMaterials;
    };

    struct FloatRTPCKey : public KeyBase
    {
        float FloatRTPCValue;
    };

    struct GenomeTextEventKey : public KeyBase
    {
        FixedString EventName;
    };

    struct HandsIKKey : public KeyBase
    {
        bool InverseKinematics;
    };

    struct LookAtEventKey : public KeyBase
    {
        bg3se::Guid Target;
        FixedString Bone;
        uint8_t TurnMode;
        uint8_t TrackingMode;
        float TurnSpeedMultiplier;
        float TorsoTurnSpeedMultiplier;
        float HeadTurnSpeedMultiplier;
        std::optional<double> Weight;
        std::optional<double> SaveZoneAngle;
        std::optional<double> HeadSafeZoneAngle;
        glm::fvec3 Offset;
        bool Reset;
        uint8_t LookAtMode;
        uint8_t LookAtInterpMode;
        bool IsEyeLookAtEnabled;
        bg3se::Guid EyeLookAtTargetId;
        FixedString EyeLookAtBone;
        glm::fvec3 EyeLookAtOffset;
    };

    struct MaterialTextureKeyKey : public KeyBase
    {
        bg3se::Guid Value;
    };

    struct PhysicsKey : public KeyBase
    {
        bool InverseKinematics;
    };

    struct PlayEffectEventKey : public KeyBase
    {
        bool PlayEffect;
    };

    struct PlayRateKey : public KeyBase
    {
        float Speed;
    };

    struct ShapeShiftKey : public KeyBase
    {
        bg3se::Guid TemplateId;
    };

    struct ShowArmorChannelKey : public KeyBase
    {
        bool Value;
    };

    struct ShowHUDKey : public KeyBase
    {
        bool ShowHUD;
    };

    struct ShowPeanutsKey : public KeyBase
    {
        bool ShowPeanuts;
    };

    struct ShowVisualKey : public KeyBase
    {
        bool ShowVisual;
    };

    struct ShowWeaponKey : public KeyBase
    {
        bool ShowWeapon;
    };

    struct SoundEventKey : public KeyBase
    {
        struct Type1
        {
            bg3se::Guid SoundEventID;
            uint8_t SoundObjectIndex;
            bool KeepAlive;
            float LoopLifetime;
        };
        struct Type2
        {
            bg3se::Guid SoundEventID;
        };
        struct Type4
        {
            uint8_t FoleyType;
            uint8_t FoleyIntensity;
            uint8_t SoundObjectIndex;
        };
        struct Type5
        {
            uint8_t VocalType;
            uint8_t SoundObjectIndex;
        };
        std::variant<Type1, Type2, Type2, Type2, Type4, Type5> data;
    };

    struct SplatterChannelKey : public KeyBase
    {
        float Value;
        uint8_t SplatterChangeMode;
    };

    struct SpringsKey : public KeyBase
    {
        bool EnableSprings;
    };

    struct SteppingFadeKey : public KeyBase
    {
        bool SteppingInOut;
    };

    struct SwitchLocationEventKey : public KeyBase
    {
        bg3se::Guid SwitchLocationEventID;
        bg3se::Guid s_LevelTemplateID;
    };

    struct SwitchStageEventKey : public KeyBase
    {
        bg3se::Guid SwitchStageEventID;
        bool ForceTransformUpdate;
        bool ForceUpdateCameraBehavior;
    };

    struct TransformChannelFloatKey : public KeyBase
    {
        float Value;
    };

    struct TransformChannelFrameOfReferenceKey : public KeyBase
    {
        struct FrameOfReference
        {
            bg3se::Guid targetId;
            FixedString targetBone;
            bool OneFrameOnly;
            bool KeepScale;
        };
        FrameOfReference frameOfReference;
    };

    struct TransformChannelQuatKey : public KeyBase
    {
        glm::fvec4 Value;
    };
END_BARE_NS()

BEGIN_BARE_NS(channels)
    template <typename T>
    struct EventKeyChannel : public TLEventKeyComponent<keys::ValueKey<T>>
    {
        T defaultVal;
    };
            
    struct TimelineActorPropertiesReflectionKeyComponent : public TLInterpolationKeyComponent<keys::ValueKey<float>>
    {
        float KeyDefault;
    };

    struct TLAtmosphereAndLightingChannelComponent : public TLInterpolationKeyComponent<keys::AtmosphereAndLightingChannelKey>
    {
        __int64 field_b0;
        __int64 field_b8;
        __int64 field_c0;
    };

    struct TLCameraDoFChannelComponent : public TLInterpolationKeyComponent<keys::CameraDoFChannelKey>
    {
        float field_b0;
        uint8_t channelNum;
        __int8 field_b5;
        __int16 field_b6;
    };

    struct TLCameraExposureChannelComponent : public TLCameraDoFChannelComponent
    {
    };

    struct TLMaterialKeyComponent : public TLInterpolationKeyComponent<keys::ValueKey<float>>
    {
        float defaultVal;
        __int32 field_b4;
    };

    struct TLMaterialTextureKeyComponent : public TLInterpolationKeyComponent<keys::MaterialTextureKeyKey>
    {
        __int64 field_b0;
        __int64 field_b8;
    };

    struct TLShowArmorChannelComponent : public TLEventKeyComponent<keys::ShowArmorChannelKey>
    {
        __int64 field_b0;
    };

    struct TLSplatterChannelComponent : public TLInterpolationKeyComponent<keys::SplatterChannelKey>
    {
        __int64 field_b0;
        uint8_t SplatterType;
    };

    struct TLTransformChannelFloatComponent : public TLInterpolationKeyComponent<keys::TransformChannelFloatKey>
    {
        float defaultVal;
        __int32 field_b4;
        Array<__int64> field_b8;
    };

    struct TLTransformChannelFrameOfReferenceComponent : public TLInterpolationKeyComponent<keys::TransformChannelFrameOfReferenceKey>
    {
        keys::TransformChannelFrameOfReferenceKey::FrameOfReference defaultVal;
        Array<__int64> field_b8;
    };

    struct TLTransformChannelQuatComponent : public TLInterpolationKeyComponent<keys::TransformChannelQuatKey>
    {
        glm::fvec4 defaultVal;
        Array<__int64> field_c0;
    };
END_BARE_NS()

struct TimelineActorPropertiesReflectionComponent : public TLBaseComponent
{
    struct Parameter : ProtectedGameObject<Parameter>
    {
        struct Range
        {
            channels::TimelineActorPropertiesReflectionKeyComponent** begin;
            channels::TimelineActorPropertiesReflectionKeyComponent** end;
        };
        virtual ~Parameter() = 0;
        virtual bool readData(void* dataReader) = 0;
        virtual Range& getRange(Range& r) = 0;

        [[bg3::hidden]] void* vtable;
        FixedString ParameterName;
        __int32 field_c;
        __int64 field_10;
        __int64 field_18;
        __int64 field_20;
        __int64 field_28;
        __int64 field_30;
        __int64 field_38;
        __int64 field_40;
        __int64 field_48;
        __int64 field_50;

        [[bg3::getter]] inline std::span<channels::TimelineActorPropertiesReflectionKeyComponent*> PropertyParameters()
        {
            [[bg3::hidden]] Range range;
            getRange(range);
            return std::span(range.begin, range.end - range.begin);
        }
    };
            
    __int64 field_80;
    Array<Parameter*> PropertyParameters;
};

struct TLAnimationComponent : public TLBaseComponent
{
    __int64 field_80;
    bg3se::Guid AnimationSourceId;
    __int64 field_98;
    __int64 field_a0;
    __int64 field_a8;
    // channels::EventKeyChannel<bool> field_b0;
    [[bg3::hidden]] std::array<__int8, 0xb8> raw_field_b0;
    [[bg3::getter]] channels::EventKeyChannel<bool>* field_b0()
    {
        return reinterpret_cast<channels::EventKeyChannel<bool>*>(&raw_field_b0);
    }
    FixedString AnimationSlot;
    bool Continuous;
    bool HoldAnimation;
    bool EnableRootMotion;
    bool IsMirrored;
    double FadeIn;
    double FadeOut;
    double FadeInOffset;
    double FadeOutOffset;
    __int64 field_190;
    double AnimationPlayRate;
    double AnimationPlayStartOffset;
    uint8_t OffsetType;
    __int8 field_1a9;
    __int16 field_1aa;
    __int32 field_1ac;
    bg3se::Guid AnimationGroup;
    struct TargetTransforms
    {
        glm::fvec4 RotationQuat;
        glm::fvec3 Position;
        float field_1c;
        float field_20;
        float Scale;
    };
    TargetTransforms TargetTransform;
    bg3se::Guid BoneGroupId;
    __int64 field_1f8;
    // channels::EventKeyChannel<bool> HideVfxChannel;
    [[bg3::hidden]] std::array<__int8, 0xb8> raw_HideVfxChannel;
    [[bg3::getter]] channels::EventKeyChannel<bool>* HideVfxChannel()
    {
        return reinterpret_cast<channels::EventKeyChannel<bool>*>(&raw_HideVfxChannel);
    }
    __int64 field_2b8;
    __int64 field_2c0;
};

struct TLAdditiveAnimationComponent : public TLAnimationComponent
{
};

struct TLAtmosphereAndLightingComponent : public TLBaseComponent
{
    std::array<channels::TLAtmosphereAndLightingChannelComponent*, 2> Channels;
};

struct TLAttachToEventComponent : public TLInterpolationKeyComponent<keys::AttachToEventKey>
{
};

struct TLAttitudeEventComponent : public TLKeyBaseComponent<keys::AttitudeEventKey>
{
    FixedString field_b0;
    __int32 field_b4;
};

struct TLCameraDoFComponent : public TLBaseComponent
{
    Array<channels::TLCameraDoFChannelComponent*> Channels;
};

struct TLCameraExposureComponent : public TLBaseComponent
{
    Array<channels::TLCameraExposureChannelComponent*> Channels;
    __int64 field_90;
};

struct TLCameraFoVComponent : public TLInterpolationKeyComponent<keys::CameraFoVKey>
{
};

struct TLCameraLookAtComponent : public TLInterpolationKeyComponent<keys::CameraLookAtKey>
{
};

struct TLEffectPhaseEventComponent : public TLEventKeyComponent<keys::EffectPhaseEventKey>
{
};

struct TLEmotionEventComponent : public TLEventKeyComponent<keys::EmotionEventKey>
{
};

struct TLFloatRTPCComponent : public TLInterpolationKeyComponent<keys::FloatRTPCKey>
{
    __int64 field_b0;
    FixedString RTPCName;
    uint8_t SoundType;
    uint8_t SoundObjectIndex;
    [[bg3::hidden]] __int16 field_be;
};

struct TLGenomeTextEventComponent : public TLEventKeyComponent<keys::GenomeTextEventKey>
{
};

struct TLHandsIKComponent : public TLEventKeyComponent<keys::HandsIKKey>
{
};

struct TLLayeredAnimationComponent : public TLAnimationComponent
{
};

struct TLLookAtEventComponent : public TLEventKeyComponent<keys::LookAtEventKey>
{
};

struct TLMaterialComponent : public TLBaseComponent
{
    struct Parameter
    {
        struct Range
        {
            Component** begin;
            Component** end;
        };
        virtual ~Parameter() = 0;
        virtual bool parseXML(void*) = 0;
        virtual Range& getRange(Range&) = 0;
        FixedString MaterialParameterName;
        __int32 field_c;
        __int64 field_10;
    };

    struct TextureParameter : public Parameter
    {
        bool TextureParam;
        bool IsVirtual;
        __int16 field_1a;
        __int32 field_1c;

        [[bg3::getter]] inline std::span<channels::TLMaterialTextureKeyComponent*> PropertyParameters()
        {
            [[bg3::hidden]] Range range;
            getRange(range);
            return std::span((channels::TLMaterialTextureKeyComponent**)range.begin, range.end - range.begin);
        }
    };

    struct MaterialParameter : public Parameter
    {
        [[bg3::getter]] inline std::span<channels::TLMaterialKeyComponent*> PropertyParameters()
        {
            [[bg3::hidden]] Range range;
            getRange(range);
            return std::span((channels::TLMaterialKeyComponent**)range.begin, range.end - range.begin);
        }
    };

    Array<Parameter*> MaterialParameters;
    // channels::EventKeyChannel<bool> VisibilityChannel;
    [[bg3::hidden]] std::array<__int8, 0xb8> raw_VisibilityChannel;
    [[bg3::getter]] channels::EventKeyChannel<bool>* VisibilityChannel()
    {
        return reinterpret_cast<channels::EventKeyChannel<bool>*>(&raw_VisibilityChannel);
    }
    bg3se::Guid GroupId;
    bool IsContinuous;
    bool IsOverlay;
    __int16 field_15a;
    float OverlayPriority;
};

struct TLPhysicsComponent : public TLEventKeyComponent<keys::PhysicsKey>
{
};

struct TLPlayEffectEventComponent : public TLEventKeyComponent<keys::PlayEffectEventKey>
{
    __int64 field_b0;
};

struct TLPlayRateComponent : public TLKeyBaseComponent<keys::PlayRateKey>
{
};

struct TLShapeShiftComponent : public TLEventKeyComponent<keys::ShapeShiftKey>
{
};

struct TLShotComponent : public TLBaseComponent
{
    __int64 field_80;
    bg3se::Guid FirstCamera;
    Array<bg3se::Guid> CameraContainers;
    bool AutomatedCamera;
    bool AutomatedLighting;
    bool DisableConditionalStaging;
    bool IsLooping;
    bool SwitchInterval;
    bool IsJCutEnabled;
    __int8 field_b1;
    __int16 field_b2;
    float JCutLength;
    __int64 field_b8;
    __int64 field_c0;
    __int64 field_c8;
    __int64 field_d0;
    bool IsLogicEnabled;
    __int8 field_d9;
    __int16 field_da;
    __int32 field_dc;
    bg3se::Guid CompanionCameraA;
    bg3se::Guid CompanionCameraB;
    bg3se::Guid CompanionCameraC;
    __int64 field_110;
    __int64 field_118;
    __int64 field_120;
    __int64 field_128;
    __int64 field_130;
    __int64 field_138;
    __int64 field_140;
    __int64 field_148;
    __int64 field_150;
};

struct TLShotHoldPreviousComponent : public TLBaseComponent
{
};

struct TLShotZoomComponent : public TLBaseComponent
{
    float StartOffset;
    float EndOffset;
};

struct TLShowArmorComponent : public TLBaseComponent
{
    __int64 field_80;
    Array<channels::TLShowArmorChannelComponent*> Channels;
    __int64 field_98;
};

struct TLShowHUDComponent : public TLEventKeyComponent<keys::ShowHUDKey>
{
};

struct TLShowPeanutsComponent : public TLEventKeyComponent<keys::ShowPeanutsKey>
{
};

struct TLShowVisualComponent : public TLEventKeyComponent<keys::ShowVisualKey>
{
};

struct TLShowWeaponComponent : public TLEventKeyComponent<keys::ShowWeaponKey>
{
};

struct TLSoundEventComponent : public TLEventKeyComponent<keys::SoundEventKey>
{
    Array<TLFloatRTPCComponent*> RTPCChannels;
};

struct TLSplatterComponent : public TLBaseComponent
{
    Array<channels::TLSplatterChannelComponent*> Channels;
};

struct TLSpringsComponent : public TLEventKeyComponent<keys::SpringsKey>
{
    uint32_t VisualFlag;
    __int32 field_b4;
};

struct TLSteppingFadeComponent : public TLEventKeyComponent<keys::SteppingFadeKey>
{
};

struct TLSwitchLocationEventComponent : public TLEventKeyComponent<keys::SwitchLocationEventKey>
{
};

struct TLSwitchStageEventComponent : public TLEventKeyComponent<keys::SwitchStageEventKey>
{
    __int64 field_b0;
    __int64 field_b8;
};

struct TLTransformComponent : public TLBaseComponent
{
    channels::TLTransformChannelFloatComponent* field_80;
    channels::TLTransformChannelFloatComponent* field_88;
    channels::TLTransformChannelFloatComponent* field_90;
    channels::TLTransformChannelQuatComponent* field_98;
    channels::TLTransformChannelFloatComponent* field_a0;
    channels::TLTransformChannelFrameOfReferenceComponent* field_a8;
    bool Continuous;
    __int8 field_b1;
    __int16 field_b2;
    __int32 field_b4;
};

struct TLVoiceComponent : public TLBaseComponent
{
    bg3se::Guid DialogNodeId;
    bg3se::Guid ReferenceId;
    uint8_t FaceType;
    __int8 field_a1;
    __int16 field_a2;
    __int32 field_a4;
    double LeftBuffer;
    bool HoldMocap;
    bool DisableMocap;
    bool IsAliased;
    bool IsMirrored;
    __int32 field_b4;
    double PerformanceFade;
    double FadeIn;
    double FadeOut;
    double FadeInOffset;
    double FadeOutOffset;
    __int64 field_e0;
    uint8_t PerformanceDriftType;
    __int8 field_e9;
    __int16 field_ea;
    __int32 field_ec;
    double HeadPitchCorrection;
    double HeadYawCorrection;
    double HeadRollCorrection;
};


struct BillboardComponent : public FxBaseComponent
{
    Transform Transform;
    RenderableObject* BillboardRenderable;
    FloatKeyFrameProperty* KeyframedOffsetProperty;
    FloatKeyFrameProperty* OrientationProperty;
    StringProperty* TextProperty;
    ColorARGBKeyFrameProperty* ColorProperty;
    FloatKeyFrameProperty* BrightnessProperty;
    FloatKeyFrameProperty* SizeProperty;
    FloatProperty* UniformScaleProperty;
    FloatKeyFrameProperty* DynamicValueProperty;
    FloatKeyFrameProperty* AlphaProperty;
    FloatProperty* ModifierXProperty;
    FloatProperty* ModifierYProperty;
    FloatProperty* ModifierZProperty;
    FloatProperty* ModifierWProperty;
    Vector3Property* OffsetProperty;
    float UniformScale;
    float SnapToTop;
    float SnapToBottom;
    float Brightness;
    bool SnapToGround;
    bool MaterialApplied;
    uint8_t Facing;
    CustomModules CustomModules;
    glm::vec3 field_178;
    glm::vec3 field_184;
    glm::vec3 Position;
    glm::vec3 KeyframedPosition;
    glm::aligned_highp_vec4 Color;
    RenderPropertyList PropertyList;
    [[bg3::hidden]] uint16_t field_1C0;
};

struct BoundingBoxComponent : public FxBaseComponent
{
    glm::vec3 Center;
    glm::vec3 Size;
    glm::vec3 Position;
    glm::vec3 RequiredSize;
    // RenderableObject* BoxVisual;
};

struct BoundingSphereComponent : public FxBaseComponent
{
    glm::vec3 Center;
    float Radius;
    glm::vec3 Position;
    float RequiredRadius;
    // RenderableObject* SphereVisual;
};

struct CameraShakeComponent : public FxBaseComponent
{
    aspk::FloatRangeProperty* FalloffStartEndProperty;
    aspk::FloatKeyFrameProperty* ShakeStrengthProperty;
    aspk::FloatKeyFrameProperty* ShakeVibrationFrequencyProperty;
    uint64_t field_B8;
    float ShakeStrength;
    float ShakeFrequency;
    float field_C8;
    float field_CC;
    float UpdateTime;
    uint8_t Flags;
};

struct DecalComponent : public FxBaseComponent
{
    ColorARGBKeyFrameProperty* ColorProperty;
    FloatKeyFrameProperty* AlphaProperty;
    FloatKeyFrameProperty* BrightnessProperty;
    FloatKeyFrameProperty* DimensionsProperty;
    FloatKeyFrameProperty* ProjectedOffsetProperty;
    FloatKeyFrameProperty* OffsetProperty;
    FloatKeyFrameProperty* RotationProperty;
    FloatProperty* UniformScaleProperty;
    FloatKeyFrameProperty* DynamicValueProperty;
    FloatProperty* ModifierXProperty;
    FloatProperty* ModifierYProperty;
    FloatProperty* ModifierZProperty;
    FloatProperty* ModifierWProperty;
    [[bg3::hidden]] void* field_108;
    Material* Material;
    EntityHandle DecalEntity;
    float Distance;
    float NormalBlendingFactor;
    float Brightness;
    float UniformScale;
    glm::vec3 Offset;
    glm::mat3x3 Axis;
    glm::vec3 Position;
    glm::vec3 KeyframedPosition;
    glm::aligned_highp_vec4 Color;
    CustomModules CustomModules;
    uint8_t CoordinateSpace;
    bool IgnoreParentRotation;
};

struct DeflectorComponent : public FxBaseComponent
{
    bool Visible;
    glm::vec3 Normal;
    glm::vec3 Position;
    FloatProperty* PositionModifierXProperty;
    FloatProperty* PositionModifierYProperty;
    FloatProperty* PositionModifierZProperty;
    RenderableObject* PlaneVisual1;
    RenderableObject* PlaneVisual2;
};

struct DragForceComponent : public FxBaseComponent
{
    FloatKeyFrameProperty* InfluenceRadiusProperty;
    FloatKeyFrameProperty* KeyframedOffsetProperty;
    FloatKeyFrameProperty* DampingProperty;
    glm::vec3 Offset;
    bool InfluenceRadiusApplied;
    float InfluenceRadius;
    glm::vec3 Position;
    glm::vec3 KeyframedPosition;
    glm::vec3 field_E4;
};

struct GravityForceComponent : public FxBaseComponent
{
    FloatKeyFrameProperty* InfluenceRadiusProperty;
    FloatKeyFrameProperty* KeyframedOffsetProperty;
    FloatKeyFrameProperty* AccelerationProperty;
    glm::vec3 Offset;
    glm::vec3 Direction;
    bool InfluenceRadiusApplied;
    float InfluenceRadius;
    glm::vec3 Position;
    glm::vec3 KeyframedPosition;
};

struct LightComponent : public FxBaseComponent
{
    ColorARGBKeyFrameProperty* ColorProperty;
    FloatKeyFrameProperty* IntensityProperty;
    FloatKeyFrameProperty* RadiusProperty;
    FloatKeyFrameProperty* PositionProperty;
    FloatKeyFrameProperty* RotationProperty;
    FloatKeyFrameProperty* FlickerAmountProperty;
    FloatKeyFrameProperty* FlickerSpeedProperty;
    FloatKeyFrameProperty* InitialRotationProperty;
    FloatKeyFrameProperty* RotationLifeProperty;
    [[bg3::hidden]] void* field_E8;
    EntityHandle LightEntity;
    float SnapToTop;
    float SnapToBottom;
    glm::vec3 CalculatedPosition;
    glm::vec3 LastPosition;
    glm::vec3 field_120;
    glm::vec3 Velocity;
    glm::vec3 Offset;
    glm::vec3 Position;
    glm::vec3 KeyframedPosition;
    glm::vec3 field_15C;
    glm::vec3 field_168;
    glm::quat Rotation;
    bool WasActive;
    bool IgnoreParentRotation;
    bool ModulateLightTemplateRadius;
    bool OverrideLightTemplateColor;
    bool OverrideLightTemplateFlickerSpeed;
    bool SnapToGround;
    bool field_18A;
    bool AnimateManually;
    uint8_t CoordinateSpace;
};

struct ModelComponent : public FxBaseComponent
{
    EntityHandle VisualEntity;
    CustomModules CustomModules;
    float field_C8;
    float FadeOutTime;
    float Alpha;
    float Brightness;
    float UniformScale;
    float SnapTop;
    float SnapBottom;
    bool LoopAnimation;
    bool Active;
    bool SnapToGround;
    bool HasVisual;
    FixedString Animation;
    FixedString field_EC;
    FixedString field_F0;
    aspk::FloatKeyFrameProperty* KeyframedOffsetProperty;
    aspk::FloatKeyFrameProperty* ScaleRotationDurationProperty;
    aspk::FloatKeyFrameProperty* InitialRotationProperty;
    aspk::FloatKeyFrameProperty* RotationLifeProperty;
    aspk::ColorARGBKeyFrameProperty* ColorProperty;
    aspk::FloatKeyFrameProperty* BrightnessProperty;
    aspk::FloatKeyFrameProperty* UniformScaleProperty;
    aspk::FloatKeyFrameProperty* AxisScaleProperty;
    aspk::FloatProperty* AnimationSpeedProperty;
    aspk::BooleanProperty* SyncAnimationResourceTimeProperty;
    aspk::FloatKeyFrameProperty* DynamicValueProperty;
    aspk::FloatKeyFrameProperty* ModelAlphaProperty;
    aspk::FloatProperty* ModelModifierXProperty;
    aspk::FloatProperty* ModelModifierYProperty;
    aspk::FloatProperty* ModelModifierZProperty;
    aspk::FloatProperty* ModelModifierWProperty;
    aspk::FloatProperty* ScaleModifierXProperty;
    aspk::FloatProperty* ScaleModifierYProperty;
    aspk::FloatProperty* ScaleModifierZProperty;
    aspk::FloatProperty* KeyframedPositionModifierXProperty;
    aspk::FloatProperty* KeyframedPositionModifierYProperty;
    aspk::FloatProperty* KeyframedPositionModifierZProperty;
    aspk::BooleanProperty* ShowSkeletonProperty;
    [[bg3::hidden]] __int64 field_1B0;
    [[bg3::hidden]] __int64 field_1B8;
    [[bg3::hidden]] __int64 field_1C0;
    [[bg3::hidden]] __int64 field_1C8;
    glm::vec3 AxisScale;
    glm::vec3 Position;
    glm::vec3 KeyframedPosition;
    glm::vec3 Rotation;
    glm::vec3 Offset;
    glm::vec3 field_20C;
    glm::vec3 field_218;
    glm::aligned_highp_vec4 Color;
    glm::quat PivotRotation;
    glm::quat PostRotation;
};

struct MovingLevelComponent : public FxBaseComponent
{
    STDString LevelTemplate;
    glm::vec3 Offset;
    glm::vec3 ScrollingDirection;
    float ScrollingSpeed;
    float ScrollingDistance;
    float UpdateTime;
};

struct OrbitForceComponent : public FxBaseComponent
{
    FloatKeyFrameProperty* KeyframedOffsetProperty;
    FloatKeyFrameProperty* OrbitRadiusProperty;
    glm::vec3 Offset;
    glm::vec3 Position;
    glm::vec3 KeyframedPosition;
    glm::vec3 field_D4;
};

struct OverlayMaterialComponent : public FxBaseComponent
{
    ColorARGBKeyFrameProperty* ColorProperty;
    FloatKeyFrameProperty* BrightnessProperty;
    FloatProperty* AnimationSpeedProperty;
    FloatKeyFrameProperty* DynamicValueProperty;
    FloatKeyFrameProperty* AlphaProperty;
    FloatProperty* ModifierXProperty;
    FloatProperty* ModifierYProperty;
    FloatProperty* ModifierZProperty;
    FloatProperty* ModifierWProperty;
    EntityHandle EffectEntity;
    FixedString Material;
    uint32_t ApplyMaps;
    CustomModules CustomModules;
    uint64_t Applied;
    uint32_t ApplyTo;
    uint32_t OverlayType;
    glm::aligned_highp_vec4 Color;
    glm::vec3 VisualPosition;
    float Brightness;
    int32_t OverlayPriority;
    float FadeTime;
    uint8_t field_150;
    uint8_t field_151;
    bool Active;
    bool HasCustomVectors;
    uint8_t field_154;
    uint8_t AlphaChannel;
    [[bg3::hidden]] uint64_t field_158;
    glm::vec4 field_160;
    glm::vec4 field_170;
    bool HasPreview;
    EntityHandle VisualEntity;
    FixedString Animation;
};

struct PackedUnitVector
{
    uint16_t X;
    uint16_t Y;
    uint16_t Z;
};

struct FxParticleData
{
    uint32_t Flags;
    uint32_t Count;
    Array<float> Lifespan;
    Array<float> Ages;
    Array<glm::vec3> Positions;
    Array<glm::vec3> Velocities;
    Array<uint16_t> Rotations;
    Array<PackedUnitVector> Normals;
    Array<PackedUnitVector> UpVectors;
    Array<uint16_t> FlipbookImageIndices;
    Array<EntityHandle> field_88;
    Array<uint16_t> InitialRotationSpeeds;
    Array<uint32_t> FixedColors;
    Array<uint16_t> FixedScalars;
    Array<float> DiscardValues;
    Array<float> UnitAges;
    Array<glm::vec3> Scales;
    Array<glm::vec3> AppliedForces;
    [[bg3::hidden]] Array<void*> MoveInstructions;
    Array<uint32_t> DiedParticles;
};

struct ParticleRenderStyle1
{
    Vector3Property* AxisAlignedOrientation;
    uint8_t Flags;
};

struct ParticleFlipbookData
{
    uint16_t InitialFrame;
    uint16_t HorizontalImages;
    uint16_t VerticalImages;
    uint16_t FrameRate;
    uint16_t FrameRangeMin;
    uint16_t FrameRangeMax;
    FloatKeyFrameProperty* ImageController;
    uint8_t Flags;
};

struct ParticleSystemSnapBehavior
{
    glm::vec3 field_0;
    glm::vec3 field_C;
    glm::mat3x3 field_18;
    float field_3C;
    float Top;
    float Bottom;
};

struct ParticleDistanceScale
{
    float Min;
    float Max;
    FloatKeyFrameProperty* Scale;
};

struct ParticleSystemComponent : public FxBaseComponent
{
    ParticleRenderStyle1* RenderStyle1;
    ParticleFlipbookData* Flipbook;
    [[bg3::hidden]] void* Emitter;
    ParticleSystemSnapBehavior* SnapBehavior;
    FloatKeyFrameProperty* KeyframedOffsetProperty;
    ColorARGBKeyFrameProperty* ParticleColorProperty;
    FloatKeyFrameProperty* ParticleBrightnessProperty;
    FloatKeyFrameProperty* ParticleAlphaProperty;
    FloatKeyFrameProperty* ParticleScaleProperty;
    FloatKeyFrameProperty* ParticleLocalOriginProperty;
    FloatKeyFrameProperty* ParticleVelocityOffsetProperty;
    FloatKeyFrameProperty* ParticleInitialRotationProperty;
    FloatKeyFrameProperty* ParticleRotationProperty;
    FloatKeyFrameProperty* ParticleInitialRotationSpeedProperty;
    FloatKeyFrameProperty* ParticleRotationSpeedProperty;
    FloatKeyFrameProperty* ParticleAxisScaleProperty;
    FloatKeyFrameProperty* EmitRateProperty;
    FloatKeyFrameProperty* EmitStutterProperty;
    FloatKeyFrameProperty* LifespanProperty;
    FloatKeyFrameProperty* InitialVelocityProperty;
    FloatKeyFrameProperty* EmitVelocityAnglesProperty;
    FloatKeyFrameProperty* EmitVelocityAxisRotationProperty;
    ColorARGBKeyFrameProperty* ColorProperty;
    FloatKeyFrameProperty* BrightnessProperty;
    FloatKeyFrameProperty* UniformScaleProperty;
    FloatKeyFrameProperty* AxisScaleProperty;
    FloatKeyFrameProperty* MaterialDynamicValueProperty;
    FloatProperty* MaterialModifierXProperty;
    FloatProperty* MaterialModifierYProperty;
    FloatProperty* MaterialModifierZProperty;
    FloatProperty* MaterialModifierWProperty;
    FloatProperty* MaterialScaleModifierXProperty;
    FloatProperty* MaterialScaleModifierYProperty;
    FloatProperty* MaterialScaleModifierZProperty;
    FloatProperty* KeyframedPositionModifierXProperty;
    FloatProperty* KeyframedPositionModifierYProperty;
    FloatProperty* KeyframedPositionModifierZProperty;
    FloatProperty* EmitRateModifierProperty;
    FloatKeyFrameProperty* InheritVelocityPercentProperty;
    glm::vec3 Offset;
    glm::vec3 LastPosition;
    glm::vec3 EmitVelocityAxis;
    glm::vec3 EmitPosition;
    float WarmUpTime;
    float RemainingParticleCount;
    float RecycleRadius;
    float RotationRateOverLife;
    float InitialVelocity;
    float Brightness_;
    float MinLifespan;
    float MaxLifespan;
    float UniformScale;
    glm::vec3 AxisScale;
    glm::vec3 Position;
    glm::vec3 KeyframedPosition;
    float ColorOverEffectLife;
    float ScaleOverEffectLife;
    glm::vec3 ScaleOverEffectLifeAxis;
    glm::aligned_highp_vec4 Color;
    float PhysicalMaterialFriction;
    float PhysicalMaterialBounce;
    float PhysicalMaterialLifetimeLoss;
    float PhysicalMaterialRadiusScale;
    glm::mat3x3 PivotRotation;
    // int field_2C4;
    Material* Material;
    FixedString Visual;
    [[bg3::hidden]] void* field_2D8;
    [[bg3::hidden]] void* MeshRenderList;
    [[bg3::hidden]] void* RenderBatch;
    [[bg3::hidden]] uint64_t field_2F0;
    uint8_t BlendStateID;
    uint8_t field_2F9;
    uint16_t field_2FA;
    uint16_t MaximumParticleCount;
    uint16_t InitialParticleCountMin;
    uint16_t InitialParticleCountMax;
    uint16_t RecycleRadius_;
    uint8_t RenderStyle;
    uint8_t CoordinateSpace;
    uint8_t EmitterType;
    uint8_t RenderOrder;
    uint8_t AttachedFXOnly;
    uint8_t Flags;
    uint8_t Flags2;
    // Array<void*> DebugShapes;
    // void* RecycleRadiusDebugShape;
    glm::aligned_highp_vec4 FixedColor;
    glm::vec3 ExternalVelocity;
    glm::vec3 ScaleMultiplier;
    glm::aligned_highp_vec4 ColorMultiplier;
    [[bg3::hidden]] Array<void*> CustomFloatModules;
    [[bg3::hidden]] Array<void*> CustomFloat3Modules;
    ParticleDistanceScale AlphaByEffectDistance;
    ParticleDistanceScale AlphaByParticleDistance;
    ParticleDistanceScale ScaleByEffectDistance;
    ParticleDistanceScale ScaleByParticleDistance;
    bool ParticleDiscardByDistanceEnabled;
    float ParticleDiscardByDistanceMin;
    float ParticleDiscardByDistanceMax;
    FloatKeyFrameProperty* ParticleDiscardByDistanceScale;
    FxParticleData ParticleData;
    [[bg3::hidden]] void* ParticleDeathSpawnFx;
    [[bg3::hidden]] void* AttachedFx;
    [[bg3::hidden]] void* field_500;
};

struct PostProcessComponent : public FxBaseComponent
{
    [[bg3::hidden]] Array<void*> field_A0; // FxPostProcessObject*
};

struct PreRollComponent : public FxBaseComponent
{
};

struct RadialForceComponent : public FxBaseComponent
{
    FloatKeyFrameProperty* InfluenceRadiusProperty;
    FloatKeyFrameProperty* KeyframedOffsetProperty;
    FloatKeyFrameProperty* AccelerationProperty;
    FloatKeyFrameProperty* LockDistanceProperty;
    glm::vec3 Offset;
    bool InfluenceRadiusApplied;
    bool LockDistanceApplied;
    float InfluenceRadius;
    glm::vec3 Position;
    glm::vec3 KeyframedPosition;
};

struct RibbonParticleData
{
    int field_0;
    int Size;
    int ParticleCount;
    int NextParticle;
    Array<uint16_t> Particles;
};

struct RibbonParticleHolder
{
    RibbonParticleData* Data;
    int field_8;
};

struct Ribbon2Component : public FxBaseComponent
{
    [[bg3::hidden]] void* EffectManager;
    [[bg3::hidden]] void* ResourceManager;
    [[bg3::hidden]] void* MaterialSystem;
    uint32_t field_B8;
    uint32_t CoordinateSpace;
    uint32_t EmissionBehavior;
    uint32_t TexCoordType;
    [[bg3::hidden]] RibbonParticleData ParticleData;
    uint32_t DynamicParametersType;
    FloatKeyFrameProperty* DynamicParameterValueProperty;
    FloatProperty* DynamicParameterModifierXProperty;
    FloatProperty* DynamicParameterModifierYProperty;
    FloatProperty* DynamicParameterModifierZProperty;
    FloatProperty* DynamicParameterModifierWProperty;
    float field_118;
    float field_11C;
    float field_120;
    float field_124;
    uint32_t ColorType;
    ColorARGBKeyFrameProperty* ColorProperty;
    FloatKeyFrameProperty* BrightnessProperty;
    FloatKeyFrameProperty* AlphaProperty;
    uint32_t ScaleType;
    FloatKeyFrameProperty* ScaleProperty;
    FloatProperty* ScaleModifierProperty;
    float field_160;
    uint32_t field_164;
    glm::vec3 InitialVelocity;
    bool UseObjectOrientation;
    glm::quat ExtraRotation;
    bool InfiniteParticleLife;
    float ParticleLife;
    EntityHandle VisualEntity;
    Visual* Visuals;
    RenderableObject* Effect;
    uint32_t MaxParticleCount;
    float EmitRate;
    float EmitDistance;
    float UpdateTime;
    float field_1B8;
    float UnitTime;
    bool InvertTexcoord;
    bool ConnectToEmitter;
    glm::vec3 field_1C4;
    glm::vec3 Velocity;
    glm::vec3 Translate;
    glm::quat Rotation;
    uint32_t Flags;
    [[bg3::hidden]] std::array<RibbonParticleHolder, 6> ParticleHolders;
    Array<Visual*> DebugShapes;
    Array<Visual*> DebugShapes2;
    glm::vec3 BoundMin;
    glm::vec3 BoundMax;
};

struct SoundComponent : public FxBaseComponent
{
    [[bg3::hidden]] void* App;
    [[bg3::hidden]] void* TransformSystem;
    [[bg3::hidden]] void* ResourceManager;
    [[bg3::hidden]] void* SoundManager;
    [[bg3::hidden]] void* SoundEvent;
    [[bg3::hidden]] void* SoundEvent2;
    [[bg3::hidden]] void* SoundEvent3;
    FixedString SoundEventName;
    FixedString SoundEventName2;
    FixedString SoundEventName3;
    int32_t PlayOn;
    uint8_t Flags;
    uint8_t Flags2;
};

struct SpinForceComponent : public FxBaseComponent
{
    FloatKeyFrameProperty* InfluenceRadiusProperty;
    FloatKeyFrameProperty* KeyframedOffsetProperty;
    FloatKeyFrameProperty* SpinVelocityProperty;
    glm::vec3 Offset;
    glm::vec3 SpinAxis;
    bool InfluenceRadiusApplied;
    float InfluenceRadius;
    glm::vec3 Position;
    glm::vec3 KeyframedPosition;
};

struct TurbulenceForceComponent : public FxBaseComponent
{
    FloatKeyFrameProperty* InfluenceRadiusProperty;
    FloatKeyFrameProperty* KeyframedOffsetProperty;
    FloatKeyFrameProperty* AccelerationProperty;
    glm::vec3 Offset;
    bool InfluenceRadiusApplied;
    float InfluenceRadius;
    glm::vec3 Position;
    glm::vec3 KeyframedPosition;
};

struct VortexForceComponent : public FxBaseComponent
{
    FloatKeyFrameProperty* InfluenceRadiusProperty;
    FloatKeyFrameProperty* KeyframedOffsetProperty;
    FloatKeyFrameProperty* SpinVelocityProperty;
    glm::vec3 Offset;
    glm::vec3 RotationAxis;
    bool InfluenceRadiusApplied;
    float InfluenceRadius;
    glm::vec3 Position;
    glm::vec3 KeyframedPosition;
};

struct WindForceComponent : public FxBaseComponent
{
    aspk::FloatKeyFrameProperty* SpeedMultiplier;
    float UpdateTime;
};


struct Timeline : public ProtectedGameObject<Timeline>
{
    virtual ~Timeline() = 0;
    virtual void Play() = 0;
    virtual bool IsStopped() const = 0;
    virtual void Clear() = 0;
    virtual void Reset(bool) = 0;
    virtual void PostUpdate() = 0;
    virtual void SetTargetVisual(uint64_t) = 0;
    virtual void ResetTargetVisual(uint64_t) = 0;
    virtual void Stop() = 0;
    virtual void InitializeInternal() = 0;
    virtual bool IsPhaseValid() const = 0;
    virtual void UpdateTargetVisual() const = 0;
    virtual void UpdateTargetVisual2() const = 0;
    virtual void SortComponents() const = 0;
    virtual void UpdateComponents(float) const = 0;
    virtual bool IsComponentInPhase(Component*, uint64_t) const = 0;
    virtual void ResetComponents() = 0;

    uint64_t PhaseIndex;
    TimelineHeader* Header;
    Array<Component*>* Components;
    uint8_t State;
    bool PlayToEnd;
    bool IsPaused;
    bool field_23;
    bool field_24;
    bool NeedsPostUpdate_M;
    float PlayingSpeed;
    float TimePlayed;
    float Duration;
    float RemainingPhaseTime;
    float JumpToTime;
    int JumpToPhase;
    int CurrentPlayCount;
    Array<Input*> Inputs;
};


struct EffectTimeline : public Timeline
{
    // This is already exposed in aspk::Timeline
    [[bg3::hidden]] TimelineHeader Header;
    // This is already exposed in aspk::Timeline
    [[bg3::hidden]] Array<Component*> Components;
};

END_NS()

BEGIN_NS(lua)

LUA_POLYMORPHIC(aspk::Property);
LUA_POLYMORPHIC(aspk::KeyFrameData);
LUA_POLYMORPHIC(aspk::Component);
LUA_POLYMORPHIC(aspk::TLMaterialComponent::Parameter);

END_NS()
