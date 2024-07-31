BEGIN_ENUM(ExtSystemType, uint32_t)
	E(UISystem)
	E(PickingHelperManager)
	E(Max)
END_ENUM()

BEGIN_ENUM(ExtResourceManagerType, uint32_t)
	E(AbilityDistributionPreset)
	E(AbilityList)
	E(ActionResourceGroup)
	E(ActionResource)
	E(ApprovalRating)
	E(AreaLevelOverride)
	E(AvatarContainerTemplate)
	E(BackgroundGoal)
	E(Background)
	E(CalendarDayRange)
	E(CampChestTemplate)

	E(AbilityDefaultValues)
	E(EquipmentDefaultValues)
	E(FeatDefaultValues)
	E(PassiveDefaultValues)
	E(PreparedSpellDefaultValues)
	E(SkillDefaultValues)
	E(SpellDefaultValues)

	E(CharacterCreationAccessorySet)
	E(CharacterCreationAppearanceMaterial)
	E(CharacterCreationAppearanceVisual)
	E(CharacterCreationEquipmentIcons)
	E(CharacterCreationEyeColor)
	E(CharacterCreationIconSettings)
	E(CharacterCreationHairColor)
	E(CharacterCreationMaterialOverride)
	E(CharacterCreationPassiveAppearance)
	E(CharacterCreationPreset)
	E(CharacterCreationSharedVisual)
	E(CharacterCreationSkinColor)
	E(CharacterCreationVOLine)

	E(CinematicArenaFrequencyGroup)
	E(ClassDescription)
	E(ColorDefinition)
	E(CompanionPreset)
	E(CustomDice)
	E(ConditionError)
	E(DeathTypeEffect)
	E(DifficultyClass)
	E(DisturbanceProperty)
	E(DLC)
	E(EncumbranceType)
	E(EquipmentList)
	E(EquipmentType)
	E(ExperienceReward)
	E(Faction)
	E(FeatSoundState)
	E(Feat)
	E(FeatDescription)
	E(FlagSoundState)
	E(God)
	E(GoldReward)
	E(Gossip)
	E(FixedHotBarSlot)
	E(ItemThrowParams)
	E(ItemWallTemplate)
	E(TrajectoryRules)
	E(LevelMap)
	E(LimbsMapping)
	E(LongRestCost)
	E(ManagedStatusVFX)
	E(MultiEffectInfo)
	E(OriginIntroEntity)
	E(Origin)
	E(PassiveList)
	E(PassiveVFX)
	E(ProgressionDescription)
	E(Progression)
	E(ProjectileDefault)
	E(Race)
	E(RandomCastOutcome)
	E(SkillList)
	E(SpellSoundTrajectory)
	E(SpellList)
	E(StatusSoundState)
	E(SurfaceCursorMessage)
	E(TadpolePower)
	E(TagSoundState)
	E(TooltipExtraText)
	E(TooltipUpcastDescription)
	E(TutorialEntries)
	E(TutorialModalEntries)
	E(TutorialUnifiedEntry)
	E(TutorialEvent)
	E(VFX)
	E(Voice)
	E(WeaponAnimationSet)
	E(WeightCategory)
	E(ShapeshiftRule)
	E(AnimationSetPriority)
	E(SpellMetaCondition)
	E(ScriptMaterialParameterOverride)
	E(ScriptMaterialPresetOverride)
	E(VisualLocatorAttachment)
	E(Flag)
	E(Tag)
	E(RulesetModifier)
	E(RulesetModifierOption)
	E(Ruleset)
	E(RulesetValue)
	E(RulesetSelectionPreset)
	E(Max)
END_ENUM()

