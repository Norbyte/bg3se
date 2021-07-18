
BEGIN_CLS(BaseComponent)
P_RO(Entity)
P_RO(Handle)

P_FUN(GetReplicationFlags, ComponentGetReplicationFlags)
P_FUN(SetReplicationFlag, ComponentSetReplicationFlag)
P_FUN(Replicate, ComponentReplicate)
END_CLS()


// TEMPORARY WORKAROUND
/*
BEGIN_CLS(IGameObject)
P_RO(Entity)
P_RO(Handle)

P_FUN(GetReplicationFlags, ComponentGetReplicationFlags)
P_FUN(SetReplicationFlag, ComponentSetReplicationFlag)
P_FUN(Replicate, ComponentReplicate)
END_CLS()
*/


BEGIN_CLS(TransformComponent)
INHERIT(BaseComponent)
P_REF(Transform)
END_CLS()


BEGIN_CLS(HearingComponent)
INHERIT(BaseComponent)
P(Sight)
P(Hearing)
P(FOV)
END_CLS()


BEGIN_CLS(ActionResourcesComponent::Amount::SubAmount)
P_RO(Amount)
P_RO(MaxAmount)
END_CLS()


BEGIN_CLS(ActionResourcesComponent::Amount)
P_RO(ResourceUUID)
P_RO(ResourceId)
P_RO(Amount)
P_RO(MaxAmount)
P_REF(SubAmounts)
P_RO(SubAmountsValid)
END_CLS()


BEGIN_CLS(ActionResourcesComponent)
INHERIT(BaseComponent)
P_REF(Resources)
END_CLS()


BEGIN_CLS(SpellBookEntry)
P_REF(Id)
P_RO(SpellUUID)
P_RO(field_38)
P_RO(CooldownType)
P_RO(field_41)
P_RO(field_42)
P_RO(SpellCastingAbility)
END_CLS()


BEGIN_CLS(SpellBookComponent)
INHERIT(BaseComponent)
P_REF(Spells)
END_CLS()


BEGIN_CLS(SurfacePathInfluence)
P_RO(SurfaceType)
P_RO(IsCloud)
P_RO(Influence)
END_CLS()


BEGIN_CLS(SurfacePathInfluencesComponent)
INHERIT(BaseComponent)
P_REF(PathInfluences)
END_CLS()


BEGIN_CLS(UseComponent)
INHERIT(BaseComponent)
P(Charges)
P(MaxCharges)
P(ItemUseType)
P(ItemComboFlag1)
P(ItemComboFlag2)
P_REF(Boosts)
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


BEGIN_CLS(SpellContainerComponent::Spell)
P_REF(SpellId)
P_RO(ItemHandle)
P_RO(SelectionType)
P_RO(field_29)
P_RO(SpellUUID)
P_RO(SpellCastingAbility)
P_RO(CooldownType)
P_RO(field_44)
P_RO(field_48)
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
P_REF(field_30)
P_REF(field_88)
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
P(Flags)
P(field_24)
P(field_28)
P(StepsType)
END_CLS()


BEGIN_CLS(ExperienceComponent)
INHERIT(BaseComponent)
P(CurrentLevelExperience)
P(NextLevelExperience)
P(TotalExperience)
P(SomeExperience)
P(field_28)
END_CLS()


BEGIN_CLS(HealthComponent)
INHERIT(BaseComponent)
P_REF(PerDamageTypeModifiers)
P(CurrentHealth)
P(CurrentHealth_M)
P(MaxHealth)
P(MaxHealth_M)
P_REF(PerDamageTypeHealthThresholds)
P(CannotDamage_M)
// TODO - map
END_CLS()


BEGIN_CLS(StatsComponent::EquipmentEntry)
P(Slot)
P(ItemHandle)
P(field_10)
P(field_11)
END_CLS()


BEGIN_CLS(StatsComponent::ClassInfo)
P(Class)
P(SubClass)
P(Priority)
P(Unknown)
END_CLS()


BEGIN_CLS(StatsComponent)
INHERIT(BaseComponent)
P(field_10)
P_REF(Abilities)
P_REF(AbilityModifiers)
P_REF(Skills)
P(SpellCastingAbility)
P(SpellDC)
P(WeaponActionDC)
P(ProficiencyBonus)
P(field_AC)
P_REF(Equipment)
P(ArmorType)
P(ArmorType_Breast)
P(Level)
P(Flanked)
P_REF(Classes)
P(UnarmedAttackAbility)
P(RangedAttackAbility)
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
P(Gender1)
P(Gender2)
END_CLS()


