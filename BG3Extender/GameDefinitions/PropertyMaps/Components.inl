
BEGIN_CLS(BaseComponent)
END_CLS()


BEGIN_CLS(UuidComponent)
INHERIT(BaseComponent)
P_RO(EntityUuid)
END_CLS()


BEGIN_CLS(UuidToHandleMappingComponent)
INHERIT(BaseComponent)
P(Mappings)
END_CLS()


BEGIN_CLS(TransformComponent)
INHERIT(BaseComponent)
P(Transform)
END_CLS()


BEGIN_CLS(HearingComponent)
INHERIT(BaseComponent)
P(Sight)
P(Hearing)
P(FOV)
END_CLS()


BEGIN_CLS(ActionResourcesComponent::Amount::SubAmount)
P(Amount)
P(MaxAmount)
END_CLS()


BEGIN_CLS(ActionResourcesComponent::Amount)
P(ResourceUUID)
P(ResourceId)
P(Amount)
P(MaxAmount)
P(field_28)
P(field_30)
P(SubAmounts)
P(SubAmountsValid)
END_CLS()


BEGIN_CLS(ActionResourcesComponent)
INHERIT(BaseComponent)
P(Resources)
END_CLS()


BEGIN_CLS(SpellBookEntry)
P(Id)
P(SpellUUID)
P(field_38)
P(CooldownType)
P(field_41)
P(field_42)
P(SpellCastingAbility)
END_CLS()


BEGIN_CLS(SpellBookComponent)
INHERIT(BaseComponent)
P(Spells)
END_CLS()


BEGIN_CLS(SurfacePathInfluence)
P(SurfaceType)
P(IsCloud)
P(Influence)
END_CLS()


BEGIN_CLS(SurfacePathInfluencesComponent)
INHERIT(BaseComponent)
P(PathInfluences)
END_CLS()


BEGIN_CLS(UseComponent)
INHERIT(BaseComponent)
P(Charges)
P(MaxCharges)
P(ItemUseType)
P(ItemComboFlag1)
P(ItemComboFlag2)
P(Boosts)
// TODO - map other fields
END_CLS()


BEGIN_CLS(WieldingComponent)
INHERIT(BaseComponent)
P(Owner)
END_CLS()


BEGIN_CLS(ActionResourceConsumeMultiplierBoostComponent)
INHERIT(BaseComponent)
P(ResourceUUID)
END_CLS()


BEGIN_CLS(SpellContainerComponent::Spell)
P(SpellId)
P(ItemHandle)
P(SelectionType)
P(field_29)
P(SpellUUID)
P(SpellCastingAbility)
P(CooldownType)
P(field_44)
P(field_48)
END_CLS()


BEGIN_CLS(SpellContainerComponent)
INHERIT(BaseComponent)
P(Spells)
END_CLS()


BEGIN_CLS(TagComponent)
INHERIT(BaseComponent)
P(Tags)
END_CLS()


BEGIN_CLS(OsirisTagComponent)
INHERIT(BaseComponent)
P(Tags)
END_CLS()


BEGIN_CLS(SpellBookPreparesComponent)
INHERIT(BaseComponent)
P(PreparedSpells)
P(field_30)
P(field_88)
END_CLS()


BEGIN_CLS(RelationComponent)
INHERIT(BaseComponent)
P(field_18)
P(field_28)
P(field_38)
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


BEGIN_CLS(ActiveComponent)
INHERIT(BaseComponent)
END_CLS()


BEGIN_CLS(EocLevelComponent)
INHERIT(BaseComponent)
P(Level)
END_CLS()


BEGIN_CLS(ClassesComponent::Class)
P(ClassUUID)
P(SubClassUUID)
P(Level)
END_CLS()


BEGIN_CLS(ClassesComponent)
INHERIT(BaseComponent)
P(Classes)
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
P(Stats)
END_CLS()


