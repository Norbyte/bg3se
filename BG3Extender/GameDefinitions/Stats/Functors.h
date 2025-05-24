#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Stats/Common.h>
#include <GameDefinitions/Interrupt.h>
#include <GameDefinitions/Hit.h>
#include <GameDefinitions/Misc.h>
#include <Lua/LuaHelpers.h>

BEGIN_NS(stats)

struct ExportedConditionalRoll
{
    RollType Type;
    stats::ConditionId Conditions;
};

struct Functor
{
    virtual ~Functor();
    virtual bool ParseParams(std::span<StringView>& params, STDString& key, PropertyContext context);
    virtual Functor* Clone() const;

    FixedString UniqueName;
    Guid FunctorUuid;
    Array<ExportedConditionalRoll> RollConditions;
    stats::ConditionId StatsConditions;
    PropertyContext PropertyContext{ 0 };
    int32_t StoryActionId{ -1 };
    ObserverType ObserverType{ ObserverType::None };
    FunctorId TypeId{ FunctorId::CustomDescription };
    FunctorFlags Flags{ 0 };

    inline FixedString const& GetElementName() const
    {
        return UniqueName;
    }

    template <class T>
    std::optional<T*> Cast()
    {
        if (TypeId == T::FunctorType) {
            return static_cast<T*>(this);
        } else {
            return {};
        }
    }
};


struct ContextData
{
    virtual ~ContextData();

    [[bg3::readonly]] FunctorContextType Type{ 0 };
    int32_t StoryActionId{ -1 };
    PropertyContext PropertyContext{ 0 };
    ActionOriginator Originator;
    [[bg3::hidden]] resource::GuidResourceBankBase* ClassResources{ nullptr };
    EntityHandle HistoryEntity;
    EntityHandle StatusSource;
    HashMap<EntityHandle, int32_t> EntityToThothContextIndex;
    int field_98{ 0 };
    uint8_t ConditionCategory{ 0 };
};

struct AttackTargetContextData : public ContextData
{
    static constexpr auto ContextType = FunctorContextType::AttackTarget;

    virtual ~AttackTargetContextData() override;

    ecs::EntityRef Caster;
    ecs::EntityRef CasterProxy;
    ecs::EntityRef Target;
    ecs::EntityRef TargetProxy;
    glm::vec3 Position;
    bool IsFromItem{ false };
    SpellIdWithPrototype SpellId;
    HitDesc Hit;
    AttackDesc Attack;
    float SomeRadius{ 2.4f };
    HitWith HitWith{ HitWith::None };
    uint32_t StatusEvent{ 0 };
    FixedString StatusId;
    FixedString field_26C;
    uint8_t StatusExitCause{ 3 };
    uint8_t field_315{ 0 };
    uint8_t field_316{ 19 };
};

struct AttackPositionContextData : public ContextData
{
    static constexpr auto ContextType = FunctorContextType::AttackPosition;

    virtual ~AttackPositionContextData() override;

    ecs::EntityRef Caster;
    glm::vec3 Position;
    float HitRadius{ -1.0f };
    SpellIdWithPrototype SpellId;
    HitDesc Hit;
    AttackDesc Attack;
    float SurfaceChangeHeight{ 2.4f };
    bool IsFromItem{ false };
};

struct MoveContextData : public ContextData
{
    static constexpr auto ContextType = FunctorContextType::Move;

    virtual ~MoveContextData() override;

    ecs::EntityRef Caster;
    ecs::EntityRef Target;
    ecs::EntityRef Source;
    glm::vec3 Position;
    float Distance{ 0.0f };
};

struct TargetContextData : public ContextData
{
    static constexpr auto ContextType = FunctorContextType::Target;

    virtual ~TargetContextData() override;

    ecs::EntityRef Source;
    ecs::EntityRef SourceProxy;
    glm::vec3 Position;
    uint8_t StatusExitCause{ 3 };
    uint8_t field_C5{ 0 };
    uint8_t field_C6{ 19 };
    SpellIdWithPrototype SpellId;
    HitDesc Hit;
    AttackDesc Attack;
    uint32_t StatusEvent{ 0 };
    FixedString StatusId;
    bool IsFromItem{ false };
};