BEGIN_ENUM(ExtComponentType, uint32_t)
	E(ActionResources)
	E(Armor)
	E(ArmorSetState)
	E(BaseHp)
	E(Bound)
	E(IsCharacter)
	E(Data)
	E(Detached)
	E(Experience)
	E(Health)
	E(Resistances)
	E(Hearing)
	E(SpellBook)
	E(Stats)
	E(StatusImmunities)
	E(SurfacePathInfluences)
	E(Use)
	E(Value)
	E(Weapon)
	E(Wielding)
	E(CustomStats)
	E(BoostCondition)
	E(BoostsContainer)
	E(ServerBoostBase)
	E(ServerStatusBoostsProcessed)
	E(ActionResourceConsumeMultiplierBoost)
	E(Tag)
	E(SpellBookPrepares)
	E(Transform)
	E(BoostInfo)
	E(Relation)
	E(Faction)
	E(CanInteract)
	E(CanSpeak)
	E(Origin)
	E(Level)
	E(EocLevel)
	E(Classes)
	E(MaterialParameterOverride)
	E(OffStage)
	E(PickingState)
	E(Player)
	E(SimpleCharacter)
	E(Speaker)
	E(WeaponSet)
	E(DifficultyCheck)
	E(AttributeFlags)
	E(BodyType)
	E(ObjectSize)
	E(BaseStats)
	E(Expertise)
	E(HealBlock)
	E(Ruleset)
	E(RulesetModifiers)

	// Stats
	E(ServerBaseData)
	E(ServerBaseSize)
	E(ServerBaseStats)
	E(ServerBaseWeapon)
	E(ServerBaseProficiency)
	E(ServerProficiencyGroupStats)

	// Shapeshift
	E(ShapeshiftState)
	E(ShapeshiftHealthReservation)
	E(ServerShapeshiftStates)

	// Animations
	E(AnimationWaterfall)
	E(DynamicAnimationTags)
	E(TemplateAnimationSetOverride)

	// Passives
	E(PassiveContainer)
	E(Passive)
	E(PassiveUsageCount)

	// Item components
	E(ItemDestroyed)
	E(ItemDye)
	E(MapMarkerStyle)
	E(ItemPortal)
	E(IsDoor)
	E(ExamineDisabled)
	E(ItemHasMoved)
	E(HasOpened)
	E(IsGold)
	E(IsItem)
	E(NewInInventory)
	E(ShouldDestroyOnSpellCast)

	// Item template components
	E(ActionType)
	E(ItemCanMove)
	E(ClimbOn)
	E(ItemTemplateDestroyed)
	E(InteractionDisabled)
	E(IsStoryItem)
	E(Ladder)
	E(WalkOn)
	E(UseAction)

	// Ownership
	E(OwnedAsLoot)
	E(OwneeCurrent)
	E(ServerDynamicLayerOwner)
	E(ServerIsCurrentOwner)
	E(ServerIsLatestOwner)
	E(ServerIsPreviousLatestOwner)
	E(ServerIsPreviousOwner)
	E(ServerIsOriginalOwner)
	E(ServerOwneeHistory)
	E(ServerOwneeRequest)

	// Inventory components
	E(InventoryMember)
	E(InventoryData)
	E(InventoryOwner)
	E(InventoryIsOwned)
	E(InventoryContainer)
	E(InventoryMemberTransform)
	E(InventoryStack)
	E(InventoryStackMember)
	E(InventoryWeight)
	E(Wielded)
	E(CanBeInInventory)
	E(CannotBePickpocketed)
	E(CannotBeTakenOut)
	E(DropOnDeathBlocked)
	E(NewItemsInside)
	E(NonTradable)

	E(ServerInventoryContainerData)
	E(ServerInventoryGroupCheck)
	E(ServerShapeshiftEquipmentHistory)
	E(CharacterHasGeneratedTradeTreasure)
	E(ServerInventoryIsReplicatedWith)
	E(ItemHasGeneratedTreasure)
	E(ReadyToBeAddedToInventory)

	E(InventoryPropertyCanBePickpocketed)
	E(ScriptPropertyCanBePickpocketed)
	E(InventoryPropertyIsDroppedOnDeath)
	E(ScriptPropertyIsDroppedOnDeath)
	E(InventoryPropertyIsTradable)
	E(ScriptPropertyIsTradable)

	// Party
	E(PartyView)
	E(PartyComposition)
	E(PartyPortals)
	E(PartyRecipes)
	E(PartyWaypoints)
	E(PartyMember)
	E(PartyFollower)
	E(CurrentlyFollowingParty)
	E(BlockFollow)
	E(Recruiter)
	E(ServerUserSnapshot)

	// Escort
	E(EscortFollower)
	E(EscortLeader)
	E(EscortLeaderPriority)
	E(EscortMember)
	E(EscortStragglersTracker)
	E(EscortHasStragglers)

	// Camp components
	E(CampChest)
	E(CampEndTheDayState)
	E(CampQuality)
	E(CampSettings)
	E(CampSupply)
	E(CampTotalSupplies)
	E(CampPresence)

	// Combat components
	E(IsInCombat)
	E(CombatParticipant)
	E(CombatState)
	E(TurnBased)
	E(TurnOrder)
	E(IsCombatPaused)
	E(ThreatRange)

	// Death
	E(Death)
	E(DeathState)
	E(DeadByDefault)
	E(ServerKiller)
	E(ServerDeathState)
	E(ServerDeathContinue)

	// Hit
	E(HitTarget)
	E(HitAttacker)
	E(HitWeapon)
	E(HitThrownObject)
	E(HitProxy)
	E(HitProxyOwner)
	E(HitReaction)

	// Identity
	E(Identity)
	E(IdentityState)
	E(OriginalIdentity)

	// Server combat components
	E(ServerCanStartCombat)
	E(ServerCombatGroupMapping)
	E(ServerEnterRequest)
	E(ServerFleeBlocked)
	E(ServerImmediateJoin)

	// Ai
	E(ServerAiInterestedInItems)
	E(ServerAiInterestingItem)
	E(ServerAiArchetype)
	E(ServerAiModifiers)

	// Spell components
	E(SpellContainer)
	E(PlayerPrepareSpell)
	E(CCPrepareSpell)
	E(SpellModificationContainer)
	E(AddedSpells)
	E(SpellBookCooldowns)
	E(LearnedSpells)
	E(SpellAiConditions)
	E(OnDamageSpells)

	// Spell casting
	E(SpellCastAnimationInfo)
	E(SpellCastCache)
	E(SpellCastCanBeTargeted)
	E(SpellCastIsCasting)
	E(SpellCastInterruptResults)
	E(SpellCastMovement)
	E(SpellCastRolls)
	E(SpellCastState)
	E(SpellSyncTargeting)

	// Server spell casting
	E(ServerSpellCastHitDelay)
	E(ServerSpellCastResponsible)
	E(ServerSpellClientInitiated)
	E(ServerSpellExternals)
	E(ServerSpellHitRegister)
	E(ServerSpellInterruptRequests)
	E(ServerSpellInterruptResults)
	E(ServerSpellCastState)
	E(ServerSpellCastCache)
	E(ServerSpellCastInterrupt)

	// Interrupts
	E(InterruptActionState)
	E(InterruptConditionallyDisabled)
	E(InterruptContainer)
	E(InterruptData)
	E(InterruptDecision)
	E(InterruptPreferences)
	E(InterruptPrepared)
	E(InterruptZone)
	E(InterruptZoneParticipant)
	E(InterruptZoneSource)

	// Server interrupts
	E(ServerInterruptAddRemoveRequests)
	E(ServerInterruptActionRequests)
	E(ServerInterruptZoneRequests)
	E(ServerInterruptInitialParticipants)
	E(ServerInterruptTurnOrderInZone)
	E(ServerInterruptDataSingleton)

	// Sight
	E(Sight)
	E(SightData)
	E(SightEntityViewshed)

	// Status
	E(StatusContainer)
	E(StatusCause)
	E(StatusID)
	E(StatusIncapacitated)
	E(StatusLifetime)
	E(StatusVisualDisabled)

	E(ServerStatusCause)
	E(ServerStatus)
	E(ServerStatusOwnership)
	E(ServerStatusDifficultyModifiers)
	E(ServerStatusUnique)
	E(ServerStatusPerforming)
	E(ServerStatusActive)
	E(ServerStatusAddedFromSaveLoad)
	E(ServerStatusAura)

	// Tadpole
	E(TadpoleTreeState)
	E(TadpolePowers)
	E(Tadpoled)
	E(HalfIllithid)
	E(FullIllithid)

	// Character creation
	E(CCCompanionDefinition)
	E(CCDefinitionCommon)
	E(CCFullRespecDefinition)
	E(CCRespecDefinition)
	E(CCChangeAppearanceDefinition)
	E(CCCharacterDefinition)
	E(CCSessionCommon)
	E(CCState)
	E(CCLevelUp)
	E(CCLevelUpDefinition)
	E(CCFullRespec)
	E(CCRespec)
	E(CCCreation)
	E(CCDefinitionLevelUp)

	E(ClientCCDefinitionState)
	E(ClientCCDefinitionStateEx)
	E(ClientCCBaseDefinitionState)
	E(ClientCCCompanionDefinition)
	E(ClientCCLevelUpDefinition)
	E(ClientCCChangeAppearanceDefinition)
	E(ClientCCFullRespecDefinition)
	E(ClientCCDummyDefinition)

	E(ServerCCAppearanceVisualTag)
	E(ServerCCEquipmentSetRequest)
	E(ServerCCGod)
	E(ServerCCUpdates)
	E(ServerCCIsCustom)

	// Experimental components
	E(Background)
	E(God)
	E(Proficiency)
	E(ProficiencyGroup)
	E(LevelUp)
	E(Floating)
	E(Voice)
	E(CustomIcon)
	E(CharacterCreationStats)
	E(Disarmable)
	E(ShortRest)
	E(IsSummon)
	E(SummonContainer)
	E(Stealth)
	E(ClientControl)
	E(IsGlobal)
	E(Savegame)
	E(DisabledEquipment)
	E(LootingState)
	E(Loot)
	E(Lock)
	E(Key)
	E(SummonLifetime)
	E(Invisibility)
	E(Icon)
	E(HotbarContainer)
	E(HotbarDecks)
	E(OriginTag)
	E(OriginPassives)
	E(ClassTag)
	E(BackgroundTag)
	E(OriginAppearanceTag)
	E(VoiceTag)
	E(GodTag)
	E(BackgroundPassives)
	E(GlobalShortRestDisabled)
	E(GlobalLongRestDisabled)
	E(StoryShortRestDisabled)
	E(FleeCapability)
	E(CanDoRest)
	E(IsInTurnBasedMode)
	E(ItemBoosts)
	E(ActiveCharacterLight)
	E(Visual)
	E(ClientEquipmentVisuals)
	E(AnimationSet)
	E(AnimationBlueprint)
	E(CanModifyHealth)
	E(AvailableLevel)
	E(CanBeLooted)
	E(CanBeDisarmed)
	E(CanTriggerRandomCasts)
	E(CanDoActions)
	E(ActionUseConditions)
	E(CanMove)
	E(CanSense)
	E(Concentration)
	E(Darkness)
	E(DualWielding)
	E(GameObjectVisual)
	E(AppearanceOverride)
	E(CharacterCreationTemplateOverride)
	E(GravityDisabled)
	E(GravityDisabledUntilMoved)
	E(CanBeWielded)
	E(Avatar)
	E(HasExclamationDialog)
	E(Trader)
	E(Ambushing)
	E(InteractionFilter)
	E(DisplayName)
	E(CustomName)
	E(Equipable)
	E(GameplayLight)
	E(Race)
	E(CanTravel)
	E(Movement)
	E(ObjectInteraction)
	E(StaticPhysics)
	E(Pathing)
	E(Steering)
	E(CanDeflectProjectiles)
	E(ActiveSkeletonSlots)
	E(Net)
	E(Physics)
	E(FTBParticipant)
	E(FTBZoneBlockReason)
	E(FTBRespect)
	E(ApprovalRatings)
	E(AttitudesToPlayers)
	E(CharacterCreationAppearance)
	E(Active)
	E(EquipmentVisual)
	E(Repose)
	E(OriginalTemplate)
	E(CanEnterChasm)
	E(DialogState)
	E(EncumbranceState)
	E(EncumbranceStats)
	E(Unsheath)
	E(UseSocket)
	E(UserAvatar)
	E(UserReservedFor)

	E(BackgroundGoals)

	E(CalendarDaysPassed)
	E(CalendarStartingDate)

	// Progression
	E(ProgressionContainer)
	E(ProgressionMeta)
	E(ProgressionChangedContainers)

	// Through
	E(CanSeeThrough)
	E(CanShootThrough)
	E(CanWalkThrough)
	E(ShootThroughType)

	// Experimental server components
	E(ServerRecruitedBy)
	E(ServerGameTimer)
	E(ServerExperienceGaveOut)
	E(ServerReplicationDependency)
	E(ServerIsUnsummoning)
	E(ServerActivationGroupContainer)
	E(ServerAnubisTag)
	E(ServerDialogTag)
	E(ServerDisplayNameList)
	E(ServerIconList)
	E(ServerRaceTag)
	E(ServerTemplateTag)
	E(ServerBoostTag)
	E(ServerSafePosition)
	E(ServerAnubisExecutor)
	E(ServerLeader)
	E(ServerBreadcrumb)
	E(ServerDelayDeathCause)
	E(ServerPickpocket)
	E(ServerReplicationDependencyOwner)
	E(ServerGameplayLightEquipment)
	E(HistoryTargetUUID)
	E(ServerGameplayLightChanges)
	E(ServerPeersInRange)
	E(ServerSurface)
	E(ServerDisarmAttempt)
	E(ServerUnsheath)
	E(ServerUnsheathScriptOverride)
	E(ServerIsLightBlocker)
	E(ServerIsVisionBlocker)
	E(ServerDarknessActive)
	E(ServerHotbarOrder)
	E(ServerInventoryItemDataPopulated)
	E(ServerVariableManager)
	E(IsMarkedForDeletion)
	E(JumpFollow)

	// Passives
	E(ServerToggledPassives)
	E(ServerPassiveBase)
	E(ServerPassivePersistentData)
	E(ServerScriptPassives)

	E(ServerCharacter)
	E(ServerItem)

	// Projectile
	E(ProjectileSource)
	E(ServerProjectile)
	E(ServerProjectileAttachment)
	E(ServerProjectileSpell)
	E(ServerProjectileInitialization)
	E(ServerOsirisTag)

	E(ClientCharacter)
	E(ClientItem)
	E(ClientProjectile)

	// Boost components
	E(ArmorClassBoost)
	E(AbilityBoost)
	E(RollBonusBoost)
	E(AdvantageBoost)
	E(ActionResourceValueBoost)
	E(CriticalHitBoost)
	E(AbilityFailedSavingThrowBoost)
	E(ResistanceBoost)
	E(WeaponDamageResistanceBoost)
	E(ProficiencyBonusOverrideBoost)
	E(JumpMaxDistanceMultiplierBoost)
	E(HalveWeaponDamageBoost)
	E(UnlockSpellBoost)
	E(SourceAdvantageBoost)
	E(ProficiencyBonusBoost)
	E(ProficiencyBoost)
	E(IncreaseMaxHPBoost)
	E(ActionResourceBlockBoost)
	E(StatusImmunityBoost)
	E(UseBoosts)
	E(CannotHarmCauseEntityBoost)
	E(TemporaryHPBoost)
	E(WeightBoost)
	E(WeightCategoryBoost)
	E(FactionOverrideBoost)
	E(ActionResourceMultiplierBoost)
	E(BlockRegainHPBoost)
	E(InitiativeBoost)
	E(DarkvisionRangeBoost)
	E(DarkvisionRangeMinBoost)
	E(DarkvisionRangeOverrideBoost)
	E(AddTagBoost)
	E(IgnoreDamageThresholdMinBoost)
	E(SkillBoost)
	E(WeaponDamageBoost)
	E(NullifyAbilityBoost)
	E(RerollBoost)
	E(DownedStatusBoost)
	E(WeaponEnchantmentBoost)
	E(GuaranteedChanceRollOutcomeBoost)
	E(AttributeBoost)
	E(GameplayLightBoost)
	E(DualWieldingBoost)
	E(SavantBoost)
	E(MinimumRollResultBoost)
	E(CharacterWeaponDamageBoost)
	E(ProjectileDeflectBoost)
	E(AbilityOverrideMinimumBoost)
	E(ACOverrideFormulaBoost)
	E(FallDamageMultiplierBoost)
	E(ActiveCharacterLightBoost)
	E(WeaponAttackTypeOverrideBoost)
	E(WeaponDamageDieOverrideBoost)
	E(CarryCapacityMultiplierBoost)
	E(WeaponPropertyBoost)
	E(WeaponAttackRollAbilityOverrideBoost)
	E(SightRangeAdditiveBoost)
	E(SightRangeMinimumBoost)
	E(SightRangeMaximumBoost)
	E(SightRangeOverrideBoost)
	E(MovementSpeedLimitBoost)
	E(UnlockSpellVariantBoost)
	E(DetectCrimesBlockBoost)
	E(BlockAbilityModifierFromACBoost)
	E(ScaleMultiplierBoost)
	E(DamageReductionBoost)
	E(ReduceCriticalAttackThresholdBoost)
	E(PhysicalForceRangeBonusBoost)
	E(ObjectSizeBoost)
	E(ObjectSizeOverrideBoost)
	E(AiArchetypeOverrideBoost)
	E(ExpertiseBonusBoost)
	E(EntityThrowDamageBoost)
	E(WeaponDamageTypeOverrideBoost)
	E(MaximizeHealingBoost)
	E(DamageBonusBoost)
	E(AdvanceSpellsBoost)
	E(SpellResistanceBoost)
	E(WeaponAttackRollBonusBoost)
	E(SpellSaveDCBoost)
	E(RedirectDamageBoost)
	E(CanSeeThroughBoost)
	E(CanShootThroughBoost)
	E(CanWalkThroughBoost)
	E(MonkWeaponDamageDiceOverrideBoost)
	E(HorizontalFOVOverrideBoost)
	E(CharacterUnarmedDamageBoost)
	E(ActionResourceReplenishTypeOverrideBoost)
	E(ActionResourcePreventReductionBoost)
	E(AttackSpellOverrideBoost)
	E(LockBoost)
	E(IgnorePointBlankDisadvantageBoost)
	E(CriticalHitExtraDiceBoost)
	E(DodgeAttackRollBoost)
	E(GameplayObscurityBoost)
	E(MaximumRollResultBoost)
	E(UnlockInterruptBoost)
	E(JumpMaxDistanceBonusBoost)
	E(ArmorAbilityModifierCapOverrideBoost)
	E(IgnoreResistanceBoost)
	E(ConcentrationIgnoreDamageBoost)
	E(IgnoreLowGroundPenaltyBoost)
	E(IgnoreSurfaceCoverBoost)

	E(Uuid)

	// Special engine components
	E(UuidToHandleMapping)

	E(Max)
