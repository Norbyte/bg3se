#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_SE()

struct GameObjectTemplate
{
    virtual ~GameObjectTemplate() = 0;
    virtual void* GetName(void*) = 0;
    virtual void* DebugDump(void*) = 0;
    virtual FixedString* GetType() = 0;
    virtual FixedString* GetRealType() = 0;
    virtual void SetTransform(Transform const&, void*) = 0;
    virtual bool IsValidV40026() = 0;
    virtual GameObjectTemplate* RealClone() = 0;
    virtual bool HasRotation() = 0;
    virtual void UpdateTransformFromLevel() = 0;
    virtual void Visit(ObjectVisitor&) = 0;
    virtual void Noop() = 0;
    virtual bool IsValid() = 0;
    virtual Guid* GetHLOD() = 0;
    virtual GameObjectTemplate* Clone() = 0;
    virtual GameObjectTemplate* PartialClone(GameObjectTemplate* other) = 0;
    virtual void OverrideFromParent(GameObjectTemplate* other, bool force) = 0;
    virtual void UpdateOverrideFlagsFromParent(GameObjectTemplate* other) = 0;
    virtual bool PostLoad() = 0;
    virtual bool HasAIBounds() = 0;
    virtual void* GetAIBounds1(void*, uint16_t) = 0;
    virtual void* GetAIBounds2(void*, uint16_t) = 0;
    virtual double GetAIBounds3(void*, uint16_t) = 0;
    virtual void* GetAIBounds4(void*, uint16_t) = 0;
    virtual double GetAIBounds5(void*, uint16_t) = 0;
    virtual void UpdateFadeFromParent(GameObjectTemplate* parent, bool) = 0;
    virtual void UpdateHierarchyOnlyFadeFromParent(GameObjectTemplate* parent, bool) = 0;
    virtual void UpdateSeeThroughFromParent(GameObjectTemplate* parent, bool) = 0;
    virtual void UpdateCollideWithCameraFromParent(GameObjectTemplate* parent, bool) = 0;
    virtual void UnknownFunc(bool) = 0;
    virtual uint16_t* GetVisualFlags(uint16_t&) = 0;
    virtual OverrideableProperty<bool>* IsPlatformOwner() = 0;
    virtual void* GetOnUsePeaceActions() = 0;
    virtual OverrideableProperty<bool>* IsTrap() = 0;

    FixedString GetTemplateType();

    //# P_GETTER(TemplateType, GetTemplateType)

    OverrideableProperty<uint32_t> Flags;
    [[bg3::hidden]]
    void* Tags;
    [[bg3::readonly]]
    FixedString Id;
    [[bg3::readonly]]
    STDString Name;
    [[bg3::readonly]]
    FixedString TemplateName;
    [[bg3::readonly]]
    FixedString ParentTemplateId;
    int32_t field_50;
    bool IsGlobal;
    bool IsDeleted;
    FixedString LevelName;
    uint8_t _Pad[4];
    OverrideableProperty<uint32_t> GroupID;
    OverrideableProperty<Transform> Transform;
    OverrideableProperty<FixedString> VisualTemplate;
    OverrideableProperty<FixedString> PhysicsTemplate;
    OverrideableProperty<FixedString> PhysicsOpenTemplate;
    OverrideableProperty<bool> CastShadow;
    OverrideableProperty<bool> ReceiveDecal;
    OverrideableProperty<bool> AllowReceiveDecalWhenAnimated;
    OverrideableProperty<bool> IsReflecting;
    OverrideableProperty<bool> IsShadowProxy;
    OverrideableProperty<uint8_t> RenderChannel;
    OverrideableProperty<glm::vec3> CameraOffset;
    OverrideableProperty<bool> HasParentModRelation;
    uint8_t _Pad2[6];
    OverrideableProperty<bool> HasGameplayValue;
    STDString FileName;
};