struct NearbyAttackedContextData : public ContextData
{
    static constexpr auto ContextType = FunctorContextType::NearbyAttacked;

    virtual ~NearbyAttackedContextData() override;

    ecs::EntityRef OriginalSource;
    ecs::EntityRef Source;
    ecs::EntityRef SourceProxy;
    ecs::EntityRef Target;
    ecs::EntityRef TargetProxy;
    glm::vec3 Position;
    bool IsFromItem{ false };
    SpellIdWithPrototype SpellId;
    HitDesc Hit;
    AttackDesc Attack;
    float SurfaceChangeHeight{ 2.4f };
    uint8_t field_314{ 2 };
};

struct NearbyAttackingContextData : public NearbyAttackedContextData
{
    static constexpr auto ContextType = FunctorContextType::NearbyAttacking;

    virtual ~NearbyAttackingContextData() override;
};

struct EquipContextData : public ContextData
{
    static constexpr auto ContextType = FunctorContextType::Equip;

    virtual ~EquipContextData() override;

    ecs::EntityRef Caster;
    ecs::EntityRef Target;
    bool UseCasterStats{ false };
};

struct SourceContextData : public ContextData
{
    static constexpr auto ContextType = FunctorContextType::Source;

    virtual ~SourceContextData() override;

    ecs::EntityRef Source;
    ecs::EntityRef SourceProxy;
};

struct InterruptContextData : public ContextData
{
    static constexpr auto ContextType = FunctorContextType::Interrupt;

    virtual ~InterruptContextData() override;

    bool OnlyAllowRollAdjustments{ false };
    ecs::EntityRef Source;
    ecs::EntityRef SourceProxy;
    ecs::EntityRef Target;
    ecs::EntityRef TargetProxy;
    ecs::EntityRef Observer;
    ecs::EntityRef ObserverProxy;
    std::optional<interrupt::RollAdjustments> RollAdjustments;
    interrupt::InterruptEvent Interrupt;
    HitDesc Hit;
    AttackDesc Attack;
    uint8_t field_428{ 0 };
    interrupt::AppliedChange Changes;
};


template <class TContext>
using ExecuteFunctorProc = void(HitResult* hit, Functors* self, TContext* params);

using ExecuteInterruptFunctorProc = void (HitResult* hit, ecs::EntityWorld* world, Functors* self, InterruptContextData* params);

struct Functors : public stats::CNamedElementManager<Functor>
{
    FixedString UniqueName;

    // Lua getter helpers
    Array<Functor*>* LuaGetFunctorList() const;
    //# P_GETTER(FunctorList, LuaGetFunctorList)
};


// FIXME - use custom implementation of base VMT or use base VMT?
struct StatsFunctorSetImpl : public Functors
{
    /*~StatsFunctorSetImpl() override;
    void ClearNextIndex() override;
    void Clear() override;
    void AddOrUpdate(Functor*) override;
    Functor* GetByIndex(int64_t) override;
    Functor* GetByName(FixedString const&) override;
    void Unknown_30() override;
    int64_t GetSize() override;
    Functor* GetByIndex2(int64_t) override;
    Functor* GetByIndex3(int64_t) override;
    void UpdateNameMap() override;*/
};

#define DEFN_FUNCTOR(ty) \
    static constexpr auto FunctorType = FunctorId::ty; \
    ~ty##Functor() override; \
    Functor* Clone() const override;
    

struct CustomDescriptionFunctor : public Functor
{
    DEFN_FUNCTOR(CustomDescription)

    FixedString Description; // Arg0
};

struct ApplyStatusFunctor : public Functor
{
    DEFN_FUNCTOR(ApplyStatus)

