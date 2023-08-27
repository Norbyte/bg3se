BEGIN_CLS(esv::Status)
P_RO(SomeHandle)
P_RO(SomeEntityHandle_Old)
P_RO(SomeState)
P_RO(StatusID_Old)
P_RO(field_38)
P_RO(NetID_Old)
P_RO(field_48)
P_RO(NetID)
P(StoryActionID)
P_REF(Originator)
P_RO(field_78)
P_RO(StatusId)
P(StartTimer)
P(LifeTime)
P(CurrentLifeTime)
P(TurnTimer)
P(Strength)
P(CauseType)
P_RO(StatusHandle)
P_RO(UnknownHandle)
P_RO(OwnerHandle)
P_REF(StatusOwner)
P_RO(StatusSource)
P_RO(StatusSourceUUID)
P_RO(StatusSourceEntityHandle)
P_RO(StatusSource_M)
P_RO(CleansedByHandle_M)
P_RO(field_110)
P_RO(Conditions)
P_RO(RemoveEvents)
P_RO(Flags2)
P_RO(Flags0)
P_RO(field_118)
P_RO(field_11C)
P_RO(TickType)
P_RO(Flags3)
P_RO(SomeFlags)

//P_FUN(GetEngineType, StatusGetEngineType)
END_CLS()


BEGIN_CLS(esv::StatusAura)
INHERIT(esv::Status)
// P_REF(Aura) - TODO - map aura type
END_CLS()


BEGIN_CLS(esv::StatusBoost)
INHERIT(esv::StatusAura)
P_REF(Spell)
P_REF(Items)
P(LoseControl)
P_REF(ItemHandles)
P(EffectTime)
P(StackId)
P(SourceDirection)
// P_REF(SurfaceChanges) - TODO
END_CLS()


BEGIN_CLS(esv::StatusActiveDefense)
INHERIT(esv::StatusBoost)
P(Charges)
P(TargetPos)
P(Target)
P(Radius)
P(Projectile)
P_RO(TargetConditions_M) // TODO - map conditions
P_REF(UnknVectors)
P_REF(PreviousTargets)
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


BEGIN_CLS(esv::StatusDestroying)
INHERIT(esv::Status)
P_REF(HitDescription)
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
P_REF(HitDescription)
P_RO(Combat)
P_RO(DyingFlags)
END_CLS()


BEGIN_CLS(esv::StatusEncumbered)
INHERIT(esv::Status)
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


BEGIN_CLS(esv::StatusHealSharing)
INHERIT(esv::StatusBoost)
P(Caster)
END_CLS()


BEGIN_CLS(esv::StatusHealSharingCaster)
INHERIT(esv::StatusBoost)
P_REF(Targets)
END_CLS()


BEGIN_CLS(esv::StatusHealing)
INHERIT(esv::StatusBoost)
P(HealAmount)
P(TimeElapsed)
P(HealEffect)
P(HealEffectId)
P(SkipInitialEffect)
P(HealingEvent)
P(HealType)
P(AbsorbSurfaceRange)
END_CLS()


BEGIN_CLS(esv::StatusIdentify)
INHERIT(esv::Status)
P(Level)
P(Identified)
P(Identifier)
END_CLS()


BEGIN_CLS(esv::StatusInSurface)
INHERIT(esv::Status)
// TODO - LayerChecks
P(Translate)
END_CLS()


BEGIN_CLS(esv::StatusInfectiousDiseased)
INHERIT(esv::StatusBoost)
P(InfectiousDiseaseDepth)
P(InfectTimer)
P(InfectiousDiseaseRadius)
P(Target)
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
P(Partner)
P(ShowOverhead)
P_REF(Spell)
P(IgnoreHasSpell)
P(IgnoreChecks)
END_CLS()


BEGIN_CLS(esv::StatusRepair)
INHERIT(esv::Status)
P(Level)
P(Repaired)
P(Repairer)
END_CLS()


BEGIN_CLS(esv::StatusRotate)
INHERIT(esv::Status)
P(Yaw)
P(RotationSpeed)
END_CLS()


BEGIN_CLS(esv::StatusSitting)
INHERIT(esv::Status)
P(Item)
P(Position)
END_CLS()


BEGIN_CLS(esv::StatusTeleportFalling)
INHERIT(esv::Status)
P(Target)
P(ReappearTime)
P_REF(Spell)
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


BEGIN_CLS(esv::StatusSummoning)
INHERIT(esv::Status)
END_CLS()


BEGIN_CLS(esv::StatusStoryFrozen)
INHERIT(esv::Status)
END_CLS()


BEGIN_CLS(esv::StatusLying)
INHERIT(esv::StatusSitting)
END_CLS()


BEGIN_CLS(esv::StatusFear)
INHERIT(esv::StatusBoost)
END_CLS()


BEGIN_CLS(esv::StatusSmelly)
INHERIT(esv::Status)
END_CLS()


BEGIN_CLS(esv::StatusClean)
INHERIT(esv::Status)
END_CLS()


BEGIN_CLS(esv::StatusWindWalker)
INHERIT(esv::StatusBoost)
END_CLS()


BEGIN_CLS(esv::StatusDecayingTouch)
INHERIT(esv::StatusBoost)
END_CLS()


BEGIN_CLS(esv::StatusUnhealable)
INHERIT(esv::Status)
END_CLS()


BEGIN_CLS(esv::StatusForceMove)
INHERIT(esv::Status)
END_CLS()


BEGIN_CLS(esv::StatusConstrained)
INHERIT(esv::StatusSitting)
END_CLS()


BEGIN_CLS(esv::StatusEffect)
INHERIT(esv::Status)
END_CLS()


BEGIN_CLS(esv::StatusDeactivated)
INHERIT(esv::StatusBoost)
END_CLS()