struct EoCGameObjectTemplate : public GameObjectTemplate
{
    [[bg3::hidden]]
    OverrideableProperty<Array<void*>> AIBounds;
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
    OverrideableProperty<FixedString> LoopSound;
    OverrideableProperty<FixedString> SoundInitEvent;
    OverrideableProperty<int16_t> SoundAttenuation;
    OverrideableProperty<Guid> HLOD;
    OverrideableProperty<uint8_t> ShootThroughType;
    OverrideableProperty<bool> CanShineThrough;
    OverrideableProperty<FixedString> ShadowPhysicsProxy;
    OverrideableProperty<uint8_t> WadableSurfaceType;
    OverrideableProperty<bool> BlockAoEDamage;
    bool ReferencedInTimeline;
}; 


struct CombatComponentTemplate
{
    [[bg3::hidden]]
    void* VMT;
    OverrideableProperty<FixedString> Archetype;
    OverrideableProperty<FixedString> SwarmGroup;
    OverrideableProperty<Guid> Faction;
    OverrideableProperty<bool> CanFight;
    OverrideableProperty<bool> CanJoinCombat;
    OverrideableProperty<FixedString> CombatGroupID;
    OverrideableProperty<STDString> CombatName;
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
    OverrideableProperty<FixedString> LightID;
    OverrideableProperty<FixedString> ActiveCharacterLightID;
    OverrideableProperty<FixedString> BloodType;
    OverrideableProperty<FixedString> CriticalHitType;
    OverrideableProperty<FixedString> DefaultDialog;
    MultiHashSet<Guid>* SpeakerGroupList;
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
    OverrideableProperty<Array<Guid>> ExcludeInDifficulty;
    OverrideableProperty<Array<Guid>> OnlyInDifficulty;
    OverrideableProperty<bool> JumpUpLadders;
    OverrideableProperty<Guid> EquipmentRace;
    [[bg3::hidden]]
    OverrideableProperty<Array<void*>> OnDeathActions;
    OverrideableProperty<float> DeathRaycastMinLength;
    OverrideableProperty<float> DeathRaycastMaxLength;
    OverrideableProperty<float> DeathRaycastHeight;
    OverrideableProperty<float> DeathRaycastVerticalLength;
    OverrideableProperty<uint8_t> BloodSurfaceType;
    OverrideableProperty<bool> ForceLifetimeDeath;
    OverrideableProperty<Guid> DeathEffect;
    OverrideableProperty<FixedString> ExplodedResourceID;
    OverrideableProperty<FixedString> ExplosionFX;
    OverrideableProperty<FixedString> DisintegratedResourceID;
    OverrideableProperty<FixedString> DisintegrateFX;
    OverrideableProperty<FixedString> AnubisConfigName;
    [[bg3::hidden]]
    OverrideableProperty<Array<void*>> SkillList;
    [[bg3::hidden]]
    OverrideableProperty<Array<void*>> ItemList;
    OverrideableProperty<Array<FixedString>> StatusList;
    OverrideableProperty<FixedString> TrophyID;
    OverrideableProperty<FixedString> SoundInitEvent;
    OverrideableProperty<FixedString> SoundMovementStartEvent;
    OverrideableProperty<FixedString> SoundMovementStopEvent;
    OverrideableProperty<int8_t> SoundObjectIndex;
    OverrideableProperty<int16_t> SoundAttenuation;
    OverrideableProperty<bool> UseOcclusion;
    OverrideableProperty<bool> UseSoundClustering;

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
    OverrideableProperty<int32_t> LevelOverride;
    OverrideableProperty<bool> CanBeTeleported;
    OverrideableProperty<FixedString> ActivationGroupId;
    OverrideableProperty<RefMap<FixedString, FixedString>> PickingPhysicsTemplates;
    OverrideableProperty<FixedString> SoftBodyCollisionTemplate;
    OverrideableProperty<FixedString> RagdollTemplate;
    [[bg3::hidden]]
    OverrideableProperty<Array<void*>> FootStepInfos;
    OverrideableProperty<uint8_t> DefaultState;
    OverrideableProperty<bool> IsLootable;
    OverrideableProperty<bool> IsEquipmentLootable;
    OverrideableProperty<bool> CanBePickedUp;
    OverrideableProperty<bool> CanBePickpocketed;
    OverrideableProperty<uint8_t> IsTradable;
    OverrideableProperty<bool> IsDroppedOnDeath;
    OverrideableProperty<uint8_t> LightChannel;
    OverrideableProperty<uint8_t> AliveInventoryType;
    OverrideableProperty<uint8_t> InventoryType;
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
};