    FixedString StatusId; // Arg1
    FixedString StatusSpecificParam1; // Arg4
    STDString StatusConditions; // Arg7
    stats::ConditionId Conditions; // Arg7
    int StatusSpecificParam2{ -1 }; // Arg5
    int StatusSpecificParam3{ -1 }; // Arg6
    bool RequiresConcentration{ false };
    [[bg3::hidden]] StatsExpressionParam* DurationLuaExpression{ nullptr }; // Arg3
    [[bg3::legacy(HasParam6)]] bool KeepAlive{ false };
};

struct SurfaceChangeFunctor : public Functor
{
    DEFN_FUNCTOR(SurfaceChange)

    float Chance{ 1.0f };
    [[bg3::legacy(field_24)]] float DetourMaxWaitTime{ 0.0f };
    float field_28{ 0.0f };    
    [[bg3::legacy(field_2C)]] float Radius{ -1.0f };
    SurfaceChange SurfaceChange{ SurfaceChange::None };
};

struct ResurrectFunctor : public Functor
{
    DEFN_FUNCTOR(Resurrect)

    float Probability{ 1.0f }; // Arg0
    float HealthPercentage{ 1.0f }; // Arg1
    TargetTypeFlags HealingType{ 0 };
};

struct SabotageFunctor : public Functor
{
    DEFN_FUNCTOR(Sabotage)

    int Amount{ 1 };
};

struct SummonFunctor : public Functor
{
    DEFN_FUNCTOR(Summon)

    FixedString Template; // Arg1
    [[bg3::legacy(AIHelper)]] FixedString AiSpellOverride; // Arg 3
    std::variant<SummonLifetimeType, float> SpawnLifetime;
    HashSet<FixedString> StatusesToApply;
    FixedString StackId;
    [[bg3::legacy(Arg5)]] bool ExtendExistingConcentration;
    [[bg3::legacy(Arg9)]] bool LateJoinPenalty;
    bool UseOwnerPassives;
};

struct ForceFunctor : public Functor
{
    DEFN_FUNCTOR(Force)

    FixedString Distance; // Arg0
    ForceFunctorOrigin Origin{ ForceFunctorOrigin::OriginToEntity }; // Arg1
    ForceFunctorAggression Aggression{ ForceFunctorAggression::Aggressive }; // Arg2
    StatsExpressionParamEx* DistanceExpression{ nullptr };
    bool ControlArc{ false };
    bool PullToOrigin{ false };
};

struct DouseFunctor : public Functor
{
    DEFN_FUNCTOR(Douse)

    [[bg3::legacy(field_20)]] float Radius{ -1.0f };
    float field_24{ 1.0f };
};

struct SwapPlacesFunctor : public Functor
{
    DEFN_FUNCTOR(SwapPlaces)

    FixedString Animation; // Arg0
    bool Arg1{ false };
    bool Arg2{ false };
};

struct PickupFunctor : public Functor
{
    DEFN_FUNCTOR(Pickup)

    [[bg3::legacy(Arg0)]] FixedString Effect;
};

struct CreateSurfaceFunctor : public Functor
{
    DEFN_FUNCTOR(CreateSurface)

    FixedString SurfaceType; // Arg2 - SurfaceType or DamageType
    float Radius{ -1.0f }; // Arg0
    float Arg4{ 0.0f };
    float Duration{ -1.0f }; //Arg1
    bool IsControlledByConcentration{ true }; // Arg3
    [[bg3::legacy(Arg5)]] bool ExtendExistingConcentration{ false };
};

struct CreateConeSurfaceFunctor : public Functor
{
    DEFN_FUNCTOR(CreateConeSurface)

    FixedString SurfaceType;
    float Radius{ -1.0f };
    float Duration{ -1.0f };
    bool IsControlledByConcentration{ false };
    bool field_55{ false };
};

struct RemoveStatusFunctor : public Functor
{
    DEFN_FUNCTOR(RemoveStatus)

    FixedString StatusId; // Arg0
};

using StatsSystem_ThrowDamageEventProc = void (void* statsSystem, void* temp5, HitDesc* hit, AttackDesc* attack, bool a5, bool a6);

