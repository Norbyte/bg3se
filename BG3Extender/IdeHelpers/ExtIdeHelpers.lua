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

--- @alias OsirisValue number|string

--- Using a DB like a function will allow inserting new values into the database (ex. `Osi.DB_IsPlayer("02a77f1f-872b-49ca-91ab-32098c443beb")`  
--- @overload fun(...:OsirisValue|nil)
--- @class OsiDatabase
local OsiDatabase = {}
--- Databases can be read using the Get method. The method checks its parameters against the database and only returns rows that match the query.  
--- The number of parameters passed to Get must be equivalent to the number of columns in the target database.  
--- Each parameter defines an (optional) filter on the corresponding column.  
--- If the parameter is nil, the column is not filtered (equivalent to passing _ in Osiris). If the parameter is not nil, only rows with matching values will be returned.
--- @vararg OsirisValue|nil
--- @return table<integer,table<integer,OsirisValue>>
function OsiDatabase:Get(...) end
--- The Delete method can be used to delete rows from databases.  
--- The number of parameters passed to Delete must be equivalent to the number of columns in the target database.  
--- Each parameter defines an (optional) filter on the corresponding column.  
--- If the parameter is nil, the column is not filtered (equivalent to passing _ in Osiris). If the parameter is not nil, only rows with matching values will be deleted. 
--- @vararg OsirisValue|nil
function OsiDatabase:Delete(...) end

--- @alias OsiFunction fun(...:OsirisValue):OsirisValue|nil
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

--- @alias OsirisEventType string|"before"|"after"|"beforeDelete"|"afterDelete"
--- @alias i16vec2 int16[]
--- @alias YesNo "Yes"|"No"
--- @alias ComponentHandle userdata
--- @alias EntityHandle userdata
--- @alias EntityRef number
--- @alias FixedString string
--- @alias Guid string
--- @alias NetId integer
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


--- @alias AIFlags string|"CanNotUse"|"IgnoreSelf"|"IgnoreDebuff"|"IgnoreBuff"|"StatusIsSecondary"|"IgnoreControl"|"CanNotTargetFrozen"
--- @alias AbilityId string|"None"|"Sentinel"|"Charisma"|"Wisdom"|"Intelligence"|"Constitution"|"Dexterity"|"Strength"
--- @alias AdvantageBoostType string|"Advantage"|"Disadvantage"
--- @alias AdvantageTypeId string|"AttackTarget"|"DeathSavingThrow"|"SavingThrow"|"AllSavingThrows"|"AttackRoll"|"AllAbilities"|"AllSkills"|"SourceDialogue"|"Concentration"|"Ability"|"Skill"
--- @alias AnimationEventPropertyType string|"FloatData"|"EFoleySoundObject"|"EVocalSoundObject"|"U8Data"|"Unknown15"|"SoundObject"|"BoneEffect"|"Foot"|"BoneAttachBone"|"WeaponEffect"|"Unknown7"|"Unknown8"|"Prop9"|"Prop10"
--- @alias AnimationSetAnimationFlags string|"NoFallback"|"AlwaysIgnore"
--- @alias ArmorType string|"None"|"Sentinel"|"Leather"|"Cloth"|"ChainShirt"|"HalfPlate"|"RingMail"|"Plate"|"Splint"|"Padded"|"BreastPlate"|"ScaleMail"|"ChainMail"|"StuddedLeather"|"Hide"
--- @alias AttackRoll string|"None"|"Charisma"|"UnarmedAttackAbility"|"WeaponAttackAbility"|"Wisdom"|"Intelligence"|"Constitution"|"Dexterity"|"Strength"|"SpellCastingAbility"
--- @alias AttributeFlags string|"InventoryBound"|"PickpocketableWhenEquipped"|"LoseDurabilityOnCharacterHit"|"InvulnerableAndInteractive"|"EnableObscurityEvents"|"ObscurityWithoutSneaking"|"SlippingImmunity"|"Floating"|"Torch"|"Unbreakable"|"Unrepairable"|"Unstorable"|"Arrow"|"Grounded"|"IgnoreClouds"|"ForceMainhandAlternativeEquipBones"|"LootableWhenEquipped"|"UseMusicalInstrumentForCasting"|"ThrownImmunity"|"InvisibilityImmunity"|"Backstab"|"BackstabImmunity"|"FloatingWhileMoving"
--- @alias BoostCauseType string|"Undefined"|"Progression"|"Item"|"Stats"|"Character"|"Osiris"|"Unknown5"|"Status"|"Passive"
--- @alias BoostType string|"ProficiencyBonusOverride"|"ActionResourceMultiplier"|"GameplayLight"|"Savant"|"Resistance"|"ActionResourceConsumeMultiplier"|"PhysicalForceRangeBonus"|"AttackSpellOverride"|"Reroll"|"AbilityOverrideMinimum"|"MonkWeaponDamageDiceOverride"|"AbilityFailedSavingThrow"|"FactionOverride"|"RedirectDamage"|"MonkWeaponAttackOverride"|"AreaDamageEvade"|"JumpMaxDistanceMultiplier"|"ProjectileDeflect"|"ObjectSize"|"ExpertiseBonus"|"RollBonus"|"ProficiencyBonus"|"AiArchetypeOverride"|"EntityThrowDamage"|"IntrinsicSummonerProficiency"|"Advantage"|"Proficiency"|"DualWielding"|"IntrinsicSourceProficiency"|"DownedStatus"|"ObjectSizeOverride"|"AdvanceSpells"|"IgnorePointBlankDisadvantage"|"AddProficiencyToAC"|"Tag"|"NullifyAbilityScore"|"DodgeAttackRoll"|"IgnoreLowGroundPenalty"|"Weight"|"IgnoreLeaveAttackRange"|"ActiveCharacterLight"|"NonLethal"|"ActionResourceBlock"|"DialogueBlock"|"WeaponProperty"|"IgnoreEnterAttackRange"|"IgnoreDamageThreshold"|"WeaponEnchantment"|"ACOverrideFormula"|"Detach"|"NoAOEDamageOnLand"|"AC"|"DarkvisionRangeMin"|"WeaponDamage"|"FallDamageMultiplier"|"HorizontalFOVOverride"|"DarkvisionRange"|"VoicebarkBlock"|"HiddenDuringCinematic"|"MovementSpeedLimit"|"UseBoosts"|"GuaranteedChanceRollOutcome"|"MinimumRollResult"|"UnlockSpellVariant"|"DamageReduction"|"MaximumRollResult"|"EnableBasicItemInteractions"|"UnlockSpell"|"DetectDisturbancesBlock"|"MaximizeHealing"|"DamageBonus"|"DarkvisionRangeOverride"|"Lootable"|"CriticalHitExtraDice"|"UnlockInterrupt"|"TwoWeaponFighting"|"CriticalDamageOnHit"|"WeaponDamageTypeOverride"|"Lock"|"ConcentrationIgnoreDamage"|"CriticalHit"|"WeaponDamageResistance"|"TemporaryHP"|"CharacterWeaponDamage"|"LeaveTriggers"|"WeaponDamageDieOverride"|"ConsumeItemBlock"|"CharacterUnarmedDamage"|"JumpMaxDistanceBonus"|"Ability"|"StatusImmunity"|"WeaponAttackTypeOverride"|"CarryCapacityMultiplier"|"ItemReturnToOwner"|"CanWalkThrough"|"CannotHarmCauseEntity"|"WeightCategory"|"Invulnerable"|"WeaponAttackRollAbilityOverride"|"SpellSaveDC"|"CanShootThrough"|"BlockSomaticComponent"|"Invisibility"|"SpellResistance"|"WeaponAttackRollBonus"|"CanSeeThrough"|"Initiative"|"CannotBeDisarmed"|"BlockAbilityModifierFromAC"|"UnarmedMagicalProperty"|"SoundsBlocked"|"BlockVerbalComponent"|"IncreaseMaxHP"|"Skill"|"BlockAbilityModifierDamageBonus"|"SourceAllyAdvantageOnAttack"|"BlockTravel"|"SightRangeOverride"|"ArmorAbilityModifierCapOverride"|"IgnoreSurfaceCover"|"AddProficiencyToDamage"|"SourceAdvantageOnAttack"|"BlockSpellCast"|"SightRangeMaximum"|"IgnoreResistance"|"BlockRegainHP"|"IgnoreFallDamage"|"SightRangeMinimum"|"ReduceCriticalAttackThreshold"|"ActionResourceReplenishTypeOverride"|"HalveWeaponDamage"|"BlockGatherAtCamp"|"SightRangeAdditive"|"ActionResourcePreventReduction"|"ActionResource"|"ActionResourceOverride"|"Attribute"|"ScaleMultiplier"|"GameplayObscurity"
--- @alias CauseType string|"None"|"Attack"|"InventoryItem"|"SurfaceMove"|"SurfaceCreate"|"SurfaceStatus"|"StatusEnter"|"StatusTick"|"Offhand"|"GM"|"WorldItemThrow"|"AURA"
--- @alias ClientGameState string|"UnloadLevel"|"UnloadModule"|"UnloadSession"|"Paused"|"PrepareRunning"|"Running"|"Disconnect"|"Join"|"Unknown"|"Save"|"StartLoading"|"StopLoading"|"StartServer"|"Movie"|"Installation"|"ModReceiving"|"Lobby"|"BuildStory"|"GeneratePsoCache"|"LoadPsoCache"|"AnalyticsSessionEnd"|"Init"|"InitMenu"|"InitNetwork"|"InitConnection"|"Idle"|"LoadMenu"|"Menu"|"LoadModule"|"Exit"|"SwapLevel"|"LoadLevel"|"LoadSession"
--- @alias CombatParticipantFlags string|"StayInAiHints"|"IsBoss"|"IsInspector"|"CanFight"|"CanJoinCombat"
--- @alias ConditionRollType string|"AttackRoll"|"DifficultyRoll"|"SavingThrowRoll"|"SkillCheckRoll"|"AbilityCheckRoll"
--- @alias CraftingStationType string|"None"|"Anvil"|"Oven"|"Wetstone"|"Well"|"BoilingPot"|"Beehive"|"SpinningWheel"|"Cauldron"|"Misc1"|"Misc2"|"Misc3"|"Misc4"
--- @alias CriticalHitBoostFlags string|"FailureNever"|"AttackTarget"|"Failure"|"AttackRoll"|"Success"|"SuccessNever"
--- @alias DamageFlags string|"HitpointsDamaged"|"SavingThrow"|"AttackAdvantage"|"Hit"|"AttackDisadvantage"|"Critical"|"Projectile"|"Surface"|"Magical"|"StatusEnter"|"Invulnerable"|"Status"|"Backstab"|"Dodge"|"Miss"|"Invisible"
--- @alias DamageType string|"None"|"Acid"|"Sentinel"|"Lightning"|"Fire"|"Poison"|"Bludgeoning"|"Slashing"|"Piercing"|"Thunder"|"Force"|"Cold"|"Radiant"|"Psychic"|"Necrotic"
--- @alias DealDamageWeaponDamageType string|"MainWeaponDamageType"|"None"|"OffhandWeaponDamageType"|"MainMeleeWeaponDamageType"|"OffhandMeleeWeaponDamageType"|"MainRangedWeaponDamageType"|"OffhandRangedWeaponDamageType"|"SourceWeaponDamageType"|"ThrownWeaponDamageType"
--- @alias DealDamageWeaponType string|"None"|"UnarmedDamage"|"SourceWeapon"|"MainWeapon"|"OffhandWeapon"|"MainMeleeWeapon"|"OffhandMeleeWeapon"|"MainRangedWeapon"|"OffhandRangedWeapon"|"ThrownWeapon"|"ImprovisedWeapon"
--- @alias DiceSizeId string|"Default"|"D4"|"D6"|"D8"|"D10"|"D12"|"D20"|"D100"
--- @alias ESurfaceFlag string|"HasItem"|"ShockwaveCloud"|"GroundSurfaceBlock"|"CloudSurfaceBlock"|"Occupied"|"Lava"|"BloodCloud"|"SurfaceExclude"|"Oil"|"Web"|"Sulfurium"|"Deepwater"|"FireCloud"|"SmokeCloud"|"Fire"|"Poison"|"FrostCloud"|"Deathfog"|"Source"|"Blessed"|"Frozen"|"Cursed"|"Purified"|"CloudBlessed"|"CloudCursed"|"CloudPurified"|"Electrified"|"CloudElectrified"|"ElectrifiedDecay"|"SomeDecay"|"Water"|"Irreplaceable"|"IrreplaceableCloud"|"HasInteractableObject"|"Blood"|"MovementBlock"|"WaterCloud"|"ProjectileBlock"|"PoisonCloud"|"HasCharacter"|"ExplosionCloud"
--- @alias EffectPropertyType string|"Bool"|"FixedString"|"Int32"|"Int32Range"|"STDString"|"FunctionType"|"Base"|"ColoredFrames"|"Float"|"FloatRange"|"Frames"|"Vector3"
--- @alias EquipmentStatsType string|"Armor"|"Shield"|"Weapon"
--- @alias ExecuteWeaponFunctorsType string|"BothHands"|"Undefined"|"OffHand"|"MainHand"
--- @alias ExtComponentType string|"ArmorSetState"|"HasOpened"|"ItemTemplateDestroyed"|"PartyMember"|"ServerImmediateJoin"|"Tadpoled"|"Savegame"|"Key"|"FleeCapability"|"GameplayLight"|"CanDeflectProjectiles"|"AttitudesToPlayers"|"ClientProjectile"|"WeaponAttackTypeOverrideBoost"|"DamageReductionBoost"|"ConcentrationIgnoreDamageBoost"|"SpellBook"|"Faction"|"IsGold"|"InventoryMemberTransform"|"PartyFollower"|"SpellContainer"|"HalfIllithid"|"Stealth"|"CanDoRest"|"CharacterCreationAppearance"|"EquipmentVisual"|"ArmorClassBoost"|"WeaponDamageDieOverrideBoost"|"ObjectSizeBoost"|"IgnoreLowGroundPenaltyBoost"|"IsCharacter"|"StatusImmunities"|"NewInInventory"|"CurrentlyFollowingParty"|"BlockFollow"|"PlayerPrepareSpell"|"FullIllithid"|"IsInTurnBasedMode"|"Repose"|"ServerExperienceGaveOut"|"AbilityBoost"|"CarryCapacityMultiplierBoost"|"ExpertiseBonusBoost"|"IgnoreSurfaceCoverBoost"|"Max"|"Armor"|"Bound"|"Stats"|"Use"|"ActionType"|"ItemCanMove"|"Recruiter"|"ServerCombatGroupMapping"|"CCPrepareSpell"|"CCDefinitionCommon"|"ItemBoosts"|"CanEnterChasm"|"ServerReplicationDependency"|"RollBonusBoost"|"WeaponAttackRollAbilityOverrideBoost"|"MaximizeHealingBoost"|"Wielding"|"Origin"|"ObjectSize"|"ClimbOn"|"CampChest"|"SpellModificationContainer"|"AddedSpells"|"InterruptData"|"CCRespecDefinition"|"AnimationBlueprint"|"DialogState"|"ServerActivationGroupContainer"|"AdvantageBoost"|"SightRangeAdditiveBoost"|"DamageBonusBoost"|"BaseHp"|"CustomStats"|"InteractionDisabled"|"CampEndTheDayState"|"Identity"|"SpellBookCooldowns"|"SpellCastAnimationInfo"|"StatusID"|"CCSessionCommon"|"CanModifyHealth"|"EncumbranceState"|"ServerDisplayNameList"|"CriticalHitBoost"|"SightRangeMinimumBoost"|"AdvanceSpellsBoost"|"BoostCondition"|"IsStoryItem"|"CampSettings"|"SpellAiConditions"|"SpellCastCanBeTargeted"|"CCState"|"Proficiency"|"IsGlobal"|"AvailableLevel"|"Concentration"|"DualWielding"|"Active"|"EncumbranceStats"|"ServerToggledPassives"|"ResistanceBoost"|"SightRangeMaximumBoost"|"SpellResistanceBoost"|"BoostsContainer"|"Player"|"Ladder"|"CampSupply"|"SpellCastCache"|"SpellCastInterruptResults"|"CCLevelUpDefinition"|"CanBeLooted"|"Unsheath"|"ServerDelayDeathCause"|"UnlockSpellBoost"|"SightRangeOverrideBoost"|"SpellSaveDCBoost"|"Experience"|"Tag"|"SpellBookPrepares"|"Transform"|"WalkOn"|"UseAction"|"CampTotalSupplies"|"Death"|"SpellCastIsCasting"|"ServerSpellCastHitDelay"|"CCFullRespec"|"LevelUp"|"CanBeDisarmed"|"Trader"|"OriginalTemplate"|"UseSocket"|"ServerReplicationDependencyOwner"|"SourceAdvantageBoost"|"MovementSpeedLimitBoost"|"RedirectDamageBoost"|"BoostInfo"|"OwnedAsLoot"|"CampPresence"|"LearnedSpells"|"SpellCastMovement"|"ServerSpellCastResponsible"|"CCRespec"|"ActiveCharacterLight"|"CanDoActions"|"UserAvatar"|"ActionResourceValueBoost"|"ProficiencyBoost"|"UnlockSpellVariantBoost"|"CanSeeThroughBoost"|"Health"|"CanInteract"|"OwneeCurrent"|"IsInCombat"|"SpellCastRolls"|"ServerSpellClientInitiated"|"CCCreation"|"CanMove"|"UserReservedFor"|"AbilityFailedSavingThrowBoost"|"IncreaseMaxHPBoost"|"DetectCrimesBlockBoost"|"CanShootThroughBoost"|"CanSpeak"|"InventoryMember"|"CombatParticipant"|"SpellCastState"|"ServerSpellHitRegister"|"CCLevelUp"|"Voice"|"CanSense"|"ServerRecruitedBy"|"WeaponDamageResistanceBoost"|"StatusImmunityBoost"|"BlockAbilityModifierFromACBoost"|"CanWalkThroughBoost"|"EocLevel"|"InventoryData"|"CombatState"|"SpellSyncTargeting"|"ServerSpellInterruptRequests"|"ProficiencyGroup"|"Darkness"|"Race"|"ServerGameTimer"|"ProficiencyBonusOverrideBoost"|"TemporaryHPBoost"|"ReduceCriticalAttackThresholdBoost"|"LockBoost"|"Classes"|"InventoryOwner"|"TurnBased"|"ServerSpellExternals"|"ServerSpellInterruptResults"|"Floating"|"GameObjectVisual"|"DisplayName"|"ShootThroughType"|"ServerIsUnsummoning"|"JumpMaxDistanceMultiplierBoost"|"WeightBoost"|"PhysicalForceRangeBonusBoost"|"DodgeAttackRollBoost"|"OffStage"|"InventoryIsOwned"|"TurnOrder"|"ServerSpellCastState"|"InterruptConditionallyDisabled"|"Disarmable"|"Icon"|"AppearanceOverride"|"ServerAnubisTag"|"HalveWeaponDamageBoost"|"UseBoosts"|"WeightCategoryBoost"|"ObjectSizeOverrideBoost"|"UnlockInterruptBoost"|"Level"|"PickingState"|"InventoryContainer"|"IsCombatPaused"|"InterruptActionState"|"InterruptZoneParticipant"|"SummonContainer"|"Visual"|"AnimationSet"|"GravityDisabled"|"Steering"|"ServerDialogTag"|"ProficiencyBonusBoost"|"FactionOverrideBoost"|"AiArchetypeOverrideBoost"|"Uuid"|"SimpleCharacter"|"AttributeFlags"|"RulesetModifiers"|"InventoryStack"|"ThreatRange"|"InterruptContainer"|"ServerInterruptAddRemoveRequests"|"ClientControl"|"CanBeWielded"|"InteractionFilter"|"Movement"|"ServerIconList"|"ActionResourceBlockBoost"|"BlockRegainHPBoost"|"EntityThrowDamageBoost"|"UuidToHandleMapping"|"WeaponSet"|"InventoryStackMember"|"DeathState"|"InterruptDecision"|"ServerInterruptActionRequests"|"CustomIcon"|"DisabledEquipment"|"Avatar"|"ServerRaceTag"|"CannotHarmCauseEntityBoost"|"InitiativeBoost"|"WeaponDamageTypeOverrideBoost"|"DifficultyCheck"|"InventoryWeight"|"DeadByDefault"|"InterruptPreferences"|"ServerInterruptZoneRequests"|"God"|"LootingState"|"Lock"|"HasExclamationDialog"|"ServerTemplateTag"|"ActionResourceMultiplierBoost"|"DarkvisionRangeBoost"|"WeaponAttackRollBonusBoost"|"Relation"|"BodyType"|"BaseStats"|"Ruleset"|"Wielded"|"HitTarget"|"InterruptPrepared"|"ServerInterruptInitialParticipants"|"Background"|"Loot"|"Equipable"|"ServerBoostTag"|"DarkvisionRangeMinBoost"|"AddTagBoost"|"MonkWeaponDamageDiceOverrideBoost"|"Expertise"|"CanBeInInventory"|"CampQuality"|"HitAttacker"|"InterruptZone"|"ServerInterruptTurnOrderInZone"|"SummonLifetime"|"ProgressionContainer"|"ServerSafePosition"|"DarkvisionRangeOverrideBoost"|"SkillBoost"|"HorizontalFOVOverrideBoost"|"HealBlock"|"CannotBePickpocketed"|"HitWeapon"|"InterruptZoneSource"|"CCCompanionDefinition"|"HotbarContainer"|"ProgressionMeta"|"CanWalkThrough"|"ServerAnubisExecutor"|"IgnoreDamageThresholdMinBoost"|"WeaponDamageBoost"|"CharacterUnarmedDamageBoost"|"ShapeshiftState"|"CannotBeTakenOut"|"HitThrownObject"|"SightData"|"CCFullRespecDefinition"|"HotbarDecks"|"CanTravel"|"CanShootThrough"|"ServerLeader"|"NullifyAbilityBoost"|"WeaponEnchantmentBoost"|"ActionResourcePreventReductionBoost"|"PassiveContainer"|"DropOnDeathBlocked"|"HitProxy"|"SightEntityViewshed"|"CCCharacterDefinition"|"Invisibility"|"OriginTag"|"ObjectInteraction"|"CanSeeThrough"|"ServerBreadcrumb"|"RerollBoost"|"GuaranteedChanceRollOutcomeBoost"|"AttackSpellOverrideBoost"|"PassiveUsageCount"|"NewItemsInside"|"HitProxyOwner"|"StatusContainer"|"CharacterCreationStats"|"OriginPassives"|"StaticPhysics"|"ServerPickpocket"|"DownedStatusBoost"|"MinimumRollResultBoost"|"IgnorePointBlankDisadvantageBoost"|"Data"|"ItemDestroyed"|"NonTradable"|"HitReaction"|"StatusCause"|"ClassTag"|"GlobalShortRestDisabled"|"Pathing"|"Physics"|"ServerCharacter"|"AttributeBoost"|"CharacterWeaponDamageBoost"|"CriticalHitExtraDiceBoost"|"ActionResources"|"Speaker"|"Passive"|"ItemDye"|"PartyView"|"IdentityState"|"StatusIncapacitated"|"BackgroundTag"|"GlobalLongRestDisabled"|"ActiveSkeletonSlots"|"ServerItem"|"GameplayLightBoost"|"ProjectileDeflectBoost"|"GameplayObscurityBoost"|"SurfacePathInfluences"|"ItemPortal"|"PartyComposition"|"OriginalIdentity"|"StatusLifetime"|"ShortRest"|"IsSummon"|"OriginAppearanceTag"|"StoryShortRestDisabled"|"Net"|"ServerProjectile"|"DualWieldingBoost"|"AbilityOverrideMinimumBoost"|"ActionResourceReplenishTypeOverrideBoost"|"MaximumRollResultBoost"|"Weapon"|"ActionResourceConsumeMultiplierBoost"|"IsDoor"|"PartyPortals"|"ServerCanStartCombat"|"StatusVisualDisabled"|"VoiceTag"|"CanTriggerRandomCasts"|"FTBParticipant"|"ServerOsirisTag"|"SavantBoost"|"ACOverrideFormulaBoost"|"JumpMaxDistanceBonusBoost"|"MaterialParameterOverride"|"MapMarkerStyle"|"ExamineDisabled"|"IsItem"|"PartyRecipes"|"ServerEnterRequest"|"Sight"|"TadpoleTreeState"|"GodTag"|"CharacterCreationTemplateOverride"|"FTBRespect"|"ClientCharacter"|"FallDamageMultiplierBoost"|"WeaponPropertyBoost"|"ArmorAbilityModifierCapOverrideBoost"|"Hearing"|"Value"|"ItemHasMoved"|"ShouldDestroyOnSpellCast"|"PartyWaypoints"|"ServerFleeBlocked"|"TadpolePowers"|"BackgroundPassives"|"GravityDisabledUntilMoved"|"ApprovalRatings"|"ClientItem"|"ActiveCharacterLightBoost"|"ScaleMultiplierBoost"|"IgnoreResistanceBoost"
--- @alias ExtQueryType string|"Max"|"UuidToHandleMapping"
--- @alias ExtResourceManagerType string|"Faction"|"AbilityDistributionPreset"|"Progression"|"Max"|"Origin"|"CharacterCreationPreset"|"CharacterCreationSkinColor"|"Tag"|"CharacterCreationSharedVisual"|"EquipmentType"|"CharacterCreationPassiveAppearance"|"Race"|"CharacterCreationMaterialOverride"|"ClassDescription"|"AbilityList"|"CharacterCreationHairColor"|"CompanionPreset"|"CharacterCreationEyeColor"|"VFX"|"CharacterCreationEquipmentIcons"|"ColorDefinition"|"God"|"Background"|"CharacterCreationIconSettings"|"SkillList"|"SpellList"|"CharacterCreationAppearanceVisual"|"Flag"|"CharacterCreationAppearanceMaterial"|"ActionResourceGroup"|"CharacterCreationAccessorySet"|"TutorialEntries"|"TutorialModalEntries"|"PassiveList"|"FeatDescription"|"LevelMap"|"ProgressionDescription"|"Gossip"|"ActionResource"|"Feat"
--- @alias FleeErrorFlags string|"PartyFollower"|"EnemyTooClose"|"CantMove"|"Downed"|"Summon"|"Incapacitated"|"Condition"|"Region"
--- @alias ForceFunctorAggression string|"Undefined"|"Aggressive"|"Friendly"|"Neutral"
--- @alias ForceFunctorOrigin string|"Undefined"|"OriginToEntity"|"OriginToTarget"|"TargetToEntity"
--- @alias FunctorExecParamsType string|"Type1"|"Type2"|"Type3"|"Type4"|"Type5"|"Type6"|"Type7"|"Type8"
--- @alias GameActionType string|"RainAction"|"StormAction"|"WallAction"|"TornadoAction"|"PathAction"|"GameObjectMoveAction"|"StatusDomeAction"
--- @alias Gender string|"Male"|"Female"
--- @alias HandednessType string|"Any"|"Two"|"One"
--- @alias HealDirection string|"Incoming"|"Outgoing"
--- @alias HealEffect string|"None"|"Heal"|"Script"|"ResistDeath"|"Surface"|"Behavior"|"Unknown4"|"Sitting"|"Lifesteal"|"NegativeDamage"|"Unknown9"|"HealSharing"|"Necromantic"|"HealSharingReflected"
--- @alias HitType string|"DoT"|"Ranged"|"Surface"|"Magic"|"Reflected"|"WeaponDamage"|"Melee"
--- @alias HitWith string|"None"|"Item"|"Unknown"|"Projectile"|"Surface"|"Magic"|"Redirection"|"FallDamage"|"CrushByFall"|"Unknown10"|"Unknown11"|"Trap"|"Weapon"
--- @alias IngredientTransformType string|"Boost"|"None"|"Transform"|"Poison"|"Consume"
--- @alias IngredientType string|"None"|"Property"|"Category"|"Object"
--- @alias InputType string|"Unknown"|"Press"|"Release"|"ValueChange"|"Hold"|"Repeat"|"AcceleratedRepeat"
--- @alias ItemDataRarity string|"Uncommon"|"Common"|"Unique"|"Sentinel"|"Epic"|"Divine"|"Legendary"|"Rare"
--- @alias ItemSlot string|"VanityBoots"|"VanityBody"|"Wings"|"Underwear"|"Helmet"|"Gloves"|"Breast"|"Cloak"|"OffHand"|"MeleeMainHand"|"MainHand"|"MeleeOffHand"|"RangedMainHand"|"RangedOffHand"|"Ring"|"MusicalInstrument"|"Boots"|"Amulet"|"Horns"|"Ring2"|"Overhead"
--- @alias LuaTypeId string|"Void"|"Array"|"Boolean"|"Integer"|"Set"|"Any"|"Map"|"Function"|"Tuple"|"Enumeration"|"Float"|"Nullable"|"Unknown"|"Variant"|"Module"|"Object"|"String"
--- @alias NetMessage string|"NETMSG_CLIENT_ACCEPT"|"NETMSG_CHARACTER_USE_MOVEMENT"|"NETMSG_TURNBASED_END_CANCEL_ENDTURN_REQUEST"|"NETMSG_ITEM_MOVE_TO_WORLD"|"NETMSG_SPELL_LEARN"|"NETMSG_COMBATLOGENTRIES"|"NETMSG_ACHIEVEMENT_PROGRESS_MESSAGE"|"NETMSG_UNPAUSE"|"NETMSG_REQUESTED_ROLL"|"NETMSG_STORY_FLAGS_LOAD"|"NETMSG_CLEAR_RECIPE_NEW_FLAG"|"NETMSG_PLAYER_JOINED"|"NETMSG_CHARACTER_MOVE_FAILED"|"NETMSG_TURNBASED_STARTTURN_CONFIRMATION"|"NETMSG_ITEM_STATUS_LIFETIME"|"NETMSG_SPELL_PREPARE"|"NETMSG_SHOW_ENTER_REGION_UI_MESSAGE"|"NETMSG_COMBINE_RESULT"|"NETMSG_READYCHECK"|"NETMSG_PASSIVE_ROLL_SEQUENCE"|"NETMSG_ACTIVE_ROLL_MODIFIERS"|"NETMSG_NEW_ITEMS_INSIDE"|"NETMSG_PLAYER_ACCEPT"|"NETMSG_CHARACTER_SELECTED_CLIMB_ON"|"NETMSG_UI_FORCETURNBASED_TURN_STARTED"|"NETMSG_ITEM_TRANSFORM"|"NETMSG_SPELL_CANCEL"|"NETMSG_CHARACTER_TELEPORT"|"NETMSG_DIALOG_STATE_MESSAGE"|"NETMSG_DIPLOMACY"|"NETMSG_CHARACTER_PATHING"|"NETMSG_ITEM_TOGGLE_IS_WARE"|"NETMSG_SCRIPT_EXTENDER"|"NETMSG_HOST_REFUSEPLAYER"|"NETMSG_PLAYER_LEFT"|"NETMSG_FOLLOWER_CANT_USE_ITEM"|"NETMSG_ITEM_CONFIRMATION"|"NETMSG_SHOW_ERROR"|"NETMSG_SCREEN_FADE_FINISHED"|"NETMSG_DIALOG_NODE_MESSAGE"|"NETMSG_DIALOG_ANSWER_HIGHLIGHT_MESSAGE"|"NETMSG_PING_REQUEST"|"NETMSG_CHARACTER_CREATION_ABORT"|"NETMSG_LONG_REST_DECISION"|"NETMSG_CLIENT_CONNECT"|"NETMSG_VOICEDATA"|"NETMSG_CHARACTER_UPDATE"|"NETMSG_ITEM_MOVED_INFORM"|"NETMSG_SAVEGAME"|"NETMSG_OPEN_CUSTOM_BOOK_UI_MESSAGE"|"NETMSG_DIALOG_ANSWER_MESSAGE"|"NETMSG_DIALOG_REQUEST_IN_DIALOGUE_ATTACK_MESSAGE"|"NETMSG_GIVE_REWARD"|"NETMSG_CHARACTER_CREATION_READY"|"NETMSG_REQUEST_RELEASE_CONCENTRATION"|"NETMSG_PLAYER_CONNECT"|"NETMSG_MIC_DISABLED"|"NETMSG_CHARACTER_CONTROL"|"NETMSG_INVENTORY_CREATE"|"NETMSG_EFFECT_CREATE"|"NETMSG_CLOSE_CUSTOM_BOOK_UI_MESSAGE"|"NETMSG_DIALOG_HISTORY_MESSAGE"|"NETMSG_DIALOG_TIMELINE_UNLOADED_MESSAGE"|"NETMSG_DLC_UPDATE"|"NETMSG_CHARACTER_CREATION_UPDATE"|"NETMSG_ROLL_STREAM_ROLL_MODE_TYPE"|"NETMSG_PLAYER_DISCONNECT"|"NETMSG_CHAT"|"NETMSG_CHARACTER_TRANSFORM"|"NETMSG_INVENTORY_CREATE_AND_OPEN"|"NETMSG_EFFECT_FORGET"|"NETMSG_OPEN_MESSAGE_BOX_MESSAGE"|"NETMSG_DIALOG_ACTORJOINS_MESSAGE"|"NETMSG_CUSTOM_STATS_DEFINITION_CREATE"|"NETMSG_LEVEL_UP_UPDATE"|"NETMSG_RESPEC_UPDATE"|"NETMSG_CHARACTER_DROP_ENTITY"|"NETMSG_PLAYER_CHANGENAME"|"NETMSG_PEER_ACTIVATE"|"NETMSG_CHARACTER_PICKPOCKET"|"NETMSG_INVENTORY_DESTROY"|"NETMSG_CACHETEMPLATE"|"NETMSG_CLOSED_MESSAGE_BOX_MESSAGE"|"NETMSG_DIALOG_ACTORLEAVES_MESSAGE"|"NETMSG_CUSTOM_STATS_DEFINITION_REMOVE"|"NETMSG_FULL_RESPEC_UPDATE"|"NETMSG_CAST_ALL"|"NETMSG_CHARACTER_CANCEL_DROP_ENTITY"|"NETMSG_PLAYER_NAMECHANGED"|"NETMSG_MODULE_LOAD"|"NETMSG_CHARACTER_OFFSTAGE"|"NETMSG_INVENTORY_VIEW_CREATE"|"NETMSG_OVERHEADTEXT"|"NETMSG_OPEN_WAYPOINT_UI_MESSAGE"|"NETMSG_DIALOG_REPLACESPEAKER_MESSAGE"|"NETMSG_CUSTOM_STATS_DEFINITION_UPDATE"|"NETMSG_CHARACTER_CREATION_LEVELUP"|"NETMSG_END_THE_DAY"|"NETMSG_CHARACTER_LOCKPICK"|"NETMSG_ECS_REPLICATION"|"NETMSG_MODULE_LOADED"|"NETMSG_CHARACTER_IN_DIALOG"|"NETMSG_INVENTORY_VIEW_DESTROY"|"NETMSG_COMBATLOG"|"NETMSG_CLOSE_UI_MESSAGE"|"NETMSG_DIALOG_INVALID_ANSWER"|"NETMSG_TIMELINE_WORLD_CINEMATIC_COMPLETED"|"NETMSG_CHARACTER_CREATION_RESPEC"|"NETMSG_SHORT_REST"|"NETMSG_CHARACTER_TRAPDISARM"|"NETMSG_SKIPMOVIE_RESULT"|"NETMSG_SESSION_LOAD"|"NETMSG_CHARACTER_LOOT"|"NETMSG_INVENTORY_VIEW_UPDATE_ITEMS"|"NETMSG_OPEN_CRAFT_UI_MESSAGE"|"NETMSG_JOURNAL_RESET"|"NETMSG_DIALOG_SUGGESTANSWER_MESSAGE"|"NETMSG_CLIENT_DIALOG_JOIN_PRIVACY_FAILED"|"NETMSG_UI_INTERACTION_STOPPED"|"NETMSG_CHARACTER_IMPROV_WEAPON"|"NETMSG_INVENTORY_ADD"|"NETMSG_PEER_DEACTIVATE"|"NETMSG_LEVEL_LOAD"|"NETMSG_CHARACTER_ITEM_USED"|"NETMSG_INVENTORY_VIEW_UPDATE_PARENTS"|"NETMSG_TELEPORT_WAYPOINT"|"NETMSG_QUEST_STEP"|"NETMSG_DIALOG_INSTANCEDIALOGCHANGED"|"NETMSG_FORCE_TURN_BASED_END_PLAYER_TURN_REQUEST"|"NETMSG_PASSIVE_TOGGLE"|"NETMSG_CHARACTER_STOWED_VISIBILITY"|"NETMSG_SELECT_DICE"|"NETMSG_SESSION_LOADED"|"NETMSG_LEVEL_CREATED"|"NETMSG_CHARACTER_UNSHEATHING"|"NETMSG_INVENTORY_VIEW_SORT"|"NETMSG_QUEST_CATEGORY_UPDATE"|"NETMSG_QUESTS_LOADED"|"NETMSG_LOBBY_DATAUPDATE"|"NETMSG_FORCE_TURN_BASED_TOGGLE_REQUEST"|"NETMSG_DUALWIELDING_TOGGLE"|"NETMSG_DECLUTTERED_ITEMS"|"NETMSG_COMBINE_ITEMS"|"NETMSG_SESSION_UNLOADED"|"NETMSG_LEVEL_LOADED"|"NETMSG_CHARACTER_END_REPOSE"|"NETMSG_INVENTORY_ITEM_UPDATE"|"NETMSG_QUEST_PROGRESS"|"NETMSG_TROPHY_UPDATE"|"NETMSG_LOBBY_USERUPDATE"|"NETMSG_CHARACTER_REQUEST_WEAPON_SET_SWITCH"|"NETMSG_UI_COMBINE_OPEN"|"NETMSG_SPELL_CAST_START_PREVIEW"|"NETMSG_LOAD_START"|"NETMSG_LEVEL_INSTANTIATE_SWAP"|"NETMSG_CHARACTER_BEHAVIOR"|"NETMSG_INVENTORY_LOCKSTATE_SYNC"|"NETMSG_JOURNALRECIPE_UPDATE"|"NETMSG_QUEST_TRACK"|"NETMSG_LOBBY_STARTGAME"|"NETMSG_CHARACTER_REQUEST_ARMOR_SET_SWITCH"|"NETMSG_TUTORIAL_CLOSED"|"NETMSG_SPELL_CAST_UPDATE_PREVIEW"|"NETMSG_LOAD_STARTED"|"NETMSG_LEVEL_SWAP_READY"|"NETMSG_PARTY_BLOCKFOLLOW"|"NETMSG_SURFACE_CREATE"|"NETMSG_MARKER_UI_UPDATE"|"NETMSG_NOTIFICATION"|"NETMSG_NET_ENTITY_CREATE"|"NETMSG_CHARACTER_CREATION_PLAYORIGINTIMELINE"|"NETMSG_TUTORIAL_UI_EVENT"|"NETMSG_SPELL_CAST_CONFIRM"|"NETMSG_LEVEL_SWAP_COMPLETE"|"NETMSG_LEVEL_START"|"NETMSG_PROJECTILE_CREATE"|"NETMSG_DARKNESSTILE_UPDATE"|"NETMSG_MARKER_UI_CREATE"|"NETMSG_LOCK_WAYPOINT"|"NETMSG_NET_ENTITY_DESTROY"|"NETMSG_TUTORIAL_USER_ACTION"|"NETMSG_HOTBAR_SLOT_REMOVE_IS_NEW_FLAG"|"NETMSG_SPELL_CAST_CANCEL"|"NETMSG_CHARACTER_CREATE"|"NETMSG_CHARACTER_AOO"|"NETMSG_PROJECTILE_UPDATE"|"NETMSG_SPELL_REMOVE_LEARNED"|"NETMSG_JOURNALDIALOGLOG_UPDATE"|"NETMSG_FLAG_UPDATE"|"NETMSG_TRIGGER_CREATE"|"NETMSG_TUTORIALS_SHOWING"|"NETMSG_SPELL_CAST_CAST"|"NETMSG_CHARACTER_ORIGINAL_TEMPLATE_UPDATE"|"NETMSG_CHARACTER_DESTROY"|"NETMSG_PARTYGROUP"|"NETMSG_MULTIPLE_TARGET_OPERATION"|"NETMSG_TADPOLE_POWER_ADD"|"NETMSG_UI_QUESTSELECTED"|"NETMSG_CAMERA_ROTATE"|"NETMSG_TRIGGER_DESTROY"|"NETMSG_TUTORIALS_RESET"|"NETMSG_USE_ITEM_ACTION"|"NETMSG_CHARACTER_CONFIRMATION"|"NETMSG_PARTYORDER"|"NETMSG_TURNBASED_FINISHTEAM"|"NETMSG_TADPOLE_POWERS_REMOVE"|"NETMSG_MYSTERY_ADVANCED"|"NETMSG_CAMERA_TARGET"|"NETMSG_TRIGGER_UPDATE"|"NETMSG_SNEAKING_CONES_VISIBLE_TOGGLE"|"NETMSG_INVENTORY_CREATE_NEW"|"NETMSG_CHARACTER_ACTIVATE"|"NETMSG_TURNBASED_SETTEAM"|"NETMSG_ITEM_CREATE"|"NETMSG_GAMECONTROL_UPDATE_S2C"|"NETMSG_MYSTERY_DISABLED"|"NETMSG_SHROUD_UPDATE"|"NETMSG_CUSTOM_STATS_CREATE"|"NETMSG_HOTBAR_SLOT_SET"|"NETMSG_INVENTORY_DESTROY_NEW"|"NETMSG_CHARACTER_DEACTIVATE"|"NETMSG_TURNBASED_FLEECOMBATRESULT"|"NETMSG_ITEM_DESTROY"|"NETMSG_GAMECONTROL_UPDATE_C2S"|"NETMSG_REGISTER_WAYPOINT"|"NETMSG_PLAYMOVIE"|"NETMSG_CUSTOM_STATS_UPDATE"|"NETMSG_HOTBAR_COLUMN_SET"|"NETMSG_INVENTORY_MOVETOSLOT"|"NETMSG_CHARACTER_ASSIGN"|"NETMSG_TURNBASED_FLEE_REQUEST"|"NETMSG_ITEM_ACTIVATE"|"NETMSG_GAMECONTROL_PRICETAG"|"NETMSG_UNLOCK_WAYPOINT"|"NETMSG_TRADE_ACTION"|"NETMSG_LOAD_GAME_WITH_ADDONS"|"NETMSG_HOTBAR_ROWS_SET"|"NETMSG_INVENTORY_TRANSFER"|"NETMSG_CHARACTER_CHANGE_OWNERSHIP"|"NETMSG_TURNBASED_ENDTURN_REQUEST"|"NETMSG_ITEM_UPDATE"|"NETMSG_CHANGE_RULESETS"|"NETMSG_LIGHTING_OVERRIDE"|"NETMSG_GAMETIME_SYNC"|"NETMSG_LOAD_GAME_WITH_ADDONS_FAIL"|"NETMSG_HOTBAR_ROW_REMOVE"|"NETMSG_INVENTORY_SORT"|"NETMSG_CHARACTER_STEERING"|"NETMSG_TURNBASED_SKIP_START_DELAY"|"NETMSG_ITEM_ACTION"|"NETMSG_REALTIME_MULTIPLAY"|"NETMSG_ATMOSPHERE_OVERRIDE"|"NETMSG_TELEPORT_ACK"|"NETMSG_CLIENT_GAME_SETTINGS"|"NETMSG_HOTBAR_LOCK_SET"|"NETMSG_INVENTORY_EQUIP"|"NETMSG_HANDSHAKE"|"NETMSG_CHARACTER_MOVEMENT_FALLING"|"NETMSG_UI_FORCETURNBASED_ENTERED"|"NETMSG_ITEM_STATUS"|"NETMSG_CHARACTER_ERROR"|"NETMSG_CAMERA_ACTIVATE"|"NETMSG_UNLOCK_ITEM"|"NETMSG_TIMELINE_HANDSHAKE"|"NETMSG_HOTBAR_CUSTOM_DECK"|"NETMSG_INVENTORY_UNEQUIP"|"NETMSG_HOST_WELCOME"|"NETMSG_CHARACTER_ACTION"|"NETMSG_UI_FORCETURNBASED_LEFT"|"NETMSG_ITEM_OFFSTAGE"|"NETMSG_REQUESTAUTOSAVE"|"NETMSG_ACHIEVEMENT_UNLOCKED_MESSAGE"|"NETMSG_DIALOG_LISTEN"|"NETMSG_TIMELINE_ACTOR_HANDSHAKE"|"NETMSG_HOTBAR_CURRENT_DECK_SET"|"NETMSG_INVENTORY_LOCK"|"NETMSG_HOST_REFUSE"|"NETMSG_CHARACTER_ACTION_DATA"|"NETMSG_UI_FORCETURNBASED_TURN_ENDED"|"NETMSG_SURFACE_META"|"NETMSG_SAVEGAMEHANDSHAKE"|"NETMSG_SAVEGAME_LOAD_FAIL"|"NETMSG_CAMERA_SPLINE"|"NETMSG_TIMELINE_NODECOMPLETED"|"NETMSG_PARTY_PRESET_SAVE"|"NETMSG_INTERRUPT_DECISION"|"NETMSG_CLIENT_JOINED"|"NETMSG_CHARACTER_ACTION_REQUEST_RESULT"|"NETMSG_CHARACTER_STATUS"|"NETMSG_ITEM_DEACTIVATE"|"NETMSG_GAMEACTION"|"NETMSG_SAVEGAMEHANDSHAKE_SCREENSHOT"|"NETMSG_SERVER_COMMAND"|"NETMSG_MUSIC_EVENT"|"NETMSG_TIMELINE_PLAYER_WATCHING"|"NETMSG_PARTY_PRESET_LOAD"|"NETMSG_INTERRUPT_SET_ASK_DATA"|"NETMSG_CLIENT_LEFT"|"NETMSG_CHARACTER_STATUS_LIFETIME"|"NETMSG_TURNBASED_CANCEL_ENDTURN_REQUEST"|"NETMSG_ITEM_DESTINATION"|"NETMSG_GAMEOVER"|"NETMSG_EFFECT_DESTROY"|"NETMSG_SERVER_NOTIFICATION"|"NETMSG_MUSIC_STATE"|"NETMSG_TIMELINE_REQUEST_FASTFORWARD"|"NETMSG_PARTY_PRESET_LEVELUP"|"NETMSG_INTERRUPT_SET_ENABLED_DATA"|"NETMSG_HOST_LEFT"|"NETMSG_CHARACTER_DIALOG"|"NETMSG_TURNBASED_BEGIN_CANCEL_ENDTURN_REQUEST"|"NETMSG_ITEM_USE_REMOTELY"|"NETMSG_ACT_OVER"|"NETMSG_COMBATLOGITEMINTERACTION"|"NETMSG_STORY_ELEMENT_UI"|"NETMSG_PAUSE"|"NETMSG_CLIENT_DIALOG_PRIVACY_CHANGED"|"NETMSG_PARTY_PRESET_SPELL"|"NETMSG_CAMERA_ARRIVED"
--- @alias PathRootType string|"Scripts"|"Mods"|"Public"|"LocalAppData"|"UserProfile"|"Localization"|"Bin"|"Root"|"Bin2"|"Projects"|"Public2"|"GameMod"|"EngineMod"|"WorkingDir"|"Data"|"Debug"
--- @alias ProficiencyBonusBoostType string|"AttackTarget"|"SavingThrow"|"AllSavingThrows"|"AttackRoll"|"AllAbilities"|"AllSkills"|"SourceDialogue"|"Ability"|"Skill"|"WeaponActionDC"
--- @alias ProficiencyGroupFlags string|"HandCrossbows"|"Handaxes"|"HeavyArmor"|"HeavyCrossbows"|"Javelins"|"LightArmor"|"LightCrossbows"|"LightHammers"|"Longbows"|"Longswords"|"Maces"|"MartialWeapons"|"Mauls"|"MediumArmor"|"Morningstars"|"Pikes"|"Quarterstaffs"|"Rapiers"|"Scimitars"|"Shields"|"Shortbows"|"Battleaxes"|"Shortswords"|"Clubs"|"Sickles"|"Daggers"|"SimpleWeapons"|"Darts"|"Slings"|"Flails"|"Spears"|"MusicalInstrument"|"Glaives"|"Tridents"|"Greataxes"|"Warhammers"|"Greatclubs"|"Warpicks"|"Greatswords"|"Halberds"
--- @alias ProjectileTypeIds string|"Magical"|"Physical"
--- @alias RecipeCategory string|"Armour"|"Common"|"Food"|"Runes"|"Grimoire"|"Objects"|"Weapons"|"Potions"|"Grenades"|"Arrows"
--- @alias RequirementType string|"Vitality"|"Telekinesis"|"None"|"Summoning"|"Loremaster"|"Barter"|"Charm"|"Persuasion"|"Immobile"|"Intimidate"|"Ranged"|"Tag"|"Sneaking"|"Pickpocket"|"Wisdom"|"Intelligence"|"TurnBased"|"Constitution"|"Combat"|"Level"|"Dexterity"|"Reason"|"Strength"|"Necromancy"|"PainReflection"|"Reflexes"|"Sourcery"
--- @alias ResistanceBoostFlags string|"BelowDamageThreshold"|"ResistantToMagical"|"ImmuneToMagical"|"VulnerableToMagical"|"ImmuneToNonMagical"|"ResistantToNonMagical"|"VulnerableToNonMagical"
--- @alias ResourceBankType string|"Script"|"Sentinel"|"AnimationBlueprint"|"DiffusionProfile"|"Dialog"|"VoiceBark"|"Effect"|"MaterialPreset"|"Visual"|"VisualSet"|"Animation"|"AnimationSet"|"Atmosphere"|"MeshProxy"|"Timeline"|"MaterialSet"|"Texture"|"BlendSpace"|"FCurve"|"Lighting"|"IKRig"|"Skeleton"|"CharacterVisual"|"VirtualTexture"|"Sound"|"TerrainBrush"|"Material"|"TileSet"|"ColorList"|"Physics"|"SkinPreset"|"ClothCollider"|"LightCookie"|"TimelineScene"|"SkeletonMirrorTable"
--- @alias ResourceReplenishType string|"Default"|"Never"|"Combat"|"ExhaustedRest"|"FullRest"|"ShortRest"|"Rest"
--- @alias RestErrorFlags string|"Script"|"Dialog"|"Combat"|"GlobalDisabled"|"FTB"|"NotEnoughResources"|"DownedOrDead"
--- @alias RestFlags string|"CanShortRest"|"CanLongRest"|"Unknown0x04"
--- @alias RollCritical string|"None"|"Success"|"Fail"
--- @alias ServerCharacterFlags string|"SteeringEnabled"|"InParty"|"PartyFollower"|"Unknown10000000000"|"IsCompanion_M"|"CannotMove"|"Unknown40000000000"|"Deactivated"|"CustomLookEnabled"|"Summon"|"WalkThrough"|"IsHuge"|"DisableCulling"|"DoNotFaceFlag"|"GMReroll"|"Multiplayer"|"Detached"|"HostControl"|"RequestStartTurn"|"Activated"|"Dead"|"Floating"|"OffStage"|"HasOwner"|"InDialog"|"CannotDie"|"FindValidPositionOnActivate"|"SpotSneakers"|"StoryNPC"|"CharCreationInProgress"|"CharacterControl"|"CharacterCreationFinished"|"Unknown8000"|"NeedsMakePlayerUpdate"|"Loaded"|"ForceNonzeroSpeed"|"DeferredRemoveEscapist"|"CannotAttachToGroups"|"LevelTransitionPending"|"Invulnerable"|"FightMode"|"CanShootThrough"|"RegisteredForAutomatedDialog"|"CannotRun"|"Unknown80000000000000"|"Unknown80000000"|"IsPlayer"|"DontCacheTemplate"|"ReservedForDialog"|"DisableWaypointUsage"|"Temporary"|"CoverAmount"|"IgnoresTriggers"|"Totem"|"Invisible"|"IsTrading"
--- @alias ServerCharacterFlags2 string|"TreasureGeneratedForTrader"|"CanGossip"|"Trader"|"IsResurrected"|"IsPet"|"Unknown0x10"|"Unknown0x40"|"Global"
--- @alias ServerCharacterFlags3 string|"CrimeWarningsEnabled"|"NeedsPlacementSnapping"
--- @alias ServerGameState string|"UnloadLevel"|"UnloadModule"|"UnloadSession"|"Paused"|"Running"|"Disconnect"|"Unknown"|"Save"|"BuildStory"|"Init"|"Uninitialized"|"Sync"|"ReloadStory"|"Idle"|"LoadModule"|"Exit"|"LoadLevel"|"LoadSession"
--- @alias ServerItemFlags string|"PositionChanged"|"IsSurfaceBlocker"|"Destroy"|"FreezeGravity"|"GMFolding"|"StoryItem"|"InUse"|"Known"|"Sticky"|"InteractionDisabled"|"Destroyed"|"IsSecretDoor"|"WalkThrough"|"WakePhysics"|"Summon"|"Invulnerable2"|"CanBePickedUp"|"ForceClientSync"|"WalkOn"|"InPartyInventory"|"SourceContainer"|"CanUse"|"Frozen"|"TeleportOnUse"|"Activated"|"InAutomatedDialog"|"Floating"|"IsSurfaceCloudBlocker"|"PinnedContainer"|"OffStage"|"NoCover"|"TransformChanged"|"CanOnlyBeUsedByOwner"|"IsMoving"|"LoadedTemplate"|"LevelTransitionPending"|"CanShootThrough"|"ClientSync1"|"Invulnerable"|"IsContainer"|"HideHP"|"CanBeMoved"|"LuckyFind"|"ForceSync"|"ReservedForDialog"|"DisableInventoryView80"|"Invisible2"|"IsDoor"|"DisableSync"|"IsLadder"|"Totem"|"Invisible"|"DontAddToHotbar"
--- @alias ServerItemFlags2 string|"UnsoldGenerated"|"TreasureGenerated"|"UnEquipLocked"|"UseRemotely"|"Global"
--- @alias ServerStatusFlags string|"ForceStatus"|"ForceFailStatus"|"RequestDelete"|"RequestDeleteAtTurnEnd"|"RequestClientSync2"|"RequestClientSync"|"Loaded"|"Started"
--- @alias ServerStatusFlags2 string|"Influence"|"DontTickWhileOnSurface"|"ExcludeFromPortraitRendering"|"IsFromItem"|"IsLifeTimeSet"|"Channeled"|"InitiateCombat"|"KeepAlive"
--- @alias ServerStatusFlags3 string|"StatusFlags3_0x40"|"StatusFlags3_0x80"|"DisableImmunityOverhead"|"IsUnique"|"NotifiedPlanManager"|"StatusFlags3_0x08"|"StatusFlags3_0x10"|"StatusFlags3_0x20"
--- @alias ServerStatusFlags4 string|"BringIntoCombat"|"IsHostileAct"|"StatusFlags4_0x04"|"StatusFlags4_0x20"|"StatusFlags4_0x40"|"IsInvulnerableVisible"|"StatusFlags4_0x80"|"IsInvulnerable"
--- @alias ServerStatusFlags5 string|"HasTriedEntering"
--- @alias SetStatusDurationType string|"SetMinimum"|"ForceSet"|"Multiply"|"Unknown"|"Add"
--- @alias ShroudType string|"Shroud"|"RegionMask"|"Sneak"|"Sight"
--- @alias SkillId string|"Arcana"|"Stealth"|"SleightOfHand"|"Sentinel"|"Acrobatics"|"Persuasion"|"Performance"|"Deception"|"Intimidation"|"Survival"|"Perception"|"Medicine"|"Insight"|"AnimalHandling"|"Athletics"|"Invalid"|"Religion"|"Nature"|"Investigation"|"History"
--- @alias SourceAdvantageType string|"SourceAllyAdvantageOnAttack"|"SourceAdvantageOnAttack"
--- @alias SpellAttackType string|"None"|"RangedUnarmedAttack"|"MeleeUnarmedAttack"|"RangedSpellAttack"|"MeleeSpellAttack"|"RangedWeaponAttack"|"MeleeWeaponAttack"|"DirectHit"|"RangedOffHandWeaponAttack"|"MeleeOffHandWeaponAttack"
--- @alias SpellAttackTypeOverride string|"Target_MainHandAttack"|"Projectile_MainHandAttack"|"Projectile_OffhandAttack"|"Target_UnarmedAttack"|"Target_OffhandAttack"
--- @alias SpellChildSelectionType string|"Singular"|"AddChildren"|"MostPowerful"
--- @alias SpellCooldownType string|"Default"|"OncePerTurn"|"OncePerCombat"|"UntilRest"|"OncePerTurnNoRealtime"|"UntilShortRest"|"OncePerShortRestPerItem"|"UntilPerRestPerItem"
--- @alias SpellFlags string|"IsJump"|"RangeIgnoreSourceBounds"|"CallAlliesSpell"|"IsAttack"|"Stealth"|"RangeIgnoreTargetBounds"|"CannotRotate"|"IsMelee"|"RangeIgnoreVerticalThreshold"|"NoCameraMove"|"TrajectoryRules"|"IsDefaultWeaponAction"|"CanDualWield"|"TargetClosestEqualGroundSurface"|"AllowMoveAndCast"|"IsLinkedSpellContainer"|"UNUSED_D"|"Wildshape"|"AbortOnSecondarySpellRollFail"|"UNUSED_E"|"IgnorePreviouslyPickedEntities"|"UnavailableInDialogs"|"PickupEntityAndMove"|"RangeIgnoreBlindness"|"AbortOnSpellRollFail"|"NoAOEDamageOnLand"|"CanAreaDamageEvade"|"DontAbortPerforming"|"NoCooldownOnMiss"|"IsSwarmAttack"|"DisplayInItemTooltip"|"HideInItemTooltip"|"IsConcentration"|"DisableBlood"|"AddFallDamageOnLand"|"IgnoreAoO"|"InventorySelection"|"IsSpell"|"IsEnemySpell"|"CannotTargetItems"|"CannotTargetTerrain"|"IgnoreVisionBlock"|"AddWeaponRange"|"HasHighGroundRangeExtension"|"IgnoreSilence"|"ConcentrationIgnoresResting"|"ImmediateCast"|"Temporary"|"HasVerbalComponent"|"CombatLogSetSingleLineRoll"|"NoSurprise"|"IsTrap"|"HasSomaticComponent"|"CannotTargetCharacter"|"IsHarmful"|"Invisible"
--- @alias SpellPrepareType string|"Unknown"|"AlwaysPrepared"|"RequiresPreparation"
--- @alias SpellSchoolId string|"None"|"Abjuration"|"Conjuration"|"Divination"|"Enchantment"|"Evocation"|"Illusion"|"Necromancy"|"Transmutation"
--- @alias SpellSourceType string|"Progression2"|"Boost"|"Shapeshift"|"SpellSet2"|"CreateExplosion"|"WeaponAttack"|"Progression"|"Sentinel"|"UnarmedAttack"|"Anubis"|"EquippedItem"|"AiTest"|"ActiveDefense"|"SpellSet"|"GameActionCreateSurface"|"Learned"|"Unknown15"|"Behavior"|"Unknown16"|"Unknown17"|"Unknown18"|"Unknown19"|"Spell"|"Unknown1A"|"PartyPreset"|"Osiris"|"Progression0"|"Progression1"
--- @alias SpellType string|"Zone"|"Rush"|"Wall"|"Throw"|"Shout"|"ProjectileStrike"|"Storm"|"MultiStrike"|"Projectile"|"Target"|"Teleportation"
--- @alias StatAttributeFlags string|"PickpocketableWhenEquipped"|"LoseDurabilityOnCharacterHit"|"InvulnerableAndInteractive"|"EnableObscurityEvents"|"ObscurityWithoutSneaking"|"SlippingImmunity"|"Floating"|"Torch"|"Unbreakable"|"Unrepairable"|"Unstorable"|"Arrow"|"Grounded"|"EMPTY"|"IgnoreClouds"|"LootableWhenEquipped"|"ThrownImmunity"|"InvisibilityImmunity"|"Backstab"|"BackstabImmunity"|"FloatingWhileMoving"
--- @alias StatCharacterFlags string|"InParty"|"IsSneaking"|"Blind"|"DrinkedPotion"|"EquipmentValidated"|"IsPlayer"|"Invisible"
--- @alias StatsDeathType string|"DoT"|"None"|"Chasm"|"Acid"|"Disintegrate"|"Cold"|"CinematicDeath"|"Radiant"|"Psychic"|"Physical"|"Necrotic"|"Lifetime"|"KnockedDown"|"Incinerate"|"Falling"|"Explode"|"Electrocution"
--- @alias StatsExpressionParamType string|"Int"|"Bool"|"StatsExpressionType"|"RollDefinition"|"StatsContextType"|"Type1"|"StatsExpressionVariableDataType"|"StatsExpressionVariableDataModifier"|"ResourceRollDefinition"
--- @alias StatsExpressionParamType2 string|"STDString"|"StatsExpressionVariableData"|"StatusGroup"|"Ability"|"Skill"
--- @alias StatsFunctorFlags string|"Self"|"Owner"|"Swap"
--- @alias StatsFunctorId string|"CreateSurface"|"SwitchDeathType"|"SwapPlaces"|"CreateExplosion"|"CreateConeSurface"|"SurfaceClearLayer"|"SurfaceChange"|"Douse"|"Pickup"|"Counterspell"|"SummonInInventory"|"Summon"|"MaximizeRoll"|"CameraWait"|"BreakConcentration"|"SpawnExtraProjectiles"|"Kill"|"RemoveAuraByChildStatus"|"Stabilize"|"RegainTemporaryHitPoints"|"ApplyStatus"|"SpawnInInventory"|"ApplyEquipmentStatus"|"Spawn"|"GainTemporaryHitPoints"|"ExecuteWeaponFunctors"|"Force"|"SetStatusDuration"|"FireProjectile"|"SetRoll"|"AdjustRoll"|"SetReroll"|"SetDisadvantage"|"Drop"|"SetDamageResistance"|"DoTeleport"|"SetAdvantage"|"Sabotage"|"DisarmWeapon"|"ResetCooldowns"|"UseSpell"|"DisarmAndStealWeapon"|"Resurrect"|"DealDamage"|"UseAttack"|"Extender"|"UseActionResource"|"RestoreResource"|"ResetCombatTurn"|"Unsummon"|"Unlock"|"RemoveUniqueStatus"|"ShortRest"|"RemoveStatusByLevel"|"CustomDescription"|"RemoveStatus"|"TriggerRandomCast"|"RegainHitPoints"|"TeleportSource"|"CreateZone"|"CreateWall"|"TutorialEvent"
--- @alias StatsItemSlot string|"Max"|"Sentinel"|"VanityBoots"|"VanityBody"|"Wings"|"Underwear"|"Helmet"|"Gloves"|"Breast"|"Cloak"|"OffHand"|"MeleeMainHand"|"MainHand"|"MeleeOffHand"|"RangedMainHand"|"RangedOffHand"|"Ring"|"MusicalInstrument"|"Boots"|"Amulet"|"Horns"|"Ring2"|"Overhead"
--- @alias StatsObserverType string|"None"|"Observer"|"Target"|"Source"
--- @alias StatsPropertyContext string|"PUSH"|"PROJECTILE"|"PROFICIENCY_CHANGED"|"OBSCURITY_CHANGED"|"SURFACE_ENTER"|"MOVED_DISTANCE"|"COMBAT_ENDED"|"CAST_RESOLVED"|"CAST"|"STATUS_REMOVED"|"LONG_REST"|"LEAVE_ATTACK_RANGE"|"STATUS_REMOVE"|"STATUS_APPLY"|"STATUS_APPLIED"|"ATTACKED"|"INVENTORY_CHANGED"|"ATTACK"|"INTERRUPT_USED"|"HEALED"|"LOCKPICKING_SUCCEEDED"|"HEAL"|"CREATE"|"AOE"|"GROUND"|"SHORT_REST"|"ATTACKING_IN_MELEE_RANGE"|"AI_ONLY"|"ATTACKED_IN_MELEE_RANGE"|"AI_IGNORE"|"EQUIP"|"ACTION_RESOURCES_CHANGED"|"ABILITY_CHECK"|"ENTER_ATTACK_RANGE"|"DAMAGED_PREVENTED"|"DAMAGE_PREVENTED"|"DAMAGED"|"DAMAGE"|"TURN"|"TARGET"|"PUSHED"
--- @alias StatsRollAdjustmentType string|"Distribute"|"All"
--- @alias StatsRollType string|"RawAbility"|"DeathSavingThrow"|"None"|"SavingThrow"|"RangedUnarmedAttack"|"SkillCheck"|"Sentinel"|"Attack"|"MeleeUnarmedAttack"|"RangedSpellAttack"|"MeleeSpellAttack"|"RangedWeaponAttack"|"MeleeWeaponAttack"|"RangedOffHandWeaponAttack"|"MeleeOffHandWeaponAttack"|"Damage"|"RangedUnarmedDamage"|"MeleeUnarmedDamage"|"RangedSpellDamage"|"MeleeSpellDamage"|"RangedWeaponDamage"|"MeleeWeaponDamage"
--- @alias StatsStatusGroup string|"SG_Light"|"SG_Invisible"|"SG_Incapacitated"|"SG_HexbladeCurse"|"SG_Frightened"|"SG_ScriptedPeaceBehaviour"|"SG_Polymorph_BeastShape_NPC"|"SG_Fleeing"|"SG_Exhausted"|"SG_Polymorph_BeastShape"|"SG_Poisoned_Story_Removable"|"SG_Doppelganger"|"SG_Poisoned_Story_NonRemovable"|"SG_Dominated"|"SG_Disguise"|"SG_Helpable_Condition"|"SG_Disease"|"SG_DropForNonMutingDialog"|"SG_DifficultTerrain"|"SG_DetectThoughts"|"SG_Cursed"|"SG_Confused"|"SG_RemoveOnRespec"|"SG_Condition"|"SG_Sleeping"|"SG_Charmed_Subtle"|"SG_Drunk"|"SG_Charmed"|"SG_WeaponCoating"|"SG_Unconscious"|"SG_CanBePickedUp"|"SG_Blinded"|"SG_Taunted"|"SG_Surface"|"SG_Approaching"|"SG_Stunned"|"SG_Restrained"|"SG_Rage"|"SG_Prone"|"SG_Possessed"|"SG_Polymorph"|"SG_Poisoned"|"SG_Petrified"|"SG_Paralyzed"|"SG_Mad"
--- @alias StatsTargetTypeFlags string|"Living"|"Guaranteed"|"Construct"|"Undead"
--- @alias StatsZoneShape string|"Square"|"Cone"
--- @alias StatusHealType string|"Vitality"|"None"|"PhysicalArmor"|"MagicArmor"|"AllArmor"|"Source"|"All"
--- @alias StatusMaterialApplyFlags string|"ApplyOnBody"|"ApplyOnArmor"|"ApplyOnWeapon"|"ApplyOnWings"|"ApplyOnHorns"|"ApplyOnOverhead"
--- @alias StatusType string|"SMELLY"|"DYING"|"INVISIBLE"|"ROTATE"|"MATERIAL"|"HEAL"|"CLIMBING"|"KNOCKED_DOWN"|"INCAPACITATED"|"INSURFACE"|"STORY_FROZEN"|"POLYMORPHED"|"SNEAKING"|"DOWNED"|"UNLOCK"|"FEAR"|"BOOST"|"TELEPORT_FALLING"|"EFFECT"|"DEACTIVATED"|"REACTION"
--- @alias SurfaceActionType string|"CreateSurfaceAction"|"CreatePuddleAction"|"TransformSurfaceAction"|"ExtinguishFireAction"|"ChangeSurfaceOnPathAction"|"ZoneAction"|"RectangleSurfaceAction"|"PolygonSurfaceAction"
--- @alias SurfaceChange string|"Freeze"|"None"|"Deelectrify"|"Electrify"|"Douse"|"Ignite"|"DestroyWater"|"Condense"|"Vaporize"|"Melt"
--- @alias SurfaceLayer string|"None"|"Cloud"|"Ground"
--- @alias SurfaceLayer8 string|"None"|"Cloud"|"Ground"
--- @alias SurfaceTransformActionType string|"Freeze"|"None"|"Deelectrify"|"Electrify"|"Douse"|"Ignite"|"Condense"|"Vaporize"|"Melt"
--- @alias SurfaceType string|"WaterFrozen"|"ShockwaveCloud"|"None"|"BloodElectrified"|"Chasm"|"CloudkillCloud"|"BloodFrozen"|"Acid"|"MaliceCloud"|"Sentinel"|"Lava"|"Grease"|"BloodCloud"|"WyvernPoison"|"StinkingCloud"|"Oil"|"Web"|"DarknessCloud"|"Deepwater"|"FogCloud"|"Vines"|"SporeWhiteCloud"|"Poison"|"Fire"|"TrialFire"|"SporeGreenCloud"|"BlackPowder"|"SporeBlackCloud"|"ShadowCursedVines"|"WaterCloudElectrified"|"DrowPoisonCloud"|"AlienOil"|"GithPheromoneGasCloud"|"IceCloud"|"Mud"|"PotionHealingCloud"|"PotionHealingGreaterCloud"|"Alcohol"|"PotionHealingSuperiorCloud"|"PotionSpeedCloud"|"InvisibleWeb"|"PotionHealingSupremeCloud"|"PotionVitalityCloud"|"BloodSilver"|"PotionInvisibilityCloud"|"PotionAntitoxinCloud"|"Hellfire"|"PotionResistanceAcidCloud"|"SporePinkCloud"|"CausticBrine"|"PotionResistanceColdCloud"|"VoidCloud"|"BloodExploding"|"PotionResistanceFireCloud"|"CrawlerMucusCloud"|"SpikeGrowth"|"PotionResistanceForceCloud"|"Cloudkill6Cloud"|"Water"|"HolyFire"|"PotionResistanceLightningCloud"|"BlackTentacles"|"PotionResistancePoisonCloud"|"Ash"|"Overgrowth"|"BlackPowderDetonationCloud"|"PurpleWormPoison"|"SerpentVenom"|"InvisibleGithAcid"|"BladeBarrier"|"Blood"|"Sewer"|"WaterCloud"|"PoisonCloud"|"WaterElectrified"|"ExplosionCloud"
--- @alias TravelErrorFlags string|"InDanger"|"DangerZone"|"CannotFastTravel"|"Condition"|"Dialog"|"FTB"
--- @alias TravelFlags string|"BlockFastTravel"|"BlockCampTravel"
--- @alias WeaponFlags string|"Light"|"Torch"|"Ammunition"|"Finesse"|"Loading"|"Reach"|"Magical"|"NeedDualWieldingBoost"|"Lance"|"Range"|"Thrown"|"Heavy"|"Twohanded"|"Versatile"|"Dippable"|"NoDualWield"|"NotSheathable"|"Net"|"Unstowable"|"Melee"|"AddToHotbar"
--- @alias WeaponType string|"None"|"Sentinel"|"Sword"|"Club"|"Axe"|"Staff"|"Crossbow"|"Spear"|"Knife"|"Wand"|"Rifle"|"Arrow"|"Bow"


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
--- @field SubAmounts ActionResourcesComponentAmountSubAmount[]|nil
--- @field field_28 uint64
--- @field field_30 uint64


--- @class ActionResourcesComponentAmountSubAmount
--- @field Amount number
--- @field MaxAmount number


--- @class ActivationGroupContainerComponent:BaseComponent
--- @field Groups ActivationGroupContainerComponentActivationGroup[]


--- @class ActivationGroupContainerComponentActivationGroup
--- @field field_0 FixedString
--- @field field_4 FixedString


--- @class ActiveCharacterLightBoostComponent:BaseComponent
--- @field LightUUID FixedString


--- @class ActiveCharacterLightComponent:BaseComponent
--- @field Light FixedString


--- @class ActiveComponent:BaseComponent


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


--- @class AnubisTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class AppearanceOverrideComponent:BaseComponent
--- @field Visual GameObjectVisualData


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


--- @class BoostTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class BoostsContainerComponent:BaseComponent
--- @field Boosts table<BoostType, EntityHandle[]>


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


--- @class BreadcrumbComponent:BaseComponent
--- @field field_118 vec3
--- @field field_18 BreadcrumbComponentElement[]


--- @class BreadcrumbComponentElement
--- @field field_0 int32
--- @field field_14 vec3
--- @field field_4 uint8
--- @field field_8 vec3


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
--- @field AdditionalChoices int32[]
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
--- @field ActiveCharacterLightID FixedString
--- @field AliveInventoryType uint8
--- @field AnimationSetResourceID FixedString
--- @field AnubisConfigName FixedString
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
--- @field LightID FixedString
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
--- @field CombatName string
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
--- @field SpellId SpellId
--- @field field_0 EntityHandle
--- @field field_8 ConcentrationComponentElement[]


--- @class ConcentrationComponentElement
--- @field field_0 EntityHandle
--- @field field_10 EntityHandle
--- @field field_18 int16
--- @field field_1A boolean
--- @field field_8 EntityHandle


--- @class ConcentrationIgnoreDamageBoostComponent:BaseComponent
--- @field SpellSchool SpellSchoolId


--- @class ConditionRoll
--- @field Ability AbilityId
--- @field DataType uint8
--- @field Difficulty int32
--- @field Roll Variant<StatsRollType0,StatsRollType1,>
--- @field RollType ConditionRollType
--- @field Skill SkillId
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


--- @class CustomStatsComponent:BaseComponent
--- @field Stats table<FixedString, int32>


--- @class DamageBonusBoostComponent:BaseComponent
--- @field Amount StatsExpressionParam
--- @field DamageType DamageType
--- @field field_31 uint8


--- @class DamageModifierMetadata
--- @field Argument Variant<DiceValues,int32,StatsRollType1,>
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
--- @field Amount Variant<int32,StatsExpressionParam,>
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


--- @class DelayDeathCauseComponent:BaseComponent
--- @field Blocked_M int32
--- @field DelayCount int32


--- @class DetectCrimesBlockBoostComponent:BaseComponent
--- @field field_0 boolean


--- @class DialogStateComponent:BaseComponent
--- @field field_0 uint8
--- @field field_1 uint8
--- @field field_2 uint8
--- @field field_4 int32
--- @field field_8 uint8


--- @class DialogTagComponent:BaseComponent
--- @field Tags Guid[]


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
--- @field Name string
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


--- @class EncumbranceStateComponent:BaseComponent
--- @field State uint32


--- @class EncumbranceStatsComponent:BaseComponent
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


--- @class ExperienceGaveOutComponent:BaseComponent
--- @field Experience int32


--- @class ExpertiseBonusBoostComponent:BaseComponent
--- @field Skill SkillId


--- @class ExpertiseComponent:BaseComponent
--- @field Expertise Array_SkillId


--- @class FTBParticipantComponent:BaseComponent
--- @field field_18 EntityHandle


--- @class FactionComponent:BaseComponent
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
--- @field Flags FleeErrorFlags
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
--- @field Flags uint32
--- @field GroupID uint32
--- @field HasGameplayValue boolean
--- @field HasParentModRelation boolean
--- @field Id FixedString
--- @field IsDeleted boolean
--- @field IsGlobal boolean
--- @field IsReflecting boolean
--- @field IsShadowProxy boolean
--- @field LevelName FixedString
--- @field Name string
--- @field ParentTemplateId FixedString
--- @field PhysicsOpenTemplate FixedString
--- @field PhysicsTemplate FixedString
--- @field ReceiveDecal boolean
--- @field RenderChannel uint8
--- @field TemplateName FixedString
--- @field TemplateType FixedString
--- @field Transform Transform
--- @field VisualTemplate FixedString
--- @field field_50 int32


--- @class GameObjectVisualComponent:BaseComponent
--- @field Icon FixedString
--- @field RootTemplateId FixedString
--- @field RootTemplateType uint8
--- @field Scale number
--- @field Type uint8


--- @class GameObjectVisualData
--- @field AdditionalChoices int32[]
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


--- @class GameTimerComponent:BaseComponent
--- @field field_18 FixedString
--- @field field_20 EntityHandle
--- @field field_28 int32
--- @field field_2C int32
--- @field field_30 int32
--- @field field_34 int32
--- @field field_38 uint8


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


--- @class GlobalLongRestDisabledComponent:BaseComponent


--- @class GlobalShortRestDisabledComponent:BaseComponent


--- @class GlobalSwitches


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
--- @field AC int32
--- @field Hp int32
--- @field MaxHp int32
--- @field MaxTemporaryHp int32
--- @field PerDamageTypeHealthThresholds uint16[][]
--- @field PerDamageTypeHealthThresholds2 uint32[][]
--- @field Resistances uint8[][]
--- @field TemporaryHp int32
--- @field field_40 Guid


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
--- @field field_0 uint8


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


--- @class IncreaseMaxHPComponent:BaseComponent
--- @field Amount Variant<int32,StatsExpressionParam,>
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


--- @class IsUnsummoningComponent:BaseComponent


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
--- @field EquipmentTypeID Guid
--- @field ExamineRotation vec3
--- @field ExcludeInDifficulty Guid[]
--- @field ForceAffectedByAura boolean
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
--- @field IsTradable uint8
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


--- @class JumpMaxDistanceBonusBoostComponent:BaseComponent
--- @field DistanceBonus number


--- @class JumpMaxDistanceMultiplierBoostComponent:BaseComponent
--- @field Amount number


--- @class KeyComponent:BaseComponent
--- @field Key FixedString


--- @class LeaderComponent:BaseComponent
--- @field Followers_M Array_EntityHandle


--- @class LevelComponent:BaseComponent
--- @field LevelName FixedString
--- @field field_0 EntityHandle


--- @class LevelUpComponent:BaseComponent
--- @field field_18 LevelUpData[]


--- @class LevelUpData
--- @field Abilities int32[]
--- @field AccessorySet Guid
--- @field Class Guid
--- @field Feat Guid
--- @field SubClass Guid
--- @field Upgrades LevelUpUpgrades
--- @field field_B0 SpellIdBase[]


--- @class LevelUpUpgrades
--- @field AbilityBonuses LevelUpUpgradesAbilityBonusData[]
--- @field Feats LevelUpUpgradesFeatData[]
--- @field Skills LevelUpUpgradesSkillData[]
--- @field Spells LevelUpUpgradesSpellData[]
--- @field Spells2 LevelUpUpgradesSpell2Data[]
--- @field Unknowns2 LevelUpUpgradesUnknown2Data[]
--- @field Unknowns4 LevelUpUpgradesUnknown4[]


--- @class LevelUpUpgradesAbilityBonusData:LevelUpUpgradesReference
--- @field AbilityBonus Guid
--- @field Array_b8 uint8[]
--- @field Array_i32 uint32[]
--- @field field_30 int32
--- @field field_60 string
--- @field field_80 int32


--- @class LevelUpUpgradesFeatData:LevelUpUpgradesReference
--- @field Array_b8 uint8[]
--- @field Feat Guid
--- @field FeatName string
--- @field field_30 int32
--- @field field_80 int32


--- @class LevelUpUpgradesReference
--- @field Class Guid
--- @field Subclass Guid
--- @field field_0 uint8
--- @field field_28 int32
--- @field field_2C uint8


--- @class LevelUpUpgradesSkillData:LevelUpUpgradesReference
--- @field Array_b8 uint8[]
--- @field Skill Guid
--- @field field_30 int32
--- @field field_60 string
--- @field field_80 int32


--- @class LevelUpUpgradesSpell2Data:LevelUpUpgradesReference
--- @field Array_FS2 LevelUpUpgradesSpell2DataStringPair[]
--- @field SpellList Guid
--- @field Spells FixedString[]
--- @field field_30 int32
--- @field field_78 string
--- @field field_80 int32


--- @class LevelUpUpgradesSpell2DataStringPair
--- @field A FixedString
--- @field B FixedString


--- @class LevelUpUpgradesSpellData:LevelUpUpgradesReference
--- @field Array_FS2 LevelUpUpgradesSpellDataStringPair[]
--- @field SpellList Guid
--- @field Spells FixedString[]
--- @field field_30 int32
--- @field field_78 string


--- @class LevelUpUpgradesSpellDataStringPair
--- @field A FixedString
--- @field B FixedString


--- @class LevelUpUpgradesUnknown2Data:LevelUpUpgradesReference
--- @field Array_b8 uint8[]
--- @field field_30 int32
--- @field field_38 Guid
--- @field field_48 uint8
--- @field field_60 string
--- @field field_80 int32


--- @class LevelUpUpgradesUnknown4:LevelUpUpgradesReference
--- @field Array_FS FixedString[]
--- @field field_30 int32
--- @field field_38 Guid
--- @field field_60 string
--- @field field_80 int32


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


--- @class MaterialParameterOverrideComponent:BaseComponent
--- @field field_0 Guid[]
--- @field field_10 MaterialParameterOverrideComponentParam[]


--- @class MaterialParameterOverrideComponentParam
--- @field field_0 string
--- @field field_18 FixedString


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


--- @class MovementComponent:BaseComponent
--- @field Acceleration number
--- @field Direction vec3
--- @field Speed number
--- @field Speed2 number


--- @class MovementSpeedLimitBoostComponent:BaseComponent
--- @field MovementType uint8


--- @class NetComponent:BaseComponent


--- @class NullifyAbilityBoostComponent:BaseComponent
--- @field Ability AbilityId


--- @class ObjectInteractionComponent:BaseComponent
--- @field Interactions EntityHandle[]


--- @class ObjectSizeBoostComponent:BaseComponent
--- @field SizeCategoryAdjustment int32


--- @class ObjectSizeComponent:BaseComponent
--- @field Size uint8
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
--- @field OriginalTemplate FixedString|nil


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


--- @class PickpocketComponent:BaseComponent
--- @field field_18 PickpocketComponentPickpocketEntry[]


--- @class PickpocketComponentPickpocketEntry
--- @field field_0 EntityHandle
--- @field field_10 int32
--- @field field_14 boolean
--- @field field_18 EntityHandle
--- @field field_8 EntityHandle


--- @class PlayerComponent:BaseComponent


--- @class ProficiencyBonusBoostComponent:BaseComponent
--- @field Ability AbilityId
--- @field Skill SkillId
--- @field Type ProficiencyBonusBoostType


--- @class ProficiencyBonusOverrideBoostComponent:BaseComponent
--- @field Value StatsExpressionParam


--- @class ProficiencyBoostComponent:BaseComponent
--- @field Flags ProficiencyGroupFlags


--- @class ProficiencyComponent:BaseComponent
--- @field field_0 int64


--- @class ProficiencyGroupComponent:BaseComponent
--- @field field_0 int64


--- @class ProgressionContainerComponent:BaseComponent
--- @field Progressions EntityHandle[][]


--- @class ProgressionMetaComponent:BaseComponent
--- @field Owner EntityHandle
--- @field Progression Guid
--- @field Race Guid
--- @field field_18 uint8
--- @field field_40 int32
--- @field field_44 uint8
--- @field field_48 int32


--- @class ProjectileDeflectBoostComponent:BaseComponent
--- @field ProjectileTypes ProjectileTypeIds


--- @class ProjectileTemplate:EoCGameObjectTemplate
--- @field Acceleration number
--- @field BeamFX FixedString
--- @field CastBone FixedString
--- @field DestroyTrailFXOnImpact boolean
--- @field DetachBeam boolean
--- @field GroundImpactFX string
--- @field IgnoreRoof boolean
--- @field ImpactFX FixedString
--- @field LifeTime number
--- @field MaxPathRadius number
--- @field MinPathRadius number
--- @field NeedsImpactSFX boolean
--- @field PathMaxArcDist number
--- @field PathMinArcDist number
--- @field PathRadius number
--- @field PathRepeat int32
--- @field PathShift number
--- @field PreviewPathImpactFX FixedString
--- @field PreviewPathMaterial FixedString
--- @field PreviewPathRadius number
--- @field ProjectilePath uint8
--- @field RotateImpact boolean
--- @field Speed number
--- @field TrailFX FixedString


--- @class RaceComponent:BaseComponent
--- @field Race Guid


--- @class RaceTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class RecruitedByComponent:BaseComponent
--- @field RecruitedBy EntityHandle


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


--- @class RespectComponent:BaseComponent


--- @class RollBonusBoostComponent:BaseComponent
--- @field Ability AbilityId
--- @field Amount StatsExpressionParam
--- @field RollType StatsRollType
--- @field Skill SkillId


--- @class RulesetComponent:BaseComponent
--- @field Modifiers table<Guid, Variant<uint8,int32,float,FixedString,bool,>>
--- @field Rulesets Guid[]


--- @class RulesetModifiersComponent:BaseComponent
--- @field Modifiers table<Guid, Variant<uint8,int32,float,FixedString,bool,>>


--- @class RuntimeStringHandle
--- @field Handle FixedString
--- @field Version uint16


--- @class SafePositionComponent:BaseComponent
--- @field Position vec3
--- @field field_24 boolean


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
--- @field field_0 uint8|nil
--- @field field_14 uint8
--- @field field_4 FixedString|nil
--- @field field_C FixedString|nil


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
--- @field SpellCastSource Guid|nil
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
--- @field Params Variant<uint8,Variant<uint8,AbilityId,SkillId,uint8,STDString,>,uint8,uint8,DiceValues,ResourceRollDefinition,uint8,int32,bool,>[]


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
--- @field Characters Array_EntityHandle
--- @field Items Array_EntityHandle
--- @field field_18 table<FixedString, EntityHandle[]>


--- @class SummonLifetimeComponent:BaseComponent
--- @field Lifetime number


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


--- @class TemplateTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class TemporaryHPBoostComponent:BaseComponent
--- @field HP StatsExpressionParam


--- @class TextureLayerConfigId
--- @field Flag uint8
--- @field TextureLayerConfig uint32


--- @class ToggledPassivesComponent:BaseComponent
--- @field Passives table<FixedString, boolean>


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
--- @field ElementType TypeInformationRef
--- @field EnumValues table<FixedString, uint64>
--- @field HasWildcardProperties boolean
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
--- @field Modifications Variant<SpellModificationModifyAreaRadius,SpellModificationModifyMaximumTargets,SpellModificationModifyNumberOfTargets,SpellModificationModifySavingThrowDisadvantage,SpellModificationModifySpellFlags,SpellModificationModifySpellRoll,SpellModificationModifyStatusDuration,SpellModificationModifySummonDuration,SpellModificationModifySurfaceDuration,SpellModificationModifyTargetRadius,SpellModificationModifyUseCosts,SpellModificationModifyVisuals,SpellModificationModifyIconGlow,SpellModificationModifyTooltipDescription,>[]
--- @field Spell string


--- @class UnsheathStateComponent:BaseComponent
--- @field field_0 EntityHandle
--- @field field_10 int32
--- @field field_14 int32
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


--- @class VisualComponent:BaseComponent
--- @field field_18 int64
--- @field field_20 uint8
--- @field field_21 uint8


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


--- @class CharacterCreationCharacterDefinition
--- @field Abilities int32[]
--- @field LevelUpData LevelUpData
--- @field Visual GameObjectVisualData
--- @field field_0 int32
--- @field field_20 CharacterCreationSomeDefinitionData
--- @field field_8 string


--- @class CharacterCreationCharacterDefinitionComponent:BaseComponent
--- @field Definition CharacterCreationCharacterDefinition
--- @field field_288 uint8
--- @field field_28C int32
--- @field field_290 uint8


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
--- @field field_0 int64
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
--- @field Definition CharacterCreationDefinitionCommon
--- @field field_0 int32
--- @field field_8 int64


--- @class CharacterCreationFullRespecDefinition
--- @field Abilities int32[]
--- @field Definition CharacterCreationSomeDefinitionData
--- @field LevelUpData LevelUpData[]
--- @field Visual GameObjectVisualData
--- @field field_0 string
--- @field field_1B0 CharacterCreationLevelUpUnknown[]


--- @class CharacterCreationFullRespecDefinitionComponent:BaseComponent
--- @field Definition CharacterCreationFullRespecDefinition
--- @field LevelUpUpgrades LevelUpUpgrades
--- @field Spells SpellIdBase[]
--- @field field_1C0 Guid
--- @field field_1D0 Guid
--- @field field_1E0 int32
--- @field field_268 uint8


--- @class CharacterCreationLevelUpDefinitionComponent:BaseComponent
--- @field Definition CharacterCreationFullRespecDefinition
--- @field LevelUp CharacterCreationLevelUpDefinitionExtra


--- @class CharacterCreationLevelUpDefinitionExtra
--- @field AppearanceElements GameObjectVisualDataAppearanceElement[]
--- @field LevelUpData LevelUpData
--- @field field_0 int32
--- @field field_160 uint8
--- @field field_164 int32
--- @field field_168 uint8
--- @field field_16C int32
--- @field field_170 EntityHandle
--- @field field_8 Guid


--- @class CharacterCreationLevelUpUnknown
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


--- @class CharacterCreationSomeDefinitionData
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


--- @class CharacterCreationStateComponent:BaseComponent
--- @field Canceled boolean
--- @field HasDummy boolean
--- @field field_2 uint8


--- @class CharacterCreationDefinitionCreationComponent:BaseComponent
--- @field field_0 EntityHandle[]
--- @field field_10 uint8


--- @class CharacterCreationDefinitionFullRespecComponent:BaseComponent
--- @field field_0 EntityHandle
--- @field field_8 EntityHandle


--- @class CharacterCreationDefinitionLevelUpComponent:BaseComponent
--- @field field_0 EntityHandle
--- @field field_10 string
--- @field field_8 EntityHandle


--- @class CharacterCreationDefinitionRespecComponent:BaseComponent
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


--- @class EclLuaGameStateChangedEvent:LuaEventBase
--- @field FromState ClientGameState
--- @field ToState ClientGameState


--- @class EocPlayerCustomData
--- @field Initialized boolean
--- @field OwnerProfileID FixedString
--- @field ReservedProfileID FixedString


--- @class EsvAnubisExecutorComponent:BaseComponent
--- @field field_18 int64
--- @field field_20 int64
--- @field field_28 uint8
--- @field field_29 uint8


--- @class EsvChangeSurfaceOnPathAction:EsvCreateSurfaceActionBase
--- @field CheckExistingSurfaces boolean
--- @field FollowHandle EntityRef
--- @field IgnoreIrreplacableSurfaces boolean
--- @field IgnoreOwnerCells boolean
--- @field IsFinished boolean
--- @field Radius number


--- @class EsvCharacter
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


--- @class EsvCharacterComponent:BaseComponent
--- @field Character EsvCharacter


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


--- @class EsvExtinguishFireAction:EsvCreateSurfaceActionBase
--- @field ExtinguishGrowTimer number
--- @field ExtinguishPosition vec3
--- @field Percentage number
--- @field Radius number
--- @field Step number


--- @class EsvIconListComponent:BaseComponent
--- @field Icons EsvIconListComponentIcon[]
--- @field field_30 uint8


--- @class EsvIconListComponentIcon
--- @field Icon FixedString
--- @field field_4 uint32


--- @class EsvItem
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


--- @class EsvItemComponent:BaseComponent
--- @field Item EsvItem


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


--- @class EsvProjectile:BaseComponent
--- @field CanDeflect boolean
--- @field Caster EntityHandle
--- @field CauseType uint8
--- @field CleanseChance number
--- @field CleanseStatuses FixedString
--- @field DamageMovingObjectOnLand boolean
--- @field DamageType DamageType
--- @field EffectHandle EntityHandle
--- @field ExplodeRadius number
--- @field Flags uint64
--- @field Hit Hit
--- @field HitInterpolation number
--- @field HitObject EntityHandle
--- @field IgnoreObjects boolean
--- @field IgnoreTargetChecks boolean
--- @field IsFromItem boolean
--- @field IsOnHold boolean
--- @field IsThrown boolean
--- @field IsTrap boolean
--- @field Launched boolean
--- @field LevelName FixedString
--- @field MyEntityHandle EntityRef
--- @field NetID NetId
--- @field Originator ActionOriginator
--- @field PathRadius number
--- @field PreviousTranslate vec3
--- @field RequestDelete boolean
--- @field ShouldFall boolean
--- @field Source EntityHandle
--- @field SourcePosition vec3
--- @field SourceWeapon EntityHandle
--- @field SpawnEffect FixedString
--- @field SpawnFXOverridesImpactFX boolean
--- @field SpellFlag0x04 boolean
--- @field SpellId SpellId
--- @field StoryActionID int32
--- @field Success boolean
--- @field TargetObject EntityHandle
--- @field TargetPosition vec3
--- @field TextKey FixedString
--- @field Used boolean
--- @field field_22C number
--- @field field_484 number
--- @field field_515 boolean
--- @field field_518 boolean
--- @field field_51E boolean
--- @field field_520 boolean


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


--- @class EsvStatus
--- @field BringIntoCombat boolean
--- @field Cause EntityRef
--- @field CauseGUID Guid
--- @field CauseType uint8
--- @field Channeled boolean
--- @field Combat_M EntityRef
--- @field ConditionsId int32
--- @field CurrentLifeTime number
--- @field DifficultyStatus FixedString
--- @field DisableImmunityOverhead boolean
--- @field DontTickWhileOnSurface boolean
--- @field ExcludeFromPortraitRendering boolean
--- @field Flags ServerStatusFlags
--- @field Flags2 ServerStatusFlags2
--- @field Flags3 ServerStatusFlags3
--- @field Flags4 ServerStatusFlags4
--- @field ForceFailStatus boolean
--- @field ForceStatus boolean
--- @field FreezeDuration boolean
--- @field Influence boolean
--- @field InitiateCombat boolean
--- @field IsFromItem boolean
--- @field IsHostileAct boolean
--- @field IsInvulnerable boolean
--- @field IsInvulnerableVisible boolean
--- @field IsLifeTimeSet boolean
--- @field IsRecoverable boolean
--- @field IsUnique boolean
--- @field KeepAlive boolean
--- @field LifeTime number
--- @field Loaded boolean
--- @field NotifiedPlanManager boolean
--- @field OriginCauseType uint8
--- @field Originator ActionOriginator
--- @field Owner EntityHandle
--- @field RemoveEvents uint32
--- @field RequestClientSync boolean
--- @field RequestClientSync2 boolean
--- @field RequestDelete boolean
--- @field RequestDeleteAtTurnEnd boolean
--- @field SourceEquippedItem EntityHandle
--- @field SourceSpell SpellId
--- @field SourceUsedItem EntityHandle
--- @field SpellCastSourceUuid Guid
--- @field SpellCastingAbility uint8
--- @field StackId FixedString
--- @field StackPriority int32
--- @field StartTimer number
--- @field Started boolean
--- @field StatusFlags3_0x08 boolean
--- @field StatusFlags3_0x10 boolean
--- @field StatusFlags3_0x20 boolean
--- @field StatusFlags3_0x40 boolean
--- @field StatusFlags3_0x80 boolean
--- @field StatusFlags4_0x04 boolean
--- @field StatusFlags4_0x20 boolean
--- @field StatusFlags4_0x40 boolean
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
--- @field field_104 uint8
--- @field field_105 uint8
--- @field field_150 Guid
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


--- @class EsvCombatCanStartCombatComponent:BaseComponent


--- @class EsvCombatCombatGroupMappingComponent:BaseComponent
--- @field CombatGroups table<FixedString, Array_EntityHandle>
--- @field Entity EntityHandle


--- @class EsvCombatEnterRequestComponent:BaseComponent
--- @field EnterRequests Array_EntityHandle


--- @class EsvCombatFleeBlockedComponent:BaseComponent


--- @class EsvCombatImmediateJoinComponent:BaseComponent


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
--- @field Roll Variant<StatsRollType0,StatsRollType1,>
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
--- @field field_28 int32
--- @field field_4 int32


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
--- @field SourcePos vec3|nil
--- @field SourceProxy EntityHandle
--- @field Target EntityHandle
--- @field TargetPos vec3|nil
--- @field TargetProxy EntityHandle
--- @field Variant Variant<InterruptInterruptType0,InterruptInterruptType1,InterruptInterruptType2,InterruptInterruptType3,InterruptInterruptType4,InterruptInterruptType5,InterruptInterruptType6,InterruptInterruptType7,InterruptInterruptType8,>
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
--- @field field_18 int32|nil
--- @field field_20 uint8
--- @field field_48 int32|nil
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
--- @field Info ItemDestroyInfo|nil


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
--- @field field_8 Guid


--- @class PartyCompositionComponentMember
--- @field field_18 Guid
--- @field field_28 uint8[]
--- @field field_8 Guid


--- @class PartyCurrentlyFollowingPartyComponent:BaseComponent


--- @class PartyFollowerComponent:BaseComponent
--- @field Following EntityHandle


--- @class PartyMemberComponent:BaseComponent
--- @field Party EntityHandle
--- @field UserID int32
--- @field field_20 Guid
--- @field field_30 uint8
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
--- @field field_18 Guid
--- @field field_8 Guid


--- @class PartyViewComponent:BaseComponent
--- @field Characters EntityHandle[]
--- @field Views PartyView[]
--- @field field_0 Guid


--- @class PartyWaypoint
--- @field Level FixedString
--- @field Name FixedString
--- @field field_8 Guid


--- @class PartyWaypointsComponent:BaseComponent
--- @field Waypoints Array_PartyWaypoint


--- @class RecruitRecruiterComponent:BaseComponent
--- @field Recruiters Array_EntityHandle


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


--- @class ResourceBackground:ResourceGuidResource
--- @field Description TranslatedString
--- @field DisplayName TranslatedString
--- @field Hidden boolean
--- @field Passives string
--- @field Tags Guid[]
--- @field field_40 int64
--- @field field_48 int64


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


--- @class ResourceCharacterVisualResource:ResourceLoadableResource
--- @field BaseVisual FixedString
--- @field BodySetVisual FixedString
--- @field ID FixedString
--- @field LocatorAttachments ResourceCharacterVisualResourceLocatorAttachment[]
--- @field MaterialOverrides ResourcePresetData
--- @field Materials table<FixedString, ResourcePresetData>
--- @field RealMaterialOverrides table<FixedString, FixedString>
--- @field ShowEquipmentVisuals boolean
--- @field Slots ResourceCharacterVisualResourceSlot[]
--- @field field_158 table<FixedString, FixedString>


--- @class ResourceCharacterVisualResourceLocatorAttachment
--- @field LocatorId FixedString
--- @field VisualResource FixedString


--- @class ResourceCharacterVisualResourceSlot
--- @field Bone FixedString
--- @field Slot FixedString
--- @field VisualResource FixedString


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


--- @class ResourceDialogResource:ResourceLoadableResource
--- @field AllowDeadSpeakers boolean
--- @field BlockFTB boolean
--- @field EnableTimeline boolean
--- @field IsAllowingJoinCombat boolean
--- @field Name string
--- @field SpeakerSlotsWithLines boolean[]
--- @field TimelineSplitScreenPriority uint8
--- @field automated boolean
--- @field childResources Guid[]
--- @field isBehaviour boolean
--- @field isPrivateDialog boolean
--- @field isSubbedDialog boolean
--- @field isWorld boolean


--- @class ResourceDiffusionProfileResource:ResourceLoadableResource
--- @field DualSpecularMix number
--- @field DualSpecularRoughnessA number
--- @field DualSpecularRoughnessB number
--- @field ScatterColor vec3
--- @field ScatterDistance number
--- @field ThickObjectTransmission boolean
--- @field ThicknessRemapMax number
--- @field ThicknessRemapMin number
--- @field TransmissionTint vec3


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


--- @class ResourceFlag:ResourceGuidResource
--- @field Description string
--- @field Name FixedString
--- @field Usage uint8


--- @class ResourceGenomeVariant
--- @field Type int64
--- @field Value int64


--- @class ResourceGod:ResourceGuidResource
--- @field Description TranslatedString
--- @field DisplayName TranslatedString
--- @field Name FixedString
--- @field Tags Guid[]


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


--- @class ResourceLevelMap:ResourceGuidResource
--- @field FallbackValue Variant<int32,ResourceLevelMapDiceRoll,>
--- @field LevelMaps Variant<int32,ResourceLevelMapDiceRoll,>[]
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


--- @class ResourceLoadableResource:ResourceResource
--- @field RefCount uint16
--- @field State uint8


--- @class ResourceMaterialPresetResource:ResourceLoadableResource
--- @field Presets ResourcePresetData


--- @class ResourceMaterialResource:ResourceTwoStepLoadableResource
--- @field DiffusionProfileUUID FixedString
--- @field MaterialType uint8
--- @field RenderChannel uint8
--- @field ScalarParameters ResourceMaterialResourceScalarParameter[]
--- @field Texture2DParameters ResourceMaterialResourceTexture2DParameter[]
--- @field Vector2Parameters ResourceMaterialResourceVector2Parameter[]
--- @field Vector3Parameters ResourceMaterialResourceVector3Parameter[]
--- @field VectorParameters ResourceMaterialResourceVectorParameter[]
--- @field VirtualTextureParameters ResourceMaterialResourceVirtualTextureParameter[]
--- @field field_48 int64


--- @class ResourceMaterialResourceScalarParameter
--- @field BaseValue number
--- @field Enabled boolean
--- @field ParameterName FixedString
--- @field Value number


--- @class ResourceMaterialResourceTexture2DParameter
--- @field Enabled boolean
--- @field ID FixedString
--- @field ParameterName FixedString


--- @class ResourceMaterialResourceVector2Parameter
--- @field BaseValue vec2
--- @field Enabled boolean
--- @field ParameterName FixedString
--- @field Value vec2


--- @class ResourceMaterialResourceVector3Parameter
--- @field BaseValue vec3
--- @field Enabled boolean
--- @field IsColor boolean
--- @field ParameterName FixedString
--- @field Value vec3


--- @class ResourceMaterialResourceVectorParameter
--- @field BaseValue vec4
--- @field Enabled boolean
--- @field IsColor boolean
--- @field ParameterName FixedString
--- @field Value vec4


--- @class ResourceMaterialResourceVirtualTextureParameter
--- @field Enabled boolean
--- @field ID FixedString
--- @field Index int32
--- @field Parameter FixedString


--- @class ResourceMaterialSetResource:ResourceTwoStepLoadableResource


--- @class ResourceMeshProxyResource:ResourceTwoStepLoadableResource
--- @field Template FixedString
--- @field field_48 int64


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


--- @class ResourcePassiveList:ResourceGuidResource
--- @field Passives FixedString[]


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


--- @class ResourceResource
--- @field Guid FixedString
--- @field SourceFile Path


--- @class ResourceScriptResource:ResourceLoadableResource
--- @field Parameters table<FixedString, ResourceScriptResourceParameter>


--- @class ResourceScriptResourceParameter
--- @field Type int32
--- @field Value string


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


--- @class ResourceSkillList:ResourceGuidResource
--- @field Skills SkillId[]


--- @class ResourceSkinPresetResource:ResourceLoadableResource
--- @field Presets ResourcePresetData


--- @class ResourceSoundResource:ResourceTwoStepLoadableResource
--- @field Duration number
--- @field Internal boolean
--- @field MaxDistance number
--- @field Preload boolean
--- @field SoundBank FixedString
--- @field SoundCategory int8
--- @field SoundCodec int8
--- @field SoundEvent FixedString
--- @field SoundEventID uint32
--- @field SoundEventUUID Guid


--- @class ResourceSpellList:ResourceGuidResource
--- @field Spells Array_FixedString


--- @class ResourceTag:ResourceGuidResource
--- @field Categories uint32
--- @field Description string
--- @field DisplayDescription TranslatedString
--- @field DisplayName TranslatedString
--- @field Icon FixedString
--- @field Name FixedString
--- @field Properties uint32


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
--- @field Type int32
--- @field Width int32
--- @field field_30 int64


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


--- @class ResourceTwoStepLoadableResource:ResourceResource
--- @field field_30 int64


--- @class ResourceVirtualTextureResource:ResourceLoadableResource
--- @field GTSHandle uint32
--- @field GTSSuffix string|nil
--- @field GTexFileName FixedString
--- @field LoadedField52 boolean
--- @field LoadedVirtualTextureLayerConfig uint32
--- @field Prefetch boolean
--- @field PrefetchMipLevel int8
--- @field ReferencedColorSpaces uint32
--- @field VirtualTextureLayerConfig uint32
--- @field field_52 uint8
--- @field field_53 uint8


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
--- @field ClosestVertices uint16[]
--- @field Name FixedString


--- @class ResourceVisualResourceObject
--- @field LOD uint8
--- @field MaterialID FixedString
--- @field ObjectID FixedString


--- @class ResourceVisualSetResource:ResourceLoadableResource


--- @class ResourceVoiceBarkResource:ResourceLoadableResource


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
--- @field HideVfxChannel ResourceEffectsTimelineChannelsEventKeyChannel_bool
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
--- @field field_b0 ResourceEffectsTimelineChannelsEventKeyChannel_bool


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


--- @class ResourceEffectsTimelineTLEventKeyComponentComponent<ResourceEffectsTimelineKeysValueKey_bool>:ResourceEffectsTimelineTLKeyBaseComponentComponent<ResourceEffectsTimelineKeysValueKey_bool>


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


--- @class ResourceEffectsTimelineTLKeyBaseComponentComponent<ResourceEffectsTimelineKeysValueKey_bool>:ResourceEffectsTimelineTLBaseComponentComponent
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
--- @field VisibilityChannel ResourceEffectsTimelineChannelsEventKeyChannel_bool
--- @field field_15a int16


--- @class ResourceEffectsTimelineTLMaterialComponentMaterialParameter:ResourceEffectsTimelineTLMaterialComponentParameter
--- @field PropertyParameters ResourceEffectsTimelineChannelsTLMaterialKeyComponentComponent[]


--- @class ResourceEffectsTimelineTLMaterialComponentParameter
--- @field MaterialParameterName FixedString
--- @field field_10 int64
--- @field field_c int32


--- @class ResourceEffectsTimelineTLMaterialComponentParameterRange
--- @field begin ResourceEffectsEffectComponentBase
--- @field end ResourceEffectsEffectComponentBase


--- @class ResourceEffectsTimelineTLMaterialComponentTextureParameter:ResourceEffectsTimelineTLMaterialComponentParameter
--- @field IsVirtual boolean
--- @field PropertyParameters ResourceEffectsTimelineChannelsTLMaterialTextureKeyComponentComponent[]
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
--- @field PropertyParameters ResourceEffectsTimelineChannelsTimelineActorPropertiesReflectionKeyComponentComponent[]
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


--- @class ResourceEffectsTimelineChannelsEventKeyChannel_bool:ResourceEffectsTimelineTLEventKeyComponentComponent<ResourceEffectsTimelineKeysValueKey_bool>
--- @field defaultVal boolean


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
--- @field HeadSafeZoneAngle number|nil
--- @field HeadTurnSpeedMultiplier number
--- @field IsEyeLookAtEnabled boolean
--- @field LookAtInterpMode uint8
--- @field LookAtMode uint8
--- @field Offset vec3
--- @field Reset boolean
--- @field SaveZoneAngle number|nil
--- @field Target Guid
--- @field TorsoTurnSpeedMultiplier number
--- @field TrackingMode uint8
--- @field TurnMode uint8
--- @field TurnSpeedMultiplier number
--- @field Weight number|nil


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
--- @field data Variant<ResourceEffectsTimelineKeysSoundEventKeyType1,ResourceEffectsTimelineKeysSoundEventKeyType2,ResourceEffectsTimelineKeysSoundEventKeyType2,ResourceEffectsTimelineKeysSoundEventKeyType2,ResourceEffectsTimelineKeysSoundEventKeyType4,ResourceEffectsTimelineKeysSoundEventKeyType5,>


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


--- @class ResourceEffectsTimelineKeysValueKey_bool:ResourceEffectsTimelineKeysKeyBase
--- @field Value boolean


--- @class ResourceEffectsTimelineKeysValueKey_float:ResourceEffectsTimelineKeysKeyBase
--- @field Value number


--- @class SightDataComponent:BaseComponent
--- @field field_0 Guid
--- @field field_10 int32
--- @field field_14 int32
--- @field field_18 int32
--- @field field_1C int32
--- @field field_20 int32
--- @field field_24 int32


--- @class SightEntityViewshedComponent:BaseComponent
--- @field field_0 Array_Guid


--- @class SightSightBaseComponent:BaseComponent
--- @field field_0 int32
--- @field field_4 int32
--- @field field_8 int32
--- @field field_C int32


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
--- @field Modification Variant<SpellModificationModifyAreaRadius,SpellModificationModifyMaximumTargets,SpellModificationModifyNumberOfTargets,SpellModificationModifySavingThrowDisadvantage,SpellModificationModifySpellFlags,SpellModificationModifySpellRoll,SpellModificationModifyStatusDuration,SpellModificationModifySummonDuration,SpellModificationModifySurfaceDuration,SpellModificationModifyTargetRadius,SpellModificationModifyUseCosts,SpellModificationModifyVisuals,SpellModificationModifyIconGlow,SpellModificationModifyTooltipDescription,>
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
--- @field CooldownType SpellCooldownType
--- @field Id SpellId
--- @field InnerEntries SpellSpellBookEntryInnerEntry[]
--- @field SpellCastingAbility AbilityId
--- @field SpellUUID Guid
--- @field field_38 int32
--- @field field_3C int32
--- @field field_41 uint8
--- @field field_42 uint8


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
--- @field field_8 EntityHandle|nil
--- @field field_80 int64
--- @field field_88 uint8


--- @class SpellCastRollsComponentRollHit
--- @field Hit Hit
--- @field field_0 FixedString


--- @class SpellCastStateComponent:BaseComponent
--- @field Caster EntityHandle
--- @field CasterMoveToPosition vec3|nil
--- @field CasterStartPosition vec3
--- @field Entity EntityHandle
--- @field SpellId SpellId
--- @field Targets SpellCastTargetInfo2[]
--- @field field_38 int32
--- @field field_60 vec3|nil
--- @field field_80 EntityHandle
--- @field field_88 int32
--- @field field_90 Guid
--- @field field_A0 string


--- @class SpellCastSyncTargetingComponent:BaseComponent
--- @field CasterMoveToPosition vec3|nil
--- @field CasterPosition vec3|nil
--- @field Targets SpellCastTargetInfo2[]
--- @field field_10 EntityHandle|nil
--- @field field_20 vec3|nil
--- @field field_40 uint8
--- @field field_44 int32
--- @field field_48 vec3|nil
--- @field field_58 vec3|nil
--- @field field_8 EntityHandle
--- @field field_88 vec3|nil


--- @class SpellCastTargetInfo
--- @field Position vec3|nil
--- @field Target EntityHandle
--- @field field_10 EntityHandle|nil


--- @class SpellCastTargetInfo2
--- @field SpellType SpellType
--- @field Target SpellCastTargetInfo
--- @field Target2 SpellCastTargetInfo|nil


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
--- @field ResolveData InterruptResolveData|nil
--- @field Source EntityRef
--- @field SourceProxy EntityRef
--- @field Target EntityRef
--- @field TargetProxy EntityRef


--- @class StatsFunctors
--- @field NextFunctorIndex int32
--- @field UniqueName FixedString


--- @class StatsModifier
--- @field EnumerationIndex int32
--- @field LevelMapIndex int32
--- @field Name FixedString


--- @class StatsModifierList
--- @field Attributes StatsCNamedElementManager_StatsModifier
--- @field Name FixedString


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


--- @class StatsSpellPrototypeUseCostGroup
--- @field Amount number
--- @field ResourceGroup Guid
--- @field Resources Guid[]
--- @field SubResourceId int32


--- @class StatsStabilizeFunctor:StatsFunctor


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


--- @class StatsTreasureTable


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




--- @class Character:StatsObject
--- @field Level string
--- @field Strength string
--- @field Dexterity string
--- @field Constitution string
--- @field Intelligence string
--- @field Wisdom string
--- @field Charisma string
--- @field Armor string
--- @field Vitality string
--- @field XPReward integer
--- @field Sight string
--- @field Hearing string
--- @field FOV string
--- @field VerticalFOV string
--- @field Weight HitAnimationType
--- @field GameSize StatsConditions
--- @field SoundSize StatsConditions
--- @field StepsType SpellSchool
--- @field ExtraProperties StatsConditions
--- @field Flags Itemslot
--- @field DefaultBoosts StatsConditions
--- @field PersonalStatusImmunities SpellActionType
--- @field PathInfluence StatsConditions
--- @field Progression Type Surface Type
--- @field ProficiencyBonusScaling integer
--- @field ProficiencyBonus string
--- @field SpellCastingAbility Weapon Group
--- @field UnarmedAttackAbility Weapon Group
--- @field UnarmedRangedAttackAbility Weapon Group
--- @field ActionResources StatsConditions
--- @field Class StatsConditions
--- @field Passives StatsConditions
--- @field BludgeoningResistance ObjectSize
--- @field SlashingResistance ObjectSize
--- @field PiercingResistance ObjectSize
--- @field AcidResistance ObjectSize
--- @field ColdResistance ObjectSize
--- @field FireResistance ObjectSize
--- @field ForceResistance ObjectSize
--- @field LightningResistance ObjectSize
--- @field NecroticResistance ObjectSize
--- @field PoisonResistance ObjectSize
--- @field PsychicResistance ObjectSize
--- @field RadiantResistance ObjectSize
--- @field ThunderResistance ObjectSize
--- @field Initiative string
--- @field Progressions StatsConditions
--- @field MinimumDetectionRange StatsConditions
--- @field DarkvisionRange StatsConditions
--- @field FallingHitEffect StatsConditions
--- @field FallingLandEffect StatsConditions
--- @field ArmorType AttributeFlags
--- @field Proficiency Group DisturbanceInvestigationKind
--- @field DynamicAnimationTag integer
--- @field DifficultyStatuses StatsConditions


--- @class Armor:StatsObject
--- @field RootTemplate StatsConditions
--- @field ItemGroup StatsConditions
--- @field Level string
--- @field Slot Act
--- @field Requirements string
--- @field UseConditions MemorizationRequirements
--- @field UseCosts StatsConditions
--- @field ArmorClass string
--- @field Durability string
--- @field DurabilityDegradeSpeed Damage Type
--- @field ValueLevel string
--- @field ValueUUID integer
--- @field ValueScale HitAnimationType
--- @field ValueRounding string
--- @field ValueOverride string
--- @field Rarity SpellAnimationIntentType
--- @field Weight HitAnimationType
--- @field GameSize StatsConditions
--- @field SoundSize StatsConditions
--- @field Spells StatsConditions
--- @field Tags StatsConditions
--- @field ExtraProperties StatsConditions
--- @field Flags Itemslot
--- @field DefaultBoosts StatsConditions
--- @field PersonalStatusImmunities SpellActionType
--- @field Boosts StatsConditions
--- @field PassivesOnEquip StatsConditions
--- @field StatusOnEquip StatsConditions
--- @field ComboCategory StatsConditions
--- @field InventoryTab StepsType
--- @field ArmorType AttributeFlags
--- @field ItemColor StatsConditions
--- @field NeedsIdentification FormatStringColor
--- @field Charges string
--- @field MaxCharges string
--- @field ObjectCategory StatsConditions
--- @field MinAmount string
--- @field MaxAmount string
--- @field Priority string
--- @field Unique string
--- @field MinLevel string
--- @field MaxLevel string
--- @field Shield FormatStringColor
--- @field Armor Class Ability Weapon Group
--- @field Ability Modifier Cap string
--- @field FallingHitEffect StatsConditions
--- @field FallingLandEffect StatsConditions
--- @field ColorPresetResource StatsConditions
--- @field Proficiency Group DisturbanceInvestigationKind
--- @field InstrumentType RestErrorFlags
--- @field StatusInInventory StatsConditions


--- @class Object:StatsObject
--- @field RootTemplate StatsConditions
--- @field Level string
--- @field ValueLevel string
--- @field ValueUUID integer
--- @field ValueScale HitAnimationType
--- @field ValueRounding string
--- @field ValueOverride string
--- @field Rarity SpellAnimationIntentType
--- @field Weight HitAnimationType
--- @field GameSize StatsConditions
--- @field SoundSize StatsConditions
--- @field ComboCategory StatsConditions
--- @field Requirements string
--- @field Vitality string
--- @field Armor string
--- @field Flags Itemslot
--- @field DefaultBoosts StatsConditions
--- @field PersonalStatusImmunities SpellActionType
--- @field InventoryTab StepsType
--- @field AddToBottomBar FormatStringColor
--- @field IgnoredByAI FormatStringColor
--- @field ObjectCategory StatsConditions
--- @field MinAmount string
--- @field MaxAmount string
--- @field Priority string
--- @field Unique string
--- @field MinLevel string
--- @field MaxLevel string
--- @field UseCosts StatsConditions
--- @field ItemUseType Tension
--- @field BludgeoningResistance ObjectSize
--- @field SlashingResistance ObjectSize
--- @field PiercingResistance ObjectSize
--- @field AcidResistance ObjectSize
--- @field ColdResistance ObjectSize
--- @field FireResistance ObjectSize
--- @field ForceResistance ObjectSize
--- @field LightningResistance ObjectSize
--- @field NecroticResistance ObjectSize
--- @field PoisonResistance ObjectSize
--- @field PsychicResistance ObjectSize
--- @field RadiantResistance ObjectSize
--- @field ThunderResistance ObjectSize
--- @field UseConditions MemorizationRequirements
--- @field FallingHitEffect StatsConditions
--- @field FallingLandEffect StatsConditions
--- @field SupplyValue string
--- @field Sight string
--- @field FOV string
--- @field VerticalFOV string
--- @field DarkvisionRange string
--- @field MinimumDetectionRange string
--- @field PassivesOnEquip StatsConditions
--- @field StatusInInventory StatsConditions


--- @class Weapon:StatsObject
--- @field RootTemplate StatsConditions
--- @field ItemGroup StatsConditions
--- @field Level string
--- @field Requirements string
--- @field UseConditions MemorizationRequirements
--- @field UseCosts StatsConditions
--- @field Damage Type Spell Attack Type
--- @field Damage StatsConditions
--- @field Damage Range string
--- @field WeaponRange string
--- @field Durability string
--- @field DurabilityDegradeSpeed Damage Type
--- @field ValueLevel string
--- @field ValueUUID integer
--- @field ValueScale HitAnimationType
--- @field ValueRounding string
--- @field ValueOverride string
--- @field Rarity SpellAnimationIntentType
--- @field Weight HitAnimationType
--- @field GameSize StatsConditions
--- @field SoundSize StatsConditions
--- @field Slot Act
--- @field Projectile StatsConditions
--- @field IgnoreVisionBlock FormatStringColor
--- @field ComboCategory StatsConditions
--- @field Spells StatsConditions
--- @field Tags StatsConditions
--- @field ExtraProperties StatsConditions
--- @field WeaponFunctors AIFlags
--- @field Flags Itemslot
--- @field DefaultBoosts StatsConditions
--- @field PersonalStatusImmunities SpellActionType
--- @field Boosts StatsConditions
--- @field BoostsOnEquipMainHand StatsConditions
--- @field BoostsOnEquipOffHand StatsConditions
--- @field PassivesOnEquip StatsConditions
--- @field PassivesMainHand StatsConditions
--- @field PassivesOffHand StatsConditions
--- @field StatusOnEquip StatsConditions
--- @field InventoryTab StepsType
--- @field NeedsIdentification FormatStringColor
--- @field Charges string
--- @field MaxCharges string
--- @field ItemColor StatsConditions
--- @field ObjectCategory StatsConditions
--- @field MinAmount string
--- @field MaxAmount string
--- @field Priority string
--- @field Unique string
--- @field MinLevel string
--- @field MaxLevel string
--- @field Weapon Group Progression Type
--- @field VersatileDamage StatsConditions
--- @field Weapon Properties CastCheckType
--- @field FallingHitEffect StatsConditions
--- @field FallingLandEffect StatsConditions
--- @field ColorPresetResource StatsConditions
--- @field Proficiency Group DisturbanceInvestigationKind
--- @field SupplyValue string
--- @field UniqueWeaponSoundSwitch StatsConditions
--- @field StatusInInventory StatsConditions


--- @class SpellData:StatsObject
--- @field SpellType StatsConditions
--- @field Level string
--- @field SpellSchool PickingState
--- @field SpellContainerID StatsConditions
--- @field Requirement AtmosphereType
--- @field ContainerSpells StatsConditions
--- @field ConcentrationSpellID StatsConditions
--- @field Cooldown StillAnimPriority
--- @field Damage StatsConditions
--- @field AIFlags StatsRollConditions
--- @field Damage Range string
--- @field SpellProperties AIFlags
--- @field TargetCeiling StatsConditions
--- @field SurfaceType Custom Properties
--- @field TargetFloor StatsConditions
--- @field SurfaceLifetime string
--- @field TargetRadius StatsConditions
--- @field SurfaceGrowStep string
--- @field ThrowOrigin StatusSheathing
--- @field AreaRadius string
--- @field SurfaceGrowInterval string
--- @field AddRangeFromAbility StatsConditions
--- @field DeathType YesNo
--- @field StrikeCount string
--- @field MaxDistance string
--- @field ExplodeRadius string
--- @field Acceleration string
--- @field AmountOfTargets StatsConditions
--- @field MaxAttacks string
--- @field MovementSpeed string
--- @field TeleportDelay string
--- @field SpellRoll WeaponFlags
--- @field NextAttackChance string
--- @field SpellSuccess AIFlags
--- @field NextAttackChanceDivider string
--- @field ThrowableSpellProperties AIFlags
--- @field SpellFail AIFlags
--- @field EndPosRadius string
--- @field OverrideSpellLevel FormatStringColor
--- @field ThrowableSpellRoll WeaponFlags
--- @field TargetConditions VerbalIntent
--- @field TargetProjectiles FormatStringColor
--- @field JumpDelay string
--- @field ThrowableSpellSuccess AIFlags
--- @field ProjectileCount StatsConditions
--- @field HitRadius HitAnimationType
--- @field ThrowableSpellFail AIFlags
--- @field ProjectileDelay string
--- @field AoEConditions VerbalIntent
--- @field MemorizationRequirements Properties
--- @field Lifetime string
--- @field OriginSpellProperties AIFlags
--- @field ThrowableTargetConditions VerbalIntent
--- @field CastTargetHitDelay string
--- @field MinHitsPerTurn string
--- @field OriginSpellRoll WeaponFlags
--- @field Angle string
--- @field Height string
--- @field MaxHitsPerTurn string
--- @field OriginSpellSuccess AIFlags
--- @field Trajectories StatsConditions
--- @field SingleSource FormatStringColor
--- @field HitDelay string
--- @field OriginSpellFail AIFlags
--- @field Icon StatsConditions
--- @field ForceTarget string
--- @field OriginTargetConditions VerbalIntent
--- @field DisplayName AuraFlags
--- @field DisplayNameRef AuraFlags
--- @field Template StatsConditions
--- @field UseWeaponDamage FormatStringColor
--- @field TeleportSelf FormatStringColor
--- @field Description AuraFlags
--- @field DescriptionRef AuraFlags
--- @field UseWeaponProperties FormatStringColor
--- @field TeleportSurface FormatStringColor
--- @field DescriptionParams StatsConditions
--- @field ExtraDescription AuraFlags
--- @field ExtraDescriptionRef AuraFlags
--- @field ExtraDescriptionParams StatsConditions
--- @field ShortDescription AuraFlags
--- @field ShortDescriptionRef AuraFlags
--- @field ShortDescriptionParams StatsConditions
--- @field TooltipDamageList StatsConditions
--- @field TooltipAttackSave StatsConditions
--- @field TooltipStatusApply StatsConditions
--- @field TooltipOnMiss StatsConditions
--- @field PrepareEffectBone StatsConditions
--- @field TooltipOnSave StatsConditions
--- @field TooltipUpcastDescription integer
--- @field StormEffect StatsConditions
--- @field TooltipUpcastDescriptionParams StatsConditions
--- @field FXScale string
--- @field TooltipPermanentWarnings StatsConditions
--- @field MaleImpactEffects StatsConditions
--- @field PrepareSound StatsConditions
--- @field FemaleImpactEffects StatsConditions
--- @field Spellbook StatsConditions
--- @field SpawnEffect StatsConditions
--- @field PrepareLoopSound StatsConditions
--- @field ReappearEffectTextEvent StatsConditions
--- @field CastSound StatsConditions
--- @field TargetSound StatsConditions
--- @field Memory Cost string
--- @field ProjectileSpells StatsConditions
--- @field VocalComponentSound StatsConditions
--- @field Distribution ProjectileType
--- @field Magic Cost string
--- @field PreviewCursor StatusAnimationType
--- @field Shuffle FormatStringColor
--- @field CastEffectTextEvent StatsConditions
--- @field Stealth FormatStringColor
--- @field CastTextEvent StatsConditions
--- @field PreviewStrikeHits FormatStringColor
--- @field Shape StatsConditions
--- @field AlternativeCastTextEvents StatsConditions
--- @field FrontOffset string
--- @field InstrumentComponentPrepareSound StatsConditions
--- @field InstrumentComponentLoopingSound StatsConditions
--- @field InstrumentComponentCastSound StatsConditions
--- @field Range string
--- @field InstrumentComponentImpactSound StatsConditions
--- @field Base string
--- @field ProjectileTerrainOffset FormatStringColor
--- @field WeaponBones StatsConditions
--- @field MovingObjectSummonTemplate StatsConditions
--- @field Autocast FormatStringColor
--- @field ProjectileType DamageSourceType
--- @field AiCalculationSpellOverride StatsConditions
--- @field SurfaceRadius string
--- @field CycleConditions VerbalIntent
--- @field ItemWall StatsConditions
--- @field UseCosts StatsConditions
--- @field ItemWallStatus StatsConditions
--- @field DualWieldingUseCosts StatsConditions
--- @field WallStartEffect StatsConditions
--- @field HitCosts StatsConditions
--- @field RitualCosts StatsConditions
--- @field WallEndEffect StatsConditions
--- @field SpellAnimation StatsConditions
--- @field DualWieldingSpellAnimation StatsConditions
--- @field VerbalIntent ResistanceFlags
--- @field SpellStyleGroup SpellCategoryFlags
--- @field WeaponTypes CastCheckType
--- @field SpellFlags Conditions
--- @field SpellActionType CursorMode
--- @field SpellActionTypePriority string
--- @field SpellCategory DisturbanceDialogueCapability
--- @field HitAnimationType SpellJumpType
--- @field SpellAnimationIntentType number
--- @field MaximumTotalTargetHP string
--- @field SpellJumpType LineOfSightFlags
--- @field SpellAnimationType FlagType
--- @field MinJumpDistance HitAnimationType
--- @field RechargeValues StatsConditions
--- @field Requirements string
--- @field ForkChance string
--- @field MaxForkCount string
--- @field ForkLevels string
--- @field ForkingConditions VerbalIntent
--- @field MemoryCost string
--- @field LineOfSightFlags SoundVocalType
--- @field RootSpellID StatsConditions
--- @field PowerLevel string
--- @field SourceLimbIndex string
--- @field RequirementConditions VerbalIntent
--- @field RequirementEvents ProjectileDistribution
--- @field PrepareEffect StatsConditions
--- @field CastEffect StatsConditions
--- @field TargetEffect StatsConditions
--- @field PreviewEffect StatsConditions
--- @field HitEffect StatsConditions
--- @field StopAtFirstContact string
--- @field TargetHitEffect StatsConditions
--- @field HitExtension HitAnimationType
--- @field TargetGroundEffect StatsConditions
--- @field OnlyHit1Target string
--- @field PositionEffect StatsConditions
--- @field BeamEffect StatsConditions
--- @field SpellEffect StatsConditions
--- @field SelectedCharacterEffect StatsConditions
--- @field IgnoreTeleport FormatStringColor
--- @field SelectedObjectEffect StatsConditions
--- @field CinematicArenaFlags SpellStyleGroup
--- @field SelectedPositionEffect StatsConditions
--- @field DisappearEffect StatsConditions
--- @field ReappearEffect StatsConditions
--- @field ImpactEffect StatsConditions
--- @field DamageType Spell Attack Type
--- @field CinematicArenaTimelineOverride integer
--- @field DelayTurnsCount string
--- @field DelayRollTarget string
--- @field DelayRollDie InstrumentType
--- @field SpellSoundMagnitude DieType
--- @field SpellSoundAftermathTrajectory StatsConditions
--- @field MaximumTargets string
--- @field HighlightConditions VerbalIntent
--- @field Sheathing SpellSoundMagnitude
--- @field SteerSpeedMultipler HitAnimationType
--- @field InterruptPrototype StatsConditions
--- @field CombatAIOverrideSpell StatsConditions
--- @field FollowUpOriginalSpell StatsConditions
--- @field TooltipSpellDCAbilities InterruptFlagsList


--- @class StatusData:StatsObject
--- @field StatusType StatsConditions
--- @field DisplayName AuraFlags
--- @field DisplayNameRef AuraFlags
--- @field Description AuraFlags
--- @field DescriptionRef AuraFlags
--- @field DescriptionParams StatsConditions
--- @field Icon StatsConditions
--- @field FormatColor Penalty PreciseQualifier
--- @field StatusEffectOverrideForItems StatsConditions
--- @field StatusEffectOverride StatsConditions
--- @field MaterialType Qualifier
--- @field Material StatsConditions
--- @field MaterialApplyBody FormatStringColor
--- @field MaterialApplyArmor FormatStringColor
--- @field MaterialApplyWeapon FormatStringColor
--- @field PlayerSameParty FormatStringColor
--- @field MaterialApplyNormalMap FormatStringColor
--- @field PlayerHasTag StatsConditions
--- @field MaterialFadeAmount string
--- @field PeaceOnly FormatStringColor
--- @field MaterialOverlayOffset string
--- @field MaterialParameters StatsConditions
--- @field AnimationStart StatsConditions
--- @field MeshEffect StatsConditions
--- @field AnimationLoop StatsConditions
--- @field StillAnimationType StatusPropertyFlags
--- @field AnimationEnd StatsConditions
--- @field StillAnimationPriority StatsFunctorContext
--- @field HitAnimationType SpellJumpType
--- @field UseLyingPickingState FormatStringColor
--- @field SoundVocalStart Relation
--- @field StatsId StatsConditions
--- @field SoundVocalLoop Relation
--- @field SoundStart StatsConditions
--- @field SoundLoop StatsConditions
--- @field SoundStop StatsConditions
--- @field SoundVocalEnd Relation
--- @field StatusSoundState StatsConditions
--- @field ImmuneFlag Itemslot
--- @field HealMultiplier string
--- @field OnApplyConditions MemorizationRequirements
--- @field StackId StatsConditions
--- @field HealStat StatusStackType
--- @field PolymorphResult StatsConditions
--- @field StackPriority string
--- @field HealType Action Type
--- @field DisableInteractions FormatStringColor
--- @field Instant FormatStringColor
--- @field Projectile StatsConditions
--- @field StackType ModifierType
--- @field HealValue StatsConditions
--- @field AiCalculationSpellOverride StatsConditions
--- @field Radius string
--- @field AuraRadius string
--- @field AbsorbSurfaceType StatsConditions
--- @field FreezeTime string
--- @field Charges string
--- @field AuraStatuses AIFlags
--- @field AbsorbSurfaceRange string
--- @field RetainSpells StatsConditions
--- @field DefendTargetPosition FormatStringColor
--- @field AuraFlags InterruptDefaultValue
--- @field BonusFromSkill ArmorType
--- @field TargetConditions StatsConditions
--- @field AuraFX StatsConditions
--- @field HealEffectId StatsConditions
--- @field SurfaceChange StatsConditions
--- @field Necromantic FormatStringColor
--- @field Spells StatsConditions
--- @field Items StatsConditions
--- @field WeaponOverride StatsConditions
--- @field ResetCooldowns StatsConditions
--- @field LeaveAction StatsConditions
--- @field DieAction StatsConditions
--- @field ForceStackOverwrite FormatStringColor
--- @field Toggle FormatStringColor
--- @field TickType StatsFunctors
--- @field Boosts StatsConditions
--- @field Passives StatsConditions
--- @field RemoveConditions MemorizationRequirements
--- @field RemoveEvents ProjectileDistribution
--- @field TooltipSave StatsConditions
--- @field StableRoll StatsConditions
--- @field TooltipDamage StatsConditions
--- @field StableRollDC string
--- @field TickFunctors AIFlags
--- @field TemplateID StatsConditions
--- @field NumStableSuccess string
--- @field StatusPropertyFlags CooldownType
--- @field Rules integer
--- @field NumStableFailed string
--- @field OnApplyFunctors AIFlags
--- @field OnSuccess AIFlags
--- @field OnRemoveFunctors AIFlags
--- @field OnRollsFailed AIFlags
--- @field OnApplyRoll MemorizationRequirements
--- @field OnApplySuccess AIFlags
--- @field OnApplyFail AIFlags
--- @field OnTickRoll MemorizationRequirements
--- @field OnTickSuccess AIFlags
--- @field OnTickFail AIFlags
--- @field OnRemoveRoll MemorizationRequirements
--- @field OnRemoveSuccess AIFlags
--- @field OnRemoveFail AIFlags
--- @field LEDEffect SpellAnimationType
--- @field StatusGroups ProficiencyGroupFlags
--- @field IsUnique string
--- @field ApplyEffect StatsConditions
--- @field HideOverheadUI string
--- @field StatusEffect StatsConditions
--- @field StatusEffectOnTurn StatsConditions
--- @field TargetEffect StatsConditions
--- @field BeamEffect StatsConditions
--- @field SplatterDirtAmount HitAnimationType
--- @field SplatterBloodAmount HitAnimationType
--- @field SplatterSweatAmount HitAnimationType
--- @field ManagedStatusEffectType CinematicArenaFlags
--- @field ManagedStatusEffectGroup StatsConditions
--- @field EndEffect StatsConditions
--- @field TooltipPermanentWarnings StatsConditions
--- @field PerformEventName StatsConditions
--- @field DynamicAnimationTag integer
--- @field Sheathing InterruptContextScope


--- @class PassiveData:StatsObject
--- @field DisplayName AuraFlags
--- @field DisplayNameRef AuraFlags
--- @field Description AuraFlags
--- @field DescriptionRef AuraFlags
--- @field DescriptionParams StatsConditions
--- @field ExtraDescription AuraFlags
--- @field ExtraDescriptionRef AuraFlags
--- @field ExtraDescriptionParams StatsConditions
--- @field LoreDescription AuraFlags
--- @field LoreDescriptionRef AuraFlags
--- @field TooltipUseCosts StatsConditions
--- @field TooltipSave StatsConditions
--- @field TooltipConditionalDamage StatsConditions
--- @field TooltipPermanentWarnings StatsConditions
--- @field Icon StatsConditions
--- @field PriorityOrder string
--- @field EnabledConditions MemorizationRequirements
--- @field EnabledContext PassiveFlags
--- @field Properties SpellFlagList
--- @field BoostContext PassiveFlags
--- @field BoostConditions MemorizationRequirements
--- @field Boosts StatsConditions
--- @field StatsFunctorContext PassiveFlags
--- @field Conditions MemorizationRequirements
--- @field StatsFunctors AIFlags
--- @field ToggleOnFunctors AIFlags
--- @field ToggleOffFunctors AIFlags
--- @field ToggleOnEffect StatsConditions
--- @field ToggleOffEffect StatsConditions
--- @field ToggleOffContext PassiveFlags
--- @field ToggleGroup StatsConditions
--- @field DynamicAnimationTag integer


--- @class InterruptData:StatsObject
--- @field DisplayName AuraFlags
--- @field DisplayNameRef AuraFlags
--- @field Description AuraFlags
--- @field DescriptionRef AuraFlags
--- @field DescriptionParams StatsConditions
--- @field ExtraDescription AuraFlags
--- @field ExtraDescriptionRef AuraFlags
--- @field ExtraDescriptionParams StatsConditions
--- @field LoreDescription AuraFlags
--- @field LoreDescriptionRef AuraFlags
--- @field Icon StatsConditions
--- @field InterruptContext AlchemyCombinationType
--- @field InterruptContextScope InterruptContext
--- @field Container StatsConditions
--- @field Conditions MemorizationRequirements
--- @field Properties AIFlags
--- @field Roll MemorizationRequirements
--- @field Success AIFlags
--- @field Failure AIFlags
--- @field Cost StatsConditions
--- @field Stack StatsConditions
--- @field Cooldown StillAnimPriority
--- @field InterruptDefaultValue ThrowOrigin
--- @field EnableCondition MemorizationRequirements
--- @field EnableContext PassiveFlags
--- @field TooltipDamageList StatsConditions
--- @field TooltipAttackSave StatsConditions
--- @field TooltipStatusApply StatsConditions
--- @field ShortDescription AuraFlags
--- @field ShortDescriptionRef AuraFlags
--- @field ShortDescriptionParams StatsConditions
--- @field TooltipOnMiss StatsConditions
--- @field TooltipOnSave StatsConditions
--- @field TooltipPermanentWarnings StatsConditions
--- @field InterruptFlags string


--- @class CriticalHitTypeData:StatsObject
--- @field AcidFX integer
--- @field BludgeoningFX integer
--- @field ColdFX integer
--- @field FireFX integer
--- @field ForceFX integer
--- @field LightningFX integer
--- @field NecroticFX integer
--- @field PiercingFX integer
--- @field PoisonFX integer
--- @field PsychicFX integer
--- @field RadiantFX integer
--- @field SlashingFX integer
--- @field ThunderFX integer




--- @class Ext_ClientNet
--- @field PostMessageToServer fun(a1:string, a2:string)
local Ext_ClientNet = {}



--- @class Ext_Debug
--- @field Crash fun(a1:int32)
--- @field DebugBreak fun()
--- @field DebugDumpLifetimes fun()
--- @field DumpStack fun()
--- @field GenerateIdeHelpers fun()
--- @field IsDeveloperMode fun():boolean
--- @field SetEntityRuntimeCheckLevel fun(a1:int32)
local Ext_Debug = {}



--- @class Ext_Entity
--- @field Get fun(a1:Guid)
--- @field GetAllEntities fun():EntityHandle[]
--- @field GetAllEntitiesWithComponent fun(a1:ExtComponentType):EntityHandle[]
--- @field GetAllEntitiesWithUuid fun():table<Guid, EntityHandle>
--- @field HandleToUuid fun(a1:EntityHandle):Guid|nil
--- @field Subscribe fun(a1:ExtComponentType)
--- @field Unsubscribe fun(a1:uint32):boolean
--- @field UuidToHandle fun(a1:Guid):EntityHandle
local Ext_Entity = {}



--- @class Ext_IO
--- @field AddPathOverride fun(a1:string, a2:string)
--- @field GetPathOverride fun(a1:string)
--- @field LoadFile fun(a1:string, a2:FixedString|nil)
--- @field SaveFile fun(a1:string, a2:string):boolean
local Ext_IO = {}



--- @class Ext_Json
--- @field Parse fun()
--- @field Stringify fun()
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
--- @field GetTranslatedString fun(a1:FixedString):string
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
--- @field BroadcastMessage fun(a1:string, a2:string)
--- @field PlayerHasExtender fun(a1:string)
--- @field PostMessageToClient fun(a1:string, a2:string, a3:string)
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
--- @field Create fun(a1:FixedString, a2:FixedString, a3:FixedString|nil)
--- @field EnumIndexToLabel fun(a1:FixedString, a2:int32):FixedString|nil
--- @field EnumLabelToIndex fun(a1:FixedString, a2:FixedString)
--- @field Get fun(a1:string, a2:int32|nil)
--- @field GetModifierAttributes fun(a1:FixedString)
--- @field GetStats fun(a1:FixedString|nil):FixedString[]
--- @field GetStatsLoadedBefore fun(a1:FixedString, a2:FixedString|nil):FixedString[]
--- @field GetStatsManager fun():StatsRPGStats
--- @field SetPersistence fun(a1:FixedString, a2:boolean)
--- @field Sync fun(a1:FixedString)
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



--- @class Ext_Types
--- @field Construct fun(a1:FixedString)
--- @field GetAllTypes fun():FixedString[]
--- @field GetObjectType fun()
--- @field GetTypeInfo fun(a1:FixedString):TypeInformation
--- @field Serialize fun()
--- @field Unserialize fun()
--- @field Validate fun():boolean
local Ext_Types = {}
--- @class GenerateIdeHelpersOptions
--- @field AddOsiris boolean Add all Osiris functions to the global Osi table. This is optional, due to the possible performance cost of having so many functions.
--- @field AddDeprecated boolean Add deprecated functions to the helpers file.
--- @field AddAliasEnums boolean Add the enums in alias format, for string comparison. Defaults to true.
--- @field UseBaseExtraData boolean Only include the base ExtraData keys/values in Shared, instead of grabbing whatever the current keys are in the mod environment.
--- @field GenerateExtraDataAsClass boolean Annotate ExtraData as a class, so it only has fields with no fixed/hardcoded values.

--- Generate an ExtIdeHelpers file  
--- @param outputPath string|nil Optional path to save the generated helper file, relative to the `Documents\Larian Studios\Divinity Original Sin 2 Definitive Edition\Osiris Data` folder  
--- @param addOsi boolean|nil If true, all Osiris functions will be included in the Osi global table. This is optional, due to the possible performance cost of having so many functions  
--- @return string fileContents Returns the file contents, for use with Ext.IO.SaveFile
function Ext_Types.GenerateIdeHelpers(outputPath, addOsi) end




--- @class Ext_Utils
--- @field GameVersion fun()
--- @field GetCommandLineParams fun():string[]
--- @field GetGlobalSwitches fun():GlobalSwitches
--- @field GetValueType fun():string
--- @field HandleToInteger fun(a1:EntityHandle):int64
--- @field Include fun()
--- @field IntegerToHandle fun(a1:int64):EntityHandle
--- @field IsValidHandle fun():boolean
--- @field LoadString fun(str:string):UserReturn Similar to lua `loadstring`, with extra safeguards.
--- @field MicrosecTime fun():number
--- @field MonotonicTime fun():int64
--- @field Print fun()
--- @field PrintError fun()
--- @field PrintWarning fun()
--- @field ShowError fun(a1:string)
--- @field ShowErrorAndExitGame fun(a1:string)
--- @field Version fun():int32
local Ext_Utils = {}



--- @class Ext_Vars
--- @field DirtyModVariables fun(a1:Guid|nil, a2:FixedString|nil)
--- @field DirtyUserVariables fun(a1:Guid|nil, a2:FixedString|nil)
--- @field GetModVariables fun(a1:Guid)
--- @field RegisterModVariable fun(a1:Guid, a2:FixedString)
--- @field RegisterUserVariable fun(a1:FixedString)
--- @field SyncModVariables fun()
--- @field SyncUserVariables fun()
local Ext_Vars = {}



--- @class ExtClient
--- @field Net Ext_ClientNet
--- @field ClientNet Ext_ClientNet
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
	[1] = "CanNotUse",
	[2] = "IgnoreSelf",
}

