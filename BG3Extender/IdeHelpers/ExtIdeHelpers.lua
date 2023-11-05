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
--- @alias AbilityId string|"Wisdom"|"Intelligence"|"Constitution"|"Dexterity"|"Strength"|"None"|"Sentinel"|"Charisma"
--- @alias AdvantageBoostType string|"Advantage"|"Disadvantage"
--- @alias AdvantageTypeId string|"Ability"|"SavingThrow"|"DeathSavingThrow"|"Concentration"|"AllSavingThrows"|"Skill"|"AllAbilities"|"AllSkills"|"SourceDialogue"|"AttackTarget"|"AttackRoll"
--- @alias ArmorType string|"Plate"|"ScaleMail"|"Hide"|"ChainMail"|"StuddedLeather"|"Leather"|"Cloth"|"None"|"ChainShirt"|"RingMail"|"Splint"|"Sentinel"|"Padded"|"BreastPlate"|"HalfPlate"
--- @alias AttackRoll string|"Wisdom"|"Intelligence"|"WeaponAttackAbility"|"Constitution"|"Dexterity"|"Strength"|"UnarmedAttackAbility"|"SpellCastingAbility"|"None"|"Charisma"
--- @alias AttributeFlags string|"Arrow"|"ForceMainhandAlternativeEquipBones"|"UseMusicalInstrumentForCasting"|"SlippingImmunity"|"Torch"|"Unbreakable"|"Unrepairable"|"Unstorable"|"Grounded"|"IgnoreClouds"|"LootableWhenEquipped"|"ThrownImmunity"|"InvisibilityImmunity"|"Backstab"|"BackstabImmunity"|"FloatingWhileMoving"|"PickpocketableWhenEquipped"|"LoseDurabilityOnCharacterHit"|"Floating"|"InvulnerableAndInteractive"|"EnableObscurityEvents"|"ObscurityWithoutSneaking"|"InventoryBound"
--- @alias BoostCauseType string|"Stats"|"Status"|"Passive"|"Character"|"Undefined"|"Osiris"|"Unknown5"|"Progression"|"Item"
--- @alias BoostType string|"WeaponDamageDieOverride"|"CarryCapacityMultiplier"|"AiArchetypeOverride"|"ExpertiseBonus"|"RedirectDamage"|"Advantage"|"CannotHarmCauseEntity"|"ProjectileDeflect"|"FallDamageMultiplier"|"WeaponAttackTypeOverride"|"EntityThrowDamage"|"BlockSomaticComponent"|"ProficiencyBonus"|"DualWielding"|"WeaponAttackRollAbilityOverride"|"AdvanceSpells"|"AddProficiencyToAC"|"Proficiency"|"DownedStatus"|"BlockAbilityModifierFromAC"|"WeaponAttackRollBonus"|"ActiveCharacterLight"|"BlockAbilityModifierDamageBonus"|"ObjectSizeOverride"|"UnarmedMagicalProperty"|"DodgeAttackRoll"|"ActionResourceBlock"|"NullifyAbilityScore"|"ArmorAbilityModifierCapOverride"|"AddProficiencyToDamage"|"SourceAllyAdvantageOnAttack"|"Weight"|"Tag"|"ACOverrideFormula"|"NonLethal"|"AC"|"SourceAdvantageOnAttack"|"WeaponProperty"|"Detach"|"ActionResourceReplenishTypeOverride"|"NoAOEDamageOnLand"|"Ability"|"DarkvisionRangeMin"|"WeaponEnchantment"|"ReduceCriticalAttackThreshold"|"ActionResourcePreventReduction"|"ActionResourceOverride"|"DarkvisionRange"|"WeaponDamage"|"MovementSpeedLimit"|"ProficiencyBonusOverride"|"ActionResourceMultiplier"|"MinimumRollResult"|"VoicebarkBlock"|"DamageReduction"|"ActionResourceConsumeMultiplier"|"UseBoosts"|"PhysicalForceRangeBonus"|"DamageBonus"|"MaximumRollResult"|"AbilityOverrideMinimum"|"UnlockSpellVariant"|"MaximizeHealing"|"MonkWeaponDamageDiceOverride"|"CriticalHitExtraDice"|"AbilityFailedSavingThrow"|"UnlockSpell"|"Lootable"|"CriticalDamageOnHit"|"MonkWeaponAttackOverride"|"CriticalHit"|"JumpMaxDistanceMultiplier"|"Lock"|"UnlockInterrupt"|"TwoWeaponFighting"|"ConsumeItemBlock"|"IntrinsicSummonerProficiency"|"LeaveTriggers"|"TemporaryHP"|"CanWalkThrough"|"IntrinsicSourceProficiency"|"JumpMaxDistanceBonus"|"ItemReturnToOwner"|"CanShootThrough"|"IgnorePointBlankDisadvantage"|"ActionResource"|"StatusImmunity"|"Invulnerable"|"CanSeeThrough"|"IgnoreLowGroundPenalty"|"IgnoreLeaveAttackRange"|"Invisibility"|"CannotBeDisarmed"|"SpellSaveDC"|"BlockVerbalComponent"|"WeightCategory"|"Initiative"|"IgnoreEnterAttackRange"|"SpellResistance"|"IncreaseMaxHP"|"IgnoreDamageThreshold"|"BlockTravel"|"SoundsBlocked"|"BlockSpellCast"|"Skill"|"HorizontalFOVOverride"|"IgnoreSurfaceCover"|"BlockRegainHP"|"HiddenDuringCinematic"|"SightRangeOverride"|"ObjectSize"|"IgnoreResistance"|"GuaranteedChanceRollOutcome"|"BlockGatherAtCamp"|"SightRangeMaximum"|"HalveWeaponDamage"|"Attribute"|"SightRangeMinimum"|"EnableBasicItemInteractions"|"SightRangeAdditive"|"DetectDisturbancesBlock"|"GameplayObscurity"|"DarkvisionRangeOverride"|"GameplayLight"|"ScaleMultiplier"|"AttackSpellOverride"|"Savant"|"ConcentrationIgnoreDamage"|"RollBonus"|"Resistance"|"CharacterWeaponDamage"|"WeaponDamageTypeOverride"|"AreaDamageEvade"|"WeaponDamageResistance"|"FactionOverride"|"Reroll"|"CharacterUnarmedDamage"
--- @alias CauseType string|"SurfaceMove"|"SurfaceCreate"|"SurfaceStatus"|"StatusEnter"|"InventoryItem"|"StatusTick"|"Offhand"|"GM"|"WorldItemThrow"|"AURA"|"Attack"|"None"
--- @alias ClientGameState string|"StartServer"|"Unknown"|"Movie"|"Installation"|"ModReceiving"|"Lobby"|"BuildStory"|"GeneratePsoCache"|"LoadPsoCache"|"AnalyticsSessionEnd"|"Init"|"InitMenu"|"LoadModule"|"InitNetwork"|"InitConnection"|"Idle"|"LoadMenu"|"Menu"|"Exit"|"SwapLevel"|"LoadLevel"|"LoadSession"|"UnloadLevel"|"UnloadModule"|"UnloadSession"|"Paused"|"PrepareRunning"|"Running"|"Disconnect"|"Join"|"Save"|"StartLoading"|"StopLoading"
--- @alias CombatParticipantFlags string|"IsBoss"|"IsInspector"|"StayInAiHints"|"CanFight"|"CanJoinCombat"
--- @alias ConditionRollType string|"DifficultyRoll"|"SavingThrowRoll"|"SkillCheckRoll"|"AbilityCheckRoll"|"AttackRoll"
--- @alias CraftingStationType string|"Anvil"|"Oven"|"Wetstone"|"Well"|"BoilingPot"|"Beehive"|"SpinningWheel"|"Cauldron"|"Misc1"|"Misc2"|"None"|"Misc3"|"Misc4"
--- @alias CriticalHitBoostFlags string|"Success"|"SuccessNever"|"FailureNever"|"AttackTarget"|"AttackRoll"|"Failure"
--- @alias DamageFlags string|"Hit"|"Projectile"|"Surface"|"StatusEnter"|"Status"|"Backstab"|"SavingThrow"|"Dodge"|"Magical"|"Miss"|"Invisible"|"Invulnerable"|"HitpointsDamaged"|"AttackAdvantage"|"AttackDisadvantage"|"Critical"
--- @alias DamageType string|"Cold"|"Radiant"|"Psychic"|"Necrotic"|"Piercing"|"Fire"|"Lightning"|"Poison"|"None"|"Bludgeoning"|"Slashing"|"Acid"|"Thunder"|"Sentinel"|"Force"
--- @alias DealDamageWeaponDamageType string|"OffhandWeaponDamageType"|"MainMeleeWeaponDamageType"|"OffhandMeleeWeaponDamageType"|"MainRangedWeaponDamageType"|"OffhandRangedWeaponDamageType"|"SourceWeaponDamageType"|"ThrownWeaponDamageType"|"None"|"MainWeaponDamageType"
--- @alias DealDamageWeaponType string|"SourceWeapon"|"UnarmedDamage"|"MainWeapon"|"OffhandWeapon"|"MainMeleeWeapon"|"OffhandMeleeWeapon"|"MainRangedWeapon"|"OffhandRangedWeapon"|"ThrownWeapon"|"None"|"ImprovisedWeapon"
--- @alias DiceSizeId string|"D8"|"D10"|"D12"|"D20"|"D100"|"Default"|"D4"|"D6"
--- @alias ESurfaceFlag string|"Blessed"|"Cursed"|"Purified"|"CloudBlessed"|"CloudCursed"|"CloudPurified"|"Water"|"Electrified"|"HasInteractableObject"|"CloudElectrified"|"ElectrifiedDecay"|"Blood"|"SomeDecay"|"Irreplaceable"|"IrreplaceableCloud"|"Lava"|"Oil"|"Frozen"|"Fire"|"MovementBlock"|"WaterCloud"|"ProjectileBlock"|"Poison"|"PoisonCloud"|"HasCharacter"|"ExplosionCloud"|"HasItem"|"ShockwaveCloud"|"GroundSurfaceBlock"|"CloudSurfaceBlock"|"Occupied"|"BloodCloud"|"SurfaceExclude"|"Source"|"Web"|"Sulfurium"|"Deepwater"|"FireCloud"|"SmokeCloud"|"FrostCloud"|"Deathfog"
--- @alias EquipmentStatsType string|"Armor"|"Shield"|"Weapon"
--- @alias ExecuteWeaponFunctorsType string|"OffHand"|"MainHand"|"BothHands"|"Undefined"
--- @alias ExtComponentType string|"Classes"|"SpellCastRolls"|"ServerInterruptZoneRequests"|"OriginPassives"|"AttitudesToPlayers"|"IncreaseMaxHPBoost"|"DarkvisionRangeMinBoost"|"CanWalkThroughBoost"|"MonkWeaponDamageDiceOverrideBoost"|"Armor"|"OffStage"|"SpellCastState"|"ServerInterruptInitialParticipants"|"ClassTag"|"ServerRecruitedBy"|"StatusImmunityBoost"|"DarkvisionRangeOverrideBoost"|"HorizontalFOVOverrideBoost"|"LockBoost"|"PickingState"|"Speaker"|"SpellSyncTargeting"|"ServerInterruptTurnOrderInZone"|"Icon"|"BackgroundTag"|"DualWielding"|"ServerGameTimer"|"TemporaryHPBoost"|"IgnoreDamageThresholdMinBoost"|"CharacterUnarmedDamageBoost"|"DodgeAttackRollBoost"|"Data"|"Level"|"SimpleCharacter"|"ServerSpellExternals"|"CCCompanionDefinition"|"BackgroundPassives"|"Active"|"ServerIsUnsummoning"|"WeightBoost"|"WeaponEnchantmentBoost"|"ActionResourceReplenishTypeOverrideBoost"|"ActionResourcePreventReductionBoost"|"UnlockInterruptBoost"|"WeaponSet"|"ServerSpellCastState"|"CCFullRespecDefinition"|"FleeCapability"|"ActiveCharacterLight"|"ServerAnubisTag"|"WeightCategoryBoost"|"GuaranteedChanceRollOutcomeBoost"|"AttackSpellOverrideBoost"|"Uuid"|"Stats"|"DifficultyCheck"|"InterruptActionState"|"CCCharacterDefinition"|"CanDoRest"|"Race"|"ServerDialogTag"|"FactionOverrideBoost"|"MinimumRollResultBoost"|"IgnorePointBlankDisadvantageBoost"|"UuidToHandleMapping"|"Tag"|"BaseStats"|"InterruptContainer"|"CharacterCreationStats"|"Stealth"|"IsInTurnBasedMode"|"ServerIconList"|"InitiativeBoost"|"CharacterWeaponDamageBoost"|"CriticalHitExtraDiceBoost"|"Expertise"|"InterruptDecision"|"GlobalShortRestDisabled"|"ItemBoosts"|"ServerRaceTag"|"DarkvisionRangeBoost"|"ProjectileDeflectBoost"|"GameplayObscurityBoost"|"Origin"|"HealBlock"|"InterruptPreferences"|"GlobalLongRestDisabled"|"AnimationBlueprint"|"Steering"|"ServerTemplateTag"|"AddTagBoost"|"AbilityOverrideMinimumBoost"|"MaximumRollResultBoost"|"InventoryMember"|"InterruptPrepared"|"IsGlobal"|"StoryShortRestDisabled"|"CanModifyHealth"|"InteractionFilter"|"ServerBoostTag"|"SkillBoost"|"ACOverrideFormulaBoost"|"JumpMaxDistanceBonusBoost"|"InventoryData"|"InterruptZone"|"AvailableLevel"|"CanTriggerRandomCasts"|"ServerSafePosition"|"WeaponDamageBoost"|"FallDamageMultiplierBoost"|"ArmorAbilityModifierCapOverrideBoost"|"InventoryOwner"|"InterruptZoneSource"|"Savegame"|"CanBeLooted"|"GravityDisabledUntilMoved"|"ServerAnubisExecutor"|"UseBoosts"|"NullifyAbilityBoost"|"ActiveCharacterLightBoost"|"IgnoreResistanceBoost"|"InventoryContainer"|"CCDefinitionCommon"|"LevelUp"|"CanBeDisarmed"|"CanDeflectProjectiles"|"ServerLeader"|"RerollBoost"|"WeaponAttackTypeOverrideBoost"|"ConcentrationIgnoreDamageBoost"|"Experience"|"SurfacePathInfluences"|"IsInCombat"|"LearnedSpells"|"CCRespecDefinition"|"CanDoActions"|"Movement"|"CharacterCreationAppearance"|"ServerBreadcrumb"|"DownedStatusBoost"|"WeaponDamageDieOverrideBoost"|"IgnoreLowGroundPenaltyBoost"|"Passive"|"Value"|"ActionResourceConsumeMultiplierBoost"|"Player"|"CombatParticipant"|"CCSessionCommon"|"Voice"|"Lock"|"CanMove"|"ServerExperienceGaveOut"|"ServerPickpocket"|"AttributeBoost"|"CarryCapacityMultiplierBoost"|"IgnoreSurfaceCoverBoost"|"Health"|"MaterialParameterOverride"|"CombatState"|"CCState"|"ShortRest"|"CanSense"|"ServerReplicationDependency"|"ServerCharacter"|"GameplayLightBoost"|"WeaponAttackRollAbilityOverrideBoost"|"TurnBased"|"ServerCombatGroupMapping"|"CCLevelUpDefinition"|"God"|"Darkness"|"ServerActivationGroupContainer"|"ServerItem"|"DualWieldingBoost"|"SightRangeAdditiveBoost"|"ActionResources"|"Transform"|"TurnOrder"|"SpellModificationContainer"|"CCFullRespec"|"GameObjectVisual"|"Physics"|"ServerDisplayNameList"|"ServerProjectile"|"SavantBoost"|"SightRangeMinimumBoost"|"Max"|"SpellBook"|"ServerCanStartCombat"|"SpellCastAnimationInfo"|"CCRespec"|"IsSummon"|"Visual"|"AppearanceOverride"|"ServerToggledPassives"|"ServerOsirisTag"|"WeaponPropertyBoost"|"SightRangeMaximumBoost"|"StatusImmunities"|"ServerEnterRequest"|"SpellCastCanBeTargeted"|"CCCreation"|"Invisibility"|"Concentration"|"GravityDisabled"|"ServerDelayDeathCause"|"ClientCharacter"|"SightRangeOverrideBoost"|"ScaleMultiplierBoost"|"Use"|"BodyType"|"ServerFleeBlocked"|"SpellCastInterruptResults"|"CCLevelUp"|"Equipable"|"ServerReplicationDependencyOwner"|"ClientItem"|"MovementSpeedLimitBoost"|"DamageReductionBoost"|"Wielding"|"ServerImmediateJoin"|"ServerSpellCastHitDelay"|"Floating"|"CustomIcon"|"ProgressionContainer"|"ClientProjectile"|"ActionResourceValueBoost"|"UnlockSpellVariantBoost"|"ObjectSizeBoost"|"CustomStats"|"SpellContainer"|"ServerSpellCastResponsible"|"Disarmable"|"ProgressionMeta"|"ArmorClassBoost"|"AbilityFailedSavingThrowBoost"|"DetectCrimesBlockBoost"|"ExpertiseBonusBoost"|"Weapon"|"BoostCondition"|"ObjectSize"|"PlayerPrepareSpell"|"ServerSpellClientInitiated"|"Background"|"SummonContainer"|"CanTravel"|"AbilityBoost"|"WeaponDamageResistanceBoost"|"BlockAbilityModifierFromACBoost"|"MaximizeHealingBoost"|"BaseHp"|"BoostsContainer"|"CCPrepareSpell"|"ServerSpellHitRegister"|"ClientControl"|"AnimationSet"|"ObjectInteraction"|"RollBonusBoost"|"ProficiencyBonusOverrideBoost"|"ReduceCriticalAttackThresholdBoost"|"DamageBonusBoost"|"SpellBookPrepares"|"AddedSpells"|"ServerSpellInterruptRequests"|"DisabledEquipment"|"StaticPhysics"|"AdvantageBoost"|"JumpMaxDistanceMultiplierBoost"|"PhysicalForceRangeBonusBoost"|"AdvanceSpellsBoost"|"PassiveContainer"|"AttributeFlags"|"SpellBookCooldowns"|"ServerSpellInterruptResults"|"LootingState"|"Pathing"|"CriticalHitBoost"|"HalveWeaponDamageBoost"|"ObjectSizeOverrideBoost"|"SpellResistanceBoost"|"BoostInfo"|"SpellAiConditions"|"InterruptConditionallyDisabled"|"Loot"|"DisplayName"|"GameplayLight"|"Sight"|"ActiveSkeletonSlots"|"ResistanceBoost"|"ProficiencyBonusBoost"|"AiArchetypeOverrideBoost"|"SpellSaveDCBoost"|"Hearing"|"Relation"|"CanInteract"|"SpellCastCache"|"InterruptData"|"InterruptZoneParticipant"|"SummonLifetime"|"Net"|"UnlockSpellBoost"|"ActionResourceBlockBoost"|"EntityThrowDamageBoost"|"RedirectDamageBoost"|"CanSpeak"|"SpellCastIsCasting"|"ServerInterruptAddRemoveRequests"|"HotbarContainer"|"FTBParticipant"|"SourceAdvantageBoost"|"CannotHarmCauseEntityBoost"|"WeaponDamageTypeOverrideBoost"|"CanSeeThroughBoost"|"EocLevel"|"SpellCastMovement"|"ServerInterruptActionRequests"|"OriginTag"|"ApprovalRatings"|"ProficiencyBoost"|"ActionResourceMultiplierBoost"|"WeaponAttackRollBonusBoost"|"CanShootThroughBoost"
--- @alias ExtQueryType string|"UuidToHandleMapping"|"Max"
--- @alias ExtResourceManagerType string|"Gossip"|"CharacterCreationIconSettings"|"Faction"|"ClassDescription"|"CharacterCreationAppearanceVisual"|"Race"|"AbilityList"|"Tag"|"CharacterCreationAppearanceMaterial"|"Origin"|"CharacterCreationAccessorySet"|"SpellList"|"ProgressionDescription"|"EquipmentType"|"CompanionPreset"|"ActionResourceGroup"|"AbilityDistributionPreset"|"VFX"|"TutorialEntries"|"ColorDefinition"|"God"|"TutorialModalEntries"|"Max"|"ActionResource"|"CharacterCreationSkinColor"|"Flag"|"CharacterCreationSharedVisual"|"Background"|"CharacterCreationPassiveAppearance"|"CharacterCreationPreset"|"CharacterCreationMaterialOverride"|"Progression"|"SkillList"|"PassiveList"|"CharacterCreationHairColor"|"FeatDescription"|"Feat"|"CharacterCreationEyeColor"|"CharacterCreationEquipmentIcons"
--- @alias FleeErrorFlags string|"Summon"|"Condition"|"EnemyTooClose"|"CantMove"|"Downed"|"Incapacitated"|"PartyFollower"|"Region"
--- @alias ForceFunctorAggression string|"Undefined"|"Aggressive"|"Friendly"
--- @alias ForceFunctorOrigin string|"Undefined"|"OriginToEntity"|"OriginToTarget"|"TargetToEntity"
--- @alias FunctorExecParamsType string|"Type1"|"Type2"|"Type3"|"Type4"|"Type5"|"Type6"|"Type7"|"Type8"
--- @alias GameActionType string|"RainAction"|"StormAction"|"WallAction"|"TornadoAction"|"PathAction"|"GameObjectMoveAction"|"StatusDomeAction"
--- @alias Gender string|"Male"|"Female"
--- @alias HandednessType string|"Two"|"One"|"Any"
--- @alias HealDirection string|"Incoming"|"Outgoing"
--- @alias HealEffect string|"Lifesteal"|"NegativeDamage"|"Unknown9"|"Surface"|"HealSharing"|"Heal"|"Necromantic"|"Script"|"HealSharingReflected"|"None"|"ResistDeath"|"Behavior"|"Unknown4"|"Sitting"
--- @alias HitType string|"Surface"|"WeaponDamage"|"Ranged"|"DoT"|"Magic"|"Melee"|"Reflected"
--- @alias HitWith string|"FallDamage"|"Trap"|"CrushByFall"|"Unknown"|"Unknown10"|"Projectile"|"Surface"|"Unknown11"|"None"|"Weapon"|"Item"|"Magic"|"Redirection"
--- @alias IngredientTransformType string|"Boost"|"Transform"|"Poison"|"None"|"Consume"
--- @alias IngredientType string|"Property"|"Category"|"None"|"Object"
--- @alias InputType string|"Unknown"|"Press"|"Release"|"ValueChange"|"Hold"|"Repeat"|"AcceleratedRepeat"
--- @alias ItemDataRarity string|"Unique"|"Legendary"|"Sentinel"|"Rare"|"Uncommon"|"Common"|"Epic"|"Divine"
--- @alias ItemSlot string|"VanityBoots"|"VanityBody"|"Helmet"|"Breast"|"Cloak"|"MeleeMainHand"|"OffHand"|"MeleeOffHand"|"MainHand"|"RangedMainHand"|"RangedOffHand"|"Ring"|"Boots"|"Amulet"|"Ring2"|"Overhead"|"Wings"|"Horns"|"MusicalInstrument"|"Underwear"|"Gloves"
--- @alias LuaTypeId string|"Function"|"Unknown"|"String"|"Array"|"Module"|"Void"|"Boolean"|"Integer"|"Set"|"Map"|"Tuple"|"Enumeration"|"Float"|"Object"|"Nullable"|"Variant"|"Any"
--- @alias NetMessage string|"NETMSG_LEVEL_LOAD"|"NETMSG_CHARACTER_CONFIRMATION"|"NETMSG_TURNBASED_FINISHTEAM"|"NETMSG_TADPOLE_POWERS_REMOVE"|"NETMSG_QUEST_STEP"|"NETMSG_MYSTERY_ADVANCED"|"NETMSG_TRIGGER_UPDATE"|"NETMSG_SNEAKING_CONES_VISIBLE_TOGGLE"|"NETMSG_INVENTORY_CREATE_NEW"|"NETMSG_SELECT_DICE"|"NETMSG_LEVEL_CREATED"|"NETMSG_CHARACTER_ACTIVATE"|"NETMSG_TURNBASED_SETTEAM"|"NETMSG_GAMECONTROL_UPDATE_S2C"|"NETMSG_QUESTS_LOADED"|"NETMSG_MYSTERY_DISABLED"|"NETMSG_CUSTOM_STATS_CREATE"|"NETMSG_HOTBAR_SLOT_SET"|"NETMSG_INVENTORY_DESTROY_NEW"|"NETMSG_COMBINE_ITEMS"|"NETMSG_LEVEL_LOADED"|"NETMSG_CHARACTER_DEACTIVATE"|"NETMSG_TURNBASED_FLEECOMBATRESULT"|"NETMSG_GAMECONTROL_UPDATE_C2S"|"NETMSG_TROPHY_UPDATE"|"NETMSG_REGISTER_WAYPOINT"|"NETMSG_CUSTOM_STATS_UPDATE"|"NETMSG_HOTBAR_COLUMN_SET"|"NETMSG_INVENTORY_MOVETOSLOT"|"NETMSG_LOAD_START"|"NETMSG_CHARACTER_ASSIGN"|"NETMSG_TURNBASED_FLEE_REQUEST"|"NETMSG_GAMECONTROL_PRICETAG"|"NETMSG_QUEST_TRACK"|"NETMSG_UNLOCK_WAYPOINT"|"NETMSG_LOAD_GAME_WITH_ADDONS"|"NETMSG_HOTBAR_ROWS_SET"|"NETMSG_INVENTORY_TRANSFER"|"NETMSG_LOAD_STARTED"|"NETMSG_CHARACTER_CHANGE_OWNERSHIP"|"NETMSG_CHARACTER_ACTION_REQUEST_RESULT"|"NETMSG_TURNBASED_ENDTURN_REQUEST"|"NETMSG_CHANGE_RULESETS"|"NETMSG_NOTIFICATION"|"NETMSG_LIGHTING_OVERRIDE"|"NETMSG_LOAD_GAME_WITH_ADDONS_FAIL"|"NETMSG_HOTBAR_ROW_REMOVE"|"NETMSG_INVENTORY_SORT"|"NETMSG_LEVEL_START"|"NETMSG_CHARACTER_STEERING"|"NETMSG_TURNBASED_CANCEL_ENDTURN_REQUEST"|"NETMSG_TURNBASED_SKIP_START_DELAY"|"NETMSG_REALTIME_MULTIPLAY"|"NETMSG_LOCK_WAYPOINT"|"NETMSG_ATMOSPHERE_OVERRIDE"|"NETMSG_CLIENT_GAME_SETTINGS"|"NETMSG_HOTBAR_LOCK_SET"|"NETMSG_INVENTORY_EQUIP"|"NETMSG_CHARACTER_MOVEMENT_FALLING"|"NETMSG_CHARACTER_AOO"|"NETMSG_TURNBASED_BEGIN_CANCEL_ENDTURN_REQUEST"|"NETMSG_UI_FORCETURNBASED_ENTERED"|"NETMSG_CHARACTER_ERROR"|"NETMSG_FLAG_UPDATE"|"NETMSG_CAMERA_ACTIVATE"|"NETMSG_TIMELINE_HANDSHAKE"|"NETMSG_HOTBAR_CUSTOM_DECK"|"NETMSG_INVENTORY_UNEQUIP"|"NETMSG_CHARACTER_ACTION"|"NETMSG_PARTYGROUP"|"NETMSG_TURNBASED_END_CANCEL_ENDTURN_REQUEST"|"NETMSG_UI_FORCETURNBASED_LEFT"|"NETMSG_REQUESTAUTOSAVE"|"NETMSG_CAMERA_ROTATE"|"NETMSG_ACHIEVEMENT_UNLOCKED_MESSAGE"|"NETMSG_TIMELINE_ACTOR_HANDSHAKE"|"NETMSG_HOTBAR_CURRENT_DECK_SET"|"NETMSG_INVENTORY_LOCK"|"NETMSG_CHARACTER_ACTION_DATA"|"NETMSG_PARTYORDER"|"NETMSG_TURNBASED_STARTTURN_CONFIRMATION"|"NETMSG_UI_FORCETURNBASED_TURN_ENDED"|"NETMSG_SAVEGAMEHANDSHAKE"|"NETMSG_CAMERA_TARGET"|"NETMSG_SAVEGAME_LOAD_FAIL"|"NETMSG_TIMELINE_NODECOMPLETED"|"NETMSG_PARTY_PRESET_SAVE"|"NETMSG_INTERRUPT_DECISION"|"NETMSG_CHARACTER_STATUS"|"NETMSG_UI_FORCETURNBASED_TURN_STARTED"|"NETMSG_ITEM_CREATE"|"NETMSG_ITEM_DEACTIVATE"|"NETMSG_SAVEGAMEHANDSHAKE_SCREENSHOT"|"NETMSG_SHROUD_UPDATE"|"NETMSG_SERVER_COMMAND"|"NETMSG_TIMELINE_PLAYER_WATCHING"|"NETMSG_PARTY_PRESET_LOAD"|"NETMSG_INTERRUPT_SET_ASK_DATA"|"NETMSG_CHARACTER_STATUS_LIFETIME"|"NETMSG_ITEM_DESTROY"|"NETMSG_ITEM_DESTINATION"|"NETMSG_EFFECT_DESTROY"|"NETMSG_PLAYMOVIE"|"NETMSG_SERVER_NOTIFICATION"|"NETMSG_DIALOG_ANSWER_HIGHLIGHT_MESSAGE"|"NETMSG_TIMELINE_REQUEST_FASTFORWARD"|"NETMSG_PARTY_PRESET_LEVELUP"|"NETMSG_INTERRUPT_SET_ENABLED_DATA"|"NETMSG_CHARACTER_DIALOG"|"NETMSG_ITEM_ACTIVATE"|"NETMSG_ITEM_USE_REMOTELY"|"NETMSG_COMBATLOGITEMINTERACTION"|"NETMSG_TRADE_ACTION"|"NETMSG_STORY_ELEMENT_UI"|"NETMSG_DIALOG_REQUEST_IN_DIALOGUE_ATTACK_MESSAGE"|"NETMSG_CLIENT_DIALOG_PRIVACY_CHANGED"|"NETMSG_PARTY_PRESET_SPELL"|"NETMSG_CAMERA_ARRIVED"|"NETMSG_CHARACTER_USE_MOVEMENT"|"NETMSG_ITEM_UPDATE"|"NETMSG_ITEM_MOVE_TO_WORLD"|"NETMSG_COMBATLOGENTRIES"|"NETMSG_GAMETIME_SYNC"|"NETMSG_ACHIEVEMENT_PROGRESS_MESSAGE"|"NETMSG_DIALOG_TIMELINE_UNLOADED_MESSAGE"|"NETMSG_REQUESTED_ROLL"|"NETMSG_STORY_FLAGS_LOAD"|"NETMSG_CLEAR_RECIPE_NEW_FLAG"|"NETMSG_CHARACTER_MOVE_FAILED"|"NETMSG_ITEM_ACTION"|"NETMSG_ITEM_STATUS_LIFETIME"|"NETMSG_SHOW_ENTER_REGION_UI_MESSAGE"|"NETMSG_TELEPORT_ACK"|"NETMSG_COMBINE_RESULT"|"NETMSG_CUSTOM_STATS_DEFINITION_CREATE"|"NETMSG_PASSIVE_ROLL_SEQUENCE"|"NETMSG_ACTIVE_ROLL_MODIFIERS"|"NETMSG_NEW_ITEMS_INSIDE"|"NETMSG_HANDSHAKE"|"NETMSG_CHARACTER_SELECTED_CLIMB_ON"|"NETMSG_ITEM_STATUS"|"NETMSG_ITEM_TRANSFORM"|"NETMSG_CHARACTER_TELEPORT"|"NETMSG_UNLOCK_ITEM"|"NETMSG_DIALOG_STATE_MESSAGE"|"NETMSG_CUSTOM_STATS_DEFINITION_REMOVE"|"NETMSG_CHARACTER_PATHING"|"NETMSG_ITEM_TOGGLE_IS_WARE"|"NETMSG_SCRIPT_EXTENDER"|"NETMSG_HOST_WELCOME"|"NETMSG_HOST_REFUSEPLAYER"|"NETMSG_FOLLOWER_CANT_USE_ITEM"|"NETMSG_ITEM_CONFIRMATION"|"NETMSG_ITEM_OFFSTAGE"|"NETMSG_SCREEN_FADE_FINISHED"|"NETMSG_DIALOG_NODE_MESSAGE"|"NETMSG_DIALOG_LISTEN"|"NETMSG_CUSTOM_STATS_DEFINITION_UPDATE"|"NETMSG_CHARACTER_CREATION_ABORT"|"NETMSG_LONG_REST_DECISION"|"NETMSG_HOST_REFUSE"|"NETMSG_CLIENT_CONNECT"|"NETMSG_CHARACTER_UPDATE"|"NETMSG_ITEM_MOVED_INFORM"|"NETMSG_SURFACE_META"|"NETMSG_OPEN_CUSTOM_BOOK_UI_MESSAGE"|"NETMSG_DIALOG_ANSWER_MESSAGE"|"NETMSG_CAMERA_SPLINE"|"NETMSG_TIMELINE_WORLD_CINEMATIC_COMPLETED"|"NETMSG_CHARACTER_CREATION_READY"|"NETMSG_REQUEST_RELEASE_CONCENTRATION"|"NETMSG_CLIENT_JOINED"|"NETMSG_PLAYER_CONNECT"|"NETMSG_CHARACTER_CONTROL"|"NETMSG_INVENTORY_CREATE"|"NETMSG_GAMEACTION"|"NETMSG_CLOSE_CUSTOM_BOOK_UI_MESSAGE"|"NETMSG_DIALOG_HISTORY_MESSAGE"|"NETMSG_MUSIC_EVENT"|"NETMSG_CLIENT_DIALOG_JOIN_PRIVACY_FAILED"|"NETMSG_CHARACTER_CREATION_UPDATE"|"NETMSG_ROLL_STREAM_ROLL_MODE_TYPE"|"NETMSG_CLIENT_LEFT"|"NETMSG_PLAYER_DISCONNECT"|"NETMSG_CHARACTER_TRANSFORM"|"NETMSG_INVENTORY_CREATE_AND_OPEN"|"NETMSG_GAMEOVER"|"NETMSG_OPEN_MESSAGE_BOX_MESSAGE"|"NETMSG_DIALOG_ACTORJOINS_MESSAGE"|"NETMSG_MUSIC_STATE"|"NETMSG_FORCE_TURN_BASED_END_PLAYER_TURN_REQUEST"|"NETMSG_LEVEL_UP_UPDATE"|"NETMSG_CHARACTER_DROP_ENTITY"|"NETMSG_HOST_LEFT"|"NETMSG_PLAYER_CHANGENAME"|"NETMSG_CHARACTER_PICKPOCKET"|"NETMSG_INVENTORY_DESTROY"|"NETMSG_ACT_OVER"|"NETMSG_CLOSED_MESSAGE_BOX_MESSAGE"|"NETMSG_DIALOG_ACTORLEAVES_MESSAGE"|"NETMSG_PAUSE"|"NETMSG_FORCE_TURN_BASED_TOGGLE_REQUEST"|"NETMSG_FULL_RESPEC_UPDATE"|"NETMSG_CHARACTER_CANCEL_DROP_ENTITY"|"NETMSG_CLIENT_ACCEPT"|"NETMSG_PLAYER_NAMECHANGED"|"NETMSG_CHARACTER_OFFSTAGE"|"NETMSG_INVENTORY_VIEW_CREATE"|"NETMSG_SPELL_LEARN"|"NETMSG_OPEN_WAYPOINT_UI_MESSAGE"|"NETMSG_DIALOG_REPLACESPEAKER_MESSAGE"|"NETMSG_UNPAUSE"|"NETMSG_CHARACTER_REQUEST_WEAPON_SET_SWITCH"|"NETMSG_CHARACTER_CREATION_LEVELUP"|"NETMSG_CHARACTER_LOCKPICK"|"NETMSG_PLAYER_JOINED"|"NETMSG_ECS_REPLICATION"|"NETMSG_CHARACTER_IN_DIALOG"|"NETMSG_INVENTORY_VIEW_DESTROY"|"NETMSG_SPELL_PREPARE"|"NETMSG_CLOSE_UI_MESSAGE"|"NETMSG_DIALOG_INVALID_ANSWER"|"NETMSG_READYCHECK"|"NETMSG_CHARACTER_REQUEST_ARMOR_SET_SWITCH"|"NETMSG_CHARACTER_CREATION_RESPEC"|"NETMSG_CHARACTER_TRAPDISARM"|"NETMSG_PLAYER_ACCEPT"|"NETMSG_SKIPMOVIE_RESULT"|"NETMSG_CHARACTER_LOOT"|"NETMSG_INVENTORY_VIEW_UPDATE_ITEMS"|"NETMSG_SPELL_CANCEL"|"NETMSG_OPEN_CRAFT_UI_MESSAGE"|"NETMSG_DIALOG_SUGGESTANSWER_MESSAGE"|"NETMSG_DIPLOMACY"|"NETMSG_CHARACTER_CREATION_PLAYORIGINTIMELINE"|"NETMSG_UI_INTERACTION_STOPPED"|"NETMSG_CHARACTER_IMPROV_WEAPON"|"NETMSG_PLAYER_LEFT"|"NETMSG_PEER_DEACTIVATE"|"NETMSG_CHARACTER_ITEM_USED"|"NETMSG_INVENTORY_VIEW_UPDATE_PARENTS"|"NETMSG_SHOW_ERROR"|"NETMSG_TELEPORT_WAYPOINT"|"NETMSG_DIALOG_INSTANCEDIALOGCHANGED"|"NETMSG_PING_REQUEST"|"NETMSG_PASSIVE_TOGGLE"|"NETMSG_HOTBAR_SLOT_REMOVE_IS_NEW_FLAG"|"NETMSG_CHARACTER_STOWED_VISIBILITY"|"NETMSG_VOICEDATA"|"NETMSG_SESSION_LOADED"|"NETMSG_CHARACTER_UNSHEATHING"|"NETMSG_INVENTORY_VIEW_SORT"|"NETMSG_SAVEGAME"|"NETMSG_QUEST_CATEGORY_UPDATE"|"NETMSG_LOBBY_DATAUPDATE"|"NETMSG_GIVE_REWARD"|"NETMSG_DUALWIELDING_TOGGLE"|"NETMSG_DECLUTTERED_ITEMS"|"NETMSG_CHARACTER_ORIGINAL_TEMPLATE_UPDATE"|"NETMSG_MIC_DISABLED"|"NETMSG_SESSION_UNLOADED"|"NETMSG_CHARACTER_END_REPOSE"|"NETMSG_INVENTORY_ITEM_UPDATE"|"NETMSG_EFFECT_CREATE"|"NETMSG_QUEST_PROGRESS"|"NETMSG_LOBBY_USERUPDATE"|"NETMSG_DLC_UPDATE"|"NETMSG_UI_COMBINE_OPEN"|"NETMSG_SPELL_CAST_START_PREVIEW"|"NETMSG_CHAT"|"NETMSG_LEVEL_INSTANTIATE_SWAP"|"NETMSG_CHARACTER_BEHAVIOR"|"NETMSG_INVENTORY_LOCKSTATE_SYNC"|"NETMSG_EFFECT_FORGET"|"NETMSG_JOURNALRECIPE_UPDATE"|"NETMSG_LOBBY_STARTGAME"|"NETMSG_RESPEC_UPDATE"|"NETMSG_TUTORIAL_CLOSED"|"NETMSG_SPELL_CAST_UPDATE_PREVIEW"|"NETMSG_PEER_ACTIVATE"|"NETMSG_LEVEL_SWAP_READY"|"NETMSG_PARTY_BLOCKFOLLOW"|"NETMSG_SURFACE_CREATE"|"NETMSG_CACHETEMPLATE"|"NETMSG_MARKER_UI_UPDATE"|"NETMSG_NET_ENTITY_CREATE"|"NETMSG_TUTORIAL_UI_EVENT"|"NETMSG_CAST_ALL"|"NETMSG_SPELL_CAST_CONFIRM"|"NETMSG_MODULE_LOAD"|"NETMSG_LEVEL_SWAP_COMPLETE"|"NETMSG_PROJECTILE_CREATE"|"NETMSG_DARKNESSTILE_UPDATE"|"NETMSG_OVERHEADTEXT"|"NETMSG_MARKER_UI_CREATE"|"NETMSG_NET_ENTITY_DESTROY"|"NETMSG_TUTORIAL_USER_ACTION"|"NETMSG_END_THE_DAY"|"NETMSG_SPELL_CAST_CANCEL"|"NETMSG_MODULE_LOADED"|"NETMSG_CHARACTER_CREATE"|"NETMSG_PROJECTILE_UPDATE"|"NETMSG_SPELL_REMOVE_LEARNED"|"NETMSG_COMBATLOG"|"NETMSG_JOURNALDIALOGLOG_UPDATE"|"NETMSG_TRIGGER_CREATE"|"NETMSG_TUTORIALS_SHOWING"|"NETMSG_SHORT_REST"|"NETMSG_SPELL_CAST_CAST"|"NETMSG_SESSION_LOAD"|"NETMSG_CHARACTER_DESTROY"|"NETMSG_MULTIPLE_TARGET_OPERATION"|"NETMSG_TADPOLE_POWER_ADD"|"NETMSG_JOURNAL_RESET"|"NETMSG_UI_QUESTSELECTED"|"NETMSG_TRIGGER_DESTROY"|"NETMSG_TUTORIALS_RESET"|"NETMSG_USE_ITEM_ACTION"|"NETMSG_INVENTORY_ADD"
--- @alias PathRootType string|"Localization"|"Bin"|"Debug"|"Bin2"|"Data"|"Projects"|"Public2"|"Scripts"|"GameMod"|"EngineMod"|"WorkingDir"|"Public"|"Mods"|"Root"|"LocalAppData"|"UserProfile"
--- @alias ProficiencyBonusBoostType string|"Ability"|"SavingThrow"|"WeaponActionDC"|"AllSavingThrows"|"Skill"|"AllAbilities"|"AllSkills"|"SourceDialogue"|"AttackTarget"|"AttackRoll"
--- @alias ProficiencyGroupFlags string|"Glaives"|"Tridents"|"Greataxes"|"Warhammers"|"Greatclubs"|"Warpicks"|"Greatswords"|"Halberds"|"HandCrossbows"|"Handaxes"|"HeavyArmor"|"HeavyCrossbows"|"Javelins"|"LightArmor"|"LightCrossbows"|"LightHammers"|"Longbows"|"Longswords"|"Maces"|"MartialWeapons"|"Mauls"|"MediumArmor"|"Morningstars"|"MusicalInstrument"|"Pikes"|"Quarterstaffs"|"Rapiers"|"Scimitars"|"Shields"|"Shortbows"|"Battleaxes"|"Shortswords"|"Clubs"|"Sickles"|"Daggers"|"SimpleWeapons"|"Darts"|"Slings"|"Flails"|"Spears"
--- @alias ProjectileTypeIds string|"Physical"|"Magical"
--- @alias RecipeCategory string|"Weapons"|"Potions"|"Grenades"|"Arrows"|"Armour"|"Food"|"Objects"|"Runes"|"Grimoire"|"Common"
--- @alias RequirementType string|"Wisdom"|"Intelligence"|"Combat"|"Constitution"|"Level"|"Dexterity"|"Strength"|"Tag"|"Persuasion"|"Intimidate"|"Ranged"|"Reason"|"Pickpocket"|"TurnBased"|"PainReflection"|"Reflexes"|"None"|"Sourcery"|"Necromancy"|"Telekinesis"|"Summoning"|"Sneaking"|"Loremaster"|"Barter"|"Charm"|"Immobile"|"Vitality"
--- @alias ResistanceBoostFlags string|"ResistantToNonMagical"|"VulnerableToNonMagical"|"BelowDamageThreshold"|"ResistantToMagical"|"ImmuneToMagical"|"VulnerableToMagical"|"ImmuneToNonMagical"
--- @alias ResourceBankType string|"Animation"|"VoiceBark"|"TileSet"|"DiffusionProfile"|"Script"|"MeshProxy"|"MaterialSet"|"MaterialPreset"|"Atmosphere"|"AnimationBlueprint"|"BlendSpace"|"FCurve"|"IKRig"|"Skeleton"|"VirtualTexture"|"TerrainBrush"|"ColorList"|"CharacterVisual"|"Texture"|"Timeline"|"SkinPreset"|"ClothCollider"|"Material"|"Physics"|"Effect"|"LightCookie"|"Visual"|"TimelineScene"|"SkeletonMirrorTable"|"Dialog"|"Sentinel"|"VisualSet"|"AnimationSet"|"Sound"|"Lighting"
--- @alias ResourceReplenishType string|"Combat"|"Never"|"Rest"|"ExhaustedRest"|"FullRest"|"Default"|"ShortRest"
--- @alias RestErrorFlags string|"Combat"|"Script"|"GlobalDisabled"|"FTB"|"NotEnoughResources"|"DownedOrDead"|"Dialog"
--- @alias RestFlags string|"CanShortRest"|"CanLongRest"|"Unknown0x04"
--- @alias RollCritical string|"Success"|"None"|"Fail"
--- @alias ServerCharacterFlags string|"Activated"|"DeferredRemoveEscapist"|"OffStage"|"Dead"|"LevelTransitionPending"|"HasOwner"|"Summon"|"RegisteredForAutomatedDialog"|"InDialog"|"Unknown80000000000000"|"CannotDie"|"StoryNPC"|"CharacterControl"|"Temporary"|"Loaded"|"Unknown8000"|"SpotSneakers"|"ForceNonzeroSpeed"|"CannotAttachToGroups"|"FightMode"|"CannotRun"|"Unknown80000000"|"DontCacheTemplate"|"ReservedForDialog"|"DisableWaypointUsage"|"IsPlayer"|"IgnoresTriggers"|"Invisible"|"CanShootThrough"|"Totem"|"Invulnerable"|"IsTrading"|"InParty"|"SteeringEnabled"|"Unknown10000000000"|"Floating"|"IsCompanion_M"|"Unknown40000000000"|"Deactivated"|"CustomLookEnabled"|"IsHuge"|"PartyFollower"|"DisableCulling"|"FindValidPositionOnActivate"|"CannotMove"|"DoNotFaceFlag"|"CharCreationInProgress"|"GMReroll"|"Multiplayer"|"CharacterCreationFinished"|"WalkThrough"|"Detached"|"HostControl"|"CoverAmount"|"NeedsMakePlayerUpdate"|"RequestStartTurn"
--- @alias ServerCharacterFlags2 string|"Unknown0x10"|"Unknown0x40"|"CanGossip"|"Global"|"TreasureGeneratedForTrader"|"Trader"|"IsResurrected"|"IsPet"
--- @alias ServerCharacterFlags3 string|"CrimeWarningsEnabled"|"NeedsPlacementSnapping"
--- @alias ServerGameState string|"Unknown"|"BuildStory"|"Init"|"LoadModule"|"Uninitialized"|"ReloadStory"|"Idle"|"Sync"|"Exit"|"LoadLevel"|"LoadSession"|"UnloadLevel"|"UnloadModule"|"UnloadSession"|"Paused"|"Running"|"Disconnect"|"Save"
--- @alias ServerItemFlags string|"Activated"|"WalkOn"|"ForceSync"|"OffStage"|"Invisible2"|"LevelTransitionPending"|"Summon"|"DisableSync"|"IsSurfaceBlocker"|"IsLadder"|"FreezeGravity"|"DontAddToHotbar"|"StoryItem"|"PositionChanged"|"DisableInventoryView80"|"Destroy"|"GMFolding"|"InUse"|"Sticky"|"IsSecretDoor"|"Invulnerable2"|"Destroyed"|"IsMoving"|"ForceClientSync"|"InPartyInventory"|"WakePhysics"|"ReservedForDialog"|"SourceContainer"|"Frozen"|"TeleportOnUse"|"CanShootThrough"|"CanUse"|"Invisible"|"Totem"|"InAutomatedDialog"|"Invulnerable"|"PinnedContainer"|"NoCover"|"Floating"|"InteractionDisabled"|"TransformChanged"|"CanOnlyBeUsedByOwner"|"LoadedTemplate"|"IsDoor"|"CanBeMoved"|"ClientSync1"|"IsContainer"|"WalkThrough"|"HideHP"|"Known"|"IsSurfaceCloudBlocker"|"CanBePickedUp"|"LuckyFind"
--- @alias ServerItemFlags2 string|"UnsoldGenerated"|"TreasureGenerated"|"UnEquipLocked"|"Global"|"UseRemotely"
--- @alias ServerStatusFlags string|"RequestDelete"|"RequestClientSync2"|"RequestClientSync"|"Loaded"|"Started"|"RequestDeleteAtTurnEnd"|"ForceStatus"|"ForceFailStatus"
--- @alias ServerStatusFlags2 string|"KeepAlive"|"IsFromItem"|"IsLifeTimeSet"|"Channeled"|"InitiateCombat"|"Influence"|"DontTickWhileOnSurface"|"ExcludeFromPortraitRendering"
--- @alias ServerStatusFlags3 string|"IsUnique"|"NotifiedPlanManager"|"StatusFlags3_0x08"|"StatusFlags3_0x10"|"StatusFlags3_0x20"|"StatusFlags3_0x40"|"StatusFlags3_0x80"|"DisableImmunityOverhead"
--- @alias ServerStatusFlags4 string|"IsInvulnerable"|"BringIntoCombat"|"IsHostileAct"|"StatusFlags4_0x04"|"StatusFlags4_0x20"|"StatusFlags4_0x40"|"StatusFlags4_0x80"|"IsInvulnerableVisible"
--- @alias ServerStatusFlags5 string|"HasTriedEntering"
--- @alias ShroudType string|"Shroud"|"RegionMask"|"Sight"|"Sneak"
--- @alias SkillId string|"Athletics"|"Religion"|"Nature"|"Investigation"|"History"|"Arcana"|"Stealth"|"Invalid"|"SleightOfHand"|"Acrobatics"|"Persuasion"|"Performance"|"Deception"|"Intimidation"|"Sentinel"|"Survival"|"Perception"|"Medicine"|"Insight"|"AnimalHandling"
--- @alias SourceAdvantageType string|"SourceAllyAdvantageOnAttack"|"SourceAdvantageOnAttack"
--- @alias SpellAttackType string|"RangedWeaponAttack"|"MeleeWeaponAttack"|"DirectHit"|"None"|"RangedOffHandWeaponAttack"|"MeleeOffHandWeaponAttack"|"RangedUnarmedAttack"|"MeleeUnarmedAttack"|"RangedSpellAttack"|"MeleeSpellAttack"
--- @alias SpellAttackTypeOverride string|"Target_UnarmedAttack"|"Target_OffhandAttack"|"Target_MainHandAttack"|"Projectile_MainHandAttack"|"Projectile_OffhandAttack"
--- @alias SpellChildSelectionType string|"Singular"|"AddChildren"|"MostPowerful"
--- @alias SpellCooldownType string|"OncePerTurnNoRealtime"|"OncePerShortRestPerItem"|"OncePerTurn"|"OncePerCombat"|"UntilRest"|"Default"|"UntilShortRest"|"UntilPerRestPerItem"
--- @alias SpellFlags string|"IsTrap"|"TrajectoryRules"|"RangeIgnoreBlindness"|"AbortOnSpellRollFail"|"CanAreaDamageEvade"|"HasVerbalComponent"|"DontAbortPerforming"|"HasSomaticComponent"|"NoCooldownOnMiss"|"IsJump"|"IsSwarmAttack"|"IsAttack"|"Stealth"|"Temporary"|"DisplayInItemTooltip"|"IsMelee"|"HasHighGroundRangeExtension"|"NoAOEDamageOnLand"|"HideInItemTooltip"|"IsConcentration"|"ConcentrationIgnoresResting"|"DisableBlood"|"AddFallDamageOnLand"|"CombatLogSetSingleLineRoll"|"IgnoreAoO"|"InventorySelection"|"CannotTargetCharacter"|"IsSpell"|"RangeIgnoreSourceBounds"|"IsEnemySpell"|"RangeIgnoreTargetBounds"|"CannotTargetItems"|"RangeIgnoreVerticalThreshold"|"CannotTargetTerrain"|"IsDefaultWeaponAction"|"IgnoreVisionBlock"|"TargetClosestEqualGroundSurface"|"AddWeaponRange"|"IsLinkedSpellContainer"|"IgnoreSilence"|"Invisible"|"AbortOnSecondarySpellRollFail"|"ImmediateCast"|"IgnorePreviouslyPickedEntities"|"NoSurprise"|"IsHarmful"|"CallAlliesSpell"|"CannotRotate"|"NoCameraMove"|"CanDualWield"|"AllowMoveAndCast"|"UNUSED_D"|"Wildshape"|"UNUSED_E"|"UnavailableInDialogs"|"PickupEntityAndMove"
--- @alias SpellPrepareType string|"Unknown"|"AlwaysPrepared"|"RequiresPreparation"
--- @alias SpellSchoolId string|"Abjuration"|"Conjuration"|"Divination"|"Enchantment"|"Evocation"|"Illusion"|"None"|"Necromancy"|"Transmutation"
--- @alias SpellSourceType string|"Unknown16"|"Unknown17"|"Spell"|"Unknown18"|"Unknown19"|"Unknown1A"|"Boost"|"SpellSet"|"CreateExplosion"|"Progression0"|"GameActionCreateSurface"|"Progression1"|"Progression2"|"SpellSet2"|"WeaponAttack"|"Sentinel"|"UnarmedAttack"|"Osiris"|"Shapeshift"|"Anubis"|"EquippedItem"|"Progression"|"AiTest"|"Behavior"|"ActiveDefense"|"PartyPreset"|"Learned"|"Unknown15"
--- @alias SpellType string|"Zone"|"Projectile"|"Rush"|"Wall"|"Throw"|"Shout"|"ProjectileStrike"|"Storm"|"MultiStrike"|"Teleportation"|"Target"
--- @alias StatAttributeFlags string|"Arrow"|"SlippingImmunity"|"Torch"|"Unbreakable"|"Unrepairable"|"Unstorable"|"Grounded"|"EMPTY"|"IgnoreClouds"|"LootableWhenEquipped"|"ThrownImmunity"|"InvisibilityImmunity"|"Backstab"|"BackstabImmunity"|"FloatingWhileMoving"|"PickpocketableWhenEquipped"|"LoseDurabilityOnCharacterHit"|"Floating"|"InvulnerableAndInteractive"|"EnableObscurityEvents"|"ObscurityWithoutSneaking"
--- @alias StatCharacterFlags string|"IsPlayer"|"Invisible"|"InParty"|"IsSneaking"|"Blind"|"DrinkedPotion"|"EquipmentValidated"
--- @alias StatsDeathType string|"Disintegrate"|"Cold"|"CinematicDeath"|"Radiant"|"Psychic"|"Physical"|"Necrotic"|"Lifetime"|"KnockedDown"|"Incinerate"|"Falling"|"Explode"|"Electrocution"|"DoT"|"None"|"Chasm"|"Acid"
--- @alias StatsExpressionParamType string|"Type1"|"Int"|"Bool"|"StatsExpressionType"|"StatsExpressionVariableDataType"|"StatsExpressionVariableDataModifier"|"RollDefinition"|"ResourceRollDefinition"|"StatsContextType"
--- @alias StatsExpressionParamType2 string|"StatusGroup"|"Ability"|"Skill"|"StatsExpressionVariableData"|"STDString"
--- @alias StatsFunctorFlags string|"Owner"|"Swap"|"Self"
--- @alias StatsFunctorId string|"ExecuteWeaponFunctors"|"DisarmWeapon"|"Sabotage"|"SummonInInventory"|"DisarmAndStealWeapon"|"Resurrect"|"Summon"|"DealDamage"|"RestoreResource"|"ResetCombatTurn"|"RemoveUniqueStatus"|"RemoveStatusByLevel"|"CustomDescription"|"RemoveStatus"|"Stabilize"|"RegainHitPoints"|"SpawnInInventory"|"CreateZone"|"Spawn"|"CreateWall"|"CreateSurface"|"SetStatusDuration"|"CreateExplosion"|"Equalize"|"CreateConeSurface"|"SetRoll"|"Pickup"|"Counterspell"|"SetReroll"|"Extender"|"SetDisadvantage"|"ShortRest"|"SetDamageResistance"|"SetAdvantage"|"MaximizeRoll"|"ResetCooldowns"|"CameraWait"|"UseSpell"|"BreakConcentration"|"UseAttack"|"Kill"|"UseActionResource"|"ApplyStatus"|"Unsummon"|"ApplyEquipmentStatus"|"Unlock"|"TriggerRandomCast"|"Force"|"TeleportSource"|"FireProjectile"|"AdjustRoll"|"SpawnExtraProjectiles"|"Douse"|"RemoveAuraByChildStatus"|"SwitchDeathType"|"SwapPlaces"|"RegainTemporaryHitPoints"|"TutorialEvent"|"SurfaceClearLayer"|"Drop"|"SurfaceChange"|"GainTemporaryHitPoints"|"DoTeleport"
--- @alias StatsItemSlot string|"VanityBoots"|"VanityBody"|"Helmet"|"Breast"|"Cloak"|"MeleeMainHand"|"OffHand"|"MeleeOffHand"|"MainHand"|"RangedMainHand"|"RangedOffHand"|"Ring"|"Boots"|"Amulet"|"Ring2"|"Overhead"|"Max"|"Wings"|"Horns"|"MusicalInstrument"|"Underwear"|"Gloves"|"Sentinel"
--- @alias StatsObserverType string|"None"|"Target"|"Source"|"Observer"
--- @alias StatsPropertyContext string|"SURFACE_ENTER"|"ATTACKING_IN_MELEE_RANGE"|"ATTACKED_IN_MELEE_RANGE"|"STATUS_REMOVED"|"ACTION_RESOURCES_CHANGED"|"DAMAGED_PREVENTED"|"STATUS_REMOVE"|"DAMAGE_PREVENTED"|"STATUS_APPLY"|"DAMAGED"|"STATUS_APPLIED"|"DAMAGE"|"CREATE"|"PUSHED"|"SHORT_REST"|"PUSH"|"PROJECTILE"|"PROFICIENCY_CHANGED"|"OBSCURITY_CHANGED"|"COMBAT_ENDED"|"MOVED_DISTANCE"|"CAST_RESOLVED"|"CAST"|"LONG_REST"|"LEAVE_ATTACK_RANGE"|"ATTACKED"|"ATTACK"|"INVENTORY_CHANGED"|"INTERRUPT_USED"|"HEALED"|"AOE"|"HEAL"|"TURN"|"GROUND"|"AI_ONLY"|"AI_IGNORE"|"TARGET"|"EQUIP"|"ABILITY_CHECK"|"ENTER_ATTACK_RANGE"|"LOCKPICKING_SUCCEEDED"
--- @alias StatsRollAdjustmentType string|"All"|"Distribute"
--- @alias StatsRollType string|"RangedWeaponAttack"|"MeleeWeaponAttack"|"RangedUnarmedDamage"|"MeleeUnarmedDamage"|"RangedSpellDamage"|"MeleeSpellDamage"|"RangedWeaponDamage"|"MeleeWeaponDamage"|"RawAbility"|"SavingThrow"|"SkillCheck"|"Attack"|"DeathSavingThrow"|"None"|"RangedOffHandWeaponAttack"|"MeleeOffHandWeaponAttack"|"Sentinel"|"RangedUnarmedAttack"|"MeleeUnarmedAttack"|"Damage"|"RangedSpellAttack"|"MeleeSpellAttack"
--- @alias StatsStatusGroup string|"SG_Exhausted"|"SG_Doppelganger"|"SG_Dominated"|"SG_Disguise"|"SG_Disease"|"SG_DifficultTerrain"|"SG_DetectThoughts"|"SG_Cursed"|"SG_Confused"|"SG_RemoveOnRespec"|"SG_Condition"|"SG_Sleeping"|"SG_Charmed_Subtle"|"SG_Drunk"|"SG_Charmed"|"SG_WeaponCoating"|"SG_Unconscious"|"SG_CanBePickedUp"|"SG_Blinded"|"SG_Taunted"|"SG_Surface"|"SG_Approaching"|"SG_ScriptedPeaceBehaviour"|"SG_Stunned"|"SG_Polymorph_BeastShape_NPC"|"SG_Restrained"|"SG_Polymorph_BeastShape"|"SG_Poisoned_Story_Removable"|"SG_Rage"|"SG_Prone"|"SG_Poisoned_Story_NonRemovable"|"SG_Possessed"|"SG_Helpable_Condition"|"SG_Polymorph"|"SG_DropForNonMutingDialog"|"SG_Poisoned"|"SG_Petrified"|"SG_Paralyzed"|"SG_Mad"|"SG_Light"|"SG_Invisible"|"SG_Incapacitated"|"SG_HexbladeCurse"|"SG_Frightened"|"SG_Fleeing"
--- @alias StatsTargetTypeFlags string|"Living"|"Guaranteed"|"Construct"|"Undead"
--- @alias StatsZoneShape string|"Square"|"Cone"
--- @alias StatusHealType string|"PhysicalArmor"|"All"|"None"|"MagicArmor"|"AllArmor"|"Source"|"Vitality"
--- @alias StatusMaterialApplyFlags string|"ApplyOnArmor"|"ApplyOnWeapon"|"ApplyOnWings"|"ApplyOnHorns"|"ApplyOnOverhead"|"ApplyOnBody"
--- @alias StatusType string|"BOOST"|"TELEPORT_FALLING"|"EFFECT"|"DEACTIVATED"|"REACTION"|"DOWNED"|"DYING"|"SMELLY"|"INVISIBLE"|"ROTATE"|"MATERIAL"|"HEAL"|"KNOCKED_DOWN"|"CLIMBING"|"INCAPACITATED"|"STORY_FROZEN"|"INSURFACE"|"SNEAKING"|"POLYMORPHED"|"UNLOCK"|"FEAR"
--- @alias SurfaceActionType string|"TransformSurfaceAction"|"ChangeSurfaceOnPathAction"|"RectangleSurfaceAction"|"CreateSurfaceAction"|"CreatePuddleAction"|"ExtinguishFireAction"|"ZoneAction"|"PolygonSurfaceAction"
--- @alias SurfaceChange string|"None"|"DestroyWater"|"Condense"|"Vaporize"|"Melt"|"Freeze"|"Deelectrify"|"Electrify"|"Douse"|"Ignite"
--- @alias SurfaceLayer string|"Cloud"|"Ground"|"None"
--- @alias SurfaceLayer8 string|"Cloud"|"Ground"|"None"
--- @alias SurfaceTransformActionType string|"None"|"Condense"|"Vaporize"|"Melt"|"Freeze"|"Deelectrify"|"Electrify"|"Douse"|"Ignite"
--- @alias SurfaceType string|"ShadowCursedVines"|"Ash"|"DrowPoisonCloud"|"PotionResistanceLightningCloud"|"AlienOil"|"IceCloud"|"PotionResistancePoisonCloud"|"Mud"|"PotionHealingCloud"|"BlackPowderDetonationCloud"|"Alcohol"|"PotionSpeedCloud"|"InvisibleWeb"|"PotionVitalityCloud"|"BloodSilver"|"PotionAntitoxinCloud"|"Water"|"Hellfire"|"SporePinkCloud"|"CausticBrine"|"VoidCloud"|"BloodExploding"|"CrawlerMucusCloud"|"Blood"|"SpikeGrowth"|"Cloudkill6Cloud"|"HolyFire"|"BlackTentacles"|"Lava"|"Overgrowth"|"PurpleWormPoison"|"Oil"|"SerpentVenom"|"InvisibleGithAcid"|"BladeBarrier"|"Fire"|"Sewer"|"WaterCloud"|"Poison"|"PoisonCloud"|"None"|"WaterElectrified"|"Chasm"|"ExplosionCloud"|"WaterFrozen"|"Acid"|"WaterCloudElectrified"|"ShockwaveCloud"|"BloodElectrified"|"CloudkillCloud"|"GithPheromoneGasCloud"|"BloodFrozen"|"MaliceCloud"|"PotionHealingGreaterCloud"|"Sentinel"|"Grease"|"BloodCloud"|"PotionHealingSuperiorCloud"|"WyvernPoison"|"StinkingCloud"|"PotionHealingSupremeCloud"|"Web"|"DarknessCloud"|"PotionInvisibilityCloud"|"Deepwater"|"FogCloud"|"PotionResistanceAcidCloud"|"Vines"|"SporeWhiteCloud"|"PotionResistanceColdCloud"|"TrialFire"|"SporeGreenCloud"|"PotionResistanceFireCloud"|"BlackPowder"|"SporeBlackCloud"|"PotionResistanceForceCloud"
--- @alias TravelErrorFlags string|"Condition"|"FTB"|"InDanger"|"DangerZone"|"Dialog"|"CannotFastTravel"
--- @alias TravelFlags string|"BlockFastTravel"|"BlockCampTravel"
--- @alias WeaponFlags string|"NeedDualWieldingBoost"|"Torch"|"Ammunition"|"Finesse"|"Loading"|"Reach"|"Lance"|"Thrown"|"Heavy"|"Twohanded"|"Versatile"|"Dippable"|"NoDualWield"|"Magical"|"NotSheathable"|"Unstowable"|"AddToHotbar"|"Light"|"Net"|"Melee"|"Range"
--- @alias WeaponType string|"Axe"|"Arrow"|"Staff"|"Crossbow"|"Spear"|"Knife"|"Wand"|"Rifle"|"Bow"|"None"|"Sentinel"|"Sword"|"Club"