struct DealDamageFunctor : public Functor
{
    DEFN_FUNCTOR(DealDamage)

    using ApplyDamageProc = HitResult * (HitResult* result, DealDamageFunctor* functor, ecs::EntityRef* casterHandle,
        ecs::EntityRef* targetHandle, glm::vec3* position, bool isFromItem, SpellIdWithPrototype* spellId, 
        int storyActionId, ActionOriginator* originator, resource::GuidResourceBankBase* classResourceMgr, 
        HitDesc* hit, AttackDesc* attack, EntityHandle* sourceHandle2, HitWith hitWith, int conditionRollIndex,
        bool entityDamagedEventParam, __int64 a17, SpellId* spellId2);

    DamageType DamageType{ DamageType::None }; // Arg2
    DealDamageWeaponType WeaponType{ DealDamageWeaponType::None }; // Arg1
    DealDamageWeaponDamageType WeaponDamageType{ DealDamageWeaponDamageType::None }; // Arg2
    StatsExpressionParam* Damage{ nullptr }; // Arg1
    int CoinMultiplier{ 0 };
    bool Nonlethal{ false }; // Arg4
    bool Magical{ false }; // Arg3
    bool IgnoreDamageBonus{ false };
    bool IgnoreEvents{ false };
    bool ConsumeCoin{ false };
    bool IgnoreImmunities{ false };
};

struct ExecuteWeaponFunctorsFunctor : public Functor
{
    DEFN_FUNCTOR(ExecuteWeaponFunctors)

    ExecuteWeaponFunctorsType WeaponType;
};

struct RegainHitPointsFunctor : public Functor
{
    DEFN_FUNCTOR(RegainHitPoints)

    StatsExpressionParam* HitPoints{ nullptr };
    TargetTypeFlags HealingType{ TargetTypeFlags::Living };
};

struct TeleportSourceFunctor : public Functor
{
    DEFN_FUNCTOR(TeleportSource)

    [[bg3::legacy(Arg1)]] bool FindValidPosition{ false };
    [[bg3::legacy(Arg2)]] bool RotateTowardsTarget{ false };
};

struct SetStatusDurationFunctor : public Functor
{
    DEFN_FUNCTOR(SetStatusDuration)

    FixedString StatusId; // Arg0
    float Duration{ 6.0f }; // Arg1
    SetStatusDurationType ChangeType{ SetStatusDurationType::ForceSet }; // Arg2
};

struct UseSpellFunctor : public Functor
{
    DEFN_FUNCTOR(UseSpell)

    FixedString SpellId; // Arg0
    bool IgnoreHasSpell{ false }; // Arg1
    bool IgnoreChecks{ false }; // Arg2
    bool Arg4{ false };
    Guid SpellCastGuid;
    [[bg3::legacy(Arg6)]] bool IgnoreBlindness{ false };
};

struct UseActionResourceFunctor : public Functor
{
    DEFN_FUNCTOR(UseActionResource)

    Guid ActionResource; // Arg0
    double Amount{ 0.0 }; // Arg1
    int Level{ 0 }; // Arg2
    bool IsPercentage{ false }; // Arg1
    bool IgnoreResourceConsumeBoosts{ false }; // Arg3
};

struct UseAttackFunctor : public Functor
{
    DEFN_FUNCTOR(UseAttack)

    bool IgnoreChecks{ false }; // Arg1
    bool IgnoreBlindness{ false }; // Arg2
};

struct CreateExplosionFunctor : public Functor
{
    DEFN_FUNCTOR(CreateExplosion)

    FixedString SpellId; // Arg0
};

struct BreakConcentrationFunctor : public Functor
{
    DEFN_FUNCTOR(BreakConcentration)
};

struct ApplyEquipmentStatusFunctor : public ApplyStatusFunctor
{
    DEFN_FUNCTOR(ApplyEquipmentStatus)

    StatsItemSlot EquipmentSlot{ StatsItemSlot::MainHand };
};

struct RestoreResourceFunctor : public Functor
{
    DEFN_FUNCTOR(RestoreResource)

