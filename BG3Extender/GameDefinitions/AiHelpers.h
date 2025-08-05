#pragma once

BEGIN_NS(esv)

struct ResourceBoost
{
    Guid ActionResource;
    int Level{ 0 };
    double Amount{ .0f };
    BoostType Type;
    bool field_21{ false };
};

struct AiDamageAmount
{
    float DamageAmount{ .0f };
    float HealAmount{ .0f };
    float DoTAmount{ .0f };
    float HoTAmount{ .0f };
    float ControlAmount{ .0f };
    float BoostAmount{ .0f };
    float PositionScore{ .0f };
    float BreakConcentrationAmount{ .0f };
};

struct AiScoreInternal : public ProtectedGameObject<AiScoreInternal>
{
    uint32_t ReasonFlags;
    uint16_t Flags1;
    LegacyRefMap<EntityHandle, AiDamageAmount> DamageAmounts;
    float FutureScore;
    float FailScore;
    Array<ResourceBoost> ResourceBoosts;
    uint8_t word30;
    uint8_t field_31;
    // Editor only
    // ScratchString ScoreLog;
    uint32_t RefCount;
};

struct AiScore
{
    AiScoreInternal* Score;
};

struct AIActiveAura
{
    EntityHandle Status;
    FixedString StatusId;
    stats::Functors* Functors{ nullptr };
    glm::vec3 Position{ .0f };
    float Radius{ .0f };
    Character* StatusOwner{ nullptr };
    LegacyRefMap<Character*, AiScore> EnterScores;
    LegacyRefMap<Character*, AiScore> Scores;
};

struct PlannedActionWithMoveSpell : public SpellId
{
    uint64_t field_40{ 0 };
};

struct AiTurnData
{
    bool field_0{ false };
    bool CanReachTarget{ false };
    bool IsExtraAction{ false };
    std::optional<PlannedActionWithMoveSpell> PlannedAction;
};

struct InAuraInteraction
{
    EntityHandle Target;
    EntityHandle Aura;
};

struct EnterAuraInteraction
{
    EntityHandle Target;
    EntityHandle Aura;
};

struct BreakConcentrationChance
{
    EntityHandle field_0;
    int field_8;
    int field_C;
};

struct BoostAction
{
    BoostParameters Params;
    EntityHandle Target;
    Guid Prototype;
    bool field_30;
};

struct ItemAction
{
    EntityHandle Item;
};

struct SurfaceInteraction
{
    [[bg3::hidden]] Surface* Surface;
    uint8_t SurfaceTransform;
    std::array<SurfaceType, 2> SurfaceType;
};

struct SurfaceOnObjectAction
{
    EntityHandle Target;
    int SurfaceType;
    bool HasLifetime;
};

struct StatusInteraction
{
    EntityHandle Target;
    FixedString StatusId;
    bool HasLifetime;
};

struct AiSpellData
{
    SpellId Spell;
    EntityHandle Item;
    std::optional<AiActionConditions> Conditions;
};

struct AiKnownState
{
    Array<StatusInteraction> StatusInteractions;
    Array<SurfaceOnObjectAction> SurfaceOnObjectActions;
    Array<SurfaceInteraction> SurfaceInteractions;
    Array<ItemAction> ItemActions;
    Array<ItemAction> ItemActions2;
    Array<BoostAction> BoostActions;
    Array<BreakConcentrationChance> BreakConcentrationChance;
    Array<EnterAuraInteraction> EnterAuraInteractions;
    Array<InAuraInteraction> InAuraInteractions;
};