--- @class ACOverrideFormulaBoostComponent:BaseComponent
--- @field AC int32
--- @field AddAbilityModifiers AbilityId[]
--- @field field_4 boolean


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


--- @class AnimationBlueprintResource:LockedResource
--- @field field_48 int64
--- @field field_50 int64
--- @field field_58 int64
--- @field field_60 int64
--- @field field_68 int64


--- @class AnimationResource:LockedResource
--- @field field_48 int64
--- @field field_50 int64
--- @field field_58 FixedString
--- @field field_5c FixedString
--- @field field_60 FixedString
--- @field field_64 FixedString
--- @field field_68 FixedString
--- @field field_6c int32
--- @field field_70 int64
--- @field field_78 int64


--- @class AnimationSetResource:ResourceWithExtraData
--- @field field_30 int64


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


--- @class AtmosphereResource:ResourceWithExtraData
--- @field field_30 int64
--- @field field_38 FixedString[]


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


--- @class BlendSpaceResource:LockedResource
--- @field field_48 string
--- @field field_60 string
--- @field field_78 int64
--- @field field_80 int64
--- @field field_88 int64
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 int64
--- @field field_a8 int64
--- @field field_b0 int64
--- @field field_b8 int64
--- @field field_c0 int64


--- @class BlockAbilityModifierFromACComponent:BaseComponent
--- @field Ability AbilityId


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
--- @field BoostEntity EntityHandle
--- @field Cause BoostCause
--- @field Owner EntityHandle
--- @field Params BoostParameters
--- @field Type BoostType
--- @field field_10 Guid
--- @field field_20 uint8
--- @field field_80 Guid