END_ENUM()

BEGIN_ENUM(ExtQueryType, uint32_t)
	E(UuidToHandleMapping)
	E(Max)
END_ENUM()

BEGIN_BITMASK_NS(esv, StatusFlags, ServerStatusFlags, uint8_t)
	EV(ForceStatus, 1)
	EV(ForceFailStatus, 2)
	EV(IgnoreStacking, 4)
	EV(RequestClientSync, 8)
	EV(Loaded, 0x10)
	EV(RequestDelete, 0x20)
	EV(RequestDeleteAtTurnEnd, 0x40)
	EV(Started, 0x80)
END_ENUM_NS()

BEGIN_BITMASK_NS(esv, StatusFlags2, ServerStatusFlags2, uint8_t)
	EV(KeepAlive, 1)
	EV(DontTickWhileOnSurface, 2)
	EV(IsOnSourceSurface, 4)
	EV(IsFromItem, 8)
	EV(IsChanneled, 0x10)
	EV(IsLifeTimeSet, 0x20)
	EV(InitiateCombat, 0x40)
	EV(Influence, 0x80)
END_ENUM_NS()


BEGIN_BITMASK_NS(esv, StatusFlags3, ServerStatusFlags3, uint8_t)
	EV(IsUnique, 1)
	EV(NotifiedPlanManager, 2)
	EV(DisableImmunityOverhead, 4)
	EV(StatusFlags3_0x08, 8)
	EV(StatusFlags3_0x10, 0x10)
	EV(StatusFlags3_0x20, 0x20)
	EV(StatusFlags3_0x40, 0x40)
	EV(StatusFlags3_0x80, 0x80)
