#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_NS(rf)

struct Camera
{
    float field_0;
    glm::vec3 field_4;
    glm::mat4 ViewMatrix;
    glm::mat4 InvViewMatrix;
    glm::mat4 ProjectionMatrix;
    glm::mat4 InvProjectionMatrix;
    //glm::mat4 GetViewProjectionMatrix;
    //glm::mat4 GetOffsetViewProjectionMatrix;
    std::array<glm::vec4, 10> CullingPlanes;
};

struct CameraController : public ProtectedGameObject<CameraController>
{
    virtual ~CameraController() = 0;
    virtual bool Enter() = 0;
    virtual bool Exit() = 0;
    virtual float GetFov() = 0;
    virtual float GetAspectRatio() = 0;
    virtual void Update(GameTime const&) = 0;
    virtual bool DoLookAt(glm::vec3 const&, glm::vec3 const&, bool) = 0;
    virtual bool EditorLookAt(glm::vec3 const&) = 0;
    virtual void EditorZoomExtents(glm::vec3 const&, float) = 0;
    virtual glm::vec3 const& GetWorldTranslate() const = 0;
    virtual void SetWorldTranslate(glm::vec3 const&) = 0;
    virtual glm::quat const& GetWorldRotate() const = 0;
    virtual void SetWorldRotate(glm::quat const&) = 0;
    virtual Transform const& GetWorldTransform() const = 0;
    virtual void SetWorldTransform(Transform const&) = 0;
    virtual void WarpMouse(bool) = 0;
    virtual void ClearInputState() = 0;
    virtual uint16_t* OnInputEvent(uint16_t* eventRetVal, void* inputEvent) = 0;
    virtual void StopMotion() = 0;
    virtual bool SetOrientation(float, float) = 0;
    virtual void SetMoveSpeed(float) = 0;
    virtual void SetLookAtShake(float, float) = 0;
    virtual void SetWobble(float, float) = 0;
    virtual glm::vec3 GetDistance_M() = 0;

    [[bg3::hidden]] __int64 field_8;
    bool ViewDirty;
    bool ProjectionDirty;
    bool IsOrthographic;
    bool OffCenter;
    float FOV;
    float NearPlane;
    float FarPlane;
    float AspectRatio;
    Transform WorldTransform;
    glm::vec3 LookAt;
    glm::vec3 field_58;
    float OrthoLeft;
    float OrthoRight;
    float OrthoBottom;
    float OrthoTop;
    FixedString ID;
    [[bg3::hidden]] __int64 field_78;
    Camera Camera;
    std::array<glm::vec3, 8> FrustumPoints;
};

END_NS()

BEGIN_SE()

struct DOFSettings
{
    bool DOF;
    bool NearOnly;
    bool FarOnly;
    float FocalDistance;
    float NearSharpDistance;
    float FarSharpDistance;
    float Aperture;
};

struct DeltaExposureSettings
{
    float DeltaCompensation;
    float DeltaMin;
    float DeltaMax;
};

struct DeltaExposure
{
    bool Enabled;
    DeltaExposureSettings Settings;
};

struct ExposureSettings
{
    float ExposureMin;
    float ExposureMax;
    float ExposureCompensation;
    float ExposureMinPercentile;
    float ExposureMaxPercentile;
    int ExposureMeteringMode;
    bool Exposure;
    DeltaExposure Delta;
};


struct PostProcessCameraSetting
{
    DOFSettings DOF;
    DOFSettings AlternateDOF;
    std::array<ExposureSettings, 2> Exposure;
    float LensFlareTreshold;
    float LensFlareIntensity;
    float LensFlareGhostDispersal;
    float LensFlareHaloWidth;
    float LensFlareChromaticDistortion;
    bool LensFlareEnabled;
    bool DOFForcedSettingsEnabled;
};

struct ColorCorrectionRange
{
    glm::vec2 XYOffset;
    float Saturation;
    float ExposureShift;
};

struct ColorCorrectionSettings
{
    ColorCorrectionRange GlobalRange;
    ColorCorrectionRange DarkRange;
    ColorCorrectionRange ShadowRange;
    ColorCorrectionRange MidtonesRange;
    ColorCorrectionRange HighlightRange;
    ColorCorrectionRange SpecularRange;
    glm::vec2 LutBlendFactors;
    int LutApplyMode;
    float Hue;
    float Saturation;
    float Contrast;
    float Pivot;
    float Shadows;
    float Highlights;
};

struct PostProcessSetting
{
    glm::mat4 WhiteBalanceMatrix;
    ColorCorrectionSettings ColorCorrection;
    [[bg3::hidden]] void* GradingLutTexture2;
    ColorCorrectionSettings BlendedColorCorrection;
    [[bg3::hidden]] void* BlendedLutTexture;
    float ColorCorrectionInterpolationFactor;
    float WhiteTemperature;
    float WhiteBalanceTint;
    float Brightness;
    bool GradingLutEnabled;
    FixedString GradingLut;
    [[bg3::hidden]] void* GradingLutTexture;
    bool UseViewModeHDRSettings;
    PostProcessCameraSetting Camera;
    float BloomThreshold;
    float BloomAmount;
    float GodRaysRayIntensity;
    float GodRaysPower;
    float GodRaysThreshold;
    float ToneMapHighlightsFixAmount;
    float FadeOut;
    glm::vec3 VignetteColor;
    float VignetteIntensity;
    float VignettePower;
    bool Bloom;
    bool GodRaysEnabled;
    bool Vignette;
};