struct CombatAiAction
{
    Guid ActionGuid;
    AiActionType Type;
    SpellId Spell;
    EntityHandle Item;
    std::optional<AiActionConditions> Conditions;
    SpellId MoveSpell;
    EntityHandle MoveItem;
    bool AvoidAoO;
    bool IsExtraAction;
    AiActionType ExtraActionType;
    bool IncludeResourceBoosts;
    EntityHandle Target;
    glm::vec3 TargetPos;
    EntityHandle Target2;
    glm::vec3 TargetPos2;
    glm::vec3 CastPosition;
    glm::vec3 PreferredPosition;
    glm::vec3 EndPosition;
    float MinRange;
    float PreferredRange;
    float MaxRange;
    uint32_t dword16C;
    EntityHandle Target3;
    float FinalScore;
    float ScoreValue2;
    AiScore Score;
    AiScore ScoreWithoutMovement;
    EntityHandle PreferredTarget;
    float ActionFinalScore;
    float PositionFinalScore;
    float MovementFinalScore;
    Array<stats::ActionResourceCost> ActionCosts;
    Array<stats::ActionResourceCost> MoveCosts;
    Array<stats::ActionResourceCost> MoveCosts2;
    AiActionFlags Flags;
    uint8_t Targeting; // ESpellTargetingState
    float DistanceSquared;
    float Distance;
    float ScoreMultiplier;
    float ScoreMultiplier2;
    std::optional<glm::vec3> field_1F0;
    double field_200;
    // Editor only
    // Array<CombatAiDebugPath> field_208;
};

struct AiItemData
{
    EntityHandle Item;
    std::optional<AiActionConditions> Conditions;
};

struct AiData
{
    bool IsCalculating;
    TrackedCompactSet<CombatAiAction> Actions;
    uint32_t NextAction;
    uint64_t ActionCount;
    bool CalculationFinished;
    bool CanMove;
    bool CheckAiHint;
    bool HasNewActions;
    bool FallbackScoreCalculated;
    bool EvaluateMovement;
    bool DontSeek;
    uint8_t field_2F;
    int NextSpellToEvaluate;
    int NextFallbackSpellToEvaluate;
    int NextSeekToEvaluate;
    int NextItemToEvaluate;
    int NextActionResourcesToEvaluate;
    int NextMovementToEvaluate;
    int field_48;
    int NextMovementToEvaluate2;
    int NextMovementToEvaluate3;
    float MovementScore;
    int32_t PathId;
    Array<AiSpellData> Spells;
    Array<AiSpellData> MovementSpells;
    Array<AiSpellData> FallbackSpells;
    Array<AiSpellData> SeekOnlySpells;
    Array<AiItemData> Items;
    Array<AiItemData> SeekOnlyItems;
    Array<EntityHandle> SeekEnemies;
    EntityHandle NavcloudPathRequest;
    HashMap<AiTilePos, float> InterestingTiles;
    HashMap<AiTilePos, float> InterestingMovementTiles;
    HashMap<AiTilePos, float> FallbackTiles;
    // Editor only
    // MultiHashMap TimesliceProfile_ETimeSliceState_PerformanceTiming;
    // MultiHashMap SpellProfile_FS_PerformanceTiming;
    // MultiHashMap field_218_FS_PerformanceTiming;
    int PathfindingRequests;
    int FramesToPathfind;
    int FramesToCalculate;
    int field_264;
    std::optional<uint64_t> TimeSinceStart;
    std::optional<uint64_t> TimeToCalculate;
    float TimeRemaining;
    HashSet<SpellId> FailedSpellEvaluations;
    EntityHandle Subject;
    uint8_t Timeslice;
};

struct AiFallbackData
{
    uint32_t field_0;
};

struct AiUsefulItem
{
    EntityHandle Item;
    bool FromLevel;
};

struct InAttackRangeChangedSource
{
    EntityHandle Entity;
    EntityHandle ProxyEntity;
    glm::vec3 Position;
    float MinVerticalBound;
    float MaxVerticalBound;
    float SourceRadius;
    float TargetCeiling;
    float TargetFloor;
};

struct OverridePos
{
    glm::vec3 Original;
    glm::vec3 Override;
};

struct AiHelpers : public BaseSystem
{
    DEFINE_SYSTEM(ServerAi, "esv::AiHelpers")

