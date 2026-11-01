BEGIN_NS(lua)

using namespace bg3se::stats;

void MakePolymorphicRef(lua_State* L, ContextData* value, LifetimeHandle lifetime)
{
#define V(type) case FunctorContextType::type: \
            MakeDirectObjectRef(L, static_cast<type##ContextData*>(value), lifetime); break;

    switch (value->Type) {
        V(AttackTarget)
        V(AttackPosition)
        V(Move)
        V(Target)
        V(NearbyAttacked)
        V(NearbyAttacking)
        V(Equip)
        V(Source)
        V(Interrupt)

    default:
        MakeDirectObjectRef(L, value, lifetime);
        break;
    }

#undef V
}

void MakePolymorphicRef(lua_State* L, Functor* value, LifetimeHandle lifetime)
{
#define V(type) case FunctorId::type: \
            MakeDirectObjectRef(L, static_cast<type##Functor*>(value), lifetime); break;

    switch (value->TypeId) {
        V(CustomDescription)
        V(ApplyStatus)
        V(SurfaceChange)
        V(Resurrect)
        V(Sabotage)
        V(Summon)
        V(Force)
        V(Douse)
        V(SwapPlaces)
        V(Pickup)
        V(CreateSurface)
        V(CreateConeSurface)
        V(RemoveStatus)
        V(DealDamage)
        V(ExecuteWeaponFunctors)
        V(RegainHitPoints)
        V(TeleportSource)
        V(SetStatusDuration)
        V(UseSpell)
        V(UseActionResource)
        V(UseAttack)
        V(CreateExplosion)
        V(BreakConcentration)
        V(ApplyEquipmentStatus)
        V(RestoreResource)
        V(Spawn)
        V(Stabilize)
        V(Unlock)
        V(ResetCombatTurn)
        V(RemoveAuraByChildStatus)
        V(SummonInInventory)
        V(SpawnInInventory)
        V(RemoveUniqueStatus)
        V(DisarmWeapon)
        V(DisarmAndStealWeapon)
        V(SwitchDeathType)
        V(TriggerRandomCast)
        V(GainTemporaryHitPoints)
        V(FireProjectile)
        V(ShortRest)
        V(CreateZone)
        V(DoTeleport)
        V(RegainTemporaryHitPoints)
        V(RemoveStatusByLevel)
        V(SurfaceClearLayer)
        V(Unsummon)
        V(CreateWall)
        V(Counterspell)
        V(AdjustRoll)
        V(SpawnExtraProjectiles)
        V(Kill)
        V(TutorialEvent)
        V(Drop)
        V(ResetCooldowns)
        V(SetRoll)
        V(SetDamageResistance)
        V(SetReroll)
        V(SetAdvantage)
        V(SetDisadvantage)
        V(MaximizeRoll)
        V(CameraWait)
        V(ModifySpellCameraFocus)
        V(Extender)

    default:
        MakeDirectObjectRef(L, value, lifetime);
        break;
    }

#undef V
}

END_NS()
