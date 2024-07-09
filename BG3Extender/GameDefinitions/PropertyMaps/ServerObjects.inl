BEGIN_CLS(esv::PlayerData)
P(PlayerHandle)
P(QuestSelected)
P(field_4C)
P(CustomData)
P(field_68)
P(PreviousPositions)
P(PreviousPositionId)
P(HelmetOption)
P(Renown)
P(CachedTension)
P(IsInDangerZone)
P(Region)
P(field_94)
END_CLS()


BEGIN_CLS(esv::StatusMachine)
P(Statuses)
END_CLS()


BEGIN_CLS(esv::Character)
INHERIT(BaseComponent)
P(Flags)
P_BITMASK(Flags)
P_RO(Level)
P_RO(MyHandle)
P(Summons)
P(CreatedTemplateItems)
P(Treasures)
P(DisabledCrime)
P(PreferredAiTargets)
P(ServerControl)
P(Template)
P(OriginalTemplate)
P(TemplateUsedForSpells)
P(StatusManager)
P(PlayerData)
P(Inventory)
P(OwnerCharacter)
P(FollowCharacter)
P(EnemyCharacter)
P(Dialog)
P(CustomTradeTreasure)
P(BaseVisual)
P(UserID)
P(UserID2)
P(GeneralSpeedMultiplier)
P(CrimeHandle)
P(PreviousCrimeHandle)
P(InvestigationTimer)
P(CrimeState)
P(PreviousCrimeState)
P(BlockNewDisturbanceReactions)
P(HasOsirisDialog)
P(NeedsUpdate)
P(ForceSynch)
P(NumConsumables)
P(Flags2)
P_BITMASK(Flags2)
P(Flags3)
P_BITMASK(Flags3)

P_GETTER(Character, LuaGetSelf)

// Functions
P_FUN(GetStatus, esv::Character::GetStatus)
P_FUN(GetStatusByType, esv::Character::GetStatusByType)
END_CLS()


BEGIN_CLS(eoc::PlayerCustomData)
P_RO(Initialized)
P(OwnerProfileID)
P(ReservedProfileID)
END_CLS()


BEGIN_CLS(esv::Item)
INHERIT(BaseComponent)
P_RO(field_10)
P(Flags)
P_BITMASK(Flags)
P_RO(Level)
P_RO(MyHandle)
P(Template)
P(OriginalTemplate)
P(StatusManager)
P(field_68)
P(field_70)
P(Stats)
P(ItemType)
P(PreviousLevel)
P(TreasureLevel)
P(Flags2)
P_BITMASK(Flags2)

P_GETTER(Item, LuaGetSelf)
END_CLS()
