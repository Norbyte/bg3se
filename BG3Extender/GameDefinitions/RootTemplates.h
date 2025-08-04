#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_SE()

struct TemplateTagContainer;

struct GameObjectTemplate
{
    virtual ~GameObjectTemplate() = 0;
    virtual void* GetName(void*) const = 0;
    virtual void* DebugDump(void*) = 0;
    virtual FixedString* GetType() const = 0;
    virtual FixedString* GetRealType() const = 0;
    virtual void SetTransform(Transform const&, void*) = 0;
    virtual bool IsValidV40026() = 0;
    virtual GameObjectTemplate* RealClone() = 0;
    virtual bool HasRotation() const = 0;
    virtual void UpdateTransformFromLevel() = 0;
    virtual void Visit(ObjectVisitor&) = 0;
    virtual void Noop() = 0;
    virtual bool IsValid() const = 0;
    virtual Guid* GetHLOD() = 0;
    virtual GameObjectTemplate* Clone() = 0;
    virtual GameObjectTemplate* PartialClone(GameObjectTemplate* other) = 0;
    virtual void OverrideFromParent(GameObjectTemplate* other, bool force) = 0;
    virtual void UpdateOverrideFlagsFromParent(GameObjectTemplate* other) = 0;
    virtual bool PostLoad() = 0;
    virtual bool HasAIBounds() const = 0;
    virtual void* GetAIBounds1(void*, uint16_t) const = 0;
    virtual void* GetAIBounds2(void*, uint16_t) const = 0;
    virtual double GetAIBounds3(void*, uint16_t) const = 0;
    virtual void* GetAIBounds4(void*, uint16_t) const = 0;
    virtual double GetAIBounds5(void*, uint16_t) const = 0;
    virtual void UpdateFadeFromParent(GameObjectTemplate* parent, bool) = 0;
    virtual void UpdateHierarchyOnlyFadeFromParent(GameObjectTemplate* parent, bool) = 0;
    virtual void UpdateSeeThroughFromParent(GameObjectTemplate* parent, bool) = 0;
    virtual void UpdateCollideWithCameraFromParent(GameObjectTemplate* parent, bool) = 0;
    virtual void UnknownFunc(bool) = 0;
    virtual uint16_t* GetVisualFlags(uint16_t&) const = 0;
    virtual OverrideableProperty<bool>* IsPlatformOwner() const = 0;
    virtual void* GetOnUsePeaceActions() const = 0;
    virtual OverrideableProperty<bool>* IsTrap() const = 0;

    FixedString GetTemplateType() const;

    //# P_GETTER(TemplateType, GetTemplateType)

    TemplateTagContainer* Tags;
    [[bg3::readonly]] FixedString Id;
    [[bg3::readonly]] FixedString TemplateName;
    [[bg3::readonly]] FixedString ParentTemplateId;
    [[bg3::readonly]] uint32_t TemplateHandle;
    [[bg3::readonly]] STDString Name;
    OverrideableProperty<uint32_t> GroupID;
    FixedString LevelName;
    [[bg3::hidden]] uint32_t _Pad;
    OverrideableProperty<Transform> Transform;
    OverrideableProperty<FixedString> VisualTemplate;
    OverrideableProperty<FixedString> PhysicsTemplate;
    OverrideableProperty<bool> CastShadow;
    OverrideableProperty<bool> ReceiveDecal;
    OverrideableProperty<bool> AllowReceiveDecalWhenAnimated;
    OverrideableProperty<bool> IsReflecting;
    OverrideableProperty<bool> IsShadowProxy;
    uint8_t GlobalDeletedFlag;
    OverrideableProperty<uint8_t> RenderChannel;
    OverrideableProperty<uint8_t> ParentTemplateFlags;
    STDString FileName;
};


struct TemplateTagContainer
{
    HashSet<Guid> DirectTags;
    HashSet<Guid> Tags;
    [[bg3::hidden]] GameObjectTemplate* Template;
    [[bg3::hidden]] void* GetTagsCallback;
};


struct BoundData
{
    [[bg3::legacy(Type)]] float Height;
    float ActualHeight;
    [[bg3::legacy(Radius2)]] float Size;
    glm::vec3 Min;
    glm::vec3 Max;
    float Radius;
    [[bg3::legacy(field_28)]] AIBoundType BoundType;
    [[bg3::legacy(AiType)]] AIShapeType Shape;
    uint8_t field_2A;
};


struct EoCGameObjectTemplate : public GameObjectTemplate
{
    OverrideableProperty<Array<BoundData>> AIBounds;
    OverrideableProperty<TranslatedString> DisplayName;
    OverrideableProperty<bool> Fadeable;
    OverrideableProperty<bool> SeeThrough;
    OverrideableProperty<bool> CollideWithCamera;
    OverrideableProperty<bool> HierarchyOnlyFade;
    OverrideableProperty<FixedString> FadeGroup;
    Array<FixedString> FadeChildren;
};


