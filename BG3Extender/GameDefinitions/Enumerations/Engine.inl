BEGIN_ENUM(LuaTypeId, uint32_t)
    EV(Unknown, 0)
    EV(Void, 1)
    EV(Boolean, 2)
    EV(Integer, 3)
    EV(Float, 4)
    EV(String, 5)
    EV(Object, 6)
    EV(Array, 7)
    EV(Set, 8)
    EV(Map, 9)
    EV(Tuple, 10)
    EV(Enumeration, 11)
    EV(Function, 12)
    EV(Any, 13)
    EV(Nullable, 14)
    EV(Module, 15)
    EV(Variant, 16)
END_ENUM()

BEGIN_ENUM(PathRootType, int)
    EV(Root, 0)
    EV(Data, 1)
    EV(Public, 2)
    EV(LocalAppData, 3)
    EV(UserProfile, 4)
    EV(Localization, 5)
    EV(Bin, 6)
    EV(Bin2, 7)
    EV(Mods, 8)
    EV(Projects, 9)
    EV(Public2, 10)
    EV(GameMod, 11)
    EV(EngineMod, 12)
    EV(Scripts, 13)
    EV(WorkingDir, 14)
    EV(Debug, 15)
END_ENUM()

BEGIN_BITMASK(InputType, uint8_t)
    EV(Press, 1)
    EV(Release, 2)
    EV(ValueChange, 4)
    EV(Hold, 8)
    EV(Repeat, 16)
    EV(AcceleratedRepeat, 32)
    EV(ClearOnRelease, 0x40)
END_ENUM()

BEGIN_BITMASK(InputModifier, uint8_t)
    EV(Alt, 1)
    EV(Ctrl, 2)
    EV(Shift, 4)
    EV(Gui, 8)
END_ENUM()

BEGIN_ENUM(InputState, int8_t)
    EV(Released, 0)
    EV(Pressed, 1)
END_ENUM()

BEGIN_ENUM_NS(ecl, GameState, ClientGameState, uint32_t)
    EV(Unknown, 0)
    E(Init)
    E(InitMenu)
    E(InitNetwork)
    E(InitConnection)
    E(Idle)
    E(LoadMenu)
    E(Menu)
    E(Exit)
    E(SwapLevel)
    EV(LoadLevel, 10)
    E(LoadModule)
    E(LoadSession)
    E(UnloadLevel)
    E(UnloadModule)
    E(UnloadSession)
    E(Paused)
    E(PrepareRunning)
    E(Running)
    E(Disconnect)
    EV(Join, 20)
    E(Save)
    E(StartLoading)
    E(StopLoading)
    E(StartServer)
    E(Movie)
    E(Installation)
    E(ModReceiving)
    E(Lobby)
    E(BuildStory)
    EV(GeneratePsoCache, 32)
    E(LoadPsoCache)
    E(AnalyticsSessionEnd)
END_ENUM_NS()


BEGIN_ENUM_NS(esv, GameState, ServerGameState, uint32_t)
    EV(Unknown, 0)
    E(Uninitialized)
    E(Init)
    E(Idle)
    E(Exit)
    E(LoadLevel)
    E(LoadModule)
    E(LoadSession)
    E(UnloadLevel)
    E(UnloadModule)
    EV(UnloadSession, 10)
    E(Sync)
    E(Paused)
    E(Running)
    E(Save)
    E(Disconnect)
    E(BuildStory)
    E(ReloadStory)
END_ENUM_NS()