--- @class BoostParameters
--- @field Boost FixedString
--- @field Params string
--- @field Params2 string


--- @class BoostTagComponent:BaseComponent
--- @field Tags Guid[]


--- @class BoostsContainerComponent:BaseComponent
--- @field Boosts table<BoostType, EntityHandle[]>
--- @field field_0 uint64


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


--- @class CanDeflectProjectilesComponent:BaseComponent
--- @field Flags uint16


--- @class CanDoActionsComponent:BaseComponent
--- @field Flags uint16


--- @class CanDoRestComponent:BaseComponent
--- @field Flags RestFlags
--- @field RestErrorFlags1 RestErrorFlags
--- @field RestErrorFlags2 RestErrorFlags


--- @class CanInteractComponent:BaseComponent
--- @field Flags uint16


--- @class CanModifyHealthComponent:BaseComponent
--- @field Flags uint16


--- @class CanMoveComponent:BaseComponent
--- @field Flags uint16


--- @class CanSeeThroughBoostComponent:BaseComponent
--- @field CanSeeThrough boolean


--- @class CanSenseComponent:BaseComponent
--- @field Flags uint16


--- @class CanShootThroughBoostComponent:BaseComponent
--- @field CanShootThrough boolean


--- @class CanSpeakComponent:BaseComponent
--- @field Flags uint16