struct CameraComponent : public BaseComponent
{
    DEFINE_COMPONENT(Camera, "ls::CameraComponent")

    uint32_t MasterBehaviorType;
    rf::CameraController* Controller;
    int ExposureSettingIndex;
    bool Active;
    bool AcceptsInput;
    bool UseCameraPPSettings;
    bool UseSplitScreenFov;
    PostProcessCameraSetting PostProcess;
};


struct DefaultCameraBehavior : public BaseComponent
{
    DEFINE_COMPONENT(DefaultCameraBehavior, "ls::DefaultCameraBehavior")

    float field_0;
    float Left;
    float Forward;
    float RotationX;
    float RotationY;
    float Zoom;
    bool CaptureInput;
    bool field_19;
    bool field_1A;
};

struct EffectCameraBehavior : public BaseComponent
{
    DEFINE_COMPONENT(EffectCameraBehavior, "ls::EffectCameraBehavior")

    glm::vec3 field_0;
};

END_SE()

BEGIN_NS(ecl)

struct GameCameraBehavior : public BaseComponent
{
    DEFINE_COMPONENT(GameCameraBehavior, "ecl::GameCameraBehavior")

    EntityHandle Trigger;
    EntityHandle Target;
    bool TargetFalling;
    bool field_11;
    bool MovingToTarget;
    bool field_13;
    float field_14;
    glm::vec3 TargetPreviousDestination;
    glm::vec3 TargetDestination;
    glm::vec3 field_30;
    glm::vec3 field_3C;
    glm::vec3 TargetCurrent;
    float Distance;
    float field_58;
    float field_5C;
    float field_60;
    float field_64;
    float field_68;
    float MovementDistanceMax;
    glm::vec3 Direction;
    glm::vec3 DirectionDestination;
    glm::vec3 field_88;
    glm::vec2 SpeedXZ;
    float XRotationSpeed;
    int XRotationSpeedMouse;
    float ZoomSpeed;
    uint8_t CameraMode;
    uint8_t field_A9;
    float RotationY;
    std::optional<glm::vec3> RotationTarget;
    float MouseRotationSpeed;
    glm::vec3 TargetLastPosition;
    glm::vec2 field_D0;
    float field_D8;
    float field_DC;
    float field_E0;
    float field_E4;
    float field_E8;
    std::optional<EntityHandle> FollowTarget;
    [[bg3::hidden]] void* Trigger1;
    int Trigger1ID;
    [[bg3::hidden]] void* Trigger2;
    int Trigger2ID;
    [[bg3::hidden]] void* Trigger3;
    int Trigger3ID;
    Array<EntityHandle> Targets;
    float LastPlayerInputTime;
    bool PlayerInControl;
    bool field_145;
    bool IsPaused;
    uint32_t TargetMode;
    bool SelectMode;
    bool WasInSelectMode;
    glm::vec3 field_150;
    float field_15C;
    float Zoom;
    float PitchDegrees;
    std::optional<bool> TacticalMode;
    std::optional<float> TacticalTimeout;
    Array<EntityHandle> EffectEntities;
    bool field_188;
    bool FreezeHeight;
    bool field_18A;
    bool field_18B;
    [[bg3::hidden]] HashMap<EntityHandle, void*> field_190__EH_CameraCollisionDebugInfo;
    glm::vec3 DebugPosition;
    float DebugOffset;
    std::optional<float> field_1E0;
    EntityHandle TrackTarget;
    bool IsMoving;
    bool IsRotating;
    bool field_1F2;
    bool IsSnapping;
    EntityHandle LastPickingTarget;
    uint8_t field_200;
    HashSet<int16_t> field_208;
    std::optional<glm::vec3> field_238;
    std::optional<glm::vec3> field_248;
};

struct CameraSelectorModeComponent : public BaseComponent
{
    DEFINE_COMPONENT(CameraSelectorMode, "ecl::camera::SelectorModeComponent")

    uint8_t Mode;
};

struct CameraTarget
{
    Array<EntityHandle> field_0;
    uint8_t field_10;
    glm::vec3 field_14;
    float field_20;
    float field_24;
    uint8_t field_28;
    uint8_t field_29;
    uint8_t field_2A;
    uint8_t field_2B;
    uint8_t field_2C;
    uint8_t field_2D;
};

struct CameraTargetComponent : public BaseComponent
{
    DEFINE_COMPONENT(CameraTarget, "ecl::camera::TargetComponent")

    CameraTarget Target;
};

struct CameraCombatTargetComponent : public BaseComponent
{
    DEFINE_COMPONENT(CameraCombatTarget, "ecl::camera::CombatTargetComponent")

    CameraTarget Target;
};

DEFINE_TAG_COMPONENT(ecl::camera, IsInSelectorModeComponent, CameraInSelectorMode)
DEFINE_TAG_COMPONENT(ecl::camera, SpellTrackingComponent, CameraSpellTracking)

END_NS()