--- @enum AbilityId
Ext_Enums.AbilityId = {
	Strength = 1,
	Dexterity = 2,
	Constitution = 3,
	Intelligence = 4,
	Wisdom = 5,
	Charisma = 6,
	Sentinel = 7,
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
	[1] = "SoundObject",
	[2] = "BoneEffect",
	[3] = "Foot",
	[4] = "BoneAttachBone",
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
	Strength = 1,
	Dexterity = 2,
	Constitution = 3,
	Intelligence = 4,
	Wisdom = 5,
	Charisma = 6,
	SpellCastingAbility = 7,
	UnarmedAttackAbility = 8,
	WeaponAttackAbility = 9,
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
	[1] = "SlippingImmunity",
	[2] = "Torch",
}

--- @enum BoostCauseType
Ext_Enums.BoostCauseType = {
	Item = 1,
	Status = 2,
	Passive = 3,
	Osiris = 4,
	Unknown5 = 5,
	Progression = 6,
	Stats = 7,
	Character = 8,
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
}

--- @enum CauseType
Ext_Enums.CauseType = {
	SurfaceMove = 1,
	SurfaceCreate = 2,
	SurfaceStatus = 3,
	StatusEnter = 4,
	StatusTick = 5,
	Attack = 6,
	Offhand = 7,
	GM = 8,
	InventoryItem = 9,
	WorldItemThrow = 10,
	[1] = "SurfaceMove",
	[2] = "SurfaceCreate",
	[3] = "SurfaceStatus",
	[4] = "StatusEnter",
	[5] = "StatusTick",
	[6] = "Attack",
	[7] = "Offhand",
	[8] = "GM",
	[9] = "InventoryItem",
	[10] = "WorldItemThrow",
}