BEGIN_CLS(DataComponent)
INHERIT(BaseComponent)
P(Weight)
P(StatsId)
P(Flags)
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
P(Resistances)
P(field_38)
P(Hp)
P(field_40)
P(MaxHp)
P(TemporaryHp)
P(MaxTemporaryHp)
P(AC)
P(PerDamageTypeHealthThresholds)
P(IsInvulnerable)
END_CLS()


BEGIN_CLS(DifficultyCheckComponent)
INHERIT(BaseComponent)
P(SpellDC)
P(field_4)
P(field_8)
END_CLS()


BEGIN_CLS(ObjectSizeComponent)
INHERIT(BaseComponent)
P(Size)
P(field_1)
END_CLS()


BEGIN_CLS(StatsComponent)
INHERIT(BaseComponent)
P(field_0)
P(Abilities)
P(AbilityModifiers)
P(Skills)
P(ProficiencyBonus)
P(SpellCastingAbility)
P(field_8C)
P(field_90)
P(field_94)
P(field_98)
P(UnarmedAttackAbility)
P(RangedAttackAbility)
END_CLS()


BEGIN_CLS(StatusImmunitiesComponent)
INHERIT(BaseComponent)
P(PersonalStatusImmunities)
END_CLS()


BEGIN_CLS(ValueComponent)
INHERIT(BaseComponent)
P(Value)
P(Rarity)
P(Unique)
END_CLS()


BEGIN_CLS(WeaponComponent)
INHERIT(BaseComponent)
P(Rolls)
P(Rolls2)
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
P(CombatHandle)
P(CombatGroupId)
P(field_C)
P(field_10)
P(field_18)
END_CLS()


BEGIN_CLS(CombatStateComponent)
INHERIT(BaseComponent)
P_RO(MyGuid)
P(Initiatives)
P(Participants)
P(field_98)
P(field_A0)
P_RO(Level)
P(field_AC)
P(field_B0)
P(field_B8)
P(field_D0)
END_CLS()


BEGIN_CLS(TurnOrderComponent::ParticipantHandleInfo)
P(Entity)
P(Initiative)
END_CLS()


BEGIN_CLS(TurnOrderComponent::Participant)
P(Handles)
P(Participant)
P(field_28)
P(Initiative)
P(field_30)
END_CLS()


BEGIN_CLS(TurnOrderComponent)
INHERIT(BaseComponent)
P(TurnOrderIndices)
P(Participants)
P(TurnOrderIndices2)
P(Participants2)
P(field_78)
P(field_7C)
END_CLS()


BEGIN_CLS(TurnBasedComponent)
INHERIT(BaseComponent)
P(field_0)
P(field_8)
P(field_C)
P(field_10)
P(field_14)
P(field_15)
P(field_16)
P(field_18)
P(field_20)
P(field_28)
P(Combat)
END_CLS()


BEGIN_CLS(BackgroundComponent)
INHERIT(BaseComponent)
P(Background)
END_CLS()


BEGIN_CLS(GodComponent)
INHERIT(BaseComponent)
P(God)
END_CLS()


BEGIN_CLS(LevelUpUpgrades::AbilityData)
P(field_0)
P(Feat)
P(Class)
P(field_28)
P(field_30)
P(Ability)
P(Array_b8)
P(field_60)
P(field_80)
END_CLS()


BEGIN_CLS(LevelUpUpgrades::SkillData)
P(field_0)
P(field_8)
P(field_18)
P(field_28)
P(field_30)
P(Skill)
P(Array_b8)
P(field_60)
P(field_80)
END_CLS()


BEGIN_CLS(LevelUpUpgrades::SpellData::StringPair)
P(A)
P(B)
END_CLS()



BEGIN_CLS(LevelUpUpgrades::SpellData)
P(field_0)
P(Race)
P(field_18)
P(field_28)
P(field_30)
P(Spell)
P(Array_FS)
P(Array_FS2)
P(field_78)
END_CLS()


BEGIN_CLS(LevelUpUpgrades::LevelUpDataInner4)
P(field_0)
P(field_8)
P(Class)
P(field_28)
P(field_30)
P(field_38)
P(Array_FS)
P(field_60)
P(field_80)
END_CLS()