struct SceneryTemplate : public EoCGameObjectTemplate
{
    OverrideableProperty<bool> CoverAmount;
    OverrideableProperty<bool> CanClimbOn;
    OverrideableProperty<bool> CanShootThrough;
    OverrideableProperty<bool> WalkThrough;
    OverrideableProperty<bool> WalkOn;
    OverrideableProperty<bool> Wadable;
    OverrideableProperty<bool> CanClickThrough;
    OverrideableProperty<bool> IsPointerBlocker;
    OverrideableProperty<bool> IsBlocker;
    OverrideableProperty<bool> IsDecorative;
    OverrideableProperty<bool> AllowCameraMovement;
    OverrideableProperty<bool> CanShineThrough;
    OverrideableProperty<bool> BlockAoEDamage;
    OverrideableProperty<uint8_t> ShootThroughType;
    OverrideableProperty<uint8_t> WadableSurfaceType;
    bool ReferencedInTimeline;
    OverrideableProperty<FixedString> LoopSound;
    OverrideableProperty<FixedString> SoundInitEvent;
    OverrideableProperty<Guid> HLOD;
    OverrideableProperty<FixedString> ShadowPhysicsProxy;
    OverrideableProperty<int16_t> SoundAttenuation;
}; 


struct CombatComponentTemplate : public ProtectedGameObject<CombatComponentTemplate>
{
    [[bg3::hidden]]
    void* VMT;
    OverrideableProperty<FixedString> Archetype;
    OverrideableProperty<FixedString> SwarmGroup;
    OverrideableProperty<Guid> Faction;
    OverrideableProperty<bool> CanFight;
    OverrideableProperty<bool> CanJoinCombat;
    OverrideableProperty<FixedString> CombatGroupID;
    OverrideableProperty<bool> IsBoss;
    OverrideableProperty<bool> StayInAiHints;
    OverrideableProperty<Guid> AiHint;
    OverrideableProperty<bool> IsInspector;
    uint8_t Unknown;
    uint8_t Unknown2;
    OverrideableProperty<float> StartCombatRange;
    OverrideableProperty<FixedString> AiUseCombatHelper;
    OverrideableProperty<Guid> ProxyOwner;
    OverrideableProperty<FixedString> ProxyAttachment;
};


struct AiTargetConditions
{
    int32_t MinimumHealthPercentage;
    int32_t MaximumHealthPercentage;
    Array<Guid> Tags;
};

struct AiActionConditions
{
    float ScoreModifier;
    int32_t StartRound;
    int32_t FallbackStartRound;
    int32_t MinimumImpact;
    bool OnlyCastOnSelf;
    uint16_t AIFlags;
    AiTargetConditions SourceConditions;
    AiTargetConditions TargetConditions;
    Array<Guid> ExcludeInNPCLoadout;
    Array<Guid> OnlyInNPCLoadout;
};

struct InventoryItemData
{
    AiActionConditions Conditions;
    STDString ItemName;
    STDString LevelName;
    FixedString UUID;
    FixedString TemplateID;
    int Amount;
    IsTradableType IsTradable;
    uint8_t Type;
    bool CanBePickpocketed;
    bool IsDroppedOnDeath;
};

struct CharacterSpellData
{
    FixedString Spell;
    AbilityId SpellCastingAbility;
    SpellLearningStrategy LearningStrategy;
    AiActionConditions Conditions;
};

struct CharacterTemplate : public EoCGameObjectTemplate
{
    CombatComponentTemplate CombatComponent;
    [[bg3::hidden]]
    OverrideableProperty<Array<void*>> ScriptConfigGlobalParameters;
    OverrideableProperty<FixedString> Icon;
    OverrideableProperty<FixedString> Stats;
    OverrideableProperty<FixedString> SpellSet;
    OverrideableProperty<FixedString> Equipment;
    OverrideableProperty<Array<Guid>> EquipmentTypes;
    OverrideableProperty<Array<FixedString>> Treasures;
    OverrideableProperty<Array<FixedString>> TradeTreasures;
    OverrideableProperty<FixedString> BloodType;
    OverrideableProperty<FixedString> CriticalHitType;
    OverrideableProperty<FixedString> DefaultDialog;
    HashSet<Guid>* SpeakerGroupList;
    OverrideableProperty<STDString> GeneratePortrait;
    OverrideableProperty<float> LadderAttachOffset;
    OverrideableProperty<float> LadderLoopSpeed;
    OverrideableProperty<bool> CanShootThrough;
    OverrideableProperty<uint8_t> ShootThroughType;
    OverrideableProperty<bool> WalkThrough;
    OverrideableProperty<bool> CanClimbLadders;
    OverrideableProperty<bool> CanConsumeItems;
    OverrideableProperty<bool> IsPlayer;
    OverrideableProperty<bool> SpotSneakers;
    OverrideableProperty<bool> CanOpenDoors;
    OverrideableProperty<bool> CanWalkThroughDoors;
    OverrideableProperty<bool> AvoidTraps;
    OverrideableProperty<bool> InfluenceTreasureLevel;

    OverrideableProperty<bool> UseOcclusion;
    OverrideableProperty<FixedString> SoundInitEvent;
    OverrideableProperty<FixedString> SoundMovementStartEvent;
    OverrideableProperty<FixedString> SoundMovementStopEvent;
    OverrideableProperty<int8_t> SoundObjectIndex;
    OverrideableProperty<int16_t> SoundAttenuation;
    OverrideableProperty<bool> UseSoundClustering;