--- @enum ClientGameState
Ext_Enums.ClientGameState = {
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
}

--- @enum CombatParticipantFlags
Ext_Enums.CombatParticipantFlags = {
	CanFight = 1,
	CanJoinCombat = 2,
	[1] = "CanFight",
	[2] = "CanJoinCombat",
}

--- @enum ConditionRollType
Ext_Enums.ConditionRollType = {
	DifficultyRoll = 1,
	AttackRoll = 2,
	[1] = "DifficultyRoll",
	[2] = "AttackRoll",
}

--- @enum CraftingStationType
Ext_Enums.CraftingStationType = {
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
	Failure,Success = 1,
	SuccessNever = 2,
	[1] = "Failure,Success",
	[2] = "SuccessNever",
}

--- @enum DamageFlags
Ext_Enums.DamageFlags = {
	Hit = 1,
	Dodge = 2,
	[1] = "Hit",
	[2] = "Dodge",
}

--- @enum DamageType
Ext_Enums.DamageType = {
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
	MainWeaponDamageType = 1,
	OffhandWeaponDamageType = 2,
	MainMeleeWeaponDamageType = 3,
	OffhandMeleeWeaponDamageType = 4,
	MainRangedWeaponDamageType = 5,
	OffhandRangedWeaponDamageType = 6,
	SourceWeaponDamageType = 7,
	ThrownWeaponDamageType = 8,
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
	D6 = 1,
	D8 = 2,
	D10 = 3,
	D12 = 4,
	D20 = 5,
	D100 = 6,
	[1] = "D6",
	[2] = "D8",
	[3] = "D10",
	[4] = "D12",
	[5] = "D20",
	[6] = "D100",
}