--- @class CanTravelComponent:BaseComponent
--- @field ErrorFlags TravelErrorFlags
--- @field Flags TravelFlags
--- @field field_2 uint16


--- @class CanTriggerRandomCastsComponent:BaseComponent


--- @class CanWalkThroughBoostComponent:BaseComponent
--- @field CanWalkThrough boolean


--- @class CannotHarmCauseEntityBoostComponent:BaseComponent
--- @field Type FixedString


--- @class CarryCapacityMultiplierBoostComponent:BaseComponent
--- @field Multiplier number


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


--- @class CharacterVisualPresetDataData
--- @field field_0 PresetDataDataData
--- @field field_30 FixedString[]


--- @class CharacterVisualResource:ResourceWithExtraData
--- @field field_100 table<FixedString, PresetData>
--- @field field_118 CharacterVisualPresetDataData
--- @field field_158 CharacterVisualPresetDataData
--- @field field_198 FixedString
--- @field field_19c int32
--- @field field_1a0 FixedString
--- @field field_1a4 int32
--- @field field_30 FixedString
--- @field field_34 int32
--- @field field_38 CharacterVisualResourceElement1[]
--- @field field_48 CharacterVisualResourceElement2[]
--- @field field_58 PresetData


--- @class CharacterVisualResourceElement1
--- @field field_0 FixedString
--- @field field_4 FixedString
--- @field field_8 FixedString


