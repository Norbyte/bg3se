// Property lists for stats functors
// (anything extending Functor)

BEGIN_CLS(stats::Functors)
// TODO - make functor list
P_RO(NextFunctorIndex)
P_RO(UniqueName)
END_CLS()

BEGIN_CLS(LuaExpressionBase)
P(ExpressionParams)
P(Code)
END_CLS()


BEGIN_CLS(LuaExpression)
INHERIT(LuaExpressionBase)
END_CLS()

BEGIN_CLS(stats::Functor::RollCondition)
P_RO(Type)
// TODO - Conditions object access
P_RO(ConditionId)
END_CLS()

BEGIN_CLS(stats::Functor)
P_RO(UniqueName)
P(RollConditions)
// TODO - Conditions object access
P_RO(StatsConditionsId)
// P_BITMASK(PropertyContext)
P(ObserverType)
P_RO(TypeId)
// P_BITMASK(Flags)
END_CLS()


BEGIN_CLS(stats::CustomDescriptionFunctor)
INHERIT(stats::Functor)
P(Description)
END_CLS()


BEGIN_CLS(stats::ApplyStatusFunctor)
INHERIT(stats::Functor)
P(StatusId)
P(StringParam)
P_RO(StatsConditions) // TODO - later write support for Conditions + StatsConditionsId
//P(Param1)
//P(Param2)
// TODO - Duration expression!
// P(Arg2_DurationLuaExpression) 
P(HasParam6)
END_CLS()


BEGIN_CLS(stats::SurfaceChangeFunctor)
INHERIT(stats::Functor)
P(Chance)
P(field_24)
P(field_28)
P(field_2C)
P(SurfaceChange)
END_CLS()


BEGIN_CLS(stats::ResurrectFunctor)
INHERIT(stats::Functor)
P(Probability)
P(HealthPercentage)
END_CLS()


BEGIN_CLS(stats::SabotageFunctor)
INHERIT(stats::Functor)
P(Amount)
END_CLS()


BEGIN_CLS(stats::SummonFunctor)
INHERIT(stats::Functor)
P(MovingObject)
P(Arg2)
P(SpawnLifetime)
P(StatusesToApply)
P(Arg3)
END_CLS()


BEGIN_CLS(stats::ForceFunctor)
INHERIT(stats::Functor)
//P(Arg0)
P(Origin)
P(Aggression)
END_CLS()


BEGIN_CLS(stats::DouseFunctor)
INHERIT(stats::Functor)
P(field_20)
P(field_24)
END_CLS()


BEGIN_CLS(stats::SwapPlacesFunctor)
INHERIT(stats::Functor)
////P(Arg0)
END_CLS()


BEGIN_CLS(stats::EqualizeFunctor)
INHERIT(stats::Functor)
P(HealType)
END_CLS()


BEGIN_CLS(stats::PickupFunctor)
INHERIT(stats::Functor)
P(Arg0)
END_CLS()


BEGIN_CLS(stats::CreateSurfaceFunctor)
INHERIT(stats::Functor)
P(SurfaceType)
P(Radius)
P(Arg4)
P(Duration)
P(IsControlledByConcentration)
END_CLS()


BEGIN_CLS(stats::CreateConeSurfaceFunctor)
INHERIT(stats::Functor)
P(Arg2)
P(Arg0)
P(Arg1)
P(Arg3)
END_CLS()


BEGIN_CLS(stats::RemoveStatusFunctor)
INHERIT(stats::Functor)
P(StatusId)
END_CLS()


BEGIN_CLS(stats::DealDamageFunctor)
INHERIT(stats::Functor)
P(DamageType)
P(WeaponType)
P(WeaponDamageType)
// TODO - Damage expression access
P(Nonlethal)
P(Magical)
P(field_34)
END_CLS()


BEGIN_CLS(stats::ExecuteWeaponFunctorsFunctor)
INHERIT(stats::Functor)
P(WeaponType)
END_CLS()


BEGIN_CLS(stats::RegainHitPointsFunctor)
INHERIT(stats::Functor)
// TODO - lua expr --  P(HitPoints)
END_CLS()


BEGIN_CLS(stats::TeleportSourceFunctor)
INHERIT(stats::Functor)
END_CLS()


BEGIN_CLS(stats::SetStatusDurationFunctor)
INHERIT(stats::Functor)
P(StatusId)
P(Duration)
P(SetIfLonger)
END_CLS()


BEGIN_CLS(stats::UseSpellFunctor)
INHERIT(stats::Functor)
P(SpellId)
P(IgnoreHasSpell)
P(IgnoreChecks)
P(Arg3)
END_CLS()


BEGIN_CLS(stats::UseActionResourceFunctor)
INHERIT(stats::Functor)
P(ActionResourceUUID)
P(Amount)
P(ResourceIndex)
P(IsPercentage)
END_CLS()


BEGIN_CLS(stats::UseAttackFunctor)
INHERIT(stats::Functor)
P(IgnoreChecks)
END_CLS()


BEGIN_CLS(stats::CreateExplosionFunctor)
INHERIT(stats::Functor)
P(SpellId)
END_CLS()


BEGIN_CLS(stats::BreakConcentrationFunctor)
INHERIT(stats::Functor)
END_CLS()


BEGIN_CLS(stats::ApplyEquipmentStatusFunctor)
INHERIT(stats::ApplyStatusFunctor)
P(EquipmentSlot)
END_CLS()


BEGIN_CLS(stats::RestoreResourceFunctor)
INHERIT(stats::Functor)
P(ActionResourceUUID)
P(Hex)
P(field_34)
// TODO -- P(LuaAmount)
P(Amount)
P(IsPercentage)
END_CLS()


BEGIN_CLS(stats::SpawnFunctor)
INHERIT(stats::Functor)
P(TemplateId)
P(Arg1)
P(StatusesToApply)
END_CLS()


BEGIN_CLS(stats::StabilizeFunctor)
INHERIT(stats::Functor)
END_CLS()


BEGIN_CLS(stats::UnlockFunctor)
INHERIT(stats::Functor)
END_CLS()


BEGIN_CLS(stats::ResetCombatTurnFunctor)
INHERIT(stats::Functor)
END_CLS()


BEGIN_CLS(stats::RemoveAuraByChildStatusFunctor)
INHERIT(stats::Functor)
P(StatusId)
END_CLS()


BEGIN_CLS(stats::SummonInInventoryFunctor)
INHERIT(stats::Functor)
P(Arg1)
//P(Arg2)
P(Arg3)
P(Arg4)
P(Arg5)
P(Arg6)
P(Arg7)
P(Arg8)
P(AdditionalArgs)
END_CLS()


BEGIN_CLS(stats::SpawnInInventoryFunctor)
INHERIT(stats::Functor)
P(Arg1)
P(Arg2)
P(Arg3)
P(Arg4)
P(Arg5)
P(Arg6)
P(AdditionalArgs)
END_CLS()


BEGIN_CLS(stats::RemoveUniqueStatusFunctor)
INHERIT(stats::Functor)
P(StatusId)
END_CLS()


BEGIN_CLS(stats::DisarmWeaponFunctor)
INHERIT(stats::Functor)
END_CLS()


BEGIN_CLS(stats::ExtenderFunctor)
INHERIT(stats::Functor)
END_CLS()