BEGIN_CLS(LevelUpUpgrades)
P(Abilities)
P(Skills)
P(Spells)
P(field_48)
END_CLS()


BEGIN_CLS(LevelUpData3)
P(field_0)
P(field_4)
P(field_10)
END_CLS()


BEGIN_CLS(LevelUpData)
P(Class)
P(SubClass)
P(Feat)
P(Abilities)
P(Upgrades)
P(field_B0)
END_CLS()


BEGIN_CLS(LevelUpComponent)
INHERIT(BaseComponent)
P(field_18)
END_CLS()


BEGIN_CLS(PlayerPrepareSpellComponent::Spell)
P(field_0)
P(field_8)
P(field_10)
END_CLS()


BEGIN_CLS(PlayerPrepareSpellComponent)
INHERIT(BaseComponent)
P(Spells)
P(field_30)
END_CLS()


BEGIN_CLS(CCPrepareSpellComponent)
INHERIT(BaseComponent)
P(Spells)
END_CLS()


BEGIN_CLS(SpellCastComponent)
INHERIT(BaseComponent)
P(field_18)
P(field_1C)
P(field_20)
P(field_28)
P(field_2C)
P(field_30)
P(field_34)
P(field_38)
P(field_39)
P(field_3A)
P(field_3B)
END_CLS()


BEGIN_CLS(FloatingComponent)
INHERIT(BaseComponent)
P(field_18)
P(field_1C)
END_CLS()


BEGIN_CLS(VoiceComponent)
INHERIT(BaseComponent)
P(Voice)
END_CLS()


BEGIN_CLS(CustomIconComponent)
INHERIT(BaseComponent)
END_CLS()


BEGIN_CLS(CharacterCreationStatsComponent)
INHERIT(BaseComponent)
P(Race)
P(SubRace)
P(field_20)
P(field_21)
P(Name)
P(Abilities)
P(field_5C)
END_CLS()


BEGIN_CLS(DisarmableComponent)
INHERIT(BaseComponent)
P(SomeTrapId)
P(CanDisarm)
END_CLS()


BEGIN_CLS(ShortRestComponent)
INHERIT(BaseComponent)
P(StateId)
END_CLS()


BEGIN_CLS(IsSummonComponent)
INHERIT(BaseComponent)
P(Owner_M)
END_CLS()


BEGIN_CLS(SummonContainerComponent)
INHERIT(BaseComponent)
P(field_18)
P(Characters)
P(Items)
END_CLS()


BEGIN_CLS(StealthComponent)
INHERIT(BaseComponent)
P(SeekHiddenFlag)
P(Position)
P(SeekHiddenTimeout)
P(field_2C)
END_CLS()


BEGIN_CLS(IsGlobalComponent)
INHERIT(BaseComponent)
END_CLS()


BEGIN_CLS(SavegameComponent)
INHERIT(BaseComponent)
P_RO(field_0)
END_CLS()


BEGIN_CLS(DisabledEquipmentComponent)
INHERIT(BaseComponent)
P(ShapeshiftFlag)
END_CLS()


BEGIN_CLS(LootingStateComponent)
INHERIT(BaseComponent)
P(Looter_M)
P(State)
P(field_24)
END_CLS()


BEGIN_CLS(LootComponent)
INHERIT(BaseComponent)
P(Flags)
P(InventoryType)
END_CLS()


BEGIN_CLS(LockComponent)
INHERIT(BaseComponent)
P(Key_M)
P(LockDC)
P(Unlocked_M)
END_CLS()


BEGIN_CLS(SummonLifetimeComponent)
INHERIT(BaseComponent)
P(Lifetime)
END_CLS()


BEGIN_CLS(InvisibilityComponent)
INHERIT(BaseComponent)
P(field_18)
P(field_1C)
P(field_28)
END_CLS()


BEGIN_CLS(IconComponent)
INHERIT(BaseComponent)
P(Icon)
END_CLS()


BEGIN_CLS(HotbarContainerComponent)
INHERIT(BaseComponent)
P(Containers)
P(ActiveContainer)
END_CLS()