--- @class CharacterVisualResourceElement2
--- @field field_0 FixedString
--- @field field_4 FixedString


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


--- @class ClothColliderResource:ResourceWithExtraData
--- @field field_30 ClothColliderResourceData[]


--- @class ClothColliderResourceData
--- @field field_0 int64
--- @field field_10 FixedString
--- @field field_14 int32
--- @field field_18 FixedString[]
--- @field field_28 FixedString
--- @field field_2c int32
--- @field field_8 int64


--- @class ColorListResource:ResourceWithExtraData
--- @field field_30 int64
--- @field field_38 ColorListResourceElement[]
--- @field field_48 int64
--- @field field_50 int64
--- @field field_58 ColorListResourceElement[]
--- @field field_68 int64
--- @field field_70 int64


--- @class ColorListResourceElement
--- @field field_0 int64
--- @field field_10 int64
--- @field field_18 int64
--- @field field_8 int64


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
--- @field Sneaking boolean
--- @field field_2 boolean
--- @field field_3 uint8
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


--- @class DelayDeathCauseComponent:BaseComponent
--- @field Blocked_M int32
--- @field DelayCount int32


--- @class DetectCrimesBlockBoostComponent:BaseComponent
--- @field field_0 boolean


--- @class DialogResource:ResourceWithExtraData
--- @field field_30 int64
--- @field field_38 int64
--- @field field_40 string[]
--- @field field_50 DialogResourceData[]
--- @field field_60 int64
--- @field field_68 string