    OverrideableProperty<Array<Guid>> ExcludeInDifficulty;
    OverrideableProperty<Array<Guid>> OnlyInDifficulty;
    OverrideableProperty<bool> JumpUpLadders;
    OverrideableProperty<bool> AnubisNonPersistent;
    OverrideableProperty<uint8_t> BloodSurfaceType;
    OverrideableProperty<bool> ForceLifetimeDeath;
    OverrideableProperty<Guid> EquipmentRace;
    OverrideableProperty<Array<IActionData*>> OnDeathActions;
    OverrideableProperty<float> DeathRaycastMinLength;
    OverrideableProperty<float> DeathRaycastMaxLength;
    OverrideableProperty<float> DeathRaycastHeight;
    OverrideableProperty<float> DeathRaycastVerticalLength;
    OverrideableProperty<Guid> DeathEffect;
    OverrideableProperty<FixedString> ExplodedResourceID;
    OverrideableProperty<FixedString> ExplosionFX;
    OverrideableProperty<FixedString> DisintegratedResourceID;
    OverrideableProperty<FixedString> DisintegrateFX;
    OverrideableProperty<FixedString> AnubisConfigName;
    OverrideableProperty<Array<CharacterSpellData>> SkillList;
    OverrideableProperty<Array<InventoryItemData>> ItemList;
    OverrideableProperty<Array<FixedString>> StatusList;
    OverrideableProperty<FixedString> TrophyID;

    OverrideableProperty<FixedString> FoleyShortResourceID;
    OverrideableProperty<FixedString> FoleyMediumResourceID;
    OverrideableProperty<FixedString> FoleyLongResourceID;

    OverrideableProperty<FixedString> VocalNoneResourceID;
    OverrideableProperty<FixedString> VocalAttackResourceID;
    OverrideableProperty<FixedString> VocalDeathResourceID;
    OverrideableProperty<FixedString> VocalDodgeResourceID;
    OverrideableProperty<FixedString> VocalPainResourceID;
    OverrideableProperty<FixedString> VocalAnticipationResourceID;
    OverrideableProperty<FixedString> VocalFallResourceID;
    OverrideableProperty<FixedString> VocalShoutResourceID;
    OverrideableProperty<FixedString> VocalSpawnResourceID;
    OverrideableProperty<FixedString> VocalAlertResourceID;
    OverrideableProperty<FixedString> VocalAngryResourceID;
    OverrideableProperty<FixedString> VocalAwakeResourceID;
    OverrideableProperty<FixedString> VocalBoredResourceID;
    OverrideableProperty<FixedString> VocalVictoryResourceID;
    OverrideableProperty<FixedString> VocalRebornResourceID;
    OverrideableProperty<FixedString> VocalRelaxedResourceID;
    OverrideableProperty<FixedString> VocalSnoreResourceID;
    OverrideableProperty<FixedString> VocalExhaustedResourceID;
    OverrideableProperty<FixedString> VocalEffortsResourceID;
    OverrideableProperty<FixedString> VocalInitiativeResourceID;
    OverrideableProperty<FixedString> VocalWeakResourceID;
    OverrideableProperty<FixedString> VocalLaughterResourceID;
    OverrideableProperty<FixedString> VocalRecoverResourceID;
    OverrideableProperty<FixedString> VocalBuffResourceID;
    OverrideableProperty<FixedString> VocalIdle1ResourceID;
    OverrideableProperty<FixedString> VocalIdle2ResourceID;
    OverrideableProperty<FixedString> VocalIdle3ResourceID;
    OverrideableProperty<FixedString> VocalIdleCombat1ResourceID;
    OverrideableProperty<FixedString> VocalIdleCombat2ResourceID;
    OverrideableProperty<FixedString> VocalIdleCombat3ResourceID;
    OverrideableProperty<FixedString> VocalGaspResourceID;
    OverrideableProperty<FixedString> VocalLaughterManiacalResourceID;
    OverrideableProperty<FixedString> VocalCinematicSuffix;

