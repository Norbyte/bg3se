#pragma once

BEGIN_NS(ecl)

struct ProvidedMultiEffectObject
{
    EntityHandle Entity;
    glm::vec3 Position;
    Array<Guid> Tags;
    ProvidedMultiEffectObjectFlags Flags{ 0 };
};


struct EffectHandlerPlayTime
{
    uint64_t Phase;
    float Time;
};

struct EffectHandlerInitInfo
{
    EntityHandle EventProviderEntity;
    EntityHandle Player;
    ProvidedMultiEffectObject SourceEffect;
    ProvidedMultiEffectObject TargetEffect;
    Array<EffectHandlerPlayTime> PlayTime;
    STDString MultiEffectDefinition;
    FixedString WeaponBones;
    float FXScale{ 1.0f };
    float ZoneRange{ 0.0f };
    DamageType DamageType{ DamageType::None };
    uint8_t VerbalIntent{ 0 };
    bool EnteredDuringSave{ false };
    bool Reset{ false };
};


struct EffectEntitySlot
{
    FixedString AttachmentName;
    FixedString BoneName;
};


struct ConditionalEffect
{
    int EffectIndex{ 0 };
    EntityHandle Effect;
    FixedString SourceBone;
    FixedString TargetBone;
    EffectEntitySlot EntitySlot;
    bool Attached{ false };
    bool TextKeyEventStarted{ false };
    bool CanCreate{ false };
    DamageType DamageType{ DamageType::None };
    uint8_t VerbalIntent{ 0 };
};


struct EffectHandler : public ProtectedGameObject<EffectHandler>
{
    [[bg3::hidden]] void* VMT;
    [[bg3::hidden]] void* TextKeyEventManager{ nullptr };
    [[bg3::hidden]] void* EffectManager{ nullptr };
    [[bg3::hidden]] ResourceManager* ResourceManager{ nullptr };
    EffectHandlerInitInfo InitInfo;
    [[bg3::hidden]] Scene* Scene{ nullptr };
    EntityHandle EventProviderEntity;
    resource::MultiEffectInfo MultiEffectInfo;
    bool IsActive{ false };
    Array<ConditionalEffect> ConditionalEffects;
};

END_NS()