--- @class DialogResourceData
--- @field field_0 int64
--- @field field_8 int64


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


--- @class DiffusionProfileResource:ResourceWithExtraData
--- @field field_30 int64
--- @field field_38 int64
--- @field field_40 int64
--- @field field_48 int64
--- @field field_50 int64
--- @field field_58 int64
--- @field field_60 int64


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


--- @class EffectResource:LockedResource
--- @field field_48 FixedString
--- @field field_4c int32
--- @field field_50 int64
--- @field field_58 int64
--- @field field_60 FixedString[]
--- @field field_70 int64
--- @field field_78 int64
--- @field field_80 int64
--- @field field_88 int64
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 int64


--- @class EntityThrowDamageBoostComponent:BaseComponent
--- @field Roll DiceValues
--- @field field_C uint8


--- @class EoCGameObjectTemplate:GameObjectTemplate
--- @field CollideWithCamera boolean
--- @field DisplayName TranslatedString
--- @field FadeChildren FixedString[]
--- @field FadeGroup FixedString
--- @field Fadeable boolean
--- @field HierarchyOnlyFade boolean
--- @field SeeThrough boolean


--- @class EoCGameObjectTemplate2:EoCGameObjectTemplate
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


--- @class EocLevelComponent:BaseComponent
--- @field Level int32


--- @class EquipableComponent:BaseComponent
--- @field Slot ItemSlot
--- @field field_18 Guid


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


--- @class FCurveResource:LockedResource
--- @field field_48 int64[]
--- @field field_58 int64
--- @field field_60 int64
--- @field field_68 int64
--- @field field_70 int64


--- @class FTBParticipantComponent:BaseComponent
--- @field field_18 EntityHandle


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


--- @class GravityDisabledComponent:BaseComponent


--- @class GravityDisabledUntilMovedComponent:BaseComponent


--- @class GuaranteedChanceRollOutcomeBoostComponent:BaseComponent
--- @field field_0 boolean


--- @class HalveWeaponDamageBoostComponent:BaseComponent
--- @field Ability AbilityId


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
--- @field FOV number
--- @field Hearing number
--- @field Sight number


--- @class Hit
--- @field ArmorAbsorption int32
--- @field AttackFlags uint8
--- @field AttackRollAbility AbilityId
--- @field AttackType SpellAttackType
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
--- @field field_17C int32
--- @field field_180 int32
--- @field field_184 int32
--- @field field_188 number
--- @field field_18C int32
--- @field field_190 vec3[]


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


--- @class IKRigResource:ResourceWithExtraData
--- @field field_30 table<FixedString, IKRigResourceMapValue>
--- @field field_48 IKRigResourceArrayStruct[]
--- @field field_58 int64
--- @field field_60 int64
--- @field field_68 int32[]
--- @field field_78 int32[]
--- @field field_88 IKRigResourceArrayStruct2[]
--- @field field_98 FixedString[]


--- @class IKRigResourceArrayStruct
--- @field field_0 FixedString
--- @field field_10 FixedString
--- @field field_14 int32
--- @field field_18 int64
--- @field field_20 int64
--- @field field_28 int64
--- @field field_30 int64
--- @field field_38 int64
--- @field field_4 FixedString
--- @field field_8 int32
--- @field field_c FixedString