BEGIN_CLS(HotbarContainerComponent::Element)
P(Item)
P(SpellId)
P(Passive)
P(field_34)
P(field_38)
END_CLS()


BEGIN_CLS(HotbarContainerComponent::Bar)
P(Index)
P(field_1)
P(Elements)
P(field_18)
P(field_1C)
P(field_20)
END_CLS()


BEGIN_CLS(OriginTagComponent)
INHERIT(BaseComponent)
P(Tags)
END_CLS()


BEGIN_CLS(OriginPassivesComponent)
INHERIT(BaseComponent)
P(field_18)
END_CLS()


BEGIN_CLS(ClassTagComponent)
INHERIT(BaseComponent)
P(Tags)
END_CLS()


BEGIN_CLS(BackgroundTagComponent)
INHERIT(BaseComponent)
P(Tags)
END_CLS()


BEGIN_CLS(PassiveInfo)
P(field_0)
P(Passive)
END_CLS()


BEGIN_CLS(BackgroundPassivesComponent)
INHERIT(BaseComponent)
P(field_18)
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
P(field_18)
P(field_1C)
P(field_20)
END_CLS()


BEGIN_CLS(CanDoRestComponent)
INHERIT(BaseComponent)
P(field_18)
P(field_19)
P(field_1A)
P(field_1B)
END_CLS()


BEGIN_CLS(ItemBoostsComponent)
INHERIT(BaseComponent)
P(field_18)
END_CLS()


BEGIN_CLS(ActiveCharacterLightComponent)
INHERIT(BaseComponent)
P(Light)
END_CLS()


BEGIN_CLS(CanModifyHealthComponent)
INHERIT(BaseComponent)
P(Flags)
END_CLS()


BEGIN_CLS(AddedSpellsComponent)
INHERIT(BaseComponent)
P(Spells)
END_CLS()


BEGIN_CLS(AvailableLevelComponent)
INHERIT(BaseComponent)
P(Level)
END_CLS()


BEGIN_CLS(CanBeLootedComponent)
INHERIT(BaseComponent)
P(Flags)
END_CLS()


BEGIN_CLS(CanDoActionsComponent)
INHERIT(BaseComponent)
P(Flags)
END_CLS()


BEGIN_CLS(CanMoveComponent)
INHERIT(BaseComponent)
P(Flags)
END_CLS()


BEGIN_CLS(CanSenseComponent)
INHERIT(BaseComponent)
P(Flags)
END_CLS()


BEGIN_CLS(ConcentrationComponent::Element)
P(field_0)
P(field_8)
P(field_10)
P(field_18)
P(field_1A)
END_CLS()


BEGIN_CLS(ConcentrationComponent)
INHERIT(BaseComponent)
P(field_0)
P(field_8)
P(SpellId)
END_CLS()


BEGIN_CLS(DarknessComponent)
INHERIT(BaseComponent)
P(field_18)
P(field_19)
P(field_1A)
P(field_1B)
P(field_1C)
P(field_1D)
P(field_20)
END_CLS()


BEGIN_CLS(DualWieldingComponent)
INHERIT(BaseComponent)
P(field_18)
P(field_1A)
P(field_1B)
P(field_1D)
P(field_1E)
END_CLS()


BEGIN_CLS(GameObjectVisualData)
P(field_58)
P(field_68)
P(field_78)
P(field_88)
P(field_98)
P(Visuals)
P(Elements)
P(field_C8)
END_CLS()


BEGIN_CLS(GameObjectVisualData::AppearanceElement)
P(Material)
P(Color)
P(field_20)
P(field_24)
P(field_28)
END_CLS()


BEGIN_CLS(GameObjectVisualComponent)
INHERIT(BaseComponent)
P(RootTemplateId)
P(RootTemplateType)
P(Icon)
P(field_24)
P(field_28)
P(VisualData)
P(field_F0)
END_CLS()


BEGIN_CLS(SpellBookCooldownsComponent::Cooldown)
P(SpellId)
P(CooldownType)
P(field_29)
P(Cooldown)
END_CLS()