    OverrideableProperty<FixedString> CharacterVisualResourceID;
    OverrideableProperty<uint8_t> CoverAmount;
    OverrideableProperty<bool> CanBeTeleported;
    OverrideableProperty<int32_t> LevelOverride;
    OverrideableProperty<FixedString> ActivationGroupId;
    OverrideableProperty<uint8_t> AliveInventoryType;
    OverrideableProperty<uint8_t> InventoryType;
    OverrideableProperty<LegacyRefMap<FixedString, FixedString>> PickingPhysicsTemplates;
    OverrideableProperty<FixedString> SoftBodyCollisionTemplate;
    OverrideableProperty<FixedString> RagdollTemplate;
    [[bg3::hidden]]
    OverrideableProperty<Array<void*>> FootStepInfos;
    OverrideableProperty<uint8_t> DefaultState;
    OverrideableProperty<bool> IsLootable;
    OverrideableProperty<bool> IsEquipmentLootable;
    OverrideableProperty<bool> CanBePickedUp;
    OverrideableProperty<bool> CanBePickpocketed;
    OverrideableProperty<IsTradableType> IsTradable;
    OverrideableProperty<bool> IsDroppedOnDeath;
    OverrideableProperty<uint8_t> LightChannel;
    OverrideableProperty<Guid> Race;
    OverrideableProperty<TranslatedString> Title;
    OverrideableProperty<FixedString> AnimationSetResourceID;
    OverrideableProperty<bool> HasPlayerApprovalRating;
    OverrideableProperty<bool> DisableEquipping;
    OverrideableProperty<bool> IsSimpleCharacter;
    OverrideableProperty<float> WorldClimbingSpeed;
    OverrideableProperty<bool> UseStandAtDestination;
    OverrideableProperty<bool> IsMovementEnabled;
    OverrideableProperty<float> MovementAcceleration;
    OverrideableProperty<float> MovementSpeedStroll;
    OverrideableProperty<float> MovementSpeedWalk;
    OverrideableProperty<float> MovementSpeedRun;
    OverrideableProperty<float> MovementSpeedSprint;
    OverrideableProperty<float> MovementSpeedDash;
    OverrideableProperty<float> MaxDashDistance;
    OverrideableProperty<float> MovementStepUpHeight;
    OverrideableProperty<FixedString> MovementTiltToRemap;
    OverrideableProperty<float> TurningNodeAngle;
    OverrideableProperty<float> TurningNodeOffset;
    OverrideableProperty<bool> IsSteeringEnabled;
    OverrideableProperty<FixedString> SteeringSpeed_CastingCurve;
    OverrideableProperty<FixedString> SteeringSpeed_MovingCurve;
    OverrideableProperty<FixedString> SteeringSpeedCurveWithoutTransitions;
    OverrideableProperty<float> SteeringSpeedFallback;
    OverrideableProperty<bool> IsWorldClimbingEnabled;
    OverrideableProperty<FixedString> WorldClimbingBlendspace_DownA;
    OverrideableProperty<FixedString> WorldClimbingBlendspace_DownB;
    OverrideableProperty<float> WorldClimbingBlendspace_DownBHeight;
    OverrideableProperty<FixedString> WorldClimbingBlendspace_UpA;
    OverrideableProperty<FixedString> WorldClimbingBlendspace_UpB;
    OverrideableProperty<float> WorldClimbingBlendspace_UpBHeight;
    OverrideableProperty<float> WorldClimbingHeight;
    OverrideableProperty<float> WorldClimbingRadius;
    OverrideableProperty<float> ProbeLookAtOffset;
    OverrideableProperty<float> ProbeSpineAOffset;
    OverrideableProperty<float> ProbeSpineBOffset;
    OverrideableProperty<float> ProbeTiltToOffset;
    OverrideableProperty<FixedString> LadderBlendspace_Attach_Down;
    OverrideableProperty<FixedString> LadderBlendspace_Attach_Up;
    OverrideableProperty<FixedString> LadderBlendspace_Detach_Down;
    OverrideableProperty<FixedString> LadderBlendspace_Detach_Up;
    OverrideableProperty<float> VFXScale;
};


struct EquipmentData : public ProtectedGameObject<EquipmentData>
{
    HashMap<Guid, FixedString> ShortHair;
    HashMap<Guid, FixedString> LongHair;
    HashMap<Guid, FixedString> WavyShortHair;
    HashMap<Guid, FixedString> WavyLongHair;
    HashMap<Guid, FixedString> CurlyShortHair;
    HashMap<Guid, FixedString> CurlyLongHair;
    HashMap<Guid, FixedString> DreadShortHair;
    HashMap<Guid, FixedString> DreadLongHair;
    HashMap<Guid, FixedString> AfroShortHair;
    HashMap<Guid, FixedString> AfroLongHair;
    HashMap<Guid, Array<FixedString>> Visuals;
    HashMap<Guid, Guid> ParentRace;
    HashSet<Guid> SyncWithParent;
    resource::VisualSet* VisualSet;
    [[bg3::hidden]] void* Slot_VMT;
    Array<FixedString> Slot;
    [[bg3::hidden]] void* Slot_junk;
};


