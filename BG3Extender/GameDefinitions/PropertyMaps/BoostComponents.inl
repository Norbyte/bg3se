BEGIN_CLS(BoostConditionComponent)
INHERIT(BaseComponent)
P_RO(ConditionFlags)
P_RO(field_1C)
END_CLS()


BEGIN_CLS(BoostsContainerComponent)
INHERIT(BaseComponent)
P(Boosts)
END_CLS()


BEGIN_CLS(BoostCause)
P(Type)
P(Cause)
P(Entity)
END_CLS()


BEGIN_CLS(BoostParameters)
P(Boost)
P(Params)
P(Params2)
END_CLS()


BEGIN_CLS(RollDefinition)
P(field_0)
P(field_1)
P(field_4)
P(field_8)
END_CLS()


BEGIN_CLS(BoostInfoComponent)
INHERIT(BaseComponent)
P(BoostEntity)
P_RO(Type)
P(field_10)
P(field_20)
P(Cause)
P(Owner)
P(Params)
P(field_80)
END_CLS()


BEGIN_CLS(ArmorClassBoostComponent)
INHERIT(BaseComponent)
P(AC)
END_CLS()


BEGIN_CLS(AbilityBoostComponent)
INHERIT(BaseComponent)
P(Ability)
P(Value)
P(field_8)
P(field_C)
END_CLS()


BEGIN_CLS(RollBonusBoostComponent)
INHERIT(BaseComponent)
P(RollType)
P(Amount)
P(Ability)
P(Skill)
END_CLS()


BEGIN_CLS(AdvantageBoostComponent)
INHERIT(BaseComponent)
P(AdvantageType)
P(Type)
P(Ability)
P(Skill)
P(Tags)
END_CLS()


BEGIN_CLS(ActionResourceValueBoostComponent)
INHERIT(BaseComponent)
P(ResourceUUID)
P(Amount2)
P(Amount)
P(DiceSize)
END_CLS()


BEGIN_CLS(CriticalHitBoostComponent)
INHERIT(BaseComponent)
P(Flags)
P(field_1)
P(Value)
END_CLS()


BEGIN_CLS(AbilityFailedSavingThrowBoostComponent)
INHERIT(BaseComponent)
P(Ability)
END_CLS()


BEGIN_CLS(ResistanceBoostComponent)
INHERIT(BaseComponent)
P(DamageType)
P(ResistanceFlags)
P(IsResistantToAll)
END_CLS()


BEGIN_CLS(WeaponDamageResistanceBoostComponent)
INHERIT(BaseComponent)
P(DamageTypes)
END_CLS()


BEGIN_CLS(ProficiencyBonusOverrideBoostComponent)
INHERIT(BaseComponent)
P(Value)
END_CLS()


BEGIN_CLS(JumpMaxDistanceMultiplierBoostComponent)
INHERIT(BaseComponent)
P(Amount)
END_CLS()


BEGIN_CLS(HalveWeaponDamageBoostComponent)
INHERIT(BaseComponent)
P(Ability)
END_CLS()


BEGIN_CLS(UnlockSpellBoostComponent)
INHERIT(BaseComponent)
P(SpellId)
P(Ability)
P(SomeUUID)
P(SpellChildSelection)
P(CooldownType)
END_CLS()


BEGIN_CLS(SourceAdvantageBoostComponent)
INHERIT(BaseComponent)
P(Type)
END_CLS()


BEGIN_CLS(ProficiencyBonusBoostComponent)
INHERIT(BaseComponent)
P(Type)
P(Ability)
P(Skill)
END_CLS()


BEGIN_CLS(ProficiencyBoostComponent)
INHERIT(BaseComponent)
P(Flags)
END_CLS()


BEGIN_CLS(IncreaseMaxHPBoostComponent)
INHERIT(BaseComponent)
// FIXME - this is Variant<int32_t, StatsExpressionParam>
//P(Amount)
//P(HasAmount)
P(field_30)
END_CLS()


BEGIN_CLS(ActionResourceBlockBoostComponent)
INHERIT(BaseComponent)
P(ResourceUUID)
P(IntParam)
END_CLS()


BEGIN_CLS(StatusImmunityBoostComponent)
INHERIT(BaseComponent)
P(StatusID)
P(UnknownUUIDs)
END_CLS()


