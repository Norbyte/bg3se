--- @meta
--- @diagnostic disable

--- Table that contains every ModTable entry for active mods.
Mods = {}

--- Special global value that contains the current mod UUID during load
--- @type FixedString
ModuleUUID = "UUID"

--- Table that gets stored in the save, unique for each ModTable entry.
--- @type table
PersistentVars = {}

--- @alias OsirisEventType string|"before"|"after"|"beforeDelete"|"afterDelete"
--- @alias i16vec2 int16[]
--- @alias YesNo "Yes"|"No"
--- @alias OsirisValue number|string

--- Using a DB like a function will allow inserting new values into the database (ex. `Osi.DB_IsPlayer("02a77f1f-872b-49ca-91ab-32098c443beb")`  
--- @overload fun(...:OsirisValue?)
--- @class OsiDatabase
local OsiDatabase = {}
--- Databases can be read using the Get method. The method checks its parameters against the database and only returns rows that match the query.  
--- The number of parameters passed to Get must be equivalent to the number of columns in the target database.  
--- Each parameter defines an (optional) filter on the corresponding column.  
--- If the parameter is nil, the column is not filtered (equivalent to passing _ in Osiris). If the parameter is not nil, only rows with matching values will be returned.
--- @param ... OsirisValue?
--- @return table<integer,table<integer,OsirisValue>>
function OsiDatabase:Get(...) end
--- The Delete method can be used to delete rows from databases.  
--- The number of parameters passed to Delete must be equivalent to the number of columns in the target database.  
--- Each parameter defines an (optional) filter on the corresponding column.  
--- If the parameter is nil, the column is not filtered (equivalent to passing _ in Osiris). If the parameter is not nil, only rows with matching values will be deleted. 
--- @param ... OsirisValue?
function OsiDatabase:Delete(...) end

--- @alias OsiFunction fun(...:OsirisValue):OsirisValue?
--- @alias OsiDynamic table<string, OsiFunction|OsiDatabase>

--- @class OsiCommonDatabases
--- @field DB_IsPlayer OsiDatabase|fun(Guid:string) All player characters
--- @field DB_Origins OsiDatabase|fun(Guid:string) All origin characters
--- @field DB_Avatars OsiDatabase|fun(Guid:string) All player characters that were created in character creation, or that have an `AVATAR` tag
--- @field DB_CombatObjects OsiDatabase|fun(Guid:string, combatID:integer) All objects in combat
--- @field DB_CombatCharacters OsiDatabase|fun(Guid:string, combatID:integer) All characters in combat
--- @field DB_Dialogs OsiDatabase|fun(Guid:string, dialog:string)|fun(GUID1:string, GUID2:string, dialog:string)|fun(GUID1:string, GUID2:string, GUID3:string, dialog:string)|fun(GUID1:string, GUID2:string, GUID3:string, GUID4:string, dialog:string) All registered dialogs for objects, the most common being the version with a single character

--- The Osi table contains databases as well as calls, queries, events, and custom PROC / QRY defintions, as long as they are used in a script.  
--- @type OsiCommonDatabases|OsiDynamic
Osi = {}
--- @alias AnyRef any
--- @alias ComponentHandle userdata
--- @alias EntityRef number
--- @alias FixedString string
--- @alias Guid string
--- @alias LSStringView string
--- @alias NetId integer
--- @alias NoesisString string
--- @alias NoesisSymbol string
--- @alias Path string
--- @alias PersistentRef any
--- @alias PersistentRegistryEntry any
--- @alias Ref any
--- @alias RegistryEntry any
--- @alias ScratchBuffer string
--- @alias ScratchString string
--- @alias StringView string
--- @alias UserId integer
--- @alias UserReturn any
--- @alias WStringView string
--- @alias int16 integer
--- @alias int32 integer
--- @alias int64 integer
--- @alias int8 integer
--- @alias StatsConditionId string
--- @alias uint16 integer
--- @alias uint32 integer
--- @alias uint64 integer
--- @alias uint8 integer
--- @alias Version int32[]
--- @alias ivec2 int32[]
--- @alias mat3 number[]
--- @alias mat3x4 number[]
--- @alias mat4 number[]
--- @alias mat4x3 number[]
--- @alias vec2 number[]
--- @alias vec3 number[]
--- @alias vec4 number[]


--- @class EntityHandle
--- @field CreateComponent fun(self:EntityHandle, type:ExtComponentType):BaseComponent
--- @field GetComponent fun(self:EntityHandle, type:ExtComponentType):BaseComponent
--- @field GetAllComponents fun(self:EntityHandle, warnOnMissing:boolean?):table<ExtComponentType,BaseComponent>
--- @field GetAllComponentNames fun(self:EntityHandle, mapped:boolean?):table<number,string>
--- @field GetEntityType fun(self:EntityHandle):number
--- @field GetSalt fun(self:EntityHandle):number
--- @field GetIndex fun(self:EntityHandle):number
--- @field IsAlive fun(self:EntityHandle):boolean
--- @field GetReplicationFlags fun(self:EntityHandle, type:ExtComponentType, qword:number?):number
--- @field SetReplicationFlags fun(self:EntityHandle, type:ExtComponentType, flags:number, qword:number?)
--- @field Replicate fun(self:EntityHandle, type:ExtComponentType)
--- @field Vars table User variables registered using Ext.Vars
--- @field ACOverrideFormulaBoost ACOverrideFormulaBoostComponent?
--- @field AbilityBoost AbilityBoostComponent?
--- @field AbilityFailedSavingThrowBoost AbilityFailedSavingThrowBoostComponent?
--- @field AbilityOverrideMinimumBoost AbilityOverrideMinimumBoostComponent?
--- @field ActionResourceBlockBoost ActionResourceBlockBoostComponent?
--- @field ActionResourceConsumeMultiplierBoost ActionResourceConsumeMultiplierBoostComponent?
--- @field ActionResourceEvents ActionResourceEventsOneFrameComponent?
--- @field ActionResourceMultiplierBoost ActionResourceMultiplierBoostComponent?
--- @field ActionResourcePreventReductionBoost ActionResourcePreventReductionBoostComponent?
--- @field ActionResourceReplenishTypeOverrideBoost ActionResourceReplenishTypeOverrideBoostComponent?
--- @field ActionResourceSpendEvent ActionResourceSpendEventOneFrameComponent?
--- @field ActionResourceValueBoost ActionResourceValueBoostComponent?
--- @field ActionResources ActionResourcesComponent?
--- @field ActionUseConditions ActionUseConditionsComponent?
--- @field ActiveCharacterLightBoost ActiveCharacterLightBoostComponent?
--- @field ActiveCharacterLight ActiveCharacterLightComponent?
--- @field Active ActiveComponent?
--- @field ActiveSkeletonSlots ActiveSkeletonSlotsComponent?
--- @field AddTagBoost AddTagBoostComponent?
--- @field AdvanceSpellsBoost AdvanceSpellsBoostComponent?
--- @field AdvantageBoost AdvantageBoostComponent?
--- @field AiArchetypeOverrideBoost AiArchetypeOverrideBoostComponent?
--- @field Ambushing AmbushingComponent?
--- @field AnimationSet AnimationSetComponent?
--- @field AnimationUpdate AnimationUpdateComponent?
--- @field AnimationWaterfall AnimationWaterfallComponent?
--- @field AppearanceOverride AppearanceOverrideComponent?
--- @field ApprovalRatings ApprovalRatingsComponent?
--- @field ArmorAbilityModifierCapOverrideBoost ArmorAbilityModifierCapOverrideBoostComponent?
--- @field ArmorClassBoost ArmorClassBoostComponent?
--- @field Armor ArmorComponent?
--- @field ArmorSetState ArmorSetStateComponent?
--- @field AttackSpellOverrideBoost AttackSpellOverrideBoostComponent?
--- @field AttitudesToPlayers AttitudesToPlayersComponent?
--- @field AttributeBoost AttributeBoostComponent?
--- @field AttributeFlags AttributeFlagsComponent?
--- @field Avatar AvatarComponent?
--- @field Background BackgroundComponent?
--- @field BackgroundPassives BackgroundPassivesComponent?
--- @field BackgroundTag BackgroundTagComponent?
--- @field BaseHp BaseHpComponent?
--- @field BaseStats BaseStatsComponent?
--- @field BlockAbilityModifierFromACBoost BlockAbilityModifierFromACComponent?
--- @field BlockRegainHPBoost BlockRegainHPBoostComponent?
--- @field BodyType BodyTypeComponent?
--- @field BoostCondition BoostConditionComponent?
--- @field BoostInfo BoostInfoComponent?
--- @field BoostsContainer BoostsContainerComponent?
--- @field Camera CameraComponent?
--- @field CanBeDisarmed CanBeDisarmedComponent?
--- @field CanBeLooted CanBeLootedComponent?
--- @field CanDeflectProjectiles CanDeflectProjectilesComponent?
--- @field CanDoActions CanDoActionsComponent?
--- @field CanDoRest CanDoRestComponent?
--- @field CanEnterChasm CanEnterChasmComponent?
--- @field CanInteract CanInteractComponent?
--- @field CanModifyHealth CanModifyHealthComponent?
--- @field CanMove CanMoveComponent?
--- @field CanSeeThroughBoost CanSeeThroughBoostComponent?
--- @field CanSeeThrough CanSeeThroughComponent?
--- @field CanSense CanSenseComponent?
--- @field CanShootThroughBoost CanShootThroughBoostComponent?
--- @field CanShootThrough CanShootThroughComponent?
--- @field CanSpeak CanSpeakComponent?
--- @field CanTrade CanTradeComponent?
--- @field CanTravel CanTravelComponent?
--- @field CanTriggerRandomCasts CanTriggerRandomCastsComponent?
--- @field CanWalkThroughBoost CanWalkThroughBoostComponent?
--- @field CanWalkThrough CanWalkThroughComponent?
--- @field CannotHarmCauseEntityBoost CannotHarmCauseEntityBoostComponent?
--- @field CarryCapacityMultiplierBoost CarryCapacityMultiplierBoostComponent?
--- @field IsCharacter CharacterComponent?
--- @field CharacterCreationAppearance CharacterCreationAppearanceComponent?
--- @field CharacterCreationStats CharacterCreationStatsComponent?
--- @field CharacterCreationTemplateOverride CharacterCreationTemplateOverrideComponent?
--- @field CharacterUnarmedDamageBoost CharacterUnarmedDamageBoostComponent?
--- @field CharacterWeaponDamageBoost CharacterWeaponDamageBoostComponent?
--- @field ClassTag ClassTagComponent?
--- @field Classes ClassesComponent?
--- @field ClientControl ClientControlComponent?
--- @field Concentration ConcentrationComponent?
--- @field ConcentrationIgnoreDamageBoost ConcentrationIgnoreDamageBoostComponent?
--- @field CriticalHitBoost CriticalHitBoostComponent?
--- @field CriticalHitExtraDiceBoost CriticalHitExtraDiceBoostComponent?
--- @field Cull CullComponent?
--- @field CustomIcon CustomIconComponent?
--- @field CustomIconsStorage CustomIconsStorageSingletonComponent?
--- @field CustomName CustomNameComponent?
--- @field CustomStats CustomStatsComponent?
--- @field DamageBonusBoost DamageBonusBoostComponent?
--- @field DamageReductionBoost DamageReductionBoostComponent?
--- @field DamageTakenBonusBoost DamageTakenBonusBoostComponent?
--- @field Darkness DarknessComponent?
--- @field DarkvisionRangeBoost DarkvisionRangeBoostComponent?
--- @field DarkvisionRangeMinBoost DarkvisionRangeMinBoostComponent?
--- @field DarkvisionRangeOverrideBoost DarkvisionRangeOverrideBoostComponent?
--- @field Data DataComponent?
--- @field DefaultCameraBehavior DefaultCameraBehavior?
--- @field Detached DetachedComponent?
--- @field DetectCrimesBlockBoost DetectCrimesBlockBoostComponent?
--- @field DialogState DialogStateComponent?
--- @field DifficultyCheck DifficultyCheckComponent?
--- @field DisabledEquipment DisabledEquipmentComponent?
--- @field Disarmable DisarmableComponent?
--- @field DisplayName DisplayNameComponent?
--- @field DodgeAttackRollBoost DodgeAttackRollBoostComponent?
--- @field DownedStatusBoost DownedStatusBoostComponent?
--- @field DualWieldingBoost DualWieldingBoostComponent?
--- @field DualWielding DualWieldingComponent?
--- @field DynamicAnimationTags DynamicAnimationTagsComponent?
--- @field EffectCameraBehavior EffectCameraBehavior?
--- @field EncumbranceState EncumbranceStateComponent?
--- @field EncumbranceStats EncumbranceStatsComponent?
--- @field EntityThrowDamageBoost EntityThrowDamageBoostComponent?
--- @field EocLevel EocLevelComponent?
--- @field Equipable EquipableComponent?
--- @field EquipmentVisual EquipmentVisualComponent?
--- @field ExpertiseBonusBoost ExpertiseBonusBoostComponent?
--- @field Expertise ExpertiseComponent?
--- @field Faction FactionComponent?
--- @field FactionOverrideBoost FactionOverrideBoostComponent?
--- @field FallDamageMultiplierBoost FallDamageMultiplierBoostComponent?
--- @field FleeCapability FleeCapabilityComponent?
--- @field Floating FloatingComponent?
--- @field GameObjectVisual GameObjectVisualComponent?
--- @field GameplayLightBoost GameplayLightBoostComponent?
--- @field GameplayLight GameplayLightComponent?
--- @field GameplayObscurityBoost GameplayObscurityBoostComponent?
--- @field GlobalLongRestDisabled GlobalLongRestDisabledComponent?
--- @field GlobalShortRestDisabled GlobalShortRestDisabledComponent?
--- @field God GodComponent?
--- @field GodTag GodTagComponent?
--- @field GravityDisabled GravityDisabledComponent?
--- @field GravityDisabledUntilMoved GravityDisabledUntilMovedComponent?
--- @field GuaranteedChanceRollOutcomeBoost GuaranteedChanceRollOutcomeBoostComponent?
--- @field HalveWeaponDamageBoost HalveWeaponDamageBoostComponent?
--- @field HasExclamationDialog HasExclamationDialogComponent?
--- @field Health HealthComponent?
--- @field Hearing HearingComponent?
--- @field HorizontalFOVOverrideBoost HorizontalFOVOverrideBoostComponent?
--- @field Icon IconComponent?
--- @field Identity IdentityComponent?
--- @field IdentityState IdentityStateComponent?
--- @field IgnoreDamageThresholdMinBoost IgnoreDamageThresholdMinBoostComponent?
--- @field IgnoreLowGroundPenaltyBoost IgnoreLowGroundPenaltyBoostComponent?
--- @field IgnorePointBlankDisadvantageBoost IgnorePointBlankDisadvantageBoostComponent?
--- @field IgnoreResistanceBoost IgnoreResistanceBoostComponent?
--- @field IgnoreSurfaceCoverBoost IgnoreSurfaceCoverBoostComponent?
--- @field IncreaseMaxHPBoost IncreaseMaxHPComponent?
--- @field InitiativeBoost InitiativeBoostComponent?
--- @field InteractionFilter InteractionFilterComponent?
--- @field Invisibility InvisibilityComponent?
--- @field IsFalling IsFallingComponent?
--- @field IsGlobal IsGlobalComponent?
--- @field IsInTurnBasedMode IsInTurnBasedModeComponent?
--- @field IsSeeThrough IsSeeThroughComponent?
--- @field IsSummon IsSummonComponent?
--- @field ItemBoosts ItemBoostsComponent?
--- @field JumpMaxDistanceBonusBoost JumpMaxDistanceBonusBoostComponent?
--- @field JumpMaxDistanceMultiplierBoost JumpMaxDistanceMultiplierBoostComponent?
--- @field Key KeyComponent?
--- @field Level LevelComponent?
--- @field LockBoost LockBoostComponent?
--- @field Lock LockComponent?
--- @field Loot LootComponent?
--- @field LootingState LootingStateComponent?
--- @field MaterialParameterOverride MaterialParameterOverrideComponent?
--- @field HealMaxIncoming MaxIncomingComponent?
--- @field HealMaxOutgoing MaxOutgoingComponent?
--- @field MaximizeHealingBoost MaximizeHealingBoostComponent?
--- @field MaximumRollResultBoost MaximumRollResultBoostComponent?
--- @field MinimumRollResultBoost MinimumRollResultBoostComponent?
--- @field MonkWeaponDamageDiceOverrideBoost MonkWeaponDamageDiceOverrideBoostComponent?
--- @field Movement MovementComponent?
--- @field MovementSpeedLimitBoost MovementSpeedLimitBoostComponent?
--- @field NullifyAbilityBoost NullifyAbilityBoostComponent?
--- @field ObjectInteraction ObjectInteractionComponent?
--- @field ObjectSizeBoost ObjectSizeBoostComponent?
--- @field ObjectSize ObjectSizeComponent?
--- @field ObjectSizeOverrideBoost ObjectSizeOverrideBoostComponent?
--- @field OffStage OffStageComponent?
--- @field OriginAppearanceTag OriginAppearanceTagComponent?
--- @field Origin OriginComponent?
--- @field OriginPassives OriginPassivesComponent?
--- @field OriginTag OriginTagComponent?
--- @field OriginalIdentity OriginalIdentityComponent?
--- @field OriginalTemplate OriginalTemplateComponent?
--- @field ServerOsirisTag OsirisTagComponent?
--- @field Passive PassiveComponent?
--- @field PassiveContainer PassiveContainerComponent?
--- @field Pathing PathingComponent?
--- @field PathingDistanceChanged PathingDistanceChangedOneFrameComponent?
--- @field PhysicalForceRangeBonusBoost PhysicalForceRangeBonusBoostComponent?
--- @field Physics PhysicsComponent?
--- @field PickingState PickingStateComponent?
--- @field Player PlayerComponent?
--- @field ProficiencyBonusBoost ProficiencyBonusBoostComponent?
--- @field ProficiencyBonusIncreaseBoost ProficiencyBonusIncreaseBoostComponent?
--- @field ProficiencyBonusOverrideBoost ProficiencyBonusOverrideBoostComponent?
--- @field ProficiencyBoost ProficiencyBoostComponent?
--- @field Proficiency ProficiencyComponent?
--- @field ProficiencyGroup ProficiencyGroupComponent?
--- @field ProjectileDeflectBoost ProjectileDeflectBoostComponent?
--- @field Race RaceComponent?
--- @field ReceivingCriticalDamageOnHitBoost ReceivingCriticalDamageOnHitBoostComponent?
--- @field RedirectDamageBoost RedirectDamageBoostComponent?
--- @field ReduceCriticalAttackThresholdBoost ReduceCriticalAttackThresholdBoostComponent?
--- @field Relation RelationComponent?
--- @field Repose ReposeStateComponent?
--- @field RequestedRoll RequestedRollComponent?
--- @field RerollBoost RerollBoostComponent?
--- @field ResistanceBoost ResistanceBoostComponent?
--- @field Resistances ResistancesComponent?
--- @field RollBonusBoost RollBonusBoostComponent?
--- @field Ruleset RulesetComponent?
--- @field RulesetModifiers RulesetModifiersComponent?
--- @field SavantBoost SavantBoostComponent?
--- @field Savegame SavegameComponent?
--- @field ScaleMultiplierBoost ScaleMultiplierBoostComponent?
--- @field SceneAttach SceneAttachComponent?
--- @field ShapeshiftState ShapeshiftStateComponent?
--- @field ShootThroughType ShootThroughTypeComponent?
--- @field SightRangeAdditiveBoost SightRangeAdditiveBoostComponent?
--- @field SightRangeMaximumBoost SightRangeMaximumBoostComponent?
--- @field SightRangeMinimumBoost SightRangeMinimumBoostComponent?
--- @field SightRangeOverrideBoost SightRangeOverrideBoostComponent?
--- @field SimpleCharacter SimpleCharacterComponent?
--- @field SkillBoost SkillBoostComponent?
--- @field Sound SoundComponent?
--- @field SourceAdvantageBoost SourceAdvantageBoostComponent?
--- @field Speaker SpeakerComponent?
--- @field SpellResistanceBoost SpellResistanceBoostComponent?
--- @field SpellSaveDCBoost SpellSaveDCBoostComponent?
--- @field StaticPhysics StaticPhysicsComponent?
--- @field Stats StatsComponent?
--- @field StatusImmunities StatusImmunitiesComponent?
--- @field StatusImmunityBoost StatusImmunityBoostComponent?
--- @field Stealth StealthComponent?
--- @field Steering SteeringComponent?
--- @field StoryShortRestDisabled StoryShortRestDisabledComponent?
--- @field SummonContainer SummonContainerComponent?
--- @field SummonLifetime SummonLifetimeComponent?
--- @field SurfacePathInfluences SurfacePathInfluencesComponent?
--- @field Tag TagComponent?
--- @field TemplateAnimationSetOverride TemplateAnimationSetOverrideComponent?
--- @field TemporaryHPBoost TemporaryHPBoostComponent?
--- @field TimeFactor TimeFactorComponent?
--- @field TimelineActorData TimelineActorDataComponent?
--- @field Trader TraderComponent?
--- @field Transform TransformComponent?
--- @field UnlockInterruptBoost UnlockInterruptBoostComponent?
--- @field UnlockSpellBoost UnlockSpellBoostComponent?
--- @field UnlockSpellVariantBoost UnlockSpellVariantBoostComponent?
--- @field Unsheath UnsheathStateComponent?
--- @field PassiveUsageCount UsageCountComponent?
--- @field UseBoosts UseBoostsComponent?
--- @field Use UseComponent?
--- @field UseSocket UseSocketComponent?
--- @field UserAvatar UserAvatarComponent?
--- @field UserReservedFor UserReservedForComponent?
--- @field Uuid UuidComponent?
--- @field UuidToHandleMapping UuidToHandleMappingComponent?
--- @field Value ValueComponent?
--- @field Visual VisualComponent?
--- @field Voice VoiceComponent?
--- @field VoiceTag VoiceTagComponent?
--- @field WeaponAttackRollAbilityOverrideBoost WeaponAttackRollAbilityOverrideBoostComponent?
--- @field WeaponAttackRollBonusBoost WeaponAttackRollBonusBoostComponent?
--- @field WeaponAttackTypeOverrideBoost WeaponAttackTypeOverrideBoostComponent?
--- @field Weapon WeaponComponent?
--- @field WeaponDamageBoost WeaponDamageBoostComponent?
--- @field WeaponDamageDieOverrideBoost WeaponDamageDieOverrideBoostComponent?
--- @field WeaponDamageResistanceBoost WeaponDamageResistanceBoostComponent?
--- @field WeaponDamageTypeOverrideBoost WeaponDamageTypeOverrideBoostComponent?
--- @field WeaponEnchantmentBoost WeaponEnchantmentBoostComponent?
--- @field WeaponPropertyBoost WeaponPropertyBoostComponent?
--- @field WeaponSet WeaponSetComponent?
--- @field WeightBoost WeightBoostComponent?
--- @field WeightCategoryBoost WeightCategoryBoostComponent?
--- @field Wielding WieldingComponent?
--- @field RollInProgress ActiveRollInProgressComponent?
--- @field RollModifiers ActiveRollModifiersComponent?
--- @field AnimationGameplayEvents AnimationGameplayEventsOneFrameComponent?
--- @field AnimationGameplayEventsSingleton AnimationGameplayEventsSingletonComponent?
--- @field AnimationTextKeyEvents AnimationTextKeyEventsOneFrameComponent?
--- @field AnimationTextKeyEventsSingleton AnimationTextKeyEventsSingletonComponent?
--- @field AnimationTriggeredEvents AnimationTriggeredEventsOneFrameComponent?
--- @field CampChest CampChestComponent?
--- @field CampEndTheDayState CampEndTheDayStateComponent?
--- @field CampPresence CampPresenceComponent?
--- @field CampQuality CampQualityComponent?
--- @field CampSettings CampSettingsComponent?
--- @field CampSupply CampSupplyComponent?
--- @field CampTotalSupplies CampTotalSuppliesComponent?
--- @field CCChangeAppearanceDefinition CharacterCreationChangeAppearanceDefinitionComponent?
--- @field CCCharacterDefinition CharacterCreationCharacterDefinitionComponent?
--- @field CCCompanionDefinition CharacterCreationCompanionDefinitionComponent?
--- @field CCDefinitionCommon CharacterCreationDefinitionCommonComponent?
--- @field CCFullRespecDefinition CharacterCreationFullRespecDefinitionComponent?
--- @field CCLevelUp CharacterCreationLevelUpComponent?
--- @field CCLevelUpDefinition CharacterCreationLevelUpDefinitionComponent?
--- @field CCRespecDefinition CharacterCreationRespecDefinitionComponent?
--- @field CCSessionCommon CharacterCreationSessionCommonComponent?
--- @field CCState CharacterCreationStateComponent?
--- @field CCCreation CharacterCreationDefinitionCreationComponent?
--- @field CCFullRespec CharacterCreationDefinitionFullRespecComponent?
--- @field CCDefinitionLevelUp CharacterCreationDefinitionLevelUpComponent?
--- @field CCRespec CharacterCreationDefinitionRespecComponent?
--- @field DeadByDefault DeathDeadByDefaultComponent?
--- @field Death DeathDeathComponent?
--- @field DeathState DeathDeathStateComponent?
--- @field DeathType DeathDeathTypeComponent?
--- @field Downed DeathDownedComponent?
--- @field DownedEvent DeathDownedEventOneFrameComponent?
--- @field CameraCombatTarget EclCameraCombatTargetComponent?
--- @field CameraSelectorMode EclCameraSelectorModeComponent?
--- @field CameraTarget EclCameraTargetComponent?
--- @field ClientCharacter EclCharacter?
--- @field ClientCharacterIconRequest EclCharacterIconRequestComponent?
--- @field ClientCharacterIconResult EclCharacterIconResultComponent?
--- @field ClientTimelineActorControl EclClientTimelineActorControlComponent?
--- @field ClientEquipmentVisuals EclEquipmentVisualsComponent?
--- @field GameCameraBehavior EclGameCameraBehavior?
--- @field CameraInSelectorMode EclIsInSelectorModeComponent?
--- @field ClientPaperdoll EclPaperdollComponent?
--- @field CameraSpellTracking EclSpellTrackingComponent?
--- @field ClientCCBaseDefinitionState EclCharacterCreationBaseDefinitionComponent?
--- @field ClientCCChangeAppearanceDefinition EclCharacterCreationChangeAppearanceDefinitionComponent?
--- @field ClientCCCompanionDefinition EclCharacterCreationCompanionDefinitionComponent?
--- @field ClientCCDefinitionState EclCharacterCreationDefinitionStateComponent?
--- @field ClientCCDefinitionStateEx EclCharacterCreationDefinitionStateExComponent?
--- @field ClientCCDummyDefinition EclCharacterCreationDummyDefinitionComponent?
--- @field ClientCCFullRespecDefinition EclCharacterCreationFullRespecDefinitionComponent?
--- @field ClientCCLevelUpDefinition EclCharacterCreationLevelUpDefinitionComponent?
--- @field ClientVisualsDesiredState EclEquipmentVisualsDesiredStateComponent?
--- @field ClientEquipmentVisibilityState EclEquipmentVisualsVisibilityStateComponent?
--- @field ClientInterruptPlayerDecision EclInterruptPlayerDecisionComponent?
--- @field BackgroundGoals EocBackgroundGoalsComponent?
--- @field CalendarDaysPassed EocCalendarDaysPassedComponent?
--- @field CalendarStartingDate EocCalendarStartingDateComponent?
--- @field CombatDelayedFanfare EocCombatDelayedFanfareComponent?
--- @field IsCombatPaused EocCombatIsCombatPausedComponent?
--- @field IsInCombat EocCombatIsInCombatComponent?
--- @field CombatIsThreatened EocCombatIsThreatenedComponent?
--- @field CombatParticipant EocCombatParticipantComponent?
--- @field CombatState EocCombatStateComponent?
--- @field ThreatRange EocCombatThreatRangeComponent?
--- @field TurnBased EocCombatTurnBasedComponent?
--- @field TurnOrder EocCombatTurnOrderComponent?
--- @field AvailableLevel EocExpAvailableLevelComponent?
--- @field CanLevelUp EocExpCanLevelUpComponent?
--- @field Experience EocExpExperienceComponent?
--- @field FTBPaused EocFtbIsFtbPausedComponent?
--- @field IsInFTB EocFtbIsInFtbComponent?
--- @field FTBParticipant EocFtbParticipantComponent?
--- @field FTBZoneBlockReason EocFtbZoneBlockReasonComponent?
--- @field HotbarContainer EocHotbarContainerComponent?
--- @field HotbarDecks EocHotbarHotbarCurrentDecksComponent?
--- @field CanBeWielded EocImprovisedWeaponCanBeWieldedComponent?
--- @field ImprovisedWeaponWielded EocImprovisedWeaponWieldedComponent?
--- @field ImprovisedWeaponWielding EocImprovisedWeaponWieldingComponent?
--- @field PickUpExecuting EocPickupPickUpExecutingComponent?
--- @field PickUpRequest EocPickupPickUpRequestComponent?
--- @field LongRestInScriptPhase EocRestLongRestInScriptPhase?
--- @field LongRestState EocRestLongRestStateComponent?
--- @field LongRestTimeline EocRestLongRestTimeline?
--- @field LongRestTimers EocRestLongRestTimers?
--- @field LongRestUsers EocRestLongRestUsers?
--- @field RestingEntities EocRestRestingEntities?
--- @field ShortRest EocRestShortRestComponent?
--- @field ActionResourceChangeResults EsvActionResourceChangeResultsSingletonComponent?
--- @field ServerActivationGroupContainer EsvActivationGroupContainerComponent?
--- @field ServerAnubisTag EsvAnubisTagComponent?
--- @field ServerBaseData EsvBaseDataComponent?
--- @field ServerBaseProficiency EsvBaseProficiencyComponent?
--- @field ServerBaseSize EsvBaseSizeComponent?
--- @field ServerBaseStats EsvBaseStatsComponent?
--- @field ServerBaseWeapon EsvBaseWeaponComponent?
--- @field ServerBoostTag EsvBoostTagComponent?
--- @field ServerBreadcrumb EsvBreadcrumbComponent?
--- @field ServerCharacter EsvCharacter?
--- @field ServerDarknessActive EsvDarknessActiveComponent?
--- @field DeathApplyKnockedOut EsvDeathApplyKnockedOutOneFrameComponent?
--- @field DeathAssignEntityToUserRequest EsvDeathAssignEntityToUserRequestOneFrameComponent?
--- @field ServerDeathContinue EsvDeathContinueComponent?
--- @field DeathDeadByDefaultRequest EsvDeathDeadByDefaultRequestOneFrameComponent?
--- @field ServerDelayDeathCause EsvDelayDeathCauseComponent?
--- @field ServerDialogTag EsvDialogTagComponent?
--- @field DiedEvent EsvDiedEventOneFrameComponent?
--- @field ServerDisarmAttempt EsvDisarmAttemptComponent?
--- @field ServerDisplayNameList EsvDisplayNameListComponent?
--- @field ServerExperienceGaveOut EsvExperienceGaveOutComponent?
--- @field FallToProne EsvFallToProneOneFrameComponent?
--- @field ServerGameTimer EsvGameTimerComponent?
--- @field ServerGameplayLightChanges EsvGameplayLightChangesComponent?
--- @field ServerGameplayLightEquipment EsvGameplayLightEquipmentComponent?
--- @field ServerIconList EsvIconListComponent?
--- @field ServerInventoryItemDataPopulated EsvInventoryItemDataPopulatedComponent?
--- @field InventoryPropertyCanBePickpocketed EsvInventoryPropertyCanBePickpocketedComponent?
--- @field InventoryPropertyIsDroppedOnDeath EsvInventoryPropertyIsDroppedOnDeathComponent?
--- @field InventoryPropertyIsTradable EsvInventoryPropertyIsTradableComponent?
--- @field ServerIsLightBlocker EsvIsLightBlockerComponent?
--- @field IsMarkedForDeletion EsvIsMarkedForDeletionComponent?
--- @field ServerIsVisionBlocker EsvIsVisionBlockerComponent?
--- @field ServerItem EsvItem?
--- @field JumpFollow EsvJumpFollowComponent?
--- @field ServerKiller EsvKillerComponent?
--- @field ServerLeader EsvLeaderComponent?
--- @field ServerMusicVolumeTriggerState EsvMusicVolumeTriggerStateComponent?
--- @field Net EsvNetComponent?
--- @field ServerHotbarOrder EsvOrderComponent?
--- @field ServerPeersInRange EsvPeersInRangeComponent?
--- @field ServerPickpocket EsvPickpocketComponent?
--- @field ServerProficiencyGroupStats EsvProficiencyGroupStatsComponent?
--- @field ServerRaceTag EsvRaceTagComponent?
--- @field ServerRecruitedBy EsvRecruitedByComponent?
--- @field ServerReplicationDependency EsvReplicationDependencyComponent?
--- @field ServerReplicationDependencyOwner EsvReplicationDependencyOwnerComponent?
--- @field ServerSafePosition EsvSafePositionComponent?
--- @field ScriptPropertyCanBePickpocketed EsvScriptPropertyCanBePickpocketedComponent?
--- @field ScriptPropertyIsDroppedOnDeath EsvScriptPropertyIsDroppedOnDeathComponent?
--- @field ScriptPropertyIsTradable EsvScriptPropertyIsTradableComponent?
--- @field ServerDeathState EsvStateComponent?
--- @field ServerSurface EsvSurfaceComponent?
--- @field HistoryTargetUUID EsvTargetUUIDComponent?
--- @field ServerTemplateTag EsvTemplateTagComponent?
--- @field ServerUnsheath EsvUnsheathDefaultComponent?
--- @field ServerUnsheathScriptOverride EsvUnsheathScriptOverrideComponent?
--- @field ServerVariableManager EsvVariableManagerComponent?
--- @field ServerRollInProgress EsvActiveRollInProgressComponent?
--- @field ServerRollStartRequest EsvActiveRollStartRequestOneFrameComponent?
--- @field ServerRollStartSpellRequest EsvActiveRollStartSpellRequestOneFrameComponent?
--- @field ServerAiModifiers EsvAiCombatAiModifiersComponent?
--- @field ServerAiArchetype EsvAiCombatArchetypeComponent?
--- @field ServerAiInterestedInItems EsvAiCombatInterestedInItemsComponent?
--- @field ServerAiInterestingItem EsvAiCombatInterestingItemComponent?
--- @field BoostBaseUpdated EsvBoostBaseUpdatedOneFrameComponent?
--- @field ServerBoostBase EsvBoostBoostBaseComponent?
--- @field BoostChangedEvent EsvBoostChangedEventOneFrameComponent?
--- @field BoostChangedEventsSingleton EsvBoostChangedEventsSingletonComponent?
--- @field BoostConditionalState EsvBoostConditionalStateComponent?
--- @field BoostProvider EsvBoostProviderComponent?
--- @field ServerStatusBoostsProcessed EsvBoostStatusBoostsProcessedComponent?
--- @field StatusBoostsRefreshed EsvBoostStatusBoostsRefreshedOneFrameComponent?
--- @field ServerCCAppearanceVisualTag EsvCharacterCreationAppearanceVisualTagComponent?
--- @field ServerCCCustomIcon EsvCharacterCreationCustomIconComponent?
--- @field ServerCCEquipmentSetRequest EsvCharacterCreationEquipmentSetRequestComponent?
--- @field ServerCCGod EsvCharacterCreationGodComponent?
--- @field ServerCCIsCustom EsvCharacterCreationIsCustomComponent?
--- @field ServerCCUpdates EsvCharacterCreationUpdatesComponent?
--- @field ServerCanStartCombat EsvCombatCanStartCombatComponent?
--- @field ServerCombatGroupMapping EsvCombatCombatGroupMappingComponent?
--- @field CombatScheduledForDelete EsvCombatCombatScheduledForDeleteOneFrameComponent?
--- @field CombatStartedEvent EsvCombatCombatStartedEventOneFrameComponent?
--- @field CombatSwitched EsvCombatCombatSwitchedComponent?
--- @field CombatantKilledEvent EsvCombatCombatantKilledEventOneFrameComponent?
--- @field DelayedFanfareRemovedDuringCombatEvent EsvCombatDelayedFanfareRemovedDuringCombatEventOneFrameComponent?
--- @field ServerEnterRequest EsvCombatEnterRequestComponent?
--- @field ServerFleeBlocked EsvCombatFleeBlockedComponent?
--- @field CombatFleeRequest EsvCombatFleeRequestComponent?
--- @field CombatFleeSuccess EsvCombatFleeSuccessOneFrameComponent?
--- @field GlobalCombatRequests EsvCombatGlobalCombatRequests?
--- @field ServerImmediateJoin EsvCombatImmediateJoinComponent?
--- @field CombatantJoinEvent EsvCombatJoinEventOneFrameComponent?
--- @field CombatJoinInCurrentRoundFailedEvent EsvCombatJoinInCurrentRoundFailedEventOneFrameComponent?
--- @field CombatJoinInCurrentRound EsvCombatJoinInCurrentRoundOneFrameComponent?
--- @field CombatJoining EsvCombatJoiningComponent?
--- @field CombatLateJoinPenalty EsvCombatLateJoinPenaltyComponent?
--- @field ServerCombatLeaveRequest EsvCombatLeaveRequestComponent?
--- @field CombatLeftEvent EsvCombatLeftEventOneFrameComponent?
--- @field CombatMerge EsvCombatMergeComponent?
--- @field CombatRequestCompletedEvent EsvCombatRequestCompletedEventOneFrameComponent?
--- @field CombatSurfaceTeamSingleton EsvCombatSurfaceTeamSingletonComponent?
--- @field CombatSurprisedJoinRequest EsvCombatSurprisedJoinRequestOneFrameComponent?
--- @field CombatSurprisedStealthRequest EsvCombatSurprisedStealthRequestOneFrameComponent?
--- @field CombatThreatRangeChangedEvent EsvCombatThreatRangeChangedEventOneFrameComponent?
--- @field ConcentrationChanged EsvConcentrationConcentrationChangedOneFrameComponent?
--- @field ConcentrationDamageCheck EsvConcentrationDamageCheckOneFrameComponent?
--- @field ConcentrationClearedEvent EsvConcentrationOnConcentrationClearedEventOneFrameComponent?
--- @field AbilityCheckEvent EsvEcsstatsAbilityCheckEventOneFrameComponent?
--- @field AttributeFlagsChangedEvent EsvEcsstatsAttributeFlagsChangedEventOneFrameComponent?
--- @field ClassesChangedEvent EsvEcsstatsClassesChangedEventOneFrameComponent?
--- @field EquipmentSlotChangedEvent EsvEcsstatsEquipmentSlotChangedEventOneFrameComponent?
--- @field LevelChanged EsvEcsstatsLevelChangedOneFrameComponent?
--- @field SavingThrowRolledEvent EsvEcsstatsSavingThrowRolledEventOneFrameComponent?
--- @field SkillCheckEvent EsvEcsstatsSkillCheckEventOneFrameComponent?
--- @field StatsAppliedEvent EsvEcsstatsStatsAppliedEventOneFrameComponent?
--- @field WeaponPropertiesChangedEvent EsvEcsstatsWeaponPropertiesChangedEventOneFrameComponent?
--- @field EscortFollower EsvEscortFollowerComponent?
--- @field EscortHasStragglers EsvEscortHasStragglersComponent?
--- @field EscortLeader EsvEscortLeaderComponent?
--- @field EscortLeaderPriority EsvEscortLeaderPriorityComponent?
--- @field EscortMember EsvEscortMemberComponent?
--- @field EscortStragglersTracker EsvEscortStragglersTrackerComponent?
--- @field FTBModeChangedEvent EsvFtbModeChangedEventOneFrameComponent?
--- @field FTBPlayersTurnEndedEvent EsvFtbPlayersTurnEndedEventOneFrameComponent?
--- @field FTBPlayersTurnStartedEvent EsvFtbPlayersTurnStartedEventOneFrameComponent?
--- @field FTBRoundEndedEvent EsvFtbRoundEndedEventOneFrameComponent?
--- @field FTBSurfaceTeamSingleton EsvFtbSurfaceTeamSingletonComponent?
--- @field FTBTimeFactorRequests EsvFtbTimeFactorRequestsSingletonComponent?
--- @field FTBTimeFactorResetRequests EsvFtbTimeFactorResetRequestsSingletonComponent?
--- @field FTBTurnBasedChangesRequest EsvFtbTurnBasedChangesRequestSingletonComponent?
--- @field FTBZone EsvFtbZoneComponent?
--- @field FTBZoneInstigator EsvFtbZoneInstigatorComponent?
--- @field HitAnimationRequest EsvHitHitAnimationRequestOneFrameComponent?
--- @field HitNotification EsvHitHitNotificationEventOneFrameComponent?
--- @field HitNotificationRequest EsvHitHitNotificationRequestOneFrameComponent?
--- @field HitResultEvent EsvHitHitResultEventOneFrameComponent?
--- @field HitJoinCombatRequest EsvHitOnHitJoinCombatRequestOneFrameComponent?
--- @field UnresolvedHitNotification EsvHitUnresolvedHitNotificationComponent?
--- @field ServerInterruptActionRequests EsvInterruptActionRequestsComponent?
--- @field ServerInterruptAddRemoveRequests EsvInterruptAddRemoveRequestsComponent?
--- @field ServerInterruptDataSingleton EsvInterruptDataSingletonComponent?
--- @field ServerInterruptInitialParticipants EsvInterruptInitialParticipantsComponent?
--- @field ServerSpellCastInterrupt EsvInterruptInterruptDataComponent?
--- @field ServerInterruptTurnOrderInZone EsvInterruptTurnOrderInZoneComponent?
--- @field ServerInterruptUsed EsvInterruptUsedOneFrameComponent?
--- @field ServerInterruptZoneRequests EsvInterruptZoneRequestsComponent?
--- @field CharacterHasGeneratedTradeTreasure EsvInventoryCharacterHasGeneratedTradeTreasureComponent?
--- @field ServerInventoryContainerData EsvInventoryContainerDataComponent?
--- @field HasGeneratedTreasure EsvInventoryEntityHasGeneratedTreasureComponent?
--- @field ServerInventoryGroupCheck EsvInventoryGroupCheckComponent?
--- @field ServerInventoryIsReplicatedWith EsvInventoryIsReplicatedWithComponent?
--- @field ReadyToBeAddedToInventory EsvInventoryReadyToBeAddedToInventoryComponent?
--- @field ServerShapeshiftEquipmentHistory EsvInventoryShapeshiftEquipmentHistoryComponent?
--- @field ServerDynamicLayerOwner EsvItemDynamicLayerOwnerComponent?
--- @field ServerIsCurrentOwner EsvOwnershipIsCurrentOwnerComponent?
--- @field ServerIsLatestOwner EsvOwnershipIsLatestOwnerComponent?
--- @field ServerIsOriginalOwner EsvOwnershipIsOriginalOwnerComponent?
--- @field ServerIsPreviousOwner EsvOwnershipIsPreviousOwnerComponent?
--- @field ServerOwneeHistory EsvOwnershipOwneeHistoryComponent?
--- @field ServerOwneeRequest EsvOwnershipOwneeRequestComponent?
--- @field ServerUserSnapshot EsvPartyUserSnapshotComponent?
--- @field ServerPassiveBoosts EsvPassiveBoostsComponent?
--- @field PassiveConditionalRollInterruptEvent EsvPassiveConditionalRollInterruptEventOneFrameComponent?
--- @field ServerPassiveBase EsvPassivePassiveBaseComponent?
--- @field PassivesUpdatedEvent EsvPassivePassivesUpdatedEventOneFrameComponent?
--- @field ServerPassivePersistentData EsvPassivePersistentDataComponent?
--- @field PassiveRequestTargetTracking EsvPassiveRequestTargetTrackingOneFrameComponent?
--- @field ServerScriptPassives EsvPassiveScriptPassivesComponent?
--- @field ServerToggledPassives EsvPassiveToggledPassivesComponent?
--- @field PassiveUpdateTargetTracking EsvPassiveUpdateTargetTrackingOneFrameComponent?
--- @field PasssiveUsageCountIncrementedEvent EsvPassiveUsageCountIncrementedEventOneFrameComponent?
--- @field ProgressionChangedContainers EsvProgressionChangedContainersComponent?
--- @field ProgressionLevelUpChanged EsvProgressionLevelUpChangedOneFrameComponent?
--- @field ServerProjectileAttachment EsvProjectileAttachmentComponent?
--- @field ProjectileImpactEvent EsvProjectileImpactEventOneFrameComponent?
--- @field ProjectileRequestTargetTracking EsvProjectileRequestTargetTrackingOneFrameComponent?
--- @field ServerProjectileSpell EsvProjectileSpellComponent?
--- @field ProjectileSplitThrowableObjectRequest EsvProjectileSplitThrowableObjectRequestOneFrameComponent?
--- @field ProjectileUpdateTargetTracking EsvProjectileUpdateTargetTrackingOneFrameComponent?
--- @field ServerRestPendingType EsvRestPendingTypeComponent?
--- @field ServerRestTypeChosenEvent EsvRestRestTypeChosenEventOneFrameComponent?
--- @field ShortRestConsumeResources EsvRestShortRestConsumeResourcesComponent?
--- @field ServerShortRestResultEvent EsvRestShortRestResultEventOneFrameComponent?
--- @field ServerRollFinishedEvent EsvRollRollFinishedEventOneFrameComponent?
--- @field ShapeshiftHealthReservation EsvShapeshiftHealthReservationComponent?
--- @field ServerShapeshiftStates EsvShapeshiftStatesComponent?
--- @field SpellBookChanged EsvSpellBookChangedOneFrameComponent?
--- @field NewSpellsAddedEvent EsvSpellNewSpellsAddedEventOneFrameComponent?
--- @field OnDamageSpells EsvSpellOnDamageSpellsComponent?
--- @field SpellsLearnedEvent EsvSpellSpellsLearnedEventOneFrameComponent?
--- @field ServerSpellCastCache EsvSpellCastCacheComponent?
--- @field ServerSpellCastHitDelay EsvSpellCastCastHitDelayComponent?
--- @field ServerSpellCastRequests EsvSpellCastCastRequestsComponent?
--- @field ServerSpellCastResponsible EsvSpellCastCastResponsibleComponent?
--- @field ServerSpellClientInitiated EsvSpellCastClientInitiatedComponent?
--- @field ServerSpellExternals EsvSpellCastExternalsComponent?
--- @field ServerSpellHitRegister EsvSpellCastHitRegisterComponent?
--- @field ServerSpellInterruptRequests EsvSpellCastInterruptRequestsComponent?
--- @field ServerSpellInterruptResults EsvSpellCastInterruptResultsComponent?
--- @field SpellCastInterruptsUsed EsvSpellCastInterruptsUsedOneFrameComponent?
--- @field SpellCastMoveDuringCastUpdateEvent EsvSpellCastMoveDuringCastUpdateEventOneFrameComponent?
--- @field SpellCastMovementAndPrecalculationEndEvent EsvSpellCastMovementAndPrecalculationEndEventOneFrameComponent?
--- @field ServerSpellCastMovement EsvSpellCastMovementComponent?
--- @field ServerSpellCastMovementInfo EsvSpellCastMovementInfoComponent?
--- @field ServerSpellCastPendingRequests EsvSpellCastPendingRequestsComponent?
--- @field ServerProjectileCache EsvSpellCastProjectileCacheComponent?
--- @field ServerSpellCastProjectilePathfindCache EsvSpellCastProjectilePathfindCacheComponent?
--- @field SpellCastRequestTargetTracking EsvSpellCastRequestTargetTrackingOneFrameComponent?
--- @field ServerSpellCastState EsvSpellCastStateComponent?
--- @field ServerSpellCastUnsheathFallbackTimer EsvSpellCastUnsheathFallbackTimerComponent?
--- @field SpellCastUpdateTargetTracking EsvSpellCastUpdateTargetTrackingOneFrameComponent?
--- @field SpellCastWeaponSetChangeRequest EsvSpellCastWeaponSetChangeRequestOneFrameComponent?
--- @field ServerSpellCastZoneRange EsvSpellCastZoneRangeComponent?
--- @field ServerStatusActive EsvStatusActiveComponent?
--- @field ServerStatusAddEvent EsvStatusAddEventOneFrameComponent?
--- @field ServerStatusAddedFromSaveLoad EsvStatusAddedFromSaveLoadComponent?
--- @field ServerAddedStatusAuraEffectEvent EsvStatusAddedStatusAuraEffectEventOneFrameComponent?
--- @field ServerStatusApplyEvent EsvStatusApplyEventOneFrameComponent?
--- @field ServerStatusAttemptEvent EsvStatusAttemptEventOneFrameComponent?
--- @field ServerStatusAttemptFailedEvent EsvStatusAttemptFailedEventOneFrameComponent?
--- @field ServerStatusAura EsvStatusAuraComponent?
--- @field ServerStatusAuraContainer EsvStatusAuraContainerComponent?
--- @field ServerStatusCause EsvStatusCauseComponent?
--- @field ServerStatusDifficultyModifiers EsvStatusDifficultyModifiersComponent?
--- @field ServerStatusDispelRollCheck EsvStatusDispelRollCheckOneFrameComponent?
--- @field ServerStatusDownedChangedEvent EsvStatusDownedChangedEventOneFrameComponent?
--- @field ServerStatusOwnership EsvStatusOwnershipComponent?
--- @field ServerStatusPerforming EsvStatusPerformingComponent?
--- @field ServerStatusRefreshed EsvStatusRefreshedOneFrameComponent?
--- @field ServerStatusRemoveEvent EsvStatusRemoveEventOneFrameComponent?
--- @field ServerRemovedStatusAuraEffectEvent EsvStatusRemovedStatusAuraEffectEventOneFrameComponent?
--- @field ServerStatusScheduledForDeletion EsvStatusScheduledForDeletionOneFrameComponent?
--- @field ServerStatus EsvStatusStatusComponent?
--- @field ServerStatusEvent EsvStatusStatusEventOneFrameComponent?
--- @field ServerStatusTurnStartEvent EsvStatusTurnStartEventOneFrameComponent?
--- @field ServerStatusUnique EsvStatusUniqueComponent?
--- @field ServerStatusUpdateTargetTracking EsvStatusUpdateTargetTrackingOneFrameComponent?
--- @field SummonAddConcentrationRequest EsvSummonAddConcentrationRequestOneFrameComponent?
--- @field SummonAddToExistingConcentrationRequest EsvSummonAddToExistingConcentrationRequestOneFrameComponent?
--- @field SummonAttachToProjectileRequest EsvSummonAttachToProjectileRequestOneFrameComponent?
--- @field SummonDespawnRequest EsvSummonDespawnRequestOneFrameComponent?
--- @field SummonExpiredRequest EsvSummonExpiredRequestOneFrameComponent?
--- @field ServerIsUnsummoning EsvSummonIsUnsummoningComponent?
--- @field SummonLateJoinPenalty EsvSummonLateJoinPenaltyOneFrameComponent?
--- @field SummonOwnerSetEvent EsvSummonOwnerSetEventOneFrameComponent?
--- @field SummonPlaceInInventoryRequest EsvSummonPlaceInInventoryRequestOneFrameComponent?
--- @field SummonSetLifetimeRequest EsvSummonSetLifetimeRequestOneFrameComponent?
--- @field SummonCreatedEvent EsvSummonSpawnCreatedEventOneFrameComponent?
--- @field TagsChangedEvent EsvTagsTagsChangedEventOneFrameComponent?
--- @field ServerTemplateChangedEvent EsvTemplatesTemplateChangedOneFrameComponent?
--- @field ServerTemplateTransformedEvent EsvTemplatesTemplateTransformedOneFrameComponent?
--- @field ServerTriggerCachedLeaveEvents EsvTriggerCachedLeaveEventsComponent?
--- @field ServerTriggerEventConfig EsvTriggerEventConfigComponent?
--- @field ServerTriggerLoadedHandled EsvTriggerLoadedHandledComponent?
--- @field ServerTriggerRegisteredFor EsvTriggerRegisteredForComponent?
--- @field ServerRegistrationSettings EsvTriggerRegistrationSettingsComponent?
--- @field ServerTriggerWorldAutoTriggered EsvTriggerTriggerWorldAutoTriggeredComponent?
--- @field ServerUpdatedRegisteredFor EsvTriggerUpdatedRegisteredForComponent?
--- @field ServerUpdatedRegisteredForItems EsvTriggerUpdatedRegisteredForItemsComponent?
--- @field Pause GamePauseComponent?
--- @field PauseExcluded GamePauseExcludedComponent?
--- @field HealBlock HealBlockComponent?
--- @field HitAttacker HitAttackerComponent?
--- @field HitLifetime HitLifetimeComponent?
--- @field HitMeta HitMetaComponent?
--- @field HitProxy HitProxyComponent?
--- @field HitProxyOwner HitProxyOwnerComponent?
--- @field HitReaction HitReactionComponent?
--- @field HitTarget HitTargetComponent?
--- @field HitThrownObject HitThrownObjectComponent?
--- @field HitWeapon HitWeaponComponent?
--- @field InterruptActionState InterruptActionStateComponent?
--- @field InterruptConditionallyDisabled InterruptConditionallyDisabledComponent?
--- @field InterruptContainer InterruptContainerComponent?
--- @field InterruptData InterruptDataComponent?
--- @field InterruptDecision InterruptDecisionComponent?
--- @field InterruptPreferences InterruptPreferencesComponent?
--- @field InterruptPrepared InterruptPreparedComponent?
--- @field InterruptZone InterruptZoneComponent?
--- @field InterruptZoneParticipant InterruptZoneParticipantComponent?
--- @field InterruptZoneSource InterruptZoneSourceComponent?
--- @field CanBeInInventory InventoryCanBeInComponent?
--- @field CannotBePickpocketed InventoryCannotBePickpocketedComponent?
--- @field CannotBeTakenOut InventoryCannotBeTakenOutComponent?
--- @field InventoryContainer InventoryContainerComponent?
--- @field InventoryData InventoryDataComponent?
--- @field DropOnDeathBlocked InventoryDropOnDeathBlockedComponent?
--- @field InventoryLocked InventoryIsLockedComponent?
--- @field InventoryIsOwned InventoryIsOwnedComponent?
--- @field InventoryMember InventoryMemberComponent?
--- @field InventoryMemberTransform InventoryMemberTransformComponent?
--- @field NewItemsInside InventoryNewItemsInsideComponent?
--- @field NonTradable InventoryNonTradableComponent?
--- @field InventoryOwner InventoryOwnerComponent?
--- @field InventoryStack InventoryStackComponent?
--- @field InventoryStackMember InventoryStackMemberComponent?
--- @field InventoryTopOwner InventoryTopOwnerComponent?
--- @field TradeBuybackData InventoryTradeBuybackDataComponent?
--- @field InventoryWeight InventoryWeightComponent?
--- @field Wielded InventoryWieldedComponent?
--- @field WieldingHistory InventoryWieldingHistoryComponent?
--- @field ItemDestroyed ItemDestroyedComponent?
--- @field ItemDestroying ItemDestroyingComponent?
--- @field IsDoor ItemDoorComponent?
--- @field ItemDye ItemDyeComponent?
--- @field ExamineDisabled ItemExamineDisabledComponent?
--- @field ItemHasMoved ItemHasMovedComponent?
--- @field HasOpened ItemHasOpenedComponent?
--- @field ItemInUse ItemInUseComponent?
--- @field IsGold ItemIsGoldComponent?
--- @field ItemIsPoisoned ItemIsPoisonedComponent?
--- @field IsItem ItemItemComponent?
--- @field MapMarkerStyle ItemMapMarkerStyleComponent?
--- @field NewInInventory ItemNewInInventoryComponent?
--- @field ItemPortal ItemPortalComponent?
--- @field ShouldDestroyOnSpellCast ItemShouldDestroyOnSpellCastComponent?
--- @field ActionType ItemTemplateActionTypeComponent?
--- @field ItemCanMove ItemTemplateCanMoveComponent?
--- @field ClimbOn ItemTemplateClimbOnComponent?
--- @field ItemTemplateDestroyed ItemTemplateDestroyedComponent?
--- @field InteractionDisabled ItemTemplateInteractionDisabledComponent?
--- @field IsStoryItem ItemTemplateIsStoryItemComponent?
--- @field Ladder ItemTemplateLadderComponent?
--- @field UseAction ItemTemplateUseActionComponent?
--- @field WalkOn ItemTemplateWalkOnComponent?
--- @field MultiplayerHost MultiplayerHostComponent?
--- @field MultiplayerUser MultiplayerUserComponent?
--- @field OwnedAsLoot OwnershipOwnedAsLootComponent?
--- @field OwneeCurrent OwnershipOwneeCurrentComponent?
--- @field BlockFollow PartyBlockFollowComponent?
--- @field PartyComposition PartyCompositionComponent?
--- @field CurrentlyFollowingParty PartyCurrentlyFollowingPartyComponent?
--- @field PartyFollower PartyFollowerComponent?
--- @field PartyMember PartyMemberComponent?
--- @field PartyPortals PartyPortalsComponent?
--- @field PartyRecipes PartyRecipesComponent?
--- @field PartyView PartyViewComponent?
--- @field PartyWaypoints PartyWaypointsComponent?
--- @field ProgressionAbilityImprovements ProgressionAbilityImprovementsComponent?
--- @field ProgressionContainer ProgressionContainerComponent?
--- @field ProgressionFeat ProgressionFeatComponent?
--- @field LevelUp ProgressionLevelUpComponent?
--- @field ProgressionMeta ProgressionMetaComponent?
--- @field ProgressionPassives ProgressionPassivesComponent?
--- @field ProgressionReplicatedFeat ProgressionReplicatedFeatComponent?
--- @field ProgressionSkills ProgressionSkillsComponent?
--- @field ProgressionSpells ProgressionSpellsComponent?
--- @field ProjectileSource ProjectileSourceInfoComponent?
--- @field Recruiter RecruitRecruiterComponent?
--- @field SightData SightDataComponent?
--- @field SightEntityViewshed SightEntityViewshedComponent?
--- @field IgnoreSurfaces SightIgnoreSurfacesComponent?
--- @field Sight SightSightBaseComponent?
--- @field AddedSpells SpellAddedSpellsComponent?
--- @field AttackSpellOverride SpellAttackSpellOverrideComponent?
--- @field SpellBook SpellBookComponent?
--- @field SpellBookPrepares SpellBookPreparesComponent?
--- @field CCPrepareSpell SpellCCPrepareSpellComponent?
--- @field LearnedSpells SpellLearnedSpellsComponent?
--- @field PlayerPrepareSpell SpellPlayerPrepareSpellComponent?
--- @field ScriptedExplosion SpellScriptedExplosionComponent?
--- @field SpellAiConditions SpellSpellAiConditionsComponent?
--- @field SpellBookCooldowns SpellSpellBookCooldownsComponent?
--- @field SpellContainer SpellSpellContainerComponent?
--- @field SpellModificationContainer SpellSpellModificationContainerComponent?
--- @field SpellCastAnimationInfo SpellCastAnimationInfoComponent?
--- @field SpellCastAnimationRequest SpellCastAnimationRequestOneFrameComponent?
--- @field SpellCastCache SpellCastCacheComponent?
--- @field SpellCastCanBeTargeted SpellCastCanBeTargetedComponent?
--- @field SpellCastEvent SpellCastCastEventOneFrameComponent?
--- @field SpellCastHitEvent SpellCastCastHitEventOneFrameComponent?
--- @field SpellCastTextKeyEvent SpellCastCastTextKeyEventOneFrameComponent?
--- @field SpellCastCounteredEvent SpellCastCounteredEventOneFrameComponent?
--- @field SpellCastDataCache SpellCastDataCacheSingletonComponent?
--- @field SpellCastDestroyEvent SpellCastDestroyEventOneFrameComponent?
--- @field SpellCastExecutionTime SpellCastExecutionTimeComponent?
--- @field SpellCastFinishedEvent SpellCastFinishedEventOneFrameComponent?
--- @field SpellCastInterruptResults SpellCastInterruptResultsComponent?
--- @field SpellCastIsCasting SpellCastIsCastingComponent?
--- @field SpellCastJumpStartEvent SpellCastJumpStartEventOneFrameComponent?
--- @field SpellCastLogicExecutionEndEvent SpellCastLogicExecutionEndEventOneFrameComponent?
--- @field SpellCastLogicExecutionStartEvent SpellCastLogicExecutionStartEventOneFrameComponent?
--- @field SpellCastMovement SpellCastMovementComponent?
--- @field SpellCastOutcome SpellCastOutcomeComponent?
--- @field SpellCastPrepareEndEvent SpellCastPrepareEndEventOneFrameComponent?
--- @field SpellCastPrepareStartEvent SpellCastPrepareStartEventOneFrameComponent?
--- @field SpellCastPreviewEndEvent SpellCastPreviewEndEventOneFrameComponent?
--- @field SpellCastRolls SpellCastRollsComponent?
--- @field SpellCastSpellRollAbortEvent SpellCastSpellRollAbortEventOneFrameComponent?
--- @field SpellCastState SpellCastStateComponent?
--- @field SpellSyncTargeting SpellCastSyncTargetingComponent?
--- @field SpellCastTargetHitEvent SpellCastTargetHitEventOneFrameComponent?
--- @field SpellCastTargetHitInterruptEvent SpellCastTargetHitInterruptEventOneFrameComponent?
--- @field SpellCastTargetReactionEvent SpellCastTargetReactionEventOneFrameComponent?
--- @field SpellCastTargetsChangedEvent SpellCastTargetsChangedEventOneFrameComponent?
--- @field SpellCastThrowPickupPositionChangedEvent SpellCastThrowPickupPositionChangedEventOneFrameComponent?
--- @field StatusCause StatusCauseComponent?
--- @field StatusContainer StatusContainerComponent?
--- @field StatusID StatusIDComponent?
--- @field StatusIncapacitated StatusIncapacitatedComponent?
--- @field StatusIndicateDarkness StatusIndicateDarknessComponent?
--- @field StatusLifetime StatusLifetimeComponent?
--- @field StatusLoseControl StatusLoseControlComponent?
--- @field StatusVisualDisabled StatusVisualDisabledComponent?
--- @field FullIllithid TadpoleTreeFullIllithidComponent?
--- @field HalfIllithid TadpoleTreeHalfIllithidComponent?
--- @field TadpolePowers TadpoleTreePowerContainerComponent?
--- @field Tadpoled TadpoleTreeTadpoledComponent?
--- @field TadpoleTreeState TadpoleTreeTreeStateComponent?
--- @field TranslateChanged TranslateChangedComponent?
--- @field TranslateChangedEvent TranslateChangedEventOneFrameComponent?
--- @field TriggerArea TriggerAreaComponent?
--- @field TriggerContainer TriggerContainerComponent?
--- @field TriggerIsInsideOf TriggerIsInsideOfComponent?
--- @field TriggerType TriggerTriggerTypeComponent?
--- @field TriggerUpdatedContainer TriggerUpdatedContainerComponent?
--- @field TriggerUpdatedPhysics TriggerUpdatedPhysicsComponent?


--- @alias AIBoundType string|"Hit"|"Move"|"Stand"
--- @alias AIFlags string|"CanNotTargetFrozen"|"CanNotUse"|"IgnoreBuff"|"IgnoreControl"|"IgnoreDebuff"|"IgnoreSelf"|"StatusIsSecondary"
--- @alias AIShapeType string|"Box"|"Cylinder"|"None"
--- @alias AbilityId string|"Charisma"|"Constitution"|"Dexterity"|"Intelligence"|"None"|"Sentinel"|"Strength"|"Wisdom"
--- @alias ActionDataType string|"Book"|"Combine"|"Constrain"|"Consume"|"CreatePuddle"|"CreateSurface"|"Destroy"|"DestroyParameters"|"DisarmTrap"|"Door"|"Equip"|"Insert"|"Ladder"|"LearnSpell"|"Lie"|"Lockpick"|"OpenClose"|"PlaySound"|"Recipe"|"ShowStoryElementUI"|"Sit"|"SpawnCharacter"|"SpellBook"|"Stand"|"StoryUse"|"StoryUseInInventory"|"Teleport"|"Throw"|"Unknown"|"Unknown31"|"Unknown34"|"Unknown35"|"UseSpell"
--- @alias AdvantageBoostType string|"Advantage"|"Disadvantage"|"None"
--- @alias AdvantageContext string|"Ability"|"AllAbilities"|"AllSavingThrows"|"AllSkills"|"AttackRoll"|"AttackTarget"|"Concentration"|"DeathSavingThrow"|"SavingThrow"|"Skill"|"SourceDialogue"
--- @alias AiBaseFlags string|"Blocker"|"CanClimbOn"|"CloudSurfaceBlock"|"Door"|"DoorItemBlock"|"GroundSurfaceBlock"|"Indestructible"|"Portal"|"PortalTarget"|"ShootBlock"|"ShootBlockCharacter"|"ShootBlockItem"|"SubgridEdge"|"Trap"|"UnknownBlock1"|"UnknownBlock2"|"UnknownBlock3"|"UnknownBlock4"|"UnknownBlock5"|"WalkBlock"|"WalkBlock2"|"WalkBlockCharacter"|"WalkBlockItem"
--- @alias AnimationSetAnimationFlags string|"AlwaysIgnore"|"NoFallback"
--- @alias AppliedMaterialFlags string|"Instantiated"|"IsOverlay"|"OverlayOriginalMapsSet"
--- @alias ArmorSetState string|"Normal"|"Vanity"
--- @alias ArmorType string|"BreastPlate"|"ChainMail"|"ChainShirt"|"Cloth"|"HalfPlate"|"Hide"|"Leather"|"None"|"Padded"|"Plate"|"RingMail"|"ScaleMail"|"Sentinel"|"Splint"|"StuddedLeather"
--- @alias AttackFlags string|"DamagedAfterMiss"|"FirstAttack"|"FromSneak"|"InstantKill"|"LastAttack"|"Reaction"
--- @alias AttackRoll string|"Charisma"|"Constitution"|"Dexterity"|"Intelligence"|"None"|"SpellCastingAbility"|"Strength"|"UnarmedAttackAbility"|"WeaponAttackAbility"|"Wisdom"
--- @alias AttributeFlags string|"Arrow"|"Backstab"|"BackstabImmunity"|"EnableObscurityEvents"|"Floating"|"FloatingWhileMoving"|"ForceMainhandAlternativeEquipBones"|"Grounded"|"IgnoreClouds"|"InventoryBound"|"InvisibilityImmunity"|"InvulnerableAndInteractive"|"LootableWhenEquipped"|"LoseDurabilityOnCharacterHit"|"ObscurityWithoutSneaking"|"PickpocketableWhenEquipped"|"SlippingImmunity"|"ThrownImmunity"|"Torch"|"Unbreakable"|"Unrepairable"|"Unstorable"|"UseMusicalInstrumentForCasting"
--- @alias AudioCodec string|"ADPCM"|"Bank"|"External"|"Opus"|"OpusNX"|"OpusWEM"|"PCM"|"PCMEX"|"Vorbis"|"XMA"
--- @alias BoostSourceType string|"Action"|"Base"|"CharacterCreation"|"Console"|"Item"|"Passive"|"Progression"|"Shapeshift"|"Status"|"Story"|"Undefined"
--- @alias BoostType string|"AC"|"ACOverrideFormula"|"Ability"|"AbilityFailedSavingThrow"|"AbilityOverrideMinimum"|"ActionResource"|"ActionResourceBlock"|"ActionResourceConsumeMultiplier"|"ActionResourceMultiplier"|"ActionResourceOverride"|"ActionResourcePreventReduction"|"ActionResourceReplenishTypeOverride"|"ActiveCharacterLight"|"AddProficiencyToAC"|"AddProficiencyToDamage"|"AdvanceSpells"|"Advantage"|"AiArchetypeOverride"|"AreaDamageEvade"|"ArmorAbilityModifierCapOverride"|"AttackSpellOverride"|"Attribute"|"BlockAbilityModifierDamageBonus"|"BlockAbilityModifierFromAC"|"BlockGatherAtCamp"|"BlockRegainHP"|"BlockSomaticComponent"|"BlockSpellCast"|"BlockTravel"|"BlockVerbalComponent"|"CanSeeThrough"|"CanShootThrough"|"CanWalkThrough"|"CannotBeDisarmed"|"CannotHarmCauseEntity"|"CarryCapacityMultiplier"|"CharacterUnarmedDamage"|"CharacterWeaponDamage"|"ConcentrationIgnoreDamage"|"ConsumeItemBlock"|"CriticalDamageOnHit"|"CriticalHit"|"CriticalHitExtraDice"|"DamageBonus"|"DamageReduction"|"DamageTakenBonus"|"DarkvisionRange"|"DarkvisionRangeMin"|"DarkvisionRangeOverride"|"Detach"|"DetectDisturbancesBlock"|"DialogueBlock"|"DodgeAttackRoll"|"DownedStatus"|"DualWielding"|"EnableBasicItemInteractions"|"EntityThrowDamage"|"ExpertiseBonus"|"FactionOverride"|"FallDamageMultiplier"|"GameplayLight"|"GameplayObscurity"|"GuaranteedChanceRollOutcome"|"HalveWeaponDamage"|"HiddenDuringCinematic"|"HorizontalFOVOverride"|"IgnoreDamageThreshold"|"IgnoreEnterAttackRange"|"IgnoreFallDamage"|"IgnoreLeaveAttackRange"|"IgnoreLowGroundPenalty"|"IgnorePointBlankDisadvantage"|"IgnoreResistance"|"IgnoreSurfaceCover"|"IncreaseMaxHP"|"Initiative"|"IntrinsicSourceProficiency"|"IntrinsicSummonerProficiency"|"Invisibility"|"Invulnerable"|"ItemReturnToOwner"|"JumpMaxDistanceBonus"|"JumpMaxDistanceMultiplier"|"LeaveTriggers"|"Lock"|"Lootable"|"MaximizeHealing"|"MaximumRollResult"|"MinimumRollResult"|"MonkWeaponAttackOverride"|"MonkWeaponDamageDiceOverride"|"MovementSpeedLimit"|"NoAOEDamageOnLand"|"NoDamageOnThrown"|"NonLethal"|"NullifyAbilityScore"|"ObjectSize"|"ObjectSizeOverride"|"PhysicalForceRangeBonus"|"Proficiency"|"ProficiencyBonus"|"ProficiencyBonusIncrease"|"ProficiencyBonusOverride"|"ProjectileDeflect"|"ReceivingCriticalDamageOnHit"|"RedirectDamage"|"ReduceCriticalAttackThreshold"|"Reroll"|"Resistance"|"RollBonus"|"Savant"|"ScaleMultiplier"|"SightRangeAdditive"|"SightRangeMaximum"|"SightRangeMinimum"|"SightRangeOverride"|"Skill"|"SoundsBlocked"|"SourceAdvantageOnAttack"|"SourceAllyAdvantageOnAttack"|"SpellResistance"|"SpellSaveDC"|"StatusImmunity"|"Tag"|"TemporaryHP"|"TwoWeaponFighting"|"UnarmedMagicalProperty"|"UnlockInterrupt"|"UnlockSpell"|"UnlockSpellVariant"|"UseBoosts"|"VoicebarkBlock"|"WeaponAttackRollAbilityOverride"|"WeaponAttackRollBonus"|"WeaponAttackTypeOverride"|"WeaponDamage"|"WeaponDamageDieOverride"|"WeaponDamageResistance"|"WeaponDamageTypeOverride"|"WeaponEnchantment"|"WeaponProperty"|"Weight"|"WeightCategory"
--- @alias BoundBaseFlags string|"UseOnDistance"|"UseTranslateOverride"
--- @alias BoundFlags string|"CanClimbOn"|"CharacterSeeThroughBlock"|"CharacterShootThroughBlock"|"CharacterWalkThroughBlock"|"Indestructible"|"Interactable"|"InvisibleTrap"|"IsPlatformOwner"|"ItemSeeThroughBlock"|"ItemShootThroughBlock"|"ItemWalkThroughBlock"|"SurfaceBlocker"|"SurfaceCloudBlocker"|"WalkOn"
--- @alias CanDoActionsFlags string|"CanAttack"|"CanCastRituals"|"CanCastSpells"|"CanDisarmTrap"|"CanDoFlag4"|"CanLockpick"|"CanPickpocket"
--- @alias CanInteractFlags string|"CanInteract"|"CanUseFlag8"|"CanUseLadder"|"CanUsePortals"
--- @alias CanMoveFlags string|"CanBeTeleported"|"CanFollow"|"CanMove"|"CanWorldClimb"|"CanWorldDrop"
--- @alias CastRequirementFlags string|"ActionResources"|"Cooldowns"|"EquipmentProficiency"|"ItemCharges"|"Range"|"Shield"|"Somatic"|"Speech"|"SpellCast"|"SpellPrepared"|"StatsConditions"|"WeaponRequirements"
--- @alias CauseType string|"AURA"|"Attack"|"InventoryItem"|"None"|"Offhand"|"StatusEnter"|"StatusTick"|"SurfaceCreate"|"SurfaceMove"|"SurfaceStatus"|"Unknown11"|"WorldItemThrow"
--- @alias ClientCharacterFlags string|"Active"|"Airborne"|"Climbing"|"HasCharacterLight"|"InDialog"|"IsPlayer"|"VisibleOffStage"
--- @alias ClientCharacterFlags2 string|"CustomVisuals"|"LootingRequested"|"PickpocketRequested"|"Pickpocketing"|"ReloadingVisuals"|"RemovingFromLevel"
--- @alias ClientCharacterFlags3 string|"CanTrade"|"HasCustomVisualAfterDeath"|"HasDialog"|"WasInputControllerActive"
--- @alias ClientCharacterTaskFlags string|"AlwaysInterruptRunningTask"|"CanInterruptRunningTask"|"CanReenterPreview"|"CannotActivateSameType"|"CannotPreviewSameType"|"CombatTargeting"|"ConditionalInterruptByPreview"|"ConditionalInterruptRunningTask"|"IgnoreAP"|"IgnorePreviewCancel"|"IsDummyTask"|"IsExecuting"|"NeedsPreviewToActivate"|"NeedsTaskToActivate"|"OverrideCursorOnHover"|"PreviewingDifferentTask"|"SupportsPicking"
--- @alias ClientCharacterTaskType string|"ClimbTo"|"ControllerSelection"|"DefaultTargetAction"|"Dialog"|"DisarmTrap"|"Drop"|"ItemCombine"|"ItemCombineDummy"|"ItemInteractArea"|"ItemMove"|"ItemMoveDummy"|"ItemUse"|"ItemUseRemotely"|"Listen"|"Lockpick"|"Loot"|"MoveController"|"MoveInDirection"|"MoveTo"|"PickUp"|"Pickpocket"|"SheathUnsheath"|"Spell"
--- @alias ClientGameState string|"AnalyticsSessionEnd"|"BuildStory"|"Disconnect"|"Exit"|"GeneratePsoCache"|"Idle"|"Init"|"InitConnection"|"InitMenu"|"InitNetwork"|"Installation"|"Join"|"LoadLevel"|"LoadMenu"|"LoadModule"|"LoadPsoCache"|"LoadSession"|"Lobby"|"Menu"|"ModReceiving"|"Movie"|"Paused"|"PrepareRunning"|"Running"|"Save"|"StartLoading"|"StartServer"|"StopLoading"|"SwapLevel"|"Unknown"|"UnloadLevel"|"UnloadModule"|"UnloadSession"
--- @alias ClientInputControllerFlags string|"ControllerMode"
--- @alias ClientInputControllerRequestFlags string|"RequestCancelMovement"|"RequestClear"|"RequestForceClear"|"RequestUpdatePreview"
--- @alias ClientStatusFlags string|"DontTickWhileOnSurface"|"HasSounds"|"IsOnSourceSurface"|"KeepAlive"|"RequestDelete"|"Started"|"VisualsCreated"|"WasEnteredDuringSave"
--- @alias ClientStatusFlags2 string|"HasLoopSound"|"IsSourceDependent"|"Ticked"
--- @alias CombatParticipantFlags string|"CanFight"|"CanJoinCombat"|"IsBoss"|"IsInspector"|"StayInAiHints"
--- @alias CombatPauseSourceType string|"Script"|"Unknown"
--- @alias ConditionRollType string|"AbilityCheckRoll"|"AttackRoll"|"DifficultyRoll"|"SavingThrowRoll"|"SkillCheckRoll"
--- @alias CriticalHitAttackFlags string|"AttackRoll"|"AttackTarget"
--- @alias CriticalHitSuccessFlags string|"Failure"|"FailureNever"|"Success"|"SuccessNever"
--- @alias CursorType string|"Arrow"|"ArrowWarning"|"Backstab"|"BackstabWarning"|"Bow"|"BowGround"|"BowWarning"|"CameraRotation"|"Cast"|"CastWarning"|"Combine"|"CombineWarning"|"InputText"|"ItemMove"|"ItemMoveWarning"|"ItemPickup"|"ItemPickupWarning"|"ItemUse"|"ItemUseWarning"|"Listen"|"ListenWarning"|"Lock"|"LockWarning"|"LootedContainer"|"Melee"|"MeleeGround"|"MeleeWarning"|"None"|"OpenContainer"|"OpenContainerNew"|"OpenContainerWarning"|"OpenDoor"|"OpenDoorWarning"|"PickPocket"|"PickPocketWarning"|"Shovel"|"ShovelWarning"|"System"|"Talk"|"TalkWarning"|"Walk"|"WalkWarning"|"Wand"|"WandGround"|"WandWarning"
--- @alias DamageFlags string|"AttackAdvantage"|"AttackDisadvantage"|"Backstab"|"Calculated"|"Critical"|"Dodge"|"Hit"|"HitpointsDamaged"|"Invisible"|"Invulnerable"|"KillingBlow"|"Magical"|"Miss"|"NonlethalCapped"|"Projectile"|"SavingThrow"|"Status"|"Surface"
--- @alias DamageType string|"Acid"|"Bludgeoning"|"Cold"|"Fire"|"Force"|"Lightning"|"Necrotic"|"None"|"Piercing"|"Poison"|"Psychic"|"Radiant"|"Sentinel"|"Slashing"|"Thunder"
--- @alias DealDamageWeaponDamageType string|"MainMeleeWeaponDamageType"|"MainRangedWeaponDamageType"|"MainWeaponDamageType"|"None"|"OffhandMeleeWeaponDamageType"|"OffhandRangedWeaponDamageType"|"OffhandWeaponDamageType"|"SourceWeaponDamageType"|"ThrownWeaponDamageType"
--- @alias DealDamageWeaponType string|"ImprovisedWeapon"|"MainMeleeWeapon"|"MainRangedWeapon"|"MainWeapon"|"None"|"OffhandMeleeWeapon"|"OffhandRangedWeapon"|"OffhandWeapon"|"SourceWeapon"|"ThrownWeapon"|"UnarmedDamage"
--- @alias DialogNodeFlags string|"AllowGrouping"|"IsEndAllDialogs"|"IsEndNode"|"IsExclusive"|"IsGameplayNode"|"IsGreetingNode"|"IsOptional"|"IsRootNode"|"ShowOnlyOnce"|"SuppressSubtitle"
--- @alias DiceSizeId string|"D10"|"D100"|"D12"|"D20"|"D4"|"D6"|"D8"|"Default"
--- @alias DynamicModifierType string|"Consumable"|"ItemSpell"|"None"|"Spell"|"ToggledPassive"
--- @alias ECSComponentChangeFlags string|"Create"|"Destroy"|"OneFrame"|"Replicate"|"ReplicatedComponent"
--- @alias ECSEntityChangeFlags string|"Create"|"Dead"|"Destroy"|"Ignore"|"Immediate"
--- @alias EEquipmentType string|"Armor"|"Sentinel"|"Shield"|"Unknown1"|"Weapon"
--- @alias EffectFlags string|"Active"|"Forget"|"FullyInitialized"|"FullyInitialized2"|"Interrupted"|"Playing"|"Pooled"|"Stopped"
--- @alias EffectPropertyType string|"Base"|"Boolean"|"ColorARGBKeyFrame"|"FixedFunction"|"FixedString"|"Float"|"FloatKeyFrame"|"FloatRange"|"Integer"|"IntegerRange"|"String"|"Vector3"
--- @alias EquipmentStatsType string|"Armor"|"Shield"|"Weapon"
--- @alias ExecuteWeaponFunctorsType string|"BothHands"|"MainHand"|"OffHand"|"Undefined"
--- @alias ExtComponentType string|"ACOverrideFormulaBoost"|"AbilityBoost"|"AbilityCheckEvent"|"AbilityFailedSavingThrowBoost"|"AbilityOverrideMinimumBoost"|"ActionResourceBlockBoost"|"ActionResourceChangeResults"|"ActionResourceConsumeMultiplierBoost"|"ActionResourceEvents"|"ActionResourceMultiplierBoost"|"ActionResourcePreventReductionBoost"|"ActionResourceReplenishTypeOverrideBoost"|"ActionResourceSpendEvent"|"ActionResourceValueBoost"|"ActionResources"|"ActionType"|"ActionUseConditions"|"Active"|"ActiveCharacterLight"|"ActiveCharacterLightBoost"|"ActiveSkeletonSlots"|"AddTagBoost"|"AddedSpells"|"AdvanceSpellsBoost"|"AdvantageBoost"|"AiArchetypeOverrideBoost"|"Ambushing"|"AnimationBlueprint"|"AnimationGameplayEvents"|"AnimationGameplayEventsSingleton"|"AnimationSet"|"AnimationTextKeyEvents"|"AnimationTextKeyEventsSingleton"|"AnimationTriggeredEvents"|"AnimationUpdate"|"AnimationWaterfall"|"AppearanceOverride"|"ApprovalRatings"|"Armor"|"ArmorAbilityModifierCapOverrideBoost"|"ArmorClassBoost"|"ArmorSetState"|"AttackSpellOverride"|"AttackSpellOverrideBoost"|"AttitudesToPlayers"|"AttributeBoost"|"AttributeFlags"|"AttributeFlagsChangedEvent"|"AvailableLevel"|"Avatar"|"Background"|"BackgroundGoals"|"BackgroundPassives"|"BackgroundTag"|"BaseHp"|"BaseStats"|"BlockAbilityModifierFromACBoost"|"BlockFollow"|"BlockRegainHPBoost"|"BodyType"|"BoostBaseUpdated"|"BoostChangedEvent"|"BoostChangedEventsSingleton"|"BoostCondition"|"BoostConditionalState"|"BoostInfo"|"BoostProvider"|"BoostsContainer"|"Bound"|"CCChangeAppearanceDefinition"|"CCCharacterDefinition"|"CCCompanionDefinition"|"CCCreation"|"CCDefinitionCommon"|"CCDefinitionLevelUp"|"CCFullRespec"|"CCFullRespecDefinition"|"CCLevelUp"|"CCLevelUpDefinition"|"CCPrepareSpell"|"CCRespec"|"CCRespecDefinition"|"CCSessionCommon"|"CCState"|"CalendarDaysPassed"|"CalendarStartingDate"|"Camera"|"CameraCombatTarget"|"CameraInSelectorMode"|"CameraSelectorMode"|"CameraSpellTracking"|"CameraTarget"|"CampChest"|"CampEndTheDayState"|"CampPresence"|"CampQuality"|"CampSettings"|"CampSupply"|"CampTotalSupplies"|"CanBeDisarmed"|"CanBeInInventory"|"CanBeLooted"|"CanBeWielded"|"CanDeflectProjectiles"|"CanDoActions"|"CanDoRest"|"CanEnterChasm"|"CanInteract"|"CanLevelUp"|"CanModifyHealth"|"CanMove"|"CanSeeThrough"|"CanSeeThroughBoost"|"CanSense"|"CanShootThrough"|"CanShootThroughBoost"|"CanSpeak"|"CanTrade"|"CanTravel"|"CanTriggerRandomCasts"|"CanWalkThrough"|"CanWalkThroughBoost"|"CannotBePickpocketed"|"CannotBeTakenOut"|"CannotHarmCauseEntityBoost"|"CarryCapacityMultiplierBoost"|"CharacterCreationAppearance"|"CharacterCreationStats"|"CharacterCreationTemplateOverride"|"CharacterHasGeneratedTradeTreasure"|"CharacterUnarmedDamageBoost"|"CharacterWeaponDamageBoost"|"ClassTag"|"Classes"|"ClassesChangedEvent"|"ClientCCBaseDefinitionState"|"ClientCCChangeAppearanceDefinition"|"ClientCCCompanionDefinition"|"ClientCCDefinitionState"|"ClientCCDefinitionStateEx"|"ClientCCDummyDefinition"|"ClientCCFullRespecDefinition"|"ClientCCLevelUpDefinition"|"ClientCharacter"|"ClientCharacterIconRequest"|"ClientCharacterIconResult"|"ClientControl"|"ClientEquipmentVisibilityState"|"ClientEquipmentVisuals"|"ClientInterruptPlayerDecision"|"ClientItem"|"ClientPaperdoll"|"ClientProjectile"|"ClientTimelineActorControl"|"ClientVisualsDesiredState"|"ClientVisualsVisibilityState"|"ClimbOn"|"Clock"|"CombatDelayedFanfare"|"CombatFleeRequest"|"CombatFleeSuccess"|"CombatIsThreatened"|"CombatJoinInCurrentRound"|"CombatJoinInCurrentRoundFailedEvent"|"CombatJoining"|"CombatLateJoinPenalty"|"CombatLeftEvent"|"CombatMerge"|"CombatParticipant"|"CombatRequestCompletedEvent"|"CombatScheduledForDelete"|"CombatStartedEvent"|"CombatState"|"CombatSurfaceTeamSingleton"|"CombatSurprisedJoinRequest"|"CombatSurprisedStealthRequest"|"CombatSwitched"|"CombatThreatRangeChangedEvent"|"CombatantJoinEvent"|"CombatantKilledEvent"|"CombinedLight"|"Concentration"|"ConcentrationChanged"|"ConcentrationClearedEvent"|"ConcentrationDamageCheck"|"ConcentrationIgnoreDamageBoost"|"Construction"|"ConstructionFilling"|"ConstructionTile"|"CriticalHitBoost"|"CriticalHitExtraDiceBoost"|"Cull"|"CurrentlyFollowingParty"|"CustomIcon"|"CustomIconsStorage"|"CustomName"|"CustomStats"|"DamageBonusBoost"|"DamageReductionBoost"|"DamageTakenBonusBoost"|"Darkness"|"DarkvisionRangeBoost"|"DarkvisionRangeMinBoost"|"DarkvisionRangeOverrideBoost"|"Data"|"DeadByDefault"|"Death"|"DeathApplyKnockedOut"|"DeathAssignEntityToUserRequest"|"DeathDeadByDefaultRequest"|"DeathState"|"DeathType"|"DefaultCameraBehavior"|"DelayedFanfareRemovedDuringCombatEvent"|"Detached"|"DetectCrimesBlockBoost"|"DialogState"|"DiedEvent"|"DifficultyCheck"|"DisabledEquipment"|"Disarmable"|"DisplayName"|"DodgeAttackRollBoost"|"Downed"|"DownedEvent"|"DownedStatusBoost"|"DropOnDeathBlocked"|"DualWielding"|"DualWieldingBoost"|"DynamicAnimationTags"|"Effect"|"EffectCameraBehavior"|"EncumbranceState"|"EncumbranceStats"|"EntityThrowDamageBoost"|"EocLevel"|"Equipable"|"EquipmentSlotChangedEvent"|"EquipmentVisual"|"EscortFollower"|"EscortHasStragglers"|"EscortLeader"|"EscortLeaderPriority"|"EscortMember"|"EscortStragglersTracker"|"ExamineDisabled"|"Experience"|"Expertise"|"ExpertiseBonusBoost"|"FTBModeChangedEvent"|"FTBParticipant"|"FTBPaused"|"FTBPlayersTurnEndedEvent"|"FTBPlayersTurnStartedEvent"|"FTBRespect"|"FTBRoundEndedEvent"|"FTBSurfaceTeamSingleton"|"FTBTimeFactorRequests"|"FTBTimeFactorResetRequests"|"FTBTurnBasedChangesRequest"|"FTBZone"|"FTBZoneBlockReason"|"FTBZoneInstigator"|"Faction"|"FactionOverrideBoost"|"FallDamageMultiplierBoost"|"FallToProne"|"FleeCapability"|"Floating"|"FogVolumeRequest"|"FullIllithid"|"GameCameraBehavior"|"GameObjectVisual"|"GameplayLight"|"GameplayLightBoost"|"GameplayObscurityBoost"|"GlobalCombatRequests"|"GlobalLongRestDisabled"|"GlobalShortRestDisabled"|"God"|"GodTag"|"GravityDisabled"|"GravityDisabledUntilMoved"|"GuaranteedChanceRollOutcomeBoost"|"HalfIllithid"|"HalveWeaponDamageBoost"|"HasExclamationDialog"|"HasGeneratedTreasure"|"HasOpened"|"HealBlock"|"HealMaxIncoming"|"HealMaxOutgoing"|"Health"|"Hearing"|"HistoryTargetUUID"|"HitAnimationRequest"|"HitAttacker"|"HitJoinCombatRequest"|"HitLifetime"|"HitMeta"|"HitNotification"|"HitNotificationRequest"|"HitProxy"|"HitProxyOwner"|"HitReaction"|"HitResultEvent"|"HitTarget"|"HitThrownObject"|"HitWeapon"|"HorizontalFOVOverrideBoost"|"HotbarContainer"|"HotbarDecks"|"Icon"|"Identity"|"IdentityState"|"IgnoreDamageThresholdMinBoost"|"IgnoreLowGroundPenaltyBoost"|"IgnorePointBlankDisadvantageBoost"|"IgnoreResistanceBoost"|"IgnoreSurfaceCoverBoost"|"IgnoreSurfaces"|"ImprovisedWeaponWielded"|"ImprovisedWeaponWielding"|"IncreaseMaxHPBoost"|"InitiativeBoost"|"InteractionDisabled"|"InteractionFilter"|"InterruptActionState"|"InterruptConditionallyDisabled"|"InterruptContainer"|"InterruptData"|"InterruptDecision"|"InterruptPreferences"|"InterruptPrepared"|"InterruptZone"|"InterruptZoneParticipant"|"InterruptZoneSource"|"InventoryContainer"|"InventoryData"|"InventoryIsOwned"|"InventoryLocked"|"InventoryMember"|"InventoryMemberTransform"|"InventoryOwner"|"InventoryPropertyCanBePickpocketed"|"InventoryPropertyIsDroppedOnDeath"|"InventoryPropertyIsTradable"|"InventoryStack"|"InventoryStackMember"|"InventoryTopOwner"|"InventoryWeight"|"Invisibility"|"IsCharacter"|"IsCombatPaused"|"IsDoor"|"IsFalling"|"IsGlobal"|"IsGold"|"IsInCombat"|"IsInFTB"|"IsInTurnBasedMode"|"IsItem"|"IsMarkedForDeletion"|"IsSeeThrough"|"IsStoryItem"|"IsSummon"|"ItemBoosts"|"ItemCanMove"|"ItemDestroyed"|"ItemDestroying"|"ItemDye"|"ItemHasGeneratedTreasure"|"ItemHasMoved"|"ItemInUse"|"ItemIsPoisoned"|"ItemPortal"|"ItemTemplateDestroyed"|"JumpFollow"|"JumpMaxDistanceBonusBoost"|"JumpMaxDistanceMultiplierBoost"|"Key"|"Ladder"|"LearnedSpells"|"Level"|"LevelChanged"|"LevelUp"|"Light"|"Lock"|"LockBoost"|"LongRestInScriptPhase"|"LongRestState"|"LongRestTimeline"|"LongRestTimers"|"LongRestUsers"|"Loot"|"LootingState"|"MapMarkerStyle"|"MaterialParameterOverride"|"Max"|"MaximizeHealingBoost"|"MaximumRollResultBoost"|"MinimumRollResultBoost"|"MonkWeaponDamageDiceOverrideBoost"|"Movement"|"MovementSpeedLimitBoost"|"MultiplayerHost"|"MultiplayerUser"|"Net"|"NewInInventory"|"NewItemsInside"|"NewSpellsAddedEvent"|"NonTradable"|"NullifyAbilityBoost"|"ObjectInteraction"|"ObjectSize"|"ObjectSizeBoost"|"ObjectSizeOverrideBoost"|"OffStage"|"OnDamageSpells"|"Origin"|"OriginAppearanceTag"|"OriginPassives"|"OriginTag"|"OriginalIdentity"|"OriginalTemplate"|"OwnedAsLoot"|"OwneeCurrent"|"PartyComposition"|"PartyFollower"|"PartyMember"|"PartyPortals"|"PartyRecipes"|"PartyView"|"PartyWaypoints"|"Passive"|"PassiveConditionalRollInterruptEvent"|"PassiveContainer"|"PassiveRequestTargetTracking"|"PassiveUpdateTargetTracking"|"PassiveUsageCount"|"PassivesUpdatedEvent"|"PasssiveUsageCountIncrementedEvent"|"Pathing"|"PathingDistanceChanged"|"Pause"|"PauseExcluded"|"PhysicalForceRangeBonusBoost"|"Physics"|"PickUpExecuting"|"PickUpRequest"|"PickingState"|"Player"|"PlayerPrepareSpell"|"Proficiency"|"ProficiencyBonusBoost"|"ProficiencyBonusIncreaseBoost"|"ProficiencyBonusOverrideBoost"|"ProficiencyBoost"|"ProficiencyGroup"|"ProgressionAbilityImprovements"|"ProgressionChangedContainers"|"ProgressionContainer"|"ProgressionFeat"|"ProgressionLevelUpChanged"|"ProgressionMeta"|"ProgressionPassives"|"ProgressionReplicatedFeat"|"ProgressionSkills"|"ProgressionSpells"|"ProjectileDeflectBoost"|"ProjectileImpactEvent"|"ProjectileRequestTargetTracking"|"ProjectileSource"|"ProjectileSplitThrowableObjectRequest"|"ProjectileUpdateTargetTracking"|"Race"|"ReadyToBeAddedToInventory"|"ReceivingCriticalDamageOnHitBoost"|"Recruiter"|"RedirectDamageBoost"|"ReduceCriticalAttackThresholdBoost"|"Relation"|"Repose"|"RequestedRoll"|"RerollBoost"|"ResistanceBoost"|"Resistances"|"RestingEntities"|"RollBonusBoost"|"RollInProgress"|"RollModifiers"|"Ruleset"|"RulesetModifiers"|"SavantBoost"|"Savegame"|"SavingThrowRolledEvent"|"ScaleMultiplierBoost"|"SceneAttach"|"Scenery"|"ScriptPropertyCanBePickpocketed"|"ScriptPropertyIsDroppedOnDeath"|"ScriptPropertyIsTradable"|"ScriptedExplosion"|"ServerAIHintAreaTrigger"|"ServerActivationGroupContainer"|"ServerAddedStatusAuraEffectEvent"|"ServerAiArchetype"|"ServerAiGridAreaTrigger"|"ServerAiInterestedInItems"|"ServerAiInterestingItem"|"ServerAiModifiers"|"ServerAnubisExecutor"|"ServerAnubisTag"|"ServerAtmosphereTrigger"|"ServerAvatarContainerTrigger"|"ServerBaseData"|"ServerBaseProficiency"|"ServerBaseSize"|"ServerBaseStats"|"ServerBaseWeapon"|"ServerBlockBronzeTimelinePlacementTrigger"|"ServerBoostBase"|"ServerBoostTag"|"ServerBreadcrumb"|"ServerCCAppearanceVisualTag"|"ServerCCCustomIcon"|"ServerCCEquipmentSetRequest"|"ServerCCGod"|"ServerCCIsCustom"|"ServerCCUpdates"|"ServerCampChestTrigger"|"ServerCampRegionTrigger"|"ServerCanStartCombat"|"ServerCharacter"|"ServerChasmRegionTrigger"|"ServerChasmSeederTrigger"|"ServerCombatGroupMapping"|"ServerCombatLeaveRequest"|"ServerCrimeAreaTrigger"|"ServerCrimeRegionTrigger"|"ServerCrowdCharacterTrigger"|"ServerDarknessActive"|"ServerDeathContinue"|"ServerDeathState"|"ServerDelayDeathCause"|"ServerDialogTag"|"ServerDisarmAttempt"|"ServerDisplayNameList"|"ServerDynamicLayerOwner"|"ServerEnterRequest"|"ServerEocAreaTrigger"|"ServerEocPointTrigger"|"ServerEventTrigger"|"ServerExperienceGaveOut"|"ServerExplorationTrigger"|"ServerFleeBlocked"|"ServerFloorTrigger"|"ServerGameTimer"|"ServerGameplayLightChanges"|"ServerGameplayLightEquipment"|"ServerHotbarOrder"|"ServerIconList"|"ServerImmediateJoin"|"ServerInterruptActionRequests"|"ServerInterruptAddRemoveRequests"|"ServerInterruptDataSingleton"|"ServerInterruptInitialParticipants"|"ServerInterruptTurnOrderInZone"|"ServerInterruptUsed"|"ServerInterruptZoneRequests"|"ServerInventoryContainerData"|"ServerInventoryGroupCheck"|"ServerInventoryIsReplicatedWith"|"ServerInventoryItemDataPopulated"|"ServerIsCurrentOwner"|"ServerIsLatestOwner"|"ServerIsLightBlocker"|"ServerIsOriginalOwner"|"ServerIsPreviousLatestOwner"|"ServerIsPreviousOwner"|"ServerIsUnsummoning"|"ServerIsVisionBlocker"|"ServerItem"|"ServerKiller"|"ServerLeader"|"ServerLightingTrigger"|"ServerMusicVolumeTrigger"|"ServerMusicVolumeTriggerState"|"ServerOsirisTag"|"ServerOwneeHistory"|"ServerOwneeRequest"|"ServerPassiveBase"|"ServerPassiveBoosts"|"ServerPassivePersistentData"|"ServerPeersInRange"|"ServerPickpocket"|"ServerPortalTrigger"|"ServerProficiencyGroupStats"|"ServerProjectile"|"ServerProjectileAttachment"|"ServerProjectileCache"|"ServerProjectileInitialization"|"ServerProjectileSpell"|"ServerRaceTag"|"ServerRecruitedBy"|"ServerRegionTrigger"|"ServerRegistrationSettings"|"ServerRemovedStatusAuraEffectEvent"|"ServerReplicationDependency"|"ServerReplicationDependencyOwner"|"ServerRestPendingType"|"ServerRestTypeChosenEvent"|"ServerRollFinishedEvent"|"ServerRollInProgress"|"ServerRollStartRequest"|"ServerRollStartSpellRequest"|"ServerRoomTrigger"|"ServerSafePosition"|"ServerScriptPassives"|"ServerShapeshiftEquipmentHistory"|"ServerShapeshiftStates"|"ServerShortRestResultEvent"|"ServerSoundVolumeTrigger"|"ServerSpellCastCache"|"ServerSpellCastHitDelay"|"ServerSpellCastInterrupt"|"ServerSpellCastMovement"|"ServerSpellCastMovementInfo"|"ServerSpellCastPendingRequests"|"ServerSpellCastProjectilePathfindCache"|"ServerSpellCastRequests"|"ServerSpellCastResponsible"|"ServerSpellCastState"|"ServerSpellCastUnsheathFallbackTimer"|"ServerSpellCastZoneRange"|"ServerSpellClientInitiated"|"ServerSpellExternals"|"ServerSpellHitRegister"|"ServerSpellInterruptRequests"|"ServerSpellInterruptResults"|"ServerStartTrigger"|"ServerStatsAreaTrigger"|"ServerStatus"|"ServerStatusActive"|"ServerStatusAddEvent"|"ServerStatusAddedFromSaveLoad"|"ServerStatusApplyEvent"|"ServerStatusAttemptEvent"|"ServerStatusAttemptFailedEvent"|"ServerStatusAura"|"ServerStatusAuraContainer"|"ServerStatusBoostsProcessed"|"ServerStatusCause"|"ServerStatusDifficultyModifiers"|"ServerStatusDispelRollCheck"|"ServerStatusDownedChangedEvent"|"ServerStatusEvent"|"ServerStatusOwnership"|"ServerStatusPerforming"|"ServerStatusRefreshed"|"ServerStatusRemoveEvent"|"ServerStatusScheduledForDeletion"|"ServerStatusTurnStartEvent"|"ServerStatusUnique"|"ServerStatusUpdateTargetTracking"|"ServerSurface"|"ServerTeleportTrigger"|"ServerTemplateChangedEvent"|"ServerTemplateTag"|"ServerTemplateTransformedEvent"|"ServerTimelineSceneTrigger"|"ServerToggledPassives"|"ServerTriggerCachedLeaveEvents"|"ServerTriggerEventConfig"|"ServerTriggerLoadedHandled"|"ServerTriggerRegisteredFor"|"ServerTriggerWorldAutoTriggered"|"ServerUnsheath"|"ServerUnsheathScriptOverride"|"ServerUpdatedRegisteredFor"|"ServerUpdatedRegisteredForItems"|"ServerUserSnapshot"|"ServerVariableManager"|"ShapeshiftHealthReservation"|"ShapeshiftState"|"ShootThroughType"|"ShortRest"|"ShortRestConsumeResources"|"ShouldDestroyOnSpellCast"|"Sight"|"SightData"|"SightEntityViewshed"|"SightRangeAdditiveBoost"|"SightRangeMaximumBoost"|"SightRangeMinimumBoost"|"SightRangeOverrideBoost"|"SimpleCharacter"|"SkillBoost"|"SkillCheckEvent"|"Sound"|"SourceAdvantageBoost"|"Speaker"|"SpellAiConditions"|"SpellBook"|"SpellBookChanged"|"SpellBookCooldowns"|"SpellBookPrepares"|"SpellCastAnimationInfo"|"SpellCastAnimationRequest"|"SpellCastCache"|"SpellCastCanBeTargeted"|"SpellCastCounteredEvent"|"SpellCastDataCache"|"SpellCastDestroyEvent"|"SpellCastEvent"|"SpellCastExecutionTime"|"SpellCastFinishedEvent"|"SpellCastHitEvent"|"SpellCastInterruptResults"|"SpellCastInterruptsUsed"|"SpellCastIsCasting"|"SpellCastJumpStartEvent"|"SpellCastLogicExecutionEndEvent"|"SpellCastLogicExecutionStartEvent"|"SpellCastMoveDuringCastUpdateEvent"|"SpellCastMovement"|"SpellCastMovementAndPrecalculationEndEvent"|"SpellCastMovementInfo"|"SpellCastOutcome"|"SpellCastPrepareEndEvent"|"SpellCastPrepareStartEvent"|"SpellCastPreviewEndEvent"|"SpellCastRequestTargetTracking"|"SpellCastRolls"|"SpellCastSpellRollAbortEvent"|"SpellCastState"|"SpellCastTargetHitEvent"|"SpellCastTargetHitInterruptEvent"|"SpellCastTargetReactionEvent"|"SpellCastTargetsChangedEvent"|"SpellCastTextKeyEvent"|"SpellCastThrowPickupPositionChangedEvent"|"SpellCastUpdateTargetTracking"|"SpellCastWeaponSetChangeRequest"|"SpellContainer"|"SpellModificationContainer"|"SpellResistanceBoost"|"SpellSaveDCBoost"|"SpellSyncTargeting"|"SpellsLearnedEvent"|"StaticPhysics"|"Stats"|"StatsAppliedEvent"|"StatusBoostsRefreshed"|"StatusCause"|"StatusContainer"|"StatusID"|"StatusImmunities"|"StatusImmunityBoost"|"StatusIncapacitated"|"StatusIndicateDarkness"|"StatusLifetime"|"StatusLoseControl"|"StatusVisualDisabled"|"Stealth"|"Steering"|"StoryShortRestDisabled"|"SummonAddConcentrationRequest"|"SummonAddToExistingConcentrationRequest"|"SummonAttachToProjectileRequest"|"SummonContainer"|"SummonCreatedEvent"|"SummonDespawnRequest"|"SummonExpiredRequest"|"SummonLateJoinPenalty"|"SummonLifetime"|"SummonOwnerSetEvent"|"SummonPlaceInInventoryRequest"|"SummonSetLifetimeRequest"|"SurfacePathInfluences"|"TLPreviewDummy"|"TadpolePowers"|"TadpoleTreeState"|"Tadpoled"|"Tag"|"TagsChangedEvent"|"TemplateAnimationSetOverride"|"TemporaryHPBoost"|"ThreatRange"|"TimeFactor"|"TimelineActorData"|"TradeBuybackData"|"Trader"|"Transform"|"TranslateChanged"|"TranslateChangedEvent"|"TriggerArea"|"TriggerContainer"|"TriggerIsInsideOf"|"TriggerType"|"TriggerUpdatedContainer"|"TriggerUpdatedPhysics"|"TurnBased"|"TurnOrder"|"UnlockInterruptBoost"|"UnlockSpellBoost"|"UnlockSpellVariantBoost"|"UnresolvedHitNotification"|"Unsheath"|"Use"|"UseAction"|"UseBoosts"|"UseSocket"|"UserAvatar"|"UserReservedFor"|"Uuid"|"UuidToHandleMapping"|"Value"|"Visual"|"Voice"|"VoiceTag"|"WalkOn"|"Weapon"|"WeaponAttackRollAbilityOverrideBoost"|"WeaponAttackRollBonusBoost"|"WeaponAttackTypeOverrideBoost"|"WeaponDamageBoost"|"WeaponDamageDieOverrideBoost"|"WeaponDamageResistanceBoost"|"WeaponDamageTypeOverrideBoost"|"WeaponEnchantmentBoost"|"WeaponPropertiesChangedEvent"|"WeaponPropertyBoost"|"WeaponSet"|"WeightBoost"|"WeightCategoryBoost"|"Wielded"|"Wielding"|"WieldingHistory"
--- @alias ExtQueryType string|"Max"|"UuidToHandleMapping"
--- @alias ExtResourceManagerType string|"AbilityDefaultValues"|"AbilityDistributionPreset"|"AbilityList"|"ActionResource"|"ActionResourceGroup"|"AnimationSetPriority"|"ApprovalRating"|"AreaLevelOverride"|"AvatarContainerTemplate"|"Background"|"BackgroundGoal"|"CalendarDayRange"|"CampChestTemplate"|"CharacterCreationAccessorySet"|"CharacterCreationAppearanceMaterial"|"CharacterCreationAppearanceVisual"|"CharacterCreationEquipmentIcons"|"CharacterCreationEyeColor"|"CharacterCreationHairColor"|"CharacterCreationIconSettings"|"CharacterCreationMaterialOverride"|"CharacterCreationPassiveAppearance"|"CharacterCreationPreset"|"CharacterCreationSharedVisual"|"CharacterCreationSkinColor"|"CharacterCreationVOLine"|"CinematicArenaFrequencyGroup"|"ClassDescription"|"ColorDefinition"|"CompanionPreset"|"ConditionError"|"CustomDice"|"DLC"|"DeathTypeEffect"|"DifficultyClass"|"DisturbanceProperty"|"EncumbranceType"|"EquipmentDefaultValues"|"EquipmentList"|"EquipmentType"|"ExperienceReward"|"Faction"|"Feat"|"FeatDefaultValues"|"FeatDescription"|"FeatSoundState"|"FixedHotBarSlot"|"Flag"|"FlagSoundState"|"God"|"GoldReward"|"Gossip"|"ItemThrowParams"|"ItemWallTemplate"|"LevelMap"|"LimbsMapping"|"LongRestCost"|"ManagedStatusVFX"|"Max"|"MultiEffectInfo"|"Origin"|"OriginIntroEntity"|"PassiveDefaultValues"|"PassiveList"|"PassiveVFX"|"PreparedSpellDefaultValues"|"Progression"|"ProgressionDescription"|"ProjectileDefault"|"Race"|"RandomCastOutcome"|"Ruleset"|"RulesetModifier"|"RulesetModifierOption"|"RulesetSelectionPreset"|"RulesetValue"|"ScriptMaterialParameterOverride"|"ScriptMaterialPresetOverride"|"ShapeshiftRule"|"SkillDefaultValues"|"SkillList"|"SpellDefaultValues"|"SpellList"|"SpellMetaCondition"|"SpellSoundTrajectory"|"StatusSoundState"|"SurfaceCursorMessage"|"TadpolePower"|"Tag"|"TagSoundState"|"TooltipExtraText"|"TooltipUpcastDescription"|"TrajectoryRules"|"TutorialEntries"|"TutorialEvent"|"TutorialModalEntries"|"TutorialUnifiedEntry"|"VFX"|"VisualLocatorAttachment"|"Voice"|"WeaponAnimationSet"|"WeightCategory"
--- @alias ExtSystemType string|"AnimationBlueprint"|"ClientCharacterIconRender"|"ClientCharacterManager"|"ClientEquipmentVisuals"|"ClientVisual"|"ClientVisualsVisibilityState"|"Max"|"PickingHelper"|"ServerActionResource"|"ServerBoost"|"ServerCapabilities"|"ServerCastRequest"|"ServerCombat"|"ServerConcentration"|"ServerDialog"|"ServerExperience"|"ServerFTBZone"|"ServerHit"|"ServerInterruptDecision"|"ServerInterruptManagement"|"ServerInterruptRequests"|"ServerLongRest"|"ServerPassive"|"ServerProgression"|"ServerRoll"|"ServerShapeshift"|"ServerShortRest"|"ServerSpell"|"ServerSpellCooldown"|"ServerSpellLearning"|"ServerStats"|"ServerSummonDespawn"|"ServerSummonSpawn"|"ServerTemplateChange"|"ServerTurnOrder"|"ServerVisual"|"SoundRouting"
--- @alias FleeErrorFlags string|"CantMove"|"Condition"|"Downed"|"EnemyTooClose"|"Incapacitated"|"PartyFollower"|"Region"|"Summon"
--- @alias ForceFunctorAggression string|"Aggressive"|"Friendly"|"Neutral"|"Undefined"
--- @alias ForceFunctorOrigin string|"OriginToEntity"|"OriginToTarget"|"TargetToEntity"|"Undefined"
--- @alias FunctorContextType string|"AttackPosition"|"AttackTarget"|"Equip"|"Interrupt"|"Move"|"NearbyAttacked"|"NearbyAttacking"|"None"|"Source"|"Target"
--- @alias GameActionType string|"GameObjectMoveAction"|"PathAction"|"RainAction"|"StatusDomeAction"|"StormAction"|"TornadoAction"|"WallAction"
--- @alias Gender string|"Female"|"Male"
--- @alias GuiButtonFlags string|"MouseButtonLeft"|"MouseButtonMiddle"|"MouseButtonRight"
--- @alias GuiChildFlags string|"AlwaysUseWindowPadding"|"AutoResizeX"|"AutoResizeY"|"Border"|"ChildAlwaysAutoResize"|"FrameStyle"|"NavFlattened"|"ResizeX"|"ResizeY"
--- @alias GuiColor string|"Border"|"BorderShadow"|"Button"|"ButtonActive"|"ButtonHovered"|"CheckMark"|"ChildBg"|"DragDropTarget"|"FrameBg"|"FrameBgActive"|"FrameBgHovered"|"Header"|"HeaderActive"|"HeaderHovered"|"MenuBarBg"|"ModalWindowDimBg"|"NavHighlight"|"NavWindowingDimBg"|"NavWindowingHighlight"|"PlotHistogram"|"PlotHistogramHovered"|"PlotLines"|"PlotLinesHovered"|"PopupBg"|"ResizeGrip"|"ResizeGripActive"|"ResizeGripHovered"|"ScrollbarBg"|"ScrollbarGrab"|"ScrollbarGrabActive"|"ScrollbarGrabHovered"|"Separator"|"SeparatorActive"|"SeparatorHovered"|"SliderGrab"|"SliderGrabActive"|"Tab"|"TabActive"|"TabDimmedSelectedOverline"|"TabHovered"|"TabUnfocused"|"TabUnfocusedActive"|"TableBorderLight"|"TableBorderStrong"|"TableHeaderBg"|"TableRowBg"|"TableRowBgAlt"|"Text"|"TextDisabled"|"TextLink"|"TextSelectedBg"|"TitleBg"|"TitleBgActive"|"TitleBgCollapsed"|"WindowBg"
--- @alias GuiColorEditFlags string|"AlphaBar"|"AlphaNoBg"|"AlphaOpaque"|"AlphaPreview"|"AlphaPreviewHalf"|"DisplayHSV"|"DisplayHex"|"DisplayRGB"|"Float"|"HDR"|"InputHSV"|"InputRGB"|"NoAlpha"|"NoBorder"|"NoDragDrop"|"NoInputs"|"NoLabel"|"NoOptions"|"NoPicker"|"NoSidePreview"|"NoSmallPreview"|"NoTooltip"|"PickerHueBar"|"PickerHueWheel"|"Uint8"
--- @alias GuiComboFlags string|"HeightLarge"|"HeightLargest"|"HeightRegular"|"HeightSmall"|"NoArrowButton"|"NoPreview"|"PopupAlignLeft"|"WidthFitPreview"
--- @alias GuiCond string|"Always"|"Appearing"|"FirstUseEver"|"Once"
--- @alias GuiDragFlags string|"AllowNullID"|"NoDisableHover"|"NoHoldToOpenOthers"|"NoPreviewTooltip"
--- @alias GuiDropFlags string|"NoDrawDefaultRect"|"NoPreviewTooltip"
--- @alias GuiInputTextFlags string|"AllowTabInput"|"AlwaysOverwrite"|"AutoSelectAll"|"CharsDecimal"|"CharsHexadecimal"|"CharsNoBlank"|"CharsScientific"|"CharsUppercase"|"CtrlEnterForNewLine"|"DisplayEmptyRefVal"|"ElideLeft"|"EnterReturnsTrue"|"EscapeClearsAll"|"Multiline"|"NoHorizontalScroll"|"NoUndoRedo"|"ParseEmptyRefVal"|"Password"|"ReadOnly"
--- @alias GuiItemFlags string|"AllowDuplicateId"|"AllowItemOverlap"|"AutoClosePopups"|"ButtonRepeat"|"Disabled"|"ItemReadOnly"|"NoItemNav"|"NoMarkEdited"|"NoNavDefaultFocus"|"NoNavDisableMouseHover"|"NoTabStop"|"NoWindowHoverableCheck"
--- @alias GuiItemStatusFlags string|"Active"|"Deactivated"|"Edited"|"Focused"|"HasClipRect"|"HasDeactivated"|"HasDisplayRect"|"HasShortcut"|"HoveredRect"|"HoveredWindow"|"ToggledOpen"|"ToggledSelection"|"Visible"
--- @alias GuiMeasureScaling string|"Absolute"|"Scaled"
--- @alias GuiPopupFlags string|"AnyPopupId"|"AnyPopupLevel"|"MouseButtonMiddle"|"MouseButtonRight"|"NoOpenOverExistingPopup"|"NoOpenOverItems"|"NoReopen"
--- @alias GuiSelectableFlags string|"AllowDoubleClick"|"AllowOverlap"|"DontClosePopups"|"Highlight"|"SelectableDisabled"|"SpanAllColumns"
--- @alias GuiSliderFlags string|"AlwaysClamp"|"ClampOnInput"|"ClampZeroRange"|"Logarithmic"|"NoInput"|"NoRoundToFormat"|"NoSpeedTweaks"|"WrapAround"
--- @alias GuiSortDirection string|"Ascending"|"Descending"|"None"
--- @alias GuiStyleVar string|"Alpha"|"ButtonTextAlign"|"CellPadding"|"ChildBorderSize"|"ChildRounding"|"DisabledAlpha"|"FrameBorderSize"|"FramePadding"|"FrameRounding"|"GrabMinSize"|"GrabRounding"|"ImageBorderSize"|"IndentSpacing"|"ItemInnerSpacing"|"ItemSpacing"|"PopupBorderSize"|"PopupRounding"|"ScrollbarRounding"|"ScrollbarSize"|"SelectableTextAlign"|"SeparatorTextAlign"|"SeparatorTextBorderSize"|"SeparatorTextPadding"|"TabBarBorderSize"|"TabBarOverlineSize"|"TabBorderSize"|"TabRounding"|"TableAngledHeadersAngle"|"TableAngledHeadersTextAlign"|"WindowBorderSize"|"WindowMinSize"|"WindowPadding"|"WindowRounding"|"WindowTitleAlign"
--- @alias GuiTabBarFlags string|"AutoSelectNewTabs"|"DrawSelectedOverline"|"FittingPolicyResizeDown"|"FittingPolicyScroll"|"NoCloseWithMiddleMouseButton"|"NoTabListScrollingButtons"|"NoTooltip"|"Reorderable"|"TabListPopupButton"
--- @alias GuiTabItemFlags string|"Leading"|"NoAssumedClosure"|"NoCloseWithMiddleMouseButton"|"NoPushId"|"NoReorder"|"NoTooltip"|"SetSelected"|"Trailing"|"UnsavedDocument"
--- @alias GuiTableColumnFlags string|"AngledHeader"|"DefaultHide"|"DefaultSort"|"Disabled"|"IndentDisable"|"IndentEnable"|"NoClip"|"NoHeaderLabel"|"NoHeaderWidth"|"NoHide"|"NoReorder"|"NoResize"|"NoSort"|"NoSortAscending"|"NoSortDescending"|"PreferSortAscending"|"PreferSortDescending"|"WidthFixed"|"WidthStretch"
--- @alias GuiTableFlags string|"Borders"|"BordersH"|"BordersInner"|"BordersInnerH"|"BordersInnerV"|"BordersOuter"|"BordersOuterH"|"BordersOuterV"|"BordersV"|"ContextMenuInBody"|"Hideable"|"HighlightHoveredColumn"|"NoBordersInBody"|"NoBordersInBodyUntilResize"|"NoClip"|"NoHostExtendX"|"NoHostExtendY"|"NoKeepColumnsVisible"|"NoPadInnerX"|"NoPadOuterX"|"NoSavedSettings"|"PadOuterX"|"PreciseWidths"|"Reorderable"|"Resizable"|"RowBg"|"ScrollX"|"ScrollY"|"SizingFixedFit"|"SizingFixedSame"|"SizingStretchProp"|"SizingStretchSame"|"SortMulti"|"SortTristate"|"Sortable"
--- @alias GuiTableRowFlags string|"Headers"
--- @alias GuiTreeNodeFlags string|"AllowOverlap"|"Bullet"|"CollapsingHeader"|"DefaultOpen"|"FramePadding"|"Framed"|"Leaf"|"NavLeftJumpsBackHere"|"NoAutoOpenOnLog"|"NoTreePushOnOpen"|"OpenOnArrow"|"OpenOnDoubleClick"|"Selected"|"SpanAllColumns"|"SpanAvailWidth"|"SpanFullWidth"|"SpanTextWidth"
--- @alias GuiWindowFlags string|"AlwaysAutoResize"|"AlwaysHorizontalScrollbar"|"AlwaysVerticalScrollbar"|"HorizontalScrollbar"|"MenuBar"|"NoBackground"|"NoBringToFrontOnFocus"|"NoCollapse"|"NoDecoration"|"NoFocusOnAppearing"|"NoInputs"|"NoMouseInputs"|"NoMove"|"NoNav"|"NoNavFocus"|"NoNavInputs"|"NoResize"|"NoSavedSettings"|"NoScrollWithMouse"|"NoScrollbar"|"NoTitleBar"|"UnsavedDocument"
--- @alias HandednessType string|"Any"|"One"|"Two"
--- @alias HealDirection string|"Incoming"|"Outgoing"
--- @alias HealEffect string|"Behavior"|"Heal"|"HealSharing"|"HealSharingReflected"|"Lifesteal"|"Necromantic"|"NegativeDamage"|"None"|"ResistDeath"|"Script"|"Sitting"|"Surface"|"Unknown4"|"Unknown9"
--- @alias HitType string|"DoT"|"Magic"|"Melee"|"Ranged"|"Reflected"|"Surface"|"WeaponDamage"
--- @alias HitWith string|"CrushByFall"|"FallDamage"|"Item"|"Magic"|"None"|"Projectile"|"Redirection"|"Surface"|"Trap"|"Unknown10"|"Unknown11"|"Weapon"
--- @alias IngredientTransformType string|"Boost"|"Consume"|"None"|"Poison"|"Transform"
--- @alias IngredientType string|"Category"|"None"|"Object"|"Property"
--- @alias InputModifier string|"Alt"|"Ctrl"|"Gui"|"Shift"
--- @alias InputRawType string|"a"|"apostrophe"|"audio_mute"|"audio_next"|"audio_play"|"audio_prev"|"audio_stop"|"b"|"back"|"backslash"|"backspace"|"backtick"|"c"|"capslock"|"comma"|"controller_a"|"controller_b"|"controller_x"|"controller_y"|"d"|"del"|"dot"|"down"|"dpad_down"|"dpad_left"|"dpad_right"|"dpad_up"|"e"|"end"|"enter"|"equals"|"escape"|"f"|"f1"|"f10"|"f11"|"f12"|"f13"|"f14"|"f15"|"f16"|"f17"|"f18"|"f19"|"f2"|"f20"|"f21"|"f22"|"f23"|"f24"|"f3"|"f4"|"f5"|"f6"|"f7"|"f8"|"f9"|"g"|"guide"|"h"|"home"|"hyphen"|"i"|"insert"|"item1"|"item10"|"item11"|"item2"|"item3"|"item4"|"item5"|"item6"|"item7"|"item8"|"item9"|"j"|"k"|"kp_0"|"kp_1"|"kp_2"|"kp_3"|"kp_4"|"kp_5"|"kp_6"|"kp_7"|"kp_8"|"kp_9"|"kp_divide"|"kp_enter"|"kp_minus"|"kp_multiply"|"kp_period"|"kp_plus"|"l"|"lalt"|"lctrl"|"left"|"left2"|"leftbracket"|"leftshoulder"|"leftstick"|"leftstick_x"|"leftstick_xneg"|"leftstick_xpos"|"leftstick_y"|"leftstick_yneg"|"leftstick_ypos"|"leftstickpress"|"lefttrigger"|"lgui"|"lshift"|"m"|"middle"|"mode"|"motion"|"motion_xneg"|"motion_xpos"|"motion_yneg"|"motion_ypos"|"n"|"num_0"|"num_1"|"num_2"|"num_3"|"num_4"|"num_5"|"num_6"|"num_7"|"num_8"|"num_9"|"numlock"|"o"|"p"|"pagedown"|"pageup"|"pause"|"printscreen"|"q"|"r"|"ralt"|"rctrl"|"rgui"|"right"|"right2"|"rightbracket"|"rightshoulder"|"rightstick"|"rightstick_x"|"rightstick_xneg"|"rightstick_xpos"|"rightstick_y"|"rightstick_yneg"|"rightstick_ypos"|"rightstickpress"|"righttrigger"|"rshift"|"s"|"scrolllock"|"slash"|"space"|"space2"|"start"|"t"|"tab"|"touch_flick"|"touch_hold"|"touch_pinch_in"|"touch_pinch_out"|"touch_press"|"touch_rotate"|"touch_tap"|"u"|"up"|"v"|"volume_down"|"volume_up"|"w"|"wheel_xneg"|"wheel_xpos"|"wheel_yneg"|"wheel_ypos"|"x"|"x1"|"x2"|"y"|"z"
--- @alias InputState string|"Pressed"|"Released"
--- @alias InputType string|"AcceleratedRepeat"|"ClearOnRelease"|"Hold"|"Press"|"Release"|"Repeat"|"ValueChange"
--- @alias InterruptRequestSource string|"Boost"|"Debug"|"LearnedSpell"
--- @alias IsTradableType string|"Default"|"NonTradable"|"Tradable"
--- @alias ItemDataRarity string|"Common"|"Divine"|"Epic"|"Legendary"|"Rare"|"Sentinel"|"Uncommon"|"Unique"
--- @alias ItemSlot string|"Amulet"|"Boots"|"Breast"|"Cloak"|"Gloves"|"Helmet"|"Horns"|"MainHand"|"MeleeMainHand"|"MeleeOffHand"|"MusicalInstrument"|"OffHand"|"Overhead"|"RangedMainHand"|"RangedOffHand"|"Ring"|"Ring2"|"Underwear"|"VanityBody"|"VanityBoots"|"Wings"
--- @alias ItemUseType string|"Arrow"|"Consumable"|"Grenade"|"None"|"Potion"|"Scroll"|"Throwable"|"Unknown1"
--- @alias LongRestState string|"Cancelled"|"Created"|"Finished"|"ReadyForDelete"|"RestTypeRequest"|"ScriptPhaseDenied"|"ScriptPhasePending"|"ScriptPhaseRequested"|"ScriptPhaseStart"|"ScriptPhaseTimeout"|"Validation"
--- @alias LongRestType string|"CanceledByObserver"|"CanceledByOwner"|"CantRest"|"Exhausted"|"Full"|"None"
--- @alias LongRestUserState string|"Finished"|"Finishing"|"Initial"|"RequestingData"|"Resting"|"StartingRest"|"WaitingConfirmation"|"WaitingOthers"
--- @alias LuaTypeId string|"Any"|"Array"|"Boolean"|"Enumeration"|"Float"|"Function"|"Integer"|"Map"|"Module"|"Nullable"|"Object"|"Set"|"String"|"Tuple"|"Unknown"|"Variant"|"Void"
--- @alias MultiEffectFlags string|"DetachSource"|"DetachTarget"|"Enabled"|"KeepRotation"|"KeepScale"|"MainHand"|"OffHand"|"UseDistance"|"UseOrientDirection"|"UseScaleOverride"
--- @alias NarrativeCombatRequestType string|"ClearInNarrativeCombat"|"Create"|"Destroy"|"SetInNarrativeCombat"
--- @alias NetMessage string|"NETMSG_ACHIEVEMENT_PROGRESS_MESSAGE"|"NETMSG_ACHIEVEMENT_UNLOCKED_MESSAGE"|"NETMSG_ACTIVE_ROLL_MODIFIERS"|"NETMSG_ACT_OVER"|"NETMSG_ATMOSPHERE_OVERRIDE"|"NETMSG_CACHETEMPLATE"|"NETMSG_CAMERA_ACTIVATE"|"NETMSG_CAMERA_ARRIVED"|"NETMSG_CAMERA_ROTATE"|"NETMSG_CAMERA_SPLINE"|"NETMSG_CAMERA_TARGET"|"NETMSG_CAST_ALL"|"NETMSG_CHANGE_RULESETS"|"NETMSG_CHARACTER_ACTION"|"NETMSG_CHARACTER_ACTION_DATA"|"NETMSG_CHARACTER_ACTION_REQUEST_RESULT"|"NETMSG_CHARACTER_ACTIVATE"|"NETMSG_CHARACTER_AOO"|"NETMSG_CHARACTER_ASSIGN"|"NETMSG_CHARACTER_BEHAVIOR"|"NETMSG_CHARACTER_CANCEL_DROP_ENTITY"|"NETMSG_CHARACTER_CHANGE_OWNERSHIP"|"NETMSG_CHARACTER_CONFIRMATION"|"NETMSG_CHARACTER_CONTROL"|"NETMSG_CHARACTER_CREATE"|"NETMSG_CHARACTER_CREATION_ABORT"|"NETMSG_CHARACTER_CREATION_LEVELUP"|"NETMSG_CHARACTER_CREATION_PLAYORIGINTIMELINE"|"NETMSG_CHARACTER_CREATION_READY"|"NETMSG_CHARACTER_CREATION_RESPEC"|"NETMSG_CHARACTER_CREATION_UPDATE"|"NETMSG_CHARACTER_DEACTIVATE"|"NETMSG_CHARACTER_DESTROY"|"NETMSG_CHARACTER_DIALOG"|"NETMSG_CHARACTER_DROP_ENTITY"|"NETMSG_CHARACTER_END_REPOSE"|"NETMSG_CHARACTER_ERROR"|"NETMSG_CHARACTER_IMPROV_WEAPON"|"NETMSG_CHARACTER_IN_DIALOG"|"NETMSG_CHARACTER_ITEM_USED"|"NETMSG_CHARACTER_LOCKPICK"|"NETMSG_CHARACTER_LOOT"|"NETMSG_CHARACTER_MOVEMENT_FALLING"|"NETMSG_CHARACTER_MOVE_FAILED"|"NETMSG_CHARACTER_OFFSTAGE"|"NETMSG_CHARACTER_ORIGINAL_TEMPLATE_UPDATE"|"NETMSG_CHARACTER_PATHING"|"NETMSG_CHARACTER_PICKPOCKET"|"NETMSG_CHARACTER_REQUEST_ARMOR_SET_SWITCH"|"NETMSG_CHARACTER_REQUEST_WEAPON_SET_SWITCH"|"NETMSG_CHARACTER_SELECTED_CLIMB_ON"|"NETMSG_CHARACTER_STATUS"|"NETMSG_CHARACTER_STATUS_LIFETIME"|"NETMSG_CHARACTER_STEERING"|"NETMSG_CHARACTER_STOWED_VISIBILITY"|"NETMSG_CHARACTER_TELEPORT"|"NETMSG_CHARACTER_TRANSFORM"|"NETMSG_CHARACTER_TRAPDISARM"|"NETMSG_CHARACTER_UNSHEATHING"|"NETMSG_CHARACTER_UPDATE"|"NETMSG_CHARACTER_USE_MOVEMENT"|"NETMSG_CHAT"|"NETMSG_CLEAR_RECIPE_NEW_FLAG"|"NETMSG_CLIENT_ACCEPT"|"NETMSG_CLIENT_CONNECT"|"NETMSG_CLIENT_DIALOG_JOIN_PRIVACY_FAILED"|"NETMSG_CLIENT_DIALOG_PRIVACY_CHANGED"|"NETMSG_CLIENT_GAME_SETTINGS"|"NETMSG_CLIENT_JOINED"|"NETMSG_CLIENT_LEFT"|"NETMSG_CLOSED_MESSAGE_BOX_MESSAGE"|"NETMSG_CLOSE_CUSTOM_BOOK_UI_MESSAGE"|"NETMSG_CLOSE_UI_MESSAGE"|"NETMSG_COMBATLOG"|"NETMSG_COMBATLOGENTRIES"|"NETMSG_COMBATLOGITEMINTERACTION"|"NETMSG_COMBINE_ITEMS"|"NETMSG_COMBINE_RESULT"|"NETMSG_CUSTOM_STATS_CREATE"|"NETMSG_CUSTOM_STATS_DEFINITION_CREATE"|"NETMSG_CUSTOM_STATS_DEFINITION_REMOVE"|"NETMSG_CUSTOM_STATS_DEFINITION_UPDATE"|"NETMSG_CUSTOM_STATS_UPDATE"|"NETMSG_DARKNESSTILE_UPDATE"|"NETMSG_DECLUTTERED_ITEMS"|"NETMSG_DIALOG_ACTORJOINS_MESSAGE"|"NETMSG_DIALOG_ACTORLEAVES_MESSAGE"|"NETMSG_DIALOG_ANSWER_HIGHLIGHT_MESSAGE"|"NETMSG_DIALOG_ANSWER_MESSAGE"|"NETMSG_DIALOG_HISTORY_MESSAGE"|"NETMSG_DIALOG_INSTANCEDIALOGCHANGED"|"NETMSG_DIALOG_INVALID_ANSWER"|"NETMSG_DIALOG_LISTEN"|"NETMSG_DIALOG_NODE_MESSAGE"|"NETMSG_DIALOG_REPLACESPEAKER_MESSAGE"|"NETMSG_DIALOG_REQUEST_IN_DIALOGUE_ATTACK_MESSAGE"|"NETMSG_DIALOG_STATE_MESSAGE"|"NETMSG_DIALOG_SUGGESTANSWER_MESSAGE"|"NETMSG_DIALOG_TIMELINE_UNLOADED_MESSAGE"|"NETMSG_DIPLOMACY"|"NETMSG_DLC_UPDATE"|"NETMSG_DUALWIELDING_TOGGLE"|"NETMSG_ECS_REPLICATION"|"NETMSG_EFFECT_CREATE"|"NETMSG_EFFECT_DESTROY"|"NETMSG_EFFECT_FORGET"|"NETMSG_END_THE_DAY"|"NETMSG_FLAG_UPDATE"|"NETMSG_FOLLOWER_CANT_USE_ITEM"|"NETMSG_FORCE_TURN_BASED_END_PLAYER_TURN_REQUEST"|"NETMSG_FORCE_TURN_BASED_TOGGLE_REQUEST"|"NETMSG_FULL_RESPEC_UPDATE"|"NETMSG_GAMEACTION"|"NETMSG_GAMECONTROL_PRICETAG"|"NETMSG_GAMECONTROL_UPDATE_C2S"|"NETMSG_GAMECONTROL_UPDATE_S2C"|"NETMSG_GAMEOVER"|"NETMSG_GAMETIME_SYNC"|"NETMSG_GIVE_REWARD"|"NETMSG_HANDSHAKE"|"NETMSG_HOST_LEFT"|"NETMSG_HOST_REFUSE"|"NETMSG_HOST_REFUSEPLAYER"|"NETMSG_HOST_WELCOME"|"NETMSG_HOTBAR_COLUMN_SET"|"NETMSG_HOTBAR_CURRENT_DECK_SET"|"NETMSG_HOTBAR_CUSTOM_DECK"|"NETMSG_HOTBAR_LOCK_SET"|"NETMSG_HOTBAR_ROWS_SET"|"NETMSG_HOTBAR_ROW_REMOVE"|"NETMSG_HOTBAR_SLOT_REMOVE_IS_NEW_FLAG"|"NETMSG_HOTBAR_SLOT_SET"|"NETMSG_INTERRUPT_DECISION"|"NETMSG_INTERRUPT_SET_ASK_DATA"|"NETMSG_INTERRUPT_SET_ENABLED_DATA"|"NETMSG_INVENTORY_ADD"|"NETMSG_INVENTORY_CREATE"|"NETMSG_INVENTORY_CREATE_AND_OPEN"|"NETMSG_INVENTORY_CREATE_NEW"|"NETMSG_INVENTORY_DESTROY"|"NETMSG_INVENTORY_DESTROY_NEW"|"NETMSG_INVENTORY_EQUIP"|"NETMSG_INVENTORY_ITEM_UPDATE"|"NETMSG_INVENTORY_LOCK"|"NETMSG_INVENTORY_LOCKSTATE_SYNC"|"NETMSG_INVENTORY_MOVETOSLOT"|"NETMSG_INVENTORY_SORT"|"NETMSG_INVENTORY_TRANSFER"|"NETMSG_INVENTORY_UNEQUIP"|"NETMSG_INVENTORY_VIEW_CREATE"|"NETMSG_INVENTORY_VIEW_DESTROY"|"NETMSG_INVENTORY_VIEW_SORT"|"NETMSG_INVENTORY_VIEW_UPDATE_ITEMS"|"NETMSG_INVENTORY_VIEW_UPDATE_PARENTS"|"NETMSG_ITEM_ACTION"|"NETMSG_ITEM_ACTIVATE"|"NETMSG_ITEM_CONFIRMATION"|"NETMSG_ITEM_CREATE"|"NETMSG_ITEM_DEACTIVATE"|"NETMSG_ITEM_DESTINATION"|"NETMSG_ITEM_DESTROY"|"NETMSG_ITEM_MOVED_INFORM"|"NETMSG_ITEM_MOVE_TO_WORLD"|"NETMSG_ITEM_OFFSTAGE"|"NETMSG_ITEM_STATUS"|"NETMSG_ITEM_STATUS_LIFETIME"|"NETMSG_ITEM_TOGGLE_IS_WARE"|"NETMSG_ITEM_TRANSFORM"|"NETMSG_ITEM_UPDATE"|"NETMSG_ITEM_USE_REMOTELY"|"NETMSG_JOURNALDIALOGLOG_UPDATE"|"NETMSG_JOURNALRECIPE_UPDATE"|"NETMSG_JOURNAL_RESET"|"NETMSG_LEVEL_CREATED"|"NETMSG_LEVEL_INSTANTIATE_SWAP"|"NETMSG_LEVEL_LOAD"|"NETMSG_LEVEL_LOADED"|"NETMSG_LEVEL_START"|"NETMSG_LEVEL_SWAP_COMPLETE"|"NETMSG_LEVEL_SWAP_READY"|"NETMSG_LEVEL_UP_UPDATE"|"NETMSG_LIGHTING_OVERRIDE"|"NETMSG_LOAD_GAME_WITH_ADDONS"|"NETMSG_LOAD_GAME_WITH_ADDONS_FAIL"|"NETMSG_LOAD_START"|"NETMSG_LOAD_STARTED"|"NETMSG_LOBBY_DATAUPDATE"|"NETMSG_LOBBY_STARTGAME"|"NETMSG_LOBBY_USERUPDATE"|"NETMSG_LOCK_WAYPOINT"|"NETMSG_LONG_REST_DECISION"|"NETMSG_MARKER_UI_CREATE"|"NETMSG_MARKER_UI_UPDATE"|"NETMSG_MIC_DISABLED"|"NETMSG_MODULE_LOAD"|"NETMSG_MODULE_LOADED"|"NETMSG_MULTIPLE_TARGET_OPERATION"|"NETMSG_MUSIC_EVENT"|"NETMSG_MUSIC_STATE"|"NETMSG_MYSTERY_ADVANCED"|"NETMSG_MYSTERY_DISABLED"|"NETMSG_NET_ENTITY_CREATE"|"NETMSG_NET_ENTITY_DESTROY"|"NETMSG_NEW_ITEMS_INSIDE"|"NETMSG_NOTIFICATION"|"NETMSG_OPEN_CRAFT_UI_MESSAGE"|"NETMSG_OPEN_CUSTOM_BOOK_UI_MESSAGE"|"NETMSG_OPEN_MESSAGE_BOX_MESSAGE"|"NETMSG_OPEN_WAYPOINT_UI_MESSAGE"|"NETMSG_OVERHEADTEXT"|"NETMSG_PARTYGROUP"|"NETMSG_PARTYORDER"|"NETMSG_PARTY_BLOCKFOLLOW"|"NETMSG_PARTY_PRESET_LEVELUP"|"NETMSG_PARTY_PRESET_LOAD"|"NETMSG_PARTY_PRESET_SAVE"|"NETMSG_PARTY_PRESET_SPELL"|"NETMSG_PASSIVE_ROLL_SEQUENCE"|"NETMSG_PASSIVE_TOGGLE"|"NETMSG_PAUSE"|"NETMSG_PEER_ACTIVATE"|"NETMSG_PEER_DEACTIVATE"|"NETMSG_PING_REQUEST"|"NETMSG_PLAYER_ACCEPT"|"NETMSG_PLAYER_CHANGENAME"|"NETMSG_PLAYER_CONNECT"|"NETMSG_PLAYER_DISCONNECT"|"NETMSG_PLAYER_JOINED"|"NETMSG_PLAYER_LEFT"|"NETMSG_PLAYER_NAMECHANGED"|"NETMSG_PLAYMOVIE"|"NETMSG_PROJECTILE_CREATE"|"NETMSG_PROJECTILE_UPDATE"|"NETMSG_QUESTS_LOADED"|"NETMSG_QUEST_CATEGORY_UPDATE"|"NETMSG_QUEST_PROGRESS"|"NETMSG_QUEST_STEP"|"NETMSG_QUEST_TRACK"|"NETMSG_READYCHECK"|"NETMSG_REALTIME_MULTIPLAY"|"NETMSG_REGISTER_WAYPOINT"|"NETMSG_REQUESTAUTOSAVE"|"NETMSG_REQUESTED_ROLL"|"NETMSG_REQUEST_RELEASE_CONCENTRATION"|"NETMSG_RESPEC_UPDATE"|"NETMSG_ROLL_STREAM_ROLL_MODE_TYPE"|"NETMSG_SAVEGAME"|"NETMSG_SAVEGAMEHANDSHAKE"|"NETMSG_SAVEGAMEHANDSHAKE_SCREENSHOT"|"NETMSG_SAVEGAME_LOAD_FAIL"|"NETMSG_SCREEN_FADE_FINISHED"|"NETMSG_SCRIPT_EXTENDER"|"NETMSG_SELECT_DICE"|"NETMSG_SERVER_COMMAND"|"NETMSG_SERVER_NOTIFICATION"|"NETMSG_SESSION_LOAD"|"NETMSG_SESSION_LOADED"|"NETMSG_SESSION_UNLOADED"|"NETMSG_SHORT_REST"|"NETMSG_SHOW_ENTER_REGION_UI_MESSAGE"|"NETMSG_SHOW_ERROR"|"NETMSG_SHROUD_UPDATE"|"NETMSG_SKIPMOVIE_RESULT"|"NETMSG_SNEAKING_CONES_VISIBLE_TOGGLE"|"NETMSG_SPELL_CANCEL"|"NETMSG_SPELL_CAST_CANCEL"|"NETMSG_SPELL_CAST_CAST"|"NETMSG_SPELL_CAST_CONFIRM"|"NETMSG_SPELL_CAST_START_PREVIEW"|"NETMSG_SPELL_CAST_UPDATE_PREVIEW"|"NETMSG_SPELL_LEARN"|"NETMSG_SPELL_PREPARE"|"NETMSG_SPELL_REMOVE_LEARNED"|"NETMSG_STORY_ELEMENT_UI"|"NETMSG_STORY_FLAGS_LOAD"|"NETMSG_SURFACE_CREATE"|"NETMSG_SURFACE_META"|"NETMSG_TADPOLE_POWERS_REMOVE"|"NETMSG_TADPOLE_POWER_ADD"|"NETMSG_TELEPORT_ACK"|"NETMSG_TELEPORT_WAYPOINT"|"NETMSG_TIMELINE_ACTOR_HANDSHAKE"|"NETMSG_TIMELINE_HANDSHAKE"|"NETMSG_TIMELINE_NODECOMPLETED"|"NETMSG_TIMELINE_PLAYER_WATCHING"|"NETMSG_TIMELINE_REQUEST_FASTFORWARD"|"NETMSG_TIMELINE_WORLD_CINEMATIC_COMPLETED"|"NETMSG_TRADE_ACTION"|"NETMSG_TRIGGER_CREATE"|"NETMSG_TRIGGER_DESTROY"|"NETMSG_TRIGGER_UPDATE"|"NETMSG_TROPHY_UPDATE"|"NETMSG_TURNBASED_BEGIN_CANCEL_ENDTURN_REQUEST"|"NETMSG_TURNBASED_CANCEL_ENDTURN_REQUEST"|"NETMSG_TURNBASED_ENDTURN_REQUEST"|"NETMSG_TURNBASED_END_CANCEL_ENDTURN_REQUEST"|"NETMSG_TURNBASED_FINISHTEAM"|"NETMSG_TURNBASED_FLEECOMBATRESULT"|"NETMSG_TURNBASED_FLEE_REQUEST"|"NETMSG_TURNBASED_SETTEAM"|"NETMSG_TURNBASED_SKIP_START_DELAY"|"NETMSG_TURNBASED_STARTTURN_CONFIRMATION"|"NETMSG_TUTORIALS_RESET"|"NETMSG_TUTORIALS_SHOWING"|"NETMSG_TUTORIAL_CLOSED"|"NETMSG_TUTORIAL_UI_EVENT"|"NETMSG_TUTORIAL_USER_ACTION"|"NETMSG_UI_COMBINE_OPEN"|"NETMSG_UI_FORCETURNBASED_ENTERED"|"NETMSG_UI_FORCETURNBASED_LEFT"|"NETMSG_UI_FORCETURNBASED_TURN_ENDED"|"NETMSG_UI_FORCETURNBASED_TURN_STARTED"|"NETMSG_UI_INTERACTION_STOPPED"|"NETMSG_UI_QUESTSELECTED"|"NETMSG_UNLOCK_ITEM"|"NETMSG_UNLOCK_WAYPOINT"|"NETMSG_UNPAUSE"|"NETMSG_USE_ITEM_ACTION"|"NETMSG_VOICEDATA"
--- @alias NsDragAction string|"DragAction_Cancel"|"DragAction_Continue"|"DragAction_Drop"
--- @alias NsHoldingState string|"HoldingState_Canceled"|"HoldingState_Completed"|"HoldingState_Started"
--- @alias NsKey string|"Key_A"|"Key_AbntC1"|"Key_AbntC2"|"Key_Add"|"Key_Apps"|"Key_B"|"Key_Back"|"Key_BrowserBack"|"Key_BrowserFavorites"|"Key_BrowserForward"|"Key_BrowserHome"|"Key_BrowserRefresh"|"Key_BrowserSearch"|"Key_BrowserStop"|"Key_C"|"Key_Cancel"|"Key_CapsLock"|"Key_Clear"|"Key_D"|"Key_D0"|"Key_D1"|"Key_D2"|"Key_D3"|"Key_D4"|"Key_D5"|"Key_D6"|"Key_D7"|"Key_D8"|"Key_D9"|"Key_DbeAlphanumeric"|"Key_DbeCodeInput"|"Key_DbeDbcsChar"|"Key_DbeDetermineString"|"Key_DbeEnterDialogConversionMode"|"Key_DbeEnterImeConfigureMode"|"Key_DbeEnterWordRegisterMode"|"Key_DbeFlushString"|"Key_DbeHiragana"|"Key_DbeKatakana"|"Key_DbeNoCodeInput"|"Key_DbeNoRoman"|"Key_DbeRoman"|"Key_DbeSbcsChar"|"Key_DeadCharProcessed"|"Key_Decimal"|"Key_Delete"|"Key_Divide"|"Key_Down"|"Key_E"|"Key_End"|"Key_Enter"|"Key_Escape"|"Key_Execute"|"Key_F"|"Key_F1"|"Key_F10"|"Key_F11"|"Key_F12"|"Key_F13"|"Key_F14"|"Key_F15"|"Key_F16"|"Key_F17"|"Key_F18"|"Key_F19"|"Key_F2"|"Key_F20"|"Key_F21"|"Key_F22"|"Key_F23"|"Key_F24"|"Key_F3"|"Key_F4"|"Key_F5"|"Key_F6"|"Key_F7"|"Key_F8"|"Key_F9"|"Key_FinalMode"|"Key_G"|"Key_GamepadAccept"|"Key_GamepadCancel"|"Key_GamepadContext1"|"Key_GamepadContext2"|"Key_GamepadContext3"|"Key_GamepadContext4"|"Key_GamepadDown"|"Key_GamepadLeft"|"Key_GamepadMenu"|"Key_GamepadPageDown"|"Key_GamepadPageLeft"|"Key_GamepadPageRight"|"Key_GamepadPageUp"|"Key_GamepadRight"|"Key_GamepadUp"|"Key_GamepadView"|"Key_H"|"Key_HangulMode"|"Key_Help"|"Key_Home"|"Key_I"|"Key_ImeAccept"|"Key_ImeConvert"|"Key_ImeModeChange"|"Key_ImeNonConvert"|"Key_ImeProcessed"|"Key_Insert"|"Key_J"|"Key_JunjaMode"|"Key_K"|"Key_KanjiMode"|"Key_L"|"Key_LWin"|"Key_LaunchApplication1"|"Key_LaunchApplication2"|"Key_LaunchMail"|"Key_Left"|"Key_LeftAlt"|"Key_LeftCtrl"|"Key_LeftShift"|"Key_LineFeed"|"Key_M"|"Key_MediaNextTrack"|"Key_MediaPlayPause"|"Key_MediaPreviousTrack"|"Key_MediaStop"|"Key_Multiply"|"Key_N"|"Key_None"|"Key_NumLock"|"Key_NumPad0"|"Key_NumPad1"|"Key_NumPad2"|"Key_NumPad3"|"Key_NumPad4"|"Key_NumPad5"|"Key_NumPad6"|"Key_NumPad7"|"Key_NumPad8"|"Key_NumPad9"|"Key_O"|"Key_Oem8"|"Key_OemBackslash"|"Key_OemClear"|"Key_OemCloseBrackets"|"Key_OemComma"|"Key_OemMinus"|"Key_OemOpenBrackets"|"Key_OemPeriod"|"Key_OemPipe"|"Key_OemPlus"|"Key_OemQuestion"|"Key_OemQuotes"|"Key_OemSemicolon"|"Key_OemTilde"|"Key_P"|"Key_PageDown"|"Key_PageLeft"|"Key_PageRight"|"Key_PageUp"|"Key_Pause"|"Key_Print"|"Key_PrintScreen"|"Key_Q"|"Key_R"|"Key_RWin"|"Key_Right"|"Key_RightAlt"|"Key_RightCtrl"|"Key_RightShift"|"Key_S"|"Key_Scroll"|"Key_Select"|"Key_SelectMedia"|"Key_Separator"|"Key_Sleep"|"Key_Space"|"Key_Subtract"|"Key_System"|"Key_T"|"Key_Tab"|"Key_U"|"Key_Up"|"Key_V"|"Key_VolumeDown"|"Key_VolumeMute"|"Key_VolumeUp"|"Key_W"|"Key_X"|"Key_Y"|"Key_Z"
--- @alias NsKeyStates string|"KeyStates_Down"|"KeyStates_None"|"KeyStates_Toggled"
--- @alias NsManipulationModes string|"ManipulationModes_Rotate"|"ManipulationModes_Scale"|"ManipulationModes_TranslateX"|"ManipulationModes_TranslateY"
--- @alias NsModifierKeys string|"ModifierKeys_Alt"|"ModifierKeys_Control"|"ModifierKeys_Shift"|"ModifierKeys_Windows"
--- @alias NsMouseAction string|"MouseAction_LeftClick"|"MouseAction_LeftDoubleClick"|"MouseAction_MiddleClick"|"MouseAction_MiddleDoubleClick"|"MouseAction_None"|"MouseAction_RightClick"|"MouseAction_RightDoubleClick"|"MouseAction_WheelClick"
--- @alias NsMouseButton string|"MouseButton_Left"|"MouseButton_Middle"|"MouseButton_Right"|"MouseButton_XButton1"|"MouseButton_XButton2"
--- @alias NsMouseButtonState string|"MouseButtonState_Pressed"|"MouseButtonState_Released"
--- @alias NsOrientation string|"Orientation_Horizontal"|"Orientation_Vertical"
--- @alias ObscurityState string|"Clear"|"HeavilyObscured"|"LightlyObscured"
--- @alias OsiFunctionType string|"Call"|"DB"|"Event"|"Proc"|"Query"
--- @alias OsirisTaskType string|"Appear"|"AutomatedDialog"|"Combine"|"Disappear"|"Drop"|"FleeFromEntity"|"FleeFromGrid"|"FleeFromRelation"|"FollowNPC"|"FollowOwnerOrLeader"|"LookAt"|"MoveInRange"|"MoveItem"|"MoveToAndTalk"|"MoveToLocation"|"MoveToObject"|"PickupItem"|"PlayAnimation"|"RateLimitedAutomatedDialog"|"Resurrect"|"Steer"|"TeleportToLocation"|"Timer"|"UseItem"|"UseSpell"|"Wander"
--- @alias PassiveSourceType string|"Debug"|"Equipment"|"Feat"|"Progression"|"Progression2"|"Script"|"Stats"|"Status"|"Tadpole"
--- @alias PathRootType string|"Bin"|"Bin2"|"Data"|"Debug"|"EngineMod"|"GameMod"|"LocalAppData"|"Localization"|"Mods"|"Projects"|"Public"|"Public2"|"Root"|"Scripts"|"UserProfile"|"WorkingDir"
--- @alias PhysicsExtraFlags string|"BlockAoEDamage"|"ShootThroughType0"|"ShootThroughType1"|"ShootThroughType2"|"ShootThroughType3"|"ShootThroughType4"|"ShootThroughType5"
--- @alias PhysicsGroupFlags string|"AttackableWhenClickThrough"|"CanClickThrough"|"CanClimbOn"|"CanSeeThrough"|"CanSeeThrough2"|"CanShineThrough"|"CanShootThrough"|"CanWalkOn"|"Character"|"ClimbableTileConstruction"|"Dead"|"DisableBoundCollision"|"Dragging"|"Fadeable"|"Falling"|"Group01"|"Group02"|"Group04"|"Group08"|"Group100"|"Group10000000"|"Group40"|"Group80"|"IsDecorative"|"Item"|"PlatformOwner"|"PointerBlocker"|"Scenery"|"Terrain"|"TimelinePreview"|"VisibleItem"|"Wadable"
--- @alias PhysicsHitType string|"DoubleSided"|"Normal"
--- @alias PhysicsType string|"Dynamic"|"Static"
--- @alias ProficiencyBonusBoostType string|"Ability"|"AllAbilities"|"AllSavingThrows"|"AllSkills"|"AttackRoll"|"AttackTarget"|"SavingThrow"|"Skill"|"SourceDialogue"|"WeaponActionDC"
--- @alias ProficiencyGroupFlags string|"Battleaxes"|"Clubs"|"Daggers"|"Darts"|"Flails"|"Glaives"|"Greataxes"|"Greatclubs"|"Greatswords"|"Halberds"|"HandCrossbows"|"Handaxes"|"HeavyArmor"|"HeavyCrossbows"|"Javelins"|"LightArmor"|"LightCrossbows"|"LightHammers"|"Longbows"|"Longswords"|"Maces"|"MartialWeapons"|"Mauls"|"MediumArmor"|"Morningstars"|"MusicalInstrument"|"Pikes"|"Quarterstaffs"|"Rapiers"|"Scimitars"|"Shields"|"Shortbows"|"Shortswords"|"Sickles"|"SimpleWeapons"|"Slings"|"Spears"|"Tridents"|"Warhammers"|"Warpicks"
--- @alias ProgressionSelectorType string|"Class"|"Feat"|"None"|"Passive"|"Race"|"SubClass"|"SubRace"
--- @alias ProgressionType string|"Class"|"Passive"|"Race"|"SubClass"
--- @alias ProjectileTypeIds string|"Magical"|"Physical"
--- @alias RequirementType string|"Barter"|"Charm"|"Combat"|"Constitution"|"Dexterity"|"Immobile"|"Intelligence"|"Intimidate"|"Level"|"Loremaster"|"Necromancy"|"None"|"PainReflection"|"Persuasion"|"Pickpocket"|"Ranged"|"Reason"|"Reflexes"|"Sneaking"|"Sourcery"|"Strength"|"Summoning"|"Tag"|"Telekinesis"|"TurnBased"|"Vitality"|"Wisdom"
--- @alias ResistanceBoostFlags string|"BelowDamageThreshold"|"ImmuneToMagical"|"ImmuneToNonMagical"|"ResistantToMagical"|"ResistantToNonMagical"|"VulnerableToMagical"|"VulnerableToNonMagical"
--- @alias ResourceBankType string|"Animation"|"AnimationBlueprint"|"AnimationSet"|"Atmosphere"|"BlendSpace"|"CharacterVisual"|"ClothCollider"|"ColorList"|"Dialog"|"DiffusionProfile"|"Effect"|"FCurve"|"IKRig"|"LightCookie"|"Lighting"|"Material"|"MaterialPreset"|"MaterialSet"|"MeshProxy"|"Physics"|"Script"|"Sentinel"|"Skeleton"|"SkeletonMirrorTable"|"SkinPreset"|"Sound"|"TerrainBrush"|"Texture"|"TileSet"|"Timeline"|"TimelineScene"|"VirtualTexture"|"Visual"|"VisualSet"|"VoiceBark"
--- @alias ResourceReplenishType string|"Combat"|"Default"|"ExhaustedRest"|"FullRest"|"Never"|"Rest"|"ShortRest"
--- @alias RestCancelReason string|"AnotherPlayerCanceled"|"ForcedCancelOnLoad"|"None"|"RestTypeSelectionCanceled"|"Script"|"Validation"|"ValidationChangedDuringSelection"
--- @alias RestErrorFlags string|"Combat"|"Dialog"|"DownedOrDead"|"DropInCCActive"|"FTB"|"GlobalDisabled"|"Location"|"NotEnoughResources"|"Script"
--- @alias RestFlags string|"CanLongRest"|"CanShortRest"|"Unknown0x04"
--- @alias RollCritical string|"Fail"|"None"|"Success"
--- @alias SDLControllerAxis string|"LeftX"|"LeftY"|"RightX"|"RightY"|"TriggerLeft"|"TriggerRight"
--- @alias SDLControllerButton string|"A"|"B"|"Back"|"DPadDown"|"DPadLeft"|"DPadRight"|"DPadUp"|"Guide"|"LeftShoulder"|"LeftStick"|"Misc1"|"Paddle1"|"Paddle2"|"Paddle3"|"Paddle4"|"RightShoulder"|"RightStick"|"Start"|"Touchpad"|"X"|"Y"
--- @alias SDLKeyEvent string|"KeyDown"|"KeyUp"
--- @alias SDLKeyModifier string|"Caps"|"LAlt"|"LCtrl"|"LGui"|"LShift"|"Mode"|"Num"|"RAlt"|"RCtrl"|"RGui"|"RShift"|"Scroll"
--- @alias SDLScanCode string|"A"|"AC_BACK"|"AC_BOOKMARKS"|"AC_FORWARD"|"AC_HOME"|"AC_REFRESH"|"AC_SEARCH"|"AC_STOP"|"AGAIN"|"ALTERASE"|"APOSTROPHE"|"APP1"|"APP2"|"APPLICATION"|"AUDIOFASTFORWARD"|"AUDIOMUTE"|"AUDIONEXT"|"AUDIOPLAY"|"AUDIOPREV"|"AUDIOREWIND"|"AUDIOSTOP"|"B"|"BACKSLASH"|"BACKSPACE"|"BRIGHTNESSDOWN"|"BRIGHTNESSUP"|"C"|"CALCULATOR"|"CALL"|"CANCEL"|"CAPSLOCK"|"CLEAR"|"CLEARAGAIN"|"COMMA"|"COMPUTER"|"COPY"|"CRSEL"|"CURRENCYSUBUNIT"|"CURRENCYUNIT"|"CUT"|"D"|"DECIMALSEPARATOR"|"DEL"|"DISPLAYSWITCH"|"DOWN"|"E"|"EJECT"|"END"|"ENDCALL"|"EQUALS"|"ESCAPE"|"EXECUTE"|"EXSEL"|"F"|"F1"|"F10"|"F11"|"F12"|"F13"|"F14"|"F15"|"F16"|"F17"|"F18"|"F19"|"F2"|"F20"|"F21"|"F22"|"F23"|"F24"|"F3"|"F4"|"F5"|"F6"|"F7"|"F8"|"F9"|"FIND"|"G"|"GRAVE"|"H"|"HELP"|"HOME"|"I"|"INSERT"|"INTERNATIONAL1"|"INTERNATIONAL2"|"INTERNATIONAL3"|"INTERNATIONAL4"|"INTERNATIONAL5"|"INTERNATIONAL6"|"INTERNATIONAL7"|"INTERNATIONAL8"|"INTERNATIONAL9"|"J"|"K"|"KBDILLUMDOWN"|"KBDILLUMTOGGLE"|"KBDILLUMUP"|"KEY_OUT"|"KP_0"|"KP_00"|"KP_000"|"KP_1"|"KP_2"|"KP_3"|"KP_4"|"KP_5"|"KP_6"|"KP_7"|"KP_8"|"KP_9"|"KP_A"|"KP_AMPERSAND"|"KP_AT"|"KP_B"|"KP_BACKSPACE"|"KP_BINARY"|"KP_C"|"KP_CLEAR"|"KP_CLEARENTRY"|"KP_COLON"|"KP_COMMA"|"KP_D"|"KP_DBLAMPERSAND"|"KP_DBLVERTICALBAR"|"KP_DECIMAL"|"KP_DIVIDE"|"KP_E"|"KP_ENTER"|"KP_EQUALS"|"KP_EQUALSAS400"|"KP_EXCLAM"|"KP_F"|"KP_GREATER"|"KP_HASH"|"KP_HEXADECIMAL"|"KP_LEFTBRACE"|"KP_LEFTPAREN"|"KP_LESS"|"KP_MEMADD"|"KP_MEMCLEAR"|"KP_MEMDIVIDE"|"KP_MEMMULTIPLY"|"KP_MEMRECALL"|"KP_MEMSTORE"|"KP_MEMSUBTRACT"|"KP_MINUS"|"KP_MULTIPLY"|"KP_OCTAL"|"KP_PERCENT"|"KP_PERIOD"|"KP_PLUS"|"KP_PLUSMINUS"|"KP_POWER"|"KP_RIGHTBRACE"|"KP_RIGHTPAREN"|"KP_SPACE"|"KP_TAB"|"KP_VERTICALBAR"|"KP_XOR"|"L"|"LALT"|"LANG1"|"LANG2"|"LANG3"|"LANG4"|"LANG5"|"LANG6"|"LANG7"|"LANG8"|"LANG9"|"LCTRL"|"LEFT"|"LEFTBRACKET"|"LGUI"|"LSHIFT"|"M"|"MAIL"|"MEDIASELECT"|"MENU"|"MINUS"|"MODE"|"MUTE"|"N"|"NONUSBACKSLASH"|"NONUSHASH"|"NUMLOCKCLEAR"|"NUM_0"|"NUM_1"|"NUM_2"|"NUM_3"|"NUM_4"|"NUM_5"|"NUM_6"|"NUM_7"|"NUM_8"|"NUM_9"|"O"|"OPER"|"P"|"PAGEDOWN"|"PAGEUP"|"PASTE"|"PAUSE"|"PERIOD"|"POWER"|"PRINTSCREEN"|"PRIOR"|"Q"|"R"|"RALT"|"RCTRL"|"RETURN"|"RETURN2"|"RGUI"|"RIGHT"|"RIGHTBRACKET"|"RSHIFT"|"S"|"SCROLLLOCK"|"SELECT"|"SEMICOLON"|"SEPARATOR"|"SLASH"|"SLEEP"|"SOFTLEFT"|"SOFTRIGHT"|"SPACE"|"STOP"|"SYSREQ"|"T"|"TAB"|"THOUSANDSSEPARATOR"|"U"|"UNDO"|"UNKNOWN"|"UP"|"V"|"VOLUMEDOWN"|"VOLUMEUP"|"W"|"WWW"|"X"|"Y"|"Z"
--- @alias SceneObjectType string|"CullableInstance"|"DeferredDecal"|"Effect"|"FogVolume"|"InstanceBatchRenderableObject"|"InstancingObject"|"Light"|"LightProbe"|"RenderableObject"|"RoomTriggerVisual"|"Visual"
--- @alias SceneryFlags string|"AllowCameraMovement"|"CanClimbOn"|"CanSeeThrough"|"CanSeeThrough2"|"CanShootThrough"|"CanWalkOn"|"CollideWithCamera"|"IsBlocker"|"Wadable"|"WalkThrough"
--- @alias ServerActionType string|"Animation"|"CombineItem"|"Incapacitated"|"KnockedDown"|"MoveItem"|"TeleportFalling"|"UseItem"
--- @alias ServerBehaviourType string|"Appear"|"FollowCharacter"|"Force"|"Move"|"MoveAndUseItem"|"MoveItem"|"MoveToAndTalk"|"Patrol"|"PickupItem"|"Wander"
--- @alias ServerCharacterFlags string|"Activated"|"CanShootThrough"|"CannotAttachToGroups"|"CannotDie"|"CannotMove"|"CannotRun"|"CharCreationInProgress"|"CharacterControl"|"CharacterCreationFinished"|"CoverAmount"|"CustomLookEnabled"|"Deactivated"|"Dead"|"DeferredRemoveEscapist"|"Detached"|"DisableCulling"|"DisableWaypointUsage"|"DoNotFaceFlag"|"DontCacheTemplate"|"FightMode"|"FindValidPositionOnActivate"|"Floating"|"GMReroll"|"HasOwner"|"HostControl"|"IgnoresTriggers"|"InDialog"|"InParty"|"Invisible"|"Invulnerable"|"IsCompanion_M"|"IsHuge"|"IsPlayer"|"IsTrading"|"LevelTransitionPending"|"Loaded"|"Multiplayer"|"NeedsMakePlayerUpdate"|"OffStage"|"PartyFollower"|"RegisteredForAutomatedDialog"|"RequestStartTurn"|"ReservedForDialog"|"SpotSneakers"|"SteeringEnabled"|"StoryControlled"|"StoryNPC"|"Summon"|"Temporary"|"Totem"|"Unknown10000000000"|"Unknown40000000000"|"Unknown8000"|"Unknown80000000"|"Unknown80000000000000"|"WalkThrough"
--- @alias ServerCharacterFlags2 string|"CanGossip"|"Global"|"IsLeveledByStory"|"IsPet"|"IsResurrected"|"TeleportingPartyHack"|"Trader"|"TreasureGeneratedForTrader"
--- @alias ServerCharacterFlags3 string|"CrimeWarningsEnabled"|"NeedsPlacementSnapping"|"SpreadActivation"|"TreasureEquip"
--- @alias ServerConcentrationRequestType string|"AddTarget"|"RemoveTarget"
--- @alias ServerGameState string|"BuildStory"|"Disconnect"|"Exit"|"Idle"|"Init"|"LoadLevel"|"LoadModule"|"LoadSession"|"Paused"|"ReloadStory"|"Running"|"Save"|"Sync"|"Uninitialized"|"Unknown"|"UnloadLevel"|"UnloadModule"|"UnloadSession"
--- @alias ServerItemFlags string|"Activated"|"CanBeMoved"|"CanBePickedUp"|"CanClimbOn"|"CanOnlyBeUsedByOwner"|"CanShootThrough"|"CanUse"|"ClientSync1"|"Destroy"|"Destroyed"|"DisableInventoryView80"|"DisableSync"|"DontAddToHotbar"|"Floating"|"ForceAffectedByAura"|"ForceClientSync"|"ForceSync"|"FreezeGravity"|"Frozen"|"GMFolding"|"HideHP"|"InAutomatedDialog"|"InPartyInventory"|"InUse"|"InteractionDisabled"|"Invisible"|"Invisible2"|"Invulnerable2"|"IsContainer"|"IsDoor"|"IsLadder"|"IsMoving"|"IsSecretDoor"|"IsSurfaceBlocker"|"IsSurfaceCloudBlocker"|"Known"|"LevelTransitionPending"|"LoadedTemplate"|"LuckyFind"|"NoCover"|"OffStage"|"PinnedContainer"|"PositionChanged"|"ReservedForDialog"|"SourceContainer"|"Sticky"|"StoryItem"|"Summon"|"TeleportOnUse"|"Totem"|"TransformChanged"|"WakePhysics"|"WalkOn"|"WalkThrough"
--- @alias ServerItemFlags2 string|"Global"|"TreasureGenerated"|"UnEquipLocked"|"UnsoldGenerated"|"UseRemotely"
--- @alias ServerMovementType string|"Climbing"|"Falling"|"Idle"|"MoveTo"|"MoveToSync"|"Repose"
--- @alias ServerSpellCastSystemEventType string|"AddDelayedCastHit"|"AddMovementInfo"|"AddProjectileCache"|"AddServerMovement"|"AddSharedMovement"|"AddZoneRange"|"AdvanceInterruptAnimationIndex"|"AdvanceReaction"|"AdvanceRoll"|"CacheFlagChange"|"Cast"|"CastHit"|"CastHitRemove"|"CastOptionsChange"|"CastRemove"|"CastTextKey"|"CastTextKeyIndexIncrease"|"CastTextKeyRemove"|"ChangeCastEndPosition"|"ChangeCastHitDelayTimer"|"ChangeCastPosition"|"ChangeMovementTransactionID"|"ChangeProjectileDelayTimer"|"ChangeStoryActionId"|"CounteredSpellCast"|"CreateWall"|"CreateZone"|"DestroyEventRemove"|"DestroySpellCastEntity"|"DoTeleport"|"Finish"|"FinishRemove"|"FireProjectile"|"InitializeIntermediateTargets"|"InitializeRolls"|"InterruptApplyResult"|"InterruptEnd"|"InterruptEventChange"|"InterruptStart"|"IsCastingRemove"|"JumpStart"|"LogicExecutionEnd"|"LogicExecutionStart"|"LogicExecutionStartRemove"|"LogicExecutionUpdate"|"MoveDuringCastEnd"|"MoveDuringCastInitialize"|"MoveDuringCastStart"|"MoveDuringCastUpdate"|"MovementEnd"|"PathfindCleanup"|"PathfindRequest"|"PathfindResults"|"PhaseChange"|"PlayAnimation"|"PopDelayedCastHit"|"PopDelayedProjectile"|"PrecalculatedConditionals"|"PrecalculatedInterruptors"|"PrecalculationEnd"|"PreparationEnd"|"PreparationStart"|"PreparationStarting"|"PreviewEnd"|"PurgePausedAnimationEvents"|"PushPausedAnimationEvents"|"SpellRollAbort"|"TargetHit"|"TargetHitInterruptRemove"|"TargetHitRemove"|"TargetReactionRemove"|"TargetsChange"|"TargetsChanged"|"ThrowPickupPositionChanged"|"UpdateConditionals"|"UpdateInterruptors"|"UseCostsChange"|"WeaponSetRequest"|"ZoneRangeUpdate"
--- @alias ServerStatusFlags string|"ForceFailStatus"|"ForceStatus"|"IgnoreStacking"|"Loaded"|"RequestClientSync"|"RequestDelete"|"RequestDeleteAtTurnEnd"|"Started"
--- @alias ServerStatusFlags2 string|"DontTickWhileOnSurface"|"Influence"|"InitiateCombat"|"IsChanneled"|"IsFromItem"|"IsLifeTimeSet"|"IsOnSourceSurface"|"KeepAlive"
--- @alias ServerStatusFlags3 string|"DisableImmunityOverhead"|"IsUnique"|"NotifiedPlanManager"|"StatusFlags3_0x08"|"StatusFlags3_0x10"|"StatusFlags3_0x20"|"StatusFlags3_0x40"|"StatusFlags3_0x80"
--- @alias ServerStatusFlags4 string|"BringIntoCombat"|"CauseIsOrigin"|"CausedByEquipment"|"IsHostileAct"|"IsInvulnerable"|"IsInvulnerableVisible"|"StatusFlags4_0x04"|"StatusFlags4_0x80"
--- @alias ServerStatusFlags5 string|"HasTriedEntering"
--- @alias ServerSteeringType string|"LookAt"|"Repose"|"Steer"|"SteerTo"|"SteerTunnel"
--- @alias SetStatusDurationType string|"Add"|"ForceSet"|"Multiply"|"SetMinimum"|"Unknown"
--- @alias ShapeshiftChangeType string|"Add"|"Multiply"|"Override"
--- @alias ShapeshiftSpellSource string|"ShapeshiftRule"|"Template"|"Unknown"
--- @alias SkillId string|"Acrobatics"|"AnimalHandling"|"Arcana"|"Athletics"|"Deception"|"History"|"Insight"|"Intimidation"|"Invalid"|"Investigation"|"Medicine"|"Nature"|"Perception"|"Performance"|"Persuasion"|"Religion"|"Sentinel"|"SleightOfHand"|"Stealth"|"Survival"
--- @alias SoundEventType string|"Ambient"|"CineHUD"|"HUD"|"Music"|"Sound"
--- @alias SourceAdvantageType string|"None"|"SourceAdvantageOnAttack"|"SourceAllyAdvantageOnAttack"
--- @alias SpellAnimationCastEvent string|"Cancel"|"Cast"|"CastContinue"|"Finished"|"Prepare"|"TargetingInfo"
--- @alias SpellAttackType string|"DirectHit"|"MeleeOffHandWeaponAttack"|"MeleeSpellAttack"|"MeleeUnarmedAttack"|"MeleeWeaponAttack"|"None"|"RangedOffHandWeaponAttack"|"RangedSpellAttack"|"RangedUnarmedAttack"|"RangedWeaponAttack"
--- @alias SpellAttackTypeOverride string|"Projectile_MainHandAttack"|"Projectile_OffhandAttack"|"Target_MainHandAttack"|"Target_OffhandAttack"|"Target_UnarmedAttack"
--- @alias SpellCastFailReason string|"BlockedRequiredMove"|"Canceled"|"CantCast"|"CantFindCastPosition"|"CantMove"|"CantSpendUseCosts"|"CantUseSource"|"Countered"|"DeadCaster"|"DeadTarget"|"EqualTargets"|"InvalidCastPosition"|"InvalidSpell"|"InvalidTarget"|"MissingAnimations"|"MissingCaster"|"MissingTarget"|"MissingTargetPosition"|"MovementFailed"|"None"|"ServerTimeout"|"SpellRollFailure"|"TooFewTargets"|"TooManyTargets"|"UnsheathFailure"
--- @alias SpellCastOptions string|"AvoidAoO"|"AvoidDangerousAuras"|"CheckProjectileTargets"|"DestroySource"|"Forced"|"FromClient"|"IgnoreCastChecks"|"IgnoreHasSpell"|"IgnoreSpellRolls"|"IgnoreTargetChecks"|"Immediate"|"IsHoverPreview"|"IsInterrupt"|"IsPreview"|"IsReaction"|"IsRoll"|"NoMovement"|"NoUnsheath"|"ShowPrepareAnimation"|"Silent"|"Unknown100000"
--- @alias SpellCastPhase string|"Finished"|"LogicExecutionEnd"|"LogicExecutionFinished"|"LogicExecutionInterrupted"|"LogicExecutionStart"|"LogicExecutionUpdate"|"MovementFinished"|"MovementStart"|"MovementSteering"|"MovementUpdate"|"None"|"PrecalculationFinished"|"PrecalculationStart"|"PrepareEnd"|"PrepareFinished"|"PrepareStart"|"PrepareUpdate"|"ValidationFindCastEndPosition"|"ValidationFindCastPosition"|"ValidationFinished"|"ValidationPathfind"|"ValidationStart"
--- @alias SpellCooldownType string|"Default"|"OncePerCombat"|"OncePerShortRestPerItem"|"OncePerTurn"|"OncePerTurnNoRealtime"|"UntilPerRestPerItem"|"UntilRest"|"UntilShortRest"
--- @alias SpellFlags string|"AbortOnSecondarySpellRollFail"|"AbortOnSpellRollFail"|"AddFallDamageOnLand"|"AddWeaponRange"|"AllowMoveAndCast"|"CallAlliesSpell"|"CanAreaDamageEvade"|"CanDualWield"|"CannotRotate"|"CannotTargetCharacter"|"CannotTargetItems"|"CannotTargetTerrain"|"CombatLogSetSingleLineRoll"|"ConcentrationIgnoresResting"|"DisableBlood"|"DisplayInItemTooltip"|"DontAbortPerforming"|"HasHighGroundRangeExtension"|"HasSomaticComponent"|"HasVerbalComponent"|"HideInItemTooltip"|"IgnoreAoO"|"IgnorePreviouslyPickedEntities"|"IgnoreSilence"|"IgnoreVisionBlock"|"ImmediateCast"|"InventorySelection"|"Invisible"|"IsAttack"|"IsConcentration"|"IsDefaultWeaponAction"|"IsEnemySpell"|"IsHarmful"|"IsJump"|"IsLinkedSpellContainer"|"IsMelee"|"IsSpell"|"IsSwarmAttack"|"IsTrap"|"NoAOEDamageOnLand"|"NoCameraMove"|"NoCooldownOnMiss"|"NoSurprise"|"PickupEntityAndMove"|"RangeIgnoreBlindness"|"RangeIgnoreSourceBounds"|"RangeIgnoreTargetBounds"|"RangeIgnoreVerticalThreshold"|"Stealth"|"TargetClosestEqualGroundSurface"|"Temporary"|"TrajectoryRules"|"UNUSED_D"|"UNUSED_E"|"UnavailableInDialogs"|"Wildshape"
--- @alias SpellLearningStrategy string|"AddChildren"|"MostPowerful"|"Singular"
--- @alias SpellMetaConditionType string|"AoE"|"Cycle"|"Forking"|"Highlight"|"None"|"OriginRoll"|"OriginTarget"|"Requirements"|"Roll"|"Target"|"ThrowableRoll"|"ThrowableTarget"
--- @alias SpellModificationValueType string|"AdditiveBase"|"AdditiveFinal"|"Multiplicative"|"Override"
--- @alias SpellPrepareType string|"AlwaysPrepared"|"RequiresPreparation"|"Unknown"
--- @alias SpellSchoolId string|"Abjuration"|"Conjuration"|"Divination"|"Enchantment"|"Evocation"|"Illusion"|"Necromancy"|"None"|"Transmutation"
--- @alias SpellSourceType string|"AiTest"|"Anubis"|"Behavior"|"Boost"|"Boost2"|"CreateExplosion"|"Debug"|"EquippedItem"|"Functor"|"GameActionCreateSurface"|"Learned"|"Osiris"|"Progression"|"ProgressionClass"|"ProgressionRace"|"ProgressionSubClass"|"RandomCast"|"Sentinel"|"Shapeshift"|"SpellSet"|"SpellSet2"|"TadpoleTree"|"UnarmedAttack"|"WeaponAttack"
--- @alias SpellType string|"MultiStrike"|"None"|"Projectile"|"ProjectileStrike"|"Rush"|"Shout"|"Storm"|"Target"|"Teleportation"|"Throw"|"Wall"|"Zone"
--- @alias StatAttributeFlags string|"Arrow"|"Backstab"|"BackstabImmunity"|"EMPTY"|"EnableObscurityEvents"|"Floating"|"FloatingWhileMoving"|"Grounded"|"IgnoreClouds"|"InvisibilityImmunity"|"InvulnerableAndInteractive"|"LootableWhenEquipped"|"LoseDurabilityOnCharacterHit"|"ObscurityWithoutSneaking"|"PickpocketableWhenEquipped"|"SlippingImmunity"|"ThrownImmunity"|"Torch"|"Unbreakable"|"Unrepairable"|"Unstorable"
--- @alias StatCharacterFlags string|"Blind"|"DrinkedPotion"|"EquipmentValidated"|"InParty"|"Invisible"|"IsPlayer"|"IsSneaking"
--- @alias StatsContextType string|"Cause"|"Owner"|"Target"|"Unspecified"
--- @alias StatsDeathType string|"Acid"|"Chasm"|"CinematicDeath"|"Cold"|"Disintegrate"|"DoT"|"Electrocution"|"Explode"|"Falling"|"Incinerate"|"KnockedDown"|"Lifetime"|"Necrotic"|"None"|"Physical"|"Psychic"|"Radiant"
--- @alias StatsDirtyFlags string|"AC"|"Abilities"|"AttackRollBonusOnWeapon"|"CurrentHP"|"DamageOnWeapon"|"Encumbrance"|"EquipmentSlotAvailability"|"FootstepsType"|"Initiative"|"Invulnerability"|"ItemUsability"|"MaxHP"|"MaxPreparedSpells"|"PhysicalForceRangeBonus"|"ProficiencyBonus"|"Resistances"|"SavingThrow"|"Skills"|"SpellDifficultyCheck"|"StatusImmunities"|"TempHP"|"UnarmedAndNaturalArmorType"|"UseBoostRequests"|"WeaponActionDC"|"WeaponAttackRollAbility"|"WeaponProperties"|"Weight"
--- @alias StatsExpressionParamType string|"Bool"|"Int"|"ResourceRollDefinition"|"RollDefinition"|"StatsContextType"|"StatsExpressionType"|"StatsExpressionVariableDataModifier"|"StatsExpressionVariableDataType"|"Type1"
--- @alias StatsExpressionParamType2 string|"Ability"|"STDString"|"Skill"|"StatsExpressionVariableData"|"StatusGroup"
--- @alias StatsExpressionType string|"Add"|"Divide"|"ForEach"|"Max"|"Multiply"|"Negate"|"Placeholder"|"ResourceRoll"|"Roll"|"Subtract"|"Variable"
--- @alias StatsExpressionVariableData string|"CurrentHP"|"DamageDone"|"Level"|"MaxHP"|"ProficiencyBonus"|"Sentinel"|"SpellCastingAbility"|"SpellDC"|"SpellPowerLevel"|"TadpolePowersCount"|"UnarmedMeleeAbility"|"WeaponActionDC"
--- @alias StatsExpressionVariableDataModifier string|"Advantage"|"DialogueCheck"|"Disadvantage"|"Flat"|"Modifier"|"SavingThrow"
--- @alias StatsExpressionVariableDataType string|"AbilityOverride"|"CharacterData"|"ClassLevel"|"LevelMapValue"|"SavingThrow"|"StatusDuration"|"StatusStacks"
--- @alias StatsFunctorFlags string|"Owner"|"Self"|"Swap"
--- @alias StatsFunctorId string|"AdjustRoll"|"ApplyEquipmentStatus"|"ApplyStatus"|"BreakConcentration"|"CameraWait"|"Counterspell"|"CreateConeSurface"|"CreateExplosion"|"CreateSurface"|"CreateWall"|"CreateZone"|"CustomDescription"|"DealDamage"|"DisarmAndStealWeapon"|"DisarmWeapon"|"DoTeleport"|"Douse"|"Drop"|"ExecuteWeaponFunctors"|"Extender"|"FireProjectile"|"Force"|"GainTemporaryHitPoints"|"Kill"|"MaximizeRoll"|"ModifySpellCameraFocus"|"Pickup"|"RegainHitPoints"|"RegainTemporaryHitPoints"|"RemoveAuraByChildStatus"|"RemoveStatus"|"RemoveStatusByLevel"|"RemoveUniqueStatus"|"ResetCombatTurn"|"ResetCooldowns"|"RestoreResource"|"Resurrect"|"Sabotage"|"SetAdvantage"|"SetDamageResistance"|"SetDisadvantage"|"SetReroll"|"SetRoll"|"SetStatusDuration"|"ShortRest"|"Spawn"|"SpawnExtraProjectiles"|"SpawnInInventory"|"Stabilize"|"Summon"|"SummonInInventory"|"SurfaceChange"|"SurfaceClearLayer"|"SwapPlaces"|"SwitchDeathType"|"TeleportSource"|"TriggerRandomCast"|"TutorialEvent"|"Unlock"|"Unsummon"|"UseActionResource"|"UseAttack"|"UseSpell"
--- @alias StatsItemSlot string|"Amulet"|"Boots"|"Breast"|"Cloak"|"Gloves"|"Helmet"|"Horns"|"MainHand"|"Max"|"MeleeMainHand"|"MeleeOffHand"|"MusicalInstrument"|"OffHand"|"Overhead"|"RangedMainHand"|"RangedOffHand"|"Ring"|"Ring2"|"Sentinel"|"Underwear"|"VanityBody"|"VanityBoots"|"Wings"
--- @alias StatsObserverType string|"None"|"Observer"|"Source"|"Target"
--- @alias StatsPropertyContext string|"ABILITY_CHECK"|"ACTION_RESOURCES_CHANGED"|"AI_IGNORE"|"AI_ONLY"|"AOE"|"ATTACK"|"ATTACKED"|"ATTACKED_IN_MELEE_RANGE"|"ATTACKING_IN_MELEE_RANGE"|"CAST"|"CAST_RESOLVED"|"COMBAT_ENDED"|"COMBAT_STARTED"|"CONSUMED"|"CREATE"|"DAMAGE"|"DAMAGED"|"DAMAGED_PREVENTED"|"DAMAGE_PREVENTED"|"ENTER_ATTACK_RANGE"|"EQUIP"|"GROUND"|"HEAL"|"HEALED"|"INTERRUPT_USED"|"INVENTORY_CHANGED"|"LEAVE_ATTACK_RANGE"|"LOCKPICKING_SUCCEEDED"|"LONG_REST"|"MOVED_DISTANCE"|"OBSCURITY_CHANGED"|"PROFICIENCY_CHANGED"|"PROJECTILE"|"PUSH"|"PUSHED"|"ROUND"|"SHORT_REST"|"STATUS_APPLIED"|"STATUS_APPLY"|"STATUS_REMOVE"|"STATUS_REMOVED"|"SURFACE_ENTER"|"TARGET"|"TURN"
--- @alias StatsRestoreResourceAmountType string|"Lua"|"None"|"Percentage"
--- @alias StatsRollAdjustmentType string|"All"|"Distribute"
--- @alias StatsRollType string|"Attack"|"Damage"|"DeathSavingThrow"|"MeleeOffHandWeaponAttack"|"MeleeSpellAttack"|"MeleeSpellDamage"|"MeleeUnarmedAttack"|"MeleeUnarmedDamage"|"MeleeWeaponAttack"|"MeleeWeaponDamage"|"None"|"RangedOffHandWeaponAttack"|"RangedSpellAttack"|"RangedSpellDamage"|"RangedUnarmedAttack"|"RangedUnarmedDamage"|"RangedWeaponAttack"|"RangedWeaponDamage"|"RawAbility"|"SavingThrow"|"Sentinel"|"SkillCheck"
--- @alias StatsSize string|"Gargantuan"|"Huge"|"Large"|"Medium"|"None"|"Small"|"Tiny"
--- @alias StatsStatusGroup string|"SG_Approaching"|"SG_Blinded"|"SG_CanBePickedUp"|"SG_Charmed"|"SG_Charmed_Subtle"|"SG_Condition"|"SG_Confused"|"SG_Cursed"|"SG_DetectThoughts"|"SG_DifficultTerrain"|"SG_Disease"|"SG_Disguise"|"SG_Dominated"|"SG_Doppelganger"|"SG_DropForNonMutingDialog"|"SG_Drunk"|"SG_Exhausted"|"SG_Fleeing"|"SG_Frightened"|"SG_Helpable_Condition"|"SG_HexbladeCurse"|"SG_Ignore_AOO"|"SG_Incapacitated"|"SG_Invisible"|"SG_Light"|"SG_Mad"|"SG_Paralyzed"|"SG_Petrified"|"SG_Poisoned"|"SG_Poisoned_Story_NonRemovable"|"SG_Poisoned_Story_Removable"|"SG_Polymorph"|"SG_Polymorph_BeastShape"|"SG_Polymorph_BeastShape_NPC"|"SG_Possessed"|"SG_Prone"|"SG_Rage"|"SG_RemoveOnRespec"|"SG_Restrained"|"SG_ScriptedPeaceBehaviour"|"SG_Sleeping"|"SG_Stunned"|"SG_Surface"|"SG_Taunted"|"SG_Unconscious"|"SG_WeaponCoating"
--- @alias StatsSummonLifetimeType string|"Permanent"|"UntilLongRest"
--- @alias StatsTargetTypeFlags string|"Construct"|"Guaranteed"|"Living"|"Undead"
--- @alias StatsUnsheathState string|"Instrument"|"Melee"|"Ranged"|"Sheathed"
--- @alias StatsZoneShape string|"Cone"|"Square"
--- @alias StatusHealType string|"All"|"AllArmor"|"MagicArmor"|"None"|"PhysicalArmor"|"Source"|"Vitality"
--- @alias StatusMaterialApplyFlags string|"ApplyOnArmor"|"ApplyOnBody"|"ApplyOnHorns"|"ApplyOnOverhead"|"ApplyOnWeapon"|"ApplyOnWings"
--- @alias StatusType string|"BOOST"|"CLIMBING"|"DEACTIVATED"|"DOWNED"|"DYING"|"EFFECT"|"FEAR"|"HEAL"|"INCAPACITATED"|"INSURFACE"|"INVISIBLE"|"KNOCKED_DOWN"|"MATERIAL"|"POLYMORPHED"|"REACTION"|"ROTATE"|"SMELLY"|"SNEAKING"|"STORY_FROZEN"|"TELEPORT_FALLING"|"UNLOCK"
--- @alias SurfaceActionType string|"ChangeSurfaceOnPathAction"|"CreatePuddleAction"|"CreateSurfaceAction"|"ExtinguishFireAction"|"PolygonSurfaceAction"|"RectangleSurfaceAction"|"TransformSurfaceAction"|"ZoneAction"
--- @alias SurfaceChange string|"Clear"|"Condense"|"Daylight"|"Deelectrify"|"DestroyWater"|"Douse"|"Electrify"|"Freeze"|"Ignite"|"Melt"|"None"|"TurnHellfire"|"UnturnHellfire"|"Vaporize"
--- @alias SurfaceLayer string|"Cloud"|"Ground"|"None"
--- @alias SurfaceLayer8 string|"Cloud"|"Ground"|"None"
--- @alias SurfaceType string|"Acid"|"Alcohol"|"AlienOil"|"Ash"|"BlackPowder"|"BlackPowderDetonationCloud"|"BlackTentacles"|"BladeBarrier"|"Blood"|"BloodCloud"|"BloodElectrified"|"BloodExploding"|"BloodFrozen"|"BloodSilver"|"CausticBrine"|"Chasm"|"Cloudkill6Cloud"|"CloudkillCloud"|"CrawlerMucusCloud"|"DarknessCloud"|"Deepwater"|"DrowPoisonCloud"|"ExplosionCloud"|"Fire"|"FogCloud"|"GithPheromoneGasCloud"|"Grease"|"Hellfire"|"HolyFire"|"IceCloud"|"InvisibleGithAcid"|"InvisibleWeb"|"Lava"|"MaliceCloud"|"Mud"|"None"|"Oil"|"Overgrowth"|"Poison"|"PoisonCloud"|"PotionAntitoxinCloud"|"PotionHealingCloud"|"PotionHealingGreaterCloud"|"PotionHealingSuperiorCloud"|"PotionHealingSupremeCloud"|"PotionInvisibilityCloud"|"PotionResistanceAcidCloud"|"PotionResistanceColdCloud"|"PotionResistanceFireCloud"|"PotionResistanceForceCloud"|"PotionResistanceLightningCloud"|"PotionResistancePoisonCloud"|"PotionSpeedCloud"|"PotionVitalityCloud"|"PurpleWormPoison"|"Sentinel"|"SerpentVenom"|"Sewer"|"ShadowCursedVines"|"ShockwaveCloud"|"SpikeGrowth"|"SporeBlackCloud"|"SporeGreenCloud"|"SporePinkCloud"|"SporeWhiteCloud"|"StinkingCloud"|"TrialFire"|"Vines"|"VoidCloud"|"Water"|"WaterCloud"|"WaterCloudElectrified"|"WaterElectrified"|"WaterFrozen"|"Web"|"WyvernPoison"
--- @alias TextKeyType string|"Attach"|"Attack"|"Effect"|"FloatData"|"Foley"|"FootMove"|"FootStep"|"Genome"|"Ragdoll"|"React"|"Sound"|"VisualCullFlag"|"Vocal"|"WeaponEffect"
--- @alias TextureType string|"T1D"|"T1DArray"|"T2D"|"T2DArray"|"T2DCube"|"T2DCubeArray"|"T2DRenderTarget"|"T3D"|"T3DArray"
--- @alias TravelErrorFlags string|"CannotFastTravel"|"Condition"|"DangerZone"|"Dialog"|"FTB"|"InDanger"
--- @alias TravelFlags string|"BlockCampTravel"|"BlockFastTravel"
--- @alias VisualAttachmentFlags string|"DestroyWithParent"|"DoNotUpdate"|"EffectComponent_M"|"ExcludeFromBounds"|"Hair"|"HasSkeleton"|"InheritAnimations"|"KeepRot"|"KeepScale"|"ParticleSystem"|"SupportsVertexColorMask"|"TextKeyEffect_M"|"UseLocalTransform"|"VisualSet"
--- @alias VisualCullFlags string|"CullDecal"|"CullNonProxy"|"CullRenderView1"|"CullRenderView2"|"CullScenery"|"CullShadow"|"CullShadow2"|"CullShadow3"|"CullShadow4"|"CullShadowProxy"
--- @alias VisualFlags string|"AllowReceiveDecalWhenAnimated"|"AllowTPose"|"CastShadow"|"DisableCulling"|"DisableLOD"|"HasBlueprint"|"HasSkeleton"|"IsEffect"|"IsScenery"|"IsShadowProxy"|"IsWall"|"PlayingAttachedEffects"|"PreparedDestroyed"|"PreparedDestroyed2"|"ReceiveColorFromParent"|"ReceiveDecal"|"Reflecting"|"SeeThrough"|"ShowMesh"|"Unknown200"
--- @alias VisualObjectType string|"Beard"|"Ears"|"Head"|"Horns"|"Type01"|"Type02"|"Type04"
--- @alias WeaponFlags string|"AddToHotbar"|"Ammunition"|"Dippable"|"Finesse"|"Heavy"|"Lance"|"Light"|"Loading"|"Magical"|"Melee"|"NeedDualWieldingBoost"|"Net"|"NoDualWield"|"NotSheathable"|"Range"|"Reach"|"Thrown"|"Torch"|"Twohanded"|"Unstowable"|"Versatile"
--- @alias WeaponSetType string|"Melee"|"Ranged"|"Unknown"
--- @alias WeaponType string|"Arrow"|"Axe"|"Bow"|"Club"|"Crossbow"|"Knife"|"None"|"Rifle"|"Sentinel"|"Spear"|"Staff"|"Sword"|"Wand"


--- @class AABound
--- @field Max vec3
--- @field Min vec3


--- @class ACOverrideFormulaBoostComponent:BaseComponent
--- @field AC int32
--- @field AddAbilityModifiers AbilityId[]
--- @field field_4 boolean


--- @class AIPortalObjectData
--- @field ActionResourceCost number
--- @field EndPos vec3
--- @field EndPosAdjusted vec3
--- @field IsAttached_M boolean
--- @field IsProhibitedToPlayers boolean
--- @field Level FixedString
--- @field PortalID EntityHandle
--- @field PortalType int32
--- @field Source EntityHandle
--- @field StartPos vec3
--- @field StartPosAdjusted vec3
--- @field Target EntityHandle
--- @field TargetLevel_M FixedString
--- @field field_62 boolean


--- @class AbilityBoostComponent:BaseComponent
--- @field Ability AbilityId
--- @field DontAdjustHealth boolean
--- @field Value int32
--- @field ValueCap int32
--- @field field_8 int32
--- @field field_C boolean


--- @class AbilityFailedSavingThrowBoostComponent:BaseComponent
--- @field Ability AbilityId


--- @class AbilityOverrideMinimumBoostComponent:BaseComponent
--- @field Ability AbilityId
--- @field Amount int32
--- @field field_8 boolean


--- @class ActionOriginator
--- @field ActionGuid Guid
--- @field CanApplyConcentration boolean
--- @field InterruptId FixedString
--- @field PassiveId FixedString
--- @field StatusId FixedString


--- @class ActionResourceBlockBoostComponent:BaseComponent
--- @field IntParam int32
--- @field Level int32
--- @field ResourceUUID Guid


--- @class ActionResourceConsumeMultiplierBoostComponent:BaseComponent
--- @field Level int32
--- @field Multiplier number
--- @field ResourceUUID Guid


--- @class ActionResourceDiceValue
--- @field Amount number
--- @field MaxAmount number


--- @class ActionResourceEntry
--- @field Amount number
--- @field DiceValues ActionResourceDiceValue[]?
--- @field Level int32
--- @field MaxAmount number
--- @field ReplenishType ResourceReplenishType
--- @field ResourceId int32
--- @field ResourceUUID Guid
--- @field SubAmounts ActionResourceDiceValue[]?
--- @field field_28 ResourceReplenishType
--- @field field_A8 uint8


--- @class ActionResourceEventsOneFrameComponent:BaseComponent
--- @field Events ActionResourceSetValueRequest[]


--- @class ActionResourceMultiplierBoostComponent:BaseComponent
--- @field DiceSize DiceSizeId
--- @field IntParam int32
--- @field IntParam2 int32
--- @field ResourceUUID Guid


--- @class ActionResourcePreventReductionBoostComponent:BaseComponent
--- @field ActionResource Guid
--- @field Amount int32


--- @class ActionResourceQuery
--- @field BoostAmount int32
--- @field ResourceId Guid


--- @class ActionResourceRefillRequest
--- @field Entity EntityHandle
--- @field Flags uint64


--- @class ActionResourceReplenishTypeOverrideBoostComponent:BaseComponent
--- @field ActionResource Guid
--- @field ReplenishType ResourceReplenishType


--- @class ActionResourceSetValueRequest
--- @field Amount number
--- @field OldAmount number
--- @field ResourceId Guid


--- @class ActionResourceSpendEventOneFrameComponent:BaseComponent
--- @field Results ActionResourceSpendResult[]


--- @class ActionResourceSpendResult
--- @field Resource Guid
--- @field Succeeded boolean


--- @class ActionResourceValueBoostComponent:BaseComponent
--- @field Amount number
--- @field Amount2 int32
--- @field DiceSize DiceSizeId
--- @field ResourceUUID Guid


--- @class ActionResourcesComponent:BaseComponent
--- @field Resources table<Guid, ActionResourceEntry[]>


--- @class ActionUseConditionsComponent:BaseComponent
--- @field Conditions int32[]


--- @class ActiveCharacterLightBoostComponent:BaseComponent
--- @field LightUUID FixedString


--- @class ActiveCharacterLightComponent:BaseComponent
--- @field Light FixedString


--- @class ActiveComponent:BaseComponent


--- @class ActiveMaterial
--- @field AlphaChannel uint8
--- @field BlendStateID uint8
--- @field DebugMaterial uint8
--- @field DirtyFlags uint8
--- @field Flags AppliedMaterialFlags
--- @field Hash uint64
--- @field Initialized boolean
--- @field Material Material
--- @field MaterialInstance Material
--- @field MaterialName FixedString
--- @field OverlayOffset number
--- @field RasterizerStateID uint8
--- @field RenderableObject RenderableObject
--- @field VirtualTextureParams ActiveMaterialVirtualTextureParam[]
--- @field SetScalar fun(self:ActiveMaterial, a1:FixedString, a2:number):boolean
--- @field SetVector2 fun(self:ActiveMaterial, a1:FixedString, a2:vec2):boolean
--- @field SetVector3 fun(self:ActiveMaterial, a1:FixedString, a2:vec3):boolean
--- @field SetVector4 fun(self:ActiveMaterial, a1:FixedString, a2:vec4):boolean


--- @class ActiveMaterialTexture2DParam
--- @field Index int16
--- @field IsStub boolean
--- @field Name FixedString


--- @class ActiveMaterialVirtualTextureParam
--- @field ID FixedString
--- @field Index int16
--- @field ParameterName FixedString


--- @class ActiveSkeletonSlotsComponent:BaseComponent
--- @field Slots FixedString[]


--- @class AddTagBoostComponent:BaseComponent
--- @field Tag Guid


--- @class AdvanceSpellsBoostComponent:BaseComponent
--- @field field_0 FixedString
--- @field field_4 int32


--- @class AdvantageBoostComponent:BaseComponent
--- @field Ability AbilityId
--- @field AdvantageType AdvantageContext
--- @field Skill SkillId
--- @field Tags Guid[]
--- @field Type AdvantageBoostType


--- @class AiActionConditions
--- @field AIFlags uint16
--- @field ExcludeInNPCLoadout Guid[]
--- @field FallbackStartRound int32
--- @field MinimumImpact int32
--- @field OnlyCastOnSelf boolean
--- @field OnlyInNPCLoadout Guid[]
--- @field ScoreModifier number
--- @field SourceConditions AiTargetConditions
--- @field StartRound int32
--- @field TargetConditions AiTargetConditions


--- @class AiArchetypeOverrideBoostComponent:BaseComponent
--- @field Archetype FixedString
--- @field Priority int32


--- @class AiFlags
--- @field Flags uint64


--- @class AiFullTile
--- @field AoOCost int32
--- @field CollidingCount int32
--- @field Portal EntityHandle
--- @field Position AiTilePos
--- @field Sibling int32
--- @field field_14 int32
--- @field field_20 int32
--- @field field_C int32


--- @class AiFullTileRef
--- @field Cost int32
--- @field FullTileIndex int32


--- @class AiGrid
--- @field ChangeLayersTask AiGridChangeLayersTask
--- @field DistanceToClosestPortal int32
--- @field FinalFullTileIndex int32
--- @field FinalReverseFullTileIndex int32
--- @field FullTiles AiFullTile[]
--- @field IgnoreMask AiIgnoreMask
--- @field LayerMap table<Guid, uint16>
--- @field Layers AiGridLayer[]
--- @field MaxIterations int32
--- @field MetaData AiMetaData[]
--- @field NextPathHandle int32
--- @field NumIterations int32
--- @field Objects BoundComponent[]
--- @field PathDistanceToClosestPortal int32
--- @field PathMap table<int32, AiPath>
--- @field PathPool AiPath[]
--- @field Paths AiPath[]
--- @field Portals AiGridPortal[]
--- @field PreciseClock int64
--- @field ReverseClimb boolean
--- @field ReverseTilesToCheck AiFullTileRef[]
--- @field SubgridBoundsDirty boolean
--- @field Subgrids table<uint32, AiSubgrid>
--- @field SubgridsAtPatch table<uint64, uint32[]>
--- @field SurfaceMetaData AiSurfaceMetaData[]
--- @field TileHeightAndStateDirty table<AiTilePos, vec2>
--- @field TilesToCheck AiFullTileRef[]
--- @field UUID FixedString
--- @field Version int32
--- @field VisualRadius int32
--- @field VisualSourcePos vec3
--- @field field_1D8 int64
--- @field field_1E0 int64
--- @field field_298 int32
--- @field field_29C int32
--- @field field_2A0 int32
--- @field field_2A4 int32
--- @field field_390 AiSomeFloodObj
--- @field field_398 AiSomeFloodObj
--- @field field_448 int64
--- @field field_450 int64
--- @field field_8 int64


--- @class AiGridChangeLayersTask
--- @field Deltas table<AiTilePos, AiGridChangeLayersTaskDelta>
--- @field Layers Guid[]
--- @field Layers2 Guid[]
--- @field field_0 int64


--- @class AiGridChangeLayersTaskDelta
--- @field AiFlags int64
--- @field AiFlags2 int64
--- @field Height uint16
--- @field LayerId uint16
--- @field field_18 int64


--- @class AiGridInternalTileData
--- @field Height int32
--- @field Tiles AiInternalTile
--- @field TilesEnd AiInternalTile
--- @field Width int32


--- @class AiGridLayer
--- @field Activated boolean
--- @field Deltas table<AiTilePos, AiGridLayerDelta>
--- @field MetaDataIndex uint16
--- @field field_40 Guid


--- @class AiGridLayerDelta
--- @field Flags AiFlags
--- @field Height number


--- @class AiGridLuaTile
--- @field CloudSurface SurfaceType
--- @field Entities EntityHandle[]
--- @field ExtraFlags uint32
--- @field Flags AiBaseFlags
--- @field GroundSurface SurfaceType
--- @field Material uint8
--- @field MaxHeight number
--- @field MetaDataIndex uint16
--- @field MinHeight number
--- @field SubgridId uint32
--- @field SurfaceMetaDataIndex uint16
--- @field TileX int16
--- @field TileY int16
--- @field UnmappedFlags uint32


--- @class AiGridPortal
--- @field AiGrid AiGrid
--- @field IsAttached boolean
--- @field IsVisited boolean
--- @field PortalData AIPortalObjectData
--- @field SourceAIPosition AiTilePos
--- @field TargetAIPosition AiTilePos


--- @class AiGridTile
--- @field Flags AiFlags
--- @field MaxHeight uint16
--- @field MetaDataIndex uint16
--- @field MinHeight uint16
--- @field SurfaceMetaDataIndex uint16


--- @class AiGridTileData
--- @field Height int32
--- @field Tiles AiGridTile
--- @field TilesEnd AiGridTile
--- @field Width int32


--- @class AiHintAreaTriggerData:ITriggerData
--- @field IsAnchor boolean


--- @class AiIgnoreMask
--- @field AggregateFlags AiFlags
--- @field AiGrid AiGrid
--- @field Flags AiFlags[]
--- @field NextId uint64
--- @field Restore boolean


--- @class AiInternalTile
--- @field Flags uint8
--- @field NodeId int32
--- @field field_1 uint8
--- @field field_2 int16


--- @class AiMetaData
--- @field EndPortals AiGridPortal[]
--- @field Entities EntityHandle[]
--- @field LayerId uint16
--- @field Portals AiGridPortal[]
--- @field Position AiTilePos


--- @class AiPath
--- @field AddBoundsToMargin boolean
--- @field AddSourceBoundsToMargin boolean
--- @field AoOPositions AiPathAoOPosition[]
--- @field AoOTiles table<AiTilePos, uint64>
--- @field AvailableKeys Set_FixedString
--- @field CanUseCombatPortals boolean
--- @field CanUseLadders boolean
--- @field CanUsePortals boolean
--- @field CheckLockedDoors boolean
--- @field Checkpoints AiPathCheckpoint[]
--- @field Climbing boolean
--- @field CloseEnoughCeiling number
--- @field CloseEnoughFloor number
--- @field CloseEnoughMax number
--- @field CloseEnoughMin number
--- @field CloseEnoughPreference number
--- @field ClosestCollidingCount int32
--- @field ClosestCost number
--- @field ClosestFullTileIndex int32
--- @field CollisionMask AiFlags
--- @field CollisionMaskMove AiFlags
--- @field CollisionMaskStand AiFlags
--- @field CoverFlags uint16
--- @field DangerousAuras DangerousAuras
--- @field DestinationReached boolean
--- @field ErrorCause uint32
--- @field FallDamageDistanceEstimate number
--- @field FallDamageMinimumDistance number
--- @field FallDeadPathfindingCost int32
--- @field FallMaxDamagePathfindingCost number
--- @field FallMinDamagePathfindingCost number
--- @field GoalFound boolean
--- @field HasLimitNode boolean
--- @field Height number
--- @field IgnoreEntities EntityHandle[]
--- @field InUse boolean
--- @field InteractionRange number
--- @field IsBidirectionalSearch boolean
--- @field IsPlayer boolean
--- @field LimitNodeIndex uint64
--- @field MovedEntities AiPathEntityPosition[]
--- @field MovingBound number
--- @field MovingBound2 number
--- @field MovingBoundTiles int32
--- @field MovingBoundTiles2 int32
--- @field Nodes AiPathNode[]
--- @field PathType int32
--- @field PickUpTarget boolean
--- @field Portal EntityHandle
--- @field PreciseItemInteraction boolean
--- @field ProjectileTarget vec3
--- @field SearchComplete boolean
--- @field SearchHorizon int16
--- @field SearchHorizon2 int16
--- @field SearchStarted boolean
--- @field Source EntityHandle
--- @field SourceAdjusted vec3
--- @field SourceOriginal vec3
--- @field StandingBound number
--- @field StandingBoundTiles int32
--- @field StepHeight number
--- @field SurfacePathInfluences SurfacePathInfluence[]
--- @field Target EntityHandle
--- @field TargetAdjusted vec3
--- @field TargetPosition vec3
--- @field TurningNodeAngle number
--- @field TurningNodeOffset number
--- @field UseSmoothing boolean
--- @field UseSplines boolean
--- @field UseStandAtDestination boolean
--- @field UseTurning boolean
--- @field WorldClimbType uint8
--- @field WorldClimbingHeight number
--- @field WorldClimbingRadius number
--- @field WorldDropType uint8
--- @field field_136 uint8
--- @field field_148 EntityHandle
--- @field field_150 boolean
--- @field field_154 int32
--- @field field_178 vec3
--- @field field_294 int32
--- @field field_298 int32
--- @field field_29C number
--- @field field_2A3 boolean
--- @field field_90 number
--- @field UsePlayerWeighting fun(self:AiPath, a1:boolean?, a2:boolean?)


--- @class AiPathAoOPosition
--- @field Distance number
--- @field Position vec3
--- @field field_10 number
--- @field field_14 number


--- @class AiPathCheckpoint
--- @field Flags uint8
--- @field Portal EntityHandle
--- @field Position vec3
--- @field field_20 EntityHandle


--- @class AiPathEntityPosition
--- @field field_0 int32
--- @field field_4 int32
--- @field field_8 int32
--- @field field_C int32


--- @class AiPathNode
--- @field Distance number
--- @field DistanceModifier number
--- @field Flags uint8
--- @field Portal EntityHandle
--- @field Position vec3


--- @class AiPathProbeGroup
--- @field Values1 int32[]
--- @field Values2 number[]
--- @field Values3 vec3[]
--- @field field_30 int32


--- @class AiPlayerWeightFuncData
--- @field CharacterBounds int32
--- @field DamagingSurfacesThreshold int32
--- @field IsAvoidingDynamics boolean
--- @field IsAvoidingObstacles boolean
--- @field IsAvoidingTraps boolean
--- @field SurfacePathInfluences SurfacePathInfluence[]
--- @field UseSurfaceInfluences boolean


--- @class AiSomeFloodObj
--- @field field_0 int64
--- @field field_28 int64
--- @field field_30 int64
--- @field field_38 int64


--- @class AiSubgrid:DataGrid
--- @field AttachedObjectRefCounts table<EntityHandle, uint64>
--- @field GUID FixedString
--- @field InternalTileGrid AiGridInternalTileData
--- @field IsActive boolean
--- @field LoadedExternally boolean
--- @field PatchesDirty boolean
--- @field RelativePlatformPosition vec3
--- @field SomeGuid FixedString
--- @field TileGrid AiGridTileData
--- @field TraversedTiles0 uint16[]
--- @field TraversedTiles1 uint16[]
--- @field WorldPos AiWorldPos
--- @field field_B0 ivec2[]


--- @class AiSurfaceMetaData
--- @field PerLayerIndices int16[]
--- @field Position AiTilePos


--- @class AiTargetConditions
--- @field MaximumHealthPercentage int32
--- @field MinimumHealthPercentage int32
--- @field Tags Guid[]


--- @class AiTilePos
--- @field SubgridId int32
--- @field X int16
--- @field Y int16


--- @class AiWorldPos
--- @field Xglobal int32
--- @field Xlocal number
--- @field Y number
--- @field Zglobal int32
--- @field Zlocal number


--- @class AmbushingComponent:BaseComponent


--- @class AnimationBlueprintComponent:BaseProxyComponent
--- @field Blueprint GnGenomeBlueprint
--- @field Flags uint8
--- @field Instance GnGenomeBlueprintInstance
--- @field InstanceId int32
--- @field Resource ResourceAnimationBlueprintResource
--- @field field_40 uint64


--- @class AnimationReceivedGameplayEvent
--- @field Args GnGenomeParametrizedEventArgs
--- @field Entity EntityHandle
--- @field Event FixedString


--- @class AnimationReceivedTextKeyEvent
--- @field Entity EntityHandle
--- @field Event TextKeyEvent


--- @class AnimationSetComponent:BaseComponent
--- @field Entries AnimationSetEntry[]
--- @field FallbackSubSet FixedString


--- @class AnimationSetEntry
--- @field Dynamic boolean
--- @field Resource FixedString
--- @field Slot FixedString
--- @field Type FixedString


--- @class AnimationTag
--- @field Tag Guid
--- @field field_10 uint8


--- @class AnimationUpdateComponent:BaseComponent


--- @class AnimationWaterfallComponent:BaseComponent
--- @field Overrides AnimationWaterfallOverride[]
--- @field Waterfall AnimationWaterfallElement[]


--- @class AnimationWaterfallElement
--- @field Resource FixedString
--- @field Slot FixedString
--- @field Type FixedString


--- @class AnimationWaterfallOverride
--- @field AnimationTag Guid
--- @field OverrideType uint8
--- @field Overrides AnimationWaterfallElement[]


--- @class AnyUserdataRef


--- @class AoERangeFilterTargetData
--- @field HeightRange number
--- @field Position vec3
--- @field Radius number


--- @class AppearanceOverrideComponent:BaseComponent
--- @field Visual CharacterCreationAppearance


--- @class AppliedMaterial:ActiveMaterial
--- @field DynamicParameter vec4
--- @field MeshVertexColor vec4


--- @class ApprovalRatingsComponent:BaseComponent
--- @field Ratings table<EntityHandle, int32>
--- @field field_70 Set_Guid


--- @class AreaTrigger:Trigger


--- @class ArmorAbilityModifierCapOverrideBoostComponent:BaseComponent
--- @field ArmorType ArmorType
--- @field Value int32


--- @class ArmorClassBoostComponent:BaseComponent
--- @field AC int32


--- @class ArmorComponent:BaseComponent
--- @field AbilityModifierCap int32
--- @field ArmorClass int32
--- @field ArmorClassAbility uint8
--- @field ArmorType int32
--- @field EquipmentType EEquipmentType


--- @class ArmorSetStateComponent:BaseComponent
--- @field State ArmorSetState


--- @class AtmosphereTrigger:AreaTrigger
--- @field AtmosphereResourceIDs FixedString[]
--- @field CurrentAtmosphereResourceID FixedString
--- @field field_84 number


--- @class AttackDesc
--- @field DamageList DamagePair[]
--- @field DamagePercentage uint8
--- @field InitialHPPercentage uint8
--- @field TotalDamageDone int32
--- @field TotalHealDone int32
--- @field field_9 uint8


--- @class AttackSpellOverrideBoostComponent:BaseComponent
--- @field AttackType SpellAttackTypeOverride
--- @field SpellId FixedString


--- @class AttitudeIdentifier
--- @field field_0 EntityHandle
--- @field field_10 Guid
--- @field field_20 uint8
--- @field field_8 uint8


--- @class AttitudesToPlayersComponent:BaseComponent
--- @field Attitudes table<AttitudeIdentifier, int32>


--- @class AttributeBoostComponent:BaseComponent
--- @field AttributeFlags AttributeFlags


--- @class AttributeFlagsComponent:BaseComponent
--- @field AttributeFlags uint32


--- @class AvatarComponent:BaseComponent


--- @class BackgroundComponent:BaseComponent
--- @field Background Guid


--- @class BackgroundPassivesComponent:BaseComponent
--- @field field_18 StatsPassivePrototype[]


--- @class BackgroundTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class BaseComponent


--- @class BaseHpComponent:BaseComponent
--- @field Vitality int32
--- @field VitalityBoost int32


--- @class BaseProxyComponent


--- @class BaseStatsComponent:BaseComponent
--- @field BaseAbilities int32[]


--- @class BaseStatsSystem:BaseSystem


--- @class BaseSystem


--- @class BlockAbilityModifierFromACComponent:BaseComponent
--- @field Ability AbilityId


--- @class BlockRegainHPBoostComponent:BaseComponent


--- @class BodyTypeComponent:BaseComponent
--- @field BodyType uint8
--- @field BodyType2 uint8


--- @class BookActionData:IActionData
--- @field BookId FixedString


--- @class BoostConditionComponent:BaseComponent
--- @field ConditionFlags int32
--- @field field_1C uint8


--- @class BoostDescription
--- @field Boost FixedString
--- @field Params FixedString
--- @field Params2 FixedString


--- @class BoostEntry
--- @field Boosts EntityHandle[]
--- @field Type BoostType


--- @class BoostInfoComponent:BaseComponent
--- @field Cause BoostSource
--- @field CauseUuid Guid
--- @field Owner EntityHandle
--- @field Params BoostDescription
--- @field field_10 Guid
--- @field field_20 uint8
--- @field field_44 uint8
--- @field field_80 Guid


--- @class BoostParameters
--- @field Owner Guid
--- @field field_10 uint8


--- @class BoostSource
--- @field Cause FixedString
--- @field Entity EntityHandle
--- @field Status ComponentHandle
--- @field Type BoostSourceType


--- @class BoostsContainerComponent:BaseComponent
--- @field Boosts BoostEntry[]


--- @class Bound
--- @field AIBounds table<AIBoundType, BoundData>
--- @field BaseFlags BoundBaseFlags
--- @field CollidingRefCount uint16
--- @field Entity EntityHandle
--- @field Flags BoundFlags
--- @field GridPoints Set_AiTilePos
--- @field MetaDataIndex int16
--- @field OwnerPlatform FixedString
--- @field RotationQuat quat
--- @field Scale number
--- @field Translate vec3
--- @field TranslateOverride vec3
--- @field field_89 uint8


--- @class BoundComponent:BaseProxyComponent
--- @field Bound Bound


--- @class BoundData
--- @field ActualHeight number
--- @field AiType AIShapeType
--- @field BoundType AIBoundType
--- @field Height number
--- @field Max vec3
--- @field Min vec3
--- @field Radius number
--- @field Radius2 number
--- @field Shape AIShapeType
--- @field Size number
--- @field Type number
--- @field field_28 AIBoundType
--- @field field_2A uint8


--- @class CameraBlockerTrigger:AreaTrigger
--- @field field_70 FixedString[]
--- @field field_80 FixedString
--- @field field_84 number


--- @class CameraComponent:BaseComponent
--- @field AcceptsInput boolean
--- @field Active boolean
--- @field Controller RfCameraController
--- @field ExposureSettingIndex int32
--- @field MasterBehaviorType uint32
--- @field PostProcess PostProcessCameraSetting
--- @field UseCameraPPSettings boolean
--- @field UseSplitScreenFov boolean


--- @class CameraLockTriggerData:ITriggerData
--- @field TriggerCameraLockPos boolean
--- @field TriggerGameCameraBehavior uint32


--- @class CampChestTriggerData:ITriggerData
--- @field Index uint8


--- @class CampRegionTriggerData:ITriggerData


--- @class CanBeDisarmedComponent:BaseComponent
--- @field Flags uint16


--- @class CanBeLootedComponent:BaseComponent
--- @field Flags uint16


--- @class CanDeflectProjectilesComponent:BaseComponent
--- @field Flags uint16


--- @class CanDoActionsComponent:BaseComponent
--- @field Flags CanDoActionsFlags


--- @class CanDoRestComponent:BaseComponent
--- @field Flags RestFlags
--- @field LongRestErrorFlags RestErrorFlags
--- @field RestErrorFlags1 RestErrorFlags
--- @field RestErrorFlags2 RestErrorFlags
--- @field ShortRestErrorFlags RestErrorFlags


--- @class CanEnterChasmComponent:BaseComponent
--- @field CanEnter boolean


--- @class CanInteractComponent:BaseComponent
--- @field Flags CanInteractFlags
--- @field Flags2 uint16


--- @class CanModifyHealthComponent:BaseComponent
--- @field Flags uint16


--- @class CanMoveComponent:BaseComponent
--- @field Flags CanMoveFlags
--- @field field_4 uint16
--- @field field_6 uint8


--- @class CanSeeThroughBoostComponent:BaseComponent
--- @field CanSeeThrough boolean


--- @class CanSeeThroughComponent:BaseComponent


--- @class CanSenseComponent:BaseComponent
--- @field Flags uint16


--- @class CanShootThroughBoostComponent:BaseComponent
--- @field CanShootThrough boolean


--- @class CanShootThroughComponent:BaseComponent


--- @class CanSpeakComponent:BaseComponent
--- @field Flags uint16


--- @class CanTradeComponent:BaseComponent


--- @class CanTravelComponent:BaseComponent
--- @field ErrorFlags TravelErrorFlags
--- @field Flags TravelFlags
--- @field field_2 uint16


--- @class CanTriggerRandomCastsComponent:BaseComponent


--- @class CanWalkThroughBoostComponent:BaseComponent
--- @field CanWalkThrough boolean


--- @class CanWalkThroughComponent:BaseComponent


--- @class CannotHarmCauseEntityBoostComponent:BaseComponent
--- @field Type FixedString


--- @class CarryCapacityMultiplierBoostComponent:BaseComponent
--- @field Multiplier number


--- @class CharacterComponent:BaseComponent


--- @class CharacterCreationAppearanceComponent:BaseComponent
--- @field AdditionalChoices number[]
--- @field Elements CharacterCreationAppearanceMaterialSetting[]
--- @field EyeColor Guid
--- @field HairColor Guid
--- @field SecondEyeColor Guid
--- @field SkinColor Guid
--- @field Visuals Guid[]


--- @class CharacterCreationStatsComponent:BaseComponent
--- @field Abilities int32[]
--- @field BodyShape uint8
--- @field BodyType uint8
--- @field Name string
--- @field Race Guid
--- @field SubRace Guid
--- @field field_5C uint8


--- @class CharacterCreationTemplateOverrideComponent:BaseComponent
--- @field Template FixedString


--- @class CharacterSpellData
--- @field Conditions AiActionConditions
--- @field LearningStrategy SpellLearningStrategy
--- @field Spell FixedString
--- @field SpellCastingAbility AbilityId


--- @class CharacterTemplate:EoCGameObjectTemplate
--- @field ActivationGroupId FixedString
--- @field AliveInventoryType uint8
--- @field AnimationSetResourceID FixedString
--- @field AnubisConfigName FixedString
--- @field AnubisNonPersistent boolean
--- @field AvoidTraps boolean
--- @field BloodSurfaceType uint8
--- @field BloodType FixedString
--- @field CanBePickedUp boolean
--- @field CanBePickpocketed boolean
--- @field CanBeTeleported boolean
--- @field CanClimbLadders boolean
--- @field CanConsumeItems boolean
--- @field CanOpenDoors boolean
--- @field CanShootThrough boolean
--- @field CanWalkThroughDoors boolean
--- @field CharacterVisualResourceID FixedString
--- @field CombatComponent CombatComponentTemplate
--- @field CoverAmount uint8
--- @field CriticalHitType FixedString
--- @field DeathEffect Guid
--- @field DeathRaycastHeight number
--- @field DeathRaycastMaxLength number
--- @field DeathRaycastMinLength number
--- @field DeathRaycastVerticalLength number
--- @field DefaultDialog FixedString
--- @field DefaultState uint8
--- @field DisableEquipping boolean
--- @field DisintegrateFX FixedString
--- @field DisintegratedResourceID FixedString
--- @field Equipment FixedString
--- @field EquipmentRace Guid
--- @field EquipmentTypes Guid[]
--- @field ExcludeInDifficulty Guid[]
--- @field ExplodedResourceID FixedString
--- @field ExplosionFX FixedString
--- @field FoleyLongResourceID FixedString
--- @field FoleyMediumResourceID FixedString
--- @field FoleyShortResourceID FixedString
--- @field ForceLifetimeDeath boolean
--- @field GeneratePortrait string
--- @field HasPlayerApprovalRating boolean
--- @field Icon FixedString
--- @field InfluenceTreasureLevel boolean
--- @field InventoryType uint8
--- @field IsDroppedOnDeath boolean
--- @field IsEquipmentLootable boolean
--- @field IsLootable boolean
--- @field IsMovementEnabled boolean
--- @field IsPlayer boolean
--- @field IsSimpleCharacter boolean
--- @field IsSteeringEnabled boolean
--- @field IsTradable IsTradableType
--- @field IsWorldClimbingEnabled boolean
--- @field ItemList InventoryItemData[]
--- @field JumpUpLadders boolean
--- @field LadderAttachOffset number
--- @field LadderBlendspace_Attach_Down FixedString
--- @field LadderBlendspace_Attach_Up FixedString
--- @field LadderBlendspace_Detach_Down FixedString
--- @field LadderBlendspace_Detach_Up FixedString
--- @field LadderLoopSpeed number
--- @field LevelOverride int32
--- @field LightChannel uint8
--- @field MaxDashDistance number
--- @field MovementAcceleration number
--- @field MovementSpeedDash number
--- @field MovementSpeedRun number
--- @field MovementSpeedSprint number
--- @field MovementSpeedStroll number
--- @field MovementSpeedWalk number
--- @field MovementStepUpHeight number
--- @field MovementTiltToRemap FixedString
--- @field OnDeathActions IActionData[]
--- @field OnlyInDifficulty Guid[]
--- @field PickingPhysicsTemplates table<FixedString, FixedString>
--- @field ProbeLookAtOffset number
--- @field ProbeSpineAOffset number
--- @field ProbeSpineBOffset number
--- @field ProbeTiltToOffset number
--- @field Race Guid
--- @field RagdollTemplate FixedString
--- @field ShootThroughType uint8
--- @field SkillList CharacterSpellData[]
--- @field SoftBodyCollisionTemplate FixedString
--- @field SoundAttenuation int16
--- @field SoundInitEvent FixedString
--- @field SoundMovementStartEvent FixedString
--- @field SoundMovementStopEvent FixedString
--- @field SoundObjectIndex int8
--- @field SpeakerGroupList Set_Guid
--- @field SpellSet FixedString
--- @field SpotSneakers boolean
--- @field Stats FixedString
--- @field StatusList FixedString[]
--- @field SteeringSpeedCurveWithoutTransitions FixedString
--- @field SteeringSpeedFallback number
--- @field SteeringSpeed_CastingCurve FixedString
--- @field SteeringSpeed_MovingCurve FixedString
--- @field Title TranslatedString
--- @field TradeTreasures FixedString[]
--- @field Treasures FixedString[]
--- @field TrophyID FixedString
--- @field TurningNodeAngle number
--- @field TurningNodeOffset number
--- @field UseOcclusion boolean
--- @field UseSoundClustering boolean
--- @field UseStandAtDestination boolean
--- @field VFXScale number
--- @field VocalAlertResourceID FixedString
--- @field VocalAngryResourceID FixedString
--- @field VocalAnticipationResourceID FixedString
--- @field VocalAttackResourceID FixedString
--- @field VocalAwakeResourceID FixedString
--- @field VocalBoredResourceID FixedString
--- @field VocalBuffResourceID FixedString
--- @field VocalCinematicSuffix FixedString
--- @field VocalDeathResourceID FixedString
--- @field VocalDodgeResourceID FixedString
--- @field VocalEffortsResourceID FixedString
--- @field VocalExhaustedResourceID FixedString
--- @field VocalFallResourceID FixedString
--- @field VocalGaspResourceID FixedString
--- @field VocalIdle1ResourceID FixedString
--- @field VocalIdle2ResourceID FixedString
--- @field VocalIdle3ResourceID FixedString
--- @field VocalIdleCombat1ResourceID FixedString
--- @field VocalIdleCombat2ResourceID FixedString
--- @field VocalIdleCombat3ResourceID FixedString
--- @field VocalInitiativeResourceID FixedString
--- @field VocalLaughterManiacalResourceID FixedString
--- @field VocalLaughterResourceID FixedString
--- @field VocalNoneResourceID FixedString
--- @field VocalPainResourceID FixedString
--- @field VocalRebornResourceID FixedString
--- @field VocalRecoverResourceID FixedString
--- @field VocalRelaxedResourceID FixedString
--- @field VocalShoutResourceID FixedString
--- @field VocalSnoreResourceID FixedString
--- @field VocalSpawnResourceID FixedString
--- @field VocalVictoryResourceID FixedString
--- @field VocalWeakResourceID FixedString
--- @field WalkThrough boolean
--- @field WorldClimbingBlendspace_DownA FixedString
--- @field WorldClimbingBlendspace_DownB FixedString
--- @field WorldClimbingBlendspace_DownBHeight number
--- @field WorldClimbingBlendspace_UpA FixedString
--- @field WorldClimbingBlendspace_UpB FixedString
--- @field WorldClimbingBlendspace_UpBHeight number
--- @field WorldClimbingHeight number
--- @field WorldClimbingRadius number
--- @field WorldClimbingSpeed number


--- @class CharacterUnarmedDamageBoostComponent:BaseComponent
--- @field Amount StatsExpressionParam
--- @field DamageType DamageType


--- @class CharacterWeaponDamageBoostComponent:BaseComponent
--- @field Amount StatsExpressionParam
--- @field DamageType DamageType


--- @class ChasmRegionTriggerData:ITriggerData
--- @field Triggers FixedString[]


--- @class ChasmSeederTriggerData:ITriggerData
--- @field MaxChasmHeightOffset number
--- @field MaxChasmTiles int32
--- @field Triggers FixedString[]


--- @class CinematicArenaTriggerData:ITriggerData
--- @field MainMenuActivateCameraEventType string
--- @field MainMenuResetEventType string
--- @field MainMenuStartEventType string
--- @field Timeline Set_Guid


--- @class ClassInfo
--- @field ClassUUID Guid
--- @field Level int32
--- @field SubClassUUID Guid


--- @class ClassTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class ClassesComponent:BaseComponent
--- @field Classes ClassInfo[]


--- @class ClientControlComponent:BaseComponent


--- @class CombatComponentTemplate
--- @field AiHint Guid
--- @field AiUseCombatHelper FixedString
--- @field Archetype FixedString
--- @field CanFight boolean
--- @field CanJoinCombat boolean
--- @field CombatGroupID FixedString
--- @field Faction Guid
--- @field IsBoss boolean
--- @field IsInspector boolean
--- @field ProxyAttachment FixedString
--- @field ProxyOwner Guid
--- @field StartCombatRange number
--- @field StayInAiHints boolean
--- @field SwarmGroup FixedString
--- @field Unknown uint8
--- @field Unknown2 uint8


--- @class CombineActionData:IActionData
--- @field CombineSlots int8
--- @field IsBase boolean


--- @class CombinedLightComponent:MoveableObject
--- @field Entity EntityHandle
--- @field InstanceGuid FixedString
--- @field Template FixedString
--- @field TemplateType uint8
--- @field Transform Transform


--- @class CombinedLightTemplate:LightTemplate
--- @field GameplayCheckLOS boolean
--- @field GameplayDirectionalDimensions vec3
--- @field GameplayEdgeSharpening number
--- @field GameplayIsActive boolean
--- @field GameplayRadius number
--- @field GameplaySpotlightAngle number
--- @field IsHalfLit boolean
--- @field IsSunlight boolean
--- @field LightCookieResource FixedString


--- @class ConcentrationComponent:BaseComponent
--- @field Caster EntityHandle
--- @field SpellId SpellId
--- @field Targets ConcentrationTarget[]
--- @field field_0 EntityHandle
--- @field field_8 ConcentrationTarget[]


--- @class ConcentrationIgnoreDamageBoostComponent:BaseComponent
--- @field SpellSchool SpellSchoolId


--- @class ConcentrationTarget
--- @field Concentration EntityHandle
--- @field CreateSurfaceConcentrationTarget boolean
--- @field Status ComponentHandle
--- @field SurfaceIndex int16
--- @field Target EntityHandle
--- @field field_0 EntityHandle
--- @field field_10 ComponentHandle
--- @field field_18 int16
--- @field field_8 EntityHandle


--- @class ConditionRoll
--- @field Ability AbilityId
--- @field AbilityCheckRoll boolean
--- @field AttackRoll boolean
--- @field DataType uint8
--- @field Difficulty int32
--- @field DifficultyRoll boolean
--- @field Roll Variant<StatsRoll,StatsExpressionResolved>
--- @field RollType ConditionRollType
--- @field RollUuid Guid
--- @field SavingThrowRoll boolean
--- @field Skill SkillId
--- @field SkillCheckRoll boolean
--- @field SwappedSourceAndTarget boolean


--- @class ConditionRolls
--- @field Rolls ConditionRoll[]


--- @class ConstrainActionData:IActionData
--- @field Damage number


--- @class Construction
--- @field Filling EntityHandle[]
--- @field InstanceId Guid
--- @field Template ConstructionTemplate
--- @field Tiles EntityHandle[]


--- @class ConstructionFilling:StandardGameObject
--- @field Construction FixedString
--- @field Entity EntityHandle
--- @field FadeGroup FixedString
--- @field Fadeable boolean
--- @field HierarchyOnlyFade boolean
--- @field Id FixedString
--- @field InstanceId Guid
--- @field Material FixedString
--- @field Physics FixedString
--- @field Renderable RenderableObject
--- @field Renderable2 RenderableObject
--- @field SeeThrough boolean
--- @field Template ConstructionFillingTemplate
--- @field Tiling number
--- @field WalkOn boolean


--- @class ConstructionFillingTemplate
--- @field BoundMax vec3
--- @field BoundMin vec3
--- @field FadeChildren FixedString[]
--- @field FadeGroup FixedString
--- @field Fadeable boolean
--- @field HLOD Guid
--- @field HiddenFromMinimapRendering boolean
--- @field HierarchyOnlyFade boolean
--- @field Id FixedString
--- @field Indices uint16[]
--- @field Material FixedString
--- @field Name string
--- @field Physics FixedString
--- @field Rotation vec4
--- @field Scale number
--- @field SeeThrough boolean
--- @field Splines ConstructionSpline[]
--- @field Tiling number
--- @field Translate vec3
--- @field UVOffset vec2
--- @field UVRotation number
--- @field Vertices ConstructionVertexElement[]
--- @field WalkOn boolean
--- @field field_D8 uint8


--- @class ConstructionPoint:ConstructionPointSchema
--- @field Branches ConstructionPoint[]
--- @field field_40 uint8
--- @field field_41 uint8


--- @class ConstructionPointSchema
--- @field InstanceId Guid
--- @field Position vec3
--- @field Rotation vec4
--- @field Scale vec3
--- @field field_38 uint8
--- @field field_39 uint8


--- @class ConstructionSpline
--- @field InstanceId Guid


--- @class ConstructionTemplate:GameObjectTemplate
--- @field ConstructionBend boolean
--- @field FadeChildren FixedString[]
--- @field FadeGroup FixedString
--- @field Fadeable boolean
--- @field HierarchyOnlyFade boolean
--- @field SeeThrough boolean
--- @field TileSet FixedString
--- @field Tiles ConstructionTileTemplate[]
--- @field field_100 int64
--- @field field_108 int64


--- @class ConstructionTile:StandardGameObject
--- @field Construction FixedString
--- @field Entity EntityHandle
--- @field Flags uint8
--- @field InstanceId Guid
--- @field Scale number
--- @field Template ConstructionTileTemplate
--- @field field_48 EntityHandle
--- @field field_5C FixedString


--- @class ConstructionTileTemplate
--- @field CanSeeThrough boolean
--- @field ClickThrough boolean
--- @field Climbable boolean
--- @field Flip boolean
--- @field HLOD Guid
--- @field OverridePhysicsResource FixedString
--- @field Rotation vec4
--- @field Scale number
--- @field ScaleX number
--- @field ShootThrough boolean
--- @field ShootThroughType uint8
--- @field Side int32
--- @field Stretchable boolean
--- @field TemplateGuid Guid
--- @field Tile Guid
--- @field Transforms Transform[]
--- @field Translate vec3
--- @field UUID FixedString
--- @field WalkOn boolean
--- @field WalkThrough boolean
--- @field gapA4 uint8


--- @class ConstructionVertexElement
--- @field Position vec3
--- @field UV vec2
--- @field field_14 int32
--- @field field_18 int32
--- @field field_1C int32
--- @field field_20 int32


--- @class ConsumeActionData:IActionData
--- @field Consume boolean
--- @field IsHiddenStatus boolean
--- @field StatsId FixedString
--- @field StatusDuration int32


--- @class CoverageSettings
--- @field EndHeight number
--- @field HorizonDistance number
--- @field Offset vec2
--- @field StartHeight number
--- @field TexResourceGUID FixedString


--- @class CreatePuddleActionData:IActionData
--- @field ApplyDeathTypeBloodCheck boolean
--- @field CellsAtGrow int32
--- @field ExternalCauseAsSurfaceOwner boolean
--- @field GrowTimer number
--- @field LifeTime number
--- @field SurfaceType SurfaceType
--- @field Timeout number
--- @field TotalCells int32


--- @class CreateSurfaceActionData:IActionData
--- @field ExternalCauseAsSurfaceOwner boolean
--- @field LifeTime number
--- @field Radius number
--- @field SurfaceType SurfaceType


--- @class CrimeAreaTriggerData:ITriggerData
--- @field CrimeArea int16


--- @class CrimeRegionTriggerData:ITriggerData
--- @field Region FixedString


--- @class CriticalHitBoostComponent:BaseComponent
--- @field Flags CriticalHitAttackFlags
--- @field SuccessFlags CriticalHitSuccessFlags
--- @field Value number
--- @field field_1 CriticalHitSuccessFlags


--- @class CriticalHitExtraDiceBoostComponent:BaseComponent
--- @field Amount uint8
--- @field AttackType SpellAttackType


--- @class CrowdCharacterCustomAnimation
--- @field AnimationShortName FixedString
--- @field Weight number


--- @class CrowdCharacterDynamicAnimationTag
--- @field Tag Guid
--- @field Weight number


--- @class CrowdCharacterTriggerData:ITriggerData
--- @field AnimationSelectionTimeMax number
--- @field AnimationSelectionTimeMin number
--- @field CharacterSpacing number
--- @field CustomAnimations CrowdCharacterCustomAnimation[]
--- @field DetourChance number
--- @field DetourMaxWaitTime number
--- @field DetourMinWaitTime number
--- @field DetourPatrolSplines FixedString[]
--- @field DispersePatrolSplines FixedString[]
--- @field DynamicAnimationTags CrowdCharacterDynamicAnimationTag[]
--- @field GroupSizeMax int16
--- @field GroupSizeMin int16
--- @field GroupSpawnTimeMax number
--- @field GroupSpawnTimeMin number
--- @field MaxCharacters int16
--- @field PatrolSplines FixedString[]
--- @field PatrolWalkingPercentage number
--- @field PointsOfInterest FixedString[]
--- @field PositionPerturbation number
--- @field ProxiesOnly boolean
--- @field ReversePatrolDirection boolean
--- @field SpawnTemplates CrowdCharacterTriggerSpawnTemplate[]
--- @field SpawningEnabled boolean
--- @field YawPerturbationDegrees number


--- @class CrowdCharacterTriggerSpawnTemplate
--- @field CharacterTemplateId FixedString
--- @field SpawnWeight number


--- @class CullComponent:BaseComponent
--- @field CullFlags uint16


--- @class CullTrigger:AreaTrigger


--- @class CustomIconComponent:BaseComponent
--- @field Icon ScratchBuffer
--- @field Source uint8


--- @class CustomIconsStorageSingletonComponent:BaseComponent
--- @field Icons table<Guid, ScratchBuffer>


--- @class CustomNameComponent:BaseComponent
--- @field Name string


--- @class CustomStatsComponent:BaseComponent
--- @field Stats table<FixedString, int32>


--- @class DOFSettings
--- @field Aperture number
--- @field DOF boolean
--- @field FarOnly boolean
--- @field FarSharpDistance number
--- @field FocalDistance number
--- @field NearOnly boolean
--- @field NearSharpDistance number


--- @class DamageBonusBoostComponent:BaseComponent
--- @field Amount StatsExpressionParam
--- @field DamageType DamageType
--- @field field_31 uint8


--- @class DamageModifierMetadata
--- @field Argument Variant<int32,RollDefinition,StatsExpressionResolved>
--- @field DamageType DamageType
--- @field Description TranslatedString
--- @field Description2 FixedString
--- @field MetadataType uint8
--- @field Source Variant<int32,RollDefinition,StatsExpressionResolved>
--- @field SourceId FixedString
--- @field SourceName TranslatedString
--- @field SourceType uint8
--- @field Value int32


--- @class DamagePair
--- @field Amount int32
--- @field DamageType DamageType


--- @class DamageReductionBoostComponent:BaseComponent
--- @field Amount Variant<int32,StatsExpressionParam>
--- @field DamageType DamageType
--- @field Flat boolean
--- @field Half boolean


--- @class DamageResistance
--- @field Flags uint8
--- @field Meta DamageModifierMetadata
--- @field Type uint8


--- @class DamageTakenBonusBoostComponent:BaseComponent
--- @field Amount StatsExpressionParam
--- @field Arg3 boolean
--- @field DamageType DamageType


--- @class DangerousAuras
--- @field Auras AoERangeFilterTargetData[]
--- @field Avoidance uint8
--- @field Target AoERangeFilterTargetData
--- @field field_14 int32


--- @class DarknessComponent:BaseComponent
--- @field ActuallySneaking boolean
--- @field CharacterSneaking boolean
--- @field GraceFrames number
--- @field GracePeriod number
--- @field Obscurity uint8
--- @field Sneaking uint8
--- @field SneakingConesVisibility boolean
--- @field field_2 boolean
--- @field field_3 boolean
--- @field field_4 boolean
--- @field field_8 number
--- @field field_C number


--- @class DarkvisionRangeBoostComponent:BaseComponent
--- @field Range number


--- @class DarkvisionRangeMinBoostComponent:BaseComponent
--- @field Range number


--- @class DarkvisionRangeOverrideBoostComponent:BaseComponent
--- @field Range number


--- @class DataComponent:BaseComponent
--- @field StatsId FixedString
--- @field StepsType uint32
--- @field Weight int32


--- @class DataGrid
--- @field CellSize number
--- @field SizeX int32
--- @field SizeY int32
--- @field Translate vec3


--- @class DefaultCameraBehavior:BaseComponent
--- @field CaptureInput boolean
--- @field Forward number
--- @field Left number
--- @field RotationX number
--- @field RotationY number
--- @field Zoom number
--- @field field_0 number
--- @field field_19 boolean
--- @field field_1A boolean


--- @class DefaultSyncedTriggerData:ITriggerData


--- @class DestroyActionData:IActionData


--- @class DestroyParametersActionData:IActionData
--- @field ExplodeFX FixedString
--- @field FadeOutDelay number
--- @field FadeOutFX FixedString
--- @field SnapToGround boolean
--- @field TargetItemState uint8
--- @field TemplateAfterDestruction FixedString
--- @field VisualDestruction FixedString
--- @field VisualWithDynamicPhysics FixedString


--- @class DetachedComponent:BaseComponent
--- @field Flags uint32


--- @class DetectCrimesBlockBoostComponent:BaseComponent
--- @field field_0 boolean


--- @class DialogStateComponent:BaseComponent
--- @field DialogId int32
--- @field field_0 uint8
--- @field field_1 uint8
--- @field field_2 uint8
--- @field field_4 int32
--- @field field_8 uint8


--- @class DifficultyCheckComponent:BaseComponent
--- @field Abilities AbilityId[]
--- @field AbilityDC int32[]
--- @field AbilityModifiers int32[]
--- @field field_0 int32[]
--- @field field_10 int32[]
--- @field field_30 uint32[]
--- @field field_40 int32
--- @field field_44 int32


--- @class DirLight
--- @field CascadeCount uint8
--- @field CascadeSpeed number
--- @field ColorAdjustedForIntensity vec3
--- @field ColorTemperatureAdjustment vec3
--- @field CoverageSettings CoverageSettings
--- @field Kelvin number
--- @field LightDistance number
--- @field LightSize number
--- @field LocalCoverageEnabled boolean
--- @field LocalCoverageScalar number
--- @field Pitch number
--- @field RotationAsVec3 vec3
--- @field ScatteringIntensityScale number
--- @field ShadowBias number
--- @field ShadowEnabled boolean
--- @field ShadowFade number
--- @field ShadowFarPlane number
--- @field ShadowNearPlane number
--- @field ShadowObscurity uint8
--- @field SunColor vec3
--- @field SunIntensity number
--- @field SunlightObscurity uint8
--- @field UseTemperature boolean
--- @field Yaw number


--- @class DisabledEquipmentComponent:BaseComponent
--- @field ShapeshiftFlag boolean


--- @class DisarmTrapActionData:IActionData
--- @field Consume boolean


--- @class DisarmableComponent:BaseComponent
--- @field field_0 Guid
--- @field field_10 uint8
--- @field field_11 uint8


--- @class DisplayNameComponent:BaseComponent
--- @field Name TranslatedString
--- @field NameKey TranslatedString
--- @field Title TranslatedString
--- @field UnknownKey TranslatedString


--- @class DodgeAttackRollBoostComponent:BaseComponent
--- @field Roll uint8
--- @field StatusType FixedString
--- @field field_0 uint8
--- @field field_4 int32


--- @class DoorActionData:IActionData
--- @field SecretDoor boolean


--- @class DownedStatusBoostComponent:BaseComponent
--- @field Priority int32
--- @field StatusId FixedString
--- @field field_4 int32


--- @class DualWieldingBoostComponent:BaseComponent
--- @field DualWielding boolean


--- @class DualWieldingComponent:BaseComponent
--- @field DisableDualWielding boolean
--- @field HasMeleeBoosts boolean
--- @field HasRangedBoosts boolean
--- @field Melee boolean
--- @field MeleeToggledOn boolean
--- @field MeleeUI boolean
--- @field Ranged boolean
--- @field RangedToggledOn boolean
--- @field RangedUI boolean
--- @field ToggledOn boolean
--- @field field_1A boolean
--- @field field_1D boolean


--- @class DynamicAnimationTagsComponent:BaseComponent
--- @field Tags AnimationTag[]


--- @class EffectCameraBehavior:BaseComponent
--- @field field_0 vec3


--- @class EffectComponent:BaseProxyComponent
--- @field AnimationName FixedString
--- @field ConstructFlags uint16
--- @field EffectName FixedString
--- @field EffectResource ResourceEffectResource
--- @field Entity EntityHandle
--- @field Flags EffectFlags
--- @field Initialized boolean
--- @field OverrideFadeCapacity number[]
--- @field OverrideFadeShadowEnabled boolean[]
--- @field OverridingFadeOpacity boolean
--- @field Parent EntityHandle
--- @field SoundEntity EntityHandle
--- @field Timeline AspkEffectTimeline
--- @field UpdateQueued boolean
--- @field field_48 EntityHandle


--- @class EncumbranceStateComponent:BaseComponent
--- @field State uint32


--- @class EncumbranceStatsComponent:BaseComponent
--- @field EncumberedWeight int32
--- @field HeavilyEncumberedWeight int32
--- @field UnencumberedWeight int32
--- @field field_0 int32
--- @field field_4 int32
--- @field field_8 int32


--- @class EntityThrowDamageBoostComponent:BaseComponent
--- @field Roll RollDefinition
--- @field field_C uint8


--- @class EoCGameObjectTemplate:GameObjectTemplate
--- @field AIBounds BoundData[]
--- @field CollideWithCamera boolean
--- @field DisplayName TranslatedString
--- @field FadeChildren FixedString[]
--- @field FadeGroup FixedString
--- @field Fadeable boolean
--- @field HierarchyOnlyFade boolean
--- @field SeeThrough boolean


--- @class EocLevelComponent:BaseComponent
--- @field Level int32


--- @class EquipActionData:IActionData


--- @class EquipableComponent:BaseComponent
--- @field EquipmentTypeID Guid
--- @field Slot ItemSlot
--- @field field_18 Guid


--- @class EquipmentData
--- @field AfroLongHair table<Guid, FixedString>
--- @field AfroShortHair table<Guid, FixedString>
--- @field CurlyLongHair table<Guid, FixedString>
--- @field CurlyShortHair table<Guid, FixedString>
--- @field DreadLongHair table<Guid, FixedString>
--- @field DreadShortHair table<Guid, FixedString>
--- @field LongHair table<Guid, FixedString>
--- @field ParentRace table<Guid, Guid>
--- @field ShortHair table<Guid, FixedString>
--- @field Slot FixedString[]
--- @field SyncWithParent Set_Guid
--- @field VisualSet ResourceVisualSet
--- @field Visuals table<Guid, FixedString[]>
--- @field WavyLongHair table<Guid, FixedString>
--- @field WavyShortHair table<Guid, FixedString>


--- @class EquipmentVisualComponent:BaseComponent
--- @field State uint8


--- @class EventTriggerData:ITriggerData
--- @field EnterEvent FixedString
--- @field LeaveEvent FixedString


--- @class ExpertiseBonusBoostComponent:BaseComponent
--- @field Skill SkillId


--- @class ExpertiseComponent:BaseComponent
--- @field Expertise Set_SkillId


--- @class ExplorationTriggerData:ITriggerData
--- @field ExplorationReward Guid
--- @field LevelOverride int32


--- @class ExposureSettings
--- @field Exposure boolean
--- @field ExposureCompensation number
--- @field ExposureMax number
--- @field ExposureMaxPercentile number
--- @field ExposureMeteringMode int32
--- @field ExposureMin number
--- @field ExposureMinPercentile number
--- @field field_1C int32
--- @field field_20 int32
--- @field field_24 int32
--- @field field_28 int32


--- @class FactionComponent:BaseComponent
--- @field SummonOwner EntityHandle
--- @field field_0 EntityHandle
--- @field field_18 Guid
--- @field field_28 EntityHandle
--- @field field_8 Guid


--- @class FactionOverrideBoostComponent:BaseComponent
--- @field Faction Guid
--- @field field_10 uint8


--- @class FadeTrigger:AreaTrigger


--- @class FallDamageMultiplierBoostComponent:BaseComponent
--- @field Amount number


--- @class FixedRollBonus
--- @field Description TranslatedString
--- @field RollBonus int32
--- @field SourceName TranslatedString
--- @field field_0 int32


--- @class FleeCapabilityComponent:BaseComponent
--- @field CurrentFleeDistance number
--- @field Flags FleeErrorFlags
--- @field FleeDistance number
--- @field field_1C number
--- @field field_20 number


--- @class FloatingComponent:BaseComponent
--- @field field_18 int32
--- @field field_1C int32


--- @class FloorTrigger:AreaTrigger
--- @field BuildingUUID FixedString
--- @field Floor int32
--- @field IsRoof boolean


--- @class FloorTriggerData:ITriggerData
--- @field AttachedPortal FixedString[]
--- @field BuildingUUID FixedString
--- @field Floor int32
--- @field IsRoof boolean
--- @field TriggersToHide FixedString[]


--- @class Fog
--- @field FogLayer0 FogLayer
--- @field FogLayer1 FogLayer
--- @field Phase number
--- @field RenderDistance number


--- @class FogLayer
--- @field Albedo vec3
--- @field Density0 number
--- @field Density1 number
--- @field Enabled boolean
--- @field Height0 number
--- @field Height1 number
--- @field NoiseCoverage number
--- @field NoiseFrequency vec3
--- @field NoiseRotation vec3
--- @field NoiseWind vec3


--- @class FogVolumeRequestComponent:MoveableObject
--- @field field_0 Guid


--- @class GameObjectTemplate
--- @field AllowReceiveDecalWhenAnimated boolean
--- @field CastShadow boolean
--- @field FileName string
--- @field GlobalDeletedFlag uint8
--- @field GroupID uint32
--- @field Id FixedString
--- @field IsReflecting boolean
--- @field IsShadowProxy boolean
--- @field LevelName FixedString
--- @field Name string
--- @field ParentTemplateFlags uint8
--- @field ParentTemplateId FixedString
--- @field PhysicsTemplate FixedString
--- @field ReceiveDecal boolean
--- @field RenderChannel uint8
--- @field Tags TemplateTagContainer
--- @field TemplateHandle uint32
--- @field TemplateName FixedString
--- @field TemplateType FixedString
--- @field Transform Transform
--- @field VisualTemplate FixedString


--- @class GameObjectVisualComponent:BaseComponent
--- @field Icon FixedString
--- @field RootTemplateId FixedString
--- @field RootTemplateType uint8
--- @field Scale number
--- @field Type uint8


--- @class GameTime
--- @field DeltaTime number
--- @field Ticks int32
--- @field Time number
--- @field Unknown number


--- @class GameplayLightBoostComponent:BaseComponent
--- @field LOS boolean
--- @field Radius number
--- @field Sharpening number
--- @field Sunlight uint8
--- @field field_0 number
--- @field field_C uint8


--- @class GameplayLightComponent:BaseComponent
--- @field Active boolean
--- @field AttachAt vec3
--- @field CheckLOS boolean
--- @field DirectionalDimensions vec3
--- @field EdgeSharpening number
--- @field IsHalfLit boolean
--- @field LightCookie FixedString
--- @field LightType uint8
--- @field PointLightVerticalLimit number?
--- @field Radius number
--- @field SpotlightAngle number
--- @field Sunlight boolean
--- @field field_0 boolean
--- @field field_10 vec3
--- @field field_1C uint8
--- @field field_1D boolean
--- @field field_1E boolean
--- @field field_20 vec3
--- @field field_2C number?
--- @field field_34 boolean
--- @field field_38 FixedString
--- @field field_3C number
--- @field field_4 number
--- @field field_40 number
--- @field field_44 uint8
--- @field field_8 number
--- @field field_C number


--- @class GameplayObscurityBoostComponent:BaseComponent
--- @field Obscurity number


--- @class GenericPropertyTag
--- @field Entity Guid
--- @field Type uint8


--- @class GlobalLongRestDisabledComponent:BaseComponent


--- @class GlobalShortRestDisabledComponent:BaseComponent


--- @class GlobalSwitches
--- @field ActiveCustomDice string
--- @field AiEnableSwarm boolean
--- @field AlwaysShowSplitter boolean
--- @field AnalyticsLevelEnded boolean
--- @field AnalyticsSessionEnded boolean
--- @field AnimationBlueprintSpeedScale_M number
--- @field AutoFillHotbarCategories uint16
--- @field AutoListenEnabled boolean
--- @field AutoRemoveHotbarSpells uint8
--- @field CameraShakeEnabled boolean
--- @field CanAutoSave boolean
--- @field CanCrossSave boolean
--- @field CanLoadDuringTimeline boolean
--- @field CanSaveDuringTimeline boolean
--- @field CentreDialogue boolean
--- @field CheckRequirements boolean
--- @field CombatCharacterHighlightMode uint8
--- @field ControllerCharacterRunThreshold number
--- @field ControllerCharacterWalkThreshold number
--- @field ControllerLayout uint8
--- @field ControllerMode uint8
--- @field ControllerRumbleStrength int32
--- @field ControllerSensitivityLeft int32
--- @field ControllerSensitivityRight int32
--- @field ControllerStickDeadZone int32
--- @field ControllerStickPressDeadZone int32
--- @field ControllerTriggerDeadZone int32
--- @field CrossplayEnabled boolean
--- @field CrossplayInUse boolean
--- @field DialogueFontSizeModifier int32
--- @field Difficulty uint8
--- @field DisableEdgePanning boolean
--- @field DisableLocalMessagePassing boolean
--- @field DisableStoryPatching boolean
--- @field DisplaySpeaker boolean
--- @field DoUnlearnCheck boolean
--- @field EnableGameplayLightSystem boolean
--- @field EnableLongPressAndHold boolean
--- @field EnablePortmapping boolean
--- @field EnableSteamP2P boolean
--- @field EnableVoiceLogging boolean
--- @field EscClosesAllUI boolean
--- @field ExtraTooltipDescriptions uint8
--- @field FadeSpeed number
--- @field Fading boolean
--- @field FirstTimeWithCrossSave boolean
--- @field ForceMono boolean
--- @field ForcePort int16
--- @field ForceSplitscreen boolean
--- @field ForceStoryPatching boolean
--- @field GameCameraAutoRotateEnabled boolean
--- @field GameCameraAutoRotateSpeed number
--- @field GameCameraEnableAttackCamera boolean
--- @field GameCameraEnableAttackCameraOtherPlayers boolean
--- @field GameCameraEnableCloseUpDialog boolean
--- @field GameCameraEnableDynamicCombatCamera boolean
--- @field GameCameraEnableFlyFollow boolean
--- @field GameCameraRotation number
--- @field GameCameraRotationLocked boolean
--- @field GameCameraSpeedMultiplier number
--- @field GameVisibilityDirect uint32
--- @field GameVisibilityLAN uint32
--- @field GameVisibilityOnline uint32
--- @field GodMode boolean
--- @field HasSeenEAMsg boolean
--- @field HideCompletedQuests boolean
--- @field HoldToEndTurn boolean
--- @field Language string
--- @field LoadAllEffectPools boolean
--- @field LogSaveLoadErrors boolean
--- @field LongRestDefaultTimeline string
--- @field LongRestScriptWaitingTime number
--- @field MaxAmountDialogsInLog int32
--- @field MaxNrOfAutoSaves int32
--- @field MaxNrOfQuickSaves int32
--- @field MouseLock boolean
--- @field MouseScrollSensitivity int32
--- @field MouseSensitivity int32
--- @field MuteSoundWhenNotFocused boolean
--- @field NoPrivateDialogues boolean
--- @field NodeWaitTimeMultiplier number
--- @field NorthFacingMinimap boolean
--- @field NrOfAutoSaves int32
--- @field NrOfQuickSaves int32
--- @field OverheadFontSizeModifier int32
--- @field PeaceCharacterHighlightMode uint8
--- @field PeaceMode boolean
--- @field PointAndClickSoundFrequency number
--- @field PostCombatAssignation uint8
--- @field RandomStartOrigin boolean
--- @field RulesetModifiers table<Guid, Variant<uint8,int32,float,FixedString,bool>>
--- @field Rulesets Guid[]
--- @field SaveStory boolean
--- @field ScreenshotDir string
--- @field ScriptLog boolean
--- @field ServerFrameCap int32
--- @field ShowCharacterCreation boolean
--- @field ShowCombatFeed boolean
--- @field ShowCombatFeedSplitScreen boolean
--- @field ShowExplicitRomanceScenes boolean
--- @field ShowFirstTimeSetup boolean
--- @field ShowGenitals boolean
--- @field ShowLocalizationMarkers boolean
--- @field ShowOriginIntroInCC boolean
--- @field ShowPings boolean
--- @field ShowSubtitles boolean
--- @field ShowTextBackground boolean
--- @field ShowTutorials boolean
--- @field ShowTutorialsAnswered boolean
--- @field ShroudEnabled boolean
--- @field SkipLarianSignUp boolean
--- @field SkipSplashScreen boolean
--- @field SoundOcclusionQuality int32
--- @field SoundPartyLosingThreshold int32
--- @field SoundPartyWinningThreshold int32
--- @field SoundVolumeDynamicRange int32
--- @field StartDay int32
--- @field StartYear int32
--- @field Stats boolean
--- @field Story boolean
--- @field StoryEvents boolean
--- @field StoryLog boolean
--- @field TacticalCharacterHighlightMode uint8
--- @field TextBackgroundOpacity int32
--- @field TimelinesAD boolean
--- @field TimelinesAttitudeMimicry boolean
--- @field TimelinesAutoContinue boolean
--- @field TimelinesCombatAuthored boolean
--- @field TimelinesEmoteMimicry boolean
--- @field TimelinesFade boolean
--- @field TimelinesLookAtMimicry boolean
--- @field TimelinesQCameras boolean
--- @field TimelinesShowDisabled boolean
--- @field TwitchEnableOverlay boolean
--- @field UIScaling int32
--- @field UnitSystem uint8
--- @field UseEndTurnFallback boolean
--- @field UseLevelCache boolean
--- @field UseRadialContextMenu boolean
--- @field UseSavingThrows boolean
--- @field WeightedRolls boolean
--- @field saveSystemECBCheckNumberOfFramesToWait int32
--- @field saveSystemECBCheckerAllowSaveOnFailure boolean
--- @field saveSystemECBCheckerEnableDetailedLogging boolean
--- @field saveSystemECBCheckerEnableLogging boolean
--- @field saveSystemECBCheckerLogSuccessfulAttempts boolean
--- @field useSaveSystemECBChecker boolean


--- @class GodComponent:BaseComponent
--- @field God Guid
--- @field GodOverride Guid?


--- @class GodTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class GravityDisabledComponent:BaseComponent


--- @class GravityDisabledUntilMovedComponent:BaseComponent
--- @field Transform Transform


--- @class GuaranteedChanceRollOutcomeBoostComponent:BaseComponent
--- @field field_0 boolean


--- @class HalveWeaponDamageBoostComponent:BaseComponent
--- @field Ability AbilityId


--- @class HasExclamationDialogComponent:BaseComponent


--- @class HealthComponent:BaseComponent
--- @field Hp int32
--- @field IsInvulnerable boolean
--- @field MaxHp int32
--- @field MaxTemporaryHp int32
--- @field TemporaryHp int32
--- @field field_10 Guid
--- @field field_20 boolean


--- @class HearingComponent:BaseComponent
--- @field Hearing number


--- @class HitDamageOverride
--- @field DamageType DamageType
--- @field OriginalValue int32
--- @field OverriddenValue int32


--- @class HitDesc
--- @field ArmorAbsorption int32
--- @field AttackFlags AttackFlags
--- @field AttackRollAbility AbilityId
--- @field CauseType CauseType
--- @field ConditionRolls ConditionRoll[]
--- @field Damage StatsDamage
--- @field DamageList DamagePair[]
--- @field DamageType DamageType
--- @field DeathType StatsDeathType
--- @field EffectFlags uint32
--- @field FallDamageMultiplier number
--- @field FallHeight number
--- @field FallMaxDamage number
--- @field FallWeight number
--- @field HealingTypes uint8
--- @field HitDescFlags uint8
--- @field HitWith HitWith
--- @field ImpactDirection vec3
--- @field ImpactForce number
--- @field ImpactPosition vec3
--- @field Inflicter EntityHandle
--- @field InflicterOwner EntityHandle
--- @field LifeSteal int32
--- @field OriginalDamageValue int32
--- @field OverriddenDamage HitDamageOverride[]
--- @field RedirectedDamage number
--- @field Results StatsDamage
--- @field SaveAbility AbilityId
--- @field SpellAttackType uint8
--- @field SpellCastGuid Guid
--- @field SpellId FixedString
--- @field SpellLevel int32
--- @field SpellPowerLevel int32
--- @field SpellSchool SpellSchoolId
--- @field StatusEntity EntityHandle
--- @field StatusId FixedString
--- @field StoryActionId int32
--- @field Throwing EntityHandle
--- @field TotalDamageDone int32
--- @field TotalHealDone int32
--- @field field_150 FixedString
--- @field field_158 EntityHandle
--- @field field_160 uint8
--- @field field_174 number
--- @field field_178 number
--- @field field_17C number
--- @field field_180 number
--- @field field_184 number
--- @field field_188 uint8
--- @field field_4F uint8


--- @class HitResult
--- @field Attack AttackDesc
--- @field Hit HitDesc
--- @field NumConditionRolls uint32
--- @field Results HitResultData


--- @class HitResultData
--- @field field_0 int32[]
--- @field field_10 int32[]
--- @field field_20 uint8[]
--- @field field_30 int32[]


--- @class HorizontalFOVOverrideBoostComponent:BaseComponent
--- @field FOV number


--- @class IActionData
--- @field Animation FixedString
--- @field Conditions string
--- @field Type ActionDataType


--- @class ITriggerData


--- @class ITriggerPhysicsData


--- @class IconComponent:BaseComponent
--- @field Icon FixedString


--- @class IdentityComponent:BaseComponent
--- @field field_0 uint8


--- @class IdentityStateComponent:BaseComponent
--- @field Disguised boolean
--- @field field_0 boolean


--- @class IgnoreDamageThresholdMinBoostComponent:BaseComponent
--- @field All boolean
--- @field Amount uint16
--- @field DamageType DamageType


--- @class IgnoreLowGroundPenaltyBoostComponent:BaseComponent
--- @field RollType StatsRollType


--- @class IgnorePointBlankDisadvantageBoostComponent:BaseComponent
--- @field Flags WeaponFlags


--- @class IgnoreResistanceBoostComponent:BaseComponent
--- @field DamageType DamageType
--- @field Flags ResistanceBoostFlags


--- @class IgnoreSurfaceCoverBoostComponent:BaseComponent
--- @field Surface SurfaceType


--- @alias ImguiHandle ExtuiStyledRenderable



--- @class IncreaseMaxHPComponent:BaseComponent
--- @field Amount Variant<int32,StatsExpressionParam>
--- @field field_30 int32


--- @class InitiativeBoostComponent:BaseComponent
--- @field Amount int32


--- @class InsertActionData:IActionData
--- @field InsertSlots int8


--- @class InspirationPointGainedRequest
--- @field Amount number
--- @field Entity EntityHandle
--- @field Entity2 EntityHandle
--- @field field_1C TranslatedString
--- @field field_C TranslatedString


--- @class InteractionFilterComponent:BaseComponent
--- @field field_0 Set_Guid
--- @field field_30 uint8
--- @field field_31 uint8


--- @class InventoryItemData
--- @field Amount int32
--- @field CanBePickpocketed boolean
--- @field Conditions AiActionConditions
--- @field IsDroppedOnDeath boolean
--- @field IsTradable IsTradableType
--- @field ItemName string
--- @field LevelName string
--- @field TemplateID FixedString
--- @field Type uint8
--- @field UUID FixedString


--- @class InvisibilityComponent:BaseComponent
--- @field field_0 uint8
--- @field field_10 uint8
--- @field field_4 vec3


--- @class IsFallingComponent:BaseComponent


--- @class IsGlobalComponent:BaseComponent


--- @class IsInTurnBasedModeComponent:BaseComponent


--- @class IsSeeThroughComponent:BaseComponent


--- @class IsSummonComponent:BaseComponent
--- @field Owner EntityHandle
--- @field Owner_M EntityHandle
--- @field Summoner EntityHandle
--- @field field_10 Guid
--- @field field_20 EntityHandle
--- @field field_28 FixedString
--- @field field_8 EntityHandle


--- @class ItemBoostsComponent:BaseComponent
--- @field Boosts EntityHandle[]


--- @class ItemTemplate:SceneryTemplate
--- @field ActivationGroupId FixedString
--- @field AllowSummonGenericUse boolean
--- @field Amount int32
--- @field AnubisConfigName FixedString
--- @field AttackableWhenClickThrough boolean
--- @field BloodSurfaceType uint8
--- @field BloodType FixedString
--- @field BookType uint8
--- @field CanBeImprovisedWeapon boolean
--- @field CanBeMoved boolean
--- @field CanBePickedUp boolean
--- @field CanBePickpocketed boolean
--- @field CinematicArenaFlags uint32
--- @field ColorPreset Guid
--- @field CombatComponent CombatComponentTemplate
--- @field ConstellationConfigName FixedString
--- @field ContainerAutoAddOnPickup boolean
--- @field ContainerContentFilterCondition string
--- @field CriticalHitType FixedString
--- @field DefaultState FixedString
--- @field Description TranslatedString
--- @field DestroyWithStack boolean
--- @field Destroyed boolean
--- @field DisarmDifficultyClassID Guid
--- @field DisplayNameAlchemy TranslatedString
--- @field DropSound FixedString
--- @field EquipSound FixedString
--- @field Equipment EquipmentData
--- @field EquipmentTypeID Guid
--- @field ExamineRotation vec3
--- @field ExcludeInDifficulty Guid[]
--- @field ForceAffectedByAura boolean
--- @field FreezeGravity uint8
--- @field GravityType uint8
--- @field Hostile boolean
--- @field Icon FixedString
--- @field IgnoreGenerics boolean
--- @field ImpactSound FixedString
--- @field InteractionFilterList Set_Guid
--- @field InteractionFilterRequirement uint8
--- @field InteractionFilterType uint8
--- @field InventoryList FixedString[]
--- @field InventoryMoveSound FixedString
--- @field InventoryType uint8
--- @field IsBlueprintDisabledByDefault boolean
--- @field IsDroppedOnDeath boolean
--- @field IsInteractionDisabled boolean
--- @field IsKey boolean
--- @field IsPlatformOwner boolean
--- @field IsPortal boolean
--- @field IsPortalProhibitedToPlayers boolean
--- @field IsPublicDomain boolean
--- @field IsSourceContainer boolean
--- @field IsSurfaceBlocker boolean
--- @field IsSurfaceCloudBlocker boolean
--- @field IsTrap boolean
--- @field ItemList InventoryItemData[]
--- @field Key FixedString
--- @field LevelOverride int32
--- @field LightChannel uint8
--- @field LockDifficultyClassID Guid
--- @field MapMarkerStyle FixedString
--- @field MaterialPreset Guid
--- @field MaxStackAmount int32
--- @field OnDestroyActions IActionData[]
--- @field OnUseDescription TranslatedString
--- @field OnUsePeaceActions IActionData[]
--- @field OnlyInDifficulty Guid[]
--- @field Owner FixedString
--- @field PermanentWarnings FixedString
--- @field PhysicsCollisionSound FixedString
--- @field PhysicsFollowAnimation boolean
--- @field PickupSound FixedString
--- @field ShortDescription TranslatedString
--- @field ShortDescriptionParams string
--- @field ShowAttachedSpellDescriptions boolean
--- @field SpeakerGroups Set_Guid
--- @field Stats FixedString
--- @field StatusList FixedString[]
--- @field StoryItem boolean
--- @field TechnicalDescription TranslatedString
--- @field TechnicalDescriptionParams string
--- @field TimelineCameraRigOverride Guid
--- @field Tooltip uint32
--- @field TreasureLevel int32
--- @field TreasureOnDestroy boolean
--- @field UnequipSound FixedString
--- @field Unimportant boolean
--- @field UnknownDescription TranslatedString
--- @field UnknownDisplayName TranslatedString
--- @field UseOcclusion boolean
--- @field UseOnDistance boolean
--- @field UsePartyLevelForTreasureLevel boolean
--- @field UseRemotely boolean
--- @field UseSound FixedString
--- @field AddUseAction fun(self:ItemTemplate, a1:ActionDataType):IActionData
--- @field RemoveUseAction fun(self:ItemTemplate, a1:int32)


--- @class JumpMaxDistanceBonusBoostComponent:BaseComponent
--- @field DistanceBonus number


--- @class JumpMaxDistanceMultiplierBoostComponent:BaseComponent
--- @field Amount number


--- @class KeyComponent:BaseComponent
--- @field Key FixedString


--- @class LadderActionData:IActionData
--- @field AllowScaling boolean
--- @field BottomHorizontalOffset number
--- @field BottomVerticalOffset number
--- @field ClimbDirection int32
--- @field NodeLadderOffest number
--- @field TopAttachNearOffset number
--- @field TopDetachOffset number
--- @field TopLineTolerance number
--- @field TopMidOffset number
--- @field TopMidToPlatformFixedLength number


--- @class LearnSpellActionData:IActionData
--- @field Consume boolean
--- @field Spell FixedString


--- @class LevelComponent:BaseComponent
--- @field LevelName FixedString
--- @field field_0 EntityHandle


--- @class LevelUpData
--- @field Abilities int32[]
--- @field AccessorySet Guid
--- @field Class Guid
--- @field Feat Guid
--- @field Spells SpellMetaId[]
--- @field SubClass Guid
--- @field Upgrades LevelUpUpgrades
--- @field field_B0 SpellMetaId[]


--- @class LevelUpUpgrades
--- @field AbilityBonuses LevelUpUpgradesAbilityBonusSelector[]
--- @field Equipment LevelUpUpgradesEquipmentSelector[]
--- @field Feats LevelUpUpgradesFeatSelector[]
--- @field Passives LevelUpUpgradesPassiveSelector[]
--- @field SkillExpertise LevelUpUpgradesSkillExpertiseSelector[]
--- @field Skills LevelUpUpgradesSkillSelector[]
--- @field Spells LevelUpUpgradesSpellSelector[]
--- @field Spells2 LevelUpUpgradesPassiveSelector[]
--- @field Unknowns2 LevelUpUpgradesSkillExpertiseSelector[]
--- @field Unknowns4 LevelUpUpgradesEquipmentSelector[]


--- @class LevelUpUpgradesAbilityBonusSelector:LevelUpUpgradesSelector
--- @field AbilityBonus Guid
--- @field Array_b8 AbilityId[]
--- @field Array_i32 uint32[]
--- @field BonusAmounts uint32[]
--- @field BonusType string
--- @field Bonuses AbilityId[]
--- @field field_60 string
--- @field field_80 int32


--- @class LevelUpUpgradesEquipmentSelector:LevelUpUpgradesSelector
--- @field Array_FS FixedString[]
--- @field Equipment FixedString[]
--- @field EquipmentList Guid
--- @field SelectorId string
--- @field field_38 Guid
--- @field field_60 string
--- @field field_80 int32


--- @class LevelUpUpgradesFeatSelector:LevelUpUpgradesSelector
--- @field AbilityImprovements AbilityId[]
--- @field Array_b8 AbilityId[]
--- @field Feat Guid
--- @field FeatName string
--- @field field_80 int32


--- @class LevelUpUpgradesPassiveSelector:LevelUpUpgradesSelector
--- @field Array_FS2 LevelUpUpgradesPassiveSelectorStringPair[]
--- @field PassiveList Guid
--- @field Passives FixedString[]
--- @field ReplacePassives LevelUpUpgradesPassiveSelectorStringPair[]
--- @field SelectorId string
--- @field SpellList Guid
--- @field Spells FixedString[]
--- @field field_78 string
--- @field field_80 int32


--- @class LevelUpUpgradesPassiveSelectorStringPair
--- @field From FixedString
--- @field To FixedString


--- @class LevelUpUpgradesSelector
--- @field Class Guid
--- @field DefinitionIndex int32
--- @field Level int32
--- @field Multiclass boolean
--- @field Subclass Guid
--- @field Type ProgressionSelectorType
--- @field field_0 ProgressionSelectorType
--- @field field_28 int32
--- @field field_2C boolean
--- @field field_30 int32


--- @class LevelUpUpgradesSkillExpertiseSelector:LevelUpUpgradesSelector
--- @field Array_b8 SkillId[]
--- @field Expertise SkillId[]
--- @field Skill Guid
--- @field field_38 Guid
--- @field field_48 uint8
--- @field field_60 string
--- @field field_80 int32


--- @class LevelUpUpgradesSkillSelector:LevelUpUpgradesSelector
--- @field Array_b8 SkillId[]
--- @field Proficiencies SkillId[]
--- @field Skill Guid
--- @field field_60 string
--- @field field_80 int32


--- @class LevelUpUpgradesSpellSelector:LevelUpUpgradesSelector
--- @field Array_FS2 LevelUpUpgradesSpellSelectorStringPair[]
--- @field ReplaceSpells LevelUpUpgradesSpellSelectorStringPair[]
--- @field SelectorId string
--- @field SpellList Guid
--- @field Spells FixedString[]
--- @field field_78 string


--- @class LevelUpUpgradesSpellSelectorStringPair
--- @field From FixedString
--- @field To FixedString


--- @class LieActionData:IActionData
--- @field Heal number


--- @class LightComponent:MoveableObject
--- @field AssociatedScene EntityHandle
--- @field Blackbody vec3
--- @field Color vec3
--- @field CullFlags uint16
--- @field DirectionLightAttenuationEnd number
--- @field DirectionLightAttenuationFunction uint8
--- @field DirectionLightAttenuationSide number
--- @field DirectionLightAttenuationSide2 number
--- @field DirectionLightDimensions vec3
--- @field EdgeSharpening number
--- @field Flags uint8
--- @field Gain number
--- @field Intensity number
--- @field Kelvin number
--- @field LightChannelFlag uint8
--- @field LightType uint8
--- @field Radius number
--- @field ScatteringIntensityScale number
--- @field SpotLightInnerAngle number
--- @field SpotLightOuterAngle number
--- @field Template LightTemplate
--- @field UUID FixedString
--- @field field_80 EntityHandle
--- @field field_C0 uint32
--- @field field_E0 vec3


--- @class LightTemplate:GameObjectTemplate
--- @field Amount number
--- @field Angle vec2
--- @field Color vec3
--- @field DirectionLightAttenuationEnd number
--- @field DirectionLightAttenuationFunction uint8
--- @field DirectionLightAttenuationSide number
--- @field DirectionLightAttenuationStart number
--- @field DirectionLightDimensions vec3
--- @field Enabled boolean
--- @field FlatFalloff boolean
--- @field Gain number
--- @field Intensity number
--- @field IsFlickering boolean
--- @field IsMoving boolean
--- @field Kelvin number
--- @field LightChannelFlag uint8
--- @field LightCookieTexture FixedString
--- @field LightType uint8
--- @field MovementAmount number
--- @field MovementSpeed number
--- @field PreExpose boolean
--- @field Radius number
--- @field ScatteringScale number
--- @field Shadow boolean
--- @field Speed number
--- @field UseTemperature boolean
--- @field VolumetricShadow boolean


--- @class Lighting
--- @field Fog Fog
--- @field LinearClearColor vec3
--- @field LinearClearColorOverride boolean
--- @field Moon MoonLight
--- @field ParentGUID FixedString
--- @field SSAOSettings SSAOSettings
--- @field SkyLight SkyLight
--- @field Sun DirLight
--- @field TimelineFog Fog
--- @field TimelineFogOverride boolean
--- @field VolumetricCloudSettings VolumetricCloudSettings


--- @class LightingTrigger:AreaTrigger
--- @field CurrentLightingResourceID FixedString
--- @field LightingResourceIDs FixedString[]
--- @field field_84 number


--- @class LocalTransform
--- @field LocalBound AABound
--- @field LocalTransform Transform


--- @class LockBoostComponent:BaseComponent
--- @field Lock Guid


--- @class LockComponent:BaseComponent
--- @field Key_M FixedString
--- @field LockDC int32
--- @field field_18 Guid[]
--- @field field_8 Guid


--- @class LockpickActionData:IActionData
--- @field Consume boolean


--- @class LootComponent:BaseComponent
--- @field Flags uint8
--- @field InventoryType uint8


--- @class LootingStateComponent:BaseComponent
--- @field Looter_M EntityHandle
--- @field State uint8
--- @field field_24 int32


--- @class Material
--- @field BlendStateID uint8
--- @field DepthStateID uint8
--- @field DiffusionProfileUUID FixedString
--- @field Flags uint32
--- @field ForwardLightingMode uint32
--- @field MaterialPassHint uint8
--- @field MaterialType uint8
--- @field MaterialUsage uint8
--- @field Name FixedString
--- @field Parameters MaterialParametersSet
--- @field Parent Material
--- @field RasterizerStateID uint8
--- @field RenderChannel uint8
--- @field Shaders FixedString[]
--- @field ShadingModel uint32
--- @field UVCount uint32
--- @field UsedWithFlags uint32
--- @field Version64 uint64
--- @field field_510 uint32
--- @field SetScalar fun(self:Material, a1:FixedString, a2:number):boolean
--- @field SetVector2 fun(self:Material, a1:FixedString, a2:vec2):boolean
--- @field SetVector3 fun(self:Material, a1:FixedString, a2:vec3):boolean
--- @field SetVector4 fun(self:Material, a1:FixedString, a2:vec4):boolean


--- @class MaterialDecalParameters
--- @field DecalDeferredProperties int16
--- @field DecalDimensions int16
--- @field DecalTiling int16
--- @field DecalWorld int16
--- @field InvWorldMatrix int16
--- @field NormalMatrix int16


--- @class MaterialMaterialCB
--- @field MaterialCBSize uint64


--- @class MaterialParametersSet
--- @field Material Material
--- @field SamplerStateParameters MaterialSamplerStateParameter[]
--- @field ScalarParameters MaterialScalarParameter[]
--- @field Texture2DParameters MaterialTexture2DParameter[]
--- @field Vector2Parameters MaterialVector2Parameter[]
--- @field Vector3Parameters MaterialVector3Parameter[]
--- @field VectorParameters MaterialVector4Parameter[]
--- @field VirtualTextureParameters MaterialVirtualTextureParameter[]


--- @class MaterialSamplerStateParameter:ResourceMaterialResourceParameter
--- @field TextureAddressMode uint8
--- @field TextureFilterOverride uint8


--- @class MaterialScalarParameter:ResourceMaterialResourceScalarParameter


--- @class MaterialShaderDescription
--- @field DecalParameters MaterialDecalParameters
--- @field DiffusionProfileIndex int16
--- @field EngineCBBinding MaterialShaderParamBinding
--- @field EngineCBSize uint16
--- @field EngineParamFlags uint16
--- @field FadeOpacity int16
--- @field LightChannel int16
--- @field LocalBoundsMax int16
--- @field LocalBoundsMin int16
--- @field MaterialCBBinding MaterialShaderParamBinding
--- @field MaterialCBSize uint16
--- @field MeshRandom int16
--- @field ReceiveDecal int16
--- @field TransformedVerticesOffsetCurr int16
--- @field TransformedVerticesOffsetPrev int16
--- @field VirtualTexture_CB_Texture int16
--- @field VirtualTexture_TilesetDataIndex int16
--- @field WorldBoundsMaxSubVisual int16
--- @field WorldBoundsMaxVisual int16
--- @field WorldBoundsMinParent int16
--- @field WorldBoundsMinSubVisual int16
--- @field WorldBoundsMinVisual int16
--- @field WorldBoundsMxaParent int16
--- @field WorldMatrixCurr int16
--- @field WorldMatrixPrev int16
--- @field field_34 uint16
--- @field field_36 uint16


--- @class MaterialShaderParamBinding
--- @field DxPsIndex int8
--- @field DxVsIndex int8
--- @field VkBindingIndex int8
--- @field VkDescriptorSet int8


--- @class MaterialTexture2DParameter:ResourceMaterialResourceTexture2DParameter


--- @class MaterialTextureBindingData
--- @field PerShaderBindings MaterialShaderParamBinding[]
--- @field UniformName FixedString


--- @class MaterialUniformBindingData
--- @field PerShaderCBOffsets int16[]
--- @field UniformName FixedString


--- @class MaterialVector2Parameter:ResourceMaterialResourceVector2Parameter


--- @class MaterialVector3Parameter:ResourceMaterialResourceVector3Parameter


--- @class MaterialVector4Parameter:ResourceMaterialResourceVector4Parameter


--- @class MaterialVirtualTextureBindingData
--- @field PerShaderBindings MaterialShaderParamBinding[]
--- @field PerShaderVTBindings MaterialVirtualTextureShaderParamBinding[]
--- @field UniformName FixedString


--- @class MaterialVirtualTextureParameter:ResourceMaterialResourceVirtualTextureParameter


--- @class MaterialVirtualTextureShaderParamBinding
--- @field Cache MaterialShaderParamBinding
--- @field Cache1 MaterialShaderParamBinding
--- @field Cache2 MaterialShaderParamBinding
--- @field Cache3 MaterialShaderParamBinding


--- @class MaterialParameterOverride
--- @field field_0 string
--- @field field_18 FixedString


--- @class MaterialParameterOverrideComponent:BaseComponent
--- @field field_0 Guid[]
--- @field field_10 MaterialParameterOverride[]


--- @class MaterialRenderingData
--- @field EngineCBSize uint16
--- @field EngineDxPsIndex uint8
--- @field EngineDxVsIndex uint8
--- @field EngineVkBindingIndex uint8
--- @field EngineVkDescriptorSet uint8
--- @field Handle EntityHandle
--- @field InstancingHash uint64
--- @field MaterialCBSize uint16
--- @field MaterialDxPsIndex uint8
--- @field MaterialDxVsIndex uint8
--- @field MaterialVkBindingIndex uint8
--- @field MaterialVkDescriptorSet uint8
--- @field ShaderIndex uint8
--- @field field_19 uint8
--- @field field_1E uint8
--- @field field_20 uint8
--- @field field_21 uint8
--- @field field_22 uint8
--- @field field_24 uint8
--- @field field_27 uint8
--- @field field_29 uint8
--- @field field_2A uint8
--- @field field_2B uint8
--- @field field_2D uint8
--- @field field_8 EntityHandle


--- @class MaxIncomingComponent:BaseComponent


--- @class MaxOutgoingComponent:BaseComponent


--- @class MaximizeHealingBoostComponent:BaseComponent
--- @field Direction HealDirection
--- @field TargetTypes StatsTargetTypeFlags


--- @class MaximumRollResultBoostComponent:BaseComponent
--- @field Result int8
--- @field RollType StatsRollType


--- @class MeshBinding
--- @field Bound1 AABound
--- @field Bound2 AABound
--- @field Transform mat4


--- @class MinimumRollResultBoostComponent:BaseComponent
--- @field Result int8
--- @field RollType StatsRollType


--- @class ModManager
--- @field AvailableMods Module[]
--- @field BaseModule Module
--- @field Flag uint8
--- @field Flag2 uint8
--- @field LoadOrderedModules Module[]
--- @field Settings ModuleSettings


--- @class Module
--- @field Addons ModuleShortDesc[]
--- @field Dependencies ModuleShortDesc[]
--- @field Info ModuleInfo
--- @field ModConflicts ModuleShortDesc[]


--- @class ModuleInfo
--- @field Author string
--- @field CharacterCreationLevelName FixedString
--- @field Description string
--- @field Directory string
--- @field FileSize uint64
--- @field Hash string
--- @field LobbyLevelName FixedString
--- @field MenuLevelName FixedString
--- @field ModVersion Version
--- @field ModuleUUID Guid
--- @field ModuleUUIDString FixedString
--- @field Name string
--- @field NumPlayers uint8
--- @field PhotoBoothLevelName FixedString
--- @field PublishHandle uint64
--- @field PublishVersion Version
--- @field StartLevelName FixedString


--- @class ModuleSettings
--- @field Mods ModuleShortDesc[]


--- @class ModuleShortDesc
--- @field Folder string
--- @field Hash string
--- @field ModVersion Version
--- @field ModuleUUID Guid
--- @field ModuleUUIDString FixedString
--- @field Name string
--- @field PublishHandle uint64
--- @field PublishVersion Version


--- @class MonkWeaponDamageDiceOverrideBoostComponent:BaseComponent
--- @field DamageDice FixedString


--- @class MoonLight
--- @field AlbedoTexResourceGUID FixedString
--- @field CastLightEnabled boolean
--- @field Color vec3
--- @field ColorAdjustedForIntensity vec3
--- @field ColorTemperatureAdjustment vec3
--- @field Distance number
--- @field Earthshine number
--- @field Enabled boolean
--- @field Intensity number
--- @field Kelvin number
--- @field MoonGlare number
--- @field NormalTexResourceGUID FixedString
--- @field Pitch number
--- @field Radius number
--- @field RotationAsVec3 vec3
--- @field TearsAlbedoTexResourceGUID FixedString
--- @field TearsNormalTexResourceGUID FixedString
--- @field TearsRotate number
--- @field TearsScale number
--- @field UseTemperature boolean
--- @field Yaw number


--- @class MoveableObject
--- @field BaseBound AABound
--- @field LocalTransform LocalTransform
--- @field MaxLODDistance number
--- @field MinLODDistance number
--- @field SceneIndex int32
--- @field WorldBound AABound
--- @field WorldTransform Transform
--- @field SetWorldRotate fun(self:MoveableObject, a1:quat)
--- @field SetWorldScale fun(self:MoveableObject, a1:vec3)
--- @field SetWorldTranslate fun(self:MoveableObject, a1:vec3)


--- @class MovementComponent:BaseComponent
--- @field Acceleration number
--- @field Direction vec3
--- @field Speed number
--- @field Speed2 number


--- @class MovementSpeedLimitBoostComponent:BaseComponent
--- @field MovementType uint8


--- @class MusicOriginSound
--- @field OriginOnly FixedString
--- @field OriginTheme FixedString
--- @field OriginThemeAddInstrument boolean


--- @class MusicVolumeEvent
--- @field Bansuri FixedString
--- @field Cello FixedString
--- @field IsStinger boolean
--- @field MusicEvent FixedString
--- @field OriginSound MusicOriginSound
--- @field Tambura FixedString
--- @field TriggerOnEnter boolean
--- @field TriggerOnlyOnce boolean
--- @field Ud FixedString


--- @class MusicVolumeTriggerData:ITriggerData
--- @field Events MusicVolumeEvent[]


--- @class NavigationCloudTriggerData:ITriggerData
--- @field GenerateType int32


--- @class NoesisBaseCommand:NoesisBaseComponent
--- @field CanExecute fun(self:NoesisBaseCommand, a1:NoesisBaseCommand, a2:NoesisBaseComponent?):boolean
--- @field Execute fun(self:NoesisBaseCommand, a1:NoesisBaseCommand, a2:NoesisBaseComponent?)


--- @class NoesisBaseComponent:NoesisBaseRefCounted


--- @class NoesisBaseObject
--- @field Type Noesis::Symbol
--- @field DependencyProperties fun(self:NoesisBaseObject, a1:NoesisBaseObject)
--- @field DirectProperties fun(self:NoesisBaseObject, a1:NoesisBaseObject)
--- @field GetAllProperties fun(self:NoesisBaseObject, a1:NoesisBaseObject)
--- @field GetProperty fun(self:NoesisBaseObject, a1:NoesisBaseObject, a2:FixedString)
--- @field SetProperty fun(self:NoesisBaseObject, a1:NoesisBaseObject, a2:FixedString, a3:AnyRef)
--- @field ToString fun(self:NoesisBaseObject, a1:NoesisBaseObject):string
--- @field TypeInfo fun(self:NoesisBaseObject, a1:NoesisBaseObject):NoesisTypeClass


--- @class NoesisBaseRefCounted:NoesisBaseObject
--- @field NumReferences uint32


--- @class NoesisDependencyData:NoesisTypeMetaData


--- @class NoesisDependencyObject:NoesisDispatcherObject


--- @class NoesisDependencyProperty:NoesisBaseComponent
--- @field IsReadOnly boolean
--- @field Name Noesis::Symbol
--- @field OwnerType NoesisTypeClass
--- @field ValueType NoesisType


--- @class NoesisDispatcherObject:NoesisBaseComponent


--- @class NoesisDoubleTappedEventArgs:NoesisTouchEventArgs


--- @class NoesisDragEventArgs:NoesisRoutedEventArgs
--- @field AllowedEffects uint32
--- @field Data NoesisBaseComponent
--- @field Effects uint32
--- @field KeyStates uint32


--- @class NoesisEventArgs


--- @class NoesisFrameworkElement:NoesisUIElement
--- @field ChildrenCount uint32
--- @field InheritanceParent NoesisFrameworkElement
--- @field Parent NoesisFrameworkElement
--- @field TemplatedParent NoesisFrameworkElement
--- @field Child fun(self:NoesisFrameworkElement, a1:NoesisFrameworkElement, a2:uint32):NoesisBaseComponent
--- @field Find fun(self:NoesisFrameworkElement, a1:NoesisFrameworkElement, a2:string):NoesisBaseComponent
--- @field Resource fun(self:NoesisFrameworkElement, a1:NoesisFrameworkElement, a2:string, a3:boolean?):NoesisBaseComponent
--- @field TreeParent fun(self:NoesisFrameworkElement, a1:NoesisFrameworkElement):NoesisBaseObject


--- @class NoesisGiveFeedbackEventArgs:NoesisRoutedEventArgs
--- @field Effects uint32
--- @field UseDefaultCursors boolean


--- @class NoesisHoldingEventArgs:NoesisTouchEventArgs
--- @field HoldingState NsHoldingState


--- @class NoesisKeyEventArgs:NoesisRoutedEventArgs
--- @field Key NsKey
--- @field KeyStates NsKeyStates
--- @field OriginalKey NsKey


--- @class NoesisKeyboardFocusChangedEventArgs:NoesisRoutedEventArgs
--- @field NewFocus NoesisUIElement
--- @field OldFocus NoesisUIElement


--- @class NoesisManipulationCompletedEventArgs:NoesisRoutedEventArgs


--- @class NoesisManipulationDeltaEventArgs:NoesisRoutedEventArgs


--- @class NoesisManipulationInertiaStartingEventArgs:NoesisRoutedEventArgs


--- @class NoesisManipulationStartedEventArgs:NoesisRoutedEventArgs


--- @class NoesisManipulationStartingEventArgs:NoesisRoutedEventArgs


--- @class NoesisMouseButtonEventArgs:NoesisMouseEventArgs
--- @field ButtonState NsMouseButtonState
--- @field ChangedButton NsMouseButton
--- @field ClickCount uint32


--- @class NoesisMouseEventArgs:NoesisRoutedEventArgs
--- @field LeftButton NsMouseButtonState
--- @field MiddleButton NsMouseButtonState
--- @field Position NoesisPoint
--- @field RightButton NsMouseButtonState
--- @field XButton1Button NsMouseButtonState
--- @field XButton2Button NsMouseButtonState


--- @class NoesisMouseWheelEventArgs:NoesisMouseEventArgs
--- @field Orientation NsOrientation
--- @field WheelRotation int32


--- @class NoesisPanel:NoesisFrameworkElement


--- @class NoesisPoint
--- @field X number
--- @field Y number


--- @class NoesisQueryContinueDragEventArgs:NoesisRoutedEventArgs
--- @field Action NsDragAction
--- @field EscapePressed boolean
--- @field KeyStates uint32


--- @class NoesisQueryCursorEventArgs:NoesisRoutedEventArgs


--- @class NoesisRightTappedEventArgs:NoesisTouchEventArgs


--- @class NoesisRoutedEvent:NoesisBaseComponent
--- @field Name Noesis::Symbol
--- @field OwnerType NoesisTypeClass


--- @class NoesisRoutedEventArgs:NoesisEventArgs
--- @field Handled boolean
--- @field Source NoesisBaseComponent


--- @class NoesisTappedEventArgs:NoesisTouchEventArgs


--- @class NoesisTextCompositionEventArgs:NoesisRoutedEventArgs
--- @field Ch uint32


--- @class NoesisTouchEventArgs:NoesisRoutedEventArgs
--- @field TouchDevice uint64
--- @field TouchPoint NoesisPoint


--- @class NoesisType:NoesisBaseObject
--- @field Name string


--- @class NoesisTypeClass
--- @field Base NoesisTypeClass
--- @field DependencyProperties NoesisDependencyProperty[]
--- @field IsInterface boolean
--- @field Properties NoesisTypeProperty[]
--- @field RoutedEvents NoesisRoutedEvent[]


--- @class NoesisTypeMeta:NoesisType


--- @class NoesisTypeMetaData:NoesisBaseComponent


--- @class NoesisTypeProperty
--- @field ContentType NoesisType
--- @field IsReadOnly boolean
--- @field Meta NoesisTypeMetaData[]
--- @field Name Noesis::Symbol


--- @class NoesisUIElement:NoesisVisual
--- @field Subscribe fun(self:NoesisUIElement, a1:NoesisUIElement, a2:FixedString, a3:FunctionRef):uint64
--- @field Unsubscribe fun(self:NoesisUIElement, a1:NoesisUIElement, a2:uint64):boolean


--- @class NoesisUIElementData:NoesisDependencyData


--- @class NoesisVisual:NoesisDependencyObject
--- @field VisualChildrenCount uint32
--- @field VisualParent NoesisVisual
--- @field VisualChild fun(self:NoesisVisual, a1:NoesisVisual, a2:uint32):NoesisVisual


--- @class NullifyAbilityBoostComponent:BaseComponent
--- @field Ability AbilityId


--- @class ObjectInteractionComponent:BaseComponent
--- @field Interactions EntityHandle[]


--- @class ObjectSizeBoostComponent:BaseComponent
--- @field SizeCategoryAdjustment int32


--- @class ObjectSizeComponent:BaseComponent
--- @field Size int8
--- @field SoundSize int8
--- @field field_1 int8


--- @class ObjectSizeOverrideBoostComponent:BaseComponent
--- @field field_0 uint8


--- @class OffStageComponent:BaseComponent


--- @class OpenCloseActionData:IActionData


--- @class OriginAppearanceTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class OriginComponent:BaseComponent
--- @field Origin FixedString
--- @field field_18 Guid


--- @class OriginPassivesComponent:BaseComponent
--- @field Passives StatsPassivePrototype[]


--- @class OriginTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class OriginalIdentityComponent:BaseComponent
--- @field field_0 uint8


--- @class OriginalTemplateComponent:BaseComponent
--- @field OriginalTemplate FixedString
--- @field TemplateType uint8


--- @class OsirisTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class PartyResourceUpdateRequest
--- @field AddNewResourceType boolean
--- @field Amount number
--- @field Entity EntityHandle
--- @field Level int32
--- @field Party EntityHandle
--- @field Resource Guid


--- @class PassiveComponent:BaseComponent
--- @field Disabled boolean
--- @field Item EntityHandle
--- @field PassiveId FixedString
--- @field Source EntityHandle
--- @field ToggledOn boolean
--- @field Type PassiveSourceType
--- @field field_0 PassiveSourceType
--- @field field_10 EntityHandle
--- @field field_18 boolean
--- @field field_19 boolean
--- @field field_1C uint32
--- @field field_8 EntityHandle


--- @class PassiveContainerComponent:BaseComponent
--- @field Passives EntityHandle[]


--- @class PassiveUsageCount
--- @field field_0 uint16
--- @field field_2 uint16
--- @field field_4 uint16
--- @field field_6 uint16
--- @field field_8 uint16


--- @class PathSettings
--- @field HasSmoothingNodes boolean
--- @field HasSpliningNodes boolean
--- @field HasTurningNodes boolean
--- @field PathClimbingMode uint8
--- @field PathDroppingMode uint8
--- @field Speed Variant<uint8,float>


--- @class PathingComponent:BaseComponent
--- @field Flags uint8
--- @field MovementTiltToRemap FixedString
--- @field PathId int32
--- @field PathMovementSpeed number
--- @field PathParameters table<FixedString, AiPathProbeGroup>
--- @field ServerControl uint8
--- @field VectorParameters table<FixedString, vec4>
--- @field field_20 vec3
--- @field field_30 int64
--- @field field_38 int32
--- @field field_48 int32


--- @class PathingDistanceChangedOneFrameComponent:BaseComponent
--- @field DistanceXZ number
--- @field field_4 number


--- @class PhysicalForceRangeBonusBoostComponent:BaseComponent
--- @field field_0 number
--- @field field_4 uint8


--- @class PhysicsComponent:BaseComponent
--- @field CollidesWith uint32
--- @field ExtraFlags uint32
--- @field Physics PhxPhysicsObject
--- @field PhysicsGroup uint32
--- @field field_14 uint8
--- @field field_15 uint8
--- @field field_16 uint8


--- @class PickingStateComponent:BaseComponent


--- @class PlaySoundActionData:IActionData
--- @field ActivateSoundEvent FixedString
--- @field PlayOnHUD boolean


--- @class PlayerComponent:BaseComponent


--- @class PlayerCustomData
--- @field Initialized boolean
--- @field OwnerProfileID FixedString
--- @field ReservedProfileID FixedString


--- @class PointSoundTriggerData:ITriggerData
--- @field AttenuationScale number
--- @field CustomMixRadius int16
--- @field Dummies vec3[]
--- @field MaxAttenuation int16
--- @field MixdownDelayTime number
--- @field MixdownSpeed uint8
--- @field SoundEvent FixedString
--- @field UseOcclusion boolean


--- @class PointSoundTriggerDummyData:ITriggerData
--- @field ParentTrigger FixedString


--- @class PointTrigger:Trigger


--- @class PortalTrigger:AreaTrigger


--- @class PostProcessCameraSetting
--- @field AlternateDOF DOFSettings
--- @field DOF DOFSettings
--- @field DOFForcedSettingsEnabled boolean
--- @field Exposure ExposureSettings[]
--- @field LensFlareChromaticDistortion number
--- @field LensFlareEnabled boolean
--- @field LensFlareGhostDispersal number
--- @field LensFlareHaloWidth number
--- @field LensFlareIntensity number
--- @field LensFlareTreshold number


--- @class PrimaryMaterialRenderingData:MaterialRenderingData
--- @field field_40 int64
--- @field field_48 int64
--- @field field_50 int64
--- @field field_58 int64


--- @class ProficiencyBonusBoostComponent:BaseComponent
--- @field Ability AbilityId
--- @field Skill SkillId
--- @field Type ProficiencyBonusBoostType


--- @class ProficiencyBonusIncreaseBoostComponent:BaseComponent
--- @field Amount int32


--- @class ProficiencyBonusOverrideBoostComponent:BaseComponent
--- @field Value StatsExpressionParam


--- @class ProficiencyBoostComponent:BaseComponent
--- @field Flags ProficiencyGroupFlags


--- @class ProficiencyComponent:BaseComponent
--- @field Flags ProficiencyGroupFlags
--- @field field_0 ProficiencyGroupFlags


--- @class ProficiencyGroupComponent:BaseComponent
--- @field Flags ProficiencyGroupFlags
--- @field field_0 ProficiencyGroupFlags


--- @class ProjectileDeflectBoostComponent:BaseComponent
--- @field ProjectileTypes ProjectileTypeIds


--- @class ProjectileTemplate:EoCGameObjectTemplate
--- @field Acceleration number
--- @field BeamFX FixedString
--- @field CastBone FixedString
--- @field CurveResourceId FixedString
--- @field DestroyTrailFXOnImpact boolean
--- @field DetachBeam boolean
--- @field DistanceMax_Bezier3 number
--- @field DistanceMax_Bezier4 number
--- @field DistanceMin_Bezier3 number
--- @field DistanceMin_Bezier4 number
--- @field GroundImpactFX string
--- @field IgnoreRoof boolean
--- @field ImpactFX FixedString
--- @field ImpactSoundResourceID FixedString
--- @field InitialSpeed number
--- @field LifeTime number
--- @field NeedsImpactSFX boolean
--- @field OffsetAMax_Bezier3 vec2
--- @field OffsetAMin_Bezier3 vec2
--- @field OffsetBMax_Bezier3 vec2
--- @field OffsetBMin_Bezier3 vec2
--- @field OffsetMax_Bezier3 vec2
--- @field OffsetMin_Bezier3 vec2
--- @field PreviewPathImpactFX FixedString
--- @field PreviewPathMaterial FixedString
--- @field PreviewPathRadius number
--- @field ProjectilePath uint8
--- @field RotateImpact boolean
--- @field RotateMode uint8
--- @field ShiftAMax_Bezier3 number
--- @field ShiftAMin_Bezier3 number
--- @field ShiftBMax_Bezier3 number
--- @field ShiftBMin_Bezier3 number
--- @field ShiftMax_Bezier3 number
--- @field ShiftMin_Bezier3 number
--- @field TrailFX FixedString
--- @field VelocityMode uint8


--- @class RaceComponent:BaseComponent
--- @field Race Guid


--- @class ReceivingCriticalDamageOnHitBoostComponent:BaseComponent
--- @field Arg1 number


--- @class RecipeActionData:IActionData
--- @field Recipe FixedString


--- @class RedirectDamageBoostComponent:BaseComponent
--- @field Amount int32
--- @field DamageType1 DamageType
--- @field DamageType2 DamageType
--- @field RedirectToDamageSource boolean
--- @field field_6 boolean


--- @class ReduceCriticalAttackThresholdBoostComponent:BaseComponent
--- @field Reduction int32
--- @field Status FixedString
--- @field field_0 int32


--- @class RegionTriggerData:ITriggerData
--- @field RegionCameraLock boolean
--- @field RegionCameraLockPos boolean
--- @field RegionMaskRegionInWorld boolean


--- @class RelationComponent:BaseComponent
--- @field field_0 table<uint32, uint8>
--- @field field_100 table<uint32, uint8>
--- @field field_140 Set_uint32
--- @field field_170 Set_uint32
--- @field field_40 table<uint32, uint8>
--- @field field_80 table<uint32, uint8>
--- @field field_C0 table<RelationId, uint8>


--- @class RelationId
--- @field field_0 Guid
--- @field field_10 EntityHandle


--- @class RenderPropertyList
--- @field AlphaChannel uint8
--- @field OpaqueChannel uint8
--- @field RenderPasses uint32
--- @field TransformType uint32
--- @field field_6 uint8
--- @field field_7 uint8


--- @class RenderableObject:MoveableObject
--- @field ActiveMaterial AppliedMaterial
--- @field AppliedMaterials AppliedMaterial[]
--- @field AppliedOverlayMaterials AppliedMaterial[]
--- @field LOD uint8
--- @field MeshRandomData vec4
--- @field PropertyList RenderPropertyList


--- @class ReposeStateComponent:BaseComponent
--- @field field_0 EntityHandle
--- @field field_18 int32
--- @field field_1C vec3
--- @field field_28 uint8
--- @field field_8 Guid


--- @class RequestedRollComponent:BaseComponent
--- @field AC uint8
--- @field Ability AbilityId
--- @field AdditionalValue int32
--- @field AdvantageType AdvantageBoostType
--- @field Canceled boolean
--- @field ConsumedInspirationPoint boolean
--- @field DC uint8
--- @field DialogId int32
--- @field DiceAdditionalValue uint8
--- @field DiscardedDiceTotal uint8
--- @field Entity2Uuid Guid
--- @field EntityUuid Guid
--- @field ExcludedBonusesTags Guid[]
--- @field Finished boolean
--- @field FixedRollBonuses FixedRollBonus[]
--- @field FromScript boolean
--- @field Metadata StatsRollMetadata
--- @field NaturalRoll uint8
--- @field PassiveRollDelay number
--- @field PassiveRollDelay2 number
--- @field RequestStop boolean
--- @field ResolvedRollBonuses ResolvedRollBonus[]
--- @field Result StatsRollResult
--- @field Roll Roll
--- @field RollComponentType uint8
--- @field RollContext uint8
--- @field RollDelayRemaining number
--- @field RollEntity EntityHandle
--- @field RollType uint8
--- @field RollUuid Guid
--- @field RolledComponentType0 boolean
--- @field Roller EntityHandle
--- @field Skill SkillId
--- @field SpellCheck uint8
--- @field StatsExpression string
--- @field Subject EntityHandle
--- @field field_1B0 string
--- @field field_1CC boolean
--- @field field_20 EntityHandle
--- @field field_4E boolean
--- @field field_4F uint8
--- @field field_55 boolean
--- @field field_58 int32
--- @field field_60 EntityHandle
--- @field field_88 boolean
--- @field field_89 uint8
--- @field field_8A uint8
--- @field field_90 Guid[]
--- @field field_A2 boolean


--- @class RerollBoostComponent:BaseComponent
--- @field AlwaysReroll boolean
--- @field RollBelow int8
--- @field RollType StatsRollType
--- @field field_1 int8
--- @field field_2 boolean


--- @class RerollCondition
--- @field KeepNew boolean
--- @field RollValue uint8


--- @class RerollValue
--- @field RerollType uint8
--- @field RollValue uint8


--- @class ResistanceBoostComponent:BaseComponent
--- @field DamageType DamageType
--- @field IsResistantToAll boolean
--- @field ResistanceFlags ResistanceBoostFlags


--- @class ResistancesComponent:BaseComponent
--- @field AC int32
--- @field PerDamageTypeHealthThresholds uint16[]
--- @field PerDamageTypeHealthThresholds2 uint32[]
--- @field Resistances ResistanceBoostFlags[]
--- @field field_E uint8


--- @class ResolvedRollBonus
--- @field Bonus int32
--- @field Description TranslatedString
--- @field DiceSize DiceSizeId
--- @field NumDice uint8
--- @field ResolvedRollBonus int32
--- @field SourceName TranslatedString


--- @class ResourceRollDefinition
--- @field field_0 Guid
--- @field field_10 uint8


--- @class ResourceSetTriggerData:ITriggerData
--- @field Atmosphere FixedString[]
--- @field FadeTime number


--- @class Roll
--- @field Advantage boolean
--- @field Disadvantage boolean
--- @field RerollConditions RerollCondition[]
--- @field Roll RollDefinition
--- @field RollType StatsRollType


--- @class RollBonusBoostComponent:BaseComponent
--- @field Ability AbilityId
--- @field Amount StatsExpressionParam
--- @field RollType StatsRollType
--- @field Skill SkillId


--- @class RollDefinition
--- @field AmountOfDices uint8
--- @field DiceAdditionalValue int32
--- @field DiceNegative boolean
--- @field DiceValue DiceSizeId
--- @field field_8 boolean


--- @class RoomTrigger:AreaTrigger


--- @class RoomTriggerData:ITriggerData
--- @field AttachedPortals FixedString[]
--- @field Border number
--- @field OneShotTrigger boolean
--- @field RenderMethod uint8


--- @class RulesetComponent:BaseComponent
--- @field Modifiers table<Guid, Variant<uint8,int32,float,FixedString,bool>>
--- @field Rulesets Guid[]


--- @class RulesetModifiersComponent:BaseComponent
--- @field Modifiers table<Guid, Variant<uint8,int32,float,FixedString,bool>>


--- @class RuntimeStringHandle
--- @field Handle FixedString
--- @field Version uint16


--- @class SSAOSettings
--- @field Bias number
--- @field DirectLightInfluence number
--- @field Enabled boolean
--- @field Intensity number
--- @field Radius number


--- @class SavantBoostComponent:BaseComponent
--- @field SpellSchool SpellSchoolId


--- @class SavegameComponent:BaseComponent


--- @class ScaleMultiplierBoostComponent:BaseComponent
--- @field Multiplier number


--- @class SceneAttachComponent:BaseComponent


--- @class SceneObjectBounds
--- @field BoundMaxX number[]
--- @field BoundMaxY number[]
--- @field BoundMaxZ number[]
--- @field BoundMinX number[]
--- @field BoundMinY number[]
--- @field BoundMinZ number[]


--- @class SceneObjects
--- @field BoundRadius number[]
--- @field CullFlags uint32[]
--- @field HLODBound SceneObjectBounds
--- @field MaxLODDistanceSquared number[]
--- @field MinLODDistanceSquared number[]
--- @field Objects MoveableObject[]
--- @field RenderPasses uint32[]
--- @field RootBound SceneObjectBounds
--- @field WorldBound SceneObjectBounds
--- @field field_150 boolean[]


--- @class SceneryTemplate:EoCGameObjectTemplate
--- @field AllowCameraMovement boolean
--- @field BlockAoEDamage boolean
--- @field CanClickThrough boolean
--- @field CanClimbOn boolean
--- @field CanShineThrough boolean
--- @field CanShootThrough boolean
--- @field CoverAmount boolean
--- @field HLOD Guid
--- @field IsBlocker boolean
--- @field IsDecorative boolean
--- @field IsPointerBlocker boolean
--- @field LoopSound FixedString
--- @field ReferencedInTimeline boolean
--- @field ShadowPhysicsProxy FixedString
--- @field ShootThroughType uint8
--- @field SoundAttenuation int16
--- @field SoundInitEvent FixedString
--- @field Wadable boolean
--- @field WadableSurfaceType uint8
--- @field WalkOn boolean
--- @field WalkThrough boolean


--- @class ShapeshiftStateComponent:BaseComponent
--- @field BloodSurfaceType uint8?
--- @field BloodType FixedString?
--- @field Template TemplateInfo?
--- @field field_C TemplateInfo?


--- @class ShootThroughTypeComponent:BaseComponent
--- @field Type uint8


--- @class ShowStoryElementUIActionData:IActionData
--- @field UIStoryInstance string
--- @field UIType int32


--- @class SightRangeAdditiveBoostComponent:BaseComponent
--- @field Range number


--- @class SightRangeMaximumBoostComponent:BaseComponent
--- @field Range number


--- @class SightRangeMinimumBoostComponent:BaseComponent
--- @field Range number


--- @class SightRangeOverrideBoostComponent:BaseComponent
--- @field Range number


--- @class SimpleCharacterComponent:BaseComponent


--- @class SitActionData:IActionData
--- @field Heal number


--- @class Skeleton
--- @field BoneCount uint16
--- @field BoneGroups table<FixedString, FixedString[]>
--- @field Bones SkeletonBone
--- @field ClothColliderResourceID FixedString
--- @field DynamicPhysicsResourceID FixedString
--- @field ID FixedString
--- @field IKRigResourceID FixedString
--- @field IsVisible boolean
--- @field MirrorTableResourceID FixedString
--- @field Physics PhxPhysicsSkinnedObject
--- @field RagdollResourceID FixedString
--- @field SkeletonContent SkeletonContent
--- @field Sockets SkeletonSocket[]
--- @field SoftbodyResourceID FixedString
--- @field SoundActivationRanges uint16[]
--- @field SoundBoneIndices uint16[]
--- @field SpringResourceID FixedString
--- @field field_28 FixedString


--- @class SkeletonBone
--- @field Index int16
--- @field Name FixedString


--- @class SkeletonContent
--- @field Name FixedString


--- @class SkeletonSocket
--- @field BoneConstraintIndex int16
--- @field BoneIndex int16
--- @field ConstraintRotationOffset mat3
--- @field ConstraintTranslationOffset vec3
--- @field ConstraintsPos int16
--- @field ConstraintsRot int16
--- @field Name FixedString
--- @field Transform Transform
--- @field UpdatedSocketConstraint int16


--- @class SkillBoostComponent:BaseComponent
--- @field Amount StatsExpressionParam
--- @field Skill SkillId


--- @class SkyLight
--- @field CirrusCloudsAmount number
--- @field CirrusCloudsColor vec3
--- @field CirrusCloudsEnabled boolean
--- @field CirrusCloudsIntensity number
--- @field Color vec3
--- @field ColorAdjustedForIntensity vec3
--- @field ColorTemperatureAdjustment vec3
--- @field Intensity number
--- @field Kelvin number
--- @field PhysicalModel int32
--- @field ProcStarsAmount number
--- @field ProcStarsEnabled boolean
--- @field ProcStarsIntensity number
--- @field ProcStarsSaturation vec2
--- @field ProcStarsShimmer number
--- @field RotateSkydomeEnabled boolean
--- @field ScatteringEnabled boolean
--- @field ScatteringIntensity number
--- @field ScatteringSunColor vec3
--- @field ScatteringSunIntensity number
--- @field SkydomeEnabled boolean
--- @field SkydomeTex FixedString
--- @field UseTemperature boolean


--- @class SomeSharedServerClientObjId
--- @field field_0 int64
--- @field field_8 int32


--- @class SomeSharedServerClientObjId2:SomeSharedServerClientObjId
--- @field field_10 int64
--- @field field_18 int32


--- @class SoundComponent:BaseComponent
--- @field ActiveData SoundComponentActiveData
--- @field Duration number
--- @field Effect FixedString
--- @field Entity EntityHandle
--- @field Flags uint32
--- @field field_1C number


--- @class SoundComponentActiveData
--- @field Entity EntityHandle
--- @field SoundObjectId uint64
--- @field SoundSwitches table<FixedString, FixedString>


--- @class SoundEvent


--- @class SoundExternalPath
--- @field Codec AudioCodec
--- @field Path string


--- @class SoundPostEventRequest
--- @field Event Variant<FixedString,SoundEvent,STDString>
--- @field Path Variant<FixedString,SoundExternalPath>
--- @field PlayerIndex uint8
--- @field Seek number
--- @field Subject Variant<EntityHandle,uint64>
--- @field Type SoundEventType
--- @field field_14 int32


--- @class SoundRTPCSync
--- @field Name FixedString
--- @field Value number
--- @field field_8 int32


--- @class SoundRoutingSystem:BaseSystem
--- @field PostEvent SoundPostEventRequest[]
--- @field Seek SoundSeekRequest[]
--- @field SetRTPC SoundSetRTPCRequest[]
--- @field SetSwitch SoundSetSwitchRequest[]


--- @class SoundSeekRequest
--- @field Entity EntityHandle
--- @field Event Variant<FixedString,SoundEvent,STDString>
--- @field PlayerIndex uint8
--- @field Seek number
--- @field field_38 int32


--- @class SoundSetRTPCRequest
--- @field Entity EntityHandle
--- @field PlayerIndex uint8
--- @field RTPC Variant<FixedString,STDString>
--- @field Reset boolean
--- @field Type SoundEventType
--- @field Value number
--- @field field_36 uint8


--- @class SoundSetSwitchRequest
--- @field Subject Variant<EntityHandle,uint64>
--- @field Switch FixedString
--- @field Value FixedString


--- @class SoundStateSync
--- @field Group FixedString
--- @field State FixedString
--- @field field_8 int32


--- @class SoundVolumeTrigger:AreaTrigger
--- @field RTPCs SoundRTPCSync[]
--- @field States SoundStateSync[]


--- @class SourceAdvantageBoostComponent:BaseComponent
--- @field Type SourceAdvantageType
--- @field field_8 EntityHandle


--- @class SpawnCharacterActionData:IActionData
--- @field LocalTemplate FixedString
--- @field RootTemplate FixedString
--- @field SpawnFX FixedString


--- @class SpeakerComponent:BaseComponent
--- @field field_0 FixedString[]


--- @class SpectatorTriggerData:ITriggerData
--- @field DOFAperature number
--- @field DOFFarOnly number
--- @field DOFFarSharpDistance number
--- @field DOFFocalDistance number
--- @field DOFNearOnly number
--- @field DOFNearSharpDistance number
--- @field FOV number
--- @field Lerp boolean
--- @field Speed number
--- @field UseDOF boolean


--- @class SpellBookActionData:IActionData
--- @field Consume boolean
--- @field Spell FixedString


--- @class SpellFunctorResult
--- @field Attack AttackDesc
--- @field Hit HitDesc
--- @field field_1D0 uint8


--- @class SpellId:SpellMetaId
--- @field Prototype FixedString


--- @class SpellIdWithPrototype:SpellId
--- @field SpellCastSource Guid?
--- @field SpellProto StatsSpellPrototype


--- @class SpellMetaId
--- @field OriginatorPrototype FixedString
--- @field ProgressionSource Guid
--- @field Source Guid
--- @field SourceType SpellSourceType


--- @class SpellResistanceBoostComponent:BaseComponent
--- @field Resistance ResistanceBoostFlags


--- @class SpellSaveDCBoostComponent:BaseComponent
--- @field DC int32


--- @class StandActionData:IActionData


--- @class StandardGameObject
--- @field GameObjectEntity EntityRef
--- @field GameObjectFlags uint16
--- @field Level FixedString


--- @class StartTriggerData:ITriggerData
--- @field Angle number
--- @field Player uint8


--- @class StaticPhysicsComponent:BaseComponent
--- @field Physics PhxPhysicsObject


--- @class StatsAreaTriggerData:ITriggerData
--- @field AreaLevelOverrideGuid Guid


--- @class StatsComponent:BaseComponent
--- @field Abilities int32[]
--- @field AbilityModifiers int32[]
--- @field ArmorType int32
--- @field ArmorType2 int32
--- @field InitiativeBonus int32
--- @field ProficiencyBonus int32
--- @field RangedAttackAbility AbilityId
--- @field Skills int32[]
--- @field SpellCastingAbility AbilityId
--- @field UnarmedAttackAbility AbilityId
--- @field field_8C int32
--- @field field_90 int32


--- @class StatsDamage
--- @field AdditionalDamage int32
--- @field BaseValue uint32
--- @field ConditionRoll StatsExpressionResolved
--- @field DamageMultiplierType uint8
--- @field DamageRolls table<DamageType, StatsRoll[]>
--- @field FinalDamage int32
--- @field FinalDamagePerType table<DamageType, int32>
--- @field Modifiers DamageModifierMetadata[]
--- @field Modifiers2 DamageModifierMetadata[]
--- @field Multiplier uint32
--- @field Resistances DamageResistance[]
--- @field SecondaryValue uint32
--- @field TotalDamage int32
--- @field TotalDamagePerType table<DamageType, int32>
--- @field field_D0 uint32
--- @field field_D4 uint32
--- @field field_D8 uint32
--- @field field_DC uint8


--- @class StatsExpressionParam
--- @field Code string
--- @field Params Variant<StatsExpressionType,Variant<StatsExpressionVariableData,AbilityId,SkillId,StatsStatusGroup,STDString>,StatsExpressionVariableDataType,StatsExpressionVariableDataModifier,RollDefinition,ResourceRollDefinition,StatsContextType,int32,bool>[]


--- @class StatsExpressionParamEx:StatsExpressionParam


--- @class StatsExpressionResolved
--- @field DamageTypeIndex int32
--- @field DamageTypeParams DamageType[]
--- @field IntIndex int32
--- @field IntParams int32[]
--- @field RollIndex int32
--- @field RollParams StatsRoll[]
--- @field StatExpression string


--- @class StatsExpressionWithMetadata:StatsExpressionParam
--- @field Cause BoostSource
--- @field field_28 uint8
--- @field field_48 TranslatedString
--- @field field_58 uint8


--- @class StatsRoll
--- @field Metadata StatsRollMetadata
--- @field Result StatsRollResult
--- @field Roll Roll


--- @class StatsRollMetadata
--- @field AbilityBoosts table<AbilityId, int32>
--- @field AutoAbilityCheckFail boolean
--- @field AutoAbilitySavingThrowFail boolean
--- @field AutoSkillCheckFail boolean
--- @field BaseUnarmedDamage int32
--- @field FixedRollBonuses FixedRollBonus[]
--- @field HasCustomMetadata boolean
--- @field HighGroundBonus int32
--- @field IsCritical boolean
--- @field LowGroundPenalty int32
--- @field ProficiencyBonus int32
--- @field ResolvedRollBonuses ResolvedRollBonus[]
--- @field ResolvedUnknowns FixedRollBonus[]
--- @field RollBonus int32
--- @field SkillBonuses table<SkillId, int32>
--- @field field_10 int32


--- @class StatsRollResult
--- @field Critical RollCritical
--- @field CriticalThreshold uint32?
--- @field DiscardedDiceTotal int32
--- @field NaturalRoll int32
--- @field RollsCount RerollValue[]
--- @field Total int32


--- @class StatusImmunitiesComponent:BaseComponent
--- @field PersonalStatusImmunities table<FixedString, Guid>


--- @class StatusImmunityBoostComponent:BaseComponent
--- @field StatusID FixedString
--- @field UnknownUUIDs Guid[]


--- @class StealthComponent:BaseComponent
--- @field Position vec3
--- @field SeekHiddenFlag boolean
--- @field SeekHiddenTimeout number
--- @field field_14 number
--- @field field_18 int32
--- @field field_1C number
--- @field field_20 number


--- @class SteeringComponent:BaseComponent
--- @field RequestSteering uint8
--- @field Speed number
--- @field TargetRotation number
--- @field field_0 vec3
--- @field field_10 number
--- @field field_14 uint8
--- @field field_18 number
--- @field field_1C uint8
--- @field field_C number


--- @class StoryShortRestDisabledComponent:BaseComponent


--- @class StoryUseActionData:IActionData


--- @class StoryUseInInventoryActionData:IActionData


--- @class SummonContainerComponent:BaseComponent
--- @field ByTag table<FixedString, EntityHandle[]>
--- @field Characters Set_EntityHandle
--- @field Items Set_EntityHandle
--- @field field_18 table<FixedString, EntityHandle[]>


--- @class SummonLifetimeComponent:BaseComponent
--- @field Lifetime Variant<uint8,float>


--- @class SurfacePathInfluence
--- @field Influence int32
--- @field IsCloud boolean
--- @field SurfaceType SurfaceType


--- @class SurfacePathInfluencesComponent:BaseComponent
--- @field PathInfluences SurfacePathInfluence[]


--- @class SurfaceStatusData
--- @field AffectedByRoll boolean
--- @field ApplyToCharacters boolean
--- @field ApplyToItems boolean
--- @field ApplyTypes uint8
--- @field Chance number
--- @field DontTickWhileOnSurface boolean
--- @field Duration number
--- @field Force boolean
--- @field KeepAlive boolean
--- @field OnlyOncePerTurn boolean
--- @field Remove boolean
--- @field StatusId FixedString
--- @field VanishOnApply boolean


--- @class SurfaceTemplate:GameObjectTemplate
--- @field AiPathColor vec3
--- @field AiPathIconFX FixedString
--- @field AiPathVisible boolean
--- @field AlwaysUseDefaultLifeTime boolean
--- @field CanEnterCombat boolean
--- @field CanSeeThrough boolean
--- @field CanShootThrough boolean
--- @field CursorMessage Guid
--- @field DecalMaterial FixedString
--- @field DefaultLifeTime number
--- @field Description TranslatedString
--- @field DisplayName TranslatedString
--- @field EnableLightSpawning boolean
--- @field FX SurfaceVisualData[]
--- @field FadeInSpeed number
--- @field FadeOutSpeed number
--- @field FallDamageMultiplier number
--- @field Icon FixedString
--- @field InstanceVisual SurfaceVisualData[]
--- @field IntroFX SurfaceVisualData[]
--- @field MaterialType uint8
--- @field NormalBlendingFactor number
--- @field ObscuredStateOverride uint8
--- @field OnEnterDistanceOverride number
--- @field OnMoveDistanceOverride number
--- @field RemoveDestroyedItems boolean
--- @field RollConditions string
--- @field Seed int32
--- @field SpawnLightRandomVerticalRangeOffset vec2
--- @field SpawnLightTemplateID FixedString
--- @field Statuses SurfaceStatusData[]
--- @field Summon FixedString
--- @field SurfaceCategory uint8
--- @field SurfaceGrowTimer number
--- @field SurfaceName FixedString
--- @field SurfaceType SurfaceType
--- @field field_158 uint32


--- @class SurfaceVisualData
--- @field GridSize number
--- @field Height vec2
--- @field RandomPlacement int32
--- @field Rotation ivec2
--- @field Scale vec2
--- @field SpawnCell int32
--- @field SurfaceNeeded int32
--- @field SurfaceRadiusMax int32
--- @field Visual FixedString


--- @class TagComponent:BaseComponent
--- @field Tags Guid[]


--- @class TeleportActionData:IActionData
--- @field AiUseInCombat boolean
--- @field BlockMapMarkerNavigation boolean
--- @field EventID FixedString
--- @field SnapToGround boolean
--- @field Source FixedString
--- @field SourceType uint8
--- @field Target FixedString
--- @field TargetType uint8
--- @field Visibility uint8


--- @class TeleportTriggerData:ITriggerData
--- @field Angle number
--- @field Zoom boolean


--- @class TemplateAnimationSetOverrideComponent:BaseComponent
--- @field Overrides AnimationWaterfallElement[]


--- @class TemplateInfo
--- @field TemplateId FixedString
--- @field TemplateType uint8


--- @class TemplateTagContainer
--- @field DirectTags Set_Guid
--- @field Tags Set_Guid


--- @class TemporaryHPBoostComponent:BaseComponent
--- @field HP StatsExpressionParam


--- @class TextKeyAttachTypeProperties:TextKeyTypeProperties
--- @field AttachBone FixedString
--- @field Bone FixedString
--- @field Detach boolean


--- @class TextKeyAttackTypeProperties:TextKeyTypeProperties


--- @class TextKeyEffectTypeProperties:TextKeyTypeProperties
--- @field Bone FixedString
--- @field BoneId int16
--- @field Effect FixedString
--- @field HardAttach boolean
--- @field InterruptLoopAtEnd boolean
--- @field field_10 FixedString


--- @class TextKeyEvent
--- @field ID FixedString
--- @field Length number
--- @field Properties TextKeyTypeProperties
--- @field Time number
--- @field Track uint8


--- @class TextKeyFloatDataTypeProperties:TextKeyTypeProperties
--- @field Data number


--- @class TextKeyFoleyTypeProperties:TextKeySoundTypeProperties
--- @field FoleyIntensity uint8
--- @field FoleyType uint8


--- @class TextKeyFootMoveTypeProperties:TextKeyTypeProperties
--- @field Data uint8


--- @class TextKeyFootStepTypeProperties:TextKeyTypeProperties
--- @field FootID uint8
--- @field PlayHearingFX boolean
--- @field PlayScuff boolean
--- @field PlaySlideFX boolean
--- @field PlaySound boolean
--- @field PlayStepFX boolean
--- @field Slide boolean


--- @class TextKeyGenomeTypeProperties:TextKeyTypeProperties


--- @class TextKeyRagdollTypeProperties:TextKeyTypeProperties
--- @field AngularVelocityModifier number
--- @field AnimationResourceID FixedString
--- @field DampingMultiplier number
--- @field DurationHardLimit number
--- @field DurationSoftLimit number
--- @field HitImpulseThreshold number
--- @field HoldLastFrame boolean
--- @field LinearVelocityModifier number
--- @field LockLimits boolean
--- @field StiffnessMultiplier number


--- @class TextKeyReactTypeProperties:TextKeyTypeProperties


--- @class TextKeySoundTypeProperties:TextKeyTypeProperties
--- @field Resource FixedString
--- @field SoundObjectIndex uint8


--- @class TextKeyTypeProperties


--- @class TextKeyVisualCullFlagTypeProperties:TextKeyTypeProperties
--- @field Enabled boolean
--- @field VisualFlag uint32


--- @class TextKeyVocalTypeProperties:TextKeyTypeProperties
--- @field VocalType uint8


--- @class TextKeyWeaponEffectTypeProperties:TextKeyTypeProperties
--- @field EffectType uint32
--- @field Weapon uint32


--- @class TextLine:TranslatedString
--- @field field_10 boolean


--- @class TextureLayerConfigId
--- @field Flag uint8
--- @field TextureLayerConfig uint32


--- @class ThrowActionData:IActionData
--- @field FallbackPreviewRadius number


--- @class TileSetConstructionLine
--- @field End TileSetConstructionPoint
--- @field InstanceId Guid
--- @field Points TileSetConstructionPoint[]
--- @field Start TileSetConstructionPoint


--- @class TileSetConstructionNeighbourPoint
--- @field NeighbourLists table<uint8, TileSetNeighbourPointTileList>
--- @field NeighbourPoints Guid[]


--- @class TileSetConstructionPoint
--- @field ConstructionPointStop boolean
--- @field Lines TileSetConstructionLine[]
--- @field NeighbourPoints TileSetConstructionNeighbourPoint[]
--- @field Point ConstructionPoint
--- @field byte40 uint8
--- @field byte41 uint8


--- @class TileSetNeighbourPointTileList
--- @field LeftCornerTiles ConstructionTileTemplate[]
--- @field NonOptimalTilesEnd uint32
--- @field NonOptimalTilesStart uint32
--- @field RightCornerTiles ConstructionTileTemplate[]
--- @field StraightTiles ConstructionTileTemplate[]


--- @class TimeFactorComponent:BaseComponent
--- @field Value number


--- @class TimelineActorDataComponent:BaseComponent
--- @field field_0 Guid
--- @field field_10 Guid
--- @field field_20 uint16
--- @field field_22 uint8
--- @field field_23 uint8


--- @class TimelineReferencedTemplate
--- @field ReferencedInTimeline boolean
--- @field Timelines Set_Guid


--- @class TimelineSceneTrigger:AreaTrigger
--- @field DestroyAfterTimeline boolean
--- @field DialogueResourceIds Set_Guid
--- @field FloodSuccess boolean
--- @field IsWorldTimeline boolean
--- @field Optional boolean
--- @field ShouldPreloadWorldTimelineActors boolean
--- @field TriggerWorldTimelineOnSkillPrepare boolean
--- @field UseSceneBounds boolean


--- @class TimelineSceneTriggerData:ITriggerData
--- @field IsUsingCalculatedSceneBounds boolean
--- @field IsWorldTimeline boolean
--- @field Optional boolean
--- @field ShouldPreloadWorldTimelineActors boolean
--- @field Timelines Set_Guid
--- @field TriggerWorldTimelineOnSkillPrepare boolean


--- @class TraderComponent:BaseComponent


--- @class Transform
--- @field RotationQuat quat
--- @field Scale vec3
--- @field Translate vec3


--- @class TransformComponent:BaseComponent
--- @field Transform Transform


--- @class TranslatedFSString
--- @field Handle RuntimeStringHandle
--- @field Get fun(self:TranslatedFSString):StringView?


--- @class TranslatedString
--- @field ArgumentString RuntimeStringHandle
--- @field Handle RuntimeStringHandle
--- @field Get fun(self:TranslatedString):StringView?


--- @class Trigger
--- @field CustomPointTransform Transform
--- @field Flags uint16
--- @field HasCustomPoint boolean
--- @field IsGlobal boolean
--- @field Level FixedString
--- @field PreviousLevel FixedString
--- @field SyncFlags uint16
--- @field Template TriggerTemplate
--- @field TriggerEntity EntityRef
--- @field TriggerType FixedString


--- @class TriggerPhysicsBox:ITriggerPhysicsData
--- @field Extents vec3


--- @class TriggerPhysicsPoly:ITriggerPhysicsData
--- @field Height number
--- @field Points vec2[]


--- @class TriggerPhysicsSphere:ITriggerPhysicsData
--- @field Radius number


--- @class TriggerTemplate:GameObjectTemplate
--- @field Color avec4
--- @field ConstellationConfigName FixedString
--- @field CustomPointTransform Transform
--- @field EventSendingMode uint8
--- @field FadeChildren FixedString[]
--- @field FadeGroup FixedString
--- @field FadeGroupOnly boolean
--- @field Fadeable boolean
--- @field GizmoColorOverride avec4
--- @field HasCustomPoint boolean
--- @field HierarchyOnlyFade boolean
--- @field IgnoreRaycast boolean
--- @field OnlyCharacterEvents boolean
--- @field PhysicsType uint8
--- @field TriggerGizmoOverride FixedString
--- @field TriggerType FixedString
--- @field UsingGizmoColorOverride boolean


--- @class TypeInformation
--- @field ComponentName FixedString
--- @field ElementType TypeInformationRef
--- @field EnumValues table<FixedString, uint64>
--- @field HasWildcardProperties boolean
--- @field IsBitfield boolean
--- @field IsBuiltin boolean
--- @field KeyType TypeInformationRef
--- @field Kind LuaTypeId
--- @field Members table<FixedString, TypeInformationRef>
--- @field Methods table<FixedString, TypeInformation>
--- @field ModuleRole FixedString
--- @field NativeName FixedString
--- @field Params TypeInformationRef[]
--- @field ParentType TypeInformationRef
--- @field ReturnValues TypeInformationRef[]
--- @field SystemName FixedString
--- @field TypeName FixedString
--- @field VarargParams boolean
--- @field VarargsReturn boolean


--- @class TypeInformationRef:TypeInformation


--- @class UGCModDependency
--- @field DependencyHandle uint64
--- @field DependencyName string


--- @class Unknown31ActionData:IActionData


--- @class Unknown34ActionData:IActionData


--- @class Unknown35ActionData:IActionData


--- @class UnlockInterruptBoostComponent:BaseComponent
--- @field Interrupt FixedString


--- @class UnlockSpellBoostComponent:BaseComponent
--- @field Ability AbilityId
--- @field CastResource Guid
--- @field CooldownType SpellCooldownType
--- @field LearningStrategy SpellLearningStrategy
--- @field SomeUUID Guid
--- @field SpellChildSelection SpellLearningStrategy
--- @field SpellId FixedString


--- @class UnlockSpellVariantBoostComponent:BaseComponent
--- @field Modifications Variant<SpellModificationModifyAreaRadius,SpellModificationModifyMaximumTargets,SpellModificationModifyNumberOfTargets,SpellModificationModifySavingThrowDisadvantage,SpellModificationModifySpellFlags,SpellModificationModifySpellRoll,SpellModificationModifyStatusDuration,SpellModificationModifySummonDuration,SpellModificationModifySurfaceDuration,SpellModificationModifyTargetRadius,SpellModificationModifyUseCosts,SpellModificationModifyVisuals,SpellModificationModifyIconGlow,SpellModificationModifyTooltipDescription>[]
--- @field Spell string


--- @class UnsheathStateComponent:BaseComponent
--- @field MainHandWeapon EntityHandle
--- @field OffHandWeapon EntityHandle
--- @field State StatsUnsheathState
--- @field field_0 EntityHandle
--- @field field_10 int32
--- @field field_14 StatsUnsheathState
--- @field field_18 uint8
--- @field field_19 uint8
--- @field field_1A uint8
--- @field field_8 EntityHandle


--- @class UsageCountComponent:BaseComponent
--- @field Passives table<FixedString, PassiveUsageCount>


--- @class UseBoostsComponent:BaseComponent
--- @field Boosts BoostDescription[]


--- @class UseComponent:BaseComponent
--- @field Boosts BoostDescription[]
--- @field BoostsOnEquipMainHand BoostDescription[]
--- @field BoostsOnEquipOffHand BoostDescription[]
--- @field CanCombine uint8
--- @field Charges int32
--- @field CombineFlag uint8
--- @field ItemUseBlocked uint8
--- @field ItemUseType ItemUseType
--- @field MaxCharges int32
--- @field Requirements StatsRequirement[]
--- @field field_19 uint8
--- @field field_1A uint8
--- @field field_1B uint8


--- @class UseSocket
--- @field field_0 int32
--- @field field_30 number[]
--- @field field_4 uint8
--- @field field_5 uint8
--- @field field_58 FixedString
--- @field field_5C FixedString
--- @field field_60 EntityHandle
--- @field field_8 number[]


--- @class UseSocketComponent:BaseComponent
--- @field Sockets table<uint32, UseSocket>


--- @class UseSpellActionData:IActionData
--- @field Class Guid
--- @field Consume boolean
--- @field Spell FixedString


--- @class UserAvatarComponent:BaseComponent
--- @field OwnerProfileID FixedString
--- @field UserID int32
--- @field field_4 FixedString
--- @field field_8 uint8


--- @class UserReservedForComponent:BaseComponent
--- @field UserID int32


--- @class UuidComponent:BaseComponent
--- @field EntityUuid Guid


--- @class UuidToHandleMappingComponent:BaseComponent
--- @field Mappings table<Guid, EntityHandle>


--- @class ValueComponent:BaseComponent
--- @field Rarity uint8
--- @field Unique boolean
--- @field Value int32


--- @class Visual:MoveableObject
--- @field Attachments VisualAttachment[]
--- @field BlendShapeBase int16
--- @field CullFlags VisualCullFlags
--- @field HasAnimatedObjects boolean
--- @field HasValidPose boolean
--- @field LODDistances number[]
--- @field LightChannel uint8
--- @field ObjectDescs VisualObjectDesc[]
--- @field Parent Visual
--- @field PhysicsFlags uint8
--- @field SkeletonSlots VisualSkeletonSlot[]
--- @field VisualEntity EntityRef
--- @field VisualFlags VisualFlags
--- @field VisualResource ResourceVisualResource
--- @field Wrinkle01BoneIndex int16
--- @field Wrinkle02BoneIndex int16
--- @field Wrinkle03BoneIndex int16
--- @field WrinkleBaseBoneIndex int16


--- @class VisualAttachment
--- @field Bone1 FixedString
--- @field Bone2 FixedString
--- @field BoneIndex1 int32
--- @field BoneIndex2 int32
--- @field BoneType1 uint8
--- @field BoneType2 uint8
--- @field Flags VisualAttachmentFlags
--- @field Visual Visual
--- @field field_20 FixedString
--- @field field_24 vec3


--- @class VisualObjectDesc
--- @field Flags VisualObjectType
--- @field LOD uint8
--- @field Renderable RenderableObject
--- @field field_8 uint8


--- @class VisualSkeletonSlot
--- @field RemapperSlot FixedString
--- @field Remaps VisualSkeletonSlotRemap[]
--- @field Skeleton Skeleton
--- @field field_4 int32


--- @class VisualSkeletonSlotRemap
--- @field MapFrom int16
--- @field MapTo int16


--- @class VisualComponent:BaseComponent
--- @field NotClustered boolean
--- @field Visual Visual
--- @field field_8 uint8
--- @field field_9 uint8


--- @class VoiceComponent:BaseComponent
--- @field Voice Guid


--- @class VoiceTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class VolumetricCloudSettings
--- @field AmbientLightFactor number
--- @field BackwardScatteringG number
--- @field BaseColor vec3
--- @field BottomFade number
--- @field ConeRadius number
--- @field CoverageEndDistance number
--- @field CoverageSettings CoverageSettings
--- @field CoverageStartDistance number
--- @field CoverageWindSpeed number
--- @field DarkOutline number
--- @field Density number
--- @field DetailScale number
--- @field Distortion number
--- @field DistortionScale number
--- @field Enabled boolean
--- @field ErosionEdgeSize number
--- @field ForwardScatteringG number
--- @field HorizonFade number
--- @field Intensity number
--- @field LODDistance number
--- @field RainCoverageMaxInfluence number
--- @field SampleScalar number
--- @field SampleThreshold number
--- @field ShadowFactor number
--- @field SunLightFactor number
--- @field SunRayLength number
--- @field TopColor vec3


--- @class WeaponAttackRollAbilityOverrideBoostComponent:BaseComponent
--- @field Ability AbilityId


--- @class WeaponAttackRollBonusBoostComponent:BaseComponent
--- @field Amount StatsExpressionParam


--- @class WeaponAttackTypeOverrideBoostComponent:BaseComponent
--- @field AttackType SpellAttackType


--- @class WeaponComponent:BaseComponent
--- @field Ability AbilityId
--- @field DamageDice DiceSizeId
--- @field DamageRange number
--- @field DamageValues StatsExpressionWithMetadata[]
--- @field Rolls table<AbilityId, RollDefinition[]>
--- @field Rolls2 table<AbilityId, RollDefinition[]>
--- @field VersatileDamageDice DiceSizeId
--- @field WeaponFunctors StatsFunctors
--- @field WeaponGroup uint8
--- @field WeaponProperties uint32
--- @field WeaponRange number
--- @field field_38 StatsExpressionWithMetadata[]


--- @class WeaponDamageBoostComponent:BaseComponent
--- @field Amount StatsExpressionParam
--- @field DamageType DamageType
--- @field field_30 boolean


--- @class WeaponDamageDieOverrideBoostComponent:BaseComponent
--- @field Roll RollDefinition


--- @class WeaponDamageResistanceBoostComponent:BaseComponent
--- @field DamageTypes DamageType[]


--- @class WeaponDamageTypeOverrideBoostComponent:BaseComponent
--- @field DamageType DamageType


--- @class WeaponEnchantmentBoostComponent:BaseComponent
--- @field Value int32


--- @class WeaponPropertyBoostComponent:BaseComponent
--- @field Properties WeaponFlags


--- @class WeaponSetComponent:BaseComponent
--- @field WeaponSet WeaponSetType


--- @class WeightBoostComponent:BaseComponent
--- @field Amount int32


--- @class WeightCategoryBoostComponent:BaseComponent
--- @field Amount int32


--- @class WieldingComponent:BaseComponent
--- @field Owner EntityHandle


--- @class ActiveRollConsumableModifier
--- @field Item EntityHandle
--- @field ModifierGuid Guid
--- @field Modifiers ActiveRollModifier[]
--- @field Source EntityHandle


--- @class ActiveRollDynamicModifierId
--- @field ModifierGuid Guid
--- @field Type DynamicModifierType


--- @class ActiveRollInProgressComponent:BaseComponent


--- @class ActiveRollItemSpellModifier
--- @field Concentration boolean
--- @field Item EntityHandle
--- @field ModifierGuid Guid
--- @field Modifiers ActiveRollModifier[]
--- @field Source EntityHandle
--- @field Spell SpellId
--- @field TargetType uint8


--- @class ActiveRollModifier
--- @field AbilityModifier int32
--- @field AbilityModifier2 int32
--- @field Advantage uint8
--- @field AdvantageReason TranslatedString
--- @field AdvantageType AdvantageBoostType
--- @field AmountOfDices uint8
--- @field BoostType BoostType
--- @field CriticalHitType RollCritical
--- @field DiceValue DiceSizeId
--- @field Source ActiveRollModifierSourceInfo
--- @field SuccessFlags CriticalHitSuccessFlags
--- @field TargetType uint8
--- @field TotalValue int32
--- @field Value int32
--- @field field_28 uint8
--- @field field_2C TranslatedString
--- @field field_40 Guid
--- @field field_F RollCritical


--- @class ActiveRollModifierSourceInfo
--- @field Cause FixedString
--- @field Equipment EntityHandle
--- @field Source EntityHandle
--- @field SourceType BoostSourceType
--- @field StackId FixedString
--- @field StackPriority int32
--- @field Status ComponentHandle
--- @field field_10 EntityHandle
--- @field field_18 ComponentHandle
--- @field field_20 uint8
--- @field field_28 EntityHandle


--- @class ActiveRollModifiersComponent:BaseComponent
--- @field ConsumableModifiers ActiveRollConsumableModifier[]
--- @field DynamicModifiers ActiveRollDynamicModifierId[]
--- @field DynamicModifiers2 ActiveRollDynamicModifierId[]
--- @field DynamicModifiers3 ActiveRollDynamicModifierId[]
--- @field Entity EntityHandle
--- @field ItemSpellModifiers ActiveRollItemSpellModifier[]
--- @field SpellModifiers ActiveRollSpellModifier[]
--- @field StaticModifiers ActiveRollStaticModifier[]
--- @field ToggledPassiveModifiers ActiveRollToggledPassiveModifier[]
--- @field field_0 EntityHandle
--- @field field_18 uint8
--- @field field_19 uint8


--- @class ActiveRollSpellModifier
--- @field Concentration boolean
--- @field ModifierGuid Guid
--- @field Modifiers ActiveRollModifier[]
--- @field Source EntityHandle
--- @field Spell SpellId
--- @field TargetType uint8


--- @class ActiveRollStaticModifier
--- @field Disabled boolean
--- @field Modifier ActiveRollModifier
--- @field ModifierGuid Guid
--- @field field_10 boolean
--- @field field_88 uint8


--- @class ActiveRollToggledPassiveModifier
--- @field ModifierGuid Guid
--- @field Modifiers ActiveRollModifier[]
--- @field Passive FixedString
--- @field PassiveEntity EntityHandle
--- @field Source EntityHandle
--- @field field_14 int32


--- @class AnimationGameplayEventsOneFrameComponent:BaseComponent
--- @field Events table<EntityHandle, AnimationReceivedEvent[]>


--- @class AnimationGameplayEventsSingletonComponent:BaseComponent
--- @field Events AnimationReceivedGameplayEvent[]


--- @class AnimationReceivedEvent
--- @field Args GnGenomeVariant[]
--- @field Event FixedString


--- @class AnimationTextKeyEventInfo
--- @field TextKey FixedString
--- @field field_4 uint8
--- @field field_8 AnimationTextKeyEventValue?


--- @class AnimationTextKeyEventValue
--- @field field_0 FixedString
--- @field field_4 uint8


--- @class AnimationTextKeyEventsOneFrameComponent:BaseComponent
--- @field Events table<EntityHandle, AnimationTextKeyEventInfo[]>


--- @class AnimationTextKeyEventsSingletonComponent:BaseComponent
--- @field Events AnimationReceivedTextKeyEvent[]


--- @class AnimationTriggeredEventsOneFrameComponent:BaseComponent
--- @field Events table<EntityHandle, FixedString[]>


--- @class AspkBaseProperty:AspkProperty


--- @class AspkBillboardComponent:AspkFxBaseComponent
--- @field AlphaProperty AspkFloatKeyFrameProperty
--- @field BillboardRenderable RenderableObject
--- @field Brightness number
--- @field BrightnessProperty AspkFloatKeyFrameProperty
--- @field Color avec4
--- @field ColorProperty AspkColorARGBKeyFrameProperty
--- @field CustomModules AspkCustomModules
--- @field DynamicValueProperty AspkFloatKeyFrameProperty
--- @field Facing uint8
--- @field KeyframedOffsetProperty AspkFloatKeyFrameProperty
--- @field KeyframedPosition vec3
--- @field MaterialApplied boolean
--- @field ModifierWProperty AspkFloatProperty
--- @field ModifierXProperty AspkFloatProperty
--- @field ModifierYProperty AspkFloatProperty
--- @field ModifierZProperty AspkFloatProperty
--- @field OffsetProperty AspkVector3Property
--- @field OrientationProperty AspkFloatKeyFrameProperty
--- @field Position vec3
--- @field PropertyList RenderPropertyList
--- @field SizeProperty AspkFloatKeyFrameProperty
--- @field SnapToBottom number
--- @field SnapToGround boolean
--- @field SnapToTop number
--- @field TextProperty AspkStringProperty
--- @field Transform Transform
--- @field UniformScale number
--- @field UniformScaleProperty AspkFloatProperty
--- @field field_178 vec3
--- @field field_184 vec3


--- @class AspkBooleanProperty:AspkProperty
--- @field Value boolean


--- @class AspkBoundingBoxComponent:AspkFxBaseComponent
--- @field Center vec3
--- @field Position vec3
--- @field RequiredSize vec3
--- @field Size vec3


--- @class AspkBoundingSphereComponent:AspkFxBaseComponent
--- @field Center vec3
--- @field Position vec3
--- @field Radius number
--- @field RequiredRadius number


--- @class AspkCameraShakeComponent:AspkFxBaseComponent
--- @field FalloffStartEndProperty AspkFloatRangeProperty
--- @field Flags uint8
--- @field ShakeFrequency number
--- @field ShakeStrength number
--- @field ShakeStrengthProperty AspkFloatKeyFrameProperty
--- @field ShakeVibrationFrequencyProperty AspkFloatKeyFrameProperty
--- @field UpdateTime number
--- @field field_B8 uint64
--- @field field_C8 number
--- @field field_CC number


--- @class AspkColorARGBKeyFrameData
--- @field Color vec4
--- @field Time number


--- @class AspkColorARGBKeyFrameProperty:AspkProperty
--- @field Frames AspkColorARGBKeyFrameData[]


--- @class AspkComponent
--- @field EndTime number
--- @field ID Guid
--- @field IsActive boolean
--- @field StartTime number
--- @field StayAlive boolean
--- @field TrackGroupIndex uint32
--- @field TypeName FixedString


--- @class AspkCubicFloatKeyFrame:AspkFloatKeyFrame
--- @field A number
--- @field B number
--- @field C number
--- @field D number
--- @field Time number


--- @class AspkCubicFloatKeyFrameData:AspkKeyFrameData
--- @field Frames AspkCubicFloatKeyFrame[]


--- @class AspkCustomModules


--- @class AspkDecalComponent:AspkFxBaseComponent
--- @field AlphaProperty AspkFloatKeyFrameProperty
--- @field Axis mat3
--- @field Brightness number
--- @field BrightnessProperty AspkFloatKeyFrameProperty
--- @field Color avec4
--- @field ColorProperty AspkColorARGBKeyFrameProperty
--- @field CoordinateSpace uint8
--- @field CustomModules AspkCustomModules
--- @field DecalEntity EntityHandle
--- @field DimensionsProperty AspkFloatKeyFrameProperty
--- @field Distance number
--- @field DynamicValueProperty AspkFloatKeyFrameProperty
--- @field IgnoreParentRotation boolean
--- @field KeyframedPosition vec3
--- @field Material Material
--- @field ModifierWProperty AspkFloatProperty
--- @field ModifierXProperty AspkFloatProperty
--- @field ModifierYProperty AspkFloatProperty
--- @field ModifierZProperty AspkFloatProperty
--- @field NormalBlendingFactor number
--- @field Offset vec3
--- @field OffsetProperty AspkFloatKeyFrameProperty
--- @field Position vec3
--- @field ProjectedOffsetProperty AspkFloatKeyFrameProperty
--- @field RotationProperty AspkFloatKeyFrameProperty
--- @field UniformScale number
--- @field UniformScaleProperty AspkFloatProperty


--- @class AspkDeflectorComponent:AspkFxBaseComponent
--- @field Normal vec3
--- @field Position vec3
--- @field PositionModifierXProperty AspkFloatProperty
--- @field PositionModifierYProperty AspkFloatProperty
--- @field PositionModifierZProperty AspkFloatProperty
--- @field Visible boolean


--- @class AspkDragForceComponent:AspkFxBaseComponent
--- @field DampingProperty AspkFloatKeyFrameProperty
--- @field InfluenceRadius number
--- @field InfluenceRadiusApplied boolean
--- @field InfluenceRadiusProperty AspkFloatKeyFrameProperty
--- @field KeyframedOffsetProperty AspkFloatKeyFrameProperty
--- @field KeyframedPosition vec3
--- @field Offset vec3
--- @field Position vec3
--- @field field_E4 vec3


--- @class AspkEffectTimeline:AspkTimeline


--- @class AspkFixedFunction


--- @class AspkFixedFunction0:AspkFixedFunction
--- @field A number
--- @field B number


--- @class AspkFixedFunction1:AspkFixedFunction
--- @field A number
--- @field B number
--- @field C number


--- @class AspkFixedFunction2:AspkFixedFunction
--- @field A number
--- @field B number
--- @field C number
--- @field D number


--- @class AspkFixedFunctionProperty:AspkProperty
--- @field Function AspkFixedFunction


--- @class AspkFixedStringProperty:AspkProperty
--- @field Value FixedString


--- @class AspkFloatKeyFrame


--- @class AspkFloatKeyFrameProperty:AspkProperty
--- @field KeyFrames AspkKeyFrameData[]


--- @class AspkFloatProperty:AspkProperty
--- @field Value number


--- @class AspkFloatRangeProperty:AspkProperty
--- @field Max number
--- @field Min number


--- @class AspkFxBaseComponent:AspkComponent
--- @field Entity EntityRef
--- @field FxType uint32
--- @field LastUpdate number
--- @field LastUpdateTimeStep number
--- @field Modules AspkModule[]
--- @field Properties table<FixedString, AspkProperty>
--- @field TimeStep number
--- @field TimeStepEdge0 number
--- @field TimeStepEdge1 number


--- @class AspkFxParticleData
--- @field Ages number[]
--- @field AppliedForces vec3[]
--- @field Count uint32
--- @field DiedParticles uint32[]
--- @field DiscardValues number[]
--- @field FixedColors uint32[]
--- @field FixedScalars uint16[]
--- @field Flags uint32
--- @field FlipbookImageIndices uint16[]
--- @field InitialRotationSpeeds uint16[]
--- @field Lifespan number[]
--- @field Normals AspkPackedUnitVector[]
--- @field Positions vec3[]
--- @field Rotations uint16[]
--- @field UnitAges number[]
--- @field UpVectors AspkPackedUnitVector[]
--- @field Velocities vec3[]
--- @field field_88 EntityHandle[]


--- @class AspkGravityForceComponent:AspkFxBaseComponent
--- @field AccelerationProperty AspkFloatKeyFrameProperty
--- @field Direction vec3
--- @field InfluenceRadius number
--- @field InfluenceRadiusApplied boolean
--- @field InfluenceRadiusProperty AspkFloatKeyFrameProperty
--- @field KeyframedOffsetProperty AspkFloatKeyFrameProperty
--- @field KeyframedPosition vec3
--- @field Offset vec3
--- @field Position vec3


--- @class AspkInput
--- @field Input AspkProperty
--- @field Name FixedString


--- @class AspkIntegerProperty:AspkProperty
--- @field Value int32


--- @class AspkIntegerRangeProperty:AspkProperty
--- @field Max int32
--- @field Min int32


--- @class AspkKeyFrameData


--- @class AspkLightComponent:AspkFxBaseComponent
--- @field AnimateManually boolean
--- @field CalculatedPosition vec3
--- @field ColorProperty AspkColorARGBKeyFrameProperty
--- @field CoordinateSpace uint8
--- @field FlickerAmountProperty AspkFloatKeyFrameProperty
--- @field FlickerSpeedProperty AspkFloatKeyFrameProperty
--- @field IgnoreParentRotation boolean
--- @field InitialRotationProperty AspkFloatKeyFrameProperty
--- @field IntensityProperty AspkFloatKeyFrameProperty
--- @field KeyframedPosition vec3
--- @field LastPosition vec3
--- @field LightEntity EntityHandle
--- @field ModulateLightTemplateRadius boolean
--- @field Offset vec3
--- @field OverrideLightTemplateColor boolean
--- @field OverrideLightTemplateFlickerSpeed boolean
--- @field Position vec3
--- @field PositionProperty AspkFloatKeyFrameProperty
--- @field RadiusProperty AspkFloatKeyFrameProperty
--- @field Rotation quat
--- @field RotationLifeProperty AspkFloatKeyFrameProperty
--- @field RotationProperty AspkFloatKeyFrameProperty
--- @field SnapToBottom number
--- @field SnapToGround boolean
--- @field SnapToTop number
--- @field WasActive boolean
--- @field field_120 vec3
--- @field field_15C vec3
--- @field field_168 vec3
--- @field field_18A boolean


--- @class AspkLinearFloatKeyFrame:AspkFloatKeyFrame
--- @field Time number
--- @field Value number


--- @class AspkLinearFloatKeyFrameData:AspkKeyFrameData
--- @field Frames AspkLinearFloatKeyFrame[]


--- @class AspkModelComponent:AspkFxBaseComponent
--- @field Active boolean
--- @field Alpha number
--- @field Animation FixedString
--- @field AnimationSpeedProperty AspkFloatProperty
--- @field AxisScale vec3
--- @field AxisScaleProperty AspkFloatKeyFrameProperty
--- @field Brightness number
--- @field BrightnessProperty AspkFloatKeyFrameProperty
--- @field Color avec4
--- @field ColorProperty AspkColorARGBKeyFrameProperty
--- @field CustomModules AspkCustomModules
--- @field DynamicValueProperty AspkFloatKeyFrameProperty
--- @field FadeOutTime number
--- @field HasVisual boolean
--- @field InitialRotationProperty AspkFloatKeyFrameProperty
--- @field KeyframedOffsetProperty AspkFloatKeyFrameProperty
--- @field KeyframedPosition vec3
--- @field KeyframedPositionModifierXProperty AspkFloatProperty
--- @field KeyframedPositionModifierYProperty AspkFloatProperty
--- @field KeyframedPositionModifierZProperty AspkFloatProperty
--- @field LoopAnimation boolean
--- @field ModelAlphaProperty AspkFloatKeyFrameProperty
--- @field ModelModifierWProperty AspkFloatProperty
--- @field ModelModifierXProperty AspkFloatProperty
--- @field ModelModifierYProperty AspkFloatProperty
--- @field ModelModifierZProperty AspkFloatProperty
--- @field Offset vec3
--- @field PivotRotation quat
--- @field Position vec3
--- @field PostRotation quat
--- @field Rotation vec3
--- @field RotationLifeProperty AspkFloatKeyFrameProperty
--- @field ScaleModifierXProperty AspkFloatProperty
--- @field ScaleModifierYProperty AspkFloatProperty
--- @field ScaleModifierZProperty AspkFloatProperty
--- @field ScaleRotationDurationProperty AspkFloatKeyFrameProperty
--- @field ShowSkeletonProperty AspkBooleanProperty
--- @field SnapBottom number
--- @field SnapToGround boolean
--- @field SnapTop number
--- @field SyncAnimationResourceTimeProperty AspkBooleanProperty
--- @field UniformScale number
--- @field UniformScaleProperty AspkFloatKeyFrameProperty
--- @field VisualEntity EntityHandle
--- @field field_20C vec3
--- @field field_218 vec3
--- @field field_C8 number
--- @field field_EC FixedString
--- @field field_F0 FixedString


--- @class AspkModule
--- @field FullName FixedString[]
--- @field Name string
--- @field Properties AspkProperty[]


--- @class AspkMovingLevelComponent:AspkFxBaseComponent
--- @field LevelTemplate string
--- @field Offset vec3
--- @field ScrollingDirection vec3
--- @field ScrollingDistance number
--- @field ScrollingSpeed number
--- @field UpdateTime number


--- @class AspkOrbitForceComponent:AspkFxBaseComponent
--- @field KeyframedOffsetProperty AspkFloatKeyFrameProperty
--- @field KeyframedPosition vec3
--- @field Offset vec3
--- @field OrbitRadiusProperty AspkFloatKeyFrameProperty
--- @field Position vec3
--- @field field_D4 vec3


--- @class AspkOverlayMaterialComponent:AspkFxBaseComponent
--- @field Active boolean
--- @field AlphaChannel uint8
--- @field AlphaProperty AspkFloatKeyFrameProperty
--- @field AnimationSpeedProperty AspkFloatProperty
--- @field Applied uint64
--- @field ApplyMaps uint32
--- @field ApplyTo uint32
--- @field Brightness number
--- @field BrightnessProperty AspkFloatKeyFrameProperty
--- @field Color avec4
--- @field ColorProperty AspkColorARGBKeyFrameProperty
--- @field CustomModules AspkCustomModules
--- @field DynamicValueProperty AspkFloatKeyFrameProperty
--- @field EffectEntity EntityHandle
--- @field FadeTime number
--- @field HasCustomVectors boolean
--- @field Material FixedString
--- @field ModifierWProperty AspkFloatProperty
--- @field ModifierXProperty AspkFloatProperty
--- @field ModifierYProperty AspkFloatProperty
--- @field ModifierZProperty AspkFloatProperty
--- @field OverlayPriority int32
--- @field OverlayType uint32
--- @field VisualPosition vec3
--- @field field_150 uint8
--- @field field_151 uint8
--- @field field_154 uint8
--- @field field_160 vec4
--- @field field_170 vec4


--- @class AspkPackedUnitVector
--- @field X uint16
--- @field Y uint16
--- @field Z uint16


--- @class AspkParticleDistanceScale
--- @field Max number
--- @field Min number
--- @field Scale AspkFloatKeyFrameProperty


--- @class AspkParticleFlipbookData
--- @field Flags uint8
--- @field FrameRangeMax uint16
--- @field FrameRangeMin uint16
--- @field FrameRate uint16
--- @field HorizontalImages uint16
--- @field ImageController AspkFloatKeyFrameProperty
--- @field InitialFrame uint16
--- @field VerticalImages uint16


--- @class AspkParticleRenderStyle1
--- @field AxisAlignedOrientation AspkVector3Property
--- @field Flags uint8


--- @class AspkParticleSystemComponent:AspkFxBaseComponent
--- @field AlphaByEffectDistance AspkParticleDistanceScale
--- @field AlphaByParticleDistance AspkParticleDistanceScale
--- @field AttachedFXOnly uint8
--- @field AxisScale vec3
--- @field AxisScaleProperty AspkFloatKeyFrameProperty
--- @field BlendStateID uint8
--- @field BrightnessProperty AspkFloatKeyFrameProperty
--- @field Brightness_ number
--- @field Color avec4
--- @field ColorMultiplier avec4
--- @field ColorOverEffectLife number
--- @field ColorProperty AspkColorARGBKeyFrameProperty
--- @field CoordinateSpace uint8
--- @field EmitPosition vec3
--- @field EmitRateModifierProperty AspkFloatProperty
--- @field EmitRateProperty AspkFloatKeyFrameProperty
--- @field EmitStutterProperty AspkFloatKeyFrameProperty
--- @field EmitVelocityAnglesProperty AspkFloatKeyFrameProperty
--- @field EmitVelocityAxis vec3
--- @field EmitVelocityAxisRotationProperty AspkFloatKeyFrameProperty
--- @field EmitterType uint8
--- @field ExternalVelocity vec3
--- @field FixedColor avec4
--- @field Flags uint8
--- @field Flags2 uint8
--- @field Flipbook AspkParticleFlipbookData
--- @field InheritVelocityPercentProperty AspkFloatKeyFrameProperty
--- @field InitialParticleCountMax uint16
--- @field InitialParticleCountMin uint16
--- @field InitialVelocity number
--- @field InitialVelocityProperty AspkFloatKeyFrameProperty
--- @field KeyframedOffsetProperty AspkFloatKeyFrameProperty
--- @field KeyframedPosition vec3
--- @field KeyframedPositionModifierXProperty AspkFloatProperty
--- @field KeyframedPositionModifierYProperty AspkFloatProperty
--- @field KeyframedPositionModifierZProperty AspkFloatProperty
--- @field LastPosition vec3
--- @field LifespanProperty AspkFloatKeyFrameProperty
--- @field Material Material
--- @field MaterialDynamicValueProperty AspkFloatKeyFrameProperty
--- @field MaterialModifierWProperty AspkFloatProperty
--- @field MaterialModifierXProperty AspkFloatProperty
--- @field MaterialModifierYProperty AspkFloatProperty
--- @field MaterialModifierZProperty AspkFloatProperty
--- @field MaterialScaleModifierXProperty AspkFloatProperty
--- @field MaterialScaleModifierYProperty AspkFloatProperty
--- @field MaterialScaleModifierZProperty AspkFloatProperty
--- @field MaxLifespan number
--- @field MaximumParticleCount uint16
--- @field MinLifespan number
--- @field Offset vec3
--- @field ParticleAlphaProperty AspkFloatKeyFrameProperty
--- @field ParticleAxisScaleProperty AspkFloatKeyFrameProperty
--- @field ParticleBrightnessProperty AspkFloatKeyFrameProperty
--- @field ParticleColorProperty AspkColorARGBKeyFrameProperty
--- @field ParticleData AspkFxParticleData
--- @field ParticleDiscardByDistanceEnabled boolean
--- @field ParticleDiscardByDistanceMax number
--- @field ParticleDiscardByDistanceMin number
--- @field ParticleDiscardByDistanceScale AspkFloatKeyFrameProperty
--- @field ParticleInitialRotationProperty AspkFloatKeyFrameProperty
--- @field ParticleInitialRotationSpeedProperty AspkFloatKeyFrameProperty
--- @field ParticleLocalOriginProperty AspkFloatKeyFrameProperty
--- @field ParticleRotationProperty AspkFloatKeyFrameProperty
--- @field ParticleRotationSpeedProperty AspkFloatKeyFrameProperty
--- @field ParticleScaleProperty AspkFloatKeyFrameProperty
--- @field ParticleVelocityOffsetProperty AspkFloatKeyFrameProperty
--- @field PhysicalMaterialBounce number
--- @field PhysicalMaterialFriction number
--- @field PhysicalMaterialLifetimeLoss number
--- @field PhysicalMaterialRadiusScale number
--- @field PivotRotation mat3
--- @field Position vec3
--- @field RecycleRadius number
--- @field RecycleRadius_ uint16
--- @field RemainingParticleCount number
--- @field RenderOrder uint8
--- @field RenderStyle uint8
--- @field RenderStyle1 AspkParticleRenderStyle1
--- @field RotationRateOverLife number
--- @field ScaleByEffectDistance AspkParticleDistanceScale
--- @field ScaleByParticleDistance AspkParticleDistanceScale
--- @field ScaleMultiplier vec3
--- @field ScaleOverEffectLife number
--- @field ScaleOverEffectLifeAxis vec3
--- @field SnapBehavior AspkParticleSystemSnapBehavior
--- @field UniformScale number
--- @field UniformScaleProperty AspkFloatKeyFrameProperty
--- @field Visual FixedString
--- @field WarmUpTime number
--- @field field_2F9 uint8
--- @field field_2FA uint16


--- @class AspkParticleSystemSnapBehavior
--- @field Bottom number
--- @field Top number
--- @field field_0 vec3
--- @field field_18 mat3
--- @field field_3C number
--- @field field_C vec3


--- @class AspkPostProcessComponent:AspkFxBaseComponent


--- @class AspkPreRollComponent:AspkFxBaseComponent


--- @class AspkProperty
--- @field AttributeName FixedString
--- @field FullName FixedString
--- @field Input FixedString
--- @field Parameters AspkPropertyParameter[]


--- @class AspkPropertyParameter
--- @field Name string
--- @field Value number


--- @class AspkQuestionHoldAutomationSettings
--- @field CycleSpeed number
--- @field CycleSpeedDeviation number
--- @field IsEnabled boolean
--- @field StartOffset number
--- @field StartOffsetDeviation number


--- @class AspkRadialForceComponent:AspkFxBaseComponent
--- @field AccelerationProperty AspkFloatKeyFrameProperty
--- @field InfluenceRadius number
--- @field InfluenceRadiusApplied boolean
--- @field InfluenceRadiusProperty AspkFloatKeyFrameProperty
--- @field KeyframedOffsetProperty AspkFloatKeyFrameProperty
--- @field KeyframedPosition vec3
--- @field LockDistanceApplied boolean
--- @field LockDistanceProperty AspkFloatKeyFrameProperty
--- @field Offset vec3
--- @field Position vec3


--- @class AspkRibbon2Component:AspkFxBaseComponent
--- @field AlphaProperty AspkFloatKeyFrameProperty
--- @field BoundMax vec3
--- @field BoundMin vec3
--- @field BrightnessProperty AspkFloatKeyFrameProperty
--- @field ColorProperty AspkColorARGBKeyFrameProperty
--- @field ColorType uint32
--- @field ConnectToEmitter boolean
--- @field CoordinateSpace uint32
--- @field DebugShapes Visual[]
--- @field DebugShapes2 Visual[]
--- @field DynamicParameterModifierWProperty AspkFloatProperty
--- @field DynamicParameterModifierXProperty AspkFloatProperty
--- @field DynamicParameterModifierYProperty AspkFloatProperty
--- @field DynamicParameterModifierZProperty AspkFloatProperty
--- @field DynamicParameterValueProperty AspkFloatKeyFrameProperty
--- @field DynamicParameterVec vec4
--- @field DynamicParametersType uint32
--- @field Effect RenderableObject
--- @field EmissionBehavior uint32
--- @field EmitDistance number
--- @field EmitRate number
--- @field ExtraRotation quat
--- @field Flags uint32
--- @field InfiniteParticleLife boolean
--- @field InitialVelocity vec3
--- @field InvertTexcoord boolean
--- @field MaxParticleCount uint32
--- @field ParticleLife number
--- @field Rotation quat
--- @field ScaleModifierProperty AspkFloatProperty
--- @field ScaleProperty AspkFloatKeyFrameProperty
--- @field ScaleType uint32
--- @field TexCoordType uint32
--- @field Translate vec3
--- @field UnitTime number
--- @field UpdateTime number
--- @field UseObjectOrientation boolean
--- @field Velocity vec3
--- @field VisualEntity EntityHandle
--- @field Visuals Visual
--- @field field_160 number
--- @field field_164 uint32
--- @field field_1B8 number
--- @field field_1C4 vec3
--- @field field_B8 uint32


--- @class AspkRibbonParticleData
--- @field NextParticle int32
--- @field ParticleCount int32
--- @field Particles uint16[]
--- @field Size int32
--- @field field_0 int32


--- @class AspkRibbonParticleHolder
--- @field Data AspkRibbonParticleData
--- @field field_8 int32


--- @class AspkSoundComponent:AspkFxBaseComponent
--- @field Flags uint8
--- @field Flags2 uint8
--- @field PlayOn int32
--- @field SoundEventName FixedString
--- @field SoundEventName2 FixedString
--- @field SoundEventName3 FixedString


--- @class AspkSpinForceComponent:AspkFxBaseComponent
--- @field InfluenceRadius number
--- @field InfluenceRadiusApplied boolean
--- @field InfluenceRadiusProperty AspkFloatKeyFrameProperty
--- @field KeyframedOffsetProperty AspkFloatKeyFrameProperty
--- @field KeyframedPosition vec3
--- @field Offset vec3
--- @field Position vec3
--- @field SpinAxis vec3
--- @field SpinVelocityProperty AspkFloatKeyFrameProperty


--- @class AspkStringProperty:AspkProperty
--- @field Value string


--- @class AspkTLActor
--- @field PeanutOverride boolean
--- @field UUID Guid


--- @class AspkTLAdditiveAnimationComponent:AspkTLAnimationComponent


--- @class AspkTLAnimationComponent:AspkTLBaseComponent
--- @field AnimationGroup Guid
--- @field AnimationPlayRate number
--- @field AnimationPlayStartOffset number
--- @field AnimationSlot FixedString
--- @field AnimationSourceId Guid
--- @field BoneGroupId Guid
--- @field Continuous boolean
--- @field EnableRootMotion boolean
--- @field FadeIn number
--- @field FadeInOffset number
--- @field FadeOut number
--- @field FadeOutOffset number
--- @field HoldAnimation boolean
--- @field IsMirrored boolean
--- @field OffsetType uint8
--- @field TargetTransform AspkTLAnimationComponentTargetTransforms
--- @field field_190 int64
--- @field field_1a9 string
--- @field field_1aa int16
--- @field field_1ac int32
--- @field field_1f8 int64
--- @field field_2b8 int64
--- @field field_2c0 int64
--- @field field_80 int64
--- @field field_98 int64
--- @field field_a0 int64
--- @field field_a8 int64


--- @class AspkTLAnimationComponentTargetTransforms
--- @field Position vec3
--- @field RotationQuat vec4
--- @field Scale number
--- @field field_1c number
--- @field field_20 number


--- @class AspkTLAtmosphereAndLightingComponent:AspkTLBaseComponent
--- @field Channels AspkChannelsTLAtmosphereAndLightingChannelComponent[]


--- @class AspkTLAttachToEventComponent:AspkTLInterpolationKeyComponent_AspkKeysAttachToEventKey


--- @class AspkTLAttitudeEventComponent:AspkTLKeyBaseComponent_AspkKeysAttitudeEventKey
--- @field field_b0 FixedString
--- @field field_b4 int32


--- @class AspkTLBaseComponent:AspkComponent
--- @field Actor AspkTLActor
--- @field IsMimicry boolean
--- @field IsSnappedToEnd boolean
--- @field PhaseIndex int64
--- @field TypeId uint32
--- @field field_68x uint8


--- @class AspkTLCameraDoFComponent:AspkTLBaseComponent
--- @field Channels AspkChannelsTLCameraDoFChannelComponent[]


--- @class AspkTLCameraExposureComponent:AspkTLBaseComponent
--- @field Channels AspkChannelsTLCameraExposureChannelComponent[]
--- @field field_90 int64


--- @class AspkTLCameraFoVComponent:AspkTLInterpolationKeyComponent_AspkKeysCameraFoVKey


--- @class AspkTLCameraLookAtComponent:AspkTLInterpolationKeyComponent_AspkKeysCameraLookAtKey


--- @class AspkTLEffectPhaseEventComponent:AspkTLEventKeyComponent_AspkKeysEffectPhaseEventKey


--- @class AspkTLEmotionEventComponent:AspkTLEventKeyComponent_AspkKeysEmotionEventKey


--- @class AspkTLEventKeyComponent_AspkKeysEffectPhaseEventKey:AspkTLKeyBaseComponent_AspkKeysEffectPhaseEventKey


--- @class AspkTLEventKeyComponent_AspkKeysEmotionEventKey:AspkTLKeyBaseComponent_AspkKeysEmotionEventKey


--- @class AspkTLEventKeyComponent_AspkKeysGenomeTextEventKey:AspkTLKeyBaseComponent_AspkKeysGenomeTextEventKey


--- @class AspkTLEventKeyComponent_AspkKeysHandsIKKey:AspkTLKeyBaseComponent_AspkKeysHandsIKKey


--- @class AspkTLEventKeyComponent_AspkKeysKeyBase:AspkTLKeyBaseComponent_AspkKeysKeyBase


--- @class AspkTLEventKeyComponent_AspkKeysLookAtEventKey:AspkTLKeyBaseComponent_AspkKeysLookAtEventKey


--- @class AspkTLEventKeyComponent_AspkKeysPhysicsKey:AspkTLKeyBaseComponent_AspkKeysPhysicsKey


--- @class AspkTLEventKeyComponent_AspkKeysPlayEffectEventKey:AspkTLKeyBaseComponent_AspkKeysPlayEffectEventKey


--- @class AspkTLEventKeyComponent_AspkKeysShapeShiftKey:AspkTLKeyBaseComponent_AspkKeysShapeShiftKey


--- @class AspkTLEventKeyComponent_AspkKeysShowArmorChannelKey:AspkTLKeyBaseComponent_AspkKeysShowArmorChannelKey


--- @class AspkTLEventKeyComponent_AspkKeysShowHUDKey:AspkTLKeyBaseComponent_AspkKeysShowHUDKey


--- @class AspkTLEventKeyComponent_AspkKeysShowPeanutsKey:AspkTLKeyBaseComponent_AspkKeysShowPeanutsKey


--- @class AspkTLEventKeyComponent_AspkKeysShowVisualKey:AspkTLKeyBaseComponent_AspkKeysShowVisualKey


--- @class AspkTLEventKeyComponent_AspkKeysShowWeaponKey:AspkTLKeyBaseComponent_AspkKeysShowWeaponKey


--- @class AspkTLEventKeyComponent_AspkKeysSoundEventKey:AspkTLKeyBaseComponent_AspkKeysSoundEventKey


--- @class AspkTLEventKeyComponent_AspkKeysSpringsKey:AspkTLKeyBaseComponent_AspkKeysSpringsKey


--- @class AspkTLEventKeyComponent_AspkKeysSteppingFadeKey:AspkTLKeyBaseComponent_AspkKeysSteppingFadeKey


--- @class AspkTLEventKeyComponent_AspkKeysSwitchLocationEventKey:AspkTLKeyBaseComponent_AspkKeysSwitchLocationEventKey


--- @class AspkTLEventKeyComponent_AspkKeysSwitchStageEventKey:AspkTLKeyBaseComponent_AspkKeysSwitchStageEventKey


--- @class AspkTLFloatRTPCComponent:AspkTLInterpolationKeyComponent_AspkKeysFloatRTPCKey
--- @field RTPCName FixedString
--- @field SoundObjectIndex uint8
--- @field SoundType uint8
--- @field field_b0 int64


--- @class AspkTLGenomeTextEventComponent:AspkTLEventKeyComponent_AspkKeysGenomeTextEventKey


--- @class AspkTLHandsIKComponent:AspkTLEventKeyComponent_AspkKeysHandsIKKey


--- @class AspkTLInterpolationKeyComponent_AspkKeysAtmosphereAndLightingChannelKey:AspkTLKeyBaseComponent_AspkKeysAtmosphereAndLightingChannelKey


--- @class AspkTLInterpolationKeyComponent_AspkKeysAttachToEventKey:AspkTLKeyBaseComponent_AspkKeysAttachToEventKey


--- @class AspkTLInterpolationKeyComponent_AspkKeysCameraDoFChannelKey:AspkTLKeyBaseComponent_AspkKeysCameraDoFChannelKey


--- @class AspkTLInterpolationKeyComponent_AspkKeysCameraFoVKey:AspkTLKeyBaseComponent_AspkKeysCameraFoVKey


--- @class AspkTLInterpolationKeyComponent_AspkKeysCameraLookAtKey:AspkTLKeyBaseComponent_AspkKeysCameraLookAtKey


--- @class AspkTLInterpolationKeyComponent_AspkKeysFloatRTPCKey:AspkTLKeyBaseComponent_AspkKeysFloatRTPCKey


--- @class AspkTLInterpolationKeyComponent_AspkKeysKeyBase:AspkTLKeyBaseComponent_AspkKeysKeyBase


--- @class AspkTLInterpolationKeyComponent_AspkKeysMaterialTextureKeyKey:AspkTLKeyBaseComponent_AspkKeysMaterialTextureKeyKey


--- @class AspkTLInterpolationKeyComponent_AspkKeysSplatterChannelKey:AspkTLKeyBaseComponent_AspkKeysSplatterChannelKey


--- @class AspkTLInterpolationKeyComponent_AspkKeysTransformChannelFloatKey:AspkTLKeyBaseComponent_AspkKeysTransformChannelFloatKey


--- @class AspkTLInterpolationKeyComponent_AspkKeysTransformChannelFrameOfReferenceKey:AspkTLKeyBaseComponent_AspkKeysTransformChannelFrameOfReferenceKey


--- @class AspkTLInterpolationKeyComponent_AspkKeysTransformChannelQuatKey:AspkTLKeyBaseComponent_AspkKeysTransformChannelQuatKey


--- @class AspkTLInterpolationKeyComponent<AspkKeysValueKey_float>:AspkTLKeyBaseComponent<AspkKeysValueKey_float>


--- @class AspkTLKeyBaseComponent_AspkKeysAtmosphereAndLightingChannelKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysAtmosphereAndLightingChannelKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysAttachToEventKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysAttachToEventKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysAttitudeEventKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysAttitudeEventKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysCameraDoFChannelKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysCameraDoFChannelKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysCameraFoVKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysCameraFoVKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysCameraLookAtKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysCameraLookAtKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysEffectPhaseEventKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysEffectPhaseEventKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysEmotionEventKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysEmotionEventKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysFloatRTPCKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysFloatRTPCKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysGenomeTextEventKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysGenomeTextEventKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysHandsIKKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysHandsIKKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysKeyBase:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysKeyBase
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysLookAtEventKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysLookAtEventKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysMaterialTextureKeyKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysMaterialTextureKeyKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysPhysicsKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysPhysicsKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysPlayEffectEventKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysPlayEffectEventKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysPlayRateKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysPlayRateKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysShapeShiftKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysShapeShiftKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysShowArmorChannelKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysShowArmorChannelKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysShowHUDKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysShowHUDKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysShowPeanutsKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysShowPeanutsKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysShowVisualKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysShowVisualKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysShowWeaponKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysShowWeaponKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysSoundEventKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysSoundEventKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysSplatterChannelKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysSplatterChannelKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysSpringsKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysSpringsKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysSteppingFadeKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysSteppingFadeKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysSwitchLocationEventKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysSwitchLocationEventKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysSwitchStageEventKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysSwitchStageEventKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysTransformChannelFloatKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysTransformChannelFloatKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysTransformChannelFrameOfReferenceKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysTransformChannelFrameOfReferenceKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent_AspkKeysTransformChannelQuatKey:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysTransformChannelQuatKey
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLKeyBaseComponent<AspkKeysValueKey_float>:AspkTLBaseComponent
--- @field HasTargetKey boolean
--- @field NeedsKeyUpdate boolean
--- @field Random uint32[]
--- @field TargetKey AspkKeysValueKey_float
--- @field field_68 number
--- @field field_91 boolean


--- @class AspkTLLayeredAnimationComponent:AspkTLAnimationComponent


--- @class AspkTLLookAtEventComponent:AspkTLEventKeyComponent_AspkKeysLookAtEventKey


--- @class AspkTLMaterialComponent:AspkTLBaseComponent
--- @field GroupId Guid
--- @field IsContinuous boolean
--- @field IsOverlay boolean
--- @field MaterialParameters AspkTLMaterialComponentParameter[]
--- @field OverlayPriority number
--- @field field_15a int16


--- @class AspkTLMaterialComponentMaterialParameter:AspkTLMaterialComponentParameter


--- @class AspkTLMaterialComponentParameter
--- @field MaterialParameterName FixedString
--- @field field_10 int64
--- @field field_c int32


--- @class AspkTLMaterialComponentParameterRange
--- @field begin AspkComponent
--- @field end AspkComponent


--- @class AspkTLMaterialComponentTextureParameter:AspkTLMaterialComponentParameter
--- @field IsVirtual boolean
--- @field TextureParam boolean
--- @field field_1a int16
--- @field field_1c int32


--- @class AspkTLPhysicsComponent:AspkTLEventKeyComponent_AspkKeysPhysicsKey


--- @class AspkTLPlayEffectEventComponent:AspkTLEventKeyComponent_AspkKeysPlayEffectEventKey
--- @field field_b0 int64


--- @class AspkTLPlayRateComponent:AspkTLKeyBaseComponent_AspkKeysPlayRateKey


--- @class AspkTLShapeShiftComponent:AspkTLEventKeyComponent_AspkKeysShapeShiftKey


--- @class AspkTLShotComponent:AspkTLBaseComponent
--- @field AutomatedCamera boolean
--- @field AutomatedLighting boolean
--- @field CameraContainers Guid[]
--- @field CompanionCameraA Guid
--- @field CompanionCameraB Guid
--- @field CompanionCameraC Guid
--- @field DisableConditionalStaging boolean
--- @field FirstCamera Guid
--- @field IsJCutEnabled boolean
--- @field IsLogicEnabled boolean
--- @field IsLooping boolean
--- @field JCutLength number
--- @field SwitchInterval boolean
--- @field field_110 int64
--- @field field_118 int64
--- @field field_120 int64
--- @field field_128 int64
--- @field field_130 int64
--- @field field_138 int64
--- @field field_140 int64
--- @field field_148 int64
--- @field field_150 int64
--- @field field_80 int64
--- @field field_b1 string
--- @field field_b2 int16
--- @field field_b8 int64
--- @field field_c0 int64
--- @field field_c8 int64
--- @field field_d0 int64
--- @field field_d9 string
--- @field field_da int16
--- @field field_dc int32


--- @class AspkTLShotHoldPreviousComponent:AspkTLBaseComponent


--- @class AspkTLShotZoomComponent:AspkTLBaseComponent
--- @field EndOffset number
--- @field StartOffset number


--- @class AspkTLShowArmorComponent:AspkTLBaseComponent
--- @field Channels AspkChannelsTLShowArmorChannelComponent[]
--- @field field_80 int64
--- @field field_98 int64


--- @class AspkTLShowHUDComponent:AspkTLEventKeyComponent_AspkKeysShowHUDKey


--- @class AspkTLShowPeanutsComponent:AspkTLEventKeyComponent_AspkKeysShowPeanutsKey


--- @class AspkTLShowVisualComponent:AspkTLEventKeyComponent_AspkKeysShowVisualKey


--- @class AspkTLShowWeaponComponent:AspkTLEventKeyComponent_AspkKeysShowWeaponKey


--- @class AspkTLSoundEventComponent:AspkTLEventKeyComponent_AspkKeysSoundEventKey
--- @field RTPCChannels AspkTLFloatRTPCComponent[]


--- @class AspkTLSplatterComponent:AspkTLBaseComponent
--- @field Channels AspkChannelsTLSplatterChannelComponent[]


--- @class AspkTLSpringsComponent:AspkTLEventKeyComponent_AspkKeysSpringsKey
--- @field VisualFlag uint32
--- @field field_b4 int32


--- @class AspkTLSteppingFadeComponent:AspkTLEventKeyComponent_AspkKeysSteppingFadeKey


--- @class AspkTLSwitchLocationEventComponent:AspkTLEventKeyComponent_AspkKeysSwitchLocationEventKey


--- @class AspkTLSwitchStageEventComponent:AspkTLEventKeyComponent_AspkKeysSwitchStageEventKey
--- @field field_b0 int64
--- @field field_b8 int64


--- @class AspkTLTransformComponent:AspkTLBaseComponent
--- @field Continuous boolean
--- @field field_80 AspkChannelsTLTransformChannelFloatComponent
--- @field field_88 AspkChannelsTLTransformChannelFloatComponent
--- @field field_90 AspkChannelsTLTransformChannelFloatComponent
--- @field field_98 AspkChannelsTLTransformChannelQuatComponent
--- @field field_a0 AspkChannelsTLTransformChannelFloatComponent
--- @field field_a8 AspkChannelsTLTransformChannelFrameOfReferenceComponent
--- @field field_b1 string
--- @field field_b2 int16
--- @field field_b4 int32


--- @class AspkTLVoiceComponent:AspkTLBaseComponent
--- @field DialogNodeId Guid
--- @field DisableMocap boolean
--- @field FaceType uint8
--- @field FadeIn number
--- @field FadeInOffset number
--- @field FadeOut number
--- @field FadeOutOffset number
--- @field HeadPitchCorrection number
--- @field HeadRollCorrection number
--- @field HeadYawCorrection number
--- @field HoldMocap boolean
--- @field IsAliased boolean
--- @field IsMirrored boolean
--- @field LeftBuffer number
--- @field PerformanceDriftType uint8
--- @field PerformanceFade number
--- @field ReferenceId Guid
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_b4 int32
--- @field field_e0 int64
--- @field field_e9 string
--- @field field_ea int16
--- @field field_ec int32


--- @class AspkTimeline
--- @field Components AspkComponent[]
--- @field CurrentPlayCount int32
--- @field Duration number
--- @field Header AspkTimelineHeader
--- @field Inputs AspkInput[]
--- @field IsPaused boolean
--- @field JumpToPhase int32
--- @field JumpToTime number
--- @field NeedsPostUpdate_M boolean
--- @field PhaseIndex uint64
--- @field PlayToEnd boolean
--- @field PlayingSpeed number
--- @field RemainingPhaseTime number
--- @field State uint8
--- @field TimePlayed number
--- @field field_23 boolean
--- @field field_24 boolean


--- @class AspkTimelineActorPropertiesReflectionComponent:AspkTLBaseComponent
--- @field PropertyParameters AspkTimelineActorPropertiesReflectionComponentParameter[]
--- @field field_80 int64


--- @class AspkTimelineActorPropertiesReflectionComponentParameter
--- @field ParameterName FixedString
--- @field field_10 int64
--- @field field_18 int64
--- @field field_20 int64
--- @field field_28 int64
--- @field field_30 int64
--- @field field_38 int64
--- @field field_40 int64
--- @field field_48 int64
--- @field field_50 int64
--- @field field_c int32


--- @class AspkTimelineActorPropertiesReflectionComponentParameterRange
--- @field begin AspkChannelsTimelineActorPropertiesReflectionKeyComponent
--- @field end AspkChannelsTimelineActorPropertiesReflectionKeyComponent


--- @class AspkTimelineHeader
--- @field Duration number
--- @field Phases AspkTimelinePhase[]


--- @class AspkTimelinePhase
--- @field DialogNodeId Guid
--- @field Duration number
--- @field EndTime number
--- @field IsOverridingTimelineQuestionHoldAutomationSettings boolean
--- @field PlayCount int32
--- @field QuestionHoldAutomation AspkQuestionHoldAutomationSettings
--- @field field_14 int32


--- @class AspkTurbulenceForceComponent:AspkFxBaseComponent
--- @field AccelerationProperty AspkFloatKeyFrameProperty
--- @field InfluenceRadius number
--- @field InfluenceRadiusApplied boolean
--- @field InfluenceRadiusProperty AspkFloatKeyFrameProperty
--- @field KeyframedOffsetProperty AspkFloatKeyFrameProperty
--- @field KeyframedPosition vec3
--- @field Offset vec3
--- @field Position vec3


--- @class AspkVector3Property:AspkProperty
--- @field Value vec3


--- @class AspkVortexForceComponent:AspkFxBaseComponent
--- @field InfluenceRadius number
--- @field InfluenceRadiusApplied boolean
--- @field InfluenceRadiusProperty AspkFloatKeyFrameProperty
--- @field KeyframedOffsetProperty AspkFloatKeyFrameProperty
--- @field KeyframedPosition vec3
--- @field Offset vec3
--- @field Position vec3
--- @field RotationAxis vec3
--- @field SpinVelocityProperty AspkFloatKeyFrameProperty


--- @class AspkWindForceComponent:AspkFxBaseComponent
--- @field SpeedMultiplier AspkFloatKeyFrameProperty
--- @field UpdateTime number


--- @class AspkChannelsTLAtmosphereAndLightingChannelComponent:AspkTLInterpolationKeyComponent_AspkKeysAtmosphereAndLightingChannelKey
--- @field field_b0 int64
--- @field field_b8 int64
--- @field field_c0 int64


--- @class AspkChannelsTLCameraDoFChannelComponent:AspkTLInterpolationKeyComponent_AspkKeysCameraDoFChannelKey
--- @field channelNum uint8
--- @field field_b0 number
--- @field field_b5 string
--- @field field_b6 int16


--- @class AspkChannelsTLCameraExposureChannelComponent:AspkChannelsTLCameraDoFChannelComponent


--- @class AspkChannelsTLMaterialKeyComponent:AspkTLInterpolationKeyComponent<AspkKeysValueKey_float>
--- @field defaultVal number
--- @field field_b4 int32


--- @class AspkChannelsTLMaterialTextureKeyComponent:AspkTLInterpolationKeyComponent_AspkKeysMaterialTextureKeyKey
--- @field field_b0 int64
--- @field field_b8 int64


--- @class AspkChannelsTLShowArmorChannelComponent:AspkTLEventKeyComponent_AspkKeysShowArmorChannelKey
--- @field field_b0 int64


--- @class AspkChannelsTLSplatterChannelComponent:AspkTLInterpolationKeyComponent_AspkKeysSplatterChannelKey
--- @field SplatterType uint8
--- @field field_b0 int64


--- @class AspkChannelsTLTransformChannelFloatComponent:AspkTLInterpolationKeyComponent_AspkKeysTransformChannelFloatKey
--- @field defaultVal number
--- @field field_b4 int32
--- @field field_b8 int64[]


--- @class AspkChannelsTLTransformChannelFrameOfReferenceComponent:AspkTLInterpolationKeyComponent_AspkKeysTransformChannelFrameOfReferenceKey
--- @field defaultVal AspkKeysTransformChannelFrameOfReferenceKeyFrameOfReference
--- @field field_b8 int64[]


--- @class AspkChannelsTLTransformChannelQuatComponent:AspkTLInterpolationKeyComponent_AspkKeysTransformChannelQuatKey
--- @field defaultVal vec4
--- @field field_c0 int64[]


--- @class AspkChannelsTimelineActorPropertiesReflectionKeyComponent:AspkTLInterpolationKeyComponent<AspkKeysValueKey_float>
--- @field KeyDefault number


--- @class AspkKeysAtmosphereAndLightingChannelKey:AspkKeysKeyBase
--- @field FadeTime number
--- @field Id Guid


--- @class AspkKeysAttachToEventKey:AspkKeysKeyBase
--- @field Bone FixedString
--- @field IsPersistent boolean
--- @field Target Guid
--- @field offset vec3


--- @class AspkKeysAttitudeEventKey:AspkKeysKeyBase
--- @field Pose FixedString
--- @field Transition FixedString


--- @class AspkKeysCameraDoFChannelKey:AspkKeysKeyBase
--- @field Value number


--- @class AspkKeysCameraFoVKey:AspkKeysKeyBase
--- @field FoV number


--- @class AspkKeysCameraLookAtKey:AspkKeysKeyBase
--- @field Bone FixedString
--- @field DampingStrength number
--- @field Framing vec2
--- @field FramingOffset vec2
--- @field FreeBounds vec2
--- @field FreeZoneDelay number
--- @field SoftBounds vec2
--- @field SoftZoneDelay number
--- @field SoftZoneRampTime number
--- @field Target Guid


--- @class AspkKeysEffectPhaseEventKey:AspkKeysKeyBase
--- @field EffectPhase int32


--- @class AspkKeysEmotionEventKey:AspkKeysKeyBase
--- @field AppliesMaterials boolean
--- @field Emotion int32
--- @field IsSustainedEmotion boolean
--- @field Variation int32


--- @class AspkKeysFloatRTPCKey:AspkKeysKeyBase
--- @field FloatRTPCValue number


--- @class AspkKeysGenomeTextEventKey:AspkKeysKeyBase
--- @field EventName FixedString


--- @class AspkKeysHandsIKKey:AspkKeysKeyBase
--- @field InverseKinematics boolean


--- @class AspkKeysKeyBase
--- @field InterpolationType uint8
--- @field Time number


--- @class AspkKeysLookAtEventKey:AspkKeysKeyBase
--- @field Bone FixedString
--- @field EyeLookAtBone FixedString
--- @field EyeLookAtOffset vec3
--- @field EyeLookAtTargetId Guid
--- @field HeadSafeZoneAngle number?
--- @field HeadTurnSpeedMultiplier number
--- @field IsEyeLookAtEnabled boolean
--- @field LookAtInterpMode uint8
--- @field LookAtMode uint8
--- @field Offset vec3
--- @field Reset boolean
--- @field SaveZoneAngle number?
--- @field Target Guid
--- @field TorsoTurnSpeedMultiplier number
--- @field TrackingMode uint8
--- @field TurnMode uint8
--- @field TurnSpeedMultiplier number
--- @field Weight number?


--- @class AspkKeysMaterialTextureKeyKey:AspkKeysKeyBase
--- @field Value Guid


--- @class AspkKeysPhysicsKey:AspkKeysKeyBase
--- @field InverseKinematics boolean


--- @class AspkKeysPlayEffectEventKey:AspkKeysKeyBase
--- @field PlayEffect boolean


--- @class AspkKeysPlayRateKey:AspkKeysKeyBase
--- @field Speed number


--- @class AspkKeysShapeShiftKey:AspkKeysKeyBase
--- @field TemplateId Guid


--- @class AspkKeysShowArmorChannelKey:AspkKeysKeyBase
--- @field Value boolean


--- @class AspkKeysShowHUDKey:AspkKeysKeyBase
--- @field ShowHUD boolean


--- @class AspkKeysShowPeanutsKey:AspkKeysKeyBase
--- @field ShowPeanuts boolean


--- @class AspkKeysShowVisualKey:AspkKeysKeyBase
--- @field ShowVisual boolean


--- @class AspkKeysShowWeaponKey:AspkKeysKeyBase
--- @field ShowWeapon boolean


--- @class AspkKeysSoundEventKey:AspkKeysKeyBase
--- @field data Variant<AspkKeysSoundEventKeyType0,AspkKeysSoundEventKeyType1,AspkKeysSoundEventKeyType2,AspkKeysSoundEventKeyType3,AspkKeysSoundEventKeyType4,AspkKeysSoundEventKeyType5>


--- @class AspkKeysSoundEventKeyType0
--- @field KeepAlive boolean
--- @field LoopLifetime number
--- @field SoundEventID Guid
--- @field SoundObjectIndex uint8


--- @class AspkKeysSoundEventKeyType1
--- @field SoundEventID Guid


--- @class AspkKeysSoundEventKeyType2
--- @field SoundEventID Guid


--- @class AspkKeysSoundEventKeyType3
--- @field SoundEventID Guid


--- @class AspkKeysSoundEventKeyType4
--- @field FoleyIntensity uint8
--- @field FoleyType uint8
--- @field SoundObjectIndex uint8


--- @class AspkKeysSoundEventKeyType5
--- @field SoundObjectIndex uint8
--- @field VocalType uint8


--- @class AspkKeysSplatterChannelKey:AspkKeysKeyBase
--- @field SplatterChangeMode uint8
--- @field Value number


--- @class AspkKeysSpringsKey:AspkKeysKeyBase
--- @field EnableSprings boolean


--- @class AspkKeysSteppingFadeKey:AspkKeysKeyBase
--- @field SteppingInOut boolean


--- @class AspkKeysSwitchLocationEventKey:AspkKeysKeyBase
--- @field SwitchLocationEventID Guid
--- @field s_LevelTemplateID Guid


--- @class AspkKeysSwitchStageEventKey:AspkKeysKeyBase
--- @field ForceTransformUpdate boolean
--- @field ForceUpdateCameraBehavior boolean
--- @field SwitchStageEventID Guid


--- @class AspkKeysTransformChannelFloatKey:AspkKeysKeyBase
--- @field Value number


--- @class AspkKeysTransformChannelFrameOfReferenceKey:AspkKeysKeyBase
--- @field frameOfReference AspkKeysTransformChannelFrameOfReferenceKeyFrameOfReference


--- @class AspkKeysTransformChannelFrameOfReferenceKeyFrameOfReference
--- @field KeepScale boolean
--- @field OneFrameOnly boolean
--- @field targetBone FixedString
--- @field targetId Guid


--- @class AspkKeysTransformChannelQuatKey:AspkKeysKeyBase
--- @field Value vec4


--- @class AspkKeysValueKey_float:AspkKeysKeyBase
--- @field Value number


--- @class CampChestComponent:BaseComponent
--- @field UserID int32
--- @field field_0 string
--- @field field_1C FixedString
--- @field field_20 EntityHandle
--- @field field_28 int32


--- @class CampEndTheDayStateComponent:BaseComponent
--- @field State uint8
--- @field field_8 EntityHandle


--- @class CampPresenceComponent:BaseComponent


--- @class CampQualityComponent:BaseComponent
--- @field field_0 int32
--- @field field_4 int32


--- @class CampSettingsComponent:BaseComponent
--- @field field_0 uint8
--- @field field_1 uint8
--- @field field_4 int32


--- @class CampSupplyComponent:BaseComponent
--- @field Amount int32


--- @class CampTotalSuppliesComponent:BaseComponent
--- @field Amount int32


--- @class CharacterCreationAppearance
--- @field AccessorySet Guid
--- @field AdditionalChoices number[]
--- @field Elements CharacterCreationAppearanceMaterialSetting[]
--- @field EyeColor Guid
--- @field HairColor Guid
--- @field Icon ScratchBuffer
--- @field SecondEyeColor Guid
--- @field SkinColor Guid
--- @field Visuals Guid[]
--- @field field_98 Guid


--- @class CharacterCreationAppearanceMaterialSetting
--- @field Color Guid
--- @field ColorIntensity number
--- @field GlossyTint number
--- @field Material Guid
--- @field MetallicTint uint32


--- @class CharacterCreationBaseCharacterDefinition
--- @field Background Guid
--- @field BodyShape uint8
--- @field BodyType uint8
--- @field Deity Guid
--- @field Origin Guid
--- @field Race Guid
--- @field RootTemplate Guid
--- @field Subrace Guid
--- @field VOLines Guid
--- @field Voice Guid
--- @field field_20 Guid
--- @field field_30 Guid
--- @field field_40 Guid
--- @field field_50 uint8
--- @field field_51 uint8
--- @field field_58 Guid
--- @field field_68 Guid
--- @field field_78 Guid
--- @field field_88 Guid
--- @field field_98 Guid
--- @field field_A8 uint8


--- @class CharacterCreationChangeAppearanceDefinitionBase
--- @field Name string
--- @field RootTemplate Guid
--- @field Visual CharacterCreationAppearance
--- @field Voice Guid
--- @field field_18 uint8
--- @field field_19 uint8
--- @field field_40 uint8


--- @class CharacterCreationChangeAppearanceDefinitionComponent:BaseComponent
--- @field Appearance CharacterCreationChangeAppearanceDefinitionBase
--- @field Definition CharacterCreationFullRespecDefinition
--- @field field_2E0 int32
--- @field field_2E4 uint8
--- @field field_2E8 EntityHandle


--- @class CharacterCreationCharacterDefinition
--- @field Abilities int32[]
--- @field Definition CharacterCreationBaseCharacterDefinition
--- @field LevelUpData LevelUpData
--- @field Name string
--- @field Visual CharacterCreationAppearance
--- @field field_0 int32
--- @field field_20 CharacterCreationBaseCharacterDefinition
--- @field field_8 string


--- @class CharacterCreationCharacterDefinitionComponent:BaseComponent
--- @field ChangeId int32
--- @field Definition CharacterCreationCharacterDefinition
--- @field NeedsSync boolean
--- @field field_288 uint8
--- @field field_28C int32
--- @field field_290 boolean


--- @class CharacterCreationCompanionDefinitionComponent:BaseComponent
--- @field HasIcon boolean
--- @field IconVersion uint32
--- @field Visual CharacterCreationAppearance
--- @field field_0 Guid
--- @field field_10 Guid
--- @field field_110 Guid
--- @field field_120 Guid
--- @field field_130 uint8
--- @field field_138 uint32
--- @field field_13C boolean
--- @field field_20 uint8
--- @field field_21 uint8
--- @field field_28 Guid


--- @class CharacterCreationDefinitionCommonComponent:BaseComponent
--- @field ChangeId int32
--- @field Error CharacterCreationValidationError
--- @field field_0 int32
--- @field field_8 int64


--- @class CharacterCreationFullRespecDefinition
--- @field Abilities int32[]
--- @field Definition CharacterCreationBaseCharacterDefinition
--- @field KnownSpells CharacterCreationLevelUpKnownSpell[]
--- @field LevelUpData LevelUpData[]
--- @field Name string
--- @field Visual CharacterCreationAppearance
--- @field field_0 string
--- @field field_1B0 CharacterCreationLevelUpKnownSpell[]


--- @class CharacterCreationFullRespecDefinitionComponent:BaseComponent
--- @field Definition CharacterCreationFullRespecDefinition
--- @field LevelUpUpgrades LevelUpUpgrades
--- @field Spells SpellMetaId[]
--- @field field_1C0 Guid
--- @field field_1D0 Guid
--- @field field_1E0 int32
--- @field field_268 uint8


--- @class CharacterCreationLevelUpComponent:BaseComponent
--- @field LevelUps LevelUpData[]


--- @class CharacterCreationLevelUpDefinitionComponent:BaseComponent
--- @field ChangeId int32
--- @field Character EntityHandle
--- @field Definition CharacterCreationFullRespecDefinition
--- @field LevelUp CharacterCreationLevelUpDefinitionExtra
--- @field NeedsSync boolean
--- @field field_160 uint8
--- @field field_164 int32
--- @field field_168 boolean
--- @field field_16C int32


--- @class CharacterCreationLevelUpDefinitionExtra
--- @field AppearanceElements CharacterCreationAppearanceMaterialSetting[]
--- @field Icon ScratchBuffer
--- @field LevelUpData LevelUpData
--- @field field_0 int32
--- @field field_8 Guid


--- @class CharacterCreationLevelUpKnownSpell
--- @field Class Guid
--- @field SpellId FixedString
--- @field field_0 Guid
--- @field field_10 FixedString


--- @class CharacterCreationRespecDefinitionComponent:BaseComponent
--- @field Definition CharacterCreationCharacterDefinition
--- @field field_0 Guid
--- @field field_298 uint8


--- @class CharacterCreationSessionCommonComponent:BaseComponent
--- @field NetId int32
--- @field field_4 int32
--- @field field_8 uint8


--- @class CharacterCreationStateComponent:BaseComponent
--- @field Canceled boolean
--- @field HasDummy boolean
--- @field field_2 uint8


--- @class CharacterCreationValidationError
--- @field AbilityPointBonuses CharacterCreationValidationSelectorError[]
--- @field Cantrips CharacterCreationValidationSelectorError[]
--- @field Flags uint64
--- @field Proficiencies CharacterCreationValidationSelectorError[]
--- @field field_0 uint64
--- @field field_10 int64
--- @field field_18 int64
--- @field field_20 int64
--- @field field_28 int64
--- @field field_30 int64
--- @field field_38 int64
--- @field field_40 CharacterCreationValidationSelectorError[]
--- @field field_50 CharacterCreationValidationSelectorError[]
--- @field field_60 CharacterCreationValidationSelectorError[]
--- @field field_70 CharacterCreationValidationSelectorError[]
--- @field field_8 int64
--- @field field_80 CharacterCreationValidationSelectorError[]
--- @field field_90 CharacterCreationValidationSelectorError[]
--- @field field_A0 CharacterCreationValidationSelectorError[]


--- @class CharacterCreationValidationSelectorError
--- @field SlotErrors uint8[]


--- @class CharacterCreationDefinitionCreationComponent:BaseComponent
--- @field field_0 EntityHandle[]
--- @field field_10 uint8


--- @class CharacterCreationDefinitionFullRespecComponent:BaseComponent
--- @field Character EntityHandle
--- @field Respec EntityHandle
--- @field field_0 EntityHandle
--- @field field_8 EntityHandle


--- @class CharacterCreationDefinitionLevelUpComponent:BaseComponent
--- @field field_0 EntityHandle
--- @field field_10 uint8
--- @field field_8 EntityHandle


--- @class CharacterCreationDefinitionRespecComponent:BaseComponent
--- @field Character EntityHandle
--- @field Respec EntityHandle
--- @field field_0 EntityHandle
--- @field field_8 EntityHandle


--- @class DeathDeadByDefaultComponent:BaseComponent
--- @field DeadByDefault uint8


--- @class DeathDeathComponent:BaseComponent
--- @field Combat Guid
--- @field DeathType uint8
--- @field LeaveTrace boolean
--- @field Target EntityHandle
--- @field field_10 Guid
--- @field field_21 uint8
--- @field field_22 uint8
--- @field field_23 uint8
--- @field field_24 uint8
--- @field field_28 int32
--- @field field_2C uint8
--- @field field_30 vec3
--- @field field_3C vec3
--- @field field_48 uint8
--- @field field_4C int32
--- @field field_60 uint8
--- @field field_61 uint8
--- @field field_62 uint8
--- @field field_64 vec3?
--- @field field_8 EntityHandle


--- @class DeathDeathStateComponent:BaseComponent
--- @field State uint32


--- @class DeathDeathTypeComponent:BaseComponent
--- @field DeathType uint8


--- @class DeathDownedComponent:BaseComponent
--- @field DownedStatus FixedString
--- @field Entity Guid


--- @class DeathDownedEventOneFrameComponent:BaseComponent


--- @class DlgActiveRollNode:DlgTaggedQuestionNode
--- @field AdvantageReason TranslatedString
--- @field AdvantageReasonReferenceString string
--- @field DetectThoughts boolean
--- @field Flags2 uint8
--- @field Roll DlgDialogRoll


--- @class DlgActorRefCount
--- @field field_0 uint16
--- @field field_2 uint16
--- @field field_4 uint16
--- @field field_8 uint16


--- @class DlgAliasNode:DlgDefaultDialogNode
--- @field OverrideApprovalRatingID boolean
--- @field SourceNode FixedString


--- @class DlgBaseTaggedNode:DlgDefaultDialogNode
--- @field TaggedTexts DlgTaggedText[]


--- @class DlgCinematicTaggedNode:DlgDefaultDialogNode
--- @field BehaviourNodeType uint8


--- @class DlgDefaultDialogNode:DlgDialogNode
--- @field AddressedSpeaker int32
--- @field Speaker int32
--- @field Stub boolean


--- @class DlgDialog
--- @field AllowDeadSpeakers boolean
--- @field Automated boolean
--- @field Category string
--- @field DefaultAddressedSpeakers table<int32, int32>
--- @field DefaultSpeakerIndex int32
--- @field FileName string
--- @field IsAllowingJoinCombat boolean
--- @field IsBehaviour boolean
--- @field IsPrivateDialog boolean
--- @field IsSFXDialog boolean
--- @field IsSubbedDialog boolean
--- @field IsSubsAnonymous boolean
--- @field IsWorld boolean
--- @field Nodes table<FixedString, DlgDialogNode>
--- @field PeanutSlots Set_int32
--- @field RefCount int32
--- @field RootNodes DlgDialogNode[]
--- @field SpeakerGroups string[]
--- @field SpeakerMappingIds Guid[]
--- @field SpeakerTags string[]
--- @field TimelineId FixedString
--- @field UUID FixedString
--- @field UnloadedTimeStamp number
--- @field field_128 int64
--- @field field_130 int64


--- @class DlgDialogGameData
--- @field AiPersonality FixedString[]
--- @field CameraTarget int32
--- @field CustomMovie string
--- @field ExtraWaitTime int32
--- @field LookAt DlgLookAtInfo[]
--- @field OriginOnly FixedString
--- @field OriginTheme FixedString
--- @field OriginThemeAddInstrument boolean
--- @field ParentNode DlgDialogNode
--- @field SoundEvent string


--- @class DlgDialogInstance
--- @field ActiveDialog DlgDialog
--- @field AllowDeadSpeakers boolean
--- @field AutomatedDialog boolean
--- @field CachedIsOnlyPlayers boolean
--- @field CanAttack boolean
--- @field CharacterCreationUser int32
--- @field CullingDistance number
--- @field CurrentNode DlgNodeData
--- @field DialogId int32
--- @field DialogResourceUUID FixedString
--- @field DialogWaitTime number
--- @field Dialogs table<FixedString, DlgDialog>
--- @field Flags uint8
--- @field ForceStopped boolean
--- @field GracefulExitState uint8
--- @field HostHighlightedAnswer int8
--- @field InitialDialogResourceUUID FixedString
--- @field IsAllowingJoinCombat boolean
--- @field IsBehaviour boolean
--- @field IsOnlyPlayers boolean
--- @field IsPaused boolean
--- @field IsPlayerWatchingTimeline boolean
--- @field IsPreview boolean
--- @field IsPrivateDialog boolean
--- @field IsTimelineEnabled boolean
--- @field IsWorld boolean
--- @field LastPlayerSpeakerIndex int32
--- @field LevelStartingDialog uint8
--- @field LoadedSerializedState uint8
--- @field LocalHighlightedAnswer int8
--- @field NodeCustomData table<FixedString, uint32>
--- @field NodeSelection DlgNodeSelectionInfo[]
--- @field OverriddenDialog DlgDialog
--- @field PartyTeleportOnEnd Variant<DlgEmptyTarget,Guid,FixedString>
--- @field PlayedDialogs Set_FixedString
--- @field PlayedNestedDialogs DlgPlayedNestedDialog[]
--- @field PopLevels FixedString[]
--- @field QueuedActors DlgQueuedActor[]
--- @field SmoothExit boolean
--- @field SpeakerLinkingIndex int32
--- @field SpeakerLinkings table<int32, int32>[]
--- @field Speakers EntityHandle[]
--- @field StartPaused boolean
--- @field State int32
--- @field TimeAccumulator int32
--- @field TimelineConfirmed boolean
--- @field TimelineSceneTriggerId Guid
--- @field UniqueSpeakerLinkingIndices int32[]
--- @field UnloadRequested boolean
--- @field UnloadTimeout number
--- @field UseSpellVocals boolean
--- @field VisitedNodes DlgVisitedNode[]
--- @field WasActivated boolean
--- @field WorldHadTriggered boolean
--- @field field_8 int64


--- @class DlgDialogManager
--- @field ActorRefCounts table<EntityHandle, DlgActorRefCount>
--- @field CachedDialogs DlgDialog[]
--- @field DialogToVariable table<int32, Guid>
--- @field DialogVariables table<Guid, DlgVariable>
--- @field Dialogs table<int32, DlgDialogInstance>
--- @field FlagDescriptions table<FixedString, DlgFlagDescription>
--- @field NextInstanceId int32
--- @field PendingInstanceLoads int32[]
--- @field ScriptFlags table<Guid, DlgScriptFlag>
--- @field SpeakerGroups table<Guid, DlgSpeakerGroup>
--- @field field_58 uint8
--- @field field_F8 int32[]


--- @class DlgDialogNode
--- @field ApprovalRatingID Guid
--- @field CheckFlags DlgFlagCollection
--- @field Children FixedString[]
--- @field ConstructorID FixedString
--- @field Flags DialogNodeFlags
--- @field GroupID FixedString
--- @field GroupIndex int32
--- @field ParentDialog DlgDialog
--- @field SetFlags DlgFlagCollection
--- @field Tags Guid[]
--- @field TransitionMode int32
--- @field UUID FixedString
--- @field WaitTime number


--- @class DlgDialogRoll
--- @field Ability AbilityId
--- @field Advantage uint8
--- @field DifficultyClass Guid
--- @field ExcludeCompanionsOptionalBonuses boolean
--- @field ExcludeSpeakerOptionalBonuses boolean
--- @field ExcludedBonusesTags Guid[]
--- @field RollTargetSpeaker int32
--- @field RollType StatsRollType
--- @field Skill SkillId


--- @class DlgEmptyTarget


--- @class DlgFallibleQuestionResultNode:DlgDefaultDialogNode
--- @field Result boolean


--- @class DlgFlag
--- @field Type FixedString
--- @field Uuid Guid
--- @field Value boolean


--- @class DlgFlagCollection
--- @field Flags table<FixedString, DlgFlag[]>


--- @class DlgFlagDescription
--- @field HasFlagParamInfo boolean
--- @field Name FixedString


--- @class DlgJumpNode:DlgDialogNode
--- @field JumpTarget FixedString
--- @field JumpTargetPoint int32


--- @class DlgLookAtInfo
--- @field Speaker int16
--- @field Target int16


--- @class DlgNestedDialogNode:DlgDialogNode
--- @field NestedDialogNodeUUID Guid
--- @field SpeakerLinking table<int32, int32>


--- @class DlgNodeData
--- @field UUID FixedString
--- @field field_10 Guid
--- @field field_4 int32
--- @field field_8 int32
--- @field field_C int32


--- @class DlgNodeSelectionInfo
--- @field LineId int32
--- @field Node DlgDialogNode
--- @field field_C boolean


--- @class DlgPassiveRollNode:DlgDefaultDialogNode
--- @field IsFlatSkillCheck boolean
--- @field Roll DlgDialogRoll


--- @class DlgPlayedNestedDialog
--- @field DialogResourceUuid FixedString
--- @field NestedDialogResourceUuid FixedString


--- @class DlgQueuedActor
--- @field SpeakerId int32
--- @field field_0 EntityHandle
--- @field field_C boolean
--- @field field_D boolean
--- @field field_E boolean
--- @field field_F boolean


--- @class DlgScriptFlag
--- @field KeywordConditionList uint64
--- @field Script string
--- @field Uuid Guid


--- @class DlgSelectSpeakerNode:DlgBaseTaggedNode
--- @field SpeakerId int32


--- @class DlgSpeakerGroup
--- @field OverwriteSpeakerUuid Guid
--- @field Uuid Guid


--- @class DlgTagRule:DlgTagRuleEntry
--- @field CheckedTags Guid[]
--- @field CombineOp int32
--- @field SpeakerIndex int32


--- @class DlgTagRuleEntry


--- @class DlgTagRuleGroup:DlgTagRuleEntry
--- @field ChildRules DlgTagRuleEntry[]
--- @field Operator int32


--- @class DlgTaggedQuestionNode:DlgBaseTaggedNode
--- @field SpeakerId2 int32
--- @field ValidatedFlags DlgFlagCollection?


--- @class DlgTaggedText
--- @field Lines DlgTextLine[]
--- @field RootRule DlgTagRuleGroup


--- @class DlgTextLine
--- @field ActualText TranslatedFSString
--- @field Content string
--- @field CustomSequenceId Guid
--- @field DialogVariables FixedString[]
--- @field HasOverrideText boolean
--- @field LineId Guid
--- @field OldText TranslatedFSString
--- @field Stub boolean
--- @field TagText TranslatedFSString
--- @field field_70 uint8


--- @class DlgTradeNode:DlgDefaultDialogNode
--- @field DefaultToBarter boolean
--- @field ItemsTagFilter string
--- @field PlayerOverride int32


--- @class DlgVariable
--- @field DefaultValue string
--- @field Type uint8
--- @field Uuid Guid


--- @class DlgVisitedNode
--- @field NodePlayed uint8
--- @field field_0 int32
--- @field field_10 int64
--- @field field_18 int32
--- @field field_1C int32
--- @field field_21 uint8
--- @field field_22 uint8
--- @field field_4 int32
--- @field field_8 int64


--- @class DlgVisualStateNode:DlgDialogNode


--- @class EclBaseController
--- @field Owner EntityRef


--- @class EclCameraCombatTargetComponent:BaseComponent
--- @field Target EclCameraTarget


--- @class EclCameraSelectorModeComponent:BaseComponent
--- @field Mode uint8


--- @class EclCameraTarget
--- @field field_0 EntityHandle[]
--- @field field_10 uint8
--- @field field_14 vec3
--- @field field_20 number
--- @field field_24 number
--- @field field_28 uint8
--- @field field_29 uint8
--- @field field_2A uint8
--- @field field_2B uint8
--- @field field_2C uint8
--- @field field_2D uint8


--- @class EclCameraTargetComponent:BaseComponent
--- @field Target EclCameraTarget


--- @class EclCharacter:BaseComponent
--- @field Active boolean
--- @field Airborne boolean
--- @field CanTrade boolean
--- @field Climbing boolean
--- @field ClothVisual Visual
--- @field CustomVisuals boolean
--- @field DialogEffect EntityHandle
--- @field Entity EntityHandle
--- @field Flags ClientCharacterFlags
--- @field Flags2 ClientCharacterFlags2
--- @field Flags3 ClientCharacterFlags3
--- @field FollowCharacter EntityHandle
--- @field HasCharacterLight boolean
--- @field HasCustomVisualAfterDeath boolean
--- @field HasDialog boolean
--- @field InDialog boolean
--- @field InUseByCharacter EntityHandle
--- @field InputController EclInputController
--- @field IsPlayer boolean
--- @field LastTick number
--- @field Level FixedString
--- @field Light EntityHandle
--- @field LootingRequested boolean
--- @field ObscurementIndicationEffect EntityHandle
--- @field OriginalTemplate CharacterTemplate
--- @field Owner EntityHandle
--- @field OwnerUserID UserId
--- @field PickpocketRequested boolean
--- @field Pickpocketing boolean
--- @field PlayerData EclPlayerCustomData
--- @field ReloadingVisuals boolean
--- @field RemovingFromLevel boolean
--- @field ReservedUserID UserId
--- @field SneakCheck number
--- @field StatusManager EclStatusMachine
--- @field SurfacePathInfluences SurfacePathInfluence[]
--- @field Template CharacterTemplate
--- @field VisibleOffStage boolean
--- @field WasInputControllerActive boolean


--- @class EclCharacterDragController:EclDragController
--- @field Active boolean
--- @field AnimationEventTriggered boolean
--- @field Character EntityHandle
--- @field Effect EclEffectHandler
--- @field Enabled boolean
--- @field ErrorFlags uint64
--- @field Outline1 EntityHandle
--- @field Rotation number
--- @field RotationRate number
--- @field TimeSpent number
--- @field TimeSpentSq number
--- @field VisualEntity EntityHandle
--- @field WorldPosition vec3
--- @field WorldRotate vec4
--- @field WorldRotate2 vec4
--- @field field_1A uint8


--- @class EclCharacterIconRenderSystem:BaseSystem
--- @field DeletedIconRequests Set_EntityHandle
--- @field EntityBeingRendered EntityHandle
--- @field Icon ScratchBuffer
--- @field IconRendererSetUp boolean
--- @field IconRequests Set_EntityHandle
--- @field PollResult uint32
--- @field ProcessingStage uint32
--- @field RenderCallback EntityHandle
--- @field RenderState int32
--- @field RenderTimeRemaining number
--- @field SessionCount uint32
--- @field field_140 EntityHandle
--- @field field_148 uint32
--- @field field_CC int32
--- @field field_D0 int32
--- @field field_D4 int32
--- @field field_D9 uint8
--- @field field_DA uint8
--- @field field_DB uint8
--- @field field_DC uint16
--- @field field_DE uint8


--- @class EclCharacterIconRequestComponent:BaseComponent
--- @field ArmorSetState ArmorSetState?
--- @field Equipment FixedString[]
--- @field Template FixedString
--- @field Trigger string
--- @field Visual FixedString
--- @field VisualSet EclVisualSetSlots
--- @field field_190 EntityHandle
--- @field field_1B0 int32


--- @class EclCharacterIconResultComponent:BaseComponent
--- @field Icon ScratchBuffer


--- @class EclCharacterManager:BaseSystem
--- @field ChangedTemplates Set_EntityHandle
--- @field ColorBlindMode uint8
--- @field PauseState boolean
--- @field RefreshEquipmentSlot table<EntityHandle, Set_ItemSlot>
--- @field RefreshLights Set_EntityHandle
--- @field ReloadPhysics Set_EntityHandle
--- @field ReloadVisualSets boolean
--- @field ReloadVisuals Set_EntityHandle
--- @field RestoreCharacters boolean
--- @field TradingModeChanges table<EntityHandle, EclTradeModeChangeRequest>
--- @field UpdateCharacterHighlights Set_EntityHandle[]
--- @field UpdateCharacterHighlights2 Set_EntityHandle[]
--- @field UpdateCombatCamera EntityHandle[]
--- @field UpdateObscurity Set_EntityHandle
--- @field UpdatePhysicsGroup Set_EntityHandle
--- @field UpdateRepose Set_EntityHandle


--- @class EclCharacterMoveTask:EclCharacterTask
--- @field BehaviourUpdated boolean
--- @field CanReachTarget boolean
--- @field EndTargetInfo NavigationTargetInfo
--- @field HasLimitPath boolean
--- @field MoveBehaviourTransactionId int32
--- @field PathDistance number
--- @field PathId int32
--- @field PathLimitDistance number
--- @field PathSettings PathSettings
--- @field PreviewEffects EclPreviewEffectMap
--- @field PreviewPathCalculated boolean
--- @field PreviousPathId int32
--- @field PreviousPathTarget vec3
--- @field PreviousPathTarget2 vec3
--- @field ShouldDrawPreview boolean
--- @field StartTargetInfo NavigationTargetInfo
--- @field TargetCircleColor int32
--- @field TargetCircleScale number
--- @field TargetHighlights EntityHandle[]
--- @field field_14C uint8


--- @class EclCharacterTask
--- @field Character EclCharacter
--- @field CharacterEntity EntityRef
--- @field EndData EclCharacterTaskData
--- @field Flags ClientCharacterTaskFlags
--- @field HighlightTargets EntityHandle[]
--- @field Previewing boolean
--- @field Ready boolean
--- @field StartData EclCharacterTaskData
--- @field TaskInfo EclTaskInfo
--- @field TaskType ClientCharacterTaskType


--- @class EclCharacterTaskData
--- @field Costs StatsActionResourceCost[]
--- @field PreviewPosition vec3
--- @field TotalCosts StatsActionResourceCost[]


--- @class EclCharacterTaskInteractItemData
--- @field CannotInteract boolean
--- @field HasHighlight boolean
--- @field IsReachable boolean


--- @class EclCharacterTaskSpellFloodData
--- @field Position vec3
--- @field Target SpellCastBaseTarget
--- @field field_0 uint16
--- @field field_2 uint16


--- @class EclCharacterTaskSpellResultData
--- @field Position vec3
--- @field Target SpellCastBaseTarget
--- @field field_40 uint8


--- @class EclCharacterTaskClimbTo:EclCharacterMoveTask
--- @field Target EntityHandle


--- @class EclCharacterTaskControllerSelection:EclCharacterTask


--- @class EclCharacterTaskDefaultTargetAction:EclCharacterTask
--- @field ActionExecuted boolean
--- @field Target EntityHandle


--- @class EclCharacterTaskDialog:EclCharacterMoveTask
--- @field Target EntityHandle
--- @field Target2 EntityHandle


--- @class EclCharacterTaskDisarmTrap:EclCharacterMoveTask
--- @field DisarmRequested int32
--- @field DisarmToolkit EntityHandle
--- @field DisarmToolkit2 EntityHandle
--- @field ForceSuccess_M boolean
--- @field Target EntityHandle
--- @field TargetSelected boolean


--- @class EclCharacterTaskDrop:EclCharacterMoveTask
--- @field DropCompleted boolean
--- @field ItemDropRequested boolean
--- @field Items EntityHandle[]
--- @field LastUpdate number
--- @field NeedsPhysicsUpdate boolean
--- @field Position vec3
--- @field Rotation quat
--- @field field_260 int32


--- @class EclCharacterTaskItemCombine:EclCharacterMoveTask
--- @field BehaviourTransactionId int32
--- @field Entered_M boolean
--- @field Items EntityHandle[]
--- @field Position vec3
--- @field field_260 FixedString
--- @field field_278 number
--- @field field_27C uint8


--- @class EclCharacterTaskItemCombineDummy:EclCharacterMoveTask
--- @field HighlightTarget EntityHandle
--- @field Item1 EntityHandle
--- @field Item2 EntityHandle


--- @class EclCharacterTaskItemInteractArea:EclCharacterTask
--- @field AreaCenter vec3
--- @field Effect EntityHandle
--- @field Items table<EntityHandle, EclCharacterTaskInteractItemData>
--- @field UpdateTime number
--- @field field_128 int32


--- @class EclCharacterTaskItemMove:EclCharacterMoveTask
--- @field Amount int32
--- @field BehaviourTransactionId int32
--- @field PlacementStack EntityHandle[]
--- @field TargetLocations vec3[]
--- @field TargetRotations quat[]
--- @field field_288 EntityHandle
--- @field field_294 uint8


--- @class EclCharacterTaskItemMoveDummy:EclCharacterMoveTask
--- @field Item EntityHandle


--- @class EclCharacterTaskItemUse:EclCharacterMoveTask
--- @field BehaviourStarted boolean
--- @field BehaviourTransactionId int32
--- @field Item EntityHandle
--- @field Task EclCharacterTaskItemUse
--- @field field_264 uint8
--- @field field_265 uint8
--- @field field_268 FixedString


--- @class EclCharacterTaskItemUseRemotely:EclCharacterTask
--- @field Action EclCharacterTaskItemUseRemotely
--- @field HighlightTarget EntityHandle


--- @class EclCharacterTaskListen:EclCharacterTask
--- @field ListenTo EntityHandle
--- @field PreviewListenTo EntityHandle


--- @class EclCharacterTaskLockpick:EclCharacterMoveTask
--- @field CanLockpick boolean
--- @field Item EntityHandle
--- @field ItemNetId NetId
--- @field LockpickingStarted boolean
--- @field TargetSelected boolean


--- @class EclCharacterTaskLoot:EclCharacterMoveTask
--- @field Target EntityHandle


--- @class EclCharacterTaskMoveController:EclCharacterTask
--- @field IgnoreMovementUntilStickIsReleased boolean


--- @class EclCharacterTaskMoveInDirection:EclCharacterTask
--- @field field_100 vec3
--- @field field_10C vec3
--- @field field_118 vec3
--- @field field_124 vec3
--- @field field_130 vec3


--- @class EclCharacterTaskMoveTo:EclCharacterMoveTask
--- @field AiFloodId int16
--- @field AlignedTargetPos vec3
--- @field GroundPosition vec3
--- @field HitBound number
--- @field HoverPos2 vec3
--- @field HoverTarget EntityHandle
--- @field MovementType int32
--- @field NextTarget NavigationTargetInfo
--- @field Started boolean
--- @field StepHeight number
--- @field Target NavigationTargetInfo
--- @field TargetEffect EntityHandle
--- @field TargetPos vec3
--- @field TargetReady boolean
--- @field TargetSearchFinished boolean


--- @class EclCharacterTaskPickUp:EclCharacterMoveTask
--- @field Amount int32
--- @field BehaviourTransactionId int32
--- @field Inventory int16
--- @field Item EntityHandle
--- @field Slot int16
--- @field TargetInventoryNetId NetId
--- @field Timeout number
--- @field field_26E uint8


--- @class EclCharacterTaskPickpocket:EclCharacterMoveTask
--- @field HoverCharacter EntityHandle
--- @field TargetCharacter EntityHandle


--- @class EclCharacterTaskSheathUnsheath:EclCharacterTask
--- @field Entered boolean


--- @class EclCharacterTaskSpell:EclCharacterMoveTask
--- @field DirectionToNextNode vec3?
--- @field DistanceXZTotal number?
--- @field Flood EclCharacterTaskSpellFloodData?
--- @field MoveInSpellRangeCache EclMoveInSpellRangeCache
--- @field PlayerId uint16
--- @field PreviewEffectMap EclPreviewEffectMap
--- @field PreviewPosition vec3?
--- @field PreviousSpellTargetingState uint8
--- @field RequestFlags uint8
--- @field Result EclCharacterTaskSpellResultData?
--- @field SourceItem EntityHandle
--- @field Spell SpellId
--- @field SpellCastGuid Guid
--- @field SpellHoverPreviewType uint8
--- @field SpellStateFlags uint8
--- @field SpellTargetingState uint8
--- @field Target SpellCastBaseTarget
--- @field Targets EntityHandle[]


--- @class EclClientTimelineActorControlComponent:BaseComponent
--- @field field_0 Guid
--- @field field_10 Guid
--- @field field_20 uint8
--- @field field_21 uint8
--- @field field_22 uint8


--- @class EclConditionalEffect
--- @field Attached boolean
--- @field CanCreate boolean
--- @field DamageType DamageType
--- @field Effect EntityHandle
--- @field EffectIndex int32
--- @field EntitySlot EclEffectEntitySlot
--- @field SourceBone FixedString
--- @field TargetBone FixedString
--- @field TextKeyEventStarted boolean
--- @field VerbalIntent uint8


--- @class EclCursorControl
--- @field CurrentOverlay CursorType
--- @field CursorName FixedString
--- @field CursorOverrides CursorType[]
--- @field FadeTimer number
--- @field RequestedCursor CursorType
--- @field RequestedOverlay CursorType
--- @field ShowCursor boolean
--- @field VisibleTimer number


--- @class EclDragController
--- @field PlayerId uint16


--- @class EclEffectEntitySlot
--- @field AttachmentName FixedString
--- @field BoneName FixedString


--- @class EclEffectHandler
--- @field ConditionalEffects EclConditionalEffect[]
--- @field EventProviderEntity EntityHandle
--- @field InitInfo EclEffectHandlerInitInfo
--- @field IsActive boolean
--- @field MultiEffectInfo ResourceMultiEffectInfo


--- @class EclEffectHandlerInitInfo
--- @field EnteredDuringSave boolean
--- @field EventProviderEntity EntityHandle
--- @field FXScale number
--- @field MultiEffectDefinition string
--- @field Player EntityHandle
--- @field SourceEffect EclProvidedMultiEffectObject
--- @field TargetEffect EclProvidedMultiEffectObject
--- @field ZoneRange number
--- @field field_70 EclEffectHandlerInitUnknown[]
--- @field field_90 FixedString
--- @field field_9C uint8


--- @class EclEffectHandlerInitUnknown
--- @field field_0 uint64
--- @field field_8 number


--- @class EclEntityStatusData
--- @field HasVFX1 boolean
--- @field HasVFX2 boolean
--- @field VFX EclStatusVFX[]


--- @class EclEquipmentRemoveMaterialRequest
--- @field CheckFlags uint8
--- @field Entity EntityHandle
--- @field Material FixedString
--- @field RemoveOverlayMaterial boolean
--- @field field_C boolean


--- @class EclEquipmentSubVisualRequest
--- @field Processed boolean
--- @field VisualEntity EntityHandle
--- @field VisualTemplate FixedString


--- @class EclEquipmentUnloadRequest
--- @field Item EntityHandle
--- @field Parent EntityHandle
--- @field Slot ItemSlot
--- @field Visual EntityHandle


--- @class EclEquipmentVisual
--- @field Item EntityHandle
--- @field SubVisuals EntityHandle[]
--- @field VisualData EclEquipmentVisualData?
--- @field VisualRequest EclEquipmentVisualRequest
--- @field field_20 boolean


--- @class EclEquipmentVisualData
--- @field AttachFlags uint32
--- @field Bone FixedString
--- @field BoneSheathed FixedString
--- @field Flags_63 uint8
--- @field HairType uint8
--- @field Item EntityHandle
--- @field ScalarParameters ResourcePresetDataScalarParameter[]
--- @field SlotAndFlags uint32
--- @field SourceBone FixedString
--- @field SourceBoneSheathed FixedString
--- @field Vector3Parameters ResourcePresetDataVector3Parameter[]
--- @field VisualFlags uint16
--- @field VisualTemplates FixedString[]
--- @field field_18 FixedString
--- @field field_20 EntityHandle


--- @class EclEquipmentVisualRequest
--- @field Data EclEquipmentVisualData
--- @field Item EntityHandle[]
--- @field SubRequests EclEquipmentSubVisualRequest[]
--- @field field_90 EntityHandle


--- @class EclEquipmentVisualsComponent:BaseComponent
--- @field Entity EntityHandle
--- @field Equipment table<ItemSlot, EclEquipmentVisual>


--- @class EclEquipmentVisualsSystem:BaseSystem
--- @field AnimationSetRequest table<EntityHandle, EclLoadAnimationFromVisualRequestData[]>
--- @field DestroyVisuals Set_EntityHandle
--- @field DyeUpdates Set_EntityHandle
--- @field InitVisualLevel table<EntityHandle, FixedString>
--- @field InventoryEvents table<EntityHandle, table<uint16, InventoryContainerSlotData>>
--- @field PendingSlotLoads table<EntityHandle, uint64>
--- @field RemoveAnimationSetRequest table<EntityHandle, Set_FixedString>
--- @field RemoveMaterials EclEquipmentRemoveMaterialRequest[]
--- @field StreamingCount int32
--- @field UnloadRequests EclEquipmentUnloadRequest[]
--- @field UnloadVisualRequestCount int32
--- @field UpdateSplatter Set_EntityHandle
--- @field VFXTargetRequest table<EntityHandle, EntityHandle>


--- @class EclGameCameraBehavior:BaseComponent
--- @field CameraMode uint8
--- @field DebugOffset number
--- @field DebugPosition vec3
--- @field Direction vec3
--- @field DirectionDestination vec3
--- @field Distance number
--- @field EffectEntities EntityHandle[]
--- @field FollowTarget EntityHandle?
--- @field FreezeHeight boolean
--- @field IsMoving boolean
--- @field IsPaused boolean
--- @field IsRotating boolean
--- @field IsSnapping boolean
--- @field LastPickingTarget EntityHandle
--- @field LastPlayerInputTime number
--- @field MouseRotationSpeed number
--- @field MovementDistanceMax number
--- @field MovingToTarget boolean
--- @field PitchDegrees number
--- @field PlayerInControl boolean
--- @field RotationTarget vec3?
--- @field RotationY number
--- @field SelectMode boolean
--- @field SpeedXZ vec2
--- @field TacticalMode boolean?
--- @field TacticalTimeout number?
--- @field Target EntityHandle
--- @field TargetCurrent vec3
--- @field TargetDestination vec3
--- @field TargetFalling boolean
--- @field TargetLastPosition vec3
--- @field TargetMode uint32
--- @field TargetPreviousDestination vec3
--- @field Targets EntityHandle[]
--- @field TrackTarget EntityHandle
--- @field Trigger EntityHandle
--- @field Trigger1ID int32
--- @field Trigger2ID int32
--- @field Trigger3ID int32
--- @field WasInSelectMode boolean
--- @field XRotationSpeed number
--- @field XRotationSpeedMouse int32
--- @field Zoom number
--- @field ZoomSpeed number
--- @field field_11 boolean
--- @field field_13 boolean
--- @field field_14 number
--- @field field_145 boolean
--- @field field_150 vec3
--- @field field_15C number
--- @field field_188 boolean
--- @field field_18A boolean
--- @field field_18B boolean
--- @field field_1E0 number?
--- @field field_1F2 boolean
--- @field field_200 uint8
--- @field field_208 Set_int16
--- @field field_238 vec3?
--- @field field_248 vec3?
--- @field field_30 vec3
--- @field field_3C vec3
--- @field field_58 number
--- @field field_5C number
--- @field field_60 number
--- @field field_64 number
--- @field field_68 number
--- @field field_88 vec3
--- @field field_A9 uint8
--- @field field_D0 vec2
--- @field field_D8 number
--- @field field_DC number
--- @field field_E0 number
--- @field field_E4 number
--- @field field_E8 number


--- @class EclGameUIDragController:EclDragController
--- @field Enabled boolean


--- @class EclInputController:EclBaseController
--- @field Active boolean
--- @field DragDropEntity EntityHandle
--- @field Flags ClientInputControllerFlags
--- @field IsInSelectorMode_M boolean
--- @field IsMouseHold boolean
--- @field IsMouseHoldMoving boolean
--- @field IsMousePressed boolean
--- @field IsNewTaskStarted boolean
--- @field MouseHoldTime number
--- @field MousePos vec2
--- @field PreviewTask EclCharacterTask
--- @field RequestFlags ClientInputControllerRequestFlags
--- @field RunningTask EclCharacterTask
--- @field Tasks EclCharacterTask[]
--- @field UICursorPreview CursorType
--- @field UICursorRunning CursorType
--- @field field_19 boolean


--- @class EclIsInSelectorModeComponent:BaseComponent


--- @class EclLoadAnimationFromVisualRequestData
--- @field field_0 FixedString
--- @field field_4 FixedString
--- @field field_8 FixedString
--- @field field_C boolean


--- @class EclManagedStatusEffect
--- @field Group Guid
--- @field Type uint8


--- @class EclManagedStatusFXGroup
--- @field References0 int32
--- @field References1 int32
--- @field Type uint8
--- @field VFX EclStatusVFX


--- @class EclManagedStatusFXs
--- @field FXs table<Guid, EclManagedStatusFXGroup>
--- @field field_40 boolean


--- @class EclManagedStatusVFX:EclStatusVFX
--- @field EffectHandler EclEffectHandler
--- @field ManagedStatusEffect EclManagedStatusEffect


--- @class EclMaterialParameterFloat
--- @field Override boolean
--- @field Parameter FixedString
--- @field Preset boolean
--- @field Value number
--- @field field_9 uint8


--- @class EclMaterialParameterPresetSlot
--- @field CCPreset FixedString
--- @field GroupName FixedString
--- @field field_8 uint32


--- @class EclMaterialParameterPresetsContainer
--- @field FloatOverrides EclMaterialParameterFloat[]
--- @field Presets table<FixedString, EclMaterialParameterPresetSlot>
--- @field TextureOverrides EclMaterialParameterTexture[]
--- @field Vec2Overrides EclMaterialParameterVec2[]
--- @field Vec3Overrides EclMaterialParameterVec3[]
--- @field Vec4Overrides EclMaterialParameterVec4[]
--- @field VirtualTextureOverrides EclMaterialParameterTexture[]
--- @field field_60 FixedString


--- @class EclMaterialParameterTexture
--- @field Override boolean
--- @field Parameter FixedString
--- @field Preset boolean
--- @field Value FixedString
--- @field field_9 uint8


--- @class EclMaterialParameterVec2
--- @field Override boolean
--- @field Parameter FixedString
--- @field Preset boolean
--- @field Value vec2
--- @field field_9 uint8


--- @class EclMaterialParameterVec3
--- @field Override boolean
--- @field Parameter FixedString
--- @field Preset boolean
--- @field Value vec3
--- @field field_9 uint8


--- @class EclMaterialParameterVec4
--- @field Override boolean
--- @field Parameter FixedString
--- @field Preset boolean
--- @field Value avec4
--- @field field_9 uint8


--- @class EclMoveInSpellRangeCache
--- @field Solution EclMoveInSpellRangeSolution?
--- @field field_0 uint8
--- @field field_10 int64
--- @field field_128 table<EntityHandle, EclMoveInSpellRangeSolution>
--- @field field_18 uint8
--- @field field_20 table<AiTilePos, number>
--- @field field_4 AiTilePos
--- @field field_60 AiTilePos?
--- @field field_C int32


--- @class EclMoveInSpellRangeSolution
--- @field Target SpellCastInitialTarget
--- @field field_0 vec3
--- @field field_70 int64
--- @field field_78 int64
--- @field field_80 number
--- @field field_94 int32
--- @field field_C int32


--- @class EclPaperdollComponent:BaseComponent
--- @field Combat boolean
--- @field Entity EntityHandle
--- @field field_8 boolean


--- @class EclPlayerCustomData
--- @field ArmorOptionState uint8
--- @field CachedTension uint8
--- @field LootTarget NetId
--- @field MemorizedSpells FixedString[]
--- @field PickpocketTarget NetId
--- @field QuestSelected FixedString


--- @class EclPlayerDragData
--- @field Active boolean
--- @field CharacterDrag EclCharacterDragController
--- @field DraggingAction boolean
--- @field Entities EntityHandle[]
--- @field GameUIDrag EclGameUIDragController
--- @field IsDragging boolean
--- @field ScreenPosition vec2
--- @field WorldDrag EclWorldDragController


--- @class EclPlayerPickingCacheValue
--- @field field_0 int32
--- @field field_4 int32
--- @field field_8 uint8


--- @class EclPlayerPickingHelper
--- @field Activated boolean
--- @field Cache table<EntityHandle, EclPlayerPickingCacheValue>
--- @field ControllerNudge vec2?
--- @field Inner EclPlayerPickingHelperInner
--- @field IsMoving boolean
--- @field PickingDirection vec3
--- @field PlayerId uint16
--- @field SelectableObjects EclPlayerPickingInRangeEntity[]
--- @field Selection EclPlayerPickingInRangeEntity?
--- @field SightCheckCache table<EntityHandle, EclPlayerPickingCacheValue>
--- @field SpellConditionsCache table<EntityHandle, EclPlayerPickingCacheValue>
--- @field TargetOverride vec3?
--- @field UIEntity EntityHandle
--- @field WindowCursorPos vec2
--- @field field_170 EclPlayerPickingInRangeEntity[]
--- @field field_180 EclPlayerPickingInRangeEntity?
--- @field field_1A8 uint8
--- @field field_1A9 uint8
--- @field field_1C0 EntityHandle
--- @field field_210 uint8
--- @field field_268_NextIndex int32
--- @field field_26C uint32
--- @field field_2B0 int32
--- @field field_2B4 int32
--- @field field_2F8 int32


--- @class EclPlayerPickingHelperInner
--- @field AiGridTile uint64
--- @field Direction0 vec3
--- @field Direction1 vec3
--- @field Direction2 vec3
--- @field Fraction number
--- @field Inner EclPlayerPickingHelperInner2[]
--- @field Normal vec3
--- @field PlaceableEntity EntityHandle
--- @field PlaceableFraction number
--- @field PlaceableNormal vec3
--- @field PlaceableObject EntityHandle
--- @field PlaceablePosition vec3
--- @field Position vec3
--- @field Position0 vec3
--- @field Position1 vec3
--- @field Position2 vec3
--- @field SceneryFraction number
--- @field SceneryNormal vec3
--- @field SceneryPosition vec3
--- @field WorldNormal vec3
--- @field WorldPosition vec3
--- @field field_118 vec3
--- @field field_124 vec3
--- @field field_130_EH EntityHandle
--- @field field_138_EH EntityHandle
--- @field field_140_Flags2 uint64
--- @field field_148 number
--- @field field_14C number
--- @field field_150 number
--- @field field_154_Flags uint8


--- @class EclPlayerPickingHelperInner2
--- @field Character EntityHandle
--- @field CharacterPosition vec3
--- @field DeadCharacter EntityHandle
--- @field DeadCharacterPosition vec3
--- @field Flags uint8
--- @field GameObject EntityHandle
--- @field Item EntityHandle
--- @field ItemPosition vec3
--- @field Tooltip EntityHandle
--- @field TooltipPosition vec3
--- @field field_24 vec3
--- @field field_48 EntityHandle
--- @field field_58 EntityHandle


--- @class EclPlayerPickingInRangeEntity
--- @field field_0_Entity EntityHandle
--- @field field_14 number
--- @field field_18 vec3
--- @field field_8_TurnOrder int32?


--- @class EclPreviewEffectList
--- @field Effects EntityHandle[]
--- @field NextEffectIndex uint64


--- @class EclPreviewEffectMap
--- @field Effects table<FixedString, EclPreviewEffectList>
--- @field Updating boolean


--- @class EclProvidedMultiEffectObject
--- @field Entity EntityHandle
--- @field Flags uint32
--- @field Position vec3
--- @field Tags Guid[]


--- @class EclScenery:BaseProxyComponent
--- @field Entity EntityRef
--- @field Entity2 EntityHandle
--- @field Flags SceneryFlags
--- @field Sound EclSceneryInitSound
--- @field Uuid Guid
--- @field Visual FixedString
--- @field VisualLoadFlags uint16


--- @class EclSceneryInitSound
--- @field Attenuation int16
--- @field InitSound FixedString
--- @field LoopSound FixedString


--- @class EclSpellTrackingComponent:BaseComponent


--- @class EclStatus
--- @field Cause EntityRef
--- @field CauseType uint8
--- @field DisableTicking boolean
--- @field Flags ClientStatusFlags
--- @field Flags2 ClientStatusFlags2
--- @field LifeTime number
--- @field NetworkId NetId
--- @field Spell SpellId
--- @field StatusHandle ComponentHandle
--- @field StatusId FixedString
--- @field StillAnimationPriority int32
--- @field StillAnimationType int32
--- @field Target EntityHandle
--- @field TickTime number
--- @field TickTimeLeft number
--- @field TickType uint8
--- @field TickingOwner EntityRef
--- @field UUID Guid


--- @class EclStatusAura:EclStatus
--- @field Aura EclStatusAuraData


--- @class EclStatusAuraData
--- @field FX FixedString
--- @field Radius number


--- @class EclStatusBeamEffectData
--- @field BeamEffect FixedString
--- @field Source EntityHandle


--- @class EclStatusBoost:EclStatusAura
--- @field ApplyEffect FixedString
--- @field BoostEffect EclStatusEffectData
--- @field EffectHandler EclEffectHandler
--- @field EffectOnTurn EclStatusEffectData
--- @field EndEffect FixedString
--- @field Icon FixedString
--- @field ManagedStatusEffect EclManagedStatusEffect
--- @field Material EclStatusVFXBoostMaterialDefinition
--- @field VisualDefinition EclStatusVisualDefinition
--- @field field_12C int32


--- @class EclStatusClimbing:EclStatus
--- @field Ladder EntityHandle
--- @field MoveDirection vec3
--- @field Rotation quat
--- @field State uint8
--- @field field_C0 boolean
--- @field field_C1 uint8
--- @field field_C2 uint8


--- @class EclStatusDeactivated:EclStatusBoost


--- @class EclStatusDowned:EclStatusIncapacitated
--- @field DownedFlags uint8
--- @field Entered uint8
--- @field field_2B8 uint8
--- @field field_2B9 uint8
--- @field field_2BA uint8
--- @field field_2BC int32
--- @field field_2C0 int32
--- @field field_2C4 int32


--- @class EclStatusDying:EclStatus


--- @class EclStatusEffect:EclStatus
--- @field BeamEffect EclStatusBeamEffectData
--- @field Effect EclStatusEffectData
--- @field IsPlayer boolean
--- @field PeaceOnly boolean
--- @field PlayerSameParty boolean
--- @field PlayerTag FixedString


--- @class EclStatusEffectData
--- @field Effect FixedString
--- @field Tags Guid[]


--- @class EclStatusFear:EclStatusBoost


--- @class EclStatusHeal:EclStatus
--- @field Effect EclStatusEffectData
--- @field HealAmount int32
--- @field HealEffect int32
--- @field HealTimer number
--- @field field_A4 int32


--- @class EclStatusInSurface:EclStatusBoost


--- @class EclStatusIncapacitated:EclStatusBoost
--- @field field_2B0 number
--- @field field_2B4 number


--- @class EclStatusInvisible:EclStatusBoost


--- @class EclStatusKnockedDown:EclStatus
--- @field Effect EclStatusEffectData


--- @class EclStatusMachine
--- @field ExternalStatuses EclStatus[]
--- @field ManagedFX table<EntityHandle, EclManagedStatusFXs>
--- @field StatusFX table<EntityHandle, EclEntityStatusData>
--- @field Statuses EclStatus[]


--- @class EclStatusMaterial:EclStatus
--- @field VFX EclStatusVFXData


--- @class EclStatusMaterialParam
--- @field Param FixedString
--- @field Value FixedString


--- @class EclStatusPolymorphed:EclStatusBoost


--- @class EclStatusReaction:EclStatus
--- @field ReactionSource EntityRef
--- @field ReactionTarget EntityRef
--- @field field_B8 uint8


--- @class EclStatusRotate:EclStatus
--- @field field_98 number
--- @field field_9C number


--- @class EclStatusSmelly:EclStatus
--- @field Effect EclStatusEffectData
--- @field Material EclStatusVFXData
--- @field field_C0 int64


--- @class EclStatusSneaking:EclStatusBoost
--- @field Effect EntityHandle
--- @field IsPlayer boolean
--- @field Obscurity uint8
--- @field SneakingFailed boolean
--- @field SneakingFailed2 boolean


--- @class EclStatusStoryFrozen:EclStatus


--- @class EclStatusTeleportFalling:EclStatus
--- @field EffectHandler EclEffectHandler
--- @field FallTimer number
--- @field TeleportSpell SpellId
--- @field field_98 vec3
--- @field field_D0 uint8


--- @class EclStatusUnlock:EclStatus
--- @field State int32


--- @class EclStatusVFX
--- @field Status ComponentHandle


--- @class EclStatusVFXApplyEffect:EclStatusVFX
--- @field Effect FixedString
--- @field EffectHandler EclEffectHandler


--- @class EclStatusVFXAura:EclStatusVFX
--- @field Aura EclStatusAuraData
--- @field Effect EntityHandle


--- @class EclStatusVFXBeam:EclStatusVFX
--- @field Beam EclStatusBeamEffectData
--- @field Created boolean
--- @field EffectHandler EclEffectHandler
--- @field Initialized boolean


--- @class EclStatusVFXBoostMaterial:EclStatusVFX
--- @field Created boolean
--- @field Material EclStatusVFXBoostMaterialDefinition
--- @field MaterialType int32
--- @field MaterialUUID FixedString


--- @class EclStatusVFXBoostMaterialDefinition
--- @field ApplyFlags uint8
--- @field ApplyNormalMap uint8
--- @field FadeAmount number
--- @field Material FixedString
--- @field MaterialParams EclStatusMaterialParam[]
--- @field MaterialType uint32
--- @field OverlayOffset number
--- @field field_14 int32
--- @field field_28 int32
--- @field field_2C number
--- @field field_30 int32


--- @class EclStatusVFXData
--- @field ApplyFlags uint8
--- @field ApplyNormalMap boolean
--- @field FadeTime number
--- @field Fading boolean
--- @field Force boolean
--- @field IsOverlayMaterial boolean
--- @field Material FixedString


--- @class EclStatusVFXEffect:EclStatusVFX
--- @field Effect EclStatusEffectData
--- @field EffectHandler EclEffectHandler


--- @class EclStatusVFXMaterial:EclStatusVFX
--- @field Created boolean
--- @field VFX EclStatusVFXData


--- @class EclStatusVisual:EclStatusVFX
--- @field Definition EclStatusVisualDefinition
--- @field Entity EntityHandle
--- @field FadeOut number
--- @field State uint8


--- @class EclStatusVisualDefinition
--- @field FadeIn number
--- @field FadeOut number
--- @field Visuals table<FixedString, FixedString[]>
--- @field field_0 uint64


--- @class EclTLPreviewDummy:BaseProxyComponent
--- @field ActiveDummy boolean
--- @field ArmorSetState ArmorSetState?
--- @field ArmorVisibilityControlled boolean
--- @field ArmorVisibilityControlled2 boolean
--- @field ClothCollisionResourceIds FixedString[]
--- @field ComponentLayoutFlags uint32
--- @field CurrentLevel FixedString
--- @field Entity EntityRef
--- @field Flags uint16
--- @field IsWeaponUnsheathed boolean
--- @field LightChannel uint8
--- @field Listener EntityHandle
--- @field Name string
--- @field OriginalCharacterTemplate FixedString
--- @field PhysicsResourceId FixedString
--- @field ShapeShiftSourceTemplateOverride FixedString
--- @field ShouldShowVanityWeapon boolean
--- @field SourceTemplateOverride FixedString
--- @field SpringCollisionResourceIds FixedString[]
--- @field TimelineType uint8
--- @field VisibilityFlags uint32
--- @field VisualResourceId FixedString
--- @field field_30 EntityHandle
--- @field field_98 FixedString[]
--- @field field_AC uint8
--- @field field_AD uint8
--- @field field_C2 uint8
--- @field field_C4 uint8


--- @class EclTaskInfo
--- @field APDescription EntityHandle
--- @field Error uint64
--- @field HP int32
--- @field HasSourceName_M boolean
--- @field IconType uint8
--- @field MaximumTotalTargetHP int32
--- @field NumberOfTargets int32
--- @field SourceName TranslatedString
--- @field Succeeded boolean
--- @field TaskType int32
--- @field Texts TextLine[]
--- @field field_10 number
--- @field field_14 boolean
--- @field field_16 boolean
--- @field field_28 EntityHandle
--- @field field_40 uint8
--- @field field_48 int32


--- @class EclTaskPreviewInfo
--- @field SpellColor int32
--- @field TargetingType int32


--- @class EclTradeModeChangeRequest
--- @field field_0 uint16
--- @field field_2 uint8


--- @class EclVisualLocatorAttachment
--- @field DisplayName FixedString
--- @field LocatorName FixedString


--- @class EclVisualSetSlot
--- @field Slot FixedString
--- @field Visual FixedString
--- @field field_8 FixedString


--- @class EclVisualSetSlots
--- @field BodySetVisual FixedString
--- @field LocatorAttachments EclVisualLocatorAttachment[]
--- @field MaterialOverrides table<FixedString, FixedString>
--- @field MaterialParameters EclMaterialParameterPresetsContainer
--- @field MaterialRemaps table<FixedString, FixedString>
--- @field Materials table<FixedString, EclMaterialParameterPresetsContainer>
--- @field ShowEquipmentVisuals boolean
--- @field VisualSet FixedString
--- @field VisualSlots EclVisualSetSlot[]


--- @class EclVisualSystem:BaseSystem
--- @field ChangeSplatterState table<EntityHandle, uint32>
--- @field ReloadAllVisuals boolean
--- @field ReloadVisuals table<EntityHandle, boolean>
--- @field SplatterType table<EntityHandle, uint32>


--- @class EclWorldDragController:EclDragController
--- @field Active boolean
--- @field DraggingEntities EntityHandle[]
--- @field Enabled boolean
--- @field ErrorFlags uint64
--- @field Outline EntityHandle
--- @field PathId int32
--- @field PathId2 int32
--- @field Rotation number
--- @field RotationRate number
--- @field State int32
--- @field TimeSpent number
--- @field TimeSpentSq number
--- @field Transforms Transform[]
--- @field Visuals EntityHandle[]
--- @field field_1A uint8
--- @field field_1B uint8
--- @field field_60 number


--- @class EclCharacterCreationBaseDefinitionComponent:BaseComponent
--- @field Definition CharacterCreationCharacterDefinition


--- @class EclCharacterCreationChangeAppearanceDefinitionComponent:BaseComponent
--- @field Definition CharacterCreationChangeAppearanceDefinitionBase


--- @class EclCharacterCreationCompanionDefinitionComponent:BaseComponent
--- @field Visual CharacterCreationAppearance
--- @field field_0 Guid
--- @field field_10 Guid
--- @field field_110 Guid
--- @field field_120 Guid
--- @field field_130 uint8
--- @field field_20 uint8
--- @field field_21 uint8
--- @field field_28 Guid


--- @class EclCharacterCreationDefinitionStateComponent:BaseComponent
--- @field ChangeId uint32
--- @field Entity EntityHandle
--- @field Errors CharacterCreationValidationError
--- @field field_0 EntityHandle
--- @field field_10 uint32
--- @field field_8 uint32
--- @field field_C uint32


--- @class EclCharacterCreationDefinitionStateExComponent:BaseComponent
--- @field ChangeId uint32
--- @field NeedsSync boolean
--- @field field_0 uint8
--- @field field_1 uint8
--- @field field_18 uint32
--- @field field_1C uint32
--- @field field_2 uint8
--- @field field_20 boolean


--- @class EclCharacterCreationDummyDefinitionComponent:BaseComponent
--- @field BodyShape uint8
--- @field BodyType uint8
--- @field ChangeId int32
--- @field ChangeId2 int32
--- @field Dummy EntityHandle
--- @field Origin Guid
--- @field Race Guid
--- @field RootTemplate Guid
--- @field Subrace Guid
--- @field Visual CharacterCreationAppearance
--- @field field_0 EntityHandle
--- @field field_10 int32
--- @field field_18_Map_unk_FS table<uint64, FixedString>
--- @field field_1A0 uint8
--- @field field_1A8 EntityHandle
--- @field field_58 uint8
--- @field field_59 uint8
--- @field field_5A uint8
--- @field field_5B uint8
--- @field field_60 Guid
--- @field field_70 Guid
--- @field field_8 int32
--- @field field_80 uint8
--- @field field_81 uint8
--- @field field_88 Guid
--- @field field_98 Guid
--- @field field_C int32


--- @class EclCharacterCreationFullRespecDefinitionComponent:BaseComponent
--- @field LevelUpUpgrades LevelUpUpgrades
--- @field field_0 Guid
--- @field field_10 Guid
--- @field field_20 int32


--- @class EclCharacterCreationLevelUpDefinitionComponent:BaseComponent
--- @field Definition CharacterCreationLevelUpDefinitionExtra


--- @class EclEquipmentEquipmentSlotVisualState
--- @field Item EntityHandle
--- @field field_8 uint8
--- @field field_9 uint8


--- @class EclEquipmentVisualsDesiredStateComponent:BaseComponent
--- @field Slots table<ItemSlot, EclEquipmentVisualData>


--- @class EclEquipmentVisualsVisibilityStateComponent:BaseComponent
--- @field Equipment table<ItemSlot, EclEquipmentEquipmentSlotVisualState>
--- @field field_40 uint8
--- @field field_41 uint8


--- @class EclEquipmentVisualsVisibilityStateSystem:BaseSystem
--- @field SetVisualState table<EntityHandle, boolean>
--- @field UnloadVisuals Set_EntityHandle


--- @class EclInterruptPlayerDecisionComponent:BaseComponent
--- @field Decisions table<EntityHandle, uint8>


--- @class EclLuaControllerAxisEvent:LuaEventBase
--- @field Axis SDLControllerAxis
--- @field DeviceId int32
--- @field Value number


--- @class EclLuaControllerButtonEvent:LuaEventBase
--- @field Button SDLControllerButton
--- @field DeviceId int32
--- @field Event SDLKeyEvent
--- @field Pressed boolean


--- @class EclLuaGameStateChangedEvent:LuaEventBase
--- @field FromState ClientGameState
--- @field ToState ClientGameState


--- @class EclLuaKeyInputEvent:LuaEventBase
--- @field Event SDLKeyEvent
--- @field Key SDLScanCode
--- @field Modifiers SDLKeyModifier
--- @field Pressed boolean
--- @field Repeat boolean


--- @class EclLuaMouseButtonEvent:LuaEventBase
--- @field Button int32
--- @field Clicks int32
--- @field Pressed boolean
--- @field X int32
--- @field Y int32


--- @class EclLuaMouseWheelEvent:LuaEventBase
--- @field ScrollX number
--- @field ScrollY number
--- @field X int32
--- @field Y int32


--- @class EclLuaViewportResizedEvent:LuaEventBase
--- @field Height int32
--- @field Width int32


--- @class EcsECSChangeLog
--- @field Entities table<EntityHandle, EcsECSEntityLog>


--- @class EcsECSComponentLog
--- @field Create boolean
--- @field Destroy boolean
--- @field Name StringView
--- @field OneFrame boolean
--- @field Replicate boolean
--- @field ReplicatedComponent boolean
--- @field Type ExtComponentType?


--- @class EcsECSEntityLog
--- @field Components table<uint16, EcsECSComponentLog>
--- @field Create boolean
--- @field Dead boolean
--- @field Destroy boolean
--- @field Entity EntityHandle
--- @field Ignore boolean
--- @field Immediate boolean


--- @class EocBackgroundGoalRecord
--- @field CategoryId FixedString
--- @field Entity Guid
--- @field Goal Guid
--- @field field_28 uint64


--- @class EocBackgroundGoalsComponent:BaseComponent
--- @field Goals table<Guid, EocBackgroundGoalRecord[]>


--- @class EocCalendarDaysPassedComponent:BaseComponent
--- @field Days int32


--- @class EocCalendarStartingDateComponent:BaseComponent
--- @field Day int32
--- @field Year int32


--- @class EocCombatCombatPosition
--- @field Entity EntityHandle
--- @field field_14 number
--- @field field_18 number
--- @field field_8 vec3


--- @class EocCombatDelayedFanfareComponent:BaseComponent


--- @class EocCombatIsCombatPausedComponent:BaseComponent
--- @field PauseSourceTypes uint8
--- @field Paused uint8


--- @class EocCombatIsInCombatComponent:BaseComponent


--- @class EocCombatIsThreatenedComponent:BaseComponent
--- @field ThreatenedBy EntityHandle[]


--- @class EocCombatParticipantComponent:BaseComponent
--- @field AiHint Guid
--- @field CombatGroupId FixedString
--- @field CombatHandle EntityHandle
--- @field Flags CombatParticipantFlags
--- @field InitiativeRoll int32
--- @field field_C int32


--- @class EocCombatStateComponent:BaseComponent
--- @field Initiatives table<EntityHandle, int32>
--- @field IsInNarrativeCombat boolean
--- @field Level FixedString
--- @field MyGuid Guid
--- @field Participants EntityHandle[]
--- @field field_98 EntityHandle
--- @field field_A0 EntityHandle
--- @field field_AC uint8
--- @field field_B0 number
--- @field field_B8 EocCombatCombatPosition[]
--- @field field_D0 uint8


--- @class EocCombatThreatRangeComponent:BaseComponent
--- @field field_0 number
--- @field field_4 int32
--- @field field_8 number


--- @class EocCombatTurnBasedComponent:BaseComponent
--- @field ActedThisRoundInCombat boolean
--- @field CanActInCombat boolean
--- @field CanAct_M boolean
--- @field Combat Guid
--- @field CombatTeam Guid
--- @field EndTurnHoldTimer number?
--- @field HadTurnInCombat boolean
--- @field IsActiveCombatTurn boolean
--- @field IsInCombat_M boolean
--- @field PauseTimer number?
--- @field Removed boolean
--- @field RequestedEndTurn boolean
--- @field Timeout number?
--- @field TurnActionsCompleted boolean
--- @field field_18 number?
--- @field field_20 number?
--- @field field_28 number?
--- @field field_A boolean
--- @field field_C boolean


--- @class EocCombatTurnBasedEntityInfo
--- @field Entity EntityHandle
--- @field Initiative int32


--- @class EocCombatTurnBasedGroup
--- @field Handles EocCombatTurnBasedEntityInfo[]
--- @field Initiative int32
--- @field IsPlayer boolean
--- @field Members EocCombatTurnBasedEntityInfo[]
--- @field Participant Guid
--- @field Round uint32
--- @field Team Guid
--- @field field_28 uint32
--- @field field_30 boolean


--- @class EocCombatTurnOrderComponent:BaseComponent
--- @field Groups EocCombatTurnBasedGroup[]
--- @field Groups2 EocCombatTurnBasedGroup[]
--- @field Participants EocCombatTurnBasedGroup[]
--- @field Participants2 EocCombatTurnBasedGroup[]
--- @field TurnOrderIndices uint64[]
--- @field TurnOrderIndices2 uint64[]
--- @field field_40 int32
--- @field field_44 int32
--- @field field_48 int32
--- @field field_4C number


--- @class EocExpAvailableLevelComponent:BaseComponent
--- @field Level int32


--- @class EocExpCanLevelUpComponent:BaseComponent


--- @class EocExpExperienceComponent:BaseComponent
--- @field CurrentLevelExperience int32
--- @field NextLevelExperience int32
--- @field TotalExperience int32
--- @field field_28 uint8


--- @class EocFtbIsFtbPausedComponent:BaseComponent


--- @class EocFtbIsInFtbComponent:BaseComponent


--- @class EocFtbParticipantComponent:BaseComponent
--- @field field_18 EntityHandle


--- @class EocFtbZoneBlockReasonComponent:BaseComponent
--- @field Reason uint8


--- @class EocHotbarBar
--- @field Elements EocHotbarSlot[]
--- @field Height uint32
--- @field Index uint8
--- @field Width uint8
--- @field field_1 uint8
--- @field field_20 string


--- @class EocHotbarContainerComponent:BaseComponent
--- @field ActiveContainer FixedString
--- @field Containers table<FixedString, EocHotbarBar[]>


--- @class EocHotbarHotbarCurrentDecksComponent:BaseComponent
--- @field Decks table<FixedString, int32>


--- @class EocHotbarSlot
--- @field IsNew boolean
--- @field Item EntityHandle
--- @field Passive FixedString
--- @field Slot uint32
--- @field SpellId SpellId


--- @class EocImprovisedWeaponCanBeWieldedComponent:BaseComponent


--- @class EocImprovisedWeaponWieldedComponent:BaseComponent
--- @field Wielder EntityHandle
--- @field field_8 uint8
--- @field field_9 uint8


--- @class EocImprovisedWeaponWieldingComponent:BaseComponent
--- @field Weapon EntityHandle


--- @class EocPickupPickUpExecutingComponent:BaseComponent


--- @class EocPickupPickUpRequestComponent:BaseComponent
--- @field State uint8
--- @field field_0 Guid


--- @class EocRestLongRestInScriptPhase:BaseComponent


--- @class EocRestLongRestStateComponent:BaseComponent
--- @field CancelReason RestCancelReason
--- @field FinishConfirmed boolean
--- @field Finishing boolean
--- @field Requester EntityHandle
--- @field State LongRestState
--- @field WaitingForOthers boolean
--- @field field_8 FixedString


--- @class EocRestLongRestTimeline:BaseComponent
--- @field Timeline Guid


--- @class EocRestLongRestTimers:BaseComponent
--- @field field_0 int32


--- @class EocRestLongRestUsers:BaseComponent
--- @field FinishConfirmation Set_UserId
--- @field RequestRestore boolean
--- @field States table<UserId, LongRestUserState>


--- @class EocRestRestingEntities:BaseComponent
--- @field HasSurfaces boolean
--- @field Party Set_EntityHandle
--- @field RestRequested Set_EntityHandle
--- @field RestStarted Set_EntityHandle
--- @field ScriptFinished Set_EntityHandle


--- @class EocRestShortRestComponent:BaseComponent


--- @class EsvAIHintAreaTrigger:EsvAreaTriggerBase
--- @field IsAnchor boolean


--- @class EsvASAnimation:EsvActionState
--- @field CancelRequested boolean
--- @field EndAnimations FixedString[]
--- @field LastTime number
--- @field LoopAnimations FixedString[]
--- @field RequestCancel boolean
--- @field RequestCancel2 boolean
--- @field RequestCancelAction2 boolean
--- @field ShapeshiftFinishFired boolean
--- @field StartAnimations FixedString[]
--- @field Time number
--- @field field_70 uint8
--- @field field_76 uint8


--- @class EsvASCombineItem:EsvASMoveBase
--- @field Ingredients EntityHandle[]
--- @field Recipe FixedString
--- @field TimeRemaining number
--- @field field_5C number
--- @field field_64 uint8
--- @field field_65 uint8


--- @class EsvASIncapacitated:EsvActionState
--- @field Duration number
--- @field EndAnimation FixedString
--- @field FinishEventFired boolean
--- @field LoopAnimation FixedString
--- @field MaxAnimationPriority int32
--- @field StartAnimation FixedString
--- @field State int32
--- @field StateChanged boolean
--- @field StatusRemoved boolean
--- @field field_44 int32
--- @field field_59 uint8


--- @class EsvASKnockedDown:EsvASMoveBase
--- @field MaxAnimationPriority int32
--- @field Prone boolean
--- @field RequestGenomeEvent boolean
--- @field RequestedState uint8
--- @field State uint8
--- @field field_42 uint8


--- @class EsvASMoveBase:EsvActionState
--- @field OsirisRequestTimer number
--- @field StoryRequestId int32
--- @field field_38 uint8
--- @field field_40 uint8


--- @class EsvASMoveItem:EsvASMoveBase
--- @field Amount int32
--- @field ItemsToMove EntityHandle[]
--- @field MoveFinished boolean
--- @field MoveTimes number[]
--- @field MoveToPositions vec3[]
--- @field Rotations quat[]


--- @class EsvASTeleportFalling:EsvActionState
--- @field Cause EntityHandle
--- @field HasDamageBeenApplied boolean
--- @field IsFromItem boolean
--- @field Spell SpellId
--- @field StoryActionId int32
--- @field TickTime number
--- @field field_38 uint8
--- @field field_39 uint8


--- @class EsvASUseItem:EsvASMoveBase
--- @field ActionType uint8
--- @field ClimbDuration number
--- @field InUse boolean
--- @field IsInteraction boolean
--- @field Item EntityHandle
--- @field UseAction IActionData
--- @field UseCompleted boolean
--- @field UseDuration2 number
--- @field UseFailed boolean
--- @field UseItem boolean
--- @field UseStartRequested boolean
--- @field UseTimer number
--- @field field_62 uint8
--- @field field_64 FixedString


--- @class EsvActionLayer
--- @field Force boolean
--- @field HasValidState boolean
--- @field NeedsSync boolean
--- @field State EsvActionState
--- @field SyncState EsvActionState


--- @class EsvActionMachine
--- @field Entering boolean[]
--- @field Entity EntityHandle
--- @field ExitRequested boolean[]
--- @field Layers EsvActionLayer[]


--- @class EsvActionResourceChangeResultsSingletonComponent:BaseComponent
--- @field Results EntityHandle[]


--- @class EsvActionResourceSystem:BaseSystem
--- @field ActionResourceBoosts table<EntityHandle, ActionResourceQuery>
--- @field IncreaseInspirationResource table<EntityHandle, InspirationPointGainedRequest[]>
--- @field RefillResources ActionResourceRefillRequest[]
--- @field RefillResources2 ActionResourceRefillRequest[]
--- @field SetResourceValue table<EntityHandle, ActionResourceSetValueRequest[]>
--- @field UpdatePartyResources PartyResourceUpdateRequest[]


--- @class EsvActionState
--- @field ActionMachine EsvActionMachine
--- @field Finished boolean
--- @field Originator ActionOriginator
--- @field SyncDataDirty boolean
--- @field TransactionId int32
--- @field Type ServerActionType
--- @field field_16 uint8


--- @class EsvActivationGroupContainerComponent:BaseComponent
--- @field Groups EsvActivationGroupData[]


--- @class EsvActivationGroupData
--- @field field_0 FixedString
--- @field field_4 FixedString


--- @class EsvAiGridAreaTrigger:EsvAreaTriggerBase


--- @class EsvAnubisExecutorComponent:BaseProxyComponent
--- @field field_10 uint8
--- @field field_11 uint8


--- @class EsvAnubisTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class EsvAreaTriggerBase:AreaTrigger
--- @field Entity EntityHandle


--- @class EsvAtmosphereTrigger:AtmosphereTrigger
--- @field Entity EntityHandle


--- @class EsvAura
--- @field EntityAuraStatuses EsvEntityAuraStatus[]
--- @field Flags uint8
--- @field Functors StatsFunctors
--- @field Radius number
--- @field Source EntityRef
--- @field SpellCastSourceUuid Guid
--- @field StoryActionId int32


--- @class EsvAvatarContainerTrigger:EsvPointTriggerBase


--- @class EsvBSAppear:EsvBehaviourState
--- @field AppearActionTransactionId int32
--- @field Finished boolean
--- @field SpawnEffect FixedString


--- @class EsvBSFollowCharacter:EsvBehaviourState
--- @field BehaviourTransactionId int32
--- @field FollowCharacter EntityHandle
--- @field FollowParty boolean
--- @field IsRegisteredToPartyGroup boolean


--- @class EsvBSForce:EsvBehaviourState
--- @field Action EsvActionState
--- @field ActionLayer boolean
--- @field HasAction boolean
--- @field HasMovement boolean
--- @field HasSteering boolean
--- @field Movement EsvMovementState
--- @field MovementLayer int32
--- @field Steering EsvSteeringState
--- @field SteeringLayer int32


--- @class EsvBSMove:EsvBehaviourState
--- @field ActionTransactionId int32
--- @field Checkpoints EsvBSMoveCheckpoint[]
--- @field DangerousAuraAvoidance uint8
--- @field Flags uint8
--- @field LadderUsePosition vec3
--- @field MovementTransactionId int32
--- @field PathDistanceXZ number
--- @field PathId int32
--- @field PathId2 int32
--- @field PathingSettings PathSettings
--- @field PathingSettings2 PathSettings
--- @field SearchHorizon uint16
--- @field State int32
--- @field SurfaceAvoidance uint32
--- @field TargetInfo NavigationTargetInfo
--- @field UseItem EntityHandle


--- @class EsvBSMoveAndUseItem:EsvBSMoveAndX
--- @field Flags2 uint8
--- @field IsInteraction boolean
--- @field Item EntityHandle
--- @field ItemActionTransactionId int32
--- @field Use boolean


--- @class EsvBSMoveAndX:EsvBSMove
--- @field ActionPathSettings PathSettings
--- @field ActionSearchHorizon uint16
--- @field field_102 boolean


--- @class EsvBSMoveCheckpoint:AiPathCheckpoint
--- @field field_30 int32
--- @field field_34 int32


--- @class EsvBSMoveItem:EsvBSMoveAndX
--- @field Amount int32
--- @field Failed boolean
--- @field Item EntityHandle
--- @field ItemActionTransactionId int32
--- @field MoveToPosition vec3
--- @field SteeringTransactionId int32


--- @class EsvBSMoveToAndTalk:EsvBSMoveAndX
--- @field DialogInstance FixedString
--- @field DialogRequested boolean
--- @field Failed boolean
--- @field Speed FixedString
--- @field TalkTo EntityHandle
--- @field TimeSpent number
--- @field Timeout number


--- @class EsvBSPatrol:EsvBehaviourState


--- @class EsvBSPickupItem:EsvBSMoveAndX
--- @field Amount int32
--- @field FailReason uint8
--- @field Failed boolean
--- @field Inventory EntityHandle
--- @field Item EntityHandle
--- @field Slot int16
--- @field field_118 Guid
--- @field field_130 uint8


--- @class EsvBSWander:EsvBehaviourState
--- @field Anchor Variant<EntityHandle,vec3>
--- @field Failed boolean
--- @field MovementTransactionId int32
--- @field Range number
--- @field Target EntityHandle
--- @field TargetPosition vec3
--- @field TimeRemaining number
--- @field WanderPathSettings PathSettings


--- @class EsvBaseController
--- @field Owner EntityRef


--- @class EsvBaseDataComponent:BaseComponent
--- @field Flags uint32
--- @field Resistances ResistanceBoostFlags[][]
--- @field Weight int32


--- @class EsvBaseProficiencyComponent:BaseComponent
--- @field Proficiency FixedString
--- @field ProficiencyGroup ProficiencyGroupFlags


--- @class EsvBaseSizeComponent:BaseComponent
--- @field GameSize uint8
--- @field SoundSize uint8


--- @class EsvBaseStatsComponent:BaseComponent
--- @field Initiative int32


--- @class EsvBaseWeaponComponent:BaseComponent
--- @field DamageList EsvBaseWeaponDamage[]


--- @class EsvBaseWeaponDamage
--- @field DamageType DamageType
--- @field Roll RollDefinition
--- @field field_10 uint8


--- @class EsvBehaviourMachine
--- @field Entity EntityHandle
--- @field Layers EsvBehaviourState[]


--- @class EsvBehaviourState
--- @field BehaviourMachine EsvBehaviourMachine
--- @field TransactionId int32
--- @field Type ServerBehaviourType


--- @class EsvBlockBronzeTimelinePlacementTrigger:EsvAreaTriggerBase


--- @class EsvBoostTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class EsvBreadcrumbComponent:BaseComponent
--- @field field_118 vec3
--- @field field_18 EsvBreadcrumbEvent[]


--- @class EsvBreadcrumbEvent
--- @field CompletedPosition vec3
--- @field Index int32
--- @field Position vec3
--- @field field_4 uint8


--- @class EsvCampChestTrigger:EsvPointTriggerBase
--- @field Index uint8


--- @class EsvCampRegionTrigger:EsvAreaTriggerBase


--- @class EsvCapabilitiesSystem:BaseSystem
--- @field CanBeDisarmedChanged Set_EntityHandle
--- @field CanBeLootedChanged Set_EntityHandle
--- @field CanDeflectProjectilesChanged Set_EntityHandle
--- @field CanDoActionsChanged Set_EntityHandle
--- @field CanDoLongRestChanged Set_EntityHandle
--- @field CanInteractChanged Set_EntityHandle
--- @field CanModifyHealthChanged Set_EntityHandle
--- @field CanMoveChanged Set_EntityHandle
--- @field CanShortRestChanged Set_EntityHandle
--- @field CanSpeakChanged Set_EntityHandle
--- @field CanTravelChanged Set_EntityHandle
--- @field CanTriggerRandomCastChanged Set_EntityHandle
--- @field FleeCapability table<EntityHandle, int32>
--- @field GlobalRestState EsvGlobalRestStateRequest[]
--- @field InvulnerableChanged Set_EntityHandle
--- @field IsRunning boolean
--- @field RestCapabilityChanged Set_EntityHandle
--- @field ShortRestPointsUpdated Set_EntityHandle
--- @field StatusesChanged Set_EntityHandle
--- @field StoryShortRestState table<EntityHandle, EsvStoryShortRestStateRequest>
--- @field TemplateChanged Set_EntityHandle
--- @field UpdateLongRestState boolean
--- @field UpdateShortRestState boolean
--- @field field_58A boolean
--- @field field_58B boolean
--- @field qword478 Set_EntityHandle
--- @field qword4A8 Set_EntityHandle


--- @class EsvChangeSurfaceOnPathAction:EsvCreateSurfaceActionBase
--- @field CheckExistingSurfaces boolean
--- @field FollowHandle EntityRef
--- @field IgnoreIrreplacableSurfaces boolean
--- @field IgnoreOwnerCells boolean
--- @field IsFinished boolean
--- @field Radius number


--- @class EsvCharacter:BaseComponent
--- @field Activated boolean
--- @field AiActionMachine EsvActionMachine
--- @field AiBehaviourMachine EsvBehaviourMachine
--- @field AiMovementMachine EsvMovementMachine
--- @field AiSteeringMachine EsvSteeringMachine
--- @field BaseVisual FixedString
--- @field BlockNewDisturbanceReactions uint8
--- @field CanGossip boolean
--- @field CanShootThrough boolean
--- @field CannotAttachToGroups boolean
--- @field CannotDie boolean
--- @field CannotMove boolean
--- @field CannotRun boolean
--- @field CharCreationInProgress boolean
--- @field Character EsvCharacter
--- @field CharacterControl boolean
--- @field CharacterCreationFinished boolean
--- @field CoverAmount boolean
--- @field CreatedTemplateItems FixedString[]
--- @field CrimeHandle int32
--- @field CrimeState uint8
--- @field CrimeWarningsEnabled boolean
--- @field CustomLookEnabled boolean
--- @field CustomTradeTreasure FixedString
--- @field Deactivated boolean
--- @field Dead boolean
--- @field DeferredRemoveEscapist boolean
--- @field Detached boolean
--- @field Dialog int32
--- @field DisableCulling boolean
--- @field DisableWaypointUsage boolean
--- @field DisabledCrime FixedString[]
--- @field DoNotFaceFlag boolean
--- @field DontCacheTemplate boolean
--- @field EnemyCharacter EntityHandle
--- @field FightMode boolean
--- @field FindValidPositionOnActivate boolean
--- @field Flags ServerCharacterFlags
--- @field Flags2 ServerCharacterFlags2
--- @field Flags3 ServerCharacterFlags3
--- @field Floating boolean
--- @field FollowCharacter EntityHandle
--- @field ForceSynch uint8
--- @field GMReroll boolean
--- @field GeneralSpeedMultiplier number
--- @field Global boolean
--- @field HasOsirisDialog uint8
--- @field HasOwner boolean
--- @field HostControl boolean
--- @field IgnoresTriggers boolean
--- @field InDialog boolean
--- @field InParty boolean
--- @field Inventory EntityHandle
--- @field InvestigationTimer number
--- @field Invisible boolean
--- @field Invulnerable boolean
--- @field IsCompanion_M boolean
--- @field IsHuge boolean
--- @field IsLeveledByStory boolean
--- @field IsPet boolean
--- @field IsPlayer boolean
--- @field IsResurrected boolean
--- @field IsTrading boolean
--- @field Level FixedString
--- @field LevelTransitionPending boolean
--- @field Loaded boolean
--- @field Multiplayer boolean
--- @field MyHandle EntityRef
--- @field NeedsMakePlayerUpdate boolean
--- @field NeedsPlacementSnapping boolean
--- @field NeedsUpdate uint8
--- @field NumConsumables uint8
--- @field OffStage boolean
--- @field OriginalTemplate CharacterTemplate
--- @field OsirisController EsvTaskController
--- @field OwnerCharacter EntityHandle
--- @field PartyFollower boolean
--- @field PlayerData EsvPlayerData
--- @field PreferredAiTargets Guid[]
--- @field PreviousCrimeHandle int32
--- @field PreviousCrimeState uint8
--- @field RegisteredForAutomatedDialog boolean
--- @field RequestStartTurn boolean
--- @field ReservedForDialog boolean
--- @field ServerControl FixedString[]
--- @field SpotSneakers boolean
--- @field SpreadActivation boolean
--- @field StatusManager EsvStatusMachine
--- @field SteeringEnabled boolean
--- @field StoryControlled boolean
--- @field StoryNPC boolean
--- @field Summon boolean
--- @field Summons EntityHandle[]
--- @field TeleportingPartyHack boolean
--- @field Template CharacterTemplate
--- @field TemplateUsedForSpells CharacterTemplate
--- @field Temporary boolean
--- @field Totem boolean
--- @field Trader boolean
--- @field TreasureEquip boolean
--- @field TreasureGeneratedForTrader boolean
--- @field Treasures FixedString[]
--- @field Unknown10000000000 boolean
--- @field Unknown40000000000 boolean
--- @field Unknown8000 boolean
--- @field Unknown80000000 boolean
--- @field Unknown80000000000000 boolean
--- @field UserID UserId
--- @field UserID2 UserId
--- @field WalkThrough boolean
--- @field GetStatus fun(self:EsvCharacter, a1:FixedString):EsvStatus
--- @field GetStatusByType fun(self:EsvCharacter, a1:StatusType):EsvStatus


--- @class EsvChasmRegionTrigger:EsvAreaTriggerBase
--- @field Triggers FixedString[]


--- @class EsvChasmSeederTrigger:EsvPointTriggerBase
--- @field MaxChasmHeightOffset number
--- @field MaxChasmTiles int32
--- @field Triggers FixedString[]


--- @class EsvClimbBlendspaceData
--- @field BlendedDuration number
--- @field StepUpHeightDelta number
--- @field WorldClimbingBlendspace FixedString


--- @class EsvClimbData
--- @field Bounds number
--- @field ClimbEndPosition vec3
--- @field ClimbEndPosition2 vec3
--- @field ClimbEndPosition3 vec3
--- @field ClimbStartPosition vec3
--- @field ClimbUp boolean
--- @field CurrentPosition vec3
--- @field HeightDelta number
--- @field MovementSpeed number
--- @field StepHeight number
--- @field WorldClimbingHeight number


--- @class EsvClimbResult
--- @field field_0 int32
--- @field field_18 int32
--- @field field_4 int32
--- @field field_8 int32
--- @field field_C vec3


--- @class EsvCreatePuddleAction:EsvCreateSurfaceActionBase
--- @field GrowSpeed number
--- @field GrowTimer number
--- @field IgnoreIrreplacableSurfaces boolean
--- @field IsFinished boolean
--- @field Step int32
--- @field SurfaceCells int32


--- @class EsvCreateSurfaceAction:EsvCreateSurfaceActionBase
--- @field CheckExistingSurfaces boolean
--- @field CurrentCellCount int32
--- @field ExcludeRadius number
--- @field GrowStep int32
--- @field GrowTimer number
--- @field IgnoreIrreplacableSurfaces boolean
--- @field InitialChangesPushed boolean
--- @field LineCheckBlock uint64
--- @field MaxHeight number
--- @field Radius number
--- @field SpellId SpellId
--- @field SurfaceConcentrationTarget uint16
--- @field SurfaceLayer SurfaceLayer8
--- @field Timer number


--- @class EsvCreateSurfaceActionBase:EsvSurfaceAction
--- @field Duration number
--- @field IsControlledByConcentration boolean
--- @field Owner EntityRef
--- @field Position vec3
--- @field SurfaceType SurfaceType


--- @class EsvCrimeAreaTrigger:EsvAreaTriggerBase
--- @field MinimumTension int32
--- @field field_7C FixedString
--- @field field_80 int64


--- @class EsvCrimeRegionTrigger:EsvAreaTriggerBase
--- @field Region FixedString


--- @class EsvCrowdCharacterTrigger:EsvAreaTriggerBase
--- @field AnimationSelectionTimeMax number
--- @field AnimationSelectionTimeMin number
--- @field CharacterSpacing number
--- @field CustomAnimations CrowdCharacterCustomAnimation[]
--- @field DetourChance number
--- @field DetourMaxWaitTime number
--- @field DetourMinWaitTime number
--- @field DetourPatrolSplines FixedString[]
--- @field DispersePatrolSplines FixedString[]
--- @field DynamicAnimationTags CrowdCharacterDynamicAnimationTag[]
--- @field GroupSizeMax int16
--- @field GroupSizeMin int16
--- @field GroupSpawnTimeMax number
--- @field GroupSpawnTimeMin number
--- @field MaxCharacters int16
--- @field PatrolSplines FixedString[]
--- @field PatrolWalkingPercentage number
--- @field PointsOfInterest FixedString[]
--- @field PositionPerturbation number
--- @field ProxiesOnly boolean
--- @field ReversePatrolDirection boolean
--- @field SpawnTemplates CrowdCharacterTriggerSpawnTemplate[]
--- @field SpawningEnabled boolean
--- @field YawPerturbationDegrees number


--- @class EsvDarknessActiveComponent:BaseComponent


--- @class EsvDeathApplyKnockedOutOneFrameComponent:BaseComponent
--- @field field_0 EntityHandle
--- @field field_10 string
--- @field field_8 uint8
--- @field field_C int32


--- @class EsvDeathAssignEntityToUserRequestOneFrameComponent:BaseComponent
--- @field UserID int32


--- @class EsvDeathContinueComponent:BaseComponent


--- @class EsvDeathDeadByDefaultRequestOneFrameComponent:BaseComponent
--- @field field_0 EntityHandle
--- @field field_8 uint8
--- @field field_9 uint8


--- @class EsvDelayDeathCauseComponent:BaseComponent
--- @field Blocked_M int32
--- @field DelayCount int32
--- @field field_8 Guid


--- @class EsvDialogTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class EsvDiedEventOneFrameComponent:BaseComponent


--- @class EsvDisarmAttempt
--- @field field_0 EntityHandle
--- @field field_10 EntityHandle
--- @field field_18 EntityHandle
--- @field field_20 int32
--- @field field_8 EntityHandle


--- @class EsvDisarmAttemptComponent:BaseComponent
--- @field Attempts EsvDisarmAttempt[]


--- @class EsvDisplayName
--- @field Name string
--- @field NameKey TranslatedString
--- @field field_10 uint8


--- @class EsvDisplayNameListComponent:BaseComponent
--- @field HideTitle uint8
--- @field Names EsvDisplayName[]
--- @field Titles EsvDisplayTitle[]
--- @field TranslatedStrings EsvDisplayTitle[]
--- @field Unknown uint8


--- @class EsvDisplayTitle
--- @field NameKey TranslatedString
--- @field field_10 uint8


--- @class EsvEntityAuraStatus
--- @field AuraStatus EntityHandle
--- @field Entity EntityHandle


--- @class EsvEocAreaTrigger:EsvAreaTriggerBase


--- @class EsvEocPointTrigger:EsvPointTriggerBase


--- @class EsvEqualizeExperienceData
--- @field Entity EntityHandle
--- @field Experience int32


--- @class EsvEventTrigger:EsvAreaTriggerBase
--- @field EnterEvent FixedString
--- @field LeaveEvent FixedString


--- @class EsvExperienceGaveOutComponent:BaseComponent
--- @field Experience int32


--- @class EsvExperienceSystem:EsvExperienceSystemBase
--- @field AvailableLevelChanged Set_EntityHandle
--- @field EqualizeExperience table<EntityHandle, EsvEqualizeExperienceData>
--- @field ExperienceOnDestroy table<EntityHandle, Set_EntityHandle>
--- @field IncreaseExperience table<EntityHandle, int32>
--- @field OverrideLevel table<EntityHandle, int32>
--- @field ReevaluateCanGainXP Set_EntityHandle


--- @class EsvExperienceSystemBase:BaseSystem


--- @class EsvExplorationTrigger:EsvAreaTriggerBase
--- @field ExplorationReward Guid
--- @field LevelOverride int32


--- @class EsvExtinguishFireAction:EsvCreateSurfaceActionBase
--- @field ExtinguishGrowTimer number
--- @field ExtinguishPosition vec3
--- @field Percentage number
--- @field Radius number
--- @field Step number


--- @class EsvFallToProneOneFrameComponent:BaseComponent


--- @class EsvFloorTrigger:FloorTrigger
--- @field Entity EntityHandle


--- @class EsvGameTimerComponent:BaseComponent
--- @field field_18 FixedString
--- @field field_20 EntityHandle
--- @field field_28 int32
--- @field field_2C int32
--- @field field_30 int32
--- @field field_34 int32
--- @field field_38 uint8


--- @class EsvGameplayLightChangesComponent:BaseComponent
--- @field field_0 uint32
--- @field field_4 uint32
--- @field field_8 uint8
--- @field field_9 uint8
--- @field field_A uint8


--- @class EsvGameplayLightEquipmentComponent:BaseComponent
--- @field field_0 Set_EntityHandle


--- @class EsvGlobalRestStateRequest
--- @field Remove int32
--- @field RestType int32


--- @class EsvIconInfo
--- @field Icon FixedString
--- @field field_4 uint32


--- @class EsvIconListComponent:BaseComponent
--- @field Icons EsvIconInfo[]


--- @class EsvInventoryItemDataPopulatedComponent:BaseComponent


--- @class EsvInventoryPropertyCanBePickpocketedComponent:BaseComponent
--- @field Tag GenericPropertyTag


--- @class EsvInventoryPropertyIsDroppedOnDeathComponent:BaseComponent
--- @field Tag GenericPropertyTag


--- @class EsvInventoryPropertyIsTradableComponent:BaseComponent
--- @field Tag GenericPropertyTag


--- @class EsvIsLightBlockerComponent:BaseComponent


--- @class EsvIsMarkedForDeletionComponent:BaseComponent


--- @class EsvIsVisionBlockerComponent:BaseComponent


--- @class EsvItem:BaseComponent
--- @field Activated boolean
--- @field CanBeMoved boolean
--- @field CanBePickedUp boolean
--- @field CanClimbOn boolean
--- @field CanOnlyBeUsedByOwner boolean
--- @field CanShootThrough boolean
--- @field CanUse boolean
--- @field ClientSync1 boolean
--- @field Destroy boolean
--- @field Destroyed boolean
--- @field DisableInventoryView80 boolean
--- @field DisableSync boolean
--- @field DontAddToHotbar boolean
--- @field Flags ServerItemFlags
--- @field Flags2 ServerItemFlags2
--- @field Floating boolean
--- @field ForceAffectedByAura boolean
--- @field ForceClientSync boolean
--- @field ForceSync boolean
--- @field FreezeGravity boolean
--- @field Frozen boolean
--- @field GMFolding boolean
--- @field Global boolean
--- @field HideHP boolean
--- @field InAutomatedDialog boolean
--- @field InPartyInventory boolean
--- @field InUse boolean
--- @field InteractionDisabled boolean
--- @field Invisible boolean
--- @field Invisible2 boolean
--- @field Invulnerable2 boolean
--- @field IsContainer boolean
--- @field IsDoor boolean
--- @field IsLadder boolean
--- @field IsMoving boolean
--- @field IsSecretDoor boolean
--- @field IsSurfaceBlocker boolean
--- @field IsSurfaceCloudBlocker boolean
--- @field Item EsvItem
--- @field ItemType FixedString
--- @field Known boolean
--- @field Level FixedString
--- @field LevelTransitionPending boolean
--- @field LoadedTemplate boolean
--- @field LuckyFind boolean
--- @field MyHandle EntityRef
--- @field NoCover boolean
--- @field OffStage boolean
--- @field OriginalTemplate ItemTemplate
--- @field PinnedContainer boolean
--- @field PositionChanged boolean
--- @field PreviousLevel FixedString
--- @field ReservedForDialog boolean
--- @field SourceContainer boolean
--- @field Stats FixedString
--- @field StatusManager EsvStatusMachine
--- @field Sticky boolean
--- @field StoryItem boolean
--- @field Summon boolean
--- @field TeleportOnUse boolean
--- @field Template ItemTemplate
--- @field Totem boolean
--- @field TransformChanged boolean
--- @field TreasureGenerated boolean
--- @field TreasureLevel int32
--- @field UnEquipLocked boolean
--- @field UnsoldGenerated boolean
--- @field UseRemotely boolean
--- @field WakePhysics boolean
--- @field WalkOn boolean
--- @field WalkThrough boolean
--- @field field_10 EntityHandle
--- @field field_68 EntityHandle
--- @field field_70 FixedString


--- @class EsvJumpFollowComponent:BaseComponent
--- @field DangerousAuras DangerousAuras
--- @field ProjectileTerrainOffset number
--- @field Spell SpellId
--- @field field_0 vec3
--- @field field_108 vec3
--- @field field_114 int32
--- @field field_118 uint8
--- @field field_11C vec3
--- @field field_128 int32
--- @field field_12C int32
--- @field field_130 uint32
--- @field field_134 uint32
--- @field field_138 uint8
--- @field field_139 uint8
--- @field field_13A uint8
--- @field field_18 int32
--- @field field_1C int32
--- @field field_20 int32
--- @field field_24 vec3
--- @field field_30 vec3
--- @field field_3C number
--- @field field_40 number
--- @field field_44 number
--- @field field_48 uint8
--- @field field_4C number
--- @field field_C vec3
--- @field field_D0 uint8
--- @field field_D4 vec3
--- @field field_E0 vec3
--- @field field_EC int32
--- @field field_F0 int64
--- @field field_F8 int32
--- @field field_FC vec3


--- @class EsvKillerComponent:BaseComponent
--- @field Killers Set_EntityHandle


--- @class EsvLeaderComponent:BaseComponent
--- @field Followers_M Set_EntityHandle


--- @class EsvLightingTrigger:LightingTrigger
--- @field Entity EntityHandle


--- @class EsvMSClimbing:EsvMovementState
--- @field Blendspace EsvClimbBlendspaceData
--- @field ClimbData EsvClimbData
--- @field ClimbResult EsvClimbResult
--- @field GenomeUpdated boolean
--- @field State int32
--- @field TimeRemaining number


--- @class EsvMSFalling:EsvMovementState
--- @field ExpectedFall boolean
--- @field FallAnimationStarted boolean
--- @field FallPosition vec3
--- @field Height number
--- @field OnDestroyedPlatform boolean
--- @field StartPosition vec3
--- @field State int32
--- @field TimeSpent number
--- @field field_37 uint8


--- @class EsvMSIdle:EsvMovementState


--- @class EsvMSMoveTo:EsvMovementState
--- @field CheckEndPosition boolean
--- @field CurrentSettings EsvMSMoveToSettings
--- @field Finished boolean
--- @field IsAiEntity boolean
--- @field NeedsPathfinding boolean
--- @field NextPathfindingTime number
--- @field NextTickTime number
--- @field PathId int32
--- @field PathId2 int32
--- @field PathfindingStarted boolean
--- @field Settings EsvMSMoveToSettings
--- @field State int32
--- @field TargetAdjusted2 vec3
--- @field TargetAdjusted3 vec3
--- @field TargetOriginal2 vec3
--- @field field_48 int32
--- @field field_50 vec3
--- @field field_5C int32
--- @field field_68 uint8


--- @class EsvMSMoveToSettings
--- @field AddSourceBoundsToMargin boolean
--- @field AiTargetCheck uint32
--- @field CloseEnoughFloor number
--- @field CloseEnoughMax number
--- @field CloseEnoughMin number
--- @field CloseEnoughPreference int32
--- @field CollisionMaskMove uint64
--- @field CollisionMaskStand uint64
--- @field DangerousAuraAvoidance uint8
--- @field HasSpliningNodes boolean
--- @field HasTurningNodes boolean
--- @field IgnoreDynamics boolean
--- @field IgnoreEntities EntityHandle[]
--- @field IsAvoidingFallDamage boolean
--- @field IsAvoidingObstacles boolean
--- @field IsPreciseItemInteraction boolean
--- @field PathClimbingMode uint8
--- @field PathDroppingMode uint8
--- @field SearchHorizon uint16
--- @field Speed Variant<uint8,float>
--- @field SurfaceAvoidance uint32
--- @field TargetAdjusted vec3
--- @field TargetHandle EntityHandle
--- @field TargetOriginal vec3
--- @field WeightFuncType uint8
--- @field field_118 int32


--- @class EsvMSMoveToSync:EsvMovementState
--- @field NextPosition vec3
--- @field PathId int32
--- @field Positions vec3[]
--- @field field_28 boolean[]
--- @field field_38 boolean[]
--- @field field_48 vec3
--- @field field_54 int32
--- @field field_58 int32
--- @field field_5C uint8
--- @field field_5D uint8


--- @class EsvMSRepose:EsvMovementState
--- @field EndRepose boolean


--- @class EsvMoveTask:EsvTask
--- @field ArriveEvent string
--- @field AvoidAoO boolean
--- @field BehaviorTransactionId int32
--- @field CurrentTarget vec3
--- @field IgnoreEntities EntityHandle[]
--- @field LastTimeSpent number
--- @field StoryTransactionId int32
--- @field TimeSpent number
--- @field field_7C uint8


--- @class EsvMovementMachine
--- @field Active boolean
--- @field Entity EntityHandle
--- @field Layers EsvMovementState[]


--- @class EsvMovementState
--- @field MovementMachine EsvMovementMachine
--- @field TransactionId int32
--- @field Type ServerMovementType


--- @class EsvMusicVolumeTrigger:EsvAreaTriggerBase
--- @field Events MusicVolumeEvent[]


--- @class EsvMusicVolumeTriggerState
--- @field Index uint32
--- @field PeerId uint16


--- @class EsvMusicVolumeTriggerStateComponent:BaseComponent
--- @field Triggered EsvMusicVolumeTriggerState[]


--- @class EsvNetComponent:BaseComponent


--- @class EsvOrderComponent:BaseComponent


--- @class EsvOsirisAppearTask:EsvTask
--- @field Angle number
--- @field Animation FixedString
--- @field FinishedEvent string
--- @field InvisibilityUpdated boolean
--- @field OnTrail boolean
--- @field OutOfSight boolean
--- @field PlayerSpawn boolean
--- @field PreventForceComplete boolean
--- @field SpawnState int32
--- @field Target EntityHandle
--- @field TargetPos vec3
--- @field Time number


--- @class EsvOsirisAutomatedDialogTask:EsvTask
--- @field Dialog Guid
--- @field DialogInstanceId int32
--- @field SpeakerList EntityHandle[]
--- @field Trigger Guid
--- @field WaitForCompletion boolean


--- @class EsvOsirisCombineTask:EsvTask
--- @field CombineAmount int32
--- @field CombineIngredients EntityHandle[]
--- @field CombineTarget EntityHandle


--- @class EsvOsirisDisappearTask:EsvTask
--- @field Angle number
--- @field BehaviourTransactionId int32
--- @field DefaultSpeed number
--- @field FinishedEvent string
--- @field IncreaseSpeed boolean
--- @field Offstage boolean
--- @field OutOfSight boolean
--- @field SpeedMultiplier number
--- @field StoryTransactionId int32
--- @field TargetPos vec3
--- @field ValidTarget boolean
--- @field field_70 int32


--- @class EsvOsirisDropTask:EsvTask
--- @field DesiredPosition vec3?
--- @field Item EntityHandle
--- @field TargetPos vec3


--- @class EsvOsirisFleeFromEntityTask:EsvMoveTask
--- @field FleeFromEntity EntityHandle
--- @field FleeRange number
--- @field Position vec3
--- @field field_9C boolean
--- @field field_A4 int32


--- @class EsvOsirisFleeFromGridTask:EsvMoveTask
--- @field Position vec3
--- @field Range number
--- @field field_9C boolean


--- @class EsvOsirisFleeFromRelationTask:EsvMoveTask
--- @field FleeFromRelation int32
--- @field FleeRange number
--- @field Position vec3
--- @field field_9C boolean
--- @field field_A4 int32


--- @class EsvOsirisFollowNPCTask:EsvTask
--- @field Target EntityHandle


--- @class EsvOsirisFollowOwnerOrLeaderTask:EsvTask
--- @field BehaviourTransactionId int32
--- @field Leader EntityHandle


--- @class EsvOsirisLookAtTask:EsvTask
--- @field Bone FixedString
--- @field Duration number
--- @field EntityHandle EntityHandle
--- @field TransactionGuid Guid
--- @field TransactionId int32


--- @class EsvOsirisMoveInRangeTask:EsvMoveTask
--- @field CachedCloseEnough number
--- @field CachedResult boolean
--- @field CachedTarget vec3
--- @field CachedTargetPos vec3
--- @field FallbackMoveCloser boolean
--- @field MaxRange number
--- @field MinRange number
--- @field MoveToMaxRange number
--- @field MoveToMinRange number
--- @field MoveToTarget EntityHandle
--- @field MoveToTargetPos vec3
--- @field MoveToWantedRange number
--- @field MustBeInTrigger boolean
--- @field OnlyUseMoveSpellIfRequired boolean
--- @field OverrideSourcePosition vec3
--- @field Spell SpellId
--- @field SpellPrototype StatsSpellPrototype
--- @field Target EntityHandle
--- @field TargetPos vec3
--- @field WantedRange number
--- @field WeaponHandle EntityHandle
--- @field field_148 int64
--- @field field_150 int64
--- @field field_158 uint8
--- @field field_160 int64
--- @field field_168 uint8
--- @field field_F8 uint8


--- @class EsvOsirisMoveItemTask:EsvTask
--- @field Amount int32
--- @field ArriveEvent string
--- @field BehaviourTransactionId int32
--- @field CheckRemovedFromInventory boolean
--- @field CheckSplitEvent boolean
--- @field Item EntityHandle
--- @field Position vec3
--- @field SplitProcessed boolean


--- @class EsvOsirisMoveToAndTalkTask:EsvTask
--- @field BehaviourTransactionId int32
--- @field DialogInstance FixedString
--- @field Speed FixedString
--- @field Target EntityHandle
--- @field Timeout number


--- @class EsvOsirisMoveToLocationTask:EsvMoveTask
--- @field MaxDistance number
--- @field MinDistance number
--- @field TargetRotation number
--- @field TargetRotationSet boolean
--- @field TargetToIgnore EntityHandle


--- @class EsvOsirisMoveToObjectTask:EsvMoveTask
--- @field DefaultSpeed number
--- @field IncreaseSpeed boolean
--- @field MaxDistance number
--- @field MinDistance number
--- @field SpeedMultiplier number
--- @field Target EntityHandle


--- @class EsvOsirisPickupItemTask:EsvTask
--- @field ArriveEvent string
--- @field BehaviourTransactionId int32
--- @field Item EntityHandle
--- @field MoveAvoidAoO boolean


--- @class EsvOsirisRateLimitedAutomatedDialogTask:EsvTask
--- @field AdHistoryId Guid
--- @field CullingDistance number
--- @field Dialog Guid
--- @field MinDelay number
--- @field SpeakerList EntityHandle[]
--- @field WaitForCompletion boolean


--- @class EsvOsirisResurrectTask:EsvTask
--- @field Animation FixedString
--- @field Chance number
--- @field HPPercentage number
--- @field IsResurrected boolean


--- @class EsvOsirisSteerTask:EsvTask
--- @field AngleTolerance number
--- @field BehaviourTransactionId int32
--- @field LookAt boolean
--- @field SnapToTarget boolean
--- @field Target EntityHandle
--- @field TargetPos vec3


--- @class EsvOsirisTeleportToLocationTask:EsvTask
--- @field AllowUnloadedLevel boolean
--- @field ArriveEvent string
--- @field Executed boolean
--- @field FindFleePosition boolean
--- @field FindPosition boolean
--- @field LeaveCombat boolean
--- @field Level FixedString
--- @field Position vec3
--- @field PreviousLevel FixedString
--- @field Rotation quat
--- @field SetRotation boolean
--- @field SnapToGround boolean
--- @field UnchainFollowers boolean
--- @field field_51 uint8


--- @class EsvOsirisTimerTask:EsvTask
--- @field Time number


--- @class EsvOsirisUseItemTask:EsvTask
--- @field ArriveEvent string
--- @field BehaviourTransactionId int32
--- @field IsInteraction boolean
--- @field Item EntityHandle
--- @field MoveAvoidAoO boolean
--- @field UseItem boolean


--- @class EsvOsirisUseSpellTask:EsvTask
--- @field CastPosition vec3
--- @field HasTargetPos boolean
--- @field HasTargetPos2 boolean
--- @field Item EntityHandle
--- @field MoveAvoidAoO boolean
--- @field Originator ActionOriginator
--- @field Spell SpellId
--- @field SpellCastGuid Guid
--- @field SpellCastOptions uint8
--- @field State uint8
--- @field Success boolean
--- @field Target EntityHandle
--- @field Target2 EntityHandle
--- @field TargetPos vec3
--- @field TargetPos2 vec3
--- @field WithoutMove boolean
--- @field field_A9 uint8


--- @class EsvOsirisWanderTask:EsvTask
--- @field Anchor Variant<EntityHandle,vec3>
--- @field BehaviourTransactionId int32
--- @field Duration number
--- @field Range number
--- @field Start vec3
--- @field Trigger EntityHandle


--- @class EsvPeersInRangeComponent:BaseComponent
--- @field Peers int32[]


--- @class EsvPickpocketAttempt
--- @field Amount int32
--- @field Item EntityHandle
--- @field Target EntityHandle
--- @field field_0 EntityHandle
--- @field field_10 int32
--- @field field_14 boolean
--- @field field_18 EntityHandle
--- @field field_8 EntityHandle


--- @class EsvPickpocketComponent:BaseComponent
--- @field Items EsvPickpocketAttempt[]
--- @field field_18 EsvPickpocketAttempt[]


--- @class EsvPlayerData
--- @field CachedTension uint8
--- @field CustomData PlayerCustomData
--- @field HelmetOption uint8
--- @field IsInDangerZone boolean
--- @field PlayerHandle EntityHandle
--- @field PreviousPositionId int32
--- @field PreviousPositions vec3[]
--- @field QuestSelected FixedString
--- @field Region FixedString
--- @field Renown int32
--- @field field_4C int32
--- @field field_68 EntityHandle
--- @field field_94 boolean


--- @class EsvPointTriggerBase:PointTrigger
--- @field Entity EntityHandle


--- @class EsvPolygonSurfaceAction:EsvCreateSurfaceActionBase
--- @field Characters EntityHandle[]
--- @field CurrentGrowTimer number
--- @field GrowStep int32
--- @field GrowTimer number
--- @field Items EntityHandle[]
--- @field LastSurfaceCellCount int32
--- @field PolygonVertices vec2[]
--- @field SomePosition vec3


--- @class EsvPortalTrigger:PortalTrigger
--- @field Entity EntityHandle


--- @class EsvProficiencyGroupStatsComponent:BaseComponent
--- @field Stats FixedString


--- @class EsvRaceTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class EsvRecruitedByComponent:BaseComponent
--- @field RecruitedBy EntityHandle


--- @class EsvRectangleSurfaceAction:EsvCreateSurfaceActionBase
--- @field Characters EntityRef[]
--- @field CurrentCellCount int32
--- @field DeathType StatsDeathType
--- @field GrowStep int32
--- @field GrowTimer number
--- @field Items EntityRef[]
--- @field Length number
--- @field LineCheckBlock uint64
--- @field MaxHeight number
--- @field SurfaceArea_M number
--- @field Target vec3
--- @field Width number


--- @class EsvRegionTrigger:EsvAreaTriggerBase
--- @field RegionMaskRegionInWorld boolean


--- @class EsvReplicationDependencyComponent:BaseComponent
--- @field Dependency EntityHandle


--- @class EsvReplicationDependencyOwnerComponent:BaseComponent
--- @field Dependents EntityHandle[]


--- @class EsvRoomTrigger:RoomTrigger
--- @field Entity EntityHandle
--- @field field_78 uint8


--- @class EsvSSLookAt:EsvSteeringState
--- @field CurrentParams EsvSSLookAtParams
--- @field Params EsvSSLookAtParams
--- @field field_58 int32


--- @class EsvSSLookAtParams
--- @field Bone FixedString
--- @field Duration number
--- @field SettingsTransactionId Guid
--- @field Target EntityHandle


--- @class EsvSSSteer:EsvSteeringState
--- @field SpeedMultiplier number
--- @field field_1C boolean


--- @class EsvSSSteerTo:EsvSteeringState
--- @field AngleTolerance number
--- @field MinDistance number
--- @field SnapToTarget boolean
--- @field SteerSpeedMultiplier number
--- @field Target EntityHandle
--- @field TargetPos vec3
--- @field TimeRemaining number
--- @field field_20 EntityHandle
--- @field field_34 vec3
--- @field field_51 uint8
--- @field field_52 uint8
--- @field field_53 uint8


--- @class EsvSSSteerTunnel:EsvSteeringState
--- @field TargetDir vec3
--- @field field_24 vec3
--- @field field_30 uint8


--- @class EsvSafePositionComponent:BaseComponent
--- @field Position vec3
--- @field field_24 boolean


--- @class EsvScriptPropertyCanBePickpocketedComponent:BaseComponent


--- @class EsvScriptPropertyIsDroppedOnDeathComponent:BaseComponent


--- @class EsvScriptPropertyIsTradableComponent:BaseComponent


--- @class EsvSoundVolumeTrigger:SoundVolumeTrigger
--- @field AreaLevelOverrideGuid Guid
--- @field Entity EntityHandle


--- @class EsvStartTrigger:EsvPointTriggerBase
--- @field Angle number
--- @field Player uint8


--- @class EsvStateComponent:BaseComponent
--- @field Flags uint32


--- @class EsvStatsAreaTrigger:EsvAreaTriggerBase
--- @field AreaLevelOverrideGuid Guid


--- @class EsvStatsEntityDirtyData
--- @field Boosts Set_EntityHandle
--- @field SetCurrentHP int32
--- @field UseBoostRequests EsvUseBoostUpdateRequest[]
--- @field field_34 int32
--- @field field_38 int64


--- @class EsvStatsSystem:BaseStatsSystem
--- @field CalculationRequests table<EntityHandle, StatsDirtyFlags>
--- @field DeferredCalculationRequests table<EntityHandle, StatsDirtyFlags>
--- @field EncumbranceChanged Set_EntityHandle
--- @field EntityDirtyData table<EntityHandle, table<uint64, EsvStatsEntityDirtyData>>
--- @field EquipmentChanged table<EntityHandle, Set_ItemSlot>
--- @field EquipmentModified Set_EntityHandle
--- @field ReloadStats Set_EntityHandle
--- @field SetEquipmentType table<EntityHandle, Guid>
--- @field StatsApplied Set_EntityHandle


--- @class EsvStatus
--- @field Cause EntityRef
--- @field CauseGUID Guid
--- @field CauseType uint8
--- @field ConditionRolls ConditionRolls
--- @field CurrentLifeTime number
--- @field DifficultyStatus FixedString
--- @field ExitCause uint8
--- @field Flags ServerStatusFlags
--- @field Flags2 ServerStatusFlags2
--- @field Flags3 ServerStatusFlags3
--- @field Flags4 ServerStatusFlags4
--- @field FreezeDuration boolean
--- @field IsRecoverable boolean
--- @field LifeTime number
--- @field OriginCauseType uint8
--- @field Originator ActionOriginator
--- @field Owner EntityHandle
--- @field RemoveConditions stats::ConditionId
--- @field RemoveEvents uint32
--- @field RootCause EntityRef
--- @field SourceEquippedItem EntityHandle
--- @field SourceSpell SpellId
--- @field SourceSurface SurfaceType
--- @field SourceUsedItem EntityHandle
--- @field SpellCastSourceUuid Guid
--- @field SpellCastingAbility uint8
--- @field StackId FixedString
--- @field StackPriority int32
--- @field StackedSpellCastSourceUuid Guid
--- @field StartTimer number
--- @field StatusHandle ComponentHandle
--- @field StatusId FixedString
--- @field StatusOwner EntityHandle[]
--- @field StatusSource EntityHandle
--- @field StoryActionID int32
--- @field Strength number
--- @field SyncEntity EntityHandle
--- @field TickType uint8
--- @field TurnTimer number
--- @field field_105 SurfaceType
--- @field field_18 int64
--- @field field_48 number
--- @field field_8 Guid
--- @field field_E0 EntityHandle
--- @field field_E8 Guid


--- @class EsvStatusAura:EsvStatus
--- @field Aura EsvAura


--- @class EsvStatusBoost:EsvStatusAura
--- @field EffectTime number
--- @field HasStatusEffectOnTurn boolean
--- @field ItemHandles EntityHandle[]
--- @field Items FixedString[]
--- @field LoseControl boolean
--- @field SourceDirection vec3
--- @field Spell FixedString[]
--- @field SurfaceChanges uint32[]


--- @class EsvStatusClimbing:EsvStatus
--- @field Direction boolean
--- @field Incapacitated_M boolean
--- @field Item EntityHandle
--- @field JumpUpLadders_M boolean
--- @field Level FixedString
--- @field MoveDirection_M vec3
--- @field Started_M boolean
--- @field Status uint8
--- @field field_120 int64
--- @field field_128 int32
--- @field field_12C int32
--- @field field_13C int32
--- @field field_140 int64


--- @class EsvStatusDeactivated:EsvStatusBoost


--- @class EsvStatusDowned:EsvStatusIncapacitated
--- @field DamageFailures int32
--- @field IsHealed boolean
--- @field IsStable boolean
--- @field NumStableFailed int32
--- @field NumStableSuccess int32
--- @field OnRollsFailed StatsFunctors
--- @field OnSuccess StatsFunctors
--- @field RollFailures int32
--- @field RollSuccesses int32
--- @field StableRoll StatsExpressionParam
--- @field StableRollDC int32


--- @class EsvStatusDying:EsvStatus


--- @class EsvStatusEffect:EsvStatus


--- @class EsvStatusFear:EsvStatusBoost


--- @class EsvStatusHeal:EsvStatus
--- @field EffectTime number
--- @field HealAmount int32
--- @field HealEffect int32
--- @field HealEffectId FixedString
--- @field TargetHealth int32


--- @class EsvStatusInSurface:EsvStatus
--- @field TargetData table<EntityHandle, EsvSurfaceLayerCheck[]>
--- @field TypeData table<SurfaceType, EsvSurfaceTypeCheck>
--- @field Unknown table<FixedString, boolean>


--- @class EsvStatusIncapacitated:EsvStatusBoost
--- @field CurrentFreezeTime number
--- @field FreezeTime number
--- @field IncapacitateFlags uint8


--- @class EsvStatusInvisible:EsvStatusBoost
--- @field InvisiblePosition vec3


--- @class EsvStatusKnockedDown:EsvStatus


--- @class EsvStatusMachine
--- @field Statuses EsvStatus[]


--- @class EsvStatusMaterial:EsvStatus
--- @field ApplyFlags uint8
--- @field ApplyNormalMap boolean
--- @field Fading boolean
--- @field Force boolean
--- @field IsOverlayMaterial boolean
--- @field MaterialUUID FixedString


--- @class EsvStatusPolymorphed:EsvStatusBoost
--- @field Id Guid


--- @class EsvStatusReaction:EsvStatus
--- @field CheckProjectileTargets boolean
--- @field FromInterrupt boolean
--- @field IgnoreChecks boolean
--- @field IgnoreHasSpell boolean
--- @field PartnerStatus ComponentHandle
--- @field ShowOverhead boolean
--- @field Source EntityRef
--- @field Spell SpellId
--- @field Target EntityRef
--- @field TargetPosition vec3
--- @field UpdatesSinceSpellCastRequest uint8


--- @class EsvStatusRotate:EsvStatus
--- @field RotationSpeed number
--- @field Yaw number


--- @class EsvStatusSmelly:EsvStatus


--- @class EsvStatusSneaking:EsvStatusBoost
--- @field ClientRequestStop boolean


--- @class EsvStatusStoryFrozen:EsvStatus


--- @class EsvStatusTeleportFalling:EsvStatus
--- @field HasDamage boolean
--- @field HasDamageBeenApplied boolean
--- @field ReappearTime number
--- @field Spell SpellId
--- @field Target vec3


--- @class EsvStatusUnlock:EsvStatus
--- @field Source EntityHandle
--- @field Success boolean
--- @field Unlocked int32
--- @field field_128 FixedString


--- @class EsvSteeringMachine
--- @field Active boolean
--- @field Entity EntityRef
--- @field Layers EsvSteeringState[]


--- @class EsvSteeringState
--- @field SteeringMachine EsvSteeringMachine
--- @field TransactionId int32
--- @field Type ServerSteeringType
--- @field field_10 boolean


--- @class EsvStoryShortRestStateRequest
--- @field Cause uint8
--- @field Enabled boolean


--- @class EsvSurfaceAction
--- @field Handle EntityHandle
--- @field Originator ActionOriginator
--- @field StoryActionID int32


--- @class EsvSurfaceComponent:BaseComponent
--- @field field_0 Guid
--- @field field_10 ComponentHandle


--- @class EsvSurfaceLayerCheck
--- @field EvaluateSurfaceApplyTypes uint8
--- @field FullyEntered boolean
--- @field Inside boolean
--- @field OnMoveCount int32
--- @field SurfaceDistanceCheck number
--- @field SurfaceOnMoveDistanceMultiplier number
--- @field Translate vec3
--- @field field_10 uint8
--- @field field_20 int32


--- @class EsvSurfaceTypeCheck
--- @field Entities Set_EntityHandle
--- @field TurnTimerCheck number


--- @class EsvTargetUUIDComponent:BaseComponent
--- @field Target Guid


--- @class EsvTask
--- @field Entity EntityHandle
--- @field Failed boolean
--- @field Flags uint32
--- @field PathSettings PathSettings
--- @field Reason int32
--- @field TaskState int32
--- @field Type OsirisTaskType


--- @class EsvTaskController:EsvBaseController
--- @field FlushRequested boolean
--- @field RemoveNextTask_M boolean
--- @field Tasks EsvTask[]
--- @field UpdateInProgress boolean


--- @class EsvTeleportTrigger:EsvPointTriggerBase
--- @field Angle number
--- @field Zoom boolean


--- @class EsvTemplateTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class EsvTimelineSceneTrigger:TimelineSceneTrigger
--- @field Entity EntityHandle


--- @class EsvTransformSurfaceAction:EsvSurfaceAction
--- @field Finished boolean
--- @field GrowCellPerSecond number
--- @field OriginSurface uint8
--- @field OwnerHandle EntityRef
--- @field PlayerCharacterNearby boolean
--- @field Position vec3
--- @field SurfaceLayer SurfaceLayer8
--- @field SurfaceLifetime number
--- @field SurfaceTransformAction uint8
--- @field Timer number


--- @class EsvUnsheathDefaultComponent:BaseComponent
--- @field field_0 int32
--- @field field_4 boolean


--- @class EsvUnsheathScriptOverrideComponent:BaseComponent
--- @field field_0 int32[]


--- @class EsvUseBoostUpdateRequest
--- @field Boost EntityHandle
--- @field IsNew boolean
--- @field Source BoostSource
--- @field field_20 Guid
--- @field field_30 uint64


--- @class EsvVariableManagerComponent:BaseComponent


--- @class EsvVisualSystem:BaseSystem
--- @field BoostsRemoved Set_EntityHandle
--- @field ItemSetVisualResource table<EntityHandle, FixedString>
--- @field UpdateVisuals Set_EntityHandle


--- @class EsvZoneAction:EsvCreateSurfaceActionBase
--- @field CurrentCellCount int64
--- @field Flags uint8
--- @field GrowStep int32
--- @field Params EsvZoneActionParams
--- @field Spell SpellId
--- @field Target vec3
--- @field Targets EntityRef[]
--- @field TextKey FixedString


--- @class EsvZoneActionParams
--- @field Flags uint8
--- @field FrontOffset number
--- @field Height number
--- @field MaxHeight number
--- @field Radius number
--- @field Shape int32
--- @field ZoneParam number


--- @class EsvActiveRollInProgressComponent:BaseComponent
--- @field Entity EntityHandle
--- @field Position vec3


--- @class EsvActiveRollStartRequestOneFrameComponent:BaseComponent
--- @field field_0 Guid
--- @field field_10 uint8


--- @class EsvActiveRollStartSpellRequestOneFrameComponent:BaseComponent
--- @field Caster EntityHandle
--- @field Flags uint32
--- @field NetGUID string
--- @field Originator ActionOriginator
--- @field Position vec3?
--- @field Source EntityHandle
--- @field Spell SpellId
--- @field StoryActionId int32
--- @field Targets SpellCastInitialTarget[]
--- @field field_28 uint32
--- @field field_30 EntityHandle
--- @field field_68 EntityHandle
--- @field field_70 EntityHandle
--- @field field_88 int32
--- @field field_A8 uint8


--- @class EsvAiCombatAiModifiersComponent:BaseComponent
--- @field Modifiers table<FixedString, number>
--- @field field_0 uint8


--- @class EsvAiCombatArchetypeComponent:BaseComponent
--- @field field_0 FixedString
--- @field field_4 FixedString
--- @field field_8 FixedString
--- @field field_C FixedString


--- @class EsvAiCombatInterestedInItemsComponent:BaseComponent
--- @field Items Set_EntityHandle


--- @class EsvAiCombatInterestingItemComponent:BaseComponent
--- @field Items Set_EntityHandle


--- @class EsvAiCombatModifierChange
--- @field field_0 string
--- @field field_18 string
--- @field field_30 number


--- @class EsvBoostAddBoostRequest
--- @field BoostDescriptions BoostDescription[]
--- @field Entity EntityHandle
--- @field Parameters BoostParameters
--- @field Source BoostSource


--- @class EsvBoostBaseUpdatedOneFrameComponent:BaseComponent


--- @class EsvBoostBoostBaseComponent:BaseComponent
--- @field field_0 FixedString[]


--- @class EsvBoostBoostChangedEvent
--- @field Description BoostDescription
--- @field Owner EntityHandle
--- @field Source BoostSource
--- @field Type BoostType
--- @field field_3C int32
--- @field field_4 int32
--- @field field_40 Guid
--- @field field_50 Guid
--- @field field_60 uint32
--- @field field_64 uint32
--- @field field_8 EntityHandle


--- @class EsvBoostBoostSystem:BaseSystem
--- @field AddDebugBoost EsvBoostAddBoostRequest[]
--- @field AddStoryBoosts EsvBoostAddBoostRequest[]
--- @field CharacterCreationUpdates Set_EntityHandle
--- @field ClearStoryBoosts Set_EntityHandle
--- @field DetachAndDestroyBoost Set_EntityHandle
--- @field ItemBoostChanges table<EntityHandle, EsvBoostEquipmentBoostAddRemoveRequest[]>
--- @field ProgressionUpdates Set_EntityHandle
--- @field ReleaseConditionHandles int32[]
--- @field ReloadBaseBoost Set_EntityHandle
--- @field ReloadStatusBoosts boolean
--- @field RemoveDebugBoosts EsvBoostRemoveBoostRequest[]
--- @field RemoveStoryBoosts EsvBoostRemoveBoostRequest[]
--- @field field_170 Set_EntityHandle


--- @class EsvBoostChangedEventOneFrameComponent:BaseComponent
--- @field Events EsvBoostBoostChangedEvent[]


--- @class EsvBoostChangedEventsSingletonComponent:BaseComponent
--- @field Events table<EntityHandle, EsvBoostBoostChangedEvent[]>


--- @class EsvBoostConditionalState
--- @field Boost EntityHandle
--- @field ConditionResult boolean


--- @class EsvBoostConditionalStateComponent:BaseComponent
--- @field States EsvBoostConditionalState[]


--- @class EsvBoostEquipmentBoostAddRemoveRequest
--- @field Action int32
--- @field Boost EntityHandle


--- @class EsvBoostProviderComponent:BaseComponent
--- @field field_0 EntityHandle[]
--- @field field_10 EntityHandle[]


--- @class EsvBoostRemoveBoostRequest
--- @field BoostDescriptions BoostDescription[]
--- @field Entity EntityHandle
--- @field Params BoostParameters
--- @field Source BoostSource
--- @field field_48 uint8


--- @class EsvBoostStatusBoostsProcessedComponent:BaseComponent


--- @class EsvBoostStatusBoostsRefreshedOneFrameComponent:BaseComponent
--- @field Causes EsvStatusRefreshCause[]


--- @class EsvCharacterCreationAppearanceVisualTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class EsvCharacterCreationCustomIconComponent:BaseComponent
--- @field Icon ScratchBuffer


--- @class EsvCharacterCreationEquipmentSetRequestComponent:BaseComponent
--- @field Requests table<EntityHandle, FixedString[]>


--- @class EsvCharacterCreationGodComponent:BaseComponent
--- @field God Guid


--- @class EsvCharacterCreationIsCustomComponent:BaseComponent


--- @class EsvCharacterCreationUpdatesComponent:BaseComponent
--- @field Updates table<EntityHandle, int32>


--- @class EsvCombatCanStartCombatComponent:BaseComponent


--- @class EsvCombatCombatGroupMappingComponent:BaseComponent
--- @field CombatGroups table<FixedString, Set_EntityHandle>
--- @field Entity EntityHandle


--- @class EsvCombatCombatGroupUpdate
--- @field Combat EntityHandle
--- @field CombatGroup FixedString
--- @field field_C boolean


--- @class EsvCombatCombatScheduledForDeleteOneFrameComponent:BaseComponent


--- @class EsvCombatCombatStartedEventOneFrameComponent:BaseComponent


--- @class EsvCombatCombatSwitchedComponent:BaseComponent
--- @field field_0 EntityHandle
--- @field field_18 Guid
--- @field field_28 EntityHandle
--- @field field_8 Guid


--- @class EsvCombatCombatantKilledEventOneFrameComponent:BaseComponent
--- @field field_0 EntityHandle
--- @field field_10 uint8
--- @field field_11 uint8
--- @field field_12 uint8
--- @field field_8 EntityHandle


--- @class EsvCombatDelayedFanfareRemovedDuringCombatEventOneFrameComponent:BaseComponent


--- @class EsvCombatEnterRequestComponent:BaseComponent
--- @field EnterRequests Set_EntityHandle


--- @class EsvCombatFleeBlockedComponent:BaseComponent


--- @class EsvCombatFleeRequestComponent:BaseComponent
--- @field RequestGuid Guid
--- @field UserID UserId


--- @class EsvCombatFleeSuccessOneFrameComponent:BaseComponent


--- @class EsvCombatGlobalCombatRequest
--- @field Action uint32
--- @field Combat EntityHandle
--- @field Source EntityHandle
--- @field Target EntityHandle


--- @class EsvCombatGlobalCombatRequests:BaseComponent
--- @field Requests EsvCombatGlobalCombatRequest[]


--- @class EsvCombatImmediateJoinComponent:BaseComponent


--- @class EsvCombatJoinEventOneFrameComponent:BaseComponent
--- @field Combat EntityHandle
--- @field ImmediateJoin boolean
--- @field Initiative int32


--- @class EsvCombatJoinInCurrentRoundFailedEventOneFrameComponent:BaseComponent


--- @class EsvCombatJoinInCurrentRoundOneFrameComponent:BaseComponent


--- @class EsvCombatJoiningComponent:BaseComponent
--- @field field_0 number


--- @class EsvCombatLateJoinPenaltyComponent:BaseComponent
--- @field field_0 number


--- @class EsvCombatLeaveRequestComponent:BaseComponent


--- @class EsvCombatLeftEventOneFrameComponent:BaseComponent
--- @field field_0 EntityHandle
--- @field field_8 Guid


--- @class EsvCombatMergeComponent:BaseComponent
--- @field Combat1 EntityHandle
--- @field Combat2 EntityHandle


--- @class EsvCombatNarrativeCombatRequest
--- @field Action NarrativeCombatRequestType
--- @field CombatGuid Guid
--- @field Entity EntityHandle


--- @class EsvCombatRequestCompletedEventOneFrameComponent:BaseComponent


--- @class EsvCombatSurfaceTeamSingletonComponent:BaseComponent
--- @field field_0 Set_EntityHandle
--- @field field_30 table<EntityHandle, Guid[]>


--- @class EsvCombatSurprisedJoinRequestOneFrameComponent:BaseComponent


--- @class EsvCombatSurprisedStealthRequestOneFrameComponent:BaseComponent


--- @class EsvCombatSystem:BaseSystem
--- @field ActivatedPeers_PeerId Set_uint32
--- @field CombatGroupUpdates EsvCombatCombatGroupUpdate[]
--- @field CombatRequests EsvCombatGlobalCombatRequest[]
--- @field DestroyCombats Set_FixedString
--- @field EndCombat Set_Guid
--- @field Flee table<EntityHandle, UserId>
--- @field JoinCombat table<EntityHandle, EntityHandle>
--- @field JoinNarrativeCombat table<EntityHandle, EntityHandle>
--- @field LeaveCombat Set_EntityHandle
--- @field NarrativeCombatRequests EsvCombatNarrativeCombatRequest[]
--- @field PauseCombat table<EntityHandle, CombatPauseSourceType>
--- @field ResetCombatTimer Set_EntityHandle
--- @field ResetTurn Set_EntityHandle
--- @field ResumeCombat table<EntityHandle, CombatPauseSourceType>
--- @field SkipCombatStartDelay Set_Guid
--- @field field_328 boolean
--- @field field_329 boolean
--- @field field_3E0_PeerId Set_uint32
--- @field field_410_PeerId Set_uint32


--- @class EsvCombatThreatRangeChangedEventOneFrameComponent:BaseComponent


--- @class EsvCombatTurnActionCompletedRequest
--- @field ActionsCompleted boolean
--- @field Entity EntityHandle


--- @class EsvCombatTurnOrderSystem:BaseSystem
--- @field BeginCancelEndTurn EntityHandle[]
--- @field CancelEndTurn EntityHandle[]
--- @field ClearTimeout EntityHandle[]
--- @field EndCancelEndTurn EntityHandle[]
--- @field EndTurn EntityHandle[]
--- @field GameState13 boolean
--- @field SetTimeout table<EntityHandle, number>
--- @field TurnCompleted EsvCombatTurnActionCompletedRequest[]


--- @class EsvConcentrationConcentrationChangedOneFrameComponent:BaseComponent
--- @field Ended SpellId
--- @field Interrupted boolean
--- @field Started SpellId


--- @class EsvConcentrationConcentrationRequest
--- @field BreakConcentration boolean
--- @field Caster EntityHandle
--- @field CheckReset boolean
--- @field ClearConcentration boolean
--- @field RequestType ServerConcentrationRequestType
--- @field SpellCastGuid Guid?
--- @field SpellId SpellId
--- @field Target ConcentrationTarget


--- @class EsvConcentrationConcentrationSystem:BaseSystem
--- @field ConcentrationRequests EsvConcentrationConcentrationRequest[]
--- @field field_48 uint8


--- @class EsvConcentrationDamageCheckOneFrameComponent:BaseComponent
--- @field Hits HitDesc[]


--- @class EsvConcentrationOnConcentrationClearedEventOneFrameComponent:BaseComponent
--- @field Status EntityHandle
--- @field SurfaceIndex int16
--- @field Target EntityHandle
--- @field field_12 boolean


--- @class EsvEcsstatsAbilityCheckEventOneFrameComponent:BaseComponent
--- @field Ability AbilityId
--- @field Source EntityHandle
--- @field Success boolean
--- @field Target EntityHandle
--- @field field_18 Guid
--- @field field_29 uint8


--- @class EsvEcsstatsAttributeFlagsChangedEventOneFrameComponent:BaseComponent


--- @class EsvEcsstatsClassesChangedEventOneFrameComponent:BaseComponent


--- @class EsvEcsstatsEquipmentSlotChangedEventOneFrameComponent:BaseComponent
--- @field Slots Set_ItemSlot


--- @class EsvEcsstatsLevelChangedOneFrameComponent:BaseComponent
--- @field NewLevel int32
--- @field PreviousLevel int32


--- @class EsvEcsstatsSavingThrowRolledEventOneFrameComponent:BaseComponent
--- @field Ability AbilityId
--- @field Advantage boolean
--- @field ConditionRoll ConditionRoll
--- @field Disadvantage boolean
--- @field RollBonus int32
--- @field RollType StatsRollType
--- @field Source EntityHandle
--- @field SpellCastUuid Guid
--- @field Success boolean
--- @field Target EntityHandle
--- @field Total int32
--- @field field_178 FixedString
--- @field field_17C uint8
--- @field field_20 FixedString
--- @field field_24 FixedString
--- @field field_28 uint8
--- @field field_2C FixedString


--- @class EsvEcsstatsSkillCheckEventOneFrameComponent:BaseComponent
--- @field Ability AbilityId
--- @field ConditionRoll ConditionRoll
--- @field Critical uint8
--- @field Skill SkillId
--- @field Source EntityHandle
--- @field SpellCastUuid Guid
--- @field SpellId FixedString
--- @field StatsExpression string
--- @field Success boolean
--- @field Target EntityHandle
--- @field field_14 FixedString
--- @field field_158 uint8


--- @class EsvEcsstatsStatsAppliedEventOneFrameComponent:BaseComponent


--- @class EsvEcsstatsWeaponPropertiesChangedEventOneFrameComponent:BaseComponent
--- @field Properties uint32


--- @class EsvEscortFollowerComponent:BaseComponent
--- @field Following EntityHandle


--- @class EsvEscortHasStragglersComponent:BaseComponent


--- @class EsvEscortLeaderComponent:BaseComponent
--- @field Group FixedString


--- @class EsvEscortLeaderPriorityComponent:BaseComponent
--- @field Priorities table<FixedString, int32>


--- @class EsvEscortMemberComponent:BaseComponent
--- @field Group FixedString


--- @class EsvEscortStragglersTrackerComponent:BaseComponent
--- @field Stragglers EntityHandle[]
--- @field field_10 vec3


--- @class EsvFtbModeChangedEventOneFrameComponent:BaseComponent
--- @field Entity EntityHandle
--- @field InFTB boolean
--- @field field_9 uint8


--- @class EsvFtbPlayersTurnEndedEventOneFrameComponent:BaseComponent


--- @class EsvFtbPlayersTurnStartedEventOneFrameComponent:BaseComponent


--- @class EsvFtbRoundEndedEventOneFrameComponent:BaseComponent


--- @class EsvFtbSurfaceTeamSingletonComponent:BaseComponent
--- @field field_0 Set_EntityHandle
--- @field field_30 table<EntityHandle, Guid[]>


--- @class EsvFtbTimeFactorRequestsSingletonComponent:BaseComponent
--- @field Requests table<EntityHandle, number>


--- @class EsvFtbTimeFactorResetRequestsSingletonComponent:BaseComponent
--- @field Requests Set_EntityHandle


--- @class EsvFtbToggleFTBParameters
--- @field ByPlayer boolean
--- @field Enter boolean
--- @field Shared boolean


--- @class EsvFtbTurnBasedChangesRequest
--- @field ActedThisRoundInCombat boolean?
--- @field CanActInCombat boolean?
--- @field CombatTeam Guid?
--- @field EndTurnHoldTimer number??
--- @field HadTurnInCombat boolean?
--- @field IsActiveCombatTurn boolean?
--- @field PauseTimer number??
--- @field Removed boolean?
--- @field RequestedEndTurn boolean?
--- @field Timeout number??
--- @field TurnActionsCompleted boolean?


--- @class EsvFtbTurnBasedChangesRequestSingletonComponent:BaseComponent
--- @field Requests table<EntityHandle, EsvFtbTurnBasedChangesRequest>


--- @class EsvFtbZoneComponent:BaseComponent
--- @field ByPlayer boolean
--- @field Creator EntityHandle
--- @field Entity EntityHandle
--- @field Shared boolean
--- @field ZoneGuid Guid
--- @field field_10 EntityHandle
--- @field field_18 uint8
--- @field field_1C number
--- @field field_20 FixedString
--- @field field_8 uint8


--- @class EsvFtbZoneInstigatorComponent:BaseComponent
--- @field Instigator EntityHandle
--- @field Shared boolean


--- @class EsvFtbZoneSystem:BaseSystem
--- @field BeginCancelEndTurn table<EntityHandle, EntityHandle[]>
--- @field CancelEndTurn table<EntityHandle, EntityHandle[]>
--- @field EndCancelEndTurn table<EntityHandle, EntityHandle[]>
--- @field EndTurn Set_EntityHandle
--- @field EnterExistingSharedFTB table<EntityHandle, EntityHandle>
--- @field LeaveExistingSharedFTB Set_EntityHandle
--- @field SetTurnActionsCompleted EsvCombatTurnActionCompletedRequest[]
--- @field ToggleForCharacter table<EntityHandle, EsvFtbToggleFTBParameters>


--- @class EsvHitAnimationEventRequestsData
--- @field HitAnimationType uint8
--- @field field_1 uint8
--- @field field_10 vec3
--- @field field_1C vec3
--- @field field_2 boolean
--- @field field_28 EntityHandle
--- @field field_4 vec3


--- @class EsvHitHitAnimationRequestOneFrameComponent:BaseComponent
--- @field Requests EsvHitAnimationEventRequestsData[]


--- @class EsvHitHitEventData
--- @field CauseType CauseType
--- @field Damage int32
--- @field DamageType DamageType
--- @field Inflicter Guid
--- @field InflicterOwner Guid
--- @field StoryActionId int32
--- @field TargetProxy Guid


--- @class EsvHitHitNotificationEventOneFrameComponent:BaseComponent
--- @field Dead boolean
--- @field Hit EsvHitHitEventData
--- @field Target EntityHandle


--- @class EsvHitHitNotificationRequestOneFrameComponent:BaseComponent
--- @field Hit EsvHitHitEventData
--- @field Target EntityHandle


--- @class EsvHitHitRequest
--- @field Ability AbilityId
--- @field DecreaseDelayDeathCount boolean
--- @field Desc HitDesc
--- @field HitWith HitWith
--- @field Originator ActionOriginator
--- @field SkipNotifications boolean
--- @field StoryActionId int32
--- @field Target EntityHandle
--- @field TargetProxy EntityHandle
--- @field Weapon EntityHandle
--- @field field_1C1 uint8
--- @field field_1FA boolean


--- @class EsvHitHitResultEventOneFrameComponent:BaseComponent
--- @field AC int32
--- @field Hit HitDesc
--- @field Lethal boolean
--- @field Originator ActionOriginator
--- @field ShouldBeDowned boolean
--- @field Target EntityHandle
--- @field field_0 ActionOriginator
--- @field field_1D0 uint8
--- @field field_1E8 uint8
--- @field field_20 HitDesc


--- @class EsvHitHitSystem:BaseSystem
--- @field HitAnimationFinished EntityHandle[]
--- @field Hits EsvHitHitRequest[]
--- @field Reactions EsvHitReactionRequest[]
--- @field field_80 uint8


--- @class EsvHitOnHitJoinCombatRequestOneFrameComponent:BaseComponent
--- @field Entities Set_EntityHandle


--- @class EsvHitReactionRequest
--- @field Hit HitDesc
--- @field Target EntityHandle
--- @field TargetProxy EntityHandle


--- @class EsvHitUnresolvedHitNotificationComponent:BaseComponent
--- @field Hits EsvHitHitEventData[]


--- @class EsvInterruptActionRequestsComponent:BaseComponent
--- @field CombatLogRequests EsvInterruptCombatLogRequestData[]
--- @field StartRequests EsvInterruptStartRequestData[]
--- @field StopRequests EsvInterruptStopRequestData[]


--- @class EsvInterruptAddRemoveRequestsComponent:BaseComponent
--- @field Requests table<EntityHandle, uint8>


--- @class EsvInterruptCombatLogRequestData
--- @field UseCosts StatsActionResourceCost[]
--- @field field_0 EntityHandle
--- @field field_18 table<EntityHandle, table<InterruptInterruptEvent, InterruptInterruptUsageEntry>>


--- @class EsvInterruptDataSingletonComponent:BaseComponent
--- @field Events InterruptUndecidedEventWithId[]
--- @field TargetHitInterrupts InterruptDelayedTargetHitInterruptEvent[]


--- @class EsvInterruptDecisionSystem:BaseSystem
--- @field Decisions table<EntityHandle, uint8>


--- @class EsvInterruptInitialParticipantData
--- @field Entities Set_EntityHandle
--- @field StartRequest EsvInterruptStartRequestData


--- @class EsvInterruptInitialParticipantsComponent:BaseComponent
--- @field Participants table<EntityHandle, EsvInterruptInitialParticipantData>


--- @class EsvInterruptInterruptDataComponent:BaseComponent
--- @field AnimationIndex int32
--- @field AnimationInterrupts InterruptAnimationInterruptData[]
--- @field Event InterruptUndecidedEvent?
--- @field NextInterruptIndex int32
--- @field PausedAnimationEvents InterruptPausedAnimationEvent[]
--- @field SpellCastGuid Guid


--- @class EsvInterruptInterruptRequest
--- @field Boost EntityHandle
--- @field Interrupt FixedString
--- @field IsRemoval boolean
--- @field Source InterruptRequestSource
--- @field Spell FixedString


--- @class EsvInterruptManagementSystem:BaseSystem
--- @field IsAsk table<EntityHandle, boolean>
--- @field IsEnabled table<EntityHandle, boolean>
--- @field RemoveContainers Set_EntityHandle


--- @class EsvInterruptRequestsSystem:BaseSystem
--- @field Requests table<EntityHandle, EsvInterruptInterruptRequest[]>


--- @class EsvInterruptStartRequestData
--- @field Event InterruptInterruptEvent
--- @field Interruptors table<EntityHandle, Set_EntityHandle>
--- @field SpellCastGuid Guid
--- @field field_158 uint8


--- @class EsvInterruptStopRequestData
--- @field Event InterruptInterruptEvent
--- @field SpellCastGuid Guid
--- @field field_118 uint8


--- @class EsvInterruptTurnOrderInZoneComponent:BaseComponent
--- @field InZone Set_EntityHandle


--- @class EsvInterruptUpdateInterruptorsRequestData
--- @field Interruptors table<EntityHandle, Set_EntityHandle>
--- @field SpellCastGuid Guid


--- @class EsvInterruptUsedOneFrameComponent:BaseComponent
--- @field Costs StatsActionResourceCost[]
--- @field Interrupts table<EntityHandle, table<InterruptInterruptEvent, InterruptInterruptUsageEntry>>


--- @class EsvInterruptZoneRequestsComponent:BaseComponent
--- @field StartRequests EsvInterruptStartRequestData[]
--- @field StopRequests EsvInterruptStopRequestData[]


--- @class EsvInventoryCharacterHasGeneratedTradeTreasureComponent:BaseComponent


--- @class EsvInventoryContainerDataComponent:BaseComponent
--- @field Flags uint16
--- @field field_4 int32


--- @class EsvInventoryEntityHasGeneratedTreasureComponent:BaseComponent


--- @class EsvInventoryGroupCheckComponent:BaseComponent
--- @field Conditions uint32


--- @class EsvInventoryIsReplicatedWithComponent:BaseComponent


--- @class EsvInventoryReadyToBeAddedToInventoryComponent:BaseComponent


--- @class EsvInventoryShapeshiftEquipmentHistoryComponent:BaseComponent
--- @field History Guid[]


--- @class EsvItemDynamicLayerOwnerComponent:BaseComponent
--- @field Owner FixedString


--- @class EsvLuaAfterExecuteFunctorEvent:LuaEventBase
--- @field Functor StatsFunctors
--- @field Hit HitResult
--- @field Params StatsContextData


--- @class EsvLuaBeforeDealDamageEvent:LuaEventBase
--- @field Attack AttackDesc
--- @field DamageSums AttackDesc
--- @field Hit HitDesc


--- @class EsvLuaDealDamageEvent:LuaEventBase
--- @field Attack AttackDesc
--- @field Caster EntityHandle
--- @field Caster2 EntityHandle
--- @field DamageSums AttackDesc
--- @field Functor StatsDealDamageFunctor
--- @field Hit HitDesc
--- @field HitWith HitWith
--- @field IsFromItem boolean
--- @field Originator ActionOriginator
--- @field Position vec3
--- @field SpellId SpellIdWithPrototype
--- @field SpellId2 SpellId
--- @field StoryActionId int32
--- @field Target EntityHandle


--- @class EsvLuaDealtDamageEvent:EsvLuaDealDamageEvent
--- @field Result HitResult


--- @class EsvLuaExecuteFunctorEvent:LuaEventBase
--- @field Functor StatsFunctors
--- @field Params StatsContextData


--- @class EsvLuaGameStateChangedEvent:LuaEventBase
--- @field FromState ServerGameState
--- @field ToState ServerGameState


--- @class EsvOwnershipIsCurrentOwnerComponent:BaseComponent
--- @field Owner Set_EntityHandle


--- @class EsvOwnershipIsLatestOwnerComponent:BaseComponent
--- @field Owner Set_EntityHandle


--- @class EsvOwnershipIsOriginalOwnerComponent:BaseComponent
--- @field Owner Set_EntityHandle


--- @class EsvOwnershipIsPreviousOwnerComponent:BaseComponent
--- @field Owner Set_EntityHandle


--- @class EsvOwnershipOwneeHistoryComponent:BaseComponent
--- @field LatestOwner EntityHandle
--- @field OriginalOwner EntityHandle
--- @field PreviousOwner EntityHandle


--- @class EsvOwnershipOwneeRequestComponent:BaseComponent
--- @field LatestOwner EntityHandle?
--- @field NewCurrentOwnee EntityHandle?
--- @field OriginalOwner EntityHandle?


--- @class EsvPartyUserSnapshotComponent:BaseComponent
--- @field PerUserCharacters table<Guid, EntityHandle[][]>
--- @field Snapshot table<Guid, EntityHandle[][]>


--- @class EsvPassiveBasicRollInfo:StatsRoll
--- @field DC int32
--- @field RollType ConditionRollType


--- @class EsvPassiveBoostsComponent:BaseComponent
--- @field Boosts Guid[]
--- @field ContextData Variant<StatsAttackTargetContextData,StatsAttackPositionContextData,StatsMoveContextData,StatsSourceContextData,StatsTargetContextData,StatsNearbyAttackedContextData,StatsNearbyAttackingContextData,StatsEquipContextData>?
--- @field field_0 Guid[]


--- @class EsvPassiveChangePassiveRequest
--- @field Entity EntityHandle
--- @field Passive FixedString
--- @field PassiveChange uint8
--- @field PassiveSourceType PassiveSourceType
--- @field Source EntityHandle


--- @class EsvPassiveConditionalRollInterruptEventOneFrameComponent:BaseComponent
--- @field Passive EntityHandle
--- @field PassiveId FixedString
--- @field PostponedRolls EsvPassivePostponedConditionalRoll[]
--- @field RollUuid Guid
--- @field Source EntityHandle
--- @field Target EntityHandle
--- @field field_0 Guid
--- @field field_10 EntityHandle
--- @field field_18 EntityHandle
--- @field field_20 EntityHandle
--- @field field_28 FixedString


--- @class EsvPassiveExecutePassiveRequest
--- @field Context Variant<StatsAttackTargetContextData,StatsAttackPositionContextData,StatsMoveContextData,StatsSourceContextData,StatsTargetContextData,StatsNearbyAttackedContextData,StatsNearbyAttackingContextData,StatsEquipContextData>
--- @field Passive EntityHandle
--- @field RollGuid Guid
--- @field field_338 uint8


--- @class EsvPassivePassiveBaseComponent:BaseComponent
--- @field Passives FixedString[]
--- @field field_0 FixedString[]


--- @class EsvPassivePassiveSystem:BaseSystem
--- @field ActiveRollPassiveRequests table<Guid, EsvPassiveBasicRollInfo>
--- @field AddPassives Set_EntityHandle
--- @field AddedPassives Set_EntityHandle
--- @field ChangePassive EsvPassiveChangePassiveRequest[]
--- @field ExecutePassives EsvPassiveExecutePassiveRequest[]
--- @field LongRest Set_EntityHandle
--- @field ObscurityChanged Set_EntityHandle
--- @field ReloadAllStatusPassives boolean
--- @field ReloadPassivesBoosts boolean
--- @field ReloadStatsPassives Set_EntityHandle
--- @field RemovePassives Set_EntityHandle
--- @field RemovedPassives Set_EntityHandle
--- @field ResetUsageCount table<EntityHandle, SpellCooldownType[]>
--- @field SurfaceEntered Set_EntityHandle
--- @field TogglePassives Set_EntityHandle
--- @field UsageCountReset Set_EntityHandle
--- @field WieldingAddPassives Set_EntityHandle
--- @field WieldingChangedPassives table<EntityHandle, Set_EntityHandle>
--- @field WieldingRemovePassives Set_EntityHandle
--- @field field_A0 Set_EntityHandle
--- @field field_D0 Set_EntityHandle


--- @class EsvPassivePassivesUpdatedEventOneFrameComponent:BaseComponent


--- @class EsvPassivePersistentDataComponent:BaseComponent
--- @field field_0 number
--- @field field_4 number


--- @class EsvPassivePostponedConditionalRoll
--- @field Condition stats::ConditionId
--- @field Roll ConditionRoll
--- @field RollIndex uint64
--- @field field_148 uint8


--- @class EsvPassiveRequestTargetTrackingOneFrameComponent:BaseComponent
--- @field Targets Set_EntityHandle
--- @field field_0 Guid


--- @class EsvPassiveScriptPassivesComponent:BaseComponent
--- @field Passives Set_FixedString


--- @class EsvPassiveToggledPassivesComponent:BaseComponent
--- @field Passives table<FixedString, boolean>


--- @class EsvPassiveUpdateTargetTrackingOneFrameComponent:BaseComponent
--- @field Targets Set_EntityHandle
--- @field field_0 Guid


--- @class EsvPassiveUsageCountIncrementedEventOneFrameComponent:BaseComponent


--- @class EsvProgressionChangedContainersComponent:BaseComponent
--- @field Changes table<EntityHandle, table<EntityHandle, EntityHandle[]>>


--- @class EsvProgressionLevelUpChangedOneFrameComponent:BaseComponent


--- @class EsvProgressionManagementSystem:BaseSystem
--- @field DestroyedProgressions Set_EntityHandle
--- @field ProgressionUpdates table<EntityHandle, int32>


--- @class EsvProjectileAttachmentComponent:BaseComponent
--- @field Attachment EntityHandle


--- @class EsvProjectileForkOnHit:EsvProjectileOnHitActionBase
--- @field ForkCount int32
--- @field ForkLevels int32
--- @field HitTargets EntityHandle[]


--- @class EsvProjectileImpactEventOneFrameComponent:BaseComponent
--- @field Target SpellCastIntermediateTarget
--- @field field_A8 EntityHandle
--- @field field_B0 boolean


--- @class EsvProjectileInitializationComponent:BaseProxyComponent
--- @field Data EsvProjectileInitializationData


--- @class EsvProjectileInitializationData
--- @field BeamSource EntityHandle
--- @field BeamTargetPos vec3?
--- @field CanDeflect boolean
--- @field Caster EntityHandle
--- @field CauseType CauseType
--- @field DamageMovingObjectOnLand boolean
--- @field ExplodeRadius number
--- @field HistoryUuid Guid
--- @field Hit HitDesc
--- @field HitAction EsvProjectileOnHitActionBase
--- @field IgnoreObjects boolean
--- @field IgnoreTargetChecks boolean
--- @field IsFromItem boolean
--- @field IsTrap boolean
--- @field MainDamageType boolean
--- @field MovementSettings PathSettings?
--- @field MovesProjectileFromCaster boolean
--- @field MovingObject EntityHandle
--- @field NoMovement boolean
--- @field Originator ActionOriginator
--- @field Path ProjectileProjectileResult?
--- @field Projectile FixedString
--- @field Source EntityHandle
--- @field SourceWeapon EntityHandle
--- @field Spell SpellId
--- @field SpellCastUuid Guid
--- @field SpellCastingAbility AbilityId
--- @field SpellData EsvProjectileSpellComponent
--- @field StartPosition vec3
--- @field StoryActionId int32
--- @field TargetObject EntityHandle
--- @field TargetPosition vec3
--- @field TextKey FixedString
--- @field ThrownObject EntityHandle
--- @field field_1C int32
--- @field field_93 boolean


--- @class EsvProjectileOnHitActionBase
--- @field Caster EntityHandle
--- @field IgnoreTargetChecks boolean
--- @field IsFromItem boolean
--- @field Spell SpellId


--- @class EsvProjectilePierceOnHit:EsvProjectileOnHitActionBase
--- @field Direction vec3
--- @field HitTargets EntityHandle[]
--- @field PierceCount int32


--- @class EsvProjectileProjectile:BaseProxyComponent
--- @field BeamEffect EntityHandle
--- @field BeamSource EntityHandle
--- @field CanDeflect boolean
--- @field Caster EntityHandle
--- @field Cause CauseType
--- @field ClientHitObject EntityHandle
--- @field DamageMovingObjectOnLand boolean
--- @field Entity EntityHandle
--- @field ExplodeImmediate boolean
--- @field ExplodeRadius number
--- @field ExplodeRadius2 number
--- @field FallbackTimer number
--- @field Flags uint64
--- @field Hit HitDesc
--- @field HitAction EsvProjectileOnHitActionBase
--- @field HitFunctors StatsFunctors
--- @field HitInterpolation number
--- @field HitObject EntityHandle
--- @field IgnoreObjects boolean
--- @field IgnoreRoof boolean
--- @field IgnoreSurfaces boolean
--- @field IgnoreTargetChecks boolean
--- @field InfluenceRadius number
--- @field IsChasmRecovery boolean
--- @field IsFromItem boolean
--- @field IsOnHold boolean
--- @field IsThrown boolean
--- @field IsTrap boolean
--- @field ItemActivated boolean
--- @field Launched boolean
--- @field Level FixedString
--- @field LifeTime number
--- @field MainDamageType DamageType
--- @field MoveFromOwner boolean
--- @field MoveFromSource boolean
--- @field MovingObject EntityHandle
--- @field MovingObjectTransform Transform
--- @field NoMovement boolean
--- @field Originator ActionOriginator
--- @field Owner EntityRef
--- @field Path PathPathMover
--- @field Path2 PathPathMover
--- @field Reacted boolean
--- @field RequestDelete boolean
--- @field ShouldFall boolean
--- @field Source EntityHandle
--- @field SourcePos vec3
--- @field SourcePosition vec3
--- @field SourceWeapon EntityHandle
--- @field SpawnEffect FixedString
--- @field SpawnFXOverridesImpactFX boolean
--- @field Spell SpellId
--- @field SpellCastUuid Guid
--- @field SpellCastingAbility AbilityId
--- @field SpellRollResult HitDesc
--- @field StoryActionId int32
--- @field Success boolean
--- @field TargetObject EntityHandle
--- @field TargetObjectPos vec3
--- @field TargetPos vec3
--- @field Template ProjectileTemplate
--- @field TextKey FixedString
--- @field Used boolean
--- @field field_526 uint8
--- @field field_574 vec3
--- @field field_8 uint32
--- @field field_C uint32


--- @class EsvProjectileRequestTargetTrackingOneFrameComponent:BaseComponent
--- @field Targets Set_EntityHandle
--- @field field_0 Guid


--- @class EsvProjectileSpellComponent:BaseComponent
--- @field Index int32
--- @field Spell EntityHandle
--- @field Target SpellCastIntermediateTarget


--- @class EsvProjectileSplitThrowableObjectRequestOneFrameComponent:BaseComponent


--- @class EsvProjectileUpdateTargetTrackingOneFrameComponent:BaseComponent
--- @field Targets Set_EntityHandle
--- @field field_0 Guid


--- @class EsvRestLongRestSystem:BaseSystem
--- @field AutoConfirmation boolean[]
--- @field Finish Set_EntityHandle
--- @field FinishConfirmation Set_UserId
--- @field Pending boolean
--- @field RequestLongRest boolean
--- @field RequestReloadStory boolean
--- @field RequestSyncedFinish boolean
--- @field RequestUserStateRestore boolean
--- @field RestConfirmed boolean
--- @field RestRequester EntityHandle
--- @field RestSupplies table<EntityHandle, int32>
--- @field ScriptDeny FixedString[]
--- @field SetPlayersAreDreaming boolean?
--- @field SetTimeline Set_Guid
--- @field UserDecision boolean
--- @field UserDecisionRestType uint8


--- @class EsvRestPendingTypeComponent:BaseComponent
--- @field CancelReason RestCancelReason
--- @field Supplies table<EntityHandle, int32>


--- @class EsvRestRestTypeChosenEventOneFrameComponent:BaseComponent
--- @field CancelReason RestCancelReason
--- @field Supplies table<EntityHandle, int32>
--- @field UserDecision boolean


--- @class EsvRestShortRestConsumeResourcesComponent:BaseComponent


--- @class EsvRestShortRestResultEventOneFrameComponent:BaseComponent
--- @field ErrorFlags RestErrorFlags
--- @field Rested boolean


--- @class EsvRestShortRestSystem:BaseSystem
--- @field FunctorRequest table<EntityHandle, boolean>
--- @field ReConRequest table<EntityHandle, boolean>
--- @field UIRequest table<EntityHandle, boolean>


--- @class EsvRollDeathSavingThrowRequest
--- @field DC Variant<int32,Guid>
--- @field Entity EntityHandle


--- @class EsvRollFinishedEvent
--- @field AC uint32
--- @field Ability AbilityId
--- @field Advantage boolean
--- @field AdvantageType1 boolean
--- @field AdvantageType2 boolean
--- @field Canceled boolean
--- @field ConsumedInspirationPoint boolean
--- @field DC uint32
--- @field DiceAdditionalValue int32
--- @field Disadvantage boolean
--- @field NaturalRoll int32
--- @field Roll EntityHandle
--- @field RollComponentType0 boolean
--- @field RollContext uint8
--- @field RollType uint8
--- @field RollUuid Guid
--- @field Roller EntityHandle
--- @field Skill SkillId
--- @field Source EntityHandle
--- @field Source2 EntityHandle
--- @field SpellCheck uint8
--- @field Subject EntityHandle
--- @field field_20 EntityHandle
--- @field field_29 boolean
--- @field field_48 boolean


--- @class EsvRollRollFinishedEventOneFrameComponent:BaseComponent
--- @field Events EsvRollFinishedEvent[]


--- @class EsvRollRollSystem:BaseSystem
--- @field ActorJoinsDialog table<EntityHandle, int32>
--- @field CancelRoll Set_EntityHandle
--- @field DeathSavingThrows EsvRollDeathSavingThrowRequest[]
--- @field DestroyRoll Set_EntityHandle
--- @field EntityToRollEntity table<EntityHandle, EntityHandle>
--- @field FinishRoll Set_Guid
--- @field RestartRoll Set_EntityHandle
--- @field RestartRoll2 Set_EntityHandle
--- @field SessionUnloadStarting boolean
--- @field StopRoll Set_Guid
--- @field field_F0 Set_EntityHandle


--- @class EsvShapeshiftAddRequest
--- @field ApplyToDead boolean
--- @field ShapeshiftRule Guid
--- @field StateGuid Guid
--- @field TargetTemplate EntityHandle
--- @field Template TemplateInfo?
--- @field field_28 boolean


--- @class EsvShapeshiftFloatOverride
--- @field Type ShapeshiftChangeType
--- @field Value number


--- @class EsvShapeshiftHealthReservationComponent:BaseComponent
--- @field Reservations table<Guid, int32>


--- @class EsvShapeshiftIntOverride
--- @field Type ShapeshiftChangeType
--- @field Value int32


--- @class EsvShapeshiftRootTemplateOverride
--- @field RootTemplate FixedString
--- @field field_114 uint8


--- @class EsvShapeshiftState
--- @field Abilities Optional_EsvShapeshiftIntOverride[]
--- @field ActionCapabilities uint16?
--- @field AiHint Guid?
--- @field ApplyVisualInCC boolean
--- @field Armor int32?
--- @field ArmorType int32?
--- @field AwarenessCapabilities uint16?
--- @field BlockLevelUp boolean
--- @field BloodSurfaceTypeInheritanceType uint8
--- @field CanSeeThrough boolean?
--- @field CanShootThrough boolean?
--- @field ChangeScript string?
--- @field CombatArchetype FixedString?
--- @field DeathActionsInheritanceType uint8
--- @field DefaultBoosts FixedString
--- @field DefaultBoostsInheritanceType uint8
--- @field DifficultyStatuses FixedString
--- @field DifficultyStatusesInheritanceType uint8
--- @field DisplayName TranslatedString?
--- @field DummyClass Guid?
--- @field Flags uint32
--- @field FlagsInheritanceType uint8
--- @field Gender uint8?
--- @field HitBloodTypeInheritanceType uint8
--- @field HotBar FixedString?
--- @field Icon FixedString?
--- @field InteractionsCapabilities uint16?
--- @field KillEntityAtZeroHP boolean
--- @field MuteEquipmentSound boolean
--- @field OverrideSpeaker boolean
--- @field Passives FixedString
--- @field PassivesInheritanceType uint8
--- @field PersonalStatusImmunities FixedString
--- @field PersonalStatusImmunitiesInheritanceType uint8
--- @field ProficiencyGroup ProficiencyGroupFlags
--- @field ProficiencyGroupsInheritanceType uint8
--- @field Race Guid?
--- @field RemoveOldTags boolean
--- @field RemovePrevSpells boolean
--- @field Resistances ResistanceBoostFlags[][]
--- @field ResistancesInheritanceType uint8
--- @field RootTemplate EsvShapeshiftRootTemplateOverride?
--- @field Scale EsvShapeshiftFloatOverride?
--- @field ShootThroughType uint8?
--- @field SpeakingCapabilities uint16?
--- @field SpellCastingAbility AbilityId?
--- @field SpellsAdd table<ShapeshiftSpellSource, FixedString[]>
--- @field SpellsRemove FixedString[]
--- @field StateGuid Guid
--- @field StepsType uint32?
--- @field SurfacePathInfluences SurfacePathInfluence[]
--- @field TagsAdd Guid[]
--- @field TagsRemove Guid[]
--- @field Title TranslatedString?
--- @field UnarmedAttackAbility AbilityId?
--- @field UnarmedRangedAttackAbility AbilityId?
--- @field UseShapeshiftIdentity boolean?
--- @field Vitality EsvShapeshiftIntOverride?
--- @field VitalityBoost EsvShapeshiftIntOverride?
--- @field WalkThrough boolean?
--- @field Weight EsvShapeshiftFloatOverride?
--- @field field_0 Guid
--- @field field_158 EntityHandle?
--- @field field_1FC uint8
--- @field field_1FD uint8
--- @field field_B4 uint8


--- @class EsvShapeshiftStatesComponent:BaseComponent
--- @field States EsvShapeshiftState[]
--- @field field_0 int32


--- @class EsvShapeshiftSystem:BaseSystem
--- @field RequestAddStates table<EntityHandle, EsvShapeshiftAddRequest[]>
--- @field RequestDeleteAllStates Set_EntityHandle
--- @field RequestDeleteStates table<EntityHandle, Guid[]>


--- @class EsvSpellBookChangedOneFrameComponent:BaseComponent


--- @class EsvSpellLearnRequest
--- @field Class Guid
--- @field SpellId FixedString


--- @class EsvSpellLearnedSpell
--- @field ProgressionSource Guid
--- @field SpellId FixedString


--- @class EsvSpellLearningSystem:BaseSystem
--- @field LearnRequests table<EntityHandle, EsvSpellLearnRequest[]>
--- @field RemoveLearnedSpells Set_EntityHandle
--- @field SavantChanged Set_EntityHandle


--- @class EsvSpellNewSpellsAddedEventOneFrameComponent:BaseComponent
--- @field Spells SpellMetaId[]


--- @class EsvSpellOnDamageSpell
--- @field Spell FixedString
--- @field field_4 int32
--- @field field_8 uint8


--- @class EsvSpellOnDamageSpellsComponent:BaseComponent
--- @field Spells EsvSpellOnDamageSpell[]


--- @class EsvSpellSpellCooldownSystem:BaseSystem
--- @field RechargeSpells_EH_bool table<EntityHandle, boolean>
--- @field ResetAllCooldowns Set_EntityHandle
--- @field ResetCooldowns table<EntityHandle, Set_SpellCooldownType>
--- @field RoundCooldowns Set_EntityHandle
--- @field StartCooldown table<EntityHandle, table<SpellId, Guid>>
--- @field UpdateCooldown table<EntityHandle, number>
--- @field UseRechargeSpell table<EntityHandle, SpellId[]>


--- @class EsvSpellSpellSystem:BaseSystem
--- @field AddSpells table<EntityHandle, SpellSpellMeta[]>
--- @field CCPrepareSpells Set_EntityHandle
--- @field InvalidateSpells Set_EntityHandle
--- @field PlayerPrepareSpell table<EntityHandle, SpellMetaId[]>
--- @field PlayerUnprepareSpell table<EntityHandle, SpellMetaId[]>
--- @field PrepareChanges Set_EntityHandle
--- @field PrepareSpell table<EntityHandle, SpellMetaId[]>
--- @field RefreshPrepareMaxAmounts Set_EntityHandle
--- @field RefreshPreparedAmounts Set_EntityHandle
--- @field ReloadSpellbook Set_EntityHandle
--- @field RemoveAllDebugSpells Set_EntityHandle
--- @field RemoveSpell table<EntityHandle, SpellMetaId[]>
--- @field UnprepareSpell table<EntityHandle, EsvSpellSpellUnprepareInfo[]>
--- @field UpdateEquipmentSpells Set_EntityHandle
--- @field UpdateItemSpells Set_EntityHandle
--- @field field_420 Set_EntityHandle


--- @class EsvSpellSpellUnprepareInfo:SpellMetaId
--- @field field_20 uint8


--- @class EsvSpellSpellsLearnedEventOneFrameComponent:BaseComponent
--- @field Spells EsvSpellLearnedSpell[]


--- @class EsvSpellCastCacheComponent:BaseComponent
--- @field Costs StatsActionResourceCost[]
--- @field HasPathfindTemplate boolean
--- @field IntermediateTargets table<FixedString, table<int32, SpellCastIntermediateTarget[]>>
--- @field MovementTransactionId uint32
--- @field PhaseFinished boolean
--- @field TargetCounts table<FixedString, int32>
--- @field TextKeyIndex uint32
--- @field TextKeyIndices table<int32, boolean>
--- @field field_54 int32
--- @field field_E0 vec3?


--- @class EsvSpellCastCastCancelRequest
--- @field Caster EntityHandle
--- @field Forced boolean
--- @field SpellCastGuid Guid?


--- @class EsvSpellCastCastConfirmRequest
--- @field Entity EntityHandle
--- @field PathSettings PathSettings?
--- @field SpellCastGuid Guid
--- @field TargetInfo NavigationTargetInfo?
--- @field field_AC boolean


--- @class EsvSpellCastCastHitDelayComponent:BaseComponent
--- @field Delays EsvSpellCastCastHitDelayInfo[]
--- @field HitDelay number
--- @field HitDelayRemaining number


--- @class EsvSpellCastCastHitDelayInfo
--- @field InitialIndex int32
--- @field IntermediateIndex int32
--- @field NextIndex int32
--- @field OverallIndex int32?
--- @field TextKey FixedString
--- @field TextKeyIndex int32


--- @class EsvSpellCastCastRequestSystem:BaseSystem
--- @field ActiveRollNodeStartRequests EsvSpellCastCastStartRequest[]
--- @field AnubisCancelRequests EsvSpellCastCastCancelRequest[]
--- @field AnubisCastRequests EsvSpellCastCastStartRequest[]
--- @field CharacterCancelRequests EsvSpellCastCastCancelRequest[]
--- @field CommandProtocolStartRequests EsvSpellCastCastStartRequest[]
--- @field ConfirmRequests EsvSpellCastCastConfirmRequest[]
--- @field GameplayControllerCancelRequests EsvSpellCastCastCancelRequest[]
--- @field ItemStartRequests EsvSpellCastCastStartRequest[]
--- @field JumpStartRequests EsvSpellCastCastStartRequest[]
--- @field NetworkCancelRequests EsvSpellCastCastCancelRequest[]
--- @field NetworkPreviewUpdateRequests EsvSpellCastPreviewSetRequest[]
--- @field NetworkStartRequests EsvSpellCastCastStartRequest[]
--- @field OsirisCancelRequests EsvSpellCastCastCancelRequest[]
--- @field OsirisCastRequests EsvSpellCastCastStartRequest[]
--- @field PlanCancelRequests EsvSpellCastCastStartRequest[]
--- @field ReactionCancelRequests EsvSpellCastCastCancelRequest[]
--- @field ReactionStartRequests EsvSpellCastCastStartRequest[]
--- @field TeleportCancelRequests EsvSpellCastCastCancelRequest[]
--- @field field_A8 EsvSpellCastCastStartRequest[]
--- @field field_E8 EsvSpellCastCastCancelRequest[]


--- @class EsvSpellCastCastRequestsComponent:BaseComponent
--- @field CancelRequests EsvSpellCastCastCancelRequest[]
--- @field ConfirmRequests EsvSpellCastCastConfirmRequest[]
--- @field PreviewSetRequests EsvSpellCastPreviewSetRequest[]
--- @field StartRequests EsvSpellCastCastStartRequest[]


--- @class EsvSpellCastCastResponsibleComponent:BaseComponent
--- @field Entity EntityHandle


--- @class EsvSpellCastCastStartRequest
--- @field CastOptions SpellCastOptions
--- @field CastPosition vec3?
--- @field Caster EntityHandle
--- @field Item EntityHandle
--- @field NetGuid string
--- @field Originator ActionOriginator
--- @field RequestGuid Guid
--- @field Spell SpellId
--- @field StoryActionId int32
--- @field Targets SpellCastInitialTarget[]
--- @field field_70 EntityHandle
--- @field field_A8 uint8


--- @class EsvSpellCastClientInitiatedComponent:BaseComponent


--- @class EsvSpellCastEventAddDelayedCastHit
--- @field InitialIndex uint64
--- @field IntermediateIndex uint32
--- @field OverallIndex uint32?
--- @field TextKey FixedString
--- @field TextKeyIndex uint32


--- @class EsvSpellCastEventAddMovementInfo
--- @field PathSettings PathSettings?
--- @field TargetInfo NavigationTargetInfo?


--- @class EsvSpellCastEventAddProjectileCache
--- @field Projectiles EsvSpellCastProjectileResultsExtraData[]


--- @class EsvSpellCastEventAddSharedMovement
--- @field Position vec3
--- @field field_C vec3?


--- @class EsvSpellCastEventAdvanceReaction
--- @field SpellConditionsType uint8
--- @field Target EntityHandle
--- @field TargetProxy EntityHandle?


--- @class EsvSpellCastEventAdvanceRoll
--- @field SpellConditionsType uint8
--- @field Target EntityHandle
--- @field TargetProxy EntityHandle?
--- @field TextKey FixedString


--- @class EsvSpellCastEventCacheFlagChange
--- @field Flag uint32


--- @class EsvSpellCastEventCast
--- @field Hit HitDesc
--- @field Target EntityHandle
--- @field TargetProxy EntityHandle?


--- @class EsvSpellCastEventCastHit
--- @field Index uint32
--- @field TextKey FixedString


--- @class EsvSpellCastEventCastOptionsChange
--- @field ClearOptions uint32
--- @field SetOptions uint32


--- @class EsvSpellCastEventCastTextKey
--- @field TextKey FixedString


--- @class EsvSpellCastEventChangeCastEndPosition
--- @field Position vec3


--- @class EsvSpellCastEventChangeCastPosition
--- @field Position vec3


--- @class EsvSpellCastEventChangeMovementTransactionId
--- @field TransactionId uint32


--- @class EsvSpellCastEventChangeProjectileDelayTimer
--- @field ProjectileDelay number
--- @field ProjectileIndex int32


--- @class EsvSpellCastEventChangeStoryActionId
--- @field StoryActionId int32


--- @class EsvSpellCastEventCreateWall
--- @field Target SpellCastInitialTarget


--- @class EsvSpellCastEventCreateZone
--- @field Target SpellCastInitialTarget
--- @field TextKey FixedString
--- @field TextKeyIndex uint32


--- @class EsvSpellCastEventDoTeleport
--- @field Target SpellCastInitialTarget
--- @field TextKey FixedString
--- @field TextKeyIndex uint32


--- @class EsvSpellCastEventFinish
--- @field FailReason SpellCastFailReason
--- @field field_1 uint8


--- @class EsvSpellCastEventFireProjectile
--- @field ProjectileIndex uint32
--- @field Target SpellCastProjectileTargetData
--- @field Trajectory FixedString
--- @field field_34C uint8


--- @class EsvSpellCastEventInitializeIntermediateTargets
--- @field Index uint32
--- @field Targets SpellCastIntermediateTarget[]
--- @field TextKey FixedString


--- @class EsvSpellCastEventInitializeRolls
--- @field Rolls SpellCastSpellRollData[]


--- @class EsvSpellCastEventInterruptApplyResult:EsvSpellCastInterruptApplyResultBase
--- @field InterruptUsage table<EntityHandle, InterruptInterruptUsage>


--- @class EsvSpellCastEventInterruptEnd
--- @field Event InterruptInterruptEvent
--- @field StopInterrupt boolean


--- @class EsvSpellCastEventInterruptEventChange
--- @field Event InterruptInterruptEvent
--- @field EventInterruptors table<InterruptInterruptEvent, table<EntityHandle, Set_EntityHandle>>
--- @field ExecutedDependencies table<Guid, InterruptExecutedDependency>
--- @field HasReplacement boolean
--- @field InterruptUsage table<EntityHandle, InterruptInterruptUsage>
--- @field Interruptors table<EntityHandle, Set_EntityHandle>
--- @field ReplacementEvent InterruptInterruptEvent
--- @field Results EsvSpellCastInterruptApplyResultBase


--- @class EsvSpellCastEventInterruptStart
--- @field Event InterruptInterruptEvent
--- @field ExecutedDependencies table<Guid, InterruptExecutedDependency>
--- @field Interruptors table<EntityHandle, Set_EntityHandle>


--- @class EsvSpellCastEventMoveDuringCastEnd
--- @field AdjustedPosition vec3
--- @field Position vec3


--- @class EsvSpellCastEventMoveDuringCastInitialize
--- @field Duration number


--- @class EsvSpellCastEventMoveDuringCastStart
--- @field TextKey FixedString


--- @class EsvSpellCastEventMoveDuringCastUpdate
--- @field AdjustedPosition vec3
--- @field Position vec3
--- @field Progress number


--- @class EsvSpellCastEventMovementEnd
--- @field PhaseFinished boolean


--- @class EsvSpellCastEventPhaseChange
--- @field Phase SpellCastPhase


--- @class EsvSpellCastEventPlayAnimation
--- @field CastEvent SpellAnimationCastEvent


--- @class EsvSpellCastEventPopDelayedProjectile
--- @field ProjectileIndex int32


--- @class EsvSpellCastEventPrecalculatedConditionals
--- @field Conditionals EsvSpellCastFunctorConditional[]


--- @class EsvSpellCastEventPrecalculatedInterruptors
--- @field HasReplacement boolean
--- @field Interruptors InterruptPrecalculatedAnimationInterruptData[]


--- @class EsvSpellCastEventPushPausedAnimationEvents
--- @field Events InterruptPausedAnimationEvent[]


--- @class EsvSpellCastEventSpellRollAbort
--- @field Hit HitDesc
--- @field Target EntityHandle
--- @field TargetProxy EntityHandle?


--- @class EsvSpellCastEventTargetHit
--- @field Attack AttackDesc
--- @field Caster EntityHandle
--- @field ContextType uint64
--- @field Hit HitDesc
--- @field NumConditionRolls uint32
--- @field SourcePosition vec3?
--- @field Target EntityHandle
--- @field TargetPosition vec3
--- @field TargetProxy EntityHandle?


--- @class EsvSpellCastEventTargetsChange
--- @field Targets SpellCastInitialTarget[]


--- @class EsvSpellCastEventTargetsChanged
--- @field Targets SpellCastInitialTarget[]


--- @class EsvSpellCastEventUpdateConditionals
--- @field Conditionals EsvSpellCastFunctorConditional[]


--- @class EsvSpellCastEventUpdateInterruptors
--- @field Interruptors table<EntityHandle, Set_EntityHandle>


--- @class EsvSpellCastEventUseCostsChange
--- @field Costs StatsActionResourceCost[]


--- @class EsvSpellCastEventWeaponSetRequest
--- @field IsRanged boolean


--- @class EsvSpellCastEventZoneRangeUpdate
--- @field field_0 number
--- @field field_4 number
--- @field field_8 number
--- @field field_C number


--- @class EsvSpellCastExternalDependency
--- @field field_0 int64
--- @field field_8 int32
--- @field field_C string


--- @class EsvSpellCastExternalsComponent:BaseComponent
--- @field Externals EsvSpellCastExternalDependency[]


--- @class EsvSpellCastFunctorConditional
--- @field Conditions stats::ConditionId
--- @field FunctorUuid Guid
--- @field Roll ConditionRoll
--- @field Target EntityHandle
--- @field WasAdjusted boolean


--- @class EsvSpellCastHitRegister
--- @field ContextType uint64
--- @field Target EntityHandle


--- @class EsvSpellCastHitRegisterComponent:BaseComponent
--- @field Hits EsvSpellCastHitRegister[]


--- @class EsvSpellCastInterruptApplyResultBase
--- @field DamageRollAdjustments table<InterruptDamageFunctorKey, InterruptDamageRollAdjustments>
--- @field HasReplacement boolean
--- @field RollAdjustments table<Guid, InterruptRollAdjustments>


--- @class EsvSpellCastInterruptRequestsComponent:BaseComponent
--- @field CombatLogRequests EsvInterruptCombatLogRequestData[]
--- @field StartRequests EsvInterruptStartRequestData[]
--- @field StopRequests EsvInterruptStopRequestData[]
--- @field UpdateInterruptorsRequests EsvInterruptUpdateInterruptorsRequestData[]


--- @class EsvSpellCastInterruptResultsComponent:BaseComponent
--- @field Conditionals EsvSpellCastFunctorConditional[]
--- @field Results table<InterruptDamageFunctorKey, InterruptDamageRollAdjustments>
--- @field Results2 EsvSpellCastFunctorConditional[]


--- @class EsvSpellCastInterruptsUsedOneFrameComponent:BaseComponent
--- @field Costs StatsActionResourceCost[]
--- @field Interrupts table<EntityHandle, table<InterruptInterruptEvent, InterruptInterruptUsageEntry>>


--- @class EsvSpellCastMoveDuringCastUpdateEventOneFrameComponent:BaseComponent
--- @field AdjustedPosition vec3
--- @field Position vec3
--- @field SurfaceTrail EsvSpellCastSurfaceTrail?


--- @class EsvSpellCastMovementAndPrecalculationEndEventOneFrameComponent:BaseComponent
--- @field PhaseFinished boolean
--- @field field_4 int32


--- @class EsvSpellCastMovementComponent:BaseComponent
--- @field Duration number
--- @field IsMoving boolean
--- @field Progress number
--- @field TextKey FixedString
--- @field field_0 vec3?


--- @class EsvSpellCastMovementInfoComponent:BaseComponent
--- @field PathSettings PathSettings?
--- @field Settings NavigationTargetInfo?


--- @class EsvSpellCastPathfindTrajectory


--- @class EsvSpellCastPendingRequestsComponent:BaseComponent
--- @field CancelRequests EsvSpellCastCastCancelRequest[]
--- @field ConfirmRequests EsvSpellCastCastConfirmRequest[]
--- @field PreviewSetRequests EsvSpellCastPreviewSetRequest[]


--- @class EsvSpellCastPreviewSetRequest
--- @field Entity EntityHandle
--- @field SpellCastGuid Guid
--- @field field_18 uint8


--- @class EsvSpellCastProjectileCacheComponent:BaseComponent
--- @field ExtraData EsvSpellCastProjectileResultsExtraData[]
--- @field ProjectileDelayTimers table<int32, number>
--- @field Target SpellCastProjectileTargetData?


--- @class EsvSpellCastProjectilePathfindCacheComponent:BaseComponent
--- @field Targets EntityHandle[]


--- @class EsvSpellCastProjectileResultsExtraData
--- @field Results ProjectileProjectileResult[]
--- @field field_0 vec3
--- @field field_C vec3


--- @class EsvSpellCastRequestTargetTrackingOneFrameComponent:BaseComponent
--- @field Targets Set_EntityHandle
--- @field field_0 Guid


--- @class EsvSpellCastStateComponent:BaseComponent
--- @field Originator ActionOriginator
--- @field Phase SpellCastPhase
--- @field StoryActionId int32
--- @field field_4 int32


--- @class EsvSpellCastSurfaceTrail
--- @field Radius number
--- @field SurfaceType SurfaceType


--- @class EsvSpellCastSystemEvent
--- @field Args Variant<EsvSpellCastEventFinish,EsvSpellCastEventMoveDuringCastInitialize,EsvSpellCastEventMoveDuringCastStart,EsvSpellCastEventMoveDuringCastUpdate,EsvSpellCastEventMoveDuringCastEnd,EsvSpellCastEventCast,EsvSpellCastEventCastHit,EsvSpellCastEventCastTextKey,EsvSpellCastEventTargetHit,EsvSpellCastEventSpellRollAbort,EsvSpellCastEventWeaponSetRequest,EsvSpellCastEventPlayAnimation,EsvSpellCastEventPhaseChange,EsvSpellCastEventCacheFlagChange,EsvSpellCastEventUseCostsChange,EsvSpellCastEventInitializeRolls,EsvSpellCastEventAdvanceRoll,EsvSpellCastEventAdvanceReaction,EsvSpellCastEventFireProjectile,EsvSpellCastEventChangeProjectileDelayTimer,EsvSpellCastEventPopDelayedProjectile,EsvSpellCastEventInitializeIntermediateTargets,EsvSpellCastEventAddDelayedCastHit,EsvSpellCastEventChangeCastPosition,EsvSpellCastEventChangeCastEndPosition,EsvSpellCastEventChangeMovementTransactionId,EsvSpellCastEventMovementEnd,EsvSpellCastEventCreateZone,EsvSpellCastEventDoTeleport,EsvSpellCastEventCreateWall,EsvSpellCastEventTargetsChanged,EsvSpellCastEventCastOptionsChange,EsvSpellCastEventTargetsChange,EsvSpellCastEventAddSharedMovement,EsvSpellCastEventAddProjectileCache,EsvSpellCastEventAddMovementInfo,EsvSpellCastEventInterruptStart,EsvSpellCastEventInterruptEventChange,EsvSpellCastEventInterruptApplyResult,EsvSpellCastEventInterruptEnd,EsvSpellCastEventPrecalculatedInterruptors,EsvSpellCastEventPushPausedAnimationEvents,EsvSpellCastEventChangeStoryActionId,EsvSpellCastEventZoneRangeUpdate,EsvSpellCastEventPrecalculatedConditionals,EsvSpellCastEventUpdateConditionals,EsvSpellCastEventUpdateInterruptors>?
--- @field Entity EntityHandle
--- @field Type ServerSpellCastSystemEventType


--- @class EsvSpellCastUnsheathFallbackTimerComponent:BaseComponent
--- @field Time number?


--- @class EsvSpellCastUpdateTargetTrackingOneFrameComponent:BaseComponent
--- @field TargetHistoryUUID Guid
--- @field Targets Set_EntityHandle


--- @class EsvSpellCastWeaponSetChangeRequestOneFrameComponent:BaseComponent
--- @field IsRanged boolean


--- @class EsvSpellCastZoneRangeComponent:BaseComponent
--- @field field_0 int32
--- @field field_4 int32
--- @field field_8 int32
--- @field field_C int32


--- @class EsvStatusActiveComponent:BaseComponent


--- @class EsvStatusAddEventOneFrameComponent:BaseComponent
--- @field Duration int32
--- @field StatusId FixedString
--- @field Target EntityHandle


--- @class EsvStatusAddedFromSaveLoadComponent:BaseComponent


--- @class EsvStatusAddedStatusAuraEffectEventOneFrameComponent:BaseComponent
--- @field field_0 FixedString
--- @field field_10 EntityHandle
--- @field field_4 int32
--- @field field_8 EntityHandle


--- @class EsvStatusApplyEventOneFrameComponent:BaseComponent
--- @field Status EntityHandle
--- @field StatusId FixedString
--- @field Target EntityHandle
--- @field field_18 Guid


--- @class EsvStatusAttemptEventOneFrameComponent:BaseComponent
--- @field Source EntityHandle
--- @field StatusId FixedString
--- @field StoryActionId int32
--- @field Target EntityHandle
--- @field Type StatusType
--- @field field_20 Guid


--- @class EsvStatusAttemptFailedEventOneFrameComponent:BaseComponent
--- @field StatusId FixedString
--- @field StoryActionId int32
--- @field Target EntityHandle
--- @field field_18 Guid


--- @class EsvStatusAura
--- @field Status FixedString
--- @field Targets Set_EntityHandle
--- @field field_0 FixedString
--- @field field_4 int32
--- @field field_8 Set_EntityHandle


--- @class EsvStatusAuraComponent:BaseComponent


--- @class EsvStatusAuraContainerComponent:BaseComponent
--- @field Auras EsvStatusAura[]


--- @class EsvStatusCauseComponent:BaseComponent
--- @field Cause Guid
--- @field StoryActionId int32


--- @class EsvStatusDestroyedEventRequest
--- @field Source EntityHandle
--- @field SourceUuid Guid
--- @field Status EntityHandle
--- @field StatusHandle ComponentHandle
--- @field StatusId FixedString
--- @field Target EntityHandle
--- @field Type StatusType
--- @field field_28 EntityHandle
--- @field field_40 Guid
--- @field field_50 Guid
--- @field field_60 Guid
--- @field field_70 int32
--- @field field_78 uint8
--- @field field_79 uint8
--- @field field_7A uint8
--- @field field_7B uint8
--- @field field_80 EntityHandle


--- @class EsvStatusDifficultyModifiersComponent:BaseComponent
--- @field Modifiers table<FixedString, Set_FixedString>


--- @class EsvStatusDispelRollCheckOneFrameComponent:BaseComponent
--- @field field_0 uint8
--- @field field_1 uint8
--- @field field_10 EntityHandle
--- @field field_8 EntityHandle


--- @class EsvStatusDownedChangedEventOneFrameComponent:BaseComponent


--- @class EsvStatusOwnershipComponent:BaseComponent
--- @field Owner EntityHandle


--- @class EsvStatusPerformingComponent:BaseComponent
--- @field PerformEvent FixedString


--- @class EsvStatusRefreshCause
--- @field StoryActionId int32
--- @field field_0 Guid
--- @field field_14 uint32


--- @class EsvStatusRefreshedOneFrameComponent:BaseComponent
--- @field Causes EsvStatusRefreshCause[]


--- @class EsvStatusRemoveEventOneFrameComponent:BaseComponent
--- @field Source EntityHandle
--- @field Status EntityHandle
--- @field StatusId FixedString
--- @field StoryActionId int32
--- @field Target EntityHandle
--- @field Type StatusType
--- @field field_10 EntityHandle
--- @field field_2C uint8
--- @field field_2D uint8
--- @field field_30 Guid
--- @field field_40 Guid
--- @field field_50 Guid


--- @class EsvStatusRemovedStatusAuraEffectEventOneFrameComponent:BaseComponent
--- @field field_0 FixedString
--- @field field_10 EntityHandle
--- @field field_4 int32
--- @field field_8 EntityHandle


--- @class EsvStatusScheduledForDeletionOneFrameComponent:BaseComponent
--- @field Request EsvStatusDestroyedEventRequest


--- @class EsvStatusStatusComponent:BaseComponent
--- @field Entity EntityHandle
--- @field SpellCastSourceUuid Guid
--- @field StatusHandle ComponentHandle
--- @field StatusId FixedString
--- @field Type StatusType


--- @class EsvStatusStatusEventOneFrameComponent:BaseComponent
--- @field Events uint32


--- @class EsvStatusTurnStartEventOneFrameComponent:BaseComponent
--- @field Status EntityHandle


--- @class EsvStatusUniqueComponent:BaseComponent
--- @field Unique table<FixedString, ComponentHandle>


--- @class EsvStatusUpdateTargetTrackingOneFrameComponent:BaseComponent
--- @field Targets Set_EntityHandle
--- @field field_0 Guid


--- @class EsvSummonAddConcentrationRequestOneFrameComponent:BaseComponent
--- @field Caster EntityHandle
--- @field Spell SpellId
--- @field UnsummonOnConcentrationBreak boolean
--- @field field_30 boolean


--- @class EsvSummonAddToExistingConcentrationRequestOneFrameComponent:BaseComponent
--- @field field_0 EntityHandle
--- @field field_8 uint8


--- @class EsvSummonAttachToProjectileRequestOneFrameComponent:BaseComponent
--- @field field_0 EntityHandle


--- @class EsvSummonDespawnRequestOneFrameComponent:BaseComponent


--- @class EsvSummonDespawnSystem:BaseSystem
--- @field KillSummon table<EntityHandle, table<EntityHandle, boolean>>
--- @field KillSummons table<EntityHandle, boolean>


--- @class EsvSummonExpiredRequestOneFrameComponent:BaseComponent


--- @class EsvSummonIsUnsummoningComponent:BaseComponent


--- @class EsvSummonItemSplitEvent
--- @field Item1 EntityHandle
--- @field Item2 EntityHandle


--- @class EsvSummonLateJoinPenaltyOneFrameComponent:BaseComponent


--- @class EsvSummonOwnerSetEventOneFrameComponent:BaseComponent
--- @field Owner EntityHandle


--- @class EsvSummonPlaceInInventoryRequestOneFrameComponent:BaseComponent
--- @field CannotRemove boolean
--- @field Owner EntityHandle
--- @field field_8 boolean
--- @field field_9 boolean


--- @class EsvSummonSetLifetimeRequestOneFrameComponent:BaseComponent
--- @field Lifetime number?


--- @class EsvSummonSpawnCreatedEventOneFrameComponent:BaseComponent
--- @field Entity EntityHandle


--- @class EsvSummonSpawnDesc
--- @field ApplyLateJoinPenalty boolean
--- @field Attack AttackDesc
--- @field Caster EntityHandle
--- @field Hit HitDesc
--- @field Level FixedString
--- @field Originator ActionOriginator
--- @field Position vec3
--- @field Spell SpellId
--- @field SpellCastSourceGuid Guid
--- @field StatusesToApply Set_FixedString
--- @field StoryActionId int32
--- @field Template FixedString


--- @class EsvSummonSpawnInInventoryDesc
--- @field Amount int32
--- @field Attack AttackDesc
--- @field Equip boolean
--- @field EquipOffHand boolean
--- @field Hit HitDesc
--- @field Level FixedString
--- @field MovingObject FixedString
--- @field Originator ActionOriginator
--- @field PlaceInInventory boolean
--- @field Source EntityHandle
--- @field SourceInventory EntityHandle?
--- @field SpellCastSourceGuid Guid
--- @field SpellId SpellId
--- @field StatusesToApply Set_FixedString
--- @field StoryActionId int32


--- @class EsvSummonSpawnSystem:BaseSystem
--- @field AttachSummon EsvSummonSummonRequest[]
--- @field CreateSpawn EsvSummonSpawnDesc[]
--- @field RemovedSummon EsvSummonSummonRequest[]
--- @field SetupOwner Set_EntityHandle
--- @field SpawnInInventoryDescs EsvSummonSpawnInInventoryDesc[]
--- @field SummonDescs EsvSummonSummonDesc[]
--- @field SummonInInventoryDescs EsvSummonSummonInInventoryDesc[]
--- @field SummonedItemWasSplit EsvSummonItemSplitEvent[]


--- @class EsvSummonSummonDesc
--- @field AddToExistingConcentration boolean
--- @field ApplyLateJoinPenalty boolean
--- @field AttachToProjectile EntityHandle
--- @field Attack AttackDesc
--- @field Container FixedString
--- @field Hit HitDesc
--- @field IsFromFunctor boolean
--- @field Level FixedString
--- @field Lifetime number?
--- @field MovingObject FixedString
--- @field Originator ActionOriginator
--- @field Position vec3
--- @field Rotation quat
--- @field SpellId SpellIdWithPrototype
--- @field StatusesToApply Set_FixedString
--- @field StoryActionId int32
--- @field SummonTopOwner EntityHandle
--- @field UseCasterPassives boolean


--- @class EsvSummonSummonInInventoryDesc
--- @field AllowOffHand boolean
--- @field Amount uint32
--- @field Attack AttackDesc
--- @field Equip boolean
--- @field ExtendExistingConcentration boolean
--- @field Hit HitDesc
--- @field Level FixedString
--- @field Lifetime number?
--- @field MovingObject FixedString
--- @field Originator ActionOriginator
--- @field PlaceInInventory boolean
--- @field Source EntityHandle
--- @field SourceInventory EntityHandle?
--- @field Spell SpellIdWithPrototype
--- @field SpellContainer FixedString
--- @field StatusesToApply Set_FixedString
--- @field StoryActionId int32


--- @class EsvSummonSummonRequest
--- @field Owner EntityHandle
--- @field Summon EntityHandle
--- @field field_10 FixedString
--- @field field_18 Guid


--- @class EsvTagsTagsChangedEventOneFrameComponent:BaseComponent
--- @field AddedTags Guid[]
--- @field RemovedTags Guid[]


--- @class EsvTemplatesChangeSystem:BaseSystem
--- @field TemplateSwitch table<EntityHandle, TemplateInfo>
--- @field TemplateTransformWithScript table<EntityHandle, TemplateInfo>


--- @class EsvTemplatesTemplateChangedOneFrameComponent:BaseComponent


--- @class EsvTemplatesTemplateTransformedOneFrameComponent:BaseComponent


--- @class EsvTriggerCachedLeaveEventData
--- @field Event FixedString
--- @field Object Guid
--- @field field_14 uint8


--- @class EsvTriggerCachedLeaveEventsComponent:BaseComponent
--- @field Updated EsvTriggerCachedLeaveEventData[]


--- @class EsvTriggerEventConfigComponent:BaseComponent
--- @field Config uint8


--- @class EsvTriggerLoadedHandledComponent:BaseComponent


--- @class EsvTriggerRegisteredForComponent:BaseComponent
--- @field RegisteredFor Set_Guid


--- @class EsvTriggerRegistrationSettingsComponent:BaseComponent
--- @field Registered boolean


--- @class EsvTriggerTriggerWorldAutoTriggeredComponent:BaseComponent


--- @class EsvTriggerUpdatedRegisteredForComponent:BaseComponent
--- @field Updated EntityHandle[]


--- @class EsvTriggerUpdatedRegisteredForItemsComponent:BaseComponent
--- @field Updated EntityHandle[]


--- @class ExtuiBulletText:ExtuiStyledRenderable


--- @class ExtuiButton:ExtuiStyledRenderable
--- @field MouseButtonLeft boolean
--- @field MouseButtonMiddle boolean
--- @field MouseButtonRight boolean
--- @field Size vec2?


--- @class ExtuiCheckbox:ExtuiStyledRenderable
--- @field Checked boolean
--- @field OnChange RegistryEntry


--- @class ExtuiChildWindow:ExtuiWindowBase
--- @field AlwaysUseWindowPadding boolean
--- @field AutoResizeX boolean
--- @field AutoResizeY boolean
--- @field Border boolean
--- @field ChildAlwaysAutoResize boolean
--- @field FrameStyle boolean
--- @field NavFlattened boolean
--- @field ResizeX boolean
--- @field ResizeY boolean
--- @field Size vec2?


--- @class ExtuiCollapsingHeader:ExtuiTreeParent
--- @field AllowOverlap boolean
--- @field Bullet boolean
--- @field CollapsingHeader boolean
--- @field DefaultOpen boolean
--- @field FramePadding boolean
--- @field Framed boolean
--- @field Leaf boolean
--- @field NavLeftJumpsBackHere boolean
--- @field NoAutoOpenOnLog boolean
--- @field NoTreePushOnOpen boolean
--- @field OpenOnArrow boolean
--- @field OpenOnDoubleClick boolean
--- @field Selected boolean
--- @field SpanAllColumns boolean
--- @field SpanAvailWidth boolean
--- @field SpanFullWidth boolean
--- @field SpanTextWidth boolean


--- @class ExtuiColorEdit:ExtuiStyledRenderable
--- @field AlphaBar boolean
--- @field AlphaNoBg boolean
--- @field AlphaOpaque boolean
--- @field AlphaPreview boolean
--- @field AlphaPreviewHalf boolean
--- @field Color vec4
--- @field DisplayHSV boolean
--- @field DisplayHex boolean
--- @field DisplayRGB boolean
--- @field Float boolean
--- @field HDR boolean
--- @field InputHSV boolean
--- @field InputRGB boolean
--- @field NoAlpha boolean
--- @field NoBorder boolean
--- @field NoDragDrop boolean
--- @field NoInputs boolean
--- @field NoLabel boolean
--- @field NoOptions boolean
--- @field NoPicker boolean
--- @field NoSidePreview boolean
--- @field NoSmallPreview boolean
--- @field NoTooltip boolean
--- @field OnChange RegistryEntry
--- @field PickerHueBar boolean
--- @field PickerHueWheel boolean
--- @field Uint8 boolean


--- @class ExtuiColorPicker:ExtuiStyledRenderable
--- @field AlphaBar boolean
--- @field AlphaNoBg boolean
--- @field AlphaOpaque boolean
--- @field AlphaPreview boolean
--- @field AlphaPreviewHalf boolean
--- @field Color vec4
--- @field DisplayHSV boolean
--- @field DisplayHex boolean
--- @field DisplayRGB boolean
--- @field Float boolean
--- @field HDR boolean
--- @field InputHSV boolean
--- @field InputRGB boolean
--- @field NoAlpha boolean
--- @field NoBorder boolean
--- @field NoDragDrop boolean
--- @field NoInputs boolean
--- @field NoLabel boolean
--- @field NoOptions boolean
--- @field NoPicker boolean
--- @field NoSidePreview boolean
--- @field NoSmallPreview boolean
--- @field NoTooltip boolean
--- @field OnChange RegistryEntry
--- @field PickerHueBar boolean
--- @field PickerHueWheel boolean
--- @field Uint8 boolean


--- @class ExtuiColumnDefinition
--- @field AngledHeader boolean
--- @field DefaultHide boolean
--- @field DefaultSort boolean
--- @field Disabled boolean
--- @field IndentDisable boolean
--- @field IndentEnable boolean
--- @field Name string
--- @field NoClip boolean
--- @field NoHeaderLabel boolean
--- @field NoHeaderWidth boolean
--- @field NoHide boolean
--- @field NoReorder boolean
--- @field NoResize boolean
--- @field NoSort boolean
--- @field NoSortAscending boolean
--- @field NoSortDescending boolean
--- @field PreferSortAscending boolean
--- @field PreferSortDescending boolean
--- @field Width number
--- @field WidthFixed boolean
--- @field WidthStretch boolean


--- @class ExtuiCombo:ExtuiStyledRenderable
--- @field HeightLarge boolean
--- @field HeightLargest boolean
--- @field HeightRegular boolean
--- @field HeightSmall boolean
--- @field NoArrowButton boolean
--- @field NoPreview boolean
--- @field OnChange RegistryEntry
--- @field Options string[]
--- @field PopupAlignLeft boolean
--- @field SelectedIndex int32
--- @field WidthFitPreview boolean


--- @class ExtuiDragInt:ExtuiStyledRenderable
--- @field AlwaysClamp boolean
--- @field ClampOnInput boolean
--- @field ClampZeroRange boolean
--- @field Components int32
--- @field Logarithmic boolean
--- @field Max ivec4
--- @field Min ivec4
--- @field NoInput boolean
--- @field NoRoundToFormat boolean
--- @field NoSpeedTweaks boolean
--- @field OnChange RegistryEntry
--- @field Value ivec4
--- @field WrapAround boolean


--- @class ExtuiDragScalar:ExtuiStyledRenderable
--- @field AlwaysClamp boolean
--- @field ClampOnInput boolean
--- @field ClampZeroRange boolean
--- @field Components int32
--- @field Logarithmic boolean
--- @field Max vec4
--- @field Min vec4
--- @field NoInput boolean
--- @field NoRoundToFormat boolean
--- @field NoSpeedTweaks boolean
--- @field OnChange RegistryEntry
--- @field Value vec4
--- @field WrapAround boolean


--- @class ExtuiDummy:ExtuiStyledRenderable
--- @field Height number
--- @field Width number


--- @class ExtuiGroup:ExtuiTreeParent


--- @class ExtuiImage:ExtuiStyledRenderable
--- @field Border vec4
--- @field ImageData ExtuiImageReference
--- @field Tint vec4


--- @class ExtuiImageButton:ExtuiStyledRenderable
--- @field Background vec4
--- @field Image ExtuiImageReference
--- @field MouseButtonLeft boolean
--- @field MouseButtonMiddle boolean
--- @field MouseButtonRight boolean
--- @field Tint vec4


--- @class ExtuiImageReference
--- @field Icon FixedString
--- @field Size vec2
--- @field TextureResource FixedString
--- @field UV0 vec2
--- @field UV1 vec2


--- @class ExtuiInputInt:ExtuiStyledRenderable
--- @field AllowTabInput boolean
--- @field AlwaysOverwrite boolean
--- @field AutoSelectAll boolean
--- @field CharsDecimal boolean
--- @field CharsHexadecimal boolean
--- @field CharsNoBlank boolean
--- @field CharsScientific boolean
--- @field CharsUppercase boolean
--- @field Components int32
--- @field CtrlEnterForNewLine boolean
--- @field DisplayEmptyRefVal boolean
--- @field ElideLeft boolean
--- @field EnterReturnsTrue boolean
--- @field EscapeClearsAll boolean
--- @field Multiline boolean
--- @field NoHorizontalScroll boolean
--- @field NoUndoRedo boolean
--- @field OnChange RegistryEntry
--- @field ParseEmptyRefVal boolean
--- @field Password boolean
--- @field ReadOnly boolean
--- @field Value ivec4


--- @class ExtuiInputScalar:ExtuiStyledRenderable
--- @field AllowTabInput boolean
--- @field AlwaysOverwrite boolean
--- @field AutoSelectAll boolean
--- @field CharsDecimal boolean
--- @field CharsHexadecimal boolean
--- @field CharsNoBlank boolean
--- @field CharsScientific boolean
--- @field CharsUppercase boolean
--- @field Components int32
--- @field CtrlEnterForNewLine boolean
--- @field DisplayEmptyRefVal boolean
--- @field ElideLeft boolean
--- @field EnterReturnsTrue boolean
--- @field EscapeClearsAll boolean
--- @field Multiline boolean
--- @field NoHorizontalScroll boolean
--- @field NoUndoRedo boolean
--- @field OnChange RegistryEntry
--- @field ParseEmptyRefVal boolean
--- @field Password boolean
--- @field ReadOnly boolean
--- @field Value vec4


--- @class ExtuiInputText:ExtuiStyledRenderable
--- @field AllowTabInput boolean
--- @field AlwaysOverwrite boolean
--- @field AutoSelectAll boolean
--- @field CharsDecimal boolean
--- @field CharsHexadecimal boolean
--- @field CharsNoBlank boolean
--- @field CharsScientific boolean
--- @field CharsUppercase boolean
--- @field CtrlEnterForNewLine boolean
--- @field DisplayEmptyRefVal boolean
--- @field ElideLeft boolean
--- @field EnterReturnsTrue boolean
--- @field EscapeClearsAll boolean
--- @field Hint string?
--- @field Multiline boolean
--- @field NoHorizontalScroll boolean
--- @field NoUndoRedo boolean
--- @field OnChange RegistryEntry
--- @field ParseEmptyRefVal boolean
--- @field Password boolean
--- @field ReadOnly boolean
--- @field SizeHint vec2?
--- @field Text string


--- @class ExtuiMenu:ExtuiTreeParent
--- @field AddItem fun(self:ExtuiMenu, a1:string, a2:string?):ExtuiMenuItem


--- @class ExtuiMenuBar:ExtuiTreeParent


--- @class ExtuiMenuItem:ExtuiStyledRenderable
--- @field Enabled boolean
--- @field Shortcut string?


--- @class ExtuiNewLine:ExtuiStyledRenderable


--- @class ExtuiPopup:ExtuiWindowBase
--- @field Open fun(self:ExtuiPopup, a1:GuiPopupFlags?)


--- @class ExtuiProgressBar:ExtuiStyledRenderable
--- @field Overlay string
--- @field Size vec2
--- @field Value number


--- @class ExtuiRadioButton:ExtuiStyledRenderable
--- @field Active boolean
--- @field OnChange RegistryEntry


--- @class ExtuiRenderable
--- @field Handle uint64
--- @field Parent uint64
--- @field ParentElement ImguiHandle
--- @field UserData RegistryEntry
--- @field Destroy fun(self:ExtuiRenderable)


--- @class ExtuiSelectable:ExtuiStyledRenderable
--- @field AllowDoubleClick boolean
--- @field AllowOverlap boolean
--- @field DontClosePopups boolean
--- @field Highlight boolean
--- @field SelectableDisabled boolean
--- @field Selected boolean
--- @field Size vec2?
--- @field SpanAllColumns boolean


--- @class ExtuiSeparator:ExtuiStyledRenderable


--- @class ExtuiSeparatorText:ExtuiStyledRenderable


--- @class ExtuiSliderInt:ExtuiStyledRenderable
--- @field AlwaysClamp boolean
--- @field ClampOnInput boolean
--- @field ClampZeroRange boolean
--- @field Components int32
--- @field Logarithmic boolean
--- @field Max ivec4
--- @field Min ivec4
--- @field NoInput boolean
--- @field NoRoundToFormat boolean
--- @field NoSpeedTweaks boolean
--- @field OnChange RegistryEntry
--- @field Value ivec4
--- @field Vertical boolean
--- @field VerticalSize vec2
--- @field WrapAround boolean


--- @class ExtuiSliderScalar:ExtuiStyledRenderable
--- @field AlwaysClamp boolean
--- @field ClampOnInput boolean
--- @field ClampZeroRange boolean
--- @field Components int32
--- @field Logarithmic boolean
--- @field Max vec4
--- @field Min vec4
--- @field NoInput boolean
--- @field NoRoundToFormat boolean
--- @field NoSpeedTweaks boolean
--- @field OnChange RegistryEntry
--- @field Value vec4
--- @field Vertical boolean
--- @field VerticalSize vec2
--- @field WrapAround boolean


--- @class ExtuiSortSpec
--- @field ColumnIndex int32
--- @field Direction GuiSortDirection


--- @class ExtuiSpacing:ExtuiStyledRenderable


--- @class ExtuiStyledRenderable:ExtuiRenderable
--- @field AbsolutePosition vec2?
--- @field AllowDuplicateId boolean
--- @field AllowItemOverlap boolean
--- @field AutoClosePopups boolean
--- @field ButtonRepeat boolean
--- @field CanDrag boolean
--- @field Disabled boolean
--- @field DragDropType FixedString
--- @field DragFlags GuiDragFlags
--- @field DragPreview ImguiHandle
--- @field DropFlags GuiDropFlags
--- @field Font FixedString
--- @field IDContext string
--- @field ItemReadOnly boolean
--- @field ItemWidth number?
--- @field Label string
--- @field NoItemNav boolean
--- @field NoMarkEdited boolean
--- @field NoNavDefaultFocus boolean
--- @field NoNavDisableMouseHover boolean
--- @field NoTabStop boolean
--- @field NoWindowHoverableCheck boolean
--- @field OnActivate RegistryEntry
--- @field OnClick RegistryEntry
--- @field OnDeactivate RegistryEntry
--- @field OnDragDrop RegistryEntry
--- @field OnDragEnd RegistryEntry
--- @field OnDragStart RegistryEntry
--- @field OnHoverEnter RegistryEntry
--- @field OnHoverLeave RegistryEntry
--- @field OnRightClick RegistryEntry
--- @field PositionOffset vec2?
--- @field RequestActivate boolean
--- @field SameLine boolean
--- @field StatusFlags GuiItemStatusFlags
--- @field TextWrapPos number?
--- @field Visible boolean
--- @field WasHovered boolean
--- @field Activate fun(self:ExtuiStyledRenderable)
--- @field GetColor fun(self:ExtuiStyledRenderable, a1:GuiColor):vec4?
--- @field GetStyle fun(self:ExtuiStyledRenderable, a1:GuiStyleVar):number?
--- @field SetColor fun(self:ExtuiStyledRenderable, a1:GuiColor, a2:vec4)
--- @field SetStyle fun(self:ExtuiStyledRenderable, a1:GuiStyleVar, a2:number, a3:number?)
--- @field Tooltip fun(self:ExtuiStyledRenderable):ExtuiTooltip


--- @class ExtuiTabBar:ExtuiTreeParent
--- @field AutoSelectNewTabs boolean
--- @field DrawSelectedOverline boolean
--- @field FittingPolicyResizeDown boolean
--- @field FittingPolicyScroll boolean
--- @field NoCloseWithMiddleMouseButton boolean
--- @field NoTabListScrollingButtons boolean
--- @field NoTooltip boolean
--- @field Reorderable boolean
--- @field TabListPopupButton boolean
--- @field AddTabItem fun(self:ExtuiTabBar, a1:string):ExtuiTabItem


--- @class ExtuiTabItem:ExtuiTreeParent
--- @field Leading boolean
--- @field NoAssumedClosure boolean
--- @field NoCloseWithMiddleMouseButton boolean
--- @field NoPushId boolean
--- @field NoReorder boolean
--- @field NoTooltip boolean
--- @field SetSelected boolean
--- @field Trailing boolean
--- @field UnsavedDocument boolean


--- @class ExtuiTable:ExtuiTreeParent
--- @field AngledHeader boolean
--- @field Borders boolean
--- @field BordersH boolean
--- @field BordersInner boolean
--- @field BordersInnerH boolean
--- @field BordersInnerV boolean
--- @field BordersOuter boolean
--- @field BordersOuterH boolean
--- @field BordersOuterV boolean
--- @field BordersV boolean
--- @field ColumnDefs ExtuiColumnDefinition[]
--- @field Columns uint32
--- @field ContextMenuInBody boolean
--- @field FreezeCols uint32
--- @field FreezeRows uint32
--- @field Hideable boolean
--- @field HighlightHoveredColumn boolean
--- @field NoBordersInBody boolean
--- @field NoBordersInBodyUntilResize boolean
--- @field NoClip boolean
--- @field NoHostExtendX boolean
--- @field NoHostExtendY boolean
--- @field NoKeepColumnsVisible boolean
--- @field NoPadInnerX boolean
--- @field NoPadOuterX boolean
--- @field NoSavedSettings boolean
--- @field OnSortChanged RegistryEntry
--- @field OptimizedDraw boolean
--- @field PadOuterX boolean
--- @field PreciseWidths boolean
--- @field Reorderable boolean
--- @field Resizable boolean
--- @field RowBg boolean
--- @field ScrollX boolean
--- @field ScrollY boolean
--- @field ShowHeader boolean
--- @field Size vec2?
--- @field SizingFixedFit boolean
--- @field SizingFixedSame boolean
--- @field SizingStretchProp boolean
--- @field SizingStretchSame boolean
--- @field SortMulti boolean
--- @field SortTristate boolean
--- @field Sortable boolean
--- @field Sorting ExtuiSortSpec[]
--- @field AddColumn fun(self:ExtuiTable, a1:string, a2:GuiTableColumnFlags?, a3:number?)
--- @field AddRow fun(self:ExtuiTable):ExtuiTableRow


--- @class ExtuiTableCell:ExtuiTreeParent


--- @class ExtuiTableRow:ExtuiTreeParent
--- @field Headers boolean
--- @field AddCell fun(self:ExtuiTableRow):ExtuiTableCell


--- @class ExtuiText:ExtuiStyledRenderable


--- @class ExtuiTextLink:ExtuiStyledRenderable


--- @class ExtuiTooltip:ExtuiWindowBase


--- @class ExtuiTree:ExtuiTreeParent
--- @field AllowOverlap boolean
--- @field Bullet boolean
--- @field CollapsingHeader boolean
--- @field DefaultOpen boolean
--- @field FramePadding boolean
--- @field Framed boolean
--- @field Leaf boolean
--- @field NavLeftJumpsBackHere boolean
--- @field NoAutoOpenOnLog boolean
--- @field NoTreePushOnOpen boolean
--- @field OnCollapse RegistryEntry
--- @field OnExpand RegistryEntry
--- @field OpenOnArrow boolean
--- @field OpenOnDoubleClick boolean
--- @field Selected boolean
--- @field SpanAllColumns boolean
--- @field SpanAvailWidth boolean
--- @field SpanFullWidth boolean
--- @field SpanTextWidth boolean
--- @field SetOpen fun(self:ExtuiTree, a1:boolean, a2:GuiCond?)


--- @class ExtuiTreeParent:ExtuiStyledRenderable
--- @field Children ImguiHandle[]
--- @field AddBulletText fun(self:ExtuiTreeParent, a1:string):ExtuiBulletText
--- @field AddButton fun(self:ExtuiTreeParent, a1:string):ExtuiButton
--- @field AddCheckbox fun(self:ExtuiTreeParent, a1:string, a2:boolean?):ExtuiCheckbox
--- @field AddChildWindow fun(self:ExtuiTreeParent, a1:string):ExtuiChildWindow
--- @field AddCollapsingHeader fun(self:ExtuiTreeParent, a1:string):ExtuiCollapsingHeader
--- @field AddColorEdit fun(self:ExtuiTreeParent, a1:string, a2:vec3?):ExtuiColorEdit
--- @field AddColorPicker fun(self:ExtuiTreeParent, a1:string, a2:vec3?):ExtuiColorPicker
--- @field AddCombo fun(self:ExtuiTreeParent, a1:string):ExtuiCombo
--- @field AddDrag fun(self:ExtuiTreeParent, a1:string, a2:number?, a3:number?, a4:number?):ExtuiDrag
--- @field AddDragInt fun(self:ExtuiTreeParent, a1:string, a2:int32?, a3:int32?, a4:int32?):ExtuiDragInt
--- @field AddDummy fun(self:ExtuiTreeParent, a1:number, a2:number):ExtuiDummy
--- @field AddGroup fun(self:ExtuiTreeParent, a1:string):ExtuiGroup
--- @field AddIcon fun(self:ExtuiTreeParent, a1:FixedString, a2:vec2?):ExtuiIcon
--- @field AddImage fun(self:ExtuiTreeParent, a1:FixedString, a2:vec2?, a3:vec2?, a4:vec2?):ExtuiImage
--- @field AddImageButton fun(self:ExtuiTreeParent, a1:string, a2:FixedString, a3:vec2?, a4:vec2?, a5:vec2?):ExtuiImageButton
--- @field AddInputInt fun(self:ExtuiTreeParent, a1:string, a2:int32?):ExtuiInputInt
--- @field AddInputScalar fun(self:ExtuiTreeParent, a1:string, a2:number?):ExtuiInputScalar
--- @field AddInputText fun(self:ExtuiTreeParent, a1:string, a2:string?):ExtuiInputText
--- @field AddMenu fun(self:ExtuiTreeParent, a1:string):ImguiHandle
--- @field AddNewLine fun(self:ExtuiTreeParent):ExtuiNewLine
--- @field AddPopup fun(self:ExtuiTreeParent, a1:string):ExtuiPopup
--- @field AddProgressBar fun(self:ExtuiTreeParent):ImguiHandle
--- @field AddRadioButton fun(self:ExtuiTreeParent, a1:string, a2:boolean?):ExtuiRadioButton
--- @field AddSelectable fun(self:ExtuiTreeParent, a1:string, a2:GuiSelectableFlags?, a3:vec2?):ImguiHandle
--- @field AddSeparator fun(self:ExtuiTreeParent):ExtuiSeparator
--- @field AddSeparatorText fun(self:ExtuiTreeParent, a1:string):ExtuiSeparatorText
--- @field AddSlider fun(self:ExtuiTreeParent, a1:string, a2:number?, a3:number?, a4:number?):ExtuiSlider
--- @field AddSliderInt fun(self:ExtuiTreeParent, a1:string, a2:int32?, a3:int32?, a4:int32?):ExtuiSliderInt
--- @field AddSpacing fun(self:ExtuiTreeParent):ExtuiSpacing
--- @field AddTabBar fun(self:ExtuiTreeParent, a1:string):ExtuiTabBar
--- @field AddTable fun(self:ExtuiTreeParent, a1:string, a2:uint32):ExtuiTable
--- @field AddText fun(self:ExtuiTreeParent, a1:string):ExtuiText
--- @field AddTextLink fun(self:ExtuiTreeParent, a1:string):ImguiHandle
--- @field AddTree fun(self:ExtuiTreeParent, a1:string):ExtuiTree
--- @field AttachChild fun(self:ExtuiTreeParent, a1:ImguiHandle):boolean
--- @field DetachChild fun(self:ExtuiTreeParent, a1:ImguiHandle):boolean
--- @field RemoveChild fun(self:ExtuiTreeParent, a1:ImguiHandle):boolean


--- @class ExtuiWindow:ExtuiWindowBase
--- @field Closeable boolean
--- @field OnClose RegistryEntry
--- @field Open boolean
--- @field Scaling GuiMeasureScaling
--- @field AddMainMenu fun(self:ExtuiWindow):ExtuiMenu


--- @class ExtuiWindowBase:ExtuiTreeParent
--- @field AlwaysAutoResize boolean
--- @field AlwaysHorizontalScrollbar boolean
--- @field AlwaysVerticalScrollbar boolean
--- @field HorizontalScrollbar boolean
--- @field LastPosition vec2
--- @field LastSize vec2
--- @field MenuBar boolean
--- @field NoBackground boolean
--- @field NoBringToFrontOnFocus boolean
--- @field NoCollapse boolean
--- @field NoDecoration boolean
--- @field NoFocusOnAppearing boolean
--- @field NoInputs boolean
--- @field NoMouseInputs boolean
--- @field NoMove boolean
--- @field NoNav boolean
--- @field NoNavFocus boolean
--- @field NoNavInputs boolean
--- @field NoResize boolean
--- @field NoSavedSettings boolean
--- @field NoScrollWithMouse boolean
--- @field NoScrollbar boolean
--- @field NoTitleBar boolean
--- @field UnsavedDocument boolean
--- @field SetBgAlpha fun(self:ExtuiWindowBase, a1:number?)
--- @field SetCollapsed fun(self:ExtuiWindowBase, a1:boolean, a2:GuiCond?)
--- @field SetContentSize fun(self:ExtuiWindowBase, a1:vec2?)
--- @field SetFocus fun(self:ExtuiWindowBase)
--- @field SetPos fun(self:ExtuiWindowBase, a1:vec2, a2:GuiCond?, a3:vec2?)
--- @field SetScroll fun(self:ExtuiWindowBase, a1:vec2?)
--- @field SetSize fun(self:ExtuiWindowBase, a1:vec2, a2:GuiCond?)
--- @field SetSizeConstraints fun(self:ExtuiWindowBase, a1:vec2?, a2:vec2?)


--- @class GamePauseComponent:BaseComponent


--- @class GamePauseExcludedComponent:BaseComponent


--- @class GnBlueprintInstanceData
--- @field Blueprint GnGenomeBlueprint
--- @field Flags uint32
--- @field ThreadEvent uint64


--- @class GnGenomeBlueprint
--- @field Events table<FixedString, GnGenomeEventTypeIndex>
--- @field ExternalGroups table<FixedString, GnGenomeExternalGroupTemplate>
--- @field IsValid boolean
--- @field Nodes GnGenomeNodeTemplate[]
--- @field Sequences GnGenomeSequenceTemplate[]
--- @field StackVariables GnGenomeVariableTemplate[]
--- @field Stacks GnGenomeVariant[][]
--- @field VariableIndices table<Guid, int16>
--- @field Variables GnGenomeVariableTemplate[]
--- @field Version Version


--- @class GnGenomeBlueprintInstance
--- @field CreationFlags uint32
--- @field Entity EntityRef
--- @field Nodes GnGenomeNode[]
--- @field Parent GnGenomeBlueprint
--- @field Sequences GnGenomeSequence[]
--- @field VariablesForEdit GnGenomeVariant[]


--- @class GnGenomeEventTypeIndex
--- @field Index uint16
--- @field Type uint8


--- @class GnGenomeExternalGroupTemplate
--- @field Variables uint16[]


--- @class GnGenomeNode


--- @class GnGenomeNodeTemplate
--- @field Index uint16


--- @class GnGenomeParametrizedEventArgs
--- @field Args GnGenomeVariant[]


--- @class GnGenomeSequence
--- @field Clock int32
--- @field Template GnGenomeSequenceTemplate


--- @class GnGenomeSequenceTemplate
--- @field Index uint16
--- @field InputVariables uint16[]
--- @field NodeEvents table<FixedString, uint16[]>
--- @field OutputVariables uint16[]


--- @class GnGenomeVariableTemplate
--- @field ID Guid
--- @field Index uint16
--- @field Name string
--- @field Type uint8
--- @field Value GnGenomeVariant


--- @class GnGenomeVariant
--- @field Type FixedString
--- @field Value UserReturn


--- @class HealBlockComponent:BaseComponent


--- @class HitAttackerComponent:BaseComponent
--- @field Attacker EntityHandle


--- @class HitLifetimeComponent:BaseComponent
--- @field Lifetime number
--- @field field_4 uint8


--- @class HitMetaComponent:BaseComponent
--- @field HitGuid Guid


--- @class HitProxyComponent:BaseComponent
--- @field Owner EntityHandle
--- @field field_8 FixedString


--- @class HitProxyOwnerComponent:BaseComponent
--- @field Owners EntityHandle[]


--- @class HitReactionComponent:BaseComponent
--- @field Reactions table<EntityHandle, HitReactionSource>


--- @class HitReactionSource
--- @field Target HitTargetInfo
--- @field field_0 EntityHandle


--- @class HitTargetComponent:BaseComponent
--- @field Target HitTargetInfo
--- @field field_0 EntityHandle
--- @field field_8 EntityHandle


--- @class HitTargetInfo
--- @field ArmorAbsorption int32
--- @field AttackFlags uint8
--- @field CauseType CauseType
--- @field ConditionRollResults boolean[]
--- @field Damage DamagePair[]
--- @field EffectFlags uint32
--- @field HitWith HitWith
--- @field ImpactDirection vec3
--- @field ImpactPosition vec3
--- @field MainDamageType DamageType
--- @field ResistanceTypes Set_uint8
--- @field SpellCastGuid Guid
--- @field SpellId FixedString
--- @field StatusId FixedString
--- @field TotalDamageDone int32
--- @field field_8 uint8
--- @field field_A0 uint8
--- @field field_A1 uint8
--- @field field_A2 uint8
--- @field field_A3 uint8


--- @class HitThrownObjectComponent:BaseComponent
--- @field ThrownObject EntityHandle


--- @class HitWeaponComponent:BaseComponent
--- @field Weapon EntityHandle


--- @class InputFireEventDesc
--- @field DeviceId uint16
--- @field Event InputInputEvent
--- @field EventDesc InputInputEventDesc
--- @field InputId InputRawType
--- @field PlayerIndex uint8
--- @field field_34 int32


--- @class InputHoldRepeatEventDesc
--- @field Acceleration int32
--- @field FireEvent InputFireEventDesc
--- @field HoldTime number
--- @field PressTime number
--- @field ReleaseTime number


--- @class InputHoldRepeatEvents
--- @field HoldEvents InputFireEventDesc[]
--- @field HoldRepeatEvents InputHoldRepeatEventDesc[]
--- @field PressEvents InputFireEventDesc[]
--- @field ReleaseEvents InputFireEventDesc[]
--- @field RepeatEvents InputFireEventDesc[]
--- @field ValueChangeEvents InputFireEventDesc[]


--- @class InputInjectDeviceEvent
--- @field DeviceId uint16
--- @field EventId uint32


--- @class InputInjectInputData:InputInputRawChange


--- @class InputInputBinding:InputInputRaw
--- @field InputPlayerIndex uint8
--- @field Modifiers InputModifier
--- @field field_9 uint8


--- @class InputInputBindingDesc
--- @field Binding InputInputBinding
--- @field BindingIndex int32
--- @field InputIndex int32


--- @class InputInputDevice
--- @field ControllerMapping int32
--- @field DeviceId uint16
--- @field InputPlayerIndex int64
--- @field field_14 number[]
--- @field field_24 uint8
--- @field field_8 int32


--- @class InputInputEvent
--- @field DeviceId uint16
--- @field EventId int32
--- @field InputPlayerIndex uint8
--- @field NewValue InputInputValue
--- @field OldValue InputInputValue
--- @field Type InputType
--- @field WasPreferred boolean


--- @class InputInputEventDesc
--- @field CategoryName FixedString
--- @field EventDesc TranslatedString
--- @field EventID int32
--- @field EventName LSStringView
--- @field Flags uint32
--- @field Type InputType
--- @field field_E8 int32


--- @class InputInputEventText
--- @field PlayerIndex uint8
--- @field Text string
--- @field TextLength uint64


--- @class InputInputManager
--- @field AllowDeviceEvents boolean
--- @field ControllerAllowKeyboardMouseInput boolean
--- @field CurrentRemap InputPendingInputRemap
--- @field DeviceEventInjects InputInjectDeviceEvent[]
--- @field Events InputFireEventDesc[]
--- @field HoldRepeatEvents table<uint32, InputHoldRepeatEvents>
--- @field InputDefinitions table<int32, InputInputEventDesc>
--- @field InputInjects InputInjectInputData[]
--- @field InputScheme InputInputScheme
--- @field InputStates table<uint16, InputInputValueSet>
--- @field LastUpdateTime number
--- @field PerDeviceData InputInputDevice[]
--- @field PlayerColors vec4[]
--- @field PlayerDeviceIDs uint16[]
--- @field PlayerDevices int32[]
--- @field PressedModifiers InputModifier
--- @field RawInputs InputInputRaw[]


--- @class InputInputRaw
--- @field DeviceId uint16
--- @field InputId InputRawType


--- @class InputInputRawChange
--- @field Debug boolean
--- @field InjectRawInput boolean
--- @field Input InputInputRaw
--- @field Value InputInputValue


--- @class InputInputScheme
--- @field ControllerRemaps table<uint16, uint16>[]
--- @field DebugEvents Set_uint32[]
--- @field DeviceLists uint16[][]
--- @field InputBindings table<uint32, InputInputBinding[]>[]
--- @field RawToBinding InputRawBindingDataMapping[]


--- @class InputInputValue
--- @field State InputState
--- @field Value number
--- @field Value2 number


--- @class InputInputValueSet
--- @field Initialized boolean
--- @field Inputs InputInputValue[]


--- @class InputPendingInputRemap
--- @field Binding InputInputBinding


--- @class InputRawBindingDataMapping
--- @field Bindings InputInputBindingDesc[]
--- @field HasModifierKeys boolean


--- @class InterruptActionEntry
--- @field Interrupt EntityHandle
--- @field Observer EntityHandle


--- @class InterruptActionStateComponent:BaseComponent
--- @field Actions InterruptActionEntry[]
--- @field Event InterruptInterruptEvent
--- @field SpellCastGuid Guid


--- @class InterruptAnimationInterruptData:InterruptPrecalculatedAnimationInterruptData
--- @field DamageRollAdjustments table<InterruptDamageFunctorKey, InterruptDamageRollAdjustments>
--- @field ExecutedDependencies2 table<Guid, InterruptExecutedDependency>
--- @field InterruptUsage table<EntityHandle, InterruptInterruptUsage>
--- @field Interruptors table<InterruptInterruptEvent, table<EntityHandle, Set_EntityHandle>>
--- @field RollAdjustments table<Guid, InterruptRollAdjustments>
--- @field field_128 int32
--- @field field_12C number


--- @class InterruptAppliedChange
--- @field Adjustments InterruptRollAdjustments[]
--- @field DamageRollAdjustments table<InterruptDamageFunctorKey, InterruptDamageRollAdjustments>
--- @field Flags uint8


--- @class InterruptCastHitEvent
--- @field AbilitySavingThrow AbilityId
--- @field Cause FixedString
--- @field CauseSpell FixedString
--- @field DamageDone int32
--- @field DamageEffectFlags DamageFlags
--- @field DamageRolls DamagePair[]
--- @field HitDescFlags uint8
--- @field Spell SpellId
--- @field SpellAttackType uint8
--- @field SpellCastGuid Guid
--- @field ThrownObject EntityHandle
--- @field field_20 int32
--- @field field_38 EntityHandle
--- @field field_68 AbilityId


--- @class InterruptConditionResultEvent
--- @field ConditionResult boolean
--- @field Conditions stats::ConditionId
--- @field DamageEffectFlags DamageFlags
--- @field FunctorUuid Guid
--- @field RollUuid Guid
--- @field SpellCastUuid Guid
--- @field SpellId FixedString
--- @field StatsFunctorContext StatsPropertyContext
--- @field field_0 Guid
--- @field field_10 FixedString
--- @field field_18 Guid
--- @field field_28 Guid
--- @field field_38 stats::ConditionId
--- @field field_48 boolean


--- @class InterruptConditionalRollEvent
--- @field Ability AbilityId
--- @field ConditionRollType ConditionRollType
--- @field DC int32
--- @field Passive EntityHandle
--- @field PassiveId FixedString
--- @field Roll InterruptRollData
--- @field RollIndex uint64
--- @field RollUuid Guid
--- @field RollUuid2 Guid
--- @field SwappedSourceAndTarget boolean
--- @field field_10 Guid
--- @field field_20 uint64
--- @field field_28 EntityHandle
--- @field field_30 FixedString
--- @field field_50 int32
--- @field field_55 boolean


--- @class InterruptConditionallyDisabledComponent:BaseComponent
--- @field Dummy uint8


--- @class InterruptContainerComponent:BaseComponent
--- @field Interrupts EntityHandle[]


--- @class InterruptDamageFunctorKey
--- @field field_0 Guid
--- @field field_10 EntityHandle


--- @class InterruptDamageRollAdjustments
--- @field DamageTypes Set_DamageType
--- @field DamageTypes2 Set_DamageType
--- @field RerollConditions RerollCondition[]
--- @field TotalAdjustments table<uint8, table<DamageType, InterruptTotalRollAdjustments>>
--- @field field_40 table<uint8, table<DamageType, int32>>


--- @class InterruptDataComponent:BaseComponent
--- @field Caster EntityHandle
--- @field Interrupt FixedString
--- @field InterruptEntity EntityHandle
--- @field Spell FixedString
--- @field field_10 EntityHandle
--- @field field_18 FixedString
--- @field field_4 uint8
--- @field field_8 EntityHandle


--- @class InterruptDeathEvent
--- @field field_0 uint8


--- @class InterruptDebugEvent
--- @field field_0 uint8


--- @class InterruptDecisionComponent:BaseComponent
--- @field Decisions table<InterruptInterruptEvent, uint8>


--- @class InterruptDelayedTargetHitInterruptEvent
--- @field Attack AttackDesc
--- @field EventUuid Guid
--- @field Hit HitDesc
--- @field Source EntityHandle
--- @field SourcePos vec3?
--- @field Spell SpellId
--- @field Target EntityHandle
--- @field TargetPos vec3?
--- @field field_10 boolean
--- @field field_14 uint32
--- @field field_1E8 EntityHandle
--- @field field_1F0 EntityHandle
--- @field field_1F8 vec3?
--- @field field_208 vec3?
--- @field field_48 Guid


--- @class InterruptDependency
--- @field field_0 Guid
--- @field field_10 uint8


--- @class InterruptEnterAttackRangeEvent
--- @field Event FixedString
--- @field Position vec3


--- @class InterruptExecutedDependency
--- @field field_0 uint32


--- @class InterruptInterruptEvent
--- @field Event Variant<InterruptSpellCastEvent,InterruptCastHitEvent,InterruptSpellRollEvent,InterruptConditionalRollEvent,InterruptEnterAttackRangeEvent,InterruptLeaveAttackRangeEvent,InterruptSpellPreDamageEvent,InterruptPlaceholderSpellRollEvent,InterruptConditionResultEvent,InterruptStatusAppliedEvent,InterruptDeathEvent,InterruptDebugEvent>
--- @field Source EntityHandle
--- @field SourcePos vec3?
--- @field SourceProxy EntityHandle
--- @field SwapSourceTarget boolean
--- @field Target EntityHandle
--- @field TargetPos vec3?
--- @field TargetProxy EntityHandle
--- @field Targets Guid[]
--- @field field_100 boolean
--- @field field_D0 Guid[]


--- @class InterruptInterruptUsage
--- @field Costs StatsActionResourceCost[]
--- @field Usage table<EntityHandle, table<InterruptInterruptEvent, InterruptInterruptUsageEntry>>


--- @class InterruptInterruptUsageEntry
--- @field ConditionRolls ConditionRolls
--- @field RollAdjustments InterruptRollAdjustments
--- @field field_0 uint8
--- @field field_70 uint8


--- @class InterruptLeaveAttackRangeEvent
--- @field Event FixedString
--- @field Position vec3


--- @class InterruptPausedAnimationEvent
--- @field PauseFinishedAnimations boolean
--- @field TextKey FixedString


--- @class InterruptPlaceholderSpellRollEvent
--- @field Conditions stats::ConditionId
--- @field Dependency InterruptDependency
--- @field FunctorUuid Guid
--- @field RollUuid Guid
--- @field SpellCastUuid Guid
--- @field SpellId FixedString
--- @field StatsFunctorContext StatsPropertyContext
--- @field TextKey FixedString
--- @field field_0 Guid
--- @field field_10 Guid
--- @field field_20 FixedString
--- @field field_28 Guid
--- @field field_58 FixedString
--- @field field_5C stats::ConditionId


--- @class InterruptPrecalculatedAnimationInterruptData
--- @field Events InterruptInterruptEvent[]
--- @field ExecutedDependencies table<Guid, InterruptExecutedDependency>
--- @field PrecalculatedInterrupts InterruptPrecalculatedInterruptEvent[]
--- @field Resolved table<Guid, InterruptResolvedData>
--- @field Usage table<EntityHandle, table<InterruptInterruptEvent, InterruptInterruptUsageEntry>>
--- @field field_20 uint8


--- @class InterruptPrecalculatedInterruptEvent:InterruptInterruptEvent
--- @field AutoInterrupts table<EntityHandle, Set_EntityHandle>
--- @field PendingInterrupts table<EntityHandle, Set_EntityHandle>
--- @field field_138 table<EntityHandle, Set_EntityHandle>
--- @field field_F8 table<EntityHandle, Set_EntityHandle>


--- @class InterruptPreferencesComponent:BaseComponent
--- @field Preferences table<FixedString, uint8>


--- @class InterruptPreparedComponent:BaseComponent
--- @field Dummy uint8


--- @class InterruptResolvedData
--- @field RerollValues RerollValue[]
--- @field field_0 int64


--- @class InterruptRollAdjustmentMetadata
--- @field Adjustment int32
--- @field Source TranslatedString


--- @class InterruptRollAdjustments
--- @field BaseAdjustment int32
--- @field FixedAdjustment int32?
--- @field FixedRollBonuses FixedRollBonus[]
--- @field RerollConditions RerollCondition[]
--- @field RerollType uint8
--- @field RerollValues RerollValue[]
--- @field ResolvedRoll int32?
--- @field ResolvedRolls FixedRollBonus[]
--- @field UseMaxRoll uint8


--- @class InterruptRollData
--- @field AmountOfDices uint8
--- @field DiceAdditionalValue int32
--- @field DiceValue uint8
--- @field InitialReroll_M int32
--- @field NaturalRoll int32
--- @field RerollType uint8
--- @field ResultCritical uint8
--- @field Total int32


--- @class InterruptSpellCastEvent
--- @field Cause FixedString
--- @field Spell SpellId
--- @field SpellCastGuid Guid


--- @class InterruptSpellPreDamageEvent
--- @field Ability AbilityId
--- @field Cause FixedString
--- @field CauseSpell FixedString
--- @field DamageEffectFlags DamageFlags
--- @field DamageFunctorId Guid
--- @field DamageType DamageType
--- @field Dependency1 InterruptDependency
--- @field Dependency2 InterruptDependency
--- @field Spell SpellId
--- @field SpellAttackType SpellAttackType
--- @field SpellCastGuid Guid
--- @field SpellRollIndex uint32
--- @field field_0 Guid
--- @field field_90 uint32


--- @class InterruptSpellRollEvent
--- @field Ability AbilityId
--- @field CauseSpell FixedString
--- @field ConditionResult boolean
--- @field ConditionRollType ConditionRollType
--- @field CriticalRollThreshold int32?
--- @field Difficulty int32
--- @field Roll InterruptRollData
--- @field RollUuid Guid
--- @field RollUuid2 Guid
--- @field Spell SpellId
--- @field SpellAttackType SpellAttackType
--- @field SpellCastGuid Guid
--- @field field_10 FixedString
--- @field field_48 int32?
--- @field field_52 boolean


--- @class InterruptStatusAppliedEvent
--- @field field_0 FixedString


--- @class InterruptTotalRollAdjustments
--- @field Metadata InterruptRollAdjustmentMetadata[]
--- @field field_0 int32
--- @field field_4 int32


--- @class InterruptUndecidedEvent
--- @field Event InterruptInterruptEvent
--- @field HasReplacement boolean
--- @field Replacement InterruptInterruptEvent
--- @field UndecidedInterrupts table<EntityHandle, Set_EntityHandle>


--- @class InterruptUndecidedEventWithId
--- @field Event InterruptUndecidedEvent
--- @field EventUuid Guid


--- @class InterruptZoneComponent:BaseComponent
--- @field field_0 Guid


--- @class InterruptZoneParticipantComponent:BaseComponent
--- @field field_0 table<EntityHandle, uint8>


--- @class InterruptZoneSourceComponent:BaseComponent
--- @field Dummy uint8


--- @class InventoryCanBeInComponent:BaseComponent


--- @class InventoryCannotBePickpocketedComponent:BaseComponent


--- @class InventoryCannotBeTakenOutComponent:BaseComponent


--- @class InventoryContainerComponent:BaseComponent
--- @field Items table<uint16, InventoryContainerSlotData>


--- @class InventoryContainerSlotData
--- @field Item EntityHandle
--- @field field_8 uint32


--- @class InventoryDataComponent:BaseComponent
--- @field Flags uint16
--- @field field_0 uint8


--- @class InventoryDropOnDeathBlockedComponent:BaseComponent


--- @class InventoryIsLockedComponent:BaseComponent


--- @class InventoryIsOwnedComponent:BaseComponent
--- @field Owner EntityHandle


--- @class InventoryMemberComponent:BaseComponent
--- @field EquipmentSlot int16
--- @field Inventory EntityHandle


--- @class InventoryMemberTransformComponent:BaseComponent
--- @field Transform Transform


--- @class InventoryNewItemsInsideComponent:BaseComponent


--- @class InventoryNonTradableComponent:BaseComponent


--- @class InventoryOwnerComponent:BaseComponent
--- @field Inventories EntityHandle[]
--- @field PrimaryInventory EntityHandle


--- @class InventoryStackComponent:BaseComponent
--- @field Arr_u64 EntityHandle[]
--- @field Arr_u8 InventoryStackEntry[]
--- @field Elements EntityHandle[]
--- @field Entries InventoryStackEntry[]


--- @class InventoryStackEntry
--- @field EntityIndex uint16
--- @field Quantity uint32


--- @class InventoryStackMemberComponent:BaseComponent
--- @field Stack EntityHandle


--- @class InventoryTopOwnerComponent:BaseComponent
--- @field TopOwner EntityHandle


--- @class InventoryTradeBuybackDataComponent:BaseComponent
--- @field Amount uint16
--- @field Buyer EntityHandle
--- @field Trader EntityHandle


--- @class InventoryWeightComponent:BaseComponent
--- @field Weight int32


--- @class InventoryWieldedComponent:BaseComponent
--- @field field_0 Guid


--- @class InventoryWieldingHistoryComponent:BaseComponent
--- @field field_0 Guid


--- @class ItemDestroyInfo
--- @field field_0 FixedString
--- @field field_4 uint8


--- @class ItemDestroyedComponent:BaseComponent
--- @field Info ItemDestroyInfo?


--- @class ItemDestroyingComponent:BaseComponent


--- @class ItemDoorComponent:BaseComponent


--- @class ItemDyeComponent:BaseComponent
--- @field Color Guid


--- @class ItemExamineDisabledComponent:BaseComponent


--- @class ItemHasMovedComponent:BaseComponent


--- @class ItemHasOpenedComponent:BaseComponent


--- @class ItemInUseComponent:BaseComponent


--- @class ItemIsGoldComponent:BaseComponent


--- @class ItemIsPoisonedComponent:BaseComponent


--- @class ItemItemComponent:BaseComponent


--- @class ItemMapMarkerStyleComponent:BaseComponent
--- @field Style FixedString


--- @class ItemNewInInventoryComponent:BaseComponent


--- @class ItemPortalComponent:BaseComponent
--- @field field_0 uint8
--- @field field_1 uint8


--- @class ItemShouldDestroyOnSpellCastComponent:BaseComponent


--- @class ItemTemplateActionTypeComponent:BaseComponent
--- @field ActionTypes Set_uint8


--- @class ItemTemplateCanMoveComponent:BaseComponent


--- @class ItemTemplateClimbOnComponent:BaseComponent


--- @class ItemTemplateDestroyedComponent:BaseComponent


--- @class ItemTemplateInteractionDisabledComponent:BaseComponent


--- @class ItemTemplateIsStoryItemComponent:BaseComponent


--- @class ItemTemplateLadderComponent:BaseComponent


--- @class ItemTemplateUseActionComponent:BaseComponent
--- @field UseActions IActionData[]


--- @class ItemTemplateWalkOnComponent:BaseComponent


--- @class LuaDoConsoleCommandEvent:LuaEventBase
--- @field Command string


--- @class LuaEmptyEvent:LuaEventBase


--- @class LuaEventBase
--- @field ActionPrevented boolean
--- @field CanPreventAction boolean
--- @field Name FixedString
--- @field Stopped boolean
--- @field PreventAction fun(self:LuaEventBase)
--- @field StopPropagation fun(self:LuaEventBase)


--- @class LuaFindPathEvent:LuaEventBase
--- @field Path AiPath


--- @class LuaNetMessageEvent:LuaEventBase
--- @field Channel string
--- @field Module Guid?
--- @field Payload string
--- @field RequestId uint32?
--- @field UserID UserId


--- @class LuaTickEvent:LuaEventBase
--- @field Time GameTime


--- @class MultiplayerHostComponent:BaseComponent


--- @class MultiplayerUserComponent:BaseComponent
--- @field UserID UserId


--- @class NavigationTargetInfo
--- @field AddSourceBoundsToMargin boolean
--- @field AiTargetCheck uint32
--- @field CloseEnoughCeiling number
--- @field CloseEnoughFloor number
--- @field CloseEnoughMax number
--- @field CloseEnoughMin number
--- @field CloseEnoughPreference int32
--- @field CoverFlags uint16
--- @field IgnoreEntities EntityHandle[]
--- @field IsAvoidingObstacles boolean
--- @field IsPreciseItemInteraction boolean
--- @field PickupEntity EntityHandle
--- @field PickupPosition vec3?
--- @field Position vec3
--- @field SurfaceAvoidance boolean
--- @field Target EntityHandle
--- @field WeightFuncType uint8
--- @field field_3F boolean


--- @class OwnershipOwnedAsLootComponent:BaseComponent


--- @class OwnershipOwneeCurrentComponent:BaseComponent
--- @field Ownee EntityHandle


--- @class PartyBlockFollowComponent:BaseComponent


--- @class PartyCompositionComponent:BaseComponent
--- @field Members PartyMember[]
--- @field Party EntityHandle
--- @field PartyUuid Guid
--- @field field_8 Guid


--- @class PartyCurrentlyFollowingPartyComponent:BaseComponent


--- @class PartyFollowerComponent:BaseComponent
--- @field Following EntityHandle


--- @class PartyMember
--- @field UserId int32
--- @field UserUuid Guid
--- @field ViewUuid Guid
--- @field field_0 int32
--- @field field_18 Guid
--- @field field_28 uint8[]
--- @field field_8 Guid


--- @class PartyMemberComponent:BaseComponent
--- @field IsPermanent boolean
--- @field Party EntityHandle
--- @field UserID int32
--- @field UserId int32
--- @field UserUuid Guid
--- @field ViewUuid Guid
--- @field field_20 Guid
--- @field field_30 boolean
--- @field field_8 Guid


--- @class PartyPortalsComponent:BaseComponent
--- @field Portals Set_EntityHandle


--- @class PartyRecipe
--- @field Name FixedString
--- @field field_4 uint8


--- @class PartyRecipesComponent:BaseComponent
--- @field Recipes PartyRecipe[]


--- @class PartyView
--- @field Characters EntityHandle[]
--- @field UserID int32
--- @field UserUuid Guid
--- @field ViewUuid Guid
--- @field field_18 Guid
--- @field field_8 Guid


--- @class PartyViewComponent:BaseComponent
--- @field Characters EntityHandle[]
--- @field PartyUuid Guid
--- @field Views PartyView[]
--- @field field_0 Guid


--- @class PartyWaypoint
--- @field Level FixedString
--- @field Name FixedString
--- @field field_8 Guid


--- @class PartyWaypointsComponent:BaseComponent
--- @field Waypoints Set_PartyWaypoint


--- @class PathBezier3Trajectory
--- @field DistanceMax number
--- @field DistanceMin number
--- @field ShiftMax number
--- @field ShiftMin number


--- @class PathBezier4Trajectory
--- @field DistanceMax number
--- @field DistanceMin number
--- @field ShiftAMax number
--- @field ShiftAMin number
--- @field ShiftBMax number
--- @field ShiftBMin number


--- @class PathConstantVelocity
--- @field ConstantVelocity number


--- @class PathLinearVelocity
--- @field Acceleration number
--- @field InitialSpeed number


--- @class PathMappedVelocity
--- @field Mapping FixedString


--- @class PathPathMover:PathSettings
--- @field ComputedTrajectoryValues number[]
--- @field ComputedVelocityValues vec3[]
--- @field Initialized boolean
--- @field InterpolateValue number
--- @field SourcePosition vec3
--- @field SourceRotation quat
--- @field TargetPosition vec3
--- @field TargetRotation quat


--- @class PathSettings
--- @field RotateMode uint8
--- @field Trajectory Variant<PathBezier3Trajectory,PathBezier4Trajectory>
--- @field Velocity Variant<PathConstantVelocity,PathLinearVelocity,PathMappedVelocity>


--- @class PhxPhysicsHit
--- @field Distance number
--- @field Normal vec3
--- @field PhysicsExtraFlags PhysicsExtraFlags
--- @field PhysicsGroup PhysicsGroupFlags
--- @field Position vec3
--- @field Shape PhxPhysicsShape


--- @class PhxPhysicsHitAll
--- @field Distances number[]
--- @field Normals vec3[]
--- @field PhysicsExtraFlags PhysicsExtraFlags[]
--- @field PhysicsGroup PhysicsGroupFlags[]
--- @field Positions vec3[]
--- @field Shapes PhxPhysicsShape[]


--- @class PhxPhysicsKinematicTargetInfo
--- @field Rotate quat
--- @field Shape PhxPhysicsShape
--- @field Translate vec3
--- @field field_24 int32


--- @class PhxPhysicsObject
--- @field CollidesWith PhysicsGroupFlags
--- @field Entity EntityHandle
--- @field PhysicsExtraFlags PhysicsExtraFlags
--- @field PhysicsGroup PhysicsGroupFlags
--- @field PhysicsObjectIndex int32
--- @field Rotate quat
--- @field Scale vec3
--- @field Shapes PhxPhysicsShape[]
--- @field Templates PhxPhysicsTemplate[]
--- @field Translate vec3


--- @class PhxPhysicsShape
--- @field Name FixedString
--- @field PhysicsObject PhxPhysicsObject
--- @field Rotation quat
--- @field Scale vec3
--- @field Translate vec3
--- @field field_34 int32


--- @class PhxPhysicsSkinnedCollisionObject:PhxPhysicsObject
--- @field HasCustomScene boolean
--- @field QueuedForDeletion boolean
--- @field SkinnedActors PhxSkinnedActorInfo[]
--- @field SoftbodyCollisionResources FixedString[]
--- @field TargetVisual Visual
--- @field field_81 boolean


--- @class PhxPhysicsSkinnedObject:PhxPhysicsSkinnedCollisionObject
--- @field AutoForward boolean
--- @field BaseCollisionVisual Visual
--- @field HasValidWeight boolean
--- @field IgnoreColliders FixedString[]
--- @field ImmediateSkinningForward boolean
--- @field InitialImpact vec3
--- @field LastVisibleTime number
--- @field MeshBindings MeshBinding[]
--- @field NeedsMeshBindingUpdate boolean
--- @field NeedsTranslateUpdate boolean
--- @field SettleTimer number
--- @field Skeleton Skeleton
--- @field UnskinnableBones PhxUnskinnedBoneInfo[]
--- @field VisualRotate quat
--- @field VisualTranslate vec3
--- @field field_160 int32
--- @field field_164 number
--- @field field_170 FixedString[]
--- @field field_180 int64


--- @class PhxPhysicsTemplate
--- @field ID FixedString


--- @class PhxSkinnedActorInfo
--- @field Blend number
--- @field BoneIndex int16
--- @field Shape PhxPhysicsShape
--- @field field_10 int64


--- @class PhxUnskinnedBoneInfo
--- @field ChildBoneIndex int16
--- @field ParentBoneIndex int16
--- @field field_18 int32
--- @field field_1C vec4
--- @field field_4 number
--- @field field_8 number
--- @field field_C vec3


--- @class ProgressionAbilityImprovementsComponent:BaseComponent
--- @field Abilities AbilityId[]
--- @field AbilityImprovements int32[]


--- @class ProgressionContainerComponent:BaseComponent
--- @field Progressions EntityHandle[][]


--- @class ProgressionFeatComponent:BaseComponent
--- @field AbilityImprovements int32[]
--- @field AddPassives FixedString[]
--- @field AddSpells ProgressionSpellDefinition[]
--- @field ConditionId int32?
--- @field Expertise SkillId[]
--- @field Feat Guid
--- @field Proficiencies SkillId[]
--- @field field_10 int32


--- @class ProgressionLevelUpComponent:BaseComponent
--- @field LevelUps LevelUpData[]
--- @field field_18 LevelUpData[]


--- @class ProgressionMetaComponent:BaseComponent
--- @field ClassLevel int32
--- @field Level int32
--- @field MulticlassSpellSlotOverride table<Guid, Guid>
--- @field Owner EntityHandle
--- @field Progression Guid
--- @field Race Guid
--- @field Source Guid
--- @field SpellSourceType SpellSourceType
--- @field field_18 uint8
--- @field field_40 int32
--- @field field_44 SpellSourceType
--- @field field_48 int32
--- @field field_50 table<Guid, Guid>


--- @class ProgressionPassivesComponent:BaseComponent
--- @field AddPassives FixedString[]
--- @field RemovePassives FixedString[]


--- @class ProgressionReplicatedFeatComponent:BaseComponent
--- @field Feat Guid
--- @field field_10 uint8
--- @field field_14 int32


--- @class ProgressionSkillsComponent:BaseComponent
--- @field Expertise SkillId[]
--- @field Proficiencies SkillId[]


--- @class ProgressionSpellDefinition
--- @field CastingAbility AbilityId
--- @field Class Guid
--- @field CooldownType SpellCooldownType
--- @field PrepareType SpellPrepareType
--- @field SpellId FixedString
--- @field SpellList Guid


--- @class ProgressionSpellsComponent:BaseComponent
--- @field AddSpells ProgressionSpellDefinition[]
--- @field RemoveSpells ProgressionSpellDefinition[]


--- @class ProjectileProjectileResult:PathSettings
--- @field InterpolateValue number
--- @field TemplateId FixedString
--- @field field_50 FixedString
--- @field field_58 EntityHandle
--- @field field_60 vec3


--- @class ProjectileSourceInfoComponent:BaseComponent
--- @field Entity EntityHandle
--- @field Spell SpellId


--- @class RecruitRecruiterComponent:BaseComponent
--- @field Recruiters Set_EntityHandle


--- @class ResourceAbilityDefaultValues:ResourceDefaultValueGuidResource
--- @field Add AbilityId[]


--- @class ResourceAbilityDistributionPreset:ResourceGuidResource
--- @field Charisma int32
--- @field ClassUuid Guid
--- @field Constitution int32
--- @field Dexterity int32
--- @field Intelligence int32
--- @field Strength int32
--- @field Wisdom int32
--- @field field_28 int32


--- @class ResourceAbilityList:ResourceGuidResource
--- @field Abilities AbilityId[]
--- @field MergedInto Guid
--- @field Name FixedString


--- @class ResourceActionResource:ResourceGuidResource
--- @field Description TranslatedString
--- @field DiceType DiceSizeId
--- @field DisplayName TranslatedString
--- @field Error TranslatedString
--- @field IsHidden boolean
--- @field IsSpellResource boolean
--- @field MaxLevel uint32
--- @field MaxValue number
--- @field Name FixedString
--- @field PartyActionResource boolean
--- @field ReplenishType ResourceReplenishType
--- @field ShowOnActionResourcePanel boolean
--- @field UpdatesSpellPowerLevel boolean


--- @class ResourceActionResourceGroup:ResourceGuidResource
--- @field ActionResourceDefinitions Guid[]
--- @field Name string
--- @field field_38 TranslatedString
--- @field field_48 TranslatedString


--- @class ResourceAnimationBlueprintResource:ResourceTwoStepLoadableResource
--- @field Blueprints GnGenomeBlueprint[]
--- @field Params table<Guid, GnGenomeVariant>


--- @class ResourceAnimationDesc
--- @field ID FixedString
--- @field flags AnimationSetAnimationFlags


--- @class ResourceAnimationResource:ResourceTwoStepLoadableResource
--- @field AdditiveLoopingAnimationID FixedString
--- @field AnchorHand uint8
--- @field Duration number
--- @field Events ResourceAnimationResourceEvent[]
--- @field LeftTransitionAnimation FixedString
--- @field Looping boolean
--- @field Offset number
--- @field RightTransitionAnimation FixedString
--- @field SkeletonResource FixedString
--- @field SupportingLeg uint8
--- @field Template FixedString
--- @field TimeStep number


--- @class ResourceAnimationResourceEvent
--- @field ID FixedString
--- @field Length number
--- @field Properties TextKeyTypeProperties
--- @field Time number
--- @field Track uint8


--- @class ResourceAnimationSet
--- @field AnimationSubSets table<FixedString, ResourceAnimationSubSet>


--- @class ResourceAnimationSetPriority:ResourceGuidResource
--- @field AddidionalObjects Guid[]
--- @field Name string
--- @field Priority int32


--- @class ResourceAnimationSetResource:ResourceLoadableResource
--- @field AnimationBank ResourceAnimationSet


--- @class ResourceAnimationSubSet
--- @field Animation table<FixedString, ResourceAnimationDesc>
--- @field FallBackSubSet FixedString


--- @class ResourceApprovalRating:ResourceGuidResource
--- @field Reactions ResourceApprovalRatingReaction[]
--- @field ReactionsSpeakers ResourceApprovalRatingReaction[]
--- @field Scope uint8


--- @class ResourceApprovalRatingReaction
--- @field id Guid
--- @field value int32


--- @class ResourceAreaLevelOverride:ResourceGuidResource
--- @field EntityLevel int32
--- @field ParentUuid Guid
--- @field TreasureLevel int32


--- @class ResourceAtmosphereResource:ResourceLoadableResource
--- @field Atmosphere ResourceAtmosphereResourceAtmosphereData
--- @field Labels FixedString[]


--- @class ResourceAtmosphereResourceAtmosphereData
--- @field Amount number
--- @field Bloom boolean
--- @field CharacterLightSetup Guid
--- @field ClothMainWindFrequency number
--- @field ClothMainWindSpeed number
--- @field ClothMaxWindDirectionOffset number
--- @field ClothWindDirectionOffsetFrequency number
--- @field ClothWindSpeed number
--- @field ClothWindVariance number
--- @field Contrast number
--- @field DOF boolean
--- @field DOFAperature number
--- @field DOFFarOnly boolean
--- @field DOFFarSharpDistance number
--- @field DOFFocalDistance number
--- @field DOFNearOnly boolean
--- @field DOFNearSharpDistance number
--- @field DarkRange ResourceAtmosphereResourceAtmosphereDataRange
--- @field EnvironmentEffect FixedString
--- @field EnvironmentEffect1 FixedString
--- @field EnvironmentEffect2 FixedString
--- @field EnvironmentEffect3 FixedString
--- @field EnvironmentEffectEnabled boolean
--- @field EnvironmentEffectEnabled1 boolean
--- @field EnvironmentEffectEnabled2 boolean
--- @field EnvironmentEffectEnabled3 boolean
--- @field EnvironmentEffectEnabledForTimeline boolean
--- @field EnvironmentEffectEnabledForTimeline1 boolean
--- @field EnvironmentEffectEnabledForTimeline2 boolean
--- @field EnvironmentEffectEnabledForTimeline3 boolean
--- @field EnvironmentEffectGlobalEnabled boolean
--- @field EnvironmentEffectOffset number
--- @field EnvironmentEffectOffset1 number
--- @field EnvironmentEffectOffset2 number
--- @field EnvironmentEffectOffset3 number
--- @field ExposureSettings ResourceAtmosphereResourceAtmosphereDataExposureSetting[]
--- @field FarPlane number
--- @field GUID FixedString
--- @field GlobalRange ResourceAtmosphereResourceAtmosphereDataRange
--- @field GodRays boolean
--- @field GodRaysIntensity number
--- @field GodRaysPower number
--- @field GodRaysThreshold number
--- @field GradingLut FixedString
--- @field GradingLutEnabled boolean
--- @field HighlightRange ResourceAtmosphereResourceAtmosphereDataRange
--- @field Highlights number
--- @field Hue number
--- @field InheritanceFlags uint32
--- @field LensFlareChromaticDistortion number
--- @field LensFlareEnabled boolean
--- @field LensFlareGhostDispersal number
--- @field LensFlareHaloWidth number
--- @field LensFlareIntensity number
--- @field LensFlareTreshold number
--- @field LocalLightSourceColor vec3
--- @field LocalLightSourceEnabled boolean
--- @field LocalLightSourceIntensity number
--- @field LocalLightSourceOverrideSettings boolean
--- @field LutApplyMode int32
--- @field LutBlendFactors vec2
--- @field MidtonesRange ResourceAtmosphereResourceAtmosphereDataRange
--- @field NearPlane number
--- @field Pivot number
--- @field Saturation number
--- @field ShadowRange ResourceAtmosphereResourceAtmosphereDataRange
--- @field Shadows number
--- @field SpecularRange ResourceAtmosphereResourceAtmosphereDataRange
--- @field Threshold number
--- @field TimelineAutomaticLightingDefaultSetup Guid
--- @field TimelineAutomaticLightingDisableFlip boolean
--- @field ToneMapHighlightsFixAmount number
--- @field Vignette boolean
--- @field VignetteColor vec3
--- @field VignetteIntensity number
--- @field VignettePower number
--- @field WhiteBalanceTemperature number
--- @field WhiteBalanceTint number
--- @field WindDirection number
--- @field WindSpeed number
--- @field field_10 mat4
--- @field field_170 int32
--- @field field_184 int32
--- @field field_188 int64
--- @field field_190 int32
--- @field field_1a8 int64
--- @field field_1b0 int64
--- @field field_1b8 int32
--- @field field_224 int32
--- @field field_d4 int32
--- @field field_d8 int64[]


--- @class ResourceAtmosphereResourceAtmosphereDataExposureSetting
--- @field Exposure boolean
--- @field ExposureCompensation number
--- @field ExposureMax number
--- @field ExposureMaxPercentile number
--- @field ExposureMeteringMode int32
--- @field ExposureMin number
--- @field ExposureMinPercentile number
--- @field field_19 string
--- @field field_1a int16
--- @field field_1c int32
--- @field field_20 int32
--- @field field_24 int32
--- @field field_28 int32


--- @class ResourceAtmosphereResourceAtmosphereDataRange
--- @field ExposureShift number
--- @field Saturation number
--- @field XYOffset vec2


--- @class ResourceAvatarContainerTemplates:ResourceGuidResource
--- @field TemplateId Guid


--- @class ResourceBackground:ResourceGuidResource
--- @field Description TranslatedString
--- @field DisplayName TranslatedString
--- @field Hidden boolean
--- @field Passives string
--- @field Tags Guid[]
--- @field field_40 int64
--- @field field_48 int64


--- @class ResourceBackgroundGoal:ResourceGuidResource
--- @field BackgroundUuid Guid
--- @field Description TranslatedString
--- @field ExperienceReward Guid
--- @field InspirationPoints int32
--- @field RewardLevel int32
--- @field Title TranslatedString


--- @class ResourceBlendSpaceResource:ResourceTwoStepLoadableResource
--- @field Adjustments ResourceBlendSpaceResourceAdjustment[]
--- @field Inputs ResourceBlendSpaceResourceInputInfo[]
--- @field MaxXValue number
--- @field MaxYValue number
--- @field MinXValue number
--- @field MinYValue number
--- @field NeedsAdjustmentCalculation boolean
--- @field XAxisName string
--- @field XElements int32
--- @field YAxisName string
--- @field YElements int32
--- @field field_b8 ResourceBlendSpaceResourceFieldB8Entry[]


--- @class ResourceBlendSpaceResourceAdjustment
--- @field Adjustment vec3
--- @field index uint16
--- @field indexes uint16[]


--- @class ResourceBlendSpaceResourceFieldB8Entry
--- @field field_0 vec2
--- @field field_10 vec2
--- @field field_18 vec2
--- @field field_20 int32
--- @field field_24 int32
--- @field field_28 int32
--- @field field_2c vec2
--- @field field_34 int32
--- @field field_38 int32
--- @field field_3c int32
--- @field field_40 vec2
--- @field field_48 int32
--- @field field_4c int32
--- @field field_50 int32
--- @field field_54 vec2
--- @field field_5c int32
--- @field field_8 vec2


--- @class ResourceBlendSpaceResourceInput
--- @field AnimationID FixedString
--- @field Position vec2
--- @field ShortNameID FixedString


--- @class ResourceBlendSpaceResourceInputInfo
--- @field AnimationID FixedString
--- @field Position vec2
--- @field ShortNameID FixedString


--- @class ResourceCalendarDayRange:ResourceGuidResource
--- @field DisplayCommonName TranslatedString
--- @field DisplayName TranslatedString
--- @field End int32
--- @field LeapYearEnd int32
--- @field LeapYearStart int32
--- @field Name FixedString
--- @field Start int32


--- @class ResourceCampChestTemplate:ResourceGuidResource
--- @field TemplateId Guid


--- @class ResourceCharacterCreationAccessorySet:ResourceGuidResource
--- @field CharacterCreationSet boolean
--- @field DefaultForRootTemplates Guid[]
--- @field DisplayName TranslatedString
--- @field RaceUUID Guid
--- @field SlotName FixedString
--- @field VisualUUID Guid[]


--- @class ResourceCharacterCreationAppearanceMaterial:ResourceGuidResource
--- @field DisplayName TranslatedString
--- @field DragonbornFemaleRootTemplate Guid
--- @field DragonbornMaleRootTemplate Guid
--- @field FemaleCameraName FixedString
--- @field FemaleRootTemplate Guid
--- @field MaleCameraName FixedString
--- @field MaleRootTemplate Guid
--- @field MaterialPresetUUID Guid
--- @field MaterialType FixedString
--- @field MaterialType2 FixedString
--- @field Name FixedString
--- @field UIColor vec4


--- @class ResourceCharacterCreationAppearanceVisual:ResourceGuidResource
--- @field BodyShape uint8
--- @field BodyType uint8
--- @field DefaultForBodyType uint8
--- @field DefaultSkinColor Guid
--- @field DisplayName TranslatedString
--- @field HeadAppearanceUUID Guid
--- @field IconIdOverride FixedString
--- @field RaceUUID Guid
--- @field RootTemplate Guid
--- @field SlotName FixedString
--- @field Tags Guid[]
--- @field TextureEntryPart FixedString
--- @field VisualResource Guid
--- @field field_3C uint32


--- @class ResourceCharacterCreationColor:ResourceGuidResource
--- @field DisplayName TranslatedString
--- @field MaterialPresetUUID Guid
--- @field Name FixedString
--- @field SkinType FixedString
--- @field UIColor vec4


--- @class ResourceCharacterCreationEquipmentIcons:ResourceGuidResource
--- @field AnimationUUID Guid
--- @field EquipmentTemplate Guid
--- @field IconGenerationTrigger FixedString
--- @field MeshIsTwoSided boolean
--- @field RootTemplate Guid
--- @field SlotName FixedString


--- @class ResourceCharacterCreationEyeColor:ResourceCharacterCreationColor


--- @class ResourceCharacterCreationHairColor:ResourceCharacterCreationColor


--- @class ResourceCharacterCreationIconSettings:ResourceGuidResource
--- @field BodyShape uint8
--- @field HeadAppearanceUUID Guid
--- @field RootTemplate Guid


--- @class ResourceCharacterCreationMaterialOverride:ResourceGuidResource
--- @field ActiveMaterialPresetUUID Guid
--- @field InactiveMaterialPresetUUID Guid
--- @field MaterialType int32
--- @field SourceMaterialUUID FixedString
--- @field TargetMaterialUUID FixedString


--- @class ResourceCharacterCreationPassiveAppearance:ResourceGuidResource
--- @field AccessorySetUUIDs Guid[]
--- @field AppearanceMaterialUUIDs Guid[]
--- @field ColorMaterialUUIDs Guid[]
--- @field Passive FixedString
--- @field RaceUUID Guid


--- @class ResourceCharacterCreationPreset:ResourceGuidResource
--- @field BodyShape uint8
--- @field BodyType uint8
--- @field CloseUpA string
--- @field CloseUpB string
--- @field Overview string
--- @field RaceUUID Guid
--- @field RootTemplate Guid
--- @field SubRaceUUID Guid
--- @field VOLinesTableUUID Guid


--- @class ResourceCharacterCreationSharedVisual:ResourceGuidResource
--- @field BoneName FixedString
--- @field DisplayName TranslatedString
--- @field SlotName FixedString
--- @field Tags Guid[]
--- @field VisualResource Guid


--- @class ResourceCharacterCreationSkinColor:ResourceCharacterCreationColor


--- @class ResourceCharacterCreationVOLine:ResourceGuidResource
--- @field TableUuid Guid
--- @field VoiceLine FixedString


--- @class ResourceCharacterVisualResource:ResourceLoadableResource
--- @field BaseVisual FixedString
--- @field VisualSet ResourceVisualSet


--- @class ResourceCinematicArenaFrequencyGroup:ResourceGuidResource
--- @field MaxFrequency number
--- @field Name FixedString
--- @field PercentageChance number


--- @class ResourceClassDescription:ResourceGuidResource
--- @field AnimationSetPriority int32
--- @field BaseHp int32
--- @field CanLearnSpells boolean
--- @field CharacterCreationPose FixedString
--- @field ClassEquipment FixedString
--- @field ClassHotbarColumns int32
--- @field CommonHotbarColumns int32
--- @field Description TranslatedString
--- @field DisplayName TranslatedString
--- @field HasGod boolean
--- @field HpPerLevel int32
--- @field IsDefaultForUseSpellAction boolean
--- @field ItemsHotbarColumns int32
--- @field LearningStrategy SpellLearningStrategy
--- @field MulticlassSpellcasterModifier number
--- @field MustPrepareSpells boolean
--- @field Name FixedString
--- @field ParentGuid Guid
--- @field PrimaryAbility AbilityId
--- @field ProgressionTableUUID Guid
--- @field SomaticEquipmentSet Set_FixedString
--- @field SoundClassType FixedString
--- @field SpellCastingAbility AbilityId
--- @field SpellList Guid
--- @field SubclassTitle TranslatedString
--- @field Tags Guid[]
--- @field Unused TranslatedString
--- @field field_71 uint8


--- @class ResourceClothColliderResource:ResourceLoadableResource
--- @field Spheres ResourceClothColliderResourceSphere[]


--- @class ResourceClothColliderResourceSphere
--- @field AttachedName FixedString
--- @field Links FixedString[]
--- @field Name FixedString
--- @field Position vec3
--- @field Radius number


--- @class ResourceColor:ResourceGuidResource
--- @field Color vec4
--- @field DisplayName TranslatedString
--- @field Name FixedString


--- @class ResourceColorListResource:ResourceLoadableResource
--- @field Colors uint32[]
--- @field IdGenerator int32
--- @field Ids int32[]


--- @class ResourceCompanionPreset:ResourceGuidResource
--- @field BodyShape uint8
--- @field BodyType uint8
--- @field CloseUpA string
--- @field CloseUpB string
--- @field Overview string
--- @field RaceUuid Guid
--- @field RootTemplate Guid
--- @field SubRaceUuid Guid
--- @field VOLinesTableUuid Guid
--- @field VoiceTableUuid Guid


--- @class ResourceConditionErrorDescription:ResourceGuidResource
--- @field DisplayName TranslatedString
--- @field ErrorDescriptionType uint8
--- @field Hidden boolean
--- @field Identifier FixedString


--- @class ResourceCustomDiceTemplate:ResourceGuidResource
--- @field Description TranslatedString
--- @field DisplayNamme TranslatedString
--- @field Flip1 boolean
--- @field Flip20 boolean
--- @field FontColour string
--- @field Icon boolean
--- @field Icon20 boolean
--- @field IsDefault boolean
--- @field Name string


--- @class ResourceDLC:ResourceGuidResource
--- @field CustomDice Guid
--- @field GalaxyAPICode uint32
--- @field Name string
--- @field PS5APICode uint32
--- @field SteamAPICode uint32
--- @field TwitchAPICode uint32
--- @field UnlockType uint8
--- @field XLiveAPICode uint32


--- @class ResourceDeathTypeEffect:ResourceGuidResource
--- @field Effects table<uint8, FixedString>
--- @field Fallback FixedString
--- @field Name string
--- @field ParentUuid Guid


--- @class ResourceDefaultValueGuidResource:ResourceGuidResource
--- @field ClassUuid Guid
--- @field Level int32
--- @field OriginUuid Guid
--- @field RaceUuid Guid
--- @field SelectorId string
--- @field SubclassUuid Guid
--- @field TableUUID Guid


--- @class ResourceDialogResource:ResourceLoadableResource
--- @field ChildResources Guid[]
--- @field Flags uint8
--- @field Flags2 uint8
--- @field Name string
--- @field SpeakerSlotsWithLines uint8[]
--- @field TimelineSplitScreenPriority uint8


--- @class ResourceDifficultyClass:ResourceGuidResource
--- @field Difficulties int32[]
--- @field Name string
--- @field field_40 uint8


--- @class ResourceDiffusionProfileResource:ResourceLoadableResource
--- @field DualSpecularMix number
--- @field DualSpecularRoughnessA number
--- @field DualSpecularRoughnessB number
--- @field ScatterColor vec3
--- @field ScatterDistance number
--- @field ThickObjectTransmission uint8
--- @field ThicknessRemapMax number
--- @field ThicknessRemapMin number
--- @field TransmissionTint vec3


--- @class ResourceDisturbanceProperty:ResourceGuidResource
--- @field Name FixedString


--- @class ResourceEffectInfo
--- @field BindSourceTo uint32
--- @field BindTargetTo uint32
--- @field DamageType DamageType
--- @field EffectResourceGuid Guid
--- @field EndTextKey string
--- @field Flags MultiEffectFlags
--- @field MaxDistance number
--- @field MinDistance number
--- @field Pivot boolean
--- @field ResourceId string
--- @field SourceBone string[]
--- @field SourceCheckEquipmentTagConditions boolean
--- @field SourceIgnoreTag Guid[]
--- @field SourceSkeletonSlot string[]
--- @field SourceSurface Guid[]
--- @field SourceTag Guid[]
--- @field StartTextKey string
--- @field TargetBone string[]
--- @field TargetCheckEquipmentTagConditions boolean
--- @field TargetIgnoreTag Guid[]
--- @field TargetSkeletonSlot string[]
--- @field TargetSurface Guid[]
--- @field TargetTag Guid[]
--- @field VerbalIntent uint8


--- @class ResourceEffectResource:ResourceTwoStepLoadableResource
--- @field BoundsMax vec3
--- @field BoundsMin vec3
--- @field Constructor ResourceTimelineConstructor
--- @field CullingDistance number
--- @field Dependencies FixedString[]
--- @field Duration number
--- @field EffectComponents AspkComponent[]
--- @field EffectName FixedString
--- @field InterruptionMode uint32
--- @field Looping boolean
--- @field UseSoundOcclusion boolean


--- @class ResourceEncumbranceType:ResourceGuidResource
--- @field Status FixedString


--- @class ResourceEquipmentDefaultValues:ResourceDefaultValueGuidResource
--- @field Add FixedString[]


--- @class ResourceEquipmentList:ResourceGuidResource
--- @field Items FixedString[]
--- @field MergedInto Guid
--- @field Name FixedString


--- @class ResourceEquipmentType:ResourceGuidResource
--- @field BoneMainSheathed FixedString
--- @field BoneMainUnsheathed FixedString
--- @field BoneOffHandSheathed FixedString
--- @field BoneOffHandUnsheathed FixedString
--- @field BoneVersatileSheathed FixedString
--- @field BoneVersatileUnsheathed FixedString
--- @field Name FixedString
--- @field SoundAttackType FixedString
--- @field SoundEquipmentType FixedString
--- @field SourceBoneAlternativeUnsheathed FixedString
--- @field SourceBoneSheathed FixedString
--- @field SourceBoneVersatileSheathed FixedString
--- @field SourceBoneVersatileUnsheathed FixedString
--- @field WeaponType_OneHanded FixedString
--- @field WeaponType_TwoHanded FixedString


--- @class ResourceExperienceRewards:ResourceGuidResource
--- @field LevelSource uint8
--- @field Name FixedString
--- @field PerLevelRewards int32[]
--- @field RewardType uint8


--- @class ResourceFCurveResource:ResourceTwoStepLoadableResource
--- @field CurveKeys ResourceFCurveResourceCurveKey[]
--- @field CurveLimits ResourceFCurveResourceCurveLimit
--- @field ProcessedCurveKeys ResourceFCurveResourceProcessedCurveKey[]


--- @class ResourceFCurveResourceCurveKey
--- @field CurveKeyConstraintType int32
--- @field CurveKeyInterpolationType int32
--- @field CurveKeyLeftTangent number
--- @field CurveKeyPosition vec2
--- @field CurveKeyRightTangent number


--- @class ResourceFCurveResourceCurveLimit
--- @field MaxX number
--- @field MaxY number
--- @field MinX number
--- @field MinY number


--- @class ResourceFCurveResourceProcessedCurveKey
--- @field CurveKeyInterpolationType int32
--- @field CurveKeyPosition vec2
--- @field Tan_CurveKeyLeftTangent number
--- @field Tan_CurveKeyRightTangent number


--- @class ResourceFaction:ResourceGuidResource
--- @field Faction FixedString
--- @field ParentGuid Guid


--- @class ResourceFeat:ResourceGuidResource
--- @field AddSpells ResourceProgressionAddedSpell[]
--- @field BoostPrototypes Guid[]
--- @field Boosts string
--- @field CanBeTakenMultipleTimes boolean
--- @field FeatRequirements ResourceFeatRequirement[]
--- @field Name FixedString
--- @field PassivePrototypesAdded StatsPassivePrototype[]
--- @field PassivePrototypesRemoved StatsPassivePrototype[]
--- @field PassivesAdded string
--- @field PassivesRemoved string
--- @field Requirements string
--- @field SelectAbilities ResourceProgressionAbility[]
--- @field SelectAbilityBonus ResourceProgressionAbilityBonus[]
--- @field SelectEquipment ResourceProgressionEquipment[]
--- @field SelectPassives ResourceProgressionPassive[]
--- @field SelectSkills ResourceProgressionSkill[]
--- @field SelectSkillsExpertise ResourceProgressionSkillExpertise[]
--- @field SelectSpells ResourceProgressionSpell[]


--- @class ResourceFeatDefaultValues:ResourceDefaultValueGuidResource
--- @field FeatUuid Guid


--- @class ResourceFeatDescription:ResourceGuidResource
--- @field Description TranslatedString
--- @field DisplayName TranslatedString
--- @field ExactMatch FixedString
--- @field FeatId Guid
--- @field Hidden boolean
--- @field ParamMatch FixedString
--- @field PassivePrototype FixedString
--- @field SelectorId FixedString
--- @field Type FixedString


--- @class ResourceFeatRequirement
--- @field Ability AbilityId?
--- @field AbilityValue int32?
--- @field ProficiencyGroupFlags uint64?
--- @field Requirement string
--- @field Type uint8


--- @class ResourceFeatSoundState:ResourceGuidResource
--- @field DefaultSwitch FixedString
--- @field FeatName FixedString
--- @field FeatUuid Guid
--- @field OverrideSwitch FixedString
--- @field OverrideSwitchGroup FixedString


--- @class ResourceFixedHotBarSlot:ResourceGuidResource
--- @field HotBarController uint8
--- @field HotBarType uint8
--- @field SlotIndex int32
--- @field SpellId FixedString


--- @class ResourceFlag:ResourceGuidResource
--- @field Description string
--- @field Name FixedString
--- @field Usage uint8


--- @class ResourceFlagSoundState:ResourceGuidResource
--- @field DefaultSwitch FixedString
--- @field FlagName FixedString
--- @field FlagUuid Guid
--- @field OverrideSwitch FixedString
--- @field OverrideSwitchGroup FixedString


--- @class ResourceGod:ResourceGuidResource
--- @field Description TranslatedString
--- @field DisplayName TranslatedString
--- @field MergedInto Guid
--- @field Name FixedString
--- @field Tags Guid[]


--- @class ResourceGoldRewards:ResourceGuidResource
--- @field Name FixedString
--- @field ParentScale number
--- @field ParentUuid Guid
--- @field PerLevelRewards int32[]


--- @class ResourceGossip:ResourceGuidResource
--- @field ConditionFlags Guid[]
--- @field DialogUUID Guid
--- @field Name FixedString
--- @field Priority int32
--- @field ResultFlags Guid[]
--- @field Type FixedString


--- @class ResourceGuidResource
--- @field ResourceUUID Guid


--- @class ResourceIKRigResource:ResourceLoadableResource
--- @field BoneCategories table<Guid, FixedString>
--- @field BoneProperties table<FixedString, ResourceIKRigResourceBoneProperty>
--- @field IKTasks ResourceIKRigResourceIKTask[]
--- @field SolverRoot boolean


--- @class ResourceIKRigResourceBoneProperty
--- @field DoFIncludesX boolean
--- @field DoFIncludesY boolean
--- @field DoFIncludesZ boolean
--- @field Enabled boolean
--- @field LimitAxis uint8
--- @field LimitEnabled boolean
--- @field Max vec3
--- @field Min vec3
--- @field Weight vec3


--- @class ResourceIKRigResourceIKTask
--- @field Bone FixedString
--- @field IKBoneCachedName FixedString
--- @field IKBoneName FixedString
--- @field IKBoneType int32
--- @field IsParentTask boolean
--- @field IsPoleObject boolean
--- @field LookAt boolean
--- @field LookAtAxis vec3
--- @field LookAtLocalSpace boolean
--- @field PositionDepth int32
--- @field PositionDoFIncludesX boolean
--- @field PositionDoFIncludesY boolean
--- @field PositionDoFIncludesZ boolean
--- @field PositionEnabled boolean
--- @field PositionWeight number
--- @field RotationDepth int32
--- @field RotationDoFIncludesX boolean
--- @field RotationDoFIncludesY boolean
--- @field RotationDoFIncludesZ boolean
--- @field RotationEnabled boolean
--- @field RotationWeight number
--- @field field_0 FixedString
--- @field field_39 string


--- @class ResourceItemThrowParams:ResourceGuidResource
--- @field Conditions string
--- @field MaxDistForOneRotation int32
--- @field MaxDistForTwoRotations int32
--- @field MaxDistForZeroRotations int32
--- @field Priority int32
--- @field RotationAxis uint32
--- @field StartAngle vec3


--- @class ResourceItemWallTemplate:ResourceGuidResource
--- @field Animation FixedString
--- @field HighLeft string
--- @field HighMiddle string
--- @field HighRight string
--- @field LowLeft string
--- @field LowMiddle string
--- @field LowRight string
--- @field Name string
--- @field Padding number
--- @field RandomRotation boolean
--- @field TimeBetweenItems number


--- @class ResourceLevelMap:ResourceGuidResource
--- @field FallbackValue Variant<int32,ResourceLevelMapDiceRoll>
--- @field LevelMaps Variant<int32,ResourceLevelMapDiceRoll>[]
--- @field Name FixedString
--- @field PreferredClassUUID Guid


--- @class ResourceLevelMapDiceRoll
--- @field AmountOfDices uint8
--- @field DiceValue DiceSizeId


--- @class ResourceLightCookieResource:ResourceTwoStepLoadableResource
--- @field Data boolean[]


--- @class ResourceLightingResource:ResourceLoadableResource
--- @field Lighting Lighting


--- @class ResourceLimbsMapping:ResourceGuidResource
--- @field BackLeft uint8[]
--- @field BackRight uint8[]
--- @field FrontLeft uint8[]
--- @field FrontRight uint8[]
--- @field RootTemplate Guid


--- @class ResourceLoadableResource:ResourceResource
--- @field RefCount uint16
--- @field State uint8


--- @class ResourceLongRestCost:ResourceGuidResource
--- @field CampGrowthDifficulty FixedString
--- @field CampQuality uint32
--- @field RequiredSupplies int32


--- @class ResourceManagedStatusVFX:ResourceGuidResource
--- @field Group FixedString
--- @field MixedEffect FixedString
--- @field NegativeEffect FixedString
--- @field PositiveEffect FixedString


--- @class ResourceMaterialPresetResource:ResourceLoadableResource
--- @field Presets ResourcePresetData


--- @class ResourceMaterialResource:ResourceTwoStepLoadableResource
--- @field DiffusionProfileUUID FixedString
--- @field Instance Material
--- @field MaterialType uint8
--- @field RenderChannel uint8
--- @field ScalarParameters ResourceMaterialResourceScalarParameter[]
--- @field Texture2DParameters ResourceMaterialResourceTexture2DParameter[]
--- @field Vector2Parameters ResourceMaterialResourceVector2Parameter[]
--- @field Vector3Parameters ResourceMaterialResourceVector3Parameter[]
--- @field VectorParameters ResourceMaterialResourceVector4Parameter[]
--- @field VirtualTextureParameters ResourceMaterialResourceVirtualTextureParameter[]


--- @class ResourceMaterialResourceParameter
--- @field Enabled boolean
--- @field ParameterName FixedString


--- @class ResourceMaterialResourceScalarParameter:ResourceMaterialResourceParameter
--- @field BaseValue number
--- @field Value number


--- @class ResourceMaterialResourceTexture2DParameter:ResourceMaterialResourceParameter
--- @field ID FixedString


--- @class ResourceMaterialResourceVector2Parameter:ResourceMaterialResourceParameter
--- @field BaseValue vec2
--- @field Value vec2


--- @class ResourceMaterialResourceVector3Parameter:ResourceMaterialResourceParameter
--- @field BaseValue vec3
--- @field IsColor boolean
--- @field Value vec3


--- @class ResourceMaterialResourceVector4Parameter:ResourceMaterialResourceParameter
--- @field BaseValue vec4
--- @field IsColor boolean
--- @field Value vec4


--- @class ResourceMaterialResourceVirtualTextureParameter:ResourceMaterialResourceParameter
--- @field ID FixedString
--- @field Index int32


--- @class ResourceMaterialSetResource:ResourceTwoStepLoadableResource


--- @class ResourceMeshProxyResource:ResourceTwoStepLoadableResource
--- @field Template FixedString
--- @field field_48 int64


--- @class ResourceMultiEffectInfo:ResourceGuidResource
--- @field EffectInfo ResourceEffectInfo[]
--- @field Name string


--- @class ResourceOrigin:ResourceGuidResource
--- @field AppearanceTags Guid[]
--- @field AvailableInCharacterCreation uint8
--- @field BackgroundUUID Guid
--- @field BodyShape uint8
--- @field BodyType uint8
--- @field ClassEquipmentOverride FixedString
--- @field ClassUUID Guid
--- @field CloseUpA string
--- @field CloseUpB string
--- @field DefaultsTemplate Guid
--- @field Description TranslatedString
--- @field DisplayName TranslatedString
--- @field ExcludesOriginUUID Guid
--- @field Flags uint32
--- @field GlobalTemplate Guid
--- @field GodUUID Guid
--- @field Identity uint8
--- @field IntroDialogUUID Guid
--- @field IsHenchman boolean
--- @field LockBody boolean
--- @field LockClass boolean
--- @field LockRace boolean
--- @field Name FixedString
--- @field Overview string
--- @field Passives string
--- @field RaceUUID Guid
--- @field ReallyTags Guid[]
--- @field SubClassUUID Guid
--- @field SubRaceUUID Guid
--- @field VoiceTableUUID Guid


--- @class ResourceOriginIntroEntity:ResourceGuidResource
--- @field DummyUuid Guid
--- @field Name FixedString
--- @field PlayerIndex int32


--- @class ResourcePassiveDefaultValues:ResourceDefaultValueGuidResource
--- @field Add FixedString[]
--- @field Replace ResourcePassiveDefaultValuesReplacement[]


--- @class ResourcePassiveDefaultValuesReplacement
--- @field From FixedString
--- @field To FixedString


--- @class ResourcePassiveList:ResourceGuidResource
--- @field MergedInto Guid
--- @field Name FixedString
--- @field Passives FixedString[]


--- @class ResourcePassivesVFX:ResourceGuidResource
--- @field CastEffect FixedString
--- @field PassiveName FixedString
--- @field PrepareEffect FixedString


--- @class ResourcePhysicsResource:ResourceLoadableResource
--- @field IgnoreColliderList FixedString[]
--- @field ObjectTemplates ResourcePhysicsResourceObjectTemplate[]
--- @field Template FixedString
--- @field field_54 boolean


--- @class ResourcePhysicsResourceObjectTemplate
--- @field PhysicsObjects ResourcePhysicsResourceObjectTemplatePhysicsObject[]
--- @field UUID FixedString


--- @class ResourcePhysicsResourceObjectTemplatePhysicsBox:ResourcePhysicsResourceObjectTemplatePhysicsObject
--- @field Extends vec3
--- @field Kinematic boolean
--- @field Mass number
--- @field UUID FixedString


--- @class ResourcePhysicsResourceObjectTemplatePhysicsCapsule:ResourcePhysicsResourceObjectTemplatePhysicsObject
--- @field Height number
--- @field Kinematic boolean
--- @field Mass number
--- @field Radius number
--- @field UUID FixedString


--- @class ResourcePhysicsResourceObjectTemplatePhysicsObject


--- @class ResourcePreparedSpellDefaultValues:ResourceDefaultValueGuidResource
--- @field Prepare FixedString[]
--- @field Unprepare FixedString[]


--- @class ResourcePresetData
--- @field MaterialPresets table<FixedString, ResourcePresetDataMapped>
--- @field MaterialResource FixedString
--- @field ScalarParameters ResourcePresetDataScalarParameter[]
--- @field Texture2DParameters ResourcePresetDataTexture2DParameter[]
--- @field Vector2Parameters ResourcePresetDataVector2Parameter[]
--- @field Vector3Parameters ResourcePresetDataVector3Parameter[]
--- @field VectorParameters ResourcePresetDataVectorParameter[]
--- @field VirtualTextureParameters ResourcePresetDataVirtualTextureParameter[]


--- @class ResourcePresetDataMapped
--- @field ForcePresetValues boolean
--- @field GroupName FixedString
--- @field MaterialPresetResource FixedString


--- @class ResourcePresetDataScalarParameter
--- @field Color boolean
--- @field Custom boolean
--- @field Enabled boolean
--- @field Parameter FixedString
--- @field Value number


--- @class ResourcePresetDataTexture2DParameter
--- @field Color boolean
--- @field Custom boolean
--- @field Enabled boolean
--- @field Parameter FixedString
--- @field Value FixedString


--- @class ResourcePresetDataVector2Parameter
--- @field Color boolean
--- @field Custom boolean
--- @field Enabled boolean
--- @field Parameter FixedString
--- @field Value vec2


--- @class ResourcePresetDataVector3Parameter
--- @field Color boolean
--- @field Custom boolean
--- @field Enabled boolean
--- @field Parameter FixedString
--- @field Value vec3


--- @class ResourcePresetDataVectorParameter
--- @field Color boolean
--- @field Custom boolean
--- @field Enabled boolean
--- @field Parameter FixedString
--- @field Value vec4


--- @class ResourcePresetDataVirtualTextureParameter
--- @field Color boolean
--- @field Custom boolean
--- @field Enabled boolean
--- @field Parameter FixedString
--- @field Value FixedString


--- @class ResourceProgression:ResourceGuidResource
--- @field AddSpells ResourceProgressionAddedSpell[]
--- @field AllowImprovement boolean
--- @field BoostPrototypes Guid[]
--- @field Boosts string
--- @field IsMulticlass boolean
--- @field Level uint8
--- @field Name string
--- @field PassivePrototypesAdded StatsPassivePrototype[]
--- @field PassivePrototypesRemoved StatsPassivePrototype[]
--- @field PassivesAdded string
--- @field PassivesRemoved string
--- @field ProgressionType ProgressionType
--- @field SelectAbilities ResourceProgressionAbility[]
--- @field SelectAbilityBonus ResourceProgressionAbilityBonus[]
--- @field SelectEquipment ResourceProgressionEquipment[]
--- @field SelectPassives ResourceProgressionPassive[]
--- @field SelectSkills ResourceProgressionSkill[]
--- @field SelectSkillsExpertise ResourceProgressionSkillExpertise[]
--- @field SelectSpells ResourceProgressionSpell[]
--- @field SubClasses Guid[]
--- @field TableUUID Guid
--- @field field_D0 FixedString[]


--- @class ResourceProgressionAbility
--- @field Arg2 int32
--- @field Arg3 int32
--- @field Arg4 string
--- @field UUID Guid


--- @class ResourceProgressionAbilityBonus
--- @field Amount int32
--- @field Amounts int32[]
--- @field BonusType string
--- @field UUID Guid


--- @class ResourceProgressionAddedSpell
--- @field Ability AbilityId
--- @field ActionResource Guid
--- @field ClassUUID Guid
--- @field CooldownType SpellCooldownType
--- @field PrepareType SpellPrepareType
--- @field SelectorId string
--- @field SpellUUID Guid


--- @class ResourceProgressionEquipment
--- @field Amount int32
--- @field Arg3 string
--- @field UUID Guid


--- @class ResourceProgressionPassive
--- @field Amount int32
--- @field Amount2 int32
--- @field Arg3 string
--- @field UUID Guid


--- @class ResourceProgressionSkill
--- @field Amount int32
--- @field Arg3 string
--- @field UUID Guid


--- @class ResourceProgressionSkillExpertise
--- @field Amount int32
--- @field Arg3 boolean
--- @field Arg4 string
--- @field UUID Guid


--- @class ResourceProgressionSpell
--- @field ActionResource Guid
--- @field Amount int32
--- @field Arg3 int32
--- @field CastingAbility AbilityId
--- @field ClassUUID Guid
--- @field CooldownType SpellCooldownType
--- @field PrepareType SpellPrepareType
--- @field SelectorId string
--- @field SpellUUID Guid


--- @class ResourceProgressionDescription:ResourceGuidResource
--- @field Description TranslatedString
--- @field DisplayName TranslatedString
--- @field ExactMatch FixedString
--- @field Hidden boolean
--- @field ParamMatch FixedString
--- @field PassivePrototype FixedString
--- @field ProgressionId Guid
--- @field ProgressionTableId Guid
--- @field SelectorId FixedString
--- @field Type FixedString


--- @class ResourceProjectileDefault:ResourceGuidResource
--- @field ProjectileDefaultType uint8
--- @field ProjectileTemplateId Guid


--- @class ResourceRace:ResourceGuidResource
--- @field Description TranslatedString
--- @field DisplayName TranslatedString
--- @field DisplayTypeUUID Guid
--- @field ExcludedGods Guid[]
--- @field EyeColors Guid[]
--- @field Gods Guid[]
--- @field HairColors Guid[]
--- @field HairGrayingColors Guid[]
--- @field HairHighlightColors Guid[]
--- @field HornColors Guid[]
--- @field HornTipColors Guid[]
--- @field LipsMakeupColors Guid[]
--- @field MakeupColors Guid[]
--- @field MergedInto Guid
--- @field Name FixedString
--- @field ParentGuid Guid
--- @field ProgressionTableUUID Guid
--- @field RaceEquipment FixedString
--- @field RaceSoundSwitch FixedString
--- @field SkinColors Guid[]
--- @field Tags Guid[]
--- @field TattooColors Guid[]
--- @field Visuals Guid[]


--- @class ResourceRandomCastOutcome:ResourceGuidResource
--- @field ClassLevel int32
--- @field ClassUuid Guid
--- @field GroupName FixedString
--- @field Level int32
--- @field Spell FixedString


--- @class ResourceResource
--- @field Guid FixedString
--- @field IsModded boolean
--- @field SourceFile Path


--- @class ResourceRuleset:ResourceGuidResource
--- @field Description TranslatedString
--- @field DisplayName TranslatedString
--- @field Name string
--- @field Parent Guid[]
--- @field ShowInCustom boolean
--- @field Type uint8


--- @class ResourceRulesetModifier:ResourceGuidResource
--- @field Default string
--- @field Description TranslatedString
--- @field DisplayName TranslatedString
--- @field EditableDuringGame boolean
--- @field LoadModule string
--- @field Max number
--- @field Name string
--- @field RulesetModifierType uint8
--- @field ShowInCustom boolean
--- @field Step number
--- @field field_54 int32


--- @class ResourceRulesetModifierOption:ResourceGuidResource
--- @field Description TranslatedString
--- @field DisplayName TranslatedString
--- @field LoadModule string
--- @field Modifier Guid
--- @field Name string
--- @field ShowInCustom boolean
--- @field Value string


--- @class ResourceRulesetSelectionPreset:ResourceGuidResource
--- @field Asset string
--- @field Description TranslatedString
--- @field DisplayName TranslatedString
--- @field IsCustom boolean
--- @field Name string
--- @field Rulesets Guid[]


--- @class ResourceRulesetValue:ResourceGuidResource
--- @field Modifier Guid
--- @field RawValue string
--- @field Ruleset Guid
--- @field Value Variant<ResourceRulesetValueNoValue,float,int32,FixedString,bool>


--- @class ResourceRulesetValueNoValue


--- @class ResourceScriptMaterialParameterOverride:ResourceGuidResource
--- @field IsFixedString uint8
--- @field Limit Variant<float,vec3,vec4,FixedString>
--- @field ParameterName FixedString
--- @field ParameterType FixedString
--- @field ParameterValue Variant<float,vec3,vec4,FixedString>
--- @field SetMode uint8


--- @class ResourceScriptMaterialPresetOverride:ResourceGuidResource
--- @field ParameterUuids Guid[]


--- @class ResourceScriptResource:ResourceLoadableResource
--- @field Parameters table<FixedString, ResourceScriptResourceParameter>


--- @class ResourceScriptResourceParameter
--- @field Type int32
--- @field Value string


--- @class ResourceShapeshiftRule:ResourceGuidResource
--- @field AbilityChanges ResourceShapeshiftRuleTypedInt[]
--- @field ActionCapabilities uint16
--- @field ApplySpellsFromTemplate boolean
--- @field ApplyTagsFromTemplate boolean
--- @field ApplyVisual boolean
--- @field ApplyVisualInCC boolean
--- @field AwarenessCapabilities uint16
--- @field BaseACOverride boolean
--- @field BlockLevelUp boolean
--- @field BloodSurfaceTypeInheritanceType uint8
--- @field ChangeAi boolean
--- @field ChangeBodyType boolean
--- @field ChangeIcon boolean
--- @field ChangeRace boolean
--- @field ChangeScript string?
--- @field DeathActionsInheritanceType uint8
--- @field DefaultBoostsInheritanceType uint8
--- @field DifficultyStatusesInheritanceType uint8
--- @field DisableEquipmentSlots boolean
--- @field DummyClass Guid?
--- @field FlagsInheritanceType uint8
--- @field FootstepsType uint8
--- @field HitBloodTypeInheritanceType uint8
--- @field Hp ResourceShapeshiftRuleTypedInt
--- @field IgnoreCustomLooks boolean
--- @field InteractionsCapabilities uint16
--- @field KillEntityAtZeroHP boolean
--- @field MuteEquipmentSound boolean
--- @field OverrideSpeaker boolean
--- @field PassivesInheritanceType uint8
--- @field PathInfluence string
--- @field PersonalStatusImmunitiesInheritanceType uint8
--- @field ProficiencyGroupsInheritanceType uint8
--- @field RemoveOldTags boolean
--- @field RemovePrevSpells boolean
--- @field ResistancesInheritanceType uint8
--- @field RetainCanSeeThrough boolean
--- @field RetainCanShootThrough boolean
--- @field RetainCanWalkThrough boolean
--- @field RetainDisplayName boolean
--- @field RuleName string
--- @field Scale ResourceShapeshiftRuleTypedFloat
--- @field SpeakingCapabilities uint16
--- @field SpellCastingAbilityInheritanceType uint8
--- @field SpellsAdd FixedString[]
--- @field SpellsRemove FixedString[]
--- @field TagsAdd Guid[]
--- @field TagsRemove Guid[]
--- @field TemporaryHp ResourceShapeshiftRuleTypedInt
--- @field UnarmedAbilityFromTemplate boolean
--- @field UseShapeshiftIdentity boolean
--- @field UseTemplateEquipmentSet boolean
--- @field Weight ResourceShapeshiftRuleTypedFloat
--- @field WildShapeHotBar boolean


--- @class ResourceShapeshiftRuleTypedFloat
--- @field Type uint8
--- @field Value number


--- @class ResourceShapeshiftRuleTypedInt
--- @field Type uint8
--- @field Value int32


--- @class ResourceSkeletonMirrorTableResource:ResourceTwoStepLoadableResource
--- @field Entries ResourceSkeletonMirrorTableResourceEntry[]


--- @class ResourceSkeletonMirrorTableResourceEntry
--- @field LeftBone FixedString
--- @field RightBone FixedString


--- @class ResourceSkeletonResource:ResourceTwoStepLoadableResource
--- @field Bones table<FixedString, ResourceSkeletonResourceBone>
--- @field ClothColliderResourceID FixedString
--- @field DynamicPhysicsResourceID FixedString
--- @field IKRigResourceID FixedString
--- @field MirrorTableResourceID FixedString
--- @field RagdollResourceID FixedString
--- @field Sockets ResourceSkeletonResourceSocket[]
--- @field SoftbodyResourceID FixedString
--- @field SpringResourceID FixedString
--- @field Template FixedString


--- @class ResourceSkeletonResourceBone
--- @field BoneGroups ResourceSkeletonResourceBoneBoneGroup[]
--- @field Position vec3
--- @field Rotation vec4
--- @field ShouldLOD boolean
--- @field SoundObjectActivationRange number
--- @field SoundObjectIndex uint8


--- @class ResourceSkeletonResourceBoneBoneGroup
--- @field IKBoneCachedName FixedString
--- @field IKBoneName FixedString
--- @field IKBoneType int32


--- @class ResourceSkeletonResourceSocket
--- @field Bone FixedString
--- @field BoneConstraint FixedString
--- @field ConstraintsPos uint16
--- @field ConstraintsRot uint16
--- @field Name FixedString
--- @field Position vec3
--- @field Rotation vec4


--- @class ResourceSkillDefaultValues:ResourceDefaultValueGuidResource
--- @field Add SkillId[]


--- @class ResourceSkillList:ResourceGuidResource
--- @field MergedInto Guid
--- @field Name FixedString
--- @field Skills SkillId[]


--- @class ResourceSkinPresetResource:ResourceLoadableResource
--- @field Presets ResourcePresetData


--- @class ResourceSoundResource:ResourceResource
--- @field Duration number
--- @field Flags uint8
--- @field MaxDistance number
--- @field SoundCodec AudioCodec
--- @field SoundEvent FixedString
--- @field SoundEventID uint32


--- @class ResourceSoundSpellTrajectoryRule:ResourceGuidResource
--- @field CurveUuid FixedString
--- @field DistancePastListener number
--- @field RuleName FixedString
--- @field SoundEvent FixedString


--- @class ResourceSpellDefaultValues:ResourceDefaultValueGuidResource
--- @field Add FixedString[]
--- @field Replace ResourceSpellDefaultValuesReplacement[]


--- @class ResourceSpellDefaultValuesReplacement
--- @field From FixedString
--- @field To FixedString


--- @class ResourceSpellList:ResourceGuidResource
--- @field MergedInto Guid
--- @field Name FixedString
--- @field Spells Set_FixedString


--- @class ResourceSpellMetaCondition:ResourceGuidResource
--- @field AdditionalConditions string
--- @field ConditionType SpellMetaConditionType
--- @field Filter string
--- @field OverrideOriginalCondition boolean


--- @class ResourceStatusSoundState:ResourceGuidResource
--- @field BlockVocals boolean
--- @field CombatVocalOverrideSwitch FixedString
--- @field SoundStateName FixedString
--- @field SpellOverrideSwitchClearState FixedString
--- @field SpellOverrideSwitchGroup FixedString
--- @field SpellOverrideSwitchState FixedString


--- @class ResourceSurfaceCursorMessage:ResourceGuidResource
--- @field CursorMessage TranslatedString
--- @field Name FixedString
--- @field Priority int32


--- @class ResourceTadpolePower:ResourceGuidResource
--- @field Name FixedString
--- @field NeedsHalfIllithidToUnlock boolean
--- @field Prerequisites Guid[]


--- @class ResourceTag:ResourceGuidResource
--- @field Categories uint32
--- @field Description string
--- @field DisplayDescription TranslatedString
--- @field DisplayName TranslatedString
--- @field Icon FixedString
--- @field Name FixedString
--- @field Properties uint32


--- @class ResourceTagSoundState:ResourceGuidResource
--- @field DefaultSwitch FixedString
--- @field OverrideSwitch FixedString
--- @field OverrideSwitchGroup FixedString
--- @field TagName FixedString
--- @field TagUuid Guid


--- @class ResourceTerrainBrushResource:ResourceLoadableResource
--- @field AlphaThreshold_maybe number
--- @field BaseColorMap FixedString
--- @field DetailNormalMap FixedString
--- @field DetailNormalScale number
--- @field DetailNormalTiling number
--- @field GradientBottomHeightColor vec3
--- @field GradientHeightBottom number
--- @field GradientHeightMidPoint number
--- @field GradientHeightTop number
--- @field GradientMidHeightColor vec3
--- @field GradientRoughnessPower number
--- @field GradientRoughnessReach number
--- @field HeightContrast number
--- @field HeightOffset number
--- @field HeightScale number
--- @field IsBlackBrush boolean
--- @field MaterialType uint8
--- @field NormalMap FixedString
--- @field PhysicalMap FixedString
--- @field Tiling number
--- @field UseDetailNormalMap boolean
--- @field UseHeightGradient_maybe boolean


--- @class ResourceTextureResource:ResourceLoadableResource
--- @field Depth int32
--- @field Height int32
--- @field SRGB boolean
--- @field Streaming boolean
--- @field Template FixedString
--- @field Type TextureType
--- @field Width int32


--- @class ResourceTileSetResource:ResourceLoadableResource
--- @field TileSet ResourceTileSetResourceExtendedRefMap


--- @class ResourceTileSetResourceExtendedRefMap
--- @field IsRoof boolean
--- @field Tiles table<Guid, ResourceTileSetResourceTile>


--- @class ResourceTileSetResourceTile
--- @field InvisibleClimbingHelperPhysicsGUID FixedString
--- @field PhysicsGUID FixedString
--- @field VisualGUID FixedString


--- @class ResourceTimelineConstructor
--- @field EffectComponents AspkComponent[]
--- @field EffectName FixedString
--- @field Header AspkTimelineHeader
--- @field Inputs AspkInput[]
--- @field Loaded boolean


--- @class ResourceTimelineResource:ResourceLoadableResource
--- @field BodyPartMocap int32
--- @field DialogResourceId Guid
--- @field FadeOutDuration number
--- @field FadeOutOnEnd boolean


--- @class ResourceTimelineSceneResource:ResourceLoadableResource
--- @field AdditionalSpeakerCount int32
--- @field Labels FixedString[]
--- @field SceneType uint8


--- @class ResourceTooltipExtraText:ResourceGuidResource
--- @field Text TranslatedString


--- @class ResourceTooltipUpcastDescription:ResourceGuidResource
--- @field Name FixedString
--- @field Text TranslatedString


--- @class ResourceTrajectoryRule:ResourceGuidResource
--- @field HeightMax number
--- @field HeightMin number
--- @field LengthMax number
--- @field LengthMin number
--- @field Priority int32
--- @field Template FixedString


--- @class ResourceTutorialEntry:ResourceGuidResource
--- @field ControllerDescription TranslatedString
--- @field ControllerVideo string
--- @field DisplayTitle TranslatedString
--- @field Hidden boolean
--- @field InputType uint8
--- @field KeyboardDescription TranslatedString
--- @field KeyboardVideo string
--- @field Section uint8


--- @class ResourceTutorialEvent:ResourceGuidResource
--- @field ActionResource Guid
--- @field EventType uint8
--- @field HotbarSlotFlags uint32
--- @field Name string
--- @field UserAction uint8


--- @class ResourceTutorialModalEntry:ResourceGuidResource
--- @field ControllerDescription TranslatedString
--- @field DisplayTitle TranslatedString
--- @field ExtraData FixedString
--- @field KeyboardDescription TranslatedString
--- @field ModalType uint8
--- @field Section uint8
--- @field TutorialID int32
--- @field TutorialName FixedString
--- @field WaitForEndCC boolean
--- @field WaitForEndDialog boolean


--- @class ResourceTutorialUnifiedEntry:ResourceGuidResource
--- @field ControllerDescription TranslatedString
--- @field ControllerInputList TranslatedString
--- @field ControllerJournalDescription TranslatedString
--- @field DisplayTitle TranslatedString
--- @field Icon string
--- @field InputType uint8
--- @field KeyboardDescription TranslatedString
--- @field KeyboardInputList TranslatedString
--- @field KeyboardJournalDescription TranslatedString
--- @field LifeTime int32
--- @field ModalType uint8
--- @field PositionOffsetX int32
--- @field PositionOffsetY int32
--- @field Section uint8
--- @field ShowInSplitScreen boolean
--- @field TutorialName FixedString
--- @field WaitForEndCC boolean
--- @field WaitForEndDialog boolean
--- @field WaitForLayers boolean
--- @field field_98 uint32


--- @class ResourceTwoStepLoadableResource:ResourceResource
--- @field field_30 int64


--- @class ResourceVFX:ResourceGuidResource
--- @field VFXEffectName FixedString
--- @field VFXGameplayName FixedString


--- @class ResourceVirtualTextureResource:ResourceLoadableResource
--- @field GTexFileName FixedString
--- @field LoadedVirtualTextureLayerConfig uint32
--- @field Prefetch boolean
--- @field PrefetchMipLevel int8
--- @field ReferencedColorSpaces uint32
--- @field RootPath string
--- @field TileSetFileName FixedString
--- @field VirtualTextureLayerConfig uint32
--- @field field_52 uint8
--- @field field_80 uint8


--- @class ResourceVisualLocatorAttachment:ResourceGuidResource
--- @field DisplayName FixedString
--- @field LocatorName FixedString


--- @class ResourceVisualResource:ResourceTwoStepLoadableResource
--- @field AnimationSetOverrides ResourceVisualResourceAnimationSetOverride[]
--- @field AnimationWaterfall FixedString[]
--- @field AttachBone FixedString
--- @field Attachments ResourceVisualResourceAttachment[]
--- @field BlueprintInstanceResourceID FixedString
--- @field Bones table<FixedString, ResourceVisualResourceBone>
--- @field BoundsMax vec3
--- @field BoundsMin vec3
--- @field Cloth ResourceVisualResourceClothData
--- @field HairPresetResourceId FixedString
--- @field HairType uint8
--- @field MaterialType uint8
--- @field NeedsSkeletonRemap boolean
--- @field Objects ResourceVisualResourceObject[]
--- @field RemapperSlotId FixedString
--- @field ScalpMaterialId FixedString
--- @field SkeletonResource FixedString
--- @field SkeletonSlot FixedString
--- @field Slot FixedString
--- @field SoftbodyResourceID FixedString
--- @field SupportsVertexColorMask boolean
--- @field Tags FixedString[]
--- @field Template FixedString
--- @field VertexColorMaskSlots FixedString[]


--- @class ResourceVisualResourceAnimationSetOverride
--- @field PairElement1 Guid
--- @field PairElement2 FixedString
--- @field strAnimationSetOverrideType int32


--- @class ResourceVisualResourceAttachment
--- @field Name FixedString
--- @field UUID FixedString


--- @class ResourceVisualResourceBone
--- @field Position vec3
--- @field Rotation vec4


--- @class ResourceVisualResourceClothData
--- @field ClothColliderResourceID FixedString
--- @field Mappings table<FixedString, ResourceVisualResourceClothHighResInfo[]>
--- @field Params ResourceVisualResourceClothParam[]


--- @class ResourceVisualResourceClothHighResInfo
--- @field Name FixedString


--- @class ResourceVisualResourceClothParam
--- @field AngularDrag vec3
--- @field AngularInertia vec3
--- @field AnimDriveDampingForce number
--- @field AnimDriveSpringForce number
--- @field AnimInfluenceMultiplier number
--- @field AtmosphericWindEnabled boolean
--- @field BendingStiffness number
--- @field CentrifugalInertia vec3
--- @field ClothMainWindFrequency number
--- @field ClothMainWindSpeed number
--- @field ClothMainWindVariance number
--- @field ClothMaxWindDirectionOffset number
--- @field ClothWindDirectionOffsetFrequency number
--- @field ClothWindSpeed number
--- @field CollisionMassScale number
--- @field CompressionLimit number
--- @field Damping vec3
--- @field DragCoefficient number
--- @field EnableCCD boolean
--- @field EnableVirtualParticles boolean
--- @field FluidDensity number
--- @field Friction number
--- @field FrontalWindSpeed number
--- @field FrontalWindVariance number
--- @field LiftCoefficient number
--- @field LinearDrag vec3
--- @field LinearInertia vec3
--- @field LinearStiffness number
--- @field OverrideAtmosphericWind boolean
--- @field SelfCollisionDistance number
--- @field SelfCollisionStiffness number
--- @field SolverFrequency number
--- @field StiffnessFrequency number
--- @field StiffnessMultiplier number
--- @field StretchLimit number
--- @field TetherConstraintScale number
--- @field TetherConstraintStiffness number
--- @field UUID FixedString


--- @class ResourceVisualResourceObject
--- @field LOD uint8
--- @field MaterialID FixedString
--- @field ObjectID FixedString


--- @class ResourceVisualSet
--- @field BodySetVisual FixedString
--- @field ID FixedString
--- @field LocatorAttachments ResourceVisualSetLocatorAttachment[]
--- @field MaterialOverrides ResourcePresetData
--- @field Materials table<FixedString, ResourcePresetData>
--- @field RealMaterialOverrides table<FixedString, FixedString>
--- @field ShowEquipmentVisuals boolean
--- @field Slots ResourceVisualSetSlot[]
--- @field field_158 table<FixedString, FixedString>


--- @class ResourceVisualSetLocatorAttachment
--- @field LocatorId FixedString
--- @field VisualResource FixedString


--- @class ResourceVisualSetSlot
--- @field Bone FixedString
--- @field Slot FixedString
--- @field VisualResource FixedString


--- @class ResourceVisualSetResource:ResourceLoadableResource


--- @class ResourceVoice:ResourceGuidResource
--- @field BodyType uint8
--- @field DisplayName TranslatedString
--- @field SpeakerUuid Guid
--- @field TableUuid Guid
--- @field Tags Guid[]


--- @class ResourceVoiceBarkResource:ResourceLoadableResource


--- @class ResourceWeaponAnimationSet:ResourceGuidResource
--- @field AnimationSetUuid FixedString
--- @field MainHand FixedString
--- @field OffHand FixedString


--- @class ResourceWeightCategories:ResourceGuidResource
--- @field MaxWeight int32
--- @field ObjectSize uint8
--- @field Tags Guid[]


--- @class RfCamera
--- @field CullingPlanes vec4[]
--- @field InvProjectionMatrix mat4
--- @field InvViewMatrix mat4
--- @field ProjectionMatrix mat4
--- @field ViewMatrix mat4
--- @field field_0 number
--- @field field_4 vec3


--- @class RfCameraController
--- @field AspectRatio number
--- @field Camera RfCamera
--- @field FOV number
--- @field FarPlane number
--- @field FrustumPoints vec3[]
--- @field ID FixedString
--- @field IsOrthographic boolean
--- @field LookAt vec3
--- @field NearPlane number
--- @field OffCenter boolean
--- @field OrthoBottom number
--- @field OrthoLeft number
--- @field OrthoRight number
--- @field OrthoTop number
--- @field ProjectionDirty boolean
--- @field ViewDirty boolean
--- @field WorldTransform Transform
--- @field field_58 vec3


--- @class SightDataComponent:BaseComponent
--- @field DarkvisionRange number
--- @field FOV number
--- @field Sight number
--- @field VerticalFOV number
--- @field field_0 Guid
--- @field field_10 number
--- @field field_14 number
--- @field field_18 number
--- @field field_1C number
--- @field field_20 int32
--- @field field_24 int32


--- @class SightEntityViewshedComponent:BaseComponent
--- @field field_0 Set_Guid


--- @class SightIgnoreSurfacesComponent:BaseComponent
--- @field SurfaceTypes Set_SurfaceType


--- @class SightSightBaseComponent:BaseComponent
--- @field DarkvisionRange number
--- @field FOV number
--- @field Sight number
--- @field VerticalFOV number
--- @field field_0 number
--- @field field_4 number
--- @field field_8 number
--- @field field_C number


--- @class SpellAddedSpellsComponent:BaseComponent
--- @field Spells SpellSpellMeta[]


--- @class SpellAttackSpellOverrideComponent:BaseComponent
--- @field AttackSpells table<uint8, FixedString>


--- @class SpellBookComponent:BaseComponent
--- @field Spells SpellSpellData[]
--- @field field_0 EntityHandle


--- @class SpellBookPreparesComponent:BaseComponent
--- @field PreparedSpells SpellMetaId[]
--- @field field_30 table<Guid, int32>
--- @field field_88 table<Guid, int32>


--- @class SpellCCPrepareSpellComponent:BaseComponent
--- @field Spells SpellMetaId[]


--- @class SpellCastRequirements
--- @field CastContext uint8
--- @field Requirements CastRequirementFlags
--- @field field_0 uint8
--- @field field_4 CastRequirementFlags


--- @class SpellCooldownData
--- @field Cooldown number
--- @field CooldownType SpellCooldownType
--- @field SpellId SpellId
--- @field field_29 uint8
--- @field field_30 Guid


--- @class SpellLearnedSpellsComponent:BaseComponent
--- @field SpellSchools Set_SpellSchoolId
--- @field field_18 table<Guid, Set_FixedString>


--- @class SpellModification
--- @field Modification Variant<SpellModificationModifyAreaRadius,SpellModificationModifyMaximumTargets,SpellModificationModifyNumberOfTargets,SpellModificationModifySavingThrowDisadvantage,SpellModificationModifySpellFlags,SpellModificationModifySpellRoll,SpellModificationModifyStatusDuration,SpellModificationModifySummonDuration,SpellModificationModifySurfaceDuration,SpellModificationModifyTargetRadius,SpellModificationModifyUseCosts,SpellModificationModifyVisuals,SpellModificationModifyIconGlow,SpellModificationModifyTooltipDescription>
--- @field Source FixedString
--- @field Spells Set_SpellId
--- @field field_0 uint8


--- @class SpellModificationModifyAreaRadius
--- @field AreaRadius number
--- @field ValueType SpellModificationValueType


--- @class SpellModificationModifyIconGlow
--- @field IconGlow uint8


--- @class SpellModificationModifyMaximumTargets
--- @field MaximumTargets int32
--- @field ValueType SpellModificationValueType


--- @class SpellModificationModifyNumberOfTargets
--- @field NumberOfTargets int32
--- @field ValueType SpellModificationValueType


--- @class SpellModificationModifySavingThrowDisadvantage
--- @field SavingThrowDisadvantage uint8


--- @class SpellModificationModifySpellFlags
--- @field SpellFlags uint8
--- @field field_1 uint8


--- @class SpellModificationModifySpellRoll
--- @field Conditions string
--- @field Conditions2 string
--- @field field_30 int32


--- @class SpellModificationModifyStatusDuration
--- @field StatusDuration number
--- @field ValueType SpellModificationValueType


--- @class SpellModificationModifySummonDuration
--- @field SummonDuration number
--- @field ValueType SpellModificationValueType


--- @class SpellModificationModifySurfaceDuration
--- @field SurfaceDuration number
--- @field ValueType SpellModificationValueType


--- @class SpellModificationModifyTargetRadius
--- @field TargetRadius number
--- @field ValueType SpellModificationValueType


--- @class SpellModificationModifyTooltipDescription
--- @field TooltipDescription uint8


--- @class SpellModificationModifyUseCosts
--- @field Expression string
--- @field Level int32
--- @field Resource Guid
--- @field Resource2 Guid
--- @field Type uint8


--- @class SpellModificationModifyVisuals
--- @field Visuals uint8


--- @class SpellPlayerPrepareSpellComponent:BaseComponent
--- @field Spells SpellMetaId[]
--- @field field_30 uint8


--- @class SpellScriptedExplosionComponent:BaseComponent
--- @field Projectile FixedString


--- @class SpellSpellAiConditionsComponent:BaseComponent
--- @field Conditions table<FixedString, AiActionConditions>


--- @class SpellSpellBookCooldownsComponent:BaseComponent
--- @field Cooldowns SpellCooldownData[]


--- @class SpellSpellContainerComponent:BaseComponent
--- @field Spells SpellSpellMeta[]


--- @class SpellSpellData
--- @field CastRequirements SpellCastRequirements[]
--- @field Charged boolean
--- @field CooldownType SpellCooldownType
--- @field Id SpellId
--- @field NumCharges int32
--- @field PreferredCastingResource Guid
--- @field PrepareType SpellPrepareType
--- @field SpellCastingAbility AbilityId
--- @field SpellUUID Guid
--- @field UsedCharges int32
--- @field field_41 boolean
--- @field field_42 SpellPrepareType


--- @class SpellSpellMeta
--- @field BoostHandle EntityHandle
--- @field ContainerSpell FixedString
--- @field CooldownType SpellCooldownType
--- @field LearningStrategy SpellLearningStrategy
--- @field LinkedSpellContainer boolean
--- @field PreferredCastingResource Guid
--- @field PrepareType SpellPrepareType
--- @field SelectionType SpellLearningStrategy
--- @field SpellCastingAbility AbilityId
--- @field SpellId SpellMetaId
--- @field SpellUUID Guid
--- @field field_29 SpellPrepareType
--- @field field_48 boolean


--- @class SpellSpellModificationContainerComponent:BaseComponent
--- @field Modifications table<FixedString, SpellModification[]>


--- @class SpellCastAnimationInfoComponent:BaseComponent
--- @field DualWielding boolean
--- @field Event SpellAnimationCastEvent
--- @field HasWeapon boolean
--- @field LoopingCastAnimation boolean
--- @field ObjectSize uint8
--- @field Target EntityHandle
--- @field TargetHitPosition vec3?
--- @field TargetIsCaster boolean
--- @field TargetPosition vec3
--- @field field_0 SpellAnimationCastEvent
--- @field field_28 uint8
--- @field field_2A boolean
--- @field field_2B uint8
--- @field field_2C boolean
--- @field field_2D boolean
--- @field field_2E uint8


--- @class SpellCastAnimationRequestOneFrameComponent:BaseComponent
--- @field CacheFlag256 boolean
--- @field DualWielding boolean
--- @field Event SpellAnimationCastEvent
--- @field HasWeapon boolean
--- @field LoopingCastAnimation boolean
--- @field ObjectSize uint8
--- @field Target EntityHandle
--- @field TargetHitPosition vec3?
--- @field TargetIsCaster boolean
--- @field TargetPosition vec3
--- @field field_28 uint8


--- @class SpellCastBaseTarget
--- @field Position vec3?
--- @field Target EntityHandle
--- @field TargetOverride EntityHandle?
--- @field TargetProxy EntityHandle?


--- @class SpellCastCacheComponent:BaseComponent
--- @field Flags uint32
--- @field SpellCastingAbility AbilityId


--- @class SpellCastCanBeTargetedComponent:BaseComponent


--- @class SpellCastCastEventOneFrameComponent:BaseComponent
--- @field Hit HitDesc
--- @field Target EntityHandle
--- @field TargetProxy EntityHandle?


--- @class SpellCastCastHitEventOneFrameComponent:BaseComponent
--- @field CastEntity EntityHandle
--- @field TextKey FixedString


--- @class SpellCastCastTextKeyEventOneFrameComponent:BaseComponent
--- @field CastEntity EntityHandle
--- @field TextKey FixedString


--- @class SpellCastCounteredEventOneFrameComponent:BaseComponent


--- @class SpellCastDataCache
--- @field Entity EntityHandle
--- @field Spell SpellId
--- @field field_0 int32
--- @field field_4 uint8
--- @field field_48 uint8
--- @field field_49 uint8
--- @field field_5 uint8


--- @class SpellCastDataCacheSingletonComponent:BaseComponent
--- @field Spells table<Guid, SpellCastDataCache>


--- @class SpellCastDestroyEventOneFrameComponent:BaseComponent
--- @field SpellCastGuid Guid


--- @class SpellCastExecutionTimeComponent:BaseComponent
--- @field Time number?


--- @class SpellCastFinishedEventOneFrameComponent:BaseComponent
--- @field Result SpellCastFailReason
--- @field field_1 uint8


--- @class SpellCastInitialTarget:SpellCastBaseTarget
--- @field Target2 SpellCastBaseTarget?
--- @field TargetingType SpellType


--- @class SpellCastIntermediateTarget:SpellCastBaseTarget
--- @field InitialTarget SpellCastInitialTarget
--- @field Target2 SpellCastInitialTarget
--- @field field_A0 uint8


--- @class SpellCastInterruptResultsComponent:BaseComponent
--- @field HasReplacement boolean
--- @field Results Set_EntityHandle


--- @class SpellCastIsCastingComponent:BaseComponent
--- @field Cast EntityHandle


--- @class SpellCastJumpStartEventOneFrameComponent:BaseComponent


--- @class SpellCastLogicExecutionEndEventOneFrameComponent:BaseComponent


--- @class SpellCastLogicExecutionStartEventOneFrameComponent:BaseComponent


--- @class SpellCastMovementComponent:BaseComponent
--- @field Position vec3
--- @field field_C vec3?


--- @class SpellCastOutcomeComponent:BaseComponent
--- @field Result SpellCastFailReason


--- @class SpellCastPrepareEndEventOneFrameComponent:BaseComponent


--- @class SpellCastPrepareStartEventOneFrameComponent:BaseComponent


--- @class SpellCastPreviewEndEventOneFrameComponent:BaseComponent


--- @class SpellCastProjectileTargetData
--- @field Hit HitDesc
--- @field PathSettings PathSettings?
--- @field Result ProjectileProjectileResult?
--- @field Target SpellCastIntermediateTarget
--- @field TextKey FixedString
--- @field ThrownObject EntityHandle
--- @field field_0 vec3
--- @field field_BC int32


--- @class SpellCastRollsComponent:BaseComponent
--- @field Rolls SpellCastSpellRollData[]


--- @class SpellCastSpellRollAbortEventOneFrameComponent:BaseComponent
--- @field Hit HitDesc
--- @field Target EntityHandle
--- @field TargetProxy EntityHandle?


--- @class SpellCastSpellRollCastEventData
--- @field CastKey FixedString
--- @field Hit HitDesc
--- @field field_0 FixedString


--- @class SpellCastSpellRollData
--- @field Casts SpellCastSpellRollCastEventData[]
--- @field Hits SpellCastSpellRollCastEventData[]
--- @field NameToCastIndex table<FixedString, int32>
--- @field NextReaction int32
--- @field SpellConditionsType SpellMetaConditionType
--- @field Target EntityHandle
--- @field TargetInfo SpellCastSpellRollTargetInfo?
--- @field TargetProxy EntityHandle?


--- @class SpellCastSpellRollTargetInfo
--- @field Flags uint64
--- @field Position vec3


--- @class SpellCastStateComponent:BaseComponent
--- @field CastEndPosition vec3?
--- @field CastOptions SpellCastOptions
--- @field CastPosition vec3?
--- @field Caster EntityHandle
--- @field CasterMoveToPosition vec3?
--- @field CasterStartPosition vec3
--- @field Entity EntityHandle
--- @field NetGuid string
--- @field Random uint32
--- @field Source EntityHandle
--- @field SpellCastGuid Guid
--- @field SpellId SpellId
--- @field Targets SpellCastInitialTarget[]
--- @field field_38 SpellCastOptions
--- @field field_60 vec3?
--- @field field_80 EntityHandle
--- @field field_88 uint32
--- @field field_A0 string


--- @class SpellCastSyncTargetingComponent:BaseComponent
--- @field CanMoveToThrowTarget boolean
--- @field CasterMoveToPosition vec3?
--- @field CasterPosition vec3?
--- @field HoverPosition vec3?
--- @field Target SpellCastBaseTarget
--- @field Targets SpellCastInitialTarget[]
--- @field field_40 boolean
--- @field field_44 int32
--- @field field_48 vec3?
--- @field field_58 vec3?
--- @field field_88 vec3?


--- @class SpellCastTargetHitEventOneFrameComponent:BaseComponent
--- @field Attack AttackDesc
--- @field Hit HitDesc
--- @field SpellCast EntityHandle
--- @field Target EntityHandle
--- @field TargetPosition vec3
--- @field TargetProxy EntityHandle?


--- @class SpellCastTargetHitInterruptEventOneFrameComponent:BaseComponent
--- @field Attack AttackDesc
--- @field Hit HitDesc
--- @field Source EntityHandle
--- @field SourcePosition vec3?
--- @field Spell SpellId
--- @field Target EntityHandle
--- @field TargetPosition vec3?
--- @field field_1D0 EntityHandle
--- @field field_1D8 EntityHandle
--- @field field_1E0 vec3?
--- @field field_1F0 vec3?


--- @class SpellCastTargetReactionEventOneFrameComponent:BaseComponent
--- @field CastEntity EntityHandle
--- @field Hit HitDesc
--- @field Target EntityHandle
--- @field TargetProxy EntityHandle?


--- @class SpellCastTargetsChangedEventOneFrameComponent:BaseComponent
--- @field Targets SpellCastInitialTarget[]


--- @class SpellCastThrowPickupPositionChangedEventOneFrameComponent:BaseComponent


--- @class StatsActionResourceCost
--- @field Amount number
--- @field ResourceGroup Guid
--- @field Resources Guid[]
--- @field SubResourceId int32


--- @class StatsAdjustRollFunctor:StatsFunctor
--- @field DamageType DamageType
--- @field Expression StatsExpressionParam
--- @field Type StatsRollAdjustmentType


--- @class StatsApplyEquipmentStatusFunctor:StatsApplyStatusFunctor
--- @field EquipmentSlot StatsItemSlot


--- @class StatsApplyStatusFunctor:StatsFunctor
--- @field Conditions stats::ConditionId
--- @field HasParam6 boolean
--- @field KeepAlive boolean
--- @field RequiresConcentration boolean
--- @field StatusConditions string
--- @field StatusId FixedString
--- @field StatusSpecificParam1 FixedString
--- @field StatusSpecificParam2 int32
--- @field StatusSpecificParam3 int32


--- @class StatsAttackPositionContextData:StatsContextData
--- @field Attack AttackDesc
--- @field Caster EntityRef
--- @field Hit HitDesc
--- @field HitRadius number
--- @field IsFromItem boolean
--- @field Position vec3
--- @field SpellId SpellIdWithPrototype
--- @field SurfaceChangeHeight number


--- @class StatsAttackTargetContextData:StatsContextData
--- @field Attack AttackDesc
--- @field Caster EntityRef
--- @field CasterProxy EntityRef
--- @field Hit HitDesc
--- @field HitWith HitWith
--- @field IsFromItem boolean
--- @field Position vec3
--- @field SomeRadius number
--- @field SpellId SpellIdWithPrototype
--- @field StatusEvent uint32
--- @field StatusExitCause uint8
--- @field StatusId FixedString
--- @field Target EntityRef
--- @field TargetProxy EntityRef
--- @field field_26C FixedString
--- @field field_315 uint8
--- @field field_316 uint8


--- @class StatsBreakConcentrationFunctor:StatsFunctor


--- @class StatsCNamedElementManager_StatsFunctor


--- @class StatsCNamedElementManager_StatsModifier
--- @field Values StatsModifier[]


--- @class StatsCNamedElementManager_StatsModifierList
--- @field Values StatsModifierList[]


--- @class StatsCNamedElementManager_StatsRPGEnumeration
--- @field Values StatsRPGEnumeration[]


--- @class StatsCNamedElementManager_StatsTreasureTable
--- @field Values StatsTreasureTable[]


--- @class StatsCameraWaitFunctor:StatsFunctor
--- @field Arg1 number


--- @class StatsContextData
--- @field ConditionCategory uint8
--- @field EntityToThothContextIndex table<EntityHandle, int32>
--- @field HistoryEntity EntityHandle
--- @field Originator ActionOriginator
--- @field PropertyContext StatsPropertyContext
--- @field StatusSource EntityHandle
--- @field StoryActionId int32
--- @field Type FunctorContextType
--- @field field_98 int32


--- @class StatsCounterspellFunctor:StatsFunctor


--- @class StatsCreateConeSurfaceFunctor:StatsFunctor
--- @field Duration number
--- @field IsControlledByConcentration boolean
--- @field Radius number
--- @field SurfaceType FixedString
--- @field field_55 boolean


--- @class StatsCreateExplosionFunctor:StatsFunctor
--- @field SpellId FixedString


--- @class StatsCreateSurfaceFunctor:StatsFunctor
--- @field Arg4 number
--- @field Arg5 boolean
--- @field Duration number
--- @field ExtendExistingConcentration boolean
--- @field IsControlledByConcentration boolean
--- @field Radius number
--- @field SurfaceType FixedString


--- @class StatsCreateWallFunctor:StatsFunctor


--- @class StatsCreateZoneFunctor:StatsFunctor
--- @field Arg2 number
--- @field Arg4 FixedString
--- @field Arg5 boolean
--- @field Duration number
--- @field Shape StatsZoneShape


--- @class StatsCustomDescriptionFunctor:StatsFunctor
--- @field Description FixedString


--- @class StatsDealDamageFunctor:StatsFunctor
--- @field CoinMultiplier int32
--- @field ConsumeCoin boolean
--- @field Damage StatsExpressionParam
--- @field DamageType DamageType
--- @field IgnoreDamageBonus boolean
--- @field IgnoreEvents boolean
--- @field IgnoreImmunities boolean
--- @field Magical boolean
--- @field Nonlethal boolean
--- @field WeaponDamageType DealDamageWeaponDamageType
--- @field WeaponType DealDamageWeaponType


--- @class StatsDescriptionInfo
--- @field Description TranslatedString
--- @field DescriptionParams string
--- @field DisplayName TranslatedString
--- @field ExtraDescription TranslatedString
--- @field ExtraDescriptionParams string
--- @field Icon FixedString
--- @field LoreDescription TranslatedString
--- @field ShortDescription TranslatedString
--- @field ShortDescriptionParams string


--- @class StatsDisarmAndStealWeaponFunctor:StatsFunctor


--- @class StatsDisarmWeaponFunctor:StatsFunctor


--- @class StatsDoTeleportFunctor:StatsFunctor
--- @field Arg1 number


--- @class StatsDouseFunctor:StatsFunctor
--- @field Radius number
--- @field field_20 number
--- @field field_24 number


--- @class StatsDropFunctor:StatsFunctor
--- @field Arg1 FixedString
--- @field Effect FixedString


--- @class StatsEquipContextData:StatsContextData
--- @field Caster EntityRef
--- @field Target EntityRef
--- @field UseCasterStats boolean


--- @class StatsExecuteWeaponFunctorsFunctor:StatsFunctor
--- @field WeaponType ExecuteWeaponFunctorsType


--- @class StatsExportedConditionalRoll
--- @field Conditions stats::ConditionId
--- @field Type StatsRollType


--- @class StatsExtenderFunctor:StatsFunctor


--- @class StatsFireProjectileFunctor:StatsFunctor
--- @field Arg1 FixedString
--- @field ProjectileTemplate FixedString


--- @class StatsForceFunctor:StatsFunctor
--- @field Aggression ForceFunctorAggression
--- @field ControlArc boolean
--- @field Distance FixedString
--- @field DistanceExpression StatsExpressionParamEx
--- @field Origin ForceFunctorOrigin
--- @field PullToOrigin boolean


--- @class StatsFunctor
--- @field Flags StatsFunctorFlags
--- @field FunctorUuid Guid
--- @field ObserverType StatsObserverType
--- @field PropertyContext StatsPropertyContext
--- @field RollConditions StatsExportedConditionalRoll[]
--- @field StatsConditions stats::ConditionId
--- @field StoryActionId int32
--- @field TypeId StatsFunctorId
--- @field UniqueName FixedString


--- @class StatsFunctorGroup
--- @field Functors StatsFunctor[]
--- @field TextKey FixedString


--- @class StatsFunctors:StatsCNamedElementManager_StatsFunctor
--- @field FunctorList StatsFunctor[]
--- @field UniqueName FixedString


--- @class StatsGainTemporaryHitPointsFunctor:StatsFunctor
--- @field HitPointsExpression StatsExpressionParam


--- @class StatsInterruptContextData:StatsContextData
--- @field Attack AttackDesc
--- @field Changes InterruptAppliedChange
--- @field Hit HitDesc
--- @field Interrupt InterruptInterruptEvent
--- @field Observer EntityRef
--- @field ObserverProxy EntityRef
--- @field OnlyAllowRollAdjustments boolean
--- @field RollAdjustments InterruptRollAdjustments?
--- @field Source EntityRef
--- @field SourceProxy EntityRef
--- @field Target EntityRef
--- @field TargetProxy EntityRef
--- @field field_428 uint8


--- @class StatsInterruptPrototype
--- @field Conditions stats::ConditionId
--- @field Container FixedString
--- @field Description StatsDescriptionInfo
--- @field EnableCondition stats::ConditionId
--- @field EnableContext uint64
--- @field FailureFunctors StatsFunctors
--- @field InterruptContext uint16
--- @field InterruptContextScope uint8
--- @field InterruptDefaultValue uint8
--- @field InterruptFlags uint8
--- @field Name FixedString
--- @field PropertiesFunctors StatsFunctors
--- @field RollCondition stats::ConditionId
--- @field Stack FixedString
--- @field SuccessFunctors StatsFunctors


--- @class StatsKillFunctor:StatsFunctor


--- @class StatsMaximizeRollFunctor:StatsFunctor
--- @field DamageType DamageType


--- @class StatsModifier
--- @field EnumerationIndex int32
--- @field LevelMapIndex int32
--- @field Name FixedString


--- @class StatsModifierList
--- @field Attributes StatsCNamedElementManager_StatsModifier
--- @field Name FixedString


--- @class StatsModifySpellCameraFocusFunctor:StatsFunctor


--- @class StatsMoveContextData:StatsContextData
--- @field Caster EntityRef
--- @field Distance number
--- @field Position vec3
--- @field Source EntityRef
--- @field Target EntityRef


--- @class StatsNearbyAttackedContextData:StatsContextData
--- @field Attack AttackDesc
--- @field Hit HitDesc
--- @field IsFromItem boolean
--- @field OriginalSource EntityRef
--- @field Position vec3
--- @field Source EntityRef
--- @field SourceProxy EntityRef
--- @field SpellId SpellIdWithPrototype
--- @field SurfaceChangeHeight number
--- @field Target EntityRef
--- @field TargetProxy EntityRef
--- @field field_314 uint8


--- @class StatsNearbyAttackingContextData:StatsNearbyAttackedContextData


--- @class StatsObject
--- @field ComboCategories FixedString[]
--- @field ComboProperties FixedString[]
--- @field ModId FixedString
--- @field ModifierList FixedString
--- @field ModifierListIndex uint32
--- @field Name FixedString
--- @field OriginalModId FixedString
--- @field Using FixedString
--- @field CopyFrom fun(self:StatsObject, a1:StatsObject, a2:FixedString):boolean
--- @field SetPersistence fun(self:StatsObject, a1:StatsObject, a2:boolean)
--- @field SetRawAttribute fun(self:StatsObject, a1:StatsObject, a2:FixedString, a3:string):boolean
--- @field Sync fun(self:StatsObject, a1:StatsObject, a2:boolean?)


--- @class StatsPassivePrototype
--- @field BoostConditionsIndex stats::ConditionId
--- @field BoostContext uint64
--- @field ConditionsIndex stats::ConditionId
--- @field Description StatsDescriptionInfo
--- @field EnabledConditions stats::ConditionId
--- @field EnabledContext uint64
--- @field Name FixedString
--- @field PriorityOrder int32
--- @field Properties uint32
--- @field StatsFunctorContext uint64
--- @field StatsFunctors StatsFunctors
--- @field ToggleGroup FixedString
--- @field ToggleOffContext uint64
--- @field ToggleOffEffect FixedString
--- @field ToggleOffFunctors StatsFunctors
--- @field ToggleOnEffect FixedString
--- @field ToggleOnFunctors StatsFunctors
--- @field TooltipConditionalDamage FixedString


--- @class StatsPickupFunctor:StatsFunctor
--- @field Arg0 FixedString
--- @field Effect FixedString


--- @class StatsRPGEnumeration
--- @field Name FixedString
--- @field Values table<FixedString, int32>


--- @class StatsRPGStats
--- @field ExtraData table<FixedString, number>
--- @field ModifierLists StatsCNamedElementManager_StatsModifierList
--- @field ModifierValueLists StatsCNamedElementManager_StatsRPGEnumeration
--- @field TreasureTables StatsCNamedElementManager_StatsTreasureTable


--- @class StatsRegainHitPointsFunctor:StatsFunctor
--- @field HealingType StatsTargetTypeFlags
--- @field HitPoints StatsExpressionParam


--- @class StatsRegainTemporaryHitPointsFunctor:StatsFunctor
--- @field HitPoints StatsExpressionParam


--- @class StatsRemoveAuraByChildStatusFunctor:StatsFunctor
--- @field StatusId FixedString


--- @class StatsRemoveStatusByLevelFunctor:StatsFunctor
--- @field Ability AbilityId?
--- @field Arg2 int8
--- @field Level int8
--- @field NameOrStatusGroup Variant<FixedString,StatsStatusGroup>


--- @class StatsRemoveStatusFunctor:StatsFunctor
--- @field StatusId FixedString


--- @class StatsRemoveUniqueStatusFunctor:StatsFunctor
--- @field StatusId FixedString


--- @class StatsRequirement
--- @field IntParam int32
--- @field Not boolean
--- @field RequirementId RequirementType
--- @field TagParam Guid


--- @class StatsResetCombatTurnFunctor:StatsFunctor


--- @class StatsResetCooldownsFunctor:StatsFunctor
--- @field CooldownType SpellCooldownType


--- @class StatsRestoreResourceFunctor:StatsFunctor
--- @field ActionResource Guid
--- @field Amount number
--- @field AmountType StatsRestoreResourceAmountType
--- @field Hex int32
--- @field Level int32
--- @field LuaAmount StatsExpressionParam


--- @class StatsResurrectFunctor:StatsFunctor
--- @field HealingType StatsTargetTypeFlags
--- @field HealthPercentage number
--- @field Probability number


--- @class StatsSabotageFunctor:StatsFunctor
--- @field Amount int32


--- @class StatsSetAdvantageFunctor:StatsFunctor


--- @class StatsSetDamageResistanceFunctor:StatsFunctor
--- @field DamageType DamageType


--- @class StatsSetDisadvantageFunctor:StatsFunctor


--- @class StatsSetRerollFunctor:StatsFunctor
--- @field Arg2 boolean
--- @field KeepNew boolean
--- @field Roll uint8


--- @class StatsSetRollFunctor:StatsFunctor
--- @field DamageType DamageType
--- @field Roll int32
--- @field Type StatsRollAdjustmentType


--- @class StatsSetStatusDurationFunctor:StatsFunctor
--- @field ChangeType SetStatusDurationType
--- @field Duration number
--- @field StatusId FixedString


--- @class StatsShortRestFunctor:StatsFunctor


--- @class StatsSourceContextData:StatsContextData
--- @field Source EntityRef
--- @field SourceProxy EntityRef


--- @class StatsSpawnExtraProjectilesFunctor:StatsFunctor
--- @field Arg1 FixedString
--- @field ProjectileSpell FixedString


--- @class StatsSpawnFunctor:StatsFunctor
--- @field AIHelper FixedString
--- @field ApplyLateJoinPenalty boolean
--- @field Arg7 boolean
--- @field StatusesToApply Set_FixedString
--- @field TemplateId FixedString


--- @class StatsSpawnInInventoryFunctor:StatsFunctor
--- @field AdditionalArgs Set_FixedString
--- @field AiSpellOverride FixedString
--- @field AllowOffHand boolean
--- @field Amount int32
--- @field Arg3 int32
--- @field Arg4 boolean
--- @field Arg5 boolean
--- @field Arg6 boolean
--- @field Arg7 FixedString
--- @field Equip boolean
--- @field PlaceInInventory boolean
--- @field StatusesToApply Set_FixedString
--- @field TemplateId FixedString


--- @class StatsSpellPrototype
--- @field AiFlags uint8
--- @field AlternativeCastTextEvents FixedString[]
--- @field CastSound FixedString
--- @field CastSoundType uint8
--- @field ChildPrototypes StatsSpellPrototype[]
--- @field CinematicArenaFlags uint32
--- @field CinematicArenaTimelineOverride Guid
--- @field CombatAIOverrideSpell FixedString
--- @field CombatAIOverrideSpells FixedString[]
--- @field ContainerSpells FixedString[]
--- @field Cooldown int8
--- @field DamageType DamageType
--- @field Description StatsDescriptionInfo
--- @field DiceValue DiceSizeId
--- @field DualWieldingSpellAnimation StatsSpellPrototypeAnimationData
--- @field DualWieldingUseCosts StatsActionResourceCost[]
--- @field HasMemoryCost boolean
--- @field HasTrajectoryRules boolean
--- @field HitAnimationType uint8
--- @field HitCostGroups StatsTextKeyHitCosts[]
--- @field InterruptPrototype FixedString
--- @field ItemWall FixedString
--- @field Level int32
--- @field LineOfSightFlags uint32
--- @field ParentPrototype StatsSpellPrototype
--- @field PathfindTemplateId uint32
--- @field PowerLevel int32
--- @field PrepareEffect FixedString
--- @field PrepareLoopSound FixedString
--- @field PrepareSound FixedString
--- @field RechargeValuesFrom int32
--- @field RechargeValuesTo int32
--- @field RequirementEvents uint32
--- @field RitualCosts StatsActionResourceCost[]
--- @field Sheathing uint8
--- @field SourceLimbIndex int8
--- @field SpellActionType uint8
--- @field SpellAnimation StatsSpellPrototypeAnimationData
--- @field SpellAnimationIntentType uint8
--- @field SpellAnimationType uint8
--- @field SpellCategory uint32
--- @field SpellContainerId FixedString
--- @field SpellFlags SpellFlags
--- @field SpellHitAnimationType uint8
--- @field SpellId FixedString
--- @field SpellJumpType uint8
--- @field SpellSchool uint8
--- @field SpellTypeId SpellType
--- @field StatsObjectIndex int32
--- @field SteerSpeedMultipler number
--- @field Trajectories FixedString[][]
--- @field UseCosts StatsActionResourceCost[]
--- @field VariableDualWieldingUseCosts StatsVariableResourceCost[]
--- @field VariableRitualCosts StatsVariableResourceCost[]
--- @field VariableUseCosts StatsVariableResourceCost[]
--- @field VerbalIntent uint32
--- @field WeaponTypes uint32
--- @field field_101 uint8
--- @field field_299 uint8


--- @class StatsSpellPrototypeAnimationData
--- @field Flags uint8
--- @field Part0 FixedString[]
--- @field Part1 FixedString[]
--- @field Part2 FixedString[][]
--- @field Part3 FixedString[]
--- @field Part4 FixedString[]
--- @field Part5 FixedString[]
--- @field Part6 FixedString[]
--- @field Part7 FixedString[]
--- @field Part8 FixedString[]


--- @class StatsStabilizeFunctor:StatsFunctor


--- @class StatsStatsFunctorSetImpl:StatsFunctors


--- @class StatsStatsSoundInfo
--- @field Key FixedString
--- @field Type uint8


--- @class StatsStatusPrototype
--- @field AbsorbSurfaceTypes SurfaceType[]
--- @field AuraFlags uint8
--- @field Boosts Guid[]
--- @field Description StatsDescriptionInfo
--- @field Flags uint8
--- @field HitAnimationType uint8
--- @field ImmuneFlag uint32
--- @field LEDEffect uint8
--- @field RemoveEvents uint32
--- @field Sheathing uint8
--- @field SoundLoop StatsStatsSoundInfo[]
--- @field SoundStart StatsStatsSoundInfo[]
--- @field SoundStop StatsStatsSoundInfo[]
--- @field StackType uint32
--- @field StatsObjectIndex int32
--- @field StatusGroups uint64
--- @field StatusId StatusType
--- @field StatusName FixedString
--- @field StatusPropertyFlags uint64
--- @field TickType uint8


--- @class StatsSummonFunctor:StatsFunctor
--- @field AIHelper FixedString
--- @field AiSpellOverride FixedString
--- @field Arg5 boolean
--- @field Arg9 boolean
--- @field ExtendExistingConcentration boolean
--- @field LateJoinPenalty boolean
--- @field SpawnLifetime Variant<StatsSummonLifetimeType,float>
--- @field StackId FixedString
--- @field StatusesToApply Set_FixedString
--- @field Template FixedString
--- @field UseOwnerPassives boolean


--- @class StatsSummonInInventoryFunctor:StatsFunctor
--- @field AdditionalArgs Set_FixedString
--- @field AiSpellOverride FixedString
--- @field AllowOffHand boolean
--- @field Amount int32
--- @field Arg6 boolean
--- @field Arg7 boolean
--- @field Arg8 FixedString
--- @field Arg9 FixedString
--- @field Equip boolean
--- @field ExtendExistingConcentration boolean
--- @field Lifetime Variant<StatsSummonLifetimeType,float>
--- @field PlaceInInventory boolean
--- @field SpellContainer FixedString
--- @field StatusesToApply Set_FixedString
--- @field TemplateId FixedString


--- @class StatsSurfaceChangeFunctor:StatsFunctor
--- @field Chance number
--- @field DetourMaxWaitTime number
--- @field Radius number
--- @field SurfaceChange SurfaceChange
--- @field field_24 number
--- @field field_28 number
--- @field field_2C number


--- @class StatsSurfaceClearLayerFunctor:StatsFunctor
--- @field Layers Set_SurfaceLayer8


--- @class StatsSwapPlacesFunctor:StatsFunctor
--- @field Animation FixedString
--- @field Arg1 boolean
--- @field Arg2 boolean


--- @class StatsSwitchDeathTypeFunctor:StatsFunctor
--- @field DeathType StatsDeathType


--- @class StatsTargetContextData:StatsContextData
--- @field Attack AttackDesc
--- @field Hit HitDesc
--- @field Position vec3
--- @field Source EntityRef
--- @field SourceProxy EntityRef
--- @field SpellId SpellIdWithPrototype
--- @field StatusEvent uint32
--- @field StatusExitCause uint8
--- @field StatusId FixedString
--- @field field_C5 uint8
--- @field field_C6 uint8


--- @class StatsTeleportSourceFunctor:StatsFunctor
--- @field Arg1 boolean
--- @field Arg2 boolean
--- @field FindValidPosition boolean
--- @field RotateTowardsTarget boolean


--- @class StatsTextKeyHitCosts
--- @field Costs StatsActionResourceCost[]
--- @field TextKey FixedString


--- @class StatsTreasureCategory
--- @field Category FixedString
--- @field Items StatsTreasureCategoryItem[]


--- @class StatsTreasureCategoryItem
--- @field ActPart int32
--- @field MaxAmount int32
--- @field MaxLevel int32
--- @field MinAmount int32
--- @field MinLevel int32
--- @field Name FixedString
--- @field Priority int32
--- @field Unique int32


--- @class StatsTreasureSubTable
--- @field Amounts int32[]
--- @field Categories StatsTreasureSubTableCategory[]
--- @field CategoryFrequencies int32[]
--- @field DropCounts StatsTreasureSubTableDropCount[]
--- @field EndLevel int32
--- @field StartLevel int32
--- @field TotalCount int32
--- @field TotalFrequency int32


--- @class StatsTreasureSubTableCategory
--- @field Frequencies uint16[]
--- @field Frequencies2 uint16[]
--- @field Frequency int32
--- @field Index int32
--- @field IsTreasureTable boolean
--- @field IsTreasureTable2 boolean


--- @class StatsTreasureSubTableDropCount
--- @field Amount int32
--- @field Chance int32


--- @class StatsTreasureTable
--- @field CanMerge boolean
--- @field IgnoreLevelDiff boolean
--- @field MaxLevel int32
--- @field MinLevel int32
--- @field Name FixedString
--- @field SubTables StatsTreasureSubTable[]
--- @field UseTreasureGroupContainers boolean


--- @class StatsTriggerRandomCastFunctor:StatsFunctor
--- @field Arg1 uint8
--- @field Arg2 number
--- @field DC uint8
--- @field Delay number
--- @field RandomCastOutcomes FixedString[]


--- @class StatsTutorialEventFunctor:StatsFunctor
--- @field Event Guid


--- @class StatsUnlockFunctor:StatsFunctor


--- @class StatsUnsummonFunctor:StatsFunctor


--- @class StatsUseActionResourceFunctor:StatsFunctor
--- @field ActionResource Guid
--- @field Amount number
--- @field IgnoreResourceConsumeBoosts boolean
--- @field IsPercentage boolean
--- @field Level int32


--- @class StatsUseAttackFunctor:StatsFunctor
--- @field IgnoreBlindness boolean
--- @field IgnoreChecks boolean


--- @class StatsUseSpellFunctor:StatsFunctor
--- @field Arg4 boolean
--- @field Arg6 boolean
--- @field IgnoreBlindness boolean
--- @field IgnoreChecks boolean
--- @field IgnoreHasSpell boolean
--- @field SpellCastGuid Guid
--- @field SpellId FixedString


--- @class StatsVariableResourceCost
--- @field Amount number
--- @field Resource Guid


--- @class StatusCauseComponent:BaseComponent
--- @field Cause EntityHandle


--- @class StatusContainerComponent:BaseComponent
--- @field Statuses table<EntityHandle, FixedString>


--- @class StatusIDComponent:BaseComponent
--- @field ID FixedString


--- @class StatusIncapacitatedComponent:BaseComponent
--- @field field_0 uint32
--- @field field_48 uint8
--- @field field_8 table<int32, int32[]>


--- @class StatusIndicateDarknessComponent:BaseComponent


--- @class StatusLifetimeComponent:BaseComponent
--- @field Lifetime number
--- @field field_0 int32
--- @field field_4 number


--- @class StatusLoseControlComponent:BaseComponent
--- @field LoseControl boolean


--- @class StatusVisualDisabledComponent:BaseComponent
--- @field Visuals Set_FixedString


--- @class TadpoleTreeFullIllithidComponent:BaseComponent


--- @class TadpoleTreeHalfIllithidComponent:BaseComponent


--- @class TadpoleTreePowerContainerComponent:BaseComponent
--- @field Powers FixedString[]


--- @class TadpoleTreeTadpoledComponent:BaseComponent


--- @class TadpoleTreeTreeStateComponent:BaseComponent
--- @field State uint8


--- @class TranslateChangeData
--- @field Entity EntityHandle
--- @field NewPosition vec3
--- @field OldPosition vec3
--- @field Override int32
--- @field field_24 boolean
--- @field field_25 boolean
--- @field field_26 boolean


--- @class TranslateChangedComponent:BaseComponent
--- @field Changes table<EntityHandle, TranslateChangeData[]>


--- @class TranslateChangedEventOneFrameComponent:BaseComponent
--- @field Changes TranslateChangeData[]


--- @class TriggerAreaComponent:BaseComponent
--- @field Bounds TriggerPhysicsBoundsInfo
--- @field Physics Variant<TriggerNonePhysics,TriggerBoxPhysics,TriggerPolyPhysics,TriggerSpherePhysics>
--- @field RotationInv quat


--- @class TriggerBoxPhysics
--- @field Extents vec3


--- @class TriggerContainerComponent:BaseComponent
--- @field Events TriggerContainerEvent[]
--- @field field_0 Set_Guid


--- @class TriggerContainerEvent
--- @field Left boolean
--- @field Object Guid
--- @field Trigger Guid


--- @class TriggerIsInsideOfComponent:BaseComponent
--- @field InsideOf Guid[]


--- @class TriggerNonePhysics


--- @class TriggerPhysicsBoundsInfo
--- @field Area number
--- @field AreaExp number
--- @field AreaXZ number
--- @field AreaXZExp number
--- @field BoundsMax vec3
--- @field BoundsMin vec3
--- @field Position vec3
--- @field field_10 uint32
--- @field field_40 vec3
--- @field field_4C vec3
--- @field field_8 uint64


--- @class TriggerPolyPhysics
--- @field Height number
--- @field Points vec2[]


--- @class TriggerSpherePhysics
--- @field Radius number


--- @class TriggerTriggerTypeComponent:BaseComponent
--- @field Type uint8


--- @class TriggerUpdatedContainerComponent:BaseComponent
--- @field Updates EntityHandle[]


--- @class TriggerUpdatedPhysicsComponent:BaseComponent
--- @field Updates EntityHandle[]


--- @class UiDCWidget:UiViewModel


--- @class UiUIInitialSubstate:NoesisBaseComponent
--- @field MetaData Noesis::String
--- @field Name Noesis::String


--- @class UiUIState:NoesisBaseComponent
--- @field ActivateOnRestore Noesis::String
--- @field DisableSoundLayersAbove Noesis::String
--- @field DisableStatesBelow Noesis::String
--- @field Events NoesisBaseComponent[]
--- @field ForceSplitscreen Noesis::String
--- @field ForceVisible Noesis::String
--- @field HideStatesBelow Noesis::String
--- @field InitialSubstates NoesisBaseComponent[]
--- @field IsModal Noesis::String
--- @field IsModalCameraControlAllowed Noesis::String
--- @field Layout Noesis::String
--- @field Name Noesis::String
--- @field Owner Noesis::String
--- @field TutorialLayer Noesis::String
--- @field Widgets NoesisBaseComponent[]


--- @class UiUIStateEvent:NoesisBaseComponent
--- @field Actions NoesisBaseComponent[]
--- @field HandlesEvent Noesis::String
--- @field Name Noesis::String


--- @class UiUIStateInstance
--- @field Flags uint8
--- @field Flags2 uint8
--- @field Layout uint8
--- @field Owner uint8
--- @field PlayerID int16
--- @field State UiUIState
--- @field StateGuid Guid
--- @field StateWidgets NoesisUIElement[]
--- @field States UiUIStateInstance[][]
--- @field TutorialLayer Noesis::String
--- @field WidgetMetadata UiUIWidgetMetadata[]
--- @field Widgets UiUIWidget[]
--- @field field_3E uint8
--- @field field_3F uint8
--- @field field_40 FixedString
--- @field field_44 int32
--- @field field_48 Noesis::String
--- @field field_68 Noesis::String
--- @field field_B8 Guid


--- @class UiUIStateMachine
--- @field PlayerID int16
--- @field RootState Noesis::String
--- @field State UiUIStateInstance
--- @field States NoesisBaseComponent[]


--- @class UiUIStateWidget:NoesisBaseComponent
--- @field AllowCreationDelay boolean
--- @field BlockedLoading boolean
--- @field Filename Noesis::String
--- @field IgnoreHitTest boolean
--- @field Layer Noesis::String
--- @field Libraries NoesisBaseComponent[]
--- @field SoundLayer Noesis::String


--- @class UiUIWidget:NoesisFrameworkElement
--- @field StateGuid Guid
--- @field WidgetContainerGrid NoesisFrameworkElement
--- @field WidgetData UiUIStateWidget
--- @field XAMLPath string


--- @class UiUIWidgetMetadata
--- @field Flags uint8
--- @field StateGuid Guid


--- @class UiViewModel:NoesisBaseComponent




--- @class Armor:StatsObject
--- @field Ability Modifier Cap integer
--- @field Armor Class Ability Ability
--- @field ArmorClass integer
--- @field ArmorType ArmorType
--- @field Boosts string
--- @field Charges integer
--- @field ColorPresetResource string
--- @field ComboCategory string
--- @field DefaultBoosts string
--- @field Durability integer
--- @field DurabilityDegradeSpeed Qualifier
--- @field ExtraProperties string
--- @field FallingHitEffect string
--- @field FallingLandEffect string
--- @field Flags AttributeFlags
--- @field GameSize string
--- @field InstrumentType InstrumentType
--- @field InventoryTab InventoryTabs
--- @field ItemColor string
--- @field ItemGroup string
--- @field Level integer
--- @field MaxAmount integer
--- @field MaxCharges integer
--- @field MaxLevel integer
--- @field MinAmount integer
--- @field MinLevel integer
--- @field NeedsIdentification YesNo
--- @field ObjectCategory string
--- @field PassivesOnEquip string
--- @field PersonalStatusImmunities StatusIDs
--- @field Priority integer
--- @field Proficiency Group ProficiencyGroupFlags
--- @field Rarity Rarity
--- @field Requirements StatsRequirements
--- @field RootTemplate string
--- @field Shield YesNo
--- @field Slot Itemslot
--- @field SoundSize string
--- @field Spells string
--- @field StatusInInventory string
--- @field StatusOnEquip string
--- @field Tags string
--- @field Unique integer
--- @field UseConditions Conditions
--- @field UseCosts string
--- @field ValueOverride integer
--- @field ValueLevel integer
--- @field ValueRounding integer
--- @field ValueScale number
--- @field ValueUUID string
--- @field Weight number


--- @class Character:StatsObject
--- @field AcidResistance ResistanceFlags
--- @field ActionResources string
--- @field Armor integer
--- @field ArmorType ArmorType
--- @field BludgeoningResistance ResistanceFlags
--- @field Charisma integer
--- @field Class string
--- @field ColdResistance ResistanceFlags
--- @field Constitution integer
--- @field DarkvisionRange string
--- @field DefaultBoosts string
--- @field Dexterity integer
--- @field DifficultyStatuses string
--- @field DynamicAnimationTag string
--- @field ExtraProperties string
--- @field FallingHitEffect string
--- @field FallingLandEffect string
--- @field FireResistance ResistanceFlags
--- @field Flags AttributeFlags
--- @field ForceResistance ResistanceFlags
--- @field FOV integer
--- @field GameSize string
--- @field Hearing integer
--- @field Initiative integer
--- @field Intelligence integer
--- @field Level integer
--- @field LightningResistance ResistanceFlags
--- @field MinimumDetectionRange string
--- @field NecroticResistance ResistanceFlags
--- @field Passives string
--- @field PathInfluence string
--- @field PersonalStatusImmunities StatusIDs
--- @field PiercingResistance ResistanceFlags
--- @field PoisonResistance ResistanceFlags
--- @field ProficiencyBonusScaling string
--- @field Proficiency Group ProficiencyGroupFlags
--- @field ProficiencyBonus integer
--- @field Progression Type Progression Type
--- @field Progressions string
--- @field PsychicResistance ResistanceFlags
--- @field RadiantResistance ResistanceFlags
--- @field Sight integer
--- @field SlashingResistance ResistanceFlags
--- @field SoundSize string
--- @field SpellCastingAbility Ability
--- @field StepsType StepsType
--- @field Strength integer
--- @field ThunderResistance ResistanceFlags
--- @field UnarmedAttackAbility Ability
--- @field UnarmedRangedAttackAbility Ability
--- @field VerticalFOV integer
--- @field Vitality integer
--- @field Weight number
--- @field Wisdom integer
--- @field XPReward string


--- @class CriticalHitTypeData:StatsObject
--- @field AcidFX string
--- @field BludgeoningFX string
--- @field ColdFX string
--- @field FireFX string
--- @field ForceFX string
--- @field LightningFX string
--- @field NecroticFX string
--- @field PiercingFX string
--- @field PoisonFX string
--- @field PsychicFX string
--- @field RadiantFX string
--- @field SlashingFX string
--- @field ThunderFX string


--- @class InterruptData:StatsObject
--- @field Conditions Conditions
--- @field Container string
--- @field Cooldown CooldownType
--- @field Cost string
--- @field Description string
--- @field DescriptionRef string
--- @field DescriptionParams string
--- @field DisplayName string
--- @field DisplayNameRef string
--- @field EnableCondition Conditions
--- @field EnableContext StatsFunctorContext
--- @field ExtraDescription string
--- @field ExtraDescriptionRef string
--- @field ExtraDescriptionParams string
--- @field Failure StatsFunctors
--- @field Icon string
--- @field InterruptContext InterruptContext
--- @field InterruptContextScope InterruptContextScope
--- @field InterruptDefaultValue InterruptDefaultValue
--- @field InterruptFlags InterruptFlagsList
--- @field LoreDescription string
--- @field LoreDescriptionRef string
--- @field Properties StatsFunctors
--- @field Roll Conditions
--- @field ShortDescription string
--- @field ShortDescriptionRef string
--- @field ShortDescriptionParams string
--- @field Stack string
--- @field Success StatsFunctors
--- @field TooltipAttackSave string
--- @field TooltipDamageList string
--- @field TooltipOnMiss string
--- @field TooltipOnSave string
--- @field TooltipPermanentWarnings string
--- @field TooltipStatusApply string


--- @class Object:StatsObject
--- @field AcidResistance ResistanceFlags
--- @field AddToBottomBar YesNo
--- @field Armor integer
--- @field BludgeoningResistance ResistanceFlags
--- @field ColdResistance ResistanceFlags
--- @field ComboCategory string
--- @field DarkvisionRange integer
--- @field DefaultBoosts string
--- @field FallingHitEffect string
--- @field FallingLandEffect string
--- @field FireResistance ResistanceFlags
--- @field Flags AttributeFlags
--- @field ForceResistance ResistanceFlags
--- @field FOV integer
--- @field GameSize string
--- @field IgnoredByAI YesNo
--- @field InventoryTab InventoryTabs
--- @field ItemUseType ItemUseTypes
--- @field Level integer
--- @field LightningResistance ResistanceFlags
--- @field MaxAmount integer
--- @field MaxLevel integer
--- @field MinAmount integer
--- @field MinimumDetectionRange integer
--- @field MinLevel integer
--- @field NecroticResistance ResistanceFlags
--- @field ObjectCategory string
--- @field PassivesOnEquip string
--- @field PersonalStatusImmunities StatusIDs
--- @field PiercingResistance ResistanceFlags
--- @field PoisonResistance ResistanceFlags
--- @field Priority integer
--- @field PsychicResistance ResistanceFlags
--- @field RadiantResistance ResistanceFlags
--- @field Rarity Rarity
--- @field Requirements StatsRequirements
--- @field RootTemplate string
--- @field Sight integer
--- @field SlashingResistance ResistanceFlags
--- @field SoundSize string
--- @field StatusInInventory string
--- @field SupplyValue integer
--- @field ThunderResistance ResistanceFlags
--- @field Unique integer
--- @field UseConditions Conditions
--- @field UseCosts string
--- @field ValueOverride integer
--- @field ValueLevel integer
--- @field ValueRounding integer
--- @field ValueScale number
--- @field ValueUUID string
--- @field VerticalFOV integer
--- @field Vitality integer
--- @field Weight number


--- @class PassiveData:StatsObject
--- @field BoostConditions Conditions
--- @field BoostContext StatsFunctorContext
--- @field Boosts string
--- @field Conditions Conditions
--- @field Description string
--- @field DescriptionRef string
--- @field DescriptionParams string
--- @field DisplayName string
--- @field DisplayNameRef string
--- @field DynamicAnimationTag string
--- @field EnabledConditions Conditions
--- @field EnabledContext StatsFunctorContext
--- @field ExtraDescription string
--- @field ExtraDescriptionRef string
--- @field ExtraDescriptionParams string
--- @field Icon string
--- @field LoreDescription string
--- @field LoreDescriptionRef string
--- @field PriorityOrder integer
--- @field Properties PassiveFlags
--- @field StatsFunctorContext StatsFunctorContext
--- @field StatsFunctors StatsFunctors
--- @field ToggleGroup string
--- @field ToggleOffContext StatsFunctorContext
--- @field ToggleOffEffect string
--- @field ToggleOffFunctors StatsFunctors
--- @field ToggleOnEffect string
--- @field ToggleOnFunctors StatsFunctors
--- @field TooltipConditionalDamage string
--- @field TooltipPermanentWarnings string
--- @field TooltipSave string
--- @field TooltipUseCosts string


--- @class SpellData:StatsObject
--- @field SpellType string
--- @field Acceleration integer
--- @field AddRangeFromAbility string
--- @field AiCalculationSpellOverride string
--- @field AIFlags AIFlags
--- @field AlternativeCastTextEvents string
--- @field AmountOfTargets string
--- @field Angle integer
--- @field SpellAnimationIntentType SpellAnimationIntentType
--- @field AoEConditions StatsConditions
--- @field AreaRadius integer
--- @field Autocast YesNo
--- @field Base integer
--- @field BeamEffect string
--- @field CastEffect string
--- @field CastEffectTextEvent string
--- @field CastSound string
--- @field CastTargetHitDelay integer
--- @field CastTextEvent string
--- @field CinematicArenaFlags CinematicArenaFlags
--- @field CinematicArenaTimelineOverride string
--- @field CombatAIOverrideSpell string
--- @field ConcentrationSpellID string
--- @field ContainerSpells string
--- @field Cooldown CooldownType
--- @field CycleConditions StatsConditions
--- @field Damage string
--- @field Damage Range integer
--- @field DamageType Damage Type
--- @field DeathType Death Type
--- @field DelayRollDie DieType
--- @field DelayRollTarget integer
--- @field DelayTurnsCount integer
--- @field Description string
--- @field DescriptionRef string
--- @field DescriptionParams string
--- @field DisappearEffect string
--- @field DisplayName string
--- @field DisplayNameRef string
--- @field Distribution ProjectileDistribution
--- @field DualWieldingSpellAnimation string
--- @field DualWieldingUseCosts string
--- @field EndPosRadius integer
--- @field ExplodeRadius integer
--- @field ExtraDescription string
--- @field ExtraDescriptionRef string
--- @field ExtraDescriptionParams string
--- @field ExtraProjectileTargetConditions StatsConditions
--- @field FemaleImpactEffects string
--- @field FollowUpOriginalSpell string
--- @field ForceTarget integer
--- @field ForkChance integer
--- @field ForkingConditions StatsConditions
--- @field ForkLevels integer
--- @field FrontOffset integer
--- @field FXScale integer
--- @field Height integer
--- @field HighlightConditions StatsConditions
--- @field HitAnimationType HitAnimationType
--- @field HitCosts string
--- @field HitDelay integer
--- @field HitEffect string
--- @field HitExtension number
--- @field HitRadius number
--- @field Icon string
--- @field IgnoreTeleport YesNo
--- @field ImpactEffect string
--- @field InstrumentComponentCastSound string
--- @field InstrumentComponentImpactSound string
--- @field InstrumentComponentLoopingSound string
--- @field InstrumentComponentPrepareSound string
--- @field InterruptPrototype string
--- @field ItemWall string
--- @field ItemWallStatus string
--- @field JumpDelay integer
--- @field Level integer
--- @field Lifetime integer
--- @field LineOfSightFlags LineOfSightFlags
--- @field Magic Cost integer
--- @field MaleImpactEffects string
--- @field MaxAttacks integer
--- @field MaxDistance integer
--- @field MaxForkCount integer
--- @field MaxHitsPerTurn integer
--- @field MaximumTargets integer
--- @field MaximumTotalTargetHP integer
--- @field MemorizationRequirements MemorizationRequirements
--- @field Memory Cost integer
--- @field MinHitsPerTurn integer
--- @field MinJumpDistance number
--- @field MovementSpeed integer
--- @field MovingObjectSummonTemplate string
--- @field NextAttackChance integer
--- @field NextAttackChanceDivider integer
--- @field OnlyHit1Target integer
--- @field OriginSpellFail StatsFunctors
--- @field OriginSpellProperties StatsFunctors
--- @field OriginSpellRoll StatsRollConditions
--- @field OriginSpellSuccess StatsFunctors
--- @field OriginTargetConditions StatsConditions
--- @field OverrideSpellLevel YesNo
--- @field PositionEffect string
--- @field PowerLevel integer
--- @field PrepareEffect string
--- @field PrepareEffectBone string
--- @field PrepareLoopSound string
--- @field PrepareSound string
--- @field PreviewCursor CursorMode
--- @field PreviewEffect string
--- @field PreviewStrikeHits YesNo
--- @field ProjectileCount string
--- @field ProjectileDelay integer
--- @field ProjectileSpells string
--- @field ProjectileTerrainOffset YesNo
--- @field ProjectileType ProjectileType
--- @field Range integer
--- @field ReappearEffect string
--- @field ReappearEffectTextEvent string
--- @field RechargeValues string
--- @field Requirement SpellRequirement
--- @field RequirementConditions StatsConditions
--- @field RequirementEvents StatusEvent
--- @field Requirements StatsRequirements
--- @field RitualCosts string
--- @field RootSpellID string
--- @field SelectedCharacterEffect string
--- @field SelectedObjectEffect string
--- @field SelectedPositionEffect string
--- @field Shape string
--- @field Sheathing SpellSheathing
--- @field ShortDescription string
--- @field ShortDescriptionRef string
--- @field ShortDescriptionParams string
--- @field Shuffle YesNo
--- @field SingleSource YesNo
--- @field SourceLimbIndex integer
--- @field SpawnEffect string
--- @field SpellActionType SpellActionType
--- @field SpellActionTypePriority integer
--- @field SpellAnimation string
--- @field SpellAnimationType SpellAnimationType
--- @field Spellbook string
--- @field SpellCategory SpellCategoryFlags
--- @field SpellContainerID string
--- @field SpellEffect string
--- @field SpellFail StatsFunctors
--- @field SpellFlags SpellFlagList
--- @field SpellJumpType SpellJumpType
--- @field MemoryCost integer
--- @field SpellProperties StatsFunctors
--- @field SpellRoll StatsRollConditions
--- @field SpellSchool SpellSchool
--- @field SpellSoundAftermathTrajectory string
--- @field SpellSoundMagnitude SpellSoundMagnitude
--- @field SpellStyleGroup SpellStyleGroup
--- @field SpellSuccess StatsFunctors
--- @field Stealth YesNo
--- @field SteerSpeedMultipler number
--- @field StopAtFirstContact integer
--- @field StormEffect string
--- @field StrikeCount integer
--- @field SurfaceGrowInterval integer
--- @field SurfaceGrowStep integer
--- @field SurfaceLifetime integer
--- @field SurfaceRadius integer
--- @field SurfaceType Surface Type
--- @field TargetCeiling string
--- @field TargetConditions StatsConditions
--- @field TargetEffect string
--- @field TargetFloor string
--- @field TargetGroundEffect string
--- @field TargetHitEffect string
--- @field TargetProjectiles YesNo
--- @field TargetRadius string
--- @field TargetSound string
--- @field TeleportDelay integer
--- @field TeleportSelf YesNo
--- @field TeleportSurface YesNo
--- @field Template string
--- @field ThrowableSpellFail StatsFunctors
--- @field ThrowableSpellProperties StatsFunctors
--- @field ThrowableSpellRoll StatsRollConditions
--- @field ThrowableSpellSuccess StatsFunctors
--- @field ThrowableTargetConditions StatsConditions
--- @field ThrowOrigin ThrowOrigin
--- @field TooltipAttackSave string
--- @field TooltipDamageList string
--- @field TooltipOnMiss string
--- @field TooltipOnSave string
--- @field TooltipPermanentWarnings string
--- @field TooltipSpellDCAbilities AbilityFlags
--- @field TooltipStatusApply string
--- @field TooltipUpcastDescription string
--- @field TooltipUpcastDescriptionParams string
--- @field Trajectories string
--- @field UseCosts string
--- @field UseWeaponDamage YesNo
--- @field UseWeaponProperties YesNo
--- @field VerbalIntent VerbalIntent
--- @field VocalComponentSound string
--- @field WallEndEffect string
--- @field WallStartEffect string
--- @field WeaponBones string
--- @field WeaponTypes WeaponFlags


--- @class StatusData:StatsObject
--- @field StatusType string
--- @field AbsorbSurfaceRange integer
--- @field AbsorbSurfaceType string
--- @field AiCalculationSpellOverride string
--- @field AnimationEnd string
--- @field AnimationLoop string
--- @field AnimationStart string
--- @field ApplyEffect string
--- @field AuraFlags AuraFlags
--- @field AuraFX string
--- @field AuraRadius integer
--- @field AuraStatuses StatsFunctors
--- @field BeamEffect string
--- @field BonusFromSkill Skill
--- @field Boosts string
--- @field Charges integer
--- @field DefendTargetPosition YesNo
--- @field Description string
--- @field DescriptionRef string
--- @field DescriptionParams string
--- @field DieAction string
--- @field DisableInteractions YesNo
--- @field DisplayName string
--- @field DisplayNameRef string
--- @field DynamicAnimationTag string
--- @field EndEffect string
--- @field ForceStackOverwrite YesNo
--- @field FormatColor FormatStringColor
--- @field FreezeTime integer
--- @field HealEffectId string
--- @field HealMultiplier integer
--- @field HealStat StatusHealType
--- @field HealType HealValueType
--- @field HealValue string
--- @field HideOverheadUI integer
--- @field HitAnimationType HitAnimationType
--- @field Icon string
--- @field ImmuneFlag AttributeFlags
--- @field Instant YesNo
--- @field IsUnique integer
--- @field Items string
--- @field LeaveAction string
--- @field LEDEffect LEDEffectType
--- @field ManagedStatusEffectGroup string
--- @field ManagedStatusEffectType ManagedStatusEffectType
--- @field Material string
--- @field MaterialApplyArmor YesNo
--- @field MaterialApplyBody YesNo
--- @field MaterialApplyNormalMap YesNo
--- @field MaterialApplyWeapon YesNo
--- @field MaterialFadeAmount integer
--- @field MaterialOverlayOffset integer
--- @field MaterialParameters string
--- @field MaterialType MaterialType
--- @field MeshEffect string
--- @field Necromantic YesNo
--- @field NumStableFailed integer
--- @field NumStableSuccess integer
--- @field OnApplyConditions Conditions
--- @field OnApplyFail StatsFunctors
--- @field OnApplyFunctors StatsFunctors
--- @field OnApplyRoll Conditions
--- @field OnApplySuccess StatsFunctors
--- @field OnRemoveFail StatsFunctors
--- @field OnRemoveFunctors StatsFunctors
--- @field OnRemoveRoll Conditions
--- @field OnRemoveSuccess StatsFunctors
--- @field OnRollsFailed StatsFunctors
--- @field OnSuccess StatsFunctors
--- @field OnTickFail StatsFunctors
--- @field OnTickRoll Conditions
--- @field OnTickSuccess StatsFunctors
--- @field Passives string
--- @field PeaceOnly YesNo
--- @field PerformEventName string
--- @field PlayerHasTag string
--- @field PlayerSameParty YesNo
--- @field PolymorphResult string
--- @field Projectile string
--- @field Radius integer
--- @field RemoveConditions Conditions
--- @field RemoveEvents StatusEvent
--- @field ResetCooldowns string
--- @field RetainSpells string
--- @field Rules string
--- @field Sheathing StatusSheathing
--- @field SoundLoop string
--- @field SoundStart string
--- @field SoundStop string
--- @field SoundVocalEnd SoundVocalType
--- @field SoundVocalLoop SoundVocalType
--- @field SoundVocalStart SoundVocalType
--- @field Spells string
--- @field SplatterBloodAmount number
--- @field SplatterDirtAmount number
--- @field SplatterSweatAmount number
--- @field StableRoll string
--- @field StableRollDC integer
--- @field StackId string
--- @field StackPriority integer
--- @field StackType StatusStackType
--- @field StatsId string
--- @field StatusEffect string
--- @field StatusEffectOnTurn string
--- @field StatusEffectOverride string
--- @field StatusEffectOverrideForItems string
--- @field StatusGroups StatusGroupFlags
--- @field StatusPropertyFlags StatusPropertyFlags
--- @field StatusSoundState string
--- @field StillAnimationPriority StillAnimPriority
--- @field StillAnimationType StatusAnimationType
--- @field SurfaceChange string
--- @field TargetConditions string
--- @field TargetEffect string
--- @field TemplateID string
--- @field TickFunctors StatsFunctors
--- @field TickType TickType
--- @field Toggle YesNo
--- @field TooltipDamage string
--- @field TooltipPermanentWarnings string
--- @field TooltipSave string
--- @field UseLyingPickingState YesNo
--- @field WeaponOverride string


--- @class Weapon:StatsObject
--- @field Boosts string
--- @field BoostsOnEquipMainHand string
--- @field BoostsOnEquipOffHand string
--- @field Charges integer
--- @field ColorPresetResource string
--- @field ComboCategory string
--- @field Damage string
--- @field Damage Range integer
--- @field Damage Type Damage Type
--- @field DefaultBoosts string
--- @field Durability integer
--- @field DurabilityDegradeSpeed Qualifier
--- @field ExtraProperties string
--- @field FallingHitEffect string
--- @field FallingLandEffect string
--- @field Flags AttributeFlags
--- @field GameSize string
--- @field IgnoreVisionBlock YesNo
--- @field InventoryTab InventoryTabs
--- @field ItemColor string
--- @field ItemGroup string
--- @field Level integer
--- @field MaxAmount integer
--- @field MaxCharges integer
--- @field MaxLevel integer
--- @field MinAmount integer
--- @field MinLevel integer
--- @field NeedsIdentification YesNo
--- @field ObjectCategory string
--- @field PassivesMainHand string
--- @field PassivesOffHand string
--- @field PassivesOnEquip string
--- @field PersonalStatusImmunities StatusIDs
--- @field Priority integer
--- @field Proficiency Group ProficiencyGroupFlags
--- @field Projectile string
--- @field Rarity Rarity
--- @field Requirements StatsRequirements
--- @field RootTemplate string
--- @field Slot Itemslot
--- @field SoundSize string
--- @field Spells string
--- @field StatusInInventory string
--- @field StatusOnEquip string
--- @field SupplyValue integer
--- @field Tags string
--- @field Unique integer
--- @field UniqueWeaponSoundSwitch string
--- @field UseConditions Conditions
--- @field UseCosts string
--- @field ValueOverride integer
--- @field ValueLevel integer
--- @field ValueRounding integer
--- @field ValueScale number
--- @field ValueUUID string
--- @field VersatileDamage string
--- @field Weapon Group Weapon Group
--- @field Weapon Properties WeaponFlags
--- @field WeaponFunctors StatsFunctors
--- @field WeaponRange integer
--- @field Weight number




--- @class Ext_ClientAudio
--- @field GetRTPC fun(a1:uint64, a2:string):number
--- @field LoadBank fun(a1:string):boolean
--- @field LoadEvent fun(a1:string):boolean
--- @field PauseAllSounds fun()
--- @field PlayExternalSound fun(a1:uint64, a2:string, a3:string, a4:AudioCodec, a5:number?):boolean
--- @field PostEvent fun(a1:uint64, a2:string, a3:number?):boolean
--- @field PrepareBank fun(a1:string):boolean
--- @field ResetRTPC fun(a1:uint64, a2:string)
--- @field ResumeAllSounds fun()
--- @field SetRTPC fun(a1:uint64, a2:string, a3:number, a4:boolean?):boolean
--- @field SetState fun(a1:string, a2:string):boolean
--- @field SetSwitch fun(a1:uint64, a2:string, a3:string):boolean
--- @field Stop fun(a1:uint64?)
--- @field UnloadBank fun(a1:string):boolean
--- @field UnloadEvent fun(a1:string):boolean
--- @field UnprepareBank fun(a1:string):boolean
local Ext_ClientAudio = {}



--- @class Ext_ClientIMGUI
--- @field EnableDemo fun(a1:boolean)
--- @field GetViewportSize fun():ivec2
--- @field LoadFont fun(a1:FixedString, a2:string, a3:number):boolean
--- @field NewWindow fun(a1:string):ExtuiWindow
--- @field SetFontScaleMultiplier fun(a1:number)
--- @field SetScale fun(a1:number)
--- @field SetUIScaleMultiplier fun(a1:number)
local Ext_ClientIMGUI = {}



--- @class Ext_ClientInput
--- @field GetInputManager fun():InputInputManager
--- @field InjectKeyDown fun(a1:SDLScanCode, a2:SDLKeyModifier?)
--- @field InjectKeyPress fun(a1:SDLScanCode, a2:SDLKeyModifier?)
--- @field InjectKeyUp fun(a1:SDLScanCode, a2:SDLKeyModifier?)
local Ext_ClientInput = {}



--- @class Ext_ClientNet
--- @field IsHost fun():boolean
--- @field PostMessageToServer fun(a1:string, a2:string, a3:Guid?, a4:FunctionRef?, a5:uint32?)
local Ext_ClientNet = {}



--- @class Ext_ClientTemplate
--- @field GetAllRootTemplates fun():table<FixedString, GameObjectTemplate>
--- @field GetRootTemplate fun(a1:FixedString):GameObjectTemplate
--- @field GetTemplate fun(a1:FixedString):GameObjectTemplate
local Ext_ClientTemplate = {}



--- @class Ext_ClientUI
--- @field GetCursorControl fun():EclCursorControl
--- @field GetDragDrop fun(a1:uint16):EclPlayerDragData
--- @field GetPickingHelper fun(a1:uint16):EclPlayerPickingHelper
--- @field GetRoot fun():NoesisFrameworkElement
--- @field GetStateMachine fun():UiUIStateMachine
--- @field SetState fun(a1:FixedString, a2:FixedString?, a3:boolean?, a4:int16?)
local Ext_ClientUI = {}



--- @class Ext_Debug
--- @field Crash fun(a1:int32)
--- @field DebugBreak fun()
--- @field DebugDumpLifetimes fun()
--- @field DumpStack fun()
--- @field GenerateIdeHelpers fun(a1:boolean?)
--- @field IsDeveloperMode fun():boolean
--- @field Reset fun()
--- @field SetEntityRuntimeCheckLevel fun(a1:int32)
local Ext_Debug = {}



--- @class Ext_Entity
--- @field ClearTrace fun()
--- @field Create fun():EntityHandle
--- @field Destroy fun(a1:EntityHandle):boolean
--- @field EnableTracing fun(a1:boolean)
--- @field Get fun(a1:AnyRef)
--- @field GetAllEntities fun():EntityHandle[]
--- @field GetAllEntitiesWithComponent fun(a1:ExtComponentType):EntityHandle[]
--- @field GetAllEntitiesWithUuid fun():table<Guid, EntityHandle>
--- @field GetRegisteredComponentTypes fun(a1:boolean?, a2:boolean?):StringView[]
--- @field GetTrace fun():EcsECSChangeLog
--- @field HandleToUuid fun(a1:EntityHandle):Guid?
--- @field OnChange fun(a1:ExtComponentType, a2:FunctionRef, a3:EntityHandle?, a4:uint64?):uint64?
--- @field OnCreate fun(a1:ExtComponentType, a2:FunctionRef, a3:EntityHandle?, a4:boolean?, a5:boolean?):uint64
--- @field OnCreateDeferred fun(a1:ExtComponentType, a2:FunctionRef, a3:EntityHandle?):uint64
--- @field OnCreateDeferredOnce fun(a1:ExtComponentType, a2:FunctionRef, a3:EntityHandle?):uint64
--- @field OnCreateOnce fun(a1:ExtComponentType, a2:FunctionRef, a3:EntityHandle?):uint64
--- @field OnDestroy fun(a1:ExtComponentType, a2:FunctionRef, a3:EntityHandle?, a4:boolean?, a5:boolean?):uint64
--- @field OnDestroyDeferred fun(a1:ExtComponentType, a2:FunctionRef, a3:EntityHandle?):uint64
--- @field OnDestroyDeferredOnce fun(a1:ExtComponentType, a2:FunctionRef, a3:EntityHandle?):uint64
--- @field OnDestroyOnce fun(a1:ExtComponentType, a2:FunctionRef, a3:EntityHandle?):uint64
--- @field OnSystemPostUpdate fun(a1:ExtSystemType, a2:FunctionRef, a3:boolean?):uint64
--- @field OnSystemUpdate fun(a1:ExtSystemType, a2:FunctionRef, a3:boolean?):uint64
--- @field Subscribe fun(a1:ExtComponentType, a2:FunctionRef, a3:EntityHandle?, a4:uint64?):uint64?
--- @field Unsubscribe fun(a1:uint64):boolean
--- @field UuidToHandle fun(a1:Guid):EntityHandle
local Ext_Entity = {}



--- @class Ext_IO
--- @field AddPathOverride fun(a1:string, a2:string)
--- @field GetPathOverride fun(a1:string):string?
--- @field LoadFile fun(a1:string, a2:FixedString?):string?
--- @field SaveFile fun(a1:string, a2:StringView):boolean
local Ext_IO = {}



--- @class Ext_Json
--- @field Parse fun(str:string):table parsedTable
--- @field Stringify fun(data:table, opts:JsonStringifyOptions?):string tableAsString
local Ext_Json = {}
--- @class JsonStringifyOptions
--- @field Beautify boolean Sorts the output table, and indents with tabs. Defaults to true.
--- @field StringifyInternalTypes boolean Defaults to false.
--- @field IterateUserdata boolean Defaults to false.
--- @field AvoidRecursion boolean Defaults to false.
--- @field MaxDepth integer Defaults to 64, the maximum value.
--- @field LimitDepth integer Defaults to -1 (off).
--- @field LimitArrayElements integer Defaults to -1 (off).



--- @class Ext_Level
--- @field BeginPathfinding fun(a1:EntityHandle, a2:vec3, a3:RegistryEntry):AiPath
--- @field BeginPathfindingImmediate fun(a1:EntityHandle, a2:vec3):AiPath
--- @field FindPath fun(a1:AiPath):boolean
--- @field GetActivePathfindingRequests fun():AiPath[]
--- @field GetEntitiesOnTile fun(a1:vec3):EntityHandle[]
--- @field GetHeightsAt fun(a1:number, a2:number):number[]
--- @field GetPathById fun(a1:int32):AiPath
--- @field GetTileDebugInfo fun(a1:vec3):AiGridLuaTile
--- @field RaycastAll fun(a1:vec3, a2:vec3, a3:PhysicsType, a4:PhysicsGroupFlags, a5:PhysicsGroupFlags, a6:int32):PhxPhysicsHitAll
--- @field RaycastAny fun(a1:vec3, a2:vec3, a3:PhysicsType, a4:PhysicsGroupFlags, a5:PhysicsGroupFlags, a6:int32):boolean
--- @field RaycastClosest fun(a1:vec3, a2:vec3, a3:PhysicsType, a4:PhysicsGroupFlags, a5:PhysicsGroupFlags, a6:int32):PhxPhysicsHit
--- @field ReleasePath fun(a1:AiPath)
--- @field SweepBoxAll fun(a1:vec3, a2:vec3, a3:vec3, a4:PhysicsType, a5:PhysicsGroupFlags, a6:PhysicsGroupFlags, a7:int32):PhxPhysicsHitAll
--- @field SweepBoxClosest fun(a1:vec3, a2:vec3, a3:vec3, a4:PhysicsType, a5:PhysicsGroupFlags, a6:PhysicsGroupFlags, a7:int32):PhxPhysicsHit
--- @field SweepCapsuleAll fun(a1:vec3, a2:vec3, a3:number, a4:number, a5:PhysicsType, a6:PhysicsGroupFlags, a7:PhysicsGroupFlags, a8:int32):PhxPhysicsHitAll
--- @field SweepCapsuleClosest fun(a1:vec3, a2:vec3, a3:number, a4:number, a5:PhysicsType, a6:PhysicsGroupFlags, a7:PhysicsGroupFlags, a8:int32):PhxPhysicsHit
--- @field SweepSphereAll fun(a1:vec3, a2:vec3, a3:number, a4:PhysicsType, a5:PhysicsGroupFlags, a6:PhysicsGroupFlags, a7:int32):PhxPhysicsHitAll
--- @field SweepSphereClosest fun(a1:vec3, a2:vec3, a3:number, a4:PhysicsType, a5:PhysicsGroupFlags, a6:PhysicsGroupFlags, a7:int32):PhxPhysicsHit
--- @field TestBox fun(a1:vec3, a2:vec3, a3:PhysicsType, a4:PhysicsGroupFlags, a5:PhysicsGroupFlags):PhxPhysicsHitAll
--- @field TestSphere fun(a1:vec3, a2:number, a3:PhysicsType, a4:PhysicsGroupFlags, a5:PhysicsGroupFlags):PhxPhysicsHitAll
local Ext_Level = {}



--- @class Ext_Loca
--- @field GetTranslatedString fun(a1:FixedString, a2:string?):string
--- @field UpdateTranslatedString fun(a1:FixedString, a2:string):boolean
local Ext_Loca = {}



--- @class Ext_Log
--- @field Print fun()
--- @field PrintError fun()
--- @field PrintWarning fun()
local Ext_Log = {}



--- @class Ext_Math
--- @field Acos fun(a1:number):number
--- @field Add fun()
--- @field Angle fun()
--- @field Asin fun(a1:number):number
--- @field Atan fun(a1:number):number
--- @field Atan2 fun(a1:number, a2:number):number
--- @field BuildFromAxisAngle3 fun(a1:vec3, a2:number):mat3
--- @field BuildFromAxisAngle4 fun(a1:vec3, a2:number):mat4
--- @field BuildFromEulerAngles3 fun(a1:vec3):mat3
--- @field BuildFromEulerAngles4 fun(a1:vec3):mat4
--- @field BuildRotation3 fun(a1:vec3, a2:number):mat3
--- @field BuildRotation4 fun(a1:vec3, a2:number):mat4
--- @field BuildScale fun(a1:vec3):mat4
--- @field BuildTranslation fun(a1:vec3):mat4
--- @field Clamp fun(a1:number, a2:number, a3:number):number
--- @field Cross fun(a1:vec3, a2:vec3)
--- @field Decompose fun(a1:mat4, a2:vec3, a3:vec3, a4:vec3)
--- @field Determinant fun()
--- @field Distance fun(a1:vec3, a2:vec3):number
--- @field Div fun()
--- @field Dot fun(a1:vec3, a2:vec3):number
--- @field ExtractAxisAngle fun():number
--- @field ExtractEulerAngles fun():vec3
--- @field Fract fun(a1:number):number
--- @field Inverse fun()
--- @field IsInf fun(a1:number):boolean
--- @field IsNaN fun(a1:number):boolean
--- @field Length fun()
--- @field Lerp fun(a1:number, a2:number, a3:number):number
--- @field Mat3ToQuat fun(a1:mat3):quat
--- @field Mat4ToQuat fun(a1:mat4):quat
--- @field Mul fun()
--- @field Normalize fun()
--- @field OuterProduct fun()
--- @field Perpendicular fun()
--- @field Project fun()
--- @field QuatDot fun(a1:quat, a2:quat):number
--- @field QuatFromEuler fun(a1:vec3):quat
--- @field QuatFromToRotation fun(a1:vec3, a2:vec3):quat
--- @field QuatInverse fun(a1:quat):quat
--- @field QuatLength fun(a1:quat):number
--- @field QuatMul fun()
--- @field QuatNormalize fun(a1:quat):quat
--- @field QuatRotate fun(a1:quat)
--- @field QuatRotateAxisAngle fun(a1:quat, a2:vec3, a3:number):quat
--- @field QuatSlerp fun(a1:quat, a2:quat, a3:number):quat
--- @field QuatToMat3 fun(a1:quat):mat3
--- @field QuatToMat4 fun(a1:quat):mat4
--- @field Random fun()
--- @field Reflect fun()
--- @field Rotate fun()
--- @field Round fun(a1:number):int64
--- @field Scale fun(a1:mat4, a2:vec3)
--- @field Sign fun(a1:number):number
--- @field Smoothstep fun(a1:number, a2:number, a3:number):number
--- @field Sub fun()
--- @field Translate fun(a1:mat4, a2:vec3)
--- @field Transpose fun()
--- @field Trunc fun(a1:number):number
local Ext_Math = {}



--- @class Ext_Mod
--- @field GetBaseMod fun():Module
--- @field GetLoadOrder fun():FixedString[]
--- @field GetMod fun(a1:FixedString):Module
--- @field GetModManager fun():ModManager
--- @field IsModLoaded fun(a1:FixedString):boolean
local Ext_Mod = {}



--- @class Ext_Resource
--- @field Get fun(a1:FixedString, a2:ResourceBankType)
--- @field GetAll fun(a1:ResourceBankType):FixedString[]
local Ext_Resource = {}



--- @class Ext_ServerNet
--- @field BroadcastMessage fun(a1:string, a2:string, a3:Guid?, a4:Guid?, a5:FunctionRef?, a6:uint32?)
--- @field IsHost fun():boolean
--- @field PlayerHasExtender fun(a1:Guid):boolean?
--- @field PostMessageToClient fun(a1:Guid, a2:string, a3:string, a4:Guid?, a5:FunctionRef?, a6:uint32?)
--- @field PostMessageToUser fun(a1:int32, a2:string, a3:string, a4:Guid?, a5:FunctionRef?, a6:uint32?)
local Ext_ServerNet = {}



--- @class Ext_ServerTemplate
--- @field GetAllCacheTemplates fun():table<FixedString, GameObjectTemplate>
--- @field GetAllLocalCacheTemplates fun():table<FixedString, GameObjectTemplate>
--- @field GetAllLocalTemplates fun():table<FixedString, GameObjectTemplate>
--- @field GetAllRootTemplates fun():table<FixedString, GameObjectTemplate>
--- @field GetCacheTemplate fun(a1:FixedString):GameObjectTemplate
--- @field GetLocalCacheTemplate fun(a1:FixedString):GameObjectTemplate
--- @field GetLocalTemplate fun(a1:FixedString):GameObjectTemplate
--- @field GetRootTemplate fun(a1:FixedString):GameObjectTemplate
--- @field GetTemplate fun(a1:FixedString):GameObjectTemplate
local Ext_ServerTemplate = {}



--- @class Ext_StaticData
--- @field Get fun(a1:Guid, a2:ExtResourceManagerType)
--- @field GetAll fun(a1:ExtResourceManagerType):Guid[]
--- @field GetByModId fun(a1:ExtResourceManagerType, a2:Guid):Guid[]
--- @field GetSources fun(a1:ExtResourceManagerType):table<Guid, Guid[]>
local Ext_StaticData = {}



--- @class Ext_Stats
--- @field TreasureCategory Ext_StatsTreasureCategory
--- @field TreasureTable Ext_StatsTreasureTable
--- @field AddAttribute fun(a1:FixedString, a2:FixedString, a3:FixedString):boolean
--- @field AddEnumerationValue fun(typeName:FixedString, enumLabel:FixedString):int32
--- @field Create fun(a1:FixedString, a2:FixedString, a3:FixedString?, a4:boolean?):StatsObject
--- @field EnumIndexToLabel fun(a1:FixedString, a2:int32):FixedString?
--- @field EnumLabelToIndex fun(a1:FixedString, a2:FixedString):int64?
--- @field ExecuteFunctor fun(a1:StatsFunctor, a2:StatsContextData)
--- @field ExecuteFunctors fun(a1:StatsFunctors, a2:StatsContextData)
--- @field Get fun(a1:string, a2:int32?, a3:boolean?, a4:boolean?):FixedString[]
--- @field GetCachedInterrupt fun(a1:FixedString):StatsInterruptPrototype
--- @field GetCachedPassive fun(a1:FixedString):StatsPassivePrototype
--- @field GetCachedSpell fun(a1:FixedString):StatsSpellPrototype
--- @field GetCachedStatus fun(a1:FixedString):StatsStatusPrototype
--- @field GetModifierAttributes fun(a1:FixedString):table<FixedString, FixedString>
--- @field GetStats fun(a1:FixedString?):FixedString[]
--- @field GetStatsLoadedBefore fun(a1:FixedString, a2:FixedString?):FixedString[]
--- @field GetStatsManager fun():StatsRPGStats
--- @field PrepareFunctorParams fun(a1:FunctorContextType):StatsContextData
--- @field SetPersistence fun(a1:FixedString, a2:boolean)
--- @field Sync fun(a1:FixedString, a2:boolean?)
local Ext_Stats = {}



--- @class Ext_StatsTreasureCategory
--- @field GetLegacy fun(id:FixedString):StatTreasureCategory
--- @field Update fun(id:FixedString, tbl:StatTreasureCategory)
local Ext_StatsTreasureCategory = {}



--- @class Ext_StatsTreasureTable
--- @field Get fun(a1:FixedString):StatsTreasureTable
--- @field GetLegacy fun(id:FixedString):StatTreasureTable
--- @field Update fun(tbl:StatTreasureTable)
local Ext_StatsTreasureTable = {}



--- @class Ext_Timer
--- @field Cancel fun(a1:uint64):boolean
--- @field ClockEpoch fun():int64
--- @field ClockTime fun():string
--- @field GameTime fun():number
--- @field MicrosecTime fun():number
--- @field MonotonicTime fun():int64
--- @field Pause fun(a1:uint64):boolean
--- @field RegisterPersistentHandler fun(a1:FixedString, a2:Ref)
--- @field Resume fun(a1:uint64):boolean
--- @field WaitFor fun(a1:number, a2:Ref, a3:number?):uint64
--- @field WaitForPersistent fun(a1:number, a2:FixedString, a3:Ref, a4:number?):uint64
--- @field WaitForRealtime fun(a1:number, a2:Ref, a3:number?):uint64
local Ext_Timer = {}



--- @class Ext_Types
--- @field Construct fun(a1:FixedString)
--- @field GetAllTypes fun():FixedString[]
--- @field GetHashSetValueAt fun(a1:AnyRef, a2:uint32)
--- @field GetObjectType fun(a1:AnyRef):FixedString|"userdata"|"lightuserdata" objectType
--- @field GetTypeInfo fun(a1:FixedString):TypeInformation
--- @field GetValueType fun(a1:AnyRef):string?
--- @field IsA fun(a1:AnyRef, a2:FixedString):boolean
--- @field Serialize fun()
--- @field TypeOf fun(a1:AnyRef):TypeInformation
--- @field Unserialize fun()
--- @field Validate fun():boolean
local Ext_Types = {}
--- @class GenerateIdeHelpersOptions
--- @field AddAliasEnums boolean Add the enums in alias format, for string comparison. Defaults to true.
--- @field UseBaseExtraData boolean Only include the base ExtraData keys/values in Shared, instead of grabbing whatever the current keys are in the mod environment.
--- @field GenerateExtraDataAsClass boolean Annotate ExtraData as a class, so it only has fields with no fixed/hardcoded values.

--- Generate an ExtIdeHelpers file
--- @param outputPath string? Optional path to save the generated helper file, relative to the `%LOCALAPPDATA%\Larian Studios\Baldur's Gate 3\Script Extender` folder
--- @param opts GenerateIdeHelpersOptions? The optional settings to use
--- @return string fileContents Returns the file contents, for use with Ext.IO.SaveFile
function Ext_Types.GenerateIdeHelpers(outputPath, opts) end




--- @class Ext_Utils
--- @field DEBUG_GetShapeshiftSystem fun():EsvShapeshiftSystem
--- @field GameVersion fun():string?
--- @field GetCommandLineParams fun():string[]
--- @field GetDialogManager fun():DlgDialogManager
--- @field GetGameState fun()
--- @field GetGlobalSwitches fun():GlobalSwitches
--- @field GetValueType fun(value:any):string|"nil"|"number"|"string"|"boolean"|"table"|"function"|"thread"|"userdata"|"lightuserdata" valueType # Returns a base type, lightuserdata, or an extender type.
--- @field HandleToInteger fun(a1:EntityHandle):int64
--- @field Include fun(modGUID:string?, path:string, replaceGlobals:table?):any returnedValue Whatever the script returned, if anything
--- @field IntegerToHandle fun(a1:int64):EntityHandle
--- @field IsValidHandle fun(handle:ComponentHandle):boolean
--- @field LoadString fun(str:string):UserReturn Similar to lua `loadstring`, with extra safeguards.
--- @field ShowError fun(a1:string)
--- @field ShowErrorAndExitGame fun(a1:string)
--- @field Version fun():int32
local Ext_Utils = {}



--- @class Ext_Vars
--- @field DirtyModVariables fun(a1:Guid?, a2:FixedString?)
--- @field DirtyUserVariables fun(a1:Guid?, a2:FixedString?)
--- @field GetEntitiesWithVariable fun(a1:FixedString):Guid[]
--- @field GetModVariables fun(a1:Guid)
--- @field RegisterModVariable fun(a1:Guid, a2:FixedString)
--- @field RegisterUserVariable fun(a1:FixedString)
--- @field SyncModVariables fun()
--- @field SyncUserVariables fun()
local Ext_Vars = {}



--- @class ExtClient
--- @field Audio Ext_ClientAudio
--- @field ClientAudio Ext_ClientAudio
--- @field IMGUI Ext_ClientIMGUI
--- @field ClientIMGUI Ext_ClientIMGUI
--- @field Input Ext_ClientInput
--- @field ClientInput Ext_ClientInput
--- @field Net Ext_ClientNet
--- @field ClientNet Ext_ClientNet
--- @field Template Ext_ClientTemplate
--- @field ClientTemplate Ext_ClientTemplate
--- @field UI Ext_ClientUI
--- @field ClientUI Ext_ClientUI
--- @field Debug Ext_Debug
--- @field Entity Ext_Entity
--- @field IO Ext_IO
--- @field Json Ext_Json
--- @field Level Ext_Level
--- @field Loca Ext_Loca
--- @field Log Ext_Log
--- @field Math Ext_Math
--- @field Mod Ext_Mod
--- @field Resource Ext_Resource
--- @field StaticData Ext_StaticData
--- @field Stats Ext_Stats
--- @field Timer Ext_Timer
--- @field Types Ext_Types
--- @field Utils Ext_Utils
--- @field Vars Ext_Vars

--- @class ExtServer
--- @field Debug Ext_Debug
--- @field Entity Ext_Entity
--- @field IO Ext_IO
--- @field Json Ext_Json
--- @field Level Ext_Level
--- @field Loca Ext_Loca
--- @field Log Ext_Log
--- @field Math Ext_Math
--- @field Mod Ext_Mod
--- @field Resource Ext_Resource
--- @field Net Ext_ServerNet
--- @field ServerNet Ext_ServerNet
--- @field Template Ext_ServerTemplate
--- @field ServerTemplate Ext_ServerTemplate
--- @field StaticData Ext_StaticData
--- @field Stats Ext_Stats
--- @field Timer Ext_Timer
--- @field Types Ext_Types
--- @field Utils Ext_Utils
--- @field Vars Ext_Vars

--#region Generated Enums

--- @class Ext_Enums
local Ext_Enums = {}


--- @enum AIBoundType
Ext_Enums.AIBoundType = {
	Move = 0,
	Hit = 1,
	Stand = 2,
	[0] = "Move",
	[1] = "Hit",
	[2] = "Stand",
}

--- @enum AIFlags
Ext_Enums.AIFlags = {
	CanNotUse = 1,
	IgnoreSelf = 2,
	IgnoreDebuff = 4,
	IgnoreBuff = 8,
	StatusIsSecondary = 16,
	IgnoreControl = 32,
	CanNotTargetFrozen = 64,
	[1] = "CanNotUse",
	[2] = "IgnoreSelf",
	[4] = "IgnoreDebuff",
	[8] = "IgnoreBuff",
	[16] = "StatusIsSecondary",
	[32] = "IgnoreControl",
	[64] = "CanNotTargetFrozen",
}

--- @enum AIShapeType
Ext_Enums.AIShapeType = {
	None = 0,
	Box = 1,
	Cylinder = 2,
	[0] = "None",
	[1] = "Box",
	[2] = "Cylinder",
}

--- @enum AbilityId
Ext_Enums.AbilityId = {
	None = 0,
	Strength = 1,
	Dexterity = 2,
	Constitution = 3,
	Intelligence = 4,
	Wisdom = 5,
	Charisma = 6,
	Sentinel = 7,
	[0] = "None",
	[1] = "Strength",
	[2] = "Dexterity",
	[3] = "Constitution",
	[4] = "Intelligence",
	[5] = "Wisdom",
	[6] = "Charisma",
	[7] = "Sentinel",
}

--- @enum ActionDataType
Ext_Enums.ActionDataType = {
	Unknown = 0,
	OpenClose = 1,
	Destroy = 2,
	Teleport = 3,
	CreateSurface = 4,
	DestroyParameters = 5,
	Equip = 6,
	Consume = 7,
	StoryUse = 8,
	Door = 9,
	CreatePuddle = 10,
	Book = 11,
	UseSpell = 12,
	SpellBook = 13,
	Sit = 14,
	Lie = 15,
	Insert = 16,
	Stand = 17,
	Lockpick = 18,
	StoryUseInInventory = 19,
	DisarmTrap = 20,
	ShowStoryElementUI = 22,
	Combine = 23,
	Ladder = 24,
	PlaySound = 26,
	SpawnCharacter = 27,
	Constrain = 28,
	Recipe = 30,
	Unknown31 = 31,
	Throw = 32,
	LearnSpell = 33,
	Unknown34 = 34,
	Unknown35 = 35,
	[0] = "Unknown",
	[1] = "OpenClose",
	[2] = "Destroy",
	[3] = "Teleport",
	[4] = "CreateSurface",
	[5] = "DestroyParameters",
	[6] = "Equip",
	[7] = "Consume",
	[8] = "StoryUse",
	[9] = "Door",
	[10] = "CreatePuddle",
	[11] = "Book",
	[12] = "UseSpell",
	[13] = "SpellBook",
	[14] = "Sit",
	[15] = "Lie",
	[16] = "Insert",
	[17] = "Stand",
	[18] = "Lockpick",
	[19] = "StoryUseInInventory",
	[20] = "DisarmTrap",
	[22] = "ShowStoryElementUI",
	[23] = "Combine",
	[24] = "Ladder",
	[26] = "PlaySound",
	[27] = "SpawnCharacter",
	[28] = "Constrain",
	[30] = "Recipe",
	[31] = "Unknown31",
	[32] = "Throw",
	[33] = "LearnSpell",
	[34] = "Unknown34",
	[35] = "Unknown35",
}

--- @enum AdvantageBoostType
Ext_Enums.AdvantageBoostType = {
	None = 0,
	Advantage = 1,
	Disadvantage = 2,
	[0] = "None",
	[1] = "Advantage",
	[2] = "Disadvantage",
}

--- @enum AdvantageContext
Ext_Enums.AdvantageContext = {
	AttackRoll = 0,
	AttackTarget = 1,
	SavingThrow = 2,
	AllSavingThrows = 3,
	Ability = 4,
	AllAbilities = 5,
	Skill = 6,
	AllSkills = 7,
	SourceDialogue = 8,
	DeathSavingThrow = 9,
	Concentration = 10,
	[0] = "AttackRoll",
	[1] = "AttackTarget",
	[2] = "SavingThrow",
	[3] = "AllSavingThrows",
	[4] = "Ability",
	[5] = "AllAbilities",
	[6] = "Skill",
	[7] = "AllSkills",
	[8] = "SourceDialogue",
	[9] = "DeathSavingThrow",
	[10] = "Concentration",
}

--- @enum AiBaseFlags
Ext_Enums.AiBaseFlags = {
	Blocker = 1,
	WalkBlock = 2,
	WalkBlock2 = 4,
	ShootBlock = 8,
	WalkBlockCharacter = 16,
	ShootBlockCharacter = 32,
	SubgridEdge = 64,
	WalkBlockItem = 128,
	ShootBlockItem = 256,
	GroundSurfaceBlock = 512,
	CloudSurfaceBlock = 1024,
	UnknownBlock1 = 2048,
	UnknownBlock2 = 4096,
	UnknownBlock3 = 8192,
	UnknownBlock4 = 16384,
	UnknownBlock5 = 32768,
	Indestructible = 65536,
	CanClimbOn = 131072,
	Trap = 262144,
	Portal = 524288,
	PortalTarget = 1048576,
	Door = 4194304,
	DoorItemBlock = 8388608,
	[1] = "Blocker",
	[2] = "WalkBlock",
	[4] = "WalkBlock2",
	[8] = "ShootBlock",
	[16] = "WalkBlockCharacter",
	[32] = "ShootBlockCharacter",
	[64] = "SubgridEdge",
	[128] = "WalkBlockItem",
	[256] = "ShootBlockItem",
	[512] = "GroundSurfaceBlock",
	[1024] = "CloudSurfaceBlock",
	[2048] = "UnknownBlock1",
	[4096] = "UnknownBlock2",
	[8192] = "UnknownBlock3",
	[16384] = "UnknownBlock4",
	[32768] = "UnknownBlock5",
	[65536] = "Indestructible",
	[131072] = "CanClimbOn",
	[262144] = "Trap",
	[524288] = "Portal",
	[1048576] = "PortalTarget",
	[4194304] = "Door",
	[8388608] = "DoorItemBlock",
}

--- @enum AnimationSetAnimationFlags
Ext_Enums.AnimationSetAnimationFlags = {
	AlwaysIgnore = 1,
	NoFallback = 2,
	[1] = "AlwaysIgnore",
	[2] = "NoFallback",
}

--- @enum AppliedMaterialFlags
Ext_Enums.AppliedMaterialFlags = {
	Instantiated = 1,
	IsOverlay = 2,
	OverlayOriginalMapsSet = 4,
	[1] = "Instantiated",
	[2] = "IsOverlay",
	[4] = "OverlayOriginalMapsSet",
}

--- @enum ArmorSetState
Ext_Enums.ArmorSetState = {
	Normal = 0,
	Vanity = 1,
	[0] = "Normal",
	[1] = "Vanity",
}

--- @enum ArmorType
Ext_Enums.ArmorType = {
	None = 0,
	Cloth = 1,
	Padded = 2,
	Leather = 3,
	StuddedLeather = 4,
	Hide = 5,
	ChainShirt = 6,
	ScaleMail = 7,
	BreastPlate = 8,
	HalfPlate = 9,
	RingMail = 10,
	ChainMail = 11,
	Splint = 12,
	Plate = 13,
	Sentinel = 14,
	[0] = "None",
	[1] = "Cloth",
	[2] = "Padded",
	[3] = "Leather",
	[4] = "StuddedLeather",
	[5] = "Hide",
	[6] = "ChainShirt",
	[7] = "ScaleMail",
	[8] = "BreastPlate",
	[9] = "HalfPlate",
	[10] = "RingMail",
	[11] = "ChainMail",
	[12] = "Splint",
	[13] = "Plate",
	[14] = "Sentinel",
}

--- @enum AttackFlags
Ext_Enums.AttackFlags = {
	InstantKill = 1,
	FirstAttack = 2,
	FromSneak = 4,
	Reaction = 8,
	LastAttack = 16,
	DamagedAfterMiss = 32,
	[1] = "InstantKill",
	[2] = "FirstAttack",
	[4] = "FromSneak",
	[8] = "Reaction",
	[16] = "LastAttack",
	[32] = "DamagedAfterMiss",
}

--- @enum AttackRoll
Ext_Enums.AttackRoll = {
	None = 0,
	Strength = 1,
	Dexterity = 2,
	Constitution = 3,
	Intelligence = 4,
	Wisdom = 5,
	Charisma = 6,
	SpellCastingAbility = 7,
	UnarmedAttackAbility = 8,
	WeaponAttackAbility = 9,
	[0] = "None",
	[1] = "Strength",
	[2] = "Dexterity",
	[3] = "Constitution",
	[4] = "Intelligence",
	[5] = "Wisdom",
	[6] = "Charisma",
	[7] = "SpellCastingAbility",
	[8] = "UnarmedAttackAbility",
	[9] = "WeaponAttackAbility",
}

--- @enum AttributeFlags
Ext_Enums.AttributeFlags = {
	SlippingImmunity = 1,
	Torch = 2,
	Arrow = 4,
	Unbreakable = 8,
	Unrepairable = 16,
	Unstorable = 32,
	Grounded = 64,
	Floating = 128,
	InventoryBound = 256,
	IgnoreClouds = 512,
	LootableWhenEquipped = 1024,
	PickpocketableWhenEquipped = 2048,
	LoseDurabilityOnCharacterHit = 4096,
	ThrownImmunity = 8192,
	InvisibilityImmunity = 16384,
	InvulnerableAndInteractive = 32768,
	Backstab = 65536,
	BackstabImmunity = 131072,
	EnableObscurityEvents = 262144,
	ObscurityWithoutSneaking = 524288,
	FloatingWhileMoving = 1048576,
	ForceMainhandAlternativeEquipBones = 2097152,
	UseMusicalInstrumentForCasting = 4194304,
	[1] = "SlippingImmunity",
	[2] = "Torch",
	[4] = "Arrow",
	[8] = "Unbreakable",
	[16] = "Unrepairable",
	[32] = "Unstorable",
	[64] = "Grounded",
	[128] = "Floating",
	[256] = "InventoryBound",
	[512] = "IgnoreClouds",
	[1024] = "LootableWhenEquipped",
	[2048] = "PickpocketableWhenEquipped",
	[4096] = "LoseDurabilityOnCharacterHit",
	[8192] = "ThrownImmunity",
	[16384] = "InvisibilityImmunity",
	[32768] = "InvulnerableAndInteractive",
	[65536] = "Backstab",
	[131072] = "BackstabImmunity",
	[262144] = "EnableObscurityEvents",
	[524288] = "ObscurityWithoutSneaking",
	[1048576] = "FloatingWhileMoving",
	[2097152] = "ForceMainhandAlternativeEquipBones",
	[4194304] = "UseMusicalInstrumentForCasting",
}

--- @enum AudioCodec
Ext_Enums.AudioCodec = {
	Bank = 0,
	PCM = 1,
	ADPCM = 2,
	XMA = 3,
	Vorbis = 4,
	PCMEX = 7,
	External = 8,
	OpusNX = 17,
	Opus = 19,
	OpusWEM = 20,
	[0] = "Bank",
	[1] = "PCM",
	[2] = "ADPCM",
	[3] = "XMA",
	[4] = "Vorbis",
	[7] = "PCMEX",
	[8] = "External",
	[17] = "OpusNX",
	[19] = "Opus",
	[20] = "OpusWEM",
}

--- @enum BoostSourceType
Ext_Enums.BoostSourceType = {
	Undefined = 0,
	Item = 1,
	Status = 2,
	Passive = 3,
	Story = 4,
	Console = 5,
	Progression = 6,
	Base = 7,
	CharacterCreation = 8,
	Shapeshift = 9,
	Action = 10,
	[0] = "Undefined",
	[1] = "Item",
	[2] = "Status",
	[3] = "Passive",
	[4] = "Story",
	[5] = "Console",
	[6] = "Progression",
	[7] = "Base",
	[8] = "CharacterCreation",
	[9] = "Shapeshift",
	[10] = "Action",
}

--- @enum BoostType
Ext_Enums.BoostType = {
	AC = 0,
	Ability = 1,
	RollBonus = 2,
	Advantage = 3,
	ActionResource = 4,
	CriticalHit = 5,
	AbilityFailedSavingThrow = 6,
	Resistance = 7,
	WeaponDamageResistance = 8,
	ProficiencyBonusOverride = 9,
	ActionResourceOverride = 10,
	AddProficiencyToAC = 11,
	JumpMaxDistanceMultiplier = 12,
	AddProficiencyToDamage = 13,
	ActionResourceConsumeMultiplier = 14,
	BlockVerbalComponent = 15,
	BlockSomaticComponent = 16,
	HalveWeaponDamage = 17,
	UnlockSpell = 19,
	SourceAdvantageOnAttack = 20,
	ProficiencyBonus = 21,
	BlockSpellCast = 22,
	Proficiency = 23,
	SourceAllyAdvantageOnAttack = 24,
	IncreaseMaxHP = 25,
	ActionResourceBlock = 26,
	StatusImmunity = 27,
	UseBoosts = 28,
	CannotHarmCauseEntity = 29,
	TemporaryHP = 30,
	Weight = 31,
	WeightCategory = 32,
	FactionOverride = 33,
	ActionResourceMultiplier = 34,
	BlockRegainHP = 35,
	Initiative = 36,
	DarkvisionRange = 37,
	DarkvisionRangeMin = 38,
	DarkvisionRangeOverride = 39,
	Tag = 40,
	IgnoreDamageThreshold = 41,
	Skill = 42,
	WeaponDamage = 43,
	NullifyAbilityScore = 44,
	IgnoreFallDamage = 45,
	Reroll = 46,
	DownedStatus = 47,
	Invulnerable = 48,
	WeaponEnchantment = 49,
	GuaranteedChanceRollOutcome = 50,
	Attribute = 51,
	IgnoreLeaveAttackRange = 52,
	GameplayLight = 53,
	DialogueBlock = 54,
	DualWielding = 55,
	Savant = 56,
	MinimumRollResult = 57,
	Lootable = 58,
	CharacterWeaponDamage = 59,
	ProjectileDeflect = 60,
	AbilityOverrideMinimum = 61,
	ACOverrideFormula = 62,
	FallDamageMultiplier = 63,
	ActiveCharacterLight = 64,
	Invisibility = 65,
	TwoWeaponFighting = 66,
	WeaponAttackTypeOverride = 67,
	WeaponDamageDieOverride = 68,
	CarryCapacityMultiplier = 69,
	WeaponProperty = 70,
	WeaponAttackRollAbilityOverride = 71,
	BlockTravel = 72,
	BlockGatherAtCamp = 73,
	BlockAbilityModifierDamageBonus = 74,
	VoicebarkBlock = 75,
	HiddenDuringCinematic = 76,
	SightRangeAdditive = 77,
	SightRangeMinimum = 78,
	SightRangeMaximum = 79,
	SightRangeOverride = 80,
	CannotBeDisarmed = 81,
	MovementSpeedLimit = 82,
	NonLethal = 83,
	UnlockSpellVariant = 84,
	DetectDisturbancesBlock = 85,
	BlockAbilityModifierFromAC = 86,
	ScaleMultiplier = 87,
	CriticalDamageOnHit = 88,
	DamageReduction = 89,
	ReduceCriticalAttackThreshold = 90,
	PhysicalForceRangeBonus = 91,
	ObjectSize = 92,
	ObjectSizeOverride = 93,
	ItemReturnToOwner = 94,
	AiArchetypeOverride = 95,
	ExpertiseBonus = 96,
	EntityThrowDamage = 97,
	WeaponDamageTypeOverride = 98,
	MaximizeHealing = 99,
	IgnoreEnterAttackRange = 100,
	DamageBonus = 101,
	Detach = 102,
	ConsumeItemBlock = 103,
	AdvanceSpells = 104,
	SpellResistance = 105,
	WeaponAttackRollBonus = 106,
	SpellSaveDC = 107,
	RedirectDamage = 108,
	CanSeeThrough = 109,
	CanShootThrough = 110,
	CanWalkThrough = 111,
	MonkWeaponAttackOverride = 112,
	MonkWeaponDamageDiceOverride = 113,
	IntrinsicSummonerProficiency = 114,
	HorizontalFOVOverride = 115,
	CharacterUnarmedDamage = 116,
	UnarmedMagicalProperty = 117,
	ActionResourceReplenishTypeOverride = 118,
	AreaDamageEvade = 119,
	ActionResourcePreventReduction = 120,
	AttackSpellOverride = 121,
	Lock = 122,
	NoAOEDamageOnLand = 123,
	IgnorePointBlankDisadvantage = 124,
	CriticalHitExtraDice = 125,
	DodgeAttackRoll = 126,
	GameplayObscurity = 127,
	MaximumRollResult = 128,
	UnlockInterrupt = 129,
	IntrinsicSourceProficiency = 130,
	JumpMaxDistanceBonus = 131,
	ArmorAbilityModifierCapOverride = 132,
	IgnoreResistance = 133,
	ConcentrationIgnoreDamage = 134,
	LeaveTriggers = 135,
	IgnoreLowGroundPenalty = 136,
	IgnoreSurfaceCover = 137,
	EnableBasicItemInteractions = 138,
	SoundsBlocked = 139,
	ProficiencyBonusIncrease = 140,
	NoDamageOnThrown = 141,
	DamageTakenBonus = 142,
	ReceivingCriticalDamageOnHit = 143,
	[0] = "AC",
	[1] = "Ability",
	[2] = "RollBonus",
	[3] = "Advantage",
	[4] = "ActionResource",
	[5] = "CriticalHit",
	[6] = "AbilityFailedSavingThrow",
	[7] = "Resistance",
	[8] = "WeaponDamageResistance",
	[9] = "ProficiencyBonusOverride",
	[10] = "ActionResourceOverride",
	[11] = "AddProficiencyToAC",
	[12] = "JumpMaxDistanceMultiplier",
	[13] = "AddProficiencyToDamage",
	[14] = "ActionResourceConsumeMultiplier",
	[15] = "BlockVerbalComponent",
	[16] = "BlockSomaticComponent",
	[17] = "HalveWeaponDamage",
	[19] = "UnlockSpell",
	[20] = "SourceAdvantageOnAttack",
	[21] = "ProficiencyBonus",
	[22] = "BlockSpellCast",
	[23] = "Proficiency",
	[24] = "SourceAllyAdvantageOnAttack",
	[25] = "IncreaseMaxHP",
	[26] = "ActionResourceBlock",
	[27] = "StatusImmunity",
	[28] = "UseBoosts",
	[29] = "CannotHarmCauseEntity",
	[30] = "TemporaryHP",
	[31] = "Weight",
	[32] = "WeightCategory",
	[33] = "FactionOverride",
	[34] = "ActionResourceMultiplier",
	[35] = "BlockRegainHP",
	[36] = "Initiative",
	[37] = "DarkvisionRange",
	[38] = "DarkvisionRangeMin",
	[39] = "DarkvisionRangeOverride",
	[40] = "Tag",
	[41] = "IgnoreDamageThreshold",
	[42] = "Skill",
	[43] = "WeaponDamage",
	[44] = "NullifyAbilityScore",
	[45] = "IgnoreFallDamage",
	[46] = "Reroll",
	[47] = "DownedStatus",
	[48] = "Invulnerable",
	[49] = "WeaponEnchantment",
	[50] = "GuaranteedChanceRollOutcome",
	[51] = "Attribute",
	[52] = "IgnoreLeaveAttackRange",
	[53] = "GameplayLight",
	[54] = "DialogueBlock",
	[55] = "DualWielding",
	[56] = "Savant",
	[57] = "MinimumRollResult",
	[58] = "Lootable",
	[59] = "CharacterWeaponDamage",
	[60] = "ProjectileDeflect",
	[61] = "AbilityOverrideMinimum",
	[62] = "ACOverrideFormula",
	[63] = "FallDamageMultiplier",
	[64] = "ActiveCharacterLight",
	[65] = "Invisibility",
	[66] = "TwoWeaponFighting",
	[67] = "WeaponAttackTypeOverride",
	[68] = "WeaponDamageDieOverride",
	[69] = "CarryCapacityMultiplier",
	[70] = "WeaponProperty",
	[71] = "WeaponAttackRollAbilityOverride",
	[72] = "BlockTravel",
	[73] = "BlockGatherAtCamp",
	[74] = "BlockAbilityModifierDamageBonus",
	[75] = "VoicebarkBlock",
	[76] = "HiddenDuringCinematic",
	[77] = "SightRangeAdditive",
	[78] = "SightRangeMinimum",
	[79] = "SightRangeMaximum",
	[80] = "SightRangeOverride",
	[81] = "CannotBeDisarmed",
	[82] = "MovementSpeedLimit",
	[83] = "NonLethal",
	[84] = "UnlockSpellVariant",
	[85] = "DetectDisturbancesBlock",
	[86] = "BlockAbilityModifierFromAC",
	[87] = "ScaleMultiplier",
	[88] = "CriticalDamageOnHit",
	[89] = "DamageReduction",
	[90] = "ReduceCriticalAttackThreshold",
	[91] = "PhysicalForceRangeBonus",
	[92] = "ObjectSize",
	[93] = "ObjectSizeOverride",
	[94] = "ItemReturnToOwner",
	[95] = "AiArchetypeOverride",
	[96] = "ExpertiseBonus",
	[97] = "EntityThrowDamage",
	[98] = "WeaponDamageTypeOverride",
	[99] = "MaximizeHealing",
	[100] = "IgnoreEnterAttackRange",
	[101] = "DamageBonus",
	[102] = "Detach",
	[103] = "ConsumeItemBlock",
	[104] = "AdvanceSpells",
	[105] = "SpellResistance",
	[106] = "WeaponAttackRollBonus",
	[107] = "SpellSaveDC",
	[108] = "RedirectDamage",
	[109] = "CanSeeThrough",
	[110] = "CanShootThrough",
	[111] = "CanWalkThrough",
	[112] = "MonkWeaponAttackOverride",
	[113] = "MonkWeaponDamageDiceOverride",
	[114] = "IntrinsicSummonerProficiency",
	[115] = "HorizontalFOVOverride",
	[116] = "CharacterUnarmedDamage",
	[117] = "UnarmedMagicalProperty",
	[118] = "ActionResourceReplenishTypeOverride",
	[119] = "AreaDamageEvade",
	[120] = "ActionResourcePreventReduction",
	[121] = "AttackSpellOverride",
	[122] = "Lock",
	[123] = "NoAOEDamageOnLand",
	[124] = "IgnorePointBlankDisadvantage",
	[125] = "CriticalHitExtraDice",
	[126] = "DodgeAttackRoll",
	[127] = "GameplayObscurity",
	[128] = "MaximumRollResult",
	[129] = "UnlockInterrupt",
	[130] = "IntrinsicSourceProficiency",
	[131] = "JumpMaxDistanceBonus",
	[132] = "ArmorAbilityModifierCapOverride",
	[133] = "IgnoreResistance",
	[134] = "ConcentrationIgnoreDamage",
	[135] = "LeaveTriggers",
	[136] = "IgnoreLowGroundPenalty",
	[137] = "IgnoreSurfaceCover",
	[138] = "EnableBasicItemInteractions",
	[139] = "SoundsBlocked",
	[140] = "ProficiencyBonusIncrease",
	[141] = "NoDamageOnThrown",
	[142] = "DamageTakenBonus",
	[143] = "ReceivingCriticalDamageOnHit",
}

--- @enum BoundBaseFlags
Ext_Enums.BoundBaseFlags = {
	UseTranslateOverride = 1,
	UseOnDistance = 2,
	[1] = "UseTranslateOverride",
	[2] = "UseOnDistance",
}

--- @enum BoundFlags
Ext_Enums.BoundFlags = {
	ItemWalkThroughBlock = 1,
	CharacterWalkThroughBlock = 2,
	ItemSeeThroughBlock = 4,
	CharacterSeeThroughBlock = 8,
	ItemShootThroughBlock = 16,
	CharacterShootThroughBlock = 32,
	Interactable = 64,
	InvisibleTrap = 128,
	Indestructible = 256,
	SurfaceBlocker = 512,
	SurfaceCloudBlocker = 1024,
	CanClimbOn = 2048,
	WalkOn = 4096,
	IsPlatformOwner = 8192,
	[1] = "ItemWalkThroughBlock",
	[2] = "CharacterWalkThroughBlock",
	[4] = "ItemSeeThroughBlock",
	[8] = "CharacterSeeThroughBlock",
	[16] = "ItemShootThroughBlock",
	[32] = "CharacterShootThroughBlock",
	[64] = "Interactable",
	[128] = "InvisibleTrap",
	[256] = "Indestructible",
	[512] = "SurfaceBlocker",
	[1024] = "SurfaceCloudBlocker",
	[2048] = "CanClimbOn",
	[4096] = "WalkOn",
	[8192] = "IsPlatformOwner",
}

--- @enum CanDoActionsFlags
Ext_Enums.CanDoActionsFlags = {
	CanCastSpells = 1,
	CanAttack = 2,
	CanDoFlag4 = 4,
	CanCastRituals = 8,
	CanPickpocket = 16,
	CanLockpick = 32,
	CanDisarmTrap = 64,
	[1] = "CanCastSpells",
	[2] = "CanAttack",
	[4] = "CanDoFlag4",
	[8] = "CanCastRituals",
	[16] = "CanPickpocket",
	[32] = "CanLockpick",
	[64] = "CanDisarmTrap",
}

--- @enum CanInteractFlags
Ext_Enums.CanInteractFlags = {
	CanInteract = 1,
	CanUsePortals = 2,
	CanUseLadder = 4,
	CanUseFlag8 = 8,
	[1] = "CanInteract",
	[2] = "CanUsePortals",
	[4] = "CanUseLadder",
	[8] = "CanUseFlag8",
}

--- @enum CanMoveFlags
Ext_Enums.CanMoveFlags = {
	CanMove = 1,
	CanBeTeleported = 2,
	CanFollow = 4,
	CanWorldClimb = 8,
	CanWorldDrop = 16,
	[1] = "CanMove",
	[2] = "CanBeTeleported",
	[4] = "CanFollow",
	[8] = "CanWorldClimb",
	[16] = "CanWorldDrop",
}

--- @enum CastRequirementFlags
Ext_Enums.CastRequirementFlags = {
	Somatic = 1,
	Speech = 2,
	ActionResources = 4,
	SpellCast = 16,
	WeaponRequirements = 32,
	Shield = 64,
	SpellPrepared = 128,
	ItemCharges = 256,
	EquipmentProficiency = 512,
	Cooldowns = 1024,
	StatsConditions = 2048,
	Range = 4096,
	[1] = "Somatic",
	[2] = "Speech",
	[4] = "ActionResources",
	[16] = "SpellCast",
	[32] = "WeaponRequirements",
	[64] = "Shield",
	[128] = "SpellPrepared",
	[256] = "ItemCharges",
	[512] = "EquipmentProficiency",
	[1024] = "Cooldowns",
	[2048] = "StatsConditions",
	[4096] = "Range",
}

--- @enum CauseType
Ext_Enums.CauseType = {
	None = 0,
	SurfaceMove = 1,
	SurfaceCreate = 2,
	SurfaceStatus = 3,
	StatusEnter = 4,
	StatusTick = 5,
	Attack = 6,
	Offhand = 7,
	AURA = 8,
	InventoryItem = 9,
	WorldItemThrow = 10,
	Unknown11 = 11,
	[0] = "None",
	[1] = "SurfaceMove",
	[2] = "SurfaceCreate",
	[3] = "SurfaceStatus",
	[4] = "StatusEnter",
	[5] = "StatusTick",
	[6] = "Attack",
	[7] = "Offhand",
	[8] = "AURA",
	[9] = "InventoryItem",
	[10] = "WorldItemThrow",
	[11] = "Unknown11",
}

--- @enum ClientCharacterFlags
Ext_Enums.ClientCharacterFlags = {
	IsPlayer = 1,
	Active = 4,
	InDialog = 64,
	Climbing = 512,
	HasCharacterLight = 8192,
	VisibleOffStage = 67108864,
	Airborne = 1125899906842624,
	[1] = "IsPlayer",
	[4] = "Active",
	[64] = "InDialog",
	[512] = "Climbing",
	[8192] = "HasCharacterLight",
	[67108864] = "VisibleOffStage",
	[1125899906842624] = "Airborne",
}

--- @enum ClientCharacterFlags2
Ext_Enums.ClientCharacterFlags2 = {
	PickpocketRequested = 2,
	Pickpocketing = 4,
	LootingRequested = 8,
	CustomVisuals = 16,
	ReloadingVisuals = 32,
	RemovingFromLevel = 128,
	[2] = "PickpocketRequested",
	[4] = "Pickpocketing",
	[8] = "LootingRequested",
	[16] = "CustomVisuals",
	[32] = "ReloadingVisuals",
	[128] = "RemovingFromLevel",
}

--- @enum ClientCharacterFlags3
Ext_Enums.ClientCharacterFlags3 = {
	HasDialog = 1,
	WasInputControllerActive = 2,
	HasCustomVisualAfterDeath = 16,
	CanTrade = 32,
	[1] = "HasDialog",
	[2] = "WasInputControllerActive",
	[16] = "HasCustomVisualAfterDeath",
	[32] = "CanTrade",
}

--- @enum ClientCharacterTaskFlags
Ext_Enums.ClientCharacterTaskFlags = {
	IsExecuting = 1,
	OverrideCursorOnHover = 2,
	IsDummyTask = 4,
	IgnoreAP = 8,
	CannotActivateSameType = 16,
	CannotPreviewSameType = 32,
	SupportsPicking = 64,
	IgnorePreviewCancel = 128,
	NeedsPreviewToActivate = 256,
	NeedsTaskToActivate = 512,
	CombatTargeting = 4096,
	CanInterruptRunningTask = 16384,
	CanReenterPreview = 32768,
	ConditionalInterruptByPreview = 65536,
	ConditionalInterruptRunningTask = 131072,
	PreviewingDifferentTask = 262144,
	AlwaysInterruptRunningTask = 524288,
	[1] = "IsExecuting",
	[2] = "OverrideCursorOnHover",
	[4] = "IsDummyTask",
	[8] = "IgnoreAP",
	[16] = "CannotActivateSameType",
	[32] = "CannotPreviewSameType",
	[64] = "SupportsPicking",
	[128] = "IgnorePreviewCancel",
	[256] = "NeedsPreviewToActivate",
	[512] = "NeedsTaskToActivate",
	[4096] = "CombatTargeting",
	[16384] = "CanInterruptRunningTask",
	[32768] = "CanReenterPreview",
	[65536] = "ConditionalInterruptByPreview",
	[131072] = "ConditionalInterruptRunningTask",
	[262144] = "PreviewingDifferentTask",
	[524288] = "AlwaysInterruptRunningTask",
}

--- @enum ClientCharacterTaskType
Ext_Enums.ClientCharacterTaskType = {
	MoveTo = 0,
	ControllerSelection = 1,
	MoveController = 2,
	MoveInDirection = 3,
	Spell = 4,
	Dialog = 5,
	ItemUseRemotely = 6,
	ItemUse = 7,
	Drop = 8,
	ItemInteractArea = 9,
	ItemMove = 10,
	ItemMoveDummy = 11,
	ItemCombine = 12,
	ItemCombineDummy = 13,
	SheathUnsheath = 14,
	Lockpick = 15,
	DisarmTrap = 16,
	Pickpocket = 17,
	PickUp = 18,
	Listen = 19,
	Loot = 20,
	DefaultTargetAction = 22,
	ClimbTo = 23,
	[0] = "MoveTo",
	[1] = "ControllerSelection",
	[2] = "MoveController",
	[3] = "MoveInDirection",
	[4] = "Spell",
	[5] = "Dialog",
	[6] = "ItemUseRemotely",
	[7] = "ItemUse",
	[8] = "Drop",
	[9] = "ItemInteractArea",
	[10] = "ItemMove",
	[11] = "ItemMoveDummy",
	[12] = "ItemCombine",
	[13] = "ItemCombineDummy",
	[14] = "SheathUnsheath",
	[15] = "Lockpick",
	[16] = "DisarmTrap",
	[17] = "Pickpocket",
	[18] = "PickUp",
	[19] = "Listen",
	[20] = "Loot",
	[22] = "DefaultTargetAction",
	[23] = "ClimbTo",
}

--- @enum ClientGameState
Ext_Enums.ClientGameState = {
	Unknown = 0,
	Init = 1,
	InitMenu = 2,
	InitNetwork = 3,
	InitConnection = 4,
	Idle = 5,
	LoadMenu = 6,
	Menu = 7,
	Exit = 8,
	SwapLevel = 9,
	LoadLevel = 10,
	LoadModule = 11,
	LoadSession = 12,
	UnloadLevel = 13,
	UnloadModule = 14,
	UnloadSession = 15,
	Paused = 16,
	PrepareRunning = 17,
	Running = 18,
	Disconnect = 19,
	Join = 20,
	Save = 21,
	StartLoading = 22,
	StopLoading = 23,
	StartServer = 24,
	Movie = 25,
	Installation = 26,
	ModReceiving = 27,
	Lobby = 28,
	BuildStory = 29,
	GeneratePsoCache = 32,
	LoadPsoCache = 33,
	AnalyticsSessionEnd = 34,
	[0] = "Unknown",
	[1] = "Init",
	[2] = "InitMenu",
	[3] = "InitNetwork",
	[4] = "InitConnection",
	[5] = "Idle",
	[6] = "LoadMenu",
	[7] = "Menu",
	[8] = "Exit",
	[9] = "SwapLevel",
	[10] = "LoadLevel",
	[11] = "LoadModule",
	[12] = "LoadSession",
	[13] = "UnloadLevel",
	[14] = "UnloadModule",
	[15] = "UnloadSession",
	[16] = "Paused",
	[17] = "PrepareRunning",
	[18] = "Running",
	[19] = "Disconnect",
	[20] = "Join",
	[21] = "Save",
	[22] = "StartLoading",
	[23] = "StopLoading",
	[24] = "StartServer",
	[25] = "Movie",
	[26] = "Installation",
	[27] = "ModReceiving",
	[28] = "Lobby",
	[29] = "BuildStory",
	[32] = "GeneratePsoCache",
	[33] = "LoadPsoCache",
	[34] = "AnalyticsSessionEnd",
}

--- @enum ClientInputControllerFlags
Ext_Enums.ClientInputControllerFlags = {
	ControllerMode = 1,
	[1] = "ControllerMode",
}

--- @enum ClientInputControllerRequestFlags
Ext_Enums.ClientInputControllerRequestFlags = {
	RequestClear = 1,
	RequestForceClear = 2,
	RequestUpdatePreview = 4,
	RequestCancelMovement = 8,
	[1] = "RequestClear",
	[2] = "RequestForceClear",
	[4] = "RequestUpdatePreview",
	[8] = "RequestCancelMovement",
}

--- @enum ClientStatusFlags
Ext_Enums.ClientStatusFlags = {
	RequestDelete = 1,
	Started = 2,
	WasEnteredDuringSave = 4,
	KeepAlive = 8,
	DontTickWhileOnSurface = 16,
	IsOnSourceSurface = 32,
	VisualsCreated = 64,
	HasSounds = 128,
	[1] = "RequestDelete",
	[2] = "Started",
	[4] = "WasEnteredDuringSave",
	[8] = "KeepAlive",
	[16] = "DontTickWhileOnSurface",
	[32] = "IsOnSourceSurface",
	[64] = "VisualsCreated",
	[128] = "HasSounds",
}

--- @enum ClientStatusFlags2
Ext_Enums.ClientStatusFlags2 = {
	HasLoopSound = 1,
	Ticked = 2,
	IsSourceDependent = 4,
	[1] = "HasLoopSound",
	[2] = "Ticked",
	[4] = "IsSourceDependent",
}

--- @enum CombatParticipantFlags
Ext_Enums.CombatParticipantFlags = {
	CanFight = 1,
	CanJoinCombat = 2,
	IsBoss = 4,
	IsInspector = 16,
	StayInAiHints = 512,
	[1] = "CanFight",
	[2] = "CanJoinCombat",
	[4] = "IsBoss",
	[16] = "IsInspector",
	[512] = "StayInAiHints",
}

--- @enum CombatPauseSourceType
Ext_Enums.CombatPauseSourceType = {
	Unknown = 0,
	Script = 1,
	[0] = "Unknown",
	[1] = "Script",
}

--- @enum ConditionRollType
Ext_Enums.ConditionRollType = {
	DifficultyRoll = 1,
	AttackRoll = 2,
	SavingThrowRoll = 4,
	SkillCheckRoll = 8,
	AbilityCheckRoll = 16,
	[1] = "DifficultyRoll",
	[2] = "AttackRoll",
	[4] = "SavingThrowRoll",
	[8] = "SkillCheckRoll",
	[16] = "AbilityCheckRoll",
}

--- @enum CriticalHitAttackFlags
Ext_Enums.CriticalHitAttackFlags = {
	AttackTarget = 1,
	AttackRoll = 2,
	[1] = "AttackTarget",
	[2] = "AttackRoll",
}

--- @enum CriticalHitSuccessFlags
Ext_Enums.CriticalHitSuccessFlags = {
	Success = 1,
	SuccessNever = 2,
	Failure = 4,
	FailureNever = 8,
	[1] = "Success",
	[2] = "SuccessNever",
	[4] = "Failure",
	[8] = "FailureNever",
}

--- @enum CursorType
Ext_Enums.CursorType = {
	None = 0,
	System = 1,
	Arrow = 2,
	ArrowWarning = 3,
	Bow = 4,
	BowWarning = 5,
	BowGround = 6,
	Wand = 7,
	WandWarning = 8,
	WandGround = 9,
	Backstab = 10,
	BackstabWarning = 11,
	Cast = 12,
	CastWarning = 13,
	Combine = 14,
	CombineWarning = 15,
	ItemMove = 20,
	ItemMoveWarning = 21,
	ItemUse = 22,
	ItemUseWarning = 23,
	ItemPickup = 24,
	ItemPickupWarning = 25,
	Lock = 26,
	LockWarning = 27,
	Melee = 28,
	MeleeWarning = 29,
	MeleeGround = 30,
	OpenContainer = 31,
	OpenContainerWarning = 32,
	OpenContainerNew = 33,
	OpenDoor = 34,
	OpenDoorWarning = 35,
	PickPocket = 36,
	PickPocketWarning = 37,
	Shovel = 40,
	ShovelWarning = 41,
	Talk = 42,
	TalkWarning = 43,
	Walk = 44,
	WalkWarning = 45,
	CameraRotation = 46,
	Listen = 47,
	ListenWarning = 48,
	InputText = 50,
	LootedContainer = 51,
	[0] = "None",
	[1] = "System",
	[2] = "Arrow",
	[3] = "ArrowWarning",
	[4] = "Bow",
	[5] = "BowWarning",
	[6] = "BowGround",
	[7] = "Wand",
	[8] = "WandWarning",
	[9] = "WandGround",
	[10] = "Backstab",
	[11] = "BackstabWarning",
	[12] = "Cast",
	[13] = "CastWarning",
	[14] = "Combine",
	[15] = "CombineWarning",
	[20] = "ItemMove",
	[21] = "ItemMoveWarning",
	[22] = "ItemUse",
	[23] = "ItemUseWarning",
	[24] = "ItemPickup",
	[25] = "ItemPickupWarning",
	[26] = "Lock",
	[27] = "LockWarning",
	[28] = "Melee",
	[29] = "MeleeWarning",
	[30] = "MeleeGround",
	[31] = "OpenContainer",
	[32] = "OpenContainerWarning",
	[33] = "OpenContainerNew",
	[34] = "OpenDoor",
	[35] = "OpenDoorWarning",
	[36] = "PickPocket",
	[37] = "PickPocketWarning",
	[40] = "Shovel",
	[41] = "ShovelWarning",
	[42] = "Talk",
	[43] = "TalkWarning",
	[44] = "Walk",
	[45] = "WalkWarning",
	[46] = "CameraRotation",
	[47] = "Listen",
	[48] = "ListenWarning",
	[50] = "InputText",
	[51] = "LootedContainer",
}

--- @enum DamageFlags
Ext_Enums.DamageFlags = {
	Hit = 1,
	Dodge = 2,
	Miss = 4,
	Critical = 8,
	Backstab = 16,
	Invisible = 32,
	Magical = 64,
	Invulnerable = 128,
	SavingThrow = 256,
	Projectile = 512,
	HitpointsDamaged = 1024,
	Surface = 2048,
	Status = 4096,
	AttackAdvantage = 8192,
	AttackDisadvantage = 16384,
	Calculated = 32768,
	KillingBlow = 65536,
	NonlethalCapped = 131072,
	[1] = "Hit",
	[2] = "Dodge",
	[4] = "Miss",
	[8] = "Critical",
	[16] = "Backstab",
	[32] = "Invisible",
	[64] = "Magical",
	[128] = "Invulnerable",
	[256] = "SavingThrow",
	[512] = "Projectile",
	[1024] = "HitpointsDamaged",
	[2048] = "Surface",
	[4096] = "Status",
	[8192] = "AttackAdvantage",
	[16384] = "AttackDisadvantage",
	[32768] = "Calculated",
	[65536] = "KillingBlow",
	[131072] = "NonlethalCapped",
}

--- @enum DamageType
Ext_Enums.DamageType = {
	None = 0,
	Slashing = 1,
	Piercing = 2,
	Bludgeoning = 3,
	Acid = 4,
	Thunder = 5,
	Necrotic = 6,
	Fire = 7,
	Lightning = 8,
	Cold = 9,
	Psychic = 10,
	Poison = 11,
	Radiant = 12,
	Force = 13,
	Sentinel = 14,
	[0] = "None",
	[1] = "Slashing",
	[2] = "Piercing",
	[3] = "Bludgeoning",
	[4] = "Acid",
	[5] = "Thunder",
	[6] = "Necrotic",
	[7] = "Fire",
	[8] = "Lightning",
	[9] = "Cold",
	[10] = "Psychic",
	[11] = "Poison",
	[12] = "Radiant",
	[13] = "Force",
	[14] = "Sentinel",
}

--- @enum DealDamageWeaponDamageType
Ext_Enums.DealDamageWeaponDamageType = {
	None = 0,
	MainWeaponDamageType = 1,
	OffhandWeaponDamageType = 2,
	MainMeleeWeaponDamageType = 3,
	OffhandMeleeWeaponDamageType = 4,
	MainRangedWeaponDamageType = 5,
	OffhandRangedWeaponDamageType = 6,
	SourceWeaponDamageType = 7,
	ThrownWeaponDamageType = 8,
	[0] = "None",
	[1] = "MainWeaponDamageType",
	[2] = "OffhandWeaponDamageType",
	[3] = "MainMeleeWeaponDamageType",
	[4] = "OffhandMeleeWeaponDamageType",
	[5] = "MainRangedWeaponDamageType",
	[6] = "OffhandRangedWeaponDamageType",
	[7] = "SourceWeaponDamageType",
	[8] = "ThrownWeaponDamageType",
}

--- @enum DealDamageWeaponType
Ext_Enums.DealDamageWeaponType = {
	None = 0,
	MainWeapon = 1,
	OffhandWeapon = 2,
	MainMeleeWeapon = 3,
	OffhandMeleeWeapon = 4,
	MainRangedWeapon = 5,
	OffhandRangedWeapon = 6,
	SourceWeapon = 7,
	UnarmedDamage = 8,
	ThrownWeapon = 9,
	ImprovisedWeapon = 10,
	[0] = "None",
	[1] = "MainWeapon",
	[2] = "OffhandWeapon",
	[3] = "MainMeleeWeapon",
	[4] = "OffhandMeleeWeapon",
	[5] = "MainRangedWeapon",
	[6] = "OffhandRangedWeapon",
	[7] = "SourceWeapon",
	[8] = "UnarmedDamage",
	[9] = "ThrownWeapon",
	[10] = "ImprovisedWeapon",
}

--- @enum DialogNodeFlags
Ext_Enums.DialogNodeFlags = {
	IsEndNode = 1,
	IsOptional = 2,
	IsExclusive = 4,
	ShowOnlyOnce = 8,
	IsGameplayNode = 16,
	IsRootNode = 32,
	SuppressSubtitle = 64,
	IsGreetingNode = 128,
	AllowGrouping = 256,
	IsEndAllDialogs = 512,
	[1] = "IsEndNode",
	[2] = "IsOptional",
	[4] = "IsExclusive",
	[8] = "ShowOnlyOnce",
	[16] = "IsGameplayNode",
	[32] = "IsRootNode",
	[64] = "SuppressSubtitle",
	[128] = "IsGreetingNode",
	[256] = "AllowGrouping",
	[512] = "IsEndAllDialogs",
}

--- @enum DiceSizeId
Ext_Enums.DiceSizeId = {
	D4 = 0,
	D6 = 1,
	D8 = 2,
	D10 = 3,
	D12 = 4,
	D20 = 5,
	D100 = 6,
	Default = 8,
	[0] = "D4",
	[1] = "D6",
	[2] = "D8",
	[3] = "D10",
	[4] = "D12",
	[5] = "D20",
	[6] = "D100",
	[8] = "Default",
}

--- @enum DynamicModifierType
Ext_Enums.DynamicModifierType = {
	None = 0,
	Consumable = 1,
	ItemSpell = 2,
	Spell = 3,
	ToggledPassive = 4,
	[0] = "None",
	[1] = "Consumable",
	[2] = "ItemSpell",
	[3] = "Spell",
	[4] = "ToggledPassive",
}

--- @enum ECSComponentChangeFlags
Ext_Enums.ECSComponentChangeFlags = {
	Create = 1,
	Destroy = 2,
	Replicate = 4,
	OneFrame = 8,
	ReplicatedComponent = 16,
	[1] = "Create",
	[2] = "Destroy",
	[4] = "Replicate",
	[8] = "OneFrame",
	[16] = "ReplicatedComponent",
}

--- @enum ECSEntityChangeFlags
Ext_Enums.ECSEntityChangeFlags = {
	Create = 1,
	Destroy = 2,
	Immediate = 4,
	Dead = 8,
	Ignore = 16,
	[1] = "Create",
	[2] = "Destroy",
	[4] = "Immediate",
	[8] = "Dead",
	[16] = "Ignore",
}

--- @enum EEquipmentType
Ext_Enums.EEquipmentType = {
	Weapon = 0,
	Unknown1 = 1,
	Shield = 2,
	Armor = 3,
	Sentinel = 4,
	[0] = "Weapon",
	[1] = "Unknown1",
	[2] = "Shield",
	[3] = "Armor",
	[4] = "Sentinel",
}

--- @enum EffectFlags
Ext_Enums.EffectFlags = {
	Forget = 1,
	Pooled = 2,
	Active = 4,
	FullyInitialized = 8,
	Playing = 16,
	Stopped = 32,
	FullyInitialized2 = 512,
	Interrupted = 1024,
	[1] = "Forget",
	[2] = "Pooled",
	[4] = "Active",
	[8] = "FullyInitialized",
	[16] = "Playing",
	[32] = "Stopped",
	[512] = "FullyInitialized2",
	[1024] = "Interrupted",
}

--- @enum EffectPropertyType
Ext_Enums.EffectPropertyType = {
	Boolean = 0,
	Integer = 1,
	IntegerRange = 2,
	ColorARGBKeyFrame = 3,
	Float = 4,
	FloatRange = 5,
	FloatKeyFrame = 6,
	String = 7,
	Vector3 = 8,
	FixedFunction = 9,
	FixedString = 10,
	Base = 11,
	[0] = "Boolean",
	[1] = "Integer",
	[2] = "IntegerRange",
	[3] = "ColorARGBKeyFrame",
	[4] = "Float",
	[5] = "FloatRange",
	[6] = "FloatKeyFrame",
	[7] = "String",
	[8] = "Vector3",
	[9] = "FixedFunction",
	[10] = "FixedString",
	[11] = "Base",
}

--- @enum EquipmentStatsType
Ext_Enums.EquipmentStatsType = {
	Weapon = 0,
	Armor = 1,
	Shield = 2,
	[0] = "Weapon",
	[1] = "Armor",
	[2] = "Shield",
}

--- @enum ExecuteWeaponFunctorsType
Ext_Enums.ExecuteWeaponFunctorsType = {
	MainHand = 0,
	OffHand = 1,
	BothHands = 2,
	Undefined = 3,
	[0] = "MainHand",
	[1] = "OffHand",
	[2] = "BothHands",
	[3] = "Undefined",
}

--- @enum ExtComponentType
Ext_Enums.ExtComponentType = {
	ActionResources = 0,
	ActionResourceSpendEvent = 1,
	ActionResourceChangeResults = 2,
	ActionResourceEvents = 3,
	Armor = 4,
	ArmorSetState = 5,
	BaseHp = 6,
	Bound = 7,
	IsCharacter = 8,
	Data = 9,
	Detached = 10,
	Health = 11,
	Resistances = 12,
	Hearing = 13,
	SpellBook = 14,
	Stats = 15,
	StatusImmunities = 16,
	SurfacePathInfluences = 17,
	Use = 18,
	Value = 19,
	Weapon = 20,
	Wielding = 21,
	CustomStats = 22,
	ActionResourceConsumeMultiplierBoost = 23,
	Tag = 24,
	SpellBookPrepares = 25,
	Transform = 26,
	TranslateChanged = 27,
	TranslateChangedEvent = 28,
	Relation = 29,
	Faction = 30,
	CanInteract = 31,
	CanSpeak = 32,
	Origin = 33,
	Level = 34,
	EocLevel = 35,
	Classes = 36,
	MaterialParameterOverride = 37,
	OffStage = 38,
	PickingState = 39,
	Player = 40,
	SimpleCharacter = 41,
	Speaker = 42,
	WeaponSet = 43,
	DifficultyCheck = 44,
	AttributeFlags = 45,
	BodyType = 46,
	ObjectSize = 47,
	BaseStats = 48,
	Expertise = 49,
	HealBlock = 50,
	Ruleset = 51,
	RulesetModifiers = 52,
	TimeFactor = 53,
	Clock = 54,
	Experience = 55,
	AvailableLevel = 56,
	CanLevelUp = 57,
	BoostCondition = 58,
	BoostsContainer = 59,
	BoostInfo = 60,
	ServerBoostBase = 61,
	ServerStatusBoostsProcessed = 62,
	BoostChangedEvent = 63,
	BoostChangedEventsSingleton = 64,
	BoostConditionalState = 65,
	BoostProvider = 66,
	StatusBoostsRefreshed = 67,
	BoostBaseUpdated = 68,
	ServerBaseData = 69,
	ServerBaseSize = 70,
	ServerBaseStats = 71,
	ServerBaseWeapon = 72,
	ServerBaseProficiency = 73,
	ServerProficiencyGroupStats = 74,
	AbilityCheckEvent = 75,
	EquipmentSlotChangedEvent = 76,
	LevelChanged = 77,
	SavingThrowRolledEvent = 78,
	SkillCheckEvent = 79,
	WeaponPropertiesChangedEvent = 80,
	AttributeFlagsChangedEvent = 81,
	ClassesChangedEvent = 82,
	StatsAppliedEvent = 83,
	ShapeshiftState = 84,
	ShapeshiftHealthReservation = 85,
	ServerShapeshiftStates = 86,
	MultiplayerUser = 87,
	MultiplayerHost = 88,
	AnimationWaterfall = 89,
	DynamicAnimationTags = 90,
	TemplateAnimationSetOverride = 91,
	AnimationTextKeyEventsSingleton = 92,
	AnimationGameplayEventsSingleton = 93,
	AnimationGameplayEvents = 94,
	AnimationTextKeyEvents = 95,
	AnimationTriggeredEvents = 96,
	Effect = 97,
	PassiveContainer = 98,
	Passive = 99,
	PassiveUsageCount = 100,
	ItemDestroyed = 101,
	ItemDye = 102,
	MapMarkerStyle = 103,
	ItemPortal = 104,
	ItemDestroying = 105,
	IsDoor = 106,
	ExamineDisabled = 107,
	ItemHasMoved = 108,
	HasOpened = 109,
	ItemInUse = 110,
	ItemIsPoisoned = 111,
	IsGold = 112,
	IsItem = 113,
	NewInInventory = 114,
	ShouldDestroyOnSpellCast = 115,
	ActionType = 116,
	ItemCanMove = 117,
	ClimbOn = 118,
	ItemTemplateDestroyed = 119,
	InteractionDisabled = 120,
	IsStoryItem = 121,
	Ladder = 122,
	WalkOn = 123,
	UseAction = 124,
	OwnedAsLoot = 125,
	OwneeCurrent = 126,
	ServerDynamicLayerOwner = 127,
	ServerIsCurrentOwner = 128,
	ServerIsLatestOwner = 129,
	ServerIsPreviousLatestOwner = 130,
	ServerIsPreviousOwner = 131,
	ServerIsOriginalOwner = 132,
	ServerOwneeHistory = 133,
	ServerOwneeRequest = 134,
	InventoryMember = 135,
	InventoryData = 136,
	InventoryOwner = 137,
	InventoryIsOwned = 138,
	InventoryContainer = 139,
	InventoryLocked = 140,
	InventoryMemberTransform = 141,
	InventoryStack = 142,
	InventoryStackMember = 143,
	InventoryTopOwner = 144,
	InventoryWeight = 145,
	TradeBuybackData = 146,
	Wielded = 147,
	WieldingHistory = 148,
	CanBeInInventory = 149,
	CannotBePickpocketed = 150,
	CannotBeTakenOut = 151,
	DropOnDeathBlocked = 152,
	NewItemsInside = 153,
	NonTradable = 154,
	ServerInventoryContainerData = 155,
	ServerInventoryGroupCheck = 156,
	ServerShapeshiftEquipmentHistory = 157,
	CharacterHasGeneratedTradeTreasure = 158,
	ServerInventoryIsReplicatedWith = 159,
	ItemHasGeneratedTreasure = 160,
	ReadyToBeAddedToInventory = 161,
	HasGeneratedTreasure = 162,
	InventoryPropertyCanBePickpocketed = 163,
	ScriptPropertyCanBePickpocketed = 164,
	InventoryPropertyIsDroppedOnDeath = 165,
	ScriptPropertyIsDroppedOnDeath = 166,
	InventoryPropertyIsTradable = 167,
	ScriptPropertyIsTradable = 168,
	PartyView = 169,
	PartyComposition = 170,
	PartyPortals = 171,
	PartyRecipes = 172,
	PartyWaypoints = 173,
	PartyMember = 174,
	PartyFollower = 175,
	CurrentlyFollowingParty = 176,
	BlockFollow = 177,
	Recruiter = 178,
	ServerUserSnapshot = 179,
	ImprovisedWeaponWielded = 180,
	ImprovisedWeaponWielding = 181,
	CanBeWielded = 182,
	PickUpRequest = 183,
	PickUpExecuting = 184,
	EscortFollower = 185,
	EscortLeader = 186,
	EscortLeaderPriority = 187,
	EscortMember = 188,
	EscortStragglersTracker = 189,
	EscortHasStragglers = 190,
	Camera = 191,
	DefaultCameraBehavior = 192,
	EffectCameraBehavior = 193,
	GameCameraBehavior = 194,
	CameraSelectorMode = 195,
	CameraTarget = 196,
	CameraCombatTarget = 197,
	CameraInSelectorMode = 198,
	CameraSpellTracking = 199,
	CampChest = 200,
	CampEndTheDayState = 201,
	CampQuality = 202,
	CampSettings = 203,
	CampSupply = 204,
	CampTotalSupplies = 205,
	CampPresence = 206,
	IsInCombat = 207,
	CombatDelayedFanfare = 208,
	CombatIsThreatened = 209,
	CombatParticipant = 210,
	CombatState = 211,
	TurnBased = 212,
	TurnOrder = 213,
	IsCombatPaused = 214,
	ThreatRange = 215,
	Death = 216,
	DeathState = 217,
	DeathType = 218,
	DeadByDefault = 219,
	Downed = 220,
	ServerKiller = 221,
	ServerDeathState = 222,
	ServerDeathContinue = 223,
	DeathApplyKnockedOut = 224,
	DeathAssignEntityToUserRequest = 225,
	DeathDeadByDefaultRequest = 226,
	DiedEvent = 227,
	DownedEvent = 228,
	FallToProne = 229,
	HitTarget = 230,
	HitAttacker = 231,
	HitWeapon = 232,
	HitThrownObject = 233,
	HitProxy = 234,
	HitProxyOwner = 235,
	HitReaction = 236,
	HitMeta = 237,
	HitLifetime = 238,
	HitAnimationRequest = 239,
	HitResultEvent = 240,
	HitJoinCombatRequest = 241,
	UnresolvedHitNotification = 242,
	HitNotification = 243,
	HitNotificationRequest = 244,
	Identity = 245,
	IdentityState = 246,
	OriginalIdentity = 247,
	ServerCanStartCombat = 248,
	ServerCombatGroupMapping = 249,
	ServerEnterRequest = 250,
	ServerFleeBlocked = 251,
	ServerImmediateJoin = 252,
	ServerCombatLeaveRequest = 253,
	CombatSwitched = 254,
	CombatFleeRequest = 255,
	GlobalCombatRequests = 256,
	CombatJoining = 257,
	CombatLateJoinPenalty = 258,
	CombatMerge = 259,
	CombatSurfaceTeamSingleton = 260,
	CombatantJoinEvent = 261,
	CombatFleeSuccess = 262,
	CombatantKilledEvent = 263,
	CombatLeftEvent = 264,
	CombatScheduledForDelete = 265,
	CombatStartedEvent = 266,
	DelayedFanfareRemovedDuringCombatEvent = 267,
	CombatJoinInCurrentRoundFailedEvent = 268,
	CombatJoinInCurrentRound = 269,
	CombatRequestCompletedEvent = 270,
	CombatSurprisedJoinRequest = 271,
	CombatSurprisedStealthRequest = 272,
	CombatThreatRangeChangedEvent = 273,
	FTBParticipant = 274,
	FTBZoneBlockReason = 275,
	FTBRespect = 276,
	FTBPaused = 277,
	IsInFTB = 278,
	FTBSurfaceTeamSingleton = 279,
	FTBTimeFactorRequests = 280,
	FTBTimeFactorResetRequests = 281,
	FTBTurnBasedChangesRequest = 282,
	FTBZone = 283,
	FTBZoneInstigator = 284,
	FTBModeChangedEvent = 285,
	FTBPlayersTurnEndedEvent = 286,
	FTBPlayersTurnStartedEvent = 287,
	FTBRoundEndedEvent = 288,
	ServerAiInterestedInItems = 289,
	ServerAiInterestingItem = 290,
	ServerAiArchetype = 291,
	ServerAiModifiers = 292,
	SpellContainer = 293,
	PlayerPrepareSpell = 294,
	CCPrepareSpell = 295,
	SpellModificationContainer = 296,
	AddedSpells = 297,
	AttackSpellOverride = 298,
	ScriptedExplosion = 299,
	SpellBookCooldowns = 300,
	LearnedSpells = 301,
	SpellAiConditions = 302,
	OnDamageSpells = 303,
	NewSpellsAddedEvent = 304,
	SpellsLearnedEvent = 305,
	SpellBookChanged = 306,
	SpellCastAnimationInfo = 307,
	SpellCastCache = 308,
	SpellCastDataCache = 309,
	SpellCastCanBeTargeted = 310,
	SpellCastIsCasting = 311,
	SpellCastInterruptResults = 312,
	SpellCastMovement = 313,
	SpellCastMovementInfo = 314,
	SpellCastRolls = 315,
	SpellCastState = 316,
	SpellSyncTargeting = 317,
	SpellCastOutcome = 318,
	SpellCastExecutionTime = 319,
	SpellCastEvent = 320,
	SpellCastFinishedEvent = 321,
	SpellCastTextKeyEvent = 322,
	SpellCastHitEvent = 323,
	SpellCastDestroyEvent = 324,
	SpellCastSpellRollAbortEvent = 325,
	SpellCastTargetHitEvent = 326,
	SpellCastTargetHitInterruptEvent = 327,
	SpellCastTargetReactionEvent = 328,
	SpellCastTargetsChangedEvent = 329,
	SpellCastAnimationRequest = 330,
	SpellCastCounteredEvent = 331,
	SpellCastJumpStartEvent = 332,
	SpellCastLogicExecutionStartEvent = 333,
	SpellCastLogicExecutionEndEvent = 334,
	SpellCastPrepareEndEvent = 335,
	SpellCastPrepareStartEvent = 336,
	SpellCastPreviewEndEvent = 337,
	SpellCastThrowPickupPositionChangedEvent = 338,
	ServerSpellCastHitDelay = 339,
	ServerSpellCastMovementInfo = 340,
	ServerSpellCastRequests = 341,
	ServerSpellCastPendingRequests = 342,
	ServerSpellCastResponsible = 343,
	ServerSpellClientInitiated = 344,
	ServerSpellExternals = 345,
	ServerSpellHitRegister = 346,
	ServerSpellInterruptRequests = 347,
	ServerSpellInterruptResults = 348,
	ServerSpellCastMovement = 349,
	ServerSpellCastState = 350,
	ServerSpellCastCache = 351,
	ServerSpellCastInterrupt = 352,
	ServerSpellCastZoneRange = 353,
	ServerSpellCastProjectilePathfindCache = 354,
	ServerSpellCastUnsheathFallbackTimer = 355,
	SpellCastInterruptsUsed = 356,
	SpellCastMoveDuringCastUpdateEvent = 357,
	SpellCastMovementAndPrecalculationEndEvent = 358,
	SpellCastRequestTargetTracking = 359,
	SpellCastUpdateTargetTracking = 360,
	SpellCastWeaponSetChangeRequest = 361,
	ConcentrationChanged = 362,
	ConcentrationDamageCheck = 363,
	ConcentrationClearedEvent = 364,
	InterruptActionState = 365,
	InterruptConditionallyDisabled = 366,
	InterruptContainer = 367,
	InterruptData = 368,
	InterruptDecision = 369,
	InterruptPreferences = 370,
	InterruptPrepared = 371,
	InterruptZone = 372,
	InterruptZoneParticipant = 373,
	InterruptZoneSource = 374,
	ServerInterruptAddRemoveRequests = 375,
	ServerInterruptActionRequests = 376,
	ServerInterruptZoneRequests = 377,
	ServerInterruptInitialParticipants = 378,
	ServerInterruptTurnOrderInZone = 379,
	ServerInterruptDataSingleton = 380,
	ServerInterruptUsed = 381,
	ClientInterruptPlayerDecision = 382,
	Sight = 383,
	SightData = 384,
	SightEntityViewshed = 385,
	IgnoreSurfaces = 386,
	StatusContainer = 387,
	StatusCause = 388,
	StatusID = 389,
	StatusIncapacitated = 390,
	StatusIndicateDarkness = 391,
	StatusLifetime = 392,
	StatusLoseControl = 393,
	StatusVisualDisabled = 394,
	ServerStatusCause = 395,
	ServerStatus = 396,
	ServerStatusOwnership = 397,
	ServerStatusDifficultyModifiers = 398,
	ServerStatusUnique = 399,
	ServerStatusPerforming = 400,
	ServerStatusActive = 401,
	ServerStatusAddedFromSaveLoad = 402,
	ServerStatusAura = 403,
	ServerStatusAuraContainer = 404,
	ServerAddedStatusAuraEffectEvent = 405,
	ServerRemovedStatusAuraEffectEvent = 406,
	ServerStatusAddEvent = 407,
	ServerStatusApplyEvent = 408,
	ServerStatusAttemptEvent = 409,
	ServerStatusAttemptFailedEvent = 410,
	ServerStatusDispelRollCheck = 411,
	ServerStatusDownedChangedEvent = 412,
	ServerStatusRefreshed = 413,
	ServerStatusRemoveEvent = 414,
	ServerStatusScheduledForDeletion = 415,
	ServerStatusEvent = 416,
	ServerStatusTurnStartEvent = 417,
	ServerStatusUpdateTargetTracking = 418,
	TadpoleTreeState = 419,
	TadpolePowers = 420,
	Tadpoled = 421,
	HalfIllithid = 422,
	FullIllithid = 423,
	ShortRest = 424,
	GlobalShortRestDisabled = 425,
	GlobalLongRestDisabled = 426,
	StoryShortRestDisabled = 427,
	CanDoRest = 428,
	LongRestInScriptPhase = 429,
	LongRestState = 430,
	LongRestTimeline = 431,
	LongRestTimers = 432,
	LongRestUsers = 433,
	RestingEntities = 434,
	ShortRestConsumeResources = 435,
	ServerRestPendingType = 436,
	ServerRestTypeChosenEvent = 437,
	ServerShortRestResultEvent = 438,
	CCCompanionDefinition = 439,
	CCDefinitionCommon = 440,
	CCFullRespecDefinition = 441,
	CCRespecDefinition = 442,
	CCChangeAppearanceDefinition = 443,
	CCCharacterDefinition = 444,
	CCSessionCommon = 445,
	CCState = 446,
	CCLevelUp = 447,
	CCLevelUpDefinition = 448,
	CCFullRespec = 449,
	CCRespec = 450,
	CCCreation = 451,
	CCDefinitionLevelUp = 452,
	ClientCCDefinitionState = 453,
	ClientCCDefinitionStateEx = 454,
	ClientCCBaseDefinitionState = 455,
	ClientCCCompanionDefinition = 456,
	ClientCCLevelUpDefinition = 457,
	ClientCCChangeAppearanceDefinition = 458,
	ClientCCFullRespecDefinition = 459,
	ClientCCDummyDefinition = 460,
	ServerCCCustomIcon = 461,
	ServerCCAppearanceVisualTag = 462,
	ServerCCEquipmentSetRequest = 463,
	ServerCCGod = 464,
	ServerCCUpdates = 465,
	ServerCCIsCustom = 466,
	TriggerArea = 467,
	TriggerType = 468,
	TriggerContainer = 469,
	TriggerIsInsideOf = 470,
	TriggerUpdatedContainer = 471,
	TriggerUpdatedPhysics = 472,
	ServerStatsAreaTrigger = 473,
	ServerSoundVolumeTrigger = 474,
	ServerAIHintAreaTrigger = 475,
	ServerCrowdCharacterTrigger = 476,
	ServerChasmSeederTrigger = 477,
	ServerTeleportTrigger = 478,
	ServerCampRegionTrigger = 479,
	ServerEocAreaTrigger = 480,
	ServerStartTrigger = 481,
	ServerAtmosphereTrigger = 482,
	ServerCrimeAreaTrigger = 483,
	ServerCrimeRegionTrigger = 484,
	ServerEventTrigger = 485,
	ServerFloorTrigger = 486,
	ServerPortalTrigger = 487,
	ServerRoomTrigger = 488,
	ServerAvatarContainerTrigger = 489,
	ServerCampChestTrigger = 490,
	ServerChasmRegionTrigger = 491,
	ServerExplorationTrigger = 492,
	ServerMusicVolumeTrigger = 493,
	ServerMusicVolumeTriggerState = 494,
	ServerTimelineSceneTrigger = 495,
	ServerBlockBronzeTimelinePlacementTrigger = 496,
	ServerAiGridAreaTrigger = 497,
	ServerRegionTrigger = 498,
	ServerLightingTrigger = 499,
	ServerEocPointTrigger = 500,
	ServerTriggerEventConfig = 501,
	ServerTriggerRegisteredFor = 502,
	ServerRegistrationSettings = 503,
	ServerUpdatedRegisteredFor = 504,
	ServerUpdatedRegisteredForItems = 505,
	ServerTriggerCachedLeaveEvents = 506,
	ServerTriggerLoadedHandled = 507,
	ServerTriggerWorldAutoTriggered = 508,
	Background = 509,
	God = 510,
	Proficiency = 511,
	ProficiencyGroup = 512,
	LevelUp = 513,
	Floating = 514,
	Voice = 515,
	CustomIcon = 516,
	CharacterCreationStats = 517,
	Disarmable = 518,
	IsSummon = 519,
	SummonContainer = 520,
	Stealth = 521,
	ClientControl = 522,
	IsGlobal = 523,
	Savegame = 524,
	LootingState = 525,
	Loot = 526,
	Lock = 527,
	Key = 528,
	SummonLifetime = 529,
	Invisibility = 530,
	Icon = 531,
	HotbarContainer = 532,
	HotbarDecks = 533,
	OriginTag = 534,
	OriginPassives = 535,
	ClassTag = 536,
	BackgroundTag = 537,
	OriginAppearanceTag = 538,
	VoiceTag = 539,
	GodTag = 540,
	BackgroundPassives = 541,
	FleeCapability = 542,
	IsInTurnBasedMode = 543,
	ItemBoosts = 544,
	ActiveCharacterLight = 545,
	Visual = 546,
	ClientPaperdoll = 547,
	TLPreviewDummy = 548,
	TimelineActorData = 549,
	ClientTimelineActorControl = 550,
	AnimationSet = 551,
	AnimationUpdate = 552,
	AnimationBlueprint = 553,
	CanModifyHealth = 554,
	CanBeLooted = 555,
	CanBeDisarmed = 556,
	CanTriggerRandomCasts = 557,
	CanDoActions = 558,
	ActionUseConditions = 559,
	CanMove = 560,
	CanSense = 561,
	Concentration = 562,
	Darkness = 563,
	DualWielding = 564,
	GameObjectVisual = 565,
	AppearanceOverride = 566,
	CharacterCreationTemplateOverride = 567,
	GravityDisabled = 568,
	GravityDisabledUntilMoved = 569,
	Avatar = 570,
	HasExclamationDialog = 571,
	Trader = 572,
	CanTrade = 573,
	IsFalling = 574,
	HealMaxIncoming = 575,
	HealMaxOutgoing = 576,
	Ambushing = 577,
	InteractionFilter = 578,
	DisplayName = 579,
	CustomName = 580,
	Equipable = 581,
	GameplayLight = 582,
	Light = 583,
	CombinedLight = 584,
	Cull = 585,
	SceneAttach = 586,
	IsSeeThrough = 587,
	CustomIconsStorage = 588,
	FogVolumeRequest = 589,
	Race = 590,
	CanTravel = 591,
	Movement = 592,
	ObjectInteraction = 593,
	StaticPhysics = 594,
	Pathing = 595,
	PathingDistanceChanged = 596,
	Steering = 597,
	CanDeflectProjectiles = 598,
	ActiveSkeletonSlots = 599,
	Net = 600,
	Physics = 601,
	ApprovalRatings = 602,
	AttitudesToPlayers = 603,
	CharacterCreationAppearance = 604,
	Active = 605,
	Repose = 606,
	OriginalTemplate = 607,
	CanEnterChasm = 608,
	DialogState = 609,
	EncumbranceState = 610,
	EncumbranceStats = 611,
	Unsheath = 612,
	UseSocket = 613,
	UserAvatar = 614,
	UserReservedFor = 615,
	Sound = 616,
	PauseExcluded = 617,
	Pause = 618,
	BackgroundGoals = 619,
	CalendarDaysPassed = 620,
	CalendarStartingDate = 621,
	DisabledEquipment = 622,
	EquipmentVisual = 623,
	ClientEquipmentVisuals = 624,
	ClientEquipmentVisibilityState = 625,
	ClientVisualsDesiredState = 626,
	ClientVisualsVisibilityState = 627,
	ProgressionContainer = 628,
	ProgressionMeta = 629,
	ProgressionChangedContainers = 630,
	ProgressionLevelUpChanged = 631,
	ProgressionAbilityImprovements = 632,
	ProgressionFeat = 633,
	ProgressionSpells = 634,
	ProgressionSkills = 635,
	ProgressionPassives = 636,
	ProgressionReplicatedFeat = 637,
	CanSeeThrough = 638,
	CanShootThrough = 639,
	CanWalkThrough = 640,
	ShootThroughType = 641,
	ServerRecruitedBy = 642,
	ServerGameTimer = 643,
	ServerExperienceGaveOut = 644,
	ServerReplicationDependency = 645,
	ServerIsUnsummoning = 646,
	ServerActivationGroupContainer = 647,
	ServerAnubisTag = 648,
	ServerDialogTag = 649,
	ServerDisplayNameList = 650,
	ServerIconList = 651,
	ServerRaceTag = 652,
	ServerTemplateTag = 653,
	ServerBoostTag = 654,
	ServerSafePosition = 655,
	ServerAnubisExecutor = 656,
	ServerLeader = 657,
	ServerBreadcrumb = 658,
	ServerDelayDeathCause = 659,
	ServerPickpocket = 660,
	ServerReplicationDependencyOwner = 661,
	ServerGameplayLightEquipment = 662,
	HistoryTargetUUID = 663,
	ServerGameplayLightChanges = 664,
	ServerPeersInRange = 665,
	ServerSurface = 666,
	ServerDisarmAttempt = 667,
	ServerUnsheath = 668,
	ServerUnsheathScriptOverride = 669,
	ServerIsLightBlocker = 670,
	ServerIsVisionBlocker = 671,
	ServerDarknessActive = 672,
	ServerHotbarOrder = 673,
	ServerInventoryItemDataPopulated = 674,
	ServerVariableManager = 675,
	IsMarkedForDeletion = 676,
	JumpFollow = 677,
	ServerTemplateChangedEvent = 678,
	ServerTemplateTransformedEvent = 679,
	TagsChangedEvent = 680,
	SummonCreatedEvent = 681,
	SummonAddConcentrationRequest = 682,
	SummonAddToExistingConcentrationRequest = 683,
	SummonAttachToProjectileRequest = 684,
	SummonOwnerSetEvent = 685,
	SummonPlaceInInventoryRequest = 686,
	SummonSetLifetimeRequest = 687,
	SummonDespawnRequest = 688,
	SummonExpiredRequest = 689,
	SummonLateJoinPenalty = 690,
	ServerToggledPassives = 691,
	ServerPassiveBoosts = 692,
	ServerPassiveBase = 693,
	ServerPassivePersistentData = 694,
	ServerScriptPassives = 695,
	PassiveRequestTargetTracking = 696,
	PassiveUpdateTargetTracking = 697,
	PassiveConditionalRollInterruptEvent = 698,
	PassivesUpdatedEvent = 699,
	PasssiveUsageCountIncrementedEvent = 700,
	Construction = 701,
	ConstructionTile = 702,
	ConstructionFilling = 703,
	ServerCharacter = 704,
	ServerItem = 705,
	ProjectileSource = 706,
	ServerProjectile = 707,
	ServerProjectileAttachment = 708,
	ServerProjectileSpell = 709,
	ServerProjectileInitialization = 710,
	ServerProjectileCache = 711,
	ServerOsirisTag = 712,
	ProjectileImpactEvent = 713,
	ProjectileRequestTargetTracking = 714,
	ProjectileUpdateTargetTracking = 715,
	ProjectileSplitThrowableObjectRequest = 716,
	ClientCharacterIconRequest = 717,
	ClientCharacterIconResult = 718,
	ClientCharacter = 719,
	ClientItem = 720,
	ClientProjectile = 721,
	Scenery = 722,
	RequestedRoll = 723,
	RollInProgress = 724,
	ServerRollFinishedEvent = 725,
	RollModifiers = 726,
	ServerRollInProgress = 727,
	ServerRollStartRequest = 728,
	ServerRollStartSpellRequest = 729,
	ArmorClassBoost = 730,
	AbilityBoost = 731,
	RollBonusBoost = 732,
	AdvantageBoost = 733,
	ActionResourceValueBoost = 734,
	CriticalHitBoost = 735,
	AbilityFailedSavingThrowBoost = 736,
	ResistanceBoost = 737,
	WeaponDamageResistanceBoost = 738,
	ProficiencyBonusOverrideBoost = 739,
	JumpMaxDistanceMultiplierBoost = 740,
	HalveWeaponDamageBoost = 741,
	UnlockSpellBoost = 742,
	SourceAdvantageBoost = 743,
	ProficiencyBonusBoost = 744,
	ProficiencyBoost = 745,
	IncreaseMaxHPBoost = 746,
	ActionResourceBlockBoost = 747,
	StatusImmunityBoost = 748,
	UseBoosts = 749,
	CannotHarmCauseEntityBoost = 750,
	TemporaryHPBoost = 751,
	WeightBoost = 752,
	WeightCategoryBoost = 753,
	FactionOverrideBoost = 754,
	ActionResourceMultiplierBoost = 755,
	BlockRegainHPBoost = 756,
	InitiativeBoost = 757,
	DarkvisionRangeBoost = 758,
	DarkvisionRangeMinBoost = 759,
	DarkvisionRangeOverrideBoost = 760,
	AddTagBoost = 761,
	IgnoreDamageThresholdMinBoost = 762,
	SkillBoost = 763,
	WeaponDamageBoost = 764,
	NullifyAbilityBoost = 765,
	RerollBoost = 766,
	DownedStatusBoost = 767,
	WeaponEnchantmentBoost = 768,
	GuaranteedChanceRollOutcomeBoost = 769,
	AttributeBoost = 770,
	GameplayLightBoost = 771,
	DualWieldingBoost = 772,
	SavantBoost = 773,
	MinimumRollResultBoost = 774,
	CharacterWeaponDamageBoost = 775,
	ProjectileDeflectBoost = 776,
	AbilityOverrideMinimumBoost = 777,
	ACOverrideFormulaBoost = 778,
	FallDamageMultiplierBoost = 779,
	ActiveCharacterLightBoost = 780,
	WeaponAttackTypeOverrideBoost = 781,
	WeaponDamageDieOverrideBoost = 782,
	CarryCapacityMultiplierBoost = 783,
	WeaponPropertyBoost = 784,
	WeaponAttackRollAbilityOverrideBoost = 785,
	SightRangeAdditiveBoost = 786,
	SightRangeMinimumBoost = 787,
	SightRangeMaximumBoost = 788,
	SightRangeOverrideBoost = 789,
	MovementSpeedLimitBoost = 790,
	UnlockSpellVariantBoost = 791,
	DetectCrimesBlockBoost = 792,
	BlockAbilityModifierFromACBoost = 793,
	ScaleMultiplierBoost = 794,
	DamageReductionBoost = 795,
	ReduceCriticalAttackThresholdBoost = 796,
	PhysicalForceRangeBonusBoost = 797,
	ObjectSizeBoost = 798,
	ObjectSizeOverrideBoost = 799,
	AiArchetypeOverrideBoost = 800,
	ExpertiseBonusBoost = 801,
	EntityThrowDamageBoost = 802,
	WeaponDamageTypeOverrideBoost = 803,
	MaximizeHealingBoost = 804,
	DamageBonusBoost = 805,
	AdvanceSpellsBoost = 806,
	SpellResistanceBoost = 807,
	WeaponAttackRollBonusBoost = 808,
	SpellSaveDCBoost = 809,
	RedirectDamageBoost = 810,
	CanSeeThroughBoost = 811,
	CanShootThroughBoost = 812,
	CanWalkThroughBoost = 813,
	MonkWeaponDamageDiceOverrideBoost = 814,
	HorizontalFOVOverrideBoost = 815,
	CharacterUnarmedDamageBoost = 816,
	ActionResourceReplenishTypeOverrideBoost = 817,
	ActionResourcePreventReductionBoost = 818,
	AttackSpellOverrideBoost = 819,
	LockBoost = 820,
	IgnorePointBlankDisadvantageBoost = 821,
	CriticalHitExtraDiceBoost = 822,
	DodgeAttackRollBoost = 823,
	GameplayObscurityBoost = 824,
	MaximumRollResultBoost = 825,
	UnlockInterruptBoost = 826,
	JumpMaxDistanceBonusBoost = 827,
	ArmorAbilityModifierCapOverrideBoost = 828,
	IgnoreResistanceBoost = 829,
	ConcentrationIgnoreDamageBoost = 830,
	IgnoreLowGroundPenaltyBoost = 831,
	IgnoreSurfaceCoverBoost = 832,
	ProficiencyBonusIncreaseBoost = 833,
	DamageTakenBonusBoost = 834,
	ReceivingCriticalDamageOnHitBoost = 835,
	Uuid = 836,
	UuidToHandleMapping = 837,
	Max = 838,
	[0] = "ActionResources",
	[1] = "ActionResourceSpendEvent",
	[2] = "ActionResourceChangeResults",
	[3] = "ActionResourceEvents",
	[4] = "Armor",
	[5] = "ArmorSetState",
	[6] = "BaseHp",
	[7] = "Bound",
	[8] = "IsCharacter",
	[9] = "Data",
	[10] = "Detached",
	[11] = "Health",
	[12] = "Resistances",
	[13] = "Hearing",
	[14] = "SpellBook",
	[15] = "Stats",
	[16] = "StatusImmunities",
	[17] = "SurfacePathInfluences",
	[18] = "Use",
	[19] = "Value",
	[20] = "Weapon",
	[21] = "Wielding",
	[22] = "CustomStats",
	[23] = "ActionResourceConsumeMultiplierBoost",
	[24] = "Tag",
	[25] = "SpellBookPrepares",
	[26] = "Transform",
	[27] = "TranslateChanged",
	[28] = "TranslateChangedEvent",
	[29] = "Relation",
	[30] = "Faction",
	[31] = "CanInteract",
	[32] = "CanSpeak",
	[33] = "Origin",
	[34] = "Level",
	[35] = "EocLevel",
	[36] = "Classes",
	[37] = "MaterialParameterOverride",
	[38] = "OffStage",
	[39] = "PickingState",
	[40] = "Player",
	[41] = "SimpleCharacter",
	[42] = "Speaker",
	[43] = "WeaponSet",
	[44] = "DifficultyCheck",
	[45] = "AttributeFlags",
	[46] = "BodyType",
	[47] = "ObjectSize",
	[48] = "BaseStats",
	[49] = "Expertise",
	[50] = "HealBlock",
	[51] = "Ruleset",
	[52] = "RulesetModifiers",
	[53] = "TimeFactor",
	[54] = "Clock",
	[55] = "Experience",
	[56] = "AvailableLevel",
	[57] = "CanLevelUp",
	[58] = "BoostCondition",
	[59] = "BoostsContainer",
	[60] = "BoostInfo",
	[61] = "ServerBoostBase",
	[62] = "ServerStatusBoostsProcessed",
	[63] = "BoostChangedEvent",
	[64] = "BoostChangedEventsSingleton",
	[65] = "BoostConditionalState",
	[66] = "BoostProvider",
	[67] = "StatusBoostsRefreshed",
	[68] = "BoostBaseUpdated",
	[69] = "ServerBaseData",
	[70] = "ServerBaseSize",
	[71] = "ServerBaseStats",
	[72] = "ServerBaseWeapon",
	[73] = "ServerBaseProficiency",
	[74] = "ServerProficiencyGroupStats",
	[75] = "AbilityCheckEvent",
	[76] = "EquipmentSlotChangedEvent",
	[77] = "LevelChanged",
	[78] = "SavingThrowRolledEvent",
	[79] = "SkillCheckEvent",
	[80] = "WeaponPropertiesChangedEvent",
	[81] = "AttributeFlagsChangedEvent",
	[82] = "ClassesChangedEvent",
	[83] = "StatsAppliedEvent",
	[84] = "ShapeshiftState",
	[85] = "ShapeshiftHealthReservation",
	[86] = "ServerShapeshiftStates",
	[87] = "MultiplayerUser",
	[88] = "MultiplayerHost",
	[89] = "AnimationWaterfall",
	[90] = "DynamicAnimationTags",
	[91] = "TemplateAnimationSetOverride",
	[92] = "AnimationTextKeyEventsSingleton",
	[93] = "AnimationGameplayEventsSingleton",
	[94] = "AnimationGameplayEvents",
	[95] = "AnimationTextKeyEvents",
	[96] = "AnimationTriggeredEvents",
	[97] = "Effect",
	[98] = "PassiveContainer",
	[99] = "Passive",
	[100] = "PassiveUsageCount",
	[101] = "ItemDestroyed",
	[102] = "ItemDye",
	[103] = "MapMarkerStyle",
	[104] = "ItemPortal",
	[105] = "ItemDestroying",
	[106] = "IsDoor",
	[107] = "ExamineDisabled",
	[108] = "ItemHasMoved",
	[109] = "HasOpened",
	[110] = "ItemInUse",
	[111] = "ItemIsPoisoned",
	[112] = "IsGold",
	[113] = "IsItem",
	[114] = "NewInInventory",
	[115] = "ShouldDestroyOnSpellCast",
	[116] = "ActionType",
	[117] = "ItemCanMove",
	[118] = "ClimbOn",
	[119] = "ItemTemplateDestroyed",
	[120] = "InteractionDisabled",
	[121] = "IsStoryItem",
	[122] = "Ladder",
	[123] = "WalkOn",
	[124] = "UseAction",
	[125] = "OwnedAsLoot",
	[126] = "OwneeCurrent",
	[127] = "ServerDynamicLayerOwner",
	[128] = "ServerIsCurrentOwner",
	[129] = "ServerIsLatestOwner",
	[130] = "ServerIsPreviousLatestOwner",
	[131] = "ServerIsPreviousOwner",
	[132] = "ServerIsOriginalOwner",
	[133] = "ServerOwneeHistory",
	[134] = "ServerOwneeRequest",
	[135] = "InventoryMember",
	[136] = "InventoryData",
	[137] = "InventoryOwner",
	[138] = "InventoryIsOwned",
	[139] = "InventoryContainer",
	[140] = "InventoryLocked",
	[141] = "InventoryMemberTransform",
	[142] = "InventoryStack",
	[143] = "InventoryStackMember",
	[144] = "InventoryTopOwner",
	[145] = "InventoryWeight",
	[146] = "TradeBuybackData",
	[147] = "Wielded",
	[148] = "WieldingHistory",
	[149] = "CanBeInInventory",
	[150] = "CannotBePickpocketed",
	[151] = "CannotBeTakenOut",
	[152] = "DropOnDeathBlocked",
	[153] = "NewItemsInside",
	[154] = "NonTradable",
	[155] = "ServerInventoryContainerData",
	[156] = "ServerInventoryGroupCheck",
	[157] = "ServerShapeshiftEquipmentHistory",
	[158] = "CharacterHasGeneratedTradeTreasure",
	[159] = "ServerInventoryIsReplicatedWith",
	[160] = "ItemHasGeneratedTreasure",
	[161] = "ReadyToBeAddedToInventory",
	[162] = "HasGeneratedTreasure",
	[163] = "InventoryPropertyCanBePickpocketed",
	[164] = "ScriptPropertyCanBePickpocketed",
	[165] = "InventoryPropertyIsDroppedOnDeath",
	[166] = "ScriptPropertyIsDroppedOnDeath",
	[167] = "InventoryPropertyIsTradable",
	[168] = "ScriptPropertyIsTradable",
	[169] = "PartyView",
	[170] = "PartyComposition",
	[171] = "PartyPortals",
	[172] = "PartyRecipes",
	[173] = "PartyWaypoints",
	[174] = "PartyMember",
	[175] = "PartyFollower",
	[176] = "CurrentlyFollowingParty",
	[177] = "BlockFollow",
	[178] = "Recruiter",
	[179] = "ServerUserSnapshot",
	[180] = "ImprovisedWeaponWielded",
	[181] = "ImprovisedWeaponWielding",
	[182] = "CanBeWielded",
	[183] = "PickUpRequest",
	[184] = "PickUpExecuting",
	[185] = "EscortFollower",
	[186] = "EscortLeader",
	[187] = "EscortLeaderPriority",
	[188] = "EscortMember",
	[189] = "EscortStragglersTracker",
	[190] = "EscortHasStragglers",
	[191] = "Camera",
	[192] = "DefaultCameraBehavior",
	[193] = "EffectCameraBehavior",
	[194] = "GameCameraBehavior",
	[195] = "CameraSelectorMode",
	[196] = "CameraTarget",
	[197] = "CameraCombatTarget",
	[198] = "CameraInSelectorMode",
	[199] = "CameraSpellTracking",
	[200] = "CampChest",
	[201] = "CampEndTheDayState",
	[202] = "CampQuality",
	[203] = "CampSettings",
	[204] = "CampSupply",
	[205] = "CampTotalSupplies",
	[206] = "CampPresence",
	[207] = "IsInCombat",
	[208] = "CombatDelayedFanfare",
	[209] = "CombatIsThreatened",
	[210] = "CombatParticipant",
	[211] = "CombatState",
	[212] = "TurnBased",
	[213] = "TurnOrder",
	[214] = "IsCombatPaused",
	[215] = "ThreatRange",
	[216] = "Death",
	[217] = "DeathState",
	[218] = "DeathType",
	[219] = "DeadByDefault",
	[220] = "Downed",
	[221] = "ServerKiller",
	[222] = "ServerDeathState",
	[223] = "ServerDeathContinue",
	[224] = "DeathApplyKnockedOut",
	[225] = "DeathAssignEntityToUserRequest",
	[226] = "DeathDeadByDefaultRequest",
	[227] = "DiedEvent",
	[228] = "DownedEvent",
	[229] = "FallToProne",
	[230] = "HitTarget",
	[231] = "HitAttacker",
	[232] = "HitWeapon",
	[233] = "HitThrownObject",
	[234] = "HitProxy",
	[235] = "HitProxyOwner",
	[236] = "HitReaction",
	[237] = "HitMeta",
	[238] = "HitLifetime",
	[239] = "HitAnimationRequest",
	[240] = "HitResultEvent",
	[241] = "HitJoinCombatRequest",
	[242] = "UnresolvedHitNotification",
	[243] = "HitNotification",
	[244] = "HitNotificationRequest",
	[245] = "Identity",
	[246] = "IdentityState",
	[247] = "OriginalIdentity",
	[248] = "ServerCanStartCombat",
	[249] = "ServerCombatGroupMapping",
	[250] = "ServerEnterRequest",
	[251] = "ServerFleeBlocked",
	[252] = "ServerImmediateJoin",
	[253] = "ServerCombatLeaveRequest",
	[254] = "CombatSwitched",
	[255] = "CombatFleeRequest",
	[256] = "GlobalCombatRequests",
	[257] = "CombatJoining",
	[258] = "CombatLateJoinPenalty",
	[259] = "CombatMerge",
	[260] = "CombatSurfaceTeamSingleton",
	[261] = "CombatantJoinEvent",
	[262] = "CombatFleeSuccess",
	[263] = "CombatantKilledEvent",
	[264] = "CombatLeftEvent",
	[265] = "CombatScheduledForDelete",
	[266] = "CombatStartedEvent",
	[267] = "DelayedFanfareRemovedDuringCombatEvent",
	[268] = "CombatJoinInCurrentRoundFailedEvent",
	[269] = "CombatJoinInCurrentRound",
	[270] = "CombatRequestCompletedEvent",
	[271] = "CombatSurprisedJoinRequest",
	[272] = "CombatSurprisedStealthRequest",
	[273] = "CombatThreatRangeChangedEvent",
	[274] = "FTBParticipant",
	[275] = "FTBZoneBlockReason",
	[276] = "FTBRespect",
	[277] = "FTBPaused",
	[278] = "IsInFTB",
	[279] = "FTBSurfaceTeamSingleton",
	[280] = "FTBTimeFactorRequests",
	[281] = "FTBTimeFactorResetRequests",
	[282] = "FTBTurnBasedChangesRequest",
	[283] = "FTBZone",
	[284] = "FTBZoneInstigator",
	[285] = "FTBModeChangedEvent",
	[286] = "FTBPlayersTurnEndedEvent",
	[287] = "FTBPlayersTurnStartedEvent",
	[288] = "FTBRoundEndedEvent",
	[289] = "ServerAiInterestedInItems",
	[290] = "ServerAiInterestingItem",
	[291] = "ServerAiArchetype",
	[292] = "ServerAiModifiers",
	[293] = "SpellContainer",
	[294] = "PlayerPrepareSpell",
	[295] = "CCPrepareSpell",
	[296] = "SpellModificationContainer",
	[297] = "AddedSpells",
	[298] = "AttackSpellOverride",
	[299] = "ScriptedExplosion",
	[300] = "SpellBookCooldowns",
	[301] = "LearnedSpells",
	[302] = "SpellAiConditions",
	[303] = "OnDamageSpells",
	[304] = "NewSpellsAddedEvent",
	[305] = "SpellsLearnedEvent",
	[306] = "SpellBookChanged",
	[307] = "SpellCastAnimationInfo",
	[308] = "SpellCastCache",
	[309] = "SpellCastDataCache",
	[310] = "SpellCastCanBeTargeted",
	[311] = "SpellCastIsCasting",
	[312] = "SpellCastInterruptResults",
	[313] = "SpellCastMovement",
	[314] = "SpellCastMovementInfo",
	[315] = "SpellCastRolls",
	[316] = "SpellCastState",
	[317] = "SpellSyncTargeting",
	[318] = "SpellCastOutcome",
	[319] = "SpellCastExecutionTime",
	[320] = "SpellCastEvent",
	[321] = "SpellCastFinishedEvent",
	[322] = "SpellCastTextKeyEvent",
	[323] = "SpellCastHitEvent",
	[324] = "SpellCastDestroyEvent",
	[325] = "SpellCastSpellRollAbortEvent",
	[326] = "SpellCastTargetHitEvent",
	[327] = "SpellCastTargetHitInterruptEvent",
	[328] = "SpellCastTargetReactionEvent",
	[329] = "SpellCastTargetsChangedEvent",
	[330] = "SpellCastAnimationRequest",
	[331] = "SpellCastCounteredEvent",
	[332] = "SpellCastJumpStartEvent",
	[333] = "SpellCastLogicExecutionStartEvent",
	[334] = "SpellCastLogicExecutionEndEvent",
	[335] = "SpellCastPrepareEndEvent",
	[336] = "SpellCastPrepareStartEvent",
	[337] = "SpellCastPreviewEndEvent",
	[338] = "SpellCastThrowPickupPositionChangedEvent",
	[339] = "ServerSpellCastHitDelay",
	[340] = "ServerSpellCastMovementInfo",
	[341] = "ServerSpellCastRequests",
	[342] = "ServerSpellCastPendingRequests",
	[343] = "ServerSpellCastResponsible",
	[344] = "ServerSpellClientInitiated",
	[345] = "ServerSpellExternals",
	[346] = "ServerSpellHitRegister",
	[347] = "ServerSpellInterruptRequests",
	[348] = "ServerSpellInterruptResults",
	[349] = "ServerSpellCastMovement",
	[350] = "ServerSpellCastState",
	[351] = "ServerSpellCastCache",
	[352] = "ServerSpellCastInterrupt",
	[353] = "ServerSpellCastZoneRange",
	[354] = "ServerSpellCastProjectilePathfindCache",
	[355] = "ServerSpellCastUnsheathFallbackTimer",
	[356] = "SpellCastInterruptsUsed",
	[357] = "SpellCastMoveDuringCastUpdateEvent",
	[358] = "SpellCastMovementAndPrecalculationEndEvent",
	[359] = "SpellCastRequestTargetTracking",
	[360] = "SpellCastUpdateTargetTracking",
	[361] = "SpellCastWeaponSetChangeRequest",
	[362] = "ConcentrationChanged",
	[363] = "ConcentrationDamageCheck",
	[364] = "ConcentrationClearedEvent",
	[365] = "InterruptActionState",
	[366] = "InterruptConditionallyDisabled",
	[367] = "InterruptContainer",
	[368] = "InterruptData",
	[369] = "InterruptDecision",
	[370] = "InterruptPreferences",
	[371] = "InterruptPrepared",
	[372] = "InterruptZone",
	[373] = "InterruptZoneParticipant",
	[374] = "InterruptZoneSource",
	[375] = "ServerInterruptAddRemoveRequests",
	[376] = "ServerInterruptActionRequests",
	[377] = "ServerInterruptZoneRequests",
	[378] = "ServerInterruptInitialParticipants",
	[379] = "ServerInterruptTurnOrderInZone",
	[380] = "ServerInterruptDataSingleton",
	[381] = "ServerInterruptUsed",
	[382] = "ClientInterruptPlayerDecision",
	[383] = "Sight",
	[384] = "SightData",
	[385] = "SightEntityViewshed",
	[386] = "IgnoreSurfaces",
	[387] = "StatusContainer",
	[388] = "StatusCause",
	[389] = "StatusID",
	[390] = "StatusIncapacitated",
	[391] = "StatusIndicateDarkness",
	[392] = "StatusLifetime",
	[393] = "StatusLoseControl",
	[394] = "StatusVisualDisabled",
	[395] = "ServerStatusCause",
	[396] = "ServerStatus",
	[397] = "ServerStatusOwnership",
	[398] = "ServerStatusDifficultyModifiers",
	[399] = "ServerStatusUnique",
	[400] = "ServerStatusPerforming",
	[401] = "ServerStatusActive",
	[402] = "ServerStatusAddedFromSaveLoad",
	[403] = "ServerStatusAura",
	[404] = "ServerStatusAuraContainer",
	[405] = "ServerAddedStatusAuraEffectEvent",
	[406] = "ServerRemovedStatusAuraEffectEvent",
	[407] = "ServerStatusAddEvent",
	[408] = "ServerStatusApplyEvent",
	[409] = "ServerStatusAttemptEvent",
	[410] = "ServerStatusAttemptFailedEvent",
	[411] = "ServerStatusDispelRollCheck",
	[412] = "ServerStatusDownedChangedEvent",
	[413] = "ServerStatusRefreshed",
	[414] = "ServerStatusRemoveEvent",
	[415] = "ServerStatusScheduledForDeletion",
	[416] = "ServerStatusEvent",
	[417] = "ServerStatusTurnStartEvent",
	[418] = "ServerStatusUpdateTargetTracking",
	[419] = "TadpoleTreeState",
	[420] = "TadpolePowers",
	[421] = "Tadpoled",
	[422] = "HalfIllithid",
	[423] = "FullIllithid",
	[424] = "ShortRest",
	[425] = "GlobalShortRestDisabled",
	[426] = "GlobalLongRestDisabled",
	[427] = "StoryShortRestDisabled",
	[428] = "CanDoRest",
	[429] = "LongRestInScriptPhase",
	[430] = "LongRestState",
	[431] = "LongRestTimeline",
	[432] = "LongRestTimers",
	[433] = "LongRestUsers",
	[434] = "RestingEntities",
	[435] = "ShortRestConsumeResources",
	[436] = "ServerRestPendingType",
	[437] = "ServerRestTypeChosenEvent",
	[438] = "ServerShortRestResultEvent",
	[439] = "CCCompanionDefinition",
	[440] = "CCDefinitionCommon",
	[441] = "CCFullRespecDefinition",
	[442] = "CCRespecDefinition",
	[443] = "CCChangeAppearanceDefinition",
	[444] = "CCCharacterDefinition",
	[445] = "CCSessionCommon",
	[446] = "CCState",
	[447] = "CCLevelUp",
	[448] = "CCLevelUpDefinition",
	[449] = "CCFullRespec",
	[450] = "CCRespec",
	[451] = "CCCreation",
	[452] = "CCDefinitionLevelUp",
	[453] = "ClientCCDefinitionState",
	[454] = "ClientCCDefinitionStateEx",
	[455] = "ClientCCBaseDefinitionState",
	[456] = "ClientCCCompanionDefinition",
	[457] = "ClientCCLevelUpDefinition",
	[458] = "ClientCCChangeAppearanceDefinition",
	[459] = "ClientCCFullRespecDefinition",
	[460] = "ClientCCDummyDefinition",
	[461] = "ServerCCCustomIcon",
	[462] = "ServerCCAppearanceVisualTag",
	[463] = "ServerCCEquipmentSetRequest",
	[464] = "ServerCCGod",
	[465] = "ServerCCUpdates",
	[466] = "ServerCCIsCustom",
	[467] = "TriggerArea",
	[468] = "TriggerType",
	[469] = "TriggerContainer",
	[470] = "TriggerIsInsideOf",
	[471] = "TriggerUpdatedContainer",
	[472] = "TriggerUpdatedPhysics",
	[473] = "ServerStatsAreaTrigger",
	[474] = "ServerSoundVolumeTrigger",
	[475] = "ServerAIHintAreaTrigger",
	[476] = "ServerCrowdCharacterTrigger",
	[477] = "ServerChasmSeederTrigger",
	[478] = "ServerTeleportTrigger",
	[479] = "ServerCampRegionTrigger",
	[480] = "ServerEocAreaTrigger",
	[481] = "ServerStartTrigger",
	[482] = "ServerAtmosphereTrigger",
	[483] = "ServerCrimeAreaTrigger",
	[484] = "ServerCrimeRegionTrigger",
	[485] = "ServerEventTrigger",
	[486] = "ServerFloorTrigger",
	[487] = "ServerPortalTrigger",
	[488] = "ServerRoomTrigger",
	[489] = "ServerAvatarContainerTrigger",
	[490] = "ServerCampChestTrigger",
	[491] = "ServerChasmRegionTrigger",
	[492] = "ServerExplorationTrigger",
	[493] = "ServerMusicVolumeTrigger",
	[494] = "ServerMusicVolumeTriggerState",
	[495] = "ServerTimelineSceneTrigger",
	[496] = "ServerBlockBronzeTimelinePlacementTrigger",
	[497] = "ServerAiGridAreaTrigger",
	[498] = "ServerRegionTrigger",
	[499] = "ServerLightingTrigger",
	[500] = "ServerEocPointTrigger",
	[501] = "ServerTriggerEventConfig",
	[502] = "ServerTriggerRegisteredFor",
	[503] = "ServerRegistrationSettings",
	[504] = "ServerUpdatedRegisteredFor",
	[505] = "ServerUpdatedRegisteredForItems",
	[506] = "ServerTriggerCachedLeaveEvents",
	[507] = "ServerTriggerLoadedHandled",
	[508] = "ServerTriggerWorldAutoTriggered",
	[509] = "Background",
	[510] = "God",
	[511] = "Proficiency",
	[512] = "ProficiencyGroup",
	[513] = "LevelUp",
	[514] = "Floating",
	[515] = "Voice",
	[516] = "CustomIcon",
	[517] = "CharacterCreationStats",
	[518] = "Disarmable",
	[519] = "IsSummon",
	[520] = "SummonContainer",
	[521] = "Stealth",
	[522] = "ClientControl",
	[523] = "IsGlobal",
	[524] = "Savegame",
	[525] = "LootingState",
	[526] = "Loot",
	[527] = "Lock",
	[528] = "Key",
	[529] = "SummonLifetime",
	[530] = "Invisibility",
	[531] = "Icon",
	[532] = "HotbarContainer",
	[533] = "HotbarDecks",
	[534] = "OriginTag",
	[535] = "OriginPassives",
	[536] = "ClassTag",
	[537] = "BackgroundTag",
	[538] = "OriginAppearanceTag",
	[539] = "VoiceTag",
	[540] = "GodTag",
	[541] = "BackgroundPassives",
	[542] = "FleeCapability",
	[543] = "IsInTurnBasedMode",
	[544] = "ItemBoosts",
	[545] = "ActiveCharacterLight",
	[546] = "Visual",
	[547] = "ClientPaperdoll",
	[548] = "TLPreviewDummy",
	[549] = "TimelineActorData",
	[550] = "ClientTimelineActorControl",
	[551] = "AnimationSet",
	[552] = "AnimationUpdate",
	[553] = "AnimationBlueprint",
	[554] = "CanModifyHealth",
	[555] = "CanBeLooted",
	[556] = "CanBeDisarmed",
	[557] = "CanTriggerRandomCasts",
	[558] = "CanDoActions",
	[559] = "ActionUseConditions",
	[560] = "CanMove",
	[561] = "CanSense",
	[562] = "Concentration",
	[563] = "Darkness",
	[564] = "DualWielding",
	[565] = "GameObjectVisual",
	[566] = "AppearanceOverride",
	[567] = "CharacterCreationTemplateOverride",
	[568] = "GravityDisabled",
	[569] = "GravityDisabledUntilMoved",
	[570] = "Avatar",
	[571] = "HasExclamationDialog",
	[572] = "Trader",
	[573] = "CanTrade",
	[574] = "IsFalling",
	[575] = "HealMaxIncoming",
	[576] = "HealMaxOutgoing",
	[577] = "Ambushing",
	[578] = "InteractionFilter",
	[579] = "DisplayName",
	[580] = "CustomName",
	[581] = "Equipable",
	[582] = "GameplayLight",
	[583] = "Light",
	[584] = "CombinedLight",
	[585] = "Cull",
	[586] = "SceneAttach",
	[587] = "IsSeeThrough",
	[588] = "CustomIconsStorage",
	[589] = "FogVolumeRequest",
	[590] = "Race",
	[591] = "CanTravel",
	[592] = "Movement",
	[593] = "ObjectInteraction",
	[594] = "StaticPhysics",
	[595] = "Pathing",
	[596] = "PathingDistanceChanged",
	[597] = "Steering",
	[598] = "CanDeflectProjectiles",
	[599] = "ActiveSkeletonSlots",
	[600] = "Net",
	[601] = "Physics",
	[602] = "ApprovalRatings",
	[603] = "AttitudesToPlayers",
	[604] = "CharacterCreationAppearance",
	[605] = "Active",
	[606] = "Repose",
	[607] = "OriginalTemplate",
	[608] = "CanEnterChasm",
	[609] = "DialogState",
	[610] = "EncumbranceState",
	[611] = "EncumbranceStats",
	[612] = "Unsheath",
	[613] = "UseSocket",
	[614] = "UserAvatar",
	[615] = "UserReservedFor",
	[616] = "Sound",
	[617] = "PauseExcluded",
	[618] = "Pause",
	[619] = "BackgroundGoals",
	[620] = "CalendarDaysPassed",
	[621] = "CalendarStartingDate",
	[622] = "DisabledEquipment",
	[623] = "EquipmentVisual",
	[624] = "ClientEquipmentVisuals",
	[625] = "ClientEquipmentVisibilityState",
	[626] = "ClientVisualsDesiredState",
	[627] = "ClientVisualsVisibilityState",
	[628] = "ProgressionContainer",
	[629] = "ProgressionMeta",
	[630] = "ProgressionChangedContainers",
	[631] = "ProgressionLevelUpChanged",
	[632] = "ProgressionAbilityImprovements",
	[633] = "ProgressionFeat",
	[634] = "ProgressionSpells",
	[635] = "ProgressionSkills",
	[636] = "ProgressionPassives",
	[637] = "ProgressionReplicatedFeat",
	[638] = "CanSeeThrough",
	[639] = "CanShootThrough",
	[640] = "CanWalkThrough",
	[641] = "ShootThroughType",
	[642] = "ServerRecruitedBy",
	[643] = "ServerGameTimer",
	[644] = "ServerExperienceGaveOut",
	[645] = "ServerReplicationDependency",
	[646] = "ServerIsUnsummoning",
	[647] = "ServerActivationGroupContainer",
	[648] = "ServerAnubisTag",
	[649] = "ServerDialogTag",
	[650] = "ServerDisplayNameList",
	[651] = "ServerIconList",
	[652] = "ServerRaceTag",
	[653] = "ServerTemplateTag",
	[654] = "ServerBoostTag",
	[655] = "ServerSafePosition",
	[656] = "ServerAnubisExecutor",
	[657] = "ServerLeader",
	[658] = "ServerBreadcrumb",
	[659] = "ServerDelayDeathCause",
	[660] = "ServerPickpocket",
	[661] = "ServerReplicationDependencyOwner",
	[662] = "ServerGameplayLightEquipment",
	[663] = "HistoryTargetUUID",
	[664] = "ServerGameplayLightChanges",
	[665] = "ServerPeersInRange",
	[666] = "ServerSurface",
	[667] = "ServerDisarmAttempt",
	[668] = "ServerUnsheath",
	[669] = "ServerUnsheathScriptOverride",
	[670] = "ServerIsLightBlocker",
	[671] = "ServerIsVisionBlocker",
	[672] = "ServerDarknessActive",
	[673] = "ServerHotbarOrder",
	[674] = "ServerInventoryItemDataPopulated",
	[675] = "ServerVariableManager",
	[676] = "IsMarkedForDeletion",
	[677] = "JumpFollow",
	[678] = "ServerTemplateChangedEvent",
	[679] = "ServerTemplateTransformedEvent",
	[680] = "TagsChangedEvent",
	[681] = "SummonCreatedEvent",
	[682] = "SummonAddConcentrationRequest",
	[683] = "SummonAddToExistingConcentrationRequest",
	[684] = "SummonAttachToProjectileRequest",
	[685] = "SummonOwnerSetEvent",
	[686] = "SummonPlaceInInventoryRequest",
	[687] = "SummonSetLifetimeRequest",
	[688] = "SummonDespawnRequest",
	[689] = "SummonExpiredRequest",
	[690] = "SummonLateJoinPenalty",
	[691] = "ServerToggledPassives",
	[692] = "ServerPassiveBoosts",
	[693] = "ServerPassiveBase",
	[694] = "ServerPassivePersistentData",
	[695] = "ServerScriptPassives",
	[696] = "PassiveRequestTargetTracking",
	[697] = "PassiveUpdateTargetTracking",
	[698] = "PassiveConditionalRollInterruptEvent",
	[699] = "PassivesUpdatedEvent",
	[700] = "PasssiveUsageCountIncrementedEvent",
	[701] = "Construction",
	[702] = "ConstructionTile",
	[703] = "ConstructionFilling",
	[704] = "ServerCharacter",
	[705] = "ServerItem",
	[706] = "ProjectileSource",
	[707] = "ServerProjectile",
	[708] = "ServerProjectileAttachment",
	[709] = "ServerProjectileSpell",
	[710] = "ServerProjectileInitialization",
	[711] = "ServerProjectileCache",
	[712] = "ServerOsirisTag",
	[713] = "ProjectileImpactEvent",
	[714] = "ProjectileRequestTargetTracking",
	[715] = "ProjectileUpdateTargetTracking",
	[716] = "ProjectileSplitThrowableObjectRequest",
	[717] = "ClientCharacterIconRequest",
	[718] = "ClientCharacterIconResult",
	[719] = "ClientCharacter",
	[720] = "ClientItem",
	[721] = "ClientProjectile",
	[722] = "Scenery",
	[723] = "RequestedRoll",
	[724] = "RollInProgress",
	[725] = "ServerRollFinishedEvent",
	[726] = "RollModifiers",
	[727] = "ServerRollInProgress",
	[728] = "ServerRollStartRequest",
	[729] = "ServerRollStartSpellRequest",
	[730] = "ArmorClassBoost",
	[731] = "AbilityBoost",
	[732] = "RollBonusBoost",
	[733] = "AdvantageBoost",
	[734] = "ActionResourceValueBoost",
	[735] = "CriticalHitBoost",
	[736] = "AbilityFailedSavingThrowBoost",
	[737] = "ResistanceBoost",
	[738] = "WeaponDamageResistanceBoost",
	[739] = "ProficiencyBonusOverrideBoost",
	[740] = "JumpMaxDistanceMultiplierBoost",
	[741] = "HalveWeaponDamageBoost",
	[742] = "UnlockSpellBoost",
	[743] = "SourceAdvantageBoost",
	[744] = "ProficiencyBonusBoost",
	[745] = "ProficiencyBoost",
	[746] = "IncreaseMaxHPBoost",
	[747] = "ActionResourceBlockBoost",
	[748] = "StatusImmunityBoost",
	[749] = "UseBoosts",
	[750] = "CannotHarmCauseEntityBoost",
	[751] = "TemporaryHPBoost",
	[752] = "WeightBoost",
	[753] = "WeightCategoryBoost",
	[754] = "FactionOverrideBoost",
	[755] = "ActionResourceMultiplierBoost",
	[756] = "BlockRegainHPBoost",
	[757] = "InitiativeBoost",
	[758] = "DarkvisionRangeBoost",
	[759] = "DarkvisionRangeMinBoost",
	[760] = "DarkvisionRangeOverrideBoost",
	[761] = "AddTagBoost",
	[762] = "IgnoreDamageThresholdMinBoost",
	[763] = "SkillBoost",
	[764] = "WeaponDamageBoost",
	[765] = "NullifyAbilityBoost",
	[766] = "RerollBoost",
	[767] = "DownedStatusBoost",
	[768] = "WeaponEnchantmentBoost",
	[769] = "GuaranteedChanceRollOutcomeBoost",
	[770] = "AttributeBoost",
	[771] = "GameplayLightBoost",
	[772] = "DualWieldingBoost",
	[773] = "SavantBoost",
	[774] = "MinimumRollResultBoost",
	[775] = "CharacterWeaponDamageBoost",
	[776] = "ProjectileDeflectBoost",
	[777] = "AbilityOverrideMinimumBoost",
	[778] = "ACOverrideFormulaBoost",
	[779] = "FallDamageMultiplierBoost",
	[780] = "ActiveCharacterLightBoost",
	[781] = "WeaponAttackTypeOverrideBoost",
	[782] = "WeaponDamageDieOverrideBoost",
	[783] = "CarryCapacityMultiplierBoost",
	[784] = "WeaponPropertyBoost",
	[785] = "WeaponAttackRollAbilityOverrideBoost",
	[786] = "SightRangeAdditiveBoost",
	[787] = "SightRangeMinimumBoost",
	[788] = "SightRangeMaximumBoost",
	[789] = "SightRangeOverrideBoost",
	[790] = "MovementSpeedLimitBoost",
	[791] = "UnlockSpellVariantBoost",
	[792] = "DetectCrimesBlockBoost",
	[793] = "BlockAbilityModifierFromACBoost",
	[794] = "ScaleMultiplierBoost",
	[795] = "DamageReductionBoost",
	[796] = "ReduceCriticalAttackThresholdBoost",
	[797] = "PhysicalForceRangeBonusBoost",
	[798] = "ObjectSizeBoost",
	[799] = "ObjectSizeOverrideBoost",
	[800] = "AiArchetypeOverrideBoost",
	[801] = "ExpertiseBonusBoost",
	[802] = "EntityThrowDamageBoost",
	[803] = "WeaponDamageTypeOverrideBoost",
	[804] = "MaximizeHealingBoost",
	[805] = "DamageBonusBoost",
	[806] = "AdvanceSpellsBoost",
	[807] = "SpellResistanceBoost",
	[808] = "WeaponAttackRollBonusBoost",
	[809] = "SpellSaveDCBoost",
	[810] = "RedirectDamageBoost",
	[811] = "CanSeeThroughBoost",
	[812] = "CanShootThroughBoost",
	[813] = "CanWalkThroughBoost",
	[814] = "MonkWeaponDamageDiceOverrideBoost",
	[815] = "HorizontalFOVOverrideBoost",
	[816] = "CharacterUnarmedDamageBoost",
	[817] = "ActionResourceReplenishTypeOverrideBoost",
	[818] = "ActionResourcePreventReductionBoost",
	[819] = "AttackSpellOverrideBoost",
	[820] = "LockBoost",
	[821] = "IgnorePointBlankDisadvantageBoost",
	[822] = "CriticalHitExtraDiceBoost",
	[823] = "DodgeAttackRollBoost",
	[824] = "GameplayObscurityBoost",
	[825] = "MaximumRollResultBoost",
	[826] = "UnlockInterruptBoost",
	[827] = "JumpMaxDistanceBonusBoost",
	[828] = "ArmorAbilityModifierCapOverrideBoost",
	[829] = "IgnoreResistanceBoost",
	[830] = "ConcentrationIgnoreDamageBoost",
	[831] = "IgnoreLowGroundPenaltyBoost",
	[832] = "IgnoreSurfaceCoverBoost",
	[833] = "ProficiencyBonusIncreaseBoost",
	[834] = "DamageTakenBonusBoost",
	[835] = "ReceivingCriticalDamageOnHitBoost",
	[836] = "Uuid",
	[837] = "UuidToHandleMapping",
	[838] = "Max",
}

--- @enum ExtQueryType
Ext_Enums.ExtQueryType = {
	UuidToHandleMapping = 0,
	Max = 1,
	[0] = "UuidToHandleMapping",
	[1] = "Max",
}

--- @enum ExtResourceManagerType
Ext_Enums.ExtResourceManagerType = {
	AbilityDistributionPreset = 0,
	AbilityList = 1,
	ActionResourceGroup = 2,
	ActionResource = 3,
	ApprovalRating = 4,
	AreaLevelOverride = 5,
	AvatarContainerTemplate = 6,
	BackgroundGoal = 7,
	Background = 8,
	CalendarDayRange = 9,
	CampChestTemplate = 10,
	AbilityDefaultValues = 11,
	EquipmentDefaultValues = 12,
	FeatDefaultValues = 13,
	PassiveDefaultValues = 14,
	PreparedSpellDefaultValues = 15,
	SkillDefaultValues = 16,
	SpellDefaultValues = 17,
	CharacterCreationAccessorySet = 18,
	CharacterCreationAppearanceMaterial = 19,
	CharacterCreationAppearanceVisual = 20,
	CharacterCreationEquipmentIcons = 21,
	CharacterCreationEyeColor = 22,
	CharacterCreationIconSettings = 23,
	CharacterCreationHairColor = 24,
	CharacterCreationMaterialOverride = 25,
	CharacterCreationPassiveAppearance = 26,
	CharacterCreationPreset = 27,
	CharacterCreationSharedVisual = 28,
	CharacterCreationSkinColor = 29,
	CharacterCreationVOLine = 30,
	CinematicArenaFrequencyGroup = 31,
	ClassDescription = 32,
	ColorDefinition = 33,
	CompanionPreset = 34,
	CustomDice = 35,
	ConditionError = 36,
	DeathTypeEffect = 37,
	DifficultyClass = 38,
	DisturbanceProperty = 39,
	DLC = 40,
	EncumbranceType = 41,
	EquipmentList = 42,
	EquipmentType = 43,
	ExperienceReward = 44,
	Faction = 45,
	FeatSoundState = 46,
	Feat = 47,
	FeatDescription = 48,
	FlagSoundState = 49,
	God = 50,
	GoldReward = 51,
	Gossip = 52,
	FixedHotBarSlot = 53,
	ItemThrowParams = 54,
	ItemWallTemplate = 55,
	TrajectoryRules = 56,
	LevelMap = 57,
	LimbsMapping = 58,
	LongRestCost = 59,
	ManagedStatusVFX = 60,
	MultiEffectInfo = 61,
	OriginIntroEntity = 62,
	Origin = 63,
	PassiveList = 64,
	PassiveVFX = 65,
	ProgressionDescription = 66,
	Progression = 67,
	ProjectileDefault = 68,
	Race = 69,
	RandomCastOutcome = 70,
	SkillList = 71,
	SpellSoundTrajectory = 72,
	SpellList = 73,
	StatusSoundState = 74,
	SurfaceCursorMessage = 75,
	TadpolePower = 76,
	TagSoundState = 77,
	TooltipExtraText = 78,
	TooltipUpcastDescription = 79,
	TutorialEntries = 80,
	TutorialModalEntries = 81,
	TutorialUnifiedEntry = 82,
	TutorialEvent = 83,
	VFX = 84,
	Voice = 85,
	WeaponAnimationSet = 86,
	WeightCategory = 87,
	ShapeshiftRule = 88,
	AnimationSetPriority = 89,
	SpellMetaCondition = 90,
	ScriptMaterialParameterOverride = 91,
	ScriptMaterialPresetOverride = 92,
	VisualLocatorAttachment = 93,
	Flag = 94,
	Tag = 95,
	RulesetModifier = 96,
	RulesetModifierOption = 97,
	Ruleset = 98,
	RulesetValue = 99,
	RulesetSelectionPreset = 100,
	Max = 101,
	[0] = "AbilityDistributionPreset",
	[1] = "AbilityList",
	[2] = "ActionResourceGroup",
	[3] = "ActionResource",
	[4] = "ApprovalRating",
	[5] = "AreaLevelOverride",
	[6] = "AvatarContainerTemplate",
	[7] = "BackgroundGoal",
	[8] = "Background",
	[9] = "CalendarDayRange",
	[10] = "CampChestTemplate",
	[11] = "AbilityDefaultValues",
	[12] = "EquipmentDefaultValues",
	[13] = "FeatDefaultValues",
	[14] = "PassiveDefaultValues",
	[15] = "PreparedSpellDefaultValues",
	[16] = "SkillDefaultValues",
	[17] = "SpellDefaultValues",
	[18] = "CharacterCreationAccessorySet",
	[19] = "CharacterCreationAppearanceMaterial",
	[20] = "CharacterCreationAppearanceVisual",
	[21] = "CharacterCreationEquipmentIcons",
	[22] = "CharacterCreationEyeColor",
	[23] = "CharacterCreationIconSettings",
	[24] = "CharacterCreationHairColor",
	[25] = "CharacterCreationMaterialOverride",
	[26] = "CharacterCreationPassiveAppearance",
	[27] = "CharacterCreationPreset",
	[28] = "CharacterCreationSharedVisual",
	[29] = "CharacterCreationSkinColor",
	[30] = "CharacterCreationVOLine",
	[31] = "CinematicArenaFrequencyGroup",
	[32] = "ClassDescription",
	[33] = "ColorDefinition",
	[34] = "CompanionPreset",
	[35] = "CustomDice",
	[36] = "ConditionError",
	[37] = "DeathTypeEffect",
	[38] = "DifficultyClass",
	[39] = "DisturbanceProperty",
	[40] = "DLC",
	[41] = "EncumbranceType",
	[42] = "EquipmentList",
	[43] = "EquipmentType",
	[44] = "ExperienceReward",
	[45] = "Faction",
	[46] = "FeatSoundState",
	[47] = "Feat",
	[48] = "FeatDescription",
	[49] = "FlagSoundState",
	[50] = "God",
	[51] = "GoldReward",
	[52] = "Gossip",
	[53] = "FixedHotBarSlot",
	[54] = "ItemThrowParams",
	[55] = "ItemWallTemplate",
	[56] = "TrajectoryRules",
	[57] = "LevelMap",
	[58] = "LimbsMapping",
	[59] = "LongRestCost",
	[60] = "ManagedStatusVFX",
	[61] = "MultiEffectInfo",
	[62] = "OriginIntroEntity",
	[63] = "Origin",
	[64] = "PassiveList",
	[65] = "PassiveVFX",
	[66] = "ProgressionDescription",
	[67] = "Progression",
	[68] = "ProjectileDefault",
	[69] = "Race",
	[70] = "RandomCastOutcome",
	[71] = "SkillList",
	[72] = "SpellSoundTrajectory",
	[73] = "SpellList",
	[74] = "StatusSoundState",
	[75] = "SurfaceCursorMessage",
	[76] = "TadpolePower",
	[77] = "TagSoundState",
	[78] = "TooltipExtraText",
	[79] = "TooltipUpcastDescription",
	[80] = "TutorialEntries",
	[81] = "TutorialModalEntries",
	[82] = "TutorialUnifiedEntry",
	[83] = "TutorialEvent",
	[84] = "VFX",
	[85] = "Voice",
	[86] = "WeaponAnimationSet",
	[87] = "WeightCategory",
	[88] = "ShapeshiftRule",
	[89] = "AnimationSetPriority",
	[90] = "SpellMetaCondition",
	[91] = "ScriptMaterialParameterOverride",
	[92] = "ScriptMaterialPresetOverride",
	[93] = "VisualLocatorAttachment",
	[94] = "Flag",
	[95] = "Tag",
	[96] = "RulesetModifier",
	[97] = "RulesetModifierOption",
	[98] = "Ruleset",
	[99] = "RulesetValue",
	[100] = "RulesetSelectionPreset",
	[101] = "Max",
}

--- @enum ExtSystemType
Ext_Enums.ExtSystemType = {
	PickingHelper = 0,
	ServerDialog = 1,
	AnimationBlueprint = 2,
	ServerShapeshift = 3,
	ServerCastRequest = 4,
	ServerSpell = 5,
	ServerCombat = 6,
	ServerTurnOrder = 7,
	ServerActionResource = 8,
	ServerBoost = 9,
	ServerPassive = 10,
	ServerRoll = 11,
	ServerStats = 12,
	ServerVisual = 13,
	ServerSpellCooldown = 14,
	ServerSpellLearning = 15,
	ServerHit = 16,
	ServerProgression = 17,
	ServerInterruptDecision = 18,
	ServerInterruptRequests = 19,
	ServerInterruptManagement = 20,
	ServerSummonSpawn = 21,
	ServerSummonDespawn = 22,
	ServerCapabilities = 23,
	ServerTemplateChange = 24,
	ServerConcentration = 25,
	ServerExperience = 26,
	ServerShortRest = 27,
	ServerLongRest = 28,
	ServerFTBZone = 29,
	ClientEquipmentVisuals = 30,
	ClientVisual = 31,
	ClientCharacterIconRender = 32,
	ClientCharacterManager = 33,
	ClientVisualsVisibilityState = 34,
	SoundRouting = 35,
	Max = 36,
	[0] = "PickingHelper",
	[1] = "ServerDialog",
	[2] = "AnimationBlueprint",
	[3] = "ServerShapeshift",
	[4] = "ServerCastRequest",
	[5] = "ServerSpell",
	[6] = "ServerCombat",
	[7] = "ServerTurnOrder",
	[8] = "ServerActionResource",
	[9] = "ServerBoost",
	[10] = "ServerPassive",
	[11] = "ServerRoll",
	[12] = "ServerStats",
	[13] = "ServerVisual",
	[14] = "ServerSpellCooldown",
	[15] = "ServerSpellLearning",
	[16] = "ServerHit",
	[17] = "ServerProgression",
	[18] = "ServerInterruptDecision",
	[19] = "ServerInterruptRequests",
	[20] = "ServerInterruptManagement",
	[21] = "ServerSummonSpawn",
	[22] = "ServerSummonDespawn",
	[23] = "ServerCapabilities",
	[24] = "ServerTemplateChange",
	[25] = "ServerConcentration",
	[26] = "ServerExperience",
	[27] = "ServerShortRest",
	[28] = "ServerLongRest",
	[29] = "ServerFTBZone",
	[30] = "ClientEquipmentVisuals",
	[31] = "ClientVisual",
	[32] = "ClientCharacterIconRender",
	[33] = "ClientCharacterManager",
	[34] = "ClientVisualsVisibilityState",
	[35] = "SoundRouting",
	[36] = "Max",
}

--- @enum FleeErrorFlags
Ext_Enums.FleeErrorFlags = {
	Region = 1,
	EnemyTooClose = 2,
	CantMove = 4,
	Downed = 8,
	Incapacitated = 16,
	Condition = 32,
	Summon = 64,
	PartyFollower = 128,
	[1] = "Region",
	[2] = "EnemyTooClose",
	[4] = "CantMove",
	[8] = "Downed",
	[16] = "Incapacitated",
	[32] = "Condition",
	[64] = "Summon",
	[128] = "PartyFollower",
}

--- @enum ForceFunctorAggression
Ext_Enums.ForceFunctorAggression = {
	Aggressive = 0,
	Friendly = 1,
	Neutral = 2,
	Undefined = 3,
	[0] = "Aggressive",
	[1] = "Friendly",
	[2] = "Neutral",
	[3] = "Undefined",
}

--- @enum ForceFunctorOrigin
Ext_Enums.ForceFunctorOrigin = {
	OriginToEntity = 0,
	OriginToTarget = 1,
	TargetToEntity = 2,
	Undefined = 3,
	[0] = "OriginToEntity",
	[1] = "OriginToTarget",
	[2] = "TargetToEntity",
	[3] = "Undefined",
}

--- @enum FunctorContextType
Ext_Enums.FunctorContextType = {
	None = 0,
	AttackTarget = 1,
	AttackPosition = 2,
	Move = 3,
	Target = 4,
	NearbyAttacked = 5,
	NearbyAttacking = 6,
	Equip = 7,
	Source = 8,
	Interrupt = 9,
	[0] = "None",
	[1] = "AttackTarget",
	[2] = "AttackPosition",
	[3] = "Move",
	[4] = "Target",
	[5] = "NearbyAttacked",
	[6] = "NearbyAttacking",
	[7] = "Equip",
	[8] = "Source",
	[9] = "Interrupt",
}

--- @enum GameActionType
Ext_Enums.GameActionType = {
	RainAction = 1,
	StormAction = 2,
	WallAction = 4,
	TornadoAction = 6,
	PathAction = 7,
	GameObjectMoveAction = 8,
	StatusDomeAction = 9,
	[1] = "RainAction",
	[2] = "StormAction",
	[4] = "WallAction",
	[6] = "TornadoAction",
	[7] = "PathAction",
	[8] = "GameObjectMoveAction",
	[9] = "StatusDomeAction",
}

--- @enum Gender
Ext_Enums.Gender = {
	Male = 0,
	Female = 1,
	[0] = "Male",
	[1] = "Female",
}

--- @enum GuiButtonFlags
Ext_Enums.GuiButtonFlags = {
	MouseButtonLeft = 1,
	MouseButtonRight = 2,
	MouseButtonMiddle = 4,
	[1] = "MouseButtonLeft",
	[2] = "MouseButtonRight",
	[4] = "MouseButtonMiddle",
}

--- @enum GuiChildFlags
Ext_Enums.GuiChildFlags = {
	Border = 1,
	AlwaysUseWindowPadding = 2,
	ResizeX = 4,
	ResizeY = 8,
	AutoResizeX = 16,
	AutoResizeY = 32,
	ChildAlwaysAutoResize = 64,
	FrameStyle = 128,
	NavFlattened = 256,
	[1] = "Border",
	[2] = "AlwaysUseWindowPadding",
	[4] = "ResizeX",
	[8] = "ResizeY",
	[16] = "AutoResizeX",
	[32] = "AutoResizeY",
	[64] = "ChildAlwaysAutoResize",
	[128] = "FrameStyle",
	[256] = "NavFlattened",
}

--- @enum GuiColor
Ext_Enums.GuiColor = {
	Text = 0,
	TextDisabled = 1,
	WindowBg = 2,
	ChildBg = 3,
	PopupBg = 4,
	Border = 5,
	BorderShadow = 6,
	FrameBg = 7,
	FrameBgHovered = 8,
	FrameBgActive = 9,
	TitleBg = 10,
	TitleBgActive = 11,
	TitleBgCollapsed = 12,
	MenuBarBg = 13,
	ScrollbarBg = 14,
	ScrollbarGrab = 15,
	ScrollbarGrabHovered = 16,
	ScrollbarGrabActive = 17,
	CheckMark = 18,
	SliderGrab = 19,
	SliderGrabActive = 20,
	Button = 21,
	ButtonHovered = 22,
	ButtonActive = 23,
	Header = 24,
	HeaderHovered = 25,
	HeaderActive = 26,
	Separator = 27,
	SeparatorHovered = 28,
	SeparatorActive = 29,
	ResizeGrip = 30,
	ResizeGripHovered = 31,
	ResizeGripActive = 32,
	TabHovered = 33,
	Tab = 34,
	TabActive = 35,
	TabUnfocused = 37,
	TabUnfocusedActive = 38,
	TabDimmedSelectedOverline = 39,
	PlotLines = 40,
	PlotLinesHovered = 41,
	PlotHistogram = 42,
	PlotHistogramHovered = 43,
	TableHeaderBg = 44,
	TableBorderStrong = 45,
	TableBorderLight = 46,
	TableRowBg = 47,
	TableRowBgAlt = 48,
	TextLink = 49,
	TextSelectedBg = 50,
	DragDropTarget = 51,
	NavHighlight = 52,
	NavWindowingHighlight = 53,
	NavWindowingDimBg = 54,
	ModalWindowDimBg = 55,
	[0] = "Text",
	[1] = "TextDisabled",
	[2] = "WindowBg",
	[3] = "ChildBg",
	[4] = "PopupBg",
	[5] = "Border",
	[6] = "BorderShadow",
	[7] = "FrameBg",
	[8] = "FrameBgHovered",
	[9] = "FrameBgActive",
	[10] = "TitleBg",
	[11] = "TitleBgActive",
	[12] = "TitleBgCollapsed",
	[13] = "MenuBarBg",
	[14] = "ScrollbarBg",
	[15] = "ScrollbarGrab",
	[16] = "ScrollbarGrabHovered",
	[17] = "ScrollbarGrabActive",
	[18] = "CheckMark",
	[19] = "SliderGrab",
	[20] = "SliderGrabActive",
	[21] = "Button",
	[22] = "ButtonHovered",
	[23] = "ButtonActive",
	[24] = "Header",
	[25] = "HeaderHovered",
	[26] = "HeaderActive",
	[27] = "Separator",
	[28] = "SeparatorHovered",
	[29] = "SeparatorActive",
	[30] = "ResizeGrip",
	[31] = "ResizeGripHovered",
	[32] = "ResizeGripActive",
	[33] = "TabHovered",
	[34] = "Tab",
	[35] = "TabActive",
	[37] = "TabUnfocused",
	[38] = "TabUnfocusedActive",
	[39] = "TabDimmedSelectedOverline",
	[40] = "PlotLines",
	[41] = "PlotLinesHovered",
	[42] = "PlotHistogram",
	[43] = "PlotHistogramHovered",
	[44] = "TableHeaderBg",
	[45] = "TableBorderStrong",
	[46] = "TableBorderLight",
	[47] = "TableRowBg",
	[48] = "TableRowBgAlt",
	[49] = "TextLink",
	[50] = "TextSelectedBg",
	[51] = "DragDropTarget",
	[52] = "NavHighlight",
	[53] = "NavWindowingHighlight",
	[54] = "NavWindowingDimBg",
	[55] = "ModalWindowDimBg",
}

--- @enum GuiColorEditFlags
Ext_Enums.GuiColorEditFlags = {
	AlphaPreview = 0,
	NoAlpha = 2,
	NoPicker = 4,
	NoOptions = 8,
	NoSmallPreview = 16,
	NoInputs = 32,
	NoTooltip = 64,
	NoLabel = 128,
	NoSidePreview = 256,
	NoDragDrop = 512,
	NoBorder = 1024,
	AlphaOpaque = 2048,
	AlphaNoBg = 4096,
	AlphaPreviewHalf = 8192,
	AlphaBar = 65536,
	HDR = 524288,
	DisplayRGB = 1048576,
	DisplayHSV = 2097152,
	DisplayHex = 4194304,
	Uint8 = 8388608,
	Float = 16777216,
	PickerHueBar = 33554432,
	PickerHueWheel = 67108864,
	InputRGB = 134217728,
	InputHSV = 268435456,
	[0] = "AlphaPreview",
	[2] = "NoAlpha",
	[4] = "NoPicker",
	[8] = "NoOptions",
	[16] = "NoSmallPreview",
	[32] = "NoInputs",
	[64] = "NoTooltip",
	[128] = "NoLabel",
	[256] = "NoSidePreview",
	[512] = "NoDragDrop",
	[1024] = "NoBorder",
	[2048] = "AlphaOpaque",
	[4096] = "AlphaNoBg",
	[8192] = "AlphaPreviewHalf",
	[65536] = "AlphaBar",
	[524288] = "HDR",
	[1048576] = "DisplayRGB",
	[2097152] = "DisplayHSV",
	[4194304] = "DisplayHex",
	[8388608] = "Uint8",
	[16777216] = "Float",
	[33554432] = "PickerHueBar",
	[67108864] = "PickerHueWheel",
	[134217728] = "InputRGB",
	[268435456] = "InputHSV",
}

--- @enum GuiComboFlags
Ext_Enums.GuiComboFlags = {
	PopupAlignLeft = 1,
	HeightSmall = 2,
	HeightRegular = 4,
	HeightLarge = 8,
	HeightLargest = 16,
	NoArrowButton = 32,
	NoPreview = 64,
	WidthFitPreview = 128,
	[1] = "PopupAlignLeft",
	[2] = "HeightSmall",
	[4] = "HeightRegular",
	[8] = "HeightLarge",
	[16] = "HeightLargest",
	[32] = "NoArrowButton",
	[64] = "NoPreview",
	[128] = "WidthFitPreview",
}

--- @enum GuiCond
Ext_Enums.GuiCond = {
	Always = 1,
	Once = 2,
	FirstUseEver = 4,
	Appearing = 8,
	[1] = "Always",
	[2] = "Once",
	[4] = "FirstUseEver",
	[8] = "Appearing",
}

--- @enum GuiDragFlags
Ext_Enums.GuiDragFlags = {
	NoPreviewTooltip = 1,
	NoDisableHover = 2,
	NoHoldToOpenOthers = 4,
	AllowNullID = 8,
	[1] = "NoPreviewTooltip",
	[2] = "NoDisableHover",
	[4] = "NoHoldToOpenOthers",
	[8] = "AllowNullID",
}

--- @enum GuiDropFlags
Ext_Enums.GuiDropFlags = {
	NoDrawDefaultRect = 2048,
	NoPreviewTooltip = 4096,
	[2048] = "NoDrawDefaultRect",
	[4096] = "NoPreviewTooltip",
}

--- @enum GuiInputTextFlags
Ext_Enums.GuiInputTextFlags = {
	CharsDecimal = 1,
	CharsHexadecimal = 2,
	CharsScientific = 4,
	CharsUppercase = 8,
	CharsNoBlank = 16,
	AllowTabInput = 32,
	EnterReturnsTrue = 64,
	EscapeClearsAll = 128,
	CtrlEnterForNewLine = 256,
	ReadOnly = 512,
	Password = 1024,
	AlwaysOverwrite = 2048,
	AutoSelectAll = 4096,
	ParseEmptyRefVal = 8192,
	DisplayEmptyRefVal = 16384,
	NoHorizontalScroll = 32768,
	NoUndoRedo = 65536,
	ElideLeft = 131072,
	Multiline = 67108864,
	[1] = "CharsDecimal",
	[2] = "CharsHexadecimal",
	[4] = "CharsScientific",
	[8] = "CharsUppercase",
	[16] = "CharsNoBlank",
	[32] = "AllowTabInput",
	[64] = "EnterReturnsTrue",
	[128] = "EscapeClearsAll",
	[256] = "CtrlEnterForNewLine",
	[512] = "ReadOnly",
	[1024] = "Password",
	[2048] = "AlwaysOverwrite",
	[4096] = "AutoSelectAll",
	[8192] = "ParseEmptyRefVal",
	[16384] = "DisplayEmptyRefVal",
	[32768] = "NoHorizontalScroll",
	[65536] = "NoUndoRedo",
	[131072] = "ElideLeft",
	[67108864] = "Multiline",
}

--- @enum GuiItemFlags
Ext_Enums.GuiItemFlags = {
	NoTabStop = 1,
	NoItemNav = 2,
	NoNavDefaultFocus = 4,
	ButtonRepeat = 8,
	AutoClosePopups = 16,
	AllowDuplicateId = 32,
	Disabled = 1024,
	ItemReadOnly = 2048,
	NoWindowHoverableCheck = 8192,
	AllowItemOverlap = 16384,
	NoNavDisableMouseHover = 32768,
	NoMarkEdited = 65536,
	[1] = "NoTabStop",
	[2] = "NoItemNav",
	[4] = "NoNavDefaultFocus",
	[8] = "ButtonRepeat",
	[16] = "AutoClosePopups",
	[32] = "AllowDuplicateId",
	[1024] = "Disabled",
	[2048] = "ItemReadOnly",
	[8192] = "NoWindowHoverableCheck",
	[16384] = "AllowItemOverlap",
	[32768] = "NoNavDisableMouseHover",
	[65536] = "NoMarkEdited",
}

--- @enum GuiItemStatusFlags
Ext_Enums.GuiItemStatusFlags = {
	HoveredRect = 1,
	HasDisplayRect = 2,
	Edited = 4,
	ToggledSelection = 8,
	ToggledOpen = 16,
	HasDeactivated = 32,
	Deactivated = 64,
	HoveredWindow = 128,
	Visible = 256,
	HasClipRect = 512,
	HasShortcut = 1024,
	Focused = 1048576,
	Active = 2097152,
	[1] = "HoveredRect",
	[2] = "HasDisplayRect",
	[4] = "Edited",
	[8] = "ToggledSelection",
	[16] = "ToggledOpen",
	[32] = "HasDeactivated",
	[64] = "Deactivated",
	[128] = "HoveredWindow",
	[256] = "Visible",
	[512] = "HasClipRect",
	[1024] = "HasShortcut",
	[1048576] = "Focused",
	[2097152] = "Active",
}

--- @enum GuiMeasureScaling
Ext_Enums.GuiMeasureScaling = {
	Absolute = 0,
	Scaled = 1,
	[0] = "Absolute",
	[1] = "Scaled",
}

--- @enum GuiPopupFlags
Ext_Enums.GuiPopupFlags = {
	MouseButtonRight = 1,
	MouseButtonMiddle = 2,
	NoReopen = 32,
	NoOpenOverExistingPopup = 128,
	NoOpenOverItems = 256,
	AnyPopupId = 1024,
	AnyPopupLevel = 2048,
	[1] = "MouseButtonRight",
	[2] = "MouseButtonMiddle",
	[32] = "NoReopen",
	[128] = "NoOpenOverExistingPopup",
	[256] = "NoOpenOverItems",
	[1024] = "AnyPopupId",
	[2048] = "AnyPopupLevel",
}

--- @enum GuiSelectableFlags
Ext_Enums.GuiSelectableFlags = {
	DontClosePopups = 1,
	SpanAllColumns = 2,
	AllowDoubleClick = 4,
	SelectableDisabled = 8,
	AllowOverlap = 16,
	Highlight = 32,
	[1] = "DontClosePopups",
	[2] = "SpanAllColumns",
	[4] = "AllowDoubleClick",
	[8] = "SelectableDisabled",
	[16] = "AllowOverlap",
	[32] = "Highlight",
}

--- @enum GuiSliderFlags
Ext_Enums.GuiSliderFlags = {
	Logarithmic = 32,
	NoRoundToFormat = 64,
	NoInput = 128,
	WrapAround = 256,
	ClampOnInput = 512,
	ClampZeroRange = 1024,
	AlwaysClamp = 1536,
	NoSpeedTweaks = 2048,
	[32] = "Logarithmic",
	[64] = "NoRoundToFormat",
	[128] = "NoInput",
	[256] = "WrapAround",
	[512] = "ClampOnInput",
	[1024] = "ClampZeroRange",
	[1536] = "AlwaysClamp",
	[2048] = "NoSpeedTweaks",
}

--- @enum GuiSortDirection
Ext_Enums.GuiSortDirection = {
	None = 0,
	Ascending = 1,
	Descending = 2,
	[0] = "None",
	[1] = "Ascending",
	[2] = "Descending",
}

--- @enum GuiStyleVar
Ext_Enums.GuiStyleVar = {
	Alpha = 0,
	DisabledAlpha = 1,
	WindowPadding = 2,
	WindowRounding = 3,
	WindowBorderSize = 4,
	WindowMinSize = 5,
	WindowTitleAlign = 6,
	ChildRounding = 7,
	ChildBorderSize = 8,
	PopupRounding = 9,
	PopupBorderSize = 10,
	FramePadding = 11,
	FrameRounding = 12,
	FrameBorderSize = 13,
	ItemSpacing = 14,
	ItemInnerSpacing = 15,
	IndentSpacing = 16,
	CellPadding = 17,
	ScrollbarSize = 18,
	ScrollbarRounding = 19,
	GrabMinSize = 20,
	GrabRounding = 21,
	ImageBorderSize = 22,
	TabRounding = 23,
	TabBorderSize = 24,
	TabBarBorderSize = 25,
	TabBarOverlineSize = 26,
	TableAngledHeadersAngle = 27,
	TableAngledHeadersTextAlign = 28,
	ButtonTextAlign = 29,
	SelectableTextAlign = 30,
	SeparatorTextBorderSize = 31,
	SeparatorTextAlign = 32,
	SeparatorTextPadding = 33,
	[0] = "Alpha",
	[1] = "DisabledAlpha",
	[2] = "WindowPadding",
	[3] = "WindowRounding",
	[4] = "WindowBorderSize",
	[5] = "WindowMinSize",
	[6] = "WindowTitleAlign",
	[7] = "ChildRounding",
	[8] = "ChildBorderSize",
	[9] = "PopupRounding",
	[10] = "PopupBorderSize",
	[11] = "FramePadding",
	[12] = "FrameRounding",
	[13] = "FrameBorderSize",
	[14] = "ItemSpacing",
	[15] = "ItemInnerSpacing",
	[16] = "IndentSpacing",
	[17] = "CellPadding",
	[18] = "ScrollbarSize",
	[19] = "ScrollbarRounding",
	[20] = "GrabMinSize",
	[21] = "GrabRounding",
	[22] = "ImageBorderSize",
	[23] = "TabRounding",
	[24] = "TabBorderSize",
	[25] = "TabBarBorderSize",
	[26] = "TabBarOverlineSize",
	[27] = "TableAngledHeadersAngle",
	[28] = "TableAngledHeadersTextAlign",
	[29] = "ButtonTextAlign",
	[30] = "SelectableTextAlign",
	[31] = "SeparatorTextBorderSize",
	[32] = "SeparatorTextAlign",
	[33] = "SeparatorTextPadding",
}

--- @enum GuiTabBarFlags
Ext_Enums.GuiTabBarFlags = {
	Reorderable = 1,
	AutoSelectNewTabs = 2,
	TabListPopupButton = 4,
	NoCloseWithMiddleMouseButton = 8,
	NoTabListScrollingButtons = 16,
	NoTooltip = 32,
	DrawSelectedOverline = 64,
	FittingPolicyResizeDown = 128,
	FittingPolicyScroll = 256,
	[1] = "Reorderable",
	[2] = "AutoSelectNewTabs",
	[4] = "TabListPopupButton",
	[8] = "NoCloseWithMiddleMouseButton",
	[16] = "NoTabListScrollingButtons",
	[32] = "NoTooltip",
	[64] = "DrawSelectedOverline",
	[128] = "FittingPolicyResizeDown",
	[256] = "FittingPolicyScroll",
}

--- @enum GuiTabItemFlags
Ext_Enums.GuiTabItemFlags = {
	UnsavedDocument = 1,
	SetSelected = 2,
	NoCloseWithMiddleMouseButton = 4,
	NoPushId = 8,
	NoTooltip = 16,
	NoReorder = 32,
	Leading = 64,
	Trailing = 128,
	NoAssumedClosure = 256,
	[1] = "UnsavedDocument",
	[2] = "SetSelected",
	[4] = "NoCloseWithMiddleMouseButton",
	[8] = "NoPushId",
	[16] = "NoTooltip",
	[32] = "NoReorder",
	[64] = "Leading",
	[128] = "Trailing",
	[256] = "NoAssumedClosure",
}

--- @enum GuiTableColumnFlags
Ext_Enums.GuiTableColumnFlags = {
	Disabled = 1,
	DefaultHide = 2,
	DefaultSort = 4,
	WidthStretch = 8,
	WidthFixed = 16,
	NoResize = 32,
	NoReorder = 64,
	NoHide = 128,
	NoClip = 256,
	NoSort = 512,
	NoSortAscending = 1024,
	NoSortDescending = 2048,
	NoHeaderLabel = 4096,
	NoHeaderWidth = 8192,
	PreferSortAscending = 16384,
	PreferSortDescending = 32768,
	IndentEnable = 65536,
	IndentDisable = 131072,
	AngledHeader = 262144,
	[1] = "Disabled",
	[2] = "DefaultHide",
	[4] = "DefaultSort",
	[8] = "WidthStretch",
	[16] = "WidthFixed",
	[32] = "NoResize",
	[64] = "NoReorder",
	[128] = "NoHide",
	[256] = "NoClip",
	[512] = "NoSort",
	[1024] = "NoSortAscending",
	[2048] = "NoSortDescending",
	[4096] = "NoHeaderLabel",
	[8192] = "NoHeaderWidth",
	[16384] = "PreferSortAscending",
	[32768] = "PreferSortDescending",
	[65536] = "IndentEnable",
	[131072] = "IndentDisable",
	[262144] = "AngledHeader",
}

--- @enum GuiTableFlags
Ext_Enums.GuiTableFlags = {
	Resizable = 1,
	Reorderable = 2,
	Hideable = 4,
	Sortable = 8,
	NoSavedSettings = 16,
	ContextMenuInBody = 32,
	RowBg = 64,
	BordersInnerH = 128,
	BordersOuterH = 256,
	BordersH = 384,
	BordersInnerV = 512,
	BordersInner = 640,
	BordersOuterV = 1024,
	BordersOuter = 1280,
	BordersV = 1536,
	Borders = 1920,
	NoBordersInBody = 2048,
	NoBordersInBodyUntilResize = 4096,
	SizingFixedFit = 8192,
	SizingFixedSame = 16384,
	SizingStretchProp = 24576,
	SizingStretchSame = 32768,
	NoHostExtendX = 65536,
	NoHostExtendY = 131072,
	NoKeepColumnsVisible = 262144,
	PreciseWidths = 524288,
	NoClip = 1048576,
	PadOuterX = 2097152,
	NoPadOuterX = 4194304,
	NoPadInnerX = 8388608,
	ScrollX = 16777216,
	ScrollY = 33554432,
	SortMulti = 67108864,
	SortTristate = 134217728,
	HighlightHoveredColumn = 268435456,
	[1] = "Resizable",
	[2] = "Reorderable",
	[4] = "Hideable",
	[8] = "Sortable",
	[16] = "NoSavedSettings",
	[32] = "ContextMenuInBody",
	[64] = "RowBg",
	[128] = "BordersInnerH",
	[256] = "BordersOuterH",
	[384] = "BordersH",
	[512] = "BordersInnerV",
	[640] = "BordersInner",
	[1024] = "BordersOuterV",
	[1280] = "BordersOuter",
	[1536] = "BordersV",
	[1920] = "Borders",
	[2048] = "NoBordersInBody",
	[4096] = "NoBordersInBodyUntilResize",
	[8192] = "SizingFixedFit",
	[16384] = "SizingFixedSame",
	[24576] = "SizingStretchProp",
	[32768] = "SizingStretchSame",
	[65536] = "NoHostExtendX",
	[131072] = "NoHostExtendY",
	[262144] = "NoKeepColumnsVisible",
	[524288] = "PreciseWidths",
	[1048576] = "NoClip",
	[2097152] = "PadOuterX",
	[4194304] = "NoPadOuterX",
	[8388608] = "NoPadInnerX",
	[16777216] = "ScrollX",
	[33554432] = "ScrollY",
	[67108864] = "SortMulti",
	[134217728] = "SortTristate",
	[268435456] = "HighlightHoveredColumn",
}

--- @enum GuiTableRowFlags
Ext_Enums.GuiTableRowFlags = {
	Headers = 1,
	[1] = "Headers",
}

--- @enum GuiTreeNodeFlags
Ext_Enums.GuiTreeNodeFlags = {
	Selected = 1,
	Framed = 2,
	AllowOverlap = 4,
	NoTreePushOnOpen = 8,
	NoAutoOpenOnLog = 16,
	CollapsingHeader = 26,
	DefaultOpen = 32,
	OpenOnDoubleClick = 64,
	OpenOnArrow = 128,
	Leaf = 256,
	Bullet = 512,
	FramePadding = 1024,
	SpanAvailWidth = 2048,
	SpanFullWidth = 4096,
	SpanTextWidth = 8192,
	SpanAllColumns = 16384,
	NavLeftJumpsBackHere = 131072,
	[1] = "Selected",
	[2] = "Framed",
	[4] = "AllowOverlap",
	[8] = "NoTreePushOnOpen",
	[16] = "NoAutoOpenOnLog",
	[26] = "CollapsingHeader",
	[32] = "DefaultOpen",
	[64] = "OpenOnDoubleClick",
	[128] = "OpenOnArrow",
	[256] = "Leaf",
	[512] = "Bullet",
	[1024] = "FramePadding",
	[2048] = "SpanAvailWidth",
	[4096] = "SpanFullWidth",
	[8192] = "SpanTextWidth",
	[16384] = "SpanAllColumns",
	[131072] = "NavLeftJumpsBackHere",
}

--- @enum GuiWindowFlags
Ext_Enums.GuiWindowFlags = {
	NoTitleBar = 1,
	NoResize = 2,
	NoMove = 4,
	NoScrollbar = 8,
	NoScrollWithMouse = 16,
	NoCollapse = 32,
	NoDecoration = 43,
	AlwaysAutoResize = 64,
	NoBackground = 128,
	NoSavedSettings = 256,
	NoMouseInputs = 512,
	MenuBar = 1024,
	HorizontalScrollbar = 2048,
	NoFocusOnAppearing = 4096,
	NoBringToFrontOnFocus = 8192,
	AlwaysVerticalScrollbar = 16384,
	AlwaysHorizontalScrollbar = 32768,
	NoNavInputs = 65536,
	NoNavFocus = 131072,
	NoNav = 196608,
	NoInputs = 197120,
	UnsavedDocument = 262144,
	[1] = "NoTitleBar",
	[2] = "NoResize",
	[4] = "NoMove",
	[8] = "NoScrollbar",
	[16] = "NoScrollWithMouse",
	[32] = "NoCollapse",
	[43] = "NoDecoration",
	[64] = "AlwaysAutoResize",
	[128] = "NoBackground",
	[256] = "NoSavedSettings",
	[512] = "NoMouseInputs",
	[1024] = "MenuBar",
	[2048] = "HorizontalScrollbar",
	[4096] = "NoFocusOnAppearing",
	[8192] = "NoBringToFrontOnFocus",
	[16384] = "AlwaysVerticalScrollbar",
	[32768] = "AlwaysHorizontalScrollbar",
	[65536] = "NoNavInputs",
	[131072] = "NoNavFocus",
	[196608] = "NoNav",
	[197120] = "NoInputs",
	[262144] = "UnsavedDocument",
}

--- @enum HandednessType
Ext_Enums.HandednessType = {
	Any = 0,
	One = 1,
	Two = 2,
	[0] = "Any",
	[1] = "One",
	[2] = "Two",
}

--- @enum HealDirection
Ext_Enums.HealDirection = {
	Incoming = 0,
	Outgoing = 1,
	[0] = "Incoming",
	[1] = "Outgoing",
}

--- @enum HealEffect
Ext_Enums.HealEffect = {
	None = 0,
	ResistDeath = 1,
	Heal = 2,
	Behavior = 3,
	Unknown4 = 4,
	Sitting = 5,
	Surface = 6,
	Lifesteal = 7,
	NegativeDamage = 8,
	Unknown9 = 9,
	Script = 10,
	HealSharing = 11,
	Necromantic = 12,
	HealSharingReflected = 13,
	[0] = "None",
	[1] = "ResistDeath",
	[2] = "Heal",
	[3] = "Behavior",
	[4] = "Unknown4",
	[5] = "Sitting",
	[6] = "Surface",
	[7] = "Lifesteal",
	[8] = "NegativeDamage",
	[9] = "Unknown9",
	[10] = "Script",
	[11] = "HealSharing",
	[12] = "Necromantic",
	[13] = "HealSharingReflected",
}

--- @enum HitType
Ext_Enums.HitType = {
	Melee = 0,
	Magic = 1,
	Ranged = 2,
	WeaponDamage = 3,
	Surface = 4,
	DoT = 5,
	Reflected = 6,
	[0] = "Melee",
	[1] = "Magic",
	[2] = "Ranged",
	[3] = "WeaponDamage",
	[4] = "Surface",
	[5] = "DoT",
	[6] = "Reflected",
}

--- @enum HitWith
Ext_Enums.HitWith = {
	None = 0,
	Weapon = 1,
	Magic = 2,
	Item = 3,
	Projectile = 4,
	Trap = 5,
	Surface = 6,
	Redirection = 7,
	FallDamage = 8,
	CrushByFall = 9,
	Unknown10 = 10,
	Unknown11 = 11,
	[0] = "None",
	[1] = "Weapon",
	[2] = "Magic",
	[3] = "Item",
	[4] = "Projectile",
	[5] = "Trap",
	[6] = "Surface",
	[7] = "Redirection",
	[8] = "FallDamage",
	[9] = "CrushByFall",
	[10] = "Unknown10",
	[11] = "Unknown11",
}

--- @enum IngredientTransformType
Ext_Enums.IngredientTransformType = {
	None = 0,
	Consume = 1,
	Transform = 2,
	Boost = 3,
	Poison = 4,
	[0] = "None",
	[1] = "Consume",
	[2] = "Transform",
	[3] = "Boost",
	[4] = "Poison",
}

--- @enum IngredientType
Ext_Enums.IngredientType = {
	None = 0,
	Object = 1,
	Category = 2,
	Property = 3,
	[0] = "None",
	[1] = "Object",
	[2] = "Category",
	[3] = "Property",
}

--- @enum InputModifier
Ext_Enums.InputModifier = {
	Alt = 1,
	Ctrl = 2,
	Shift = 4,
	Gui = 8,
	[1] = "Alt",
	[2] = "Ctrl",
	[4] = "Shift",
	[8] = "Gui",
}

--- @enum InputRawType
Ext_Enums.InputRawType = {
	enter = 0,
	escape = 1,
	backspace = 2,
	tab = 3,
	capslock = 4,
	space = 5,
	printscreen = 6,
	scrolllock = 7,
	pause = 8,
	insert = 9,
	home = 10,
	pageup = 11,
	del = 12,
	end = 13,
	pagedown = 14,
	comma = 15,
	hyphen = 16,
	dot = 17,
	slash = 18,
	space2 = 19,
	equals = 20,
	leftbracket = 21,
	backslash = 22,
	rightbracket = 23,
	backtick = 24,
	apostrophe = 25,
	num_0 = 26,
	num_1 = 27,
	num_2 = 28,
	num_3 = 29,
	num_4 = 30,
	num_5 = 31,
	num_6 = 32,
	num_7 = 33,
	num_8 = 34,
	num_9 = 35,
	a = 36,
	b = 37,
	c = 38,
	d = 39,
	e = 40,
	f = 41,
	g = 42,
	h = 43,
	i = 44,
	j = 45,
	k = 46,
	l = 47,
	m = 48,
	n = 49,
	o = 50,
	p = 51,
	q = 52,
	r = 53,
	s = 54,
	t = 55,
	u = 56,
	v = 57,
	w = 58,
	x = 59,
	y = 60,
	z = 61,
	f1 = 62,
	f2 = 63,
	f3 = 64,
	f4 = 65,
	f5 = 66,
	f6 = 67,
	f7 = 68,
	f8 = 69,
	f9 = 70,
	f10 = 71,
	f11 = 72,
	f12 = 73,
	f13 = 74,
	f14 = 75,
	f15 = 76,
	f16 = 77,
	f17 = 78,
	f18 = 79,
	f19 = 80,
	f20 = 81,
	f21 = 82,
	f22 = 83,
	f23 = 84,
	f24 = 85,
	right = 86,
	left = 87,
	down = 88,
	up = 89,
	numlock = 90,
	kp_divide = 91,
	kp_multiply = 92,
	kp_minus = 93,
	kp_plus = 94,
	kp_enter = 95,
	kp_1 = 96,
	kp_2 = 97,
	kp_3 = 98,
	kp_4 = 99,
	kp_5 = 100,
	kp_6 = 101,
	kp_7 = 102,
	kp_8 = 103,
	kp_9 = 104,
	kp_0 = 105,
	kp_period = 106,
	lctrl = 107,
	lshift = 108,
	lalt = 109,
	lgui = 110,
	rctrl = 111,
	rshift = 112,
	ralt = 113,
	rgui = 114,
	mode = 115,
	volume_up = 116,
	volume_down = 117,
	audio_next = 118,
	audio_prev = 119,
	audio_stop = 120,
	audio_play = 121,
	audio_mute = 122,
	left2 = 123,
	middle = 124,
	right2 = 125,
	x1 = 126,
	x2 = 127,
	motion = 128,
	motion_xneg = 129,
	motion_ypos = 130,
	motion_xpos = 131,
	motion_yneg = 132,
	wheel_xpos = 133,
	wheel_xneg = 134,
	wheel_ypos = 135,
	wheel_yneg = 136,
	leftstick = 137,
	leftstick_x = 138,
	leftstick_y = 139,
	leftstick_xneg = 140,
	leftstick_ypos = 141,
	leftstick_xpos = 142,
	leftstick_yneg = 143,
	rightstick = 144,
	rightstick_x = 145,
	rightstick_xneg = 146,
	rightstick_y = 147,
	rightstick_ypos = 148,
	rightstick_xpos = 149,
	rightstick_yneg = 150,
	lefttrigger = 151,
	righttrigger = 152,
	controller_a = 153,
	controller_b = 154,
	controller_x = 155,
	controller_y = 156,
	back = 157,
	guide = 158,
	start = 159,
	leftstickpress = 160,
	rightstickpress = 161,
	leftshoulder = 162,
	rightshoulder = 163,
	dpad_up = 164,
	dpad_down = 165,
	dpad_left = 166,
	dpad_right = 167,
	touch_tap = 168,
	touch_hold = 169,
	touch_pinch_in = 170,
	touch_pinch_out = 171,
	touch_rotate = 172,
	touch_flick = 173,
	touch_press = 174,
	item1 = 175,
	item2 = 176,
	item3 = 177,
	item4 = 178,
	item5 = 179,
	item6 = 180,
	item7 = 181,
	item8 = 182,
	item9 = 183,
	item10 = 184,
	item11 = 185,
	[0] = "enter",
	[1] = "escape",
	[2] = "backspace",
	[3] = "tab",
	[4] = "capslock",
	[5] = "space",
	[6] = "printscreen",
	[7] = "scrolllock",
	[8] = "pause",
	[9] = "insert",
	[10] = "home",
	[11] = "pageup",
	[12] = "del",
	[13] = "end",
	[14] = "pagedown",
	[15] = "comma",
	[16] = "hyphen",
	[17] = "dot",
	[18] = "slash",
	[19] = "space2",
	[20] = "equals",
	[21] = "leftbracket",
	[22] = "backslash",
	[23] = "rightbracket",
	[24] = "backtick",
	[25] = "apostrophe",
	[26] = "num_0",
	[27] = "num_1",
	[28] = "num_2",
	[29] = "num_3",
	[30] = "num_4",
	[31] = "num_5",
	[32] = "num_6",
	[33] = "num_7",
	[34] = "num_8",
	[35] = "num_9",
	[36] = "a",
	[37] = "b",
	[38] = "c",
	[39] = "d",
	[40] = "e",
	[41] = "f",
	[42] = "g",
	[43] = "h",
	[44] = "i",
	[45] = "j",
	[46] = "k",
	[47] = "l",
	[48] = "m",
	[49] = "n",
	[50] = "o",
	[51] = "p",
	[52] = "q",
	[53] = "r",
	[54] = "s",
	[55] = "t",
	[56] = "u",
	[57] = "v",
	[58] = "w",
	[59] = "x",
	[60] = "y",
	[61] = "z",
	[62] = "f1",
	[63] = "f2",
	[64] = "f3",
	[65] = "f4",
	[66] = "f5",
	[67] = "f6",
	[68] = "f7",
	[69] = "f8",
	[70] = "f9",
	[71] = "f10",
	[72] = "f11",
	[73] = "f12",
	[74] = "f13",
	[75] = "f14",
	[76] = "f15",
	[77] = "f16",
	[78] = "f17",
	[79] = "f18",
	[80] = "f19",
	[81] = "f20",
	[82] = "f21",
	[83] = "f22",
	[84] = "f23",
	[85] = "f24",
	[86] = "right",
	[87] = "left",
	[88] = "down",
	[89] = "up",
	[90] = "numlock",
	[91] = "kp_divide",
	[92] = "kp_multiply",
	[93] = "kp_minus",
	[94] = "kp_plus",
	[95] = "kp_enter",
	[96] = "kp_1",
	[97] = "kp_2",
	[98] = "kp_3",
	[99] = "kp_4",
	[100] = "kp_5",
	[101] = "kp_6",
	[102] = "kp_7",
	[103] = "kp_8",
	[104] = "kp_9",
	[105] = "kp_0",
	[106] = "kp_period",
	[107] = "lctrl",
	[108] = "lshift",
	[109] = "lalt",
	[110] = "lgui",
	[111] = "rctrl",
	[112] = "rshift",
	[113] = "ralt",
	[114] = "rgui",
	[115] = "mode",
	[116] = "volume_up",
	[117] = "volume_down",
	[118] = "audio_next",
	[119] = "audio_prev",
	[120] = "audio_stop",
	[121] = "audio_play",
	[122] = "audio_mute",
	[123] = "left2",
	[124] = "middle",
	[125] = "right2",
	[126] = "x1",
	[127] = "x2",
	[128] = "motion",
	[129] = "motion_xneg",
	[130] = "motion_ypos",
	[131] = "motion_xpos",
	[132] = "motion_yneg",
	[133] = "wheel_xpos",
	[134] = "wheel_xneg",
	[135] = "wheel_ypos",
	[136] = "wheel_yneg",
	[137] = "leftstick",
	[138] = "leftstick_x",
	[139] = "leftstick_y",
	[140] = "leftstick_xneg",
	[141] = "leftstick_ypos",
	[142] = "leftstick_xpos",
	[143] = "leftstick_yneg",
	[144] = "rightstick",
	[145] = "rightstick_x",
	[146] = "rightstick_xneg",
	[147] = "rightstick_y",
	[148] = "rightstick_ypos",
	[149] = "rightstick_xpos",
	[150] = "rightstick_yneg",
	[151] = "lefttrigger",
	[152] = "righttrigger",
	[153] = "controller_a",
	[154] = "controller_b",
	[155] = "controller_x",
	[156] = "controller_y",
	[157] = "back",
	[158] = "guide",
	[159] = "start",
	[160] = "leftstickpress",
	[161] = "rightstickpress",
	[162] = "leftshoulder",
	[163] = "rightshoulder",
	[164] = "dpad_up",
	[165] = "dpad_down",
	[166] = "dpad_left",
	[167] = "dpad_right",
	[168] = "touch_tap",
	[169] = "touch_hold",
	[170] = "touch_pinch_in",
	[171] = "touch_pinch_out",
	[172] = "touch_rotate",
	[173] = "touch_flick",
	[174] = "touch_press",
	[175] = "item1",
	[176] = "item2",
	[177] = "item3",
	[178] = "item4",
	[179] = "item5",
	[180] = "item6",
	[181] = "item7",
	[182] = "item8",
	[183] = "item9",
	[184] = "item10",
	[185] = "item11",
}

--- @enum InputState
Ext_Enums.InputState = {
	Released = 0,
	Pressed = 1,
	[0] = "Released",
	[1] = "Pressed",
}

--- @enum InputType
Ext_Enums.InputType = {
	Press = 1,
	Release = 2,
	ValueChange = 4,
	Hold = 8,
	Repeat = 16,
	AcceleratedRepeat = 32,
	ClearOnRelease = 64,
	[1] = "Press",
	[2] = "Release",
	[4] = "ValueChange",
	[8] = "Hold",
	[16] = "Repeat",
	[32] = "AcceleratedRepeat",
	[64] = "ClearOnRelease",
}

--- @enum InterruptRequestSource
Ext_Enums.InterruptRequestSource = {
	Boost = 0,
	Debug = 1,
	LearnedSpell = 2,
	[0] = "Boost",
	[1] = "Debug",
	[2] = "LearnedSpell",
}

--- @enum IsTradableType
Ext_Enums.IsTradableType = {
	Default = 0,
	Tradable = 1,
	NonTradable = 2,
	[0] = "Default",
	[1] = "Tradable",
	[2] = "NonTradable",
}

--- @enum ItemDataRarity
Ext_Enums.ItemDataRarity = {
	Common = 0,
	Unique = 1,
	Uncommon = 2,
	Rare = 3,
	Epic = 4,
	Legendary = 5,
	Divine = 6,
	Sentinel = 7,
	[0] = "Common",
	[1] = "Unique",
	[2] = "Uncommon",
	[3] = "Rare",
	[4] = "Epic",
	[5] = "Legendary",
	[6] = "Divine",
	[7] = "Sentinel",
}

--- @enum ItemSlot
Ext_Enums.ItemSlot = {
	Helmet = 0,
	Breast = 1,
	Cloak = 2,
	MeleeMainHand = 3,
	MeleeOffHand = 4,
	RangedMainHand = 5,
	RangedOffHand = 6,
	Ring = 7,
	Underwear = 8,
	Boots = 9,
	Gloves = 10,
	Amulet = 11,
	Ring2 = 12,
	Wings = 13,
	Horns = 14,
	Overhead = 15,
	MusicalInstrument = 16,
	VanityBody = 17,
	VanityBoots = 18,
	MainHand = 19,
	OffHand = 20,
	[0] = "Helmet",
	[1] = "Breast",
	[2] = "Cloak",
	[3] = "MeleeMainHand",
	[4] = "MeleeOffHand",
	[5] = "RangedMainHand",
	[6] = "RangedOffHand",
	[7] = "Ring",
	[8] = "Underwear",
	[9] = "Boots",
	[10] = "Gloves",
	[11] = "Amulet",
	[12] = "Ring2",
	[13] = "Wings",
	[14] = "Horns",
	[15] = "Overhead",
	[16] = "MusicalInstrument",
	[17] = "VanityBody",
	[18] = "VanityBoots",
	[19] = "MainHand",
	[20] = "OffHand",
}

--- @enum ItemUseType
Ext_Enums.ItemUseType = {
	None = 0,
	Unknown1 = 1,
	Grenade = 2,
	Arrow = 3,
	Scroll = 4,
	Potion = 5,
	Throwable = 6,
	Consumable = 7,
	[0] = "None",
	[1] = "Unknown1",
	[2] = "Grenade",
	[3] = "Arrow",
	[4] = "Scroll",
	[5] = "Potion",
	[6] = "Throwable",
	[7] = "Consumable",
}

--- @enum LongRestState
Ext_Enums.LongRestState = {
	Created = 0,
	Validation = 1,
	RestTypeRequest = 2,
	ScriptPhaseStart = 3,
	ScriptPhaseRequested = 4,
	ScriptPhasePending = 5,
	ScriptPhaseTimeout = 6,
	ScriptPhaseDenied = 7,
	Cancelled = 8,
	Finished = 9,
	ReadyForDelete = 10,
	[0] = "Created",
	[1] = "Validation",
	[2] = "RestTypeRequest",
	[3] = "ScriptPhaseStart",
	[4] = "ScriptPhaseRequested",
	[5] = "ScriptPhasePending",
	[6] = "ScriptPhaseTimeout",
	[7] = "ScriptPhaseDenied",
	[8] = "Cancelled",
	[9] = "Finished",
	[10] = "ReadyForDelete",
}

--- @enum LongRestType
Ext_Enums.LongRestType = {
	None = 0,
	Full = 1,
	Exhausted = 2,
	CanceledByOwner = 3,
	CanceledByObserver = 4,
	CantRest = 5,
	[0] = "None",
	[1] = "Full",
	[2] = "Exhausted",
	[3] = "CanceledByOwner",
	[4] = "CanceledByObserver",
	[5] = "CantRest",
}

--- @enum LongRestUserState
Ext_Enums.LongRestUserState = {
	Initial = 0,
	StartingRest = 1,
	RequestingData = 2,
	Resting = 3,
	WaitingConfirmation = 4,
	WaitingOthers = 5,
	Finishing = 6,
	Finished = 7,
	[0] = "Initial",
	[1] = "StartingRest",
	[2] = "RequestingData",
	[3] = "Resting",
	[4] = "WaitingConfirmation",
	[5] = "WaitingOthers",
	[6] = "Finishing",
	[7] = "Finished",
}

--- @enum LuaTypeId
Ext_Enums.LuaTypeId = {
	Unknown = 0,
	Void = 1,
	Boolean = 2,
	Integer = 3,
	Float = 4,
	String = 5,
	Object = 6,
	Array = 7,
	Set = 8,
	Map = 9,
	Tuple = 10,
	Enumeration = 11,
	Function = 12,
	Any = 13,
	Nullable = 14,
	Module = 15,
	Variant = 16,
	[0] = "Unknown",
	[1] = "Void",
	[2] = "Boolean",
	[3] = "Integer",
	[4] = "Float",
	[5] = "String",
	[6] = "Object",
	[7] = "Array",
	[8] = "Set",
	[9] = "Map",
	[10] = "Tuple",
	[11] = "Enumeration",
	[12] = "Function",
	[13] = "Any",
	[14] = "Nullable",
	[15] = "Module",
	[16] = "Variant",
}

--- @enum MultiEffectFlags
Ext_Enums.MultiEffectFlags = {
	KeepRotation = 1,
	KeepScale = 2,
	UseOrientDirection = 4,
	UseDistance = 8,
	UseScaleOverride = 16,
	DetachSource = 32,
	DetachTarget = 64,
	MainHand = 128,
	OffHand = 256,
	Enabled = 512,
	[1] = "KeepRotation",
	[2] = "KeepScale",
	[4] = "UseOrientDirection",
	[8] = "UseDistance",
	[16] = "UseScaleOverride",
	[32] = "DetachSource",
	[64] = "DetachTarget",
	[128] = "MainHand",
	[256] = "OffHand",
	[512] = "Enabled",
}

--- @enum NarrativeCombatRequestType
Ext_Enums.NarrativeCombatRequestType = {
	Create = 0,
	Destroy = 1,
	SetInNarrativeCombat = 2,
	ClearInNarrativeCombat = 3,
	[0] = "Create",
	[1] = "Destroy",
	[2] = "SetInNarrativeCombat",
	[3] = "ClearInNarrativeCombat",
}

--- @enum NetMessage
Ext_Enums.NetMessage = {
	NETMSG_HANDSHAKE = 1,
	NETMSG_HOST_WELCOME = 2,
	NETMSG_HOST_REFUSE = 3,
	NETMSG_HOST_REFUSEPLAYER = 4,
	NETMSG_HOST_LEFT = 5,
	NETMSG_CLIENT_CONNECT = 6,
	NETMSG_CLIENT_ACCEPT = 7,
	NETMSG_CLIENT_JOINED = 8,
	NETMSG_CLIENT_LEFT = 9,
	NETMSG_PLAYER_CONNECT = 10,
	NETMSG_PLAYER_DISCONNECT = 11,
	NETMSG_PLAYER_ACCEPT = 12,
	NETMSG_PLAYER_JOINED = 13,
	NETMSG_PLAYER_LEFT = 14,
	NETMSG_PLAYER_CHANGENAME = 15,
	NETMSG_PLAYER_NAMECHANGED = 16,
	NETMSG_ECS_REPLICATION = 17,
	NETMSG_VOICEDATA = 18,
	NETMSG_SKIPMOVIE_RESULT = 19,
	NETMSG_MIC_DISABLED = 20,
	NETMSG_SURFACE_CREATE = 22,
	NETMSG_SURFACE_META = 23,
	NETMSG_DARKNESSTILE_UPDATE = 24,
	NETMSG_GAMEACTION = 25,
	NETMSG_CHARACTER_CREATE = 26,
	NETMSG_CHARACTER_DESTROY = 27,
	NETMSG_CHARACTER_CONFIRMATION = 28,
	NETMSG_CHARACTER_ACTIVATE = 29,
	NETMSG_CHARACTER_DEACTIVATE = 30,
	NETMSG_CHARACTER_ASSIGN = 31,
	NETMSG_CHARACTER_STEERING = 32,
	NETMSG_CHARACTER_ACTION = 33,
	NETMSG_CHARACTER_ACTION_DATA = 34,
	NETMSG_CHARACTER_ACTION_REQUEST_RESULT = 35,
	NETMSG_CHARACTER_STATUS = 36,
	NETMSG_CHARACTER_STATUS_LIFETIME = 37,
	NETMSG_CHARACTER_UPDATE = 38,
	NETMSG_CHARACTER_CONTROL = 39,
	NETMSG_CHARACTER_DIALOG = 40,
	NETMSG_CHARACTER_UNSHEATHING = 41,
	NETMSG_CHARACTER_END_REPOSE = 42,
	NETMSG_CHARACTER_USE_MOVEMENT = 43,
	NETMSG_CHARACTER_MOVE_FAILED = 44,
	NETMSG_CHARACTER_SELECTED_CLIMB_ON = 45,
	NETMSG_FOLLOWER_CANT_USE_ITEM = 46,
	NETMSG_CHARACTER_AOO = 47,
	NETMSG_CHARACTER_TRANSFORM = 48,
	NETMSG_CHARACTER_TELEPORT = 49,
	NETMSG_CHARACTER_PICKPOCKET = 50,
	NETMSG_CHARACTER_OFFSTAGE = 51,
	NETMSG_CHARACTER_LOOT = 52,
	NETMSG_CHARACTER_ITEM_USED = 53,
	NETMSG_CHARACTER_IN_DIALOG = 54,
	NETMSG_CHARACTER_BEHAVIOR = 55,
	NETMSG_PARTYGROUP = 60,
	NETMSG_PARTYORDER = 61,
	NETMSG_PARTY_BLOCKFOLLOW = 64,
	NETMSG_SHROUD_UPDATE = 65,
	NETMSG_ITEM_CREATE = 67,
	NETMSG_ITEM_DESTROY = 68,
	NETMSG_ITEM_ACTIVATE = 69,
	NETMSG_ITEM_DEACTIVATE = 70,
	NETMSG_ITEM_DESTINATION = 71,
	NETMSG_ITEM_UPDATE = 72,
	NETMSG_ITEM_USE_REMOTELY = 73,
	NETMSG_ITEM_MOVE_TO_WORLD = 74,
	NETMSG_ITEM_ACTION = 75,
	NETMSG_ITEM_STATUS = 76,
	NETMSG_ITEM_STATUS_LIFETIME = 77,
	NETMSG_ITEM_TRANSFORM = 78,
	NETMSG_ITEM_CONFIRMATION = 79,
	NETMSG_ITEM_OFFSTAGE = 80,
	NETMSG_ITEM_MOVED_INFORM = 81,
	NETMSG_INVENTORY_CREATE = 82,
	NETMSG_INVENTORY_CREATE_AND_OPEN = 83,
	NETMSG_INVENTORY_DESTROY = 84,
	NETMSG_INVENTORY_VIEW_CREATE = 85,
	NETMSG_INVENTORY_VIEW_DESTROY = 86,
	NETMSG_INVENTORY_VIEW_UPDATE_ITEMS = 87,
	NETMSG_INVENTORY_VIEW_UPDATE_PARENTS = 88,
	NETMSG_INVENTORY_VIEW_SORT = 89,
	NETMSG_INVENTORY_ITEM_UPDATE = 90,
	NETMSG_INVENTORY_LOCKSTATE_SYNC = 91,
	NETMSG_PROJECTILE_CREATE = 92,
	NETMSG_PROJECTILE_UPDATE = 93,
	NETMSG_TURNBASED_FINISHTEAM = 98,
	NETMSG_TURNBASED_SETTEAM = 99,
	NETMSG_TURNBASED_FLEECOMBATRESULT = 100,
	NETMSG_TURNBASED_FLEE_REQUEST = 101,
	NETMSG_TURNBASED_ENDTURN_REQUEST = 102,
	NETMSG_TURNBASED_CANCEL_ENDTURN_REQUEST = 103,
	NETMSG_TURNBASED_BEGIN_CANCEL_ENDTURN_REQUEST = 104,
	NETMSG_TURNBASED_END_CANCEL_ENDTURN_REQUEST = 105,
	NETMSG_TURNBASED_STARTTURN_CONFIRMATION = 106,
	NETMSG_TURNBASED_SKIP_START_DELAY = 107,
	NETMSG_UI_FORCETURNBASED_ENTERED = 108,
	NETMSG_UI_FORCETURNBASED_LEFT = 109,
	NETMSG_UI_FORCETURNBASED_TURN_STARTED = 110,
	NETMSG_UI_FORCETURNBASED_TURN_ENDED = 111,
	NETMSG_NET_ENTITY_CREATE = 116,
	NETMSG_NET_ENTITY_DESTROY = 117,
	NETMSG_PEER_ACTIVATE = 118,
	NETMSG_PEER_DEACTIVATE = 119,
	NETMSG_SPELL_LEARN = 120,
	NETMSG_SPELL_REMOVE_LEARNED = 121,
	NETMSG_SPELL_PREPARE = 122,
	NETMSG_SPELL_CANCEL = 123,
	NETMSG_TADPOLE_POWER_ADD = 124,
	NETMSG_TADPOLE_POWERS_REMOVE = 125,
	NETMSG_EFFECT_CREATE = 126,
	NETMSG_EFFECT_FORGET = 127,
	NETMSG_EFFECT_DESTROY = 128,
	NETMSG_GAMECONTROL_UPDATE_S2C = 129,
	NETMSG_GAMECONTROL_UPDATE_C2S = 130,
	NETMSG_GAMECONTROL_PRICETAG = 131,
	NETMSG_PLAYMOVIE = 132,
	NETMSG_CHARACTER_ERROR = 133,
	NETMSG_CACHETEMPLATE = 134,
	NETMSG_OVERHEADTEXT = 135,
	NETMSG_COMBATLOG = 136,
	NETMSG_COMBATLOGITEMINTERACTION = 137,
	NETMSG_COMBATLOGENTRIES = 138,
	NETMSG_SCREEN_FADE_FINISHED = 139,
	NETMSG_OPEN_CUSTOM_BOOK_UI_MESSAGE = 140,
	NETMSG_CLOSE_CUSTOM_BOOK_UI_MESSAGE = 141,
	NETMSG_OPEN_MESSAGE_BOX_MESSAGE = 142,
	NETMSG_CLOSED_MESSAGE_BOX_MESSAGE = 143,
	NETMSG_OPEN_WAYPOINT_UI_MESSAGE = 144,
	NETMSG_CLOSE_UI_MESSAGE = 145,
	NETMSG_SHOW_ENTER_REGION_UI_MESSAGE = 146,
	NETMSG_JOURNAL_RESET = 147,
	NETMSG_QUEST_STEP = 148,
	NETMSG_QUEST_CATEGORY_UPDATE = 149,
	NETMSG_QUEST_PROGRESS = 150,
	NETMSG_QUESTS_LOADED = 151,
	NETMSG_TROPHY_UPDATE = 152,
	NETMSG_JOURNALRECIPE_UPDATE = 153,
	NETMSG_MARKER_UI_CREATE = 154,
	NETMSG_MARKER_UI_UPDATE = 155,
	NETMSG_QUEST_TRACK = 156,
	NETMSG_UI_QUESTSELECTED = 157,
	NETMSG_MYSTERY_ADVANCED = 158,
	NETMSG_MYSTERY_DISABLED = 159,
	NETMSG_NOTIFICATION = 160,
	NETMSG_GAMEOVER = 161,
	NETMSG_ACT_OVER = 162,
	NETMSG_REGISTER_WAYPOINT = 163,
	NETMSG_FLAG_UPDATE = 164,
	NETMSG_DIALOG_STATE_MESSAGE = 168,
	NETMSG_DIALOG_NODE_MESSAGE = 169,
	NETMSG_DIALOG_ANSWER_MESSAGE = 170,
	NETMSG_DIALOG_ANSWER_HIGHLIGHT_MESSAGE = 171,
	NETMSG_DIALOG_HISTORY_MESSAGE = 172,
	NETMSG_DIALOG_ACTORJOINS_MESSAGE = 173,
	NETMSG_DIALOG_ACTORLEAVES_MESSAGE = 174,
	NETMSG_DIALOG_REPLACESPEAKER_MESSAGE = 175,
	NETMSG_DIALOG_SUGGESTANSWER_MESSAGE = 176,
	NETMSG_DIALOG_REQUEST_IN_DIALOGUE_ATTACK_MESSAGE = 177,
	NETMSG_DIALOG_TIMELINE_UNLOADED_MESSAGE = 178,
	NETMSG_TRADE_ACTION = 179,
	NETMSG_CAMERA_ACTIVATE = 180,
	NETMSG_CAMERA_ROTATE = 181,
	NETMSG_CAMERA_TARGET = 182,
	NETMSG_LIGHTING_OVERRIDE = 183,
	NETMSG_ATMOSPHERE_OVERRIDE = 184,
	NETMSG_CHAT = 185,
	NETMSG_GAMETIME_SYNC = 186,
	NETMSG_REQUESTAUTOSAVE = 188,
	NETMSG_SAVEGAME = 189,
	NETMSG_SAVEGAMEHANDSHAKE = 190,
	NETMSG_SAVEGAMEHANDSHAKE_SCREENSHOT = 191,
	NETMSG_MODULE_LOAD = 192,
	NETMSG_MODULE_LOADED = 193,
	NETMSG_SESSION_LOAD = 194,
	NETMSG_SESSION_LOADED = 195,
	NETMSG_SESSION_UNLOADED = 196,
	NETMSG_LEVEL_LOAD = 197,
	NETMSG_LEVEL_CREATED = 198,
	NETMSG_LEVEL_LOADED = 199,
	NETMSG_LOAD_START = 200,
	NETMSG_LOAD_STARTED = 201,
	NETMSG_LEVEL_INSTANTIATE_SWAP = 202,
	NETMSG_LEVEL_SWAP_READY = 203,
	NETMSG_LEVEL_SWAP_COMPLETE = 204,
	NETMSG_LEVEL_START = 205,
	NETMSG_CHANGE_RULESETS = 206,
	NETMSG_CHARACTER_CHANGE_OWNERSHIP = 207,
	NETMSG_JOURNALDIALOGLOG_UPDATE = 208,
	NETMSG_ACHIEVEMENT_UNLOCKED_MESSAGE = 209,
	NETMSG_SAVEGAME_LOAD_FAIL = 210,
	NETMSG_SERVER_COMMAND = 211,
	NETMSG_SERVER_NOTIFICATION = 212,
	NETMSG_STORY_ELEMENT_UI = 213,
	NETMSG_OPEN_CRAFT_UI_MESSAGE = 214,
	NETMSG_ACHIEVEMENT_PROGRESS_MESSAGE = 215,
	NETMSG_UNLOCK_WAYPOINT = 216,
	NETMSG_LOCK_WAYPOINT = 218,
	NETMSG_COMBINE_RESULT = 219,
	NETMSG_UNLOCK_ITEM = 220,
	NETMSG_TELEPORT_WAYPOINT = 221,
	NETMSG_SHORT_REST = 222,
	NETMSG_CAST_ALL = 223,
	NETMSG_END_THE_DAY = 224,
	NETMSG_LONG_REST_DECISION = 225,
	NETMSG_REALTIME_MULTIPLAY = 226,
	NETMSG_DIALOG_LISTEN = 227,
	NETMSG_DIALOG_INVALID_ANSWER = 228,
	NETMSG_MUSIC_STATE = 229,
	NETMSG_MUSIC_EVENT = 230,
	NETMSG_LOBBY_DATAUPDATE = 231,
	NETMSG_LOBBY_USERUPDATE = 232,
	NETMSG_LOBBY_STARTGAME = 233,
	NETMSG_TELEPORT_ACK = 234,
	NETMSG_CAMERA_SPLINE = 236,
	NETMSG_CHARACTER_MOVEMENT_FALLING = 237,
	NETMSG_READYCHECK = 238,
	NETMSG_PAUSE = 239,
	NETMSG_UNPAUSE = 240,
	NETMSG_DIPLOMACY = 241,
	NETMSG_TRIGGER_CREATE = 242,
	NETMSG_TRIGGER_DESTROY = 243,
	NETMSG_TRIGGER_UPDATE = 244,
	NETMSG_PING_REQUEST = 245,
	NETMSG_CUSTOM_STATS_DEFINITION_CREATE = 246,
	NETMSG_CUSTOM_STATS_DEFINITION_REMOVE = 247,
	NETMSG_CUSTOM_STATS_DEFINITION_UPDATE = 248,
	NETMSG_CUSTOM_STATS_CREATE = 249,
	NETMSG_CUSTOM_STATS_UPDATE = 250,
	NETMSG_GIVE_REWARD = 251,
	NETMSG_LOAD_GAME_WITH_ADDONS = 252,
	NETMSG_LOAD_GAME_WITH_ADDONS_FAIL = 253,
	NETMSG_SHOW_ERROR = 254,
	NETMSG_CLIENT_GAME_SETTINGS = 255,
	NETMSG_MULTIPLE_TARGET_OPERATION = 257,
	NETMSG_DLC_UPDATE = 258,
	NETMSG_TIMELINE_HANDSHAKE = 259,
	NETMSG_TIMELINE_ACTOR_HANDSHAKE = 260,
	NETMSG_TIMELINE_NODECOMPLETED = 261,
	NETMSG_TIMELINE_PLAYER_WATCHING = 262,
	NETMSG_TIMELINE_REQUEST_FASTFORWARD = 263,
	NETMSG_TIMELINE_WORLD_CINEMATIC_COMPLETED = 264,
	NETMSG_FORCE_TURN_BASED_END_PLAYER_TURN_REQUEST = 265,
	NETMSG_FORCE_TURN_BASED_TOGGLE_REQUEST = 266,
	NETMSG_REQUESTED_ROLL = 267,
	NETMSG_PASSIVE_ROLL_SEQUENCE = 268,
	NETMSG_CHARACTER_PATHING = 269,
	NETMSG_CHARACTER_REQUEST_WEAPON_SET_SWITCH = 270,
	NETMSG_CHARACTER_REQUEST_ARMOR_SET_SWITCH = 271,
	NETMSG_CHARACTER_CREATION_ABORT = 272,
	NETMSG_CHARACTER_CREATION_READY = 273,
	NETMSG_CHARACTER_CREATION_UPDATE = 274,
	NETMSG_CHARACTER_CREATION_PLAYORIGINTIMELINE = 275,
	NETMSG_LEVEL_UP_UPDATE = 276,
	NETMSG_RESPEC_UPDATE = 277,
	NETMSG_FULL_RESPEC_UPDATE = 278,
	NETMSG_CHARACTER_CREATION_LEVELUP = 279,
	NETMSG_CHARACTER_CREATION_RESPEC = 280,
	NETMSG_UI_COMBINE_OPEN = 281,
	NETMSG_UI_INTERACTION_STOPPED = 282,
	NETMSG_TUTORIAL_CLOSED = 283,
	NETMSG_TUTORIAL_UI_EVENT = 284,
	NETMSG_TUTORIAL_USER_ACTION = 285,
	NETMSG_TUTORIALS_SHOWING = 286,
	NETMSG_TUTORIALS_RESET = 287,
	NETMSG_PASSIVE_TOGGLE = 288,
	NETMSG_DUALWIELDING_TOGGLE = 289,
	NETMSG_SNEAKING_CONES_VISIBLE_TOGGLE = 290,
	NETMSG_HOTBAR_SLOT_SET = 291,
	NETMSG_HOTBAR_SLOT_REMOVE_IS_NEW_FLAG = 292,
	NETMSG_HOTBAR_COLUMN_SET = 293,
	NETMSG_HOTBAR_ROWS_SET = 294,
	NETMSG_HOTBAR_ROW_REMOVE = 295,
	NETMSG_HOTBAR_LOCK_SET = 296,
	NETMSG_HOTBAR_CUSTOM_DECK = 297,
	NETMSG_HOTBAR_CURRENT_DECK_SET = 298,
	NETMSG_PARTY_PRESET_SAVE = 299,
	NETMSG_PARTY_PRESET_LOAD = 300,
	NETMSG_PARTY_PRESET_LEVELUP = 301,
	NETMSG_PARTY_PRESET_SPELL = 302,
	NETMSG_STORY_FLAGS_LOAD = 303,
	NETMSG_ITEM_TOGGLE_IS_WARE = 304,
	NETMSG_ROLL_STREAM_ROLL_MODE_TYPE = 305,
	NETMSG_REQUEST_RELEASE_CONCENTRATION = 306,
	NETMSG_ACTIVE_ROLL_MODIFIERS = 307,
	NETMSG_CHARACTER_DROP_ENTITY = 308,
	NETMSG_CHARACTER_CANCEL_DROP_ENTITY = 309,
	NETMSG_CHARACTER_LOCKPICK = 310,
	NETMSG_CHARACTER_TRAPDISARM = 311,
	NETMSG_CHARACTER_IMPROV_WEAPON = 312,
	NETMSG_CHARACTER_STOWED_VISIBILITY = 313,
	NETMSG_DECLUTTERED_ITEMS = 314,
	NETMSG_SPELL_CAST_START_PREVIEW = 315,
	NETMSG_SPELL_CAST_UPDATE_PREVIEW = 316,
	NETMSG_SPELL_CAST_CONFIRM = 317,
	NETMSG_SPELL_CAST_CANCEL = 318,
	NETMSG_SPELL_CAST_CAST = 319,
	NETMSG_USE_ITEM_ACTION = 320,
	NETMSG_INVENTORY_CREATE_NEW = 321,
	NETMSG_INVENTORY_DESTROY_NEW = 322,
	NETMSG_INVENTORY_ADD = 323,
	NETMSG_INVENTORY_MOVETOSLOT = 324,
	NETMSG_INVENTORY_TRANSFER = 325,
	NETMSG_INVENTORY_SORT = 326,
	NETMSG_INVENTORY_EQUIP = 327,
	NETMSG_INVENTORY_UNEQUIP = 328,
	NETMSG_INVENTORY_LOCK = 329,
	NETMSG_DIALOG_INSTANCEDIALOGCHANGED = 330,
	NETMSG_CHARACTER_ORIGINAL_TEMPLATE_UPDATE = 331,
	NETMSG_INTERRUPT_DECISION = 332,
	NETMSG_INTERRUPT_SET_ASK_DATA = 333,
	NETMSG_INTERRUPT_SET_ENABLED_DATA = 334,
	NETMSG_SELECT_DICE = 335,
	NETMSG_CAMERA_ARRIVED = 336,
	NETMSG_CLEAR_RECIPE_NEW_FLAG = 337,
	NETMSG_COMBINE_ITEMS = 338,
	NETMSG_NEW_ITEMS_INSIDE = 339,
	NETMSG_CLIENT_DIALOG_PRIVACY_CHANGED = 340,
	NETMSG_CLIENT_DIALOG_JOIN_PRIVACY_FAILED = 341,
	NETMSG_SCRIPT_EXTENDER = 400,
	[1] = "NETMSG_HANDSHAKE",
	[2] = "NETMSG_HOST_WELCOME",
	[3] = "NETMSG_HOST_REFUSE",
	[4] = "NETMSG_HOST_REFUSEPLAYER",
	[5] = "NETMSG_HOST_LEFT",
	[6] = "NETMSG_CLIENT_CONNECT",
	[7] = "NETMSG_CLIENT_ACCEPT",
	[8] = "NETMSG_CLIENT_JOINED",
	[9] = "NETMSG_CLIENT_LEFT",
	[10] = "NETMSG_PLAYER_CONNECT",
	[11] = "NETMSG_PLAYER_DISCONNECT",
	[12] = "NETMSG_PLAYER_ACCEPT",
	[13] = "NETMSG_PLAYER_JOINED",
	[14] = "NETMSG_PLAYER_LEFT",
	[15] = "NETMSG_PLAYER_CHANGENAME",
	[16] = "NETMSG_PLAYER_NAMECHANGED",
	[17] = "NETMSG_ECS_REPLICATION",
	[18] = "NETMSG_VOICEDATA",
	[19] = "NETMSG_SKIPMOVIE_RESULT",
	[20] = "NETMSG_MIC_DISABLED",
	[22] = "NETMSG_SURFACE_CREATE",
	[23] = "NETMSG_SURFACE_META",
	[24] = "NETMSG_DARKNESSTILE_UPDATE",
	[25] = "NETMSG_GAMEACTION",
	[26] = "NETMSG_CHARACTER_CREATE",
	[27] = "NETMSG_CHARACTER_DESTROY",
	[28] = "NETMSG_CHARACTER_CONFIRMATION",
	[29] = "NETMSG_CHARACTER_ACTIVATE",
	[30] = "NETMSG_CHARACTER_DEACTIVATE",
	[31] = "NETMSG_CHARACTER_ASSIGN",
	[32] = "NETMSG_CHARACTER_STEERING",
	[33] = "NETMSG_CHARACTER_ACTION",
	[34] = "NETMSG_CHARACTER_ACTION_DATA",
	[35] = "NETMSG_CHARACTER_ACTION_REQUEST_RESULT",
	[36] = "NETMSG_CHARACTER_STATUS",
	[37] = "NETMSG_CHARACTER_STATUS_LIFETIME",
	[38] = "NETMSG_CHARACTER_UPDATE",
	[39] = "NETMSG_CHARACTER_CONTROL",
	[40] = "NETMSG_CHARACTER_DIALOG",
	[41] = "NETMSG_CHARACTER_UNSHEATHING",
	[42] = "NETMSG_CHARACTER_END_REPOSE",
	[43] = "NETMSG_CHARACTER_USE_MOVEMENT",
	[44] = "NETMSG_CHARACTER_MOVE_FAILED",
	[45] = "NETMSG_CHARACTER_SELECTED_CLIMB_ON",
	[46] = "NETMSG_FOLLOWER_CANT_USE_ITEM",
	[47] = "NETMSG_CHARACTER_AOO",
	[48] = "NETMSG_CHARACTER_TRANSFORM",
	[49] = "NETMSG_CHARACTER_TELEPORT",
	[50] = "NETMSG_CHARACTER_PICKPOCKET",
	[51] = "NETMSG_CHARACTER_OFFSTAGE",
	[52] = "NETMSG_CHARACTER_LOOT",
	[53] = "NETMSG_CHARACTER_ITEM_USED",
	[54] = "NETMSG_CHARACTER_IN_DIALOG",
	[55] = "NETMSG_CHARACTER_BEHAVIOR",
	[60] = "NETMSG_PARTYGROUP",
	[61] = "NETMSG_PARTYORDER",
	[64] = "NETMSG_PARTY_BLOCKFOLLOW",
	[65] = "NETMSG_SHROUD_UPDATE",
	[67] = "NETMSG_ITEM_CREATE",
	[68] = "NETMSG_ITEM_DESTROY",
	[69] = "NETMSG_ITEM_ACTIVATE",
	[70] = "NETMSG_ITEM_DEACTIVATE",
	[71] = "NETMSG_ITEM_DESTINATION",
	[72] = "NETMSG_ITEM_UPDATE",
	[73] = "NETMSG_ITEM_USE_REMOTELY",
	[74] = "NETMSG_ITEM_MOVE_TO_WORLD",
	[75] = "NETMSG_ITEM_ACTION",
	[76] = "NETMSG_ITEM_STATUS",
	[77] = "NETMSG_ITEM_STATUS_LIFETIME",
	[78] = "NETMSG_ITEM_TRANSFORM",
	[79] = "NETMSG_ITEM_CONFIRMATION",
	[80] = "NETMSG_ITEM_OFFSTAGE",
	[81] = "NETMSG_ITEM_MOVED_INFORM",
	[82] = "NETMSG_INVENTORY_CREATE",
	[83] = "NETMSG_INVENTORY_CREATE_AND_OPEN",
	[84] = "NETMSG_INVENTORY_DESTROY",
	[85] = "NETMSG_INVENTORY_VIEW_CREATE",
	[86] = "NETMSG_INVENTORY_VIEW_DESTROY",
	[87] = "NETMSG_INVENTORY_VIEW_UPDATE_ITEMS",
	[88] = "NETMSG_INVENTORY_VIEW_UPDATE_PARENTS",
	[89] = "NETMSG_INVENTORY_VIEW_SORT",
	[90] = "NETMSG_INVENTORY_ITEM_UPDATE",
	[91] = "NETMSG_INVENTORY_LOCKSTATE_SYNC",
	[92] = "NETMSG_PROJECTILE_CREATE",
	[93] = "NETMSG_PROJECTILE_UPDATE",
	[98] = "NETMSG_TURNBASED_FINISHTEAM",
	[99] = "NETMSG_TURNBASED_SETTEAM",
	[100] = "NETMSG_TURNBASED_FLEECOMBATRESULT",
	[101] = "NETMSG_TURNBASED_FLEE_REQUEST",
	[102] = "NETMSG_TURNBASED_ENDTURN_REQUEST",
	[103] = "NETMSG_TURNBASED_CANCEL_ENDTURN_REQUEST",
	[104] = "NETMSG_TURNBASED_BEGIN_CANCEL_ENDTURN_REQUEST",
	[105] = "NETMSG_TURNBASED_END_CANCEL_ENDTURN_REQUEST",
	[106] = "NETMSG_TURNBASED_STARTTURN_CONFIRMATION",
	[107] = "NETMSG_TURNBASED_SKIP_START_DELAY",
	[108] = "NETMSG_UI_FORCETURNBASED_ENTERED",
	[109] = "NETMSG_UI_FORCETURNBASED_LEFT",
	[110] = "NETMSG_UI_FORCETURNBASED_TURN_STARTED",
	[111] = "NETMSG_UI_FORCETURNBASED_TURN_ENDED",
	[116] = "NETMSG_NET_ENTITY_CREATE",
	[117] = "NETMSG_NET_ENTITY_DESTROY",
	[118] = "NETMSG_PEER_ACTIVATE",
	[119] = "NETMSG_PEER_DEACTIVATE",
	[120] = "NETMSG_SPELL_LEARN",
	[121] = "NETMSG_SPELL_REMOVE_LEARNED",
	[122] = "NETMSG_SPELL_PREPARE",
	[123] = "NETMSG_SPELL_CANCEL",
	[124] = "NETMSG_TADPOLE_POWER_ADD",
	[125] = "NETMSG_TADPOLE_POWERS_REMOVE",
	[126] = "NETMSG_EFFECT_CREATE",
	[127] = "NETMSG_EFFECT_FORGET",
	[128] = "NETMSG_EFFECT_DESTROY",
	[129] = "NETMSG_GAMECONTROL_UPDATE_S2C",
	[130] = "NETMSG_GAMECONTROL_UPDATE_C2S",
	[131] = "NETMSG_GAMECONTROL_PRICETAG",
	[132] = "NETMSG_PLAYMOVIE",
	[133] = "NETMSG_CHARACTER_ERROR",
	[134] = "NETMSG_CACHETEMPLATE",
	[135] = "NETMSG_OVERHEADTEXT",
	[136] = "NETMSG_COMBATLOG",
	[137] = "NETMSG_COMBATLOGITEMINTERACTION",
	[138] = "NETMSG_COMBATLOGENTRIES",
	[139] = "NETMSG_SCREEN_FADE_FINISHED",
	[140] = "NETMSG_OPEN_CUSTOM_BOOK_UI_MESSAGE",
	[141] = "NETMSG_CLOSE_CUSTOM_BOOK_UI_MESSAGE",
	[142] = "NETMSG_OPEN_MESSAGE_BOX_MESSAGE",
	[143] = "NETMSG_CLOSED_MESSAGE_BOX_MESSAGE",
	[144] = "NETMSG_OPEN_WAYPOINT_UI_MESSAGE",
	[145] = "NETMSG_CLOSE_UI_MESSAGE",
	[146] = "NETMSG_SHOW_ENTER_REGION_UI_MESSAGE",
	[147] = "NETMSG_JOURNAL_RESET",
	[148] = "NETMSG_QUEST_STEP",
	[149] = "NETMSG_QUEST_CATEGORY_UPDATE",
	[150] = "NETMSG_QUEST_PROGRESS",
	[151] = "NETMSG_QUESTS_LOADED",
	[152] = "NETMSG_TROPHY_UPDATE",
	[153] = "NETMSG_JOURNALRECIPE_UPDATE",
	[154] = "NETMSG_MARKER_UI_CREATE",
	[155] = "NETMSG_MARKER_UI_UPDATE",
	[156] = "NETMSG_QUEST_TRACK",
	[157] = "NETMSG_UI_QUESTSELECTED",
	[158] = "NETMSG_MYSTERY_ADVANCED",
	[159] = "NETMSG_MYSTERY_DISABLED",
	[160] = "NETMSG_NOTIFICATION",
	[161] = "NETMSG_GAMEOVER",
	[162] = "NETMSG_ACT_OVER",
	[163] = "NETMSG_REGISTER_WAYPOINT",
	[164] = "NETMSG_FLAG_UPDATE",
	[168] = "NETMSG_DIALOG_STATE_MESSAGE",
	[169] = "NETMSG_DIALOG_NODE_MESSAGE",
	[170] = "NETMSG_DIALOG_ANSWER_MESSAGE",
	[171] = "NETMSG_DIALOG_ANSWER_HIGHLIGHT_MESSAGE",
	[172] = "NETMSG_DIALOG_HISTORY_MESSAGE",
	[173] = "NETMSG_DIALOG_ACTORJOINS_MESSAGE",
	[174] = "NETMSG_DIALOG_ACTORLEAVES_MESSAGE",
	[175] = "NETMSG_DIALOG_REPLACESPEAKER_MESSAGE",
	[176] = "NETMSG_DIALOG_SUGGESTANSWER_MESSAGE",
	[177] = "NETMSG_DIALOG_REQUEST_IN_DIALOGUE_ATTACK_MESSAGE",
	[178] = "NETMSG_DIALOG_TIMELINE_UNLOADED_MESSAGE",
	[179] = "NETMSG_TRADE_ACTION",
	[180] = "NETMSG_CAMERA_ACTIVATE",
	[181] = "NETMSG_CAMERA_ROTATE",
	[182] = "NETMSG_CAMERA_TARGET",
	[183] = "NETMSG_LIGHTING_OVERRIDE",
	[184] = "NETMSG_ATMOSPHERE_OVERRIDE",
	[185] = "NETMSG_CHAT",
	[186] = "NETMSG_GAMETIME_SYNC",
	[188] = "NETMSG_REQUESTAUTOSAVE",
	[189] = "NETMSG_SAVEGAME",
	[190] = "NETMSG_SAVEGAMEHANDSHAKE",
	[191] = "NETMSG_SAVEGAMEHANDSHAKE_SCREENSHOT",
	[192] = "NETMSG_MODULE_LOAD",
	[193] = "NETMSG_MODULE_LOADED",
	[194] = "NETMSG_SESSION_LOAD",
	[195] = "NETMSG_SESSION_LOADED",
	[196] = "NETMSG_SESSION_UNLOADED",
	[197] = "NETMSG_LEVEL_LOAD",
	[198] = "NETMSG_LEVEL_CREATED",
	[199] = "NETMSG_LEVEL_LOADED",
	[200] = "NETMSG_LOAD_START",
	[201] = "NETMSG_LOAD_STARTED",
	[202] = "NETMSG_LEVEL_INSTANTIATE_SWAP",
	[203] = "NETMSG_LEVEL_SWAP_READY",
	[204] = "NETMSG_LEVEL_SWAP_COMPLETE",
	[205] = "NETMSG_LEVEL_START",
	[206] = "NETMSG_CHANGE_RULESETS",
	[207] = "NETMSG_CHARACTER_CHANGE_OWNERSHIP",
	[208] = "NETMSG_JOURNALDIALOGLOG_UPDATE",
	[209] = "NETMSG_ACHIEVEMENT_UNLOCKED_MESSAGE",
	[210] = "NETMSG_SAVEGAME_LOAD_FAIL",
	[211] = "NETMSG_SERVER_COMMAND",
	[212] = "NETMSG_SERVER_NOTIFICATION",
	[213] = "NETMSG_STORY_ELEMENT_UI",
	[214] = "NETMSG_OPEN_CRAFT_UI_MESSAGE",
	[215] = "NETMSG_ACHIEVEMENT_PROGRESS_MESSAGE",
	[216] = "NETMSG_UNLOCK_WAYPOINT",
	[218] = "NETMSG_LOCK_WAYPOINT",
	[219] = "NETMSG_COMBINE_RESULT",
	[220] = "NETMSG_UNLOCK_ITEM",
	[221] = "NETMSG_TELEPORT_WAYPOINT",
	[222] = "NETMSG_SHORT_REST",
	[223] = "NETMSG_CAST_ALL",
	[224] = "NETMSG_END_THE_DAY",
	[225] = "NETMSG_LONG_REST_DECISION",
	[226] = "NETMSG_REALTIME_MULTIPLAY",
	[227] = "NETMSG_DIALOG_LISTEN",
	[228] = "NETMSG_DIALOG_INVALID_ANSWER",
	[229] = "NETMSG_MUSIC_STATE",
	[230] = "NETMSG_MUSIC_EVENT",
	[231] = "NETMSG_LOBBY_DATAUPDATE",
	[232] = "NETMSG_LOBBY_USERUPDATE",
	[233] = "NETMSG_LOBBY_STARTGAME",
	[234] = "NETMSG_TELEPORT_ACK",
	[236] = "NETMSG_CAMERA_SPLINE",
	[237] = "NETMSG_CHARACTER_MOVEMENT_FALLING",
	[238] = "NETMSG_READYCHECK",
	[239] = "NETMSG_PAUSE",
	[240] = "NETMSG_UNPAUSE",
	[241] = "NETMSG_DIPLOMACY",
	[242] = "NETMSG_TRIGGER_CREATE",
	[243] = "NETMSG_TRIGGER_DESTROY",
	[244] = "NETMSG_TRIGGER_UPDATE",
	[245] = "NETMSG_PING_REQUEST",
	[246] = "NETMSG_CUSTOM_STATS_DEFINITION_CREATE",
	[247] = "NETMSG_CUSTOM_STATS_DEFINITION_REMOVE",
	[248] = "NETMSG_CUSTOM_STATS_DEFINITION_UPDATE",
	[249] = "NETMSG_CUSTOM_STATS_CREATE",
	[250] = "NETMSG_CUSTOM_STATS_UPDATE",
	[251] = "NETMSG_GIVE_REWARD",
	[252] = "NETMSG_LOAD_GAME_WITH_ADDONS",
	[253] = "NETMSG_LOAD_GAME_WITH_ADDONS_FAIL",
	[254] = "NETMSG_SHOW_ERROR",
	[255] = "NETMSG_CLIENT_GAME_SETTINGS",
	[257] = "NETMSG_MULTIPLE_TARGET_OPERATION",
	[258] = "NETMSG_DLC_UPDATE",
	[259] = "NETMSG_TIMELINE_HANDSHAKE",
	[260] = "NETMSG_TIMELINE_ACTOR_HANDSHAKE",
	[261] = "NETMSG_TIMELINE_NODECOMPLETED",
	[262] = "NETMSG_TIMELINE_PLAYER_WATCHING",
	[263] = "NETMSG_TIMELINE_REQUEST_FASTFORWARD",
	[264] = "NETMSG_TIMELINE_WORLD_CINEMATIC_COMPLETED",
	[265] = "NETMSG_FORCE_TURN_BASED_END_PLAYER_TURN_REQUEST",
	[266] = "NETMSG_FORCE_TURN_BASED_TOGGLE_REQUEST",
	[267] = "NETMSG_REQUESTED_ROLL",
	[268] = "NETMSG_PASSIVE_ROLL_SEQUENCE",
	[269] = "NETMSG_CHARACTER_PATHING",
	[270] = "NETMSG_CHARACTER_REQUEST_WEAPON_SET_SWITCH",
	[271] = "NETMSG_CHARACTER_REQUEST_ARMOR_SET_SWITCH",
	[272] = "NETMSG_CHARACTER_CREATION_ABORT",
	[273] = "NETMSG_CHARACTER_CREATION_READY",
	[274] = "NETMSG_CHARACTER_CREATION_UPDATE",
	[275] = "NETMSG_CHARACTER_CREATION_PLAYORIGINTIMELINE",
	[276] = "NETMSG_LEVEL_UP_UPDATE",
	[277] = "NETMSG_RESPEC_UPDATE",
	[278] = "NETMSG_FULL_RESPEC_UPDATE",
	[279] = "NETMSG_CHARACTER_CREATION_LEVELUP",
	[280] = "NETMSG_CHARACTER_CREATION_RESPEC",
	[281] = "NETMSG_UI_COMBINE_OPEN",
	[282] = "NETMSG_UI_INTERACTION_STOPPED",
	[283] = "NETMSG_TUTORIAL_CLOSED",
	[284] = "NETMSG_TUTORIAL_UI_EVENT",
	[285] = "NETMSG_TUTORIAL_USER_ACTION",
	[286] = "NETMSG_TUTORIALS_SHOWING",
	[287] = "NETMSG_TUTORIALS_RESET",
	[288] = "NETMSG_PASSIVE_TOGGLE",
	[289] = "NETMSG_DUALWIELDING_TOGGLE",
	[290] = "NETMSG_SNEAKING_CONES_VISIBLE_TOGGLE",
	[291] = "NETMSG_HOTBAR_SLOT_SET",
	[292] = "NETMSG_HOTBAR_SLOT_REMOVE_IS_NEW_FLAG",
	[293] = "NETMSG_HOTBAR_COLUMN_SET",
	[294] = "NETMSG_HOTBAR_ROWS_SET",
	[295] = "NETMSG_HOTBAR_ROW_REMOVE",
	[296] = "NETMSG_HOTBAR_LOCK_SET",
	[297] = "NETMSG_HOTBAR_CUSTOM_DECK",
	[298] = "NETMSG_HOTBAR_CURRENT_DECK_SET",
	[299] = "NETMSG_PARTY_PRESET_SAVE",
	[300] = "NETMSG_PARTY_PRESET_LOAD",
	[301] = "NETMSG_PARTY_PRESET_LEVELUP",
	[302] = "NETMSG_PARTY_PRESET_SPELL",
	[303] = "NETMSG_STORY_FLAGS_LOAD",
	[304] = "NETMSG_ITEM_TOGGLE_IS_WARE",
	[305] = "NETMSG_ROLL_STREAM_ROLL_MODE_TYPE",
	[306] = "NETMSG_REQUEST_RELEASE_CONCENTRATION",
	[307] = "NETMSG_ACTIVE_ROLL_MODIFIERS",
	[308] = "NETMSG_CHARACTER_DROP_ENTITY",
	[309] = "NETMSG_CHARACTER_CANCEL_DROP_ENTITY",
	[310] = "NETMSG_CHARACTER_LOCKPICK",
	[311] = "NETMSG_CHARACTER_TRAPDISARM",
	[312] = "NETMSG_CHARACTER_IMPROV_WEAPON",
	[313] = "NETMSG_CHARACTER_STOWED_VISIBILITY",
	[314] = "NETMSG_DECLUTTERED_ITEMS",
	[315] = "NETMSG_SPELL_CAST_START_PREVIEW",
	[316] = "NETMSG_SPELL_CAST_UPDATE_PREVIEW",
	[317] = "NETMSG_SPELL_CAST_CONFIRM",
	[318] = "NETMSG_SPELL_CAST_CANCEL",
	[319] = "NETMSG_SPELL_CAST_CAST",
	[320] = "NETMSG_USE_ITEM_ACTION",
	[321] = "NETMSG_INVENTORY_CREATE_NEW",
	[322] = "NETMSG_INVENTORY_DESTROY_NEW",
	[323] = "NETMSG_INVENTORY_ADD",
	[324] = "NETMSG_INVENTORY_MOVETOSLOT",
	[325] = "NETMSG_INVENTORY_TRANSFER",
	[326] = "NETMSG_INVENTORY_SORT",
	[327] = "NETMSG_INVENTORY_EQUIP",
	[328] = "NETMSG_INVENTORY_UNEQUIP",
	[329] = "NETMSG_INVENTORY_LOCK",
	[330] = "NETMSG_DIALOG_INSTANCEDIALOGCHANGED",
	[331] = "NETMSG_CHARACTER_ORIGINAL_TEMPLATE_UPDATE",
	[332] = "NETMSG_INTERRUPT_DECISION",
	[333] = "NETMSG_INTERRUPT_SET_ASK_DATA",
	[334] = "NETMSG_INTERRUPT_SET_ENABLED_DATA",
	[335] = "NETMSG_SELECT_DICE",
	[336] = "NETMSG_CAMERA_ARRIVED",
	[337] = "NETMSG_CLEAR_RECIPE_NEW_FLAG",
	[338] = "NETMSG_COMBINE_ITEMS",
	[339] = "NETMSG_NEW_ITEMS_INSIDE",
	[340] = "NETMSG_CLIENT_DIALOG_PRIVACY_CHANGED",
	[341] = "NETMSG_CLIENT_DIALOG_JOIN_PRIVACY_FAILED",
	[400] = "NETMSG_SCRIPT_EXTENDER",
}

--- @enum NsDragAction
Ext_Enums.NsDragAction = {
	DragAction_Continue = 0,
	DragAction_Drop = 1,
	DragAction_Cancel = 2,
	[0] = "DragAction_Continue",
	[1] = "DragAction_Drop",
	[2] = "DragAction_Cancel",
}

--- @enum NsHoldingState
Ext_Enums.NsHoldingState = {
	HoldingState_Started = 0,
	HoldingState_Completed = 1,
	HoldingState_Canceled = 2,
	[0] = "HoldingState_Started",
	[1] = "HoldingState_Completed",
	[2] = "HoldingState_Canceled",
}

--- @enum NsKey
Ext_Enums.NsKey = {
	Key_None = 0,
	Key_Cancel = 1,
	Key_Back = 2,
	Key_Tab = 3,
	Key_LineFeed = 4,
	Key_Clear = 5,
	Key_Enter = 6,
	Key_Pause = 7,
	Key_CapsLock = 8,
	Key_HangulMode = 9,
	Key_JunjaMode = 10,
	Key_FinalMode = 11,
	Key_KanjiMode = 12,
	Key_Escape = 13,
	Key_ImeConvert = 14,
	Key_ImeNonConvert = 15,
	Key_ImeAccept = 16,
	Key_ImeModeChange = 17,
	Key_Space = 18,
	Key_PageUp = 19,
	Key_PageDown = 20,
	Key_End = 21,
	Key_Home = 22,
	Key_Left = 23,
	Key_Up = 24,
	Key_Right = 25,
	Key_Down = 26,
	Key_Select = 27,
	Key_Print = 28,
	Key_Execute = 29,
	Key_PrintScreen = 30,
	Key_Insert = 31,
	Key_Delete = 32,
	Key_Help = 33,
	Key_D0 = 34,
	Key_D1 = 35,
	Key_D2 = 36,
	Key_D3 = 37,
	Key_D4 = 38,
	Key_D5 = 39,
	Key_D6 = 40,
	Key_D7 = 41,
	Key_D8 = 42,
	Key_D9 = 43,
	Key_A = 44,
	Key_B = 45,
	Key_C = 46,
	Key_D = 47,
	Key_E = 48,
	Key_F = 49,
	Key_G = 50,
	Key_H = 51,
	Key_I = 52,
	Key_J = 53,
	Key_K = 54,
	Key_L = 55,
	Key_M = 56,
	Key_N = 57,
	Key_O = 58,
	Key_P = 59,
	Key_Q = 60,
	Key_R = 61,
	Key_S = 62,
	Key_T = 63,
	Key_U = 64,
	Key_V = 65,
	Key_W = 66,
	Key_X = 67,
	Key_Y = 68,
	Key_Z = 69,
	Key_LWin = 70,
	Key_RWin = 71,
	Key_Apps = 72,
	Key_Sleep = 73,
	Key_NumPad0 = 74,
	Key_NumPad1 = 75,
	Key_NumPad2 = 76,
	Key_NumPad3 = 77,
	Key_NumPad4 = 78,
	Key_NumPad5 = 79,
	Key_NumPad6 = 80,
	Key_NumPad7 = 81,
	Key_NumPad8 = 82,
	Key_NumPad9 = 83,
	Key_Multiply = 84,
	Key_Add = 85,
	Key_Separator = 86,
	Key_Subtract = 87,
	Key_Decimal = 88,
	Key_Divide = 89,
	Key_F1 = 90,
	Key_F2 = 91,
	Key_F3 = 92,
	Key_F4 = 93,
	Key_F5 = 94,
	Key_F6 = 95,
	Key_F7 = 96,
	Key_F8 = 97,
	Key_F9 = 98,
	Key_F10 = 99,
	Key_F11 = 100,
	Key_F12 = 101,
	Key_F13 = 102,
	Key_F14 = 103,
	Key_F15 = 104,
	Key_F16 = 105,
	Key_F17 = 106,
	Key_F18 = 107,
	Key_F19 = 108,
	Key_F20 = 109,
	Key_F21 = 110,
	Key_F22 = 111,
	Key_F23 = 112,
	Key_F24 = 113,
	Key_NumLock = 114,
	Key_Scroll = 115,
	Key_LeftShift = 116,
	Key_RightShift = 117,
	Key_LeftCtrl = 118,
	Key_RightCtrl = 119,
	Key_LeftAlt = 120,
	Key_RightAlt = 121,
	Key_BrowserBack = 122,
	Key_BrowserForward = 123,
	Key_BrowserRefresh = 124,
	Key_BrowserStop = 125,
	Key_BrowserSearch = 126,
	Key_BrowserFavorites = 127,
	Key_BrowserHome = 128,
	Key_VolumeMute = 129,
	Key_VolumeDown = 130,
	Key_VolumeUp = 131,
	Key_MediaNextTrack = 132,
	Key_MediaPreviousTrack = 133,
	Key_MediaStop = 134,
	Key_MediaPlayPause = 135,
	Key_LaunchMail = 136,
	Key_SelectMedia = 137,
	Key_LaunchApplication1 = 138,
	Key_LaunchApplication2 = 139,
	Key_OemSemicolon = 140,
	Key_OemPlus = 141,
	Key_OemComma = 142,
	Key_OemMinus = 143,
	Key_OemPeriod = 144,
	Key_OemQuestion = 145,
	Key_OemTilde = 146,
	Key_AbntC1 = 147,
	Key_AbntC2 = 148,
	Key_OemOpenBrackets = 149,
	Key_OemPipe = 150,
	Key_OemCloseBrackets = 151,
	Key_OemQuotes = 152,
	Key_Oem8 = 153,
	Key_OemBackslash = 154,
	Key_ImeProcessed = 155,
	Key_System = 156,
	Key_DbeAlphanumeric = 157,
	Key_DbeKatakana = 158,
	Key_DbeHiragana = 159,
	Key_DbeSbcsChar = 160,
	Key_DbeDbcsChar = 161,
	Key_DbeRoman = 162,
	Key_DbeNoRoman = 163,
	Key_DbeEnterWordRegisterMode = 164,
	Key_DbeEnterImeConfigureMode = 165,
	Key_DbeFlushString = 166,
	Key_DbeCodeInput = 167,
	Key_DbeNoCodeInput = 168,
	Key_DbeDetermineString = 169,
	Key_DbeEnterDialogConversionMode = 170,
	Key_OemClear = 171,
	Key_DeadCharProcessed = 172,
	Key_PageLeft = 173,
	Key_PageRight = 174,
	Key_GamepadLeft = 175,
	Key_GamepadUp = 176,
	Key_GamepadRight = 177,
	Key_GamepadDown = 178,
	Key_GamepadAccept = 179,
	Key_GamepadCancel = 180,
	Key_GamepadMenu = 181,
	Key_GamepadView = 182,
	Key_GamepadPageUp = 183,
	Key_GamepadPageDown = 184,
	Key_GamepadPageLeft = 185,
	Key_GamepadPageRight = 186,
	Key_GamepadContext1 = 187,
	Key_GamepadContext2 = 188,
	Key_GamepadContext3 = 189,
	Key_GamepadContext4 = 190,
	[0] = "Key_None",
	[1] = "Key_Cancel",
	[2] = "Key_Back",
	[3] = "Key_Tab",
	[4] = "Key_LineFeed",
	[5] = "Key_Clear",
	[6] = "Key_Enter",
	[7] = "Key_Pause",
	[8] = "Key_CapsLock",
	[9] = "Key_HangulMode",
	[10] = "Key_JunjaMode",
	[11] = "Key_FinalMode",
	[12] = "Key_KanjiMode",
	[13] = "Key_Escape",
	[14] = "Key_ImeConvert",
	[15] = "Key_ImeNonConvert",
	[16] = "Key_ImeAccept",
	[17] = "Key_ImeModeChange",
	[18] = "Key_Space",
	[19] = "Key_PageUp",
	[20] = "Key_PageDown",
	[21] = "Key_End",
	[22] = "Key_Home",
	[23] = "Key_Left",
	[24] = "Key_Up",
	[25] = "Key_Right",
	[26] = "Key_Down",
	[27] = "Key_Select",
	[28] = "Key_Print",
	[29] = "Key_Execute",
	[30] = "Key_PrintScreen",
	[31] = "Key_Insert",
	[32] = "Key_Delete",
	[33] = "Key_Help",
	[34] = "Key_D0",
	[35] = "Key_D1",
	[36] = "Key_D2",
	[37] = "Key_D3",
	[38] = "Key_D4",
	[39] = "Key_D5",
	[40] = "Key_D6",
	[41] = "Key_D7",
	[42] = "Key_D8",
	[43] = "Key_D9",
	[44] = "Key_A",
	[45] = "Key_B",
	[46] = "Key_C",
	[47] = "Key_D",
	[48] = "Key_E",
	[49] = "Key_F",
	[50] = "Key_G",
	[51] = "Key_H",
	[52] = "Key_I",
	[53] = "Key_J",
	[54] = "Key_K",
	[55] = "Key_L",
	[56] = "Key_M",
	[57] = "Key_N",
	[58] = "Key_O",
	[59] = "Key_P",
	[60] = "Key_Q",
	[61] = "Key_R",
	[62] = "Key_S",
	[63] = "Key_T",
	[64] = "Key_U",
	[65] = "Key_V",
	[66] = "Key_W",
	[67] = "Key_X",
	[68] = "Key_Y",
	[69] = "Key_Z",
	[70] = "Key_LWin",
	[71] = "Key_RWin",
	[72] = "Key_Apps",
	[73] = "Key_Sleep",
	[74] = "Key_NumPad0",
	[75] = "Key_NumPad1",
	[76] = "Key_NumPad2",
	[77] = "Key_NumPad3",
	[78] = "Key_NumPad4",
	[79] = "Key_NumPad5",
	[80] = "Key_NumPad6",
	[81] = "Key_NumPad7",
	[82] = "Key_NumPad8",
	[83] = "Key_NumPad9",
	[84] = "Key_Multiply",
	[85] = "Key_Add",
	[86] = "Key_Separator",
	[87] = "Key_Subtract",
	[88] = "Key_Decimal",
	[89] = "Key_Divide",
	[90] = "Key_F1",
	[91] = "Key_F2",
	[92] = "Key_F3",
	[93] = "Key_F4",
	[94] = "Key_F5",
	[95] = "Key_F6",
	[96] = "Key_F7",
	[97] = "Key_F8",
	[98] = "Key_F9",
	[99] = "Key_F10",
	[100] = "Key_F11",
	[101] = "Key_F12",
	[102] = "Key_F13",
	[103] = "Key_F14",
	[104] = "Key_F15",
	[105] = "Key_F16",
	[106] = "Key_F17",
	[107] = "Key_F18",
	[108] = "Key_F19",
	[109] = "Key_F20",
	[110] = "Key_F21",
	[111] = "Key_F22",
	[112] = "Key_F23",
	[113] = "Key_F24",
	[114] = "Key_NumLock",
	[115] = "Key_Scroll",
	[116] = "Key_LeftShift",
	[117] = "Key_RightShift",
	[118] = "Key_LeftCtrl",
	[119] = "Key_RightCtrl",
	[120] = "Key_LeftAlt",
	[121] = "Key_RightAlt",
	[122] = "Key_BrowserBack",
	[123] = "Key_BrowserForward",
	[124] = "Key_BrowserRefresh",
	[125] = "Key_BrowserStop",
	[126] = "Key_BrowserSearch",
	[127] = "Key_BrowserFavorites",
	[128] = "Key_BrowserHome",
	[129] = "Key_VolumeMute",
	[130] = "Key_VolumeDown",
	[131] = "Key_VolumeUp",
	[132] = "Key_MediaNextTrack",
	[133] = "Key_MediaPreviousTrack",
	[134] = "Key_MediaStop",
	[135] = "Key_MediaPlayPause",
	[136] = "Key_LaunchMail",
	[137] = "Key_SelectMedia",
	[138] = "Key_LaunchApplication1",
	[139] = "Key_LaunchApplication2",
	[140] = "Key_OemSemicolon",
	[141] = "Key_OemPlus",
	[142] = "Key_OemComma",
	[143] = "Key_OemMinus",
	[144] = "Key_OemPeriod",
	[145] = "Key_OemQuestion",
	[146] = "Key_OemTilde",
	[147] = "Key_AbntC1",
	[148] = "Key_AbntC2",
	[149] = "Key_OemOpenBrackets",
	[150] = "Key_OemPipe",
	[151] = "Key_OemCloseBrackets",
	[152] = "Key_OemQuotes",
	[153] = "Key_Oem8",
	[154] = "Key_OemBackslash",
	[155] = "Key_ImeProcessed",
	[156] = "Key_System",
	[157] = "Key_DbeAlphanumeric",
	[158] = "Key_DbeKatakana",
	[159] = "Key_DbeHiragana",
	[160] = "Key_DbeSbcsChar",
	[161] = "Key_DbeDbcsChar",
	[162] = "Key_DbeRoman",
	[163] = "Key_DbeNoRoman",
	[164] = "Key_DbeEnterWordRegisterMode",
	[165] = "Key_DbeEnterImeConfigureMode",
	[166] = "Key_DbeFlushString",
	[167] = "Key_DbeCodeInput",
	[168] = "Key_DbeNoCodeInput",
	[169] = "Key_DbeDetermineString",
	[170] = "Key_DbeEnterDialogConversionMode",
	[171] = "Key_OemClear",
	[172] = "Key_DeadCharProcessed",
	[173] = "Key_PageLeft",
	[174] = "Key_PageRight",
	[175] = "Key_GamepadLeft",
	[176] = "Key_GamepadUp",
	[177] = "Key_GamepadRight",
	[178] = "Key_GamepadDown",
	[179] = "Key_GamepadAccept",
	[180] = "Key_GamepadCancel",
	[181] = "Key_GamepadMenu",
	[182] = "Key_GamepadView",
	[183] = "Key_GamepadPageUp",
	[184] = "Key_GamepadPageDown",
	[185] = "Key_GamepadPageLeft",
	[186] = "Key_GamepadPageRight",
	[187] = "Key_GamepadContext1",
	[188] = "Key_GamepadContext2",
	[189] = "Key_GamepadContext3",
	[190] = "Key_GamepadContext4",
}

--- @enum NsKeyStates
Ext_Enums.NsKeyStates = {
	KeyStates_None = 0,
	KeyStates_Down = 1,
	KeyStates_Toggled = 2,
	[0] = "KeyStates_None",
	[1] = "KeyStates_Down",
	[2] = "KeyStates_Toggled",
}

--- @enum NsManipulationModes
Ext_Enums.NsManipulationModes = {
	ManipulationModes_TranslateX = 0,
	ManipulationModes_TranslateY = 1,
	ManipulationModes_Rotate = 2,
	ManipulationModes_Scale = 3,
	[0] = "ManipulationModes_TranslateX",
	[1] = "ManipulationModes_TranslateY",
	[2] = "ManipulationModes_Rotate",
	[3] = "ManipulationModes_Scale",
}

--- @enum NsModifierKeys
Ext_Enums.NsModifierKeys = {
	ModifierKeys_Alt = 0,
	ModifierKeys_Control = 1,
	ModifierKeys_Shift = 2,
	ModifierKeys_Windows = 3,
	[0] = "ModifierKeys_Alt",
	[1] = "ModifierKeys_Control",
	[2] = "ModifierKeys_Shift",
	[3] = "ModifierKeys_Windows",
}

--- @enum NsMouseAction
Ext_Enums.NsMouseAction = {
	MouseAction_None = 0,
	MouseAction_LeftClick = 1,
	MouseAction_RightClick = 2,
	MouseAction_MiddleClick = 3,
	MouseAction_WheelClick = 4,
	MouseAction_LeftDoubleClick = 5,
	MouseAction_RightDoubleClick = 6,
	MouseAction_MiddleDoubleClick = 7,
	[0] = "MouseAction_None",
	[1] = "MouseAction_LeftClick",
	[2] = "MouseAction_RightClick",
	[3] = "MouseAction_MiddleClick",
	[4] = "MouseAction_WheelClick",
	[5] = "MouseAction_LeftDoubleClick",
	[6] = "MouseAction_RightDoubleClick",
	[7] = "MouseAction_MiddleDoubleClick",
}

--- @enum NsMouseButton
Ext_Enums.NsMouseButton = {
	MouseButton_Left = 0,
	MouseButton_Right = 1,
	MouseButton_Middle = 2,
	MouseButton_XButton1 = 3,
	MouseButton_XButton2 = 4,
	[0] = "MouseButton_Left",
	[1] = "MouseButton_Right",
	[2] = "MouseButton_Middle",
	[3] = "MouseButton_XButton1",
	[4] = "MouseButton_XButton2",
}

--- @enum NsMouseButtonState
Ext_Enums.NsMouseButtonState = {
	MouseButtonState_Released = 0,
	MouseButtonState_Pressed = 1,
	[0] = "MouseButtonState_Released",
	[1] = "MouseButtonState_Pressed",
}

--- @enum NsOrientation
Ext_Enums.NsOrientation = {
	Orientation_Horizontal = 0,
	Orientation_Vertical = 1,
	[0] = "Orientation_Horizontal",
	[1] = "Orientation_Vertical",
}

--- @enum ObscurityState
Ext_Enums.ObscurityState = {
	Clear = 0,
	LightlyObscured = 1,
	HeavilyObscured = 2,
	[0] = "Clear",
	[1] = "LightlyObscured",
	[2] = "HeavilyObscured",
}

--- @enum OsiFunctionType
Ext_Enums.OsiFunctionType = {
	Call = 0,
	Query = 1,
	Event = 2,
	DB = 3,
	Proc = 4,
	[0] = "Call",
	[1] = "Query",
	[2] = "Event",
	[3] = "DB",
	[4] = "Proc",
}

--- @enum OsirisTaskType
Ext_Enums.OsirisTaskType = {
	MoveToLocation = 0,
	MoveToObject = 1,
	FleeFromGrid = 2,
	MoveInRange = 3,
	TeleportToLocation = 4,
	Appear = 6,
	Disappear = 7,
	FollowNPC = 8,
	FollowOwnerOrLeader = 9,
	Wander = 10,
	Steer = 11,
	PlayAnimation = 12,
	Drop = 13,
	PickupItem = 14,
	UseItem = 15,
	MoveItem = 16,
	Resurrect = 17,
	UseSpell = 18,
	MoveToAndTalk = 19,
	LookAt = 20,
	Timer = 21,
	AutomatedDialog = 22,
	RateLimitedAutomatedDialog = 23,
	FleeFromRelation = 24,
	FleeFromEntity = 25,
	Combine = 26,
	[0] = "MoveToLocation",
	[1] = "MoveToObject",
	[2] = "FleeFromGrid",
	[3] = "MoveInRange",
	[4] = "TeleportToLocation",
	[6] = "Appear",
	[7] = "Disappear",
	[8] = "FollowNPC",
	[9] = "FollowOwnerOrLeader",
	[10] = "Wander",
	[11] = "Steer",
	[12] = "PlayAnimation",
	[13] = "Drop",
	[14] = "PickupItem",
	[15] = "UseItem",
	[16] = "MoveItem",
	[17] = "Resurrect",
	[18] = "UseSpell",
	[19] = "MoveToAndTalk",
	[20] = "LookAt",
	[21] = "Timer",
	[22] = "AutomatedDialog",
	[23] = "RateLimitedAutomatedDialog",
	[24] = "FleeFromRelation",
	[25] = "FleeFromEntity",
	[26] = "Combine",
}

--- @enum PassiveSourceType
Ext_Enums.PassiveSourceType = {
	Feat = 0,
	Progression = 1,
	Progression2 = 2,
	Status = 3,
	Equipment = 4,
	Stats = 5,
	Tadpole = 6,
	Script = 7,
	Debug = 8,
	[0] = "Feat",
	[1] = "Progression",
	[2] = "Progression2",
	[3] = "Status",
	[4] = "Equipment",
	[5] = "Stats",
	[6] = "Tadpole",
	[7] = "Script",
	[8] = "Debug",
}

--- @enum PathRootType
Ext_Enums.PathRootType = {
	Root = 0,
	Data = 1,
	Public = 2,
	LocalAppData = 3,
	UserProfile = 4,
	Localization = 5,
	Bin = 6,
	Bin2 = 7,
	Mods = 8,
	Projects = 9,
	Public2 = 10,
	GameMod = 11,
	EngineMod = 12,
	Scripts = 13,
	WorkingDir = 14,
	Debug = 15,
	[0] = "Root",
	[1] = "Data",
	[2] = "Public",
	[3] = "LocalAppData",
	[4] = "UserProfile",
	[5] = "Localization",
	[6] = "Bin",
	[7] = "Bin2",
	[8] = "Mods",
	[9] = "Projects",
	[10] = "Public2",
	[11] = "GameMod",
	[12] = "EngineMod",
	[13] = "Scripts",
	[14] = "WorkingDir",
	[15] = "Debug",
}

--- @enum PhysicsExtraFlags
Ext_Enums.PhysicsExtraFlags = {
	ShootThroughType0 = 1,
	ShootThroughType1 = 2,
	ShootThroughType2 = 4,
	ShootThroughType3 = 8,
	ShootThroughType4 = 16,
	ShootThroughType5 = 32,
	BlockAoEDamage = 64,
	[1] = "ShootThroughType0",
	[2] = "ShootThroughType1",
	[4] = "ShootThroughType2",
	[8] = "ShootThroughType3",
	[16] = "ShootThroughType4",
	[32] = "ShootThroughType5",
	[64] = "BlockAoEDamage",
}

--- @enum PhysicsGroupFlags
Ext_Enums.PhysicsGroupFlags = {
	Group01 = 1,
	Group02 = 2,
	Group04 = 4,
	Group08 = 8,
	Terrain = 16,
	CanWalkOn = 32,
	Group40 = 64,
	Group80 = 128,
	Group100 = 256,
	Character = 512,
	Item = 1024,
	Scenery = 2048,
	Fadeable = 4096,
	Wadable = 8192,
	Dead = 16384,
	VisibleItem = 32768,
	CanClickThrough = 65536,
	CanSeeThrough = 131072,
	PointerBlocker = 262144,
	Dragging = 524288,
	CanClimbOn = 1048576,
	IsDecorative = 2097152,
	CanShootThrough = 4194304,
	CanSeeThrough2 = 8388608,
	ClimbableTileConstruction = 16777216,
	DisableBoundCollision = 33554432,
	PlatformOwner = 67108864,
	Falling = 134217728,
	Group10000000 = 268435456,
	TimelinePreview = 536870912,
	CanShineThrough = 1073741824,
	AttackableWhenClickThrough = 2147483648,
	[1] = "Group01",
	[2] = "Group02",
	[4] = "Group04",
	[8] = "Group08",
	[16] = "Terrain",
	[32] = "CanWalkOn",
	[64] = "Group40",
	[128] = "Group80",
	[256] = "Group100",
	[512] = "Character",
	[1024] = "Item",
	[2048] = "Scenery",
	[4096] = "Fadeable",
	[8192] = "Wadable",
	[16384] = "Dead",
	[32768] = "VisibleItem",
	[65536] = "CanClickThrough",
	[131072] = "CanSeeThrough",
	[262144] = "PointerBlocker",
	[524288] = "Dragging",
	[1048576] = "CanClimbOn",
	[2097152] = "IsDecorative",
	[4194304] = "CanShootThrough",
	[8388608] = "CanSeeThrough2",
	[16777216] = "ClimbableTileConstruction",
	[33554432] = "DisableBoundCollision",
	[67108864] = "PlatformOwner",
	[134217728] = "Falling",
	[268435456] = "Group10000000",
	[536870912] = "TimelinePreview",
	[1073741824] = "CanShineThrough",
	[2147483648] = "AttackableWhenClickThrough",
}

--- @enum PhysicsHitType
Ext_Enums.PhysicsHitType = {
	Normal = 1,
	DoubleSided = 8,
	[1] = "Normal",
	[8] = "DoubleSided",
}

--- @enum PhysicsType
Ext_Enums.PhysicsType = {
	Static = 1,
	Dynamic = 4,
	[1] = "Static",
	[4] = "Dynamic",
}

--- @enum ProficiencyBonusBoostType
Ext_Enums.ProficiencyBonusBoostType = {
	AttackRoll = 0,
	AttackTarget = 1,
	SavingThrow = 2,
	AllSavingThrows = 3,
	Ability = 4,
	AllAbilities = 5,
	Skill = 6,
	AllSkills = 7,
	SourceDialogue = 8,
	WeaponActionDC = 9,
	[0] = "AttackRoll",
	[1] = "AttackTarget",
	[2] = "SavingThrow",
	[3] = "AllSavingThrows",
	[4] = "Ability",
	[5] = "AllAbilities",
	[6] = "Skill",
	[7] = "AllSkills",
	[8] = "SourceDialogue",
	[9] = "WeaponActionDC",
}

--- @enum ProficiencyGroupFlags
Ext_Enums.ProficiencyGroupFlags = {
	Battleaxes = 1,
	Clubs = 2,
	Daggers = 4,
	Darts = 8,
	Flails = 16,
	Glaives = 32,
	Greataxes = 64,
	Greatclubs = 128,
	Greatswords = 256,
	Halberds = 512,
	HandCrossbows = 1024,
	Handaxes = 2048,
	HeavyArmor = 4096,
	HeavyCrossbows = 8192,
	Javelins = 16384,
	LightArmor = 32768,
	LightCrossbows = 65536,
	LightHammers = 131072,
	Longbows = 262144,
	Longswords = 524288,
	Maces = 1048576,
	MartialWeapons = 2097152,
	Mauls = 4194304,
	MediumArmor = 8388608,
	Morningstars = 16777216,
	Pikes = 33554432,
	Quarterstaffs = 67108864,
	Rapiers = 134217728,
	Scimitars = 268435456,
	Shields = 536870912,
	Shortbows = 1073741824,
	Shortswords = 2147483648,
	Sickles = 4294967296,
	SimpleWeapons = 8589934592,
	Slings = 17179869184,
	Spears = 34359738368,
	Tridents = 68719476736,
	Warhammers = 137438953472,
	Warpicks = 274877906944,
	MusicalInstrument = 549755813888,
	[1] = "Battleaxes",
	[2] = "Clubs",
	[4] = "Daggers",
	[8] = "Darts",
	[16] = "Flails",
	[32] = "Glaives",
	[64] = "Greataxes",
	[128] = "Greatclubs",
	[256] = "Greatswords",
	[512] = "Halberds",
	[1024] = "HandCrossbows",
	[2048] = "Handaxes",
	[4096] = "HeavyArmor",
	[8192] = "HeavyCrossbows",
	[16384] = "Javelins",
	[32768] = "LightArmor",
	[65536] = "LightCrossbows",
	[131072] = "LightHammers",
	[262144] = "Longbows",
	[524288] = "Longswords",
	[1048576] = "Maces",
	[2097152] = "MartialWeapons",
	[4194304] = "Mauls",
	[8388608] = "MediumArmor",
	[16777216] = "Morningstars",
	[33554432] = "Pikes",
	[67108864] = "Quarterstaffs",
	[134217728] = "Rapiers",
	[268435456] = "Scimitars",
	[536870912] = "Shields",
	[1073741824] = "Shortbows",
	[2147483648] = "Shortswords",
	[4294967296] = "Sickles",
	[8589934592] = "SimpleWeapons",
	[17179869184] = "Slings",
	[34359738368] = "Spears",
	[68719476736] = "Tridents",
	[137438953472] = "Warhammers",
	[274877906944] = "Warpicks",
	[549755813888] = "MusicalInstrument",
}

--- @enum ProgressionSelectorType
Ext_Enums.ProgressionSelectorType = {
	None = 0,
	Race = 1,
	SubRace = 2,
	Class = 3,
	SubClass = 4,
	Feat = 5,
	Passive = 6,
	[0] = "None",
	[1] = "Race",
	[2] = "SubRace",
	[3] = "Class",
	[4] = "SubClass",
	[5] = "Feat",
	[6] = "Passive",
}

--- @enum ProgressionType
Ext_Enums.ProgressionType = {
	Class = 0,
	SubClass = 1,
	Race = 2,
	Passive = 3,
	[0] = "Class",
	[1] = "SubClass",
	[2] = "Race",
	[3] = "Passive",
}

--- @enum ProjectileTypeIds
Ext_Enums.ProjectileTypeIds = {
	Physical = 1,
	Magical = 2,
	[1] = "Physical",
	[2] = "Magical",
}

--- @enum RequirementType
Ext_Enums.RequirementType = {
	None = 0,
	Level = 1,
	Strength = 2,
	Dexterity = 3,
	Intelligence = 4,
	Constitution = 5,
	Wisdom = 6,
	PainReflection = 8,
	Ranged = 9,
	Reflexes = 10,
	Vitality = 11,
	Sourcery = 12,
	Telekinesis = 13,
	Necromancy = 15,
	Summoning = 16,
	Sneaking = 17,
	Pickpocket = 18,
	Loremaster = 19,
	Barter = 20,
	Charm = 21,
	Intimidate = 22,
	Reason = 23,
	Persuasion = 24,
	Combat = 25,
	TurnBased = 26,
	Immobile = 27,
	Tag = 28,
	[0] = "None",
	[1] = "Level",
	[2] = "Strength",
	[3] = "Dexterity",
	[4] = "Intelligence",
	[5] = "Constitution",
	[6] = "Wisdom",
	[8] = "PainReflection",
	[9] = "Ranged",
	[10] = "Reflexes",
	[11] = "Vitality",
	[12] = "Sourcery",
	[13] = "Telekinesis",
	[15] = "Necromancy",
	[16] = "Summoning",
	[17] = "Sneaking",
	[18] = "Pickpocket",
	[19] = "Loremaster",
	[20] = "Barter",
	[21] = "Charm",
	[22] = "Intimidate",
	[23] = "Reason",
	[24] = "Persuasion",
	[25] = "Combat",
	[26] = "TurnBased",
	[27] = "Immobile",
	[28] = "Tag",
}

--- @enum ResistanceBoostFlags
Ext_Enums.ResistanceBoostFlags = {
	BelowDamageThreshold = 1,
	ResistantToMagical = 2,
	ImmuneToMagical = 4,
	VulnerableToMagical = 8,
	ResistantToNonMagical = 16,
	ImmuneToNonMagical = 32,
	VulnerableToNonMagical = 64,
	[1] = "BelowDamageThreshold",
	[2] = "ResistantToMagical",
	[4] = "ImmuneToMagical",
	[8] = "VulnerableToMagical",
	[16] = "ResistantToNonMagical",
	[32] = "ImmuneToNonMagical",
	[64] = "VulnerableToNonMagical",
}

--- @enum ResourceBankType
Ext_Enums.ResourceBankType = {
	Visual = 0,
	VisualSet = 1,
	Animation = 2,
	AnimationSet = 3,
	Texture = 4,
	Material = 5,
	Physics = 6,
	Effect = 7,
	Script = 8,
	Sound = 9,
	Lighting = 10,
	Atmosphere = 11,
	AnimationBlueprint = 12,
	MeshProxy = 13,
	MaterialSet = 14,
	BlendSpace = 15,
	FCurve = 16,
	Timeline = 17,
	Dialog = 18,
	VoiceBark = 19,
	TileSet = 20,
	IKRig = 21,
	Skeleton = 22,
	VirtualTexture = 23,
	TerrainBrush = 24,
	ColorList = 25,
	CharacterVisual = 26,
	MaterialPreset = 27,
	SkinPreset = 28,
	ClothCollider = 29,
	DiffusionProfile = 30,
	LightCookie = 31,
	TimelineScene = 32,
	SkeletonMirrorTable = 33,
	Sentinel = 34,
	[0] = "Visual",
	[1] = "VisualSet",
	[2] = "Animation",
	[3] = "AnimationSet",
	[4] = "Texture",
	[5] = "Material",
	[6] = "Physics",
	[7] = "Effect",
	[8] = "Script",
	[9] = "Sound",
	[10] = "Lighting",
	[11] = "Atmosphere",
	[12] = "AnimationBlueprint",
	[13] = "MeshProxy",
	[14] = "MaterialSet",
	[15] = "BlendSpace",
	[16] = "FCurve",
	[17] = "Timeline",
	[18] = "Dialog",
	[19] = "VoiceBark",
	[20] = "TileSet",
	[21] = "IKRig",
	[22] = "Skeleton",
	[23] = "VirtualTexture",
	[24] = "TerrainBrush",
	[25] = "ColorList",
	[26] = "CharacterVisual",
	[27] = "MaterialPreset",
	[28] = "SkinPreset",
	[29] = "ClothCollider",
	[30] = "DiffusionProfile",
	[31] = "LightCookie",
	[32] = "TimelineScene",
	[33] = "SkeletonMirrorTable",
	[34] = "Sentinel",
}

--- @enum ResourceReplenishType
Ext_Enums.ResourceReplenishType = {
	Never = 1,
	Default = 2,
	Combat = 4,
	Rest = 8,
	ShortRest = 16,
	FullRest = 32,
	ExhaustedRest = 64,
	[1] = "Never",
	[2] = "Default",
	[4] = "Combat",
	[8] = "Rest",
	[16] = "ShortRest",
	[32] = "FullRest",
	[64] = "ExhaustedRest",
}

--- @enum RestCancelReason
Ext_Enums.RestCancelReason = {
	None = 0,
	Validation = 1,
	Script = 2,
	RestTypeSelectionCanceled = 3,
	ValidationChangedDuringSelection = 4,
	AnotherPlayerCanceled = 5,
	ForcedCancelOnLoad = 6,
	[0] = "None",
	[1] = "Validation",
	[2] = "Script",
	[3] = "RestTypeSelectionCanceled",
	[4] = "ValidationChangedDuringSelection",
	[5] = "AnotherPlayerCanceled",
	[6] = "ForcedCancelOnLoad",
}

--- @enum RestErrorFlags
Ext_Enums.RestErrorFlags = {
	GlobalDisabled = 1,
	Script = 2,
	Dialog = 4,
	Combat = 8,
	FTB = 16,
	NotEnoughResources = 32,
	DownedOrDead = 64,
	DropInCCActive = 128,
	Location = 256,
	[1] = "GlobalDisabled",
	[2] = "Script",
	[4] = "Dialog",
	[8] = "Combat",
	[16] = "FTB",
	[32] = "NotEnoughResources",
	[64] = "DownedOrDead",
	[128] = "DropInCCActive",
	[256] = "Location",
}

--- @enum RestFlags
Ext_Enums.RestFlags = {
	CanShortRest = 1,
	CanLongRest = 2,
	Unknown0x04 = 4,
	[1] = "CanShortRest",
	[2] = "CanLongRest",
	[4] = "Unknown0x04",
}

--- @enum RollCritical
Ext_Enums.RollCritical = {
	None = 0,
	Success = 1,
	Fail = 2,
	[0] = "None",
	[1] = "Success",
	[2] = "Fail",
}

--- @enum SDLControllerAxis
Ext_Enums.SDLControllerAxis = {
	LeftX = 0,
	LeftY = 1,
	RightX = 2,
	RightY = 3,
	TriggerLeft = 4,
	TriggerRight = 5,
	[0] = "LeftX",
	[1] = "LeftY",
	[2] = "RightX",
	[3] = "RightY",
	[4] = "TriggerLeft",
	[5] = "TriggerRight",
}

--- @enum SDLControllerButton
Ext_Enums.SDLControllerButton = {
	A = 0,
	B = 1,
	X = 2,
	Y = 3,
	Back = 4,
	Guide = 5,
	Start = 6,
	LeftStick = 7,
	RightStick = 8,
	LeftShoulder = 9,
	RightShoulder = 10,
	DPadUp = 11,
	DPadDown = 12,
	DPadLeft = 13,
	DPadRight = 14,
	Misc1 = 15,
	Paddle1 = 16,
	Paddle2 = 17,
	Paddle3 = 18,
	Paddle4 = 19,
	Touchpad = 20,
	[0] = "A",
	[1] = "B",
	[2] = "X",
	[3] = "Y",
	[4] = "Back",
	[5] = "Guide",
	[6] = "Start",
	[7] = "LeftStick",
	[8] = "RightStick",
	[9] = "LeftShoulder",
	[10] = "RightShoulder",
	[11] = "DPadUp",
	[12] = "DPadDown",
	[13] = "DPadLeft",
	[14] = "DPadRight",
	[15] = "Misc1",
	[16] = "Paddle1",
	[17] = "Paddle2",
	[18] = "Paddle3",
	[19] = "Paddle4",
	[20] = "Touchpad",
}

--- @enum SDLKeyEvent
Ext_Enums.SDLKeyEvent = {
	KeyDown = 0,
	KeyUp = 1,
	[0] = "KeyDown",
	[1] = "KeyUp",
}

--- @enum SDLKeyModifier
Ext_Enums.SDLKeyModifier = {
	LShift = 1,
	RShift = 2,
	LCtrl = 64,
	RCtrl = 128,
	LAlt = 256,
	RAlt = 512,
	LGui = 1024,
	RGui = 2048,
	Num = 4096,
	Caps = 8192,
	Mode = 16384,
	Scroll = 32768,
	[1] = "LShift",
	[2] = "RShift",
	[64] = "LCtrl",
	[128] = "RCtrl",
	[256] = "LAlt",
	[512] = "RAlt",
	[1024] = "LGui",
	[2048] = "RGui",
	[4096] = "Num",
	[8192] = "Caps",
	[16384] = "Mode",
	[32768] = "Scroll",
}

--- @enum SDLScanCode
Ext_Enums.SDLScanCode = {
	UNKNOWN = 0,
	A = 4,
	B = 5,
	C = 6,
	D = 7,
	E = 8,
	F = 9,
	G = 10,
	H = 11,
	I = 12,
	J = 13,
	K = 14,
	L = 15,
	M = 16,
	N = 17,
	O = 18,
	P = 19,
	Q = 20,
	R = 21,
	S = 22,
	T = 23,
	U = 24,
	V = 25,
	W = 26,
	X = 27,
	Y = 28,
	Z = 29,
	NUM_1 = 30,
	NUM_2 = 31,
	NUM_3 = 32,
	NUM_4 = 33,
	NUM_5 = 34,
	NUM_6 = 35,
	NUM_7 = 36,
	NUM_8 = 37,
	NUM_9 = 38,
	NUM_0 = 39,
	RETURN = 40,
	ESCAPE = 41,
	BACKSPACE = 42,
	TAB = 43,
	SPACE = 44,
	MINUS = 45,
	EQUALS = 46,
	LEFTBRACKET = 47,
	RIGHTBRACKET = 48,
	BACKSLASH = 49,
	NONUSHASH = 50,
	SEMICOLON = 51,
	APOSTROPHE = 52,
	GRAVE = 53,
	COMMA = 54,
	PERIOD = 55,
	SLASH = 56,
	CAPSLOCK = 57,
	F1 = 58,
	F2 = 59,
	F3 = 60,
	F4 = 61,
	F5 = 62,
	F6 = 63,
	F7 = 64,
	F8 = 65,
	F9 = 66,
	F10 = 67,
	F11 = 68,
	F12 = 69,
	PRINTSCREEN = 70,
	SCROLLLOCK = 71,
	PAUSE = 72,
	INSERT = 73,
	HOME = 74,
	PAGEUP = 75,
	DEL = 76,
	END = 77,
	PAGEDOWN = 78,
	RIGHT = 79,
	LEFT = 80,
	DOWN = 81,
	UP = 82,
	NUMLOCKCLEAR = 83,
	KP_DIVIDE = 84,
	KP_MULTIPLY = 85,
	KP_MINUS = 86,
	KP_PLUS = 87,
	KP_ENTER = 88,
	KP_1 = 89,
	KP_2 = 90,
	KP_3 = 91,
	KP_4 = 92,
	KP_5 = 93,
	KP_6 = 94,
	KP_7 = 95,
	KP_8 = 96,
	KP_9 = 97,
	KP_0 = 98,
	KP_PERIOD = 99,
	NONUSBACKSLASH = 100,
	APPLICATION = 101,
	POWER = 102,
	KP_EQUALS = 103,
	F13 = 104,
	F14 = 105,
	F15 = 106,
	F16 = 107,
	F17 = 108,
	F18 = 109,
	F19 = 110,
	F20 = 111,
	F21 = 112,
	F22 = 113,
	F23 = 114,
	F24 = 115,
	EXECUTE = 116,
	HELP = 117,
	MENU = 118,
	SELECT = 119,
	STOP = 120,
	AGAIN = 121,
	UNDO = 122,
	CUT = 123,
	COPY = 124,
	PASTE = 125,
	FIND = 126,
	MUTE = 127,
	VOLUMEUP = 128,
	VOLUMEDOWN = 129,
	KP_COMMA = 133,
	KP_EQUALSAS400 = 134,
	INTERNATIONAL1 = 135,
	INTERNATIONAL2 = 136,
	INTERNATIONAL3 = 137,
	INTERNATIONAL4 = 138,
	INTERNATIONAL5 = 139,
	INTERNATIONAL6 = 140,
	INTERNATIONAL7 = 141,
	INTERNATIONAL8 = 142,
	INTERNATIONAL9 = 143,
	LANG1 = 144,
	LANG2 = 145,
	LANG3 = 146,
	LANG4 = 147,
	LANG5 = 148,
	LANG6 = 149,
	LANG7 = 150,
	LANG8 = 151,
	LANG9 = 152,
	ALTERASE = 153,
	SYSREQ = 154,
	CANCEL = 155,
	CLEAR = 156,
	PRIOR = 157,
	RETURN2 = 158,
	SEPARATOR = 159,
	KEY_OUT = 160,
	OPER = 161,
	CLEARAGAIN = 162,
	CRSEL = 163,
	EXSEL = 164,
	KP_00 = 176,
	KP_000 = 177,
	THOUSANDSSEPARATOR = 178,
	DECIMALSEPARATOR = 179,
	CURRENCYUNIT = 180,
	CURRENCYSUBUNIT = 181,
	KP_LEFTPAREN = 182,
	KP_RIGHTPAREN = 183,
	KP_LEFTBRACE = 184,
	KP_RIGHTBRACE = 185,
	KP_TAB = 186,
	KP_BACKSPACE = 187,
	KP_A = 188,
	KP_B = 189,
	KP_C = 190,
	KP_D = 191,
	KP_E = 192,
	KP_F = 193,
	KP_XOR = 194,
	KP_POWER = 195,
	KP_PERCENT = 196,
	KP_LESS = 197,
	KP_GREATER = 198,
	KP_AMPERSAND = 199,
	KP_DBLAMPERSAND = 200,
	KP_VERTICALBAR = 201,
	KP_DBLVERTICALBAR = 202,
	KP_COLON = 203,
	KP_HASH = 204,
	KP_SPACE = 205,
	KP_AT = 206,
	KP_EXCLAM = 207,
	KP_MEMSTORE = 208,
	KP_MEMRECALL = 209,
	KP_MEMCLEAR = 210,
	KP_MEMADD = 211,
	KP_MEMSUBTRACT = 212,
	KP_MEMMULTIPLY = 213,
	KP_MEMDIVIDE = 214,
	KP_PLUSMINUS = 215,
	KP_CLEAR = 216,
	KP_CLEARENTRY = 217,
	KP_BINARY = 218,
	KP_OCTAL = 219,
	KP_DECIMAL = 220,
	KP_HEXADECIMAL = 221,
	LCTRL = 224,
	LSHIFT = 225,
	LALT = 226,
	LGUI = 227,
	RCTRL = 228,
	RSHIFT = 229,
	RALT = 230,
	RGUI = 231,
	MODE = 257,
	AUDIONEXT = 258,
	AUDIOPREV = 259,
	AUDIOSTOP = 260,
	AUDIOPLAY = 261,
	AUDIOMUTE = 262,
	MEDIASELECT = 263,
	WWW = 264,
	MAIL = 265,
	CALCULATOR = 266,
	COMPUTER = 267,
	AC_SEARCH = 268,
	AC_HOME = 269,
	AC_BACK = 270,
	AC_FORWARD = 271,
	AC_STOP = 272,
	AC_REFRESH = 273,
	AC_BOOKMARKS = 274,
	BRIGHTNESSDOWN = 275,
	BRIGHTNESSUP = 276,
	DISPLAYSWITCH = 277,
	KBDILLUMTOGGLE = 278,
	KBDILLUMDOWN = 279,
	KBDILLUMUP = 280,
	EJECT = 281,
	SLEEP = 282,
	APP1 = 283,
	APP2 = 284,
	AUDIOREWIND = 285,
	AUDIOFASTFORWARD = 286,
	SOFTLEFT = 287,
	SOFTRIGHT = 288,
	CALL = 289,
	ENDCALL = 290,
	[0] = "UNKNOWN",
	[4] = "A",
	[5] = "B",
	[6] = "C",
	[7] = "D",
	[8] = "E",
	[9] = "F",
	[10] = "G",
	[11] = "H",
	[12] = "I",
	[13] = "J",
	[14] = "K",
	[15] = "L",
	[16] = "M",
	[17] = "N",
	[18] = "O",
	[19] = "P",
	[20] = "Q",
	[21] = "R",
	[22] = "S",
	[23] = "T",
	[24] = "U",
	[25] = "V",
	[26] = "W",
	[27] = "X",
	[28] = "Y",
	[29] = "Z",
	[30] = "NUM_1",
	[31] = "NUM_2",
	[32] = "NUM_3",
	[33] = "NUM_4",
	[34] = "NUM_5",
	[35] = "NUM_6",
	[36] = "NUM_7",
	[37] = "NUM_8",
	[38] = "NUM_9",
	[39] = "NUM_0",
	[40] = "RETURN",
	[41] = "ESCAPE",
	[42] = "BACKSPACE",
	[43] = "TAB",
	[44] = "SPACE",
	[45] = "MINUS",
	[46] = "EQUALS",
	[47] = "LEFTBRACKET",
	[48] = "RIGHTBRACKET",
	[49] = "BACKSLASH",
	[50] = "NONUSHASH",
	[51] = "SEMICOLON",
	[52] = "APOSTROPHE",
	[53] = "GRAVE",
	[54] = "COMMA",
	[55] = "PERIOD",
	[56] = "SLASH",
	[57] = "CAPSLOCK",
	[58] = "F1",
	[59] = "F2",
	[60] = "F3",
	[61] = "F4",
	[62] = "F5",
	[63] = "F6",
	[64] = "F7",
	[65] = "F8",
	[66] = "F9",
	[67] = "F10",
	[68] = "F11",
	[69] = "F12",
	[70] = "PRINTSCREEN",
	[71] = "SCROLLLOCK",
	[72] = "PAUSE",
	[73] = "INSERT",
	[74] = "HOME",
	[75] = "PAGEUP",
	[76] = "DEL",
	[77] = "END",
	[78] = "PAGEDOWN",
	[79] = "RIGHT",
	[80] = "LEFT",
	[81] = "DOWN",
	[82] = "UP",
	[83] = "NUMLOCKCLEAR",
	[84] = "KP_DIVIDE",
	[85] = "KP_MULTIPLY",
	[86] = "KP_MINUS",
	[87] = "KP_PLUS",
	[88] = "KP_ENTER",
	[89] = "KP_1",
	[90] = "KP_2",
	[91] = "KP_3",
	[92] = "KP_4",
	[93] = "KP_5",
	[94] = "KP_6",
	[95] = "KP_7",
	[96] = "KP_8",
	[97] = "KP_9",
	[98] = "KP_0",
	[99] = "KP_PERIOD",
	[100] = "NONUSBACKSLASH",
	[101] = "APPLICATION",
	[102] = "POWER",
	[103] = "KP_EQUALS",
	[104] = "F13",
	[105] = "F14",
	[106] = "F15",
	[107] = "F16",
	[108] = "F17",
	[109] = "F18",
	[110] = "F19",
	[111] = "F20",
	[112] = "F21",
	[113] = "F22",
	[114] = "F23",
	[115] = "F24",
	[116] = "EXECUTE",
	[117] = "HELP",
	[118] = "MENU",
	[119] = "SELECT",
	[120] = "STOP",
	[121] = "AGAIN",
	[122] = "UNDO",
	[123] = "CUT",
	[124] = "COPY",
	[125] = "PASTE",
	[126] = "FIND",
	[127] = "MUTE",
	[128] = "VOLUMEUP",
	[129] = "VOLUMEDOWN",
	[133] = "KP_COMMA",
	[134] = "KP_EQUALSAS400",
	[135] = "INTERNATIONAL1",
	[136] = "INTERNATIONAL2",
	[137] = "INTERNATIONAL3",
	[138] = "INTERNATIONAL4",
	[139] = "INTERNATIONAL5",
	[140] = "INTERNATIONAL6",
	[141] = "INTERNATIONAL7",
	[142] = "INTERNATIONAL8",
	[143] = "INTERNATIONAL9",
	[144] = "LANG1",
	[145] = "LANG2",
	[146] = "LANG3",
	[147] = "LANG4",
	[148] = "LANG5",
	[149] = "LANG6",
	[150] = "LANG7",
	[151] = "LANG8",
	[152] = "LANG9",
	[153] = "ALTERASE",
	[154] = "SYSREQ",
	[155] = "CANCEL",
	[156] = "CLEAR",
	[157] = "PRIOR",
	[158] = "RETURN2",
	[159] = "SEPARATOR",
	[160] = "KEY_OUT",
	[161] = "OPER",
	[162] = "CLEARAGAIN",
	[163] = "CRSEL",
	[164] = "EXSEL",
	[176] = "KP_00",
	[177] = "KP_000",
	[178] = "THOUSANDSSEPARATOR",
	[179] = "DECIMALSEPARATOR",
	[180] = "CURRENCYUNIT",
	[181] = "CURRENCYSUBUNIT",
	[182] = "KP_LEFTPAREN",
	[183] = "KP_RIGHTPAREN",
	[184] = "KP_LEFTBRACE",
	[185] = "KP_RIGHTBRACE",
	[186] = "KP_TAB",
	[187] = "KP_BACKSPACE",
	[188] = "KP_A",
	[189] = "KP_B",
	[190] = "KP_C",
	[191] = "KP_D",
	[192] = "KP_E",
	[193] = "KP_F",
	[194] = "KP_XOR",
	[195] = "KP_POWER",
	[196] = "KP_PERCENT",
	[197] = "KP_LESS",
	[198] = "KP_GREATER",
	[199] = "KP_AMPERSAND",
	[200] = "KP_DBLAMPERSAND",
	[201] = "KP_VERTICALBAR",
	[202] = "KP_DBLVERTICALBAR",
	[203] = "KP_COLON",
	[204] = "KP_HASH",
	[205] = "KP_SPACE",
	[206] = "KP_AT",
	[207] = "KP_EXCLAM",
	[208] = "KP_MEMSTORE",
	[209] = "KP_MEMRECALL",
	[210] = "KP_MEMCLEAR",
	[211] = "KP_MEMADD",
	[212] = "KP_MEMSUBTRACT",
	[213] = "KP_MEMMULTIPLY",
	[214] = "KP_MEMDIVIDE",
	[215] = "KP_PLUSMINUS",
	[216] = "KP_CLEAR",
	[217] = "KP_CLEARENTRY",
	[218] = "KP_BINARY",
	[219] = "KP_OCTAL",
	[220] = "KP_DECIMAL",
	[221] = "KP_HEXADECIMAL",
	[224] = "LCTRL",
	[225] = "LSHIFT",
	[226] = "LALT",
	[227] = "LGUI",
	[228] = "RCTRL",
	[229] = "RSHIFT",
	[230] = "RALT",
	[231] = "RGUI",
	[257] = "MODE",
	[258] = "AUDIONEXT",
	[259] = "AUDIOPREV",
	[260] = "AUDIOSTOP",
	[261] = "AUDIOPLAY",
	[262] = "AUDIOMUTE",
	[263] = "MEDIASELECT",
	[264] = "WWW",
	[265] = "MAIL",
	[266] = "CALCULATOR",
	[267] = "COMPUTER",
	[268] = "AC_SEARCH",
	[269] = "AC_HOME",
	[270] = "AC_BACK",
	[271] = "AC_FORWARD",
	[272] = "AC_STOP",
	[273] = "AC_REFRESH",
	[274] = "AC_BOOKMARKS",
	[275] = "BRIGHTNESSDOWN",
	[276] = "BRIGHTNESSUP",
	[277] = "DISPLAYSWITCH",
	[278] = "KBDILLUMTOGGLE",
	[279] = "KBDILLUMDOWN",
	[280] = "KBDILLUMUP",
	[281] = "EJECT",
	[282] = "SLEEP",
	[283] = "APP1",
	[284] = "APP2",
	[285] = "AUDIOREWIND",
	[286] = "AUDIOFASTFORWARD",
	[287] = "SOFTLEFT",
	[288] = "SOFTRIGHT",
	[289] = "CALL",
	[290] = "ENDCALL",
}

--- @enum SceneObjectType
Ext_Enums.SceneObjectType = {
	RenderableObject = 0,
	Light = 1,
	LightProbe = 2,
	FogVolume = 3,
	Visual = 4,
	Effect = 5,
	InstanceBatchRenderableObject = 6,
	InstancingObject = 7,
	CullableInstance = 8,
	DeferredDecal = 9,
	RoomTriggerVisual = 10,
	[0] = "RenderableObject",
	[1] = "Light",
	[2] = "LightProbe",
	[3] = "FogVolume",
	[4] = "Visual",
	[5] = "Effect",
	[6] = "InstanceBatchRenderableObject",
	[7] = "InstancingObject",
	[8] = "CullableInstance",
	[9] = "DeferredDecal",
	[10] = "RoomTriggerVisual",
}

--- @enum SceneryFlags
Ext_Enums.SceneryFlags = {
	WalkThrough = 1,
	CanClimbOn = 2,
	CanWalkOn = 4,
	CanShootThrough = 8,
	CanSeeThrough = 16,
	Wadable = 32,
	CanSeeThrough2 = 64,
	CollideWithCamera = 128,
	AllowCameraMovement = 256,
	IsBlocker = 512,
	[1] = "WalkThrough",
	[2] = "CanClimbOn",
	[4] = "CanWalkOn",
	[8] = "CanShootThrough",
	[16] = "CanSeeThrough",
	[32] = "Wadable",
	[64] = "CanSeeThrough2",
	[128] = "CollideWithCamera",
	[256] = "AllowCameraMovement",
	[512] = "IsBlocker",
}

--- @enum ServerActionType
Ext_Enums.ServerActionType = {
	Animation = 2,
	MoveItem = 4,
	KnockedDown = 5,
	UseItem = 6,
	CombineItem = 8,
	TeleportFalling = 9,
	Incapacitated = 10,
	[2] = "Animation",
	[4] = "MoveItem",
	[5] = "KnockedDown",
	[6] = "UseItem",
	[8] = "CombineItem",
	[9] = "TeleportFalling",
	[10] = "Incapacitated",
}

--- @enum ServerBehaviourType
Ext_Enums.ServerBehaviourType = {
	Move = 2,
	Wander = 3,
	Force = 4,
	PickupItem = 5,
	MoveItem = 6,
	MoveAndUseItem = 7,
	Appear = 10,
	MoveToAndTalk = 11,
	FollowCharacter = 12,
	Patrol = 13,
	[2] = "Move",
	[3] = "Wander",
	[4] = "Force",
	[5] = "PickupItem",
	[6] = "MoveItem",
	[7] = "MoveAndUseItem",
	[10] = "Appear",
	[11] = "MoveToAndTalk",
	[12] = "FollowCharacter",
	[13] = "Patrol",
}

--- @enum ServerCharacterFlags
Ext_Enums.ServerCharacterFlags = {
	IsPlayer = 1,
	Multiplayer = 2,
	InParty = 4,
	HostControl = 8,
	Activated = 16,
	OffStage = 32,
	Dead = 64,
	HasOwner = 128,
	InDialog = 256,
	Summon = 512,
	Invulnerable = 1024,
	CannotDie = 2048,
	StoryNPC = 4096,
	CharacterControl = 8192,
	Loaded = 16384,
	Unknown8000 = 32768,
	Invisible = 65536,
	StoryControlled = 131072,
	FindValidPositionOnActivate = 262144,
	CannotAttachToGroups = 524288,
	CharCreationInProgress = 1048576,
	CharacterCreationFinished = 2097152,
	Floating = 4194304,
	FightMode = 8388608,
	SpotSneakers = 16777216,
	Temporary = 33554432,
	CannotMove = 67108864,
	CannotRun = 134217728,
	WalkThrough = 268435456,
	CoverAmount = 536870912,
	CanShootThrough = 1073741824,
	Unknown80000000 = 2147483648,
	DontCacheTemplate = 4294967296,
	ReservedForDialog = 8589934592,
	PartyFollower = 17179869184,
	DisableWaypointUsage = 34359738368,
	IgnoresTriggers = 68719476736,
	Totem = 137438953472,
	IsTrading = 274877906944,
	SteeringEnabled = 549755813888,
	Unknown10000000000 = 1099511627776,
	IsCompanion_M = 2199023255552,
	Unknown40000000000 = 4398046511104,
	Deactivated = 8796093022208,
	CustomLookEnabled = 17592186044416,
	IsHuge = 35184372088832,
	DisableCulling = 70368744177664,
	DoNotFaceFlag = 140737488355328,
	NeedsMakePlayerUpdate = 281474976710656,
	GMReroll = 562949953421312,
	Detached = 1125899906842624,
	RequestStartTurn = 2251799813685248,
	DeferredRemoveEscapist = 4503599627370496,
	LevelTransitionPending = 9007199254740992,
	RegisteredForAutomatedDialog = 18014398509481984,
	Unknown80000000000000 = 36028797018963968,
	[1] = "IsPlayer",
	[2] = "Multiplayer",
	[4] = "InParty",
	[8] = "HostControl",
	[16] = "Activated",
	[32] = "OffStage",
	[64] = "Dead",
	[128] = "HasOwner",
	[256] = "InDialog",
	[512] = "Summon",
	[1024] = "Invulnerable",
	[2048] = "CannotDie",
	[4096] = "StoryNPC",
	[8192] = "CharacterControl",
	[16384] = "Loaded",
	[32768] = "Unknown8000",
	[65536] = "Invisible",
	[131072] = "StoryControlled",
	[262144] = "FindValidPositionOnActivate",
	[524288] = "CannotAttachToGroups",
	[1048576] = "CharCreationInProgress",
	[2097152] = "CharacterCreationFinished",
	[4194304] = "Floating",
	[8388608] = "FightMode",
	[16777216] = "SpotSneakers",
	[33554432] = "Temporary",
	[67108864] = "CannotMove",
	[134217728] = "CannotRun",
	[268435456] = "WalkThrough",
	[536870912] = "CoverAmount",
	[1073741824] = "CanShootThrough",
	[2147483648] = "Unknown80000000",
	[4294967296] = "DontCacheTemplate",
	[8589934592] = "ReservedForDialog",
	[17179869184] = "PartyFollower",
	[34359738368] = "DisableWaypointUsage",
	[68719476736] = "IgnoresTriggers",
	[137438953472] = "Totem",
	[274877906944] = "IsTrading",
	[549755813888] = "SteeringEnabled",
	[1099511627776] = "Unknown10000000000",
	[2199023255552] = "IsCompanion_M",
	[4398046511104] = "Unknown40000000000",
	[8796093022208] = "Deactivated",
	[17592186044416] = "CustomLookEnabled",
	[35184372088832] = "IsHuge",
	[70368744177664] = "DisableCulling",
	[140737488355328] = "DoNotFaceFlag",
	[281474976710656] = "NeedsMakePlayerUpdate",
	[562949953421312] = "GMReroll",
	[1125899906842624] = "Detached",
	[2251799813685248] = "RequestStartTurn",
	[4503599627370496] = "DeferredRemoveEscapist",
	[9007199254740992] = "LevelTransitionPending",
	[18014398509481984] = "RegisteredForAutomatedDialog",
	[36028797018963968] = "Unknown80000000000000",
}

--- @enum ServerCharacterFlags2
Ext_Enums.ServerCharacterFlags2 = {
	Global = 1,
	TreasureGeneratedForTrader = 2,
	Trader = 4,
	IsResurrected = 8,
	TeleportingPartyHack = 16,
	IsPet = 32,
	IsLeveledByStory = 64,
	CanGossip = 128,
	[1] = "Global",
	[2] = "TreasureGeneratedForTrader",
	[4] = "Trader",
	[8] = "IsResurrected",
	[16] = "TeleportingPartyHack",
	[32] = "IsPet",
	[64] = "IsLeveledByStory",
	[128] = "CanGossip",
}

--- @enum ServerCharacterFlags3
Ext_Enums.ServerCharacterFlags3 = {
	NeedsPlacementSnapping = 1,
	CrimeWarningsEnabled = 2,
	SpreadActivation = 8,
	TreasureEquip = 16,
	[1] = "NeedsPlacementSnapping",
	[2] = "CrimeWarningsEnabled",
	[8] = "SpreadActivation",
	[16] = "TreasureEquip",
}

--- @enum ServerConcentrationRequestType
Ext_Enums.ServerConcentrationRequestType = {
	RemoveTarget = 1,
	AddTarget = 2,
	[1] = "RemoveTarget",
	[2] = "AddTarget",
}

--- @enum ServerGameState
Ext_Enums.ServerGameState = {
	Unknown = 0,
	Uninitialized = 1,
	Init = 2,
	Idle = 3,
	Exit = 4,
	LoadLevel = 5,
	LoadModule = 6,
	LoadSession = 7,
	UnloadLevel = 8,
	UnloadModule = 9,
	UnloadSession = 10,
	Sync = 11,
	Paused = 12,
	Running = 13,
	Save = 14,
	Disconnect = 15,
	BuildStory = 16,
	ReloadStory = 17,
	[0] = "Unknown",
	[1] = "Uninitialized",
	[2] = "Init",
	[3] = "Idle",
	[4] = "Exit",
	[5] = "LoadLevel",
	[6] = "LoadModule",
	[7] = "LoadSession",
	[8] = "UnloadLevel",
	[9] = "UnloadModule",
	[10] = "UnloadSession",
	[11] = "Sync",
	[12] = "Paused",
	[13] = "Running",
	[14] = "Save",
	[15] = "Disconnect",
	[16] = "BuildStory",
	[17] = "ReloadStory",
}

--- @enum ServerItemFlags
Ext_Enums.ServerItemFlags = {
	Activated = 1,
	OffStage = 2,
	CanBePickedUp = 4,
	CanBeMoved = 8,
	WalkOn = 16,
	WalkThrough = 32,
	NoCover = 64,
	CanShootThrough = 128,
	CanUse = 256,
	InteractionDisabled = 512,
	TransformChanged = 1024,
	Destroyed = 2048,
	IsMoving = 4096,
	CanOnlyBeUsedByOwner = 8192,
	LoadedTemplate = 16384,
	IsDoor = 32768,
	Invisible = 65536,
	StoryItem = 131072,
	ClientSync1 = 262144,
	WakePhysics = 524288,
	Summon = 1048576,
	IsContainer = 2097152,
	HideHP = 4194304,
	LuckyFind = 8388608,
	FreezeGravity = 16777216,
	ForceSync = 33554432,
	Known = 67108864,
	Invisible2 = 134217728,
	DisableSync = 268435456,
	IsLadder = 536870912,
	ReservedForDialog = 1073741824,
	ForceAffectedByAura = 2147483648,
	DisableInventoryView80 = 4294967296,
	DontAddToHotbar = 8589934592,
	PositionChanged = 17179869184,
	Totem = 34359738368,
	Destroy = 137438953472,
	GMFolding = 274877906944,
	InUse = 549755813888,
	Sticky = 1099511627776,
	IsSecretDoor = 2199023255552,
	Floating = 4398046511104,
	IsSurfaceBlocker = 8796093022208,
	IsSurfaceCloudBlocker = 17592186044416,
	Invulnerable2 = 35184372088832,
	ForceClientSync = 70368744177664,
	InPartyInventory = 140737488355328,
	SourceContainer = 281474976710656,
	Frozen = 562949953421312,
	LevelTransitionPending = 1125899906842624,
	TeleportOnUse = 2251799813685248,
	InAutomatedDialog = 4503599627370496,
	PinnedContainer = 9007199254740992,
	CanClimbOn = 18014398509481984,
	[1] = "Activated",
	[2] = "OffStage",
	[4] = "CanBePickedUp",
	[8] = "CanBeMoved",
	[16] = "WalkOn",
	[32] = "WalkThrough",
	[64] = "NoCover",
	[128] = "CanShootThrough",
	[256] = "CanUse",
	[512] = "InteractionDisabled",
	[1024] = "TransformChanged",
	[2048] = "Destroyed",
	[4096] = "IsMoving",
	[8192] = "CanOnlyBeUsedByOwner",
	[16384] = "LoadedTemplate",
	[32768] = "IsDoor",
	[65536] = "Invisible",
	[131072] = "StoryItem",
	[262144] = "ClientSync1",
	[524288] = "WakePhysics",
	[1048576] = "Summon",
	[2097152] = "IsContainer",
	[4194304] = "HideHP",
	[8388608] = "LuckyFind",
	[16777216] = "FreezeGravity",
	[33554432] = "ForceSync",
	[67108864] = "Known",
	[134217728] = "Invisible2",
	[268435456] = "DisableSync",
	[536870912] = "IsLadder",
	[1073741824] = "ReservedForDialog",
	[2147483648] = "ForceAffectedByAura",
	[4294967296] = "DisableInventoryView80",
	[8589934592] = "DontAddToHotbar",
	[17179869184] = "PositionChanged",
	[34359738368] = "Totem",
	[137438953472] = "Destroy",
	[274877906944] = "GMFolding",
	[549755813888] = "InUse",
	[1099511627776] = "Sticky",
	[2199023255552] = "IsSecretDoor",
	[4398046511104] = "Floating",
	[8796093022208] = "IsSurfaceBlocker",
	[17592186044416] = "IsSurfaceCloudBlocker",
	[35184372088832] = "Invulnerable2",
	[70368744177664] = "ForceClientSync",
	[140737488355328] = "InPartyInventory",
	[281474976710656] = "SourceContainer",
	[562949953421312] = "Frozen",
	[1125899906842624] = "LevelTransitionPending",
	[2251799813685248] = "TeleportOnUse",
	[4503599627370496] = "InAutomatedDialog",
	[9007199254740992] = "PinnedContainer",
	[18014398509481984] = "CanClimbOn",
}

--- @enum ServerItemFlags2
Ext_Enums.ServerItemFlags2 = {
	UnsoldGenerated = 1,
	Global = 2,
	TreasureGenerated = 8,
	UnEquipLocked = 16,
	UseRemotely = 32,
	[1] = "UnsoldGenerated",
	[2] = "Global",
	[8] = "TreasureGenerated",
	[16] = "UnEquipLocked",
	[32] = "UseRemotely",
}

--- @enum ServerMovementType
Ext_Enums.ServerMovementType = {
	MoveTo = 2,
	MoveToSync = 3,
	Idle = 4,
	Climbing = 5,
	Falling = 6,
	Repose = 7,
	[2] = "MoveTo",
	[3] = "MoveToSync",
	[4] = "Idle",
	[5] = "Climbing",
	[6] = "Falling",
	[7] = "Repose",
}

--- @enum ServerSpellCastSystemEventType
Ext_Enums.ServerSpellCastSystemEventType = {
	Finish = 0,
	FinishRemove = 1,
	PreparationStart = 2,
	PreparationEnd = 3,
	LogicExecutionStart = 4,
	LogicExecutionStartRemove = 5,
	LogicExecutionEnd = 6,
	MoveDuringCastInitialize = 7,
	MoveDuringCastStart = 8,
	MoveDuringCastUpdate = 9,
	MoveDuringCastEnd = 10,
	TargetHit = 11,
	TargetHitRemove = 12,
	TargetHitInterruptRemove = 13,
	Cast = 14,
	CastRemove = 15,
	CastHit = 16,
	CastHitRemove = 17,
	CastTextKey = 18,
	CastTextKeyRemove = 19,
	CastTextKeyIndexIncrease = 20,
	WeaponSetRequest = 21,
	IsCastingRemove = 22,
	DestroySpellCastEntity = 23,
	DestroyEventRemove = 24,
	JumpStart = 25,
	PlayAnimation = 26,
	MovementEnd = 27,
	TargetReactionRemove = 28,
	TargetsChanged = 29,
	AddProjectileCache = 30,
	AddZoneRange = 31,
	AddSharedMovement = 32,
	AddServerMovement = 33,
	AddMovementInfo = 34,
	PreviewEnd = 35,
	CounteredSpellCast = 36,
	SpellRollAbort = 37,
	PrecalculationEnd = 38,
	PrecalculatedConditionals = 39,
	ThrowPickupPositionChanged = 40,
	PhaseChange = 41,
	CacheFlagChange = 42,
	UseCostsChange = 43,
	InitializeRolls = 44,
	AdvanceRoll = 45,
	AdvanceReaction = 46,
	FireProjectile = 47,
	ChangeProjectileDelayTimer = 48,
	PopDelayedProjectile = 49,
	InitializeIntermediateTargets = 50,
	ChangeCastHitDelayTimer = 51,
	AddDelayedCastHit = 52,
	PopDelayedCastHit = 53,
	ChangeCastPosition = 54,
	ChangeCastEndPosition = 55,
	ChangeMovementTransactionID = 56,
	CreateZone = 57,
	DoTeleport = 58,
	CreateWall = 59,
	CastOptionsChange = 60,
	TargetsChange = 61,
	PathfindRequest = 62,
	PathfindResults = 63,
	PathfindCleanup = 64,
	ChangeStoryActionId = 65,
	InterruptStart = 66,
	InterruptEventChange = 67,
	InterruptApplyResult = 68,
	AdvanceInterruptAnimationIndex = 69,
	InterruptEnd = 70,
	PrecalculatedInterruptors = 71,
	PushPausedAnimationEvents = 72,
	PurgePausedAnimationEvents = 73,
	ZoneRangeUpdate = 74,
	UpdateConditionals = 75,
	UpdateInterruptors = 76,
	LogicExecutionUpdate = 77,
	PreparationStarting = 78,
	[0] = "Finish",
	[1] = "FinishRemove",
	[2] = "PreparationStart",
	[3] = "PreparationEnd",
	[4] = "LogicExecutionStart",
	[5] = "LogicExecutionStartRemove",
	[6] = "LogicExecutionEnd",
	[7] = "MoveDuringCastInitialize",
	[8] = "MoveDuringCastStart",
	[9] = "MoveDuringCastUpdate",
	[10] = "MoveDuringCastEnd",
	[11] = "TargetHit",
	[12] = "TargetHitRemove",
	[13] = "TargetHitInterruptRemove",
	[14] = "Cast",
	[15] = "CastRemove",
	[16] = "CastHit",
	[17] = "CastHitRemove",
	[18] = "CastTextKey",
	[19] = "CastTextKeyRemove",
	[20] = "CastTextKeyIndexIncrease",
	[21] = "WeaponSetRequest",
	[22] = "IsCastingRemove",
	[23] = "DestroySpellCastEntity",
	[24] = "DestroyEventRemove",
	[25] = "JumpStart",
	[26] = "PlayAnimation",
	[27] = "MovementEnd",
	[28] = "TargetReactionRemove",
	[29] = "TargetsChanged",
	[30] = "AddProjectileCache",
	[31] = "AddZoneRange",
	[32] = "AddSharedMovement",
	[33] = "AddServerMovement",
	[34] = "AddMovementInfo",
	[35] = "PreviewEnd",
	[36] = "CounteredSpellCast",
	[37] = "SpellRollAbort",
	[38] = "PrecalculationEnd",
	[39] = "PrecalculatedConditionals",
	[40] = "ThrowPickupPositionChanged",
	[41] = "PhaseChange",
	[42] = "CacheFlagChange",
	[43] = "UseCostsChange",
	[44] = "InitializeRolls",
	[45] = "AdvanceRoll",
	[46] = "AdvanceReaction",
	[47] = "FireProjectile",
	[48] = "ChangeProjectileDelayTimer",
	[49] = "PopDelayedProjectile",
	[50] = "InitializeIntermediateTargets",
	[51] = "ChangeCastHitDelayTimer",
	[52] = "AddDelayedCastHit",
	[53] = "PopDelayedCastHit",
	[54] = "ChangeCastPosition",
	[55] = "ChangeCastEndPosition",
	[56] = "ChangeMovementTransactionID",
	[57] = "CreateZone",
	[58] = "DoTeleport",
	[59] = "CreateWall",
	[60] = "CastOptionsChange",
	[61] = "TargetsChange",
	[62] = "PathfindRequest",
	[63] = "PathfindResults",
	[64] = "PathfindCleanup",
	[65] = "ChangeStoryActionId",
	[66] = "InterruptStart",
	[67] = "InterruptEventChange",
	[68] = "InterruptApplyResult",
	[69] = "AdvanceInterruptAnimationIndex",
	[70] = "InterruptEnd",
	[71] = "PrecalculatedInterruptors",
	[72] = "PushPausedAnimationEvents",
	[73] = "PurgePausedAnimationEvents",
	[74] = "ZoneRangeUpdate",
	[75] = "UpdateConditionals",
	[76] = "UpdateInterruptors",
	[77] = "LogicExecutionUpdate",
	[78] = "PreparationStarting",
}

--- @enum ServerStatusFlags
Ext_Enums.ServerStatusFlags = {
	ForceStatus = 1,
	ForceFailStatus = 2,
	IgnoreStacking = 4,
	RequestClientSync = 8,
	Loaded = 16,
	RequestDelete = 32,
	RequestDeleteAtTurnEnd = 64,
	Started = 128,
	[1] = "ForceStatus",
	[2] = "ForceFailStatus",
	[4] = "IgnoreStacking",
	[8] = "RequestClientSync",
	[16] = "Loaded",
	[32] = "RequestDelete",
	[64] = "RequestDeleteAtTurnEnd",
	[128] = "Started",
}

--- @enum ServerStatusFlags2
Ext_Enums.ServerStatusFlags2 = {
	KeepAlive = 1,
	DontTickWhileOnSurface = 2,
	IsOnSourceSurface = 4,
	IsFromItem = 8,
	IsChanneled = 16,
	IsLifeTimeSet = 32,
	InitiateCombat = 64,
	Influence = 128,
	[1] = "KeepAlive",
	[2] = "DontTickWhileOnSurface",
	[4] = "IsOnSourceSurface",
	[8] = "IsFromItem",
	[16] = "IsChanneled",
	[32] = "IsLifeTimeSet",
	[64] = "InitiateCombat",
	[128] = "Influence",
}

--- @enum ServerStatusFlags3
Ext_Enums.ServerStatusFlags3 = {
	IsUnique = 1,
	NotifiedPlanManager = 2,
	DisableImmunityOverhead = 4,
	StatusFlags3_0x08 = 8,
	StatusFlags3_0x10 = 16,
	StatusFlags3_0x20 = 32,
	StatusFlags3_0x40 = 64,
	StatusFlags3_0x80 = 128,
	[1] = "IsUnique",
	[2] = "NotifiedPlanManager",
	[4] = "DisableImmunityOverhead",
	[8] = "StatusFlags3_0x08",
	[16] = "StatusFlags3_0x10",
	[32] = "StatusFlags3_0x20",
	[64] = "StatusFlags3_0x40",
	[128] = "StatusFlags3_0x80",
}

--- @enum ServerStatusFlags4
Ext_Enums.ServerStatusFlags4 = {
	BringIntoCombat = 1,
	IsHostileAct = 2,
	StatusFlags4_0x04 = 4,
	IsInvulnerable = 8,
	IsInvulnerableVisible = 16,
	CausedByEquipment = 32,
	CauseIsOrigin = 64,
	StatusFlags4_0x80 = 128,
	[1] = "BringIntoCombat",
	[2] = "IsHostileAct",
	[4] = "StatusFlags4_0x04",
	[8] = "IsInvulnerable",
	[16] = "IsInvulnerableVisible",
	[32] = "CausedByEquipment",
	[64] = "CauseIsOrigin",
	[128] = "StatusFlags4_0x80",
}

--- @enum ServerStatusFlags5
Ext_Enums.ServerStatusFlags5 = {
	HasTriedEntering = 1,
	[1] = "HasTriedEntering",
}

--- @enum ServerSteeringType
Ext_Enums.ServerSteeringType = {
	Steer = 2,
	LookAt = 3,
	SteerTo = 4,
	Repose = 5,
	SteerTunnel = 6,
	[2] = "Steer",
	[3] = "LookAt",
	[4] = "SteerTo",
	[5] = "Repose",
	[6] = "SteerTunnel",
}

--- @enum SetStatusDurationType
Ext_Enums.SetStatusDurationType = {
	SetMinimum = 0,
	ForceSet = 1,
	Add = 2,
	Multiply = 3,
	Unknown = 4,
	[0] = "SetMinimum",
	[1] = "ForceSet",
	[2] = "Add",
	[3] = "Multiply",
	[4] = "Unknown",
}

--- @enum ShapeshiftChangeType
Ext_Enums.ShapeshiftChangeType = {
	Add = 0,
	Multiply = 1,
	Override = 2,
	[0] = "Add",
	[1] = "Multiply",
	[2] = "Override",
}

--- @enum ShapeshiftSpellSource
Ext_Enums.ShapeshiftSpellSource = {
	ShapeshiftRule = 0,
	Unknown = 1,
	Template = 2,
	[0] = "ShapeshiftRule",
	[1] = "Unknown",
	[2] = "Template",
}

--- @enum SkillId
Ext_Enums.SkillId = {
	Deception = 0,
	Intimidation = 1,
	Performance = 2,
	Persuasion = 3,
	Acrobatics = 4,
	SleightOfHand = 5,
	Stealth = 6,
	Arcana = 7,
	History = 8,
	Investigation = 9,
	Nature = 10,
	Religion = 11,
	Athletics = 12,
	AnimalHandling = 13,
	Insight = 14,
	Medicine = 15,
	Perception = 16,
	Survival = 17,
	Invalid = 18,
	Sentinel = 19,
	[0] = "Deception",
	[1] = "Intimidation",
	[2] = "Performance",
	[3] = "Persuasion",
	[4] = "Acrobatics",
	[5] = "SleightOfHand",
	[6] = "Stealth",
	[7] = "Arcana",
	[8] = "History",
	[9] = "Investigation",
	[10] = "Nature",
	[11] = "Religion",
	[12] = "Athletics",
	[13] = "AnimalHandling",
	[14] = "Insight",
	[15] = "Medicine",
	[16] = "Perception",
	[17] = "Survival",
	[18] = "Invalid",
	[19] = "Sentinel",
}

--- @enum SoundEventType
Ext_Enums.SoundEventType = {
	Sound = 0,
	Music = 1,
	Ambient = 2,
	HUD = 3,
	CineHUD = 4,
	[0] = "Sound",
	[1] = "Music",
	[2] = "Ambient",
	[3] = "HUD",
	[4] = "CineHUD",
}

--- @enum SourceAdvantageType
Ext_Enums.SourceAdvantageType = {
	None = 0,
	SourceAdvantageOnAttack = 1,
	SourceAllyAdvantageOnAttack = 2,
	[0] = "None",
	[1] = "SourceAdvantageOnAttack",
	[2] = "SourceAllyAdvantageOnAttack",
}

--- @enum SpellAnimationCastEvent
Ext_Enums.SpellAnimationCastEvent = {
	Prepare = 0,
	Cast = 1,
	CastContinue = 2,
	Cancel = 3,
	Finished = 4,
	TargetingInfo = 5,
	[0] = "Prepare",
	[1] = "Cast",
	[2] = "CastContinue",
	[3] = "Cancel",
	[4] = "Finished",
	[5] = "TargetingInfo",
}

--- @enum SpellAttackType
Ext_Enums.SpellAttackType = {
	None = 0,
	DirectHit = 1,
	MeleeWeaponAttack = 2,
	RangedWeaponAttack = 3,
	MeleeOffHandWeaponAttack = 4,
	RangedOffHandWeaponAttack = 5,
	MeleeSpellAttack = 6,
	RangedSpellAttack = 7,
	MeleeUnarmedAttack = 8,
	RangedUnarmedAttack = 9,
	[0] = "None",
	[1] = "DirectHit",
	[2] = "MeleeWeaponAttack",
	[3] = "RangedWeaponAttack",
	[4] = "MeleeOffHandWeaponAttack",
	[5] = "RangedOffHandWeaponAttack",
	[6] = "MeleeSpellAttack",
	[7] = "RangedSpellAttack",
	[8] = "MeleeUnarmedAttack",
	[9] = "RangedUnarmedAttack",
}

--- @enum SpellAttackTypeOverride
Ext_Enums.SpellAttackTypeOverride = {
	Target_MainHandAttack = 0,
	Target_OffhandAttack = 1,
	Projectile_MainHandAttack = 2,
	Projectile_OffhandAttack = 3,
	Target_UnarmedAttack = 4,
	[0] = "Target_MainHandAttack",
	[1] = "Target_OffhandAttack",
	[2] = "Projectile_MainHandAttack",
	[3] = "Projectile_OffhandAttack",
	[4] = "Target_UnarmedAttack",
}

--- @enum SpellCastFailReason
Ext_Enums.SpellCastFailReason = {
	None = 0,
	Canceled = 2,
	InvalidSpell = 3,
	MissingTarget = 4,
	CantCast = 5,
	InvalidTarget = 6,
	CantUseSource = 7,
	CantSpendUseCosts = 8,
	MissingAnimations = 9,
	MissingTargetPosition = 10,
	TooManyTargets = 11,
	TooFewTargets = 12,
	MissingCaster = 13,
	DeadTarget = 14,
	EqualTargets = 15,
	CantFindCastPosition = 16,
	BlockedRequiredMove = 17,
	InvalidCastPosition = 18,
	CantMove = 19,
	MovementFailed = 20,
	ServerTimeout = 21,
	DeadCaster = 22,
	Countered = 23,
	SpellRollFailure = 24,
	UnsheathFailure = 25,
	[0] = "None",
	[2] = "Canceled",
	[3] = "InvalidSpell",
	[4] = "MissingTarget",
	[5] = "CantCast",
	[6] = "InvalidTarget",
	[7] = "CantUseSource",
	[8] = "CantSpendUseCosts",
	[9] = "MissingAnimations",
	[10] = "MissingTargetPosition",
	[11] = "TooManyTargets",
	[12] = "TooFewTargets",
	[13] = "MissingCaster",
	[14] = "DeadTarget",
	[15] = "EqualTargets",
	[16] = "CantFindCastPosition",
	[17] = "BlockedRequiredMove",
	[18] = "InvalidCastPosition",
	[19] = "CantMove",
	[20] = "MovementFailed",
	[21] = "ServerTimeout",
	[22] = "DeadCaster",
	[23] = "Countered",
	[24] = "SpellRollFailure",
	[25] = "UnsheathFailure",
}

--- @enum SpellCastOptions
Ext_Enums.SpellCastOptions = {
	IgnoreHasSpell = 1,
	IgnoreCastChecks = 2,
	IgnoreSpellRolls = 4,
	IsReaction = 8,
	NoMovement = 16,
	AvoidAoO = 32,
	DestroySource = 64,
	Immediate = 128,
	Silent = 256,
	IgnoreTargetChecks = 512,
	IsPreview = 1024,
	IsHoverPreview = 2048,
	ShowPrepareAnimation = 4096,
	Forced = 8192,
	IsRoll = 16384,
	IsInterrupt = 32768,
	FromClient = 65536,
	NoUnsheath = 131072,
	CheckProjectileTargets = 262144,
	AvoidDangerousAuras = 524288,
	Unknown100000 = 1048576,
	[1] = "IgnoreHasSpell",
	[2] = "IgnoreCastChecks",
	[4] = "IgnoreSpellRolls",
	[8] = "IsReaction",
	[16] = "NoMovement",
	[32] = "AvoidAoO",
	[64] = "DestroySource",
	[128] = "Immediate",
	[256] = "Silent",
	[512] = "IgnoreTargetChecks",
	[1024] = "IsPreview",
	[2048] = "IsHoverPreview",
	[4096] = "ShowPrepareAnimation",
	[8192] = "Forced",
	[16384] = "IsRoll",
	[32768] = "IsInterrupt",
	[65536] = "FromClient",
	[131072] = "NoUnsheath",
	[262144] = "CheckProjectileTargets",
	[524288] = "AvoidDangerousAuras",
	[1048576] = "Unknown100000",
}

--- @enum SpellCastPhase
Ext_Enums.SpellCastPhase = {
	None = 0,
	ValidationStart = 1,
	ValidationFindCastPosition = 2,
	ValidationFindCastEndPosition = 3,
	ValidationPathfind = 4,
	ValidationFinished = 5,
	MovementStart = 6,
	MovementUpdate = 7,
	MovementSteering = 8,
	MovementFinished = 9,
	PrecalculationStart = 10,
	PrecalculationFinished = 11,
	PrepareStart = 12,
	PrepareUpdate = 13,
	PrepareEnd = 14,
	PrepareFinished = 15,
	LogicExecutionStart = 16,
	LogicExecutionUpdate = 17,
	LogicExecutionInterrupted = 18,
	LogicExecutionEnd = 19,
	LogicExecutionFinished = 20,
	Finished = 21,
	[0] = "None",
	[1] = "ValidationStart",
	[2] = "ValidationFindCastPosition",
	[3] = "ValidationFindCastEndPosition",
	[4] = "ValidationPathfind",
	[5] = "ValidationFinished",
	[6] = "MovementStart",
	[7] = "MovementUpdate",
	[8] = "MovementSteering",
	[9] = "MovementFinished",
	[10] = "PrecalculationStart",
	[11] = "PrecalculationFinished",
	[12] = "PrepareStart",
	[13] = "PrepareUpdate",
	[14] = "PrepareEnd",
	[15] = "PrepareFinished",
	[16] = "LogicExecutionStart",
	[17] = "LogicExecutionUpdate",
	[18] = "LogicExecutionInterrupted",
	[19] = "LogicExecutionEnd",
	[20] = "LogicExecutionFinished",
	[21] = "Finished",
}

--- @enum SpellCooldownType
Ext_Enums.SpellCooldownType = {
	Default = 0,
	OncePerTurn = 1,
	OncePerCombat = 2,
	UntilRest = 3,
	OncePerTurnNoRealtime = 4,
	UntilShortRest = 5,
	UntilPerRestPerItem = 6,
	OncePerShortRestPerItem = 7,
	[0] = "Default",
	[1] = "OncePerTurn",
	[2] = "OncePerCombat",
	[3] = "UntilRest",
	[4] = "OncePerTurnNoRealtime",
	[5] = "UntilShortRest",
	[6] = "UntilPerRestPerItem",
	[7] = "OncePerShortRestPerItem",
}

--- @enum SpellFlags
Ext_Enums.SpellFlags = {
	HasVerbalComponent = 1,
	HasSomaticComponent = 2,
	IsJump = 4,
	IsAttack = 8,
	IsMelee = 16,
	HasHighGroundRangeExtension = 32,
	IsConcentration = 64,
	AddFallDamageOnLand = 128,
	ConcentrationIgnoresResting = 256,
	InventorySelection = 512,
	IsSpell = 1024,
	CombatLogSetSingleLineRoll = 2048,
	IsEnemySpell = 4096,
	CannotTargetCharacter = 8192,
	CannotTargetItems = 16384,
	CannotTargetTerrain = 32768,
	IgnoreVisionBlock = 65536,
	Stealth = 131072,
	AddWeaponRange = 262144,
	IgnoreSilence = 524288,
	ImmediateCast = 1048576,
	RangeIgnoreSourceBounds = 2097152,
	RangeIgnoreTargetBounds = 4194304,
	RangeIgnoreVerticalThreshold = 8388608,
	NoSurprise = 16777216,
	IsHarmful = 33554432,
	IsTrap = 67108864,
	IsDefaultWeaponAction = 134217728,
	CallAlliesSpell = 268435456,
	TargetClosestEqualGroundSurface = 536870912,
	CannotRotate = 1073741824,
	NoCameraMove = 2147483648,
	CanDualWield = 4294967296,
	IsLinkedSpellContainer = 8589934592,
	Invisible = 17179869184,
	AllowMoveAndCast = 34359738368,
	UNUSED_D = 68719476736,
	Wildshape = 137438953472,
	UNUSED_E = 274877906944,
	UnavailableInDialogs = 549755813888,
	TrajectoryRules = 1099511627776,
	PickupEntityAndMove = 2199023255552,
	Temporary = 4398046511104,
	RangeIgnoreBlindness = 8796093022208,
	AbortOnSpellRollFail = 17592186044416,
	AbortOnSecondarySpellRollFail = 35184372088832,
	CanAreaDamageEvade = 70368744177664,
	DontAbortPerforming = 140737488355328,
	NoCooldownOnMiss = 281474976710656,
	NoAOEDamageOnLand = 562949953421312,
	IsSwarmAttack = 1125899906842624,
	DisplayInItemTooltip = 2251799813685248,
	HideInItemTooltip = 4503599627370496,
	DisableBlood = 9007199254740992,
	IgnorePreviouslyPickedEntities = 18014398509481984,
	IgnoreAoO = 36028797018963968,
	[1] = "HasVerbalComponent",
	[2] = "HasSomaticComponent",
	[4] = "IsJump",
	[8] = "IsAttack",
	[16] = "IsMelee",
	[32] = "HasHighGroundRangeExtension",
	[64] = "IsConcentration",
	[128] = "AddFallDamageOnLand",
	[256] = "ConcentrationIgnoresResting",
	[512] = "InventorySelection",
	[1024] = "IsSpell",
	[2048] = "CombatLogSetSingleLineRoll",
	[4096] = "IsEnemySpell",
	[8192] = "CannotTargetCharacter",
	[16384] = "CannotTargetItems",
	[32768] = "CannotTargetTerrain",
	[65536] = "IgnoreVisionBlock",
	[131072] = "Stealth",
	[262144] = "AddWeaponRange",
	[524288] = "IgnoreSilence",
	[1048576] = "ImmediateCast",
	[2097152] = "RangeIgnoreSourceBounds",
	[4194304] = "RangeIgnoreTargetBounds",
	[8388608] = "RangeIgnoreVerticalThreshold",
	[16777216] = "NoSurprise",
	[33554432] = "IsHarmful",
	[67108864] = "IsTrap",
	[134217728] = "IsDefaultWeaponAction",
	[268435456] = "CallAlliesSpell",
	[536870912] = "TargetClosestEqualGroundSurface",
	[1073741824] = "CannotRotate",
	[2147483648] = "NoCameraMove",
	[4294967296] = "CanDualWield",
	[8589934592] = "IsLinkedSpellContainer",
	[17179869184] = "Invisible",
	[34359738368] = "AllowMoveAndCast",
	[68719476736] = "UNUSED_D",
	[137438953472] = "Wildshape",
	[274877906944] = "UNUSED_E",
	[549755813888] = "UnavailableInDialogs",
	[1099511627776] = "TrajectoryRules",
	[2199023255552] = "PickupEntityAndMove",
	[4398046511104] = "Temporary",
	[8796093022208] = "RangeIgnoreBlindness",
	[17592186044416] = "AbortOnSpellRollFail",
	[35184372088832] = "AbortOnSecondarySpellRollFail",
	[70368744177664] = "CanAreaDamageEvade",
	[140737488355328] = "DontAbortPerforming",
	[281474976710656] = "NoCooldownOnMiss",
	[562949953421312] = "NoAOEDamageOnLand",
	[1125899906842624] = "IsSwarmAttack",
	[2251799813685248] = "DisplayInItemTooltip",
	[4503599627370496] = "HideInItemTooltip",
	[9007199254740992] = "DisableBlood",
	[18014398509481984] = "IgnorePreviouslyPickedEntities",
	[36028797018963968] = "IgnoreAoO",
}

--- @enum SpellLearningStrategy
Ext_Enums.SpellLearningStrategy = {
	Singular = 0,
	AddChildren = 1,
	MostPowerful = 2,
	[0] = "Singular",
	[1] = "AddChildren",
	[2] = "MostPowerful",
}

--- @enum SpellMetaConditionType
Ext_Enums.SpellMetaConditionType = {
	None = 0,
	Roll = 1,
	OriginRoll = 2,
	ThrowableRoll = 3,
	AoE = 4,
	Target = 5,
	ThrowableTarget = 6,
	Forking = 7,
	Cycle = 8,
	Requirements = 9,
	OriginTarget = 10,
	Highlight = 11,
	[0] = "None",
	[1] = "Roll",
	[2] = "OriginRoll",
	[3] = "ThrowableRoll",
	[4] = "AoE",
	[5] = "Target",
	[6] = "ThrowableTarget",
	[7] = "Forking",
	[8] = "Cycle",
	[9] = "Requirements",
	[10] = "OriginTarget",
	[11] = "Highlight",
}

--- @enum SpellModificationValueType
Ext_Enums.SpellModificationValueType = {
	AdditiveBase = 0,
	AdditiveFinal = 1,
	Multiplicative = 2,
	Override = 3,
	[0] = "AdditiveBase",
	[1] = "AdditiveFinal",
	[2] = "Multiplicative",
	[3] = "Override",
}

--- @enum SpellPrepareType
Ext_Enums.SpellPrepareType = {
	AlwaysPrepared = 0,
	RequiresPreparation = 1,
	Unknown = 2,
	[0] = "AlwaysPrepared",
	[1] = "RequiresPreparation",
	[2] = "Unknown",
}

--- @enum SpellSchoolId
Ext_Enums.SpellSchoolId = {
	None = 0,
	Abjuration = 1,
	Conjuration = 2,
	Divination = 3,
	Enchantment = 4,
	Evocation = 5,
	Illusion = 6,
	Necromancy = 7,
	Transmutation = 8,
	[0] = "None",
	[1] = "Abjuration",
	[2] = "Conjuration",
	[3] = "Divination",
	[4] = "Enchantment",
	[5] = "Evocation",
	[6] = "Illusion",
	[7] = "Necromancy",
	[8] = "Transmutation",
}

--- @enum SpellSourceType
Ext_Enums.SpellSourceType = {
	ProgressionClass = 0,
	ProgressionSubClass = 1,
	ProgressionRace = 2,
	Boost = 3,
	Shapeshift = 4,
	SpellSet2 = 5,
	SpellSet = 6,
	WeaponAttack = 7,
	UnarmedAttack = 8,
	Osiris = 9,
	Anubis = 10,
	Behavior = 11,
	Debug = 12,
	EquippedItem = 13,
	GameActionCreateSurface = 14,
	Functor = 15,
	CreateExplosion = 16,
	AiTest = 17,
	Learned = 18,
	Boost2 = 19,
	Progression = 20,
	RandomCast = 21,
	TadpoleTree = 22,
	Sentinel = 23,
	[0] = "ProgressionClass",
	[1] = "ProgressionSubClass",
	[2] = "ProgressionRace",
	[3] = "Boost",
	[4] = "Shapeshift",
	[5] = "SpellSet2",
	[6] = "SpellSet",
	[7] = "WeaponAttack",
	[8] = "UnarmedAttack",
	[9] = "Osiris",
	[10] = "Anubis",
	[11] = "Behavior",
	[12] = "Debug",
	[13] = "EquippedItem",
	[14] = "GameActionCreateSurface",
	[15] = "Functor",
	[16] = "CreateExplosion",
	[17] = "AiTest",
	[18] = "Learned",
	[19] = "Boost2",
	[20] = "Progression",
	[21] = "RandomCast",
	[22] = "TadpoleTree",
	[23] = "Sentinel",
}

--- @enum SpellType
Ext_Enums.SpellType = {
	None = 0,
	Zone = 1,
	MultiStrike = 2,
	Projectile = 3,
	ProjectileStrike = 4,
	Rush = 5,
	Shout = 6,
	Storm = 7,
	Target = 8,
	Teleportation = 9,
	Wall = 10,
	Throw = 11,
	[0] = "None",
	[1] = "Zone",
	[2] = "MultiStrike",
	[3] = "Projectile",
	[4] = "ProjectileStrike",
	[5] = "Rush",
	[6] = "Shout",
	[7] = "Storm",
	[8] = "Target",
	[9] = "Teleportation",
	[10] = "Wall",
	[11] = "Throw",
}

--- @enum StatAttributeFlags
Ext_Enums.StatAttributeFlags = {
	SlippingImmunity = 1,
	Torch = 2,
	Arrow = 4,
	Unbreakable = 8,
	Unrepairable = 16,
	Unstorable = 32,
	Grounded = 64,
	Floating = 128,
	EMPTY = 256,
	IgnoreClouds = 512,
	LootableWhenEquipped = 1024,
	PickpocketableWhenEquipped = 2048,
	LoseDurabilityOnCharacterHit = 4096,
	ThrownImmunity = 8192,
	InvisibilityImmunity = 16384,
	InvulnerableAndInteractive = 32768,
	Backstab = 65536,
	BackstabImmunity = 131072,
	EnableObscurityEvents = 262144,
	ObscurityWithoutSneaking = 524288,
	FloatingWhileMoving = 1048576,
	[1] = "SlippingImmunity",
	[2] = "Torch",
	[4] = "Arrow",
	[8] = "Unbreakable",
	[16] = "Unrepairable",
	[32] = "Unstorable",
	[64] = "Grounded",
	[128] = "Floating",
	[256] = "EMPTY",
	[512] = "IgnoreClouds",
	[1024] = "LootableWhenEquipped",
	[2048] = "PickpocketableWhenEquipped",
	[4096] = "LoseDurabilityOnCharacterHit",
	[8192] = "ThrownImmunity",
	[16384] = "InvisibilityImmunity",
	[32768] = "InvulnerableAndInteractive",
	[65536] = "Backstab",
	[131072] = "BackstabImmunity",
	[262144] = "EnableObscurityEvents",
	[524288] = "ObscurityWithoutSneaking",
	[1048576] = "FloatingWhileMoving",
}

--- @enum StatCharacterFlags
Ext_Enums.StatCharacterFlags = {
	IsPlayer = 1,
	InParty = 2,
	IsSneaking = 4,
	Invisible = 8,
	Blind = 16,
	DrinkedPotion = 32,
	EquipmentValidated = 64,
	[1] = "IsPlayer",
	[2] = "InParty",
	[4] = "IsSneaking",
	[8] = "Invisible",
	[16] = "Blind",
	[32] = "DrinkedPotion",
	[64] = "EquipmentValidated",
}

--- @enum StatsContextType
Ext_Enums.StatsContextType = {
	Unspecified = 0,
	Target = 1,
	Owner = 2,
	Cause = 3,
	[0] = "Unspecified",
	[1] = "Target",
	[2] = "Owner",
	[3] = "Cause",
}

--- @enum StatsDeathType
Ext_Enums.StatsDeathType = {
	None = 0,
	Acid = 1,
	Chasm = 2,
	DoT = 3,
	Electrocution = 4,
	Explode = 5,
	Falling = 6,
	Incinerate = 7,
	KnockedDown = 8,
	Lifetime = 9,
	Necrotic = 10,
	Physical = 11,
	Psychic = 12,
	Radiant = 13,
	CinematicDeath = 14,
	Cold = 15,
	Disintegrate = 16,
	[0] = "None",
	[1] = "Acid",
	[2] = "Chasm",
	[3] = "DoT",
	[4] = "Electrocution",
	[5] = "Explode",
	[6] = "Falling",
	[7] = "Incinerate",
	[8] = "KnockedDown",
	[9] = "Lifetime",
	[10] = "Necrotic",
	[11] = "Physical",
	[12] = "Psychic",
	[13] = "Radiant",
	[14] = "CinematicDeath",
	[15] = "Cold",
	[16] = "Disintegrate",
}

--- @enum StatsDirtyFlags
Ext_Enums.StatsDirtyFlags = {
	AC = 1,
	Abilities = 2,
	Skills = 4,
	Resistances = 8,
	ProficiencyBonus = 16,
	SavingThrow = 32,
	MaxHP = 64,
	StatusImmunities = 128,
	FootstepsType = 512,
	Weight = 1024,
	TempHP = 2048,
	Initiative = 4096,
	WeaponActionDC = 8192,
	MaxPreparedSpells = 16384,
	Invulnerability = 32768,
	ItemUsability = 262144,
	Encumbrance = 524288,
	UnarmedAndNaturalArmorType = 1048576,
	EquipmentSlotAvailability = 2097152,
	WeaponProperties = 4194304,
	WeaponAttackRollAbility = 8388608,
	PhysicalForceRangeBonus = 16777216,
	SpellDifficultyCheck = 33554432,
	DamageOnWeapon = 67108864,
	UseBoostRequests = 134217728,
	CurrentHP = 268435456,
	AttackRollBonusOnWeapon = 536870912,
	[1] = "AC",
	[2] = "Abilities",
	[4] = "Skills",
	[8] = "Resistances",
	[16] = "ProficiencyBonus",
	[32] = "SavingThrow",
	[64] = "MaxHP",
	[128] = "StatusImmunities",
	[512] = "FootstepsType",
	[1024] = "Weight",
	[2048] = "TempHP",
	[4096] = "Initiative",
	[8192] = "WeaponActionDC",
	[16384] = "MaxPreparedSpells",
	[32768] = "Invulnerability",
	[262144] = "ItemUsability",
	[524288] = "Encumbrance",
	[1048576] = "UnarmedAndNaturalArmorType",
	[2097152] = "EquipmentSlotAvailability",
	[4194304] = "WeaponProperties",
	[8388608] = "WeaponAttackRollAbility",
	[16777216] = "PhysicalForceRangeBonus",
	[33554432] = "SpellDifficultyCheck",
	[67108864] = "DamageOnWeapon",
	[134217728] = "UseBoostRequests",
	[268435456] = "CurrentHP",
	[536870912] = "AttackRollBonusOnWeapon",
}

--- @enum StatsExpressionParamType
Ext_Enums.StatsExpressionParamType = {
	StatsExpressionType = 0,
	Type1 = 1,
	StatsExpressionVariableDataType = 2,
	StatsExpressionVariableDataModifier = 3,
	RollDefinition = 4,
	ResourceRollDefinition = 5,
	StatsContextType = 6,
	Int = 7,
	Bool = 8,
	[0] = "StatsExpressionType",
	[1] = "Type1",
	[2] = "StatsExpressionVariableDataType",
	[3] = "StatsExpressionVariableDataModifier",
	[4] = "RollDefinition",
	[5] = "ResourceRollDefinition",
	[6] = "StatsContextType",
	[7] = "Int",
	[8] = "Bool",
}

--- @enum StatsExpressionParamType2
Ext_Enums.StatsExpressionParamType2 = {
	StatsExpressionVariableData = 0,
	Ability = 1,
	Skill = 2,
	StatusGroup = 3,
	STDString = 4,
	[0] = "StatsExpressionVariableData",
	[1] = "Ability",
	[2] = "Skill",
	[3] = "StatusGroup",
	[4] = "STDString",
}

--- @enum StatsExpressionType
Ext_Enums.StatsExpressionType = {
	Roll = 0,
	Add = 1,
	Subtract = 2,
	Divide = 3,
	Multiply = 4,
	Max = 5,
	ForEach = 6,
	Placeholder = 7,
	ResourceRoll = 8,
	Variable = 9,
	Negate = 10,
	[0] = "Roll",
	[1] = "Add",
	[2] = "Subtract",
	[3] = "Divide",
	[4] = "Multiply",
	[5] = "Max",
	[6] = "ForEach",
	[7] = "Placeholder",
	[8] = "ResourceRoll",
	[9] = "Variable",
	[10] = "Negate",
}

--- @enum StatsExpressionVariableData
Ext_Enums.StatsExpressionVariableData = {
	SpellCastingAbility = 0,
	ProficiencyBonus = 1,
	Level = 2,
	SpellDC = 3,
	WeaponActionDC = 4,
	UnarmedMeleeAbility = 5,
	CurrentHP = 6,
	MaxHP = 7,
	SpellPowerLevel = 8,
	TadpolePowersCount = 9,
	DamageDone = 10,
	Sentinel = 11,
	[0] = "SpellCastingAbility",
	[1] = "ProficiencyBonus",
	[2] = "Level",
	[3] = "SpellDC",
	[4] = "WeaponActionDC",
	[5] = "UnarmedMeleeAbility",
	[6] = "CurrentHP",
	[7] = "MaxHP",
	[8] = "SpellPowerLevel",
	[9] = "TadpolePowersCount",
	[10] = "DamageDone",
	[11] = "Sentinel",
}

--- @enum StatsExpressionVariableDataModifier
Ext_Enums.StatsExpressionVariableDataModifier = {
	Modifier = 1,
	Flat = 2,
	SavingThrow = 4,
	DialogueCheck = 8,
	Advantage = 16,
	Disadvantage = 32,
	[1] = "Modifier",
	[2] = "Flat",
	[4] = "SavingThrow",
	[8] = "DialogueCheck",
	[16] = "Advantage",
	[32] = "Disadvantage",
}

--- @enum StatsExpressionVariableDataType
Ext_Enums.StatsExpressionVariableDataType = {
	AbilityOverride = 0,
	SavingThrow = 1,
	StatusDuration = 2,
	StatusStacks = 3,
	CharacterData = 4,
	LevelMapValue = 5,
	ClassLevel = 6,
	[0] = "AbilityOverride",
	[1] = "SavingThrow",
	[2] = "StatusDuration",
	[3] = "StatusStacks",
	[4] = "CharacterData",
	[5] = "LevelMapValue",
	[6] = "ClassLevel",
}

--- @enum StatsFunctorFlags
Ext_Enums.StatsFunctorFlags = {
	Self = 1,
	Owner = 2,
	Swap = 4,
	[1] = "Self",
	[2] = "Owner",
	[4] = "Swap",
}

--- @enum StatsFunctorId
Ext_Enums.StatsFunctorId = {
	CustomDescription = 0,
	ApplyStatus = 1,
	SurfaceChange = 2,
	Resurrect = 3,
	Sabotage = 4,
	Summon = 5,
	Force = 6,
	Douse = 7,
	SwapPlaces = 8,
	Pickup = 9,
	CreateSurface = 10,
	CreateConeSurface = 11,
	RemoveStatus = 12,
	DealDamage = 13,
	ExecuteWeaponFunctors = 14,
	RegainHitPoints = 15,
	TeleportSource = 16,
	SetStatusDuration = 17,
	UseSpell = 18,
	UseActionResource = 19,
	UseAttack = 20,
	CreateExplosion = 21,
	BreakConcentration = 22,
	ApplyEquipmentStatus = 23,
	RestoreResource = 24,
	Spawn = 25,
	Stabilize = 26,
	Unlock = 27,
	ResetCombatTurn = 28,
	RemoveAuraByChildStatus = 29,
	SummonInInventory = 30,
	SpawnInInventory = 31,
	RemoveUniqueStatus = 32,
	DisarmWeapon = 33,
	DisarmAndStealWeapon = 34,
	SwitchDeathType = 35,
	TriggerRandomCast = 36,
	GainTemporaryHitPoints = 37,
	FireProjectile = 38,
	ShortRest = 39,
	CreateZone = 40,
	DoTeleport = 41,
	RegainTemporaryHitPoints = 42,
	RemoveStatusByLevel = 43,
	SurfaceClearLayer = 44,
	Unsummon = 45,
	CreateWall = 46,
	Counterspell = 47,
	AdjustRoll = 48,
	SpawnExtraProjectiles = 49,
	Kill = 50,
	TutorialEvent = 51,
	Drop = 52,
	ResetCooldowns = 53,
	SetRoll = 54,
	SetDamageResistance = 55,
	SetReroll = 56,
	SetAdvantage = 57,
	SetDisadvantage = 58,
	MaximizeRoll = 59,
	CameraWait = 60,
	ModifySpellCameraFocus = 61,
	Extender = 127,
	[0] = "CustomDescription",
	[1] = "ApplyStatus",
	[2] = "SurfaceChange",
	[3] = "Resurrect",
	[4] = "Sabotage",
	[5] = "Summon",
	[6] = "Force",
	[7] = "Douse",
	[8] = "SwapPlaces",
	[9] = "Pickup",
	[10] = "CreateSurface",
	[11] = "CreateConeSurface",
	[12] = "RemoveStatus",
	[13] = "DealDamage",
	[14] = "ExecuteWeaponFunctors",
	[15] = "RegainHitPoints",
	[16] = "TeleportSource",
	[17] = "SetStatusDuration",
	[18] = "UseSpell",
	[19] = "UseActionResource",
	[20] = "UseAttack",
	[21] = "CreateExplosion",
	[22] = "BreakConcentration",
	[23] = "ApplyEquipmentStatus",
	[24] = "RestoreResource",
	[25] = "Spawn",
	[26] = "Stabilize",
	[27] = "Unlock",
	[28] = "ResetCombatTurn",
	[29] = "RemoveAuraByChildStatus",
	[30] = "SummonInInventory",
	[31] = "SpawnInInventory",
	[32] = "RemoveUniqueStatus",
	[33] = "DisarmWeapon",
	[34] = "DisarmAndStealWeapon",
	[35] = "SwitchDeathType",
	[36] = "TriggerRandomCast",
	[37] = "GainTemporaryHitPoints",
	[38] = "FireProjectile",
	[39] = "ShortRest",
	[40] = "CreateZone",
	[41] = "DoTeleport",
	[42] = "RegainTemporaryHitPoints",
	[43] = "RemoveStatusByLevel",
	[44] = "SurfaceClearLayer",
	[45] = "Unsummon",
	[46] = "CreateWall",
	[47] = "Counterspell",
	[48] = "AdjustRoll",
	[49] = "SpawnExtraProjectiles",
	[50] = "Kill",
	[51] = "TutorialEvent",
	[52] = "Drop",
	[53] = "ResetCooldowns",
	[54] = "SetRoll",
	[55] = "SetDamageResistance",
	[56] = "SetReroll",
	[57] = "SetAdvantage",
	[58] = "SetDisadvantage",
	[59] = "MaximizeRoll",
	[60] = "CameraWait",
	[61] = "ModifySpellCameraFocus",
	[127] = "Extender",
}

--- @enum StatsItemSlot
Ext_Enums.StatsItemSlot = {
	Helmet = 0,
	Breast = 1,
	Cloak = 2,
	MeleeMainHand = 3,
	MeleeOffHand = 4,
	RangedMainHand = 5,
	RangedOffHand = 6,
	Ring = 7,
	Underwear = 8,
	Boots = 9,
	Gloves = 10,
	Amulet = 11,
	Ring2 = 12,
	Wings = 13,
	Horns = 14,
	Overhead = 15,
	MusicalInstrument = 16,
	VanityBody = 17,
	VanityBoots = 18,
	MainHand = 19,
	OffHand = 20,
	Max = 21,
	Sentinel = 22,
	[0] = "Helmet",
	[1] = "Breast",
	[2] = "Cloak",
	[3] = "MeleeMainHand",
	[4] = "MeleeOffHand",
	[5] = "RangedMainHand",
	[6] = "RangedOffHand",
	[7] = "Ring",
	[8] = "Underwear",
	[9] = "Boots",
	[10] = "Gloves",
	[11] = "Amulet",
	[12] = "Ring2",
	[13] = "Wings",
	[14] = "Horns",
	[15] = "Overhead",
	[16] = "MusicalInstrument",
	[17] = "VanityBody",
	[18] = "VanityBoots",
	[19] = "MainHand",
	[20] = "OffHand",
	[21] = "Max",
	[22] = "Sentinel",
}

--- @enum StatsObserverType
Ext_Enums.StatsObserverType = {
	None = 0,
	Observer = 1,
	Target = 2,
	Source = 3,
	[0] = "None",
	[1] = "Observer",
	[2] = "Target",
	[3] = "Source",
}

--- @enum StatsPropertyContext
Ext_Enums.StatsPropertyContext = {
	TARGET = 1,
	AOE = 2,
	CAST = 4,
	EQUIP = 8,
	GROUND = 16,
	LEAVE_ATTACK_RANGE = 32,
	ATTACKED_IN_MELEE_RANGE = 64,
	ATTACKING_IN_MELEE_RANGE = 128,
	PROFICIENCY_CHANGED = 256,
	STATUS_APPLY = 512,
	STATUS_APPLIED = 1024,
	STATUS_REMOVE = 2048,
	MOVED_DISTANCE = 4096,
	AI_ONLY = 8192,
	AI_IGNORE = 16384,
	ATTACK = 32768,
	ATTACKED = 65536,
	DAMAGE = 131072,
	HEAL = 262144,
	STATUS_REMOVED = 524288,
	OBSCURITY_CHANGED = 1048576,
	SHORT_REST = 2097152,
	DAMAGED = 4194304,
	HEALED = 8388608,
	ABILITY_CHECK = 16777216,
	CAST_RESOLVED = 33554432,
	LONG_REST = 67108864,
	CREATE = 134217728,
	PUSH = 268435456,
	PUSHED = 536870912,
	INVENTORY_CHANGED = 1073741824,
	ENTER_ATTACK_RANGE = 2147483648,
	PROJECTILE = 4294967296,
	COMBAT_ENDED = 8589934592,
	TURN = 17179869184,
	ACTION_RESOURCES_CHANGED = 34359738368,
	SURFACE_ENTER = 68719476736,
	LOCKPICKING_SUCCEEDED = 137438953472,
	INTERRUPT_USED = 274877906944,
	DAMAGE_PREVENTED = 549755813888,
	DAMAGED_PREVENTED = 1099511627776,
	ROUND = 2199023255552,
	COMBAT_STARTED = 4398046511104,
	CONSUMED = 8796093022208,
	[1] = "TARGET",
	[2] = "AOE",
	[4] = "CAST",
	[8] = "EQUIP",
	[16] = "GROUND",
	[32] = "LEAVE_ATTACK_RANGE",
	[64] = "ATTACKED_IN_MELEE_RANGE",
	[128] = "ATTACKING_IN_MELEE_RANGE",
	[256] = "PROFICIENCY_CHANGED",
	[512] = "STATUS_APPLY",
	[1024] = "STATUS_APPLIED",
	[2048] = "STATUS_REMOVE",
	[4096] = "MOVED_DISTANCE",
	[8192] = "AI_ONLY",
	[16384] = "AI_IGNORE",
	[32768] = "ATTACK",
	[65536] = "ATTACKED",
	[131072] = "DAMAGE",
	[262144] = "HEAL",
	[524288] = "STATUS_REMOVED",
	[1048576] = "OBSCURITY_CHANGED",
	[2097152] = "SHORT_REST",
	[4194304] = "DAMAGED",
	[8388608] = "HEALED",
	[16777216] = "ABILITY_CHECK",
	[33554432] = "CAST_RESOLVED",
	[67108864] = "LONG_REST",
	[134217728] = "CREATE",
	[268435456] = "PUSH",
	[536870912] = "PUSHED",
	[1073741824] = "INVENTORY_CHANGED",
	[2147483648] = "ENTER_ATTACK_RANGE",
	[4294967296] = "PROJECTILE",
	[8589934592] = "COMBAT_ENDED",
	[17179869184] = "TURN",
	[34359738368] = "ACTION_RESOURCES_CHANGED",
	[68719476736] = "SURFACE_ENTER",
	[137438953472] = "LOCKPICKING_SUCCEEDED",
	[274877906944] = "INTERRUPT_USED",
	[549755813888] = "DAMAGE_PREVENTED",
	[1099511627776] = "DAMAGED_PREVENTED",
	[2199023255552] = "ROUND",
	[4398046511104] = "COMBAT_STARTED",
	[8796093022208] = "CONSUMED",
}

--- @enum StatsRestoreResourceAmountType
Ext_Enums.StatsRestoreResourceAmountType = {
	Lua = 0,
	Percentage = 1,
	None = 2,
	[0] = "Lua",
	[1] = "Percentage",
	[2] = "None",
}

--- @enum StatsRollAdjustmentType
Ext_Enums.StatsRollAdjustmentType = {
	All = 0,
	Distribute = 1,
	[0] = "All",
	[1] = "Distribute",
}

--- @enum StatsRollType
Ext_Enums.StatsRollType = {
	None = 0,
	Attack = 1,
	MeleeWeaponAttack = 2,
	RangedWeaponAttack = 3,
	MeleeSpellAttack = 4,
	RangedSpellAttack = 5,
	MeleeUnarmedAttack = 6,
	RangedUnarmedAttack = 7,
	SkillCheck = 8,
	SavingThrow = 9,
	RawAbility = 10,
	Damage = 11,
	MeleeOffHandWeaponAttack = 12,
	RangedOffHandWeaponAttack = 13,
	DeathSavingThrow = 14,
	MeleeWeaponDamage = 15,
	RangedWeaponDamage = 16,
	MeleeSpellDamage = 17,
	RangedSpellDamage = 18,
	MeleeUnarmedDamage = 19,
	RangedUnarmedDamage = 20,
	Sentinel = 21,
	[0] = "None",
	[1] = "Attack",
	[2] = "MeleeWeaponAttack",
	[3] = "RangedWeaponAttack",
	[4] = "MeleeSpellAttack",
	[5] = "RangedSpellAttack",
	[6] = "MeleeUnarmedAttack",
	[7] = "RangedUnarmedAttack",
	[8] = "SkillCheck",
	[9] = "SavingThrow",
	[10] = "RawAbility",
	[11] = "Damage",
	[12] = "MeleeOffHandWeaponAttack",
	[13] = "RangedOffHandWeaponAttack",
	[14] = "DeathSavingThrow",
	[15] = "MeleeWeaponDamage",
	[16] = "RangedWeaponDamage",
	[17] = "MeleeSpellDamage",
	[18] = "RangedSpellDamage",
	[19] = "MeleeUnarmedDamage",
	[20] = "RangedUnarmedDamage",
	[21] = "Sentinel",
}

--- @enum StatsSize
Ext_Enums.StatsSize = {
	None = 0,
	Tiny = 1,
	Small = 2,
	Medium = 3,
	Large = 4,
	Huge = 5,
	Gargantuan = 6,
	[0] = "None",
	[1] = "Tiny",
	[2] = "Small",
	[3] = "Medium",
	[4] = "Large",
	[5] = "Huge",
	[6] = "Gargantuan",
}

--- @enum StatsStatusGroup
Ext_Enums.StatsStatusGroup = {
	SG_Condition = 1,
	SG_Blinded = 2,
	SG_Charmed = 4,
	SG_Cursed = 8,
	SG_Disease = 16,
	SG_Exhausted = 32,
	SG_Frightened = 64,
	SG_Incapacitated = 128,
	SG_Invisible = 256,
	SG_Poisoned = 512,
	SG_Prone = 1024,
	SG_Restrained = 2048,
	SG_Stunned = 4096,
	SG_Unconscious = 8192,
	SG_Surface = 16384,
	SG_Polymorph = 32768,
	SG_Paralyzed = 65536,
	SG_Light = 131072,
	SG_Disguise = 262144,
	SG_Possessed = 524288,
	SG_Petrified = 1048576,
	SG_Polymorph_BeastShape = 2097152,
	SG_Polymorph_BeastShape_NPC = 4194304,
	SG_Poisoned_Story_Removable = 8388608,
	SG_Poisoned_Story_NonRemovable = 16777216,
	SG_Charmed_Subtle = 33554432,
	SG_Helpable_Condition = 67108864,
	SG_Rage = 134217728,
	SG_Taunted = 268435456,
	SG_Approaching = 536870912,
	SG_Dominated = 1073741824,
	SG_Fleeing = 2147483648,
	SG_Confused = 4294967296,
	SG_Mad = 8589934592,
	SG_DetectThoughts = 17179869184,
	SG_DifficultTerrain = 34359738368,
	SG_ScriptedPeaceBehaviour = 68719476736,
	SG_DropForNonMutingDialog = 137438953472,
	SG_HexbladeCurse = 274877906944,
	SG_WeaponCoating = 549755813888,
	SG_Doppelganger = 1099511627776,
	SG_CanBePickedUp = 2199023255552,
	SG_Drunk = 4398046511104,
	SG_Sleeping = 8796093022208,
	SG_RemoveOnRespec = 17592186044416,
	SG_Ignore_AOO = 35184372088832,
	[1] = "SG_Condition",
	[2] = "SG_Blinded",
	[4] = "SG_Charmed",
	[8] = "SG_Cursed",
	[16] = "SG_Disease",
	[32] = "SG_Exhausted",
	[64] = "SG_Frightened",
	[128] = "SG_Incapacitated",
	[256] = "SG_Invisible",
	[512] = "SG_Poisoned",
	[1024] = "SG_Prone",
	[2048] = "SG_Restrained",
	[4096] = "SG_Stunned",
	[8192] = "SG_Unconscious",
	[16384] = "SG_Surface",
	[32768] = "SG_Polymorph",
	[65536] = "SG_Paralyzed",
	[131072] = "SG_Light",
	[262144] = "SG_Disguise",
	[524288] = "SG_Possessed",
	[1048576] = "SG_Petrified",
	[2097152] = "SG_Polymorph_BeastShape",
	[4194304] = "SG_Polymorph_BeastShape_NPC",
	[8388608] = "SG_Poisoned_Story_Removable",
	[16777216] = "SG_Poisoned_Story_NonRemovable",
	[33554432] = "SG_Charmed_Subtle",
	[67108864] = "SG_Helpable_Condition",
	[134217728] = "SG_Rage",
	[268435456] = "SG_Taunted",
	[536870912] = "SG_Approaching",
	[1073741824] = "SG_Dominated",
	[2147483648] = "SG_Fleeing",
	[4294967296] = "SG_Confused",
	[8589934592] = "SG_Mad",
	[17179869184] = "SG_DetectThoughts",
	[34359738368] = "SG_DifficultTerrain",
	[68719476736] = "SG_ScriptedPeaceBehaviour",
	[137438953472] = "SG_DropForNonMutingDialog",
	[274877906944] = "SG_HexbladeCurse",
	[549755813888] = "SG_WeaponCoating",
	[1099511627776] = "SG_Doppelganger",
	[2199023255552] = "SG_CanBePickedUp",
	[4398046511104] = "SG_Drunk",
	[8796093022208] = "SG_Sleeping",
	[17592186044416] = "SG_RemoveOnRespec",
	[35184372088832] = "SG_Ignore_AOO",
}

--- @enum StatsSummonLifetimeType
Ext_Enums.StatsSummonLifetimeType = {
	Permanent = 0,
	UntilLongRest = 1,
	[0] = "Permanent",
	[1] = "UntilLongRest",
}

--- @enum StatsTargetTypeFlags
Ext_Enums.StatsTargetTypeFlags = {
	Living = 1,
	Guaranteed = 2,
	Construct = 4,
	Undead = 8,
	[1] = "Living",
	[2] = "Guaranteed",
	[4] = "Construct",
	[8] = "Undead",
}

--- @enum StatsUnsheathState
Ext_Enums.StatsUnsheathState = {
	Instrument = 0,
	Melee = 1,
	Ranged = 2,
	Sheathed = 3,
	[0] = "Instrument",
	[1] = "Melee",
	[2] = "Ranged",
	[3] = "Sheathed",
}

--- @enum StatsZoneShape
Ext_Enums.StatsZoneShape = {
	Cone = 0,
	Square = 1,
	[0] = "Cone",
	[1] = "Square",
}

--- @enum StatusHealType
Ext_Enums.StatusHealType = {
	None = 0,
	Vitality = 1,
	PhysicalArmor = 2,
	MagicArmor = 3,
	AllArmor = 4,
	All = 5,
	Source = 6,
	[0] = "None",
	[1] = "Vitality",
	[2] = "PhysicalArmor",
	[3] = "MagicArmor",
	[4] = "AllArmor",
	[5] = "All",
	[6] = "Source",
}

--- @enum StatusMaterialApplyFlags
Ext_Enums.StatusMaterialApplyFlags = {
	ApplyOnBody = 1,
	ApplyOnArmor = 2,
	ApplyOnWeapon = 4,
	ApplyOnWings = 8,
	ApplyOnHorns = 16,
	ApplyOnOverhead = 32,
	[1] = "ApplyOnBody",
	[2] = "ApplyOnArmor",
	[4] = "ApplyOnWeapon",
	[8] = "ApplyOnWings",
	[16] = "ApplyOnHorns",
	[32] = "ApplyOnOverhead",
}

--- @enum StatusType
Ext_Enums.StatusType = {
	DYING = 1,
	HEAL = 2,
	KNOCKED_DOWN = 3,
	TELEPORT_FALLING = 4,
	BOOST = 5,
	REACTION = 6,
	STORY_FROZEN = 7,
	SNEAKING = 8,
	UNLOCK = 9,
	FEAR = 10,
	SMELLY = 11,
	INVISIBLE = 12,
	ROTATE = 13,
	MATERIAL = 14,
	CLIMBING = 15,
	INCAPACITATED = 16,
	INSURFACE = 17,
	POLYMORPHED = 18,
	EFFECT = 19,
	DEACTIVATED = 20,
	DOWNED = 21,
	[1] = "DYING",
	[2] = "HEAL",
	[3] = "KNOCKED_DOWN",
	[4] = "TELEPORT_FALLING",
	[5] = "BOOST",
	[6] = "REACTION",
	[7] = "STORY_FROZEN",
	[8] = "SNEAKING",
	[9] = "UNLOCK",
	[10] = "FEAR",
	[11] = "SMELLY",
	[12] = "INVISIBLE",
	[13] = "ROTATE",
	[14] = "MATERIAL",
	[15] = "CLIMBING",
	[16] = "INCAPACITATED",
	[17] = "INSURFACE",
	[18] = "POLYMORPHED",
	[19] = "EFFECT",
	[20] = "DEACTIVATED",
	[21] = "DOWNED",
}

--- @enum SurfaceActionType
Ext_Enums.SurfaceActionType = {
	CreateSurfaceAction = 1,
	CreatePuddleAction = 2,
	ExtinguishFireAction = 3,
	ZoneAction = 4,
	TransformSurfaceAction = 5,
	ChangeSurfaceOnPathAction = 6,
	RectangleSurfaceAction = 7,
	PolygonSurfaceAction = 8,
	[1] = "CreateSurfaceAction",
	[2] = "CreatePuddleAction",
	[3] = "ExtinguishFireAction",
	[4] = "ZoneAction",
	[5] = "TransformSurfaceAction",
	[6] = "ChangeSurfaceOnPathAction",
	[7] = "RectangleSurfaceAction",
	[8] = "PolygonSurfaceAction",
}

--- @enum SurfaceChange
Ext_Enums.SurfaceChange = {
	None = 0,
	Ignite = 1,
	Douse = 2,
	Electrify = 3,
	Deelectrify = 4,
	Freeze = 5,
	Melt = 6,
	Vaporize = 7,
	Condense = 8,
	DestroyWater = 9,
	Clear = 10,
	Daylight = 11,
	TurnHellfire = 12,
	UnturnHellfire = 13,
	[0] = "None",
	[1] = "Ignite",
	[2] = "Douse",
	[3] = "Electrify",
	[4] = "Deelectrify",
	[5] = "Freeze",
	[6] = "Melt",
	[7] = "Vaporize",
	[8] = "Condense",
	[9] = "DestroyWater",
	[10] = "Clear",
	[11] = "Daylight",
	[12] = "TurnHellfire",
	[13] = "UnturnHellfire",
}

--- @enum SurfaceLayer
Ext_Enums.SurfaceLayer = {
	Ground = 0,
	Cloud = 1,
	None = 2,
	[0] = "Ground",
	[1] = "Cloud",
	[2] = "None",
}

--- @enum SurfaceLayer8
Ext_Enums.SurfaceLayer8 = {
	Ground = 0,
	Cloud = 1,
	None = 2,
	[0] = "Ground",
	[1] = "Cloud",
	[2] = "None",
}

--- @enum SurfaceType
Ext_Enums.SurfaceType = {
	None = 0,
	Water = 1,
	WaterElectrified = 2,
	WaterFrozen = 3,
	Blood = 4,
	BloodElectrified = 5,
	BloodFrozen = 6,
	Poison = 7,
	Oil = 8,
	Lava = 9,
	Grease = 10,
	WyvernPoison = 11,
	Web = 12,
	Deepwater = 13,
	Vines = 14,
	Fire = 15,
	Acid = 16,
	TrialFire = 17,
	BlackPowder = 18,
	ShadowCursedVines = 19,
	AlienOil = 20,
	Mud = 21,
	Alcohol = 22,
	InvisibleWeb = 23,
	BloodSilver = 24,
	Chasm = 25,
	Hellfire = 26,
	CausticBrine = 27,
	BloodExploding = 28,
	Ash = 29,
	SpikeGrowth = 30,
	HolyFire = 31,
	BlackTentacles = 32,
	Overgrowth = 33,
	PurpleWormPoison = 34,
	SerpentVenom = 35,
	InvisibleGithAcid = 36,
	BladeBarrier = 37,
	Sewer = 38,
	WaterCloud = 39,
	WaterCloudElectrified = 40,
	PoisonCloud = 41,
	ExplosionCloud = 42,
	ShockwaveCloud = 43,
	CloudkillCloud = 44,
	MaliceCloud = 45,
	BloodCloud = 46,
	StinkingCloud = 47,
	DarknessCloud = 48,
	FogCloud = 49,
	GithPheromoneGasCloud = 50,
	SporeWhiteCloud = 51,
	SporeGreenCloud = 52,
	SporeBlackCloud = 53,
	DrowPoisonCloud = 54,
	IceCloud = 55,
	PotionHealingCloud = 56,
	PotionHealingGreaterCloud = 57,
	PotionHealingSuperiorCloud = 58,
	PotionHealingSupremeCloud = 59,
	PotionInvisibilityCloud = 60,
	PotionSpeedCloud = 61,
	PotionVitalityCloud = 62,
	PotionAntitoxinCloud = 63,
	PotionResistanceAcidCloud = 64,
	PotionResistanceColdCloud = 65,
	PotionResistanceFireCloud = 66,
	PotionResistanceForceCloud = 67,
	PotionResistanceLightningCloud = 68,
	PotionResistancePoisonCloud = 69,
	SporePinkCloud = 70,
	BlackPowderDetonationCloud = 71,
	VoidCloud = 72,
	CrawlerMucusCloud = 73,
	Cloudkill6Cloud = 74,
	Sentinel = 75,
	[0] = "None",
	[1] = "Water",
	[2] = "WaterElectrified",
	[3] = "WaterFrozen",
	[4] = "Blood",
	[5] = "BloodElectrified",
	[6] = "BloodFrozen",
	[7] = "Poison",
	[8] = "Oil",
	[9] = "Lava",
	[10] = "Grease",
	[11] = "WyvernPoison",
	[12] = "Web",
	[13] = "Deepwater",
	[14] = "Vines",
	[15] = "Fire",
	[16] = "Acid",
	[17] = "TrialFire",
	[18] = "BlackPowder",
	[19] = "ShadowCursedVines",
	[20] = "AlienOil",
	[21] = "Mud",
	[22] = "Alcohol",
	[23] = "InvisibleWeb",
	[24] = "BloodSilver",
	[25] = "Chasm",
	[26] = "Hellfire",
	[27] = "CausticBrine",
	[28] = "BloodExploding",
	[29] = "Ash",
	[30] = "SpikeGrowth",
	[31] = "HolyFire",
	[32] = "BlackTentacles",
	[33] = "Overgrowth",
	[34] = "PurpleWormPoison",
	[35] = "SerpentVenom",
	[36] = "InvisibleGithAcid",
	[37] = "BladeBarrier",
	[38] = "Sewer",
	[39] = "WaterCloud",
	[40] = "WaterCloudElectrified",
	[41] = "PoisonCloud",
	[42] = "ExplosionCloud",
	[43] = "ShockwaveCloud",
	[44] = "CloudkillCloud",
	[45] = "MaliceCloud",
	[46] = "BloodCloud",
	[47] = "StinkingCloud",
	[48] = "DarknessCloud",
	[49] = "FogCloud",
	[50] = "GithPheromoneGasCloud",
	[51] = "SporeWhiteCloud",
	[52] = "SporeGreenCloud",
	[53] = "SporeBlackCloud",
	[54] = "DrowPoisonCloud",
	[55] = "IceCloud",
	[56] = "PotionHealingCloud",
	[57] = "PotionHealingGreaterCloud",
	[58] = "PotionHealingSuperiorCloud",
	[59] = "PotionHealingSupremeCloud",
	[60] = "PotionInvisibilityCloud",
	[61] = "PotionSpeedCloud",
	[62] = "PotionVitalityCloud",
	[63] = "PotionAntitoxinCloud",
	[64] = "PotionResistanceAcidCloud",
	[65] = "PotionResistanceColdCloud",
	[66] = "PotionResistanceFireCloud",
	[67] = "PotionResistanceForceCloud",
	[68] = "PotionResistanceLightningCloud",
	[69] = "PotionResistancePoisonCloud",
	[70] = "SporePinkCloud",
	[71] = "BlackPowderDetonationCloud",
	[72] = "VoidCloud",
	[73] = "CrawlerMucusCloud",
	[74] = "Cloudkill6Cloud",
	[75] = "Sentinel",
}

--- @enum TextKeyType
Ext_Enums.TextKeyType = {
	Sound = 1,
	Effect = 2,
	FootStep = 3,
	Attach = 4,
	WeaponEffect = 6,
	Genome = 7,
	Attack = 8,
	Ragdoll = 9,
	VisualCullFlag = 10,
	FloatData = 11,
	Foley = 12,
	Vocal = 13,
	FootMove = 14,
	React = 15,
	[1] = "Sound",
	[2] = "Effect",
	[3] = "FootStep",
	[4] = "Attach",
	[6] = "WeaponEffect",
	[7] = "Genome",
	[8] = "Attack",
	[9] = "Ragdoll",
	[10] = "VisualCullFlag",
	[11] = "FloatData",
	[12] = "Foley",
	[13] = "Vocal",
	[14] = "FootMove",
	[15] = "React",
}

--- @enum TextureType
Ext_Enums.TextureType = {
	T1D = 0,
	T2D = 1,
	T3D = 2,
	T1DArray = 3,
	T2DArray = 4,
	T3DArray = 5,
	T2DCube = 6,
	T2DCubeArray = 7,
	T2DRenderTarget = 8,
	[0] = "T1D",
	[1] = "T2D",
	[2] = "T3D",
	[3] = "T1DArray",
	[4] = "T2DArray",
	[5] = "T3DArray",
	[6] = "T2DCube",
	[7] = "T2DCubeArray",
	[8] = "T2DRenderTarget",
}

--- @enum TravelErrorFlags
Ext_Enums.TravelErrorFlags = {
	InDanger = 2,
	Condition = 4,
	DangerZone = 8,
	Dialog = 16,
	CannotFastTravel = 64,
	FTB = 256,
	[2] = "InDanger",
	[4] = "Condition",
	[8] = "DangerZone",
	[16] = "Dialog",
	[64] = "CannotFastTravel",
	[256] = "FTB",
}

--- @enum TravelFlags
Ext_Enums.TravelFlags = {
	BlockFastTravel = 1,
	BlockCampTravel = 2,
	[1] = "BlockFastTravel",
	[2] = "BlockCampTravel",
}

--- @enum VisualAttachmentFlags
Ext_Enums.VisualAttachmentFlags = {
	ExcludeFromBounds = 1,
	KeepRot = 4,
	KeepScale = 8,
	UseLocalTransform = 16,
	InheritAnimations = 32,
	VisualSet = 64,
	SupportsVertexColorMask = 128,
	Hair = 256,
	EffectComponent_M = 512,
	TextKeyEffect_M = 1024,
	DoNotUpdate = 2048,
	ParticleSystem = 4096,
	HasSkeleton = 8192,
	DestroyWithParent = 32768,
	[1] = "ExcludeFromBounds",
	[4] = "KeepRot",
	[8] = "KeepScale",
	[16] = "UseLocalTransform",
	[32] = "InheritAnimations",
	[64] = "VisualSet",
	[128] = "SupportsVertexColorMask",
	[256] = "Hair",
	[512] = "EffectComponent_M",
	[1024] = "TextKeyEffect_M",
	[2048] = "DoNotUpdate",
	[4096] = "ParticleSystem",
	[8192] = "HasSkeleton",
	[32768] = "DestroyWithParent",
}

--- @enum VisualCullFlags
Ext_Enums.VisualCullFlags = {
	CullNonProxy = 2,
	CullShadow = 4,
	CullShadow2 = 8,
	CullShadow3 = 16,
	CullDecal = 128,
	CullRenderView1 = 512,
	CullRenderView2 = 1024,
	CullScenery = 2048,
	CullShadowProxy = 16384,
	CullShadow4 = 32768,
	[2] = "CullNonProxy",
	[4] = "CullShadow",
	[8] = "CullShadow2",
	[16] = "CullShadow3",
	[128] = "CullDecal",
	[512] = "CullRenderView1",
	[1024] = "CullRenderView2",
	[2048] = "CullScenery",
	[16384] = "CullShadowProxy",
	[32768] = "CullShadow4",
}

--- @enum VisualFlags
Ext_Enums.VisualFlags = {
	CastShadow = 1,
	ReceiveDecal = 2,
	Reflecting = 4,
	DisableLOD = 8,
	DisableCulling = 16,
	IsWall = 32,
	IsShadowProxy = 64,
	AllowReceiveDecalWhenAnimated = 128,
	IsEffect = 256,
	Unknown200 = 512,
	IsScenery = 1024,
	PlayingAttachedEffects = 2048,
	ShowMesh = 4096,
	HasSkeleton = 8192,
	ReceiveColorFromParent = 16384,
	AllowTPose = 32768,
	HasBlueprint = 65536,
	SeeThrough = 131072,
	PreparedDestroyed = 1073741824,
	PreparedDestroyed2 = 2147483648,
	[1] = "CastShadow",
	[2] = "ReceiveDecal",
	[4] = "Reflecting",
	[8] = "DisableLOD",
	[16] = "DisableCulling",
	[32] = "IsWall",
	[64] = "IsShadowProxy",
	[128] = "AllowReceiveDecalWhenAnimated",
	[256] = "IsEffect",
	[512] = "Unknown200",
	[1024] = "IsScenery",
	[2048] = "PlayingAttachedEffects",
	[4096] = "ShowMesh",
	[8192] = "HasSkeleton",
	[16384] = "ReceiveColorFromParent",
	[32768] = "AllowTPose",
	[65536] = "HasBlueprint",
	[131072] = "SeeThrough",
	[1073741824] = "PreparedDestroyed",
	[2147483648] = "PreparedDestroyed2",
}

--- @enum VisualObjectType
Ext_Enums.VisualObjectType = {
	Type01 = 1,
	Type02 = 2,
	Type04 = 4,
	Beard = 8,
	Ears = 16,
	Head = 32,
	Horns = 64,
	[1] = "Type01",
	[2] = "Type02",
	[4] = "Type04",
	[8] = "Beard",
	[16] = "Ears",
	[32] = "Head",
	[64] = "Horns",
}

--- @enum WeaponFlags
Ext_Enums.WeaponFlags = {
	Light = 1,
	Ammunition = 2,
	Finesse = 4,
	Heavy = 8,
	Loading = 16,
	Range = 32,
	Reach = 64,
	Lance = 128,
	Net = 256,
	Thrown = 512,
	Twohanded = 1024,
	Versatile = 2048,
	Melee = 4096,
	Dippable = 8192,
	Torch = 16384,
	NoDualWield = 32768,
	Magical = 65536,
	NeedDualWieldingBoost = 131072,
	NotSheathable = 262144,
	Unstowable = 524288,
	AddToHotbar = 1048576,
	[1] = "Light",
	[2] = "Ammunition",
	[4] = "Finesse",
	[8] = "Heavy",
	[16] = "Loading",
	[32] = "Range",
	[64] = "Reach",
	[128] = "Lance",
	[256] = "Net",
	[512] = "Thrown",
	[1024] = "Twohanded",
	[2048] = "Versatile",
	[4096] = "Melee",
	[8192] = "Dippable",
	[16384] = "Torch",
	[32768] = "NoDualWield",
	[65536] = "Magical",
	[131072] = "NeedDualWieldingBoost",
	[262144] = "NotSheathable",
	[524288] = "Unstowable",
	[1048576] = "AddToHotbar",
}

--- @enum WeaponSetType
Ext_Enums.WeaponSetType = {
	Melee = 0,
	Ranged = 1,
	Unknown = 2,
	[0] = "Melee",
	[1] = "Ranged",
	[2] = "Unknown",
}

--- @enum WeaponType
Ext_Enums.WeaponType = {
	None = 0,
	Sword = 1,
	Club = 2,
	Axe = 3,
	Staff = 4,
	Bow = 5,
	Crossbow = 6,
	Spear = 7,
	Knife = 8,
	Wand = 9,
	Arrow = 10,
	Rifle = 11,
	Sentinel = 12,
	[0] = "None",
	[1] = "Sword",
	[2] = "Club",
	[3] = "Axe",
	[4] = "Staff",
	[5] = "Bow",
	[6] = "Crossbow",
	[7] = "Spear",
	[8] = "Knife",
	[9] = "Wand",
	[10] = "Arrow",
	[11] = "Rifle",
	[12] = "Sentinel",
}

--#endregion


--- @class Ext_System
--- @field ServerStats EsvStatsSystem
--- @field ServerCombat EsvCombatSystem
--- @field ServerRoll EsvRollRollSystem
--- @field ServerTemplateChange EsvTemplatesChangeSystem
--- @field ServerInterruptManagement EsvInterruptManagementSystem
--- @field ServerPassive EsvPassivePassiveSystem
--- @field ServerLongRest EsvRestLongRestSystem
--- @field ServerActionResource EsvActionResourceSystem
--- @field ServerConcentration EsvConcentrationConcentrationSystem
--- @field ClientEquipmentVisuals EclEquipmentVisualsSystem
--- @field ClientCharacterIconRender EclCharacterIconRenderSystem
--- @field ServerInterruptDecision EsvInterruptDecisionSystem
--- @field ServerCastRequest EsvSpellCastCastRequestSystem
--- @field ServerTurnOrder EsvCombatTurnOrderSystem
--- @field ServerShapeshift EsvShapeshiftSystem
--- @field ServerExperience EsvExperienceSystem
--- @field ServerSummonSpawn EsvSummonSpawnSystem
--- @field ServerHit EsvHitHitSystem
--- @field ServerShortRest EsvRestShortRestSystem
--- @field ServerSpellLearning EsvSpellLearningSystem
--- @field ServerFTBZone EsvFtbZoneSystem
--- @field ClientVisual EclVisualSystem
--- @field ServerProgression EsvProgressionManagementSystem
--- @field ClientVisualsVisibilityState EclEquipmentVisualsVisibilityStateSystem
--- @field ServerSummonDespawn EsvSummonDespawnSystem
--- @field ServerSpellCooldown EsvSpellSpellCooldownSystem
--- @field ServerCapabilities EsvCapabilitiesSystem
--- @field ClientCharacterManager EclCharacterManager
--- @field ServerInterruptRequests EsvInterruptRequestsSystem
--- @field SoundRouting SoundRoutingSystem
--- @field ServerVisual EsvVisualSystem
--- @field ServerBoost EsvBoostBoostSystem
--- @field ServerSpell EsvSpellSpellSystem

--- @class Ext
--- @field Audio Ext_ClientAudio
--- @field ClientAudio Ext_ClientAudio
--- @field IMGUI Ext_ClientIMGUI
--- @field ClientIMGUI Ext_ClientIMGUI
--- @field Input Ext_ClientInput
--- @field ClientInput Ext_ClientInput
--- @field Net Ext_ClientNet|Ext_ServerNet
--- @field ClientNet Ext_ClientNet
--- @field Template Ext_ClientTemplate|Ext_ServerTemplate
--- @field ClientTemplate Ext_ClientTemplate
--- @field UI Ext_ClientUI
--- @field ClientUI Ext_ClientUI
--- @field Debug Ext_Debug
--- @field Entity Ext_Entity
--- @field IO Ext_IO
--- @field Json Ext_Json
--- @field Level Ext_Level
--- @field Loca Ext_Loca
--- @field Log Ext_Log
--- @field Math Ext_Math
--- @field Mod Ext_Mod
--- @field Resource Ext_Resource
--- @field ServerNet Ext_ServerNet
--- @field ServerTemplate Ext_ServerTemplate
--- @field StaticData Ext_StaticData
--- @field Stats Ext_Stats
--- @field Timer Ext_Timer
--- @field Types Ext_Types
--- @field Utils Ext_Utils
--- @field Vars Ext_Vars
--- @field Enums Ext_Enums
--- @field System Ext_System
Ext = {Events = {}}


--- @class SubscribableEvent<T>:{ (Subscribe:fun(self:SubscribableEvent, callback:fun(e:T|LuaEventBase), opts:{Priority:integer, Once:boolean}?):integer), (Unsubscribe:fun(self:SubscribableEvent, index:integer))}

--- Developer functions for the SubscribableEvent type. 
--- Throw can be used to manually throw the event, but special care may be needed to ensure the table used for the event data is valid.  
--- @class SubscribableEventDev<T>:{ (Throw:fun(self:SubscribableEvent, e:T|LuaEventBase))}

--#region Extender Events
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<LuaDoConsoleCommandEvent>  
Ext.Events.DoConsoleCommand = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<EclLuaControllerAxisEvent>  
Ext.Events.EclLuaControllerAxis = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<EclLuaControllerButtonEvent>  
Ext.Events.EclLuaControllerButton = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<EclLuaGameStateChangedEvent>  
Ext.Events.EclLuaGameStateChanged = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<EclLuaKeyInputEvent>  
Ext.Events.EclLuaKeyInput = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<EclLuaMouseButtonEvent>  
Ext.Events.EclLuaMouseButton = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<EclLuaMouseWheelEvent>  
Ext.Events.EclLuaMouseWheel = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<EclLuaViewportResizedEvent>  
Ext.Events.EclLuaViewportResized = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<LuaEmptyEvent>  
Ext.Events.Empty = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<EsvLuaAfterExecuteFunctorEvent>  
Ext.Events.EsvLuaAfterExecuteFunctor = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<EsvLuaBeforeDealDamageEvent>  
Ext.Events.EsvLuaBeforeDealDamage = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<EsvLuaDealDamageEvent>  
Ext.Events.EsvLuaDealDamage = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<EsvLuaExecuteFunctorEvent>  
Ext.Events.EsvLuaExecuteFunctor = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<EsvLuaGameStateChangedEvent>  
Ext.Events.EsvLuaGameStateChanged = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<LuaFindPathEvent>  
Ext.Events.FindPath = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<LuaEmptyEvent>  
Ext.Events.GameStateChanged = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<LuaEmptyEvent>  
Ext.Events.Log = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<LuaEmptyEvent>  
Ext.Events.ModuleLoadStarted = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<LuaEmptyEvent>  
Ext.Events.ModuleResume = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<LuaNetMessageEvent>  
Ext.Events.NetMessage = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<LuaEmptyEvent>  
Ext.Events.NetModMessage = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<LuaEmptyEvent>  
Ext.Events.ResetCompleted = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<LuaEmptyEvent>  
Ext.Events.SessionLoaded = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<LuaEmptyEvent>  
Ext.Events.SessionLoading = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<LuaEmptyEvent>  
Ext.Events.Shutdown = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<LuaEmptyEvent>  
Ext.Events.StatsLoaded = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<LuaEmptyEvent>  
Ext.Events.StatsStructureLoaded = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<LuaTickEvent>  
Ext.Events.Tick = {}
--#endregion


--#region Extender Functions / Globals

--- @alias NetListenerCallback fun(channel:string, payload:string, user:UserId?)

--- Registers a listener that is called when a network message is received on the specified channel
--- @param channel string Network channel name
--- @param handler NetListenerCallback Lua handler
function Ext.RegisterNetListener(channel, handler) end

--- Loads the specified Lua file
--- @param fileName string? Path of Lua file, relative to Mods/<Mod>/Story/RawFiles/Lua
--- @see Ext_Utils#Include
--- @return any
function Ext.Require(fileName) end

--- Returns whether the code is executing in a client context
--- @return boolean
function Ext.IsClient() end

--- Returns whether the code is executing in a server context
--- @return boolean
function Ext.IsServer() end

--- Console window shortcut for Ext.Dump
_D = Ext.Dump

--- Console window shortcut for Ext.DumpShallow
_DS = Ext.Dump

--- Console window shortcut for Ext.Utils.Print
_P = Ext.Utils.Print

--- Console window helper to get current player character 
--- This is the host on the server, or the hotbar character on the client  
--- @return EsvCharacter|EclCharacter
_C = function() end

--- Console window helper to get character being examined on the client-side  
--- @return EclCharacter
_E = function() end

--- Console window helper to get the host's equipped weapon on the server-side  
--- @return EsvItem
_W = function() end

--- Helper for dumping variables to the console  
--- This is essentially `Ext.Utils.Print(Ext.DumpExport(val))`  
--- @param val any
function Ext.Dump(val) end

--- Helper for dumping variables to a string
--- @param val any
--- @return string
function Ext.DumpExport(val) end

--- Register a callback that runs on the next tick, and is then unregistered afterwards  
--- @param callback fun(e:LuaTickEventParams)
function Ext.OnNextTick(callback) end

--- Registers a listener that is called when a console command is entered in the dev console
--- @param cmd string Console command
--- @param handler fun(cmd:string, ...:string)
function Ext.RegisterConsoleCommand(cmd, handler) end

--#endregion

--#region ExtraData

Ext.ExtraData = {
	["NPC max combat turn time"] = 20.0,
	["ThrowDistanceMin"] = 3.0,
	["ThrowDistanceMax"] = 18.0,
	["ThrowStrengthCapMultiplier"] = 0.20000000298023,
	["ThrowWeightMultiplier"] = 2.0,
	["Telekinesis Range"] = 4.0,
	["End Of Combat SightRange Multiplier"] = 3.0,
	["Sneak Damage Multiplier"] = 1.0,
	["Infectious Disease Depth"] = 5.0,
	["Infectious Disease Radius"] = 5.0,
	["Haste Speed Modifier"] = 1.5,
	["Slow Speed Modifier"] = 0.80000001192093,
	["Ally Joins Ally SightRange Multiplier"] = 2.5,
	["Surface Distance Evaluation"] = 2.0,
	["Once Per Combat Spell Realtime Cooldown"] = 1.0,
	["HintDuration"] = 5.0,
	["Projectile Terrain Offset"] = 3.0,
	["Surface Clear Owner Time"] = 1.0,
	["Decaying Touch Damage Modifier"] = 1.0,
	["FirstItemTypeShift"] = 9.0,
	["SecondItemTypeShift"] = 16.0,
	["PickpocketGoldValuePerPoint"] = 50.0,
	["PickpocketWeightPerPoint"] = 2000.0,
	["PickpocketExperienceLevelsPerPoint"] = 4.0,
	["PersuasionAttitudeBonusPerPoint"] = 5.0,
	["AbilityBaseValue"] = 10.0,
	["AbilityCharCreationBonus"] = 1.0,
	["AbilityLevelGrowth"] = 0.0,
	["AbilityBoostGrowth"] = 0.0,
	["AbilityGrowthDamp"] = 0.0,
	["AbilitySoftCap"] = 40.0,
	["WitsGrowthDamp"] = 0.0,
	["VitalityStartingAmount"] = 21.0,
	["VitalityExponentialGrowth"] = 1.25,
	["VitalityLinearGrowth"] = 9.0909996032715,
	["VitalityToDamageRatio"] = 5.0,
	["VitalityToDamageRatioGrowth"] = 0.20000000298023,
	["ExpectedDamageBoostFromAbilityPerLevel"] = 0.064999997615814,
	["ExpectedDamageBoostFromSpellSchoolPerLevel"] = 0.014999999664724,
	["ExpectedDamageBoostFromWeaponSkillPerLevel"] = 0.025000000372529,
	["ExpectedConGrowthForArmorCalculation"] = 1.0,
	["FirstVitalityLeapLevel"] = 9.0,
	["FirstVitalityLeapGrowth"] = 1.25,
	["SecondVitalityLeapLevel"] = 13.0,
	["SecondVitalityLeapGrowth"] = 1.25,
	["ThirdVitalityLeapLevel"] = 16.0,
	["ThirdVitalityLeapGrowth"] = 1.25,
	["FourthVitalityLeapLevel"] = 18.0,
	["FourthVitalityLeapGrowth"] = 1.3500000238419,
	["DamageBoostFromAbility"] = 0.050000000745058,
	["MonsterDamageBoostPerLevel"] = 0.019999999552965,
	["PhysicalArmourBoostFromAbility"] = 0.0,
	["MagicArmourBoostFromAbility"] = 0.0,
	["VitalityBoostFromAbility"] = 0.070000000298023,
	["DodgingBoostFromAbility"] = 0.0,
	["HealToDamageRatio"] = 1.2999999523163,
	["ArmorToVitalityRatio"] = 0.55000001192093,
	["ArmorRegenTimer"] = 0.0099999997764826,
	["ArmorRegenConstGrowth"] = 1.0,
	["ArmorRegenPercentageGrowth"] = 10.0,
	["ArmorAfterHitCooldown"] = -7.0,
	["MagicArmorRegenTimer"] = 0.0099999997764826,
	["MagicArmorRegenConstGrowth"] = 1.0,
	["MagicArmorRegenPercentageGrowth"] = 10.0,
	["MagicArmorAfterHitCooldown"] = -7.0,
	["ArmorHeadPercentage"] = 0.15000000596046,
	["ArmorUpperBodyPercentage"] = 0.30000001192093,
	["ArmorLowerBodyPercentage"] = 0.20000000298023,
	["ArmorShieldPercentage"] = 0.5,
	["ArmorHandsPercentage"] = 0.15000000596046,
	["ArmorFeetPercentage"] = 0.15000000596046,
	["ArmorAmuletPercentage"] = 0.11999999731779,
	["ArmorRingPercentage"] = 0.079999998211861,
	["CharacterBaseMemoryCapacity"] = 100.0,
	["CharacterBaseMemoryCapacityGrowth"] = 0.5,
	["CharacterAbilityPointsPerMemoryCapacity"] = 1.0,
	["CombatSkillCap"] = 10.0,
	["CombatSkillLevelGrowth"] = 100.0,
	["CombatSkillNpcGrowth"] = 0.0,
	["CombatSkillDamageBonus"] = 5.0,
	["CombatSkillCritBonus"] = 1.0,
	["CombatSkillCritMultiplierBonus"] = 5.0,
	["CombatSkillAccuracyBonus"] = 5.0,
	["CombatSkillDodgingBonus"] = 1.0,
	["CombatSkillReflectionBonus"] = 5.0,
	["NumStartingCivilSkillPoints"] = 2.0,
	["CivilSkillCap"] = 5.0,
	["CivilSkillLevelGrowth"] = 100.0,
	["CivilPointOffset"] = 0.0,
	["SavethrowLowChance"] = 15.0,
	["SavethrowHighChance"] = 50.0,
	["SavethrowBelowLowPenalty"] = 5.0,
	["SavethrowPenaltyCap"] = -30.0,
	["CriticalBonusFromWits"] = 1.0,
	["InitiativeBonusFromWits"] = 1.0,
	["WeaponAccuracyPenaltyPerLevel"] = -20.0,
	["WeaponAccuracyPenaltyCap"] = -80.0,
	["ShieldAPCost"] = 0.0,
	["WeaponWeightLight"] = 500.0,
	["WeaponWeightMedium"] = 1050.0,
	["WeaponWeightHeavy"] = 2600.0,
	["WeaponWeightGiant"] = 8200.0,
	["HighGroundThreshold"] = 2.2999999523163,
	["LowGroundAttackRollPenalty"] = -2.0,
	["HighGroundAttackRollBonus"] = 2.0,
	["LowGroundMeleeRange"] = 0.0,
	["HighGroundMeleeRange"] = 0.5,
	["HighGroundRangeMultiplier"] = 1.0,
	["SneakDefaultAPCost"] = 1.0,
	["BlindRangePenalty"] = 3.0,
	["RangeBoostedGlobalCap"] = 30.0,
	["SurfaceDurationFromHitFloorReaction"] = 18.0,
	["SurfaceDurationFireIgniteOverride"] = 12.0,
	["SurfaceDurationFromCharacterBleeding"] = -1.0,
	["SurfaceDurationAfterDecay"] = -1.0,
	["SmokeDurationAfterDecay"] = 6.0,
	["DualWieldingAPPenalty"] = 2.0,
	["DualWieldingDamagePenalty"] = 0.5,
	["ChanceToSetStatusOnContact"] = 100.0,
	["SkillPhysArmorBonusBase"] = 5.0,
	["SkillPhysArmorBonusPerPoint"] = 2.0,
	["SkillPhysArmorBonusMax"] = 5.0,
	["SkillMagicArmorBonusBase"] = 5.0,
	["SkillMagicArmorBonusPerPoint"] = 2.0,
	["SkillMagicArmorBonusMax"] = 5.0,
	["SkillVitalityBonusBase"] = 3.0,
	["SkillVitalityBonusPerPoint"] = 1.0,
	["SkillVitalityBonusMax"] = 3.0,
	["SpellSchoolDamageToPhysicalArmorPerPoint"] = 0.0,
	["SpellSchoolDamageToMagicArmorPerPoint"] = 0.0,
	["SpellSchoolArmorRestoredPerPoint"] = 5.0,
	["SpellSchoolVitalityRestoredPerPoint"] = 5.0,
	["SpellSchoolHighGroundBonusPerPoint"] = 5.0,
	["SpellSchoolFireDamageBoostPerPoint"] = 5.0,
	["SpellSchoolPoisonAndEarthDamageBoostPerPoint"] = 5.0,
	["SpellSchoolAirDamageBoostPerPoint"] = 5.0,
	["SpellSchoolWaterDamageBoostPerPoint"] = 5.0,
	["SpellSchoolPhysicalDamageBoostPerPoint"] = 5.0,
	["SpellSchoolSulfuricDamageBoostPerPoint"] = 5.0,
	["SpellSchoolLifeStealPerPoint"] = 10.0,
	["SpiritVisionFallbackRadius"] = 10.0,
	["AiCoverProjectileTurnMemory"] = 2.0,
	["CarryWeightBase"] = 40.0,
	["CarryWeightPerStr"] = 10000.0,
	["DeflectProjectileRange"] = 1.0,
	["CleaveRangeOverride"] = 125.0,
	["FleeDistance"] = 13.0,
	["GlobalGoldValueMultiplier"] = 1.0,
	["PriceBarterCoefficient"] = 0.10000000149012,
	["PriceAttitudeCoefficient"] = 0.0049999998882413,
	["PriceRoundToFiveAfterAmount"] = 100.0,
	["PriceRoundToTenAfterAmount"] = 1000.0,
	["GMCharacterAbilityCap"] = 100.0,
	["GMCharacterArmorCap"] = 999999.0,
	["GMCharacterResistanceMin"] = -1000.0,
	["GMCharacterResistanceMax"] = 1000.0,
	["GMCharacterAPCap"] = 100.0,
	["GMCharacterSPCap"] = 3.0,
	["GMItemLevelCap"] = 50.0,
	["GMItemAbilityCap"] = 100.0,
	["GMItemArmorMin"] = -999999.0,
	["GMItemArmorMax"] = 999999.0,
	["GMItemResistanceMin"] = -1000.0,
	["GMItemResistanceMax"] = 1000.0,
	["TraderDroppedItemsPercentage"] = 51.0,
	["TraderDroppedItemsCap"] = 3.0,
	["TraderDonationsRequiredAttitude"] = -45.0,
	["TeleportUnchainDistance"] = 50.0,
	["FlankingElevationThreshold"] = 1.2000000476837,
	["DefaultDC"] = 15.0,
	["FallDamageBaseDamage"] = 0.0099999997764826,
	["FallDamageMinimumDistance"] = 4.0,
	["FallDamageMaximumDistance"] = 21.0,
	["FallDamageDamageType"] = 3.0,
	["FallDamagePronePercent"] = 0.25,
	["FallDamageMultiplierHugeGargantuan"] = 2.0,
	["FallImpactPushDistance"] = 2.0,
	["FallImpactConstant"] = 4.9999998736894e-06,
	["FallImpactMinWeight"] = 0.5,
	["LethalHP"] = 1.0,
	["SpinningSpeed"] = 5.0,
	["ItemWeightLight"] = 100.0,
	["ItemWeightMedium"] = 1000.0,
	["ItemWeightHeavy"] = 50000.0,
	["WisdomTierHigh"] = 10.0,
	["CoinPileMediumThreshold"] = 100.0,
	["CoinPileBigThreshold"] = 500.0,
	["DarknessHeightOffset"] = 0.10000000149012,
	["DarknessGracePeriod"] = 0.20000000298023,
	["DarknessGraceFrames"] = 6.0,
	["SightConePreviewMaxDistanceFromPlayerSq"] = 900.0,
	["SpellMoveFloodMaxSourceDistance"] = 9.0,
	["UnarmedRange"] = 1.5,
	["ActiveRollRerollInspirationCost"] = 1.0,
	["PickpocketWeightSquaredThreshold"] = 250000.0,
	["PickpocketPriceSquaredThreshold"] = 100.0,
	["JoinAllyInCombatRadius"] = 7.0,
	["AbilityMaxValue"] = 30.0,
	["PassiveRollDelay"] = 0.40000000596046,
	["PickpocketingMaxPrice"] = 1000.0,
	["PickpocketingWeightModifierConstant"] = 0.20000000298023,
	["PickpocketingPriceModifierConstant"] = 0.5,
	["PickpocketingPriceDC"] = 20.0,
	["PickpocketingMinimumDC"] = 5.0,
	["FollowDistance"] = 70.0,
	["SneakFollowDistance"] = 5.0,
	["SeekHiddenRange"] = 3.0,
	["SeekHiddenTimeout"] = 0.5,
	["DamageTerrainOffset"] = 3.25,
	["SurfaceOnMoveDistanceMultiplier"] = 1.0,
	["SurfaceOnEnterDistanceMultiplier"] = 0.5,
	["GameplayLightsFadeTime"] = 0.5,
	["MaxPickingDistance"] = 30.0,
	["LearnSpellCostPerLevel"] = 50.0,
	["SavantLearnSpellCostPerLevel"] = 25.0,
	["MaxShortRestPoints"] = 2.0,
	["DualWieldingPlayersDefaultOn"] = 1.0,
	["DualWieldingNPCsDefaultOn"] = 1.0,
	["MaximumXPCap"] = 200000.0,
	["CombatCameraEndDelay"] = 1.0,
	["DamagingSurfacesThreshold"] = 35.0,
	["FollowThroughDamagingSurfaceDistance"] = 20.0,
	["CameraTakeControlTimer"] = 1.0,
	["CameraEnableTakeControlOnEndTurn"] = 1.0,
	["CombatCameraRangedAttackWait"] = 1.0,
	["CCTurnPauseTime"] = 1.0,
	["BaseSpellDifficultyCheck"] = 8.0,
	["GlobalBaseACModifier"] = 0.0,
	["RollStreamDialogDebtRange"] = 1.0,
	["RollStreamPlayerSpellDebtRange"] = 1.0,
	["RollStreamNPCSpellDebtRange"] = 1.0,
	["RollStreamPlayerRandomCastDebtRange"] = 1.0,
	["RollStreamNPCRandomCastDebtRange"] = 1.0,
	["RollStreamSuccessDebtConsumeMultiplier"] = 2.0,
	["RollStreamFailDebtConsumeMultiplier"] = 2.0,
	["ContainerCloseDistance"] = 0.0099999997764826,
	["CharacterInteractionHeightMin"] = 1.0,
	["MaxPickUpMultiplier"] = 5.0,
	["PointAndClickPostDelay"] = 0.10000000149012,
	["PortraitClickSpamThreshold"] = 10.0,
	["CombatCameraStatusEndTurnWait"] = 1.0,
	["Disarm_MaxDistance"] = 4.0,
	["Disarm_ThrowAngle"] = 100.0,
	["Disarm_RandomConeAngle"] = 40.0,
	["EncumberedMultiplier"] = 7.0,
	["HeavilyEncumberedMultiplier"] = 9.0,
	["CarryLimitMultiplier"] = 10.0,
	["ShoveCurveConstant"] = 65.0,
	["ShoveDistanceMax"] = 6.0,
	["ShoveDistanceMin"] = 1.0,
	["ShoveMultiplier"] = 12.0,
	["DragCheckMultiplier"] = 12.0,
	["DragCurveConstant"] = 65.0,
	["DragDistanceMax"] = 6.0,
	["ForceFunctorFallbackStrength"] = 10.0,
	["ExhaustedRest_HealthPercent"] = 50.0,
	["ExhaustedRest_ResourcePercent"] = 50.0,
	["ActiveRollPartyNearbyDistance"] = 9.0,
	["ActiveRollDisableIgnoreRange"] = 0.0,
	["CannotActTimeout"] = 2.0,
	["AutoSuccessCastingInActiveRoll"] = 1.0,
	["SplatterDistanceThreshold"] = 120.0,
	["SplatterDirtPerDistance"] = 0.10000000149012,
	["SplatterBloodPerDistance"] = 0.20000000298023,
	["SplatterBloodPerAttack"] = 0.10000000149012,
	["SplatterMaxBloodLimit"] = 1.0,
	["SplatterMaxDirtLimit"] = 0.89999997615814,
	["SplatterSweatDelta"] = 0.10000000149012,
	["PickupObjectMultiplier"] = 5.0,
	["MoveObjectMultiplier"] = 12.0,
	["MoveObjectRangeCheckMultiplier"] = 12.0,
	["MoveObjectRangeCurveConstant"] = 65.0,
	["MoveObjectRangeDistanceMax"] = 6.0,
	["ReactTimeThreshold"] = 0.20000000298023,
	["AISwarmMoveGroupMinDistance"] = 15.0,
	["ScaleChangeSpeed"] = 0.5,
	["SizeChangeWeightMultiplier"] = 2.3499999046326,
	["PickpocketInteractionRange"] = 1.0,
	["DangerousLevelGap"] = 2.0,
	["FallMinDamagePathfindingCost"] = 1000.0,
	["FallMaxDamagePathfindingCost"] = 2000.0,
	["FallDeadPathfindingCost"] = 10000.0,
	["PerformerZoneJoinRadius"] = 8.0,
	["PerformerZoneSilenceRadius"] = 0.0,
	["MaxPerformerZones"] = 4.0,
	["PerformerZoneMaxPlayersByType"] = 4.0,
	["FootstepMixdownTime"] = 10.0,
	["FootstepMixWaitTime"] = 5.0,
	["AmbienceMixDownDelayTime"] = 10.0,
	["DefaultInstrumentStowedVisuals"] = 2.0,
	["HoverStateMixLeaveDelayTime"] = 1.0,
	["HoverStateMixEnterDelayTime"] = 2.0,
	["Minimum SightRange When Calculating End Of Combat Range"] = 12.0,
	["InterruptZoneRange"] = 23.0,
	["CameraFlyingZoomDistanceExtra"] = 10.0,
	["CameraFlyingZoomDistanceRemap"] = 0.5,
	["InterruptNearbyDistance"] = 18.0,
	["PingCooldownTime"] = 2.0,
	["SoundFootstepGroupMinSize"] = 3.0,
	["SoundFootstepGroupMaxRadius"] = 8.0,
	["SoundFootstepGroupBoundRadius"] = 0.5,
	["PingMarkerLifeTime"] = 4.0,
	["EscortStragglerDistance"] = 10.0,
	["DefaultUnifiedTutorialsLifeTime"] = 12000.0,
	["ForceFunctorConeAngle"] = 30.0,
	["SplitScreenMaxSoundListenerDistance"] = 20.0,
	["TutorialHotbarSlotRemovedTimeout"] = 2.0,
	["CamListener_DistPercentFromCam_Close"] = 0.0,
	["CamListener_DistPercentFromCam_Medium"] = 70.0,
	["CamListener_DistPercentFromCam_Far"] = 40.0,
	["CamListener_ZoomOut_Close"] = 0.0,
	["CamListener_ZoomOut_Medium"] = 0.40000000596046,
	["CamListener_ZoomOut_Far"] = 0.60000002384186,
	["InitiativeDie"] = 4.0,
	["PassiveSkillScoreAdvMod"] = 5.0,
	["TransferredEvidenceOnGroundRadius"] = 8.0,
	["PointLightVerticalLimit"] = 2.5,
	["CombatCameraDeathAnimationWait"] = 2.0,
	["MoveToTargetCloseEnoughMin"] = 0.5,
	["MoveToTargetCloseEnoughMax"] = 3.5,
	["DialogInstanceFlagRange"] = 10.0,
	["InterruptFarDistance"] = 30.0,
	["Level1"] = 300.0,
	["Level10"] = 20000.0,
	["Level11"] = 24000.0,
	["Level12"] = 30000.0,
	["Level2"] = 600.0,
	["Level3"] = 1800.0,
	["Level4"] = 3800.0,
	["Level5"] = 6500.0,
	["Level6"] = 8000.0,
	["Level7"] = 9000.0,
	["Level8"] = 12000.0,
	["Level9"] = 14000.0,
	["MaxXPLevel"] = 12.0,
	["MultiplyEffectsByDurationTurnLimit"] = 20.0,
	["PhotoModeCameraFloorDistance"] = 0.050000000745058,
	["PhotoModeCameraLookAtSmoothing"] = 20.0,
	["PhotoModeCameraMovementSpeed"] = 6.0,
	["PhotoModeCameraRange"] = 25.0,
	["PhotoModeCameraRotationSpeed"] = 1.2000000476837,
	["SpellZoneSurfaceGrowIntervalDefault"] = 1.0,
	["SpellZoneSurfaceGrowStepDefault"] = 100.0,
}

--#endregion