struct ItemTemplate : public SceneryTemplate
{
    CombatComponentTemplate CombatComponent;
    OverrideableProperty<Array<FixedString>> InventoryList;
    HashSet<Guid>* SpeakerGroups;
    OverrideableProperty<FixedString> Icon;
    OverrideableProperty<bool> CanBePickedUp;
    OverrideableProperty<bool> CanBePickpocketed;
    OverrideableProperty<bool> IsDroppedOnDeath;
    OverrideableProperty<bool> CanBeMoved;
    OverrideableProperty<bool> Destroyed;
    OverrideableProperty<bool> IsInteractionDisabled;
    OverrideableProperty<bool> StoryItem;
    OverrideableProperty<bool> DestroyWithStack;
    OverrideableProperty<bool> IsPlatformOwner;
    OverrideableProperty<bool> IsKey;
    OverrideableProperty<bool> IsTrap;
    OverrideableProperty<bool> IsSurfaceBlocker;
    OverrideableProperty<bool> IsSurfaceCloudBlocker;
    OverrideableProperty<bool> TreasureOnDestroy;
    OverrideableProperty<bool> UsePartyLevelForTreasureLevel;
    OverrideableProperty<bool> Unimportant;
    OverrideableProperty<bool> Hostile;
    OverrideableProperty<bool> UseOnDistance;
    OverrideableProperty<bool> UseRemotely;
    OverrideableProperty<bool> PhysicsFollowAnimation;
    OverrideableProperty<bool> CanBeImprovisedWeapon;
    OverrideableProperty<bool> ForceAffectedByAura;
    OverrideableProperty<bool> IsBlueprintDisabledByDefault;
    OverrideableProperty<Array<Guid>> ExcludeInDifficulty;
    OverrideableProperty<Array<Guid>> OnlyInDifficulty;
    OverrideableProperty<TranslatedString> UnknownDisplayName;
    OverrideableProperty<bool> ShowAttachedSpellDescriptions;
    uint8_t GravityType;
    uint8_t FreezeGravity;
    OverrideableProperty<uint32_t> Tooltip;
    OverrideableProperty<FixedString> Stats;
    OverrideableProperty<Array<IActionData*>> OnUsePeaceActions;
    OverrideableProperty<Array<IActionData*>> OnDestroyActions;
    OverrideableProperty<TranslatedString> OnUseDescription;
    OverrideableProperty<bool> IsPortal;
    OverrideableProperty<bool> AttackableWhenClickThrough;
    [[bg3::hidden]] OverrideableProperty<Array<void*>> Scripts;
    [[bg3::hidden]] OverrideableProperty<HashMap<FixedString, void*>> ScriptOverrides;
    OverrideableProperty<FixedString> AnubisConfigName;
    [[bg3::hidden]] OverrideableProperty<Array<void*>> ScriptConfigGlobalParameters;
    OverrideableProperty<FixedString> ConstellationConfigName;
    [[bg3::hidden]] OverrideableProperty<Array<void*>> ConstellationConfigGlobalParameters;
    OverrideableProperty<Array<InventoryItemData>> ItemList;
    OverrideableProperty<Array<FixedString>> StatusList;
    OverrideableProperty<FixedString> DefaultState;
    OverrideableProperty<FixedString> Owner;
    OverrideableProperty<FixedString> Key;
    OverrideableProperty<FixedString> BloodType;
    OverrideableProperty<FixedString> CriticalHitType;
    OverrideableProperty<FixedString> MapMarkerStyle;
    OverrideableProperty<Guid> LockDifficultyClassID;
    OverrideableProperty<Guid> DisarmDifficultyClassID;
    OverrideableProperty<int32_t> Amount;
    OverrideableProperty<int32_t> MaxStackAmount;
    OverrideableProperty<int32_t> TreasureLevel;
    OverrideableProperty<EquipmentData*> Equipment;
    OverrideableProperty<FixedString> DropSound;
    OverrideableProperty<FixedString> PickupSound;
    OverrideableProperty<FixedString> UseSound;
    OverrideableProperty<FixedString> EquipSound;
    OverrideableProperty<FixedString> UnequipSound;
    OverrideableProperty<FixedString> InventoryMoveSound;
    OverrideableProperty<FixedString> ImpactSound;
    OverrideableProperty<FixedString> PhysicsCollisionSound;
    OverrideableProperty<bool> UseOcclusion;
    OverrideableProperty<uint8_t> BloodSurfaceType;
    OverrideableProperty<uint8_t> BookType;
    OverrideableProperty<uint8_t> InventoryType;
    OverrideableProperty<TranslatedString> DisplayNameAlchemy;
    OverrideableProperty<TranslatedString> Description;
    OverrideableProperty<TranslatedString> TechnicalDescription;
    OverrideableProperty<TranslatedString> ShortDescription;
    OverrideableProperty<STDString> TechnicalDescriptionParams;
    OverrideableProperty<STDString> ShortDescriptionParams;
    OverrideableProperty<TranslatedString> UnknownDescription;
    OverrideableProperty<FixedString> PermanentWarnings;
    OverrideableProperty<bool> ContainerAutoAddOnPickup;
    OverrideableProperty<STDString> ContainerContentFilterCondition;
    HashSet<Guid>* InteractionFilterList;
    OverrideableProperty<uint8_t> InteractionFilterType;
    OverrideableProperty<uint8_t> InteractionFilterRequirement;
    OverrideableProperty<FixedString> ActivationGroupId;
    OverrideableProperty<int32_t> LevelOverride;
    OverrideableProperty<bool> IsSourceContainer;
    OverrideableProperty<bool> IsPublicDomain;
    OverrideableProperty<bool> IgnoreGenerics;
    OverrideableProperty<bool> AllowSummonGenericUse;
    OverrideableProperty<bool> IsPortalProhibitedToPlayers;
    OverrideableProperty<uint8_t> LightChannel;
    OverrideableProperty<Guid> EquipmentTypeID;
    OverrideableProperty<uint32_t> CinematicArenaFlags;
    OverrideableProperty<Guid> TimelineCameraRigOverride;
    OverrideableProperty<Guid> MaterialPreset;
    OverrideableProperty<Guid> ColorPreset;
    OverrideableProperty<glm::vec3> ExamineRotation;

    // Lua helpers
    //# P_FUN(AddUseAction, ItemTemplate::AddUseAction)
    //# P_FUN(RemoveUseAction, ItemTemplate::RemoveUseAction)
    IActionData* AddUseAction(ActionDataType type);
    void RemoveUseAction(int32_t index);
};