BEGIN_CLS(UseBoostsComponent)
INHERIT(BaseComponent)
P(Boosts)
END_CLS()


BEGIN_CLS(CannotHarmCauseEntityBoostComponent)
INHERIT(BaseComponent)
P(Type)
END_CLS()


BEGIN_CLS(TemporaryHPBoostComponent)
INHERIT(BaseComponent)
P(HP)
END_CLS()


BEGIN_CLS(WeightBoostComponent)
INHERIT(BaseComponent)
P(Amount)
END_CLS()


BEGIN_CLS(WeightCategoryBoostComponent)
INHERIT(BaseComponent)
P(Amount)
END_CLS()


BEGIN_CLS(FactionOverrideBoostComponent)
INHERIT(BaseComponent)
P(Faction)
P(field_10)
END_CLS()


BEGIN_CLS(ActionResourceMultiplierBoostComponent)
INHERIT(BaseComponent)
P(ResourceUUID)
P(IntParam2)
P(IntParam)
P(DiceSize)
END_CLS()


BEGIN_CLS(InitiativeBoostComponent)
INHERIT(BaseComponent)
P(Amount)
END_CLS()


BEGIN_CLS(DarkvisionRangeBoostComponent)
INHERIT(BaseComponent)
P(Range)
END_CLS()


BEGIN_CLS(DarkvisionRangeMinBoostComponent)
INHERIT(BaseComponent)
P(Range)
END_CLS()


BEGIN_CLS(DarkvisionRangeOverrideBoostComponent)
INHERIT(BaseComponent)
P(Range)
END_CLS()


BEGIN_CLS(AddTagBoostComponent)
INHERIT(BaseComponent)
P(Tag)
END_CLS()


BEGIN_CLS(IgnoreDamageThresholdMinBoostComponent)
INHERIT(BaseComponent)
P(DamageType)
P(All)
P(Amount)
END_CLS()


BEGIN_CLS(SkillBoostComponent)
INHERIT(BaseComponent)
P(Skill)
P(Amount)
END_CLS()


BEGIN_CLS(WeaponDamageBoostComponent)
INHERIT(BaseComponent)
P(DamageType)
P(Amount)
P(field_30)
END_CLS()


BEGIN_CLS(NullifyAbilityBoostComponent)
INHERIT(BaseComponent)
P(Ability)
END_CLS()


BEGIN_CLS(RerollBoostComponent)
INHERIT(BaseComponent)
P(RollType)
P(field_1)
P(field_2)
END_CLS()


BEGIN_CLS(DownedStatusBoostComponent)
INHERIT(BaseComponent)
P(StatusId)
P(field_4)
END_CLS()


BEGIN_CLS(WeaponEnchantmentBoostComponent)
INHERIT(BaseComponent)
P(Value)
END_CLS()


BEGIN_CLS(GuaranteedChanceRollOutcomeBoostComponent)
INHERIT(BaseComponent)
P(field_0)
END_CLS()


BEGIN_CLS(AttributeBoostComponent)
INHERIT(BaseComponent)
P(AttributeFlags)
END_CLS()


BEGIN_CLS(GameplayLightBoostComponent)
INHERIT(BaseComponent)
P(field_0)
P(field_4)
P(field_8)
P(field_C)
END_CLS()


BEGIN_CLS(DualWieldingBoostComponent)
INHERIT(BaseComponent)
P(DualWielding)
END_CLS()


BEGIN_CLS(SavantBoostComponent)
INHERIT(BaseComponent)
P(SpellSchool)
END_CLS()


BEGIN_CLS(MinimumRollResultBoostComponent)
INHERIT(BaseComponent)
P(RollType)
P(Result)
END_CLS()


BEGIN_CLS(CharacterWeaponDamageBoostComponent)
INHERIT(BaseComponent)
P(Amount)
P(DamageType)
END_CLS()


BEGIN_CLS(ProjectileDeflectBoostComponent)
INHERIT(BaseComponent)
P(ProjectileTypes)
END_CLS()


BEGIN_CLS(AbilityOverrideMinimumBoostComponent)
INHERIT(BaseComponent)
P(Ability)
P(Amount)
END_CLS()