END_ENUM_NS()


BEGIN_BITMASK_NS(esv, StatusFlags4, ServerStatusFlags4, uint8_t)
	EV(BringIntoCombat, 1)
	EV(IsHostileAct, 2)
	EV(StatusFlags4_0x04, 4)
	EV(IsInvulnerable, 8)
	EV(IsInvulnerableVisible, 0x10)
	EV(CausedByEquipment, 0x20)
	EV(CauseIsOrigin, 0x40)
	EV(StatusFlags4_0x80, 0x80)
END_ENUM_NS()


BEGIN_BITMASK_NS(esv, StatusFlags5, ServerStatusFlags5, uint8_t)
	EV(HasTriedEntering, 1)
END_ENUM_NS()


// Persistent flags: 0x2414170022110
// AI flags: 0x4000070000060
BEGIN_BITMASK_NS(esv, CharacterFlags, ServerCharacterFlags, uint64_t)
	EV(IsPlayer, 0x01)
	EV(Multiplayer, 0x02)
	EV(InParty, 0x04)
	EV(HostControl, 0x08)
	EV(Activated, 0x10)
	EV(OffStage, 0x20)
	EV(Dead, 0x40)
	EV(HasOwner, 0x80)
	EV(InDialog, 0x100)
	EV(Summon, 0x200)
	EV(Invulnerable, 0x400)
	EV(CannotDie, 0x800)
	EV(StoryNPC, 0x1000)
	EV(CharacterControl, 0x2000)
	EV(Loaded, 0x4000)
	EV(Unknown8000, 0x8000)
	EV(Invisible, 0x10000)
	EV(StoryControlled, 0x20000)
	EV(FindValidPositionOnActivate, 0x40000)
	EV(CannotAttachToGroups, 0x80000)
	EV(CharCreationInProgress, 0x100000)
	EV(CharacterCreationFinished, 0x200000)
	EV(Floating, 0x400000)
	EV(FightMode, 0x800000)
	EV(SpotSneakers, 0x1000000)
	EV(Temporary, 0x2000000)
	EV(CannotMove, 0x4000000)
	EV(CannotRun, 0x8000000)
	EV(WalkThrough, 0x10000000)
	EV(CoverAmount, 0x20000000)
	EV(CanShootThrough, 0x40000000)
	EV(Unknown80000000, 0x80000000)
	EV(DontCacheTemplate, 0x100000000)
	EV(ReservedForDialog, 0x200000000)
	EV(PartyFollower, 0x400000000)
	EV(DisableWaypointUsage, 0x800000000)
	EV(IgnoresTriggers, 0x1000000000)
	EV(Totem, 0x2000000000)
	EV(IsTrading, 0x4000000000)
	EV(SteeringEnabled, 0x8000000000)
	EV(Unknown10000000000, 0x10000000000)
	EV(IsCompanion_M, 0x20000000000)
	EV(Unknown40000000000, 0x40000000000)
	EV(Deactivated, 0x80000000000)
	EV(CustomLookEnabled, 0x100000000000)
	EV(IsHuge, 0x200000000000)
	EV(DisableCulling, 0x400000000000)
	EV(DoNotFaceFlag, 0x800000000000)
	EV(NeedsMakePlayerUpdate, 0x1000000000000)
	EV(GMReroll, 0x2000000000000)
	EV(Detached, 0x4000000000000)
	EV(RequestStartTurn, 0x8000000000000)
	EV(DeferredRemoveEscapist, 0x10000000000000)
	EV(LevelTransitionPending, 0x20000000000000)
	EV(RegisteredForAutomatedDialog, 0x40000000000000)
	EV(Unknown80000000000000, 0x80000000000000)