--- @enum ESurfaceFlag
Ext_Enums.ESurfaceFlag = {
	MovementBlock = 1,
	[1] = "MovementBlock",
}

--- @enum EffectPropertyType
Ext_Enums.EffectPropertyType = {
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
	Armor = 1,
	Shield = 2,
	[1] = "Armor",
	[2] = "Shield",
}

--- @enum ExecuteWeaponFunctorsType
Ext_Enums.ExecuteWeaponFunctorsType = {
	OffHand = 1,
	BothHands = 2,
	Undefined = 3,
	[1] = "OffHand",
	[2] = "BothHands",
	[3] = "Undefined",
}

--- @enum ExtComponentType
Ext_Enums.ExtComponentType = {
	Armor = 1,
	ArmorSetState = 2,
	BaseHp = 3,
	Bound = 4,
	IsCharacter = 5,
	Data = 6,
	Experience = 7,
	Health = 8,
	Hearing = 9,
	SpellBook = 10,
	Stats = 11,
	StatusImmunities = 12,
	SurfacePathInfluences = 13,
	Use = 14,
	Value = 15,
	Weapon = 16,
	Wielding = 17,
	CustomStats = 18,
	BoostCondition = 19,
	BoostsContainer = 20,
	ActionResourceConsumeMultiplierBoost = 21,
	Tag = 22,
	SpellBookPrepares = 23,
	Transform = 24,
	BoostInfo = 25,
	Relation = 26,
	Faction = 27,
	CanInteract = 28,
	CanSpeak = 29,
	Origin = 30,
	Level = 31,
	EocLevel = 32,
	Classes = 33,
	MaterialParameterOverride = 34,
	OffStage = 35,
	PickingState = 36,
	Player = 37,
	SimpleCharacter = 38,
	Speaker = 39,
	WeaponSet = 40,
	DifficultyCheck = 41,
	AttributeFlags = 42,
	BodyType = 43,
	ObjectSize = 44,
	BaseStats = 45,
	Expertise = 46,
	HealBlock = 47,
	Ruleset = 48,
	RulesetModifiers = 49,
	ShapeshiftState = 50,
	PassiveContainer = 51,
	Passive = 52,
	PassiveUsageCount = 53,
	ItemDestroyed = 54,
	ItemDye = 55,
	MapMarkerStyle = 56,
	ItemPortal = 57,
	IsDoor = 58,
	ExamineDisabled = 59,
	ItemHasMoved = 60,
	HasOpened = 61,
	IsGold = 62,
	IsItem = 63,
	NewInInventory = 64,
	ShouldDestroyOnSpellCast = 65,
	ActionType = 66,
	ItemCanMove = 67,
	ClimbOn = 68,
	ItemTemplateDestroyed = 69,
	InteractionDisabled = 70,
	IsStoryItem = 71,
	Ladder = 72,
	WalkOn = 73,
	UseAction = 74,
	OwnedAsLoot = 75,
	OwneeCurrent = 76,
	InventoryMember = 77,
	InventoryData = 78,
	InventoryOwner = 79,
	InventoryIsOwned = 80,
	InventoryContainer = 81,
	InventoryMemberTransform = 82,
	InventoryStack = 83,
	InventoryStackMember = 84,
	InventoryWeight = 85,
	Wielded = 86,
	CanBeInInventory = 87,
	CannotBePickpocketed = 88,
	CannotBeTakenOut = 89,
	DropOnDeathBlocked = 90,
	NewItemsInside = 91,
	NonTradable = 92,
	PartyView = 93,
	PartyComposition = 94,
	PartyPortals = 95,
	PartyRecipes = 96,
	PartyWaypoints = 97,
	PartyMember = 98,
	PartyFollower = 99,
	CurrentlyFollowingParty = 100,
	BlockFollow = 101,
	Recruiter = 102,
	CampChest = 103,
	CampEndTheDayState = 104,
	CampQuality = 105,
	CampSettings = 106,
	CampSupply = 107,
	CampTotalSupplies = 108,
	CampPresence = 109,
	IsInCombat = 110,
	CombatParticipant = 111,
	CombatState = 112,
	TurnBased = 113,
	TurnOrder = 114,
	IsCombatPaused = 115,
	ThreatRange = 116,
	Death = 117,
	DeathState = 118,
	DeadByDefault = 119,
	HitTarget = 120,
	HitAttacker = 121,
	HitWeapon = 122,
	HitThrownObject = 123,
	HitProxy = 124,
	HitProxyOwner = 125,
	HitReaction = 126,
	Identity = 127,
	IdentityState = 128,
	OriginalIdentity = 129,
	ServerCanStartCombat = 130,
	ServerCombatGroupMapping = 131,
	ServerEnterRequest = 132,
	ServerFleeBlocked = 133,
	ServerImmediateJoin = 134,
	SpellContainer = 135,
	PlayerPrepareSpell = 136,
	CCPrepareSpell = 137,
	SpellModificationContainer = 138,
	AddedSpells = 139,
	SpellBookCooldowns = 140,
	LearnedSpells = 141,
	SpellAiConditions = 142,
	SpellCastAnimationInfo = 143,
	SpellCastCache = 144,
	SpellCastCanBeTargeted = 145,
	SpellCastIsCasting = 146,
	SpellCastInterruptResults = 147,
	SpellCastMovement = 148,
	SpellCastRolls = 149,
	SpellCastState = 150,
	SpellSyncTargeting = 151,
	ServerSpellCastHitDelay = 152,
	ServerSpellCastResponsible = 153,
	ServerSpellClientInitiated = 154,
	ServerSpellExternals = 155,
	ServerSpellHitRegister = 156,
	ServerSpellInterruptRequests = 157,
	ServerSpellInterruptResults = 158,
	ServerSpellCastState = 159,
	InterruptActionState = 160,
	InterruptConditionallyDisabled = 161,
	InterruptContainer = 162,
	InterruptData = 163,
	InterruptDecision = 164,
	InterruptPreferences = 165,
	InterruptPrepared = 166,
	InterruptZone = 167,
	InterruptZoneParticipant = 168,
	InterruptZoneSource = 169,
	ServerInterruptAddRemoveRequests = 170,
	ServerInterruptActionRequests = 171,
	ServerInterruptZoneRequests = 172,
	ServerInterruptInitialParticipants = 173,
	ServerInterruptTurnOrderInZone = 174,
	Sight = 175,
	SightData = 176,
	SightEntityViewshed = 177,
	StatusContainer = 178,
	StatusCause = 179,
	StatusID = 180,
	StatusIncapacitated = 181,
	StatusLifetime = 182,
	StatusVisualDisabled = 183,
	TadpoleTreeState = 184,
	TadpolePowers = 185,
	Tadpoled = 186,
	HalfIllithid = 187,
	FullIllithid = 188,
	CCCompanionDefinition = 189,
	CCDefinitionCommon = 190,
	CCFullRespecDefinition = 191,
	CCRespecDefinition = 192,
	CCCharacterDefinition = 193,
	CCSessionCommon = 194,
	CCState = 195,
	CCLevelUpDefinition = 196,
	CCFullRespec = 197,
	CCRespec = 198,
	CCCreation = 199,
	CCLevelUp = 200,
	Background = 201,
	God = 202,
	Proficiency = 203,
	ProficiencyGroup = 204,
	LevelUp = 205,
	Floating = 206,
	Voice = 207,
	CustomIcon = 208,
	CharacterCreationStats = 209,
	Disarmable = 210,
	ShortRest = 211,
	IsSummon = 212,
	SummonContainer = 213,
	Stealth = 214,
	ClientControl = 215,
	IsGlobal = 216,
	Savegame = 217,
	DisabledEquipment = 218,
	LootingState = 219,
	Loot = 220,
	Lock = 221,
	Key = 222,
	SummonLifetime = 223,
	Invisibility = 224,
	Icon = 225,
	HotbarContainer = 226,
	HotbarDecks = 227,
	OriginTag = 228,
	OriginPassives = 229,
	ClassTag = 230,
	BackgroundTag = 231,
	OriginAppearanceTag = 232,
	VoiceTag = 233,
	GodTag = 234,
	BackgroundPassives = 235,
	GlobalShortRestDisabled = 236,
	GlobalLongRestDisabled = 237,
	StoryShortRestDisabled = 238,
	FleeCapability = 239,
	CanDoRest = 240,
	IsInTurnBasedMode = 241,
	ItemBoosts = 242,
	ActiveCharacterLight = 243,
	Visual = 244,
	AnimationSet = 245,
	AnimationBlueprint = 246,
	CanModifyHealth = 247,
	AvailableLevel = 248,
	CanBeLooted = 249,
	CanBeDisarmed = 250,
	CanTriggerRandomCasts = 251,
	CanDoActions = 252,
	CanMove = 253,
	CanSense = 254,
	Concentration = 255,
	Darkness = 256,
	DualWielding = 257,
	GameObjectVisual = 258,
	AppearanceOverride = 259,
	CharacterCreationTemplateOverride = 260,
	GravityDisabled = 261,
	GravityDisabledUntilMoved = 262,
	CanBeWielded = 263,
	Avatar = 264,
	HasExclamationDialog = 265,
	Trader = 266,
	InteractionFilter = 267,
	DisplayName = 268,
	Equipable = 269,
	GameplayLight = 270,
	ProgressionContainer = 271,
	ProgressionMeta = 272,
	Race = 273,
	CanTravel = 274,
	Movement = 275,
	ObjectInteraction = 276,
	StaticPhysics = 277,
	Pathing = 278,
	Steering = 279,
	CanDeflectProjectiles = 280,
	ActiveSkeletonSlots = 281,
	Net = 282,
	Physics = 283,
	FTBParticipant = 284,
	FTBRespect = 285,
	ApprovalRatings = 286,
	AttitudesToPlayers = 287,
	CharacterCreationAppearance = 288,
	Active = 289,
	EquipmentVisual = 290,
	Repose = 291,
	OriginalTemplate = 292,
	CanEnterChasm = 293,
	DialogState = 294,
	EncumbranceState = 295,
	EncumbranceStats = 296,
	Unsheath = 297,
	UseSocket = 298,
	UserAvatar = 299,
	UserReservedFor = 300,
	CanSeeThrough = 301,
	CanShootThrough = 302,
	CanWalkThrough = 303,
	ShootThroughType = 304,
	ServerRecruitedBy = 305,
	ServerGameTimer = 306,
	ServerExperienceGaveOut = 307,
	ServerReplicationDependency = 308,
	ServerIsUnsummoning = 309,
	ServerActivationGroupContainer = 310,
	ServerAnubisTag = 311,
	ServerDialogTag = 312,
	ServerDisplayNameList = 313,
	ServerIconList = 314,
	ServerRaceTag = 315,
	ServerTemplateTag = 316,
	ServerToggledPassives = 317,
	ServerBoostTag = 318,
	ServerSafePosition = 319,
	ServerAnubisExecutor = 320,
	ServerLeader = 321,
	ServerBreadcrumb = 322,
	ServerDelayDeathCause = 323,
	ServerPickpocket = 324,
	ServerReplicationDependencyOwner = 325,
	ServerCharacter = 326,
	ServerItem = 327,
	ServerProjectile = 328,
	ServerOsirisTag = 329,
	ClientCharacter = 330,
	ClientItem = 331,
	ClientProjectile = 332,
	ArmorClassBoost = 333,
	AbilityBoost = 334,
	RollBonusBoost = 335,
	AdvantageBoost = 336,
	ActionResourceValueBoost = 337,
	CriticalHitBoost = 338,
	AbilityFailedSavingThrowBoost = 339,
	ResistanceBoost = 340,
	WeaponDamageResistanceBoost = 341,
	ProficiencyBonusOverrideBoost = 342,
	JumpMaxDistanceMultiplierBoost = 343,
	HalveWeaponDamageBoost = 344,
	UnlockSpellBoost = 345,
	SourceAdvantageBoost = 346,
	ProficiencyBonusBoost = 347,
	ProficiencyBoost = 348,
	IncreaseMaxHPBoost = 349,
	ActionResourceBlockBoost = 350,
	StatusImmunityBoost = 351,
	UseBoosts = 352,
	CannotHarmCauseEntityBoost = 353,
	TemporaryHPBoost = 354,
	WeightBoost = 355,
	WeightCategoryBoost = 356,
	FactionOverrideBoost = 357,
	ActionResourceMultiplierBoost = 358,
	BlockRegainHPBoost = 359,
	InitiativeBoost = 360,
	DarkvisionRangeBoost = 361,
	DarkvisionRangeMinBoost = 362,
	DarkvisionRangeOverrideBoost = 363,
	AddTagBoost = 364,
	IgnoreDamageThresholdMinBoost = 365,
	SkillBoost = 366,
	WeaponDamageBoost = 367,
	NullifyAbilityBoost = 368,
	RerollBoost = 369,
	DownedStatusBoost = 370,
	WeaponEnchantmentBoost = 371,
	GuaranteedChanceRollOutcomeBoost = 372,
	AttributeBoost = 373,
	GameplayLightBoost = 374,
	DualWieldingBoost = 375,
	SavantBoost = 376,
	MinimumRollResultBoost = 377,
	CharacterWeaponDamageBoost = 378,
	ProjectileDeflectBoost = 379,
	AbilityOverrideMinimumBoost = 380,
	ACOverrideFormulaBoost = 381,
	FallDamageMultiplierBoost = 382,
	ActiveCharacterLightBoost = 383,
	WeaponAttackTypeOverrideBoost = 384,
	WeaponDamageDieOverrideBoost = 385,
	CarryCapacityMultiplierBoost = 386,
	WeaponPropertyBoost = 387,
	WeaponAttackRollAbilityOverrideBoost = 388,
	SightRangeAdditiveBoost = 389,
	SightRangeMinimumBoost = 390,
	SightRangeMaximumBoost = 391,
	SightRangeOverrideBoost = 392,
	MovementSpeedLimitBoost = 393,
	UnlockSpellVariantBoost = 394,
	DetectCrimesBlockBoost = 395,
	BlockAbilityModifierFromACBoost = 396,
	ScaleMultiplierBoost = 397,
	DamageReductionBoost = 398,
	ReduceCriticalAttackThresholdBoost = 399,
	PhysicalForceRangeBonusBoost = 400,
	ObjectSizeBoost = 401,
	ObjectSizeOverrideBoost = 402,
	AiArchetypeOverrideBoost = 403,
	ExpertiseBonusBoost = 404,
	EntityThrowDamageBoost = 405,
	WeaponDamageTypeOverrideBoost = 406,
	MaximizeHealingBoost = 407,
	DamageBonusBoost = 408,
	AdvanceSpellsBoost = 409,
	SpellResistanceBoost = 410,
	WeaponAttackRollBonusBoost = 411,
	SpellSaveDCBoost = 412,
	RedirectDamageBoost = 413,
	CanSeeThroughBoost = 414,
	CanShootThroughBoost = 415,
	CanWalkThroughBoost = 416,
	MonkWeaponDamageDiceOverrideBoost = 417,
	HorizontalFOVOverrideBoost = 418,
	CharacterUnarmedDamageBoost = 419,
	ActionResourceReplenishTypeOverrideBoost = 420,
	ActionResourcePreventReductionBoost = 421,
	AttackSpellOverrideBoost = 422,
	LockBoost = 423,
	IgnorePointBlankDisadvantageBoost = 424,
	CriticalHitExtraDiceBoost = 425,
	DodgeAttackRollBoost = 426,
	GameplayObscurityBoost = 427,
	MaximumRollResultBoost = 428,
	UnlockInterruptBoost = 429,
	JumpMaxDistanceBonusBoost = 430,
	ArmorAbilityModifierCapOverrideBoost = 431,
	IgnoreResistanceBoost = 432,
	ConcentrationIgnoreDamageBoost = 433,
	IgnoreLowGroundPenaltyBoost = 434,
	IgnoreSurfaceCoverBoost = 435,
	Uuid = 436,
	UuidToHandleMapping = 437,
	Max = 438,
	[1] = "Armor",
	[2] = "ArmorSetState",
	[3] = "BaseHp",
	[4] = "Bound",
	[5] = "IsCharacter",
	[6] = "Data",
	[7] = "Experience",
	[8] = "Health",
	[9] = "Hearing",
	[10] = "SpellBook",
	[11] = "Stats",
	[12] = "StatusImmunities",
	[13] = "SurfacePathInfluences",
	[14] = "Use",
	[15] = "Value",
	[16] = "Weapon",
	[17] = "Wielding",
	[18] = "CustomStats",
	[19] = "BoostCondition",
	[20] = "BoostsContainer",
	[21] = "ActionResourceConsumeMultiplierBoost",
	[22] = "Tag",
	[23] = "SpellBookPrepares",
	[24] = "Transform",
	[25] = "BoostInfo",
	[26] = "Relation",
	[27] = "Faction",
	[28] = "CanInteract",
	[29] = "CanSpeak",
	[30] = "Origin",
	[31] = "Level",
	[32] = "EocLevel",
	[33] = "Classes",
	[34] = "MaterialParameterOverride",
	[35] = "OffStage",
	[36] = "PickingState",
	[37] = "Player",
	[38] = "SimpleCharacter",
	[39] = "Speaker",
	[40] = "WeaponSet",
	[41] = "DifficultyCheck",
	[42] = "AttributeFlags",
	[43] = "BodyType",
	[44] = "ObjectSize",
	[45] = "BaseStats",
	[46] = "Expertise",
	[47] = "HealBlock",
	[48] = "Ruleset",
	[49] = "RulesetModifiers",
	[50] = "ShapeshiftState",
	[51] = "PassiveContainer",
	[52] = "Passive",
	[53] = "PassiveUsageCount",
	[54] = "ItemDestroyed",
	[55] = "ItemDye",
	[56] = "MapMarkerStyle",
	[57] = "ItemPortal",
	[58] = "IsDoor",
	[59] = "ExamineDisabled",
	[60] = "ItemHasMoved",
	[61] = "HasOpened",
	[62] = "IsGold",
	[63] = "IsItem",
	[64] = "NewInInventory",
	[65] = "ShouldDestroyOnSpellCast",
	[66] = "ActionType",
	[67] = "ItemCanMove",
	[68] = "ClimbOn",
	[69] = "ItemTemplateDestroyed",
	[70] = "InteractionDisabled",
	[71] = "IsStoryItem",
	[72] = "Ladder",
	[73] = "WalkOn",
	[74] = "UseAction",
	[75] = "OwnedAsLoot",
	[76] = "OwneeCurrent",
	[77] = "InventoryMember",
	[78] = "InventoryData",
	[79] = "InventoryOwner",
	[80] = "InventoryIsOwned",
	[81] = "InventoryContainer",
	[82] = "InventoryMemberTransform",
	[83] = "InventoryStack",
	[84] = "InventoryStackMember",
	[85] = "InventoryWeight",
	[86] = "Wielded",
	[87] = "CanBeInInventory",
	[88] = "CannotBePickpocketed",
	[89] = "CannotBeTakenOut",
	[90] = "DropOnDeathBlocked",
	[91] = "NewItemsInside",
	[92] = "NonTradable",
	[93] = "PartyView",
	[94] = "PartyComposition",
	[95] = "PartyPortals",
	[96] = "PartyRecipes",
	[97] = "PartyWaypoints",
	[98] = "PartyMember",
	[99] = "PartyFollower",
	[100] = "CurrentlyFollowingParty",
	[101] = "BlockFollow",
	[102] = "Recruiter",
	[103] = "CampChest",
	[104] = "CampEndTheDayState",
	[105] = "CampQuality",
	[106] = "CampSettings",
	[107] = "CampSupply",
	[108] = "CampTotalSupplies",
	[109] = "CampPresence",
	[110] = "IsInCombat",
	[111] = "CombatParticipant",
	[112] = "CombatState",
	[113] = "TurnBased",
	[114] = "TurnOrder",
	[115] = "IsCombatPaused",
	[116] = "ThreatRange",
	[117] = "Death",
	[118] = "DeathState",
	[119] = "DeadByDefault",
	[120] = "HitTarget",
	[121] = "HitAttacker",
	[122] = "HitWeapon",
	[123] = "HitThrownObject",
	[124] = "HitProxy",
	[125] = "HitProxyOwner",
	[126] = "HitReaction",
	[127] = "Identity",
	[128] = "IdentityState",
	[129] = "OriginalIdentity",
	[130] = "ServerCanStartCombat",
	[131] = "ServerCombatGroupMapping",
	[132] = "ServerEnterRequest",
	[133] = "ServerFleeBlocked",
	[134] = "ServerImmediateJoin",
	[135] = "SpellContainer",
	[136] = "PlayerPrepareSpell",
	[137] = "CCPrepareSpell",
	[138] = "SpellModificationContainer",
	[139] = "AddedSpells",
	[140] = "SpellBookCooldowns",
	[141] = "LearnedSpells",
	[142] = "SpellAiConditions",
	[143] = "SpellCastAnimationInfo",
	[144] = "SpellCastCache",
	[145] = "SpellCastCanBeTargeted",
	[146] = "SpellCastIsCasting",
	[147] = "SpellCastInterruptResults",
	[148] = "SpellCastMovement",
	[149] = "SpellCastRolls",
	[150] = "SpellCastState",
	[151] = "SpellSyncTargeting",
	[152] = "ServerSpellCastHitDelay",
	[153] = "ServerSpellCastResponsible",
	[154] = "ServerSpellClientInitiated",
	[155] = "ServerSpellExternals",
	[156] = "ServerSpellHitRegister",
	[157] = "ServerSpellInterruptRequests",
	[158] = "ServerSpellInterruptResults",
	[159] = "ServerSpellCastState",
	[160] = "InterruptActionState",
	[161] = "InterruptConditionallyDisabled",
	[162] = "InterruptContainer",
	[163] = "InterruptData",
	[164] = "InterruptDecision",
	[165] = "InterruptPreferences",
	[166] = "InterruptPrepared",
	[167] = "InterruptZone",
	[168] = "InterruptZoneParticipant",
	[169] = "InterruptZoneSource",
	[170] = "ServerInterruptAddRemoveRequests",
	[171] = "ServerInterruptActionRequests",
	[172] = "ServerInterruptZoneRequests",
	[173] = "ServerInterruptInitialParticipants",
	[174] = "ServerInterruptTurnOrderInZone",
	[175] = "Sight",
	[176] = "SightData",
	[177] = "SightEntityViewshed",
	[178] = "StatusContainer",
	[179] = "StatusCause",
	[180] = "StatusID",
	[181] = "StatusIncapacitated",
	[182] = "StatusLifetime",
	[183] = "StatusVisualDisabled",
	[184] = "TadpoleTreeState",
	[185] = "TadpolePowers",
	[186] = "Tadpoled",
	[187] = "HalfIllithid",
	[188] = "FullIllithid",
	[189] = "CCCompanionDefinition",
	[190] = "CCDefinitionCommon",
	[191] = "CCFullRespecDefinition",
	[192] = "CCRespecDefinition",
	[193] = "CCCharacterDefinition",
	[194] = "CCSessionCommon",
	[195] = "CCState",
	[196] = "CCLevelUpDefinition",
	[197] = "CCFullRespec",
	[198] = "CCRespec",
	[199] = "CCCreation",
	[200] = "CCLevelUp",
	[201] = "Background",
	[202] = "God",
	[203] = "Proficiency",
	[204] = "ProficiencyGroup",
	[205] = "LevelUp",
	[206] = "Floating",
	[207] = "Voice",
	[208] = "CustomIcon",
	[209] = "CharacterCreationStats",
	[210] = "Disarmable",
	[211] = "ShortRest",
	[212] = "IsSummon",
	[213] = "SummonContainer",
	[214] = "Stealth",
	[215] = "ClientControl",
	[216] = "IsGlobal",
	[217] = "Savegame",
	[218] = "DisabledEquipment",
	[219] = "LootingState",
	[220] = "Loot",
	[221] = "Lock",
	[222] = "Key",
	[223] = "SummonLifetime",
	[224] = "Invisibility",
	[225] = "Icon",
	[226] = "HotbarContainer",
	[227] = "HotbarDecks",
	[228] = "OriginTag",
	[229] = "OriginPassives",
	[230] = "ClassTag",
	[231] = "BackgroundTag",
	[232] = "OriginAppearanceTag",
	[233] = "VoiceTag",
	[234] = "GodTag",
	[235] = "BackgroundPassives",
	[236] = "GlobalShortRestDisabled",
	[237] = "GlobalLongRestDisabled",
	[238] = "StoryShortRestDisabled",
	[239] = "FleeCapability",
	[240] = "CanDoRest",
	[241] = "IsInTurnBasedMode",
	[242] = "ItemBoosts",
	[243] = "ActiveCharacterLight",
	[244] = "Visual",
	[245] = "AnimationSet",
	[246] = "AnimationBlueprint",
	[247] = "CanModifyHealth",
	[248] = "AvailableLevel",
	[249] = "CanBeLooted",
	[250] = "CanBeDisarmed",
	[251] = "CanTriggerRandomCasts",
	[252] = "CanDoActions",
	[253] = "CanMove",
	[254] = "CanSense",
	[255] = "Concentration",
	[256] = "Darkness",
	[257] = "DualWielding",
	[258] = "GameObjectVisual",
	[259] = "AppearanceOverride",
	[260] = "CharacterCreationTemplateOverride",
	[261] = "GravityDisabled",
	[262] = "GravityDisabledUntilMoved",
	[263] = "CanBeWielded",
	[264] = "Avatar",
	[265] = "HasExclamationDialog",
	[266] = "Trader",
	[267] = "InteractionFilter",
	[268] = "DisplayName",
	[269] = "Equipable",
	[270] = "GameplayLight",
	[271] = "ProgressionContainer",
	[272] = "ProgressionMeta",
	[273] = "Race",
	[274] = "CanTravel",
	[275] = "Movement",
	[276] = "ObjectInteraction",
	[277] = "StaticPhysics",
	[278] = "Pathing",
	[279] = "Steering",
	[280] = "CanDeflectProjectiles",
	[281] = "ActiveSkeletonSlots",
	[282] = "Net",
	[283] = "Physics",
	[284] = "FTBParticipant",
	[285] = "FTBRespect",
	[286] = "ApprovalRatings",
	[287] = "AttitudesToPlayers",
	[288] = "CharacterCreationAppearance",
	[289] = "Active",
	[290] = "EquipmentVisual",
	[291] = "Repose",
	[292] = "OriginalTemplate",
	[293] = "CanEnterChasm",
	[294] = "DialogState",
	[295] = "EncumbranceState",
	[296] = "EncumbranceStats",
	[297] = "Unsheath",
	[298] = "UseSocket",
	[299] = "UserAvatar",
	[300] = "UserReservedFor",
	[301] = "CanSeeThrough",
	[302] = "CanShootThrough",
	[303] = "CanWalkThrough",
	[304] = "ShootThroughType",
	[305] = "ServerRecruitedBy",
	[306] = "ServerGameTimer",
	[307] = "ServerExperienceGaveOut",
	[308] = "ServerReplicationDependency",
	[309] = "ServerIsUnsummoning",
	[310] = "ServerActivationGroupContainer",
	[311] = "ServerAnubisTag",
	[312] = "ServerDialogTag",
	[313] = "ServerDisplayNameList",
	[314] = "ServerIconList",
	[315] = "ServerRaceTag",
	[316] = "ServerTemplateTag",
	[317] = "ServerToggledPassives",
	[318] = "ServerBoostTag",
	[319] = "ServerSafePosition",
	[320] = "ServerAnubisExecutor",
	[321] = "ServerLeader",
	[322] = "ServerBreadcrumb",
	[323] = "ServerDelayDeathCause",
	[324] = "ServerPickpocket",
	[325] = "ServerReplicationDependencyOwner",
	[326] = "ServerCharacter",
	[327] = "ServerItem",
	[328] = "ServerProjectile",
	[329] = "ServerOsirisTag",
	[330] = "ClientCharacter",
	[331] = "ClientItem",
	[332] = "ClientProjectile",
	[333] = "ArmorClassBoost",
	[334] = "AbilityBoost",
	[335] = "RollBonusBoost",
	[336] = "AdvantageBoost",
	[337] = "ActionResourceValueBoost",
	[338] = "CriticalHitBoost",
	[339] = "AbilityFailedSavingThrowBoost",
	[340] = "ResistanceBoost",
	[341] = "WeaponDamageResistanceBoost",
	[342] = "ProficiencyBonusOverrideBoost",
	[343] = "JumpMaxDistanceMultiplierBoost",
	[344] = "HalveWeaponDamageBoost",
	[345] = "UnlockSpellBoost",
	[346] = "SourceAdvantageBoost",
	[347] = "ProficiencyBonusBoost",
	[348] = "ProficiencyBoost",
	[349] = "IncreaseMaxHPBoost",
	[350] = "ActionResourceBlockBoost",
	[351] = "StatusImmunityBoost",
	[352] = "UseBoosts",
	[353] = "CannotHarmCauseEntityBoost",
	[354] = "TemporaryHPBoost",
	[355] = "WeightBoost",
	[356] = "WeightCategoryBoost",
	[357] = "FactionOverrideBoost",
	[358] = "ActionResourceMultiplierBoost",
	[359] = "BlockRegainHPBoost",
	[360] = "InitiativeBoost",
	[361] = "DarkvisionRangeBoost",
	[362] = "DarkvisionRangeMinBoost",
	[363] = "DarkvisionRangeOverrideBoost",
	[364] = "AddTagBoost",
	[365] = "IgnoreDamageThresholdMinBoost",
	[366] = "SkillBoost",
	[367] = "WeaponDamageBoost",
	[368] = "NullifyAbilityBoost",
	[369] = "RerollBoost",
	[370] = "DownedStatusBoost",
	[371] = "WeaponEnchantmentBoost",
	[372] = "GuaranteedChanceRollOutcomeBoost",
	[373] = "AttributeBoost",
	[374] = "GameplayLightBoost",
	[375] = "DualWieldingBoost",
	[376] = "SavantBoost",
	[377] = "MinimumRollResultBoost",
	[378] = "CharacterWeaponDamageBoost",
	[379] = "ProjectileDeflectBoost",
	[380] = "AbilityOverrideMinimumBoost",
	[381] = "ACOverrideFormulaBoost",
	[382] = "FallDamageMultiplierBoost",
	[383] = "ActiveCharacterLightBoost",
	[384] = "WeaponAttackTypeOverrideBoost",
	[385] = "WeaponDamageDieOverrideBoost",
	[386] = "CarryCapacityMultiplierBoost",
	[387] = "WeaponPropertyBoost",
	[388] = "WeaponAttackRollAbilityOverrideBoost",
	[389] = "SightRangeAdditiveBoost",
	[390] = "SightRangeMinimumBoost",
	[391] = "SightRangeMaximumBoost",
	[392] = "SightRangeOverrideBoost",
	[393] = "MovementSpeedLimitBoost",
	[394] = "UnlockSpellVariantBoost",
	[395] = "DetectCrimesBlockBoost",
	[396] = "BlockAbilityModifierFromACBoost",
	[397] = "ScaleMultiplierBoost",
	[398] = "DamageReductionBoost",
	[399] = "ReduceCriticalAttackThresholdBoost",
	[400] = "PhysicalForceRangeBonusBoost",
	[401] = "ObjectSizeBoost",
	[402] = "ObjectSizeOverrideBoost",
	[403] = "AiArchetypeOverrideBoost",
	[404] = "ExpertiseBonusBoost",
	[405] = "EntityThrowDamageBoost",
	[406] = "WeaponDamageTypeOverrideBoost",
	[407] = "MaximizeHealingBoost",
	[408] = "DamageBonusBoost",
	[409] = "AdvanceSpellsBoost",
	[410] = "SpellResistanceBoost",
	[411] = "WeaponAttackRollBonusBoost",
	[412] = "SpellSaveDCBoost",
	[413] = "RedirectDamageBoost",
	[414] = "CanSeeThroughBoost",
	[415] = "CanShootThroughBoost",
	[416] = "CanWalkThroughBoost",
	[417] = "MonkWeaponDamageDiceOverrideBoost",
	[418] = "HorizontalFOVOverrideBoost",
	[419] = "CharacterUnarmedDamageBoost",
	[420] = "ActionResourceReplenishTypeOverrideBoost",
	[421] = "ActionResourcePreventReductionBoost",
	[422] = "AttackSpellOverrideBoost",
	[423] = "LockBoost",
	[424] = "IgnorePointBlankDisadvantageBoost",
	[425] = "CriticalHitExtraDiceBoost",
	[426] = "DodgeAttackRollBoost",
	[427] = "GameplayObscurityBoost",
	[428] = "MaximumRollResultBoost",
	[429] = "UnlockInterruptBoost",
	[430] = "JumpMaxDistanceBonusBoost",
	[431] = "ArmorAbilityModifierCapOverrideBoost",
	[432] = "IgnoreResistanceBoost",
	[433] = "ConcentrationIgnoreDamageBoost",
	[434] = "IgnoreLowGroundPenaltyBoost",
	[435] = "IgnoreSurfaceCoverBoost",
	[436] = "Uuid",
	[437] = "UuidToHandleMapping",
	[438] = "Max",
}