    Guid ActionResource; // Arg0
    [[bg3::legacy(Hex)]] int Level{ 0 }; // Arg2
    StatsExpressionParam* LuaAmount{ nullptr }; // Arg1
    double Amount{ 0.0 }; // Arg1
    RestoreResourceAmountType AmountType{ RestoreResourceAmountType::None };
};

struct SpawnFunctor : public Functor
{
    DEFN_FUNCTOR(Spawn)

    FixedString TemplateId; // Arg0
    FixedString AIHelper;
    HashSet<FixedString> StatusesToApply;
    [[bg3::legacy(Arg7)]] bool ApplyLateJoinPenalty{ false };
};

struct StabilizeFunctor : public Functor
{
    DEFN_FUNCTOR(Stabilize)
};

struct UnlockFunctor : public Functor
{
    DEFN_FUNCTOR(Unlock)
};

struct ResetCombatTurnFunctor : public Functor
{
    DEFN_FUNCTOR(ResetCombatTurn)
};

struct RemoveAuraByChildStatusFunctor : public Functor
{
    DEFN_FUNCTOR(RemoveAuraByChildStatus)

    FixedString StatusId; // Arg0
};

struct SummonInInventoryFunctor : public Functor
{
    DEFN_FUNCTOR(SummonInInventory)

    FixedString TemplateId;
    [[bg3::legacy(Arg8)]] FixedString AiSpellOverride;
    std::variant<SummonLifetimeType, float> Lifetime;
    [[bg3::legacy(AdditionalArgs)]] HashSet<FixedString> StatusesToApply;
    [[bg3::legacy(Arg9)]] FixedString SpellContainer;
    int32_t Amount{ 0 };
    bool PlaceInInventory{ false };
    bool Equip{ false };
    [[bg3::legacy(Arg6)]] bool AllowOffHand{ false };
    [[bg3::legacy(Arg7)]] bool ExtendExistingConcentration{ false };
};

struct SpawnInInventoryFunctor : public Functor
{
    DEFN_FUNCTOR(SpawnInInventory)

    FixedString TemplateId;
    [[bg3::legacy(Arg7)]] FixedString AiSpellOverride;
    [[bg3::legacy(Arg3)]] int32_t Amount{ 0 };
    [[bg3::legacy(Arg4)]] bool PlaceInInventory{ false };
    [[bg3::legacy(Arg5)]] bool Equip{ false };
    [[bg3::legacy(Arg6)]] bool AllowOffHand{ false };
    [[bg3::legacy(AdditionalArgs)]] HashSet<FixedString> StatusesToApply;
};

struct RemoveUniqueStatusFunctor : public Functor
{
    DEFN_FUNCTOR(RemoveUniqueStatus)

    FixedString StatusId; // Arg0
};

struct DisarmWeaponFunctor : public Functor
{
    DEFN_FUNCTOR(DisarmWeapon)
};

struct DisarmAndStealWeaponFunctor : public Functor
{
    DEFN_FUNCTOR(DisarmAndStealWeapon)
};

struct SwitchDeathTypeFunctor : public Functor
{
    DEFN_FUNCTOR(SwitchDeathType)

    DeathType DeathType;
};

struct TriggerRandomCastFunctor : public Functor
{
    DEFN_FUNCTOR(TriggerRandomCast)

    [[bg3::legacy(Arg1)]] uint8_t DC;
    [[bg3::legacy(Arg2)]] float Delay;
    Array<FixedString> RandomCastOutcomes;
};

struct GainTemporaryHitPointsFunctor : public Functor
{
    DEFN_FUNCTOR(GainTemporaryHitPoints)

    StatsExpressionParam* HitPointsExpression;
};

struct FireProjectileFunctor : public Functor
{
    DEFN_FUNCTOR(FireProjectile)

    [[bg3::legacy(Arg1)]] FixedString ProjectileTemplate;
};

struct ShortRestFunctor : public Functor
{
    DEFN_FUNCTOR(ShortRest)
};