BEGIN_ENUM(ResourceBankType, uint32_t)
    EV(Visual, 0)
    EV(VisualSet, 1)
    EV(Animation, 2)
    EV(AnimationSet, 3)
    EV(Texture, 4)
    EV(Material, 5)
    EV(Physics, 6)
    EV(Effect, 7)
    EV(Script, 8)
    EV(Sound, 9)
    EV(Lighting, 10)
    EV(Atmosphere, 11)
    EV(AnimationBlueprint, 12)
    EV(MeshProxy, 13)
    EV(MaterialSet, 14)
    EV(BlendSpace, 15)
    EV(FCurve, 16)
    EV(Timeline, 17)
    EV(Dialog, 18)
    EV(VoiceBark, 19)
    EV(TileSet, 20)
    EV(IKRig, 21)
    EV(Skeleton, 22)
    EV(VirtualTexture, 23)
    EV(TerrainBrush, 24)
    EV(ColorList, 25)
    EV(CharacterVisual, 26)
    EV(MaterialPreset, 27)
    EV(SkinPreset, 28)
    EV(ClothCollider, 29)
    EV(DiffusionProfile, 30)
    EV(LightCookie, 31)
    EV(TimelineScene, 32)
    EV(SkeletonMirrorTable, 33)
    EV(Sentinel, 34)
END_ENUM()


BEGIN_ENUM(TextureType, uint8_t)
    EV(T1D, 0)
    EV(T2D, 1)
    EV(T3D, 2)
    EV(T1DArray, 3)
    EV(T2DArray, 4)
    EV(T3DArray, 5)
    EV(T2DCube, 6)
    EV(T2DCubeArray, 7)
    EV(T2DRenderTarget, 8)
END_ENUM()


BEGIN_ENUM(SceneObjectType, uint32_t)
    EV(RenderableObject, 0)
    EV(Light, 1)
    EV(LightProbe, 2)
    EV(FogVolume, 3)
    EV(Visual, 4)
    EV(Effect, 5)
    EV(InstanceBatchRenderableObject, 6)
    EV(InstancingObject, 7)
    EV(CullableInstance, 8)
    EV(DeferredDecal, 9)
    EV(RoomTriggerVisual, 10)
END_ENUM()


BEGIN_BITMASK(AnimationSetAnimationFlags, uint8_t)
    EV(AlwaysIgnore, 1)
    EV(NoFallback, 2)
END_ENUM()

BEGIN_ENUM(TextKeyType, uint8_t)
    EV(Sound, 1)
    EV(Effect, 2)
    EV(FootStep, 3)
    EV(Attach, 4)
    EV(WeaponEffect, 6)
    EV(Genome, 7)
    EV(Attack, 8)
    EV(Ragdoll, 9)
    EV(VisualCullFlag, 10)
    EV(FloatData, 11)
    EV(Foley, 12)
    EV(Vocal, 13)
    EV(FootMove, 14)
    EV(React, 15)
END_ENUM()

BEGIN_ENUM_NS(aspk, PropertyType, EffectPropertyType, uint8_t)
    EV(Boolean, 0)
    EV(Integer, 1)
    EV(IntegerRange, 2)
    EV(ColorARGBKeyFrame, 3)
    EV(Float, 4)
    EV(FloatRange, 5)
    EV(FloatKeyFrame, 6)
    EV(String, 7)
    EV(Vector3, 8)
    EV(FixedFunction, 9)
    EV(FixedString, 10)
    EV(Base, 11)
END_ENUM_NS()

BEGIN_BITMASK(AppliedMaterialFlags, uint8_t)
    EV(Instantiated, 1)
    EV(IsOverlay, 2)
    EV(OverlayOriginalMapsSet, 4)
END_ENUM()

BEGIN_ENUM(AIBoundType, uint8_t)
    EV(Move, 0)
    EV(Hit, 1)
    EV(Stand, 2)
END_ENUM()

BEGIN_ENUM(AIShapeType, uint8_t)
    EV(None, 0)
    EV(Box, 1)
    EV(Cylinder, 2)
END_ENUM()

BEGIN_ENUM(OsiFunctionType, uint8_t)
    EV(Call, 0)
    EV(Query, 1)
    EV(Event, 2)
    EV(DB, 3)
    EV(Proc, 4)
END_ENUM()