BEGIN_CLS(WeaponComponent)
INHERIT(BaseComponent)
P_REF(Rolls)
P_REF(Rolls2)
P(WeaponRange)
P(DamageRange)
P(WeaponProperties)
P(WeaponGroup)
P(Ability)
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
P_REF(Initiatives)
P_REF(Participants)
P_RO(field_98)
P_RO(field_A0)
P_RO(Level)
P_RO(field_AC)
P_RO(field_B0)
P_REF(field_B8)
P_RO(field_D0)
END_CLS()


BEGIN_CLS(TurnOrderComponent::ParticipantHandleInfo)
P_RO(Entity)
P_RO(Initiative)
END_CLS()


BEGIN_CLS(TurnOrderComponent::Participant)
P_REF(Handles)
P_RO(Participant)
P_RO(field_28)
P_RO(Initiative)
P_RO(field_30)
END_CLS()


BEGIN_CLS(TurnOrderComponent)
INHERIT(BaseComponent)
P_REF(TurnOrderIndices)
P_REF(Participants)
P_REF(TurnOrderIndices2)
P_REF(Participants2)
P_RO(field_78)
P_RO(field_7C)
END_CLS()


BEGIN_CLS(TurnBasedComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_19)
P_RO(field_1A)
P_RO(field_1C)
P_RO(field_20)
P_RO(field_21)
P_RO(field_24)
P_RO(field_28)
P_RO(field_2C)
P_RO(field_30)
P_RO(field_38)
END_CLS()


BEGIN_CLS(BackgroundComponent)
INHERIT(BaseComponent)
P_RO(Background)
END_CLS()


BEGIN_CLS(GodComponent)
INHERIT(BaseComponent)
P_RO(God)
END_CLS()


BEGIN_CLS(LevelUpUpgrades::AbilityData)
P_RO(field_0)
P_RO(Feat)
P_RO(Class)
P_RO(field_28)
P_RO(field_30)
P_RO(Ability)
P_REF(Array_b8)
P_RO(field_60)
P_RO(field_80)
END_CLS()


BEGIN_CLS(LevelUpUpgrades::SkillData)
P_RO(field_0)
P_RO(field_8)
P_RO(field_18)
P_RO(field_28)
P_RO(field_30)
P_RO(Skill)
P_REF(Array_b8)
P_RO(field_60)
P_RO(field_80)
END_CLS()


BEGIN_CLS(LevelUpUpgrades::SpellData::StringPair)
P_RO(A)
P_RO(B)
END_CLS()



BEGIN_CLS(LevelUpUpgrades::SpellData)
P_RO(field_0)
P_RO(Race)
P_RO(field_18)
P_RO(field_28)
P_RO(field_30)
P_RO(Spell)
P_REF(Array_FS)
P_REF(Array_FS2)
P_RO(field_78)
END_CLS()


BEGIN_CLS(LevelUpUpgrades::LevelUpDataInner4)
P_RO(field_0)
P_RO(field_8)
P_RO(Class)
P_RO(field_28)
P_RO(field_30)
P_RO(field_38)
P_REF(Array_FS)
P_RO(field_60)
P_RO(field_80)
END_CLS()


BEGIN_CLS(LevelUpUpgrades)
P_REF(Abilities)
P_REF(Skills)
P_REF(Spells)
P_REF(field_48)
END_CLS()


BEGIN_CLS(LevelUpData3)
P_RO(field_0)
P_RO(field_4)
P_RO(field_10)
END_CLS()


BEGIN_CLS(LevelUpData)
P_RO(Class)
P_RO(SubClass)
P_RO(Feat)
P_REF(field_30)
P_REF(Upgrades)
P_REF(field_B0)
END_CLS()


BEGIN_CLS(LevelUpComponent)
INHERIT(BaseComponent)
P_REF(field_18)
END_CLS()


BEGIN_CLS(PlayerPrepareSpellComponent::Spell)
P_RO(field_0)
P_RO(field_4)
P_RO(field_8)
P_RO(field_10)
END_CLS()