END_ENUM_NS()


BEGIN_BITMASK_NS(esv, CharacterFlags2, ServerCharacterFlags2, uint8_t)
	EV(Global, 0x01)
	EV(TreasureGeneratedForTrader, 0x02)
	EV(Trader, 0x04)
	EV(IsResurrected, 0x08)
	EV(TeleportingPartyHack, 0x10)
	EV(IsPet, 0x20)
	EV(IsLeveledByStory, 0x40)
	EV(CanGossip, 0x80)
END_ENUM_NS()


BEGIN_BITMASK_NS(esv, CharacterFlags3, ServerCharacterFlags3, uint8_t)
	EV(NeedsPlacementSnapping, 0x01)
	EV(CrimeWarningsEnabled, 0x02)
	EV(SpreadActivation, 0x08)
	EV(TreasureEquip, 0x10)
END_ENUM_NS()

BEGIN_BITMASK_NS(esv, ItemFlags, ServerItemFlags, uint64_t)
	EV(Activated, 0x01)
	EV(OffStage, 0x02)
	EV(CanBePickedUp, 0x04)
	EV(CanBeMoved, 0x08)
	EV(WalkOn, 0x10)
	EV(WalkThrough, 0x20)
	EV(NoCover, 0x40)
	EV(CanShootThrough, 0x80)
	EV(CanUse, 0x100)
	EV(InteractionDisabled, 0x200)
	EV(TransformChanged, 0x400)
	EV(Destroyed, 0x800)
	EV(IsMoving, 0x1000)
	EV(CanOnlyBeUsedByOwner, 0x2000)
	EV(LoadedTemplate, 0x4000)
	EV(IsDoor, 0x8000)
	EV(Invisible, 0x10000)
	EV(StoryItem, 0x20000)
	EV(ClientSync1, 0x40000)
	EV(WakePhysics, 0x80000)
	EV(Summon, 0x100000)
	EV(IsContainer, 0x200000)
	EV(HideHP, 0x400000)
	EV(LuckyFind, 0x800000)
	EV(FreezeGravity, 0x1000000)
	EV(ForceSync, 0x2000000)
	EV(Known, 0x4000000)
	EV(Invisible2, 0x8000000)
	EV(DisableSync, 0x10000000)
	EV(IsLadder, 0x20000000)
	EV(ReservedForDialog, 0x40000000)
	EV(Invulnerable, 0x80000000)
	EV(DisableInventoryView80, 0x100000000)
	EV(DontAddToHotbar, 0x200000000)
	EV(PositionChanged, 0x400000000)
	EV(Totem, 0x800000000)
	EV(Destroy, 0x2000000000)
	EV(GMFolding, 0x4000000000)
	EV(InUse, 0x8000000000)
	EV(Sticky, 0x10000000000)
	EV(IsSecretDoor, 0x20000000000)
	EV(Floating, 0x40000000000)
	EV(IsSurfaceBlocker, 0x80000000000)
	EV(IsSurfaceCloudBlocker, 0x100000000000)
	EV(Invulnerable2, 0x200000000000)
	EV(ForceClientSync, 0x400000000000)
	EV(InPartyInventory, 0x800000000000)
	EV(SourceContainer, 0x1000000000000)
	EV(Frozen, 0x2000000000000)
	EV(LevelTransitionPending, 0x4000000000000)
	EV(TeleportOnUse, 0x8000000000000)
	EV(InAutomatedDialog, 0x10000000000000)
	EV(PinnedContainer, 0x20000000000000)