--- @enum ExtQueryType
Ext_Enums.ExtQueryType = {
	Max = 1,
	[1] = "Max",
}

--- @enum ExtResourceManagerType
Ext_Enums.ExtResourceManagerType = {
	Faction = 1,
	Race = 2,
	AbilityDistributionPreset = 3,
	CompanionPreset = 4,
	Origin = 5,
	Background = 6,
	God = 7,
	AbilityList = 8,
	SkillList = 9,
	SpellList = 10,
	PassiveList = 11,
	Progression = 12,
	ProgressionDescription = 13,
	Gossip = 14,
	ActionResource = 15,
	ActionResourceGroup = 16,
	EquipmentType = 17,
	VFX = 18,
	CharacterCreationPreset = 19,
	CharacterCreationSkinColor = 20,
	CharacterCreationEyeColor = 21,
	CharacterCreationHairColor = 22,
	CharacterCreationAccessorySet = 23,
	CharacterCreationEquipmentIcons = 24,
	CharacterCreationIconSettings = 25,
	CharacterCreationMaterialOverride = 26,
	CharacterCreationAppearanceMaterial = 27,
	CharacterCreationPassiveAppearance = 28,
	CharacterCreationAppearanceVisual = 29,
	CharacterCreationSharedVisual = 30,
	TutorialEntries = 31,
	Feat = 32,
	FeatDescription = 33,
	TutorialModalEntries = 34,
	ClassDescription = 35,
	ColorDefinition = 36,
	Flag = 37,
	LevelMap = 38,
	Max = 39,
	[1] = "Faction",
	[2] = "Race",
	[3] = "AbilityDistributionPreset",
	[4] = "CompanionPreset",
	[5] = "Origin",
	[6] = "Background",
	[7] = "God",
	[8] = "AbilityList",
	[9] = "SkillList",
	[10] = "SpellList",
	[11] = "PassiveList",
	[12] = "Progression",
	[13] = "ProgressionDescription",
	[14] = "Gossip",
	[15] = "ActionResource",
	[16] = "ActionResourceGroup",
	[17] = "EquipmentType",
	[18] = "VFX",
	[19] = "CharacterCreationPreset",
	[20] = "CharacterCreationSkinColor",
	[21] = "CharacterCreationEyeColor",
	[22] = "CharacterCreationHairColor",
	[23] = "CharacterCreationAccessorySet",
	[24] = "CharacterCreationEquipmentIcons",
	[25] = "CharacterCreationIconSettings",
	[26] = "CharacterCreationMaterialOverride",
	[27] = "CharacterCreationAppearanceMaterial",
	[28] = "CharacterCreationPassiveAppearance",
	[29] = "CharacterCreationAppearanceVisual",
	[30] = "CharacterCreationSharedVisual",
	[31] = "TutorialEntries",
	[32] = "Feat",
	[33] = "FeatDescription",
	[34] = "TutorialModalEntries",
	[35] = "ClassDescription",
	[36] = "ColorDefinition",
	[37] = "Flag",
	[38] = "LevelMap",
	[39] = "Max",
}