struct CreateZoneFunctor : public Functor
{
    DEFN_FUNCTOR(CreateZone)

    ZoneShape Shape; // Arg1
    FixedString Arg4;
    float Arg2;
    float Duration; // Arg3
    bool Arg5;
};

struct DoTeleportFunctor : public Functor
{
    DEFN_FUNCTOR(DoTeleport)

    float Arg1;
};

struct RegainTemporaryHitPointsFunctor : public Functor
{
    DEFN_FUNCTOR(RegainTemporaryHitPoints)

    StatsExpressionParam* HitPoints;
};

struct RemoveStatusByLevelFunctor : public Functor
{
    DEFN_FUNCTOR(RemoveStatusByLevel)

    std::variant<FixedString, StatusGroup> NameOrStatusGroup; // Arg1
    [[bg3::legacy(Arg2)]] int8_t Level;
    std::optional<AbilityId> Ability;
};

struct SurfaceClearLayerFunctor : public Functor
{
    DEFN_FUNCTOR(SurfaceClearLayer)

    HashSet<SurfaceLayer8> Layers;
};

struct UnsummonFunctor : public Functor
{
    DEFN_FUNCTOR(Unsummon)
};

struct CreateWallFunctor : public Functor
{
    DEFN_FUNCTOR(CreateWall)
};

struct CounterspellFunctor : public Functor
{
    DEFN_FUNCTOR(Counterspell)
};

struct AdjustRollFunctor : public Functor
{
    DEFN_FUNCTOR(AdjustRoll)

    StatsExpressionParam* Expression; // Arg1
    RollAdjustmentType Type; // Arg2
    DamageType DamageType; // Arg2
};

struct SpawnExtraProjectilesFunctor : public Functor
{
    DEFN_FUNCTOR(SpawnExtraProjectiles)

    [[bg3::legacy(Arg1)]] FixedString ProjectileSpell;
};

struct KillFunctor : public Functor
{
    DEFN_FUNCTOR(Kill)
};

struct TutorialEventFunctor : public Functor
{
    DEFN_FUNCTOR(TutorialEvent)

    Guid Event;
};

struct DropFunctor : public Functor
{
    DEFN_FUNCTOR(Drop)

    [[bg3::legacy(Arg1)]] FixedString Effect;
};

struct ResetCooldownsFunctor : public Functor
{
    DEFN_FUNCTOR(ResetCooldowns)

    SpellCooldownType CooldownType;
};

struct SetRollFunctor : public Functor
{
    DEFN_FUNCTOR(SetRoll)

    int Roll;
    RollAdjustmentType Type; // Arg2
    DamageType DamageType; // Arg2
};

struct SetDamageResistanceFunctor : public Functor
{
    DEFN_FUNCTOR(SetDamageResistance)

    DamageType DamageType; // Arg1
};

struct SetRerollFunctor : public Functor
{
    DEFN_FUNCTOR(SetReroll)

    uint8_t Roll;
    [[bg3::legacy(Arg2)]] bool KeepNew;
};

struct SetAdvantageFunctor : public Functor
{
    DEFN_FUNCTOR(SetAdvantage)
};

struct SetDisadvantageFunctor : public Functor
{
    DEFN_FUNCTOR(SetDisadvantage)
};

struct MaximizeRollFunctor : public Functor
{
    DEFN_FUNCTOR(MaximizeRoll)

    DamageType DamageType;
};

struct CameraWaitFunctor : public Functor
{
    DEFN_FUNCTOR(CameraWait)

    float Arg1;
};

struct ModifySpellCameraFocusFunctor : public Functor
{
    DEFN_FUNCTOR(ModifySpellCameraFocus)

    // Unmapped
};

struct ExtenderFunctor : public Functor
{
    DEFN_FUNCTOR(Extender)
};


END_NS()

BEGIN_NS(lua)

LUA_POLYMORPHIC(bg3se::stats::ContextData)
LUA_POLYMORPHIC(bg3se::stats::Functor)

END_NS()