BEGIN_CLS(PlayerPrepareSpellComponent)
INHERIT(BaseComponent)
P_REF(field_18)
P_RO(field_30)
END_CLS()


BEGIN_CLS(CCPrepareSpellComponent)
INHERIT(BaseComponent)
P_REF(field_18)
END_CLS()


BEGIN_CLS(SpellCastComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_1C)
P_RO(field_20)
P_RO(field_28)
P_RO(field_2C)
P_RO(field_30)
P_RO(field_34)
P_RO(field_38)
P_RO(field_39)
P_RO(field_3A)
P_RO(field_3B)
END_CLS()


BEGIN_CLS(FloatingComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_1C)
END_CLS()


BEGIN_CLS(VoiceComponent)
INHERIT(BaseComponent)
P_RO(Voice)
END_CLS()


BEGIN_CLS(CustomIconComponent)
INHERIT(BaseComponent)
END_CLS()


BEGIN_CLS(CharacterCreationStatsComponent)
INHERIT(BaseComponent)
P_RO(Race)
P_RO(SubRace)
P_RO(field_38)
P_RO(Name)
P_RO(field_60)
P_RO(field_68)
P_RO(field_6C)
P_RO(field_70)
P_RO(field_74)
P_RO(field_78)
END_CLS()


BEGIN_CLS(DisarmableComponent)
INHERIT(BaseComponent)
P_RO(SomeTrapId)
P(CanDisarm)
END_CLS()


BEGIN_CLS(ShortRestComponent)
INHERIT(BaseComponent)
P_RO(StateId)
END_CLS()


BEGIN_CLS(IsSummonComponent)
INHERIT(BaseComponent)
P_RO(Owner_M)
END_CLS()


BEGIN_CLS(SummonContainerComponent)
INHERIT(BaseComponent)
P_REF(field_18)
P_REF(Characters)
P_REF(Items)
END_CLS()


BEGIN_CLS(StealthComponent)
INHERIT(BaseComponent)
P(SeekHiddenFlag)
P(Position)
P(SeekHiddenTimeout)
P_RO(field_2C)
END_CLS()


BEGIN_CLS(IsGlobalComponent)
INHERIT(BaseComponent)
END_CLS()


BEGIN_CLS(SavegameComponent)
INHERIT(BaseComponent)
P_RO(field_18)
END_CLS()


BEGIN_CLS(DisabledEquipmentComponent)
INHERIT(BaseComponent)
P_RO(ShapeshiftFlag)
END_CLS()


BEGIN_CLS(LootingStateComponent)
INHERIT(BaseComponent)
P_RO(Looter_M)
P_RO(State)
P_RO(field_24)
END_CLS()


BEGIN_CLS(LootComponent)
INHERIT(BaseComponent)
P_RO(Flags)
P_RO(InventoryType)
END_CLS()


BEGIN_CLS(LockComponent)
INHERIT(BaseComponent)
P_RO(Key_M)
P_RO(LockDC)
P_RO(Unlocked_M)
END_CLS()


BEGIN_CLS(SummonLifetimeComponent)
INHERIT(BaseComponent)
P_RO(Lifetime)
END_CLS()


BEGIN_CLS(InvisibilityComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_1C)
P_RO(field_28)
END_CLS()


BEGIN_CLS(IconComponent)
INHERIT(BaseComponent)
P_RO(Icon)
END_CLS()


BEGIN_CLS(HotbarContainerComponent)
INHERIT(BaseComponent)
P_REF(Containers)
P_RO(ActiveContainer)
END_CLS()


BEGIN_CLS(HotbarContainerComponent::Element)
P_RO(Item)
P_REF(SpellId)
P_RO(Passive)
END_CLS()


BEGIN_CLS(HotbarContainerComponent::Bar)
P_REF(Elements)
P_RO(SomeFlag)
END_CLS()


BEGIN_CLS(OriginTagComponent)
INHERIT(BaseComponent)
P_REF(Tags)
END_CLS()


BEGIN_CLS(OriginPassivesComponent)
INHERIT(BaseComponent)
P_REF(field_18)
END_CLS()


BEGIN_CLS(GodTagComponent)
INHERIT(BaseComponent)
P_REF(Tags)
END_CLS()