struct ProjectileTemplate : public EoCGameObjectTemplate
{
    OverrideableProperty<float> LifeTime;
    OverrideableProperty<FixedString> CastBone;
    OverrideableProperty<FixedString> ImpactFX;
    OverrideableProperty<STDString> GroundImpactFX;
    OverrideableProperty<FixedString> TrailFX;
    OverrideableProperty<bool> DestroyTrailFXOnImpact;
    OverrideableProperty<bool> NeedsImpactSFX;
    OverrideableProperty<FixedString> BeamFX;
    OverrideableProperty<FixedString> PreviewPathMaterial;
    OverrideableProperty<FixedString> PreviewPathImpactFX;
    OverrideableProperty<FixedString> ImpactSoundResourceID;
    OverrideableProperty<float> PreviewPathRadius;
    OverrideableProperty<bool> RotateImpact;
    OverrideableProperty<bool> IgnoreRoof;
    OverrideableProperty<bool> DetachBeam;
    OverrideableProperty<uint8_t> ProjectilePath;
    OverrideableProperty<float> DistanceMin_Bezier3;
    OverrideableProperty<float> DistanceMax_Bezier3;
    OverrideableProperty<glm::vec2> OffsetMin_Bezier3;
    OverrideableProperty<glm::vec2> OffsetMax_Bezier3;
    OverrideableProperty<float> ShiftMin_Bezier3;
    OverrideableProperty<float> ShiftMax_Bezier3;
    OverrideableProperty<float> DistanceMin_Bezier4;
    OverrideableProperty<float> DistanceMax_Bezier4;
    OverrideableProperty<glm::vec2> OffsetAMin_Bezier3;
    OverrideableProperty<glm::vec2> OffsetAMax_Bezier3;
    OverrideableProperty<glm::vec2> OffsetBMin_Bezier3;
    OverrideableProperty<glm::vec2> OffsetBMax_Bezier3;
    OverrideableProperty<float> ShiftAMin_Bezier3;
    OverrideableProperty<float> ShiftAMax_Bezier3;
    OverrideableProperty<float> ShiftBMin_Bezier3;
    OverrideableProperty<float> ShiftBMax_Bezier3;
    OverrideableProperty<uint8_t> RotateMode;
    OverrideableProperty<uint8_t> VelocityMode;
    OverrideableProperty<float> InitialSpeed;
    OverrideableProperty<float> Acceleration;
    OverrideableProperty<FixedString> CurveResourceId;
};

struct SurfaceVisualData
{
    FixedString Visual;
    glm::vec2 Height;
    glm::ivec2 Rotation;
    glm::vec2 Scale;
    float GridSize;
    int SpawnCell;
    int RandomPlacement;
    int SurfaceNeeded;
    int SurfaceRadiusMax;
};

struct SurfaceStatusData
{
    FixedString StatusId;
    uint8_t ApplyTypes;
    float Chance;
    float Duration;
    bool Remove;
    bool ApplyToCharacters;
    bool ApplyToItems;
    bool KeepAlive;
    bool DontTickWhileOnSurface;
    bool VanishOnApply;
    bool Force;
    bool AffectedByRoll;
    bool OnlyOncePerTurn;
};


struct SurfaceTemplate : public GameObjectTemplate
{
    SurfaceType SurfaceType;
    FixedString SurfaceName;
    uint32_t field_158;
    OverrideableProperty<TranslatedString> DisplayName;
    OverrideableProperty<TranslatedString> Description;
    OverrideableProperty<Guid> CursorMessage;
    OverrideableProperty<FixedString> Icon;
    OverrideableProperty<FixedString> DecalMaterial;
    OverrideableProperty<uint8_t> MaterialType;
    OverrideableProperty<uint8_t> SurfaceCategory;
    OverrideableProperty<bool> CanEnterCombat;
    OverrideableProperty<bool> AlwaysUseDefaultLifeTime;
    OverrideableProperty<float> DefaultLifeTime;
    OverrideableProperty<float> SurfaceGrowTimer;
    OverrideableProperty<float> FadeInSpeed;
    OverrideableProperty<float> FadeOutSpeed;
    OverrideableProperty<float> FallDamageMultiplier;
    OverrideableProperty<int32_t> Seed;
    OverrideableProperty<float> NormalBlendingFactor;
    Array<SurfaceVisualData> InstanceVisual;
    Array<SurfaceVisualData> IntroFX;
    Array<SurfaceVisualData> FX;
    Array<SurfaceStatusData> Statuses;
    OverrideableProperty<FixedString> Summon;
    OverrideableProperty<bool> RemoveDestroyedItems;
    OverrideableProperty<bool> CanSeeThrough;
    OverrideableProperty<bool> CanShootThrough;
    OverrideableProperty<STDString> RollConditions;
    OverrideableProperty<uint8_t> ObscuredStateOverride;
    OverrideableProperty<float> OnEnterDistanceOverride;
    OverrideableProperty<float> OnMoveDistanceOverride;
    OverrideableProperty<bool> AiPathVisible;
    OverrideableProperty<glm::vec3> AiPathColor;
    OverrideableProperty<FixedString> AiPathIconFX;
    OverrideableProperty<bool> EnableLightSpawning;
    OverrideableProperty<FixedString> SpawnLightTemplateID;
    OverrideableProperty<glm::vec2> SpawnLightRandomVerticalRangeOffset;
};


struct TimelineReferencedTemplate
{
    bool ReferencedInTimeline;
    HashSet<Guid> Timelines;
};


struct LightTemplate : public GameObjectTemplate
{
    OverrideableProperty<glm::vec3> Color;
    OverrideableProperty<glm::vec2> Angle;
    OverrideableProperty<float> Kelvin;
    OverrideableProperty<float> Radius;
    OverrideableProperty<float> Intensity;
    OverrideableProperty<float> ScatteringScale;
    OverrideableProperty<float> Gain;
    OverrideableProperty<float> Speed;
    OverrideableProperty<float> Amount;
    OverrideableProperty<float> MovementSpeed;
    OverrideableProperty<float> MovementAmount;
    OverrideableProperty<bool> UseTemperature;
    OverrideableProperty<bool> Enabled;
    OverrideableProperty<bool> IsFlickering;
    OverrideableProperty<bool> IsMoving;
    OverrideableProperty<bool> Shadow;
    OverrideableProperty<bool> VolumetricShadow;
    OverrideableProperty<bool> PreExpose;
    OverrideableProperty<bool> FlatFalloff;
    OverrideableProperty<uint8_t> LightChannelFlag;
    OverrideableProperty<uint8_t> LightType;
    OverrideableProperty<FixedString> LightCookieTexture;
    OverrideableProperty<glm::vec3> DirectionLightDimensions;
    OverrideableProperty<uint8_t> DirectionLightAttenuationFunction;
    OverrideableProperty<float> DirectionLightAttenuationStart;
    OverrideableProperty<float> DirectionLightAttenuationEnd;
    OverrideableProperty<float> DirectionLightAttenuationSide;
    // Editor only?
    // TimelineReferencedTemplate ReferencedInTimelines;
};