BEGIN_CLS(SpellBookCooldownsComponent)
INHERIT(BaseComponent)
P(Cooldowns)
END_CLS()


BEGIN_CLS(DisplayNameComponent)
INHERIT(BaseComponent)
P(NameKey)
P(UnknownKey)
P(Name)
END_CLS()


BEGIN_CLS(EquipableComponent)
INHERIT(BaseComponent)
P(field_18)
P(Slot_M)
END_CLS()


BEGIN_CLS(GameplayLightComponent)
INHERIT(BaseComponent)
P(field_18)
P(field_1C)
P(field_20)
P(field_24)
P(field_28)
END_CLS()


BEGIN_CLS(ProgressionContainerComponent)
INHERIT(BaseComponent)
P(Progressions)
END_CLS()


BEGIN_CLS(ProgressionMetaComponent)
INHERIT(BaseComponent)
P(field_18)
P(Progression)
P(Race)
P(field_40)
P(field_44)
P(field_48)
P(Owner)
END_CLS()


BEGIN_CLS(RaceComponent)
INHERIT(BaseComponent)
P(Race)
END_CLS()


BEGIN_CLS(CanTravelComponent)
INHERIT(BaseComponent)
P(field_18)
P(field_1A)
P(field_1C)
END_CLS()


BEGIN_CLS(MovementComponent)
INHERIT(BaseComponent)
P(field_18)
P(field_1C)
P(field_20)
P(field_24)
P(field_28)
P(field_2C)
END_CLS()


BEGIN_CLS(ObjectInteractionComponent)
INHERIT(BaseComponent)
P(field_18)
END_CLS()


BEGIN_CLS(PathingComponent)
INHERIT(BaseComponent)
P(field_18)
P(field_28)
P(field_38)
P(field_40)
P(CurveUUID)
P(field_48)
P(field_50)
P(field_58)
P(field_59)
P(field_5A)
P(field_5B)
P(field_5C)
END_CLS()


BEGIN_CLS(SteeringComponent)
INHERIT(BaseComponent)
P(field_0)
P(field_C)
P(field_10)
P(field_14)
P(field_18)
P(field_1C)
END_CLS()


BEGIN_CLS(CanDeflectProjectilesComponent)
INHERIT(BaseComponent)
P(Flags)
END_CLS()


BEGIN_CLS(LearnedSpellsComponent)
INHERIT(BaseComponent)
P(field_18)
P(field_70)
END_CLS()


BEGIN_CLS(SpellAiConditionsComponent)
INHERIT(BaseComponent)
P(field_18)
END_CLS()


BEGIN_CLS(ActiveSkeletonSlotsComponent)
INHERIT(BaseComponent)
P(Slots)
END_CLS()


BEGIN_CLS(NetComponent)
INHERIT(BaseComponent)
P(field_18)
P(NetID)
END_CLS()


BEGIN_CLS(PhysicsComponent)
INHERIT(BaseComponent)
P(field_18)
END_CLS()




BEGIN_CLS(RecruitedByComponent)
INHERIT(BaseComponent)
P(RecruitedBy)
END_CLS()


BEGIN_CLS(GameTimerComponent)
INHERIT(BaseComponent)
P(field_18)
P(field_20)
P(field_28)
P(field_2C)
P(field_30)
P(field_34)
P(field_38)
END_CLS()


BEGIN_CLS(ExperienceGaveOutComponent)
INHERIT(BaseComponent)
P(Experience)
END_CLS()


BEGIN_CLS(ReplicationDependencyComponent)
INHERIT(BaseComponent)
P(Dependency)
END_CLS()


BEGIN_CLS(IsUnsummoningComponent)
INHERIT(BaseComponent)
END_CLS()


BEGIN_CLS(FleeBlockedComponent)
INHERIT(BaseComponent)
END_CLS()


BEGIN_CLS(ActivationGroupContainerComponent::ActivationGroup)
P(field_0)
P(field_4)
END_CLS()


BEGIN_CLS(ActivationGroupContainerComponent)
INHERIT(BaseComponent)
P(Groups)
END_CLS()


