FS_NAME(Empty, "");
FS_NAME(UnknownTSHandle, "ls::TranslatedStringRepository::s_HandleUnknown");

// Stat attributes
FS(Level);
FS(Name);
FS(Using);
FS(AIFlags);
FS(ModifierList);
FS(OriginalModId);
FS(ComboProperties);
FS(ComboCategories);
FS(StatusType);
FS(SpellType);
FS(Boosts);

// Stat modifier types
FS(Character);
FS(Armor);
FS(Object);
FS(Weapon);
FS(SpellData);
FS(StatusData);
FS(PassiveData);
FS(InterruptData);

FS(SpellSet);
FS(EquipmentSet);
FS(TreasureTable);
FS(TreasureCategory);
FS(ItemGroup);
FS(NameGroup);

// Stat proxy functions
FS(Sync);
FS(SetPersistence);
FS(SetRawAttribute);
FS(CopyFrom);

// Entity proxy functions
FS(Vars);

// Osi proxy functions
FS(Get);
FS(Delete);
FS(Exists);
FS(Arities);
FS(InputArities);

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
FS(MemorizationRequirements);
FS(TranslatedString);

FS(AttributeFlags);
FS(SpellFlagList);
FS(WeaponFlags);
FS(ResistanceFlags);
FS(PassiveFlags);
FS(ProficiencyGroupFlags);
FS(CinematicArenaFlags);
FS(StatsFunctorContext);
FS(StatusEvent);
FS(StatusPropertyFlags);
FS(StatusGroupFlags);
FS(LineOfSightFlags);
FS(SpellCategoryFlags);
FS(InterruptContext);
FS(InterruptContextScope);
FS(InterruptDefaultValue);
FS(InterruptFlagsList);
FS(AuraFlags);
FS(AbilityFlags);

FS(Default);

// Event properties
FS(ReadOnly);
FS(CanPreventAction);

// Savegames
FS(ScriptExtenderSave);
FS(ExtenderVersion);
FS(LuaVariables);
FS(Mod);
FS(ModId);

// IO context types
FS(user);
FS(data);

// User variables
FS(Type);
FS(Value);
FS(Variable);
FS(UserVariables);
FS(EntityVariables);
FS(Entity);
FS(ModVariables);
FS(ModVariable);
FS(Module);

// Timers
FS(PersistentTimers);
FS(GameTimers);
FS(RealtimeTimers);
FS(Timer);
FS(Time);
FS(FrozenTime);
FS(Repeat);
FS(Paused);
FS(Handler);
FS(Args);

// Enums
FS(Label);
FS(EnumValue);
FS(EnumName);
FS(__Labels);
FS(__Value);
FS(__EnumName);

// Root template types
FS(LevelTemplate);
FS(character);
FS(item);
FS(scenery);
FS(surface);
FS(projectile);
FS(trigger);
FS(decal);
FS(prefab);
FS(light);
FS(constellation);
FS(CombinedLight);
FS(TileConstruction);

// Default IMGUI fonts
FS(Tiny);
FS(Small);
FS(Medium);
FS(Large);
FS(Big);

// Dialog node types
FS(Alias);
FS(Jump);
FS(TagQuestion);
FS(TagAnswer);
FS(TagGreeting);
FS(CinematicTagged);
FS(SelectSpeaker);
FS(FallibleQuestionResult);
FS(VisualState);
FS(NestedDialog);
FS(ActiveRoll);
FS(PassiveRoll);
FS(Trade);
FS(Pop);

// Genome type names
FS(Bool);
FS(Float);
FS(Int);
FS(Enum);
FS(String);
FS(Path);
FS(Vector3);
FS(Rotator3);
FS(Float3x3);
FS(Float4x4);
FS(FloatSet);
FS(IntSet);
FS(ShortNameSet);
FS(StringSet);
FS(FixedStringSet);
FS(AnimationSimpleName);
FS(AnimationSetID);
FS(AnimationComplexName);
FS(TimelineData);

// AllSpark component type names
FS(BaseComponent);
FS_NAME(Ribbon20, "Ribbon 2.0");
FS(Billboard);
FS(BoundingBox);
FS(BoundingSphere);
FS(CameraShake);
FS(Decal);
FS(Deflector);
FS(DragForce);
FS(GravityForce);
FS(Light);
FS(Model);
FS(MovingLevel);
FS(OrbitForce);
FS(OverlayMaterial);
FS(ParticleSystem);
FS(PostProcess);
FS(PreRoll);
FS(RadialForce);
FS(Sound);
FS(SpinForce);
FS(TurbulenceForce);
FS(VortexForce);
FS(WindForce);
FS(TLBase);
FS(TimelineActorPropertiesReflection);
FS(TLAdditiveAnimation);
FS(TLAnimation);
FS(TLAtmosphereAndLighting);
FS(TLAttachToEvent);
FS(TLAttitudeEvent);
FS(TLCameraDoF);
FS(TLCameraExposure);
FS(TLCameraFoV);
FS(TLCameraLookAt);
FS(TLEffectPhaseEvent);
FS(TLEmotionEvent);
FS(TLFloatRTPC);
FS(TLGenomeTextEvent);
FS(TLHandsIK);
FS(TLLayeredAnimation);
FS(TLLookAtEvent);
FS(TLMaterial);
FS(TLPhysics);
FS(TLPlayEffectEvent);
FS(TLPlayRate);
FS(TLShapeShift);
FS(TLShot);
FS(TLShotHoldPrevious);
FS(TLShotZoom);
FS(TLShowArmor);
FS(TLShowHUD);
FS(TLShowPeanuts);
FS(TLShowVisual);
FS(TLShowWeapon);
FS(TLSoundEvent);
FS(TLSplatter);
FS(TLSprings);
FS(TLSteppingFade);
FS(TLSwitchLocationEvent);
FS(TLSwitchStageEvent);
FS(TLTransform);
FS(TLVoice);
FS(TLEventKey);
FS(TLInterpolationKey);
FS(TLKeyBase);
FS(TimelineActorPropertiesReflectionKey);
FS(TLAtmosphereAndLightingChannel);
FS(TLCameraDoFChannel);
FS(TLCameraExposureChannel);
FS(TLMaterialKey);
FS(TLMaterialTextureKey);
FS(TLShowArmorChannel);
FS(TLSplatterChannel);