struct CombinedLightTemplate : public LightTemplate
{
    OverrideableProperty<bool> GameplayIsActive;
    OverrideableProperty<float> GameplayRadius;
    OverrideableProperty<float> GameplayEdgeSharpening;
    OverrideableProperty<bool> GameplayCheckLOS;
    OverrideableProperty<float> GameplaySpotlightAngle;
    OverrideableProperty<glm::vec3> GameplayDirectionalDimensions;
    OverrideableProperty<FixedString> LightCookieResource;
    OverrideableProperty<bool> IsHalfLit;
    OverrideableProperty<bool> IsSunlight;
};

struct ConstructionVertexElement
{
    glm::vec3 Position;
    glm::vec2 UV;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
};

struct ConstructionPointSchema
{
    glm::vec4 Rotation;
    glm::vec3 Position;
    glm::vec3 Scale;
    Guid InstanceId;
    uint8_t field_38;
    uint8_t field_39;
};

struct ConstructionPoint : public ConstructionPointSchema
{
    uint8_t field_40;
    uint8_t field_41;
    Array<ConstructionPoint*> Branches;
};

struct ConstructionSpline : public ProtectedGameObject<ConstructionSpline>
{
    Guid InstanceId;
    [[bg3::hidden]] HashSet<ConstructionPoint*> Points;
    [[bg3::hidden]] LegacyRefMap<void*, void*>* field_40;
};

struct ConstructionTileTemplate;

struct TileSetNeighbourPointTileList
{
    uint32_t NonOptimalTilesStart;
    uint32_t NonOptimalTilesEnd;
    Array<ConstructionTileTemplate*> LeftCornerTiles;
    Array<ConstructionTileTemplate*> RightCornerTiles;
    Array<ConstructionTileTemplate*> StraightTiles;
};

struct TileSetConstructionNeighbourPoint
{
    Array<Guid> NeighbourPoints;
    HashMap<uint8_t, TileSetNeighbourPointTileList*> NeighbourLists;
};

struct TileSetConstructionLine;

struct TileSetConstructionPoint : public ProtectedGameObject<TileSetConstructionPoint>
{
    [[bg3::hidden]] void* VMT;
    Array<TileSetConstructionLine*> Lines;
    [[bg3::hidden]] Array<void*> field_18;
    Array<TileSetConstructionNeighbourPoint*> NeighbourPoints;
    ConstructionPoint* Point;
    uint8_t byte40;
    uint8_t byte41;
    bool ConstructionPointStop;
};

struct TileSetConstructionLine
{
    Array<TileSetConstructionPoint*> Points;
    TileSetConstructionPoint* Start;
    TileSetConstructionPoint* End;
    Guid InstanceId;
};


struct ConstructionFillingTemplate : public ProtectedGameObject<ConstructionFillingTemplate>
{
    virtual ~ConstructionFillingTemplate();

    Array<FixedString> FadeChildren;
    Array<ConstructionVertexElement> Vertices;
    Array<uint16_t> Indices;
    STDString Name;
    FixedString Id;
    FixedString Material;
    FixedString Physics;
    FixedString FadeGroup;
    float Tiling;
    float UVRotation;
    glm::vec2 UVOffset;
    float Scale;
    glm::vec4 Rotation;
    glm::vec3 Translate;
    glm::vec3 BoundMin;
    glm::vec3 BoundMax;
    bool WalkOn;
    bool SeeThrough;
    bool Fadeable;
    bool HierarchyOnlyFade;
    Guid HLOD;
    [[bg3::hidden]] void* gapC0;
    Array<ConstructionSpline*> Splines;
    uint8_t field_D8;
    bool HiddenFromMinimapRendering;
};

struct ConstructionTileTemplate
{
    virtual ~ConstructionTileTemplate();

    Array<Transform> Transforms;
    Guid Tile;
    Guid HLOD;
    FixedString UUID;
    FixedString OverridePhysicsResource;
    glm::vec4 Rotation;
    glm::vec3 Translate;
    float ScaleX;
    float Scale;
    uint8_t ShootThroughType;
    bool WalkOn;
    bool Climbable;
    bool ShootThrough;
    bool CanSeeThrough;
    bool WalkThrough;
    bool ClickThrough;
    [[bg3::hidden]] HashSet<void*> ReferenceFromNeighbourPoint; // TileSetNeighbourPointTileList*
    Guid TemplateGuid;
    int Side;
    uint8_t gapA4;
    bool Flip;
    bool Stretchable;
};