    [[bg3::hidden]] void* GameEventListenerVMT;
    [[bg3::hidden]] void* TrajectoryRules;
    [[bg3::hidden]] void* SpellPrototypeManager;
    [[bg3::hidden]] void* DifficultyClassManager;
    [[bg3::hidden]] void* InterruptPrototypeManager;
    [[bg3::hidden]] void* GlobalTemplateManager;
    [[bg3::hidden]] void* FactionContainer;
    [[bg3::hidden]] void* GlobalSwitches;
    [[bg3::hidden]] void* ThothMachine;
    bool field_58;
    HashMap<EntityHandle, AiData*> Ai;
    HashMap<EntityHandle, AiTurnData> Turns;
    HashMap<EntityHandle, AiFallbackData> Fallback;
    Array<EntityHandle> CharacterAiRequests;
    Array<EntityHandle> ProcessingCharacterAiRequests;
    int field_140;
    Character* CurrentCharacter;
    [[bg3::hidden]] AiGrid* CurrentAiGrid;
    [[bg3::hidden]] Level* CurrentLevel;
    uint64_t CurrentModifiers;
    Array<uint16_t> AiFlags;
    uint16_t CurrentAiFlags;
    uint64_t qword180;
    Array<AiKnownState*> KnownStates;
    AiKnownState* KnownState;
    TrackedCompactSet<FixedString> field_1A0;
    [[bg3::hidden]] Array<void*> Surfaces;
    Array<Character*> Enemies;
    Array<Character*> Allies;
    Array<Character*> Neutrals;
    HashSet<EntityHandle> NavcloudPathRequests;
    Array<EntityHandle> CombatCharacterEnities;
    Array<Character*> CombatCharacters;
    Array<Character*> CombatEnemies;
    Array<Character*> CombatHiddenEnemies;
    Array<Character*> CombatAllies;
    Array<Character*> CombatNeutrals;
    Array<AiUsefulItem> UsefulItems;
    Array<EntityHandle> ThrowableItems;
    std::optional<EntityHandle> field_2A0;
    Array<AIActiveAura> ActiveAuras;
    Array<InAttackRangeChangedSource> InAttackRangeChangedSources;
    [[bg3::hidden]] Array<void*> JumpAttackRangeLeftScores; // AICachedJumpAttackRangeLeftScore
    int CombatLowestHealth;
    int CombatHighestHealth;
    int field_2E8;
    int field_2EC;
    float MaxThreatRadius;
    bool field_2F8;
    glm::vec3 Position;
    LegacyRefMap<EntityHandle, Array<OverridePos>> OverrideTranslates;
    LegacyRefMap<EntityHandle, OverridePos> OverrideTranslate;
    [[bg3::hidden]] LegacyRefMap<void*, void*> ComputedCharacterDamage;
    [[bg3::hidden]] LegacyRefMap<void*, void*> ComputedItemDamage;
    [[bg3::hidden]] LegacyRefMap<void*, void*> ComputedCharacterHeal;
    [[bg3::hidden]] LegacyRefMap<void*, void*> ComputedDestroyItem;
    [[bg3::hidden]] LegacyRefMap<void*, void*> ComputedDamageItem2;
    [[bg3::hidden]] LegacyRefMap<void*, void*> ComputedFunctors;
    [[bg3::hidden]] LegacyRefMap<void*, void*> ComputedStatus;
    [[bg3::hidden]] LegacyRefMap<void*, void*> ComputedSurfaces;
    [[bg3::hidden]] LegacyRefMap<void*, void*> ComputedBoosts;
    [[bg3::hidden]] LegacyRefMap<void*, void*> ComputedConcentration;
    [[bg3::hidden]] LegacyRefMap<void*, void*> ComputedAverageDamage;
    [[bg3::hidden]] HashMap<void*, void*> ComputedConditionChances_int_Arr_pair_ConditionChanceCacheData_float;
    [[bg3::hidden]] LegacyRefMap<void*, void*> SurfaceHits;
};


END_NS()
