// Property lists for stats functors
// (anything extending StatsFunctorBase)

BEGIN_CLS(StatsFunctorBase)
P_RO(UniqueName)
P_RO(TypeId)
P(PropertyContext)
P_RO(StatsConditionsId)
// TODO - Conditions object access
P(IsSelf)
P_RO(StoryActionId)
END_CLS()


BEGIN_CLS(CustomDescriptionFunctor)
INHERIT(StatsFunctorBase)
P(Description)
END_CLS()


BEGIN_CLS(ApplyStatusFunctor)
INHERIT(StatsFunctorBase)
P(StatusId)
P(StringParam)
P_RO(StatsConditions) // TODO - later write support for Conditions + StatsConditionsId
P(Duration)
P(Param1)
P(Param2)
END_CLS()


BEGIN_CLS(SurfaceChangeFunctor)
INHERIT(StatsFunctorBase)
P(Chance)
P(field_24)
P(field_28)
P(field_2C)
P(SurfaceChange)
END_CLS()


BEGIN_CLS(ResurrectFunctor)
INHERIT(StatsFunctorBase)
P(Probability)
P(HealthPercentage)
END_CLS()


BEGIN_CLS(SabotageFunctor)
INHERIT(StatsFunctorBase)
P(Amount)
END_CLS()


BEGIN_CLS(SummonFunctor)
INHERIT(StatsFunctorBase)
P(MovingObject)
P(field_24)
P(SpawnLifetime)
P(StatusesToApply)
P(field_48)
END_CLS()


BEGIN_CLS(ForceFunctor)
INHERIT(StatsFunctorBase)
P(Arg0)
P(Origin)
P(Aggression)
END_CLS()


BEGIN_CLS(DouseFunctor)
INHERIT(StatsFunctorBase)
P(field_20)
P(field_24)
END_CLS()


BEGIN_CLS(SwapPlacesFunctor)
INHERIT(StatsFunctorBase)
P(Arg0)
END_CLS()


BEGIN_CLS(EqualizeFunctor)
INHERIT(StatsFunctorBase)
P(HealType)
END_CLS()


BEGIN_CLS(PickupFunctor)
INHERIT(StatsFunctorBase)
P(Arg0)
END_CLS()


BEGIN_CLS(CreateSurfaceFunctor)
INHERIT(StatsFunctorBase)
P(SurfaceType)
P(Radius)
P(Arg4)
P(Duration)
P(IsControlledByConcentration)
END_CLS()


BEGIN_CLS(CreateConeSurfaceFunctor)
INHERIT(StatsFunctorBase)
P(Arg2)
P(Arg0)
P(Arg1)
P(Arg3)
END_CLS()


BEGIN_CLS(RemoveStatusFunctor)
INHERIT(StatsFunctorBase)
P(StatusId)
END_CLS()


BEGIN_CLS(DealDamageFunctor)
INHERIT(StatsFunctorBase)
P(DamageType)
P(WeaponType)
P(WeaponDamageType)
// TODO - Damage expression access
P(Nonlethal)
P(Magical)
END_CLS()


BEGIN_CLS(ExecuteWeaponFunctorsFunctor)
INHERIT(StatsFunctorBase)
P(WeaponType)
END_CLS()


BEGIN_CLS(RegainHitPointsFunctor)
INHERIT(StatsFunctorBase)
// TODO - lua expr --  P(HitPoints)
END_CLS()


BEGIN_CLS(TeleportSourceFunctor)
INHERIT(StatsFunctorBase)
END_CLS()


BEGIN_CLS(SetStatusDurationFunctor)
INHERIT(StatsFunctorBase)
P(StatusId)
P(Duration)
P(SetIfLonger)
END_CLS()


BEGIN_CLS(UseSpellFunctor)
INHERIT(StatsFunctorBase)
P(SpellId)
P(IgnoreHasSpell)
P(IgnoreChecks)
P(Arg3)
END_CLS()


BEGIN_CLS(UseActionResourceFunctor)
INHERIT(StatsFunctorBase)
P(ActionResourceUUID)
P(Amount)
P(ResourceIndex)
P(IsPercentage)
END_CLS()


BEGIN_CLS(UseAttackFunctor)
INHERIT(StatsFunctorBase)
P(IgnoreChecks)
END_CLS()


BEGIN_CLS(CreateExplosionFunctor)
INHERIT(StatsFunctorBase)
P(SpellId)
END_CLS()


BEGIN_CLS(BreakConcentrationFunctor)
INHERIT(StatsFunctorBase)
END_CLS()


BEGIN_CLS(ApplyEquipmentStatusFunctor)
INHERIT(ApplyStatusFunctor)
P(EquipmentSlot)
END_CLS()


BEGIN_CLS(RestoreResourceFunctor)
INHERIT(StatsFunctorBase)
P(ActionResourceUUID)
P(Hex)
P(field_34)
// TODO -- P(LuaAmount)
P(Amount)
P(IsPercentage)
END_CLS()


BEGIN_CLS(SpawnFunctor)
INHERIT(StatsFunctorBase)
P(TemplateId)
P(Arg1)
P(StatusesToApply)
END_CLS()


BEGIN_CLS(StabilizeFunctor)
INHERIT(StatsFunctorBase)
END_CLS()


BEGIN_CLS(UnlockFunctor)
INHERIT(StatsFunctorBase)
END_CLS()


BEGIN_CLS(ResetCombatTurnFunctor)
INHERIT(StatsFunctorBase)
END_CLS()


BEGIN_CLS(RemoveAuraByChildStatusFunctor)
INHERIT(StatsFunctorBase)
P(StatusId)
END_CLS()


BEGIN_CLS(SummonInInventoryFunctor)
INHERIT(StatsFunctorBase)
P(Arg1)
P(Arg2)
P(Arg3)
P(Arg4)
P(Arg5)
P(Arg6)
P(Arg7)
P(Arg8)
P(AdditionalArgs)
END_CLS()


BEGIN_CLS(SpawnInInventoryFunctor)
INHERIT(StatsFunctorBase)
P(Arg1)
P(Arg2)
P(Arg3)
P(Arg4)
P(Arg5)
P(Arg6)
P(AdditionalArgs)
END_CLS()


BEGIN_CLS(ExtenderFunctor)
INHERIT(StatsFunctorBase)
END_CLS()