struct ItemTemplate : public SceneryTemplate
{
    CombatComponentTemplate CombatComponent;
    OverrideableProperty<Array<FixedString>> InventoryList;
    MultiHashSet<Guid>* SpeakerGroups;
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
    OverrideableProperty<Array<Guid>> ExcludeInDifficulty;
    OverrideableProperty<Array<Guid>> OnlyInDifficulty;
    OverrideableProperty<bool> UsePartyLevelForTreasureLevel;
    OverrideableProperty<bool> Unimportant;
    OverrideableProperty<bool> Hostile;
    OverrideableProperty<bool> UseOnDistance;
    OverrideableProperty<bool> UseRemotely;
    OverrideableProperty<bool> PhysicsFollowAnimation;
    OverrideableProperty<bool> CanBeImprovisedWeapon;
    OverrideableProperty<bool> ForceAffectedByAura;
    OverrideableProperty<bool> IsBlueprintDisabledByDefault;
    OverrideableProperty<uint8_t> IsTradable;
    OverrideableProperty<TranslatedString> UnknownDisplayName;
    OverrideableProperty<uint8_t> GravityType;
    OverrideableProperty<uint32_t> Tooltip;
    OverrideableProperty<FixedString> Stats;
    [[bg3::hidden]]
    OverrideableProperty<Array<void*>> OnUsePeaceActions;
    [[bg3::hidden]]
    OverrideableProperty<Array<void*>> OnDestroyActions;
    OverrideableProperty<TranslatedString> OnUseDescription;
    [[bg3::hidden]]
    OverrideableProperty<Array<void*>> Scripts;
    [[bg3::hidden]]
    OverrideableProperty<MultiHashMap<FixedString, void*>> ScriptOverrides;
    OverrideableProperty<FixedString> AnubisConfigName;
    [[bg3::hidden]]
    OverrideableProperty<Array<void*>> ScriptConfigGlobalParameters;
    OverrideableProperty<FixedString> ConstellationConfigName;
    [[bg3::hidden]]
    OverrideableProperty<Array<void*>> ConstellationConfigGlobalParameters;
    [[bg3::hidden]]
    OverrideableProperty<Array<void*>> ItemList;
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
    [[bg3::hidden]]
    OverrideableProperty<void*> SomeTemplateInfo;
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
    OverrideableProperty<STDString> TechnicalDescriptionParams;
    OverrideableProperty<TranslatedString> ShortDescription;
    OverrideableProperty<STDString> ShortDescriptionParams;
    OverrideableProperty<TranslatedString> UnknownDescription;
    OverrideableProperty<bool> ShowAttachedSpellDescriptions;
    OverrideableProperty<FixedString> PermanentWarnings;
    OverrideableProperty<bool> ContainerAutoAddOnPickup;
    OverrideableProperty<STDString> ContainerContentFilterCondition;
    MultiHashSet<Guid>* InteractionFilterList;
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
    OverrideableProperty<bool> IsPortal;
    OverrideableProperty<bool> AttackableWhenClickThrough;
    OverrideableProperty<uint32_t> CinematicArenaFlags;
    OverrideableProperty<Guid> TimelineCameraRigOverride;
    OverrideableProperty<Guid> MaterialPreset;
    OverrideableProperty<Guid> ColorPreset;
    OverrideableProperty<glm::vec3> ExamineRotation;
    bool V4_0_0_26Flag;
};