END_ENUM_NS()

BEGIN_BITMASK_NS(esv, ItemFlags2, ServerItemFlags2, uint8_t)
	EV(UnsoldGenerated, 0x01)
	EV(Global, 0x02)
	EV(TreasureGenerated, 0x08)
	EV(UnEquipLocked, 0x10)
	EV(UseRemotely, 0x20)
END_ENUM_NS()


BEGIN_ENUM(BoostType, uint8_t)
	E(AC)
	E(Ability)
	E(RollBonus)
	E(Advantage)
	E(ActionResource)
	E(CriticalHit)
	E(AbilityFailedSavingThrow)
	E(Resistance)
	E(WeaponDamageResistance)
	E(ProficiencyBonusOverride)
	E(ActionResourceOverride) // Has no component
	E(AddProficiencyToAC) // Has no component
	EV(JumpMaxDistanceMultiplier, 12)
	E(AddProficiencyToDamage) // Has no component
	EV(ActionResourceConsumeMultiplier, 14)
	E(BlockVerbalComponent) // Has no component
	E(BlockSomaticComponent) // Has no component
	EV(HalveWeaponDamage, 17)
	EV(UnlockSpell, 19)
	EV(SourceAdvantageOnAttack, 20)
	EV(ProficiencyBonus, 21)
	E(BlockSpellCast) // Has no component
	EV(Proficiency, 23)
	E(SourceAllyAdvantageOnAttack)
	E(IncreaseMaxHP)
	E(ActionResourceBlock)
	E(StatusImmunity)
	E(UseBoosts)
	E(CannotHarmCauseEntity)
	E(TemporaryHP)
	E(Weight)
	E(WeightCategory)
	E(FactionOverride)
	E(ActionResourceMultiplier)
	E(BlockRegainHP)
	E(Initiative)
	E(DarkvisionRange)
	E(DarkvisionRangeMin)
	E(DarkvisionRangeOverride)
	E(Tag)
	E(IgnoreDamageThreshold)
	E(Skill)
	E(WeaponDamage)
	E(NullifyAbilityScore)
	E(IgnoreFallDamage)
	EV(Reroll, 46)
	EV(DownedStatus, 47)
	E(Invulnerable) // Has no component
	EV(WeaponEnchantment, 49)
	EV(GuaranteedChanceRollOutcome, 50)
	EV(Attribute, 51)
	E(IgnoreLeaveAttackRange) // Has no component
	EV(GameplayLight, 53)
	E(DialogueBlock)
	E(DualWielding)
	EV(Savant, 56)
	EV(MinimumRollResult, 57)
	E(Lootable) // Has no component
	EV(CharacterWeaponDamage, 59)
	E(ProjectileDeflect)
	E(AbilityOverrideMinimum)
	E(ACOverrideFormula)
	E(FallDamageMultiplier)
	EV(ActiveCharacterLight, 64)
	E(Invisibility) // Has no component
	E(TwoWeaponFighting) // Has no component
	EV(WeaponAttackTypeOverride, 67)
	E(WeaponDamageDieOverride)
	E(CarryCapacityMultiplier)
	E(WeaponProperty)
	EV(WeaponAttackRollAbilityOverride, 71)
	E(BlockTravel) // Has no component
	E(BlockGatherAtCamp) // Has no component
	E(BlockAbilityModifierDamageBonus) // Has no component
	E(VoicebarkBlock) // Has no component
	E(HiddenDuringCinematic) // Has no component
	EV(SightRangeAdditive, 77)
	E(SightRangeMinimum)
	E(SightRangeMaximum)
	EV(SightRangeOverride, 80)
	E(CannotBeDisarmed) // Has no component
	EV(MovementSpeedLimit, 82)
	E(NonLethal) // Has no component
	EV(UnlockSpellVariant, 84)
	E(DetectDisturbancesBlock)
	E(BlockAbilityModifierFromAC)
	EV(ScaleMultiplier, 87)
	E(CriticalDamageOnHit) // Has no component
	EV(DamageReduction, 89)
	E(ReduceCriticalAttackThreshold)
	E(PhysicalForceRangeBonus)
	E(ObjectSize)
	EV(ObjectSizeOverride, 93)
	E(ItemReturnToOwner) // Has no component
	EV(AiArchetypeOverride, 95)
	E(ExpertiseBonus)
	E(EntityThrowDamage)
	E(WeaponDamageTypeOverride)
	EV(MaximizeHealing, 99)
	E(IgnoreEnterAttackRange) // Has no component
	EV(DamageBonus, 101)
	E(Detach) // Has no component
	E(ConsumeItemBlock) // Has no component
	EV(AdvanceSpells, 104)
	E(SpellResistance)
	E(WeaponAttackRollBonus)
	E(SpellSaveDC)
	E(RedirectDamage)
	E(CanSeeThrough)
	E(CanShootThrough)
	EV(CanWalkThrough, 111)
	E(MonkWeaponAttackOverride) // Has no component
	EV(MonkWeaponDamageDiceOverride, 113)
	E(IntrinsicSummonerProficiency) // Has no component
	EV(HorizontalFOVOverride, 115)
	EV(CharacterUnarmedDamage, 116)
	E(UnarmedMagicalProperty) // Has no component
	EV(ActionResourceReplenishTypeOverride, 118)
	E(AreaDamageEvade) // Has no component
	EV(ActionResourcePreventReduction, 120)
	EV(AttackSpellOverride, 121)
	EV(Lock, 122)
	E(NoAOEDamageOnLand) // Has no component
	EV(IgnorePointBlankDisadvantage, 124)
	E(CriticalHitExtraDice)
	E(DodgeAttackRoll)
	E(GameplayObscurity)
	E(MaximumRollResult)
	EV(UnlockInterrupt, 129)
	E(IntrinsicSourceProficiency) // Has no component
	EV(JumpMaxDistanceBonus, 131)
	E(ArmorAbilityModifierCapOverride)
	E(IgnoreResistance)
	EV(ConcentrationIgnoreDamage, 134)
	E(LeaveTriggers) // Has no component
	EV(IgnoreLowGroundPenalty, 136)
	E(IgnoreSurfaceCover)
	E(EnableBasicItemInteractions) // Has no component
	E(SoundsBlocked) // Has no component
	E(ProficiencyBonusIncrease)
	E(NoDamageOnThrown)
	E(DamageTakenBonus)
	E(ReceivingCriticalDamageOnHit)
END_ENUM()

BEGIN_ENUM_NS(ecs, EntityChangeFlags, ECSEntityChangeFlags, uint8_t)
	EV(Create, 1)
	EV(Destroy, 2)
	EV(Immediate, 4)
	EV(Dead, 8)
	EV(Ignore, 0x10)
END_ENUM_NS()


BEGIN_BITMASK(BoundBaseFlags, uint8_t)
	EV(UseTranslateOverride, 1)
	EV(UseOnDistance, 2)
END_ENUM()