--- @enum FleeErrorFlags
Ext_Enums.FleeErrorFlags = {
	Region = 1,
	EnemyTooClose = 2,
	[1] = "Region",
	[2] = "EnemyTooClose",
}

--- @enum ForceFunctorAggression
Ext_Enums.ForceFunctorAggression = {
	Friendly = 1,
	Neutral = 2,
	Undefined = 3,
	[1] = "Friendly",
	[2] = "Neutral",
	[3] = "Undefined",
}

--- @enum ForceFunctorOrigin
Ext_Enums.ForceFunctorOrigin = {
	OriginToTarget = 1,
	TargetToEntity = 2,
	Undefined = 3,
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
	[1] = "RainAction",
	[2] = "StormAction",
}

--- @enum Gender
Ext_Enums.Gender = {
	Female = 1,
	[1] = "Female",
}

--- @enum HandednessType
Ext_Enums.HandednessType = {
	One = 1,
	Two = 2,
	[1] = "One",
	[2] = "Two",
}

--- @enum HealDirection
Ext_Enums.HealDirection = {
	Outgoing = 1,
	[1] = "Outgoing",
}

--- @enum HealEffect
Ext_Enums.HealEffect = {
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
	Magic = 1,
	Ranged = 2,
	WeaponDamage = 3,
	Surface = 4,
	DoT = 5,
	Reflected = 6,
	[1] = "Magic",
	[2] = "Ranged",
	[3] = "WeaponDamage",
	[4] = "Surface",
	[5] = "DoT",
	[6] = "Reflected",
}

