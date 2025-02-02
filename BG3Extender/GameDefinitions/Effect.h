#pragma once

BEGIN_NS(ecl)

struct ProvidedMultiEffectObject
{
    EntityHandle Entity;
    glm::vec3 Position;
    Array<Guid> Tags;
    uint32_t Flags;
};


struct EffectHandlerInitUnknown
{
    uint64_t field_0;
    float field_8;
};

struct EffectHandlerInitInfo
{
    EntityHandle EventProviderEntity;
    EntityHandle Player;
    ProvidedMultiEffectObject SourceEffect;
    ProvidedMultiEffectObject TargetEffect;
    Array<EffectHandlerInitUnknown> field_70;
    STDString MultiEffectDefinition;
    FixedString field_90;
    float FXScale;
    float ZoneRange;
    uint8_t field_9C;
    bool EnteredDuringSave;
};


struct EffectEntitySlot
{
    FixedString AttachmentName;
    FixedString BoneName;
};


struct ConditionalEffect
{
    int EffectIndex;
    EntityHandle Effect;
    FixedString SourceBone;
    FixedString TargetBone;
    EffectEntitySlot EntitySlot;
    bool Attached;
    bool TextKeyEventStarted;
    bool CanCreate;
    DamageType DamageType;
    uint8_t VerbalIntent;
};


struct EffectHandler : public ProtectedGameObject<EffectHandler>
{
    [[bg3::hidden]] void* VMT;
    [[bg3::hidden]] void* TextKeyEventManager;
    [[bg3::hidden]] void* EffectManager;
    [[bg3::hidden]] ResourceManager* ResourceManager;
    EffectHandlerInitInfo InitInfo;
    [[bg3::hidden]] void* Scene;
    EntityHandle EventProviderEntity;
    resource::MultiEffectInfo MultiEffectInfo;
    bool IsActive;
    Array<ConditionalEffect> ConditionalEffects;
};

END_NS()

