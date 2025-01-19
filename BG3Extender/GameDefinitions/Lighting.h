#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_SE()

struct MoonLight
{
    bool Enabled;
    bool CastLightEnabled;
    float Yaw;
    float Pitch;
    glm::vec3 RotationAsVec3;
    float Distance;
    float Radius;
    glm::vec3 Color;
    glm::vec3 ColorAdjustedForIntensity;
    bool UseTemperature;
    float Kelvin;
    glm::vec3 ColorTemperatureAdjustment;
    float Intensity;
    float Earthshine;
    float MoonGlare;
    float TearsScale;
    float TearsRotate;
    FixedString AlbedoTexResourceGUID;
    FixedString NormalTexResourceGUID;
    FixedString TearsAlbedoTexResourceGUID;
    FixedString TearsNormalTexResourceGUID;
};

struct CoverageSettings
{
    float StartHeight;
    float EndHeight;
    float HorizonDistance;
    FixedString TexResourceGUID;
    glm::vec2 Offset;
};

struct VolumetricCloudSettings
{
    bool Enabled;
    float Intensity;
    float AmbientLightFactor;
    float SunLightFactor;
    float ShadowFactor;
    glm::vec3 BaseColor;
    glm::vec3 TopColor;
    float BottomFade;
    float HorizonFade;
    float SampleScalar;
    float SampleThreshold;
    float DetailScale;
    float ErosionEdgeSize;
    float Distortion;
    float DistortionScale;
    float Density;
    float ForwardScatteringG;
    float BackwardScatteringG;
    float DarkOutline;
    float SunRayLength;
    float ConeRadius;
    float LODDistance;
    CoverageSettings CoverageSettings;
    float CoverageStartDistance;
    float CoverageEndDistance;
    float CoverageWindSpeed;
    float RainCoverageMaxInfluence;
};

struct SkyLight
{
    glm::vec3 Color;
    float Intensity;
    float Kelvin;
    bool UseTemperature;
    glm::vec3 ColorTemperatureAdjustment;
    glm::vec3 ColorAdjustedForIntensity;
    FixedString SkydomeTex;
    bool SkydomeEnabled;
    bool RotateSkydomeEnabled;
    int32_t PhysicalModel;
    float ScatteringIntensity;
    float ScatteringSunIntensity;
    glm::vec3 ScatteringSunColor;
    bool ScatteringEnabled;
    float ProcStarsIntensity;
    float ProcStarsAmount;
    float ProcStarsShimmer;
    glm::vec2 ProcStarsSaturation;
    bool ProcStarsEnabled;
    float CirrusCloudsIntensity;
    float CirrusCloudsAmount;
    glm::vec3 CirrusCloudsColor;
    bool CirrusCloudsEnabled;
};

struct SSAOSettings
{
    bool Enabled;
    float Radius;
    float Bias;
    float Intensity;
    float DirectLightInfluence;
};

struct FogLayer
{
    glm::fvec3 Albedo;
    float Density0;
    float Density1;
    float Height0;
    float Height1;
    float NoiseCoverage;
    glm::fvec3 NoiseFrequency;
    glm::fvec3 NoiseRotation;
    glm::fvec3 NoiseWind;
    bool Enabled;
};

struct Fog
{
    float Phase;
    float RenderDistance;
    FogLayer FogLayer0;
    FogLayer FogLayer1;
};

struct DirLight
{
    glm::vec3 SunColor;
    float Kelvin;
    float SunIntensity;
    bool UseTemperature;
    glm::vec3 ColorTemperatureAdjustment;
    glm::vec3 ColorAdjustedForIntensity;
    glm::vec3 RotationAsVec3;
    float Yaw;
    float Pitch;
    bool ShadowEnabled;
    float ShadowFarPlane;
    float ShadowNearPlane;
    float ShadowFade;
    float ShadowBias;
    float LightSize;
    float LightDistance;
    float CascadeSpeed;
    uint8_t CascadeCount;
    uint8_t ShadowObscurity;
    uint8_t SunlightObscurity;
    CoverageSettings CoverageSettings;
    float LocalCoverageScalar;
    bool LocalCoverageEnabled;
    float ScatteringIntensityScale;
};

struct Lighting
{
    [[bg3::hidden]] void* VMT;
    FixedString ParentGUID;
    DirLight Sun;
    MoonLight Moon;
    SkyLight SkyLight;
    SSAOSettings SSAOSettings;
    VolumetricCloudSettings VolumetricCloudSettings;
    Fog Fog;
    bg3se::Fog TimelineFog;
    bool TimelineFogOverride;
    glm::vec3 LinearClearColor;
    bool LinearClearColorOverride;
};

END_SE()