--- @enum HitWith
Ext_Enums.HitWith = {
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
	Consume = 1,
	Transform = 2,
	Boost = 3,
	Poison = 4,
	[1] = "Consume",
	[2] = "Transform",
	[3] = "Boost",
	[4] = "Poison",
}

--- @enum IngredientType
Ext_Enums.IngredientType = {
	Object = 1,
	Category = 2,
	Property = 3,
	[1] = "Object",
	[2] = "Category",
	[3] = "Property",
}

--- @enum InputType
Ext_Enums.InputType = {
	Unknown,Press = 1,
	Unknown,Release = 2,
	[1] = "Unknown,Press",
	[2] = "Unknown,Release",
}

--- @enum ItemDataRarity
Ext_Enums.ItemDataRarity = {
	Unique = 1,
	Uncommon = 2,
	Rare = 3,
	Epic = 4,
	Legendary = 5,
	Divine = 6,
	Sentinel = 7,
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
}

--- @enum PathRootType
Ext_Enums.PathRootType = {
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
	AttackTarget = 1,
	SavingThrow = 2,
	AllSavingThrows = 3,
	Ability = 4,
	AllAbilities = 5,
	Skill = 6,
	AllSkills = 7,
	SourceDialogue = 8,
	WeaponActionDC = 9,
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
	[1] = "Battleaxes",
	[2] = "Clubs",
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
	Weapons = 1,
	Potions = 2,
	Grenades = 3,
	Arrows = 4,
	Armour = 5,
	Food = 6,
	Objects = 7,
	Runes = 8,
	Grimoire = 9,
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
	Level = 1,
	Strength = 2,
	Dexterity = 3,
	Intelligence = 4,
	Constitution = 5,
	Wisdom = 6,
	[1] = "Level",
	[2] = "Strength",
	[3] = "Dexterity",
	[4] = "Intelligence",
	[5] = "Constitution",
	[6] = "Wisdom",
}