BEGIN_BITMASK(DialogNodeFlags, uint16_t)
    EV(IsEndNode, 0x1)
    EV(IsOptional, 0x2)
    EV(IsExclusive, 0x4)
    EV(ShowOnlyOnce, 0x8)
    EV(IsGameplayNode, 0x10)
    EV(IsRootNode, 0x20)
    EV(SuppressSubtitle, 0x40)
    EV(IsGreetingNode, 0x80)
    EV(AllowGrouping, 0x100)
    EV(IsEndAllDialogs, 0x200)
END_ENUM()

BEGIN_BITMASK(VisualFlags, uint32_t)
    EV(CastShadow, 0x1)
    EV(ReceiveDecal, 0x2)
    EV(Reflecting, 0x4)
    EV(DisableLOD, 0x8)
    EV(DisableCulling, 0x10)
    EV(IsWall, 0x20)
    EV(IsShadowProxy, 0x40)
    EV(AllowReceiveDecalWhenAnimated, 0x80)
    EV(IsEffect, 0x100)
    EV(Unknown200, 0x200)
    EV(IsScenery, 0x400)
    EV(PlayingAttachedEffects, 0x800)
    EV(ShowMesh, 0x1000)
    EV(HasSkeleton, 0x2000)
    EV(ReceiveColorFromParent, 0x4000)
    EV(AllowTPose, 0x8000)
    EV(HasBlueprint, 0x10000)
    EV(SeeThrough, 0x20000)
    EV(PreparedDestroyed, 0x40000000)
    EV(PreparedDestroyed2, 0x80000000)
END_ENUM()

BEGIN_BITMASK(VisualCullFlags, uint16_t)
    EV(CullNonProxy, 0x02)
    EV(CullShadow, 0x04)
    EV(CullShadow2, 0x08)
    EV(CullShadow3, 0x10)
    EV(CullDecal, 0x80)
    EV(CullRenderView1, 0x200)
    EV(CullRenderView2, 0x400)
    EV(CullScenery, 0x800)
    EV(CullShadowProxy, 0x4000)
    EV(CullShadow4, 0x8000)
END_ENUM()

BEGIN_BITMASK(VisualAttachmentFlags, uint32_t)
    EV(ExcludeFromBounds, 0x01)
    EV(KeepRot, 0x04)
    EV(KeepScale, 0x08)
    EV(UseLocalTransform, 0x10)
    EV(InheritAnimations, 0x20)
    EV(VisualSet, 0x40)
    EV(SupportsVertexColorMask, 0x80)
    EV(Hair, 0x100)
    EV(EffectComponent_M, 0x200)
    EV(TextKeyEffect_M, 0x400)
    EV(DoNotUpdate, 0x800)
    EV(ParticleSystem, 0x1000)
    EV(HasSkeleton, 0x2000)
    EV(DestroyWithParent, 0x8000)
END_ENUM()

BEGIN_BITMASK(VisualObjectType, uint8_t)
    EV(Type01, 0x01)
    EV(Type02, 0x02)
    EV(Type04, 0x04)
    EV(Beard, 0x08)
    EV(Ears, 0x10)
    EV(Head, 0x20)
    EV(Horns, 0x40)
END_ENUM()

BEGIN_BITMASK(EffectFlags, uint16_t)
    EV(Forget, 0x1)
    EV(Pooled, 0x2)
    EV(Active, 0x4)
    EV(FullyInitialized, 0x8)
    EV(Playing, 0x10)
    EV(Stopped, 0x20)
    EV(FullyInitialized2, 0x200)
    EV(Interrupted, 0x400)
END_ENUM()

BEGIN_BITMASK(MultiEffectFlags, uint32_t)
    EV(KeepRotation, 0x1)
    EV(KeepScale, 0x2)
    EV(UseOrientDirection, 0x4)
    EV(UseDistance, 0x8)
    EV(UseScaleOverride, 0x10)
    EV(DetachSource, 0x20)
    EV(DetachTarget, 0x40)
    EV(MainHand, 0x80)
    EV(OffHand, 0x100)
    EV(Enabled, 0x200)