BEGIN_CLS(ClassTagComponent)
INHERIT(BaseComponent)
P_REF(Tags)
END_CLS()


BEGIN_CLS(BackgroundTagComponent)
INHERIT(BaseComponent)
P_REF(Tags)
END_CLS()


BEGIN_CLS(PassiveInfo)
P_RO(field_0)
P_RO(Passive)
END_CLS()


BEGIN_CLS(BackgroundPassivesComponent)
INHERIT(BaseComponent)
P_REF(field_18)
END_CLS()


BEGIN_CLS(GlobalShortRestDisabledComponent)
INHERIT(BaseComponent)
END_CLS()


BEGIN_CLS(GlobalLongRestDisabledComponent)
INHERIT(BaseComponent)
END_CLS()


BEGIN_CLS(StoryShortRestDisabledComponent)
INHERIT(BaseComponent)
END_CLS()


BEGIN_CLS(FleeCapabilityComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_1C)
P_RO(field_20)
END_CLS()


BEGIN_CLS(CanDoRestComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_19)
P_RO(field_1A)
P_RO(field_1B)
END_CLS()


BEGIN_CLS(ItemBoostsComponent)
INHERIT(BaseComponent)
P_REF(field_18)
END_CLS()


BEGIN_CLS(ActiveCharacterLightComponent)
INHERIT(BaseComponent)
P_RO(Light)
END_CLS()


BEGIN_CLS(AnimationSetComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_20)
P_RO(field_21)
P_RO(field_28)
P_RO(field_2C)
END_CLS()


BEGIN_CLS(AnimationBlueprintComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_20)
P_RO(field_28)
P_RO(field_30)
P_RO(field_38)
P_RO(field_40)
P_RO(field_48)
P_RO(field_49)
END_CLS()


BEGIN_CLS(CanModifyHealthComponent)
INHERIT(BaseComponent)
P_RO(Flags)
END_CLS()


BEGIN_CLS(AddedSpellsComponent::SpellInfo)
P_RO(SpellId)
P_RO(field_8)
P_RO(field_18)
P_RO(field_20)
P_RO(field_28)
P_RO(field_29)
P_RO(SomeUUID)
P_RO(field_40)
P_RO(Cooldown)
P_RO(field_44)
P_RO(field_48)
END_CLS()


BEGIN_CLS(AddedSpellsComponent)
INHERIT(BaseComponent)
P_REF(Spells)
END_CLS()


BEGIN_CLS(AvailableLevelComponent)
INHERIT(BaseComponent)
P_RO(Level)
END_CLS()


BEGIN_CLS(CanBeLootedComponent)
INHERIT(BaseComponent)
P_RO(Flags)
END_CLS()


BEGIN_CLS(CanDoActionsComponent)
INHERIT(BaseComponent)
P_RO(Flags)
END_CLS()


BEGIN_CLS(CanMoveComponent)
INHERIT(BaseComponent)
P_RO(Flags)
END_CLS()


BEGIN_CLS(CanSenseComponent)
INHERIT(BaseComponent)
P_RO(Flags)
END_CLS()


BEGIN_CLS(ConcentrationComponent::Element)
P_RO(field_0)
P_RO(field_8)
P_RO(field_10)
P_RO(field_12)
END_CLS()


BEGIN_CLS(ConcentrationComponent)
INHERIT(BaseComponent)
P_REF(field_18)
P_REF(SpellId)
END_CLS()


BEGIN_CLS(DarknessComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_19)
P_RO(field_1A)
P_RO(field_1B)
P_RO(field_1C)
P_RO(field_1D)
P_RO(field_20)
END_CLS()


BEGIN_CLS(DualWieldingComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_1A)
P_RO(field_1B)
P_RO(field_1D)
P_RO(field_1E)
END_CLS()


BEGIN_CLS(GameObjectVisualComponent)
INHERIT(BaseComponent)
P_RO(RootTemplateId)
P_RO(RootTemplateType)
P_RO(Icon)
P_RO(field_24)
P_RO(field_28)
END_CLS()


BEGIN_CLS(InventorySlotComponent)
INHERIT(BaseComponent)
P_RO(Slot)
END_CLS()