struct ConstructionTemplate : public GameObjectTemplate
{
    __int64 field_100;
    __int64 field_108;
    Array<ConstructionTileTemplate*> Tiles;
    Array<FixedString> FadeChildren;
    OverrideableProperty<FixedString> FadeGroup;
    OverrideableProperty<FixedString> TileSet;
    OverrideableProperty<bool> ConstructionBend;
    OverrideableProperty<bool> Fadeable;
    OverrideableProperty<bool> SeeThrough;
    OverrideableProperty<bool> HierarchyOnlyFade;
    // Editor only?
    // [[bg3::hidden]] Array<TileSetConstructionPoint*> Points;
    // [[bg3::hidden]] Array<TileSetConstructionLine*> Lines;
    // [[bg3::hidden]] void* Spline;
    // [[bg3::hidden]] LegacyRefMap<int, int>* field_170;
    // [[bg3::hidden]] LegacyRefMap<int, int>* field_178;
    // OverrideableProperty<bool> WalkOn;
    // OverrideableProperty<bool> Climbable;
    // OverrideableProperty<bool> ShootThrough;
    // OverrideableProperty<uint8_t> ShootThroughType;
    // OverrideableProperty<bool> CanSeeThrough;
    // OverrideableProperty<bool> WalkThrough;
    // OverrideableProperty<bool> ClickThrough;
    // OverrideableProperty<bool> ConstructionPlaceTwoTiles;
};

struct ITriggerData : public ProtectedGameObject<ITriggerData>
{
    virtual ~ITriggerData() = 0;
};

struct ITriggerPhysicsData : public ProtectedGameObject<ITriggerPhysicsData>
{
    virtual ~ITriggerPhysicsData() = 0;
};

struct TriggerPhysicsSphere : public ITriggerPhysicsData
{
    float Radius;
};

struct TriggerPhysicsBox : public ITriggerPhysicsData
{
    glm::vec3 Extents;
};

struct TriggerPhysicsPoly : public ITriggerPhysicsData
{
    Array<glm::vec2> Points;
    float Height;
};

struct TriggerTemplate : public GameObjectTemplate
{
    [[bg3::hidden]] ITriggerData* TriggerData;
    [[bg3::hidden]] ITriggerPhysicsData* PhysicsData;
    OverrideableProperty<FixedString> TriggerType;
    OverrideableProperty<uint8_t> PhysicsType;
    OverrideableProperty<glm::aligned_highp_vec4> Color;
    OverrideableProperty<FixedString> TriggerGizmoOverride;
    OverrideableProperty<bool> Fadeable;
    OverrideableProperty<bool> HierarchyOnlyFade;
    OverrideableProperty<bool> FadeGroupOnly;
    OverrideableProperty<bool> IgnoreRaycast;
    OverrideableProperty<FixedString> FadeGroup;
    OverrideableProperty<Array<FixedString>> FadeChildren;
    OverrideableProperty<glm::aligned_highp_vec4> GizmoColorOverride;
    OverrideableProperty<bool> UsingGizmoColorOverride;
    OverrideableProperty<bool> HasCustomPoint;
    OverrideableProperty<bg3se::Transform> CustomPointTransform;
    [[bg3::hidden]] OverrideableProperty<Array<void*>> ConstellationConfigGlobalParameters;
    OverrideableProperty<FixedString> ConstellationConfigName;
    OverrideableProperty<uint8_t> EventSendingMode;
    OverrideableProperty<bool> OnlyCharacterEvents;
};


struct [[bg3::hidden]] GlobalTemplateBank
{
    void* VMT;
    LegacyMap<FixedString, GameObjectTemplate*> Templates;
    Array<void*> field_20;
    Array<void*> field_30;
    Array<void*> field_40;
    Array<void*> field_50;
    int field_60;
    int field_64;
    FixedString field_68;
};


struct [[bg3::hidden]] GlobalTemplateManager
{
    void* VMT;
    LegacyMap<FixedString, GameObjectTemplate*> Templates;
    std::array<GlobalTemplateBank*, 2> Banks;
};


struct [[bg3::hidden]] CacheTemplateManagerBase
{
    void* VMT;
    uint8_t TemplateManagerType;
    HashMap<FixedString, GameObjectTemplate*> Templates;
    HashMap<uint32_t, GameObjectTemplate*> TemplatesByHandle;
    HashMap<uint32_t, uint32_t> RefCountsByHandle;
    SRWLock Lock;
    Array<void*> NewTemplates;
    Array<void*> CacheTemplateRemovers;
    bool field_100;
};


struct [[bg3::hidden]] LocalTemplateManager
{
    SRWLOCK Lock;
    LegacyRefMap<FixedString, GameObjectTemplate*> Templates;
    LegacyRefMap<uint16_t, Array<GameObjectTemplate*>*> TemplatesByType;
    LegacyRefMap<uint32_t, GameObjectTemplate*> TemplatesByHandle;
    void* LocalLoadHelper;
    CRITICAL_SECTION CriticalSection;
    LegacyRefMap<FixedString, LegacyRefMap<FixedString, GameObjectTemplate*>> TemplatesByLevel;
    int field_78;
};


END_SE()

BEGIN_NS(lua)

LUA_POLYMORPHIC(GameObjectTemplate)
LUA_INFINITE_LIFETIME(GameObjectTemplate)
LUA_INFINITE_LIFETIME(EoCGameObjectTemplate)
LUA_INFINITE_LIFETIME(SceneryTemplate)
LUA_INFINITE_LIFETIME(ItemTemplate)
LUA_INFINITE_LIFETIME(CharacterTemplate)
LUA_INFINITE_LIFETIME(ProjectileTemplate)
LUA_INFINITE_LIFETIME(SurfaceTemplate)
LUA_INFINITE_LIFETIME(LightTemplate)
LUA_INFINITE_LIFETIME(CombinedLightTemplate)

END_NS()