--- @class IKRigResourceArrayStruct2
--- @field field_0 int64
--- @field field_8 int64


--- @class IKRigResourceMapValue
--- @field field_0 int64
--- @field field_10 int64
--- @field field_18 int64
--- @field field_20 int64
--- @field field_8 int64


--- @class IconComponent:BaseComponent
--- @field Icon FixedString


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


--- @class ItemTemplate:EoCGameObjectTemplate2
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
--- @field V4_0_0_26Flag boolean


--- @class JumpMaxDistanceBonusBoostComponent:BaseComponent
--- @field DistanceBonus number


--- @class JumpMaxDistanceMultiplierBoostComponent:BaseComponent
--- @field Amount number


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


--- @class LightCookieResource:LockedResource
--- @field field_48 int64
--- @field field_50 int64
--- @field field_58 int64
--- @field field_60 int64
--- @field field_68 int64
--- @field field_70 int64
--- @field field_78 int64
--- @field field_80 int64
--- @field field_88 int64
--- @field field_90 int64
--- @field field_98 int64
--- @field field_a0 int64
--- @field field_a8 int64
--- @field field_b0 int64
--- @field field_b8 int64
--- @field field_c0 int64
--- @field field_c8 int64


--- @class LightingResource:ResourceWithExtraData
--- @field field_48 LightingResourceDetails


--- @class LightingResourceDetails
--- @field data string[]
--- @field field_8 FixedString


--- @class LockBoostComponent:BaseComponent
--- @field Lock Guid


--- @class LockComponent:BaseComponent
--- @field Key_M FixedString
--- @field LockDC int32
--- @field field_18 Guid[]
--- @field field_8 Guid


--- @class LockedResource:Resource
--- @field field_30 int64
--- @field field_40 int64


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


--- @class MaterialPresetResource:ResourceWithExtraData
--- @field field_30 PresetData


--- @class MaterialResource:LockedResource
--- @field field_48 int64
--- @field field_50 MaterialResourceElement1[]
--- @field field_60 MaterialResourceElement2[]
--- @field field_70 MaterialResourceElement3[]
--- @field field_80 MaterialResourceElement4[]
--- @field field_90 MaterialResourceElement5[]
--- @field field_a0 MaterialResourceElement6[]
--- @field field_b0 FixedString
--- @field field_b4 int16
--- @field field_b6 int16


--- @class MaterialResourceElement1
--- @field field_0 FixedString
--- @field field_4 int32
--- @field field_8 int64


--- @class MaterialResourceElement2
--- @field field_0 FixedString
--- @field field_10 int64
--- @field field_4 int32
--- @field field_8 int64


--- @class MaterialResourceElement3
--- @field field_0 FixedString
--- @field field_10 int32
--- @field field_14 int32
--- @field field_18 int32
--- @field field_1c int32
--- @field field_20 int32
--- @field field_4 int32
--- @field field_8 int32
--- @field field_c int32


--- @class MaterialResourceElement4
--- @field field_0 FixedString
--- @field field_10 int64
--- @field field_18 int64
--- @field field_20 int64
--- @field field_28 int64
--- @field field_30 int64
--- @field field_38 int64
--- @field field_4 int32
--- @field field_8 int64


--- @class MaterialResourceElement5
--- @field field_0 FixedString
--- @field field_4 int32
--- @field field_8 FixedString


--- @class MaterialResourceElement6
--- @field field_0 FixedString
--- @field field_4 int32
--- @field field_8 int32
--- @field field_c FixedString


--- @class MaterialSetResource:LockedResource


--- @class MaximizeHealingBoostComponent:BaseComponent
--- @field Direction HealDirection
--- @field TargetTypes StatsTargetTypeFlags


--- @class MaximumRollResultBoostComponent:BaseComponent
--- @field Result int8
--- @field RollType StatsRollType


--- @class MeshProxyResource:LockedResource
--- @field field_48 int64
--- @field field_50 FixedString
--- @field field_54 int32


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


--- @class OriginComponent:BaseComponent
--- @field Origin FixedString
--- @field field_18 Guid


--- @class OriginPassivesComponent:BaseComponent
--- @field field_18 PassiveInfo[]


--- @class OriginTagComponent:BaseComponent
--- @field Tags Guid[]


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


--- @class PhysicsResource:ResourceWithExtraData
--- @field field_30 int64
--- @field field_38 int64
--- @field field_40 FixedString[]
--- @field field_50 FixedString
--- @field field_54 string
--- @field field_55 string
--- @field field_56 int16


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


--- @class PresetData
--- @field field_0 PresetDataElement1[]
--- @field field_10 PresetDataElement2[]
--- @field field_20 PresetDataElement3[]
--- @field field_30 PresetDataElement4[]
--- @field field_40 PresetDataElement5[]
--- @field field_50 PresetDataElement5[]
--- @field field_60 FixedString
--- @field field_64 int32
--- @field field_68 PresetDataData


--- @class PresetDataElement1
--- @field field_0 FixedString
--- @field field_4 int32
--- @field field_8 int32


--- @class PresetDataElement2
--- @field field_0 FixedString
--- @field field_4 int32
--- @field field_8 int32
--- @field field_c int32


--- @class PresetDataElement3
--- @field field_0 FixedString
--- @field field_10 int32
--- @field field_4 int32
--- @field field_8 int32
--- @field field_c int32


--- @class PresetDataElement4
--- @field field_0 FixedString
--- @field field_10 int64
--- @field field_18 int64
--- @field field_20 int64
--- @field field_28 int64
--- @field field_4 int32
--- @field field_8 int64


--- @class PresetDataElement5
--- @field field_0 FixedString
--- @field field_4 FixedString
--- @field field_8 int32


--- @class PresetDataData
--- @field field_0 PresetDataDataData
--- @field field_30 PresetDataDataElement1[]


--- @class PresetDataDataElement1
--- @field field_0 FixedString
--- @field field_4 FixedString
--- @field field_8 int32


--- @class PresetDataDataData
--- @field field_0 int32[]
--- @field field_10 int32[]
--- @field field_20 FixedString[]


--- @class ProficiencyBonusBoostComponent:BaseComponent
--- @field Ability AbilityId
--- @field Skill SkillId
--- @field Type ProficiencyBonusBoostType


--- @class ProficiencyBonusOverrideBoostComponent:BaseComponent
--- @field Value StatsExpressionParam


--- @class ProficiencyBoostComponent:BaseComponent
--- @field Flags ProficiencyGroupFlags


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


--- @class Resource
--- @field Guid FixedString
--- @field SourceFile Path


--- @class ResourceManager
--- @field EffectFactory int64
--- @field GameAnalytics_M int64
--- @field SoundManager_M int64
--- @field UI_M int64
--- @field field_0 int64
--- @field field_130 int64
--- @field field_138 int64
--- @field field_30 int64
--- @field field_38 int64
--- @field field_40 FixedString[]
--- @field field_50 int64
--- @field field_60 int64
--- @field field_68 int64
--- @field field_70 int64
--- @field field_78 int64
--- @field field_80 int64
--- @field field_88 int64
--- @field field_98 int64
--- @field field_A0 int64
--- @field field_B0 int64
--- @field field_e0 int64
--- @field field_e8 int64


--- @class ResourceRollDefinition
--- @field field_0 Guid
--- @field field_10 uint8


--- @class ResourceWithExtraData:Resource
--- @field field_28 int16
--- @field field_2a string
--- @field possibly_unused_field_2b string
--- @field possibly_unused_field_2c int32


--- @class RollBonusBoostComponent:BaseComponent
--- @field Ability AbilityId
--- @field Amount StatsExpressionParam
--- @field RollType StatsRollType
--- @field Skill SkillId


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


--- @class ScriptResource:ResourceWithExtraData
--- @field field_30 table<FixedString, int64>


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


--- @class SkeletonMirrorTableResource:LockedResource
--- @field field_48 SkeletonMirrorTableResourceFixedStringPair[]


--- @class SkeletonMirrorTableResourceFixedStringPair
--- @field field_0 FixedString
--- @field field_4 FixedString


--- @class SkeletonResource:LockedResource
--- @field field_48 table<FixedString, SkeletonResourceElement1[]>
--- @field field_60 SkeletonResourceElement2[]
--- @field field_70 int64
--- @field field_78 FixedString
--- @field field_7c FixedString
--- @field field_80 FixedString
--- @field field_84 FixedString
--- @field field_88 FixedString
--- @field field_8c FixedString
--- @field field_90 FixedString
--- @field field_94 FixedString
--- @field field_98 int64


--- @class SkeletonResourceElement1
--- @field field_0 int32
--- @field field_4 FixedString
--- @field field_8 FixedString


--- @class SkeletonResourceElement2
--- @field field_0 FixedString
--- @field field_10 int32
--- @field field_14 int32
--- @field field_18 int32
--- @field field_20 int32
--- @field field_24 int32
--- @field field_28 int32
--- @field field_4 FixedString
--- @field field_8 FixedString
--- @field field_c int32


--- @class SkillBoostComponent:BaseComponent
--- @field Amount StatsExpressionParam
--- @field Skill SkillId


--- @class SkinPresetResource:ResourceWithExtraData
--- @field field_30 PresetData


--- @class SomeSharedServerClientObjId
--- @field field_0 int64
--- @field field_8 int32


--- @class SomeSharedServerClientObjId2:SomeSharedServerClientObjId
--- @field field_10 int64
--- @field field_18 int32


--- @class SoundResource:LockedResource
--- @field field_48 int32
--- @field field_4c FixedString
--- @field field_50 FixedString
--- @field field_54 int32
--- @field field_58 int64
--- @field field_60 int64
--- @field field_68 int64
--- @field field_70 int64


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


--- @class TerrainBrushResource:ResourceWithExtraData
--- @field field_30 FixedString
--- @field field_34 FixedString
--- @field field_38 FixedString
--- @field field_3c FixedString
--- @field field_40 int64
--- @field field_48 int64
--- @field field_50 int64
--- @field field_58 int64
--- @field field_60 int64
--- @field field_68 int64
--- @field field_70 int64
--- @field field_78 int64
--- @field field_80 int64
--- @field field_88 int64


--- @class TextureResource:ResourceWithExtraData
--- @field field_30 int64
--- @field field_38 FixedString
--- @field field_3c int32
--- @field field_40 int64
--- @field field_48 int64
--- @field field_50 int64


--- @class TileSetResource:ResourceWithExtraData
--- @field field_30 TileSetResourceExtendedRefMap


--- @class TileSetResourceExtendedRefMap
--- @field field_0 table<Guid, TileSetResourceValue>
--- @field field_10 int64


--- @class TileSetResourceValue
--- @field field_0 FixedString
--- @field field_10 int32
--- @field field_4 FixedString
--- @field field_c FixedString


--- @class TimelineResource:ResourceWithExtraData
--- @field field_30 int64
--- @field field_38 int64
--- @field field_40 int64
--- @field field_48 int64


--- @class TimelineSceneResource:ResourceWithExtraData
--- @field field_30 FixedString[]
--- @field field_40 int64


--- @class ToggledPassivesComponent:BaseComponent
--- @field Passives table<FixedString, boolean>


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


--- @class UuidComponent:BaseComponent
--- @field EntityUuid Guid


--- @class UuidToHandleMappingComponent:BaseComponent
--- @field Mappings table<Guid, EntityHandle>


--- @class ValueComponent:BaseComponent
--- @field Rarity uint8
--- @field Unique boolean
--- @field Value int32


--- @class VirtualTextureResource:ResourceWithExtraData
--- @field field_30 int64
--- @field field_38 int64
--- @field field_40 FixedString
--- @field field_44 int32
--- @field field_48 int64
--- @field field_50 int64


--- @class VisualComponent:BaseComponent
--- @field field_18 int64
--- @field field_20 uint8
--- @field field_21 uint8


--- @class VisualPresetDataData
--- @field field_0 PresetDataDataData
--- @field field_30 VisualPresetDataDataElement1[][]


--- @class VisualPresetDataDataElement1
--- @field field_0 FixedString
--- @field field_4 int32
--- @field field_8 int16[]