BEGIN_CLS(SpellBookCooldowns::Cooldown)
P_REF(SpellId)
P_RO(CooldownType)
P_RO(field_29)
P_RO(Cooldown)
END_CLS()


BEGIN_CLS(SpellBookCooldowns)
INHERIT(BaseComponent)
P_REF(Cooldowns)
END_CLS()


BEGIN_CLS(DisplayNameComponent)
INHERIT(BaseComponent)
P_RO(NameKey)
P_RO(UnknownKey)
P_RO(Name)
END_CLS()


BEGIN_CLS(EquipableComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(Slot_M)
END_CLS()


BEGIN_CLS(GameplayLightComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_1C)
P_RO(field_20)
P_RO(field_24)
P_RO(field_28)
END_CLS()


BEGIN_CLS(ProgressionContainerComponent)
INHERIT(BaseComponent)
P_REF(Progressions)
END_CLS()


BEGIN_CLS(ProgressionMetaComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(Progression)
P_RO(Race)
P_RO(field_40)
P_RO(field_44)
P_RO(field_48)
P_RO(Owner)
END_CLS()


BEGIN_CLS(RaceComponent)
INHERIT(BaseComponent)
P_RO(Race)
END_CLS()


BEGIN_CLS(SightComponent)
INHERIT(BaseComponent)
P_RO(Sight)
END_CLS()


BEGIN_CLS(CanTravelComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_1A)
P_RO(field_1C)
END_CLS()


BEGIN_CLS(CanBeInInventoryComponent)
INHERIT(BaseComponent)
P_RO(Flags)
END_CLS()


BEGIN_CLS(MovementComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_1C)
P_RO(field_20)
P_RO(field_24)
P_RO(field_28)
P_RO(field_2C)
END_CLS()


BEGIN_CLS(ObjectInteractionComponent)
INHERIT(BaseComponent)
P_RO(field_18)
END_CLS()


BEGIN_CLS(PathingComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_28)
P_RO(field_38)
P_RO(field_40)
P_RO(CurveUUID)
P_RO(field_48)
P_RO(field_50)
P_RO(field_58)
P_RO(field_59)
P_RO(field_5A)
P_RO(field_5B)
P_RO(field_5C)
END_CLS()


BEGIN_CLS(SteeringComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_1C)
P_RO(field_20)
P_RO(field_24)
// FIXME remainder
END_CLS()


BEGIN_CLS(CanDeflectProjectilesComponent)
INHERIT(BaseComponent)
P_RO(Flags)
END_CLS()


BEGIN_CLS(LearnedSpells)
INHERIT(BaseComponent)
P_REF(field_18)
P_REF(field_70)
END_CLS()


BEGIN_CLS(SpellAiConditions)
INHERIT(BaseComponent)
P_REF(field_18)
END_CLS()


BEGIN_CLS(ActiveSkeletonSlotsComponent)
INHERIT(BaseComponent)
P_REF(Slots)
END_CLS()


BEGIN_CLS(NetComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(NetID)
END_CLS()


BEGIN_CLS(PhysicsComponent)
INHERIT(BaseComponent)
P_RO(field_18)
END_CLS()




BEGIN_CLS(RecruitedByComponent)
INHERIT(BaseComponent)
P_RO(RecruitedBy)
END_CLS()


BEGIN_CLS(GameTimerComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_20)
P_RO(field_28)
P_RO(field_2C)
P_RO(field_30)
P_RO(field_34)
P_RO(field_38)
END_CLS()


BEGIN_CLS(ExperienceGaveOutComponent)
INHERIT(BaseComponent)
P_RO(Experience)
END_CLS()


BEGIN_CLS(ReplicationDependencyComponent)
INHERIT(BaseComponent)
P_RO(Dependency)
END_CLS()


BEGIN_CLS(IsUnsummoningComponent)
INHERIT(BaseComponent)
END_CLS()


BEGIN_CLS(FleeBlockedComponent)
INHERIT(BaseComponent)
END_CLS()


BEGIN_CLS(ActivationGroupContainerComponent::ActivationGroup)
P_RO(field_0)
P_RO(field_4)
END_CLS()


BEGIN_CLS(ActivationGroupContainerComponent)
INHERIT(BaseComponent)
P_REF(Groups)
END_CLS()


BEGIN_CLS(AnubisTagComponent)
INHERIT(BaseComponent)
P_RO(Tags)
END_CLS()


BEGIN_CLS(DialogTagComponent)
INHERIT(BaseComponent)
P_RO(Tags)
END_CLS()


BEGIN_CLS(DisplayName)
P_RO(NameKey)
P_RO(Unknown)
P_RO(Unknown2)
P_RO(Name)
END_CLS()


BEGIN_CLS(DisplayNameListComponent)
INHERIT(BaseComponent)
P_REF(Names)
P_RO(TranslatedStrings)
END_CLS()


BEGIN_CLS(IconListComponent::Icon)
P_RO(Icon)
P_RO(field_4)
END_CLS()


BEGIN_CLS(IconListComponent)
INHERIT(BaseComponent)
P_REF(Icons)
P_RO(field_30)
END_CLS()


BEGIN_CLS(PlanTagComponent)
INHERIT(BaseComponent)
P_RO(Tags)
END_CLS()


BEGIN_CLS(RaceTagComponent)
INHERIT(BaseComponent)
P_RO(Tags)
END_CLS()


BEGIN_CLS(TemplateTagComponent)
INHERIT(BaseComponent)
P_RO(Tags)
END_CLS()


BEGIN_CLS(ToggledPassivesComponent)
INHERIT(BaseComponent)
P_REF(Passives)
END_CLS()


BEGIN_CLS(BoostTagComponent)
INHERIT(BaseComponent)
P_RO(Tags)
END_CLS()


BEGIN_CLS(TriggerStateComponent)
INHERIT(BaseComponent)
P_REF(Triggers)
END_CLS()


BEGIN_CLS(SafePositionComponent)
INHERIT(BaseComponent)
P(Position)
P_RO(field_24)
END_CLS()


BEGIN_CLS(AnubisExecutorComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_20)
P_RO(field_28)
P_RO(field_29)
END_CLS()



BEGIN_CLS(StaticPhysicsComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_20)
P_RO(field_28)
P_RO(field_30)
END_CLS()


BEGIN_CLS(AnubisComponent)
INHERIT(BaseComponent)
P_RO(field_18)
//P_RO(field_38)
//P_RO(field_40)
END_CLS()


BEGIN_CLS(LeaderComponent)
INHERIT(BaseComponent)
P_REF(Followers_M)
END_CLS()


BEGIN_CLS(BreadcrumbComponent::Element)
P_RO(field_0)
P_RO(field_4)
P_RO(field_8)
P_RO(field_14)
END_CLS()


BEGIN_CLS(BreadcrumbComponent)
INHERIT(BaseComponent)
P_REF(field_18)
P_RO(field_118)
END_CLS()


BEGIN_CLS(DelayDeathCauseComponent)
INHERIT(BaseComponent)
P_RO(DelayCount)
P_RO(Blocked_M)
END_CLS()


BEGIN_CLS(PickpocketComponent::PickpocketEntry)
P_RO(field_0)
P_RO(field_8)
P_RO(field_10)
P_RO(field_14)
P_RO(field_18)
END_CLS()


BEGIN_CLS(PickpocketComponent)
INHERIT(BaseComponent)
P_REF(field_18)
END_CLS()


BEGIN_CLS(ReplicationDependencyOwnerComponent)
INHERIT(BaseComponent)
P_REF(Dependents)
END_CLS()


BEGIN_CLS(FTBParticipantComponent)
INHERIT(BaseComponent)
P_RO(field_18)
END_CLS()


BEGIN_CLS(UnsheathInfoComponent)
INHERIT(BaseComponent)
P_RO(field_18)
P_RO(field_19)
P_RO(field_1A)
P_RO(field_1B)
END_CLS()


BEGIN_CLS(ApprovalRatingsComponent)
INHERIT(BaseComponent)
P_REF(Ratings)
P_REF(field_70)
END_CLS()


BEGIN_CLS(CharacterCreationAppearanceComponent::AppearanceElement)
P_RO(Material)
P_RO(Color)
P_RO(field_20)
END_CLS()


BEGIN_CLS(CharacterCreationAppearanceComponent)
INHERIT(BaseComponent)
P_REF(Visuals)
P_REF(Elements)
P_RO(SkinColor)
P_RO(EyeColor)
P_RO(field_68)
END_CLS()
