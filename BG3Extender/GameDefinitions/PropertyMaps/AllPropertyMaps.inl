// Wrapper file for including all property map files

#include <GameDefinitions/PropertyMaps/CommonTypes.inl>
#include <GameDefinitions/PropertyMaps/Functors.inl>
#include <GameDefinitions/PropertyMaps/FunctorExecParams.inl>
#include <GameDefinitions/PropertyMaps/ServerObjects.inl>
#include <GameDefinitions/PropertyMaps/SurfaceActions.inl>
#include <GameDefinitions/PropertyMaps/Statuses.inl>
#include <GameDefinitions/PropertyMaps/Events.inl>

#if defined(ENABLE_UI)
#include <GameDefinitions/PropertyMaps/UI.inl>
#endif

#if defined(ENABLE_IMGUI)
#include <GameDefinitions/PropertyMaps/IMGUI.inl>
#endif

// WIP - not exported yet!
BEGIN_CLS(GlobalSwitches)
P_RO(UIScaling)
P_RO(ControllerMode)
P(NodeWaitTimeMultiplier)
P(AnimationBlueprintSpeedScale_M)
P(MouseSensitivity)
P(MouseScrollSensitivity)
P(ControllerSensitivity)
P(ControllerStickDeadZone)
P(ControllerStickPressDeadZone)
P(ControllerTriggerDeadZone)
P_RO(StoryLog)
P_RO(Story)
P_RO(StoryEvents)
P_RO(DisableStoryPatching)
P_RO(ForceStoryPatching)
P(RandomStartOrigin_M)
P_RO(Stats)
P_RO(Language)
P(ShowLocalizationMarkers)
P_RO(ForcePort)
P_RO(EnablePortmapping)
P_RO(GameVisibilityOnline)
P_RO(GameVisibilityLAN)
P_RO(GameVisibilityDirect)
P_RO(AutoListenEnabled)
P_RO(CameraShakeEnabled)
P_RO(TwitchEnableOverlay)
P(LongRestScriptWaitingTime)
P(LongRestDefaultTimeline)
P(UseLevelCache)
P(LoadAllEffectPools)
P(ScriptLog)
P(ShroudEnabled)
P(CheckRequirements)
P(UseEndTurnFallback)
P(ShowCharacterCreation)
P(ShowOriginIntroInCC)
P(DisableArmorSavingThrows)
P(AlwaysShowSplitter)
P(ResetTutorialsOnNewGame)
P(ControllerLayout)
P(NorthFacingMinimap)
P(HoldToEndTurn)
P(ExtraTooltipDescriptions)
P(SoundVolumeDynamicRange)
P(SoundOcclusionQuality)
P(MuteSoundWhenNotFocused)
P(CentreDialogue)
P(ForceMono)
P(SoundPartyLosingThreshold)
P(SoundPartyWinningThreshold)
P(GameCameraRotation)
P(GameCameraSpeedMultiplier)
P(GameCameraRotationLocked)
P(GameCameraEnableCloseUpDialog)
P(GameCameraEnableDynamicCombatCamera)
P(CombatTimelines)
P(GameCameraEnableAttackCameraOtherPlayers)
P(GameCameraEnableFlyFollow)
P(GameCameraAutoRotateEnabled)
P(GameCameraAutoRotateSpeed)
P(NrOfQuickSaves)
P(NrOfAutoSaves)
P(MaxNrOfQuickSaves)
P(MaxNrOfAutoSaves)
P(CanAutoSave)
P(CanCrossSave)
P(CanSaveDuringTimeline)
P(CanLoadDuringTimeline)
P(FirstTimeWithCrossSave)
P(HasSeenEAMsg)
P(ForceSplitscreen)
P(MaxAmountDialogsInLog)
P(ControllerCharacterWalkThreshold)
P(ControllerCharacterRunThreshold)
P(ControllerRumbleStrength)
P(EnableVoiceLogging)
P(EnableLongPressAndHold)
P(MouseLock)
P(DisableEdgePanning)
P(EscClosesAllUI)
P(ShowSubtitles)
P(DisplaySpeaker)
P(ShowTextBackground)
P(TextBackgroundOpacity)
P(DialogueFontSizeModifier)
P(OverheadFontSizeModifier)
P(UseRadialContextMenu)
P(LogSaveLoadErrors)
END_CLS()

BEGIN_CLS(stats::TreasureTable)
P_RO(Name)
P(MinLevel)
P(MaxLevel)
P(IgnoreLevelDiff)
P(UseTreasureGroupContainers)
P(CanMerge)
P(SubTables)
END_CLS()

BEGIN_CLS(stats::TreasureSubTable)
P(Categories)
P(CategoryFrequencies)
P(TotalFrequency)
P(DropCounts)
P(Amounts)
P(TotalCount)
P(StartLevel)
P(EndLevel)
END_CLS()

BEGIN_CLS(stats::TreasureSubTable::Category)
P(Index)
P(Frequency)
P(Frequencies)
P(Frequencies2)
P(IsTreasureTable)
P(IsTreasureTable2)
END_CLS()

BEGIN_CLS(stats::TreasureSubTable::DropCount)
P(Chance)
P(Amount)
END_CLS()

BEGIN_CLS(stats::TreasureCategory)
P(Category)
P(Items)
END_CLS()

BEGIN_CLS(stats::TreasureCategory::Item)
P(Name)
P(Priority)
P(MinAmount)
P(MaxAmount)
P(ActPart)
P(Unique)
P(MinLevel)
P(MaxLevel)
END_CLS()

BEGIN_CLS(stats::RPGEnumeration)
P(Name)
P(Values)
END_CLS()

BEGIN_CLS(stats::Modifier)
P(EnumerationIndex)
P(LevelMapIndex)
P(Name)
END_CLS()

BEGIN_CLS(stats::ModifierList)
P(Name)
P(Attributes)
END_CLS()

BEGIN_CLS(stats::CNamedElementManager<stats::RPGEnumeration>)
P(Primitives)
P(NameHashMap)
END_CLS()

BEGIN_CLS(stats::CNamedElementManager<stats::ModifierList>)
P(Primitives)
P(NameHashMap)
END_CLS()

BEGIN_CLS(stats::CNamedElementManager<stats::Modifier>)
P(Primitives)
P(NameHashMap)
END_CLS()

BEGIN_CLS(stats::CNamedElementManager<stats::TreasureTable>)
P(Primitives)
P(NameHashMap)
END_CLS()

BEGIN_CLS(stats::RPGStats)
P(ModifierValueLists)
P(ModifierLists)
P(TreasureTables)
P(ExtraData)
END_CLS()

#include <GameDefinitions/PropertyMaps/Generated.inl>