--- @class VisualResource:LockedResource
--- @field field_100 FixedString
--- @field field_104 FixedString
--- @field field_108 FixedString
--- @field field_10c FixedString
--- @field field_110 FixedString
--- @field field_114 int32
--- @field field_118 VisualPresetDataData
--- @field field_158 int64
--- @field field_160 int64
--- @field field_168 int64
--- @field field_170 int64
--- @field field_50 FixedString[]
--- @field field_60 int64
--- @field field_68 VisualResourceElement1[]
--- @field field_78 VisualResourceElement2[]
--- @field field_88 VisualResourceElement3[]
--- @field field_98 VisualResourceElement4[]
--- @field field_a8 FixedString[]
--- @field field_b8 table<FixedString, VisualResourceMapped>
--- @field field_d0 FixedString[]
--- @field field_e0 int64
--- @field field_e8 FixedString
--- @field field_ec FixedString
--- @field field_f0 FixedString
--- @field field_f4 FixedString
--- @field field_f8 FixedString
--- @field field_fc FixedString


--- @class VisualResourceElement1
--- @field field_0 FixedString
--- @field field_4 FixedString
--- @field field_8 int32


--- @class VisualResourceElement2
--- @field field_0 FixedString
--- @field field_4 FixedString


--- @class VisualResourceElement3
--- @field field_0 FixedString
--- @field field_10 int32
--- @field field_14 int32
--- @field field_18 int32
--- @field field_1c int32
--- @field field_20 int32
--- @field field_24 int32
--- @field field_28 int32
--- @field field_2c int32
--- @field field_30 int32
--- @field field_34 int32
--- @field field_38 int32
--- @field field_3c int32
--- @field field_4 int32
--- @field field_40 int32
--- @field field_44 int32
--- @field field_48 int32
--- @field field_4c int32
--- @field field_50 int32
--- @field field_54 int32
--- @field field_58 int32
--- @field field_5c int32
--- @field field_60 int32
--- @field field_64 int32
--- @field field_68 int32
--- @field field_6c int32
--- @field field_70 int32
--- @field field_74 int32
--- @field field_78 int32
--- @field field_7c int32
--- @field field_8 int32
--- @field field_80 int32
--- @field field_84 int32
--- @field field_88 int32
--- @field field_8c int32
--- @field field_90 int32
--- @field field_94 int32
--- @field field_98 int32
--- @field field_9c int32
--- @field field_a0 int32
--- @field field_a4 int32
--- @field field_a8 int32
--- @field field_ac int32
--- @field field_b0 int32
--- @field field_b4 int32
--- @field field_b8 int32
--- @field field_c int32


--- @class VisualResourceElement4
--- @field field_0 int64
--- @field field_10 FixedString
--- @field field_14 int32
--- @field field_8 int64


--- @class VisualResourceMapped
--- @field field_0 int64
--- @field field_10 int64
--- @field field_8 int64


--- @class VisualSetResource:ResourceWithExtraData


--- @class VoiceBarkResource:ResourceWithExtraData


--- @class VoiceComponent:BaseComponent
--- @field Voice Guid


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


--- @class CombatTurnBasedComponent:BaseComponent
--- @field ActedThisRoundInCombat boolean
--- @field CanAct_M boolean
--- @field Combat Guid
--- @field Entity EntityHandle
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
--- @field PreviousLevel FixedString
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
--- @field DisableUse uint8
--- @field DontAddToHotbar boolean
--- @field Flags ServerItemFlags
--- @field Flags2 ServerItemFlags2
--- @field Floating boolean
--- @field ForceClientSync boolean
--- @field ForceSync boolean
--- @field ForceSynch uint8
--- @field FreezeGravity boolean
--- @field Frozen boolean
--- @field GMFolding boolean
--- @field Global boolean
--- @field HideHP boolean
--- @field InAutomatedDialog boolean
--- @field InPartyInventory boolean
--- @field InUse boolean
--- @field InheritedForceSynch uint8
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
--- @field MovingCount uint8
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
--- @field field_0 int32
--- @field field_10 int32
--- @field field_14 FixedString
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
--- @field Dummy uint8


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
--- @field field_0 uint8
--- @field field_28 int32
--- @field field_4 int32


--- @class HealBlockComponent:BaseComponent


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


--- @class InventoryContainerComponent:BaseComponent
--- @field Items table<uint16, InventoryContainerComponentItem>


--- @class InventoryContainerComponentItem
--- @field Item EntityHandle
--- @field field_8 uint32


--- @class InventoryDataComponent:BaseComponent
--- @field Flags uint16
--- @field field_0 uint8


--- @class InventoryMemberComponent:BaseComponent
--- @field EquipmentSlot int16
--- @field Inventory EntityHandle


--- @class InventoryOwnerComponent:BaseComponent
--- @field Inventories EntityHandle[]
--- @field PrimaryInventory EntityHandle


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


--- @class ResourceBackground:ResourceGuidResource
--- @field Description TranslatedString
--- @field DisplayName TranslatedString
--- @field Hidden boolean
--- @field Passives string
--- @field Tags Guid[]
--- @field field_40 int64
--- @field field_48 int64


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


--- @class ResourceColor:ResourceGuidResource
--- @field Color vec4
--- @field DisplayName TranslatedString
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


--- @class ResourceSkillList:ResourceGuidResource
--- @field Skills SkillId[]


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
--- @field field_0 uint8
--- @field field_10 uint8
--- @field field_14 vec3
--- @field field_20 EntityHandle
--- @field field_28 uint8
--- @field field_29 uint8
--- @field field_2A uint8
--- @field field_2B uint8
--- @field field_2C uint8
--- @field field_2D uint8
--- @field field_2E uint8
--- @field field_4 vec3


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


--- @class SpellCastReposeState
--- @field field_0 int64
--- @field field_10 EntityHandle|nil
--- @field field_20 vec3|nil
--- @field field_8 EntityHandle


--- @class SpellCastReposeState2
--- @field Repose SpellCastReposeState
--- @field Repose2 SpellCastReposeState|nil
--- @field field_30 uint8


--- @class SpellCastRollsComponent:BaseComponent
--- @field Rolls SpellCastRollsComponentRoll[]


--- @class SpellCastRollsComponentRoll
--- @field Hits SpellCastRollsComponentRollHit[]
--- @field MHS_FS_i32 table<FixedString, int32>
--- @field field_0 EntityHandle
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
--- @field Entity EntityHandle
--- @field Repose SpellCastReposeState2[]
--- @field SpellId SpellIdBase
--- @field field_38 int32
--- @field field_50 vec3|nil
--- @field field_60 vec3|nil
--- @field field_70 vec3
--- @field field_8 EntityHandle
--- @field field_80 EntityHandle
--- @field field_88 int32
--- @field field_90 Guid
--- @field field_A0 string


--- @class SpellCastSyncTargetingComponent:BaseComponent
--- @field Repose SpellCastReposeState2[]
--- @field field_10 EntityHandle|nil
--- @field field_20 vec3|nil
--- @field field_40 uint8
--- @field field_44 int32
--- @field field_48 vec3|nil
--- @field field_58 vec3|nil
--- @field field_68 vec3|nil
--- @field field_78 vec3|nil
--- @field field_8 EntityHandle
--- @field field_88 vec3|nil


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


--- @class StatsEqualizeFunctor:StatsFunctor
--- @field HealType StatusHealType


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
--- @field SetIfLonger boolean
--- @field StatusId FixedString


--- @class StatsSpawnFunctor:StatsFunctor
--- @field Arg1 FixedString
--- @field StatusesToApply Array_FixedString
--- @field TemplateId FixedString


--- @class StatsSpawnInInventoryFunctor:StatsFunctor
--- @field AdditionalArgs Array_FixedString
--- @field Arg1 FixedString
--- @field Arg2 number
--- @field Arg3 boolean
--- @field Arg4 boolean
--- @field Arg5 boolean
--- @field Arg6 FixedString


--- @class StatsSpellPrototype


--- @class StatsSpellPrototypeUseCostGroup
--- @field Amount number
--- @field ResourceGroup Guid
--- @field Resources Guid[]
--- @field SubResourceId int32


--- @class StatsStabilizeFunctor:StatsFunctor


--- @class StatsSummonFunctor:StatsFunctor
--- @field Arg2 FixedString
--- @field Arg3 boolean
--- @field MovingObject FixedString
--- @field SpawnLifetime number
--- @field StatusesToApply Array_FixedString


--- @class StatsSummonInInventoryFunctor:StatsFunctor
--- @field AdditionalArgs Array_FixedString
--- @field Arg1 FixedString
--- @field Arg3 number
--- @field Arg4 boolean
--- @field Arg5 boolean
--- @field Arg6 boolean
--- @field Arg7 boolean
--- @field Arg8 FixedString


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
--- @field Acceleration string
--- @field ExplodeRadius string
--- @field TeleportDelay string
--- @field AmountOfTargets StatsConditions
--- @field MaxAttacks string
--- @field MovementSpeed string
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
--- @field OriginSpellProperties AIFlags
--- @field ThrowableSpellFail AIFlags
--- @field ProjectileDelay string
--- @field AoEConditions VerbalIntent
--- @field MemorizationRequirements Properties
--- @field Lifetime string
--- @field OriginSpellRoll WeaponFlags
--- @field ThrowableTargetConditions VerbalIntent
--- @field CastTargetHitDelay string
--- @field MinHitsPerTurn string
--- @field OriginSpellSuccess AIFlags
--- @field Angle string
--- @field Height string
--- @field MaxHitsPerTurn string
--- @field OriginSpellFail AIFlags
--- @field Trajectories StatsConditions
--- @field SingleSource FormatStringColor
--- @field HitDelay string
--- @field OriginTargetConditions VerbalIntent
--- @field Icon StatsConditions
--- @field ForceTarget string
--- @field TeleportSelf FormatStringColor
--- @field DisplayName AuraFlags
--- @field DisplayNameRef AuraFlags
--- @field Template StatsConditions
--- @field UseWeaponDamage FormatStringColor
--- @field TeleportSurface FormatStringColor
--- @field Description AuraFlags
--- @field DescriptionRef AuraFlags
--- @field UseWeaponProperties FormatStringColor
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
--- @field ItemWall StatsConditions
--- @field CycleConditions VerbalIntent
--- @field ItemWallStatus StatsConditions
--- @field UseCosts StatsConditions
--- @field WallStartEffect StatsConditions
--- @field DualWieldingUseCosts StatsConditions
--- @field WallEndEffect StatsConditions
--- @field HitCosts StatsConditions
--- @field RitualCosts StatsConditions
--- @field SpellAnimation StatsConditions
--- @field DualWieldingSpellAnimation StatsConditions
--- @field VerbalIntent ResistanceFlags
--- @field SpellStyleGroup SpellCategoryFlags
--- @field WeaponTypes CastCheckType
--- @field SpellFlags Conditions
--- @field SpellActionType CursorMode
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
--- @field IgnoreTeleport FormatStringColor
--- @field SelectedCharacterEffect StatsConditions
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
--- @field LoadFile fun(a1:string)
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



--- @class Ext_StaticData
--- @field Get fun(a1:Guid, a2:ExtResourceManagerType)
--- @field GetAll fun(a1:ExtResourceManagerType):Guid[]
local Ext_StaticData = {}



--- @class Ext_Stats
--- @field TreasureCategory Ext_StatsTreasureCategory
--- @field TreasureTable Ext_StatsTreasureTable
--- @field AddAttribute fun(a1:FixedString, a2:FixedString, a3:FixedString):boolean
--- @field AddEnumerationValue fun(typeName:FixedString, enumLabel:FixedString):int32
--- @field Create fun(a1:FixedString, a2:FixedString)
--- @field EnumIndexToLabel fun(a1:FixedString, a2:int32)
--- @field EnumLabelToIndex fun(a1:FixedString, a2:FixedString)
--- @field Get fun(a1:string, a2:int32|nil)
--- @field GetModifierAttributes fun(a1:FixedString)
--- @field GetStats fun():FixedString[]
--- @field GetStatsLoadedBefore fun(a1:FixedString):FixedString[]
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
--- @field MonotonicTime fun():int64
--- @field Print fun()
--- @field PrintError fun()
--- @field PrintWarning fun()
--- @field ShowErrorAndExitGame fun(a1:string)
--- @field Version fun():int32
local Ext_Utils = {}



--- @class Ext_Vars
--- @field DirtyModVariables fun(a1:Guid|nil)
--- @field DirtyUserVariables fun(a1:Guid|nil)
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
--- @field StaticData Ext_StaticData
--- @field Stats Ext_Stats
--- @field Types Ext_Types
--- @field Utils Ext_Utils
--- @field Vars Ext_Vars

--#region Generated Enums

--- @class Ext_Enums
local Ext_Enums = {}


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
--- @field StaticData Ext_StaticData
--- @field Stats Ext_Stats
--- @field Types Ext_Types
--- @field Utils Ext_Utils
--- @field Vars Ext_Vars
--- @field Enums Ext_Enums
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
Ext.Events.ModuleLoading = {}
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

