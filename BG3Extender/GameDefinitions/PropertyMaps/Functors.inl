// Property lists for stats functors
// (anything extending Functor)

BEGIN_CLS(stats::Object::FunctorGroup)
P(TextKey)
// Lua read-only helper until RO properties or transparent index support is added
P_GETTER(Functors, GetFunctors)
END_CLS()

BEGIN_CLS(stats::Functors)
// TODO - make functor list
P_RO(NextFunctorIndex)
P_RO(UniqueName)
END_CLS()

BEGIN_CLS(stats::Functor::RollCondition)
P_RO(Type)
// TODO - Conditions object access
P_RO(ConditionId)
END_CLS()

BEGIN_CLS(stats::Functor)
P_RO(UniqueName)
P_RO(FunctorUuid)
P(RollConditions)
// TODO - Conditions object access
P_RO(StatsConditionsId)
P(PropertyContext)
P(StoryActionId)
P(ObserverType)
P_RO(TypeId)
P_BITMASK(Flags)
END_CLS()


BEGIN_CLS(stats::CustomDescriptionFunctor)
INHERIT(stats::Functor)
P(Description)
END_CLS()


BEGIN_CLS(stats::ApplyStatusFunctor)
INHERIT(stats::Functor)
P(StatusId)
P(StatusSpecificParam1)
P_RO(StatsConditions) // TODO - later write support for Conditions + StatsConditionsId
P(StatusSpecificParam2)
P(StatusSpecificParam3)
P(RequiresConcentration)
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
P_BITMASK(HealingType)
END_CLS()


BEGIN_CLS(stats::SabotageFunctor)
INHERIT(stats::Functor)
P(Amount)
END_CLS()


BEGIN_CLS(stats::SummonFunctor)
INHERIT(stats::Functor)
P(Template)
P(AIHelper)
P(SpawnLifetime)
P(StatusesToApply)
P(StackId)
P(Arg5)
P(Arg9)
END_CLS()


BEGIN_CLS(stats::ForceFunctor)
INHERIT(stats::Functor)
P(Distance)
P(Origin)
P(Aggression)
// FIXME - DistanceExpression
P(ControlArc)
P(PullToOrigin)
END_CLS()


BEGIN_CLS(stats::DouseFunctor)
INHERIT(stats::Functor)
P(field_20)
P(field_24)
END_CLS()


BEGIN_CLS(stats::SwapPlacesFunctor)
INHERIT(stats::Functor)
P(Animation)
P(Arg1)
P(Arg2)
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
P(Arg5)
END_CLS()


BEGIN_CLS(stats::CreateConeSurfaceFunctor)
INHERIT(stats::Functor)
P(SurfaceType)
P(Radius)
P(Duration)
P(IsControlledByConcentration)
P(field_55)
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
P(CoinMultiplier)
P(Nonlethal)
P(Magical)
P(IgnoreDamageBonus)
P(IgnoreEvents)
P(ConsumeCoin)
P(IgnoreImmunities)
END_CLS()


BEGIN_CLS(stats::ExecuteWeaponFunctorsFunctor)
INHERIT(stats::Functor)
P(WeaponType)
END_CLS()


BEGIN_CLS(stats::RegainHitPointsFunctor)
INHERIT(stats::Functor)
// TODO - lua expr --  P(HitPoints)
P_BITMASK(HealingType)
END_CLS()


BEGIN_CLS(stats::TeleportSourceFunctor)
INHERIT(stats::Functor)
P(Arg1)
P(Arg2)
END_CLS()


BEGIN_CLS(stats::SetStatusDurationFunctor)
INHERIT(stats::Functor)
P(StatusId)
P(Duration)
P(ChangeType)
END_CLS()


BEGIN_CLS(stats::UseSpellFunctor)
INHERIT(stats::Functor)
P(SpellId)
P(IgnoreHasSpell)
P(IgnoreChecks)
P(Arg4)
P(SpellCastGuid)
P(Arg6)
END_CLS()


BEGIN_CLS(stats::UseActionResourceFunctor)
INHERIT(stats::Functor)
P(ActionResource)
P(Amount)
P(Level)
P(IsPercentage)
P(IgnoreResourceConsumeBoosts)
END_CLS()


BEGIN_CLS(stats::UseAttackFunctor)
INHERIT(stats::Functor)
P(IgnoreChecks)
P(Arg2)
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
P(ActionResource)
P(Hex)
P(field_34)
// TODO -- P(LuaAmount)
P(Amount)
P(AmountType)
END_CLS()


BEGIN_CLS(stats::SpawnFunctor)
INHERIT(stats::Functor)
P(TemplateId)
P(AIHelper)
P(StatusesToApply)
P(Arg7)
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
P(TemplateId)
P(Arg8)
P(Lifetime)
P(AdditionalArgs)
P(Arg9)
P(Amount)
P(Equip)
P(Arg5)
P(Arg6)
P(Arg7)
END_CLS()