BEGIN_CLS(ACOverrideFormulaBoostComponent)
INHERIT(BaseComponent)
P(AC)
P(field_4)
P(Abilities)
END_CLS()


BEGIN_CLS(FallDamageMultiplierBoostComponent)
INHERIT(BaseComponent)
P(Amount)
END_CLS()


BEGIN_CLS(ActiveCharacterLightBoostComponent)
INHERIT(BaseComponent)
P(LightUUID)
END_CLS()


BEGIN_CLS(WeaponAttackTypeOverrideBoostComponent)
INHERIT(BaseComponent)
P(AttackType)
END_CLS()


BEGIN_CLS(WeaponDamageDieOverrideBoostComponent)
INHERIT(BaseComponent)
P(Roll)
END_CLS()


BEGIN_CLS(CarryCapacityMultiplierBoostComponent)
INHERIT(BaseComponent)
P(Multiplier)
END_CLS()


BEGIN_CLS(WeaponPropertyBoostComponent)
INHERIT(BaseComponent)
P(Properties)
END_CLS()


BEGIN_CLS(WeaponAttackRollAbilityOverrideBoostComponent)
INHERIT(BaseComponent)
P(Ability)
END_CLS()


BEGIN_CLS(SightRangeAdditiveBoostComponent)
INHERIT(BaseComponent)
P(Range)
END_CLS()


BEGIN_CLS(SightRangeMinimumBoostComponent)
INHERIT(BaseComponent)
P(Range)
END_CLS()


BEGIN_CLS(SightRangeMaximumBoostComponent)
INHERIT(BaseComponent)
P(Range)
END_CLS()


BEGIN_CLS(SightRangeOverrideBoostComponent)
INHERIT(BaseComponent)
P(Range)
END_CLS()


BEGIN_CLS(MovementSpeedLimitBoostComponent)
INHERIT(BaseComponent)
P(MovementType)
END_CLS()


BEGIN_CLS(UnlockSpellVariantBoostComponent)
INHERIT(BaseComponent)
P(field_0)
END_CLS()


BEGIN_CLS(DetectCrimesBlockBoostComponent)
INHERIT(BaseComponent)
P(field_0)
END_CLS()


BEGIN_CLS(BlockAbilityModifierFromACComponent)
INHERIT(BaseComponent)
P(Ability)
END_CLS()


BEGIN_CLS(ScaleMultiplierBoostComponent)
INHERIT(BaseComponent)
P(Multiplier)
END_CLS()


BEGIN_CLS(DamageReductionBoostComponent)
INHERIT(BaseComponent)
P(DamageType)
P(Amount)
P(HasAmount)
P(field_38)
P(field_39)
END_CLS()


BEGIN_CLS(ReduceCriticalAttackThresholdBoostComponent)
INHERIT(BaseComponent)
P(field_0)
P(field_4)
P(field_8)
END_CLS()


BEGIN_CLS(PhysicalForceRangeBonusBoostComponent)
INHERIT(BaseComponent)
P(field_0)
P(field_4)
END_CLS()


BEGIN_CLS(ObjectSizeBoostComponent)
INHERIT(BaseComponent)
P(SizeCategoryAdjustment)
END_CLS()


BEGIN_CLS(ObjectSizeOverrideBoostComponent)
INHERIT(BaseComponent)
P(field_0)
END_CLS()


BEGIN_CLS(AiArchetypeOverrideBoostComponent)
INHERIT(BaseComponent)
P(Archetype)
P(Priority)
END_CLS()


BEGIN_CLS(ExpertiseBonusBoostComponent)
INHERIT(BaseComponent)
P(Skill)
END_CLS()


BEGIN_CLS(EntityThrowDamageBoostComponent)
INHERIT(BaseComponent)
P(Roll)
P(field_C)
END_CLS()


BEGIN_CLS(WeaponDamageTypeOverrideBoostComponent)
INHERIT(BaseComponent)
P(DamageType)
END_CLS()


BEGIN_CLS(MaximizeHealingBoostComponent)
INHERIT(BaseComponent)
P(Direction)
P(TargetTypes)
END_CLS()


BEGIN_CLS(DamageBonusBoostComponent)
INHERIT(BaseComponent)
P(Amount)
P(DamageType)
P(field_31)
END_CLS()


