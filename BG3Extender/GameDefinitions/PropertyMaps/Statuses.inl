BEGIN_CLS(esv::Status)
P(field_8)
P_RO(field_18)
P(StoryActionID)
P(Originator)
P_RO(field_48)
P_RO(StatusId)
P(StartTimer)
P(LifeTime)
P(CurrentLifeTime)
P(TurnTimer)
P(Strength)
P(CauseType)
P(OriginCauseType)
P(SourceEquippedItem)
P(SourceUsedItem)
P_RO(StatusHandle)
P(SyncEntity)
P_RO(Owner)
P(StatusOwner)
P(Cause)
P(CauseGUID)
P(StatusSource)
P(RootCause)
P(field_E0)
P(field_E8)
P_RO(RemoveConditions)
P(RemoveEvents)
P(Flags)
P_BITMASK(Flags)
P(Flags2)
P_BITMASK(Flags2)
P(Flags3)
P_BITMASK(Flags3)
P(FreezeDuration)
P(ExitCause)
P(field_105)
P(SpellCastingAbility)
P(SourceSpell)
P(SpellCastSourceUuid)
P(StackId)
P(StackPriority)
P(IsRecoverable)
P(DifficultyStatus)
P(StackedSpellCastSourceUuid)
// TODO - P(ConditionRolls)
P(TickType)
P(Flags4)
P_BITMASK(Flags4)
END_CLS()


BEGIN_CLS(esv::StatusAura)
INHERIT(esv::Status)
// P(Aura) - TODO - map aura type
END_CLS()


BEGIN_CLS(esv::StatusBoost)
INHERIT(esv::StatusAura)
P(Spell)
P(Items)
P(LoseControl)
P(ItemHandles)
P(EffectTime)
P(BoostStackId)
P(SourceDirection)
// P(SurfaceChanges) - TODO
END_CLS()


BEGIN_CLS(esv::StatusClimbing)
INHERIT(esv::Status)
P_RO(MoveDirection_M)
P_RO(Item)
P_RO(Level)
P_RO(Status)
P_RO(Direction)
P_RO(JumpUpLadders_M)
P_RO(Incapacitated_M)
P_RO(Started_M)
END_CLS()


BEGIN_CLS(esv::StatusIncapacitated)
INHERIT(esv::StatusBoost)
P(CurrentFreezeTime)
P(FreezeTime)
P(IncapacitateFlags)
P(IncapacitationAnimationFinished)
END_CLS()


BEGIN_CLS(esv::StatusDowned)
INHERIT(esv::StatusIncapacitated)
P(NumStableSuccess)
P(NumStableFailed)
P(StableRollDC)
// FIXME - Roll, OnFailed, OnSuccess, pRoll
P(IsStable)
P(IsHealed)
P(RollSuccesses)
P(RollFailures)
P(DamageFailures)
END_CLS()


BEGIN_CLS(esv::StatusDying)
INHERIT(esv::Status)
P_RO(Source)
P(HitDescription)
P_RO(Combat)
P_RO(DyingFlags)
END_CLS()


BEGIN_CLS(esv::StatusHeal)
INHERIT(esv::Status)
P(EffectTime)
P(HealAmount)
P(HealEffect)
P(HealEffectId)
P(HealType)
P(AbsorbSurfaceRange)
P(TargetDependentHeal)
P(TargetDependentValue)
P(TargetDependentHealAmount)
END_CLS()


BEGIN_CLS(esv::StatusInSurface)
INHERIT(esv::Status)
// TODO - LayerChecks
P(Translate)
END_CLS()


BEGIN_CLS(esv::StatusInvisible)
INHERIT(esv::StatusBoost)
P(InvisiblePosition)
END_CLS()


BEGIN_CLS(esv::StatusKnockedDown)
INHERIT(esv::Status)
P(KnockedDownState)
P(IsInstant)
END_CLS()


BEGIN_CLS(esv::StatusMaterial)
INHERIT(esv::Status)
P(MaterialUUID)
P(ApplyFlags)
P(IsOverlayMaterial)
P(Fading)
P(ApplyNormalMap)
P(Force)
END_CLS()


BEGIN_CLS(esv::StatusPolymorphed)
INHERIT(esv::StatusBoost)
P(Id)
END_CLS()


BEGIN_CLS(esv::StatusReaction)
INHERIT(esv::Status)
P(Source)
P(Target)
P(TargetPosition)
P(ShowOverhead)
P(Spell)
P(IgnoreHasSpell)
P(IgnoreChecks)
END_CLS()


BEGIN_CLS(esv::StatusRotate)
INHERIT(esv::Status)
P(Yaw)
P(RotationSpeed)
END_CLS()


BEGIN_CLS(esv::StatusTeleportFalling)
INHERIT(esv::Status)
P(Target)
P(ReappearTime)
P(Spell)
P(HasDamage)
P(HasDamageBeenApplied)
END_CLS()


BEGIN_CLS(esv::StatusUnlock)
INHERIT(esv::Status)
P(Source)
P(Success)
P(Unlocked)
END_CLS()


BEGIN_CLS(esv::StatusSneaking)
INHERIT(esv::StatusBoost)
P(ClientRequestStop)
END_CLS()


BEGIN_CLS(esv::StatusStoryFrozen)
INHERIT(esv::Status)
END_CLS()


BEGIN_CLS(esv::StatusFear)
INHERIT(esv::StatusBoost)
END_CLS()


BEGIN_CLS(esv::StatusSmelly)
INHERIT(esv::Status)
END_CLS()


BEGIN_CLS(esv::StatusEffect)
INHERIT(esv::Status)
END_CLS()


BEGIN_CLS(esv::StatusDeactivated)
INHERIT(esv::StatusBoost)
END_CLS()
