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
--- @alias NetId integer
--- @alias NoesisString string
--- @alias NoesisSymbol string
--- @alias Path string
--- @alias PersistentRef any
--- @alias PersistentRegistryEntry any
--- @alias Ref any
--- @alias RegistryEntry any
--- @alias UserId integer
--- @alias UserReturn any
--- @alias int16 integer
--- @alias int32 integer
--- @alias int64 integer
--- @alias int8 integer
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
--- @field ActionResourceMultiplierBoost ActionResourceMultiplierBoostComponent?
--- @field ActionResourcePreventReductionBoost ActionResourcePreventReductionBoostComponent?
--- @field ActionResourceReplenishTypeOverrideBoost ActionResourceReplenishTypeOverrideBoostComponent?
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
--- @field AvailableLevel AvailableLevelComponent?
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
--- @field Bound BoundComponent?
--- @field CanBeDisarmed CanBeDisarmedComponent?
--- @field CanBeLooted CanBeLootedComponent?
--- @field CanBeWielded CanBeWieldedComponent?
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
--- @field CustomIcon CustomIconComponent?
--- @field CustomName CustomNameComponent?
--- @field CustomStats CustomStatsComponent?
--- @field DamageBonusBoost DamageBonusBoostComponent?
--- @field DamageReductionBoost DamageReductionBoostComponent?
--- @field Darkness DarknessComponent?
--- @field DarkvisionRangeBoost DarkvisionRangeBoostComponent?
--- @field DarkvisionRangeMinBoost DarkvisionRangeMinBoostComponent?
--- @field DarkvisionRangeOverrideBoost DarkvisionRangeOverrideBoostComponent?
--- @field Data DataComponent?
--- @field DeadByDefault DeadByDefaultComponent?
--- @field Death DeathComponent?
--- @field DeathState DeathStateComponent?
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
--- @field EncumbranceState EncumbranceStateComponent?
--- @field EncumbranceStats EncumbranceStatsComponent?
--- @field EntityThrowDamageBoost EntityThrowDamageBoostComponent?
--- @field EocLevel EocLevelComponent?
--- @field Equipable EquipableComponent?
--- @field EquipmentVisual EquipmentVisualComponent?
--- @field Experience ExperienceComponent?
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
--- @field HotbarContainer HotbarContainerComponent?
--- @field HotbarDecks HotbarCurrentDecksComponent?
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
--- @field IsGlobal IsGlobalComponent?
--- @field IsInTurnBasedMode IsInTurnBasedModeComponent?
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
--- @field PhysicalForceRangeBonusBoost PhysicalForceRangeBonusBoostComponent?
--- @field Physics PhysicsComponent?
--- @field PickingState PickingStateComponent?
--- @field Player PlayerComponent?
--- @field ProficiencyBonusBoost ProficiencyBonusBoostComponent?
--- @field ProficiencyBonusOverrideBoost ProficiencyBonusOverrideBoostComponent?
--- @field ProficiencyBoost ProficiencyBoostComponent?
--- @field Proficiency ProficiencyComponent?
--- @field ProficiencyGroup ProficiencyGroupComponent?
--- @field ProjectileDeflectBoost ProjectileDeflectBoostComponent?
--- @field Race RaceComponent?
--- @field RedirectDamageBoost RedirectDamageBoostComponent?
--- @field ReduceCriticalAttackThresholdBoost ReduceCriticalAttackThresholdBoostComponent?
--- @field Relation RelationComponent?
--- @field Repose ReposeStateComponent?
--- @field RerollBoost RerollBoostComponent?
--- @field ResistanceBoost ResistanceBoostComponent?
--- @field Resistances ResistancesComponent?
--- @field RollBonusBoost RollBonusBoostComponent?
--- @field Ruleset RulesetComponent?
--- @field RulesetModifiers RulesetModifiersComponent?
--- @field SavantBoost SavantBoostComponent?
--- @field Savegame SavegameComponent?
--- @field ScaleMultiplierBoost ScaleMultiplierBoostComponent?
--- @field ShapeshiftState ShapeshiftStateComponent?
--- @field ShootThroughType ShootThroughTypeComponent?
--- @field ShortRest ShortRestComponent?
--- @field SightRangeAdditiveBoost SightRangeAdditiveBoostComponent?
--- @field SightRangeMaximumBoost SightRangeMaximumBoostComponent?
--- @field SightRangeMinimumBoost SightRangeMinimumBoostComponent?
--- @field SightRangeOverrideBoost SightRangeOverrideBoostComponent?
--- @field SimpleCharacter SimpleCharacterComponent?
--- @field SkillBoost SkillBoostComponent?
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
--- @field IsCombatPaused CombatIsCombatPausedComponent?
--- @field IsInCombat CombatIsInCombatComponent?
--- @field CombatParticipant CombatParticipantComponent?
--- @field CombatState CombatStateComponent?
--- @field ThreatRange CombatThreatRangeComponent?
--- @field TurnBased CombatTurnBasedComponent?
--- @field TurnOrder CombatTurnOrderComponent?
--- @field ClientEquipmentVisuals EclEquipmentVisualsComponent?
--- @field ClientCCBaseDefinitionState EclCharacterCreationBaseDefinitionComponent?
--- @field ClientCCChangeAppearanceDefinition EclCharacterCreationChangeAppearanceDefinitionComponent?
--- @field ClientCCCompanionDefinition EclCharacterCreationCompanionDefinitionComponent?
--- @field ClientCCDefinitionState EclCharacterCreationDefinitionStateComponent?
--- @field ClientCCDefinitionStateEx EclCharacterCreationDefinitionStateExComponent?
--- @field ClientCCDummyDefinition EclCharacterCreationDummyDefinitionComponent?
--- @field ClientCCFullRespecDefinition EclCharacterCreationFullRespecDefinitionComponent?
--- @field ClientCCLevelUpDefinition EclCharacterCreationLevelUpDefinitionComponent?
--- @field BackgroundGoals EocBackgroundGoalsComponent?
--- @field CalendarDaysPassed EocCalendarDaysPassedComponent?
--- @field CalendarStartingDate EocCalendarStartingDateComponent?
--- @field FTBParticipant EocFtbParticipantComponent?
--- @field FTBRespect EocFtbRespectComponent?
--- @field FTBZoneBlockReason EocFtbZoneBlockReasonComponent?
--- @field ProjectileSource EocProjectileSourceInfoComponent?
--- @field ServerActivationGroupContainer EsvActivationGroupContainerComponent?
--- @field ServerAnubisExecutor EsvAnubisExecutorComponent?
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
--- @field ServerDeathContinue EsvDeathContinueComponent?
--- @field ServerDelayDeathCause EsvDelayDeathCauseComponent?
--- @field ServerDialogTag EsvDialogTagComponent?
--- @field ServerDisarmAttempt EsvDisarmAttemptComponent?
--- @field ServerDisplayNameList EsvDisplayNameListComponent?
--- @field ServerExperienceGaveOut EsvExperienceGaveOutComponent?
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
--- @field ServerIsUnsummoning EsvIsUnsummoningComponent?
--- @field ServerIsVisionBlocker EsvIsVisionBlockerComponent?
--- @field ServerItem EsvItem?
--- @field JumpFollow EsvJumpFollowComponent?
--- @field ServerKiller EsvKillerComponent?
--- @field ServerLeader EsvLeaderComponent?
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
--- @field ServerAiModifiers EsvAiCombatAiModifiersComponent?
--- @field ServerAiArchetype EsvAiCombatArchetypeComponent?
--- @field ServerAiInterestedInItems EsvAiCombatInterestedInItemsComponent?
--- @field ServerAiInterestingItem EsvAiCombatInterestingItemComponent?
--- @field ServerBoostBase EsvBoostBoostBaseComponent?
--- @field ServerStatusBoostsProcessed EsvBoostStatusBoostsProcessedComponent?
--- @field ServerCCAppearanceVisualTag EsvCharacterCreationAppearanceVisualTagComponent?
--- @field ServerCCEquipmentSetRequest EsvCharacterCreationEquipmentSetRequestComponent?
--- @field ServerCCGod EsvCharacterCreationGodComponent?
--- @field ServerCCIsCustom EsvCharacterCreationIsCustomComponent?
--- @field ServerCCUpdates EsvCharacterCreationUpdatesComponent?
--- @field ServerCanStartCombat EsvCombatCanStartCombatComponent?
--- @field ServerCombatGroupMapping EsvCombatCombatGroupMappingComponent?
--- @field ServerEnterRequest EsvCombatEnterRequestComponent?
--- @field ServerFleeBlocked EsvCombatFleeBlockedComponent?
--- @field ServerImmediateJoin EsvCombatImmediateJoinComponent?
--- @field EscortFollower EsvEscortFollowerComponent?
--- @field EscortHasStragglers EsvEscortHasStragglersComponent?
--- @field EscortLeader EsvEscortLeaderComponent?
--- @field EscortLeaderPriority EsvEscortLeaderPriorityComponent?
--- @field EscortMember EsvEscortMemberComponent?
--- @field EscortStragglersTracker EsvEscortStragglersTrackerComponent?
--- @field ServerInterruptActionRequests EsvInterruptActionRequestsComponent?
--- @field ServerInterruptAddRemoveRequests EsvInterruptAddRemoveRequestsComponent?
--- @field ServerInterruptInitialParticipants EsvInterruptInitialParticipantsComponent?
--- @field ServerInterruptTurnOrderInZone EsvInterruptTurnOrderInZoneComponent?
--- @field ServerInterruptZoneRequests EsvInterruptZoneRequestsComponent?
--- @field CharacterHasGeneratedTradeTreasure EsvInventoryCharacterHasGeneratedTradeTreasureComponent?
--- @field ServerInventoryContainerData EsvInventoryContainerDataComponent?
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
--- @field ServerPassiveBase EsvPassivePassiveBaseComponent?
--- @field ServerPassivePersistentData EsvPassivePersistentDataComponent?
--- @field ServerScriptPassives EsvPassiveScriptPassivesComponent?
--- @field ServerToggledPassives EsvPassiveToggledPassivesComponent?
--- @field ProgressionChangedContainers EsvProgressionChangedContainersComponent?
--- @field ServerProjectileAttachment EsvProjectileAttachmentComponent?
--- @field ServerProjectileSpell EsvProjectileSpellComponent?
--- @field ShapeshiftHealthReservation EsvShapeshiftHealthReservationComponent?
--- @field ServerShapeshiftStates EsvShapeshiftStatesComponent?
--- @field OnDamageSpells EsvSpellOnDamageSpellsComponent?
--- @field ServerSpellCastHitDelay EsvSpellCastCastHitDelayComponent?
--- @field ServerSpellCastResponsible EsvSpellCastCastResponsibleComponent?
--- @field ServerSpellClientInitiated EsvSpellCastClientInitiatedComponent?
--- @field ServerSpellExternals EsvSpellCastExternalsComponent?
--- @field ServerSpellHitRegister EsvSpellCastHitRegisterComponent?
--- @field ServerSpellInterruptRequests EsvSpellCastInterruptRequestsComponent?
--- @field ServerSpellInterruptResults EsvSpellCastInterruptResultsComponent?
--- @field ServerSpellCastState EsvSpellCastStateComponent?
--- @field ServerStatusActive EsvStatusActiveComponent?
--- @field ServerStatusAddedFromSaveLoad EsvStatusAddedFromSaveLoadComponent?
--- @field ServerStatusAura EsvStatusAuraComponent?
--- @field ServerStatusCause EsvStatusCauseComponent?
--- @field ServerStatusDifficultyModifiers EsvStatusDifficultyModifiersComponent?
--- @field ServerStatusOwnership EsvStatusOwnershipComponent?
--- @field ServerStatusPerforming EsvStatusPerformingComponent?
--- @field ServerStatus EsvStatusStatusComponent?
--- @field ServerStatusUnique EsvStatusUniqueComponent?
--- @field HealBlock HealBlockComponent?
--- @field HitAttacker HitAttackerComponent?
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
--- @field InventoryIsOwned InventoryIsOwnedComponent?
--- @field InventoryMember InventoryMemberComponent?
--- @field InventoryMemberTransform InventoryMemberTransformComponent?
--- @field NewItemsInside InventoryNewItemsInsideComponent?
--- @field NonTradable InventoryNonTradableComponent?
--- @field InventoryOwner InventoryOwnerComponent?
--- @field InventoryStack InventoryStackComponent?
--- @field InventoryStackMember InventoryStackMemberComponent?
--- @field InventoryWeight InventoryWeightComponent?
--- @field Wielded InventoryWieldedComponent?
--- @field ItemDestroyed ItemDestroyedComponent?
--- @field IsDoor ItemDoorComponent?
--- @field ItemDye ItemDyeComponent?
--- @field ExamineDisabled ItemExamineDisabledComponent?
--- @field ItemHasMoved ItemHasMovedComponent?
--- @field HasOpened ItemHasOpenedComponent?
--- @field IsGold ItemIsGoldComponent?
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
--- @field ProgressionContainer ProgressionContainerComponent?
--- @field LevelUp ProgressionLevelUpComponent?
--- @field ProgressionMeta ProgressionMetaComponent?
--- @field Recruiter RecruitRecruiterComponent?
--- @field SightData SightDataComponent?
--- @field SightEntityViewshed SightEntityViewshedComponent?
--- @field Sight SightSightBaseComponent?
--- @field AddedSpells SpellAddedSpellsComponent?
--- @field SpellBook SpellBookComponent?
--- @field SpellBookPrepares SpellBookPreparesComponent?
--- @field CCPrepareSpell SpellCCPrepareSpellComponent?
--- @field LearnedSpells SpellLearnedSpellsComponent?
--- @field PlayerPrepareSpell SpellPlayerPrepareSpellComponent?
--- @field SpellAiConditions SpellSpellAiConditionsComponent?
--- @field SpellBookCooldowns SpellSpellBookCooldownsComponent?
--- @field SpellContainer SpellSpellContainerComponent?
--- @field SpellModificationContainer SpellSpellModificationContainerComponent?
--- @field SpellCastAnimationInfo SpellCastAnimationInfoComponent?
--- @field SpellCastCache SpellCastCacheComponent?
--- @field SpellCastCanBeTargeted SpellCastCanBeTargetedComponent?
--- @field SpellCastInterruptResults SpellCastInterruptResultsComponent?
--- @field SpellCastIsCasting SpellCastIsCastingComponent?
--- @field SpellCastMovement SpellCastMovementComponent?
--- @field SpellCastRolls SpellCastRollsComponent?
--- @field SpellCastState SpellCastStateComponent?
--- @field SpellSyncTargeting SpellCastSyncTargetingComponent?
--- @field StatusCause StatusCauseComponent?
--- @field StatusContainer StatusContainerComponent?
--- @field StatusID StatusIDComponent?
--- @field StatusIncapacitated StatusIncapacitatedComponent?
--- @field StatusLifetime StatusLifetimeComponent?
--- @field StatusVisualDisabled StatusVisualDisabledComponent?
--- @field FullIllithid TadpoleTreeFullIllithidComponent?
--- @field HalfIllithid TadpoleTreeHalfIllithidComponent?
--- @field TadpolePowers TadpoleTreePowerContainerComponent?
--- @field Tadpoled TadpoleTreeTadpoledComponent?
--- @field TadpoleTreeState TadpoleTreeTreeStateComponent?


--- @alias AIFlags string|"CanNotTargetFrozen"|"CanNotUse"|"IgnoreBuff"|"IgnoreControl"|"IgnoreDebuff"|"IgnoreSelf"|"StatusIsSecondary"
--- @alias AbilityId string|"Charisma"|"Constitution"|"Dexterity"|"Intelligence"|"None"|"Sentinel"|"Strength"|"Wisdom"
--- @alias AdvantageBoostType string|"Advantage"|"Disadvantage"
--- @alias AdvantageTypeId string|"Ability"|"AllAbilities"|"AllSavingThrows"|"AllSkills"|"AttackRoll"|"AttackTarget"|"Concentration"|"DeathSavingThrow"|"SavingThrow"|"Skill"|"SourceDialogue"
--- @alias AnimationEventPropertyType string|"BoneAttachBone"|"BoneEffect"|"EFoleySoundObject"|"EVocalSoundObject"|"FloatData"|"Foot"|"Prop10"|"Prop9"|"SoundObject"|"U8Data"|"Unknown15"|"Unknown7"|"Unknown8"|"WeaponEffect"
--- @alias AnimationSetAnimationFlags string|"AlwaysIgnore"|"NoFallback"
--- @alias ArmorType string|"BreastPlate"|"ChainMail"|"ChainShirt"|"Cloth"|"HalfPlate"|"Hide"|"Leather"|"None"|"Padded"|"Plate"|"RingMail"|"ScaleMail"|"Sentinel"|"Splint"|"StuddedLeather"
--- @alias AttackRoll string|"Charisma"|"Constitution"|"Dexterity"|"Intelligence"|"None"|"SpellCastingAbility"|"Strength"|"UnarmedAttackAbility"|"WeaponAttackAbility"|"Wisdom"
--- @alias AttributeFlags string|"Arrow"|"Backstab"|"BackstabImmunity"|"EnableObscurityEvents"|"Floating"|"FloatingWhileMoving"|"ForceMainhandAlternativeEquipBones"|"Grounded"|"IgnoreClouds"|"InventoryBound"|"InvisibilityImmunity"|"InvulnerableAndInteractive"|"LootableWhenEquipped"|"LoseDurabilityOnCharacterHit"|"ObscurityWithoutSneaking"|"PickpocketableWhenEquipped"|"SlippingImmunity"|"ThrownImmunity"|"Torch"|"Unbreakable"|"Unrepairable"|"Unstorable"|"UseMusicalInstrumentForCasting"
--- @alias BoostCauseType string|"Character"|"Item"|"Osiris"|"Passive"|"Progression"|"Stats"|"Status"|"Undefined"|"Unknown5"
--- @alias BoostType string|"AC"|"ACOverrideFormula"|"Ability"|"AbilityFailedSavingThrow"|"AbilityOverrideMinimum"|"ActionResource"|"ActionResourceBlock"|"ActionResourceConsumeMultiplier"|"ActionResourceMultiplier"|"ActionResourceOverride"|"ActionResourcePreventReduction"|"ActionResourceReplenishTypeOverride"|"ActiveCharacterLight"|"AddProficiencyToAC"|"AddProficiencyToDamage"|"AdvanceSpells"|"Advantage"|"AiArchetypeOverride"|"AreaDamageEvade"|"ArmorAbilityModifierCapOverride"|"AttackSpellOverride"|"Attribute"|"BlockAbilityModifierDamageBonus"|"BlockAbilityModifierFromAC"|"BlockGatherAtCamp"|"BlockRegainHP"|"BlockSomaticComponent"|"BlockSpellCast"|"BlockTravel"|"BlockVerbalComponent"|"CanSeeThrough"|"CanShootThrough"|"CanWalkThrough"|"CannotBeDisarmed"|"CannotHarmCauseEntity"|"CarryCapacityMultiplier"|"CharacterUnarmedDamage"|"CharacterWeaponDamage"|"ConcentrationIgnoreDamage"|"ConsumeItemBlock"|"CriticalDamageOnHit"|"CriticalHit"|"CriticalHitExtraDice"|"DamageBonus"|"DamageReduction"|"DamageTakenBonus"|"DarkvisionRange"|"DarkvisionRangeMin"|"DarkvisionRangeOverride"|"Detach"|"DetectDisturbancesBlock"|"DialogueBlock"|"DodgeAttackRoll"|"DownedStatus"|"DualWielding"|"EnableBasicItemInteractions"|"EntityThrowDamage"|"ExpertiseBonus"|"FactionOverride"|"FallDamageMultiplier"|"GameplayLight"|"GameplayObscurity"|"GuaranteedChanceRollOutcome"|"HalveWeaponDamage"|"HiddenDuringCinematic"|"HorizontalFOVOverride"|"IgnoreDamageThreshold"|"IgnoreEnterAttackRange"|"IgnoreFallDamage"|"IgnoreLeaveAttackRange"|"IgnoreLowGroundPenalty"|"IgnorePointBlankDisadvantage"|"IgnoreResistance"|"IgnoreSurfaceCover"|"IncreaseMaxHP"|"Initiative"|"IntrinsicSourceProficiency"|"IntrinsicSummonerProficiency"|"Invisibility"|"Invulnerable"|"ItemReturnToOwner"|"JumpMaxDistanceBonus"|"JumpMaxDistanceMultiplier"|"LeaveTriggers"|"Lock"|"Lootable"|"MaximizeHealing"|"MaximumRollResult"|"MinimumRollResult"|"MonkWeaponAttackOverride"|"MonkWeaponDamageDiceOverride"|"MovementSpeedLimit"|"NoAOEDamageOnLand"|"NoDamageOnThrown"|"NonLethal"|"NullifyAbilityScore"|"ObjectSize"|"ObjectSizeOverride"|"PhysicalForceRangeBonus"|"Proficiency"|"ProficiencyBonus"|"ProficiencyBonusIncrease"|"ProficiencyBonusOverride"|"ProjectileDeflect"|"ReceivingCriticalDamageOnHit"|"RedirectDamage"|"ReduceCriticalAttackThreshold"|"Reroll"|"Resistance"|"RollBonus"|"Savant"|"ScaleMultiplier"|"SightRangeAdditive"|"SightRangeMaximum"|"SightRangeMinimum"|"SightRangeOverride"|"Skill"|"SoundsBlocked"|"SourceAdvantageOnAttack"|"SourceAllyAdvantageOnAttack"|"SpellResistance"|"SpellSaveDC"|"StatusImmunity"|"Tag"|"TemporaryHP"|"TwoWeaponFighting"|"UnarmedMagicalProperty"|"UnlockInterrupt"|"UnlockSpell"|"UnlockSpellVariant"|"UseBoosts"|"VoicebarkBlock"|"WeaponAttackRollAbilityOverride"|"WeaponAttackRollBonus"|"WeaponAttackTypeOverride"|"WeaponDamage"|"WeaponDamageDieOverride"|"WeaponDamageResistance"|"WeaponDamageTypeOverride"|"WeaponEnchantment"|"WeaponProperty"|"Weight"|"WeightCategory"
--- @alias CauseType string|"AURA"|"Attack"|"InventoryItem"|"None"|"Offhand"|"StatusEnter"|"StatusTick"|"SurfaceCreate"|"SurfaceMove"|"SurfaceStatus"|"WorldItemThrow"
--- @alias ClientGameState string|"AnalyticsSessionEnd"|"BuildStory"|"Disconnect"|"Exit"|"GeneratePsoCache"|"Idle"|"Init"|"InitConnection"|"InitMenu"|"InitNetwork"|"Installation"|"Join"|"LoadLevel"|"LoadMenu"|"LoadModule"|"LoadPsoCache"|"LoadSession"|"Lobby"|"Menu"|"ModReceiving"|"Movie"|"Paused"|"PrepareRunning"|"Running"|"Save"|"StartLoading"|"StartServer"|"StopLoading"|"SwapLevel"|"Unknown"|"UnloadLevel"|"UnloadModule"|"UnloadSession"
--- @alias CombatParticipantFlags string|"CanFight"|"CanJoinCombat"|"IsBoss"|"IsInspector"|"StayInAiHints"
--- @alias ConditionRollType string|"AbilityCheckRoll"|"AttackRoll"|"DifficultyRoll"|"SavingThrowRoll"|"SkillCheckRoll"
--- @alias CraftingStationType string|"Anvil"|"Beehive"|"BoilingPot"|"Cauldron"|"Misc1"|"Misc2"|"Misc3"|"Misc4"|"None"|"Oven"|"SpinningWheel"|"Well"|"Wetstone"
--- @alias CriticalHitBoostFlags string|"AttackRoll"|"AttackTarget"|"Failure"|"FailureNever"|"Success"|"SuccessNever"
--- @alias DamageFlags string|"AttackAdvantage"|"AttackDisadvantage"|"Backstab"|"Critical"|"Dodge"|"Hit"|"HitpointsDamaged"|"Invisible"|"Invulnerable"|"Magical"|"Miss"|"Projectile"|"SavingThrow"|"Status"|"StatusEnter"|"Surface"
--- @alias DamageType string|"Acid"|"Bludgeoning"|"Cold"|"Fire"|"Force"|"Lightning"|"Necrotic"|"None"|"Piercing"|"Poison"|"Psychic"|"Radiant"|"Sentinel"|"Slashing"|"Thunder"
--- @alias DealDamageWeaponDamageType string|"MainMeleeWeaponDamageType"|"MainRangedWeaponDamageType"|"MainWeaponDamageType"|"None"|"OffhandMeleeWeaponDamageType"|"OffhandRangedWeaponDamageType"|"OffhandWeaponDamageType"|"SourceWeaponDamageType"|"ThrownWeaponDamageType"
--- @alias DealDamageWeaponType string|"ImprovisedWeapon"|"MainMeleeWeapon"|"MainRangedWeapon"|"MainWeapon"|"None"|"OffhandMeleeWeapon"|"OffhandRangedWeapon"|"OffhandWeapon"|"SourceWeapon"|"ThrownWeapon"|"UnarmedDamage"
--- @alias DiceSizeId string|"D10"|"D100"|"D12"|"D20"|"D4"|"D6"|"D8"|"Default"
--- @alias ESurfaceFlag string|"Blessed"|"Blood"|"BloodCloud"|"CloudBlessed"|"CloudCursed"|"CloudElectrified"|"CloudPurified"|"CloudSurfaceBlock"|"Cursed"|"Deathfog"|"Deepwater"|"Electrified"|"ElectrifiedDecay"|"ExplosionCloud"|"Fire"|"FireCloud"|"FrostCloud"|"Frozen"|"GroundSurfaceBlock"|"HasCharacter"|"HasInteractableObject"|"HasItem"|"Irreplaceable"|"IrreplaceableCloud"|"Lava"|"MovementBlock"|"Occupied"|"Oil"|"Poison"|"PoisonCloud"|"ProjectileBlock"|"Purified"|"ShockwaveCloud"|"SmokeCloud"|"SomeDecay"|"Source"|"Sulfurium"|"SurfaceExclude"|"Water"|"WaterCloud"|"Web"
--- @alias EffectPropertyType string|"Base"|"Bool"|"ColoredFrames"|"FixedString"|"Float"|"FloatRange"|"Frames"|"FunctionType"|"Int32"|"Int32Range"|"STDString"|"Vector3"
--- @alias EquipmentStatsType string|"Armor"|"Shield"|"Weapon"
--- @alias ExecuteWeaponFunctorsType string|"BothHands"|"MainHand"|"OffHand"|"Undefined"
--- @alias ExtComponentType string|"ACOverrideFormulaBoost"|"AbilityBoost"|"AbilityFailedSavingThrowBoost"|"AbilityOverrideMinimumBoost"|"ActionResourceBlockBoost"|"ActionResourceConsumeMultiplierBoost"|"ActionResourceMultiplierBoost"|"ActionResourcePreventReductionBoost"|"ActionResourceReplenishTypeOverrideBoost"|"ActionResourceValueBoost"|"ActionResources"|"ActionType"|"ActionUseConditions"|"Active"|"ActiveCharacterLight"|"ActiveCharacterLightBoost"|"ActiveSkeletonSlots"|"AddTagBoost"|"AddedSpells"|"AdvanceSpellsBoost"|"AdvantageBoost"|"AiArchetypeOverrideBoost"|"Ambushing"|"AnimationBlueprint"|"AnimationSet"|"AnimationWaterfall"|"AppearanceOverride"|"ApprovalRatings"|"Armor"|"ArmorAbilityModifierCapOverrideBoost"|"ArmorClassBoost"|"ArmorSetState"|"AttackSpellOverrideBoost"|"AttitudesToPlayers"|"AttributeBoost"|"AttributeFlags"|"AvailableLevel"|"Avatar"|"Background"|"BackgroundGoals"|"BackgroundPassives"|"BackgroundTag"|"BaseHp"|"BaseStats"|"BlockAbilityModifierFromACBoost"|"BlockFollow"|"BlockRegainHPBoost"|"BodyType"|"BoostCondition"|"BoostInfo"|"BoostsContainer"|"Bound"|"CCChangeAppearanceDefinition"|"CCCharacterDefinition"|"CCCompanionDefinition"|"CCCreation"|"CCDefinitionCommon"|"CCDefinitionLevelUp"|"CCFullRespec"|"CCFullRespecDefinition"|"CCLevelUp"|"CCLevelUpDefinition"|"CCPrepareSpell"|"CCRespec"|"CCRespecDefinition"|"CCSessionCommon"|"CCState"|"CalendarDaysPassed"|"CalendarStartingDate"|"CampChest"|"CampEndTheDayState"|"CampPresence"|"CampQuality"|"CampSettings"|"CampSupply"|"CampTotalSupplies"|"CanBeDisarmed"|"CanBeInInventory"|"CanBeLooted"|"CanBeWielded"|"CanDeflectProjectiles"|"CanDoActions"|"CanDoRest"|"CanEnterChasm"|"CanInteract"|"CanModifyHealth"|"CanMove"|"CanSeeThrough"|"CanSeeThroughBoost"|"CanSense"|"CanShootThrough"|"CanShootThroughBoost"|"CanSpeak"|"CanTravel"|"CanTriggerRandomCasts"|"CanWalkThrough"|"CanWalkThroughBoost"|"CannotBePickpocketed"|"CannotBeTakenOut"|"CannotHarmCauseEntityBoost"|"CarryCapacityMultiplierBoost"|"CharacterCreationAppearance"|"CharacterCreationStats"|"CharacterCreationTemplateOverride"|"CharacterHasGeneratedTradeTreasure"|"CharacterUnarmedDamageBoost"|"CharacterWeaponDamageBoost"|"ClassTag"|"Classes"|"ClientCCBaseDefinitionState"|"ClientCCChangeAppearanceDefinition"|"ClientCCCompanionDefinition"|"ClientCCDefinitionState"|"ClientCCDefinitionStateEx"|"ClientCCDummyDefinition"|"ClientCCFullRespecDefinition"|"ClientCCLevelUpDefinition"|"ClientCharacter"|"ClientControl"|"ClientEquipmentVisuals"|"ClientItem"|"ClientProjectile"|"ClimbOn"|"CombatParticipant"|"CombatState"|"Concentration"|"ConcentrationIgnoreDamageBoost"|"CriticalHitBoost"|"CriticalHitExtraDiceBoost"|"CurrentlyFollowingParty"|"CustomIcon"|"CustomName"|"CustomStats"|"DamageBonusBoost"|"DamageReductionBoost"|"Darkness"|"DarkvisionRangeBoost"|"DarkvisionRangeMinBoost"|"DarkvisionRangeOverrideBoost"|"Data"|"DeadByDefault"|"Death"|"DeathState"|"Detached"|"DetectCrimesBlockBoost"|"DialogState"|"DifficultyCheck"|"DisabledEquipment"|"Disarmable"|"DisplayName"|"DodgeAttackRollBoost"|"DownedStatusBoost"|"DropOnDeathBlocked"|"DualWielding"|"DualWieldingBoost"|"DynamicAnimationTags"|"EncumbranceState"|"EncumbranceStats"|"EntityThrowDamageBoost"|"EocLevel"|"Equipable"|"EquipmentVisual"|"EscortFollower"|"EscortHasStragglers"|"EscortLeader"|"EscortLeaderPriority"|"EscortMember"|"EscortStragglersTracker"|"ExamineDisabled"|"Experience"|"Expertise"|"ExpertiseBonusBoost"|"FTBParticipant"|"FTBRespect"|"FTBZoneBlockReason"|"Faction"|"FactionOverrideBoost"|"FallDamageMultiplierBoost"|"FleeCapability"|"Floating"|"FullIllithid"|"GameObjectVisual"|"GameplayLight"|"GameplayLightBoost"|"GameplayObscurityBoost"|"GlobalLongRestDisabled"|"GlobalShortRestDisabled"|"God"|"GodTag"|"GravityDisabled"|"GravityDisabledUntilMoved"|"GuaranteedChanceRollOutcomeBoost"|"HalfIllithid"|"HalveWeaponDamageBoost"|"HasExclamationDialog"|"HasOpened"|"HealBlock"|"Health"|"Hearing"|"HistoryTargetUUID"|"HitAttacker"|"HitProxy"|"HitProxyOwner"|"HitReaction"|"HitTarget"|"HitThrownObject"|"HitWeapon"|"HorizontalFOVOverrideBoost"|"HotbarContainer"|"HotbarDecks"|"Icon"|"Identity"|"IdentityState"|"IgnoreDamageThresholdMinBoost"|"IgnoreLowGroundPenaltyBoost"|"IgnorePointBlankDisadvantageBoost"|"IgnoreResistanceBoost"|"IgnoreSurfaceCoverBoost"|"IncreaseMaxHPBoost"|"InitiativeBoost"|"InteractionDisabled"|"InteractionFilter"|"InterruptActionState"|"InterruptConditionallyDisabled"|"InterruptContainer"|"InterruptData"|"InterruptDecision"|"InterruptPreferences"|"InterruptPrepared"|"InterruptZone"|"InterruptZoneParticipant"|"InterruptZoneSource"|"InventoryContainer"|"InventoryData"|"InventoryIsOwned"|"InventoryMember"|"InventoryMemberTransform"|"InventoryOwner"|"InventoryPropertyCanBePickpocketed"|"InventoryPropertyIsDroppedOnDeath"|"InventoryPropertyIsTradable"|"InventoryStack"|"InventoryStackMember"|"InventoryWeight"|"Invisibility"|"IsCharacter"|"IsCombatPaused"|"IsDoor"|"IsGlobal"|"IsGold"|"IsInCombat"|"IsInTurnBasedMode"|"IsItem"|"IsMarkedForDeletion"|"IsStoryItem"|"IsSummon"|"ItemBoosts"|"ItemCanMove"|"ItemDestroyed"|"ItemDye"|"ItemHasGeneratedTreasure"|"ItemHasMoved"|"ItemPortal"|"ItemTemplateDestroyed"|"JumpFollow"|"JumpMaxDistanceBonusBoost"|"JumpMaxDistanceMultiplierBoost"|"Key"|"Ladder"|"LearnedSpells"|"Level"|"LevelUp"|"Lock"|"LockBoost"|"Loot"|"LootingState"|"MapMarkerStyle"|"MaterialParameterOverride"|"Max"|"MaximizeHealingBoost"|"MaximumRollResultBoost"|"MinimumRollResultBoost"|"MonkWeaponDamageDiceOverrideBoost"|"Movement"|"MovementSpeedLimitBoost"|"Net"|"NewInInventory"|"NewItemsInside"|"NonTradable"|"NullifyAbilityBoost"|"ObjectInteraction"|"ObjectSize"|"ObjectSizeBoost"|"ObjectSizeOverrideBoost"|"OffStage"|"OnDamageSpells"|"Origin"|"OriginAppearanceTag"|"OriginPassives"|"OriginTag"|"OriginalIdentity"|"OriginalTemplate"|"OwnedAsLoot"|"OwneeCurrent"|"PartyComposition"|"PartyFollower"|"PartyMember"|"PartyPortals"|"PartyRecipes"|"PartyView"|"PartyWaypoints"|"Passive"|"PassiveContainer"|"PassiveUsageCount"|"Pathing"|"PhysicalForceRangeBonusBoost"|"Physics"|"PickingState"|"Player"|"PlayerPrepareSpell"|"Proficiency"|"ProficiencyBonusBoost"|"ProficiencyBonusOverrideBoost"|"ProficiencyBoost"|"ProficiencyGroup"|"ProgressionChangedContainers"|"ProgressionContainer"|"ProgressionMeta"|"ProjectileDeflectBoost"|"ProjectileSource"|"Race"|"ReadyToBeAddedToInventory"|"Recruiter"|"RedirectDamageBoost"|"ReduceCriticalAttackThresholdBoost"|"Relation"|"Repose"|"RerollBoost"|"ResistanceBoost"|"Resistances"|"RollBonusBoost"|"Ruleset"|"RulesetModifiers"|"SavantBoost"|"Savegame"|"ScaleMultiplierBoost"|"ScriptPropertyCanBePickpocketed"|"ScriptPropertyIsDroppedOnDeath"|"ScriptPropertyIsTradable"|"ServerActivationGroupContainer"|"ServerAiArchetype"|"ServerAiInterestedInItems"|"ServerAiInterestingItem"|"ServerAiModifiers"|"ServerAnubisExecutor"|"ServerAnubisTag"|"ServerBaseData"|"ServerBaseProficiency"|"ServerBaseSize"|"ServerBaseStats"|"ServerBaseWeapon"|"ServerBoostBase"|"ServerBoostTag"|"ServerBreadcrumb"|"ServerCCAppearanceVisualTag"|"ServerCCEquipmentSetRequest"|"ServerCCGod"|"ServerCCIsCustom"|"ServerCCUpdates"|"ServerCanStartCombat"|"ServerCharacter"|"ServerCombatGroupMapping"|"ServerDarknessActive"|"ServerDeathContinue"|"ServerDeathState"|"ServerDelayDeathCause"|"ServerDialogTag"|"ServerDisarmAttempt"|"ServerDisplayNameList"|"ServerDynamicLayerOwner"|"ServerEnterRequest"|"ServerExperienceGaveOut"|"ServerFleeBlocked"|"ServerGameTimer"|"ServerGameplayLightChanges"|"ServerGameplayLightEquipment"|"ServerHotbarOrder"|"ServerIconList"|"ServerImmediateJoin"|"ServerInterruptActionRequests"|"ServerInterruptAddRemoveRequests"|"ServerInterruptInitialParticipants"|"ServerInterruptTurnOrderInZone"|"ServerInterruptZoneRequests"|"ServerInventoryContainerData"|"ServerInventoryGroupCheck"|"ServerInventoryIsReplicatedWith"|"ServerInventoryItemDataPopulated"|"ServerIsCurrentOwner"|"ServerIsLatestOwner"|"ServerIsLightBlocker"|"ServerIsOriginalOwner"|"ServerIsPreviousLatestOwner"|"ServerIsPreviousOwner"|"ServerIsUnsummoning"|"ServerIsVisionBlocker"|"ServerItem"|"ServerKiller"|"ServerLeader"|"ServerOsirisTag"|"ServerOwneeHistory"|"ServerOwneeRequest"|"ServerPassiveBase"|"ServerPassivePersistentData"|"ServerPeersInRange"|"ServerPickpocket"|"ServerProficiencyGroupStats"|"ServerProjectile"|"ServerProjectileAttachment"|"ServerProjectileInitialization"|"ServerProjectileSpell"|"ServerRaceTag"|"ServerRecruitedBy"|"ServerReplicationDependency"|"ServerReplicationDependencyOwner"|"ServerSafePosition"|"ServerScriptPassives"|"ServerShapeshiftEquipmentHistory"|"ServerShapeshiftStates"|"ServerSpellCastHitDelay"|"ServerSpellCastResponsible"|"ServerSpellCastState"|"ServerSpellClientInitiated"|"ServerSpellExternals"|"ServerSpellHitRegister"|"ServerSpellInterruptRequests"|"ServerSpellInterruptResults"|"ServerStatus"|"ServerStatusActive"|"ServerStatusAddedFromSaveLoad"|"ServerStatusAura"|"ServerStatusBoostsProcessed"|"ServerStatusCause"|"ServerStatusDifficultyModifiers"|"ServerStatusOwnership"|"ServerStatusPerforming"|"ServerStatusUnique"|"ServerSurface"|"ServerTemplateTag"|"ServerToggledPassives"|"ServerUnsheath"|"ServerUnsheathScriptOverride"|"ServerUserSnapshot"|"ServerVariableManager"|"ShapeshiftHealthReservation"|"ShapeshiftState"|"ShootThroughType"|"ShortRest"|"ShouldDestroyOnSpellCast"|"Sight"|"SightData"|"SightEntityViewshed"|"SightRangeAdditiveBoost"|"SightRangeMaximumBoost"|"SightRangeMinimumBoost"|"SightRangeOverrideBoost"|"SimpleCharacter"|"SkillBoost"|"SourceAdvantageBoost"|"Speaker"|"SpellAiConditions"|"SpellBook"|"SpellBookCooldowns"|"SpellBookPrepares"|"SpellCastAnimationInfo"|"SpellCastCache"|"SpellCastCanBeTargeted"|"SpellCastInterruptResults"|"SpellCastIsCasting"|"SpellCastMovement"|"SpellCastRolls"|"SpellCastState"|"SpellContainer"|"SpellModificationContainer"|"SpellResistanceBoost"|"SpellSaveDCBoost"|"SpellSyncTargeting"|"StaticPhysics"|"Stats"|"StatusCause"|"StatusContainer"|"StatusID"|"StatusImmunities"|"StatusImmunityBoost"|"StatusIncapacitated"|"StatusLifetime"|"StatusVisualDisabled"|"Stealth"|"Steering"|"StoryShortRestDisabled"|"SummonContainer"|"SummonLifetime"|"SurfacePathInfluences"|"TadpolePowers"|"TadpoleTreeState"|"Tadpoled"|"Tag"|"TemplateAnimationSetOverride"|"TemporaryHPBoost"|"ThreatRange"|"Trader"|"Transform"|"TurnBased"|"TurnOrder"|"UnlockInterruptBoost"|"UnlockSpellBoost"|"UnlockSpellVariantBoost"|"Unsheath"|"Use"|"UseAction"|"UseBoosts"|"UseSocket"|"UserAvatar"|"UserReservedFor"|"Uuid"|"UuidToHandleMapping"|"Value"|"Visual"|"Voice"|"VoiceTag"|"WalkOn"|"Weapon"|"WeaponAttackRollAbilityOverrideBoost"|"WeaponAttackRollBonusBoost"|"WeaponAttackTypeOverrideBoost"|"WeaponDamageBoost"|"WeaponDamageDieOverrideBoost"|"WeaponDamageResistanceBoost"|"WeaponDamageTypeOverrideBoost"|"WeaponEnchantmentBoost"|"WeaponPropertyBoost"|"WeaponSet"|"WeightBoost"|"WeightCategoryBoost"|"Wielded"|"Wielding"
--- @alias ExtQueryType string|"Max"|"UuidToHandleMapping"
--- @alias ExtResourceManagerType string|"AbilityDefaultValues"|"AbilityDistributionPreset"|"AbilityList"|"ActionResource"|"ActionResourceGroup"|"AnimationSetPriority"|"ApprovalRating"|"AreaLevelOverride"|"AvatarContainerTemplate"|"Background"|"BackgroundGoal"|"CalendarDayRange"|"CampChestTemplate"|"CharacterCreationAccessorySet"|"CharacterCreationAppearanceMaterial"|"CharacterCreationAppearanceVisual"|"CharacterCreationEquipmentIcons"|"CharacterCreationEyeColor"|"CharacterCreationHairColor"|"CharacterCreationIconSettings"|"CharacterCreationMaterialOverride"|"CharacterCreationPassiveAppearance"|"CharacterCreationPreset"|"CharacterCreationSharedVisual"|"CharacterCreationSkinColor"|"CharacterCreationVOLine"|"CinematicArenaFrequencyGroup"|"ClassDescription"|"ColorDefinition"|"CompanionPreset"|"ConditionError"|"CustomDice"|"DLC"|"DeathTypeEffect"|"DifficultyClass"|"DisturbanceProperty"|"EncumbranceType"|"EquipmentDefaultValues"|"EquipmentList"|"EquipmentType"|"ExperienceReward"|"Faction"|"Feat"|"FeatDefaultValues"|"FeatDescription"|"FeatSoundState"|"FixedHotBarSlot"|"Flag"|"FlagSoundState"|"God"|"GoldReward"|"Gossip"|"ItemThrowParams"|"ItemWallTemplate"|"LevelMap"|"LimbsMapping"|"LongRestCost"|"ManagedStatusVFX"|"Max"|"MultiEffectInfo"|"Origin"|"OriginIntroEntity"|"PassiveDefaultValues"|"PassiveList"|"PassiveVFX"|"PreparedSpellDefaultValues"|"Progression"|"ProgressionDescription"|"ProjectileDefault"|"Race"|"RandomCastOutcome"|"Ruleset"|"RulesetModifier"|"RulesetModifierOption"|"RulesetSelectionPreset"|"RulesetValue"|"ScriptMaterialParameterOverride"|"ScriptMaterialPresetOverride"|"ShapeshiftRule"|"SkillDefaultValues"|"SkillList"|"SpellDefaultValues"|"SpellList"|"SpellMetaCondition"|"SpellSoundTrajectory"|"StatusSoundState"|"SurfaceCursorMessage"|"TadpolePower"|"Tag"|"TagSoundState"|"TooltipExtraText"|"TooltipUpcastDescription"|"TrajectoryRules"|"TutorialEntries"|"TutorialEvent"|"TutorialModalEntries"|"TutorialUnifiedEntry"|"VFX"|"VisualLocatorAttachment"|"Voice"|"WeaponAnimationSet"|"WeightCategory"
--- @alias ExtSystemType string|"Max"|"PickingHelperManager"|"UISystem"
--- @alias FleeErrorFlags string|"CantMove"|"Condition"|"Downed"|"EnemyTooClose"|"Incapacitated"|"PartyFollower"|"Region"|"Summon"
--- @alias ForceFunctorAggression string|"Aggressive"|"Friendly"|"Neutral"|"Undefined"
--- @alias ForceFunctorOrigin string|"OriginToEntity"|"OriginToTarget"|"TargetToEntity"|"Undefined"
--- @alias FunctorExecParamsType string|"Type1"|"Type2"|"Type3"|"Type4"|"Type5"|"Type6"|"Type7"|"Type8"
--- @alias GameActionType string|"GameObjectMoveAction"|"PathAction"|"RainAction"|"StatusDomeAction"|"StormAction"|"TornadoAction"|"WallAction"
--- @alias Gender string|"Female"|"Male"
--- @alias GuiButtonFlags string|"MouseButtonMiddle"|"MouseButtonRight"
--- @alias GuiChildFlags string|"AlwaysUseWindowPadding"|"AutoResizeX"|"AutoResizeY"|"Border"|"ChildAlwaysAutoResize"|"FrameStyle"|"ResizeX"|"ResizeY"
--- @alias GuiColor string|"Border"|"BorderShadow"|"Button"|"ButtonActive"|"ButtonHovered"|"CheckMark"|"ChildBg"|"DragDropTarget"|"FrameBg"|"FrameBgActive"|"FrameBgHovered"|"Header"|"HeaderActive"|"HeaderHovered"|"MenuBarBg"|"ModalWindowDimBg"|"NavHighlight"|"NavWindowingDimBg"|"NavWindowingHighlight"|"PlotHistogram"|"PlotHistogramHovered"|"PlotLines"|"PlotLinesHovered"|"PopupBg"|"ResizeGrip"|"ResizeGripActive"|"ResizeGripHovered"|"ScrollbarBg"|"ScrollbarGrab"|"ScrollbarGrabActive"|"ScrollbarGrabHovered"|"Separator"|"SeparatorActive"|"SeparatorHovered"|"SliderGrab"|"SliderGrabActive"|"Tab"|"TabActive"|"TabHovered"|"TabUnfocused"|"TabUnfocusedActive"|"TableBorderLight"|"TableBorderStrong"|"TableHeaderBg"|"TableRowBg"|"TableRowBgAlt"|"Text"|"TextDisabled"|"TextSelectedBg"|"TitleBg"|"TitleBgActive"|"TitleBgCollapsed"|"WindowBg"
--- @alias GuiColorEditFlags string|"AlphaBar"|"AlphaPreview"|"AlphaPreviewHalf"|"DisplayHSV"|"DisplayHex"|"DisplayRGB"|"Float"|"HDR"|"InputHSV"|"InputRGB"|"NoAlpha"|"NoBorder"|"NoDragDrop"|"NoInputs"|"NoLabel"|"NoOptions"|"NoPicker"|"NoSidePreview"|"NoSmallPreview"|"NoTooltip"|"PickerHueBar"|"PickerHueWheel"|"Uint8"
--- @alias GuiComboFlags string|"HeightLarge"|"HeightLargest"|"HeightRegular"|"HeightSmall"|"NoArrowButton"|"NoPreview"|"PopupAlignLeft"|"WidthFitPreview"
--- @alias GuiCond string|"Always"|"Appearing"|"FirstUseEver"|"Once"
--- @alias GuiInputTextFlags string|"AllowTabInput"|"AlwaysOverwrite"|"AutoSelectAll"|"CallbackAlways"|"CallbackCharFilter"|"CallbackCompletion"|"CallbackEdit"|"CallbackHistory"|"CallbackResize"|"CharsDecimal"|"CharsHexadecimal"|"CharsNoBlank"|"CharsScientific"|"CharsUppercase"|"CtrlEnterForNewLine"|"EnterReturnsTrue"|"EscapeClearsAll"|"Multiline"|"NoHorizontalScroll"|"NoUndoRedo"|"Password"|"ReadOnly"
--- @alias GuiPopupFlags string|"AnyPopupId"|"AnyPopupLevel"|"MouseButtonMiddle"|"MouseButtonRight"|"NoOpenOverExistingPopup"|"NoOpenOverItems"|"NoReopen"
--- @alias GuiSliderFlags string|"AlwaysClamp"|"Logarithmic"|"NoInput"|"NoRoundToFormat"
--- @alias GuiStyleVar string|"Alpha"|"ButtonTextAlign"|"CellPadding"|"ChildBorderSize"|"ChildRounding"|"DisabledAlpha"|"FrameBorderSize"|"FramePadding"|"FrameRounding"|"GrabMinSize"|"GrabRounding"|"IndentSpacing"|"ItemInnerSpacing"|"ItemSpacing"|"PopupBorderSize"|"PopupRounding"|"ScrollbarRounding"|"ScrollbarSize"|"SelectableTextAlign"|"SeparatorTextAlign"|"SeparatorTextBorderSize"|"SeparatorTextPadding"|"TabBarBorderSize"|"TabRounding"|"WindowBorderSize"|"WindowMinSize"|"WindowPadding"|"WindowRounding"|"WindowTitleAlign"
--- @alias GuiTabBarFlags string|"AutoSelectNewTabs"|"FittingPolicyDefault"|"FittingPolicyMask"|"FittingPolicyResizeDown"|"FittingPolicyScroll"|"NoCloseWithMiddleMouseButton"|"NoTabListScrollingButtons"|"NoTooltip"|"Reorderable"|"TabListPopupButton"
--- @alias GuiTabItemFlags string|"Leading"|"NoAssumedClosure"|"NoCloseWithMiddleMouseButton"|"NoPushId"|"NoReorder"|"NoTooltip"|"SetSelected"|"Trailing"|"UnsavedDocument"
--- @alias GuiTableColumnFlags string|"AngledHeader"|"DefaultHide"|"DefaultSort"|"Disabled"|"IndentDisable"|"IndentEnable"|"NoClip"|"NoHeaderLabel"|"NoHeaderWidth"|"NoHide"|"NoReorder"|"NoResize"|"NoSort"|"NoSortAscending"|"NoSortDescending"|"PreferSortAscending"|"PreferSortDescending"|"WidthFixed"|"WidthStretch"
--- @alias GuiTableFlags string|"Borders"|"BordersH"|"BordersInner"|"BordersInnerH"|"BordersInnerV"|"BordersOuter"|"BordersOuterH"|"BordersOuterV"|"BordersV"|"ContextMenuInBody"|"Hideable"|"HighlightHoveredColumn"|"NoBordersInBody"|"NoBordersInBodyUntilResize"|"NoClip"|"NoHostExtendX"|"NoHostExtendY"|"NoKeepColumnsVisible"|"NoPadInnerX"|"NoPadOuterX"|"NoSavedSettings"|"PadOuterX"|"PreciseWidths"|"Reorderable"|"Resizable"|"RowBg"|"ScrollX"|"ScrollY"|"SizingFixedFit"|"SizingFixedSame"|"SizingStretchProp"|"SizingStretchSame"|"SortMulti"|"SortTristate"|"Sortable"
--- @alias GuiTableRowFlags string|"Headers"
--- @alias GuiTreeNodeFlags string|"AllowOverlap"|"Bullet"|"CollapsingHeader"|"DefaultOpen"|"FramePadding"|"Framed"|"Leaf"|"NavLeftJumpsBackHere"|"NoAutoOpenOnLog"|"NoTreePushOnOpen"|"OpenOnArrow"|"OpenOnDoubleClick"|"Selected"|"SpanAllColumns"|"SpanAvailWidth"|"SpanFullWidth"
--- @alias GuiWindowFlags string|"AlwaysAutoResize"|"AlwaysHorizontalScrollbar"|"AlwaysVerticalScrollbar"|"HorizontalScrollbar"|"MenuBar"|"NoBackground"|"NoBringToFrontOnFocus"|"NoCollapse"|"NoDecoration"|"NoFocusOnAppearing"|"NoInputs"|"NoMouseInputs"|"NoMove"|"NoNav"|"NoNavFocus"|"NoNavInputs"|"NoResize"|"NoSavedSettings"|"NoScrollWithMouse"|"NoScrollbar"|"NoTitleBar"|"UnsavedDocument"
--- @alias HandednessType string|"Any"|"One"|"Two"
--- @alias HealDirection string|"Incoming"|"Outgoing"
--- @alias HealEffect string|"Behavior"|"Heal"|"HealSharing"|"HealSharingReflected"|"Lifesteal"|"Necromantic"|"NegativeDamage"|"None"|"ResistDeath"|"Script"|"Sitting"|"Surface"|"Unknown4"|"Unknown9"
--- @alias HitType string|"DoT"|"Magic"|"Melee"|"Ranged"|"Reflected"|"Surface"|"WeaponDamage"
--- @alias HitWith string|"CrushByFall"|"FallDamage"|"Item"|"Magic"|"None"|"Projectile"|"Redirection"|"Surface"|"Trap"|"Unknown"|"Unknown10"|"Unknown11"|"Weapon"
--- @alias IngredientTransformType string|"Boost"|"Consume"|"None"|"Poison"|"Transform"
--- @alias IngredientType string|"Category"|"None"|"Object"|"Property"
--- @alias InputType string|"AcceleratedRepeat"|"Hold"|"Press"|"Release"|"Repeat"|"Unknown"|"ValueChange"
--- @alias ItemDataRarity string|"Common"|"Divine"|"Epic"|"Legendary"|"Rare"|"Sentinel"|"Uncommon"|"Unique"
--- @alias ItemSlot string|"Amulet"|"Boots"|"Breast"|"Cloak"|"Gloves"|"Helmet"|"Horns"|"MainHand"|"MeleeMainHand"|"MeleeOffHand"|"MusicalInstrument"|"OffHand"|"Overhead"|"RangedMainHand"|"RangedOffHand"|"Ring"|"Ring2"|"Underwear"|"VanityBody"|"VanityBoots"|"Wings"
--- @alias LuaTypeId string|"Any"|"Array"|"Boolean"|"Enumeration"|"Float"|"Function"|"Integer"|"Map"|"Module"|"Nullable"|"Object"|"Set"|"String"|"Tuple"|"Unknown"|"Variant"|"Void"
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
--- @alias PathRootType string|"Bin"|"Bin2"|"Data"|"Debug"|"EngineMod"|"GameMod"|"LocalAppData"|"Localization"|"Mods"|"Projects"|"Public"|"Public2"|"Root"|"Scripts"|"UserProfile"|"WorkingDir"
--- @alias ProficiencyBonusBoostType string|"Ability"|"AllAbilities"|"AllSavingThrows"|"AllSkills"|"AttackRoll"|"AttackTarget"|"SavingThrow"|"Skill"|"SourceDialogue"|"WeaponActionDC"
--- @alias ProficiencyGroupFlags string|"Battleaxes"|"Clubs"|"Daggers"|"Darts"|"Flails"|"Glaives"|"Greataxes"|"Greatclubs"|"Greatswords"|"Halberds"|"HandCrossbows"|"Handaxes"|"HeavyArmor"|"HeavyCrossbows"|"Javelins"|"LightArmor"|"LightCrossbows"|"LightHammers"|"Longbows"|"Longswords"|"Maces"|"MartialWeapons"|"Mauls"|"MediumArmor"|"Morningstars"|"MusicalInstrument"|"Pikes"|"Quarterstaffs"|"Rapiers"|"Scimitars"|"Shields"|"Shortbows"|"Shortswords"|"Sickles"|"SimpleWeapons"|"Slings"|"Spears"|"Tridents"|"Warhammers"|"Warpicks"
--- @alias ProjectileTypeIds string|"Magical"|"Physical"
--- @alias RecipeCategory string|"Armour"|"Arrows"|"Common"|"Food"|"Grenades"|"Grimoire"|"Objects"|"Potions"|"Runes"|"Weapons"
--- @alias RequirementType string|"Barter"|"Charm"|"Combat"|"Constitution"|"Dexterity"|"Immobile"|"Intelligence"|"Intimidate"|"Level"|"Loremaster"|"Necromancy"|"None"|"PainReflection"|"Persuasion"|"Pickpocket"|"Ranged"|"Reason"|"Reflexes"|"Sneaking"|"Sourcery"|"Strength"|"Summoning"|"Tag"|"Telekinesis"|"TurnBased"|"Vitality"|"Wisdom"
--- @alias ResistanceBoostFlags string|"BelowDamageThreshold"|"ImmuneToMagical"|"ImmuneToNonMagical"|"ResistantToMagical"|"ResistantToNonMagical"|"VulnerableToMagical"|"VulnerableToNonMagical"
--- @alias ResourceBankType string|"Animation"|"AnimationBlueprint"|"AnimationSet"|"Atmosphere"|"BlendSpace"|"CharacterVisual"|"ClothCollider"|"ColorList"|"Dialog"|"DiffusionProfile"|"Effect"|"FCurve"|"IKRig"|"LightCookie"|"Lighting"|"Material"|"MaterialPreset"|"MaterialSet"|"MeshProxy"|"Physics"|"Script"|"Sentinel"|"Skeleton"|"SkeletonMirrorTable"|"SkinPreset"|"Sound"|"TerrainBrush"|"Texture"|"TileSet"|"Timeline"|"TimelineScene"|"VirtualTexture"|"Visual"|"VisualSet"|"VoiceBark"
--- @alias ResourceReplenishType string|"Combat"|"Default"|"ExhaustedRest"|"FullRest"|"Never"|"Rest"|"ShortRest"
--- @alias RestErrorFlags string|"Combat"|"Dialog"|"DownedOrDead"|"FTB"|"GlobalDisabled"|"NotEnoughResources"|"Script"
--- @alias RestFlags string|"CanLongRest"|"CanShortRest"|"Unknown0x04"
--- @alias RollCritical string|"Fail"|"None"|"Success"
--- @alias SDLKeyEvent string|"KeyDown"|"KeyUp"
--- @alias SDLKeyModifier string|"Caps"|"LAlt"|"LCtrl"|"LGui"|"LShift"|"Mode"|"Num"|"RAlt"|"RCtrl"|"RGui"|"RShift"|"Scroll"
--- @alias SDLScanCode string|"A"|"AC_BACK"|"AC_BOOKMARKS"|"AC_FORWARD"|"AC_HOME"|"AC_REFRESH"|"AC_SEARCH"|"AC_STOP"|"AGAIN"|"ALTERASE"|"APOSTROPHE"|"APP1"|"APP2"|"APPLICATION"|"AUDIOFASTFORWARD"|"AUDIOMUTE"|"AUDIONEXT"|"AUDIOPLAY"|"AUDIOPREV"|"AUDIOREWIND"|"AUDIOSTOP"|"B"|"BACKSLASH"|"BACKSPACE"|"BRIGHTNESSDOWN"|"BRIGHTNESSUP"|"C"|"CALCULATOR"|"CALL"|"CANCEL"|"CAPSLOCK"|"CLEAR"|"CLEARAGAIN"|"COMMA"|"COMPUTER"|"COPY"|"CRSEL"|"CURRENCYSUBUNIT"|"CURRENCYUNIT"|"CUT"|"D"|"DECIMALSEPARATOR"|"DEL"|"DISPLAYSWITCH"|"DOWN"|"E"|"EJECT"|"END"|"ENDCALL"|"EQUALS"|"ESCAPE"|"EXECUTE"|"EXSEL"|"F"|"F1"|"F10"|"F11"|"F12"|"F13"|"F14"|"F15"|"F16"|"F17"|"F18"|"F19"|"F2"|"F20"|"F21"|"F22"|"F23"|"F24"|"F3"|"F4"|"F5"|"F6"|"F7"|"F8"|"F9"|"FIND"|"G"|"GRAVE"|"H"|"HELP"|"HOME"|"I"|"INSERT"|"INTERNATIONAL1"|"INTERNATIONAL2"|"INTERNATIONAL3"|"INTERNATIONAL4"|"INTERNATIONAL5"|"INTERNATIONAL6"|"INTERNATIONAL7"|"INTERNATIONAL8"|"INTERNATIONAL9"|"J"|"K"|"KBDILLUMDOWN"|"KBDILLUMTOGGLE"|"KBDILLUMUP"|"KEY_OUT"|"KP_0"|"KP_00"|"KP_000"|"KP_1"|"KP_2"|"KP_3"|"KP_4"|"KP_5"|"KP_6"|"KP_7"|"KP_8"|"KP_9"|"KP_A"|"KP_AMPERSAND"|"KP_AT"|"KP_B"|"KP_BACKSPACE"|"KP_BINARY"|"KP_C"|"KP_CLEAR"|"KP_CLEARENTRY"|"KP_COLON"|"KP_COMMA"|"KP_D"|"KP_DBLAMPERSAND"|"KP_DBLVERTICALBAR"|"KP_DECIMAL"|"KP_DIVIDE"|"KP_E"|"KP_ENTER"|"KP_EQUALS"|"KP_EQUALSAS400"|"KP_EXCLAM"|"KP_F"|"KP_GREATER"|"KP_HASH"|"KP_HEXADECIMAL"|"KP_LEFTBRACE"|"KP_LEFTPAREN"|"KP_LESS"|"KP_MEMADD"|"KP_MEMCLEAR"|"KP_MEMDIVIDE"|"KP_MEMMULTIPLY"|"KP_MEMRECALL"|"KP_MEMSTORE"|"KP_MEMSUBTRACT"|"KP_MINUS"|"KP_MULTIPLY"|"KP_OCTAL"|"KP_PERCENT"|"KP_PERIOD"|"KP_PLUS"|"KP_PLUSMINUS"|"KP_POWER"|"KP_RIGHTBRACE"|"KP_RIGHTPAREN"|"KP_SPACE"|"KP_TAB"|"KP_VERTICALBAR"|"KP_XOR"|"L"|"LALT"|"LANG1"|"LANG2"|"LANG3"|"LANG4"|"LANG5"|"LANG6"|"LANG7"|"LANG8"|"LANG9"|"LCTRL"|"LEFT"|"LEFTBRACKET"|"LGUI"|"LSHIFT"|"M"|"MAIL"|"MEDIASELECT"|"MENU"|"MINUS"|"MODE"|"MUTE"|"N"|"NONUSBACKSLASH"|"NONUSHASH"|"NUMLOCKCLEAR"|"NUM_0"|"NUM_1"|"NUM_2"|"NUM_3"|"NUM_4"|"NUM_5"|"NUM_6"|"NUM_7"|"NUM_8"|"NUM_9"|"O"|"OPER"|"P"|"PAGEDOWN"|"PAGEUP"|"PASTE"|"PAUSE"|"PERIOD"|"POWER"|"PRINTSCREEN"|"PRIOR"|"Q"|"R"|"RALT"|"RCTRL"|"RETURN"|"RETURN2"|"RGUI"|"RIGHT"|"RIGHTBRACKET"|"RSHIFT"|"S"|"SCROLLLOCK"|"SELECT"|"SEMICOLON"|"SEPARATOR"|"SLASH"|"SLEEP"|"SOFTLEFT"|"SOFTRIGHT"|"SPACE"|"STOP"|"SYSREQ"|"T"|"TAB"|"THOUSANDSSEPARATOR"|"U"|"UNDO"|"UNKNOWN"|"UP"|"V"|"VOLUMEDOWN"|"VOLUMEUP"|"W"|"WWW"|"X"|"Y"|"Z"
--- @alias ServerCharacterFlags string|"Activated"|"CanShootThrough"|"CannotAttachToGroups"|"CannotDie"|"CannotMove"|"CannotRun"|"CharCreationInProgress"|"CharacterControl"|"CharacterCreationFinished"|"CoverAmount"|"CustomLookEnabled"|"Deactivated"|"Dead"|"DeferredRemoveEscapist"|"Detached"|"DisableCulling"|"DisableWaypointUsage"|"DoNotFaceFlag"|"DontCacheTemplate"|"FightMode"|"FindValidPositionOnActivate"|"Floating"|"ForceNonzeroSpeed"|"GMReroll"|"HasOwner"|"HostControl"|"IgnoresTriggers"|"InDialog"|"InParty"|"Invisible"|"Invulnerable"|"IsCompanion_M"|"IsHuge"|"IsPlayer"|"IsTrading"|"LevelTransitionPending"|"Loaded"|"Multiplayer"|"NeedsMakePlayerUpdate"|"OffStage"|"PartyFollower"|"RegisteredForAutomatedDialog"|"RequestStartTurn"|"ReservedForDialog"|"SpotSneakers"|"SteeringEnabled"|"StoryNPC"|"Summon"|"Temporary"|"Totem"|"Unknown10000000000"|"Unknown40000000000"|"Unknown8000"|"Unknown80000000"|"Unknown80000000000000"|"WalkThrough"
--- @alias ServerCharacterFlags2 string|"CanGossip"|"Global"|"IsPet"|"IsResurrected"|"Trader"|"TreasureGeneratedForTrader"|"Unknown0x10"|"Unknown0x40"
--- @alias ServerCharacterFlags3 string|"CrimeWarningsEnabled"|"NeedsPlacementSnapping"
--- @alias ServerGameState string|"BuildStory"|"Disconnect"|"Exit"|"Idle"|"Init"|"LoadLevel"|"LoadModule"|"LoadSession"|"Paused"|"ReloadStory"|"Running"|"Save"|"Sync"|"Uninitialized"|"Unknown"|"UnloadLevel"|"UnloadModule"|"UnloadSession"
--- @alias ServerItemFlags string|"Activated"|"CanBeMoved"|"CanBePickedUp"|"CanOnlyBeUsedByOwner"|"CanShootThrough"|"CanUse"|"ClientSync1"|"Destroy"|"Destroyed"|"DisableInventoryView80"|"DisableSync"|"DontAddToHotbar"|"Floating"|"ForceClientSync"|"ForceSync"|"FreezeGravity"|"Frozen"|"GMFolding"|"HideHP"|"InAutomatedDialog"|"InPartyInventory"|"InUse"|"InteractionDisabled"|"Invisible"|"Invisible2"|"Invulnerable"|"Invulnerable2"|"IsContainer"|"IsDoor"|"IsLadder"|"IsMoving"|"IsSecretDoor"|"IsSurfaceBlocker"|"IsSurfaceCloudBlocker"|"Known"|"LevelTransitionPending"|"LoadedTemplate"|"LuckyFind"|"NoCover"|"OffStage"|"PinnedContainer"|"PositionChanged"|"ReservedForDialog"|"SourceContainer"|"Sticky"|"StoryItem"|"Summon"|"TeleportOnUse"|"Totem"|"TransformChanged"|"WakePhysics"|"WalkOn"|"WalkThrough"
--- @alias ServerItemFlags2 string|"Global"|"TreasureGenerated"|"UnEquipLocked"|"UnsoldGenerated"|"UseRemotely"
--- @alias ServerStatusFlags string|"ForceFailStatus"|"ForceStatus"|"IgnoreStacking"|"Loaded"|"RequestClientSync"|"RequestDelete"|"RequestDeleteAtTurnEnd"|"Started"
--- @alias ServerStatusFlags2 string|"DontTickWhileOnSurface"|"Influence"|"InitiateCombat"|"IsChanneled"|"IsFromItem"|"IsLifeTimeSet"|"IsOnSourceSurface"|"KeepAlive"
--- @alias ServerStatusFlags3 string|"DisableImmunityOverhead"|"IsUnique"|"NotifiedPlanManager"|"StatusFlags3_0x08"|"StatusFlags3_0x10"|"StatusFlags3_0x20"|"StatusFlags3_0x40"|"StatusFlags3_0x80"
--- @alias ServerStatusFlags4 string|"BringIntoCombat"|"CauseIsOrigin"|"CausedByEquipment"|"IsHostileAct"|"IsInvulnerable"|"IsInvulnerableVisible"|"StatusFlags4_0x04"|"StatusFlags4_0x80"
--- @alias ServerStatusFlags5 string|"HasTriedEntering"
--- @alias SetStatusDurationType string|"Add"|"ForceSet"|"Multiply"|"SetMinimum"|"Unknown"
--- @alias ShroudType string|"RegionMask"|"Shroud"|"Sight"|"Sneak"
--- @alias SkillId string|"Acrobatics"|"AnimalHandling"|"Arcana"|"Athletics"|"Deception"|"History"|"Insight"|"Intimidation"|"Invalid"|"Investigation"|"Medicine"|"Nature"|"Perception"|"Performance"|"Persuasion"|"Religion"|"Sentinel"|"SleightOfHand"|"Stealth"|"Survival"
--- @alias SourceAdvantageType string|"SourceAdvantageOnAttack"|"SourceAllyAdvantageOnAttack"
--- @alias SpellAttackType string|"DirectHit"|"MeleeOffHandWeaponAttack"|"MeleeSpellAttack"|"MeleeUnarmedAttack"|"MeleeWeaponAttack"|"None"|"RangedOffHandWeaponAttack"|"RangedSpellAttack"|"RangedUnarmedAttack"|"RangedWeaponAttack"
--- @alias SpellAttackTypeOverride string|"Projectile_MainHandAttack"|"Projectile_OffhandAttack"|"Target_MainHandAttack"|"Target_OffhandAttack"|"Target_UnarmedAttack"
--- @alias SpellChildSelectionType string|"AddChildren"|"MostPowerful"|"Singular"
--- @alias SpellCooldownType string|"Default"|"OncePerCombat"|"OncePerShortRestPerItem"|"OncePerTurn"|"OncePerTurnNoRealtime"|"UntilPerRestPerItem"|"UntilRest"|"UntilShortRest"
--- @alias SpellFlags string|"AbortOnSecondarySpellRollFail"|"AbortOnSpellRollFail"|"AddFallDamageOnLand"|"AddWeaponRange"|"AllowMoveAndCast"|"CallAlliesSpell"|"CanAreaDamageEvade"|"CanDualWield"|"CannotRotate"|"CannotTargetCharacter"|"CannotTargetItems"|"CannotTargetTerrain"|"CombatLogSetSingleLineRoll"|"ConcentrationIgnoresResting"|"DisableBlood"|"DisplayInItemTooltip"|"DontAbortPerforming"|"HasHighGroundRangeExtension"|"HasSomaticComponent"|"HasVerbalComponent"|"HideInItemTooltip"|"IgnoreAoO"|"IgnorePreviouslyPickedEntities"|"IgnoreSilence"|"IgnoreVisionBlock"|"ImmediateCast"|"InventorySelection"|"Invisible"|"IsAttack"|"IsConcentration"|"IsDefaultWeaponAction"|"IsEnemySpell"|"IsHarmful"|"IsJump"|"IsLinkedSpellContainer"|"IsMelee"|"IsSpell"|"IsSwarmAttack"|"IsTrap"|"NoAOEDamageOnLand"|"NoCameraMove"|"NoCooldownOnMiss"|"NoSurprise"|"PickupEntityAndMove"|"RangeIgnoreBlindness"|"RangeIgnoreSourceBounds"|"RangeIgnoreTargetBounds"|"RangeIgnoreVerticalThreshold"|"Stealth"|"TargetClosestEqualGroundSurface"|"Temporary"|"TrajectoryRules"|"UNUSED_D"|"UNUSED_E"|"UnavailableInDialogs"|"Wildshape"
--- @alias SpellPrepareType string|"AlwaysPrepared"|"RequiresPreparation"|"Unknown"
--- @alias SpellSchoolId string|"Abjuration"|"Conjuration"|"Divination"|"Enchantment"|"Evocation"|"Illusion"|"Necromancy"|"None"|"Transmutation"
--- @alias SpellSourceType string|"ActiveDefense"|"AiTest"|"Anubis"|"Behavior"|"Boost"|"CreateExplosion"|"EquippedItem"|"GameActionCreateSurface"|"Learned"|"Osiris"|"PartyPreset"|"Progression"|"Progression0"|"Progression1"|"Progression2"|"Sentinel"|"Shapeshift"|"Spell"|"SpellSet"|"SpellSet2"|"UnarmedAttack"|"Unknown15"|"Unknown16"|"Unknown17"|"Unknown18"|"Unknown19"|"Unknown1A"|"WeaponAttack"
--- @alias SpellType string|"MultiStrike"|"Projectile"|"ProjectileStrike"|"Rush"|"Shout"|"Storm"|"Target"|"Teleportation"|"Throw"|"Wall"|"Zone"
--- @alias StatAttributeFlags string|"Arrow"|"Backstab"|"BackstabImmunity"|"EMPTY"|"EnableObscurityEvents"|"Floating"|"FloatingWhileMoving"|"Grounded"|"IgnoreClouds"|"InvisibilityImmunity"|"InvulnerableAndInteractive"|"LootableWhenEquipped"|"LoseDurabilityOnCharacterHit"|"ObscurityWithoutSneaking"|"PickpocketableWhenEquipped"|"SlippingImmunity"|"ThrownImmunity"|"Torch"|"Unbreakable"|"Unrepairable"|"Unstorable"
--- @alias StatCharacterFlags string|"Blind"|"DrinkedPotion"|"EquipmentValidated"|"InParty"|"Invisible"|"IsPlayer"|"IsSneaking"
--- @alias StatsDeathType string|"Acid"|"Chasm"|"CinematicDeath"|"Cold"|"Disintegrate"|"DoT"|"Electrocution"|"Explode"|"Falling"|"Incinerate"|"KnockedDown"|"Lifetime"|"Necrotic"|"None"|"Physical"|"Psychic"|"Radiant"
--- @alias StatsExpressionParamType string|"Bool"|"Int"|"ResourceRollDefinition"|"RollDefinition"|"StatsContextType"|"StatsExpressionType"|"StatsExpressionVariableDataModifier"|"StatsExpressionVariableDataType"|"Type1"
--- @alias StatsExpressionParamType2 string|"Ability"|"STDString"|"Skill"|"StatsExpressionVariableData"|"StatusGroup"
--- @alias StatsFunctorFlags string|"Owner"|"Self"|"Swap"
--- @alias StatsFunctorId string|"AdjustRoll"|"ApplyEquipmentStatus"|"ApplyStatus"|"BreakConcentration"|"CameraWait"|"Counterspell"|"CreateConeSurface"|"CreateExplosion"|"CreateSurface"|"CreateWall"|"CreateZone"|"CustomDescription"|"DealDamage"|"DisarmAndStealWeapon"|"DisarmWeapon"|"DoTeleport"|"Douse"|"Drop"|"ExecuteWeaponFunctors"|"Extender"|"FireProjectile"|"Force"|"GainTemporaryHitPoints"|"Kill"|"MaximizeRoll"|"Pickup"|"RegainHitPoints"|"RegainTemporaryHitPoints"|"RemoveAuraByChildStatus"|"RemoveStatus"|"RemoveStatusByLevel"|"RemoveUniqueStatus"|"ResetCombatTurn"|"ResetCooldowns"|"RestoreResource"|"Resurrect"|"Sabotage"|"SetAdvantage"|"SetDamageResistance"|"SetDisadvantage"|"SetReroll"|"SetRoll"|"SetStatusDuration"|"ShortRest"|"Spawn"|"SpawnExtraProjectiles"|"SpawnInInventory"|"Stabilize"|"Summon"|"SummonInInventory"|"SurfaceChange"|"SurfaceClearLayer"|"SwapPlaces"|"SwitchDeathType"|"TeleportSource"|"TriggerRandomCast"|"TutorialEvent"|"Unlock"|"Unsummon"|"UseActionResource"|"UseAttack"|"UseSpell"
--- @alias StatsItemSlot string|"Amulet"|"Boots"|"Breast"|"Cloak"|"Gloves"|"Helmet"|"Horns"|"MainHand"|"Max"|"MeleeMainHand"|"MeleeOffHand"|"MusicalInstrument"|"OffHand"|"Overhead"|"RangedMainHand"|"RangedOffHand"|"Ring"|"Ring2"|"Sentinel"|"Underwear"|"VanityBody"|"VanityBoots"|"Wings"
--- @alias StatsObserverType string|"None"|"Observer"|"Source"|"Target"
--- @alias StatsPropertyContext string|"ABILITY_CHECK"|"ACTION_RESOURCES_CHANGED"|"AI_IGNORE"|"AI_ONLY"|"AOE"|"ATTACK"|"ATTACKED"|"ATTACKED_IN_MELEE_RANGE"|"ATTACKING_IN_MELEE_RANGE"|"CAST"|"CAST_RESOLVED"|"COMBAT_ENDED"|"CREATE"|"DAMAGE"|"DAMAGED"|"DAMAGED_PREVENTED"|"DAMAGE_PREVENTED"|"ENTER_ATTACK_RANGE"|"EQUIP"|"GROUND"|"HEAL"|"HEALED"|"INTERRUPT_USED"|"INVENTORY_CHANGED"|"LEAVE_ATTACK_RANGE"|"LOCKPICKING_SUCCEEDED"|"LONG_REST"|"MOVED_DISTANCE"|"OBSCURITY_CHANGED"|"PROFICIENCY_CHANGED"|"PROJECTILE"|"PUSH"|"PUSHED"|"SHORT_REST"|"STATUS_APPLIED"|"STATUS_APPLY"|"STATUS_REMOVE"|"STATUS_REMOVED"|"SURFACE_ENTER"|"TARGET"|"TURN"
--- @alias StatsRollAdjustmentType string|"All"|"Distribute"
--- @alias StatsRollType string|"Attack"|"Damage"|"DeathSavingThrow"|"MeleeOffHandWeaponAttack"|"MeleeSpellAttack"|"MeleeSpellDamage"|"MeleeUnarmedAttack"|"MeleeUnarmedDamage"|"MeleeWeaponAttack"|"MeleeWeaponDamage"|"None"|"RangedOffHandWeaponAttack"|"RangedSpellAttack"|"RangedSpellDamage"|"RangedUnarmedAttack"|"RangedUnarmedDamage"|"RangedWeaponAttack"|"RangedWeaponDamage"|"RawAbility"|"SavingThrow"|"Sentinel"|"SkillCheck"
--- @alias StatsStatusGroup string|"SG_Approaching"|"SG_Blinded"|"SG_CanBePickedUp"|"SG_Charmed"|"SG_Charmed_Subtle"|"SG_Condition"|"SG_Confused"|"SG_Cursed"|"SG_DetectThoughts"|"SG_DifficultTerrain"|"SG_Disease"|"SG_Disguise"|"SG_Dominated"|"SG_Doppelganger"|"SG_DropForNonMutingDialog"|"SG_Drunk"|"SG_Exhausted"|"SG_Fleeing"|"SG_Frightened"|"SG_Helpable_Condition"|"SG_HexbladeCurse"|"SG_Ignore_AOO"|"SG_Incapacitated"|"SG_Invisible"|"SG_Light"|"SG_Mad"|"SG_Paralyzed"|"SG_Petrified"|"SG_Poisoned"|"SG_Poisoned_Story_NonRemovable"|"SG_Poisoned_Story_Removable"|"SG_Polymorph"|"SG_Polymorph_BeastShape"|"SG_Polymorph_BeastShape_NPC"|"SG_Possessed"|"SG_Prone"|"SG_Rage"|"SG_RemoveOnRespec"|"SG_Restrained"|"SG_ScriptedPeaceBehaviour"|"SG_Sleeping"|"SG_Stunned"|"SG_Surface"|"SG_Taunted"|"SG_Unconscious"|"SG_WeaponCoating"
--- @alias StatsTargetTypeFlags string|"Construct"|"Guaranteed"|"Living"|"Undead"
--- @alias StatsUnsheathState string|"Instrument"|"Melee"|"Ranged"|"Sheathed"
--- @alias StatsZoneShape string|"Cone"|"Square"
--- @alias StatusHealType string|"All"|"AllArmor"|"MagicArmor"|"None"|"PhysicalArmor"|"Source"|"Vitality"
--- @alias StatusMaterialApplyFlags string|"ApplyOnArmor"|"ApplyOnBody"|"ApplyOnHorns"|"ApplyOnOverhead"|"ApplyOnWeapon"|"ApplyOnWings"
--- @alias StatusType string|"BOOST"|"CLIMBING"|"DEACTIVATED"|"DOWNED"|"DYING"|"EFFECT"|"FEAR"|"HEAL"|"INCAPACITATED"|"INSURFACE"|"INVISIBLE"|"KNOCKED_DOWN"|"MATERIAL"|"POLYMORPHED"|"REACTION"|"ROTATE"|"SMELLY"|"SNEAKING"|"STORY_FROZEN"|"TELEPORT_FALLING"|"UNLOCK"
--- @alias SurfaceActionType string|"ChangeSurfaceOnPathAction"|"CreatePuddleAction"|"CreateSurfaceAction"|"ExtinguishFireAction"|"PolygonSurfaceAction"|"RectangleSurfaceAction"|"TransformSurfaceAction"|"ZoneAction"
--- @alias SurfaceChange string|"Condense"|"Deelectrify"|"DestroyWater"|"Douse"|"Electrify"|"Freeze"|"Ignite"|"Melt"|"None"|"Vaporize"
--- @alias SurfaceLayer string|"Cloud"|"Ground"|"None"
--- @alias SurfaceLayer8 string|"Cloud"|"Ground"|"None"
--- @alias SurfaceTransformActionType string|"Condense"|"Deelectrify"|"Douse"|"Electrify"|"Freeze"|"Ignite"|"Melt"|"None"|"Vaporize"
--- @alias SurfaceType string|"Acid"|"Alcohol"|"AlienOil"|"Ash"|"BlackPowder"|"BlackPowderDetonationCloud"|"BlackTentacles"|"BladeBarrier"|"Blood"|"BloodCloud"|"BloodElectrified"|"BloodExploding"|"BloodFrozen"|"BloodSilver"|"CausticBrine"|"Chasm"|"Cloudkill6Cloud"|"CloudkillCloud"|"CrawlerMucusCloud"|"DarknessCloud"|"Deepwater"|"DrowPoisonCloud"|"ExplosionCloud"|"Fire"|"FogCloud"|"GithPheromoneGasCloud"|"Grease"|"Hellfire"|"HolyFire"|"IceCloud"|"InvisibleGithAcid"|"InvisibleWeb"|"Lava"|"MaliceCloud"|"Mud"|"None"|"Oil"|"Overgrowth"|"Poison"|"PoisonCloud"|"PotionAntitoxinCloud"|"PotionHealingCloud"|"PotionHealingGreaterCloud"|"PotionHealingSuperiorCloud"|"PotionHealingSupremeCloud"|"PotionInvisibilityCloud"|"PotionResistanceAcidCloud"|"PotionResistanceColdCloud"|"PotionResistanceFireCloud"|"PotionResistanceForceCloud"|"PotionResistanceLightningCloud"|"PotionResistancePoisonCloud"|"PotionSpeedCloud"|"PotionVitalityCloud"|"PurpleWormPoison"|"Sentinel"|"SerpentVenom"|"Sewer"|"ShadowCursedVines"|"ShockwaveCloud"|"SpikeGrowth"|"SporeBlackCloud"|"SporeGreenCloud"|"SporePinkCloud"|"SporeWhiteCloud"|"StinkingCloud"|"TrialFire"|"Vines"|"VoidCloud"|"Water"|"WaterCloud"|"WaterCloudElectrified"|"WaterElectrified"|"WaterFrozen"|"Web"|"WyvernPoison"
--- @alias TravelErrorFlags string|"CannotFastTravel"|"Condition"|"DangerZone"|"Dialog"|"FTB"|"InDanger"
--- @alias TravelFlags string|"BlockCampTravel"|"BlockFastTravel"
--- @alias WeaponFlags string|"AddToHotbar"|"Ammunition"|"Dippable"|"Finesse"|"Heavy"|"Lance"|"Light"|"Loading"|"Magical"|"Melee"|"NeedDualWieldingBoost"|"Net"|"NoDualWield"|"NotSheathable"|"Range"|"Reach"|"Thrown"|"Torch"|"Twohanded"|"Unstowable"|"Versatile"
--- @alias WeaponType string|"Arrow"|"Axe"|"Bow"|"Club"|"Crossbow"|"Knife"|"None"|"Rifle"|"Sentinel"|"Spear"|"Staff"|"Sword"|"Wand"


--- @class AABound
--- @field Max vec3
--- @field Min vec3


--- @class ACOverrideFormulaBoostComponent:BaseComponent
--- @field AC int32
--- @field AddAbilityModifiers AbilityId[]
--- @field field_4 boolean


--- @class AIBound
--- @field AiType uint8
--- @field Height number
--- @field Max vec3
--- @field Min vec3
--- @field Radius number
--- @field Radius2 number
--- @field Type int32
--- @field field_28 uint8
--- @field field_2A uint8


--- @class AbilityBoostComponent:BaseComponent
--- @field Ability AbilityId
--- @field Value int32
--- @field field_8 int32
--- @field field_C int8


--- @class AbilityFailedSavingThrowBoostComponent:BaseComponent
--- @field Ability AbilityId


--- @class AbilityOverrideMinimumBoostComponent:BaseComponent
--- @field Ability AbilityId
--- @field Amount int32


--- @class ActionOriginator
--- @field ActionGuid Guid
--- @field CanApplyConcentration boolean
--- @field InterruptId FixedString
--- @field PassiveId FixedString
--- @field StatusId FixedString


--- @class ActionResourceBlockBoostComponent:BaseComponent
--- @field IntParam int32
--- @field ResourceUUID Guid


--- @class ActionResourceConsumeMultiplierBoostComponent:BaseComponent
--- @field Multiplier int32
--- @field ResourceUUID Guid
--- @field field_30 int64


--- @class ActionResourceMultiplierBoostComponent:BaseComponent
--- @field DiceSize DiceSizeId
--- @field IntParam int32
--- @field IntParam2 int32
--- @field ResourceUUID Guid


--- @class ActionResourcePreventReductionBoostComponent:BaseComponent
--- @field ActionResource Guid
--- @field Amount int32


--- @class ActionResourceReplenishTypeOverrideBoostComponent:BaseComponent
--- @field ActionResource Guid
--- @field ReplenishType ResourceReplenishType


--- @class ActionResourceValueBoostComponent:BaseComponent
--- @field Amount number
--- @field Amount2 int32
--- @field DiceSize DiceSizeId
--- @field ResourceUUID Guid


--- @class ActionResourcesComponent:BaseComponent
--- @field Resources table<Guid, ActionResourcesComponentAmount[]>


--- @class ActionResourcesComponentAmount
--- @field Amount number
--- @field MaxAmount number
--- @field ResourceId int32
--- @field ResourceUUID Guid
--- @field SubAmounts ActionResourcesComponentAmountSubAmount[]?
--- @field field_28 uint64
--- @field field_30 uint64


--- @class ActionResourcesComponentAmountSubAmount
--- @field Amount number
--- @field MaxAmount number


--- @class ActionUseConditionsComponent:BaseComponent
--- @field Conditions int32[]


--- @class ActiveCharacterLightBoostComponent:BaseComponent
--- @field LightUUID FixedString


--- @class ActiveCharacterLightComponent:BaseComponent
--- @field Light FixedString


--- @class ActiveComponent:BaseComponent


--- @class ActiveMaterial
--- @field BlendFlags uint8
--- @field BlendStateID uint8
--- @field Flags uint8
--- @field Hash uint64
--- @field MaterialInstance MaterialInstance
--- @field MaterialName FixedString
--- @field RasterizerStateID uint8
--- @field RenderableObject RenderableObject
--- @field Texture2DParams ActiveMaterialTexture2DParam[]
--- @field UVCount_M uint8
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
--- @field AdvantageType AdvantageTypeId
--- @field Skill SkillId
--- @field Tags Guid[]
--- @field Type AdvantageBoostType


--- @class AiArchetypeOverrideBoostComponent:BaseComponent
--- @field Archetype FixedString
--- @field Priority int32


--- @class AmbushingComponent:BaseComponent


--- @class AnimationTag
--- @field Tag Guid
--- @field field_10 uint8


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


--- @class AppearanceOverrideComponent:BaseComponent
--- @field Visual GameObjectVisualData


--- @class AppliedMaterial:ActiveMaterial
--- @field DynamicParameter vec4
--- @field MeshVertexColor vec4


--- @class ApprovalRatingsComponent:BaseComponent
--- @field Ratings table<EntityHandle, int32>
--- @field field_70 Array_Guid


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
--- @field Shield uint8


--- @class ArmorSetStateComponent:BaseComponent
--- @field State uint8


--- @class AttackSpellOverrideBoostComponent:BaseComponent
--- @field AttackType SpellAttackTypeOverride
--- @field SpellId FixedString


--- @class AttitudesToPlayersComponent:BaseComponent
--- @field Attitudes table<AttitudesToPlayersComponentKey, int32>


--- @class AttitudesToPlayersComponentKey
--- @field field_0 EntityHandle
--- @field field_10 Guid
--- @field field_20 uint8
--- @field field_8 uint8


--- @class AttributeBoostComponent:BaseComponent
--- @field AttributeFlags AttributeFlags


--- @class AttributeFlagsComponent:BaseComponent
--- @field AttributeFlags uint32


--- @class AvailableLevelComponent:BaseComponent
--- @field Level int32


--- @class AvatarComponent:BaseComponent


--- @class BackgroundComponent:BaseComponent
--- @field Background Guid


--- @class BackgroundPassivesComponent:BaseComponent
--- @field field_18 PassiveInfo[]


--- @class BackgroundTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class BaseComponent


--- @class BaseHpComponent:BaseComponent
--- @field Vitality int32
--- @field VitalityBoost int32


--- @class BaseProxyComponent


--- @class BaseStatsComponent:BaseComponent
--- @field BaseAbilities int32[]


--- @class BlockAbilityModifierFromACComponent:BaseComponent
--- @field Ability AbilityId


--- @class BlockRegainHPBoostComponent:BaseComponent


--- @class BodyTypeComponent:BaseComponent
--- @field BodyType uint8
--- @field BodyType2 uint8


--- @class BoostCause
--- @field Cause FixedString
--- @field Entity EntityHandle
--- @field Type BoostCauseType
--- @field field_10 uint64


--- @class BoostConditionComponent:BaseComponent
--- @field ConditionFlags int32
--- @field field_1C uint8


--- @class BoostInfoComponent:BaseComponent
--- @field Cause BoostCause
--- @field Owner EntityHandle
--- @field Params BoostParameters
--- @field field_10 Guid
--- @field field_20 uint8
--- @field field_80 Guid


--- @class BoostParameters
--- @field Boost FixedString
--- @field Params FixedString
--- @field Params2 FixedString


--- @class BoostsContainerComponent:BaseComponent
--- @field Boosts BoostsContainerComponentBoostTypeInfo[]


--- @class BoostsContainerComponentBoostTypeInfo
--- @field Boosts EntityHandle[]
--- @field Type BoostType


--- @class Bound
--- @field AIBounds table<uint16, AIBound>
--- @field Entity EntityHandle
--- @field Flags uint8
--- @field Flags2 uint16
--- @field RotationQuat quat
--- @field Scale number
--- @field SomeRefCount uint16
--- @field Translate vec3
--- @field TranslateOverride vec3
--- @field field_28 Array_uint32
--- @field field_78 int16
--- @field field_89 uint8
--- @field field_8C FixedString


--- @class BoundComponent:BaseComponent
--- @field Bound Bound


--- @class CanBeDisarmedComponent:BaseComponent
--- @field Flags uint16


--- @class CanBeLootedComponent:BaseComponent
--- @field Flags uint16


--- @class CanBeWieldedComponent:BaseComponent


--- @class CanDeflectProjectilesComponent:BaseComponent
--- @field Flags uint16


--- @class CanDoActionsComponent:BaseComponent
--- @field Flags uint16


--- @class CanDoRestComponent:BaseComponent
--- @field Flags RestFlags
--- @field RestErrorFlags1 RestErrorFlags
--- @field RestErrorFlags2 RestErrorFlags


--- @class CanEnterChasmComponent:BaseComponent
--- @field CanEnter boolean


--- @class CanInteractComponent:BaseComponent
--- @field Flags uint16


--- @class CanModifyHealthComponent:BaseComponent
--- @field Flags uint16


--- @class CanMoveComponent:BaseComponent
--- @field Flags uint16


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
--- @field Elements GameObjectVisualDataAppearanceElement[]
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
--- @field IsTradable uint8
--- @field IsWorldClimbingEnabled boolean
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
--- @field OnlyInDifficulty Guid[]
--- @field PickingPhysicsTemplates table<FixedString, FixedString>
--- @field ProbeLookAtOffset number
--- @field ProbeSpineAOffset number
--- @field ProbeSpineBOffset number
--- @field ProbeTiltToOffset number
--- @field Race Guid
--- @field RagdollTemplate FixedString
--- @field ShootThroughType uint8
--- @field SoftBodyCollisionTemplate FixedString
--- @field SoundAttenuation int16
--- @field SoundInitEvent FixedString
--- @field SoundMovementStartEvent FixedString
--- @field SoundMovementStopEvent FixedString
--- @field SoundObjectIndex int8
--- @field SpeakerGroupList Array_Guid
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


--- @class ClassTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class ClassesComponent:BaseComponent
--- @field Classes ClassesComponentClass[]


--- @class ClassesComponentClass
--- @field ClassUUID Guid
--- @field Level int32
--- @field SubClassUUID Guid


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


--- @class ConcentrationComponent:BaseComponent
--- @field Caster EntityHandle
--- @field SpellId SpellId
--- @field Targets ConcentrationComponentTargetInfo[]
--- @field field_0 EntityHandle
--- @field field_8 ConcentrationComponentTargetInfo[]


--- @class ConcentrationComponentTargetInfo
--- @field Concentration EntityHandle
--- @field Status EntityHandle
--- @field Target EntityHandle
--- @field field_0 EntityHandle
--- @field field_10 EntityHandle
--- @field field_18 int16
--- @field field_1A boolean
--- @field field_8 EntityHandle


--- @class ConcentrationIgnoreDamageBoostComponent:BaseComponent
--- @field SpellSchool SpellSchoolId


--- @class ConditionRoll
--- @field Ability AbilityId
--- @field AbilityCheckRoll boolean
--- @field AttackRoll boolean
--- @field DataType uint8
--- @field Difficulty int32
--- @field DifficultyRoll boolean
--- @field Roll Variant<StatsRollType0,StatsRollType1>
--- @field RollType ConditionRollType
--- @field SavingThrowRoll boolean
--- @field Skill SkillId
--- @field SkillCheckRoll boolean
--- @field field_120 Guid
--- @field field_130 boolean


--- @class CriticalHitBoostComponent:BaseComponent
--- @field Flags CriticalHitBoostFlags
--- @field Value int32
--- @field field_1 uint8


--- @class CriticalHitExtraDiceBoostComponent:BaseComponent
--- @field Amount uint8
--- @field AttackType SpellAttackType


--- @class CustomIconComponent:BaseComponent


--- @class CustomNameComponent:BaseComponent
--- @field Name string


--- @class CustomStatsComponent:BaseComponent
--- @field Stats table<FixedString, int32>


--- @class DamageBonusBoostComponent:BaseComponent
--- @field Amount StatsExpressionParam
--- @field DamageType DamageType
--- @field field_31 uint8


--- @class DamageModifierMetadata
--- @field Argument Variant<DiceValues,int32,StatsRollType1>
--- @field DamageType DamageType
--- @field Description TranslatedString
--- @field Description2 FixedString
--- @field MetadataType uint8
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
--- @field Flags uint32
--- @field Meta DamageModifierMetadata


--- @class DamageSums
--- @field DamagePercentage int8
--- @field TotalDamageDone int32
--- @field TotalHealDone int32
--- @field field_9 int8
--- @field field_A int8


--- @class DarknessComponent:BaseComponent
--- @field Obscurity uint8
--- @field Sneaking uint8
--- @field field_2 uint8
--- @field field_3 uint8
--- @field field_4 uint8
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


--- @class DeadByDefaultComponent:BaseComponent
--- @field DeadByDefault boolean


--- @class DeathComponent:BaseComponent
--- @field DeathType uint8
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
--- @field field_50 Guid
--- @field field_60 uint8
--- @field field_61 uint8
--- @field field_62 uint8
--- @field field_64 int32
--- @field field_68 int64
--- @field field_70 int64
--- @field field_8 EntityHandle


--- @class DeathStateComponent:BaseComponent
--- @field State uint32


--- @class DetachedComponent:BaseComponent
--- @field Flags uint32


--- @class DetectCrimesBlockBoostComponent:BaseComponent
--- @field field_0 boolean


--- @class DialogStateComponent:BaseComponent
--- @field field_0 uint8
--- @field field_1 uint8
--- @field field_2 uint8
--- @field field_4 int32
--- @field field_8 uint8


--- @class DiceValues
--- @field AmountOfDices uint8
--- @field DiceAdditionalValue int32
--- @field DiceValue DiceSizeId
--- @field field_8 uint8


--- @class DifficultyCheckComponent:BaseComponent
--- @field SpellDC int32
--- @field field_4 int32
--- @field field_8 int32


--- @class DisabledEquipmentComponent:BaseComponent
--- @field ShapeshiftFlag boolean


--- @class DisarmableComponent:BaseComponent
--- @field field_0 Guid
--- @field field_10 uint8
--- @field field_11 uint8


--- @class DisplayNameComponent:BaseComponent
--- @field NameKey TranslatedString
--- @field UnknownKey TranslatedString


--- @class DodgeAttackRollBoostComponent:BaseComponent
--- @field StatusGroup StatsStatusGroup
--- @field StatusType StatusType
--- @field field_0 uint8
--- @field field_4 int32


--- @class DownedStatusBoostComponent:BaseComponent
--- @field StatusId FixedString
--- @field field_4 int32


--- @class DualWieldingBoostComponent:BaseComponent
--- @field DualWielding boolean


--- @class DualWieldingComponent:BaseComponent
--- @field DisableDualWielding boolean
--- @field Melee boolean
--- @field MeleeUI boolean
--- @field Ranged boolean
--- @field RangedUI boolean
--- @field field_1A uint8
--- @field field_1D uint8


--- @class DynamicAnimationTagsComponent:BaseComponent
--- @field Tags AnimationTag[]


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
--- @field Roll DiceValues
--- @field field_C uint8


--- @class EoCGameObjectTemplate:GameObjectTemplate
--- @field AIBounds AIBound[]
--- @field CollideWithCamera boolean
--- @field DisplayName TranslatedString
--- @field FadeChildren FixedString[]
--- @field FadeGroup FixedString
--- @field Fadeable boolean
--- @field HierarchyOnlyFade boolean
--- @field SeeThrough boolean


--- @class EocLevelComponent:BaseComponent
--- @field Level int32


--- @class EquipableComponent:BaseComponent
--- @field EquipmentTypeID Guid
--- @field Slot ItemSlot
--- @field field_18 Guid


--- @class EquipmentVisualComponent:BaseComponent
--- @field State uint8


--- @class ExperienceComponent:BaseComponent
--- @field CurrentLevelExperience int32
--- @field NextLevelExperience int32
--- @field SomeExperience int32
--- @field TotalExperience int32
--- @field field_28 uint8


--- @class ExpertiseBonusBoostComponent:BaseComponent
--- @field Skill SkillId


--- @class ExpertiseComponent:BaseComponent
--- @field Expertise Array_SkillId


--- @class FactionComponent:BaseComponent
--- @field SummonOwner EntityHandle
--- @field field_0 EntityHandle
--- @field field_18 Guid
--- @field field_28 EntityHandle
--- @field field_8 Guid


--- @class FactionOverrideBoostComponent:BaseComponent
--- @field Faction Guid
--- @field field_10 uint8


--- @class FallDamageMultiplierBoostComponent:BaseComponent
--- @field Amount number


--- @class FleeCapabilityComponent:BaseComponent
--- @field CurrentFleeDistance number
--- @field Flags FleeErrorFlags
--- @field FleeDistance number
--- @field field_1C number
--- @field field_20 number


--- @class FloatingComponent:BaseComponent
--- @field field_18 int32
--- @field field_1C int32


--- @class GameObjectTemplate
--- @field AllowReceiveDecalWhenAnimated boolean
--- @field CameraOffset vec3
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
--- @field PhysicsOpenTemplate FixedString
--- @field PhysicsTemplate FixedString
--- @field ReceiveDecal boolean
--- @field RenderChannel uint8
--- @field TemplateName FixedString
--- @field TemplateType FixedString
--- @field Transform Transform
--- @field VisualTemplate FixedString
--- @field field_8 uint64


--- @class GameObjectVisualComponent:BaseComponent
--- @field Icon FixedString
--- @field RootTemplateId FixedString
--- @field RootTemplateType uint8
--- @field Scale number
--- @field Type uint8


--- @class GameObjectVisualData
--- @field AccessorySet Guid
--- @field AdditionalChoices number[]
--- @field Elements GameObjectVisualDataAppearanceElement[]
--- @field EyeColor Guid
--- @field HairColor Guid
--- @field SecondEyeColor Guid
--- @field SkinColor Guid
--- @field Visuals Guid[]
--- @field field_98 Guid


--- @class GameObjectVisualDataAppearanceElement
--- @field Color Guid
--- @field ColorIntensity number
--- @field GlossyTint number
--- @field Material Guid
--- @field MetallicTint uint32


--- @class GameTime
--- @field DeltaTime number
--- @field Ticks int32
--- @field Time number
--- @field Unknown number


--- @class GameplayLightBoostComponent:BaseComponent
--- @field field_0 int32
--- @field field_4 boolean
--- @field field_8 int32
--- @field field_C uint8


--- @class GameplayLightComponent:BaseComponent
--- @field field_18 int32
--- @field field_1C uint8
--- @field field_20 int32
--- @field field_24 int32
--- @field field_28 int32


--- @class GameplayObscurityBoostComponent:BaseComponent
--- @field Obscurity number


--- @class GenericPropertyTag
--- @field Entity Guid
--- @field Type uint8


--- @class GlobalLongRestDisabledComponent:BaseComponent


--- @class GlobalShortRestDisabledComponent:BaseComponent


--- @class GlobalSwitches
--- @field AlwaysShowSplitter boolean
--- @field AnimationBlueprintSpeedScale_M number
--- @field AutoListenEnabled boolean
--- @field CameraShakeEnabled boolean
--- @field CanAutoSave boolean
--- @field CanCrossSave boolean
--- @field CanLoadDuringTimeline boolean
--- @field CanSaveDuringTimeline boolean
--- @field CentreDialogue boolean
--- @field CheckRequirements boolean
--- @field CombatTimelines boolean
--- @field ControllerCharacterRunThreshold number
--- @field ControllerCharacterWalkThreshold number
--- @field ControllerLayout boolean
--- @field ControllerMode uint8
--- @field ControllerRumbleStrength int32
--- @field ControllerSensitivity int32
--- @field ControllerStickDeadZone int32
--- @field ControllerStickPressDeadZone int32
--- @field ControllerTriggerDeadZone int32
--- @field DialogueFontSizeModifier int32
--- @field DisableArmorSavingThrows boolean
--- @field DisableEdgePanning boolean
--- @field DisableStoryPatching boolean
--- @field DisplaySpeaker boolean
--- @field EnableLongPressAndHold boolean
--- @field EnablePortmapping boolean
--- @field EnableVoiceLogging boolean
--- @field EscClosesAllUI boolean
--- @field ExtraTooltipDescriptions boolean
--- @field FirstTimeWithCrossSave boolean
--- @field ForceMono boolean
--- @field ForcePort int16
--- @field ForceSplitscreen boolean
--- @field ForceStoryPatching boolean
--- @field GameCameraAutoRotateEnabled boolean
--- @field GameCameraAutoRotateSpeed number
--- @field GameCameraEnableAttackCameraOtherPlayers boolean
--- @field GameCameraEnableCloseUpDialog boolean
--- @field GameCameraEnableDynamicCombatCamera boolean
--- @field GameCameraEnableFlyFollow boolean
--- @field GameCameraRotation int32
--- @field GameCameraRotationLocked boolean
--- @field GameCameraSpeedMultiplier number
--- @field GameVisibilityDirect uint32
--- @field GameVisibilityLAN uint32
--- @field GameVisibilityOnline uint32
--- @field HasSeenEAMsg boolean
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
--- @field NodeWaitTimeMultiplier number
--- @field NorthFacingMinimap boolean
--- @field NrOfAutoSaves int32
--- @field NrOfQuickSaves int32
--- @field OverheadFontSizeModifier int32
--- @field RandomStartOrigin_M boolean
--- @field ResetTutorialsOnNewGame boolean
--- @field ScriptLog boolean
--- @field ShowCharacterCreation boolean
--- @field ShowLocalizationMarkers uint8
--- @field ShowOriginIntroInCC boolean
--- @field ShowSubtitles boolean
--- @field ShowTextBackground boolean
--- @field ShroudEnabled boolean
--- @field SoundOcclusionQuality int32
--- @field SoundPartyLosingThreshold int32
--- @field SoundPartyWinningThreshold int32
--- @field SoundVolumeDynamicRange int32
--- @field Stats boolean
--- @field Story boolean
--- @field StoryEvents boolean
--- @field StoryLog boolean
--- @field TextBackgroundOpacity int32
--- @field TwitchEnableOverlay boolean
--- @field UIScaling int32
--- @field UseEndTurnFallback boolean
--- @field UseLevelCache boolean
--- @field UseRadialContextMenu boolean


--- @class GodComponent:BaseComponent
--- @field God Guid


--- @class GodTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class GravityDisabledComponent:BaseComponent


--- @class GravityDisabledUntilMovedComponent:BaseComponent


--- @class GuaranteedChanceRollOutcomeBoostComponent:BaseComponent
--- @field field_0 boolean


--- @class HalveWeaponDamageBoostComponent:BaseComponent
--- @field Ability AbilityId


--- @class HasExclamationDialogComponent:BaseComponent


--- @class HealthComponent:BaseComponent
--- @field Hp int32
--- @field MaxHp int32
--- @field MaxTemporaryHp int32
--- @field TemporaryHp int32
--- @field field_10 Guid
--- @field field_20 uint8


--- @class HearingComponent:BaseComponent
--- @field Hearing number


--- @class Hit
--- @field ArmorAbsorption int32
--- @field AttackFlags uint8
--- @field AttackRollAbility AbilityId
--- @field CauseType CauseType
--- @field ConditionRolls ConditionRoll[]
--- @field DamageList DamagePair[]
--- @field DamageType DamageType
--- @field DeathType StatsDeathType
--- @field EffectFlags uint32
--- @field HitDescFlags uint8
--- @field HitWith HitWith
--- @field ImpactDirection vec3
--- @field ImpactForce number
--- @field ImpactPosition vec3
--- @field Inflicter EntityHandle
--- @field InflicterOwner EntityHandle
--- @field LifeSteal int32
--- @field OriginalDamageValue int32
--- @field Results HitResultMetadata
--- @field SaveAbility AbilityId
--- @field SpellCastGuid Guid
--- @field SpellId FixedString
--- @field SpellLevel int32
--- @field SpellPowerLevel int32
--- @field SpellSchool SpellSchoolId
--- @field StoryActionId int32
--- @field Throwing EntityHandle
--- @field TotalDamageDone int32
--- @field TotalHealDone int32
--- @field field_150 FixedString
--- @field field_158 EntityHandle
--- @field field_160 uint8
--- @field field_174 int32
--- @field field_178 int32
--- @field field_17C int32
--- @field field_180 int32
--- @field field_184 int32
--- @field field_188 uint8
--- @field field_190 vec3[]
--- @field field_4F uint8


--- @class HitResultMetadata
--- @field AdditionalDamage int32
--- @field ConditionRoll StatsRollType1
--- @field DamageRolls table<DamageType, StatsRollType0[]>
--- @field FinalDamage int32
--- @field FinalDamagePerType table<DamageType, int32>
--- @field Modifiers DamageModifierMetadata[]
--- @field Modifiers2 DamageModifierMetadata[]
--- @field Resistances DamageResistance[]
--- @field TotalDamage int32
--- @field TotalDamagePerType table<DamageType, int32>
--- @field field_D0 uint32
--- @field field_D4 uint32
--- @field field_D8 uint32
--- @field field_DC uint8


--- @class HitResult
--- @field DamageList DamagePair[]
--- @field DamageSums DamageSums
--- @field Hit Hit
--- @field NumConditionRolls uint32
--- @field Results HitResultData


--- @class HitResultData
--- @field field_0 int32[]
--- @field field_10 int32[]
--- @field field_20 uint8[]
--- @field field_30 int32[]


--- @class HorizontalFOVOverrideBoostComponent:BaseComponent
--- @field FOV number


--- @class HotbarContainerComponent:BaseComponent
--- @field ActiveContainer FixedString
--- @field Containers table<FixedString, HotbarContainerComponentBar[]>


--- @class HotbarContainerComponentBar
--- @field Elements HotbarContainerComponentElement[]
--- @field Height uint32
--- @field Index uint8
--- @field Width uint8
--- @field field_1 uint8
--- @field field_20 string


--- @class HotbarContainerComponentElement
--- @field IsNew boolean
--- @field Item EntityHandle
--- @field Passive FixedString
--- @field Slot uint32
--- @field SpellId SpellId


--- @class HotbarCurrentDecksComponent:BaseComponent
--- @field Decks table<FixedString, int32>


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
--- @field SurfaceType SurfaceType


--- @alias ImguiHandle ExtuiStyledRenderable



--- @class IncreaseMaxHPComponent:BaseComponent
--- @field Amount Variant<int32,StatsExpressionParam>
--- @field field_30 int32


--- @class InitiativeBoostComponent:BaseComponent
--- @field Amount int32


--- @class InteractionFilterComponent:BaseComponent
--- @field field_0 Array_Guid
--- @field field_30 uint8
--- @field field_31 uint8


--- @class InvisibilityComponent:BaseComponent
--- @field field_0 uint8
--- @field field_10 uint8
--- @field field_4 vec3


--- @class IsGlobalComponent:BaseComponent


--- @class IsInTurnBasedModeComponent:BaseComponent


--- @class IsSummonComponent:BaseComponent
--- @field Owner_M EntityHandle


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
--- @field Equipment ItemTemplateEquipmentData
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
--- @field InteractionFilterList Array_Guid
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
--- @field Key FixedString
--- @field LevelOverride int32
--- @field LightChannel uint8
--- @field LockDifficultyClassID Guid
--- @field MapMarkerStyle FixedString
--- @field MaterialPreset Guid
--- @field MaxStackAmount int32
--- @field OnUseDescription TranslatedString
--- @field OnlyInDifficulty Guid[]
--- @field Owner FixedString
--- @field PermanentWarnings FixedString
--- @field PhysicsCollisionSound FixedString
--- @field PhysicsFollowAnimation boolean
--- @field PickupSound FixedString
--- @field ShortDescription TranslatedString
--- @field ShortDescriptionParams string
--- @field ShowAttachedSpellDescriptions boolean
--- @field SpeakerGroups Array_Guid
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


--- @class ItemTemplateEquipmentData
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
--- @field SyncWithParent Array_Guid
--- @field VisualSet ResourceVisualSet
--- @field Visuals table<Guid, FixedString[]>
--- @field WavyLongHair table<Guid, FixedString>
--- @field WavyShortHair table<Guid, FixedString>


--- @class JumpMaxDistanceBonusBoostComponent:BaseComponent
--- @field DistanceBonus number


--- @class JumpMaxDistanceMultiplierBoostComponent:BaseComponent
--- @field Amount number


--- @class KeyComponent:BaseComponent
--- @field Key FixedString


--- @class LevelComponent:BaseComponent
--- @field LevelName FixedString
--- @field field_0 EntityHandle


--- @class LevelUpData
--- @field Abilities int32[]
--- @field AccessorySet Guid
--- @field Class Guid
--- @field Feat Guid
--- @field Spells SpellIdBase[]
--- @field SubClass Guid
--- @field Upgrades LevelUpUpgrades
--- @field field_B0 SpellIdBase[]


--- @class LevelUpUpgrades
--- @field AbilityBonuses LevelUpUpgradesAbilityBonusData[]
--- @field Equipment LevelUpUpgradesEquipmentData[]
--- @field Feats LevelUpUpgradesFeatData[]
--- @field Passives LevelUpUpgradesPassiveData[]
--- @field SkillBonuses LevelUpUpgradesSkillBonusData[]
--- @field Skills LevelUpUpgradesSkillData[]
--- @field Spells LevelUpUpgradesSpellData[]
--- @field Spells2 LevelUpUpgradesPassiveData[]
--- @field Unknowns2 LevelUpUpgradesSkillBonusData[]
--- @field Unknowns4 LevelUpUpgradesEquipmentData[]


--- @class LevelUpUpgradesAbilityBonusData:LevelUpUpgradesReference
--- @field AbilityBonus Guid
--- @field Array_b8 AbilityId[]
--- @field Array_i32 uint32[]
--- @field BonusAmounts uint32[]
--- @field BonusType string
--- @field Bonuses AbilityId[]
--- @field field_30 int32
--- @field field_60 string
--- @field field_80 int32


--- @class LevelUpUpgradesEquipmentData:LevelUpUpgradesReference
--- @field Array_FS FixedString[]
--- @field Equipment FixedString[]
--- @field EquipmentList Guid
--- @field SelectorId string
--- @field field_30 int32
--- @field field_38 Guid
--- @field field_60 string
--- @field field_80 int32


--- @class LevelUpUpgradesFeatData:LevelUpUpgradesReference
--- @field Array_b8 uint8[]
--- @field Feat Guid
--- @field FeatName string
--- @field field_30 int32
--- @field field_80 int32


--- @class LevelUpUpgradesPassiveData:LevelUpUpgradesReference
--- @field Array_FS2 LevelUpUpgradesPassiveDataStringPair[]
--- @field PassiveList Guid
--- @field Passives FixedString[]
--- @field ReplacePassives LevelUpUpgradesPassiveDataStringPair[]
--- @field SelectorId string
--- @field SpellList Guid
--- @field Spells FixedString[]
--- @field field_30 int32
--- @field field_78 string
--- @field field_80 int32


--- @class LevelUpUpgradesPassiveDataStringPair
--- @field From FixedString
--- @field To FixedString


--- @class LevelUpUpgradesReference
--- @field Class Guid
--- @field Level int32
--- @field Multiclass boolean
--- @field Subclass Guid
--- @field Type uint8
--- @field field_0 uint8
--- @field field_28 int32
--- @field field_2C boolean


--- @class LevelUpUpgradesSkillBonusData:LevelUpUpgradesReference
--- @field Array_b8 SkillId[]
--- @field Expertise SkillId[]
--- @field Skill Guid
--- @field field_30 int32
--- @field field_38 Guid
--- @field field_48 uint8
--- @field field_60 string
--- @field field_80 int32


--- @class LevelUpUpgradesSkillData:LevelUpUpgradesReference
--- @field Array_b8 SkillId[]
--- @field Proficiencies SkillId[]
--- @field Skill Guid
--- @field field_30 int32
--- @field field_60 string
--- @field field_80 int32


--- @class LevelUpUpgradesSpellData:LevelUpUpgradesReference
--- @field Array_FS2 LevelUpUpgradesSpellDataStringPair[]
--- @field ReplaceSpells LevelUpUpgradesSpellDataStringPair[]
--- @field SelectorId string
--- @field SpellList Guid
--- @field Spells FixedString[]
--- @field field_30 int32
--- @field field_78 string


--- @class LevelUpUpgradesSpellDataStringPair
--- @field From FixedString
--- @field To FixedString


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


--- @class LootComponent:BaseComponent
--- @field Flags uint8
--- @field InventoryType uint8


--- @class LootingStateComponent:BaseComponent
--- @field Looter_M EntityHandle
--- @field State uint8
--- @field field_24 int32


--- @class MaterialInstance
--- @field BlendStateID uint8
--- @field DepthStateID uint8
--- @field DiffusionProfileUUID FixedString
--- @field Flags uint32
--- @field ForwardLightingMode uint32
--- @field MaterialPassHint uint8
--- @field MaterialType uint8
--- @field MaterialUsage uint8
--- @field Name FixedString
--- @field Parameters MaterialInstanceParametersSet
--- @field Parent MaterialInstance
--- @field RasterizerStateID uint8
--- @field RenderChannel uint8
--- @field Shaders FixedString[]
--- @field ShadingModel uint32
--- @field UVCount uint32
--- @field UsedWithFlags uint32
--- @field Version64 uint64
--- @field field_510 uint32
--- @field SetScalar fun(self:MaterialInstance, a1:FixedString, a2:number):boolean
--- @field SetVector2 fun(self:MaterialInstance, a1:FixedString, a2:vec2):boolean
--- @field SetVector3 fun(self:MaterialInstance, a1:FixedString, a2:vec3):boolean
--- @field SetVector4 fun(self:MaterialInstance, a1:FixedString, a2:vec4):boolean


--- @class MaterialInstanceDecalParameters
--- @field DecalDeferredProperties int16
--- @field DecalDimensions int16
--- @field DecalTiling int16
--- @field DecalWorld int16
--- @field InvWorldMatrix int16
--- @field NormalMatrix int16


--- @class MaterialInstanceMaterialCB
--- @field MaterialCBSize uint64


--- @class MaterialInstanceParametersSet
--- @field Material MaterialInstance
--- @field SamplerStateParameters MaterialInstanceSamplerStateParameter[]
--- @field ScalarParameters MaterialInstanceScalarParameter[]
--- @field Texture2DParameters MaterialInstanceTexture2DParameter[]
--- @field Vector2Parameters MaterialInstanceVector2Parameter[]
--- @field Vector3Parameters MaterialInstanceVector3Parameter[]
--- @field VectorParameters MaterialInstanceVector4Parameter[]
--- @field VirtualTextureParameters MaterialInstanceVirtualTextureParameter[]


--- @class MaterialInstanceSamplerStateParameter:ResourceMaterialResourceParameter
--- @field TextureAddressMode uint8
--- @field TextureFilterOverride uint8


--- @class MaterialInstanceScalarParameter:ResourceMaterialResourceScalarParameter


--- @class MaterialInstanceShaderDescription
--- @field DecalParameters MaterialInstanceDecalParameters
--- @field DiffusionProfileIndex int16
--- @field EngineCBBinding MaterialInstanceShaderParamBinding
--- @field EngineCBSize uint16
--- @field EngineParamFlags uint16
--- @field FadeOpacity int16
--- @field LightChannel int16
--- @field LocalBoundsMax int16
--- @field LocalBoundsMin int16
--- @field MaterialCBBinding MaterialInstanceShaderParamBinding
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


--- @class MaterialInstanceShaderParamBinding
--- @field DxPsIndex int8
--- @field DxVsIndex int8
--- @field VkBindingIndex int8
--- @field VkDescriptorSet int8


--- @class MaterialInstanceTexture2DParameter:ResourceMaterialResourceTexture2DParameter


--- @class MaterialInstanceTextureBindingData
--- @field PerShaderBindings MaterialInstanceShaderParamBinding[]
--- @field UniformName FixedString


--- @class MaterialInstanceUniformBindingData
--- @field PerShaderCBOffsets int16[]
--- @field UniformName FixedString


--- @class MaterialInstanceVector2Parameter:ResourceMaterialResourceVector2Parameter


--- @class MaterialInstanceVector3Parameter:ResourceMaterialResourceVector3Parameter


--- @class MaterialInstanceVector4Parameter:ResourceMaterialResourceVector4Parameter


--- @class MaterialInstanceVirtualTextureBindingData
--- @field PerShaderBindings MaterialInstanceShaderParamBinding[]
--- @field PerShaderVTBindings MaterialInstanceVirtualTextureShaderParamBinding[]
--- @field UniformName FixedString


--- @class MaterialInstanceVirtualTextureParameter:ResourceMaterialResourceVirtualTextureParameter


--- @class MaterialInstanceVirtualTextureShaderParamBinding
--- @field Cache MaterialInstanceShaderParamBinding
--- @field Cache1 MaterialInstanceShaderParamBinding
--- @field Cache2 MaterialInstanceShaderParamBinding
--- @field Cache3 MaterialInstanceShaderParamBinding


--- @class MaterialParameterOverrideComponent:BaseComponent
--- @field field_0 Guid[]
--- @field field_10 MaterialParameterOverrideComponentParam[]


--- @class MaterialParameterOverrideComponentParam
--- @field field_0 string
--- @field field_18 FixedString


--- @class MaterialParameterSet
--- @field EngineCBSize uint16
--- @field EngineDxPsIndex uint8
--- @field EngineDxVsIndex uint8
--- @field EngineVkBindingIndex uint8
--- @field EngineVkDescriptorSet uint8
--- @field Handle EntityHandle
--- @field MaterialCBSize uint16
--- @field MaterialDxPsIndex uint8
--- @field MaterialDxVsIndex uint8
--- @field MaterialHash uint64
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


--- @class MaximizeHealingBoostComponent:BaseComponent
--- @field Direction HealDirection
--- @field TargetTypes StatsTargetTypeFlags


--- @class MaximumRollResultBoostComponent:BaseComponent
--- @field Result int8
--- @field RollType StatsRollType


--- @class MinimumRollResultBoostComponent:BaseComponent
--- @field Result int8
--- @field RollType StatsRollType


--- @class ModManager
--- @field AvailableMods Module[]
--- @field BaseModule Module
--- @field Flag uint8
--- @field Settings ModuleSettings
--- @field Unknown uint64


--- @class Module
--- @field AddonModules Module[]
--- @field ContainedModules Module[]
--- @field DependentModules Module[]
--- @field FinishedLoading boolean
--- @field HasValidHash boolean
--- @field Info ModuleInfo
--- @field LoadOrderedModules Module[]
--- @field UsesLsfFormat boolean


--- @class ModuleInfo
--- @field Author string
--- @field CharacterCreationLevelName FixedString
--- @field Description string
--- @field Directory string
--- @field Hash string
--- @field LobbyLevelName FixedString
--- @field MainMenuBackgroundVideo FixedString
--- @field MenuLevelName FixedString
--- @field ModVersion Version
--- @field ModuleType FixedString
--- @field ModuleUUID Guid
--- @field ModuleUUIDString FixedString
--- @field Name string
--- @field NumPlayers uint8
--- @field PhotoBoothLevelName FixedString
--- @field PublishVersion Version
--- @field StartLevelName FixedString
--- @field Tags string[]
--- @field TargetModes FixedString[]


--- @class ModuleSettings
--- @field ModOrder FixedString[]
--- @field Mods ModuleShortDesc[]


--- @class ModuleShortDesc
--- @field Folder string
--- @field Hash string
--- @field ModVersion Version
--- @field ModuleUUID FixedString
--- @field Name string
--- @field PublishVersion Version


--- @class MonkWeaponDamageDiceOverrideBoostComponent:BaseComponent
--- @field DamageDice FixedString


--- @class MoveableObject
--- @field BaseBound AABound
--- @field DirtyFlags uint8
--- @field LocalTransform LocalTransform
--- @field SceneNodeIndex_M int32
--- @field SceneSize_M vec2
--- @field WorldBound AABound
--- @field WorldTransform Transform


--- @class MovementComponent:BaseComponent
--- @field Acceleration number
--- @field Direction vec3
--- @field Speed number
--- @field Speed2 number


--- @class MovementSpeedLimitBoostComponent:BaseComponent
--- @field MovementType uint8


--- @class NoesisBaseCommand:NoesisBaseComponent
--- @field CanExecute fun(self:NoesisBaseCommand, a1:NoesisBaseCommand, a2:NoesisBaseComponent?):boolean
--- @field Execute fun(self:NoesisBaseCommand, a1:NoesisBaseCommand, a2:NoesisBaseComponent?)


--- @class NoesisBaseComponent:NoesisBaseRefCounted


--- @class NoesisBaseObject
--- @field Type Noesis::Symbol
--- @field GetAllProperties fun(self:NoesisBaseObject, a1:NoesisBaseObject)
--- @field GetProperty fun(self:NoesisBaseObject, a1:NoesisBaseObject, a2:Noesis::Symbol)
--- @field SetProperty fun(self:NoesisBaseObject, a1:NoesisBaseObject, a2:Noesis::Symbol, a3:AnyRef)
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


--- @class NoesisTypeClass:NoesisTypeMeta
--- @field Base NoesisTypeClass
--- @field Dependency NoesisDependencyData
--- @field Events NoesisTypeProperty[]
--- @field Interfaces NoesisTypeClassAncestorInfo[]
--- @field IsInterface boolean
--- @field Properties NoesisTypeProperty[]
--- @field UIElement NoesisUIElementData
--- @field GetDependencyProperties fun(self:NoesisTypeClass, a1:NoesisTypeClass):NoesisDependencyProperty[]
--- @field GetRoutedEvents fun(self:NoesisTypeClass, a1:NoesisTypeClass):NoesisRoutedEvent[]


--- @class NoesisTypeClassAncestorInfo
--- @field type NoesisTypeClass


--- @class NoesisTypeMeta:NoesisType


--- @class NoesisTypeMetaData:NoesisBaseObject


--- @class NoesisTypeProperty
--- @field ContentType NoesisType
--- @field IsReadOnly boolean
--- @field Meta NoesisTypeMetaData[]
--- @field Name Noesis::Symbol


--- @class NoesisUIElement:NoesisVisual
--- @field Subscribe fun(self:NoesisUIElement, a1:NoesisUIElement, a2:Noesis::Symbol, a3:FunctionRef):uint64
--- @field Unsubscribe fun(self:NoesisUIElement, a1:NoesisUIElement, a2:uint64):boolean


--- @class NoesisUIElementData:NoesisDependencyData
--- @field GetAllEvents fun(self:NoesisUIElementData, a1:NoesisUIElementData):NoesisRoutedEvent[]
--- @field GetEvent fun(self:NoesisUIElementData, a1:NoesisUIElementData, a2:Noesis::Symbol):NoesisRoutedEvent


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
--- @field Size uint8
--- @field SoundSize uint8
--- @field field_1 uint8


--- @class ObjectSizeOverrideBoostComponent:BaseComponent
--- @field field_0 uint8


--- @class OffStageComponent:BaseComponent


--- @class OriginAppearanceTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class OriginComponent:BaseComponent
--- @field Origin FixedString
--- @field field_18 Guid


--- @class OriginPassivesComponent:BaseComponent
--- @field field_18 PassiveInfo[]


--- @class OriginTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class OriginalIdentityComponent:BaseComponent
--- @field field_0 uint8


--- @class OriginalTemplateComponent:BaseComponent
--- @field OriginalTemplate FixedString?


--- @class OsirisTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class PassiveComponent:BaseComponent
--- @field PassiveId FixedString
--- @field field_0 uint8
--- @field field_10 EntityHandle
--- @field field_18 uint8
--- @field field_19 uint8
--- @field field_1C uint32
--- @field field_8 EntityHandle


--- @class PassiveContainerComponent:BaseComponent
--- @field Passives EntityHandle[]


--- @class PassiveInfo
--- @field Passive uint32
--- @field field_0 uint32


--- @class PassiveUsageCount
--- @field field_0 uint16
--- @field field_2 uint16
--- @field field_4 uint16
--- @field field_6 uint16
--- @field field_8 uint16


--- @class PathingComponent:BaseComponent
--- @field Flags uint8
--- @field MovementTiltToRemap FixedString
--- @field PathId int32
--- @field PathMovementSpeed number
--- @field PathParameters table<FixedString, PathingComponentParam>
--- @field ServerControl uint8
--- @field VectorParameters table<FixedString, vec4>
--- @field field_20 int64
--- @field field_28 int32
--- @field field_30 int64
--- @field field_38 int32
--- @field field_48 int32


--- @class PathingComponentParam
--- @field Values1 int32[]
--- @field Values2 number[]
--- @field Values3 vec3[]
--- @field field_30 int32


--- @class PhysicalForceRangeBonusBoostComponent:BaseComponent
--- @field field_0 number
--- @field field_4 uint8


--- @class PhysicsComponent:BaseComponent
--- @field field_10 int32
--- @field field_14 uint8
--- @field field_15 uint8
--- @field field_16 uint8
--- @field field_8 int32
--- @field field_C int32


--- @class PickingStateComponent:BaseComponent


--- @class PlayerComponent:BaseComponent


--- @class PrimaryMaterialParameterSet:MaterialParameterSet
--- @field field_40 int64
--- @field field_48 int64
--- @field field_50 int64
--- @field field_58 int64


--- @class ProficiencyBonusBoostComponent:BaseComponent
--- @field Ability AbilityId
--- @field Skill SkillId
--- @field Type ProficiencyBonusBoostType


--- @class ProficiencyBonusOverrideBoostComponent:BaseComponent
--- @field Value StatsExpressionParam


--- @class ProficiencyBoostComponent:BaseComponent
--- @field Flags ProficiencyGroupFlags


--- @class ProficiencyComponent:BaseComponent
--- @field Flags ProficiencyGroupFlags
--- @field field_0 ProficiencyGroupFlags


--- @class ProficiencyGroupComponent:BaseComponent
--- @field field_0 int64


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


--- @class RedirectDamageBoostComponent:BaseComponent
--- @field Amount int32
--- @field DamageType1 DamageType
--- @field DamageType2 DamageType
--- @field field_6 boolean


--- @class ReduceCriticalAttackThresholdBoostComponent:BaseComponent
--- @field field_0 int32
--- @field field_4 int32
--- @field field_8 int64


--- @class RelationComponent:BaseComponent
--- @field field_0 table<uint32, uint8>
--- @field field_100 table<uint32, uint8>
--- @field field_140 Array_uint32
--- @field field_170 Array_uint32
--- @field field_40 table<uint32, uint8>
--- @field field_80 table<uint32, uint8>
--- @field field_C0 table<RelationComponentGuidAndHandle, uint8>


--- @class RelationComponentGuidAndHandle
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
--- @field _Pad uint32


--- @class ReposeStateComponent:BaseComponent
--- @field field_0 EntityHandle
--- @field field_18 int32
--- @field field_1C vec3
--- @field field_28 uint8
--- @field field_8 Guid


--- @class RerollBoostComponent:BaseComponent
--- @field RollType StatsRollType
--- @field field_1 int8
--- @field field_2 boolean


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
--- @field Description TranslatedString
--- @field DiceSize DiceSizeId
--- @field NumDice uint8
--- @field ResolvedRollBonus int32


--- @class ResolvedUnknown
--- @field Description TranslatedString
--- @field field_0 int32


--- @class ResourceRollDefinition
--- @field field_0 Guid
--- @field field_10 uint8


--- @class RollBonusBoostComponent:BaseComponent
--- @field Ability AbilityId
--- @field Amount StatsExpressionParam
--- @field RollType StatsRollType
--- @field Skill SkillId


--- @class RulesetComponent:BaseComponent
--- @field Modifiers table<Guid, Variant<uint8,int32,float,FixedString,bool>>
--- @field Rulesets Guid[]


--- @class RulesetModifiersComponent:BaseComponent
--- @field Modifiers table<Guid, Variant<uint8,int32,float,FixedString,bool>>


--- @class RuntimeStringHandle
--- @field Handle FixedString
--- @field Version uint16


--- @class SavantBoostComponent:BaseComponent
--- @field SpellSchool SpellSchoolId


--- @class SavegameComponent:BaseComponent


--- @class ScaleMultiplierBoostComponent:BaseComponent
--- @field Multiplier number


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
--- @field field_0 uint8?
--- @field field_14 uint8
--- @field field_4 FixedString?
--- @field field_C FixedString?


--- @class ShootThroughTypeComponent:BaseComponent
--- @field Type uint8


--- @class ShortRestComponent:BaseComponent
--- @field StateId uint8


--- @class SightRangeAdditiveBoostComponent:BaseComponent
--- @field Range number


--- @class SightRangeMaximumBoostComponent:BaseComponent
--- @field Range number


--- @class SightRangeMinimumBoostComponent:BaseComponent
--- @field Range number


--- @class SightRangeOverrideBoostComponent:BaseComponent
--- @field Range number


--- @class SimpleCharacterComponent:BaseComponent


--- @class SkillBoostComponent:BaseComponent
--- @field Amount StatsExpressionParam
--- @field Skill SkillId


--- @class SomeSharedServerClientObjId
--- @field field_0 int64
--- @field field_8 int32


--- @class SomeSharedServerClientObjId2:SomeSharedServerClientObjId
--- @field field_10 int64
--- @field field_18 int32


--- @class SourceAdvantageBoostComponent:BaseComponent
--- @field Type SourceAdvantageType


--- @class SpeakerComponent:BaseComponent
--- @field field_0 FixedString[]


--- @class SpellId
--- @field OriginatorPrototype FixedString
--- @field ProgressionSource Guid
--- @field Prototype FixedString
--- @field SourceType SpellSourceType


--- @class SpellIdBase
--- @field OriginatorPrototype FixedString
--- @field ProgressionSource Guid
--- @field SourceType SpellSourceType


--- @class SpellIdWithPrototype:SpellId
--- @field SpellCastSource Guid?
--- @field SpellProto StatsSpellPrototype


--- @class SpellResistanceBoostComponent:BaseComponent
--- @field Resistance ResistanceBoostFlags


--- @class SpellSaveDCBoostComponent:BaseComponent
--- @field DC int32


--- @class StaticPhysicsComponent:BaseComponent


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


--- @class StatsExpressionParam
--- @field Code string
--- @field Params Variant<uint8,Variant<uint8,AbilityId,SkillId,uint8,STDString>,uint8,uint8,DiceValues,ResourceRollDefinition,uint8,int32,bool>[]


--- @class StatsExpressionParamEx:StatsExpressionParam


--- @class StatsRollMetadata
--- @field AbilityBoosts table<AbilityId, int32>
--- @field AutoAbilityCheckFail boolean
--- @field AutoAbilitySavingThrowFail boolean
--- @field AutoSkillCheckFail boolean
--- @field HasCustomMetadata boolean
--- @field IsCritical boolean
--- @field ProficiencyBonus int32
--- @field ResolvedRollBonuses ResolvedRollBonus[]
--- @field ResolvedUnknowns ResolvedUnknown[]
--- @field RollBonus int32
--- @field SkillBonuses table<SkillId, int32>
--- @field field_10 int32
--- @field field_14 int32
--- @field field_8 int64


--- @class StatsRollResult
--- @field Critical RollCritical
--- @field DiceSize uint32
--- @field DiscardedDiceTotal int32
--- @field NaturalRoll int32
--- @field Total int32
--- @field field_14 uint8
--- @field field_18 int16[]


--- @class StatsRollRoll
--- @field Advantage boolean
--- @field Disadvantage boolean
--- @field Roll DiceValues
--- @field RollType StatsRollType
--- @field field_10 int16[]


--- @class StatsRollType0
--- @field Metadata StatsRollMetadata
--- @field Result StatsRollResult
--- @field Roll StatsRollRoll


--- @class StatsRollType1
--- @field DamageTypeIndex int32
--- @field DamageTypeParams DamageType[]
--- @field IntIndex int32
--- @field IntParams int32[]
--- @field RollIndex int32
--- @field RollParams StatsRollType0[]
--- @field StatExpression string


--- @class StatusImmunitiesComponent:BaseComponent
--- @field PersonalStatusImmunities table<FixedString, Guid>


--- @class StatusImmunityBoostComponent:BaseComponent
--- @field StatusID FixedString
--- @field UnknownUUIDs Guid[]


--- @class StealthComponent:BaseComponent
--- @field Position vec3
--- @field SeekHiddenFlag boolean
--- @field SeekHiddenTimeout number
--- @field field_2C int32


--- @class SteeringComponent:BaseComponent
--- @field field_0 vec3
--- @field field_10 number
--- @field field_14 uint8
--- @field field_18 number
--- @field field_1C uint8
--- @field field_C number


--- @class StoryShortRestDisabledComponent:BaseComponent


--- @class SummonContainerComponent:BaseComponent
--- @field ByTag table<FixedString, EntityHandle[]>
--- @field Characters Array_EntityHandle
--- @field Items Array_EntityHandle
--- @field field_18 table<FixedString, EntityHandle[]>


--- @class SummonLifetimeComponent:BaseComponent
--- @field Lifetime Variant<uint8,float>


--- @class SurfacePathInfluence
--- @field Influence int32
--- @field IsCloud boolean
--- @field SurfaceType SurfaceType


--- @class SurfacePathInfluencesComponent:BaseComponent
--- @field PathInfluences SurfacePathInfluence[]


--- @class SurfaceTemplate:GameObjectTemplate
--- @field AiPathColor vec3
--- @field AiPathIconFX FixedString
--- @field AlwaysUseDefaultLifeTime boolean
--- @field CanEnterCombat boolean
--- @field CanSeeThrough boolean
--- @field CanShootThrough boolean
--- @field DecalMaterial FixedString
--- @field DefaultLifeTime number
--- @field Description TranslatedString
--- @field DisplayName TranslatedString
--- @field FX SurfaceTemplateVisualData[]
--- @field FadeInSpeed number
--- @field FadeOutSpeed number
--- @field FallDamageMultiplier number
--- @field InstanceVisual SurfaceTemplateVisualData[]
--- @field IntroFX SurfaceTemplateVisualData[]
--- @field MaterialType uint8
--- @field NormalBlendingFactor number
--- @field ObscuredStateOverride uint8
--- @field OnEnterDistanceOverride number
--- @field OnMoveDistanceOverride number
--- @field RemoveDestroyedItems boolean
--- @field RollConditions string
--- @field Seed int32
--- @field Statuses SurfaceTemplateStatusData[]
--- @field Summon FixedString
--- @field SurfaceCategory uint8
--- @field SurfaceGrowTimer number
--- @field SurfaceName FixedString
--- @field SurfaceType SurfaceType
--- @field field_188 uint32


--- @class SurfaceTemplateStatusData
--- @field AffectedByRoll boolean
--- @field ApplyToCharacters boolean
--- @field ApplyToItems boolean
--- @field ApplyTypes uint8
--- @field Chance number
--- @field Duration number
--- @field Force boolean
--- @field KeepAlive boolean
--- @field OnlyOncePerTurn boolean
--- @field Remove boolean
--- @field StatusId FixedString
--- @field VanishOnApply boolean


--- @class SurfaceTemplateVisualData
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


--- @class TemplateAnimationSetOverrideComponent:BaseComponent
--- @field Overrides AnimationWaterfallElement[]


--- @class TemporaryHPBoostComponent:BaseComponent
--- @field HP StatsExpressionParam


--- @class TextureLayerConfigId
--- @field Flag uint8
--- @field TextureLayerConfig uint32


--- @class TraderComponent:BaseComponent


--- @class Transform
--- @field Scale vec3
--- @field Translate vec3


--- @class TransformComponent:BaseComponent
--- @field Transform Transform


--- @class TranslatedString
--- @field ArgumentString RuntimeStringHandle
--- @field Handle RuntimeStringHandle


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
--- @field TypeName FixedString
--- @field VarargParams boolean
--- @field VarargsReturn boolean


--- @class TypeInformationRef:TypeInformation


--- @class UnlockInterruptBoostComponent:BaseComponent
--- @field Interrupt FixedString


--- @class UnlockSpellBoostComponent:BaseComponent
--- @field Ability AbilityId
--- @field CooldownType SpellCooldownType
--- @field SomeUUID Guid
--- @field SpellChildSelection SpellChildSelectionType
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
--- @field Boosts BoostParameters[]


--- @class UseComponent:BaseComponent
--- @field Boosts BoostParameters[]
--- @field BoostsOnEquipMainHand BoostParameters[]
--- @field BoostsOnEquipOffHand BoostParameters[]
--- @field Charges int32
--- @field ItemUseType uint8
--- @field MaxCharges int32
--- @field Requirements StatsRequirement[]
--- @field field_19 uint8
--- @field field_1A uint8
--- @field field_1B uint8


--- @class UseSocket
--- @field field_0 int32
--- @field field_30 uint32[]
--- @field field_4 uint8
--- @field field_5 uint8
--- @field field_58 FixedString
--- @field field_5C FixedString
--- @field field_60 EntityHandle
--- @field field_8 uint32[]


--- @class UseSocketComponent:BaseComponent
--- @field Sockets table<uint32, UseSocket>


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


--- @class VirtualTextureManagerBaseLayerConfig
--- @field A uint64
--- @field B uint64
--- @field C uint64


--- @class Visual:MoveableObject
--- @field Attachments VisualAttachment[]
--- @field BlendShapeBase int16
--- @field CullFlags uint16
--- @field LODDistances number[]
--- @field ObjectDescs VisualObjectDesc[]
--- @field OwnsNode uint8
--- @field Parent Visual
--- @field VisualEntity EntityRef
--- @field VisualFlags uint32
--- @field VisualResource ResourceVisualResource
--- @field Wrinkle01 int16
--- @field Wrinkle02 int16
--- @field Wrinkle03 int16
--- @field WrinkleBase int16
--- @field field_F6 uint8
--- @field field_F7 uint8


--- @class VisualAttachment
--- @field Bone1 FixedString
--- @field Bone2 FixedString
--- @field BoneIndex1 int32
--- @field BoneIndex2 int32
--- @field Flags uint32
--- @field HasBone1 boolean
--- @field HasBone2 boolean
--- @field Visual Visual
--- @field field_1E uint8
--- @field field_1F uint8
--- @field field_20 FixedString
--- @field field_24 vec3


--- @class VisualBoneRemap
--- @field field_0 int16
--- @field field_2 int16


--- @class VisualObjectDesc
--- @field Flags uint8
--- @field Renderable RenderableObject
--- @field field_8 uint8


--- @class VisualComponent:BaseComponent
--- @field NotClustered boolean
--- @field Visual Visual
--- @field field_8 uint8
--- @field field_9 uint8


--- @class VoiceComponent:BaseComponent
--- @field Voice Guid


--- @class VoiceTagComponent:BaseComponent
--- @field Tags Guid[]


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
--- @field Rolls table<AbilityId, DiceValues[]>
--- @field Rolls2 table<AbilityId, DiceValues[]>
--- @field VersatileDamageDice DiceSizeId
--- @field WeaponGroup uint8
--- @field WeaponProperties uint32
--- @field WeaponRange number
--- @field field_38 WeaponComponentElement1[]


--- @class WeaponComponentElement1:StatsExpressionParam
--- @field Cause BoostCause
--- @field field_28 uint8
--- @field field_48 TranslatedString
--- @field field_58 uint8


--- @class WeaponDamageBoostComponent:BaseComponent
--- @field Amount StatsExpressionParam
--- @field DamageType DamageType
--- @field field_30 boolean


--- @class WeaponDamageDieOverrideBoostComponent:BaseComponent
--- @field Roll DiceValues


--- @class WeaponDamageResistanceBoostComponent:BaseComponent
--- @field DamageTypes DamageType[]


--- @class WeaponDamageTypeOverrideBoostComponent:BaseComponent
--- @field DamageType DamageType


--- @class WeaponEnchantmentBoostComponent:BaseComponent
--- @field Value int32


--- @class WeaponPropertyBoostComponent:BaseComponent
--- @field Properties WeaponFlags


--- @class WeaponSetComponent:BaseComponent


--- @class WeightBoostComponent:BaseComponent
--- @field Amount int32


--- @class WeightCategoryBoostComponent:BaseComponent
--- @field Amount int32


--- @class WieldingComponent:BaseComponent
--- @field Owner EntityHandle


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
--- @field Visual GameObjectVisualData
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
--- @field Visual GameObjectVisualData
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
--- @field Visual GameObjectVisualData
--- @field field_0 Guid
--- @field field_10 Guid
--- @field field_110 Guid
--- @field field_120 Guid
--- @field field_130 uint8
--- @field field_20 uint8
--- @field field_21 uint8
--- @field field_28 Guid


--- @class CharacterCreationDefinitionCommon
--- @field AbilityPointBonuses uint8[][]
--- @field Cantrips uint8[][]
--- @field Flags uint64
--- @field Proficiencies uint8[][]
--- @field field_0 uint64
--- @field field_10 int64
--- @field field_18 int64
--- @field field_20 int64
--- @field field_28 int64
--- @field field_30 int64
--- @field field_38 int64
--- @field field_40 uint8[][]
--- @field field_50 uint8[][]
--- @field field_60 uint8[][]
--- @field field_70 uint8[][]
--- @field field_8 int64
--- @field field_80 uint8[][]
--- @field field_90 uint8[][]
--- @field field_A0 uint8[][]


--- @class CharacterCreationDefinitionCommonComponent:BaseComponent
--- @field ChangeId int32
--- @field Definition CharacterCreationDefinitionCommon
--- @field field_0 int32
--- @field field_8 int64


--- @class CharacterCreationFullRespecDefinition
--- @field Abilities int32[]
--- @field Definition CharacterCreationBaseCharacterDefinition
--- @field KnownSpells CharacterCreationLevelUpKnownSpell[]
--- @field LevelUpData LevelUpData[]
--- @field Name string
--- @field Visual GameObjectVisualData
--- @field field_0 string
--- @field field_1B0 CharacterCreationLevelUpKnownSpell[]


--- @class CharacterCreationFullRespecDefinitionComponent:BaseComponent
--- @field Definition CharacterCreationFullRespecDefinition
--- @field LevelUpUpgrades LevelUpUpgrades
--- @field Spells SpellIdBase[]
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
--- @field AppearanceElements GameObjectVisualDataAppearanceElement[]
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


--- @class CombatIsCombatPausedComponent:BaseComponent
--- @field Paused boolean


--- @class CombatIsInCombatComponent:BaseComponent


--- @class CombatParticipantComponent:BaseComponent
--- @field AiHint Guid
--- @field CombatGroupId FixedString
--- @field CombatHandle EntityHandle
--- @field Flags CombatParticipantFlags
--- @field InitiativeRoll int32
--- @field field_C int32


--- @class CombatStateComponent:BaseComponent
--- @field Initiatives table<EntityHandle, int32>
--- @field Level FixedString
--- @field MyGuid Guid
--- @field Participants EntityHandle[]
--- @field field_98 EntityHandle
--- @field field_A0 EntityHandle
--- @field field_AC uint8
--- @field field_B0 number
--- @field field_B8 EntityHandle[]
--- @field field_D0 uint8


--- @class CombatThreatRangeComponent:BaseComponent
--- @field field_0 number
--- @field field_4 int32
--- @field field_8 int32


--- @class CombatTurnBasedComponent:BaseComponent
--- @field ActedThisRoundInCombat boolean
--- @field CanAct_M boolean
--- @field Combat Guid
--- @field HadTurnInCombat boolean
--- @field IsInCombat_M boolean
--- @field RequestedEndTurn boolean
--- @field field_10 int32
--- @field field_18 int32
--- @field field_1C uint8
--- @field field_20 int32
--- @field field_24 uint8
--- @field field_28 uint64
--- @field field_8 uint8
--- @field field_A boolean
--- @field field_C boolean


--- @class CombatTurnOrderComponent:BaseComponent
--- @field Participants CombatTurnOrderComponentParticipant[]
--- @field Participants2 CombatTurnOrderComponentParticipant[]
--- @field TurnOrderIndices uint64[]
--- @field TurnOrderIndices2 uint64[]
--- @field field_78 int32
--- @field field_7C int32


--- @class CombatTurnOrderComponentParticipant
--- @field Handles CombatTurnOrderComponentParticipantHandleInfo[]
--- @field Initiative int32
--- @field Participant Guid
--- @field field_28 uint32
--- @field field_30 uint8


--- @class CombatTurnOrderComponentParticipantHandleInfo
--- @field Entity EntityHandle
--- @field Initiative int32


--- @class EclEquipmentVisualData
--- @field BoneSheathed FixedString
--- @field Flags_60 uint32
--- @field Flags_64 uint16
--- @field Flags_6C uint8
--- @field HairType uint8
--- @field Item EntityRef
--- @field ScalarParameters ResourcePresetDataScalarParameter[]
--- @field SlotAndFlags uint32
--- @field SourceBoneSheathed FixedString
--- @field Vector3Parameters ResourcePresetDataVector3Parameter[]
--- @field VisualTemplates FixedString[]
--- @field field_18 FixedString
--- @field field_20 EntityHandle


--- @class EclEquipmentVisualRequest
--- @field Data EclEquipmentVisualData
--- @field Item EntityHandle[]
--- @field SubRequests EclEquipmentVisualRequestSubVisualRequest[]
--- @field field_90 EntityHandle


--- @class EclEquipmentVisualRequestSubVisualRequest
--- @field Processed boolean
--- @field VisualEntity EntityHandle
--- @field VisualTemplate FixedString


--- @class EclEquipmentVisualsComponent:BaseComponent
--- @field Entity EntityHandle
--- @field Equipment EclEquipmentVisualsComponentVisualElement[]


--- @class EclEquipmentVisualsComponentVisualElement
--- @field Item EntityHandle
--- @field SubVisuals EntityHandle[]
--- @field VisualRequest EclEquipmentVisualRequest
--- @field field_20 boolean


--- @class EclPlayerPickingHelper
--- @field ControllerNudge number
--- @field ControllerNudge2 number
--- @field Inner EclPlayerPickingHelperInner
--- @field IsMoving boolean
--- @field PlayerId uint16
--- @field WindowCursorPos vec2
--- @field field_16A uint8
--- @field field_170 EclPlayerPickingHelperField170Entry[]
--- @field field_180 EclPlayerPickingHelperField170Entry?
--- @field field_1AC number
--- @field field_1B0 number
--- @field field_1B4 uint8
--- @field field_1B5 uint8
--- @field field_1B6 uint8
--- @field field_1B7 uint8
--- @field field_1B8 uint8
--- @field field_1B9 uint8
--- @field field_1C0 EntityHandle
--- @field field_1C8_SteeringRotM number
--- @field field_1CC int32
--- @field field_1D0_SteeringRotM2 number
--- @field field_1DC boolean
--- @field field_210 uint8
--- @field field_218 vec3
--- @field field_228_EH_unk table<EntityHandle, EclPlayerPickingHelperMHEntry>
--- @field field_268_NextIndex int32
--- @field field_26C number
--- @field field_270_EH_unk table<EntityHandle, EclPlayerPickingHelperMHEntry>
--- @field field_2B0 int32
--- @field field_2B4 number
--- @field field_2B8_EH_MHentry table<EntityHandle, EclPlayerPickingHelperMHEntry>
--- @field field_2F8 int32


--- @class EclPlayerPickingHelperField170Entry
--- @field field_0_Entity EntityHandle
--- @field field_10 number
--- @field field_14 number
--- @field field_18 number
--- @field field_1C number
--- @field field_20 number
--- @field field_24 number
--- @field field_8_TurnOrder int32
--- @field field_C_InCombat uint8


--- @class EclPlayerPickingHelperMHEntry
--- @field field_0 int32
--- @field field_4 int32
--- @field field_8 uint8


--- @class EclPlayerPickingHelperInner
--- @field Direction0 vec3
--- @field Direction1 vec3
--- @field Direction2 vec3
--- @field Inner EclPlayerPickingHelperInner2[]
--- @field Position0 vec3
--- @field Position1 vec3
--- @field Position2 vec3
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
--- @field field_24 vec3
--- @field field_48 EntityHandle
--- @field field_58 EntityHandle


--- @class EclCharacterCreationBaseDefinitionComponent:BaseComponent
--- @field Definition CharacterCreationCharacterDefinition


--- @class EclCharacterCreationChangeAppearanceDefinitionComponent:BaseComponent
--- @field Definition CharacterCreationChangeAppearanceDefinitionBase


--- @class EclCharacterCreationCompanionDefinitionComponent:BaseComponent
--- @field Visual GameObjectVisualData
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
--- @field Definition CharacterCreationDefinitionCommon
--- @field Entity EntityHandle
--- @field field_0 EntityHandle
--- @field field_10 uint32
--- @field field_8 number
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
--- @field Visual GameObjectVisualData
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


--- @class EclLuaGameStateChangedEvent:LuaEventBase
--- @field FromState ClientGameState
--- @field ToState ClientGameState


--- @class EclLuaKeyInputEvent:LuaEventBase
--- @field Caps boolean
--- @field Event SDLKeyEvent
--- @field Key SDLScanCode
--- @field LAlt boolean
--- @field LCtrl boolean
--- @field LGui boolean
--- @field LShift boolean
--- @field Mode boolean
--- @field Modifiers SDLKeyModifier
--- @field Num boolean
--- @field Pressed boolean
--- @field RAlt boolean
--- @field RCtrl boolean
--- @field RGui boolean
--- @field RShift boolean
--- @field Repeat boolean
--- @field Scroll boolean


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


--- @class EocPlayerCustomData
--- @field Initialized boolean
--- @field OwnerProfileID FixedString
--- @field ReservedProfileID FixedString


--- @class EocBackgroundGoalsComponent:BaseComponent
--- @field Goals table<Guid, EocBackgroundGoalsComponentGoal[]>


--- @class EocBackgroundGoalsComponentGoal
--- @field CategoryId FixedString
--- @field Entity Guid
--- @field Goal Guid
--- @field field_28 uint64


--- @class EocCalendarDaysPassedComponent:BaseComponent
--- @field Days int32


--- @class EocCalendarStartingDateComponent:BaseComponent
--- @field Day int32
--- @field Year int32


--- @class EocFtbParticipantComponent:BaseComponent
--- @field field_18 EntityHandle


--- @class EocFtbRespectComponent:BaseComponent


--- @class EocFtbZoneBlockReasonComponent:BaseComponent
--- @field Reason uint8


--- @class EocProjectileSourceInfoComponent:BaseComponent
--- @field Spell SpellId


--- @class EsvActivationGroupContainerComponent:BaseComponent
--- @field Groups EsvActivationGroupContainerComponentActivationGroup[]


--- @class EsvActivationGroupContainerComponentActivationGroup
--- @field field_0 FixedString
--- @field field_4 FixedString


--- @class EsvAnubisExecutorComponent:BaseComponent
--- @field field_18 int64
--- @field field_20 int64
--- @field field_28 uint8
--- @field field_29 uint8


--- @class EsvAnubisTagComponent:BaseComponent
--- @field Tags Guid[]


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
--- @field DamageList EsvBaseWeaponComponentDamage[]


--- @class EsvBaseWeaponComponentDamage
--- @field DamageType DamageType
--- @field Roll DiceValues
--- @field field_10 uint8


--- @class EsvBoostTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class EsvBreadcrumbComponent:BaseComponent
--- @field field_118 vec3
--- @field field_18 EsvBreadcrumbComponentElement[]


--- @class EsvBreadcrumbComponentElement
--- @field field_0 int32
--- @field field_14 vec3
--- @field field_4 uint8
--- @field field_8 vec3


--- @class EsvChangeSurfaceOnPathAction:EsvCreateSurfaceActionBase
--- @field CheckExistingSurfaces boolean
--- @field FollowHandle EntityRef
--- @field IgnoreIrreplacableSurfaces boolean
--- @field IgnoreOwnerCells boolean
--- @field IsFinished boolean
--- @field Radius number


--- @class EsvCharacter:BaseComponent
--- @field Activated boolean
--- @field BaseVisual FixedString
--- @field BlockNewDisturbanceReactions boolean
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
--- @field ForceNonzeroSpeed boolean
--- @field ForceSynch uint8
--- @field GMReroll boolean
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
--- @field OwnerCharacter EntityHandle
--- @field PartyFollower boolean
--- @field PlayerData EsvPlayerData
--- @field PreferredAiTargets Guid[]
--- @field PreviousCrimeHandle int32
--- @field PreviousCrimeState uint8
--- @field RegisteredForAutomatedDialog boolean
--- @field RequestStartTurn boolean
--- @field ReservedForDialog boolean
--- @field SpotSneakers boolean
--- @field StatusManager EsvStatusMachine
--- @field SteeringEnabled boolean
--- @field StoryNPC boolean
--- @field Summon boolean
--- @field Template CharacterTemplate
--- @field TemplateUsedForSpells CharacterTemplate
--- @field Temporary boolean
--- @field Totem boolean
--- @field Trader boolean
--- @field TreasureGeneratedForTrader boolean
--- @field Treasures FixedString[]
--- @field Unknown0x10 boolean
--- @field Unknown0x40 boolean
--- @field Unknown10000000000 boolean
--- @field Unknown40000000000 boolean
--- @field Unknown8000 boolean
--- @field Unknown80000000 boolean
--- @field Unknown80000000000000 boolean
--- @field UserID UserId
--- @field UserID2 UserId
--- @field WalkThrough boolean
--- @field field_B8 FixedString[]
--- @field GetStatus fun(self:EsvCharacter, a1:FixedString):EsvStatus
--- @field GetStatusByType fun(self:EsvCharacter, a1:StatusType):EsvStatus


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


--- @class EsvDarknessActiveComponent:BaseComponent


--- @class EsvDeathContinueComponent:BaseComponent


--- @class EsvDelayDeathCauseComponent:BaseComponent
--- @field Blocked_M int32
--- @field DelayCount int32


--- @class EsvDialogTagComponent:BaseComponent
--- @field Tags Guid[]


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
--- @field Names EsvDisplayName[]
--- @field TranslatedStrings EsvDisplayNameTranslatedString[]


--- @class EsvDisplayNameTranslatedString
--- @field NameKey TranslatedString
--- @field field_10 uint8


--- @class EsvExperienceGaveOutComponent:BaseComponent
--- @field Experience int32


--- @class EsvExtinguishFireAction:EsvCreateSurfaceActionBase
--- @field ExtinguishGrowTimer number
--- @field ExtinguishPosition vec3
--- @field Percentage number
--- @field Radius number
--- @field Step number


--- @class EsvGameTimerComponent:BaseComponent
--- @field field_18 FixedString
--- @field field_20 EntityHandle
--- @field field_28 int32
--- @field field_2C int32
--- @field field_30 int32
--- @field field_34 int32
--- @field field_38 uint8


--- @class EsvGameplayLightChangesComponent:BaseComponent
--- @field field_0 int64
--- @field field_8 uint8
--- @field field_9 uint8
--- @field field_A uint8


--- @class EsvGameplayLightEquipmentComponent:BaseComponent
--- @field field_0 Array_EntityHandle


--- @class EsvIconListComponent:BaseComponent
--- @field Icons EsvIconListComponentIcon[]


--- @class EsvIconListComponentIcon
--- @field Icon FixedString
--- @field field_4 uint32


--- @class EsvInventoryItemDataPopulatedComponent:BaseComponent


--- @class EsvInventoryPropertyCanBePickpocketedComponent:BaseComponent
--- @field Tag GenericPropertyTag


--- @class EsvInventoryPropertyIsDroppedOnDeathComponent:BaseComponent
--- @field Tag GenericPropertyTag


--- @class EsvInventoryPropertyIsTradableComponent:BaseComponent
--- @field Tag GenericPropertyTag


--- @class EsvIsLightBlockerComponent:BaseComponent


--- @class EsvIsMarkedForDeletionComponent:BaseComponent


--- @class EsvIsUnsummoningComponent:BaseComponent


--- @class EsvIsVisionBlockerComponent:BaseComponent


--- @class EsvItem:BaseComponent
--- @field Activated boolean
--- @field CanBeMoved boolean
--- @field CanBePickedUp boolean
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
--- @field Invulnerable boolean
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
--- @field Arr_vec3 vec3[]
--- @field Flags uint8
--- @field ProjectileTerrainOffset number
--- @field Spell SpellId
--- @field SpellType uint8
--- @field field_0 vec3
--- @field field_100 int32
--- @field field_104 number
--- @field field_108 uint8
--- @field field_10A uint8
--- @field field_18 int32
--- @field field_1C int32
--- @field field_20 int32
--- @field field_24 vec3
--- @field field_30 vec3
--- @field field_3C number
--- @field field_40 number
--- @field field_44 number
--- @field field_48 string
--- @field field_4C number
--- @field field_A4 vec3
--- @field field_B0 vec3
--- @field field_BC number
--- @field field_C vec3
--- @field field_C0 vec3
--- @field field_CC vec3
--- @field field_D8 vec3
--- @field field_E4 uint8
--- @field field_EC int32
--- @field field_F0 uint64
--- @field field_F8 uint8
--- @field field_FC int32


--- @class EsvKillerComponent:BaseComponent
--- @field Killers Array_EntityHandle


--- @class EsvLeaderComponent:BaseComponent
--- @field Followers_M Array_EntityHandle


--- @class EsvNetComponent:BaseComponent


--- @class EsvOrderComponent:BaseComponent


--- @class EsvPeersInRangeComponent:BaseComponent
--- @field Peers int32[]


--- @class EsvPickpocketComponent:BaseComponent
--- @field Items EsvPickpocketComponentPickpocketEntry[]
--- @field field_18 EsvPickpocketComponentPickpocketEntry[]


--- @class EsvPickpocketComponentPickpocketEntry
--- @field Amount int32
--- @field Item EntityHandle
--- @field Target EntityHandle
--- @field field_0 EntityHandle
--- @field field_10 int32
--- @field field_14 boolean
--- @field field_18 EntityHandle
--- @field field_8 EntityHandle


--- @class EsvPlayerData
--- @field CachedTension uint8
--- @field CustomData EocPlayerCustomData
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


--- @class EsvPolygonSurfaceAction:EsvCreateSurfaceActionBase
--- @field Characters EntityHandle[]
--- @field CurrentGrowTimer number
--- @field GrowStep int32
--- @field GrowTimer number
--- @field Items EntityHandle[]
--- @field LastSurfaceCellCount int32
--- @field PolygonVertices vec2[]
--- @field SomePosition vec3


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


--- @class EsvReplicationDependencyComponent:BaseComponent
--- @field Dependency EntityHandle


--- @class EsvReplicationDependencyOwnerComponent:BaseComponent
--- @field Dependents EntityHandle[]


--- @class EsvSafePositionComponent:BaseComponent
--- @field Position vec3
--- @field field_24 boolean


--- @class EsvScriptPropertyCanBePickpocketedComponent:BaseComponent


--- @class EsvScriptPropertyIsDroppedOnDeathComponent:BaseComponent


--- @class EsvScriptPropertyIsTradableComponent:BaseComponent


--- @class EsvStateComponent:BaseComponent
--- @field Flags uint32


--- @class EsvStatus
--- @field BringIntoCombat boolean
--- @field Cause EntityRef
--- @field CauseGUID Guid
--- @field CauseIsOrigin boolean
--- @field CauseType uint8
--- @field CausedByEquipment boolean
--- @field CurrentLifeTime number
--- @field DifficultyStatus FixedString
--- @field DisableImmunityOverhead boolean
--- @field DontTickWhileOnSurface boolean
--- @field ExitCause uint8
--- @field Flags ServerStatusFlags
--- @field Flags2 ServerStatusFlags2
--- @field Flags3 ServerStatusFlags3
--- @field Flags4 ServerStatusFlags4
--- @field ForceFailStatus boolean
--- @field ForceStatus boolean
--- @field FreezeDuration boolean
--- @field IgnoreStacking boolean
--- @field Influence boolean
--- @field InitiateCombat boolean
--- @field IsChanneled boolean
--- @field IsFromItem boolean
--- @field IsHostileAct boolean
--- @field IsInvulnerable boolean
--- @field IsInvulnerableVisible boolean
--- @field IsLifeTimeSet boolean
--- @field IsOnSourceSurface boolean
--- @field IsRecoverable boolean
--- @field IsUnique boolean
--- @field KeepAlive boolean
--- @field LifeTime number
--- @field Loaded boolean
--- @field NotifiedPlanManager boolean
--- @field OriginCauseType uint8
--- @field Originator ActionOriginator
--- @field Owner EntityHandle
--- @field RemoveConditionsId int32
--- @field RemoveEvents uint32
--- @field RequestClientSync boolean
--- @field RequestDelete boolean
--- @field RequestDeleteAtTurnEnd boolean
--- @field RootCause EntityRef
--- @field SourceEquippedItem EntityHandle
--- @field SourceSpell SpellId
--- @field SourceUsedItem EntityHandle
--- @field SpellCastSourceUuid Guid
--- @field SpellCastingAbility uint8
--- @field StackId FixedString
--- @field StackPriority int32
--- @field StackedSpellCastSourceUuid Guid
--- @field StartTimer number
--- @field Started boolean
--- @field StatusFlags3_0x08 boolean
--- @field StatusFlags3_0x10 boolean
--- @field StatusFlags3_0x20 boolean
--- @field StatusFlags3_0x40 boolean
--- @field StatusFlags3_0x80 boolean
--- @field StatusFlags4_0x04 boolean
--- @field StatusFlags4_0x80 boolean
--- @field StatusHandle ComponentHandle
--- @field StatusId FixedString
--- @field StatusOwner EntityHandle[]
--- @field StatusSource EntityHandle
--- @field StoryActionID int32
--- @field Strength number
--- @field SyncEntity EntityHandle
--- @field TickType uint8
--- @field TurnTimer number
--- @field field_105 uint8
--- @field field_18 int64
--- @field field_48 number
--- @field field_8 Guid
--- @field field_E0 EntityHandle
--- @field field_E8 Guid


--- @class EsvStatusAura:EsvStatus


--- @class EsvStatusBoost:EsvStatusAura
--- @field BoostStackId FixedString
--- @field EffectTime number
--- @field ItemHandles EntityHandle[]
--- @field Items FixedString[]
--- @field LoseControl boolean
--- @field SourceDirection vec3
--- @field Spell FixedString[]


--- @class EsvStatusClimbing:EsvStatus
--- @field Direction boolean
--- @field Incapacitated_M boolean
--- @field Item EntityHandle
--- @field JumpUpLadders_M boolean
--- @field Level FixedString
--- @field MoveDirection_M vec3
--- @field Started_M boolean
--- @field Status uint8


--- @class EsvStatusDeactivated:EsvStatusBoost


--- @class EsvStatusDowned:EsvStatusIncapacitated
--- @field DamageFailures int32
--- @field IsHealed boolean
--- @field IsStable boolean
--- @field NumStableFailed int32
--- @field NumStableSuccess int32
--- @field RollFailures int32
--- @field RollSuccesses int32
--- @field StableRollDC int32


--- @class EsvStatusDying:EsvStatus
--- @field Combat Guid
--- @field DyingFlags uint8
--- @field HitDescription Hit
--- @field Source EntityRef


--- @class EsvStatusEffect:EsvStatus


--- @class EsvStatusFear:EsvStatusBoost


--- @class EsvStatusHeal:EsvStatus
--- @field AbsorbSurfaceRange int32
--- @field EffectTime number
--- @field HealAmount int32
--- @field HealEffect int32
--- @field HealEffectId FixedString
--- @field HealType uint8
--- @field TargetDependentHeal uint8
--- @field TargetDependentHealAmount int32
--- @field TargetDependentValue int32


--- @class EsvStatusInSurface:EsvStatus
--- @field Translate vec3


--- @class EsvStatusIncapacitated:EsvStatusBoost
--- @field CurrentFreezeTime number
--- @field FreezeTime number
--- @field IncapacitateFlags uint8
--- @field IncapacitationAnimationFinished boolean


--- @class EsvStatusInvisible:EsvStatusBoost
--- @field InvisiblePosition vec3


--- @class EsvStatusKnockedDown:EsvStatus
--- @field IsInstant boolean
--- @field KnockedDownState uint8


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
--- @field IgnoreChecks boolean
--- @field IgnoreHasSpell boolean
--- @field Partner EntityHandle
--- @field ShowOverhead boolean
--- @field Source EntityRef
--- @field Spell SpellId
--- @field Target EntityRef
--- @field TargetPosition vec3


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


--- @class EsvSurfaceAction
--- @field Handle EntityHandle
--- @field Originator ActionOriginator
--- @field StoryActionID int32


--- @class EsvSurfaceComponent:BaseComponent
--- @field field_0 int64
--- @field field_10 EntityHandle
--- @field field_8 int64


--- @class EsvTargetUUIDComponent:BaseComponent
--- @field Target Guid


--- @class EsvTemplateTagComponent:BaseComponent
--- @field Tags Guid[]


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


--- @class EsvVariableManagerComponent:BaseComponent


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


--- @class EsvAiCombatAiModifiersComponent:BaseComponent
--- @field Modifiers table<FixedString, Guid>


--- @class EsvAiCombatArchetypeComponent:BaseComponent
--- @field field_0 FixedString
--- @field field_4 FixedString
--- @field field_8 FixedString
--- @field field_C FixedString


--- @class EsvAiCombatInterestedInItemsComponent:BaseComponent
--- @field Items Array_EntityHandle


--- @class EsvAiCombatInterestingItemComponent:BaseComponent
--- @field Items Array_EntityHandle


--- @class EsvBoostBoostBaseComponent:BaseComponent
--- @field field_0 FixedString[]


--- @class EsvBoostStatusBoostsProcessedComponent:BaseComponent


--- @class EsvCharacterCreationAppearanceVisualTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class EsvCharacterCreationEquipmentSetRequestComponent:BaseComponent
--- @field Requests table<EntityHandle, FixedString[]>


--- @class EsvCharacterCreationGodComponent:BaseComponent
--- @field God Guid


--- @class EsvCharacterCreationIsCustomComponent:BaseComponent


--- @class EsvCharacterCreationUpdatesComponent:BaseComponent
--- @field Updates table<EntityHandle, int32>


--- @class EsvCombatCanStartCombatComponent:BaseComponent


--- @class EsvCombatCombatGroupMappingComponent:BaseComponent
--- @field CombatGroups table<FixedString, Array_EntityHandle>
--- @field Entity EntityHandle


--- @class EsvCombatEnterRequestComponent:BaseComponent
--- @field EnterRequests Array_EntityHandle


--- @class EsvCombatFleeBlockedComponent:BaseComponent


--- @class EsvCombatImmediateJoinComponent:BaseComponent


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


--- @class EsvInterruptActionRequest1
--- @field MHM_EH_MHS_EH table<EntityHandle, Array_EntityHandle>
--- @field field_0 int64
--- @field field_10 InterruptInterruptVariant2
--- @field field_158 int64
--- @field field_8 int64


--- @class EsvInterruptActionRequest2
--- @field field_0 int64
--- @field field_10 InterruptInterruptVariant2
--- @field field_118 int64
--- @field field_8 int64


--- @class EsvInterruptActionRequest3
--- @field MHM_EH_MHS_EH table<EntityHandle, Array_EntityHandle>
--- @field field_0 int64
--- @field field_8 int64


--- @class EsvInterruptActionRequest4
--- @field UseCosts StatsSpellPrototypeUseCostGroup[]
--- @field field_0 int64
--- @field field_18 table<EntityHandle, table<InterruptInterruptVariant2, ConditionRoll>>


--- @class EsvInterruptActionRequestsComponent:BaseComponent
--- @field Requests1 EsvInterruptActionRequest1[]
--- @field Requests2 EsvInterruptActionRequest2[]
--- @field Requests3 EsvInterruptActionRequest3[]


--- @class EsvInterruptAddRemoveRequestsComponent:BaseComponent
--- @field Requests table<EntityHandle, uint8>


--- @class EsvInterruptInitialParticipantsComponent:BaseComponent
--- @field Participants table<EntityHandle, EsvInterruptInitialParticipantsComponentParticipant>


--- @class EsvInterruptInitialParticipantsComponentParticipant
--- @field Entities Array_EntityHandle
--- @field Request1 EsvInterruptActionRequest1


--- @class EsvInterruptTurnOrderInZoneComponent:BaseComponent
--- @field InZone Array_EntityHandle


--- @class EsvInterruptZoneRequestsComponent:BaseComponent
--- @field Requests1 EsvInterruptActionRequest1[]
--- @field Requests2 EsvInterruptActionRequest2[]


--- @class EsvInventoryCharacterHasGeneratedTradeTreasureComponent:BaseComponent


--- @class EsvInventoryContainerDataComponent:BaseComponent
--- @field Flags uint16
--- @field field_4 int32


--- @class EsvInventoryGroupCheckComponent:BaseComponent
--- @field ConditionId int32


--- @class EsvInventoryIsReplicatedWithComponent:BaseComponent


--- @class EsvInventoryReadyToBeAddedToInventoryComponent:BaseComponent


--- @class EsvInventoryShapeshiftEquipmentHistoryComponent:BaseComponent
--- @field History Guid[]


--- @class EsvItemDynamicLayerOwnerComponent:BaseComponent
--- @field Owner FixedString


--- @class EsvLuaAfterExecuteFunctorEvent:LuaEventBase
--- @field Functor StatsFunctors
--- @field Hit HitResult
--- @field Params StatsBaseFunctorExecParams


--- @class EsvLuaBeforeDealDamageEvent:LuaEventBase
--- @field DamageSums DamageSums
--- @field Hit Hit


--- @class EsvLuaDealDamageEvent:LuaEventBase
--- @field Caster EntityHandle
--- @field DamageSums DamageSums
--- @field Functor StatsDealDamageFunctor
--- @field Hit Hit
--- @field HitWith HitWith
--- @field IsFromItem boolean
--- @field Originator ActionOriginator
--- @field Position vec3
--- @field SpellId SpellIdWithPrototype
--- @field StoryActionId int32
--- @field Target EntityHandle


--- @class EsvLuaDealtDamageEvent:EsvLuaDealDamageEvent
--- @field Result HitResult


--- @class EsvLuaExecuteFunctorEvent:LuaEventBase
--- @field Functor StatsFunctors
--- @field Params StatsBaseFunctorExecParams


--- @class EsvLuaGameStateChangedEvent:LuaEventBase
--- @field FromState ServerGameState
--- @field ToState ServerGameState


--- @class EsvOwnershipIsCurrentOwnerComponent:BaseComponent
--- @field Owner Array_EntityHandle


--- @class EsvOwnershipIsLatestOwnerComponent:BaseComponent
--- @field Owner Array_EntityHandle


--- @class EsvOwnershipIsOriginalOwnerComponent:BaseComponent
--- @field Owner Array_EntityHandle


--- @class EsvOwnershipIsPreviousOwnerComponent:BaseComponent
--- @field Owner Array_EntityHandle


--- @class EsvOwnershipOwneeHistoryComponent:BaseComponent
--- @field LatestOwner EntityHandle
--- @field OriginalOwner EntityHandle
--- @field PreviousLatestOwner EntityHandle
--- @field PreviousOwner EntityHandle


--- @class EsvOwnershipOwneeRequestComponent:BaseComponent
--- @field LatestOwner EntityHandle
--- @field NewCurrentOwnee EntityHandle
--- @field OriginalOwner EntityHandle
--- @field RequestChangeCurrentOwnee boolean
--- @field RequestChangeLatestOwner boolean
--- @field RequestChangeOriginalOwner boolean


--- @class EsvPartyUserSnapshotComponent:BaseComponent
--- @field PerUserCharacters table<Guid, EntityHandle[][]>
--- @field Snapshot table<Guid, EntityHandle[][]>


--- @class EsvPassivePassiveBaseComponent:BaseComponent
--- @field field_0 FixedString[]


--- @class EsvPassivePersistentDataComponent:BaseComponent
--- @field field_0 int32
--- @field field_4 int32


--- @class EsvPassiveScriptPassivesComponent:BaseComponent
--- @field Passives Array_FixedString


--- @class EsvPassiveToggledPassivesComponent:BaseComponent
--- @field Passives table<FixedString, boolean>


--- @class EsvProgressionChangedContainersComponent:BaseComponent
--- @field Changes table<EntityHandle, table<EntityHandle, EntityHandle[]>>


--- @class EsvProjectileAttachmentComponent:BaseComponent
--- @field Attachment EntityHandle


--- @class EsvProjectileBezier3Trajectory
--- @field DistanceMax number
--- @field DistanceMin number
--- @field ShiftMax number
--- @field ShiftMin number


--- @class EsvProjectileBezier4Trajectory
--- @field DistanceMax number
--- @field DistanceMin number
--- @field ShiftAMax number
--- @field ShiftAMin number
--- @field ShiftBMax number
--- @field ShiftBMin number


--- @class EsvProjectileConstantVelocity
--- @field ConstantVelocity number


--- @class EsvProjectileForkOnHit:EsvProjectileOnHitActionBase
--- @field ForkCount int32
--- @field ForkLevels int32
--- @field HitTargets EntityHandle[]


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
--- @field Hit Hit
--- @field HitAction EsvProjectileOnHitActionBase
--- @field IgnoreObjects boolean
--- @field IgnoreTargetChecks boolean
--- @field IsFromItem boolean
--- @field IsTrap boolean
--- @field MainDamageType boolean
--- @field MovementSettings EsvProjectileMovementSettings?
--- @field MovingObject EntityHandle
--- @field NoMovement boolean
--- @field Originator ActionOriginator
--- @field Path EsvProjectileProjectileMovementSettings?
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
--- @field TargetTracking2_M EntityHandle
--- @field TargetTracking_M EntityHandle
--- @field TextKey FixedString
--- @field ThrownObject EntityHandle
--- @field field_1C int32
--- @field field_93 uint8


--- @class EsvProjectileLinearVelocity
--- @field Acceleration number
--- @field InitialSpeed number


--- @class EsvProjectileMappedVelocity
--- @field Mapping FixedString


--- @class EsvProjectileMovementSettings
--- @field RotateMode uint8
--- @field Trajectory Variant<EsvProjectileBezier3Trajectory,EsvProjectileBezier4Trajectory>
--- @field Velocity Variant<EsvProjectileConstantVelocity,EsvProjectileLinearVelocity,EsvProjectileMappedVelocity>


--- @class EsvProjectileOnHitActionBase
--- @field Caster EntityHandle
--- @field IgnoreTargetChecks boolean
--- @field IsFromItem boolean
--- @field Spell SpellId


--- @class EsvProjectilePathDescription:EsvProjectileMovementSettings
--- @field Active boolean
--- @field ComputedTrajectoryValues number[]
--- @field ComputedVelocityValues vec3[]
--- @field InterpolateValue number
--- @field SourcePosition vec3
--- @field SourceRotation vec4
--- @field TargetPosition vec3
--- @field TargetRotation vec4


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
--- @field ExplodeRadius number
--- @field ExplodeRadius2 number
--- @field FallbackTimer number
--- @field Flags uint64
--- @field Hit Hit
--- @field HitAction EsvProjectileOnHitActionBase
--- @field HitFunctors StatsFunctors
--- @field HitInterpolation number
--- @field HitObject EntityHandle
--- @field IgnoreObjects boolean
--- @field IgnoreRoof boolean
--- @field IgnoreTargetChecks boolean
--- @field IsFromItem boolean
--- @field IsOnHold boolean
--- @field IsThrown boolean
--- @field IsTrap boolean
--- @field ItemActivated boolean
--- @field Launched boolean
--- @field Level FixedString
--- @field LifeTime number
--- @field MainDamageType DamageType
--- @field MovingObject EntityHandle
--- @field MovingObjectPosition vec3
--- @field MovingObjectRotation quat
--- @field MovingObjectScale vec3
--- @field NoMovement boolean
--- @field Originator ActionOriginator
--- @field Owner EntityRef
--- @field Path EsvProjectilePathDescription
--- @field Path2 EsvProjectilePathDescription
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
--- @field StoryActionId int32
--- @field Success boolean
--- @field TargetObject EntityHandle
--- @field TargetObjectPos vec3
--- @field TargetPos vec3
--- @field Template ProjectileTemplate
--- @field TextKey FixedString
--- @field Used boolean
--- @field field_51D uint8
--- @field field_523 uint8
--- @field field_526 uint8
--- @field field_574 vec3
--- @field field_8 int64


--- @class EsvProjectileProjectileMovementSettings:EsvProjectileMovementSettings
--- @field InterpolateValue number
--- @field field_50 FixedString
--- @field field_58 EntityHandle
--- @field field_60 int64
--- @field field_68 int32


--- @class EsvProjectileSpellComponent:BaseComponent
--- @field Spell EntityHandle
--- @field Target SpellCastMultiTargetInfo2
--- @field field_B0 int32
--- @field field_B4 uint8


--- @class EsvShapeshiftFloatOverride
--- @field Flags uint32
--- @field Value number


--- @class EsvShapeshiftHealthReservationComponent:BaseComponent
--- @field Reservations table<Guid, int32>


--- @class EsvShapeshiftIntOverride
--- @field Flags uint32
--- @field Value int32


--- @class EsvShapeshiftRootTemplateOverride
--- @field RootTemplate FixedString
--- @field field_114 uint8


--- @class EsvShapeshiftState
--- @field Abilities Optional_EsvShapeshiftIntOverride[]
--- @field AiHint Guid?
--- @field Armor int32?
--- @field ArmorType int32?
--- @field CanShootThrough boolean?
--- @field CombatArchetype FixedString?
--- @field CoverAmount boolean?
--- @field CustomDefaultBoosts boolean
--- @field CustomDifficultyStatuses boolean
--- @field CustomFlags boolean
--- @field CustomPassives boolean
--- @field CustomPersonalStatusImmunities boolean
--- @field CustomProficiencyGroup boolean
--- @field CustomResistances boolean
--- @field DefaultBoosts FixedString
--- @field DifficultyStatuses FixedString
--- @field DisplayName TranslatedString?
--- @field Flags uint32
--- @field Gender uint8?
--- @field Passives FixedString
--- @field PersonalStatusImmunities FixedString
--- @field ProficiencyGroup ProficiencyGroupFlags
--- @field Race Guid?
--- @field Resistances ResistanceBoostFlags[][]
--- @field RootTemplate EsvShapeshiftRootTemplateOverride?
--- @field ShootThroughType uint8?
--- @field SpellCastingAbility AbilityId?
--- @field SpellSet FixedString?
--- @field StepsType uint32?
--- @field Title TranslatedString?
--- @field UnarmedAttackAbility AbilityId?
--- @field UnarmedRangedAttackAbility AbilityId?
--- @field Vitality EsvShapeshiftIntOverride?
--- @field VitalityBoost EsvShapeshiftIntOverride?
--- @field WalkThrough boolean?
--- @field Weight EsvShapeshiftFloatOverride?
--- @field field_0 Guid
--- @field field_14C EsvShapeshiftIntOverride?
--- @field field_158 EntityHandle?
--- @field field_168 uint8
--- @field field_169 uint8
--- @field field_16A uint8
--- @field field_170 table<EntityHandle, FixedString[]>
--- @field field_1B0 FixedString[]
--- @field field_1C0 uint8
--- @field field_1FC uint8
--- @field field_1FD uint8
--- @field field_258 uint8
--- @field field_259 uint8
--- @field field_25A uint8
--- @field field_260 uint64
--- @field field_268 uint64
--- @field field_270 uint8
--- @field field_7C FixedString?
--- @field field_B2 uint8?
--- @field field_B4 uint8
--- @field field_B5 uint8
--- @field field_B8 Guid[]
--- @field field_C8 Guid[]
--- @field field_D8 uint8?
--- @field field_DA uint16?
--- @field field_DE uint16?
--- @field field_E2 uint16?
--- @field field_E6 uint16?
--- @field field_F0 string?


--- @class EsvShapeshiftStatesComponent:BaseComponent
--- @field States EsvShapeshiftState[]
--- @field field_0 int32


--- @class EsvSpellOnDamageSpell
--- @field Spell FixedString
--- @field field_4 int32
--- @field field_8 uint8


--- @class EsvSpellOnDamageSpellsComponent:BaseComponent
--- @field Spells EsvSpellOnDamageSpell[]


--- @class EsvSpellCastCastHitDelayComponent:BaseComponent
--- @field CastHitDelays EsvSpellCastCastHitDelayComponentCastHitDelay[]
--- @field CastTargetHitDelay number
--- @field CastTargetHitDelay2 number


--- @class EsvSpellCastCastHitDelayComponentCastHitDelay
--- @field HitNumber int32
--- @field TextKey FixedString
--- @field field_10 number
--- @field field_18 int32
--- @field field_4 int32
--- @field field_8 int32
--- @field field_C int32


--- @class EsvSpellCastCastResponsibleComponent:BaseComponent
--- @field Entity EntityHandle


--- @class EsvSpellCastCastState
--- @field field_0 Guid
--- @field field_10 FixedString
--- @field field_14 FixedString
--- @field field_18 FixedString
--- @field field_1C uint8


--- @class EsvSpellCastClientInitiatedComponent:BaseComponent


--- @class EsvSpellCastExternalsComponent:BaseComponent
--- @field Externals Guid[]


--- @class EsvSpellCastHitRegisterComponent:BaseComponent
--- @field Hits Guid[]


--- @class EsvSpellCastInterruptIdentifier
--- @field field_0 Guid
--- @field field_10 uint64


--- @class EsvSpellCastInterruptRequestsComponent:BaseComponent
--- @field Requests1 EsvInterruptActionRequest1[]
--- @field Requests2 EsvInterruptActionRequest2[]
--- @field Requests3 EsvInterruptActionRequest3[]
--- @field Requests4 EsvInterruptActionRequest4[]


--- @class EsvSpellCastInterruptResult
--- @field field_0 table<uint8, table<uint8, EsvSpellCastInterruptRollData>>
--- @field field_40 table<uint8, table<uint8, uint64>>
--- @field field_50 uint16[]
--- @field field_60 Array_uint8
--- @field field_90 Array_uint8


--- @class EsvSpellCastInterruptResult2
--- @field Roll Variant<StatsRollType0,StatsRollType1>
--- @field _Pad uint32
--- @field field_0 int64
--- @field field_10 int64
--- @field field_138 int32
--- @field field_140 int64
--- @field field_148 int64
--- @field field_150 uint8
--- @field field_151 uint8
--- @field field_152 uint8
--- @field field_18 int32
--- @field field_1C uint8
--- @field field_20 uint8
--- @field field_21 uint8
--- @field field_8 int64


--- @class EsvSpellCastInterruptResultsComponent:BaseComponent
--- @field Results table<EsvSpellCastInterruptIdentifier, EsvSpellCastInterruptResult>
--- @field Results2 EsvSpellCastInterruptResult2[]


--- @class EsvSpellCastInterruptRollData
--- @field field_0 int64
--- @field field_8 ResolvedUnknown[]


--- @class EsvSpellCastStateComponent:BaseComponent
--- @field State EsvSpellCastCastState
--- @field Status uint8
--- @field field_28 int32
--- @field field_4 int32


--- @class EsvStatusActiveComponent:BaseComponent


--- @class EsvStatusAddedFromSaveLoadComponent:BaseComponent


--- @class EsvStatusAuraComponent:BaseComponent


--- @class EsvStatusCauseComponent:BaseComponent
--- @field Cause Guid
--- @field StoryActionId int32


--- @class EsvStatusDifficultyModifiersComponent:BaseComponent
--- @field Modifiers table<FixedString, Array_FixedString>


--- @class EsvStatusOwnershipComponent:BaseComponent
--- @field Owner EntityHandle


--- @class EsvStatusPerformingComponent:BaseComponent
--- @field field_0 FixedString


--- @class EsvStatusStatusComponent:BaseComponent
--- @field Entity EntityHandle
--- @field SpellCastSourceUuid Guid
--- @field StatusHandle ComponentHandle
--- @field StatusId FixedString
--- @field Type StatusType


--- @class EsvStatusUniqueComponent:BaseComponent
--- @field Unique table<FixedString, EntityHandle>


--- @class ExtuiBulletText:ExtuiStyledRenderable


--- @class ExtuiButton:ExtuiStyledRenderable
--- @field MouseButtonMiddle boolean
--- @field MouseButtonRight boolean
--- @field OnClick RegistryEntry
--- @field Size vec2?


--- @class ExtuiCheckbox:ExtuiStyledRenderable
--- @field Checked boolean
--- @field OnChange RegistryEntry


--- @class ExtuiChildWindow:ExtuiTreeParent
--- @field AlwaysAutoResize boolean
--- @field AlwaysHorizontalScrollbar boolean
--- @field AlwaysUseWindowPadding boolean
--- @field AlwaysVerticalScrollbar boolean
--- @field AutoResizeX boolean
--- @field AutoResizeY boolean
--- @field Border boolean
--- @field ChildAlwaysAutoResize boolean
--- @field FrameStyle boolean
--- @field HorizontalScrollbar boolean
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
--- @field ResizeX boolean
--- @field ResizeY boolean
--- @field Size vec2?
--- @field UnsavedDocument boolean


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


--- @class ExtuiColorEdit:ExtuiStyledRenderable
--- @field AlphaBar boolean
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
--- @field Components int32
--- @field Logarithmic boolean
--- @field Max ivec4
--- @field Min ivec4
--- @field NoInput boolean
--- @field NoRoundToFormat boolean
--- @field OnChange RegistryEntry
--- @field Value ivec4


--- @class ExtuiDragScalar:ExtuiStyledRenderable
--- @field AlwaysClamp boolean
--- @field Components int32
--- @field Logarithmic boolean
--- @field Max vec4
--- @field Min vec4
--- @field NoInput boolean
--- @field NoRoundToFormat boolean
--- @field OnChange RegistryEntry
--- @field Value vec4


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
--- @field MouseButtonMiddle boolean
--- @field MouseButtonRight boolean
--- @field OnClick RegistryEntry
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
--- @field CallbackAlways boolean
--- @field CallbackCharFilter boolean
--- @field CallbackCompletion boolean
--- @field CallbackEdit boolean
--- @field CallbackHistory boolean
--- @field CallbackResize boolean
--- @field CharsDecimal boolean
--- @field CharsHexadecimal boolean
--- @field CharsNoBlank boolean
--- @field CharsScientific boolean
--- @field CharsUppercase boolean
--- @field Components int32
--- @field CtrlEnterForNewLine boolean
--- @field EnterReturnsTrue boolean
--- @field EscapeClearsAll boolean
--- @field Multiline boolean
--- @field NoHorizontalScroll boolean
--- @field NoUndoRedo boolean
--- @field OnChange RegistryEntry
--- @field Password boolean
--- @field ReadOnly boolean
--- @field Value ivec4


--- @class ExtuiInputScalar:ExtuiStyledRenderable
--- @field AllowTabInput boolean
--- @field AlwaysOverwrite boolean
--- @field AutoSelectAll boolean
--- @field CallbackAlways boolean
--- @field CallbackCharFilter boolean
--- @field CallbackCompletion boolean
--- @field CallbackEdit boolean
--- @field CallbackHistory boolean
--- @field CallbackResize boolean
--- @field CharsDecimal boolean
--- @field CharsHexadecimal boolean
--- @field CharsNoBlank boolean
--- @field CharsScientific boolean
--- @field CharsUppercase boolean
--- @field Components int32
--- @field CtrlEnterForNewLine boolean
--- @field EnterReturnsTrue boolean
--- @field EscapeClearsAll boolean
--- @field Multiline boolean
--- @field NoHorizontalScroll boolean
--- @field NoUndoRedo boolean
--- @field OnChange RegistryEntry
--- @field Password boolean
--- @field ReadOnly boolean
--- @field Value vec4


--- @class ExtuiInputText:ExtuiStyledRenderable
--- @field AllowTabInput boolean
--- @field AlwaysOverwrite boolean
--- @field AutoSelectAll boolean
--- @field CallbackAlways boolean
--- @field CallbackCharFilter boolean
--- @field CallbackCompletion boolean
--- @field CallbackEdit boolean
--- @field CallbackHistory boolean
--- @field CallbackResize boolean
--- @field CharsDecimal boolean
--- @field CharsHexadecimal boolean
--- @field CharsNoBlank boolean
--- @field CharsScientific boolean
--- @field CharsUppercase boolean
--- @field CtrlEnterForNewLine boolean
--- @field EnterReturnsTrue boolean
--- @field EscapeClearsAll boolean
--- @field Hint string?
--- @field Multiline boolean
--- @field NoHorizontalScroll boolean
--- @field NoUndoRedo boolean
--- @field OnChange RegistryEntry
--- @field Password boolean
--- @field ReadOnly boolean
--- @field SizeHint vec2?
--- @field Text string


--- @class ExtuiMenu:ExtuiTreeParent
--- @field AddItem fun(self:ExtuiMenu, a1:string, a2:string?):ExtuiMenuItem


--- @class ExtuiMenuBar:ExtuiTreeParent


--- @class ExtuiMenuItem:ExtuiStyledRenderable
--- @field Enabled boolean
--- @field OnClick RegistryEntry
--- @field Shortcut string?


--- @class ExtuiNewLine:ExtuiStyledRenderable


--- @class ExtuiPopup:ExtuiTreeParent
--- @field AlwaysAutoResize boolean
--- @field AlwaysHorizontalScrollbar boolean
--- @field AlwaysVerticalScrollbar boolean
--- @field HorizontalScrollbar boolean
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
--- @field Open fun(self:ExtuiPopup, a1:GuiPopupFlags?)


--- @class ExtuiRadioButton:ExtuiStyledRenderable
--- @field Active boolean
--- @field OnChange RegistryEntry


--- @class ExtuiRenderable
--- @field Handle uint64
--- @field Parent uint64
--- @field UserData RegistryEntry
--- @field Destroy fun(self:ExtuiRenderable)


--- @class ExtuiSeparator:ExtuiStyledRenderable


--- @class ExtuiSeparatorText:ExtuiStyledRenderable


--- @class ExtuiSliderInt:ExtuiStyledRenderable
--- @field AlwaysClamp boolean
--- @field Components int32
--- @field Logarithmic boolean
--- @field Max ivec4
--- @field Min ivec4
--- @field NoInput boolean
--- @field NoRoundToFormat boolean
--- @field OnChange RegistryEntry
--- @field Value ivec4


--- @class ExtuiSliderScalar:ExtuiStyledRenderable
--- @field AlwaysClamp boolean
--- @field Components int32
--- @field Logarithmic boolean
--- @field Max vec4
--- @field Min vec4
--- @field NoInput boolean
--- @field NoRoundToFormat boolean
--- @field OnChange RegistryEntry
--- @field Value vec4


--- @class ExtuiSpacing:ExtuiStyledRenderable


--- @class ExtuiStyledRenderable:ExtuiRenderable
--- @field AbsolutePosition vec2?
--- @field Font FixedString
--- @field IDContext string
--- @field ItemWidth number?
--- @field Label string
--- @field OnActivate RegistryEntry
--- @field OnDeactivate RegistryEntry
--- @field PositionOffset vec2?
--- @field SameLine boolean
--- @field Visible boolean
--- @field SetColor fun(self:ExtuiStyledRenderable, a1:GuiColor, a2:vec4)
--- @field SetStyle fun(self:ExtuiStyledRenderable, a1:GuiStyleVar, a2:number, a3:number?)
--- @field Tooltip fun(self:ExtuiStyledRenderable):ExtuiTooltip


--- @class ExtuiTabBar:ExtuiTreeParent
--- @field AutoSelectNewTabs boolean
--- @field FittingPolicyDefault boolean
--- @field FittingPolicyMask boolean
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
--- @field PadOuterX boolean
--- @field PreciseWidths boolean
--- @field Reorderable boolean
--- @field Resizable boolean
--- @field RowBg boolean
--- @field ScrollX boolean
--- @field ScrollY boolean
--- @field SizingFixedFit boolean
--- @field SizingFixedSame boolean
--- @field SizingStretchProp boolean
--- @field SizingStretchSame boolean
--- @field SortMulti boolean
--- @field SortTristate boolean
--- @field Sortable boolean
--- @field AddColumn fun(self:ExtuiTable, a1:string, a2:GuiTableColumnFlags?, a3:number?)
--- @field AddRow fun(self:ExtuiTable):ExtuiTableRow


--- @class ExtuiTableCell:ExtuiTreeParent


--- @class ExtuiTableRow:ExtuiTreeParent
--- @field Headers boolean
--- @field AddCell fun(self:ExtuiTableRow):ExtuiTableCell


--- @class ExtuiText:ExtuiStyledRenderable


--- @class ExtuiTooltip:ExtuiTreeParent


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
--- @field OnClick RegistryEntry
--- @field OnCollapse RegistryEntry
--- @field OnExpand RegistryEntry
--- @field OpenOnArrow boolean
--- @field OpenOnDoubleClick boolean
--- @field Selected boolean
--- @field SpanAllColumns boolean
--- @field SpanAvailWidth boolean
--- @field SpanFullWidth boolean
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
--- @field AddRadioButton fun(self:ExtuiTreeParent, a1:string, a2:boolean?):ExtuiRadioButton
--- @field AddSeparator fun(self:ExtuiTreeParent):ExtuiSeparator
--- @field AddSeparatorText fun(self:ExtuiTreeParent, a1:string):ExtuiSeparatorText
--- @field AddSlider fun(self:ExtuiTreeParent, a1:string, a2:number?, a3:number?, a4:number?):ExtuiSlider
--- @field AddSliderInt fun(self:ExtuiTreeParent, a1:string, a2:int32?, a3:int32?, a4:int32?):ExtuiSliderInt
--- @field AddSpacing fun(self:ExtuiTreeParent):ExtuiSpacing
--- @field AddTabBar fun(self:ExtuiTreeParent, a1:string):ExtuiTabBar
--- @field AddTable fun(self:ExtuiTreeParent, a1:string, a2:uint32):ExtuiTable
--- @field AddText fun(self:ExtuiTreeParent, a1:string):ExtuiText
--- @field AddTree fun(self:ExtuiTreeParent, a1:string):ExtuiTree
--- @field AttachChild fun(self:ExtuiTreeParent, a1:ImguiHandle):boolean
--- @field DetachChild fun(self:ExtuiTreeParent, a1:ImguiHandle):boolean
--- @field RemoveChild fun(self:ExtuiTreeParent, a1:ImguiHandle):boolean


--- @class ExtuiWindow:ExtuiTreeParent
--- @field AlwaysAutoResize boolean
--- @field AlwaysHorizontalScrollbar boolean
--- @field AlwaysVerticalScrollbar boolean
--- @field Closeable boolean
--- @field HorizontalScrollbar boolean
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
--- @field OnClose RegistryEntry
--- @field Open boolean
--- @field UnsavedDocument boolean
--- @field AddMainMenu fun(self:ExtuiWindow):ExtuiMenu
--- @field SetBgAlpha fun(self:ExtuiWindow, a1:number?)
--- @field SetCollapsed fun(self:ExtuiWindow, a1:boolean, a2:GuiCond?)
--- @field SetContentSize fun(self:ExtuiWindow, a1:vec2?)
--- @field SetFocus fun(self:ExtuiWindow)
--- @field SetPos fun(self:ExtuiWindow, a1:vec2, a2:GuiCond?, a3:vec2?)
--- @field SetScroll fun(self:ExtuiWindow, a1:vec2?)
--- @field SetSize fun(self:ExtuiWindow, a1:vec2, a2:GuiCond?)
--- @field SetSizeConstraints fun(self:ExtuiWindow, a1:vec2?, a2:vec2?)


--- @class HealBlockComponent:BaseComponent


--- @class HitAttackerComponent:BaseComponent
--- @field Attacker EntityHandle


--- @class HitProxyComponent:BaseComponent
--- @field Owner EntityHandle
--- @field field_8 FixedString


--- @class HitProxyOwnerComponent:BaseComponent
--- @field Owners EntityHandle[]


--- @class HitReactionComponent:BaseComponent
--- @field Reactions table<EntityHandle, HitReactionComponentReaction>


--- @class HitReactionComponentReaction
--- @field Target HitTargetInfo
--- @field field_0 uint64


--- @class HitTargetComponent:BaseComponent
--- @field Target HitTargetInfo
--- @field field_0 EntityHandle
--- @field field_8 EntityHandle


--- @class HitTargetInfo
--- @field field_0 FixedString
--- @field field_18 vec3
--- @field field_24 int32
--- @field field_28 int32
--- @field field_2C int32
--- @field field_30 uint8
--- @field field_31 uint8
--- @field field_32 uint8
--- @field field_38 uint8[]
--- @field field_4 FixedString
--- @field field_48 uint8
--- @field field_50 uint8[]
--- @field field_60 Array_uint8
--- @field field_8 uint8
--- @field field_90 Guid
--- @field field_A0 uint8
--- @field field_A1 uint8
--- @field field_A2 uint8
--- @field field_A3 uint8
--- @field field_C vec3


--- @class HitThrownObjectComponent:BaseComponent
--- @field ThrownObject EntityHandle


--- @class HitWeaponComponent:BaseComponent
--- @field Weapon EntityHandle


--- @class InterruptActionStateComponent:BaseComponent
--- @field Arr_EHx2 InterruptInterruptEntities[]
--- @field Variant InterruptInterruptVariant2
--- @field field_118 Guid


--- @class InterruptConditionallyDisabledComponent:BaseComponent
--- @field Dummy uint8


--- @class InterruptContainerComponent:BaseComponent
--- @field Interrupts EntityHandle[]


--- @class InterruptDataComponent:BaseComponent
--- @field Interrupt FixedString
--- @field field_10 EntityHandle
--- @field field_18 FixedString
--- @field field_4 uint8
--- @field field_8 EntityHandle


--- @class InterruptDecisionComponent:BaseComponent
--- @field Decisions table<InterruptInterruptVariant2, uint8>


--- @class InterruptExecuteResult
--- @field ResolveData table<Guid, InterruptResolveData>
--- @field field_0 uint8


--- @class InterruptInterruptEntities
--- @field field_0 EntityHandle
--- @field field_8 EntityHandle


--- @class InterruptInterruptIdentifier
--- @field field_0 uint64
--- @field field_10 uint64
--- @field field_8 uint64


--- @class InterruptInterruptType0
--- @field field_0 FixedString
--- @field field_18 SpellId
--- @field field_8 Guid


--- @class InterruptInterruptType1
--- @field field_0 int64
--- @field field_18 int32
--- @field field_1C uint8
--- @field field_20 int32
--- @field field_28 EntityHandle[]
--- @field field_38 EntityHandle
--- @field field_40 SpellId
--- @field field_68 uint8
--- @field field_69 uint8
--- @field field_8 Guid


--- @class InterruptInterruptType2
--- @field field_0 Guid
--- @field field_10 FixedString
--- @field field_18 Guid
--- @field field_28 uint8
--- @field field_2C uint8
--- @field field_2D uint8
--- @field field_30 int32
--- @field field_34 int32
--- @field field_38 int32
--- @field field_3C uint8
--- @field field_3D uint8
--- @field field_40 int32
--- @field field_44 uint8
--- @field field_48 int32
--- @field field_4C uint8
--- @field field_50 uint8
--- @field field_51 uint8
--- @field field_52 uint8
--- @field field_58 Guid
--- @field field_68 SpellId


--- @class InterruptInterruptType3
--- @field field_0 int64
--- @field field_10 int64
--- @field field_18 int64
--- @field field_20 int64
--- @field field_28 int64
--- @field field_30 FixedString
--- @field field_34 uint8
--- @field field_38 int64
--- @field field_40 int64
--- @field field_48 int64
--- @field field_50 int32
--- @field field_54 uint8
--- @field field_55 uint8
--- @field field_8 int64


--- @class InterruptInterruptType4
--- @field field_0 FixedString
--- @field field_4 vec3


--- @class InterruptInterruptType5
--- @field field_0 FixedString
--- @field field_4 vec3


--- @class InterruptInterruptType6
--- @field field_0 Guid
--- @field field_10 FixedString
--- @field field_18 Guid
--- @field field_28 string
--- @field field_40 string
--- @field field_44 int32
--- @field field_48 string
--- @field field_49 string
--- @field field_50 Guid
--- @field field_60 string
--- @field field_68 Guid
--- @field field_78 string
--- @field field_80 SpellId


--- @class InterruptInterruptType7
--- @field field_0 Guid
--- @field field_10 Guid
--- @field field_20 FixedString
--- @field field_28 Guid
--- @field field_38 Guid
--- @field field_48 uint8
--- @field field_50 int64
--- @field field_58 FixedString
--- @field field_5C int32


--- @class InterruptInterruptType8
--- @field field_0 Guid
--- @field field_10 FixedString
--- @field field_18 Guid
--- @field field_28 Guid
--- @field field_38 int32
--- @field field_40 int64
--- @field field_48 string
--- @field field_4C int32


--- @class InterruptInterruptVariant2
--- @field Source EntityHandle
--- @field SourcePos vec3?
--- @field SourceProxy EntityHandle
--- @field Target EntityHandle
--- @field TargetPos vec3?
--- @field TargetProxy EntityHandle
--- @field Variant Variant<InterruptInterruptType0,InterruptInterruptType1,InterruptInterruptType2,InterruptInterruptType3,InterruptInterruptType4,InterruptInterruptType5,InterruptInterruptType6,InterruptInterruptType7,InterruptInterruptType8>
--- @field field_100 uint8
--- @field field_D0 InterruptInterruptEntities[]


--- @class InterruptInterruptVariantContainer
--- @field MHM_EH_MHS_EH table<EntityHandle, Array_EntityHandle>
--- @field Variant InterruptInterruptVariant2
--- @field field_108 boolean
--- @field field_110 InterruptInterruptVariant2


--- @class InterruptPreferencesComponent:BaseComponent
--- @field Preferences table<FixedString, uint8>


--- @class InterruptPreparedComponent:BaseComponent
--- @field Dummy uint8


--- @class InterruptResolveData
--- @field Arr2_2b InterruptResolveDataElement[]
--- @field Arr_2b InterruptResolveDataElement[]
--- @field ResolvedRolls ResolvedUnknown[]
--- @field field_0 int32
--- @field field_18 int32?
--- @field field_20 uint8
--- @field field_48 int32?
--- @field field_50 uint8


--- @class InterruptResolveDataElement
--- @field A uint8
--- @field B uint8


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
--- @field Items table<uint16, InventoryContainerComponentItem>


--- @class InventoryContainerComponentItem
--- @field Item EntityHandle
--- @field field_8 uint32


--- @class InventoryDataComponent:BaseComponent
--- @field Flags uint16
--- @field field_0 uint8


--- @class InventoryDropOnDeathBlockedComponent:BaseComponent


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
--- @field Arr_u8 uint8[]
--- @field Elements EntityHandle[]


--- @class InventoryStackMemberComponent:BaseComponent
--- @field Stack EntityHandle


--- @class InventoryWeightComponent:BaseComponent
--- @field Weight int32


--- @class InventoryWieldedComponent:BaseComponent
--- @field field_0 Guid


--- @class ItemDestroyInfo
--- @field field_0 FixedString
--- @field field_4 uint8


--- @class ItemDestroyedComponent:BaseComponent
--- @field Info ItemDestroyInfo?


--- @class ItemDoorComponent:BaseComponent


--- @class ItemDyeComponent:BaseComponent
--- @field Color Guid


--- @class ItemExamineDisabledComponent:BaseComponent


--- @class ItemHasMovedComponent:BaseComponent


--- @class ItemHasOpenedComponent:BaseComponent


--- @class ItemIsGoldComponent:BaseComponent


--- @class ItemItemComponent:BaseComponent


--- @class ItemMapMarkerStyleComponent:BaseComponent
--- @field Style FixedString


--- @class ItemNewInInventoryComponent:BaseComponent


--- @class ItemPortalComponent:BaseComponent
--- @field field_0 uint8
--- @field field_1 uint8


--- @class ItemShouldDestroyOnSpellCastComponent:BaseComponent


--- @class ItemTemplateActionTypeComponent:BaseComponent
--- @field ActionTypes Array_uint8


--- @class ItemTemplateCanMoveComponent:BaseComponent


--- @class ItemTemplateClimbOnComponent:BaseComponent


--- @class ItemTemplateDestroyedComponent:BaseComponent


--- @class ItemTemplateInteractionDisabledComponent:BaseComponent


--- @class ItemTemplateIsStoryItemComponent:BaseComponent


--- @class ItemTemplateLadderComponent:BaseComponent


--- @class ItemTemplateUseActionComponent:BaseComponent


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


--- @class LuaNetMessageEvent:LuaEventBase
--- @field Channel string
--- @field Payload string
--- @field UserID UserId


--- @class LuaTickEvent:LuaEventBase
--- @field Time GameTime


--- @class OwnershipOwnedAsLootComponent:BaseComponent


--- @class OwnershipOwneeCurrentComponent:BaseComponent
--- @field Ownee EntityHandle


--- @class PartyBlockFollowComponent:BaseComponent


--- @class PartyCompositionComponent:BaseComponent
--- @field Members PartyCompositionComponentMember[]
--- @field Party EntityHandle
--- @field PartyUuid Guid
--- @field field_8 Guid


--- @class PartyCompositionComponentMember
--- @field UserId int32
--- @field UserUuid Guid
--- @field ViewUuid Guid
--- @field field_0 int32
--- @field field_18 Guid
--- @field field_28 uint8[]
--- @field field_8 Guid


--- @class PartyCurrentlyFollowingPartyComponent:BaseComponent


--- @class PartyFollowerComponent:BaseComponent
--- @field Following EntityHandle


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
--- @field Portals Array_EntityHandle


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
--- @field Waypoints Array_PartyWaypoint


--- @class ProgressionContainerComponent:BaseComponent
--- @field Progressions EntityHandle[][]


--- @class ProgressionLevelUpComponent:BaseComponent
--- @field LevelUps LevelUpData[]
--- @field field_18 LevelUpData[]


--- @class ProgressionMetaComponent:BaseComponent
--- @field Owner EntityHandle
--- @field Progression Guid
--- @field Race Guid
--- @field field_18 uint8
--- @field field_40 int32
--- @field field_44 uint8
--- @field field_48 int32


--- @class RecruitRecruiterComponent:BaseComponent
--- @field Recruiters Array_EntityHandle


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
--- @field Spells AbilityId[]


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
--- @field Params table<Guid, ResourceGenomeVariant>
--- @field field_50 int64[]


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
--- @field Properties ResourceAnimationResourceEventPropertiesHolder
--- @field Time number
--- @field Track uint8


--- @class ResourceAnimationResourceEventBoneAttachBoneProperties:ResourceAnimationResourceEventPropertiesHolder
--- @field AttachBone FixedString
--- @field Bone FixedString
--- @field Detach boolean


--- @class ResourceAnimationResourceEventBoneEffectProperties:ResourceAnimationResourceEventPropertiesHolder
--- @field Bone FixedString
--- @field Effect FixedString
--- @field HardAttach boolean
--- @field InterruptLoopAtEnd boolean


--- @class ResourceAnimationResourceEventEFoleySoundObjectProperties:ResourceAnimationResourceEventSoundObjectProperties
--- @field EFoleyIntensity uint8
--- @field EFoleyType uint8


--- @class ResourceAnimationResourceEventEVocalSoundObjectProperties:ResourceAnimationResourceEventSoundObjectProperties
--- @field EVocalType uint8


--- @class ResourceAnimationResourceEventFloatDataProperties:ResourceAnimationResourceEventPropertiesHolder
--- @field Data number


--- @class ResourceAnimationResourceEventFootProperties:ResourceAnimationResourceEventPropertiesHolder
--- @field FootID uint8
--- @field PlayHearingFX boolean
--- @field PlayScuff boolean
--- @field PlaySlideFX boolean
--- @field PlaySound boolean
--- @field PlayStepFX boolean
--- @field Slide boolean


--- @class ResourceAnimationResourceEventProp10Properties:ResourceAnimationResourceEventPropertiesHolder
--- @field Enabled boolean
--- @field VisualFlag uint32


--- @class ResourceAnimationResourceEventProp9Properties:ResourceAnimationResourceEventPropertiesHolder
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


--- @class ResourceAnimationResourceEventPropertiesHolder


--- @class ResourceAnimationResourceEventSoundObjectProperties:ResourceAnimationResourceEventPropertiesHolder
--- @field Resource FixedString
--- @field SoundObjectIndex uint8


--- @class ResourceAnimationResourceEventU8DataProperties:ResourceAnimationResourceEventPropertiesHolder
--- @field Data uint8


--- @class ResourceAnimationResourceEventUnknown15Properties:ResourceAnimationResourceEventPropertiesHolder


--- @class ResourceAnimationResourceEventUnknown7Properties:ResourceAnimationResourceEventPropertiesHolder


--- @class ResourceAnimationResourceEventUnknown8Properties:ResourceAnimationResourceEventPropertiesHolder


--- @class ResourceAnimationResourceEventWeaponEffectProperties:ResourceAnimationResourceEventPropertiesHolder
--- @field EffectType int32
--- @field Weapon int32


--- @class ResourceAnimationSetPriority:ResourceGuidResource
--- @field AddidionalObjects Guid[]
--- @field Name string
--- @field Priority int32


--- @class ResourceAnimationSetResource:ResourceLoadableResource
--- @field AnimationBank ResourceAnimationSetResourceBank


--- @class ResourceAnimationSetResourceBank
--- @field AnimationSubSets table<FixedString, ResourceAnimationSetResourceSubset>
--- @field ShortNameSetId Guid


--- @class ResourceAnimationSetResourceSubset
--- @field Animation table<FixedString, ResourceAnimationSetResourceSubsetAnimations>
--- @field FallBackSubSet FixedString


--- @class ResourceAnimationSetResourceSubsetAnimations
--- @field ID FixedString
--- @field flags AnimationSetAnimationFlags


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
--- @field IsSomaticWithInstrument boolean
--- @field ItemsHotbarColumns int32
--- @field LearningStrategy uint8
--- @field MulticlassSpellcasterModifier number
--- @field MustPrepareSpells boolean
--- @field Name FixedString
--- @field ParentGuid Guid
--- @field PrimaryAbility uint8
--- @field ProgressionTableUUID Guid
--- @field SoundClassType FixedString
--- @field SpellCastingAbility uint8
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
--- @field Flags uint32
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
--- @field CullingDistance number
--- @field Dependencies FixedString[]
--- @field Duration number
--- @field EffectComponents ResourceEffectsEffectComponentBase[]
--- @field EffectName FixedString
--- @field InterruptionMode uint32
--- @field Looping boolean
--- @field TimelineContent int64
--- @field TimelineContentEffect ResourceEffectResourceTimelineContentEffects
--- @field UseSoundOcclusion boolean


--- @class ResourceEffectResourceTimelineContentEffects
--- @field Duration number
--- @field EffectComponents ResourceEffectsEffectComponentBase[]
--- @field EffectName FixedString
--- @field Inputs ResourceEffectResourceTimelineContentEffectsInput[]
--- @field Phases ResourceEffectResourceTimelineContentEffectsPhase[]
--- @field filled boolean


--- @class ResourceEffectResourceTimelineContentEffectsInput
--- @field Input ResourceEffectsProperty
--- @field Name FixedString


--- @class ResourceEffectResourceTimelineContentEffectsPhase
--- @field DialogNodeId Guid
--- @field Duration number
--- @field IsOverridingTimelineQuestionHoldAutomationSettings boolean
--- @field PlayCount int32
--- @field QuestionHoldAutomation ResourceEffectResourceTimelineContentEffectsPhaseQuestionHoldAutomationSettings
--- @field StartTime number
--- @field field_14 int32


--- @class ResourceEffectResourceTimelineContentEffectsPhaseQuestionHoldAutomationSettings
--- @field CycleSpeed number
--- @field CycleSpeedDeviation number
--- @field IsEnabled boolean
--- @field StartOffset number
--- @field StartOffsetDeviation number


--- @class ResourceEncumbranceType:ResourceGuidResource
--- @field Status FixedString


--- @class ResourceEquipmentDefaultValues:ResourceDefaultValueGuidResource
--- @field Add FixedString[]


--- @class ResourceEquipmentList:ResourceGuidResource
--- @field Items FixedString[]


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
--- @field Boosts string
--- @field CanBeTakenMultipleTimes boolean
--- @field Name FixedString
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


--- @class ResourceGenomeVariant
--- @field Type int64
--- @field Value int64


--- @class ResourceGod:ResourceGuidResource
--- @field Description TranslatedString
--- @field DisplayName TranslatedString
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
--- @field RandomRotation number


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
--- @field Lighting ResourceLightingResourceDetails


--- @class ResourceLightingResourceDetails
--- @field Fog ResourceLightingResourceDetailsFogSettings
--- @field Moon ResourceLightingResourceDetailsMoonDetails
--- @field ParentGUID FixedString
--- @field SSAOSettings ResourceLightingResourceDetailsDetailsSSAOSettings
--- @field SkyLight ResourceLightingResourceDetailsSkyLightDetails
--- @field Sun ResourceLightingResourceDetailsSunDetails
--- @field TimelineFog ResourceLightingResourceDetailsFogSettings
--- @field TimelineFogOverride boolean
--- @field VolumetricCloudSettings ResourceLightingResourceDetailsDetailsVolumetricCloudSettings
--- @field field_368 int64
--- @field field_370 int64


--- @class ResourceLightingResourceDetailsDetailsCoverageSettings
--- @field EndHeight number
--- @field HorizonDistance number
--- @field Offset vec2
--- @field StartHeight number
--- @field TexResourceGUID FixedString


--- @class ResourceLightingResourceDetailsDetailsSSAOSettings
--- @field Bias number
--- @field DirectLightInfluence number
--- @field Enabled boolean
--- @field Intensity number
--- @field Radius number


--- @class ResourceLightingResourceDetailsDetailsVolumetricCloudSettings
--- @field AmbientLightFactor number
--- @field BackwardScatteringG number
--- @field BaseColor vec3
--- @field BottomFade number
--- @field ConeRadius number
--- @field CoverageEndDistance number
--- @field CoverageSettings ResourceLightingResourceDetailsDetailsCoverageSettings
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


--- @class ResourceLightingResourceDetailsFogLayer
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


--- @class ResourceLightingResourceDetailsFogSettings
--- @field FogLayer0 ResourceLightingResourceDetailsFogLayer
--- @field FogLayer1 ResourceLightingResourceDetailsFogLayer
--- @field Phase number
--- @field RenderDistance number


--- @class ResourceLightingResourceDetailsMoonDetails
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


--- @class ResourceLightingResourceDetailsSkyLightDetails
--- @field CirrusCloudsAmount number
--- @field CirrusCloudsColor vec3
--- @field CirrusCloudsEnabled_FixedString boolean
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


--- @class ResourceLightingResourceDetailsSunDetails
--- @field CascadeCount uint8
--- @field CascadeSpeed number
--- @field ColorAdjustedForIntensity vec3
--- @field ColorTemperatureAdjustment vec3
--- @field CoverageSettings ResourceLightingResourceDetailsDetailsCoverageSettings
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
--- @field Instance MaterialInstance
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
--- @field Boosts string
--- @field IsMulticlass boolean
--- @field Level uint8
--- @field Name string
--- @field PassivesAdded string
--- @field PassivesRemoved string
--- @field ProgressionType uint8
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
--- @field PreviewAnimationResource FixedString
--- @field RagdollResourceID FixedString
--- @field Sockets ResourceSkeletonResourceSocket[]
--- @field SoftbodyResourceID FixedString
--- @field SpringResourceID FixedString
--- @field Template FixedString
--- @field field_98 int64


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
--- @field Skills SkillId[]


--- @class ResourceSkinPresetResource:ResourceLoadableResource
--- @field Presets ResourcePresetData


--- @class ResourceSoundResource:ResourceTwoStepLoadableResource
--- @field Duration number
--- @field Flags uint8
--- @field MaxDistance number
--- @field SoundCodec uint8
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
--- @field Spells Array_FixedString


--- @class ResourceSpellMetaCondition:ResourceGuidResource
--- @field AdditionalConditions string
--- @field ConditionType uint8
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
--- @field Type uint8
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
--- @field GTSHandle uint32
--- @field GTSSuffix string?
--- @field GTexFileName FixedString
--- @field LoadedField52 boolean
--- @field LoadedVirtualTextureLayerConfig uint32
--- @field Prefetch boolean
--- @field PrefetchMipLevel int8
--- @field ReferencedColorSpaces uint32
--- @field VirtualTextureLayerConfig uint32
--- @field field_52 uint8
--- @field field_53 uint8


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
--- @field ClothColliderResourceID FixedString
--- @field ClothParams ResourceVisualResourceClothParam[]
--- @field ClothProxyMapping table<FixedString, ResourceVisualResourceClothProxy[]>
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


--- @class ResourceVisualResourceClothProxy
--- @field Name FixedString


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


--- @class ResourceEffectsBaseEffectComponent:ResourceEffectsEffectComponentBase
--- @field Modules ResourceEffectsBaseEffectComponentModule[]
--- @field Properties ResourceEffectsProperty[]
--- @field PropertiesByFullName table<FixedString, ResourceEffectsProperty>
--- @field field_48 int64
--- @field field_50 int64
--- @field field_58 int64
--- @field field_60 int64
--- @field field_68 int64
--- @field field_a8 int64
--- @field field_b0 int64


--- @class ResourceEffectsBaseEffectComponentModule
--- @field FullName FixedString[]
--- @field Name string
--- @field Properties ResourceEffectsProperty[]


--- @class ResourceEffectsBaseProperty:ResourceEffectsProperty


--- @class ResourceEffectsBillboardComponent:ResourceEffectsBaseEffectComponent
--- @field field_b8 int64[]


--- @class ResourceEffectsBoolProperty:ResourceEffectsProperty
--- @field Value boolean


--- @class ResourceEffectsBoundingBoxComponent:ResourceEffectsBaseEffectComponent
--- @field field_b8 int64[]


--- @class ResourceEffectsBoundingSphereComponent:ResourceEffectsBaseEffectComponent
--- @field field_b8 int64[]


--- @class ResourceEffectsCameraShakeComponent:ResourceEffectsBaseEffectComponent
--- @field field_b8 int64[]


--- @class ResourceEffectsColoredFramesProperty:ResourceEffectsProperty
--- @field Frames ResourceEffectsColoredFramesPropertyFramesArray


--- @class ResourceEffectsColoredFramesPropertyFramesArray
--- @field data ResourceEffectsColoredFramesPropertyFramesArrayFrame[]


--- @class ResourceEffectsColoredFramesPropertyFramesArrayFrame
--- @field Color vec4
--- @field Time number


--- @class ResourceEffectsDecalComponent:ResourceEffectsBaseEffectComponent
--- @field field_b8 int64[]


--- @class ResourceEffectsDeflectorComponent:ResourceEffectsBaseEffectComponent
--- @field field_b8 int64[]


--- @class ResourceEffectsDragForceComponent:ResourceEffectsBaseEffectComponent
--- @field field_b8 int64[]


--- @class ResourceEffectsEffectComponentBase
--- @field EndTime number
--- @field GUID Guid
--- @field Name string
--- @field StartTime number
--- @field Track uint32
--- @field field_30 int64
--- @field field_44 int32


--- @class ResourceEffectsFixedStringProperty:ResourceEffectsProperty
--- @field Value FixedString


--- @class ResourceEffectsFloatProperty:ResourceEffectsProperty
--- @field Value number


--- @class ResourceEffectsFloatRangeProperty:ResourceEffectsProperty
--- @field Max number
--- @field Min number


--- @class ResourceEffectsFramesProperty:ResourceEffectsProperty
--- @field Frames ResourceEffectsFramesPropertyVirtualFrames[]


--- @class ResourceEffectsFramesPropertyFrames0:ResourceEffectsFramesPropertyVirtualFrames
--- @field Frames ResourceEffectsFramesPropertyFrames0Frame[]


--- @class ResourceEffectsFramesPropertyFrames0Frame
--- @field Time number
--- @field Value number
--- @field field_0 int64
--- @field field_14 int32
--- @field field_c int32


--- @class ResourceEffectsFramesPropertyFrames1:ResourceEffectsFramesPropertyVirtualFrames
--- @field Frames ResourceEffectsFramesPropertyFrames1Frame[]


--- @class ResourceEffectsFramesPropertyFrames1Frame
--- @field A number
--- @field B number
--- @field C number
--- @field D number
--- @field Time number
--- @field field_0 int64
--- @field field_c int32


--- @class ResourceEffectsFramesPropertyVirtualFrames


--- @class ResourceEffectsFunctionTypeProperty:ResourceEffectsProperty
--- @field Function ResourceEffectsFunctionTypePropertyVirtualFunction


--- @class ResourceEffectsFunctionTypePropertyFunction0:ResourceEffectsFunctionTypePropertyVirtualFunction
--- @field A number
--- @field B number


--- @class ResourceEffectsFunctionTypePropertyFunction1:ResourceEffectsFunctionTypePropertyVirtualFunction
--- @field A number
--- @field B number
--- @field C number


--- @class ResourceEffectsFunctionTypePropertyFunction2:ResourceEffectsFunctionTypePropertyVirtualFunction
--- @field A number
--- @field B number
--- @field C number
--- @field D number


--- @class ResourceEffectsFunctionTypePropertyVirtualFunction


--- @class ResourceEffectsGravityForceComponent:ResourceEffectsBaseEffectComponent
--- @field field_b8 int64[]


--- @class ResourceEffectsInt32Property:ResourceEffectsProperty
--- @field Value int32


--- @class ResourceEffectsInt32RangeProperty:ResourceEffectsProperty
--- @field Max int32
--- @field Min int32


--- @class ResourceEffectsLightComponent:ResourceEffectsBaseEffectComponent
--- @field field_b8 int64[]


--- @class ResourceEffectsModelComponent:ResourceEffectsBaseEffectComponent
--- @field field_b8 int64[]
--- @field field_d8 int32
--- @field field_dc FixedString
--- @field field_e0 FixedString
--- @field field_e4 FixedString
--- @field field_e8 int64[]


--- @class ResourceEffectsMovingLevelComponent:ResourceEffectsBaseEffectComponent
--- @field field_b8 string
--- @field field_d0 int64[]


--- @class ResourceEffectsOrbitForceComponent:ResourceEffectsBaseEffectComponent
--- @field field_b8 int64[]


--- @class ResourceEffectsOverlayMaterialComponent:ResourceEffectsBaseEffectComponent
--- @field field_108 FixedString
--- @field field_10c int32
--- @field field_110 int64[]
--- @field field_b8 int64[]


--- @class ResourceEffectsParticleSystemComponent:ResourceEffectsBaseEffectComponent
--- @field ParticleData ResourceEffectsParticleSystemComponentParticleDatas
--- @field field_4d0 int64[]
--- @field field_b8 int64[]


--- @class ResourceEffectsParticleSystemComponentParticleDatas
--- @field Ages number[]
--- @field AppliedForces vec3[]
--- @field Count int32
--- @field DiscardValues number[]
--- @field FixedColors uint32[]
--- @field FixedScalars uint16[]
--- @field FlipbookImageIndices uint16[]
--- @field InitialRotationSpeeds uint16[]
--- @field Lifespan number[]
--- @field Normals vec3[]
--- @field Positions vec3[]
--- @field Rotations uint16[]
--- @field Scales vec3[]
--- @field UnitAges number[]
--- @field UpVectors vec3[]
--- @field Velocities vec3[]
--- @field field_0 int32
--- @field field_88 int64
--- @field field_90 int64


--- @class ResourceEffectsPostProcessComponent:ResourceEffectsBaseEffectComponent
--- @field field_b8 int64[]


--- @class ResourceEffectsPreRollComponent:ResourceEffectsBaseEffectComponent


--- @class ResourceEffectsProperty
--- @field AttributeName FixedString
--- @field FullName FixedString
--- @field Input FixedString
--- @field Parameters ResourceEffectsPropertyParameter[]


--- @class ResourceEffectsPropertyParameter
--- @field Name string
--- @field Value number


--- @class ResourceEffectsRadialForceComponent:ResourceEffectsBaseEffectComponent
--- @field field_b8 int64[]


--- @class ResourceEffectsRibbon2Component:ResourceEffectsBaseEffectComponent
--- @field field_b8 int64[]


--- @class ResourceEffectsSTDStringProperty:ResourceEffectsProperty
--- @field Value string


--- @class ResourceEffectsSoundComponent:ResourceEffectsBaseEffectComponent
--- @field field_100 int64[]
--- @field field_b8 int64[]
--- @field field_f0 FixedString
--- @field field_f4 FixedString
--- @field field_f8 FixedString
--- @field field_fc int32


--- @class ResourceEffectsSpinForceComponent:ResourceEffectsBaseEffectComponent
--- @field field_b8 int64[]


--- @class ResourceEffectsTurbulentForceComponent:ResourceEffectsBaseEffectComponent
--- @field field_b8 int64[]


--- @class ResourceEffectsVector3Property:ResourceEffectsProperty
--- @field Value vec3


--- @class ResourceEffectsVortexForceComponent:ResourceEffectsBaseEffectComponent
--- @field field_b8 int64[]


--- @class ResourceEffectsWindForceComponent:ResourceEffectsBaseEffectComponent
--- @field field_b8 int64[]


--- @class ResourceEffectsTimelineActor
--- @field PeanutOverride boolean
--- @field UUID Guid


--- @class ResourceEffectsTimelineTLAdditiveAnimationComponent:ResourceEffectsTimelineTLAnimationComponent


--- @class ResourceEffectsTimelineTLAnimationComponent:ResourceEffectsTimelineTLBaseComponentComponent
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
--- @field TargetTransform ResourceEffectsTimelineTLAnimationComponentTargetTransforms
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


--- @class ResourceEffectsTimelineTLAnimationComponentTargetTransforms
--- @field Position vec3
--- @field RotationQuat vec4
--- @field Scale number
--- @field field_1c number
--- @field field_20 number


--- @class ResourceEffectsTimelineTLAtmosphereAndLightingComponent:ResourceEffectsTimelineTLBaseComponentComponent
--- @field Channels ResourceEffectsTimelineChannelsTLAtmosphereAndLightingChannelComponent[]


--- @class ResourceEffectsTimelineTLAttachToEventComponent:ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysAttachToEventKey


--- @class ResourceEffectsTimelineTLAttitudeEventComponent:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysAttitudeEventKey
--- @field field_b0 FixedString
--- @field field_b4 int32


--- @class ResourceEffectsTimelineTLBaseComponentComponent:ResourceEffectsEffectComponentBase
--- @field Actor ResourceEffectsTimelineActor
--- @field IsMimicry boolean
--- @field IsSnappedToEnd boolean
--- @field PhaseIndex int64
--- @field field_68 int32
--- @field type_integer int32


--- @class ResourceEffectsTimelineTLCameraDoFComponent:ResourceEffectsTimelineTLBaseComponentComponent
--- @field Channels ResourceEffectsTimelineChannelsTLCameraDoFChannelComponent[]


--- @class ResourceEffectsTimelineTLCameraExposureComponent:ResourceEffectsTimelineTLBaseComponentComponent
--- @field Channels ResourceEffectsTimelineChannelsTLCameraExposureChannelComponent[]
--- @field field_90 int64


--- @class ResourceEffectsTimelineTLCameraFoVComponent:ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysCameraFoVKey


--- @class ResourceEffectsTimelineTLCameraLookAtComponent:ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysCameraLookAtKey


--- @class ResourceEffectsTimelineTLEffectPhaseEventComponent:ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysEffectPhaseEventKey


--- @class ResourceEffectsTimelineTLEmotionEventComponent:ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysEmotionEventKey


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysEffectPhaseEventKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysEffectPhaseEventKey


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysEmotionEventKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysEmotionEventKey


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysGenomeTextEventKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysGenomeTextEventKey


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysHandsIKKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysHandsIKKey


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysKeyBase:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysKeyBase


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysLookAtEventKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysLookAtEventKey


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysPhysicsKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysPhysicsKey


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysPlayEffectEventKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysPlayEffectEventKey


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysShapeShiftKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysShapeShiftKey


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysShowArmorChannelKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysShowArmorChannelKey


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysShowHUDKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysShowHUDKey


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysShowPeanutsKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysShowPeanutsKey


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysShowVisualKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysShowVisualKey


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysShowWeaponKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysShowWeaponKey


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysSoundEventKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysSoundEventKey


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysSpringsKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysSpringsKey


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysSteppingFadeKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysSteppingFadeKey


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysSwitchLocationEventKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysSwitchLocationEventKey


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysSwitchStageEventKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysSwitchStageEventKey


--- @class ResourceEffectsTimelineTLFloatRTPCComponent:ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysFloatRTPCKey
--- @field RTPCName FixedString
--- @field SoundObjectIndex uint8
--- @field SoundType uint8
--- @field field_b0 int64


--- @class ResourceEffectsTimelineTLGenomeTextEventComponent:ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysGenomeTextEventKey


--- @class ResourceEffectsTimelineTLHandsIKComponent:ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysHandsIKKey


--- @class ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysAtmosphereAndLightingChannelKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysAtmosphereAndLightingChannelKey


--- @class ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysAttachToEventKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysAttachToEventKey


--- @class ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysCameraDoFChannelKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysCameraDoFChannelKey


--- @class ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysCameraFoVKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysCameraFoVKey


--- @class ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysCameraLookAtKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysCameraLookAtKey


--- @class ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysFloatRTPCKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysFloatRTPCKey


--- @class ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysKeyBase:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysKeyBase


--- @class ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysMaterialTextureKeyKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysMaterialTextureKeyKey


--- @class ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysSplatterChannelKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysSplatterChannelKey


--- @class ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysTransformChannelFloatKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysTransformChannelFloatKey


--- @class ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysTransformChannelFrameOfReferenceKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysTransformChannelFrameOfReferenceKey


--- @class ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysTransformChannelQuatKey:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysTransformChannelQuatKey


--- @class ResourceEffectsTimelineTLInterpolationKeyComponentComponent<ResourceEffectsTimelineKeysValueKey_float>:ResourceEffectsTimelineTLKeyBaseComponentComponent<ResourceEffectsTimelineKeysValueKey_float>


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysAtmosphereAndLightingChannelKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysAttachToEventKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysAttitudeEventKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysCameraDoFChannelKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysCameraFoVKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysCameraLookAtKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysEffectPhaseEventKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysEmotionEventKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysFloatRTPCKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysGenomeTextEventKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysHandsIKKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysKeyBase:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysLookAtEventKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysMaterialTextureKeyKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysPhysicsKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysPlayEffectEventKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysPlayRateKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysShapeShiftKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysShowArmorChannelKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysShowHUDKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysShowPeanutsKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysShowVisualKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysShowWeaponKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysSoundEventKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysSplatterChannelKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysSpringsKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysSteppingFadeKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysSwitchLocationEventKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysSwitchStageEventKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysTransformChannelFloatKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysTransformChannelFrameOfReferenceKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysTransformChannelQuatKey:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent<ResourceEffectsTimelineKeysValueKey_float>:ResourceEffectsTimelineTLBaseComponentComponent
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 string
--- @field field_a1 string
--- @field field_a2 int16
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32


--- @class ResourceEffectsTimelineTLLayeredAnimationComponent:ResourceEffectsTimelineTLAnimationComponent


--- @class ResourceEffectsTimelineTLLookAtEventComponent:ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysLookAtEventKey


--- @class ResourceEffectsTimelineTLMaterialComponent:ResourceEffectsTimelineTLBaseComponentComponent
--- @field GroupId Guid
--- @field IsContinuous boolean
--- @field IsOverlay boolean
--- @field MaterialParameters ResourceEffectsTimelineTLMaterialComponentParameter[]
--- @field OverlayPriority number
--- @field field_15a int16


--- @class ResourceEffectsTimelineTLMaterialComponentMaterialParameter:ResourceEffectsTimelineTLMaterialComponentParameter


--- @class ResourceEffectsTimelineTLMaterialComponentParameter
--- @field MaterialParameterName FixedString
--- @field field_10 int64
--- @field field_c int32


--- @class ResourceEffectsTimelineTLMaterialComponentParameterRange
--- @field begin ResourceEffectsEffectComponentBase
--- @field end ResourceEffectsEffectComponentBase


--- @class ResourceEffectsTimelineTLMaterialComponentTextureParameter:ResourceEffectsTimelineTLMaterialComponentParameter
--- @field IsVirtual boolean
--- @field TextureParam boolean
--- @field field_1a int16
--- @field field_1c int32


--- @class ResourceEffectsTimelineTLPhysicsComponent:ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysPhysicsKey


--- @class ResourceEffectsTimelineTLPlayEffectEventComponent:ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysPlayEffectEventKey
--- @field field_b0 int64


--- @class ResourceEffectsTimelineTLPlayRateComponent:ResourceEffectsTimelineTLKeyBaseComponentComponent_ResourceEffectsTimelineKeysPlayRateKey


--- @class ResourceEffectsTimelineTLShapeShiftComponent:ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysShapeShiftKey


--- @class ResourceEffectsTimelineTLShotComponent:ResourceEffectsTimelineTLBaseComponentComponent
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


--- @class ResourceEffectsTimelineTLShotHoldPreviousComponent:ResourceEffectsTimelineTLBaseComponentComponent


--- @class ResourceEffectsTimelineTLShotZoomComponent:ResourceEffectsTimelineTLBaseComponentComponent
--- @field EndOffset number
--- @field StartOffset number


--- @class ResourceEffectsTimelineTLShowArmorComponent:ResourceEffectsTimelineTLBaseComponentComponent
--- @field Channels ResourceEffectsTimelineChannelsTLShowArmorChannelComponent[]
--- @field field_80 int64
--- @field field_98 int64


--- @class ResourceEffectsTimelineTLShowHUDComponent:ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysShowHUDKey


--- @class ResourceEffectsTimelineTLShowPeanutsComponent:ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysShowPeanutsKey


--- @class ResourceEffectsTimelineTLShowVisualComponent:ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysShowVisualKey


--- @class ResourceEffectsTimelineTLShowWeaponComponent:ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysShowWeaponKey


--- @class ResourceEffectsTimelineTLSoundEventComponent:ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysSoundEventKey
--- @field RTPCChannels ResourceEffectsTimelineTLFloatRTPCComponent[]


--- @class ResourceEffectsTimelineTLSplatterComponent:ResourceEffectsTimelineTLBaseComponentComponent
--- @field Channels ResourceEffectsTimelineChannelsTLSplatterChannelComponent[]


--- @class ResourceEffectsTimelineTLSpringsComponent:ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysSpringsKey
--- @field VisualFlag uint32
--- @field field_b4 int32


--- @class ResourceEffectsTimelineTLSteppingFadeComponent:ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysSteppingFadeKey


--- @class ResourceEffectsTimelineTLSwitchLocationEventComponent:ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysSwitchLocationEventKey


--- @class ResourceEffectsTimelineTLSwitchStageEventComponent:ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysSwitchStageEventKey
--- @field field_b0 int64
--- @field field_b8 int64


--- @class ResourceEffectsTimelineTLTransformComponent:ResourceEffectsTimelineTLBaseComponentComponent
--- @field Continuous boolean
--- @field field_80 ResourceEffectsTimelineChannelsTLTransformChannelFloatComponent
--- @field field_88 ResourceEffectsTimelineChannelsTLTransformChannelFloatComponent
--- @field field_90 ResourceEffectsTimelineChannelsTLTransformChannelFloatComponent
--- @field field_98 ResourceEffectsTimelineChannelsTLTransformChannelQuatComponent
--- @field field_a0 ResourceEffectsTimelineChannelsTLTransformChannelFloatComponent
--- @field field_a8 ResourceEffectsTimelineChannelsTLTransformChannelFrameOfReferenceComponent
--- @field field_b1 string
--- @field field_b2 int16
--- @field field_b4 int32


--- @class ResourceEffectsTimelineTLVoiceComponent:ResourceEffectsTimelineTLBaseComponentComponent
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


--- @class ResourceEffectsTimelineTimelineActorPropertiesReflectionComponent:ResourceEffectsTimelineTLBaseComponentComponent
--- @field PropertyParameters ResourceEffectsTimelineTimelineActorPropertiesReflectionComponentParameter[]
--- @field field_80 int64


--- @class ResourceEffectsTimelineTimelineActorPropertiesReflectionComponentParameter
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


--- @class ResourceEffectsTimelineTimelineActorPropertiesReflectionComponentParameterRange
--- @field begin ResourceEffectsTimelineChannelsTimelineActorPropertiesReflectionKeyComponentComponent
--- @field end ResourceEffectsTimelineChannelsTimelineActorPropertiesReflectionKeyComponentComponent


--- @class ResourceEffectsTimelineChannelsTLAtmosphereAndLightingChannelComponent:ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysAtmosphereAndLightingChannelKey
--- @field field_b0 int64
--- @field field_b8 int64
--- @field field_c0 int64


--- @class ResourceEffectsTimelineChannelsTLCameraDoFChannelComponent:ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysCameraDoFChannelKey
--- @field channelNum uint8
--- @field field_b0 number
--- @field field_b5 string
--- @field field_b6 int16


--- @class ResourceEffectsTimelineChannelsTLCameraExposureChannelComponent:ResourceEffectsTimelineChannelsTLCameraDoFChannelComponent


--- @class ResourceEffectsTimelineChannelsTLMaterialKeyComponentComponent:ResourceEffectsTimelineTLInterpolationKeyComponentComponent<ResourceEffectsTimelineKeysValueKey_float>
--- @field defaultVal number
--- @field field_b4 int32


--- @class ResourceEffectsTimelineChannelsTLMaterialTextureKeyComponentComponent:ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysMaterialTextureKeyKey
--- @field field_b0 int64
--- @field field_b8 int64


--- @class ResourceEffectsTimelineChannelsTLShowArmorChannelComponent:ResourceEffectsTimelineTLEventKeyComponentComponent_ResourceEffectsTimelineKeysShowArmorChannelKey
--- @field field_b0 int64


--- @class ResourceEffectsTimelineChannelsTLSplatterChannelComponent:ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysSplatterChannelKey
--- @field SplatterType uint8
--- @field field_b0 int64


--- @class ResourceEffectsTimelineChannelsTLTransformChannelFloatComponent:ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysTransformChannelFloatKey
--- @field defaultVal number
--- @field field_b4 int32
--- @field field_b8 int64[]


--- @class ResourceEffectsTimelineChannelsTLTransformChannelFrameOfReferenceComponent:ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysTransformChannelFrameOfReferenceKey
--- @field defaultVal ResourceEffectsTimelineKeysTransformChannelFrameOfReferenceKeyFrameOfReference
--- @field field_b8 int64[]


--- @class ResourceEffectsTimelineChannelsTLTransformChannelQuatComponent:ResourceEffectsTimelineTLInterpolationKeyComponentComponent_ResourceEffectsTimelineKeysTransformChannelQuatKey
--- @field defaultVal vec4
--- @field field_c0 int64[]


--- @class ResourceEffectsTimelineChannelsTimelineActorPropertiesReflectionKeyComponentComponent:ResourceEffectsTimelineTLInterpolationKeyComponentComponent<ResourceEffectsTimelineKeysValueKey_float>
--- @field KeyDefault number


--- @class ResourceEffectsTimelineKeysAtmosphereAndLightingChannelKey:ResourceEffectsTimelineKeysKeyBase
--- @field fadeTime number
--- @field id Guid


--- @class ResourceEffectsTimelineKeysAttachToEventKey:ResourceEffectsTimelineKeysKeyBase
--- @field Bone FixedString
--- @field IsPersistent boolean
--- @field Target Guid
--- @field offset vec3


--- @class ResourceEffectsTimelineKeysAttitudeEventKey:ResourceEffectsTimelineKeysKeyBase
--- @field Pose FixedString
--- @field Transition FixedString


--- @class ResourceEffectsTimelineKeysCameraDoFChannelKey:ResourceEffectsTimelineKeysKeyBase
--- @field Value number


--- @class ResourceEffectsTimelineKeysCameraFoVKey:ResourceEffectsTimelineKeysKeyBase
--- @field FoV number


--- @class ResourceEffectsTimelineKeysCameraLookAtKey:ResourceEffectsTimelineKeysKeyBase
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


--- @class ResourceEffectsTimelineKeysEffectPhaseEventKey:ResourceEffectsTimelineKeysKeyBase
--- @field EffectPhase int32


--- @class ResourceEffectsTimelineKeysEmotionEventKey:ResourceEffectsTimelineKeysKeyBase
--- @field AppliesMaterials boolean
--- @field Emotion int32
--- @field IsSustainedEmotion boolean
--- @field Variation int32


--- @class ResourceEffectsTimelineKeysFloatRTPCKey:ResourceEffectsTimelineKeysKeyBase
--- @field FloatRTPCValue number


--- @class ResourceEffectsTimelineKeysGenomeTextEventKey:ResourceEffectsTimelineKeysKeyBase
--- @field EventName FixedString


--- @class ResourceEffectsTimelineKeysHandsIKKey:ResourceEffectsTimelineKeysKeyBase
--- @field InverseKinematics boolean


--- @class ResourceEffectsTimelineKeysKeyBase
--- @field InterpolationType uint8
--- @field Time number


--- @class ResourceEffectsTimelineKeysLookAtEventKey:ResourceEffectsTimelineKeysKeyBase
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


--- @class ResourceEffectsTimelineKeysMaterialTextureKeyKey:ResourceEffectsTimelineKeysKeyBase
--- @field Value Guid


--- @class ResourceEffectsTimelineKeysPhysicsKey:ResourceEffectsTimelineKeysKeyBase
--- @field InverseKinematics boolean


--- @class ResourceEffectsTimelineKeysPlayEffectEventKey:ResourceEffectsTimelineKeysKeyBase
--- @field PlayEffect boolean


--- @class ResourceEffectsTimelineKeysPlayRateKey:ResourceEffectsTimelineKeysKeyBase
--- @field Speed number


--- @class ResourceEffectsTimelineKeysShapeShiftKey:ResourceEffectsTimelineKeysKeyBase
--- @field TemplateId Guid


--- @class ResourceEffectsTimelineKeysShowArmorChannelKey:ResourceEffectsTimelineKeysKeyBase
--- @field Value boolean


--- @class ResourceEffectsTimelineKeysShowHUDKey:ResourceEffectsTimelineKeysKeyBase
--- @field ShowHUD boolean


--- @class ResourceEffectsTimelineKeysShowPeanutsKey:ResourceEffectsTimelineKeysKeyBase
--- @field ShowPeanuts boolean


--- @class ResourceEffectsTimelineKeysShowVisualKey:ResourceEffectsTimelineKeysKeyBase
--- @field ShowVisual boolean


--- @class ResourceEffectsTimelineKeysShowWeaponKey:ResourceEffectsTimelineKeysKeyBase
--- @field ShowWeapon boolean


--- @class ResourceEffectsTimelineKeysSoundEventKey:ResourceEffectsTimelineKeysKeyBase
--- @field data Variant<ResourceEffectsTimelineKeysSoundEventKeyType1,ResourceEffectsTimelineKeysSoundEventKeyType2,ResourceEffectsTimelineKeysSoundEventKeyType2,ResourceEffectsTimelineKeysSoundEventKeyType2,ResourceEffectsTimelineKeysSoundEventKeyType4,ResourceEffectsTimelineKeysSoundEventKeyType5>


--- @class ResourceEffectsTimelineKeysSoundEventKeyType1
--- @field KeepAlive boolean
--- @field LoopLifetime number
--- @field SoundEventID Guid
--- @field SoundObjectIndex uint8
--- @field field_2a int16


--- @class ResourceEffectsTimelineKeysSoundEventKeyType2
--- @field SoundEventID Guid


--- @class ResourceEffectsTimelineKeysSoundEventKeyType4
--- @field FoleyIntensity uint8
--- @field FoleyType uint8
--- @field SoundObjectIndex uint8


--- @class ResourceEffectsTimelineKeysSoundEventKeyType5
--- @field SoundObjectIndex uint8
--- @field VocalType uint8


--- @class ResourceEffectsTimelineKeysSplatterChannelKey:ResourceEffectsTimelineKeysKeyBase
--- @field SplatterChangeMode uint8
--- @field Value number


--- @class ResourceEffectsTimelineKeysSpringsKey:ResourceEffectsTimelineKeysKeyBase
--- @field EnableSprings boolean


--- @class ResourceEffectsTimelineKeysSteppingFadeKey:ResourceEffectsTimelineKeysKeyBase
--- @field SteppingInOut boolean


--- @class ResourceEffectsTimelineKeysSwitchLocationEventKey:ResourceEffectsTimelineKeysKeyBase
--- @field SwitchLocationEventID Guid
--- @field s_LevelTemplateID Guid


--- @class ResourceEffectsTimelineKeysSwitchStageEventKey:ResourceEffectsTimelineKeysKeyBase
--- @field ForceTransformUpdate boolean
--- @field ForceUpdateCameraBehavior boolean
--- @field SwitchStageEventID Guid


--- @class ResourceEffectsTimelineKeysTransformChannelFloatKey:ResourceEffectsTimelineKeysKeyBase
--- @field Value number


--- @class ResourceEffectsTimelineKeysTransformChannelFrameOfReferenceKey:ResourceEffectsTimelineKeysKeyBase
--- @field frameOfReference ResourceEffectsTimelineKeysTransformChannelFrameOfReferenceKeyFrameOfReference


--- @class ResourceEffectsTimelineKeysTransformChannelFrameOfReferenceKeyFrameOfReference
--- @field KeepScale boolean
--- @field OneFrameOnly boolean
--- @field targetBone FixedString
--- @field targetId Guid


--- @class ResourceEffectsTimelineKeysTransformChannelQuatKey:ResourceEffectsTimelineKeysKeyBase
--- @field Value vec4


--- @class ResourceEffectsTimelineKeysValueKey_float:ResourceEffectsTimelineKeysKeyBase
--- @field Value number


--- @class SightDataComponent:BaseComponent
--- @field field_0 Guid
--- @field field_10 number
--- @field field_14 number
--- @field field_18 number
--- @field field_1C number
--- @field field_20 int32
--- @field field_24 int32


--- @class SightEntityViewshedComponent:BaseComponent
--- @field field_0 Array_Guid


--- @class SightSightBaseComponent:BaseComponent
--- @field field_0 number
--- @field field_4 number
--- @field field_8 number
--- @field field_C number


--- @class SpellAddedSpellsComponent:BaseComponent
--- @field Spells SpellSpellContainerComponentSpell[]


--- @class SpellBookComponent:BaseComponent
--- @field Spells SpellSpellBookEntry[]
--- @field field_0 uint64


--- @class SpellBookPreparesComponent:BaseComponent
--- @field PreparedSpells SpellIdBase[]
--- @field field_30 table<Guid, int32>
--- @field field_88 table<Guid, int32>


--- @class SpellCCPrepareSpellComponent:BaseComponent
--- @field Spells SpellIdBase[]


--- @class SpellLearnedSpellsComponent:BaseComponent
--- @field field_18 table<Guid, Array_FixedString>
--- @field field_70 Array_uint8


--- @class SpellModification
--- @field Modification Variant<SpellModificationModifyAreaRadius,SpellModificationModifyMaximumTargets,SpellModificationModifyNumberOfTargets,SpellModificationModifySavingThrowDisadvantage,SpellModificationModifySpellFlags,SpellModificationModifySpellRoll,SpellModificationModifyStatusDuration,SpellModificationModifySummonDuration,SpellModificationModifySurfaceDuration,SpellModificationModifyTargetRadius,SpellModificationModifyUseCosts,SpellModificationModifyVisuals,SpellModificationModifyIconGlow,SpellModificationModifyTooltipDescription>
--- @field Spells Array_SpellId
--- @field field_0 uint8
--- @field field_4 FixedString


--- @class SpellModificationModifyAreaRadius
--- @field Value uint64


--- @class SpellModificationModifyIconGlow
--- @field Value uint8


--- @class SpellModificationModifyMaximumTargets
--- @field Value uint64


--- @class SpellModificationModifyNumberOfTargets
--- @field Value vec3


--- @class SpellModificationModifySavingThrowDisadvantage
--- @field Value uint8


--- @class SpellModificationModifySpellFlags
--- @field Value uint8
--- @field field_1 uint8


--- @class SpellModificationModifySpellRoll
--- @field field_0 string
--- @field field_18 string
--- @field field_30 int32


--- @class SpellModificationModifyStatusDuration
--- @field Value uint64


--- @class SpellModificationModifySummonDuration
--- @field Value uint64


--- @class SpellModificationModifySurfaceDuration
--- @field Value uint64


--- @class SpellModificationModifyTargetRadius
--- @field Value uint64


--- @class SpellModificationModifyTooltipDescription
--- @field Value uint8


--- @class SpellModificationModifyUseCosts
--- @field Type uint8
--- @field field_10 Guid
--- @field field_18 string
--- @field field_30 int32
--- @field field_38 Guid


--- @class SpellModificationModifyVisuals
--- @field Value uint8


--- @class SpellPlayerPrepareSpellComponent:BaseComponent
--- @field Spells SpellIdBase[]
--- @field field_30 uint8


--- @class SpellSpellAiConditionsComponent:BaseComponent
--- @field field_18 table<FixedString, uint64>


--- @class SpellSpellBookCooldownsComponent:BaseComponent
--- @field Cooldowns SpellSpellBookCooldownsComponentCooldown[]


--- @class SpellSpellBookCooldownsComponentCooldown
--- @field Cooldown number
--- @field CooldownType SpellCooldownType
--- @field SpellId SpellId
--- @field field_29 uint8
--- @field field_30 Guid


--- @class SpellSpellBookEntry
--- @field Charged boolean
--- @field CooldownType SpellCooldownType
--- @field Id SpellId
--- @field InnerEntries SpellSpellBookEntryInnerEntry[]
--- @field PrepareType SpellPrepareType
--- @field SpellCastingAbility AbilityId
--- @field SpellUUID Guid
--- @field field_38 int32
--- @field field_3C int32
--- @field field_41 boolean
--- @field field_42 SpellPrepareType


--- @class SpellSpellBookEntryInnerEntry
--- @field field_0 uint8
--- @field field_4 int32


--- @class SpellSpellContainerComponent:BaseComponent
--- @field Spells SpellSpellContainerComponentSpell[]


--- @class SpellSpellContainerComponentSpell
--- @field ContainerSpell FixedString
--- @field CooldownType SpellCooldownType
--- @field ItemHandle EntityHandle
--- @field SelectionType SpellChildSelectionType
--- @field SpellCastingAbility AbilityId
--- @field SpellId SpellIdBase
--- @field SpellUUID Guid
--- @field field_29 uint8
--- @field field_48 uint8


--- @class SpellSpellModificationContainerComponent:BaseComponent
--- @field Modifications table<FixedString, SpellModification[]>


--- @class SpellCastAnimationInfoComponent:BaseComponent
--- @field Target EntityHandle
--- @field TargetPosition vec3
--- @field TargetRotation vec3
--- @field field_0 uint8
--- @field field_10 uint8
--- @field field_28 uint8
--- @field field_29 uint8
--- @field field_2A uint8
--- @field field_2B uint8
--- @field field_2C uint8
--- @field field_2D uint8
--- @field field_2E uint8


--- @class SpellCastCacheComponent:BaseComponent
--- @field field_0 uint8
--- @field field_4 uint32


--- @class SpellCastCanBeTargetedComponent:BaseComponent
--- @field Dummy uint8


--- @class SpellCastInterruptResultsComponent:BaseComponent
--- @field Results Array_EntityHandle
--- @field field_0 string


--- @class SpellCastIsCastingComponent:BaseComponent
--- @field Cast EntityHandle


--- @class SpellCastMovementComponent:BaseComponent
--- @field field_0 vec3
--- @field field_18 boolean
--- @field field_C vec3


--- @class SpellCastMultiTargetInfo:SpellCastTargetInfo
--- @field SpellType SpellType
--- @field Target2 SpellCastTargetInfo?


--- @class SpellCastMultiTargetInfo2:SpellCastTargetInfo
--- @field Target2 SpellCastMultiTargetInfo
--- @field field_A0 uint8


--- @class SpellCastRollsComponent:BaseComponent
--- @field Rolls SpellCastRollsComponentRoll[]


--- @class SpellCastRollsComponentRoll
--- @field Hits SpellCastRollsComponentRollHit[]
--- @field MHS_FS_i32 table<FixedString, int32>
--- @field Target EntityHandle
--- @field field_68 int32
--- @field field_6C uint8
--- @field field_70 int64
--- @field field_78 int64
--- @field field_8 EntityHandle?
--- @field field_80 int64
--- @field field_88 uint8


--- @class SpellCastRollsComponentRollHit
--- @field Hit Hit
--- @field field_0 FixedString


--- @class SpellCastStateComponent:BaseComponent
--- @field Caster EntityHandle
--- @field CasterMoveToPosition vec3?
--- @field CasterStartPosition vec3
--- @field Entity EntityHandle
--- @field SpellId SpellId
--- @field Targets SpellCastMultiTargetInfo[]
--- @field field_38 int32
--- @field field_60 vec3?
--- @field field_80 EntityHandle
--- @field field_88 int32
--- @field field_90 Guid
--- @field field_A0 string


--- @class SpellCastSyncTargetingComponent:BaseComponent
--- @field CasterMoveToPosition vec3?
--- @field CasterPosition vec3?
--- @field Targets SpellCastMultiTargetInfo[]
--- @field field_10 EntityHandle?
--- @field field_20 vec3?
--- @field field_40 uint8
--- @field field_44 int32
--- @field field_48 vec3?
--- @field field_58 vec3?
--- @field field_8 EntityHandle
--- @field field_88 vec3?


--- @class SpellCastTargetInfo
--- @field Position vec3?
--- @field Target EntityHandle
--- @field field_10 EntityHandle?


--- @class StatsApplyEquipmentStatusFunctor:StatsApplyStatusFunctor
--- @field EquipmentSlot StatsItemSlot


--- @class StatsApplyStatusFunctor:StatsFunctor
--- @field HasParam6 boolean
--- @field StatsConditions string
--- @field StatusId FixedString
--- @field StringParam FixedString


--- @class StatsBaseFunctorExecParams
--- @field EntityToThothContextIndex table<EntityHandle, int32>
--- @field HistoryEntity EntityHandle
--- @field Originator ActionOriginator
--- @field ParamsTypeId FunctorExecParamsType
--- @field PropertyContext StatsPropertyContext
--- @field StatusSource EntityHandle
--- @field StoryActionId int32
--- @field field_98 int32
--- @field field_9C boolean


--- @class StatsBreakConcentrationFunctor:StatsFunctor


--- @class StatsCNamedElementManager_StatsModifier
--- @field NameHashMap table<FixedString, uint32>
--- @field Primitives StatsModifier[]


--- @class StatsCNamedElementManager_StatsModifierList
--- @field NameHashMap table<FixedString, uint32>
--- @field Primitives StatsModifierList[]


--- @class StatsCNamedElementManager_StatsRPGEnumeration
--- @field NameHashMap table<FixedString, uint32>
--- @field Primitives StatsRPGEnumeration[]


--- @class StatsCNamedElementManager_StatsTreasureTable
--- @field NameHashMap table<FixedString, uint32>
--- @field Primitives StatsTreasureTable[]


--- @class StatsCreateConeSurfaceFunctor:StatsFunctor
--- @field Arg0 number
--- @field Arg1 number
--- @field Arg2 FixedString
--- @field Arg3 boolean


--- @class StatsCreateExplosionFunctor:StatsFunctor
--- @field SpellId FixedString


--- @class StatsCreateSurfaceFunctor:StatsFunctor
--- @field Arg4 number
--- @field Duration number
--- @field IsControlledByConcentration boolean
--- @field Radius number
--- @field SurfaceType FixedString


--- @class StatsCustomDescriptionFunctor:StatsFunctor
--- @field Description FixedString


--- @class StatsDealDamageFunctor:StatsFunctor
--- @field DamageType DamageType
--- @field Magical boolean
--- @field Nonlethal boolean
--- @field WeaponDamageType DealDamageWeaponDamageType
--- @field WeaponType DealDamageWeaponType
--- @field field_34 int32


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


--- @class StatsDisarmWeaponFunctor:StatsFunctor


--- @class StatsDouseFunctor:StatsFunctor
--- @field field_20 number
--- @field field_24 number


--- @class StatsExecuteWeaponFunctorsFunctor:StatsFunctor
--- @field WeaponType ExecuteWeaponFunctorsType


--- @class StatsExtenderFunctor:StatsFunctor


--- @class StatsForceFunctor:StatsFunctor
--- @field Aggression ForceFunctorAggression
--- @field Origin ForceFunctorOrigin


--- @class StatsFunctor
--- @field ObserverType StatsObserverType
--- @field RollConditions StatsFunctorRollCondition[]
--- @field StatsConditionsId int32
--- @field TypeId StatsFunctorId
--- @field UniqueName FixedString


--- @class StatsFunctorRollCondition
--- @field ConditionId int32
--- @field Type StatsRollType


--- @class StatsFunctorExecParamsType1:StatsBaseFunctorExecParams
--- @field Caster EntityRef
--- @field CasterProxy EntityRef
--- @field DamageSums DamageSums
--- @field Hit Hit
--- @field HitWith HitWith
--- @field IsFromItem boolean
--- @field Position vec3
--- @field SomeRadius number
--- @field SpellId SpellIdWithPrototype
--- @field Target EntityRef
--- @field TargetProxy EntityRef
--- @field field_26C FixedString
--- @field field_2F8 uint64
--- @field field_300 uint64
--- @field field_310 uint32
--- @field field_314 uint32
--- @field field_31C uint8


--- @class StatsFunctorExecParamsType2:StatsBaseFunctorExecParams
--- @field Caster EntityRef
--- @field DamageSums DamageSums
--- @field ExplodeRadius number
--- @field Hit Hit
--- @field IsFromItem boolean
--- @field Position vec3
--- @field SomeRadius number
--- @field SpellId SpellIdWithPrototype


--- @class StatsFunctorExecParamsType3:StatsBaseFunctorExecParams
--- @field Caster EntityRef
--- @field Distance number
--- @field Position vec3
--- @field Target EntityRef
--- @field field_C0 EntityRef


--- @class StatsFunctorExecParamsType4:StatsBaseFunctorExecParams
--- @field Caster EntityRef
--- @field DamageSums DamageSums
--- @field Hit Hit
--- @field Position vec3
--- @field SpellId SpellIdWithPrototype
--- @field field_2D8 uint64
--- @field field_2E0 uint64
--- @field field_2E8 uint32
--- @field field_2EC FixedString
--- @field field_2F0 uint8
--- @field field_B0 EntityRef


--- @class StatsFunctorExecParamsType5:StatsBaseFunctorExecParams
--- @field Caster EntityRef
--- @field DamageSums DamageSums
--- @field Hit Hit
--- @field IsFromItem boolean
--- @field Owner_M EntityRef
--- @field Position vec3
--- @field SpellId SpellIdWithPrototype
--- @field Target EntityRef
--- @field field_D0 EntityRef
--- @field field_E0 EntityRef


--- @class StatsFunctorExecParamsType6:StatsBaseFunctorExecParams
--- @field Caster EntityRef
--- @field DamageSums DamageSums
--- @field Hit Hit
--- @field IsFromItem boolean
--- @field Position vec3
--- @field SpellId SpellIdWithPrototype
--- @field Target EntityRef
--- @field TargetProxy EntityRef
--- @field field_D0 EntityRef
--- @field field_E0 EntityRef


--- @class StatsFunctorExecParamsType7:StatsBaseFunctorExecParams
--- @field Caster EntityRef
--- @field Target EntityRef
--- @field UseCasterStats boolean


--- @class StatsFunctorExecParamsType8:StatsBaseFunctorExecParams
--- @field Caster EntityRef
--- @field Target EntityRef


--- @class StatsFunctorExecParamsType9:StatsBaseFunctorExecParams
--- @field DamageList DamagePair[]
--- @field DamageSums DamageSums
--- @field ExecuteInterruptResult InterruptExecuteResult
--- @field Hit Hit
--- @field Interrupt InterruptInterruptVariant2
--- @field Observer EntityRef
--- @field ObserverProxy EntityRef
--- @field OnlyAllowRollAdjustments boolean
--- @field ResolveData InterruptResolveData?
--- @field Source EntityRef
--- @field SourceProxy EntityRef
--- @field Target EntityRef
--- @field TargetProxy EntityRef


--- @class StatsFunctors
--- @field NextFunctorIndex int32
--- @field UniqueName FixedString


--- @class StatsInterruptPrototype
--- @field Conditions int32
--- @field Container FixedString
--- @field Description StatsDescriptionInfo
--- @field EnableCondition int32
--- @field EnableContext uint64
--- @field FailureFunctors StatsFunctors
--- @field InterruptContext uint16
--- @field InterruptContextScope uint8
--- @field InterruptDefaultValue uint8
--- @field InterruptFlags uint8
--- @field Name FixedString
--- @field PropertiesFunctors StatsFunctors
--- @field RollCondition int32
--- @field Stack FixedString
--- @field SuccessFunctors StatsFunctors


--- @class StatsModifier
--- @field EnumerationIndex int32
--- @field LevelMapIndex int32
--- @field Name FixedString


--- @class StatsModifierList
--- @field Attributes StatsCNamedElementManager_StatsModifier
--- @field Name FixedString


--- @class StatsPassivePrototype
--- @field BoostConditionsIndex int32
--- @field BoostContext uint64
--- @field ConditionsIndex int32
--- @field Description StatsDescriptionInfo
--- @field EnabledConditions int32
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


--- @class StatsRPGEnumeration
--- @field Name FixedString
--- @field Values table<FixedString, int32>


--- @class StatsRPGStats
--- @field ExtraData table<FixedString, number>
--- @field ModifierLists StatsCNamedElementManager_StatsModifierList
--- @field ModifierValueLists StatsCNamedElementManager_StatsRPGEnumeration
--- @field TreasureTables StatsCNamedElementManager_StatsTreasureTable


--- @class StatsRegainHitPointsFunctor:StatsFunctor


--- @class StatsRemoveAuraByChildStatusFunctor:StatsFunctor
--- @field StatusId FixedString


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


--- @class StatsRestoreResourceFunctor:StatsFunctor
--- @field ActionResourceUUID Guid
--- @field Amount number
--- @field Hex int32
--- @field IsPercentage boolean
--- @field field_34 int32


--- @class StatsResurrectFunctor:StatsFunctor
--- @field HealthPercentage number
--- @field Probability number


--- @class StatsSabotageFunctor:StatsFunctor
--- @field Amount int32


--- @class StatsSetStatusDurationFunctor:StatsFunctor
--- @field Duration number
--- @field StatusId FixedString


--- @class StatsSpawnFunctor:StatsFunctor
--- @field Arg1 FixedString
--- @field StatusesToApply Array_FixedString
--- @field TemplateId FixedString


--- @class StatsSpawnInInventoryFunctor:StatsFunctor


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
--- @field DualWieldingSpellAnimation StatsSpellPrototypeAnimation
--- @field DualWieldingUseCosts StatsSpellPrototypeUseCost[]
--- @field DualWieldingUseCostsGroups StatsSpellPrototypeUseCostGroup[]
--- @field HasMemoryCost boolean
--- @field HitAnimationType uint8
--- @field HitCostGroups StatsSpellPrototypeUseCost[]
--- @field InterruptPrototype FixedString
--- @field ItemWall FixedString
--- @field Level int32
--- @field LineOfSightFlags uint32
--- @field ParentPrototype StatsSpellPrototype
--- @field PowerLevel int32
--- @field PrepareEffect FixedString
--- @field PrepareLoopSound FixedString
--- @field PrepareSound FixedString
--- @field RechargeValuesFrom int32
--- @field RechargeValuesTo int32
--- @field RequirementEvents uint32
--- @field RitualCostGroups StatsSpellPrototypeUseCostGroup[]
--- @field RitualCosts StatsSpellPrototypeUseCost[]
--- @field Sheathing uint8
--- @field SourceLimbIndex int8
--- @field SpellActionType uint8
--- @field SpellAnimation StatsSpellPrototypeAnimation
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
--- @field UseCostGroups StatsSpellPrototypeUseCostGroup[]
--- @field UseCosts StatsSpellPrototypeUseCost[]
--- @field VerbalIntent uint32
--- @field WeaponTypes uint32
--- @field field_101 uint8
--- @field field_299 uint8


--- @class StatsSpellPrototypeAnimation
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


--- @class StatsSpellPrototypeUseCost
--- @field Amount number
--- @field Resource Guid


--- @class StatsSpellPrototypeUseCostGroup
--- @field Amount number
--- @field ResourceGroup Guid
--- @field Resources Guid[]
--- @field SubResourceId int32


--- @class StatsStabilizeFunctor:StatsFunctor


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
--- @field SoundLoop FixedString[]
--- @field SoundStart FixedString[]
--- @field SoundStop FixedString[]
--- @field StackType uint32
--- @field StatsObjectIndex int32
--- @field StatusGroups uint64
--- @field StatusId StatusType
--- @field StatusName FixedString
--- @field StatusPropertyFlags uint64
--- @field TickType uint8


--- @class StatsSummonFunctor:StatsFunctor
--- @field Arg3 FixedString
--- @field Arg5 boolean
--- @field SpawnLifetime number
--- @field StatusesToApply Array_FixedString
--- @field Template FixedString


--- @class StatsSummonInInventoryFunctor:StatsFunctor


--- @class StatsSurfaceChangeFunctor:StatsFunctor
--- @field Chance number
--- @field SurfaceChange SurfaceChange
--- @field field_24 number
--- @field field_28 number
--- @field field_2C number


--- @class StatsSwapPlacesFunctor:StatsFunctor


--- @class StatsTeleportSourceFunctor:StatsFunctor


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


--- @class StatsUnlockFunctor:StatsFunctor


--- @class StatsUseActionResourceFunctor:StatsFunctor
--- @field ActionResourceUUID Guid
--- @field Amount number
--- @field IsPercentage boolean
--- @field ResourceIndex int32


--- @class StatsUseAttackFunctor:StatsFunctor
--- @field IgnoreChecks boolean


--- @class StatsUseSpellFunctor:StatsFunctor
--- @field Arg3 boolean
--- @field IgnoreChecks boolean
--- @field IgnoreHasSpell boolean
--- @field SpellId FixedString


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


--- @class StatusLifetimeComponent:BaseComponent
--- @field field_0 int32
--- @field field_4 int32


--- @class StatusVisualDisabledComponent:BaseComponent
--- @field Visuals Array_FixedString


--- @class TadpoleTreeFullIllithidComponent:BaseComponent


--- @class TadpoleTreeHalfIllithidComponent:BaseComponent


--- @class TadpoleTreePowerContainerComponent:BaseComponent
--- @field Powers FixedString[]


--- @class TadpoleTreeTadpoledComponent:BaseComponent


--- @class TadpoleTreeTreeStateComponent:BaseComponent
--- @field State uint8


--- @class UiDCWidget:UiViewModel
--- @field Layout uint8
--- @field Name Noesis::String
--- @field PlayerId int16


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
--- @field GetRTPC fun():number
--- @field LoadBank fun(a1:string):boolean
--- @field LoadEvent fun(a1:string):boolean
--- @field PauseAllSounds fun()
--- @field PlayExternalSound fun():boolean
--- @field PostEvent fun():boolean
--- @field PrepareBank fun(a1:string):boolean
--- @field ResetRTPC fun()
--- @field ResumeAllSounds fun()
--- @field SetRTPC fun():boolean
--- @field SetState fun(a1:string, a2:string):boolean
--- @field SetSwitch fun():boolean
--- @field Stop fun()
--- @field UnloadBank fun(a1:string):boolean
--- @field UnloadEvent fun(a1:string):boolean
--- @field UnprepareBank fun(a1:string):boolean
local Ext_ClientAudio = {}



--- @class Ext_ClientIMGUI
--- @field EnableDemo fun(a1:boolean)
--- @field LoadFont fun(a1:FixedString, a2:string, a3:number):boolean
--- @field NewWindow fun(a1:string):ExtuiWindow
--- @field SetScale fun(a1:number)
local Ext_ClientIMGUI = {}



--- @class Ext_ClientNet
--- @field PostMessageToServer fun(a1:string, a2:string)
local Ext_ClientNet = {}



--- @class Ext_ClientTemplate
--- @field GetAllRootTemplates fun()
--- @field GetRootTemplate fun(a1:FixedString):GameObjectTemplate
--- @field GetTemplate fun(a1:FixedString):GameObjectTemplate
local Ext_ClientTemplate = {}



--- @class Ext_ClientUI
--- @field GetPickingHelper fun(a1:uint16):EclPlayerPickingHelper
--- @field GetRoot fun():NoesisFrameworkElement
--- @field GetStateMachine fun():UiUIStateMachine
--- @field SetState fun(a1:FixedString, a2:FixedString?, a3:boolean?)
local Ext_ClientUI = {}



--- @class Ext_Debug
--- @field Crash fun(a1:int32)
--- @field DebugBreak fun()
--- @field DebugDumpLifetimes fun()
--- @field DumpStack fun()
--- @field GenerateIdeHelpers fun(a1:boolean?)
--- @field IsDeveloperMode fun():boolean
--- @field SetEntityRuntimeCheckLevel fun(a1:int32)
local Ext_Debug = {}



--- @class Ext_Entity
--- @field Get fun(a1:Guid)
--- @field GetAllEntities fun():EntityHandle[]
--- @field GetAllEntitiesWithComponent fun(a1:ExtComponentType):EntityHandle[]
--- @field GetAllEntitiesWithUuid fun():table<Guid, EntityHandle>
--- @field HandleToUuid fun(a1:EntityHandle):Guid?
--- @field OnChange fun(a1:ExtComponentType, a2:FunctionRef, a3:EntityHandle?):uint64
--- @field OnCreate fun(a1:ExtComponentType, a2:FunctionRef, a3:EntityHandle?):uint64
--- @field OnDestroy fun(a1:ExtComponentType, a2:FunctionRef, a3:EntityHandle?):uint64
--- @field Subscribe fun(a1:ExtComponentType, a2:FunctionRef, a3:EntityHandle?):uint64
--- @field Unsubscribe fun(a1:uint64):boolean
--- @field UuidToHandle fun(a1:Guid):EntityHandle
local Ext_Entity = {}



--- @class Ext_IO
--- @field AddPathOverride fun(a1:string, a2:string)
--- @field GetPathOverride fun(a1:string):string?
--- @field LoadFile fun(a1:string, a2:FixedString?):string?
--- @field SaveFile fun(a1:string, a2:string):boolean
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



--- @class Ext_Loca
--- @field GetTranslatedString fun(a1:FixedString, a2:string?):string
--- @field UpdateTranslatedString fun(a1:FixedString, a2:string):boolean
local Ext_Loca = {}



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
--- @field Mul fun()
--- @field Normalize fun()
--- @field OuterProduct fun()
--- @field Perpendicular fun()
--- @field Project fun()
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
--- @field GetLoadOrder fun()
--- @field GetMod fun(a1:string):Module
--- @field GetModManager fun():ModManager
--- @field IsModLoaded fun(a1:string):boolean
local Ext_Mod = {}



--- @class Ext_Resource
--- @field Get fun(a1:FixedString, a2:ResourceBankType)
--- @field GetAll fun(a1:ResourceBankType):FixedString[]
local Ext_Resource = {}



--- @class Ext_ServerNet
--- @field BroadcastMessage fun(a1:string, a2:string, a3:Guid?)
--- @field PlayerHasExtender fun(a1:Guid):boolean?
--- @field PostMessageToClient fun(a1:Guid, a2:string, a3:string)
--- @field PostMessageToUser fun(a1:int32, a2:string, a3:string)
local Ext_ServerNet = {}



--- @class Ext_ServerTemplate
--- @field GetAllCacheTemplates fun()
--- @field GetAllLocalCacheTemplates fun()
--- @field GetAllLocalTemplates fun()
--- @field GetAllRootTemplates fun()
--- @field GetCacheTemplate fun(a1:FixedString):GameObjectTemplate
--- @field GetLocalCacheTemplate fun(a1:FixedString):GameObjectTemplate
--- @field GetLocalTemplate fun(a1:FixedString):GameObjectTemplate
--- @field GetRootTemplate fun(a1:FixedString):GameObjectTemplate
--- @field GetTemplate fun(a1:FixedString):GameObjectTemplate
local Ext_ServerTemplate = {}



--- @class Ext_StaticData
--- @field Get fun(a1:Guid, a2:ExtResourceManagerType)
--- @field GetAll fun(a1:ExtResourceManagerType):Guid[]
local Ext_StaticData = {}



--- @class Ext_Stats
--- @field TreasureCategory Ext_StatsTreasureCategory
--- @field TreasureTable Ext_StatsTreasureTable
--- @field AddAttribute fun(a1:FixedString, a2:FixedString, a3:FixedString):boolean
--- @field AddEnumerationValue fun(typeName:FixedString, enumLabel:FixedString):int32
--- @field Create fun(a1:FixedString, a2:FixedString, a3:FixedString?, a4:boolean?)
--- @field EnumIndexToLabel fun(a1:FixedString, a2:int32):FixedString?
--- @field EnumLabelToIndex fun(a1:FixedString, a2:FixedString)
--- @field Get fun(a1:string, a2:int32?, a3:boolean?, a4:boolean?):FixedString[]
--- @field GetCachedInterrupt fun(a1:FixedString):StatsInterruptPrototype
--- @field GetCachedPassive fun(a1:FixedString):StatsPassivePrototype
--- @field GetCachedSpell fun(a1:FixedString):StatsSpellPrototype
--- @field GetCachedStatus fun(a1:FixedString):StatsStatusPrototype
--- @field GetModifierAttributes fun(a1:FixedString)
--- @field GetStats fun(a1:FixedString?):FixedString[]
--- @field GetStatsLoadedBefore fun(a1:FixedString, a2:FixedString?):FixedString[]
--- @field GetStatsManager fun():StatsRPGStats
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
--- @field RegisterPersistentHandler fun(a1:FixedString, a2:Ref)
--- @field WaitFor fun(a1:number, a2:Ref, a3:number?):uint64
--- @field WaitForPersistent fun(a1:number, a2:FixedString, a3:Ref):uint64
--- @field WaitForRealtime fun(a1:number, a2:Ref, a3:number?):uint64
local Ext_Timer = {}



--- @class Ext_Types
--- @field Construct fun(a1:FixedString)
--- @field GetAllTypes fun():FixedString[]
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
--- @field GameTime fun():number
--- @field GameVersion fun():string?
--- @field GetCommandLineParams fun():string[]
--- @field GetGlobalSwitches fun():GlobalSwitches
--- @field GetValueType fun(value:any):string|"nil"|"number"|"string"|"boolean"|"table"|"function"|"thread"|"userdata"|"lightuserdata" valueType # Returns a base type, lightuserdata, or an extender type.
--- @field HandleToInteger fun(a1:EntityHandle):int64
--- @field Include fun(modGUID:string?, path:string, replaceGlobals:table?):any returnedValue Whatever the script returned, if anything
--- @field IntegerToHandle fun(a1:int64):EntityHandle
--- @field IsValidHandle fun(handle:ComponentHandle):boolean
--- @field LoadString fun(str:string):UserReturn Similar to lua `loadstring`, with extra safeguards.
--- @field MicrosecTime fun():number
--- @field MonotonicTime fun():int64
--- @field Print fun(...:any)
--- @field PrintError fun(...:any)
--- @field PrintWarning fun(...:any)
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
--- @field Loca Ext_Loca
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
--- @field Loca Ext_Loca
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

--- @enum AdvantageBoostType
Ext_Enums.AdvantageBoostType = {
	Advantage = 1,
	Disadvantage = 2,
	[1] = "Advantage",
	[2] = "Disadvantage",
}

--- @enum AdvantageTypeId
Ext_Enums.AdvantageTypeId = {
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

--- @enum AnimationEventPropertyType
Ext_Enums.AnimationEventPropertyType = {
	SoundObject = 1,
	BoneEffect = 2,
	Foot = 3,
	BoneAttachBone = 4,
	WeaponEffect = 6,
	Unknown7 = 7,
	Unknown8 = 8,
	Prop9 = 9,
	Prop10 = 10,
	FloatData = 11,
	EFoleySoundObject = 12,
	EVocalSoundObject = 13,
	U8Data = 14,
	Unknown15 = 15,
	[1] = "SoundObject",
	[2] = "BoneEffect",
	[3] = "Foot",
	[4] = "BoneAttachBone",
	[6] = "WeaponEffect",
	[7] = "Unknown7",
	[8] = "Unknown8",
	[9] = "Prop9",
	[10] = "Prop10",
	[11] = "FloatData",
	[12] = "EFoleySoundObject",
	[13] = "EVocalSoundObject",
	[14] = "U8Data",
	[15] = "Unknown15",
}

--- @enum AnimationSetAnimationFlags
Ext_Enums.AnimationSetAnimationFlags = {
	AlwaysIgnore = 1,
	NoFallback = 2,
	[1] = "AlwaysIgnore",
	[2] = "NoFallback",
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

--- @enum BoostCauseType
Ext_Enums.BoostCauseType = {
	Undefined = 0,
	Item = 1,
	Status = 2,
	Passive = 3,
	Osiris = 4,
	Unknown5 = 5,
	Progression = 6,
	Stats = 7,
	Character = 8,
	[0] = "Undefined",
	[1] = "Item",
	[2] = "Status",
	[3] = "Passive",
	[4] = "Osiris",
	[5] = "Unknown5",
	[6] = "Progression",
	[7] = "Stats",
	[8] = "Character",
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

--- @enum CraftingStationType
Ext_Enums.CraftingStationType = {
	None = 0,
	Anvil = 1,
	Oven = 2,
	Wetstone = 3,
	Well = 4,
	BoilingPot = 5,
	Beehive = 6,
	SpinningWheel = 7,
	Cauldron = 8,
	Misc1 = 9,
	Misc2 = 10,
	Misc3 = 11,
	Misc4 = 12,
	[0] = "None",
	[1] = "Anvil",
	[2] = "Oven",
	[3] = "Wetstone",
	[4] = "Well",
	[5] = "BoilingPot",
	[6] = "Beehive",
	[7] = "SpinningWheel",
	[8] = "Cauldron",
	[9] = "Misc1",
	[10] = "Misc2",
	[11] = "Misc3",
	[12] = "Misc4",
}

--- @enum CriticalHitBoostFlags
Ext_Enums.CriticalHitBoostFlags = {
	Failure = 1,
	SuccessNever = 2,
	FailureNever = 8,
	AttackTarget = 16,
	AttackRoll = 32,
	[1] = "Failure",
	[2] = "SuccessNever",
	[8] = "FailureNever",
	[16] = "AttackTarget",
	[32] = "AttackRoll",
}

--- @enum DamageFlags
Ext_Enums.DamageFlags = {
	Hit = 1,
	Dodge = 2,
	Miss = 4,
	StatusEnter = 8,
	Critical = 16,
	Backstab = 32,
	Invisible = 64,
	Magical = 128,
	Invulnerable = 256,
	SavingThrow = 512,
	Projectile = 1024,
	HitpointsDamaged = 2048,
	Surface = 4096,
	Status = 8192,
	AttackAdvantage = 16384,
	AttackDisadvantage = 32768,
	[1] = "Hit",
	[2] = "Dodge",
	[4] = "Miss",
	[8] = "StatusEnter",
	[16] = "Critical",
	[32] = "Backstab",
	[64] = "Invisible",
	[128] = "Magical",
	[256] = "Invulnerable",
	[512] = "SavingThrow",
	[1024] = "Projectile",
	[2048] = "HitpointsDamaged",
	[4096] = "Surface",
	[8192] = "Status",
	[16384] = "AttackAdvantage",
	[32768] = "AttackDisadvantage",
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

--- @enum ESurfaceFlag
Ext_Enums.ESurfaceFlag = {
	MovementBlock = 1,
	ProjectileBlock = 4,
	HasCharacter = 16,
	HasItem = 128,
	HasInteractableObject = 256,
	GroundSurfaceBlock = 512,
	CloudSurfaceBlock = 1024,
	Occupied = 4096,
	SurfaceExclude = 65536,
	Fire = 16777216,
	Water = 33554432,
	Blood = 67108864,
	Poison = 134217728,
	Oil = 268435456,
	Lava = 536870912,
	Source = 1073741824,
	Web = 2147483648,
	Deepwater = 4294967296,
	Sulfurium = 8589934592,
	FireCloud = 34359738368,
	WaterCloud = 68719476736,
	BloodCloud = 137438953472,
	PoisonCloud = 274877906944,
	SmokeCloud = 549755813888,
	ExplosionCloud = 1099511627776,
	FrostCloud = 2199023255552,
	Deathfog = 4398046511104,
	ShockwaveCloud = 8796093022208,
	Blessed = 70368744177664,
	Cursed = 140737488355328,
	Purified = 281474976710656,
	CloudBlessed = 1125899906842624,
	CloudCursed = 2251799813685248,
	CloudPurified = 4503599627370496,
	Electrified = 18014398509481984,
	Frozen = 36028797018963968,
	CloudElectrified = 72057594037927936,
	ElectrifiedDecay = 144115188075855872,
	SomeDecay = 288230376151711744,
	IrreplaceableCloud = 576460752303423488,
	Irreplaceable = 4611686018427387904,
	[1] = "MovementBlock",
	[4] = "ProjectileBlock",
	[16] = "HasCharacter",
	[128] = "HasItem",
	[256] = "HasInteractableObject",
	[512] = "GroundSurfaceBlock",
	[1024] = "CloudSurfaceBlock",
	[4096] = "Occupied",
	[65536] = "SurfaceExclude",
	[16777216] = "Fire",
	[33554432] = "Water",
	[67108864] = "Blood",
	[134217728] = "Poison",
	[268435456] = "Oil",
	[536870912] = "Lava",
	[1073741824] = "Source",
	[2147483648] = "Web",
	[4294967296] = "Deepwater",
	[8589934592] = "Sulfurium",
	[34359738368] = "FireCloud",
	[68719476736] = "WaterCloud",
	[137438953472] = "BloodCloud",
	[274877906944] = "PoisonCloud",
	[549755813888] = "SmokeCloud",
	[1099511627776] = "ExplosionCloud",
	[2199023255552] = "FrostCloud",
	[4398046511104] = "Deathfog",
	[8796093022208] = "ShockwaveCloud",
	[70368744177664] = "Blessed",
	[140737488355328] = "Cursed",
	[281474976710656] = "Purified",
	[1125899906842624] = "CloudBlessed",
	[2251799813685248] = "CloudCursed",
	[4503599627370496] = "CloudPurified",
	[18014398509481984] = "Electrified",
	[36028797018963968] = "Frozen",
	[72057594037927936] = "CloudElectrified",
	[144115188075855872] = "ElectrifiedDecay",
	[288230376151711744] = "SomeDecay",
	[576460752303423488] = "IrreplaceableCloud",
	[4611686018427387904] = "Irreplaceable",
}

--- @enum EffectPropertyType
Ext_Enums.EffectPropertyType = {
	Bool = 0,
	Int32 = 1,
	Int32Range = 2,
	ColoredFrames = 3,
	Float = 4,
	FloatRange = 5,
	Frames = 6,
	STDString = 7,
	Vector3 = 8,
	FunctionType = 9,
	FixedString = 10,
	Base = 11,
	[0] = "Bool",
	[1] = "Int32",
	[2] = "Int32Range",
	[3] = "ColoredFrames",
	[4] = "Float",
	[5] = "FloatRange",
	[6] = "Frames",
	[7] = "STDString",
	[8] = "Vector3",
	[9] = "FunctionType",
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
	Armor = 1,
	ArmorSetState = 2,
	BaseHp = 3,
	Bound = 4,
	IsCharacter = 5,
	Data = 6,
	Detached = 7,
	Experience = 8,
	Health = 9,
	Resistances = 10,
	Hearing = 11,
	SpellBook = 12,
	Stats = 13,
	StatusImmunities = 14,
	SurfacePathInfluences = 15,
	Use = 16,
	Value = 17,
	Weapon = 18,
	Wielding = 19,
	CustomStats = 20,
	BoostCondition = 21,
	BoostsContainer = 22,
	ServerBoostBase = 23,
	ServerStatusBoostsProcessed = 24,
	ActionResourceConsumeMultiplierBoost = 25,
	Tag = 26,
	SpellBookPrepares = 27,
	Transform = 28,
	BoostInfo = 29,
	Relation = 30,
	Faction = 31,
	CanInteract = 32,
	CanSpeak = 33,
	Origin = 34,
	Level = 35,
	EocLevel = 36,
	Classes = 37,
	MaterialParameterOverride = 38,
	OffStage = 39,
	PickingState = 40,
	Player = 41,
	SimpleCharacter = 42,
	Speaker = 43,
	WeaponSet = 44,
	DifficultyCheck = 45,
	AttributeFlags = 46,
	BodyType = 47,
	ObjectSize = 48,
	BaseStats = 49,
	Expertise = 50,
	HealBlock = 51,
	Ruleset = 52,
	RulesetModifiers = 53,
	ServerBaseData = 54,
	ServerBaseSize = 55,
	ServerBaseStats = 56,
	ServerBaseWeapon = 57,
	ServerBaseProficiency = 58,
	ServerProficiencyGroupStats = 59,
	ShapeshiftState = 60,
	ShapeshiftHealthReservation = 61,
	ServerShapeshiftStates = 62,
	AnimationWaterfall = 63,
	DynamicAnimationTags = 64,
	TemplateAnimationSetOverride = 65,
	PassiveContainer = 66,
	Passive = 67,
	PassiveUsageCount = 68,
	ItemDestroyed = 69,
	ItemDye = 70,
	MapMarkerStyle = 71,
	ItemPortal = 72,
	IsDoor = 73,
	ExamineDisabled = 74,
	ItemHasMoved = 75,
	HasOpened = 76,
	IsGold = 77,
	IsItem = 78,
	NewInInventory = 79,
	ShouldDestroyOnSpellCast = 80,
	ActionType = 81,
	ItemCanMove = 82,
	ClimbOn = 83,
	ItemTemplateDestroyed = 84,
	InteractionDisabled = 85,
	IsStoryItem = 86,
	Ladder = 87,
	WalkOn = 88,
	UseAction = 89,
	OwnedAsLoot = 90,
	OwneeCurrent = 91,
	ServerDynamicLayerOwner = 92,
	ServerIsCurrentOwner = 93,
	ServerIsLatestOwner = 94,
	ServerIsPreviousLatestOwner = 95,
	ServerIsPreviousOwner = 96,
	ServerIsOriginalOwner = 97,
	ServerOwneeHistory = 98,
	ServerOwneeRequest = 99,
	InventoryMember = 100,
	InventoryData = 101,
	InventoryOwner = 102,
	InventoryIsOwned = 103,
	InventoryContainer = 104,
	InventoryMemberTransform = 105,
	InventoryStack = 106,
	InventoryStackMember = 107,
	InventoryWeight = 108,
	Wielded = 109,
	CanBeInInventory = 110,
	CannotBePickpocketed = 111,
	CannotBeTakenOut = 112,
	DropOnDeathBlocked = 113,
	NewItemsInside = 114,
	NonTradable = 115,
	ServerInventoryContainerData = 116,
	ServerInventoryGroupCheck = 117,
	ServerShapeshiftEquipmentHistory = 118,
	CharacterHasGeneratedTradeTreasure = 119,
	ServerInventoryIsReplicatedWith = 120,
	ItemHasGeneratedTreasure = 121,
	ReadyToBeAddedToInventory = 122,
	InventoryPropertyCanBePickpocketed = 123,
	ScriptPropertyCanBePickpocketed = 124,
	InventoryPropertyIsDroppedOnDeath = 125,
	ScriptPropertyIsDroppedOnDeath = 126,
	InventoryPropertyIsTradable = 127,
	ScriptPropertyIsTradable = 128,
	PartyView = 129,
	PartyComposition = 130,
	PartyPortals = 131,
	PartyRecipes = 132,
	PartyWaypoints = 133,
	PartyMember = 134,
	PartyFollower = 135,
	CurrentlyFollowingParty = 136,
	BlockFollow = 137,
	Recruiter = 138,
	ServerUserSnapshot = 139,
	EscortFollower = 140,
	EscortLeader = 141,
	EscortLeaderPriority = 142,
	EscortMember = 143,
	EscortStragglersTracker = 144,
	EscortHasStragglers = 145,
	CampChest = 146,
	CampEndTheDayState = 147,
	CampQuality = 148,
	CampSettings = 149,
	CampSupply = 150,
	CampTotalSupplies = 151,
	CampPresence = 152,
	IsInCombat = 153,
	CombatParticipant = 154,
	CombatState = 155,
	TurnBased = 156,
	TurnOrder = 157,
	IsCombatPaused = 158,
	ThreatRange = 159,
	Death = 160,
	DeathState = 161,
	DeadByDefault = 162,
	ServerKiller = 163,
	ServerDeathState = 164,
	ServerDeathContinue = 165,
	HitTarget = 166,
	HitAttacker = 167,
	HitWeapon = 168,
	HitThrownObject = 169,
	HitProxy = 170,
	HitProxyOwner = 171,
	HitReaction = 172,
	Identity = 173,
	IdentityState = 174,
	OriginalIdentity = 175,
	ServerCanStartCombat = 176,
	ServerCombatGroupMapping = 177,
	ServerEnterRequest = 178,
	ServerFleeBlocked = 179,
	ServerImmediateJoin = 180,
	ServerAiInterestedInItems = 181,
	ServerAiInterestingItem = 182,
	ServerAiArchetype = 183,
	ServerAiModifiers = 184,
	SpellContainer = 185,
	PlayerPrepareSpell = 186,
	CCPrepareSpell = 187,
	SpellModificationContainer = 188,
	AddedSpells = 189,
	SpellBookCooldowns = 190,
	LearnedSpells = 191,
	SpellAiConditions = 192,
	OnDamageSpells = 193,
	SpellCastAnimationInfo = 194,
	SpellCastCache = 195,
	SpellCastCanBeTargeted = 196,
	SpellCastIsCasting = 197,
	SpellCastInterruptResults = 198,
	SpellCastMovement = 199,
	SpellCastRolls = 200,
	SpellCastState = 201,
	SpellSyncTargeting = 202,
	ServerSpellCastHitDelay = 203,
	ServerSpellCastResponsible = 204,
	ServerSpellClientInitiated = 205,
	ServerSpellExternals = 206,
	ServerSpellHitRegister = 207,
	ServerSpellInterruptRequests = 208,
	ServerSpellInterruptResults = 209,
	ServerSpellCastState = 210,
	InterruptActionState = 211,
	InterruptConditionallyDisabled = 212,
	InterruptContainer = 213,
	InterruptData = 214,
	InterruptDecision = 215,
	InterruptPreferences = 216,
	InterruptPrepared = 217,
	InterruptZone = 218,
	InterruptZoneParticipant = 219,
	InterruptZoneSource = 220,
	ServerInterruptAddRemoveRequests = 221,
	ServerInterruptActionRequests = 222,
	ServerInterruptZoneRequests = 223,
	ServerInterruptInitialParticipants = 224,
	ServerInterruptTurnOrderInZone = 225,
	Sight = 226,
	SightData = 227,
	SightEntityViewshed = 228,
	StatusContainer = 229,
	StatusCause = 230,
	StatusID = 231,
	StatusIncapacitated = 232,
	StatusLifetime = 233,
	StatusVisualDisabled = 234,
	ServerStatusCause = 235,
	ServerStatus = 236,
	ServerStatusOwnership = 237,
	ServerStatusDifficultyModifiers = 238,
	ServerStatusUnique = 239,
	ServerStatusPerforming = 240,
	ServerStatusActive = 241,
	ServerStatusAddedFromSaveLoad = 242,
	ServerStatusAura = 243,
	TadpoleTreeState = 244,
	TadpolePowers = 245,
	Tadpoled = 246,
	HalfIllithid = 247,
	FullIllithid = 248,
	CCCompanionDefinition = 249,
	CCDefinitionCommon = 250,
	CCFullRespecDefinition = 251,
	CCRespecDefinition = 252,
	CCChangeAppearanceDefinition = 253,
	CCCharacterDefinition = 254,
	CCSessionCommon = 255,
	CCState = 256,
	CCLevelUp = 257,
	CCLevelUpDefinition = 258,
	CCFullRespec = 259,
	CCRespec = 260,
	CCCreation = 261,
	CCDefinitionLevelUp = 262,
	ClientCCDefinitionState = 263,
	ClientCCDefinitionStateEx = 264,
	ClientCCBaseDefinitionState = 265,
	ClientCCCompanionDefinition = 266,
	ClientCCLevelUpDefinition = 267,
	ClientCCChangeAppearanceDefinition = 268,
	ClientCCFullRespecDefinition = 269,
	ClientCCDummyDefinition = 270,
	ServerCCAppearanceVisualTag = 271,
	ServerCCEquipmentSetRequest = 272,
	ServerCCGod = 273,
	ServerCCUpdates = 274,
	ServerCCIsCustom = 275,
	Background = 276,
	God = 277,
	Proficiency = 278,
	ProficiencyGroup = 279,
	LevelUp = 280,
	Floating = 281,
	Voice = 282,
	CustomIcon = 283,
	CharacterCreationStats = 284,
	Disarmable = 285,
	ShortRest = 286,
	IsSummon = 287,
	SummonContainer = 288,
	Stealth = 289,
	ClientControl = 290,
	IsGlobal = 291,
	Savegame = 292,
	DisabledEquipment = 293,
	LootingState = 294,
	Loot = 295,
	Lock = 296,
	Key = 297,
	SummonLifetime = 298,
	Invisibility = 299,
	Icon = 300,
	HotbarContainer = 301,
	HotbarDecks = 302,
	OriginTag = 303,
	OriginPassives = 304,
	ClassTag = 305,
	BackgroundTag = 306,
	OriginAppearanceTag = 307,
	VoiceTag = 308,
	GodTag = 309,
	BackgroundPassives = 310,
	GlobalShortRestDisabled = 311,
	GlobalLongRestDisabled = 312,
	StoryShortRestDisabled = 313,
	FleeCapability = 314,
	CanDoRest = 315,
	IsInTurnBasedMode = 316,
	ItemBoosts = 317,
	ActiveCharacterLight = 318,
	Visual = 319,
	ClientEquipmentVisuals = 320,
	AnimationSet = 321,
	AnimationBlueprint = 322,
	CanModifyHealth = 323,
	AvailableLevel = 324,
	CanBeLooted = 325,
	CanBeDisarmed = 326,
	CanTriggerRandomCasts = 327,
	CanDoActions = 328,
	ActionUseConditions = 329,
	CanMove = 330,
	CanSense = 331,
	Concentration = 332,
	Darkness = 333,
	DualWielding = 334,
	GameObjectVisual = 335,
	AppearanceOverride = 336,
	CharacterCreationTemplateOverride = 337,
	GravityDisabled = 338,
	GravityDisabledUntilMoved = 339,
	CanBeWielded = 340,
	Avatar = 341,
	HasExclamationDialog = 342,
	Trader = 343,
	Ambushing = 344,
	InteractionFilter = 345,
	DisplayName = 346,
	CustomName = 347,
	Equipable = 348,
	GameplayLight = 349,
	Race = 350,
	CanTravel = 351,
	Movement = 352,
	ObjectInteraction = 353,
	StaticPhysics = 354,
	Pathing = 355,
	Steering = 356,
	CanDeflectProjectiles = 357,
	ActiveSkeletonSlots = 358,
	Net = 359,
	Physics = 360,
	FTBParticipant = 361,
	FTBZoneBlockReason = 362,
	FTBRespect = 363,
	ApprovalRatings = 364,
	AttitudesToPlayers = 365,
	CharacterCreationAppearance = 366,
	Active = 367,
	EquipmentVisual = 368,
	Repose = 369,
	OriginalTemplate = 370,
	CanEnterChasm = 371,
	DialogState = 372,
	EncumbranceState = 373,
	EncumbranceStats = 374,
	Unsheath = 375,
	UseSocket = 376,
	UserAvatar = 377,
	UserReservedFor = 378,
	BackgroundGoals = 379,
	CalendarDaysPassed = 380,
	CalendarStartingDate = 381,
	ProgressionContainer = 382,
	ProgressionMeta = 383,
	ProgressionChangedContainers = 384,
	CanSeeThrough = 385,
	CanShootThrough = 386,
	CanWalkThrough = 387,
	ShootThroughType = 388,
	ServerRecruitedBy = 389,
	ServerGameTimer = 390,
	ServerExperienceGaveOut = 391,
	ServerReplicationDependency = 392,
	ServerIsUnsummoning = 393,
	ServerActivationGroupContainer = 394,
	ServerAnubisTag = 395,
	ServerDialogTag = 396,
	ServerDisplayNameList = 397,
	ServerIconList = 398,
	ServerRaceTag = 399,
	ServerTemplateTag = 400,
	ServerBoostTag = 401,
	ServerSafePosition = 402,
	ServerAnubisExecutor = 403,
	ServerLeader = 404,
	ServerBreadcrumb = 405,
	ServerDelayDeathCause = 406,
	ServerPickpocket = 407,
	ServerReplicationDependencyOwner = 408,
	ServerGameplayLightEquipment = 409,
	HistoryTargetUUID = 410,
	ServerGameplayLightChanges = 411,
	ServerPeersInRange = 412,
	ServerSurface = 413,
	ServerDisarmAttempt = 414,
	ServerUnsheath = 415,
	ServerUnsheathScriptOverride = 416,
	ServerIsLightBlocker = 417,
	ServerIsVisionBlocker = 418,
	ServerDarknessActive = 419,
	ServerHotbarOrder = 420,
	ServerInventoryItemDataPopulated = 421,
	ServerVariableManager = 422,
	IsMarkedForDeletion = 423,
	JumpFollow = 424,
	ServerToggledPassives = 425,
	ServerPassiveBase = 426,
	ServerPassivePersistentData = 427,
	ServerScriptPassives = 428,
	ServerCharacter = 429,
	ServerItem = 430,
	ProjectileSource = 431,
	ServerProjectile = 432,
	ServerProjectileAttachment = 433,
	ServerProjectileSpell = 434,
	ServerProjectileInitialization = 435,
	ServerOsirisTag = 436,
	ClientCharacter = 437,
	ClientItem = 438,
	ClientProjectile = 439,
	ArmorClassBoost = 440,
	AbilityBoost = 441,
	RollBonusBoost = 442,
	AdvantageBoost = 443,
	ActionResourceValueBoost = 444,
	CriticalHitBoost = 445,
	AbilityFailedSavingThrowBoost = 446,
	ResistanceBoost = 447,
	WeaponDamageResistanceBoost = 448,
	ProficiencyBonusOverrideBoost = 449,
	JumpMaxDistanceMultiplierBoost = 450,
	HalveWeaponDamageBoost = 451,
	UnlockSpellBoost = 452,
	SourceAdvantageBoost = 453,
	ProficiencyBonusBoost = 454,
	ProficiencyBoost = 455,
	IncreaseMaxHPBoost = 456,
	ActionResourceBlockBoost = 457,
	StatusImmunityBoost = 458,
	UseBoosts = 459,
	CannotHarmCauseEntityBoost = 460,
	TemporaryHPBoost = 461,
	WeightBoost = 462,
	WeightCategoryBoost = 463,
	FactionOverrideBoost = 464,
	ActionResourceMultiplierBoost = 465,
	BlockRegainHPBoost = 466,
	InitiativeBoost = 467,
	DarkvisionRangeBoost = 468,
	DarkvisionRangeMinBoost = 469,
	DarkvisionRangeOverrideBoost = 470,
	AddTagBoost = 471,
	IgnoreDamageThresholdMinBoost = 472,
	SkillBoost = 473,
	WeaponDamageBoost = 474,
	NullifyAbilityBoost = 475,
	RerollBoost = 476,
	DownedStatusBoost = 477,
	WeaponEnchantmentBoost = 478,
	GuaranteedChanceRollOutcomeBoost = 479,
	AttributeBoost = 480,
	GameplayLightBoost = 481,
	DualWieldingBoost = 482,
	SavantBoost = 483,
	MinimumRollResultBoost = 484,
	CharacterWeaponDamageBoost = 485,
	ProjectileDeflectBoost = 486,
	AbilityOverrideMinimumBoost = 487,
	ACOverrideFormulaBoost = 488,
	FallDamageMultiplierBoost = 489,
	ActiveCharacterLightBoost = 490,
	WeaponAttackTypeOverrideBoost = 491,
	WeaponDamageDieOverrideBoost = 492,
	CarryCapacityMultiplierBoost = 493,
	WeaponPropertyBoost = 494,
	WeaponAttackRollAbilityOverrideBoost = 495,
	SightRangeAdditiveBoost = 496,
	SightRangeMinimumBoost = 497,
	SightRangeMaximumBoost = 498,
	SightRangeOverrideBoost = 499,
	MovementSpeedLimitBoost = 500,
	UnlockSpellVariantBoost = 501,
	DetectCrimesBlockBoost = 502,
	BlockAbilityModifierFromACBoost = 503,
	ScaleMultiplierBoost = 504,
	DamageReductionBoost = 505,
	ReduceCriticalAttackThresholdBoost = 506,
	PhysicalForceRangeBonusBoost = 507,
	ObjectSizeBoost = 508,
	ObjectSizeOverrideBoost = 509,
	AiArchetypeOverrideBoost = 510,
	ExpertiseBonusBoost = 511,
	EntityThrowDamageBoost = 512,
	WeaponDamageTypeOverrideBoost = 513,
	MaximizeHealingBoost = 514,
	DamageBonusBoost = 515,
	AdvanceSpellsBoost = 516,
	SpellResistanceBoost = 517,
	WeaponAttackRollBonusBoost = 518,
	SpellSaveDCBoost = 519,
	RedirectDamageBoost = 520,
	CanSeeThroughBoost = 521,
	CanShootThroughBoost = 522,
	CanWalkThroughBoost = 523,
	MonkWeaponDamageDiceOverrideBoost = 524,
	HorizontalFOVOverrideBoost = 525,
	CharacterUnarmedDamageBoost = 526,
	ActionResourceReplenishTypeOverrideBoost = 527,
	ActionResourcePreventReductionBoost = 528,
	AttackSpellOverrideBoost = 529,
	LockBoost = 530,
	IgnorePointBlankDisadvantageBoost = 531,
	CriticalHitExtraDiceBoost = 532,
	DodgeAttackRollBoost = 533,
	GameplayObscurityBoost = 534,
	MaximumRollResultBoost = 535,
	UnlockInterruptBoost = 536,
	JumpMaxDistanceBonusBoost = 537,
	ArmorAbilityModifierCapOverrideBoost = 538,
	IgnoreResistanceBoost = 539,
	ConcentrationIgnoreDamageBoost = 540,
	IgnoreLowGroundPenaltyBoost = 541,
	IgnoreSurfaceCoverBoost = 542,
	Uuid = 543,
	UuidToHandleMapping = 544,
	Max = 545,
	[0] = "ActionResources",
	[1] = "Armor",
	[2] = "ArmorSetState",
	[3] = "BaseHp",
	[4] = "Bound",
	[5] = "IsCharacter",
	[6] = "Data",
	[7] = "Detached",
	[8] = "Experience",
	[9] = "Health",
	[10] = "Resistances",
	[11] = "Hearing",
	[12] = "SpellBook",
	[13] = "Stats",
	[14] = "StatusImmunities",
	[15] = "SurfacePathInfluences",
	[16] = "Use",
	[17] = "Value",
	[18] = "Weapon",
	[19] = "Wielding",
	[20] = "CustomStats",
	[21] = "BoostCondition",
	[22] = "BoostsContainer",
	[23] = "ServerBoostBase",
	[24] = "ServerStatusBoostsProcessed",
	[25] = "ActionResourceConsumeMultiplierBoost",
	[26] = "Tag",
	[27] = "SpellBookPrepares",
	[28] = "Transform",
	[29] = "BoostInfo",
	[30] = "Relation",
	[31] = "Faction",
	[32] = "CanInteract",
	[33] = "CanSpeak",
	[34] = "Origin",
	[35] = "Level",
	[36] = "EocLevel",
	[37] = "Classes",
	[38] = "MaterialParameterOverride",
	[39] = "OffStage",
	[40] = "PickingState",
	[41] = "Player",
	[42] = "SimpleCharacter",
	[43] = "Speaker",
	[44] = "WeaponSet",
	[45] = "DifficultyCheck",
	[46] = "AttributeFlags",
	[47] = "BodyType",
	[48] = "ObjectSize",
	[49] = "BaseStats",
	[50] = "Expertise",
	[51] = "HealBlock",
	[52] = "Ruleset",
	[53] = "RulesetModifiers",
	[54] = "ServerBaseData",
	[55] = "ServerBaseSize",
	[56] = "ServerBaseStats",
	[57] = "ServerBaseWeapon",
	[58] = "ServerBaseProficiency",
	[59] = "ServerProficiencyGroupStats",
	[60] = "ShapeshiftState",
	[61] = "ShapeshiftHealthReservation",
	[62] = "ServerShapeshiftStates",
	[63] = "AnimationWaterfall",
	[64] = "DynamicAnimationTags",
	[65] = "TemplateAnimationSetOverride",
	[66] = "PassiveContainer",
	[67] = "Passive",
	[68] = "PassiveUsageCount",
	[69] = "ItemDestroyed",
	[70] = "ItemDye",
	[71] = "MapMarkerStyle",
	[72] = "ItemPortal",
	[73] = "IsDoor",
	[74] = "ExamineDisabled",
	[75] = "ItemHasMoved",
	[76] = "HasOpened",
	[77] = "IsGold",
	[78] = "IsItem",
	[79] = "NewInInventory",
	[80] = "ShouldDestroyOnSpellCast",
	[81] = "ActionType",
	[82] = "ItemCanMove",
	[83] = "ClimbOn",
	[84] = "ItemTemplateDestroyed",
	[85] = "InteractionDisabled",
	[86] = "IsStoryItem",
	[87] = "Ladder",
	[88] = "WalkOn",
	[89] = "UseAction",
	[90] = "OwnedAsLoot",
	[91] = "OwneeCurrent",
	[92] = "ServerDynamicLayerOwner",
	[93] = "ServerIsCurrentOwner",
	[94] = "ServerIsLatestOwner",
	[95] = "ServerIsPreviousLatestOwner",
	[96] = "ServerIsPreviousOwner",
	[97] = "ServerIsOriginalOwner",
	[98] = "ServerOwneeHistory",
	[99] = "ServerOwneeRequest",
	[100] = "InventoryMember",
	[101] = "InventoryData",
	[102] = "InventoryOwner",
	[103] = "InventoryIsOwned",
	[104] = "InventoryContainer",
	[105] = "InventoryMemberTransform",
	[106] = "InventoryStack",
	[107] = "InventoryStackMember",
	[108] = "InventoryWeight",
	[109] = "Wielded",
	[110] = "CanBeInInventory",
	[111] = "CannotBePickpocketed",
	[112] = "CannotBeTakenOut",
	[113] = "DropOnDeathBlocked",
	[114] = "NewItemsInside",
	[115] = "NonTradable",
	[116] = "ServerInventoryContainerData",
	[117] = "ServerInventoryGroupCheck",
	[118] = "ServerShapeshiftEquipmentHistory",
	[119] = "CharacterHasGeneratedTradeTreasure",
	[120] = "ServerInventoryIsReplicatedWith",
	[121] = "ItemHasGeneratedTreasure",
	[122] = "ReadyToBeAddedToInventory",
	[123] = "InventoryPropertyCanBePickpocketed",
	[124] = "ScriptPropertyCanBePickpocketed",
	[125] = "InventoryPropertyIsDroppedOnDeath",
	[126] = "ScriptPropertyIsDroppedOnDeath",
	[127] = "InventoryPropertyIsTradable",
	[128] = "ScriptPropertyIsTradable",
	[129] = "PartyView",
	[130] = "PartyComposition",
	[131] = "PartyPortals",
	[132] = "PartyRecipes",
	[133] = "PartyWaypoints",
	[134] = "PartyMember",
	[135] = "PartyFollower",
	[136] = "CurrentlyFollowingParty",
	[137] = "BlockFollow",
	[138] = "Recruiter",
	[139] = "ServerUserSnapshot",
	[140] = "EscortFollower",
	[141] = "EscortLeader",
	[142] = "EscortLeaderPriority",
	[143] = "EscortMember",
	[144] = "EscortStragglersTracker",
	[145] = "EscortHasStragglers",
	[146] = "CampChest",
	[147] = "CampEndTheDayState",
	[148] = "CampQuality",
	[149] = "CampSettings",
	[150] = "CampSupply",
	[151] = "CampTotalSupplies",
	[152] = "CampPresence",
	[153] = "IsInCombat",
	[154] = "CombatParticipant",
	[155] = "CombatState",
	[156] = "TurnBased",
	[157] = "TurnOrder",
	[158] = "IsCombatPaused",
	[159] = "ThreatRange",
	[160] = "Death",
	[161] = "DeathState",
	[162] = "DeadByDefault",
	[163] = "ServerKiller",
	[164] = "ServerDeathState",
	[165] = "ServerDeathContinue",
	[166] = "HitTarget",
	[167] = "HitAttacker",
	[168] = "HitWeapon",
	[169] = "HitThrownObject",
	[170] = "HitProxy",
	[171] = "HitProxyOwner",
	[172] = "HitReaction",
	[173] = "Identity",
	[174] = "IdentityState",
	[175] = "OriginalIdentity",
	[176] = "ServerCanStartCombat",
	[177] = "ServerCombatGroupMapping",
	[178] = "ServerEnterRequest",
	[179] = "ServerFleeBlocked",
	[180] = "ServerImmediateJoin",
	[181] = "ServerAiInterestedInItems",
	[182] = "ServerAiInterestingItem",
	[183] = "ServerAiArchetype",
	[184] = "ServerAiModifiers",
	[185] = "SpellContainer",
	[186] = "PlayerPrepareSpell",
	[187] = "CCPrepareSpell",
	[188] = "SpellModificationContainer",
	[189] = "AddedSpells",
	[190] = "SpellBookCooldowns",
	[191] = "LearnedSpells",
	[192] = "SpellAiConditions",
	[193] = "OnDamageSpells",
	[194] = "SpellCastAnimationInfo",
	[195] = "SpellCastCache",
	[196] = "SpellCastCanBeTargeted",
	[197] = "SpellCastIsCasting",
	[198] = "SpellCastInterruptResults",
	[199] = "SpellCastMovement",
	[200] = "SpellCastRolls",
	[201] = "SpellCastState",
	[202] = "SpellSyncTargeting",
	[203] = "ServerSpellCastHitDelay",
	[204] = "ServerSpellCastResponsible",
	[205] = "ServerSpellClientInitiated",
	[206] = "ServerSpellExternals",
	[207] = "ServerSpellHitRegister",
	[208] = "ServerSpellInterruptRequests",
	[209] = "ServerSpellInterruptResults",
	[210] = "ServerSpellCastState",
	[211] = "InterruptActionState",
	[212] = "InterruptConditionallyDisabled",
	[213] = "InterruptContainer",
	[214] = "InterruptData",
	[215] = "InterruptDecision",
	[216] = "InterruptPreferences",
	[217] = "InterruptPrepared",
	[218] = "InterruptZone",
	[219] = "InterruptZoneParticipant",
	[220] = "InterruptZoneSource",
	[221] = "ServerInterruptAddRemoveRequests",
	[222] = "ServerInterruptActionRequests",
	[223] = "ServerInterruptZoneRequests",
	[224] = "ServerInterruptInitialParticipants",
	[225] = "ServerInterruptTurnOrderInZone",
	[226] = "Sight",
	[227] = "SightData",
	[228] = "SightEntityViewshed",
	[229] = "StatusContainer",
	[230] = "StatusCause",
	[231] = "StatusID",
	[232] = "StatusIncapacitated",
	[233] = "StatusLifetime",
	[234] = "StatusVisualDisabled",
	[235] = "ServerStatusCause",
	[236] = "ServerStatus",
	[237] = "ServerStatusOwnership",
	[238] = "ServerStatusDifficultyModifiers",
	[239] = "ServerStatusUnique",
	[240] = "ServerStatusPerforming",
	[241] = "ServerStatusActive",
	[242] = "ServerStatusAddedFromSaveLoad",
	[243] = "ServerStatusAura",
	[244] = "TadpoleTreeState",
	[245] = "TadpolePowers",
	[246] = "Tadpoled",
	[247] = "HalfIllithid",
	[248] = "FullIllithid",
	[249] = "CCCompanionDefinition",
	[250] = "CCDefinitionCommon",
	[251] = "CCFullRespecDefinition",
	[252] = "CCRespecDefinition",
	[253] = "CCChangeAppearanceDefinition",
	[254] = "CCCharacterDefinition",
	[255] = "CCSessionCommon",
	[256] = "CCState",
	[257] = "CCLevelUp",
	[258] = "CCLevelUpDefinition",
	[259] = "CCFullRespec",
	[260] = "CCRespec",
	[261] = "CCCreation",
	[262] = "CCDefinitionLevelUp",
	[263] = "ClientCCDefinitionState",
	[264] = "ClientCCDefinitionStateEx",
	[265] = "ClientCCBaseDefinitionState",
	[266] = "ClientCCCompanionDefinition",
	[267] = "ClientCCLevelUpDefinition",
	[268] = "ClientCCChangeAppearanceDefinition",
	[269] = "ClientCCFullRespecDefinition",
	[270] = "ClientCCDummyDefinition",
	[271] = "ServerCCAppearanceVisualTag",
	[272] = "ServerCCEquipmentSetRequest",
	[273] = "ServerCCGod",
	[274] = "ServerCCUpdates",
	[275] = "ServerCCIsCustom",
	[276] = "Background",
	[277] = "God",
	[278] = "Proficiency",
	[279] = "ProficiencyGroup",
	[280] = "LevelUp",
	[281] = "Floating",
	[282] = "Voice",
	[283] = "CustomIcon",
	[284] = "CharacterCreationStats",
	[285] = "Disarmable",
	[286] = "ShortRest",
	[287] = "IsSummon",
	[288] = "SummonContainer",
	[289] = "Stealth",
	[290] = "ClientControl",
	[291] = "IsGlobal",
	[292] = "Savegame",
	[293] = "DisabledEquipment",
	[294] = "LootingState",
	[295] = "Loot",
	[296] = "Lock",
	[297] = "Key",
	[298] = "SummonLifetime",
	[299] = "Invisibility",
	[300] = "Icon",
	[301] = "HotbarContainer",
	[302] = "HotbarDecks",
	[303] = "OriginTag",
	[304] = "OriginPassives",
	[305] = "ClassTag",
	[306] = "BackgroundTag",
	[307] = "OriginAppearanceTag",
	[308] = "VoiceTag",
	[309] = "GodTag",
	[310] = "BackgroundPassives",
	[311] = "GlobalShortRestDisabled",
	[312] = "GlobalLongRestDisabled",
	[313] = "StoryShortRestDisabled",
	[314] = "FleeCapability",
	[315] = "CanDoRest",
	[316] = "IsInTurnBasedMode",
	[317] = "ItemBoosts",
	[318] = "ActiveCharacterLight",
	[319] = "Visual",
	[320] = "ClientEquipmentVisuals",
	[321] = "AnimationSet",
	[322] = "AnimationBlueprint",
	[323] = "CanModifyHealth",
	[324] = "AvailableLevel",
	[325] = "CanBeLooted",
	[326] = "CanBeDisarmed",
	[327] = "CanTriggerRandomCasts",
	[328] = "CanDoActions",
	[329] = "ActionUseConditions",
	[330] = "CanMove",
	[331] = "CanSense",
	[332] = "Concentration",
	[333] = "Darkness",
	[334] = "DualWielding",
	[335] = "GameObjectVisual",
	[336] = "AppearanceOverride",
	[337] = "CharacterCreationTemplateOverride",
	[338] = "GravityDisabled",
	[339] = "GravityDisabledUntilMoved",
	[340] = "CanBeWielded",
	[341] = "Avatar",
	[342] = "HasExclamationDialog",
	[343] = "Trader",
	[344] = "Ambushing",
	[345] = "InteractionFilter",
	[346] = "DisplayName",
	[347] = "CustomName",
	[348] = "Equipable",
	[349] = "GameplayLight",
	[350] = "Race",
	[351] = "CanTravel",
	[352] = "Movement",
	[353] = "ObjectInteraction",
	[354] = "StaticPhysics",
	[355] = "Pathing",
	[356] = "Steering",
	[357] = "CanDeflectProjectiles",
	[358] = "ActiveSkeletonSlots",
	[359] = "Net",
	[360] = "Physics",
	[361] = "FTBParticipant",
	[362] = "FTBZoneBlockReason",
	[363] = "FTBRespect",
	[364] = "ApprovalRatings",
	[365] = "AttitudesToPlayers",
	[366] = "CharacterCreationAppearance",
	[367] = "Active",
	[368] = "EquipmentVisual",
	[369] = "Repose",
	[370] = "OriginalTemplate",
	[371] = "CanEnterChasm",
	[372] = "DialogState",
	[373] = "EncumbranceState",
	[374] = "EncumbranceStats",
	[375] = "Unsheath",
	[376] = "UseSocket",
	[377] = "UserAvatar",
	[378] = "UserReservedFor",
	[379] = "BackgroundGoals",
	[380] = "CalendarDaysPassed",
	[381] = "CalendarStartingDate",
	[382] = "ProgressionContainer",
	[383] = "ProgressionMeta",
	[384] = "ProgressionChangedContainers",
	[385] = "CanSeeThrough",
	[386] = "CanShootThrough",
	[387] = "CanWalkThrough",
	[388] = "ShootThroughType",
	[389] = "ServerRecruitedBy",
	[390] = "ServerGameTimer",
	[391] = "ServerExperienceGaveOut",
	[392] = "ServerReplicationDependency",
	[393] = "ServerIsUnsummoning",
	[394] = "ServerActivationGroupContainer",
	[395] = "ServerAnubisTag",
	[396] = "ServerDialogTag",
	[397] = "ServerDisplayNameList",
	[398] = "ServerIconList",
	[399] = "ServerRaceTag",
	[400] = "ServerTemplateTag",
	[401] = "ServerBoostTag",
	[402] = "ServerSafePosition",
	[403] = "ServerAnubisExecutor",
	[404] = "ServerLeader",
	[405] = "ServerBreadcrumb",
	[406] = "ServerDelayDeathCause",
	[407] = "ServerPickpocket",
	[408] = "ServerReplicationDependencyOwner",
	[409] = "ServerGameplayLightEquipment",
	[410] = "HistoryTargetUUID",
	[411] = "ServerGameplayLightChanges",
	[412] = "ServerPeersInRange",
	[413] = "ServerSurface",
	[414] = "ServerDisarmAttempt",
	[415] = "ServerUnsheath",
	[416] = "ServerUnsheathScriptOverride",
	[417] = "ServerIsLightBlocker",
	[418] = "ServerIsVisionBlocker",
	[419] = "ServerDarknessActive",
	[420] = "ServerHotbarOrder",
	[421] = "ServerInventoryItemDataPopulated",
	[422] = "ServerVariableManager",
	[423] = "IsMarkedForDeletion",
	[424] = "JumpFollow",
	[425] = "ServerToggledPassives",
	[426] = "ServerPassiveBase",
	[427] = "ServerPassivePersistentData",
	[428] = "ServerScriptPassives",
	[429] = "ServerCharacter",
	[430] = "ServerItem",
	[431] = "ProjectileSource",
	[432] = "ServerProjectile",
	[433] = "ServerProjectileAttachment",
	[434] = "ServerProjectileSpell",
	[435] = "ServerProjectileInitialization",
	[436] = "ServerOsirisTag",
	[437] = "ClientCharacter",
	[438] = "ClientItem",
	[439] = "ClientProjectile",
	[440] = "ArmorClassBoost",
	[441] = "AbilityBoost",
	[442] = "RollBonusBoost",
	[443] = "AdvantageBoost",
	[444] = "ActionResourceValueBoost",
	[445] = "CriticalHitBoost",
	[446] = "AbilityFailedSavingThrowBoost",
	[447] = "ResistanceBoost",
	[448] = "WeaponDamageResistanceBoost",
	[449] = "ProficiencyBonusOverrideBoost",
	[450] = "JumpMaxDistanceMultiplierBoost",
	[451] = "HalveWeaponDamageBoost",
	[452] = "UnlockSpellBoost",
	[453] = "SourceAdvantageBoost",
	[454] = "ProficiencyBonusBoost",
	[455] = "ProficiencyBoost",
	[456] = "IncreaseMaxHPBoost",
	[457] = "ActionResourceBlockBoost",
	[458] = "StatusImmunityBoost",
	[459] = "UseBoosts",
	[460] = "CannotHarmCauseEntityBoost",
	[461] = "TemporaryHPBoost",
	[462] = "WeightBoost",
	[463] = "WeightCategoryBoost",
	[464] = "FactionOverrideBoost",
	[465] = "ActionResourceMultiplierBoost",
	[466] = "BlockRegainHPBoost",
	[467] = "InitiativeBoost",
	[468] = "DarkvisionRangeBoost",
	[469] = "DarkvisionRangeMinBoost",
	[470] = "DarkvisionRangeOverrideBoost",
	[471] = "AddTagBoost",
	[472] = "IgnoreDamageThresholdMinBoost",
	[473] = "SkillBoost",
	[474] = "WeaponDamageBoost",
	[475] = "NullifyAbilityBoost",
	[476] = "RerollBoost",
	[477] = "DownedStatusBoost",
	[478] = "WeaponEnchantmentBoost",
	[479] = "GuaranteedChanceRollOutcomeBoost",
	[480] = "AttributeBoost",
	[481] = "GameplayLightBoost",
	[482] = "DualWieldingBoost",
	[483] = "SavantBoost",
	[484] = "MinimumRollResultBoost",
	[485] = "CharacterWeaponDamageBoost",
	[486] = "ProjectileDeflectBoost",
	[487] = "AbilityOverrideMinimumBoost",
	[488] = "ACOverrideFormulaBoost",
	[489] = "FallDamageMultiplierBoost",
	[490] = "ActiveCharacterLightBoost",
	[491] = "WeaponAttackTypeOverrideBoost",
	[492] = "WeaponDamageDieOverrideBoost",
	[493] = "CarryCapacityMultiplierBoost",
	[494] = "WeaponPropertyBoost",
	[495] = "WeaponAttackRollAbilityOverrideBoost",
	[496] = "SightRangeAdditiveBoost",
	[497] = "SightRangeMinimumBoost",
	[498] = "SightRangeMaximumBoost",
	[499] = "SightRangeOverrideBoost",
	[500] = "MovementSpeedLimitBoost",
	[501] = "UnlockSpellVariantBoost",
	[502] = "DetectCrimesBlockBoost",
	[503] = "BlockAbilityModifierFromACBoost",
	[504] = "ScaleMultiplierBoost",
	[505] = "DamageReductionBoost",
	[506] = "ReduceCriticalAttackThresholdBoost",
	[507] = "PhysicalForceRangeBonusBoost",
	[508] = "ObjectSizeBoost",
	[509] = "ObjectSizeOverrideBoost",
	[510] = "AiArchetypeOverrideBoost",
	[511] = "ExpertiseBonusBoost",
	[512] = "EntityThrowDamageBoost",
	[513] = "WeaponDamageTypeOverrideBoost",
	[514] = "MaximizeHealingBoost",
	[515] = "DamageBonusBoost",
	[516] = "AdvanceSpellsBoost",
	[517] = "SpellResistanceBoost",
	[518] = "WeaponAttackRollBonusBoost",
	[519] = "SpellSaveDCBoost",
	[520] = "RedirectDamageBoost",
	[521] = "CanSeeThroughBoost",
	[522] = "CanShootThroughBoost",
	[523] = "CanWalkThroughBoost",
	[524] = "MonkWeaponDamageDiceOverrideBoost",
	[525] = "HorizontalFOVOverrideBoost",
	[526] = "CharacterUnarmedDamageBoost",
	[527] = "ActionResourceReplenishTypeOverrideBoost",
	[528] = "ActionResourcePreventReductionBoost",
	[529] = "AttackSpellOverrideBoost",
	[530] = "LockBoost",
	[531] = "IgnorePointBlankDisadvantageBoost",
	[532] = "CriticalHitExtraDiceBoost",
	[533] = "DodgeAttackRollBoost",
	[534] = "GameplayObscurityBoost",
	[535] = "MaximumRollResultBoost",
	[536] = "UnlockInterruptBoost",
	[537] = "JumpMaxDistanceBonusBoost",
	[538] = "ArmorAbilityModifierCapOverrideBoost",
	[539] = "IgnoreResistanceBoost",
	[540] = "ConcentrationIgnoreDamageBoost",
	[541] = "IgnoreLowGroundPenaltyBoost",
	[542] = "IgnoreSurfaceCoverBoost",
	[543] = "Uuid",
	[544] = "UuidToHandleMapping",
	[545] = "Max",
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
	UISystem = 0,
	PickingHelperManager = 1,
	Max = 2,
	[0] = "UISystem",
	[1] = "PickingHelperManager",
	[2] = "Max",
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

--- @enum FunctorExecParamsType
Ext_Enums.FunctorExecParamsType = {
	Type1 = 1,
	Type2 = 2,
	Type3 = 3,
	Type4 = 4,
	Type5 = 5,
	Type6 = 6,
	Type7 = 7,
	Type8 = 8,
	[1] = "Type1",
	[2] = "Type2",
	[3] = "Type3",
	[4] = "Type4",
	[5] = "Type5",
	[6] = "Type6",
	[7] = "Type7",
	[8] = "Type8",
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
	MouseButtonRight = 2,
	MouseButtonMiddle = 4,
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
	[1] = "Border",
	[2] = "AlwaysUseWindowPadding",
	[4] = "ResizeX",
	[8] = "ResizeY",
	[16] = "AutoResizeX",
	[32] = "AutoResizeY",
	[64] = "ChildAlwaysAutoResize",
	[128] = "FrameStyle",
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
	Tab = 33,
	TabHovered = 34,
	TabActive = 35,
	TabUnfocused = 36,
	TabUnfocusedActive = 37,
	PlotLines = 38,
	PlotLinesHovered = 39,
	PlotHistogram = 40,
	PlotHistogramHovered = 41,
	TableHeaderBg = 42,
	TableBorderStrong = 43,
	TableBorderLight = 44,
	TableRowBg = 45,
	TableRowBgAlt = 46,
	TextSelectedBg = 47,
	DragDropTarget = 48,
	NavHighlight = 49,
	NavWindowingHighlight = 50,
	NavWindowingDimBg = 51,
	ModalWindowDimBg = 52,
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
	[33] = "Tab",
	[34] = "TabHovered",
	[35] = "TabActive",
	[36] = "TabUnfocused",
	[37] = "TabUnfocusedActive",
	[38] = "PlotLines",
	[39] = "PlotLinesHovered",
	[40] = "PlotHistogram",
	[41] = "PlotHistogramHovered",
	[42] = "TableHeaderBg",
	[43] = "TableBorderStrong",
	[44] = "TableBorderLight",
	[45] = "TableRowBg",
	[46] = "TableRowBgAlt",
	[47] = "TextSelectedBg",
	[48] = "DragDropTarget",
	[49] = "NavHighlight",
	[50] = "NavWindowingHighlight",
	[51] = "NavWindowingDimBg",
	[52] = "ModalWindowDimBg",
}

--- @enum GuiColorEditFlags
Ext_Enums.GuiColorEditFlags = {
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
	AlphaBar = 65536,
	AlphaPreview = 131072,
	AlphaPreviewHalf = 262144,
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
	[65536] = "AlphaBar",
	[131072] = "AlphaPreview",
	[262144] = "AlphaPreviewHalf",
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

--- @enum GuiInputTextFlags
Ext_Enums.GuiInputTextFlags = {
	CharsDecimal = 1,
	CharsHexadecimal = 2,
	CharsUppercase = 4,
	CharsNoBlank = 8,
	AutoSelectAll = 16,
	EnterReturnsTrue = 32,
	CallbackCompletion = 64,
	CallbackHistory = 128,
	CallbackAlways = 256,
	CallbackCharFilter = 512,
	AllowTabInput = 1024,
	CtrlEnterForNewLine = 2048,
	NoHorizontalScroll = 4096,
	AlwaysOverwrite = 8192,
	ReadOnly = 16384,
	Password = 32768,
	NoUndoRedo = 65536,
	CharsScientific = 131072,
	CallbackResize = 262144,
	CallbackEdit = 524288,
	EscapeClearsAll = 1048576,
	Multiline = 67108864,
	[1] = "CharsDecimal",
	[2] = "CharsHexadecimal",
	[4] = "CharsUppercase",
	[8] = "CharsNoBlank",
	[16] = "AutoSelectAll",
	[32] = "EnterReturnsTrue",
	[64] = "CallbackCompletion",
	[128] = "CallbackHistory",
	[256] = "CallbackAlways",
	[512] = "CallbackCharFilter",
	[1024] = "AllowTabInput",
	[2048] = "CtrlEnterForNewLine",
	[4096] = "NoHorizontalScroll",
	[8192] = "AlwaysOverwrite",
	[16384] = "ReadOnly",
	[32768] = "Password",
	[65536] = "NoUndoRedo",
	[131072] = "CharsScientific",
	[262144] = "CallbackResize",
	[524288] = "CallbackEdit",
	[1048576] = "EscapeClearsAll",
	[67108864] = "Multiline",
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

--- @enum GuiSliderFlags
Ext_Enums.GuiSliderFlags = {
	AlwaysClamp = 16,
	Logarithmic = 32,
	NoRoundToFormat = 64,
	NoInput = 128,
	[16] = "AlwaysClamp",
	[32] = "Logarithmic",
	[64] = "NoRoundToFormat",
	[128] = "NoInput",
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
	TabRounding = 22,
	TabBarBorderSize = 23,
	ButtonTextAlign = 24,
	SelectableTextAlign = 25,
	SeparatorTextBorderSize = 26,
	SeparatorTextAlign = 27,
	SeparatorTextPadding = 28,
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
	[22] = "TabRounding",
	[23] = "TabBarBorderSize",
	[24] = "ButtonTextAlign",
	[25] = "SelectableTextAlign",
	[26] = "SeparatorTextBorderSize",
	[27] = "SeparatorTextAlign",
	[28] = "SeparatorTextPadding",
}

--- @enum GuiTabBarFlags
Ext_Enums.GuiTabBarFlags = {
	Reorderable = 1,
	AutoSelectNewTabs = 2,
	TabListPopupButton = 4,
	NoCloseWithMiddleMouseButton = 8,
	NoTabListScrollingButtons = 16,
	NoTooltip = 32,
	FittingPolicyResizeDown = 64,
	FittingPolicyScroll = 128,
	FittingPolicyMask = 192,
	[1] = "Reorderable",
	[2] = "AutoSelectNewTabs",
	[4] = "TabListPopupButton",
	[8] = "NoCloseWithMiddleMouseButton",
	[16] = "NoTabListScrollingButtons",
	[32] = "NoTooltip",
	[64] = "FittingPolicyResizeDown",
	[128] = "FittingPolicyScroll",
	[192] = "FittingPolicyMask",
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
	BordersOuterH = 384,
	BordersInnerV = 512,
	BordersInnerV = 640,
	BordersOuterV = 1024,
	BordersOuter = 1280,
	BordersInnerV = 1536,
	BordersOuter = 1920,
	NoBordersInBody = 2048,
	NoBordersInBodyUntilResize = 4096,
	SizingFixedFit = 8192,
	SizingFixedSame = 16384,
	SizingFixedSame = 24576,
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
	[384] = "BordersOuterH",
	[512] = "BordersInnerV",
	[640] = "BordersInnerV",
	[1024] = "BordersOuterV",
	[1280] = "BordersOuter",
	[1536] = "BordersInnerV",
	[1920] = "BordersOuter",
	[2048] = "NoBordersInBody",
	[4096] = "NoBordersInBodyUntilResize",
	[8192] = "SizingFixedFit",
	[16384] = "SizingFixedSame",
	[24576] = "SizingFixedSame",
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
	NoTreePushOnOpen = 26,
	DefaultOpen = 32,
	OpenOnDoubleClick = 64,
	OpenOnArrow = 128,
	Leaf = 256,
	Bullet = 512,
	FramePadding = 1024,
	SpanAvailWidth = 2048,
	SpanFullWidth = 4096,
	SpanAllColumns = 8192,
	NavLeftJumpsBackHere = 16384,
	[1] = "Selected",
	[2] = "Framed",
	[4] = "AllowOverlap",
	[8] = "NoTreePushOnOpen",
	[16] = "NoAutoOpenOnLog",
	[26] = "NoTreePushOnOpen",
	[32] = "DefaultOpen",
	[64] = "OpenOnDoubleClick",
	[128] = "OpenOnArrow",
	[256] = "Leaf",
	[512] = "Bullet",
	[1024] = "FramePadding",
	[2048] = "SpanAvailWidth",
	[4096] = "SpanFullWidth",
	[8192] = "SpanAllColumns",
	[16384] = "NavLeftJumpsBackHere",
}

--- @enum GuiWindowFlags
Ext_Enums.GuiWindowFlags = {
	NoTitleBar = 1,
	NoResize = 2,
	NoMove = 4,
	NoScrollbar = 8,
	NoScrollWithMouse = 16,
	NoCollapse = 32,
	NoScrollbar = 43,
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
	NoNav = 197120,
	UnsavedDocument = 262144,
	[1] = "NoTitleBar",
	[2] = "NoResize",
	[4] = "NoMove",
	[8] = "NoScrollbar",
	[16] = "NoScrollWithMouse",
	[32] = "NoCollapse",
	[43] = "NoScrollbar",
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
	[197120] = "NoNav",
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
	Unknown = 9,
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
	[9] = "Unknown",
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

--- @enum InputType
Ext_Enums.InputType = {
	Unknown = 0,
	Unknown = 1,
	Unknown = 2,
	ValueChange = 4,
	Hold = 8,
	Repeat = 16,
	Unknown = 32,
	[0] = "Unknown",
	[1] = "Unknown",
	[2] = "Unknown",
	[4] = "ValueChange",
	[8] = "Hold",
	[16] = "Repeat",
	[32] = "Unknown",
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
	ManipulationModes_TranslateX = 1,
	ManipulationModes_TranslateY = 2,
	ManipulationModes_Rotate = 4,
	ManipulationModes_Scale = 8,
	[1] = "ManipulationModes_TranslateX",
	[2] = "ManipulationModes_TranslateY",
	[4] = "ManipulationModes_Rotate",
	[8] = "ManipulationModes_Scale",
}

--- @enum NsModifierKeys
Ext_Enums.NsModifierKeys = {
	ModifierKeys_Alt = 1,
	ModifierKeys_Control = 2,
	ModifierKeys_Shift = 4,
	ModifierKeys_Windows = 8,
	[1] = "ModifierKeys_Alt",
	[2] = "ModifierKeys_Control",
	[4] = "ModifierKeys_Shift",
	[8] = "ModifierKeys_Windows",
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

--- @enum ProjectileTypeIds
Ext_Enums.ProjectileTypeIds = {
	Physical = 1,
	Magical = 2,
	[1] = "Physical",
	[2] = "Magical",
}

--- @enum RecipeCategory
Ext_Enums.RecipeCategory = {
	Common = 0,
	Weapons = 1,
	Potions = 2,
	Grenades = 3,
	Arrows = 4,
	Armour = 5,
	Food = 6,
	Objects = 7,
	Runes = 8,
	Grimoire = 9,
	[0] = "Common",
	[1] = "Weapons",
	[2] = "Potions",
	[3] = "Grenades",
	[4] = "Arrows",
	[5] = "Armour",
	[6] = "Food",
	[7] = "Objects",
	[8] = "Runes",
	[9] = "Grimoire",
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

--- @enum RestErrorFlags
Ext_Enums.RestErrorFlags = {
	GlobalDisabled = 1,
	Script = 2,
	Dialog = 4,
	Combat = 8,
	FTB = 16,
	NotEnoughResources = 32,
	DownedOrDead = 64,
	[1] = "GlobalDisabled",
	[2] = "Script",
	[4] = "Dialog",
	[8] = "Combat",
	[16] = "FTB",
	[32] = "NotEnoughResources",
	[64] = "DownedOrDead",
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
	ForceNonzeroSpeed = 131072,
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
	[131072] = "ForceNonzeroSpeed",
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
	Unknown0x10 = 16,
	IsPet = 32,
	Unknown0x40 = 64,
	CanGossip = 128,
	[1] = "Global",
	[2] = "TreasureGeneratedForTrader",
	[4] = "Trader",
	[8] = "IsResurrected",
	[16] = "Unknown0x10",
	[32] = "IsPet",
	[64] = "Unknown0x40",
	[128] = "CanGossip",
}

--- @enum ServerCharacterFlags3
Ext_Enums.ServerCharacterFlags3 = {
	NeedsPlacementSnapping = 1,
	CrimeWarningsEnabled = 2,
	[1] = "NeedsPlacementSnapping",
	[2] = "CrimeWarningsEnabled",
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
	Invulnerable = 2147483648,
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
	[2147483648] = "Invulnerable",
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

--- @enum ShroudType
Ext_Enums.ShroudType = {
	Shroud = 0,
	Sight = 1,
	Sneak = 2,
	RegionMask = 3,
	[0] = "Shroud",
	[1] = "Sight",
	[2] = "Sneak",
	[3] = "RegionMask",
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

--- @enum SourceAdvantageType
Ext_Enums.SourceAdvantageType = {
	SourceAdvantageOnAttack = 1,
	SourceAllyAdvantageOnAttack = 2,
	[1] = "SourceAdvantageOnAttack",
	[2] = "SourceAllyAdvantageOnAttack",
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

--- @enum SpellChildSelectionType
Ext_Enums.SpellChildSelectionType = {
	Singular = 0,
	AddChildren = 1,
	MostPowerful = 2,
	[0] = "Singular",
	[1] = "AddChildren",
	[2] = "MostPowerful",
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
	Progression0 = 0,
	Progression1 = 1,
	Progression2 = 2,
	Boost = 3,
	Shapeshift = 4,
	SpellSet2 = 5,
	SpellSet = 6,
	WeaponAttack = 7,
	UnarmedAttack = 8,
	Osiris = 9,
	Anubis = 10,
	Behavior = 11,
	PartyPreset = 12,
	EquippedItem = 13,
	GameActionCreateSurface = 14,
	AiTest = 15,
	CreateExplosion = 16,
	Spell = 17,
	ActiveDefense = 18,
	Learned = 19,
	Progression = 20,
	Unknown15 = 21,
	Unknown16 = 22,
	Unknown17 = 23,
	Unknown18 = 24,
	Unknown19 = 25,
	Unknown1A = 26,
	Sentinel = 27,
	[0] = "Progression0",
	[1] = "Progression1",
	[2] = "Progression2",
	[3] = "Boost",
	[4] = "Shapeshift",
	[5] = "SpellSet2",
	[6] = "SpellSet",
	[7] = "WeaponAttack",
	[8] = "UnarmedAttack",
	[9] = "Osiris",
	[10] = "Anubis",
	[11] = "Behavior",
	[12] = "PartyPreset",
	[13] = "EquippedItem",
	[14] = "GameActionCreateSurface",
	[15] = "AiTest",
	[16] = "CreateExplosion",
	[17] = "Spell",
	[18] = "ActiveDefense",
	[19] = "Learned",
	[20] = "Progression",
	[21] = "Unknown15",
	[22] = "Unknown16",
	[23] = "Unknown17",
	[24] = "Unknown18",
	[25] = "Unknown19",
	[26] = "Unknown1A",
	[27] = "Sentinel",
}

--- @enum SpellType
Ext_Enums.SpellType = {
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
	Ability = 0,
	Skill = 1,
	StatusGroup = 2,
	STDString = 3,
	[0] = "Ability",
	[1] = "Skill",
	[2] = "StatusGroup",
	[3] = "STDString",
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

--- @enum SurfaceTransformActionType
Ext_Enums.SurfaceTransformActionType = {
	None = 0,
	Ignite = 1,
	Douse = 2,
	Electrify = 3,
	Deelectrify = 4,
	Freeze = 5,
	Melt = 6,
	Vaporize = 7,
	Condense = 8,
	[0] = "None",
	[1] = "Ignite",
	[2] = "Douse",
	[3] = "Electrify",
	[4] = "Deelectrify",
	[5] = "Freeze",
	[6] = "Melt",
	[7] = "Vaporize",
	[8] = "Condense",
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

--- @class Ext
--- @field Audio Ext_ClientAudio
--- @field ClientAudio Ext_ClientAudio
--- @field IMGUI Ext_ClientIMGUI
--- @field ClientIMGUI Ext_ClientIMGUI
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
--- @field Loca Ext_Loca
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
--- @type SubscribableEvent<LuaEmptyEvent>  
Ext.Events.GameStateChanged = {}
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
Ext.Events.ResetCompleted = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<LuaEmptyEvent>  
Ext.Events.SessionLoaded = {}
--- 🔨🔧**Server/Client**🔧🔨  
--- @type SubscribableEvent<LuaEmptyEvent>  
Ext.Events.SessionLoading = {}
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
}

--#endregion

