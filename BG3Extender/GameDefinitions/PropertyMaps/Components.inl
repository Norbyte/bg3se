
BEGIN_CLS(BaseComponent)
P_RO(Entity)
P_RO(ComponentHandle)
END_CLS()


BEGIN_CLS(TransformComponent)
INHERIT(BaseComponent)
P_REF(Transform)
END_CLS()


BEGIN_CLS(SenseComponent)
INHERIT(BaseComponent)
P(Sight)
P(Hearing)
P(FOV)
END_CLS()


BEGIN_CLS(ActionResourcesComponent)
INHERIT(BaseComponent)
P_REF(Resources)
END_CLS()


BEGIN_CLS(SpellBookComponent)
INHERIT(BaseComponent)
P_REF(Spells)
END_CLS()


BEGIN_CLS(SurfacePathInfluencesComponent)
INHERIT(BaseComponent)
P_REF(PathInfluences)
END_CLS()


BEGIN_CLS(UseComponent)
INHERIT(BaseComponent)
P_RO(Slot)
P(Charges)
P(MaxCharges)
P(ItemUseType)
// TODO - map other fields
END_CLS()


BEGIN_CLS(WieldingComponent)
INHERIT(BaseComponent)
P_RO(Owner)
END_CLS()


BEGIN_CLS(ActionResourceConsumeMultiplierBoost)
INHERIT(BaseComponent)
P_RO(ResourceUUID)
END_CLS()


BEGIN_CLS(SpellContainerComponent)
INHERIT(BaseComponent)
P_REF(Spells)
END_CLS()


BEGIN_CLS(TagComponent)
INHERIT(BaseComponent)
P_REF(Tags)
END_CLS()


BEGIN_CLS(OsirisTagComponent)
INHERIT(BaseComponent)
P_REF(Tags)
END_CLS()


BEGIN_CLS(SpellBookPreparesComponent)
INHERIT(BaseComponent)
P_REF(PreparedSpells)
END_CLS()


BEGIN_CLS(RelationComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_28)
P_RO(field_38)
END_CLS()


BEGIN_CLS(CanInteractComponent)
INHERIT(BaseComponent)
P(Flags)
END_CLS()


BEGIN_CLS(CanSpeakComponent)
INHERIT(BaseComponent)
P(Flags)
END_CLS()


BEGIN_CLS(OriginComponent)
INHERIT(BaseComponent)
P(Origin)
END_CLS()


BEGIN_CLS(LevelComponent)
INHERIT(BaseComponent)
P_RO(LevelName)
END_CLS()


BEGIN_CLS(esv::ActiveComponent)
INHERIT(BaseComponent)
END_CLS()


BEGIN_CLS(ArmorComponent)
INHERIT(BaseComponent)
P(ArmorType)
P(ArmorClass)
P(AbilityModifierCap)
P(ArmorClassAbility)
P(Shield)
END_CLS()


BEGIN_CLS(BaseHpComponent)
INHERIT(BaseComponent)
P(Vitality)
P(VitalityBoost)
END_CLS()


BEGIN_CLS(CustomStatsComponent)
INHERIT(BaseComponent)
P_REF(Stats)
END_CLS()


BEGIN_CLS(DataComponent)
INHERIT(BaseComponent)
P(Weight_M)
P(StatsId)
P(StatusImmunityFlags)
P(field_24)
P(StepsType)
END_CLS()


BEGIN_CLS(ExperienceComponent)
INHERIT(BaseComponent)
P(Level)
P(NextLevelExperience)
P(Experience_M)
P(SomeExperience)
P(field_28)
END_CLS()


BEGIN_CLS(HealthComponent)
INHERIT(BaseComponent)
P(CurrentHealth)
P(MaxHealth)
P(BoostHealth_M)
// TODO - map
END_CLS()


BEGIN_CLS(StatsComponent)
INHERIT(BaseComponent)
// TODO - Abilities
// TODO - Skills
P(SpellCastingAbility)
P(AdditionalDamage_M)
P_REF(Equipment)
P(ArmorType)
P(Level)
P(Flanked_M)
P_REF(Classes)
P(UnarmedAttackAbility)
P(SomeAttackAbility)
// TODO - map
END_CLS()


BEGIN_CLS(StatusImmunitiesComponent)
INHERIT(BaseComponent)
P_REF(PersonalStatusImmunities)
END_CLS()


BEGIN_CLS(ValueComponent)
INHERIT(BaseComponent)
P(Value)
P(Rarity)
P(Unique)
END_CLS()


BEGIN_CLS(GenderComponent)
INHERIT(BaseComponent)
P(Gender)
P(Gender2)
END_CLS()


BEGIN_CLS(WeaponComponent)
INHERIT(BaseComponent)
P(WeaponRange)
P(DamageRange)
P(WeaponProperties)
P(WeaponGroup)
END_CLS()


BEGIN_CLS(PassiveContainerComponent)
INHERIT(BaseComponent)
// TODO - map
END_CLS()


BEGIN_CLS(PassiveComponent)
INHERIT(BaseComponent)
P_RO(PassiveId)
P_RO(StatsFunctorContext)
// TODO - map
END_CLS()


BEGIN_CLS(CombatParticipantComponent)
INHERIT(BaseComponent)
P_RO(CombatHandle)
P_RO(CombatGroupId)
P_RO(Flags)
P_RO(Archetype)
P_RO(AiHint)
P_RO(CombatGroupChangedFlag_M)
END_CLS()


BEGIN_CLS(CombatStateComponent)
INHERIT(BaseComponent)
P_RO(MyGuid)
// TODO - map
END_CLS()


BEGIN_CLS(TurnOrderComponent)
INHERIT(BaseComponent)
P_REF(TurnOrderIndices)
P_REF(Participants)
// TODO - map
END_CLS()


BEGIN_CLS(TurnBasedComponent)
INHERIT(BaseComponent)
P_RO(CombatHandle_M)
// TODO - map
END_CLS()

