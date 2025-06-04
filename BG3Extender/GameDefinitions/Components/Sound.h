#pragma once

BEGIN_SE()

struct SoundEvent
{
};

struct SoundComponentActiveData
{
    uint64_t SoundObjectId;
    EntityHandle Entity;
    [[bg3::hidden]] void* field_10;
    LegacyRefMap<FixedString, FixedString> SoundSwitches;
};

struct SoundComponent : public BaseComponent
{
    DEFINE_COMPONENT(Sound, "ls::SoundComponent")

    EntityHandle Entity;
    SoundComponentActiveData* ActiveData;
    uint32_t Flags;
    float Duration;
    FixedString Effect;
    float field_1C;
};

struct SoundExternalPath
{
    STDString Path;
    AudioCodec Codec{ AudioCodec::Vorbis };
};

struct SoundPostEventRequest
{
    std::variant<EntityHandle, uint64_t> Subject;
    SoundEventType Type{ SoundEventType::Sound };
    int field_14{ 0 };
    float Seek{ -1.0f };
    std::variant<FixedString, SoundEvent*, STDString> Event;
    std::variant<FixedString, SoundExternalPath> Path;
    [[bg3::hidden]] void* Callback{ nullptr };
    uint8_t PlayerIndex{ 0 };
};

struct SoundSetRTPCRequest
{
    EntityHandle Entity;
    SoundEventType Type{ SoundEventType::Sound };
    std::variant<FixedString, STDString> RTPC;
    float Value{ 0.0f };
    uint8_t PlayerIndex{ 0 };
    bool Reset{ false };
    uint8_t field_36{ 0 };
};

struct SoundSetSwitchRequest
{
    std::variant<EntityHandle, uint64_t> Subject;
    FixedString Switch;
    FixedString Value;
};

struct SoundSeekRequest
{
    EntityHandle Entity;
    SoundEventType Type{ SoundEventType::Sound };;
    std::variant<FixedString, SoundEvent*, STDString> Event;
    float Seek{ -1.0f };
    uint8_t PlayerIndex{ 0 };
    int field_38{ 0 };
};

struct SoundRoutingSystem : public BaseSystem
{
    DEFINE_SYSTEM(SoundRouting, "ls::SoundRoutingSystem")

    Array<SoundPostEventRequest> PostEvent;
    Array<SoundSetSwitchRequest> SetSwitch;
    Array<SoundSetRTPCRequest> SetRTPC;
    Array<SoundSeekRequest> Seek;
    [[bg3::hidden]] void* SoundRoutingHelper;
    [[bg3::hidden]] void* ResourceManager;
};


END_SE()
