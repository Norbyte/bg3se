#pragma once

BEGIN_NS(thoth::shared)

struct ConditionData
{
    uint32_t RefCount;
    Guid Id;
    STDString Script;
};

struct ConditionMap
{
    SparseArray<uint16_t> SlotRemaps;
    Array<stats::ConditionId> Handles;
    Array<ConditionData> Conditions;
};

struct ConditionManager : public ProtectedGameObject<ConditionManager>
{
    PagedArray<stats::ConditionId> Handles;
    uint16_t FirstReuseIndex;
    uint16_t LastReuseIndex;
    uint32_t ReuseListSize;
    HashMap<Guid, stats::ConditionId> DynamicConditions;
    ConditionMap Conditions;
    Array<stats::ConditionId> StaticConditions;
};

END_NS()