struct ProjectileTemplate : public EoCGameObjectTemplate
{
    OverrideableProperty<float> LifeTime;
    OverrideableProperty<float> Speed;
    OverrideableProperty<float> Acceleration;
    OverrideableProperty<FixedString> CastBone;
    OverrideableProperty<FixedString> ImpactFX;
    OverrideableProperty<STDString> GroundImpactFX;
    OverrideableProperty<FixedString> TrailFX;
    OverrideableProperty<bool> DestroyTrailFXOnImpact;
    OverrideableProperty<FixedString> BeamFX;
    OverrideableProperty<FixedString> PreviewPathMaterial;
    OverrideableProperty<FixedString> PreviewPathImpactFX;
    OverrideableProperty<float> PreviewPathRadius;
    OverrideableProperty<bool> RotateImpact;
    OverrideableProperty<bool> IgnoreRoof;
    OverrideableProperty<bool> DetachBeam;
    OverrideableProperty<bool> NeedsImpactSFX;
    OverrideableProperty<uint8_t> ProjectilePath;
    OverrideableProperty<float> PathShift;
    OverrideableProperty<float> PathRadius;
    OverrideableProperty<float> MinPathRadius;
    OverrideableProperty<float> MaxPathRadius;
    OverrideableProperty<float> PathMinArcDist;
    OverrideableProperty<float> PathMaxArcDist;
    OverrideableProperty<int32_t> PathRepeat;
};

struct SurfaceTemplate : public GameObjectTemplate
{
    struct VisualData
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

    struct StatusData
    {
        FixedString StatusId;
        uint8_t ApplyTypes;
        float Chance;
        float Duration;
        bool Remove;
        bool ApplyToCharacters;
        bool ApplyToItems;
        bool KeepAlive;
        bool VanishOnApply;
        bool Force;
        bool AffectedByRoll;
        bool OnlyOncePerTurn;
    };

    SurfaceType SurfaceType;
    FixedString SurfaceName;
    uint64_t field_158[4];
    uint32_t field_188;
    OverrideableProperty<TranslatedString> DisplayName;
    OverrideableProperty<TranslatedString> Description;
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
    Array<VisualData> InstanceVisual;
    Array<VisualData> IntroFX;
    Array<VisualData> FX;
    Array<StatusData> Statuses;
    OverrideableProperty<FixedString> Summon;
    OverrideableProperty<bool> RemoveDestroyedItems;
    OverrideableProperty<bool> CanSeeThrough;
    OverrideableProperty<bool> CanShootThrough;
    OverrideableProperty<STDString> RollConditions;
    OverrideableProperty<uint8_t> ObscuredStateOverride;
    OverrideableProperty<float> OnEnterDistanceOverride;
    OverrideableProperty<float> OnMoveDistanceOverride;
    OverrideableProperty<glm::vec3> AiPathColor;
    OverrideableProperty<FixedString> AiPathIconFX;
};


struct [[bg3::hidden]] GlobalTemplateBank
{
    void* VMT;
    Map<FixedString, GameObjectTemplate*> Templates;
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
    Map<FixedString, GameObjectTemplate*> Templates;
    std::array<GlobalTemplateBank*, 2> Banks;
};


struct [[bg3::hidden]] CacheTemplateManagerBase
{
    void* VMT;
    uint8_t TemplateManagerType;
    MultiHashMap<FixedString, GameObjectTemplate*> Templates;
    MultiHashMap<uint32_t, GameObjectTemplate*> TemplatesByHandle;
    MultiHashMap<uint32_t, uint32_t> RefCountsByHandle;
    SRWLock Lock;
    Array<void*> NewTemplates;
    Array<void*> CacheTemplateRemovers;
    bool field_100;
};


struct [[bg3::hidden]] LocalTemplateManager
{
    SRWLOCK Lock;
    RefMap<FixedString, GameObjectTemplate*> Templates;
    RefMap<uint16_t, Array<GameObjectTemplate*>*> TemplatesByType;
    RefMap<uint32_t, GameObjectTemplate*> TemplatesByHandle;
    void* LocalLoadHelper;
    CRITICAL_SECTION CriticalSection;
    RefMap<FixedString, RefMap<FixedString, GameObjectTemplate*>> TemplatesByLevel;
    int field_78;
};


END_SE()

BEGIN_NS(lua)

LUA_POLYMORPHIC(GameObjectTemplate)

END_NS()