END_ENUM()

BEGIN_ENUM(StatsExpressionType, uint8_t)
    EV(Roll, 0)
    EV(Add, 1)
    EV(Subtract, 2)
    EV(Divide, 3)
    EV(Multiply, 4)
    EV(Max, 5)
    EV(ForEach, 6)
    EV(Placeholder, 7)
    EV(ResourceRoll, 8)
    EV(Variable, 9)
    EV(Negate, 10)
END_ENUM()

BEGIN_ENUM(StatsContextType, uint8_t)
    EV(Unspecified, 0)
    EV(Target, 1)
    EV(Owner, 2)
    EV(Cause, 3)
END_ENUM()

BEGIN_ENUM(StatsExpressionVariableData, uint8_t)
    EV(SpellCastingAbility, 0)
    EV(ProficiencyBonus, 1)
    EV(Level, 2)
    EV(SpellDC, 3)
    EV(WeaponActionDC, 4)
    EV(UnarmedMeleeAbility, 5)
    EV(CurrentHP, 6)
    EV(MaxHP, 7)
    EV(SpellPowerLevel, 8)
    EV(TadpolePowersCount, 9)
    EV(DamageDone, 10)
    EV(Sentinel, 11)
END_ENUM()

BEGIN_ENUM(StatsExpressionVariableDataType, uint8_t)
    EV(AbilityOverride, 0)
    EV(SavingThrow, 1)
    EV(StatusDuration, 2)
    EV(StatusStacks, 3)
    EV(CharacterData, 4)
    EV(LevelMapValue, 5)
    EV(ClassLevel, 6)
END_ENUM()

BEGIN_BITMASK(StatsExpressionVariableDataModifier, uint8_t)
    EV(Modifier, 1)
    EV(Flat, 2)
    EV(SavingThrow, 4)
    EV(DialogueCheck, 8)
    EV(Advantage, 0x10)
    EV(Disadvantage, 0x20)
END_ENUM()

BEGIN_ENUM_NS(esv, OsirisTaskType, OsirisTaskType, uint32_t)
    EV(MoveToLocation, 0)
    EV(MoveToObject, 1)
    EV(FleeFromGrid, 2)
    EV(MoveInRange, 3)
    EV(TeleportToLocation, 4)
    EV(Appear, 6)
    EV(Disappear, 7)
    EV(FollowNPC, 8)
    EV(FollowOwnerOrLeader, 9)
    EV(Wander, 10)
    EV(Steer, 11)
    EV(PlayAnimation, 12)
    EV(Drop, 13)
    EV(PickupItem, 14)
    EV(UseItem, 15)
    EV(MoveItem, 16)
    EV(Resurrect, 17)
    EV(UseSpell, 18)
    EV(MoveToAndTalk, 19)
    EV(LookAt, 20)
    EV(Timer, 21)
    EV(AutomatedDialog, 22)
    EV(RateLimitedAutomatedDialog, 23)
    EV(FleeFromRelation, 24)
    EV(FleeFromEntity, 25)
    EV(Combine, 26)
END_ENUM_NS()

BEGIN_ENUM_NS(esv, BehaviourType, ServerBehaviourType, uint8_t)
    EV(Move, 2)
    EV(Wander, 3)
    EV(Force, 4)
    EV(PickupItem, 5)
    EV(MoveItem, 6)
    EV(MoveAndUseItem, 7)
    EV(Appear, 10)
    EV(MoveToAndTalk, 11)
    EV(FollowCharacter, 12)
    EV(Patrol, 13)
END_ENUM_NS()

BEGIN_ENUM_NS(esv, ActionType, ServerActionType, uint8_t)
    EV(Animation, 2)
    EV(MoveItem, 4)
    EV(KnockedDown, 5)
    EV(UseItem, 6)
    EV(CombineItem, 8)
    EV(TeleportFalling, 9)
    EV(Incapacitated, 10)
END_ENUM_NS()