BEGIN_CLS(stats::SpawnInInventoryFunctor)
INHERIT(stats::Functor)
P(TemplateId)
P(Arg7)
P(Arg3)
P(AdditionalArgs)
P(Arg4)
P(Arg5)
P(Arg6)
END_CLS()


BEGIN_CLS(stats::RemoveUniqueStatusFunctor)
INHERIT(stats::Functor)
P(StatusId)
END_CLS()


BEGIN_CLS(stats::DisarmWeaponFunctor)
INHERIT(stats::Functor)
END_CLS()


BEGIN_CLS(stats::DisarmAndStealWeaponFunctor)
INHERIT(stats::Functor)
END_CLS()


BEGIN_CLS(stats::SwitchDeathTypeFunctor)
INHERIT(stats::Functor)
P(DeathType)
END_CLS()


BEGIN_CLS(stats::TriggerRandomCastFunctor)
INHERIT(stats::Functor)
P(Arg1)
P(Arg2)
P(RandomCastOutcomes)
END_CLS()


BEGIN_CLS(stats::GainTemporaryHitPointsFunctor)
INHERIT(stats::Functor)
// FIXME - HitPointsExpression
END_CLS()


BEGIN_CLS(stats::FireProjectileFunctor)
INHERIT(stats::Functor)
P(Arg1)
END_CLS()


BEGIN_CLS(stats::ShortRestFunctor)
INHERIT(stats::Functor)
END_CLS()


BEGIN_CLS(stats::CreateZoneFunctor)
INHERIT(stats::Functor)
P(Shape)
P(Arg4)
P(Arg2)
P(Duration)
P(Arg5)
END_CLS()


BEGIN_CLS(stats::DoTeleportFunctor)
INHERIT(stats::Functor)
P(Arg1)
END_CLS()


BEGIN_CLS(stats::RegainTemporaryHitPointsFunctor)
INHERIT(stats::Functor)
// FIXME - HitPointsExpression
END_CLS()


BEGIN_CLS(stats::RemoveStatusByLevelFunctor)
INHERIT(stats::Functor)
P(NameOrStatusGroup)
P(Arg2)
P(Ability)
END_CLS()


BEGIN_CLS(stats::SurfaceClearLayerFunctor)
INHERIT(stats::Functor)
P(Layers)
END_CLS()


BEGIN_CLS(stats::UnsummonFunctor)
INHERIT(stats::Functor)
END_CLS()


BEGIN_CLS(stats::CreateWallFunctor)
INHERIT(stats::Functor)
END_CLS()


BEGIN_CLS(stats::CounterspellFunctor)
INHERIT(stats::Functor)
END_CLS()


BEGIN_CLS(stats::AdjustRollFunctor)
INHERIT(stats::Functor)
// FIXME - Expression
P(Type)
P(DamageType)
END_CLS()


BEGIN_CLS(stats::SpawnExtraProjectilesFunctor)
INHERIT(stats::Functor)
P(Arg1)
END_CLS()


BEGIN_CLS(stats::KillFunctor)
INHERIT(stats::Functor)
END_CLS()


BEGIN_CLS(stats::TutorialEventFunctor)
INHERIT(stats::Functor)
P(Event)
END_CLS()


BEGIN_CLS(stats::DropFunctor)
INHERIT(stats::Functor)
P(Arg1)
END_CLS()


BEGIN_CLS(stats::ResetCooldownsFunctor)
INHERIT(stats::Functor)
P(CooldownType)
END_CLS()


BEGIN_CLS(stats::SetRollFunctor)
INHERIT(stats::Functor)
P(Roll)
P(Type)
P(DamageType)
END_CLS()


BEGIN_CLS(stats::SetDamageResistanceFunctor)
INHERIT(stats::Functor)
P(DamageType)
END_CLS()


BEGIN_CLS(stats::SetRerollFunctor)
INHERIT(stats::Functor)
P(Roll)
P(Arg2)
END_CLS()


BEGIN_CLS(stats::SetAdvantageFunctor)
INHERIT(stats::Functor)
END_CLS()


BEGIN_CLS(stats::SetDisadvantageFunctor)
INHERIT(stats::Functor)
END_CLS()


BEGIN_CLS(stats::MaximizeRollFunctor)
INHERIT(stats::Functor)
P(DamageType)
END_CLS()


BEGIN_CLS(stats::CameraWaitFunctor)
INHERIT(stats::Functor)
P(Arg1)
END_CLS()


BEGIN_CLS(stats::ExtenderFunctor)
INHERIT(stats::Functor)
END_CLS()

