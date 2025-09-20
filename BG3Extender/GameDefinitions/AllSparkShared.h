#pragma once

BEGIN_SE()

struct RenderPropertyList
{
    uint32_t RenderPasses;
    uint8_t OpaqueChannel;
    uint8_t AlphaChannel;
    uint8_t TransformType;
};

END_SE()

BEGIN_NS(aspk)

struct QuestionHoldAutomationSettings
{
    QuestionHoldAutomationSettings() = delete;

    [[bg3::hidden]] void* VMT;
    bool IsEnabled;
    float CycleSpeed;
    float CycleSpeedDeviation;
    float StartOffset;
    float StartOffsetDeviation;
};


struct TimelinePhase
{
    TimelinePhase() = delete;

    [[bg3::hidden]] void* VMT;
    float Duration;
    float EndTime;
    int PlayCount;
    int field_14;
    Guid DialogNodeId;
    bool IsOverridingTimelineQuestionHoldAutomationSettings;
    QuestionHoldAutomationSettings QuestionHoldAutomation;
};


struct TimelineHeader : public ProtectedGameObject<TimelineHeader>
{
    float Duration;
    LegacyArray<TimelinePhase> Phases;
};

END_NS()