BEGIN_ENUM_NS(esv, MovementType, ServerMovementType, uint32_t)
    EV(MoveTo, 2)
    EV(MoveToSync, 3)
    EV(Idle, 4)
    EV(Climbing, 5)
    EV(Falling, 6)
    EV(Repose, 7)
END_ENUM_NS()

BEGIN_ENUM_NS(esv, SteeringType, ServerSteeringType, uint32_t)
    EV(Steer, 2)
    EV(LookAt, 3)
    EV(SteerTo, 4)
    EV(Repose, 5)
    EV(SteerTunnel, 6)
END_ENUM_NS()

BEGIN_ENUM(CursorType, uint32_t)
    EV(None, 0)
    EV(System, 1)
    EV(Arrow, 2)
    EV(ArrowWarning, 3)
    EV(Bow, 4)
    EV(BowWarning, 5)
    EV(BowGround, 6)
    EV(Wand, 7)
    EV(WandWarning, 8)
    EV(WandGround, 9)
    EV(Backstab, 0xA)
    EV(BackstabWarning, 0xB)
    EV(Cast, 0xC)
    EV(CastWarning, 0xD)
    EV(Combine, 0xE)
    EV(CombineWarning, 0xF)
    EV(ItemMove, 0x14)
    EV(ItemMoveWarning, 0x15)
    EV(ItemUse, 0x16)
    EV(ItemUseWarning, 0x17)
    EV(ItemPickup, 0x18)
    EV(ItemPickupWarning, 0x19)
    EV(Lock, 0x1A)
    EV(LockWarning, 0x1B)
    EV(Melee, 0x1C)
    EV(MeleeWarning, 0x1D)
    EV(MeleeGround, 0x1E)
    EV(OpenContainer, 0x1F)
    EV(OpenContainerWarning, 0x20)
    EV(OpenContainerNew, 0x21)
    EV(OpenDoor, 0x22)
    EV(OpenDoorWarning, 0x23)
    EV(PickPocket, 0x24)
    EV(PickPocketWarning, 0x25)
    EV(Shovel, 0x28)
    EV(ShovelWarning, 0x29)
    EV(Talk, 0x2A)
    EV(TalkWarning, 0x2B)
    EV(Walk, 0x2C)
    EV(WalkWarning, 0x2D)
    EV(CameraRotation, 0x2E)
    EV(Listen, 0x2F)
    EV(ListenWarning, 0x30)
    EV(InputText, 0x32)
    EV(LootedContainer, 0x33)
END_ENUM()

BEGIN_ENUM(AudioCodec, uint8_t)
    EV(Bank, 0)
    EV(PCM, 1)
    EV(ADPCM, 2)
    EV(XMA, 3)
    EV(Vorbis, 4)
    EV(PCMEX, 7)
    EV(External, 8)
    EV(OpusNX, 17)
    EV(Opus, 19)
    EV(OpusWEM, 20)
END_ENUM()

BEGIN_ENUM(SoundEventType, uint32_t)
    EV(Sound, 0)
    EV(Music, 1)
    EV(Ambient, 2)
    EV(HUD, 3)
    EV(CineHUD, 4)
END_ENUM()

BEGIN_BITMASK(SoundEventFlags, uint32_t)
    EV(SoundCache, 1)
    EV(UnloadCallback, 2)
    EV(WaitForLoad, 4)
    EV(EnableGetSourcePlayPosition, 8)
END_ENUM()

BEGIN_BITMASK_NS(ecl, CharacterTaskFlags, ClientCharacterTaskFlags, uint32_t)
    EV(IsExecuting, 0x1)
    EV(OverrideCursorOnHover, 0x2)
    EV(IsDummyTask, 0x4)
    EV(IgnoreAP, 0x8)
    EV(CannotActivateSameType, 0x10)
    EV(CannotPreviewSameType, 0x20)
    EV(SupportsPicking, 0x40)
    EV(IgnorePreviewCancel, 0x80)
    EV(NeedsPreviewToActivate, 0x100)
    EV(NeedsTaskToActivate, 0x200)
    EV(CombatTargeting, 0x1000)
    EV(CanInterruptRunningTask, 0x4000)
    EV(CanReenterPreview, 0x8000)
    EV(ConditionalInterruptByPreview, 0x10000)
    EV(ConditionalInterruptRunningTask, 0x20000)
    EV(PreviewingDifferentTask, 0x40000)
    EV(AlwaysInterruptRunningTask, 0x80000)