BEGIN_CLS(AdvanceSpellsBoostComponent)
INHERIT(BaseComponent)
P(field_0)
P(field_4)
END_CLS()


BEGIN_CLS(SpellResistanceBoostComponent)
INHERIT(BaseComponent)
P(Resistance)
END_CLS()


BEGIN_CLS(WeaponAttackRollBonusBoostComponent)
INHERIT(BaseComponent)
P(Amount)
END_CLS()


BEGIN_CLS(SpellSaveDCBoostComponent)
INHERIT(BaseComponent)
P(DC)
END_CLS()


BEGIN_CLS(RedirectDamageBoostComponent)
INHERIT(BaseComponent)
P(Amount)
P(DamageType1)
P(DamageType2)
P(field_6)
END_CLS()


BEGIN_CLS(CanSeeThroughBoostComponent)
INHERIT(BaseComponent)
P(CanSeeThrough)
END_CLS()


BEGIN_CLS(CanShootThroughBoostComponent)
INHERIT(BaseComponent)
P(CanShootThrough)
END_CLS()


BEGIN_CLS(CanWalkThroughBoostComponent)
INHERIT(BaseComponent)
P(CanWalkThrough)
END_CLS()


BEGIN_CLS(MonkWeaponDamageDiceOverrideBoostComponent)
INHERIT(BaseComponent)
P(DamageDice)
END_CLS()


BEGIN_CLS(HorizontalFOVOverrideBoostComponent)
INHERIT(BaseComponent)
P(FOV)
END_CLS()


BEGIN_CLS(CharacterUnarmedDamageBoostComponent)
INHERIT(BaseComponent)
P(Amount)
P(DamageType)
END_CLS()


BEGIN_CLS(ActionResourceReplenishTypeOverrideBoostComponent)
INHERIT(BaseComponent)
P(ActionResource)
P(ReplenishType)
END_CLS()


BEGIN_CLS(ActionResourcePreventReductionBoostComponent)
INHERIT(BaseComponent)
P(ActionResource)
P(Amount)
END_CLS()


BEGIN_CLS(AttackSpellOverrideBoostComponent)
INHERIT(BaseComponent)
P(SpellId)
P(AttackType)
END_CLS()


BEGIN_CLS(LockBoostComponent)
INHERIT(BaseComponent)
P(Lock)
END_CLS()


BEGIN_CLS(IgnorePointBlankDisadvantageBoostComponent)
INHERIT(BaseComponent)
P(Flags)
END_CLS()


BEGIN_CLS(CriticalHitExtraDiceBoostComponent)
INHERIT(BaseComponent)
P(Amount)
P(AttackType)
END_CLS()


BEGIN_CLS(DodgeAttackRollBoostComponent)
INHERIT(BaseComponent)
P(field_0)
P(field_4)
P(StatusType)
P(StatusGroup)
END_CLS()


BEGIN_CLS(GameplayObscurityBoostComponent)
INHERIT(BaseComponent)
P(Obscurity)
END_CLS()


BEGIN_CLS(MaximumRollResultBoostComponent)
INHERIT(BaseComponent)
P(RollType)
P(Result)
END_CLS()


BEGIN_CLS(UnlockInterruptBoostComponent)
INHERIT(BaseComponent)
P(Interrupt)
END_CLS()


BEGIN_CLS(JumpMaxDistanceBonusBoostComponent)
INHERIT(BaseComponent)
P(DistanceBonus)
END_CLS()


BEGIN_CLS(ArmorAbilityModifierCapOverrideBoostComponent)
INHERIT(BaseComponent)
P(ArmorType)
P(Value)
END_CLS()


BEGIN_CLS(IgnoreResistanceBoostComponent)
INHERIT(BaseComponent)
P(DamageType)
P(Flags)
END_CLS()


BEGIN_CLS(ConcentrationIgnoreDamageBoostComponent)
INHERIT(BaseComponent)
P(SpellSchool)
END_CLS()


BEGIN_CLS(IgnoreLowGroundPenaltyBoostComponent)
INHERIT(BaseComponent)
P(RollType)
END_CLS()


BEGIN_CLS(IgnoreSurfaceCoverBoostComponent)
INHERIT(BaseComponent)
P(SurfaceType)
END_CLS()