--- @enum ResistanceBoostFlags
Ext_Enums.ResistanceBoostFlags = {
	BelowDamageThreshold = 1,
	ResistantToMagical = 2,
	[1] = "BelowDamageThreshold",
	[2] = "ResistantToMagical",
}

--- @enum ResourceBankType
Ext_Enums.ResourceBankType = {
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
	[1] = "Never",
	[2] = "Default",
}

--- @enum RestErrorFlags
Ext_Enums.RestErrorFlags = {
	GlobalDisabled = 1,
	Script = 2,
	[1] = "GlobalDisabled",
	[2] = "Script",
}

--- @enum RestFlags
Ext_Enums.RestFlags = {
	CanShortRest = 1,
	CanLongRest = 2,
	[1] = "CanShortRest",
	[2] = "CanLongRest",
}

--- @enum RollCritical
Ext_Enums.RollCritical = {
	Success = 1,
	Fail = 2,
	[1] = "Success",
	[2] = "Fail",
}

--- @enum ServerCharacterFlags
Ext_Enums.ServerCharacterFlags = {
	IsPlayer = 1,
	Multiplayer = 2,
	[1] = "IsPlayer",
	[2] = "Multiplayer",
}

--- @enum ServerCharacterFlags2
Ext_Enums.ServerCharacterFlags2 = {
	Global = 1,
	TreasureGeneratedForTrader = 2,
	[1] = "Global",
	[2] = "TreasureGeneratedForTrader",
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
	[1] = "Activated",
	[2] = "OffStage",
}

--- @enum ServerItemFlags2
Ext_Enums.ServerItemFlags2 = {
	UnsoldGenerated = 1,
	Global = 2,
	[1] = "UnsoldGenerated",
	[2] = "Global",
}

--- @enum ServerStatusFlags
Ext_Enums.ServerStatusFlags = {
	ForceStatus = 1,
	ForceFailStatus = 2,
	[1] = "ForceStatus",
	[2] = "ForceFailStatus",
}

--- @enum ServerStatusFlags2
Ext_Enums.ServerStatusFlags2 = {
	KeepAlive = 1,
	DontTickWhileOnSurface = 2,
	[1] = "KeepAlive",
	[2] = "DontTickWhileOnSurface",
}

--- @enum ServerStatusFlags3
Ext_Enums.ServerStatusFlags3 = {
	IsUnique = 1,
	NotifiedPlanManager = 2,
	[1] = "IsUnique",
	[2] = "NotifiedPlanManager",
}

--- @enum ServerStatusFlags4
Ext_Enums.ServerStatusFlags4 = {
	BringIntoCombat = 1,
	IsHostileAct = 2,
	[1] = "BringIntoCombat",
	[2] = "IsHostileAct",
}

--- @enum ServerStatusFlags5
Ext_Enums.ServerStatusFlags5 = {
	HasTriedEntering = 1,
	[1] = "HasTriedEntering",
}

--- @enum SetStatusDurationType
Ext_Enums.SetStatusDurationType = {
	ForceSet = 1,
	Add = 2,
	Multiply = 3,
	Unknown = 4,
	[1] = "ForceSet",
	[2] = "Add",
	[3] = "Multiply",
	[4] = "Unknown",
}

--- @enum ShroudType
Ext_Enums.ShroudType = {
	Sight = 1,
	Sneak = 2,
	RegionMask = 3,
	[1] = "Sight",
	[2] = "Sneak",
	[3] = "RegionMask",
}

--- @enum SkillId
Ext_Enums.SkillId = {
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
	DirectHit = 1,
	MeleeWeaponAttack = 2,
	RangedWeaponAttack = 3,
	MeleeOffHandWeaponAttack = 4,
	RangedOffHandWeaponAttack = 5,
	MeleeSpellAttack = 6,
	RangedSpellAttack = 7,
	MeleeUnarmedAttack = 8,
	RangedUnarmedAttack = 9,
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
	Target_OffhandAttack = 1,
	Projectile_MainHandAttack = 2,
	Projectile_OffhandAttack = 3,
	Target_UnarmedAttack = 4,
	[1] = "Target_OffhandAttack",
	[2] = "Projectile_MainHandAttack",
	[3] = "Projectile_OffhandAttack",
	[4] = "Target_UnarmedAttack",
}

--- @enum SpellChildSelectionType
Ext_Enums.SpellChildSelectionType = {
	AddChildren = 1,
	MostPowerful = 2,
	[1] = "AddChildren",
	[2] = "MostPowerful",
}

--- @enum SpellCooldownType
Ext_Enums.SpellCooldownType = {
	OncePerTurn = 1,
	OncePerCombat = 2,
	UntilRest = 3,
	OncePerTurnNoRealtime = 4,
	UntilShortRest = 5,
	UntilPerRestPerItem = 6,
	OncePerShortRestPerItem = 7,
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
	[1] = "HasVerbalComponent",
	[2] = "HasSomaticComponent",
}

--- @enum SpellPrepareType
Ext_Enums.SpellPrepareType = {
	RequiresPreparation = 1,
	Unknown = 2,
	[1] = "RequiresPreparation",
	[2] = "Unknown",
}

--- @enum SpellSchoolId
Ext_Enums.SpellSchoolId = {
	Abjuration = 1,
	Conjuration = 2,
	Divination = 3,
	Enchantment = 4,
	Evocation = 5,
	Illusion = 6,
	Necromancy = 7,
	Transmutation = 8,
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
	[1] = "SlippingImmunity",
	[2] = "Torch",
}

--- @enum StatCharacterFlags
Ext_Enums.StatCharacterFlags = {
	IsPlayer = 1,
	InParty = 2,
	[1] = "IsPlayer",
	[2] = "InParty",
}

--- @enum StatsDeathType
Ext_Enums.StatsDeathType = {
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
	Type1 = 1,
	StatsExpressionVariableDataType = 2,
	StatsExpressionVariableDataModifier = 3,
	RollDefinition = 4,
	ResourceRollDefinition = 5,
	StatsContextType = 6,
	Int = 7,
	Bool = 8,
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
	Skill = 1,
	StatusGroup = 2,
	STDString = 3,
	[1] = "Skill",
	[2] = "StatusGroup",
	[3] = "STDString",
}

--- @enum StatsFunctorFlags
Ext_Enums.StatsFunctorFlags = {
	Self = 1,
	Owner = 2,
	[1] = "Self",
	[2] = "Owner",
}

--- @enum StatsFunctorId
Ext_Enums.StatsFunctorId = {
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
}

--- @enum StatsItemSlot
Ext_Enums.StatsItemSlot = {
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
	Observer = 1,
	Target = 2,
	Source = 3,
	[1] = "Observer",
	[2] = "Target",
	[3] = "Source",
}

--- @enum StatsPropertyContext
Ext_Enums.StatsPropertyContext = {
	TARGET = 1,
	AOE = 2,
	[1] = "TARGET",
	[2] = "AOE",
}

--- @enum StatsRollAdjustmentType
Ext_Enums.StatsRollAdjustmentType = {
	Distribute = 1,
	[1] = "Distribute",
}

--- @enum StatsRollType
Ext_Enums.StatsRollType = {
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
	[1] = "SG_Condition",
	[2] = "SG_Blinded",
}

--- @enum StatsTargetTypeFlags
Ext_Enums.StatsTargetTypeFlags = {
	Living = 1,
	Guaranteed = 2,
	[1] = "Living",
	[2] = "Guaranteed",
}

--- @enum StatsZoneShape
Ext_Enums.StatsZoneShape = {
	Square = 1,
	[1] = "Square",
}

--- @enum StatusHealType
Ext_Enums.StatusHealType = {
	Vitality = 1,
	PhysicalArmor = 2,
	MagicArmor = 3,
	AllArmor = 4,
	All = 5,
	Source = 6,
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
	[1] = "ApplyOnBody",
	[2] = "ApplyOnArmor",
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
	Ignite = 1,
	Douse = 2,
	Electrify = 3,
	Deelectrify = 4,
	Freeze = 5,
	Melt = 6,
	Vaporize = 7,
	Condense = 8,
	DestroyWater = 9,
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
	Cloud = 1,
	None = 2,
	[1] = "Cloud",
	[2] = "None",
}

--- @enum SurfaceLayer8
Ext_Enums.SurfaceLayer8 = {
	Cloud = 1,
	None = 2,
	[1] = "Cloud",
	[2] = "None",
}

--- @enum SurfaceTransformActionType
Ext_Enums.SurfaceTransformActionType = {
	Ignite = 1,
	Douse = 2,
	Electrify = 3,
	Deelectrify = 4,
	Freeze = 5,
	Melt = 6,
	Vaporize = 7,
	Condense = 8,
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
	[1] = "Light",
	[2] = "Ammunition",
}

--- @enum WeaponType
Ext_Enums.WeaponType = {
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

--#region Ext.Osiris

--- @class Ext_Osiris
local Ext_Osiris = {}

--- @alias EventType "before"|"after"|"beforeDelete"|"afterDelete"

--- Used to register a listener of an Osiris event
--- @param name string the name of an Osiris event
--- @param arity number the number of arguments to the handler
--- @param event EventType the type of event to capture
--- @param handler OsiFunction the function to call when the event is triggered
function Ext_Osiris.RegisterListener(name, arity, event, handler) end

--#endregion

--- @class Ext
--- @field Net Ext_ClientNet|Ext_ServerNet
--- @field ClientNet Ext_ClientNet
--- @field Debug Ext_Debug
--- @field Entity Ext_Entity
--- @field IO Ext_IO
--- @field Json Ext_Json
--- @field Loca Ext_Loca
--- @field Math Ext_Math
--- @field Mod Ext_Mod
--- @field Resource Ext_Resource
--- @field ServerNet Ext_ServerNet
--- @field Template Ext_ServerTemplate
--- @field ServerTemplate Ext_ServerTemplate
--- @field StaticData Ext_StaticData
--- @field Stats Ext_Stats
--- @field Types Ext_Types
--- @field Utils Ext_Utils
--- @field Vars Ext_Vars
--- @field Enums Ext_Enums
--- @field Osiris Ext_Osiris
Ext = {Events = {}}


--- @class SubscribableEvent<T>:{ (Subscribe:fun(self:SubscribableEvent, callback:fun(e:T|LuaEventBase), opts:{Priority:integer, Once:boolean}|nil):integer), (Unsubscribe:fun(self:SubscribableEvent, index:integer))}

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

--- @alias NetListenerCallback fun(channel:string, payload:string, user:UserId|nil)

--- Registers a listener that is called when a network message is received on the specified channel
--- @param channel string Network channel name
--- @param handler NetListenerCallback Lua handler
function Ext.RegisterNetListener(channel, handler) end

--- Loads the specified Lua file
--- @param fileName string|nil Path of Lua file, relative to Mods/<Mod>/Story/RawFiles/Lua
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
}

--#endregion