BEGIN_CLS(AnubisTagComponent)
INHERIT(BaseComponent)
P(Tags)
END_CLS()


BEGIN_CLS(DialogTagComponent)
INHERIT(BaseComponent)
P(Tags)
END_CLS()


BEGIN_CLS(DisplayName)
P(NameKey)
P(Unknown)
P(Unknown2)
P(Name)
END_CLS()


BEGIN_CLS(DisplayNameListComponent)
INHERIT(BaseComponent)
P(Names)
P(TranslatedStrings)
END_CLS()


BEGIN_CLS(IconListComponent::Icon)
P(Icon)
P(field_4)
END_CLS()


BEGIN_CLS(IconListComponent)
INHERIT(BaseComponent)
P(Icons)
P(field_30)
END_CLS()


BEGIN_CLS(RaceTagComponent)
INHERIT(BaseComponent)
P(Tags)
END_CLS()


BEGIN_CLS(TemplateTagComponent)
INHERIT(BaseComponent)
P(Tags)
END_CLS()


BEGIN_CLS(ToggledPassivesComponent)
INHERIT(BaseComponent)
P(Passives)
END_CLS()


BEGIN_CLS(BoostTagComponent)
INHERIT(BaseComponent)
P(Tags)
END_CLS()


BEGIN_CLS(SafePositionComponent)
INHERIT(BaseComponent)
P(Position)
P(field_24)
END_CLS()


BEGIN_CLS(AnubisExecutorComponent)
INHERIT(BaseComponent)
P(field_18)
P(field_20)
P(field_28)
P(field_29)
END_CLS()



BEGIN_CLS(StaticPhysicsComponent)
INHERIT(BaseComponent)
P(field_18)
P(field_20)
P(field_28)
P(field_30)
END_CLS()


BEGIN_CLS(LeaderComponent)
INHERIT(BaseComponent)
P(Followers_M)
END_CLS()


BEGIN_CLS(BreadcrumbComponent::Element)
P(field_0)
P(field_4)
P(field_8)
P(field_14)
END_CLS()


BEGIN_CLS(BreadcrumbComponent)
INHERIT(BaseComponent)
P(field_18)
P(field_118)
END_CLS()


BEGIN_CLS(DelayDeathCauseComponent)
INHERIT(BaseComponent)
P(DelayCount)
P(Blocked_M)
END_CLS()


BEGIN_CLS(PickpocketComponent::PickpocketEntry)
P(field_0)
P(field_8)
P(field_10)
P(field_14)
P(field_18)
END_CLS()


BEGIN_CLS(PickpocketComponent)
INHERIT(BaseComponent)
P(field_18)
END_CLS()


BEGIN_CLS(ReplicationDependencyOwnerComponent)
INHERIT(BaseComponent)
P(Dependents)
END_CLS()


BEGIN_CLS(FTBParticipantComponent)
INHERIT(BaseComponent)
P(field_18)
END_CLS()


BEGIN_CLS(ApprovalRatingsComponent)
INHERIT(BaseComponent)
P(Ratings)
P(field_70)
END_CLS()


BEGIN_CLS(CharacterCreationAppearanceComponent)
INHERIT(BaseComponent)
P(Visuals)
P(Elements)
P(field_20)
P(SkinColor)
P(EyeColor)
P(field_50)
P(field_60)
END_CLS()


BEGIN_CLS(inventory::DataComponent)
INHERIT(BaseComponent)
P(field_0)
P(Flags)
END_CLS()


BEGIN_CLS(inventory::OwnerComponent)
INHERIT(BaseComponent)
P(Inventories)
P(PrimaryInventory)
END_CLS()


BEGIN_CLS(inventory::ContainerComponent::Item)
P(Item)
P(field_8)
END_CLS()


BEGIN_CLS(inventory::ContainerComponent)
INHERIT(BaseComponent)
P(Items)
END_CLS()


BEGIN_CLS(inventory::MemberComponent)
INHERIT(BaseComponent)
P(Inventory)
P(EquipmentSlot)
END_CLS()