END_ENUM_NS()

BEGIN_BITMASK_NS(ecl, InputControllerFlags, ClientInputControllerFlags, uint8_t)
    EV(ControllerMode, 0x1)
END_ENUM_NS()

BEGIN_BITMASK_NS(ecl, InputControllerRequestFlags, ClientInputControllerRequestFlags, uint8_t)
    EV(RequestClear, 0x1)
    EV(RequestForceClear, 0x2)
    EV(RequestUpdatePreview, 0x4)
    EV(RequestCancelMovement, 0x8)
END_ENUM_NS()

BEGIN_ENUM(InputRawType, uint16_t)
    EV(enter, 0)
    EV(escape, 1)
    EV(backspace, 2)
    EV(tab, 3)
    EV(capslock, 4)
    EV(space, 5)
    EV(printscreen, 6)
    EV(scrolllock, 7)
    EV(pause, 8)
    EV(insert, 9)
    EV(home, 10)
    EV(pageup, 11)
    EV(del, 12)
    EV(end, 13)
    EV(pagedown, 14)
    EV(comma, 15)
    EV(hyphen, 16)
    EV(dot, 17)
    EV(slash, 18)
    EV(space2, 19)
    EV(equals, 20)
    EV(leftbracket, 21)
    EV(backslash, 22)
    EV(rightbracket, 23)
    EV(backtick, 24)
    EV(apostrophe, 25)
    EV(num_0, 26)
    EV(num_1, 27)
    EV(num_2, 28)
    EV(num_3, 29)
    EV(num_4, 30)
    EV(num_5, 31)
    EV(num_6, 32)
    EV(num_7, 33)
    EV(num_8, 34)
    EV(num_9, 35)
    EV(a, 36)
    EV(b, 37)
    EV(c, 38)
    EV(d, 39)
    EV(e, 40)
    EV(f, 41)
    EV(g, 42)
    EV(h, 43)
    EV(i, 44)
    EV(j, 45)
    EV(k, 46)
    EV(l, 47)
    EV(m, 48)
    EV(n, 49)
    EV(o, 50)
    EV(p, 51)
    EV(q, 52)
    EV(r, 53)
    EV(s, 54)
    EV(t, 55)
    EV(u, 56)
    EV(v, 57)
    EV(w, 58)
    EV(x, 59)
    EV(y, 60)
    EV(z, 61)
    EV(f1, 62)
    EV(f2, 63)
    EV(f3, 64)
    EV(f4, 65)
    EV(f5, 66)
    EV(f6, 67)
    EV(f7, 68)
    EV(f8, 69)
    EV(f9, 70)
    EV(f10, 71)
    EV(f11, 72)
    EV(f12, 73)
    EV(f13, 74)
    EV(f14, 75)
    EV(f15, 76)
    EV(f16, 77)
    EV(f17, 78)
    EV(f18, 79)
    EV(f19, 80)
    EV(f20, 81)
    EV(f21, 82)
    EV(f22, 83)
    EV(f23, 84)
    EV(f24, 85)
    EV(right, 86)
    EV(left, 87)
    EV(down, 88)
    EV(up, 89)
    EV(numlock, 90)
    EV(kp_divide, 91)
    EV(kp_multiply, 92)
    EV(kp_minus, 93)
    EV(kp_plus, 94)
    EV(kp_enter, 95)
    EV(kp_1, 96)
    EV(kp_2, 97)
    EV(kp_3, 98)
    EV(kp_4, 99)
    EV(kp_5, 100)
    EV(kp_6, 101)
    EV(kp_7, 102)
    EV(kp_8, 103)
    EV(kp_9, 104)
    EV(kp_0, 105)
    EV(kp_period, 106)
    EV(lctrl, 107)
    EV(lshift, 108)
    EV(lalt, 109)
    EV(lgui, 110)
    EV(rctrl, 111)
    EV(rshift, 112)
    EV(ralt, 113)
    EV(rgui, 114)
    EV(mode, 115)
    EV(volume_up, 116)
    EV(volume_down, 117)
    EV(audio_next, 118)
    EV(audio_prev, 119)
    EV(audio_stop, 120)
    EV(audio_play, 121)
    EV(audio_mute, 122)
    EV(left2, 123)
    EV(middle, 124)
    EV(right2, 125)
    EV(x1, 126)
    EV(x2, 127)
    EV(motion, 128)
    EV(motion_xneg, 129)
    EV(motion_ypos, 130)
    EV(motion_xpos, 131)
    EV(motion_yneg, 132)
    EV(wheel_xpos, 133)
    EV(wheel_xneg, 134)
    EV(wheel_ypos, 135)
    EV(wheel_yneg, 136)
    EV(leftstick, 137)
    EV(leftstick_x, 138)
    EV(leftstick_y, 139)
    EV(leftstick_xneg, 140)
    EV(leftstick_ypos, 141)
    EV(leftstick_xpos, 142)
    EV(leftstick_yneg, 143)
    EV(rightstick, 144)
    EV(rightstick_x, 145)
    EV(rightstick_xneg, 146)
    EV(rightstick_y, 147)
    EV(rightstick_ypos, 148)
    EV(rightstick_xpos, 149)
    EV(rightstick_yneg, 150)
    EV(lefttrigger, 151)
    EV(righttrigger, 152)
    EV(controller_a, 153)
    EV(controller_b, 154)
    EV(controller_x, 155)
    EV(controller_y, 156)
    EV(back, 157)
    EV(guide, 158)
    EV(start, 159)
    EV(leftstickpress, 160)
    EV(rightstickpress, 161)
    EV(leftshoulder, 162)
    EV(rightshoulder, 163)
    EV(dpad_up, 164)
    EV(dpad_down, 165)
    EV(dpad_left, 166)
    EV(dpad_right, 167)
    EV(touch_tap, 168)
    EV(touch_hold, 169)
    EV(touch_pinch_in, 170)
    EV(touch_pinch_out, 171)
    EV(touch_rotate, 172)
    EV(touch_flick, 173)
    EV(touch_press, 174)
    EV(item1, 175)
    EV(item2, 176)
    EV(item3, 177)
    EV(item4, 178)
    EV(item5, 179)
    EV(item6, 180)
    EV(item7, 181)
    EV(item8, 182)
    EV(item9, 183)
    EV(item10, 184)
    EV(item11, 185)
END_ENUM()

BEGIN_ENUM_NS(ecl, CharacterTaskType, ClientCharacterTaskType, uint32_t)
    EV(MoveTo, 0)
    EV(ControllerSelection, 1)
    EV(MoveController, 2)
    EV(MoveInDirection, 3)
    EV(Spell, 4)
    EV(Dialog, 5)
    EV(ItemUseRemotely, 6)
    EV(ItemUse, 7)
    EV(Drop, 8)
    EV(ItemInteractArea, 9)
    EV(ItemMove, 10)
    EV(ItemMoveDummy, 11)
    EV(ItemCombine, 12)
    EV(ItemCombineDummy, 13)
    EV(SheathUnsheath, 14)
    EV(Lockpick, 15)
    EV(DisarmTrap, 16)
    EV(Pickpocket, 17)
    EV(PickUp, 18)
    EV(Listen, 19)
    EV(Loot, 20)
    EV(DefaultTargetAction, 22)
    EV(ClimbTo, 23)
END_ENUM_NS()
