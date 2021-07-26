FS_NAME(Empty, "");
FS(CurrentVitality);
FS(CurrentArmor);
FS(CurrentMagicArmor);
FS(ForceStatus);
FS(ForceFailStatus);
FS(RequestDeleteAtTurnEnd);
FS(LifeTime);
FS(CurrentLifeTime);
FS(SimulateHit);
FS(HitType);
FS(NoHitRoll);
FS(ProcWindWalker);
FS(ForceReduceDurability);
FS(HighGround);
FS(CriticalRoll);
FS(HitReason);
FS(DamageSourceType);
FS(Strength);
FS(ImpactPosition);
FS(ImpactOrigin);
FS(ImpactDirection);
FS(SkillId);
FS(SourcePosition);
FS(TargetPosition);
FS(HitObjectPosition);
FS(Caster);
FS(Source);
FS(Target);
FS(HitObject);
FS(Handle);
FS(Activated);
FS(OffStage);
FS(Destroyed);
FS(Global);
FS(IsPlayer);
FS(Multiplayer);
FS(InParty);
FS(HostControl);
FS(Dead);
FS(HasOwner);
FS(InDialog);
FS(Summon);
FS(CharacterControl);
FS(InArena);
FS(Deactivated);
FS(MadePlayer);
FS(IsGameMaster);
FS(IsPossessed);
FS(CurrentTemplate);
FS(OriginalTemplate);
FS(TemplateUsedForSpells);
FS_NAME(NullStringHandle, "ls::TranslatedStringRepository::s_HandleUnknown");

// Game object funcs
FS(Stats);
FS(GetOwnerCharacter);
FS(WalkSpeed);
FS(RunSpeed);
FS(Construct);
FS(ResetProgression);
FS(GenerationBoosts);
FS(RuneBoosts);

// AI grid funcs
FS(SearchForCell);
FS(GetCellInfo);
FS(GetAiFlags);
FS(SetAiFlags);
FS(OffsetX);
FS(OffsetY);
FS(OffsetZ);
FS(GridScale);
FS(Width);
FS(Height);

// Character stats
FS(DynamicStats);
FS(MainWeapon);
FS(OffHandWeapon);
FS(NotSneaking);
FS(Rotation);
FS(Position);
FS(MyGuid);
FS(NetID);
FS(GetItemBySlot);
FS(PlayerCustomData);
FS(RootTemplate);
FS(Sight);

// Stat attributes
FS(Level);
FS(Name);
FS(Using);
FS(AIFlags);
FS(IngredientType);
FS(IngredientTransformType);
FS(ItemDataRarity);
FS(RecipeCategory);
FS(CraftingStationType);
FS(ShroudType);
FS(ComboCategory);
FS(SkillProperties);
FS(ExtraProperties);
FS(Lifetime);

// Spell stat properties
FS(Ability);
FS(Tier);
FS(Requirement);
FS(Icon);
FS_NAME(MagicCost, "Magic Cost");
FS_NAME(MemoryCost, "Memory Cost");
FS(ActionPoints);
FS(Cooldown);
FS(CooldownReduction);
FS(ChargeDuration);
FS(DisplayName);

// Status stat properties
FS(StatusType);
FS(HIT);
FS(MADNESS);

// Stat modifier types
FS(Character);
FS(Armor);
FS(Object);
FS(Weapon);
FS(SpellData);
FS(StatusData);
FS(PassiveData);

FS(SpellSet);
FS(EquipmentSet);
FS(TreasureTable);
FS(TreasureCategory);
FS(ItemGroup);
FS(NameGroup);

// Stat proxy functions
FS(Sync);
FS(SetPersistence);

// Entity proxy functions
FS(HasRawComponent);
FS(GetComponentHandles);
FS(GetComponent);
FS(GetAllComponents);
FS(GetEntityType);
FS(GetSalt);
FS(GetIndex);
FS(IsAlive);

// Stat modifier value types
FS(FixedString);
FS(StatusIDs);
FS(ConstantInt);
FS(ConstantFloat);
FS(Guid);
FS(StatsFunctors);
FS(Conditions);
FS(TargetConditions);
FS(UseConditions);
FS(RollConditions);
FS(Requirements);

FS(AttributeFlags);
FS(SpellFlagList);
FS(WeaponFlags);
FS(ResistanceFlags);
FS(PassiveFlags);
FS(ProficiencyGroupFlags);
FS(StatsFunctorContext);
FS(StatusEvent);
FS(StatusPropertyFlags);
FS(StatusGroupFlags);
FS(LineOfSightFlags);

FS(Default);

// Enumerations
FS(StatusHealType);
FS_NAME(OsirisTask, "Osiris Task");
FS_NAME(GameAction, "Game Action");
FS_NAME(SurfaceChange, "Surface Change");
FS_NAME(CustomProperties, "Custom Properties");
FS(PathRootType);
FS(ComponentHandleType);
FS(ExtComponentType);
FS(ExtResourceManagerType);
FS(GameState);
FS(ComponentType);
FS(NetMessage);
FS(PlayerUpgradeAttribute);
FS(StatsFunctorActionId);
FS(FunctorExecParamsType);
FS(StatsPropertyContext);
FS(DealDamageWeaponType);
FS(DealDamageWeaponDamageType);
FS(ExecuteWeaponFunctorsType);
FS(ForceFunctorOrigin);
FS(ForceFunctorAggression);
FS(StatGetterType);
FS(RequirementType);
FS(WeaponType);
FS(ArmorType);
FS(HandednessType);
FS(AbilityType);
FS(AbilityId);
FS(RollTypeId);
FS(SpellSchoolId);
FS(ProjectileTypeIds);
FS(AttackRoll);
FS(AdvantageTypeId);
FS(AdvantageBoostType);
FS(DiceSizeId);
FS(SpellSourceType);
FS(Gender);
FS(SpellChildSelectionType);
FS(SpellCooldownType);
FS(SourceAdvantageType);
FS(ProficiencyBonusBoostType);
FS(CriticalHitBoostFlags);
FS(ResistanceBoostFlags);
FS(HighGroundBonus);
FS(EquipmentStatsType);
FS(StatusFlags);
FS(StatusFlags0);
FS(StatusFlags1);
FS(StatusFlags2);
FS(CauseType);
FS(DeathType);
FS(DamageType);
FS(ItemSlot);
FS(ItemSlot32);
FS(StatCharacterFlags);
FS(HitFlag);
FS(HitWith);
FS(DamageFlags);
FS(HealEffect);
FS(CharacterFlags);
FS(CharacterFlags2);
FS(CharacterFlags3);
FS(ItemFlags);
FS(ItemFlags2);
FS(GameActionType);
FS(StatAttributeFlags);
FS(TalentType);
FS(SurfaceType);
FS(SurfaceLayer);
FS(SurfaceLayer8);
FS(SurfaceActionType);
FS(SurfaceTransformActionType);
FS(ESurfaceFlag);
FS(BoostType);
FS(SpellType);
FS(ResourceBankType);

// Savegames
FS(ScriptExtenderSave);
FS(ExtenderVersion);
FS(LuaVariables);
FS(Mod);
FS(ModId);

FS(StatObject);
FS(StatId);
FS(StatType);
FS(Blob);
