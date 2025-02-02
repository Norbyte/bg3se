BEGIN_NS(stats)

Array<Functor*>* Functors::LuaGetFunctorList() const
{
    return const_cast<Array<Functor*>*>(&Values);
}

Functor::~Functor()
{}

bool Functor::ParseParams(std::span<StringView>& params, STDString& key, stats::PropertyContext context)
{
    // Not implemented for now
    return false;
}

Functor* Functor::Clone() const
{
    return GameAlloc<Functor>(*this);
}

#define IMPL_FUNCTOR(ty) \
    ty##Functor::~ty##Functor() {} \
    Functor* ty##Functor::Clone() const { \
        return GameAlloc<ty##Functor>(*this); \
    }

IMPL_FUNCTOR(CustomDescription)
IMPL_FUNCTOR(ApplyStatus)
IMPL_FUNCTOR(SurfaceChange)
IMPL_FUNCTOR(Resurrect)
IMPL_FUNCTOR(Sabotage)
IMPL_FUNCTOR(Summon)
IMPL_FUNCTOR(Force)
IMPL_FUNCTOR(Douse)
IMPL_FUNCTOR(SwapPlaces)
IMPL_FUNCTOR(Pickup)
IMPL_FUNCTOR(CreateSurface)
IMPL_FUNCTOR(CreateConeSurface)
IMPL_FUNCTOR(RemoveStatus)
IMPL_FUNCTOR(DealDamage)
IMPL_FUNCTOR(ExecuteWeaponFunctors)
IMPL_FUNCTOR(RegainHitPoints)
IMPL_FUNCTOR(TeleportSource)
IMPL_FUNCTOR(SetStatusDuration)
IMPL_FUNCTOR(UseSpell)
IMPL_FUNCTOR(UseActionResource)
IMPL_FUNCTOR(UseAttack)
IMPL_FUNCTOR(CreateExplosion)
IMPL_FUNCTOR(BreakConcentration)
IMPL_FUNCTOR(ApplyEquipmentStatus)
IMPL_FUNCTOR(RestoreResource)
IMPL_FUNCTOR(Spawn)
IMPL_FUNCTOR(Stabilize)
IMPL_FUNCTOR(Unlock)
IMPL_FUNCTOR(ResetCombatTurn)
IMPL_FUNCTOR(RemoveAuraByChildStatus)
IMPL_FUNCTOR(SummonInInventory)
IMPL_FUNCTOR(SpawnInInventory)
IMPL_FUNCTOR(RemoveUniqueStatus)
IMPL_FUNCTOR(DisarmWeapon)
IMPL_FUNCTOR(DisarmAndStealWeapon)
IMPL_FUNCTOR(SwitchDeathType)
IMPL_FUNCTOR(TriggerRandomCast)
IMPL_FUNCTOR(GainTemporaryHitPoints)
IMPL_FUNCTOR(FireProjectile)
IMPL_FUNCTOR(ShortRest)
IMPL_FUNCTOR(CreateZone)
IMPL_FUNCTOR(DoTeleport)
IMPL_FUNCTOR(RegainTemporaryHitPoints)
IMPL_FUNCTOR(RemoveStatusByLevel)
IMPL_FUNCTOR(SurfaceClearLayer)
IMPL_FUNCTOR(Unsummon)
IMPL_FUNCTOR(CreateWall)
IMPL_FUNCTOR(Counterspell)
IMPL_FUNCTOR(AdjustRoll)
IMPL_FUNCTOR(SpawnExtraProjectiles)
IMPL_FUNCTOR(Kill)
IMPL_FUNCTOR(TutorialEvent)
IMPL_FUNCTOR(Drop)
IMPL_FUNCTOR(ResetCooldowns)
IMPL_FUNCTOR(SetRoll)
IMPL_FUNCTOR(SetDamageResistance)
IMPL_FUNCTOR(SetReroll)
IMPL_FUNCTOR(SetAdvantage)
IMPL_FUNCTOR(SetDisadvantage)
IMPL_FUNCTOR(MaximizeRoll)
IMPL_FUNCTOR(CameraWait)
IMPL_FUNCTOR(ModifySpellCameraFocus)
IMPL_FUNCTOR(Extender)

#define IMPL_CONTEXT(ty) ty##ContextData::~ty##ContextData() {}

ContextData::~ContextData()
{}

IMPL_CONTEXT(AttackTarget)
IMPL_CONTEXT(AttackPosition)
IMPL_CONTEXT(Move)
IMPL_CONTEXT(Target)
IMPL_CONTEXT(NearbyAttacked)
IMPL_CONTEXT(NearbyAttacking)
IMPL_CONTEXT(Equip)
IMPL_CONTEXT(Source)
IMPL_CONTEXT(Interrupt)

END_NS()
