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
P(SomeFlag)
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
P(Ability)
P(Skill)
END_CLS()


BEGIN_CLS(IncreaseMaxHPBoostComponent)
INHERIT(BaseComponent)
P(HP)
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


BEGIN_CLS(TemporaryHPBoostComponent)
INHERIT(BaseComponent)
P(HP)
END_CLS()


BEGIN_CLS(WeightBoostComponent)
INHERIT(BaseComponent)
P(Amount)
END_CLS()


BEGIN_CLS(FactionOverrideBoostComponent)
INHERIT(BaseComponent)
P(Faction)
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
P(TagUUID)
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
P(DamageMin)
P(DamageMax)
P(field_24)
END_CLS()


BEGIN_CLS(NullifyAbilityBoostComponent)
INHERIT(BaseComponent)
P(Ability)
END_CLS()


BEGIN_CLS(RerollBoostComponent)
INHERIT(BaseComponent)
P(RollType)
P(field_19)
P(field_1A)
END_CLS()


BEGIN_CLS(DownedStatusBoostComponent)
INHERIT(BaseComponent)
P(StatusId)
END_CLS()


BEGIN_CLS(WeaponEnchantmentBoostComponent)
INHERIT(BaseComponent)
P(Value)
END_CLS()


BEGIN_CLS(GuaranteedChanceRollOutcomeBoostComponent)
INHERIT(BaseComponent)
P(field_18)
END_CLS()


BEGIN_CLS(AttributeBoostComponent)
INHERIT(BaseComponent)
P(AttributeFlags)
END_CLS()


BEGIN_CLS(GameplayLightBoostComponent)
INHERIT(BaseComponent)
P(field_18)
P(field_1C)
P(field_20)
END_CLS()


BEGIN_CLS(DualWieldingBoostComponent)
INHERIT(BaseComponent)
P(field_18)
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
P(MinDamage)
P(MaxDamage)
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


BEGIN_CLS(FallDamageMultiplierBoostComponent)
INHERIT(BaseComponent)
P(Amount)
END_CLS()
