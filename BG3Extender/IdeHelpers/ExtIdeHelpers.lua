-- Special global value that contains the current mod UUID during load
ModuleUUID = "UUID"

Osi = {}

--- Object handles are a type of userdata (lightuserdata)
--- @class ObjectHandle

--- @class DamageItem
--- @field public DamageType string
--- @field public Amount integer
local DamageItem = {}

--- @class DamageList
local DamageList = {}

--- Returns the amount of damage with the specified type
--- @param self DamageList
--- @param damageType string DamageType enumeration
--- @return integer
function DamageList.GetByType (self, damageType) end
    
--- Add damage
--- @param self DamageList
--- @param damageType string DamageType enumeration
--- @param amount integer
function DamageList.Add (self, damageType, amount) end
    
--- Clear damage list.
--- If damageType is specified it only removes damage items with the specified type.
--- @param self DamageList
--- @param damageType string|nil DamageType enumeration
function DamageList.Clear (self, damageType) end
    
--- Multiply every damage item with the specified value
--- @param self DamageList
--- @param multiplier number
function DamageList.Multiply (self, multiplier) end
    
--- Merge another DamageList into this list
--- @param self DamageList
--- @param list DamageList List to merge
function DamageList.Merge (self, list) end
    
--- Converts all damages to the specified type
--- @param self DamageList
--- @param damageType string DamageType enumeration
function DamageList.ConvertDamageType (self, damageType) end
    
--- Aggregate all damage items with the same type
--- @param self DamageList
function DamageList.AggregateSameTypeDamages (self) end
    
--- Returns all damage items as a table
--- @param self DamageList
--- @return DamageItem[]
function DamageList.ToTable (self) end

--- @class CombatComponentTemplate
--- @field public Archetype string
--- @field public CanFight boolean
--- @field public CanJoinCombat boolean
--- @field public CombatGroupID string
--- @field public CombatName string
--- @field public IsBoss boolean
--- @field public StayInAiHints boolean
--- @field public AiHint string
--- @field public IsInspector boolean
--- @field public StartCombatRange number


--- @class GameObjectTemplate
--- @field public Id string
--- @field public Name string
--- @field public TemplateName string
--- @field public ParentTemplateId string
--- @field public IsGlobal boolean
--- @field public IsDeleted boolean
--- @field public IsForcedType boolean
--- @field public LevelName string
--- @field public GroupID string
--- @field public Transform Transform
--- @field public NonUniformScale boolean
--- @field public VisualTemplate string
--- @field public PhysicsTemplate string
--- @field public CastShadow boolean
--- @field public ReceiveDecal boolean
--- @field public AllowReceiveDecalWhenAnimated boolean
--- @field public IsReflecting boolean
--- @field public IsShadowProxy boolean
--- @field public RenderChannel number
--- @field public CameraOffset number[]
--- @field public HasParentModRelation boolean
--- @field public HasGameplayValue boolean
--- @field public FileName string


--- @class EoCGameObjectTemplate : GameObjectTemplate
--- FIXME - map AIBounds
--- @field public DisplayName string
--- @field public Opacity number
--- @field public Fadeable boolean
--- @field public FadeIn boolean
--- @field public SeeThrough boolean
--- @field public HierarchyOnlyFade boolean
--- @field public FadeGroup string
--- @field public FadeChildren string[]
--- @field public GameMasterSpawnSubSection string


--- @class EoCGameObjectTemplate2 : EoCGameObjectTemplate
--- @field public CoverAmount boolean
--- @field public CanClimbOn boolean
--- @field public CanShootThrough boolean
--- @field public WalkThrough boolean
--- @field public WalkOn boolean
--- @field public Wadable boolean
--- @field public CanClickThrough boolean
--- @field public IsPointerBlocker boolean
--- @field public IsBlocker boolean
--- @field public IsDecorative boolean
--- @field public GenerateRunningDeepWater boolean
--- @field public LoopSound string
--- @field public SoundInitEvent string
--- @field public SoundAttenuation number
--- @field public HLOD GUID


--- @class CharacterTemplate : EoCGameObjectTemplate
--- @field public CombatComponent CombatComponentTemplate
--- FIXME - map ScriptConfigGlobalParameters
--- @field public Icon string
--- @field public Stats string
--- @field public SpellSet string
--- @field public Equipment string
--- @field public Treasures string[]
--- @field public TradeTreasures string[]
--- @field public LightID string
--- @field public ActiveCharacterLightID string
--- @field public BloodType string
--- @field public DefaultDialog string
--- FIXME - map SpeakerGroupList
--- @field public GeneratePortrait string
--- @field public LadderAttachSpeed number
--- @field public LadderLoopSpeed number
--- @field public LadderDetachSpeed number
--- @field public CanShootThrough boolean
--- @field public WalkThrough boolean
--- @field public CanClimbLadders boolean
--- @field public IsPlayer boolean
--- @field public SpotSneakers boolean
--- @field public CanOpenDoors boolean
--- @field public AvoidTraps boolean
--- @field public InfluenceTreasureLevel boolean
--- @field public HardcoreOnly boolean
--- @field public NotHardcore boolean
--- @field public JumpUpLadders boolean
--- @field public IsHuge boolean
--- @field public EquipmentRace GUID
--- FIXME - map OnDeathActions
--- @field public DeathRaycastMinLength number
--- @field public DeathRaycastMaxLength number
--- @field public DeathRaycastHeight number
--- @field public DeathRaycastVerticalLength number
--- @field public ExplodedResourceID string
--- @field public ExplosionFX string
--- FIXME - map Scripts
--- @field public AnubisConfigName string
--- FIXME - map SkillList
--- FIXME - map ItemList
--- @field public TrophyID string
--- @field public SoundInitEvent string
--- @field public SoundObjectIndex integer
--- @field public SoundAttenuation integer
--- @field public FoleyShortResourceID string
--- @field public FoleyMediumResourceID string
--- @field public FoleyLongResourceID string
--- FIXME - vocal resource IDs
--- @field public CharacterVisualResourceID string
--- @field public CoverAmount number
--- @field public LevelOverride number
--- @field public ForceUnsheathSkills boolean
--- @field public CanBeTeleported boolean
--- @field public ActivationGroupId string
--- @field public PickingPhysicsTemplates string[]
--- @field public SoftBodyCollisionTemplate string
--- @field public RagdollTemplate string
--- @field public DefaultState number
--- @field public GhostTemplate string
--- @field public IsLootable boolean
--- @field public LightChannel integer
--- @field public AliveInventoryType integer
--- @field public InventoryType integer
--- @field public Race GUID
--- @field public Title TranslatedString
--- @field public AnimationSetResourceID string
--- @field public HasPlayerApprovalRating boolean
--- @field public CanLongRest boolean
--- @field public DisableEquipping boolean
--- @field public WorldClimbingSpeed number
--- @field public IsMovementEnabled boolean
--- @field public MovementAcceleration number
--- @field public MovementSpeedStroll number
--- @field public MovementSpeedRun number
--- @field public MovementSpeedSprint number
--- @field public MovementSpeedDash number
--- @field public MaxDashDistance number
--- @field public MovementStepUpHeight number
--- @field public MovementTiltToRemap string
--- @field public TurningNodeAngle number
--- @field public TurningNodeOffset number
--- @field public IsSteeringEnabled boolean
--- @field public SteeringSpeedCurveWithoutTransitions string
--- @field public SteeringSpeedFallback number
--- @field public IsWorldClimbingEnabled boolean
--- @field public WorldClimbingBlendspace_DownA string
--- @field public WorldClimbingBlendspace_DownB string
--- @field public WorldClimbingBlendspace_DownBHeight number
--- @field public WorldClimbingBlendspace_UpA string
--- @field public WorldClimbingBlendspace_UpB string
--- @field public WorldClimbingBlendspace_UpBHeight number
--- @field public WorldClimbingHeight number
--- @field public ProbeLookAtOffset number
--- @field public ProbeSpineAOffset number
--- @field public ProbeSpineBOffset number
--- @field public ProbeTiltToOffset number
--- @field public VFXScale number


--- @class ItemTemplate : EoCGameObjectTemplate2
--- @field public CombatTemplate CombatComponentTemplate
--- @field public Icon string
--- @field public CanBePickedUp boolean
--- @field public CanBeMoved boolean
--- @field public Destroyed boolean
--- @field public IsInteractionDisabled boolean
--- @field public StoryItem boolean
--- @field public FreezeGravity boolean
--- @field public IsKey boolean
--- @field public IsTrap boolean
--- @field public IsSurfaceBlocker boolean
--- @field public IsSurfaceCloudBlocker boolean
--- @field public TreasureOnDestroy boolean
--- @field public IsHuge boolean
--- @field public HardcoreOnly boolean
--- @field public NotHardcore boolean
--- @field public UsePartyLevelForTreasureLevel boolean
--- @field public Unimportant boolean
--- @field public Hostile boolean
--- @field public UseOnDistance boolean
--- @field public UseRemotely boolean
--- @field public PhysicsFollowAnimation boolean
--- @field public ItemDisplayName string
--- @field public Tooltip number
--- @field public Stats string
--- @field public InventoryList string[]
--- @field public OnUseDescription TranslatedString
--- FIXME - map Scripts, ScriptConfigGlobalParameters, ItemList
--- @field public AnubisConfigName string
--- @field public DefaultState string
--- @field public Owner string
--- @field public Key string
--- @field public BloodType string
--- @field public LockDC number
--- @field public DisarmDC number
--- @field public Amount number
--- @field public MaxStackAmount number
--- @field public TreasureLevel number
--- @field public PickupSound string
--- @field public UseSound string
--- @field public EquipSound string
--- @field public UnequipSound string
--- @field public InventoryMoveSound string
--- @field public ImpactSound string
--- @field public SoundObjectIndex integer
--- @field public InventoryType integer
--- @field public Description string
--- @field public ItemDescription string
--- @field public Speaker string
--- @field public AltSpeaker string
--- @field public SpeakerGroupList string[]
--- @field public LevelOverride number
--- @field public Floating boolean
--- @field public IsSourceContainer boolean
--- @field public MeshProxy string
--- @field public ShortHair string
--- @field public LongHair string
--- @field public IsPublicDomain boolean
--- @field public AllowSummonTeleport boolean
--- @field public IsPortalProhibitedToPlayers boolean
--- @field public LightChannel integer
--- @field public EquipmentTypeID GUID


--- @class ProjectileTemplate : EoCGameObjectTemplate
--- @field public LifeTime number
--- @field public Speed number
--- @field public Acceleration number
--- @field public CastBone string
--- @field public ImpactFX string
--- @field public GroundImpactFX string
--- @field public TrailFX string
--- @field public DestroyTrailFXOnImpact boolean
--- @field public BeamFX string
--- @field public PreviewPathMaterial string
--- @field public PreviewPathImpactFX string
--- @field public PreviewPathRadius number
--- @field public RotateImpact boolean
--- @field public IgnoreRoof boolean
--- @field public DetachBeam boolean
--- @field public NeedsImpactSFX boolean
--- @field public ProjectilePath boolean
--- @field public PathShift string
--- @field public PathRadius number
--- @field public MinPathRadius number
--- @field public MaxPathRadius number
--- @field public PathMinArcDist number
--- @field public PathMaxArcDist number
--- @field public PathRepeat integer


--- @class SurfaceTemplateStatus
--- @field public StatusId string
--- @field public ApplyTypes integer
--- @field public Chance number
--- @field public Duration number
--- @field public Remove boolean
--- @field public ApplyToCharacters boolean
--- @field public ApplyToItems boolean
--- @field public KeepAlive boolean
--- @field public VanishOnReapply boolean
--- @field public Force boolean
--- @field public AffectedByRoll boolean
--- @field public OnlyOncePerTurn boolean


--- @class SurfaceTemplateVisual
--- @field public Visual string
--- @field public Height number[]
--- @field public Rotation integer[]
--- @field public Scale number[]
--- @field public GridSize number
--- @field public SpawnCell integer
--- @field public RandomPlacement integer
--- @field public SurfaceNeeded integer
--- @field public SurfaceRadiusMax integer


--- @class SurfaceTemplate : GameObjectTemplate
--- FIXME - map surfaceTypeId, surfaceType
--- @field public DisplayName string
--- @field public Description string
--- @field public DecalMaterial string
--- @field public MaterialType integer
--- @field public SurfaceCategory integer
--- @field public CanEnterCombat boolean
--- @field public AlwaysUseDefaultLifeTime boolean
--- @field public DefaultLifeTime number
--- @field public SurfaceGrowTimer number
--- @field public FadeInSpeed number
--- @field public FadeOutSpeed number
--- @field public FallDamageMultiplier number
--- @field public Seed integer
--- @field public NormalBlendingFactor integer
--- @field public InstanceVisual SurfaceTemplateVisual[]
--- @field public IntroFX SurfaceTemplateVisual[]
--- @field public FX SurfaceTemplateVisual[]
--- @field public Statuses SurfaceTemplateStatus[]
--- @field public Summon string
--- @field public RemoveDestroyedItems boolean
--- @field public CanSeeThrough boolean
--- @field public CanShootThrough boolean
--- @field public RollConditions string
--- @field public ObscuredStateOverride integer
--- @field public OnEnterDistanceOverride number
--- @field public OnMoveDistanceOverride number
--- @field public AiPathColor number[]
--- @field public AiPathIconFX string


-- FIXME EclStatus

--- @class EsvStatus
--- @field public StatusType string Engine status type (HIT, DAMAGE, CONSUME, ...)
--- @field public StatusId string
--- @field public CanEnterChance integer
--- @field public StartTimer number
--- @field public LifeTime number
--- @field public CurrentLifeTime number
--- @field public TurnTimer number
--- @field public Strength number
--- @field public StatsMultiplier number
--- @field public DamageSourceType string See CauseType enumeration
--- @field public StatusHandle ObjectHandle
--- @field public TargetHandle ObjectHandle Handle of game object this status was applied to
--- @field public StatusSourceHandle ObjectHandle Handle of game object that caused this status
---
--- StatusFlags0
--- @field public KeepAlive boolean
--- @field public IsOnSourceSurface boolean
--- @field public IsFromItem boolean
--- @field public Channeled boolean
--- @field public IsLifeTimeSet boolean
--- @field public InitiateCombat boolean
--- @field public Influence boolean
--- StatusFlags1
--- @field public BringIntoCombat boolean
--- @field public IsHostileAct boolean
--- @field public IsInvulnerable boolean
--- @field public IsResistingDeath boolean
--- StatusFlags2
--- @field public ForceStatus boolean
--- @field public ForceFailStatus boolean
--- @field public RequestClientSync boolean
--- @field public RequestDelete boolean
--- @field public RequestDeleteAtTurnEnd boolean
--- @field public Started boolean
local EsvStatus = {}

--- @class EsvStatusHit : EsvStatus
--- @field public HitByHandle ObjectHandle
--- @field public HitWithHandle ObjectHandle
--- @field public WeaponHandle ObjectHandle
--- @field public HitReason string
--- @field public SkillId string
--- @field public Interruption boolean
--- @field public AllowInterruptAction boolean
--- @field public ForceInterrupt boolean
--- @field public DecDelayDeathCount boolean
--- @field public ImpactPosition number[]
--- @field public ImpactOrigin number[]
--- @field public ImpactDirection number[]
local EsvStatusHit = {}

--- @class EsvStatusConsumeBase : EsvStatus
--- TODO - Skills, Items, ResetCooldownsSet, StatsIDs?
--- @field public ResetAllCooldowns boolean
--- @field public ResetOncePerCombat boolean
--- @field public ScaleWithVitality boolean
--- @field public LoseControl boolean
--- @field public ApplyStatusOnTick string
--- @field public EffectTime number
--- @field public StatsId string
--- @field public StackId string
--- @field public OriginalWeaponStatsId string
--- @field public OverrideWeaponStatsId string
--- @field public OverrideWeaponHandle ObjectHandle
--- @field public SavingThrow integer TODO enum + enum prop!
--- @field public SourceDirection number[]
--- @field public Turn integer
--- @field public HealEffectOverride string See HealEffect enumeration
--- @field public Poisoned boolean
local EsvStatusConsumeBase = {}

--- @class EsvStatusDying : EsvStatus
--- @field public SourceHandle ObjectHandle
--- @field public SourceType integer
--- @field public DeathType string See DeathType enumeration
--- @field public AttackDirection integer
--- @field public ImpactDirection number[]
--- @field public IsAlreadyDead boolean
--- @field public DieActionsCompleted boolean
--- @field public ForceNoGhost boolean
--- @field public SkipAnimation boolean
--- @field public DontThrowDeathEvent boolean
--- @field public InflicterHandle ObjectHandle
--- @field public DisputeTargetHandle ObjectHandle
--- @field public CombatId integer
--- @field public IgnoreGodMode boolean
local EsvStatusDying = {}

--- @class EsvStatusHeal : EsvStatus
--- @field public EffectTime number
--- @field public HealAmount integer
--- @field public HealEffect string See HealEffect enumeration
--- @field public HealEffectId string
--- @field public HealType string See StatusHealType enumeration
--- @field public AbsorbSurfaceRange integer
--- @field public TargetDependentHeal boolean
local EsvStatusHeal = {}

--- @class EsvStatusMuted : EsvStatusConsumeBase

--- @class EsvStatusCharmed : EsvStatusConsumeBase
--- @field public UserId integer
--- @field public OriginalOwnerCharacterHandle ObjectHandle
local EsvStatusCharmed = {}

--- @class EsvStatusKnockedDown : EsvStatus
--- @field public KnockedDownState integer
--- @field public IsInstant boolean
local EsvStatusKnockedDown = {}

--- @class EsvStatusSummoning : EsvStatus
--- @field public AnimationDuration number
--- @field public SummonLevel integer
local EsvStatusSummoning = {}

--- @class EsvStatusHealing : EsvStatus
--- @field public HealAmount integer
--- @field public TimeElapsed number
--- @field public HealEffect string See HealEffect enumeration
--- @field public HealEffectId string
--- @field public SkipInitialEffect boolean
--- @field public HealingEvent number
--- @field public HealStat string See StatusHealType enumeration
--- @field public AbsorbSurfaceRange integer
local EsvStatusHealing = {}

--- @class EsvStatusThrown : EsvStatus
--- @field public Level integer
--- @field public CasterHandle ObjectHandle
--- @field public AnimationDuration number
--- @field public IsThrowingSelf boolean
--- @field public LandingEstimate number
--- @field public Landed boolean
local EsvStatusThrown = {}

--- @class EsvStatusTeleportFall : EsvStatus
--- @field public Target number[]
--- @field public ReappearTime number
--- @field public SkillId string
--- @field public HasDamage boolean
--- @field public HasDamageBeenApplied boolean
local EsvStatusTeleportFall = {}

--- @class EsvStatusConsume : EsvStatusConsumeBase

--- @class EsvStatusCombat : EsvStatus
--- @field public ReadyForCombat boolean
--- @field public OwnerTeamId integer
local EsvStatusCombat = {}

--- @class EsvStatusAoO : EsvStatus
--- @field public SourceHandle ObjectHandle
--- @field public TargetHandle ObjectHandle
--- @field public PartnerHandle ObjectHandle
--- @field public ActivateAoOBoost boolean
--- @field public ShowOverhead boolean
local EsvStatusAoO = {}

--- @class EsvStatusStoryFrozen : EsvStatus

--- @class EsvStatusSneaking : EsvStatus
--- @field public ClientRequestStop boolean
local EsvStatusSneaking = {}

--- @class EsvStatusUnlock : EsvStatus
--- @field public SourceHandle ObjectHandle
--- @field public Key string
--- @field public Level integer
--- @field public Unlocked integer
local EsvStatusUnlock = {}

--- @class EsvStatusFear : EsvStatus

--- @class EsvStatusBoost : EsvStatus
--- @field public EffectTime number
--- @field public BoostId string
local EsvStatusBoost = {}

--- @class EsvStatusUnsheathed : EsvStatus
--- @field public Force boolean
local EsvStatusUnsheathed = {}

--- @class EsvStatusStance : EsvStatusConsumeBase
--- @field public SkillId string
local EsvStatusStance = {}

--- @class EsvStatusLying : EsvStatus
--- @field public ItemHandle ObjectHandle
--- @field public Position number[]
--- @field public Index integer
--- @field public TimeElapsed number
--- @field public Heal number
local EsvStatusLying = {}

--- @class EsvStatusBlind : EsvStatusConsumeBase

--- @class EsvStatusSmelly : EsvStatus

--- @class EsvStatusClean : EsvStatus

--- @class EsvStatusInfectiousDiseased : EsvStatusConsumeBase
--- @field public Infections integer
--- @field public InfectTimer number
--- @field public Radius number
--- @field public TargetHandle ObjectHandle
local EsvStatusInfectiousDiseased = {}

--- @class EsvStatusInvisible : EsvStatusConsumeBase
--- @field public InvisiblePosition number[]
local EsvStatusInvisible = {}

--- @class EsvStatusRotate : EsvStatus
--- @field public Yaw number
--- @field public RotationSpeed number
local EsvStatusRotate = {}

--- @class EsvStatusEncumbered : EsvStatusConsumeBase

--- @class EsvStatusIdentify : EsvStatus
--- @field public Level integer
--- @field public Identified integer
--- @field public IdentifierHandle ObjectHandle
local EsvStatusIdentify = {}

--- @class EsvStatusRepair : EsvStatus
--- @field public Level integer
--- @field public Repaired integer
--- @field public RepairerHandle ObjectHandle
local EsvStatusRepair = {}

--- @class EsvStatusMaterial : EsvStatus
--- @field public MaterialUUID string
--- @field public ApplyOnBody boolean
--- @field public ApplyOnArmor boolean
--- @field public ApplyOnWeapon boolean
--- @field public ApplyOnWings boolean
--- @field public ApplyOnHorns boolean
--- @field public ApplyOnOverhead boolean
--- @field public IsOverlayMaterial boolean
--- @field public Fading boolean
--- @field public ApplyNormalMap boolean
--- @field public Force boolean
local EsvStatusMaterial = {}

--- @class EsvStatusLeadership : EsvStatusConsumeBase

--- @class EsvStatusExplode : EsvStatus
--- @field public Projectile string
local EsvStatusExplode = {}

--- @class EsvStatusAdrenaline : EsvStatusConsumeBase
--- @field public InitialAPMod integer
--- @field public SecondaryAPMod integer
--- @field public CombatTurn integer
local EsvStatusAdrenaline = {}

--- @class EsvStatusShacklesOfPain : EsvStatusConsumeBase
--- @field public CasterHandle ObjectHandle
local EsvStatusShacklesOfPain = {}

--- @class EsvStatusShacklesOfPainCaster : EsvStatusConsumeBase
--- @field public VictimHandle ObjectHandle
local EsvStatusShacklesOfPainCaster = {}

--- @class EsvStatusWindWalker : EsvStatusConsumeBase

--- @class EsvStatusDarkAvenger : EsvStatusConsumeBase

--- @class EsvStatusRemorse : EsvStatusConsumeBase

--- @class EsvStatusDecayingTouch : EsvStatusConsumeBase

--- @class EsvStatusUnhealable : EsvStatus

--- @class EsvStatusFlanked : EsvStatus

--- @class EsvStatusChanneling : EsvStatusStance

--- @class EsvStatusDrain : EsvStatus
--- @field public Infused integer
local EsvStatusDrain = {}

--- @class EsvStatusLingeringWounds : EsvStatusConsumeBase

--- @class EsvStatusInfused : EsvStatus

--- @class EsvStatusSpiritVision : EsvStatusConsumeBase
--- @field public SpiritVisionSkillId string
local EsvStatusSpiritVision = {}

--- @class EsvStatusSpirit : EsvStatus
local EsvStatusSpirit = {}

--- @class EsvStatusDamage : EsvStatusConsumeBase
--- @field public DamageEvent integer
--- @field public HitTimer number
--- @field public TimeElapsed number
--- @field public DamageLevel integer
--- @field public DamageStats string
--- @field public SpawnBlood boolean
local EsvStatusDamage = {}

--- @class EsvStatusForceMove : EsvStatusConsumeBase

--- @class EsvStatusClimbing : EsvStatus
--- @field public MoveDirection number[]
--- @field public LadderHandle ObjectHandle
--- @field public Level string
--- @field public Status integer
--- @field public Direction boolean
local EsvStatusClimbing = {}

--- @class EsvStatusIncapacitated : EsvStatusConsumeBase
--- @field public CurrentFreezeTime number
--- @field public FreezeTime number
--- @field public FrozenFlag integer
local EsvStatusIncapacitated = {}

--- @class EsvStatusInSurface : EsvStatus
--- @field public SurfaceTimerCheck number
--- @field public SurfaceDistanceCheck number
--- @field public Translate number[]
--- @field public Force boolean
--- TODO -- document surface layer flags!
local EsvStatusInSurface = {}

--- @class EsvStatusSourceMuted : EsvStatus

--- @class EsvStatusOverpowered : EsvStatusConsumeBase

--- @class EsvStatusCombustion : EsvStatusConsumeBase

--- @class EsvStatusPolymorphed : EsvStatusConsumeBase
--- @field public OriginalTemplate string
--- @field public TransformedRace string
--- @field public OriginalTemplateType integer
--- @field public PolymorphResult string
--- @field public DisableInteractions boolean
local EsvStatusPolymorphed = {}

--- @class EsvStatusDamageOnMove : EsvStatusDamage
--- @field public DistancePerDamage number
--- @field public DistanceTraveled number
local EsvStatusDamageOnMove = {}

--- @class EsvStatusDemonicBargain : EsvStatus

--- @class EsvStatusGuardianAngel : EsvStatusConsumeBase

--- @class EsvStatusFloating : EsvStatusConsumeBase

--- @class EsvStatusChallenge : EsvStatusConsumeBase
--- @field public SourceHandle ObjectHandle
--- @field public Target boolean
local EsvStatusChallenge = {}

--- @class EsvStatusDisarmed : EsvStatusConsumeBase

--- @class EsvStatusHealSharing : EsvStatusConsumeBase
--- @field public CasterHandle ObjectHandle
local EsvStatusHealSharing = {}

--- @class EsvStatusHealSharingCaster : EsvStatusConsumeBase
--- @field public TargetHandle ObjectHandle
local EsvStatusHealSharingCaster = {}

--- @class EsvStatusExtraTurn : EsvStatusConsumeBase

--- @class EsvStatusActiveDefense : EsvStatusConsumeBase
--- @field public Charges integer
--- @field public TargetPos number[]
--- @field public TargetHandle ObjectHandle
--- @field public Radius number
--- @field public Projectile string
local EsvStatusActiveDefense = {}

--- @class EsvStatusSpark : EsvStatusConsumeBase
--- @field public Charges integer
--- @field public Radius number
--- @field public Projectile string
local EsvStatusSpark = {}

--- @class EsvStatusPlayDead : EsvStatusConsumeBase

--- @class EsvStatusConstrained : EsvStatusLying

--- @class EsvStatusEffect : EsvStatus

--- @class EsvStatusDeactivated : EsvStatusConsumeBase

--- @class EsvStatusTutorialBed : EsvStatus



--- @class StatCharacterDynamic : StatBase
--- Properties from PropertyMap
--- @field public SummonLifelinkModifier integer
--- @field public Strength integer
--- @field public Memory integer
--- @field public Intelligence integer
--- @field public Movement integer
--- @field public MovementSpeedBoost integer
--- @field public Finesse integer
--- @field public Wits integer
--- @field public Constitution integer
--- @field public FireResistance integer
--- @field public EarthResistance integer
--- @field public WaterResistance integer
--- @field public AirResistance integer
--- @field public PoisonResistance integer
--- @field public ShadowResistance integer
--- @field public Willpower integer
--- @field public Bodybuilding integer
--- @field public PiercingResistance integer
--- @field public PhysicalResistance integer
--- @field public CorrosiveResistance integer
--- @field public MagicResistance integer
--- @field public CustomResistance integer
--- @field public Sight integer
--- @field public Hearing integer
--- @field public FOV integer
--- @field public APMaximum integer
--- @field public APStart integer
--- @field public APRecovery integer
--- @field public CriticalChance integer
--- @field public Initiative integer
--- @field public Vitality integer
--- @field public VitalityBoost integer
--- @field public MagicPoints integer
--- @field public Level integer
--- @field public Gain integer
--- @field public Armor integer
--- @field public MagicArmor integer
--- @field public ArmorBoost integer
--- @field public MagicArmorBoost integer
--- @field public ArmorBoostGrowthPerLevel integer
--- @field public MagicArmorBoostGrowthPerLevel integer
--- @field public DamageBoost integer
--- @field public DamageBoostGrowthPerLevel integer
--- @field public Accuracy integer
--- @field public Dodge integer
--- @field public MaxResistance integer
--- @field public LifeSteal integer
--- @field public Weight integer
--- @field public ChanceToHitBoost integer
--- @field public RangeBoost integer
--- @field public APCostBoost integer
--- @field public SPCostBoost integer
--- @field public MaxSummons integer
--- @field public BonusWeaponDamageMultiplier integer
--- @field public TranslationKey integer
--- @field public BonusWeapon integer
--- @field public StepsType integer
-- TODO Abilities, Talents, RemovedTalents, Traits
local StatCharacterDynamic = {
}


--- @class StatBase
--- "Fake" base class to avoid declaring attribute flags/talents/abilities twice
---
--- StatAttributeFlags
--- @field public FreezeImmunity boolean
--- @field public BurnImmunity boolean
--- @field public StunImmunity boolean
--- @field public PoisonImmunity boolean
--- @field public CharmImmunity boolean
--- @field public FearImmunity boolean
--- @field public KnockdownImmunity boolean
--- @field public MuteImmunity boolean
--- @field public ChilledImmunity boolean
--- @field public WarmImmunity boolean
--- @field public WetImmunity boolean
--- @field public BleedingImmunity boolean
--- @field public CrippledImmunity boolean
--- @field public BlindImmunity boolean
--- @field public CursedImmunity boolean
--- @field public WeakImmunity boolean
--- @field public SlowedImmunity boolean
--- @field public DiseasedImmunity boolean
--- @field public InfectiousDiseasedImmunity boolean
--- @field public PetrifiedImmunity boolean
--- @field public DrunkImmunity boolean
--- @field public SlippingImmunity boolean
--- @field public FreezeContact boolean
--- @field public BurnContact boolean
--- @field public StunContact boolean
--- @field public PoisonContact boolean
--- @field public ChillContact boolean
--- @field public Torch boolean
--- @field public Arrow boolean
--- @field public Unbreakable boolean
--- @field public Unrepairable boolean
--- @field public Unstorable boolean
--- @field public Grounded boolean
--- @field public HastedImmunity boolean
--- @field public TauntedImmunity boolean
--- @field public SleepingImmunity boolean
--- @field public AcidImmunity boolean
--- @field public SuffocatingImmunity boolean
--- @field public RegeneratingImmunity boolean
--- @field public DisarmedImmunity boolean
--- @field public DecayingImmunity boolean
--- @field public ClairvoyantImmunity boolean
--- @field public EnragedImmunity boolean
--- @field public BlessedImmunity boolean
--- @field public ProtectFromSummon boolean
--- @field public Floating boolean
--- @field public DeflectProjectiles boolean
--- @field public IgnoreClouds boolean
--- @field public MadnessImmunity boolean
--- @field public ChickenImmunity boolean
--- @field public IgnoreCursedOil boolean
--- @field public ShockedImmunity boolean
--- @field public WebImmunity boolean
--- @field public LootableWhenEquipped boolean
--- @field public PickpocketableWhenEquipped boolean
--- @field public LoseDurabilityOnCharacterHit boolean
--- @field public EntangledContact boolean
--- @field public ShacklesOfPainImmunity boolean
--- @field public MagicalSulfur boolean
--- @field public ThrownImmunity boolean
--- @field public InvisibilityImmunity boolean
---
--- Talents (prefix TALENT_)
--- @field public TALENT_None boolean
--- @field public TALENT_ItemMovement boolean
--- @field public TALENT_ItemCreation boolean
--- @field public TALENT_Flanking boolean
--- @field public TALENT_AttackOfOpportunity boolean
--- @field public TALENT_Backstab boolean
--- @field public TALENT_Trade boolean
--- @field public TALENT_Lockpick boolean
--- @field public TALENT_ChanceToHitRanged boolean
--- @field public TALENT_ChanceToHitMelee boolean
--- @field public TALENT_Damage boolean
--- @field public TALENT_ActionPoints boolean
--- @field public TALENT_ActionPoints2 boolean
--- @field public TALENT_Criticals boolean
--- @field public TALENT_IncreasedArmor boolean
--- @field public TALENT_Sight boolean
--- @field public TALENT_ResistFear boolean
--- @field public TALENT_ResistKnockdown boolean
--- @field public TALENT_ResistStun boolean
--- @field public TALENT_ResistPoison boolean
--- @field public TALENT_ResistSilence boolean
--- @field public TALENT_ResistDead boolean
--- @field public TALENT_Carry boolean
--- @field public TALENT_Throwing boolean
--- @field public TALENT_Repair boolean
--- @field public TALENT_ExpGain boolean
--- @field public TALENT_ExtraStatPoints boolean
--- @field public TALENT_ExtraSkillPoints boolean
--- @field public TALENT_Durability boolean
--- @field public TALENT_Awareness boolean
--- @field public TALENT_Vitality boolean
--- @field public TALENT_FireSpells boolean
--- @field public TALENT_WaterSpells boolean
--- @field public TALENT_AirSpells boolean
--- @field public TALENT_EarthSpells boolean
--- @field public TALENT_Charm boolean
--- @field public TALENT_Intimidate boolean
--- @field public TALENT_Reason boolean
--- @field public TALENT_Luck boolean
--- @field public TALENT_Initiative boolean
--- @field public TALENT_InventoryAccess boolean
--- @field public TALENT_AvoidDetection boolean
--- @field public TALENT_AnimalEmpathy boolean
--- @field public TALENT_Escapist boolean
--- @field public TALENT_StandYourGround boolean
--- @field public TALENT_SurpriseAttack boolean
--- @field public TALENT_LightStep boolean
--- @field public TALENT_ResurrectToFullHealth boolean
--- @field public TALENT_Scientist boolean
--- @field public TALENT_Raistlin boolean
--- @field public TALENT_MrKnowItAll boolean
--- @field public TALENT_WhatARush boolean
--- @field public TALENT_FaroutDude boolean
--- @field public TALENT_Leech boolean
--- @field public TALENT_ElementalAffinity boolean
--- @field public TALENT_FiveStarRestaurant boolean
--- @field public TALENT_Bully boolean
--- @field public TALENT_ElementalRanger boolean
--- @field public TALENT_LightningRod boolean
--- @field public TALENT_Politician boolean
--- @field public TALENT_WeatherProof boolean
--- @field public TALENT_LoneWolf boolean
--- @field public TALENT_Zombie boolean
--- @field public TALENT_Demon boolean
--- @field public TALENT_IceKing boolean
--- @field public TALENT_Courageous boolean
--- @field public TALENT_GoldenMage boolean
--- @field public TALENT_WalkItOff boolean
--- @field public TALENT_FolkDancer boolean
--- @field public TALENT_SpillNoBlood boolean
--- @field public TALENT_Stench boolean
--- @field public TALENT_Kickstarter boolean
--- @field public TALENT_WarriorLoreNaturalArmor boolean
--- @field public TALENT_WarriorLoreNaturalHealth boolean
--- @field public TALENT_WarriorLoreNaturalResistance boolean
--- @field public TALENT_RangerLoreArrowRecover boolean
--- @field public TALENT_RangerLoreEvasionBonus boolean
--- @field public TALENT_RangerLoreRangedAPBonus boolean
--- @field public TALENT_RogueLoreDaggerAPBonus boolean
--- @field public TALENT_RogueLoreDaggerBackStab boolean
--- @field public TALENT_RogueLoreMovementBonus boolean
--- @field public TALENT_RogueLoreHoldResistance boolean
--- @field public TALENT_NoAttackOfOpportunity boolean
--- @field public TALENT_WarriorLoreGrenadeRange boolean
--- @field public TALENT_RogueLoreGrenadePrecision boolean
--- @field public TALENT_WandCharge boolean
--- @field public TALENT_DualWieldingDodging boolean
--- @field public TALENT_Human_Inventive boolean
--- @field public TALENT_Human_Civil boolean
--- @field public TALENT_Elf_Lore boolean
--- @field public TALENT_Elf_CorpseEating boolean
--- @field public TALENT_Dwarf_Sturdy boolean
--- @field public TALENT_Dwarf_Sneaking boolean
--- @field public TALENT_Lizard_Resistance boolean
--- @field public TALENT_Lizard_Persuasion boolean
--- @field public TALENT_Perfectionist boolean
--- @field public TALENT_Executioner boolean
--- @field public TALENT_ViolentMagic boolean
--- @field public TALENT_QuickStep boolean
--- @field public TALENT_Quest_SpidersKiss_Str boolean
--- @field public TALENT_Quest_SpidersKiss_Int boolean
--- @field public TALENT_Quest_SpidersKiss_Per boolean
--- @field public TALENT_Quest_SpidersKiss_Null boolean
--- @field public TALENT_Memory boolean
--- @field public TALENT_Quest_TradeSecrets boolean
--- @field public TALENT_Quest_GhostTree boolean
--- @field public TALENT_BeastMaster boolean
--- @field public TALENT_LivingArmor boolean
--- @field public TALENT_Torturer boolean
--- @field public TALENT_Ambidextrous boolean
--- @field public TALENT_Unstable boolean
--- @field public TALENT_ResurrectExtraHealth boolean
--- @field public TALENT_NaturalConductor boolean
--- @field public TALENT_Quest_Rooted boolean
--- @field public TALENT_PainDrinker boolean
--- @field public TALENT_DeathfogResistant boolean
--- @field public TALENT_Sourcerer boolean
--- @field public TALENT_Rager boolean
--- @field public TALENT_Elementalist boolean
--- @field public TALENT_Sadist boolean
--- @field public TALENT_Haymaker boolean
--- @field public TALENT_Gladiator boolean
--- @field public TALENT_Indomitable boolean
--- @field public TALENT_WildMag boolean
--- @field public TALENT_Jitterbug boolean
--- @field public TALENT_Soulcatcher boolean
--- @field public TALENT_MasterThief boolean
--- @field public TALENT_GreedyVessel boolean
--- @field public TALENT_MagicCycles boolean
---
--- Abilities
--- @field public WarriorLore integer
--- @field public RangerLore integer
--- @field public RogueLore integer
--- @field public SingleHanded integer
--- @field public TwoHanded integer
--- @field public PainReflection integer
--- @field public Ranged integer
--- @field public Shield integer
--- @field public Reflexes integer
--- @field public PhysicalArmorMastery integer
--- @field public MagicArmorMastery integer
--- @field public VitalityMastery integer
--- @field public Sourcery integer
--- @field public FireSpecialist integer
--- @field public WaterSpecialist integer
--- @field public AirSpecialist integer
--- @field public EarthSpecialist integer
--- @field public Necromancy integer
--- @field public Summoning integer
--- @field public Polymorph integer
--- @field public Telekinesis integer
--- @field public Repair integer
--- @field public Sneaking integer
--- @field public Pickpocket integer
--- @field public Thievery integer
--- @field public Loremaster integer
--- @field public Crafting integer
--- @field public Barter integer
--- @field public Charm integer
--- @field public Intimidate integer
--- @field public Reason integer
--- @field public Persuasion integer
--- @field public Leadership integer
--- @field public Luck integer
--- @field public DualWielding integer
--- @field public Wand integer
--- @field public Perseverance integer
--- @field public Runecrafting integer
--- @field public Brewmaster integer
--- @field public Sulfurology integer
local StatBase = {}


--- @class StatCharacter : StatBase
--- Properties from PropertyMap
--- @field public Level integer
--- @field public Name string
--- @field public AIFlags integer
--- @field public CurrentVitality integer
--- @field public CurrentArmor integer
--- @field public CurrentMagicArmor integer
--- @field public ArmorAfterHitCooldownMultiplier integer
--- @field public MagicArmorAfterHitCooldownMultiplier integer
--- @field public MPStart integer
--- @field public CurrentAP integer
--- @field public BonusActionPoints integer
--- @field public Experience integer
--- @field public Reputation integer
--- @field public Flanked integer
--- @field public Karma integer
--- @field public MaxResistance integer
--- @field public HasTwoHandedWeapon integer
--- @field public IsIncapacitatedRefCount integer
--- @field public MaxVitality integer
--- @field public BaseMaxVitality integer
--- @field public MaxArmor integer
--- @field public BaseMaxArmor integer
--- @field public MaxMagicArmor integer
--- @field public BaseMaxMagicArmor integer
--- @field public Sight number
--- @field public BaseSight number
--- @field public MaxSummons integer
--- @field public BaseMaxSummons integer
--- @field public MaxMpOverride integer
---
--- StatCharacterFlags
--- @field public IsPlayer boolean
--- @field public InParty boolean
--- @field public IsSneaking boolean
--- @field public Invisible boolean
--- @field public Blind boolean
--- @field public DrinkedPotion boolean
--- @field public EquipmentValidated boolean
---
--- Properties from CDivinityStats_Character::GetStat
--- @field public PhysicalResistance integer
--- @field public PiercingResistance integer
--- @field public CorrosiveResistance integer
--- @field public MagicResistance integer
---
--- Base properties from CDivinityStats_Character::GetStat
--- @field public BasePhysicalResistance integer
--- @field public BasePiercingResistance integer
--- @field public BaseCorrosiveResistance integer
--- @field public BaseMagicResistance integer
---
--- Properties from CharacterStatsGetters::GetStat
--- @field public MaxMp integer
--- @field public APStart integer
--- @field public APRecovery integer
--- @field public APMaximum integer
--- @field public Strength integer
--- @field public Finesse integer
--- @field public Intelligence integer
--- @field public Constitution integer
--- @field public Memory integer
--- @field public Wits integer
--- @field public Accuracy integer
--- @field public Dodge integer
--- @field public CriticalChance integer
--- @field public FireResistance integer
--- @field public EarthResistance integer
--- @field public WaterResistance integer
--- @field public AirResistance integer
--- @field public PoisonResistance integer
--- @field public ShadowResistance integer
--- @field public CustomResistance integer
--- @field public LifeSteal integer
--- @field public Hearing integer
--- @field public Movement integer
--- @field public Initiative integer
--- @field public BlockChance integer
--- @field public ChanceToHitBoost integer
---
--- Base properties from CharacterStatsGetters::GetStat
--- @field public BaseMaxMp integer
--- @field public BaseAPStart integer
--- @field public BaseAPRecovery integer
--- @field public BaseAPMaximum integer
--- @field public BaseStrength integer
--- @field public BaseFinesse integer
--- @field public BaseIntelligence integer
--- @field public BaseConstitution integer
--- @field public BaseMemory integer
--- @field public BaseWits integer
--- @field public BaseAccuracy integer
--- @field public BaseDodge integer
--- @field public BaseCriticalChance integer
--- @field public BaseFireResistance integer
--- @field public BaseEarthResistance integer
--- @field public BaseWaterResistance integer
--- @field public BaseAirResistance integer
--- @field public BasePoisonResistance integer
--- @field public BaseShadowResistance integer
--- @field public BaseCustomResistance integer
--- @field public BaseLifeSteal integer
--- @field public BaseHearing integer
--- @field public BaseMovement integer
--- @field public BaseInitiative integer
--- @field public BaseBlockChance integer
--- @field public BaseChanceToHitBoost integer
---
--- Properties from CharacterFetchStat
--- @field public DynamicStats StatCharacterDynamic[]
--- @field public MainWeapon StatItem
--- @field public OffHandWeapon StatItem
--- @field public DamageBoost integer
--- @field public Character EsvCharacter
--- @field public Rotation number[]
--- @field public Position number[]
--- @field public MyGuid string
--- @field public NetID integer
local StatCharacter = {
    --- Returns the item equipped in the specified slot
    --- @param self StatCharacter
    --- @param slot string See Itemslot enumeration
    --- @return StatItem|nil
    GetItemBySlot = function (self, slot) end
}


--- @class StatItemDynamic : StatBase
--- @field public Durability integer
--- @field public DurabilityDegradeSpeed integer
--- @field public StrengthBoost integer
--- @field public FinesseBoost integer
--- @field public IntelligenceBoost integer
--- @field public ConstitutionBoost integer
--- @field public MemoryBoost integer
--- @field public WitsBoost integer
--- @field public SightBoost integer
--- @field public HearingBoost integer
--- @field public VitalityBoost integer
--- @field public SourcePointsBoost integer
--- @field public MaxAP integer
--- @field public StartAP integer
--- @field public APRecovery integer
--- @field public AccuracyBoost integer
--- @field public DodgeBoost integer
--- @field public LifeSteal integer
--- @field public CriticalChance integer
--- @field public ChanceToHitBoost integer
--- @field public MovementSpeedBoost integer
--- @field public RuneSlots integer
--- @field public RuneSlots_V1 integer
--- @field public FireResistance integer
--- @field public AirResistance integer
--- @field public WaterResistance integer
--- @field public EarthResistance integer
--- @field public PoisonResistance integer
--- @field public ShadowResistance integer
--- @field public PiercingResistance integer
--- @field public CorrosiveResistance integer
--- @field public PhysicalResistance integer
--- @field public MagicResistance integer
--- @field public CustomResistance integer
--- @field public Movement integer
--- @field public Initiative integer
--- @field public Willpower integer
--- @field public Bodybuilding integer
--- @field public MaxSummons integer
--- @field public Value integer
--- @field public Weight integer
--- @field public Skills string
--- @field public ItemColor string
--- @field public ModifierType integer
--- @field public ObjectInstanceName string
--- @field public BoostName string
--- @field public StatsType string See EquipmentStatsType enumeration
--- @field public DamageType integer Weapon only! - See DamageType enumeration
--- @field public MinDamage integer Weapon only!
--- @field public MaxDamage integer Weapon only!
--- @field public DamageBoost integer Weapon only!
--- @field public DamageFromBase integer Weapon only!
--- @field public CriticalDamage integer Weapon only!
--- @field public WeaponRange integer Weapon only!
--- @field public CleaveAngle integer Weapon only!
--- @field public CleavePercentage integer Weapon only!
--- @field public AttackAPCost integer Weapon only!
--- @field public ArmorValue integer Shield/Armor only!
--- @field public ArmorBoost integer Shield/Armor only!
--- @field public MagicArmorValue integer Shield/Armor only!
--- @field public MagicArmorBoost integer Shield/Armor only!
--- @field public Blocking integer Shield only!
--- TODO - add attribute flags object support
--- TODO - AbilityModifiers, Talents, Reflection
local StatItemDynamic = {}


--- @class StatProperty
--- @field Context string[] Target|Self|AoE|SelfOnHit|SelfOnEquip
--- @field Type string Custom|Status|SurfaceChange|GameAction|OsirisTask|Sabotage|Summon|Force
--- @field Condition string|nil
local StatProperty = {}

--- @class StatPropertyCustom : StatProperty
--- @field Type string Custom
--- @field Action string
local StatPropertyStatus = {}

--- @class StatPropertyStatus : StatProperty
--- @field Type string Status
--- @field Action string Status name
--- @field StatusChance number
--- @field Duration number
--- @field StatsId string
--- @field Arg4 integer
--- @field Arg5 integer
--- @field SurfaceBoost boolean
--- @field SurfaceBoosts string[] Labels from "Surface Type" enumeration
local StatPropertyStatus = {}

--- @class StatPropertySurfaceChange : StatProperty
--- @field Type string SurfaceChange
--- @field Action string Label from "Surface Change" enumeration
--- @field SurfaceChance number
--- @field Lifetime number
--- @field StatusChance number
--- @field Radius number
local StatPropertySurfaceChange = {}

--- @class StatPropertySabotage : StatProperty
--- @field Type string Sabotage
--- @field Amount integer
local StatPropertySabotage = {}

--- @class StatPropertySummon : StatProperty
--- @field Type string Summon
--- @field Template string
--- @field Duration number
--- @field IsTotem boolean
--- @field Skill string
local StatPropertySummon = {}

--- @class StatPropertyForce : StatProperty
--- @field Type string Force
--- @field Distance integer
local StatPropertyForce = {}

--- @class StatPropertyGameAction : StatProperty
--- @field Type string GameAction
--- @field Action string Label from "Game Action" enumeration
--- @field Arg1 number
--- @field Arg2 number
--- @field Arg3 string
--- @field Arg4 number
--- @field Arg5 number
--- @field StatusHealType string Label from "StatusHealType" enumeration
local StatPropertyGameAction = {}

--- @class StatPropertyOsirisTask : StatProperty
--- @field Type string OsirisTask
--- @field Action string Label from "Osiris Task" enumeration
--- @field Chance number
--- @field VitalityOnRevive integer
local StatPropertyOsirisTask = {}

--- @class StatPropertyExtender : StatProperty
--- @field Type string Extender
--- @field Action string Custom SkillProperties action name
--- @field Arg1 number
--- @field Arg2 number
--- @field Arg3 string
--- @field Arg4 integer
--- @field Arg5 integer
local StatPropertyExtender = {}


--- @class StatItem : StatBase
--- Properties from property map
--- @field public Level integer
--- @field public Name string
--- @field public InstanceId integer
--- @field public ItemType string See EquipmentStatsType enumeration
--- @field public ItemSlot string See ItemSlot enumeration
--- @field public WeaponType string See WeaponType enumeration
--- @field public AnimType integer TODO maybe an enum?
--- @field public WeaponRange integer
--- @field public IsIdentified integer
--- @field public IsTwoHanded boolean
--- @field public ShouldSyncStats boolean
--- @field public HasModifiedSkills boolean
--- @field public Skills string
--- @field public DamageTypeOverwrite string See DamageType enumeration
--- @field public Durability integer
--- @field public DurabilityCounter integer
--- @field public ItemTypeReal string
--- @field public MaxCharges integer
--- @field public Charges integer
---
--- Properties from ItemFetchStat
--- @field public DynamicStats StatItemDynamic[]
---
--- Indirect properties from StatEntryWeapon/Armor/Shield
--- @field public Using string
--- @field public Damage integer
--- @field public Act string See Act enumeration
--- @field public Handedness string See Handedness enumeration
--- @field public DamageBoost integer
--- @field public DamageFromBase integer
--- @field public CriticalDamage integer
--- @field public CriticalChance integer
--- @field public Movement integer
--- @field public Initiative integer
--- @field public Requirements StatRequirement[]
--- @field public Slot string See Itemslot enumeration
--- @field public DurabilityDegradeSpeed string See Qualifier enumeration
--- @field public Value integer
--- @field public ModifierType string See ModifierType enumeration
--- @field public Projectile string
--- @field public StrengthBoost string See Penalty Qualifier enumeration
--- @field public FinesseBoost string See Penalty Qualifier enumeration
--- @field public IntelligenceBoost string See Penalty Qualifier enumeration
--- @field public ConstitutionBoost string See Penalty Qualifier enumeration
--- @field public MemoryBoost string See Penalty Qualifier enumeration
--- @field public WitsBoost string See Penalty Qualifier enumeration
--- @field public SingleHanded integer
--- @field public TwoHanded integer
--- @field public Ranged integer
--- @field public DualWielding integer
--- @field public RogueLore integer
--- @field public WarriorLore integer
--- @field public RangerLore integer
--- @field public FireSpecialist integer
--- @field public WaterSpecialist integer
--- @field public AirSpecialist integer
--- @field public EarthSpecialist integer
--- @field public Sourcery integer
--- @field public Necromancy integer
--- @field public Polymorph integer
--- @field public Summoning integer
--- @field public Leadership integer
--- @field public PainReflection integer
--- @field public Perseverance integer
--- @field public Telekinesis integer
--- @field public Sneaking integer
--- @field public Thievery integer
--- @field public Loremaster integer
--- @field public Repair integer
--- @field public Barter integer
--- @field public Persuasion integer
--- @field public Luck integer
--- @field public Fire integer
--- @field public Earth integer
--- @field public Water integer
--- @field public Air integer
--- @field public Poison integer
--- @field public Physical integer
--- @field public Piercing integer
--- @field public SightBoost string See Penalty Qualifier enumeration
--- @field public HearingBoost string See Penalty Qualifier enumeration
--- @field public VitalityBoost integer
--- @field public MagicPointsBoost string See Penalty Qualifier enumeration
--- @field public ChanceToHitBoost integer
--- @field public APMaximum integer
--- @field public APStart integer
--- @field public APRecovery integer
--- @field public AccuracyBoost integer
--- @field public DodgeBoost integer
--- @field public Weight integer
--- @field public AttackAPCost integer
--- @field public ComboCategory string
--- @field public Flags string See AttributeFlags enumeration
--- @field public Boosts string
--- @field public InventoryTab string See InventoryTabs enumeration
--- @field public Reflection string
--- @field public ItemGroup string
--- @field public ObjectCategory string
--- @field public MinAmount integer
--- @field public MaxAmount integer
--- @field public Priority integer
--- @field public Unique integer
--- @field public MinLevel integer
--- @field public MaxLevel integer
--- @field public ItemColor string
--- @field public MaxSummons integer
--- @field public RuneSlots integer
--- @field public RuneSlots_V1 integer
--- @field public NeedsIdentification string See YesNo enumeration
--- @field public LifeSteal integer
--- @field public CleavePercentage integer
--- @field public CleaveAngle integer
--- @field public Talents string
--- @field public IgnoreVisionBlock string See YesNo enumeration
--- @field public Tags string
--- @field public ArmorBoost integer
--- @field public MagicArmorBoost integer
--- @field public Blocking integer
--- @field public ExtraProperties StatProperty[]
local StatItem = {}


--- @class EclGameObject
--- @field public NetID integer
--- @field public MyGuid string
--- TODO - Add more properties via virtual getters?
--- (IsGlobal, GetComponentType, GetFlags, GetTags, T/R/S, Velocity, Height, Level?)
local EclGameObject = {}


--- @class EclItem : EclGameObject
--- @field public RootTemplate ItemTemplate
--- @field public Stats StatItem
--- @field public Handle ObjectHandle
---
--- @field public WorldPos number[]
--- @field public CurrentLevel string
--- @field public Scale number
--- @field public StatsId string
--- @field public Weight integer
--- @field public KeyName string
--- @field public Level integer
--- @field public ItemType string
--- @field public GoldValueOverride integer
--- @field public BaseWeightOverwrite integer
--- @field public ItemColorOverride integer
--- @field public DisplayName string
local EclItem = {}

--- Returns the GUID of all items within the inventory of the item
--- @param self EclItem
--- @return string[]
function EclItem.GetInventoryItems (self) end

--- Returns the GUID of character that currently owns the item; nil if the item is not in a character inventory
--- @param self EclItem
--- @return string|nil
function EclItem.GetOwnerCharacter (self) end

--- Returns whether the item has the specified tag
--- @param self EclItem
--- @param tag string
--- @return boolean
function EclItem.HasTag (self, tag) end

--- Returns all tags on the item
--- @param self EclItem
--- @return string[]
function EclItem.GetTags (self) end

--- Returns the first status with the specified status ID, if one exists.
--- @param self EclItem
--- @param statusId string Status ID
--- @return EclStatus|nil
function EclItem.GetStatus (self, statusId) end

--- Returns the first status with the specified engine status type, if one exists.
--- @param self EclItem
--- @param type string Status type
--- @return EclStatus|nil
function EclItem.GetStatusByType (self, type) end

--- Returns all statuses on the item
--- @param self EclItem
--- @return string[]
function EclItem.GetStatuses (self) end

--- Returns all statuses on the item
--- @param self EclItem
--- @return EclStatus[]
function EclItem.GetStatusObjects (self) end



--- @class EclCharacter : EclGameObject
--- @field public RootTemplate CharacterTemplate
--- @field public PlayerCustomData PlayerCustomData
--- @field public Stats StatCharacter
--- @field public Handle ObjectHandle
---
--- @field public WorldPos number[]
--- @field public CurrentLevel string
--- @field public Scale number
--- @field public AnimationOverride string
--- @field public UserID integer
--- @field public DisplayName string
--- @field public StoryDisplayName string
--- @field public OriginalDisplayName string
--- @field public WalkSpeedOverride number
--- @field public RunSpeedOverride number
--- @field public Archetype string
--- @field public CorpseLootable boolean
local EclCharacter = {}

--- Returns the GUID of all items within the inventory of the character
--- @param self EclCharacter
--- @return string[]
function EclCharacter.GetInventoryItems (self) end

--- Returns the item equipped in the specified slot
--- @param self EclCharacter
--- @param slot string See ItemSlot enumeration
--- @return EclItem|nil
function EclCharacter.GetItemBySlot (self, slot) end

--- Returns whether the character has the specified tag
--- @param self EclCharacter
--- @param tag string
--- @return boolean
function EclCharacter.HasTag (self, tag) end

--- Returns all tags on the character
--- @param self EclCharacter
--- @return string[]
function EclCharacter.GetTags (self) end

--- Returns the first status with the specified status ID, if one exists.
--- @param self EclCharacter
--- @param statusId string Status ID
--- @return EclStatus|nil
function EclCharacter.GetStatus (self, statusId) end

--- Returns the first status with the specified engine status type, if one exists.
--- @param self EclCharacter
--- @param type string Status type
--- @return EclStatus|nil
function EclCharacter.GetStatusByType (self, type) end

--- Returns all statuses on the character
--- @param self EclCharacter
--- @return string[]
function EclCharacter.GetStatuses (self) end

--- Returns all statuses on the character
--- @param self EclCharacter
--- @return EclStatus[]
function EclCharacter.GetStatusObjects (self) end

--- Update model scale of the character.
--- @param self EclCharacter
--- @param scale number 
function EclCharacter.SetScale (self, scale) end



--- @class EsvGameObject
--- @field public NetID integer
--- @field public MyGuid string
--- TODO - Add more properties via virtual getters?
--- (IsGlobal, GetComponentType, GetFlags, GetTags, T/R/S, Velocity, Height, Level?)
local EsvGameObject = {}


--- @class EsvItem : EsvGameObject
--- @field public Handle ObjectHandle
--- @field public RootTemplate ItemTemplate
--- @field public WorldPos number[]
--- @field public CurrentLevel string
--- @field public Scale number
--- @field public CustomDisplayName string
--- @field public CustomDescription string
--- @field public CustomBookContent string
--- @field public StatsId string
--- @field public InventoryHandle ObjectHandle
--- @field public ParentInventoryHandle ObjectHandle
--- @field public Slot integer
--- @field public Amount integer
--- @field public Vitality integer
--- @field public Armor integer
--- @field public InUseByCharacterHandle ObjectHandle
--- @field public Key string
--- @field public LockLevel integer
--- @field public OwnerHandle ObjectHandle
--- @field public ItemType string
--- @field public GoldValueOverwrite integer
--- @field public WeightValueOverwrite integer
--- @field public TreasureLevel integer
--- @field public LevelOverride integer
--- @field public ForceSynch boolean
--- @field public DisplayName string
--- From ItemFlags
--- @field public Activated boolean
--- @field public OffStage boolean
--- @field public CanBePickedUp boolean
--- @field public CanBeMoved boolean
--- @field public WalkOn boolean
--- @field public WalkThrough boolean
--- @field public NoCover boolean
--- @field public CanShootThrough boolean
--- @field public CanUse boolean
--- @field public InteractionDisabled boolean
--- @field public Destroyed boolean
--- @field public LoadedTemplate boolean
--- @field public IsDoor boolean
--- @field public StoryItem boolean
--- @field public Summon boolean
--- @field public FreezeGravity boolean
--- @field public ForceSync boolean
--- @field public IsLadder boolean
--- @field public PositionChanged boolean
--- @field public Totem boolean
--- @field public Destroy boolean
--- @field public GMFolding boolean
--- @field public Sticky boolean
--- @field public DoorFlag boolean
--- @field public Floating boolean
--- @field public IsSurfaceBlocker boolean
--- @field public IsSurfaceCloudBlocker boolean
--- @field public SourceContainer boolean
--- @field public Frozen boolean
--- @field public TeleportOnUse boolean
--- @field public PinnedContainer boolean
--- From ItemFlags2
--- @field public UnsoldGenerated boolean
--- @field public IsKey boolean
--- @field public Global boolean
--- @field public CanConsume boolean
--- @field public TreasureGenerated boolean
--- @field public UnEquipLocked boolean
--- @field public UseRemotely boolean
--- 
--- @field public Stats StatItem
local EsvItem = {}

--- Returns all boosts that were added during treasure generation
--- @param self EsvItem
--- @return string[]
function EsvItem.GetGeneratedBoosts (self) end

--- Returns the GUID of all items within the inventory of the item
--- @param self EsvItem
--- @return string[]
function EsvItem.GetInventoryItems (self) end

--- Returns the GUID of all characters within the specified radius
--- @param self EsvItem
--- @return string[]
function EsvItem.GetNearbyCharacters (self, radius) end

--- Returns whether the item has the specified tag
--- @param self EsvItem
--- @param tag string
--- @return boolean
function EsvItem.HasTag (self, tag) end

--- Returns all tags on the item
--- @param self EsvItem
--- @return string[]
function EsvItem.GetTags (self) end

--- Returns the first status with the specified status ID, if one exists.
--- @param self EsvItem
--- @param statusId string Status ID
--- @return EsvStatus|nil
function EsvItem.GetStatus (self, statusId) end

--- Returns the first status with the specified engine status type, if one exists.
--- @param self EsvItem
--- @param type string Status type
--- @return EsvStatus|nil
function EsvItem.GetStatusByType (self, type) end

--- Returns all statuses on the item
--- @param self EsvItem
--- @return string[]
function EsvItem.GetStatuses (self) end

--- Returns all statuses on the character
--- @param self EclItem
--- @return EsvStatus[]
function EsvItem.GetStatusObjects (self) end




--- @class ItemDefinition
--- @field public RootTemplate string
--- @field public OriginalRootTemplate string
--- @field public Slot integer
--- @field public Amount integer
--- @field public GoldValueOverwrite integer
--- @field public WeightValueOverwrite integer
--- @field public DamageTypeOverwrite string DamageType enumeration
--- @field public ItemType string
--- @field public CustomDisplayName string
--- @field public CustomDescription string
--- @field public CustomBookContent string
--- @field public GenerationStatsId string
--- @field public GenerationItemType string
--- @field public GenerationRandom integer
--- @field public GenerationLevel integer
--- @field public StatsLevel integer
--- @field public Key string
--- @field public LockLevel integer
--- @field public EquipmentStatsType integer
--- @field public HasModifiedSkills boolean
--- @field public Skills string
--- @field public HasGeneratedStats boolean
--- @field public IsIdentified boolean
--- @field public GMFolding boolean
--- @field public CanUseRemotely boolean
--- @field public GenerationBoosts string[]
--- @field public RuneBoosts string[]
local ItemDefinition = {}

--- Clears item progression data (name group, level group, etc.)
--- @param self ItemDefinition
function ItemDefinition.ResetProgression (self) end



--- @class ItemConstructor
--- Creates an item based on a parsed item or newly created item definition.
--- Should be initialized using Ext.CreateItemConstructor(item/template) first.
--- Item definitions can be accessed using c[1], c[2], etc.
--- For non-recursive item cloning (i.e. creating a single item), there is only one item (c[1]).
--- For container cloning, the contained items are accessible using c[2], c[3], etc.
local ItemConstructor = {}

--- Constructs an instance of the item contained in the constructor definition.
--- The definition is cleared after the item is created.
--- @param self ItemConstructor
--- @return EsvItem|nil
function ItemConstructor.Construct (self) end



--- @class PlayerCustomData
--- @field public CustomLookEnabled boolean
--- @field public Name string
--- @field public ClassType string
--- @field public SkinColor integer
--- @field public HairColor integer
--- @field public ClothColor1 integer
--- @field public ClothColor2 integer
--- @field public ClothColor3 integer
--- @field public IsMale boolean
--- @field public Race string
--- @field public OriginName string
--- @field public Icon string
--- @field public MusicInstrument string
--- @field public OwnerProfileID string
--- @field public ReservedProfileID string
--- @field public AiPersonality string
--- @field public Speaker string
local PlayerCustomData = {
}


--- @class EsvSkillInfo
--- @field public ActiveCooldown number
--- @field public IsActivated boolean
--- @field public IsLearned boolean
--- @field public ZeroMemory boolean
--- @field public OncePerCombat boolean
--- @field public NumCharges number
local EsvSkillInfo = {}


--- @class EsvCharacter : EsvGameObject
--- @field public RootTemplate CharacterTemplate
--- @field public PlayerCustomData PlayerCustomData
--- @field public Stats StatCharacter
--- @field public DisplayName string
--- @field public Handle ObjectHandle
---
--- @field public NetID integer
--- @field public MyGuid string
--- @field public WorldPos number[]
--- @field public CurrentLevel string
--- @field public Scale number
--- @field public AnimationOverride string
--- @field public WalkSpeedOverride number
--- @field public RunSpeedOverride number
--- @field public NeedsUpdateCount integer
--- @field public ScriptForceUpdateCount integer
--- @field public ForceSynchCount integer
--- @field public InventoryHandle ObjectHandle Not useful yet as you can't use inventory handles for any API
--- @field public SkillBeingPrepared string Set while preparing a skill, cleared afterwards
--- @field public LifeTime number Used for summons to indicate remaining lifetime
--- @field public TurnTimer number
--- @field public TriggerTrapsTimer number
--- @field public UserID integer
--- @field public ReservedUserID integer
--- @field public OwnerHandle ObjectHandle Handle of owner character (in case of summons/followers)
--- @field public FollowCharacterHandle ObjectHandle
--- @field public SpiritCharacterHandle ObjectHandle
--- @field public CorpseCharacterHandle ObjectHandle
--- @field public PartialAP number Movement AP
--- @field public AnimType integer
--- @field public DelayDeathCount integer
--- @field public AnimationSetOverride string
--- @field public OriginalTransformDisplayName string
--- @field public PartyHandle ObjectHandle
--- @field public CustomTradeTreasure string
--- @field public IsAlarmed boolean
--- @field public CrimeWarningsEnabled boolean
--- @field public CrimeInterrogationEnabled boolean
--- @field public MovingCasterHandle ObjectHandle
--- @field public Archetype string
--- @field public EquipmentColor string
--- @field public ProjectileTemplate string
--- @field public ReadyCheckBlocked boolean
--- @field public CorpseLootable boolean
--- @field public CustomBloodSurface string
--- @field public PreviousLevel string
---
--- CharacterFlags 0
--- @field public IsPlayer boolean
--- @field public Multiplayer boolean
--- @field public InParty boolean
--- @field public HostControl boolean
--- @field public Activated boolean
--- @field public OffStage boolean
--- @field public Dead boolean
--- @field public HasOwner boolean
--- @field public InDialog boolean
--- @field public Summon boolean
--- @field public CannotDie boolean
--- @field public CharacterControl boolean
--- @field public Loaded boolean
--- @field public InArena boolean
--- @field public CharacterCreationFinished boolean
--- @field public Floating boolean
--- @field public SpotSneakers boolean
--- @field public Temporary boolean
--- @field public WalkThrough boolean
--- @field public CoverAmount boolean
--- @field public CanShootThrough boolean
--- @field public PartyFollower boolean
--- @field public Totem boolean
--- @field public NoRotate boolean
--- @field public Deactivated boolean
--- @field public IsHuge boolean
--- @field public MadePlayer boolean
--- @field public LevelTransitionPending boolean
--- @field public RegisteredForAutomatedDialog boolean
---
--- CharacterFlags 2
--- @field public Global boolean
--- @field public HasOsirisDialog boolean
--- @field public HasDefaultDialog boolean
--- @field public TreasureGeneratedForTrader boolean
--- @field public Trader boolean
--- @field public Resurrected boolean
---
--- CharacterFlags 3
--- @field public IsPet boolean
--- @field public IsSpectating boolean
--- @field public NoReptuationEffects boolean
--- @field public HasWalkSpeedOverride boolean
--- @field public HasRunSpeedOverride boolean
--- @field public IsGameMaster boolean
--- @field public IsPossessed boolean
--- @field public ManuallyLeveled boolean
local EsvCharacter = {}

--- Returns the GUID of all items in the characters inventory
--- @param self EsvCharacter
--- @return string[]
function EsvCharacter.GetInventoryItems (self) end

--- Returns detailed information about the specified skill
--- @param self EsvCharacter
--- @param skillId string
--- @return EsvSkillInfo
function EsvCharacter.GetSkillInfo (self, skillId) end

--- Returns the name of all skills available to the character
--- @param self EsvCharacter
--- @return string[]
function EsvCharacter.GetSkills (self) end

--- Returns the GUID of all characters within the specified radius
--- @param self EsvCharacter
--- @return string[]
function EsvCharacter.GetNearbyCharacters (self, radius) end

--- Returns the GUID of all summons owned by the character
--- @param self EsvCharacter
--- @return string[]
function EsvCharacter.GetSummons (self) end

--- Returns whether the character has the specified tag
--- @param self EsvCharacter
--- @param tag string
--- @return boolean
function EsvCharacter.HasTag (self, tag) end

--- Returns all tags on the character
--- @param self EsvCharacter
--- @return string[]
function EsvCharacter.GetTags (self) end

--- Returns the first status with the specified status ID, if one exists.
--- @param self EsvCharacter
--- @param statusId string Status ID
--- @return EsvStatus|nil
function EsvCharacter.GetStatus (self, statusId) end

--- Returns the first status with the specified engine status type, if one exists.
--- @param self EsvCharacter
--- @param type string Status type
--- @return EsvStatus|nil
function EsvCharacter.GetStatusByType (self, type) end

--- Returns all statuses on the character
--- @param self EsvCharacter
--- @return string[]
function EsvCharacter.GetStatuses (self) end

--- Returns all statuses on the character
--- @param self EsvCharacter
--- @return EsvStatus[]
function EsvCharacter.GetStatusObjects (self) end

--- Update model scale of the character.
--- NOTE: This change must be manually synchronized to the client!
--- @param self EsvCharacter
--- @param scale number 
function EsvCharacter.SetScale (self, scale) end



--- @class EsvSurface
--- @field public NetId integer
--- @field public MyHandle ObjectHandle
--- @field public SurfaceType string See SurfaceType enumeration
--- @field public RootTemplate SurfaceTemplate
--- @field public Flags integer
--- @field public TeamId integer
--- @field public OwnerHandle ObjectHandle
--- @field public LifeTime number
--- @field public LifeTimeFromTemplate boolean
--- @field public StatusChance number
--- @field public Index integer
--- @field public OwnershipTimer number


--- @class EsvShootProjectileRequest
--- @field public SkillId string
--- @field public Caster ObjectHandle
--- @field public Source ObjectHandle
--- @field public Target ObjectHandle
--- @field public StartPosition number[]
--- @field public EndPosition number[]
--- @field public Random integer
--- @field public CasterLevel integer
--- @field public IsTrap boolean
--- @field public UnknownFlag1 boolean
--- @field public CleanseStatuses string
--- @field public StatusClearChance integer
--- @field public IsFromItem boolean
--- @field public IsStealthed boolean
--- @field public IgnoreObjects boolean
--- TODO - DamageList, HitObject
local EsvShootProjectileRequest = {}


--- @class EsvProjectile : EsvGameObject
--- @field public RootTemplate ProjectileTemplate
--- @field public Handle ObjectHandle
--- @field public NetID integer
--- @field public MyGuid string
--- @field public CasterHandle ObjectHandle
--- @field public SourceHandle ObjectHandle
--- @field public TargetObjectHandle ObjectHandle
--- @field public HitObjectHandle ObjectHandle
--- @field public SourcePosition number[]
--- @field public TargetPosition number[]
--- @field public DamageType string
--- @field public DamageSourceType string
--- @field public LifeTime number
--- @field public HitInterpolation integer
--- @field public ExplodeRadius0 number
--- @field public ExplodeRadius1 number
--- @field public DeathType string
--- @field public SkillId string
--- @field public WeaponHandle ObjectHandle
--- @field public MovingEffectHandle ObjectHandle
--- @field public SpawnEffect string
--- @field public SpawnFXOverridesImpactFX boolean
--- @field public EffectHandle string
--- @field public RequestDelete boolean
--- @field public Launched boolean
--- @field public IsTrap boolean
--- @field public UseCharacterStats boolean
--- @field public ReduceDurability boolean
--- @field public AlwaysDamage boolean
--- @field public ForceTarget boolean
--- @field public IsFromItem boolean
--- @field public DivideDamage boolean
--- @field public IgnoreRoof boolean
--- @field public CanDeflect boolean
--- @field public IgnoreObjects boolean
--- @field public CleanseStatuses string
--- @field public StatusClearChance integer
--- @field public Position number[]
--- @field public PrevPosition number[]
--- @field public Velocity number[]
--- @field public Scale number
--- @field public CurrentLevel string
local EsvProjectile = {}


--- @class EsvSurfaceAction
--- @field public MyHandle ObjectHandle
local EsvSurfaceAction = {}


--- @class EsvCreateSurfaceActionBase : EsvSurfaceAction
--- @field public OwnerHandle ObjectHandle
--- @field public Duration number
--- @field public StatusChance number
--- @field public Position number[]
--- @field public SurfaceType string
local EsvCreateSurfaceActionBase = {}


--- @class EsvCreateSurfaceAction : EsvCreateSurfaceActionBase
--- @field public Radius number
--- @field public ExcludeRadius number
--- @field public MaxHeight number
--- @field public IgnoreIrreplacableSurfaces boolean
--- @field public CheckExistingSurfaces boolean
--- @field public SurfaceCollisionFlags integer
--- @field public SurfaceCollisionNotOnFlags integer
--- @field public Timer number
--- @field public GrowTimer number
--- @field public GrowStep integer
--- @field public SurfaceLayer integer
local EsvCreateSurfaceAction = {}


--- @class EsvChangeSurfaceOnPathAction : EsvCreateSurfaceActionBase
--- @field public FollowObject ObjectHandle
--- @field public Radius number
--- @field public IgnoreIrreplacableSurfaces boolean
--- @field public CheckExistingSurfaces boolean
--- @field public SurfaceCollisionFlags integer
--- @field public SurfaceCollisionNotOnFlags integer
--- @field public IgnoreOwnerCells boolean
local EsvChangeSurfaceOnPathAction = {}


--- @class EsvCreatePuddleAction : EsvCreateSurfaceActionBase
--- @field public SurfaceCells integer
--- @field public Step integer
--- @field public GrowSpeed number
--- @field public IgnoreIrreplacableSurfaces boolean
--- @field public GrowTimer number
local EsvCreatePuddleAction = {}


--- @class EsvExtinguishFireAction : EsvCreateSurfaceActionBase
--- @field public Position number[]
--- @field public Radius number
--- @field public Percentage number
--- @field public GrowTimer number
--- @field public Step number
local EsvExtinguishFireAction = {}


--- @class EsvRectangleSurfaceAction : EsvCreateSurfaceActionBase
--- @field public DamageList DamageList
--- @field public Target number[]
--- @field public SurfaceArea number
--- @field public Width number
--- @field public Length number
--- @field public GrowTimer number
--- @field public MaxHeight number
--- @field public GrowStep integer
--- @field public AiFlags integer
--- @field public DeathType string
--- @field public LineCheckBlock integer
local EsvRectangleSurfaceAction = {}


--- @class EsvPolygonSurfaceAction : EsvCreateSurfaceActionBase
--- @field public DamageList DamageList
--- @field public Vertices number[][]
--- @field public PositionX number
--- @field public PositionZ number
--- @field public GrowTimer number
--- @field public GrowStep integer
local EsvPolygonSurfaceAction = {}


--- @class EsvSwapSurfaceAction : EsvCreateSurfaceActionBase
--- @field public Radius number
--- @field public ExcludeRadius number
--- @field public MaxHeight number
--- @field public Target number[]
--- @field public IgnoreIrreplacableSurfaces boolean
--- @field public CheckExistingSurfaces boolean
--- @field public SurfaceCollisionFlags integer
--- @field public SurfaceCollisionNotOnFlags integer
--- @field public LineCheckBlock integer
--- @field public GrowTimer number
--- @field public GrowStep integer
local EsvSwapSurfaceAction = {}


--- @class EsvZoneAction : EsvCreateSurfaceActionBase
--- @field public SkillId string
--- @field public DamageList DamageList
--- @field public Target number[]
--- @field public Shape integer
--- @field public Radius number
--- @field public AngleOrBase number
--- @field public BackStart number
--- @field public FrontOffset number
--- @field public MaxHeight number
--- @field public GrowTimer number
--- @field public GrowStep integer
--- @field public AiFlags integer
--- @field public DeathType string
local EsvZoneAction = {}


--- @class EsvTransformSurfaceAction : EsvSurfaceAction
--- @field public SurfaceTransformAction string
--- @field public OriginSurface string
--- @field public SurfaceLayer integer
--- @field public GrowCellPerSecond number
--- @field public OwnerHandle2 ObjectHandle
--- @field public Position number[]
--- @field public SurfaceLifetime number
--- @field public SurfaceStatusChance number
local EsvTransformSurfaceAction = {}


--- @class ModInfo
--- @field public UUID string
--- @field public Name string
--- @field public Version integer
--- @field public PublishVersion integer
--- @field public Directory string
--- @field public Author string
--- @field public Description string
--- @field public ModuleType string
--- @field public Dependencies string[] Dependency mod UUID-s
local ModInfo = {}


--- @class CustomSkillProperty
--- @field public GetDescription fun(property:StatPropertyExtender):string|nil
--- @field public ExecuteOnPosition fun(property:StatPropertyExtender, attacker: EsvGameObject, position: number[], areaRadius: number, isFromItem: boolean, skill: StatEntrySkillData|nil, hit: HitRequest|nil)
--- @field public ExecuteOnTarget fun(property:StatPropertyExtender, attacker: EsvGameObject, target: EsvGameObject, position: number[], isFromItem: boolean, skill: StatEntrySkillData|nil, hit: HitRequest|nil)


--- @class StatSkillSet
--- @field public Name string
--- @field public Skills string[]

--- @class StatEquipmentGroup
--- @field public Name string
--- @field public Equipment string[]

--- @class StatEquipmentSet
--- @field public Name string
--- @field public Groups StatEquipmentGroup[]


--- @class ItemComboIngredients
--- @field public Object string
--- @field public IngredientType string See IngredientType enumeration
--- @field public Transform string See IngredientTransformType enumeration
--- @field public ItemRarity string See ItemDataRarity enumeration (ValueLists.txt only!)
local ItemComboIngredients = {}

--- @class ItemComboResultElement
--- @field public Result string
--- @field public Boost string
--- @field public ResultAmount number
local ItemComboResultElement = {}

--- @class ItemComboResult
--- @field public Requirement string See Ability enum
--- @field public ReqLevel number
--- @field public PreviewStatsId string
--- @field public PreviewIcon string
--- @field public PreviewTooltip string
--- @field public Name string
--- @field public Results ItemComboResultElement[]
local ItemComboResult = {}

--- @class ItemCombo
--- @field public Name string
--- @field public RecipeCategory string See RecipeCategory enum
--- @field public CraftingStation string See CraftingStationType enum
--- @field public Ingredients ItemComboIngredients[]
--- @field public Results ItemComboResult[]
local ItemCombo = {}

--- @class ItemComboPreviewData
--- @field public Name string
--- @field public Type string
--- @field public StatsId string
--- @field public Tooltip string
--- @field public Icon string
local ItemComboPreviewData = {}

--- @class ItemComboPropertyElement
--- @field public ObjectId string
--- @field public IngredientType string See IngredientType enumeration
--- @field public Result string
local ItemComboPropertyElement = {}

--- @class ItemComboProperty
--- @field public Name string
--- @field public PreviewIcon string
--- @field public PreviewTooltip string
--- @field public Entries ItemComboPropertyElement[]
local ItemComboProperty = {}


--- @class ItemNameGroupLink
--- @field public NameGroup string
--- @field public NoneCoolSuffix number
--- @field public ItemName string
local ItemNameGroupLink = {}

--- @class ItemRootGroup
--- @field public MinLevel number
--- @field public MaxLevel number
--- @field public RootGroup string
--- @field public NameGroupLinks ItemNameGroupLink[]
local ItemRootGroup = {}

--- @class ItemLevelGroup
--- @field public MinLevel number
--- @field public MaxLevel number
--- @field public Name string
--- @field public RootGroups ItemRootGroup[]
local ItemLevelGroup = {}

--- @class ItemGroup
--- @field public Name string
--- @field public LevelGroups ItemLevelGroup[]
local ItemGroup = {}

--- @class ItemNameGroupName
--- @field public Name string
--- @field public Name2 string
local ItemNameGroupName = {}

--- @class ItemNameGroup
--- @field public Name string
--- @field public Names ItemNameGroupName[]
--- @field public NamesCool ItemNameGroupName[]
local ItemNameGroup = {}


--- @class StatRequirement
--- @field public Requirement string
--- @field public Param string|integer
--- @field public Not boolean
local StatRequirement = {}


--- @class StatEntryWeapon
--- @field public Name string
--- @field public Level integer
--- @field public Using string
--- @field public Damage integer
--- @field public Act string See Act enumeration
--- @field public Handedness string See Handedness enumeration
--- @field public IsTwoHanded string See YesNo enumeration
--- @field public DamageBoost integer
--- @field public DamageFromBase integer
--- @field public CriticalDamage integer
--- @field public CriticalChance integer
--- @field public Movement integer
--- @field public Initiative integer
--- @field public Requirements StatRequirement[]
--- @field public Slot string See Itemslot enumeration
--- @field public Durability integer
--- @field public DurabilityDegradeSpeed string See Qualifier enumeration
--- @field public Value integer
--- @field public WeaponType string See WeaponType enumeration
--- @field public AnimType string See AnimType enumeration
--- @field public WeaponRange integer
--- @field public ModifierType string See ModifierType enumeration
--- @field public Projectile string
--- @field public StrengthBoost string See Penalty Qualifier enumeration
--- @field public FinesseBoost string See Penalty Qualifier enumeration
--- @field public IntelligenceBoost string See Penalty Qualifier enumeration
--- @field public ConstitutionBoost string See Penalty Qualifier enumeration
--- @field public MemoryBoost string See Penalty Qualifier enumeration
--- @field public WitsBoost string See Penalty Qualifier enumeration
--- @field public SingleHanded integer
--- @field public TwoHanded integer
--- @field public Ranged integer
--- @field public DualWielding integer
--- @field public RogueLore integer
--- @field public WarriorLore integer
--- @field public RangerLore integer
--- @field public FireSpecialist integer
--- @field public WaterSpecialist integer
--- @field public AirSpecialist integer
--- @field public EarthSpecialist integer
--- @field public Sourcery integer
--- @field public Necromancy integer
--- @field public Polymorph integer
--- @field public Summoning integer
--- @field public Leadership integer
--- @field public PainReflection integer
--- @field public Perseverance integer
--- @field public Telekinesis integer
--- @field public Sneaking integer
--- @field public Thievery integer
--- @field public Loremaster integer
--- @field public Repair integer
--- @field public Barter integer
--- @field public Persuasion integer
--- @field public Luck integer
--- @field public Fire integer
--- @field public Earth integer
--- @field public Water integer
--- @field public Air integer
--- @field public Poison integer
--- @field public Physical integer
--- @field public Piercing integer
--- @field public SightBoost string See Penalty Qualifier enumeration
--- @field public HearingBoost string See Penalty Qualifier enumeration
--- @field public VitalityBoost integer
--- @field public MagicPointsBoost string See Penalty Qualifier enumeration
--- @field public ChanceToHitBoost integer
--- @field public APMaximum integer
--- @field public APStart integer
--- @field public APRecovery integer
--- @field public AccuracyBoost integer
--- @field public DodgeBoost integer
--- @field public Weight integer
--- @field public AttackAPCost integer
--- @field public ComboCategory string
--- @field public Flags string See AttributeFlags enumeration
--- @field public Boosts string
--- @field public InventoryTab string See InventoryTabs enumeration
--- @field public Charges integer
--- @field public MaxCharges integer
--- @field public Skills string
--- @field public Reflection string
--- @field public ItemGroup string
--- @field public ObjectCategory string
--- @field public MinAmount integer
--- @field public MaxAmount integer
--- @field public Priority integer
--- @field public Unique integer
--- @field public MinLevel integer
--- @field public MaxLevel integer
--- @field public ItemColor string
--- @field public MaxSummons integer
--- @field public RuneSlots integer
--- @field public RuneSlots_V1 integer
--- @field public NeedsIdentification string See YesNo enumeration
--- @field public LifeSteal integer
--- @field public CleavePercentage integer
--- @field public CleaveAngle integer
--- @field public Talents string
--- @field public IgnoreVisionBlock string See YesNo enumeration
--- @field public Tags string
--- @field public ExtraProperties StatProperty[]
local StatEntryWeapon = {
    --- @type string See Damage Type enumeration
    ['Damage Type'] = "",
    --- @type integer
    ['Damage Range'] = 0,
    --- @type string See ActPart enumeration
    ['Act part'] = ""
}


--- @class StatEntryArmor
--- @field public Name string
--- @field public Level integer
--- @field public Using string
--- @field public ArmorBoost integer
--- @field public MagicArmorBoost integer
--- @field public Movement integer
--- @field public Initiative integer
--- @field public Requirements StatRequirement[]
--- @field public Slot string See Itemslot enumeration
--- @field public Durability integer
--- @field public DurabilityDegradeSpeed string See Qualifier enumeration
--- @field public Value integer
--- @field public ModifierType string See ModifierType enumeration
--- @field public Act string See Act enumeration
--- @field public Fire integer
--- @field public Air integer
--- @field public Water integer
--- @field public Earth integer
--- @field public Poison integer
--- @field public Piercing integer
--- @field public Physical integer
--- @field public StrengthBoost string See Penalty Qualifier enumeration
--- @field public FinesseBoost string See Penalty Qualifier enumeration
--- @field public IntelligenceBoost string See Penalty Qualifier enumeration
--- @field public ConstitutionBoost string See Penalty Qualifier enumeration
--- @field public MemoryBoost string See Penalty Qualifier enumeration
--- @field public WitsBoost string See Penalty Qualifier enumeration
--- @field public SingleHanded integer
--- @field public TwoHanded integer
--- @field public Ranged integer
--- @field public DualWielding integer
--- @field public RogueLore integer
--- @field public WarriorLore integer
--- @field public RangerLore integer
--- @field public FireSpecialist integer
--- @field public WaterSpecialist integer
--- @field public AirSpecialist integer
--- @field public EarthSpecialist integer
--- @field public Sourcery integer
--- @field public Necromancy integer
--- @field public Polymorph integer
--- @field public Summoning integer
--- @field public PainReflection integer
--- @field public Perseverance integer
--- @field public Leadership integer
--- @field public Telekinesis integer
--- @field public Sneaking integer
--- @field public Thievery integer
--- @field public Loremaster integer
--- @field public Repair integer
--- @field public Barter integer
--- @field public Persuasion integer
--- @field public Luck integer
--- @field public SightBoost string See Penalty Qualifier enumeration
--- @field public HearingBoost string See Penalty Qualifier enumeration
--- @field public VitalityBoost integer
--- @field public MagicPointsBoost string See Penalty Qualifier enumeration
--- @field public ChanceToHitBoost integer
--- @field public APMaximum integer
--- @field public APStart integer
--- @field public APRecovery integer
--- @field public AccuracyBoost integer
--- @field public DodgeBoost integer
--- @field public CriticalChance integer
--- @field public ComboCategory string
--- @field public Weight integer
--- @field public InventoryTab string See InventoryTabs enumeration
--- @field public Flags string See AttributeFlags enumeration
--- @field public ArmorType string See ArmorType enumeration
--- @field public Boosts string
--- @field public Skills string
--- @field public ItemColor string
--- @field public Reflection string
--- @field public ItemGroup string
--- @field public ObjectCategory string
--- @field public MinAmount integer
--- @field public MaxAmount integer
--- @field public Priority integer
--- @field public Unique integer
--- @field public MinLevel integer
--- @field public MaxLevel integer
--- @field public MaxSummons integer
--- @field public NeedsIdentification string See YesNo enumeration
--- @field public Charges integer
--- @field public RuneSlots integer
--- @field public RuneSlots_V1 integer
--- @field public MaxCharges integer
--- @field public Talents string
--- @field public Tags string
--- @field public ExtraProperties StatProperty[]
local StatEntryArmor = {
    ['Armor Defense Value'] = 0,
    ['Magic Armor Value'] = 0,
    --- @type string See ActPart enumeration
    ['Act part'] = ""
}


--- @class StatEntryShield
--- @field public Name string
--- @field public Level integer
--- @field public Using string
--- @field public ArmorBoost integer
--- @field public MagicArmorBoost integer
--- @field public Movement integer
--- @field public Initiative integer
--- @field public Requirements StatRequirement[]
--- @field public Slot string See Itemslot enumeration
--- @field public Durability integer
--- @field public DurabilityDegradeSpeed string See Qualifier enumeration
--- @field public Value integer
--- @field public ModifierType string See ModifierType enumeration
--- @field public Act string See Act enumeration
--- @field public Fire integer
--- @field public Air integer
--- @field public Water integer
--- @field public Earth integer
--- @field public Poison integer
--- @field public Piercing integer
--- @field public Physical integer
--- @field public Blocking integer
--- @field public StrengthBoost string See Penalty Qualifier enumeration
--- @field public FinesseBoost string See Penalty Qualifier enumeration
--- @field public IntelligenceBoost string See Penalty Qualifier enumeration
--- @field public ConstitutionBoost string See Penalty Qualifier enumeration
--- @field public MemoryBoost string See Penalty Qualifier enumeration
--- @field public WitsBoost string See Penalty Qualifier enumeration
--- @field public SingleHanded integer
--- @field public TwoHanded integer
--- @field public Ranged integer
--- @field public DualWielding integer
--- @field public RogueLore integer
--- @field public WarriorLore integer
--- @field public RangerLore integer
--- @field public FireSpecialist integer
--- @field public WaterSpecialist integer
--- @field public AirSpecialist integer
--- @field public EarthSpecialist integer
--- @field public Sourcery integer
--- @field public Necromancy integer
--- @field public Polymorph integer
--- @field public Summoning integer
--- @field public Leadership integer
--- @field public PainReflection integer
--- @field public Perseverance integer
--- @field public Telekinesis integer
--- @field public Sneaking integer
--- @field public Thievery integer
--- @field public Loremaster integer
--- @field public Repair integer
--- @field public Barter integer
--- @field public Persuasion integer
--- @field public Luck integer
--- @field public SightBoost string See Penalty Qualifier enumeration
--- @field public HearingBoost string See Penalty Qualifier enumeration
--- @field public VitalityBoost integer
--- @field public MagicPointsBoost string See Penalty Qualifier enumeration
--- @field public ChanceToHitBoost integer
--- @field public APMaximum integer
--- @field public APStart integer
--- @field public APRecovery integer
--- @field public AccuracyBoost integer
--- @field public DodgeBoost integer
--- @field public CriticalChance string See Penalty Qualifier enumeration
--- @field public ComboCategory string
--- @field public Weight integer
--- @field public InventoryTab string See InventoryTabs enumeration
--- @field public Flags string See AttributeFlags enumeration
--- @field public Skills string
--- @field public Reflection string
--- @field public ItemGroup string
--- @field public ObjectCategory string
--- @field public MinAmount integer
--- @field public MaxAmount integer
--- @field public Priority integer
--- @field public Unique integer
--- @field public MinLevel integer
--- @field public MaxLevel integer
--- @field public ItemColor string
--- @field public MaxSummons integer
--- @field public RuneSlots integer
--- @field public RuneSlots_V1 integer
--- @field public NeedsIdentification string See YesNo enumeration
--- @field public Talents string
--- @field public Tags string
--- @field public ExtraProperties StatProperty[]
local StatEntryShield = {
    ['Armor Defense Value'] = 0,
    ['Magic Armor Value'] = 0,
    --- @type string See ActPart enumeration
    ['Act part'] = ""
}


--- @class StatEntryPotion
--- @field public Name string
--- @field public Level integer
--- @field public Using string
--- @field public ModifierType string See ModifierType enumeration
--- @field public VitalityBoost integer
--- @field public Strength string See Penalty PreciseQualifier enumeration
--- @field public Finesse string See Penalty PreciseQualifier enumeration
--- @field public Intelligence string See Penalty PreciseQualifier enumeration
--- @field public Constitution string See Penalty PreciseQualifier enumeration
--- @field public Memory string See Penalty PreciseQualifier enumeration
--- @field public Wits string See Penalty PreciseQualifier enumeration
--- @field public SingleHanded integer
--- @field public TwoHanded integer
--- @field public Ranged integer
--- @field public DualWielding integer
--- @field public RogueLore integer
--- @field public WarriorLore integer
--- @field public RangerLore integer
--- @field public FireSpecialist integer
--- @field public WaterSpecialist integer
--- @field public AirSpecialist integer
--- @field public EarthSpecialist integer
--- @field public Sourcery integer
--- @field public Necromancy integer
--- @field public Polymorph integer
--- @field public Summoning integer
--- @field public PainReflection integer
--- @field public Perseverance integer
--- @field public Leadership integer
--- @field public Telekinesis integer
--- @field public Sneaking integer
--- @field public Thievery integer
--- @field public Loremaster integer
--- @field public Repair integer
--- @field public Barter integer
--- @field public Persuasion integer
--- @field public Luck integer
--- @field public FireResistance integer
--- @field public EarthResistance integer
--- @field public WaterResistance integer
--- @field public AirResistance integer
--- @field public PoisonResistance integer
--- @field public PhysicalResistance integer
--- @field public PiercingResistance integer
--- @field public Sight integer
--- @field public Hearing string See Penalty Qualifier enumeration
--- @field public Initiative integer
--- @field public Vitality integer
--- @field public VitalityPercentage integer
--- @field public MagicPoints integer
--- @field public ActionPoints integer
--- @field public ChanceToHitBoost integer
--- @field public AccuracyBoost integer
--- @field public DodgeBoost integer
--- @field public DamageBoost integer
--- @field public APCostBoost integer
--- @field public SPCostBoost integer
--- @field public APMaximum integer
--- @field public APStart integer
--- @field public APRecovery integer
--- @field public Movement integer
--- @field public MovementSpeedBoost integer
--- @field public Gain string See BigQualifier enumeration
--- @field public Armor integer
--- @field public MagicArmor integer
--- @field public ArmorBoost integer
--- @field public MagicArmorBoost integer
--- @field public CriticalChance integer
--- @field public Act string See Act enumeration
--- @field public Duration integer
--- @field public UseAPCost integer
--- @field public ComboCategory string
--- @field public StackId string
--- @field public BoostConditions string
--- @field public Flags string See AttributeFlags enumeration
--- @field public StatusMaterial string
--- @field public StatusEffect string
--- @field public StatusIcon string
--- @field public SavingThrow string See SavingThrow enumeration
--- @field public Weight integer
--- @field public Value integer
--- @field public InventoryTab string See InventoryTabs enumeration
--- @field public UnknownBeforeConsume string See YesNo enumeration
--- @field public Reflection string
--- @field public Damage string See Qualifier enumeration
--- @field public DamageType string See Damage Type enumeration
--- @field public AuraRadius integer
--- @field public AuraSelf string
--- @field public AuraAllies string
--- @field public AuraEnemies string
--- @field public AuraNeutrals string
--- @field public AuraItems string
--- @field public AuraFX string
--- @field public RootTemplate string
--- @field public ObjectCategory string
--- @field public MinAmount integer
--- @field public MaxAmount integer
--- @field public Priority integer
--- @field public Unique integer
--- @field public MinLevel integer
--- @field public MaxLevel integer
--- @field public BloodSurfaceType string
--- @field public MaxSummons integer
--- @field public AddToBottomBar string See YesNo enumeration
--- @field public SummonLifelinkModifier integer
--- @field public IgnoredByAI string See YesNo enumeration
--- @field public RangeBoost integer
--- @field public BonusWeapon string
--- @field public AiCalculationStatsOverride string
--- @field public RuneEffectWeapon string
--- @field public RuneEffectUpperbody string
--- @field public RuneEffectAmulet string
--- @field public RuneLevel integer
--- @field public LifeSteal integer
--- @field public IsFood string See YesNo enumeration
--- @field public IsConsumable string See YesNo enumeration
--- @field public ExtraProperties StatProperty[]
local StatEntryPotion = {
    ['Damage Multiplier'] = 0,
    ['Damage Range'] = 0,
    --- @type string See ActPart enumeration
    ['Act part'] = ""
}


--- @class StatEntryObject
--- @field public Name string
--- @field public Level integer
--- @field public Using string
--- @field public ModifierType string See ModifierType enumeration
--- @field public Act string See Act enumeration
--- @field public UseAPCost integer
--- @field public Value integer
--- @field public ComboCategory string
--- @field public Weight integer
--- @field public Strength string See PreciseQualifier enumeration
--- @field public Finesse string See PreciseQualifier enumeration
--- @field public Intelligence string See PreciseQualifier enumeration
--- @field public Constitution string See PreciseQualifier enumeration
--- @field public Memory string See PreciseQualifier enumeration
--- @field public Wits string See PreciseQualifier enumeration
--- @field public Vitality integer
--- @field public Armor string See PreciseQualifier enumeration
--- @field public FireResistance integer
--- @field public EarthResistance integer
--- @field public WaterResistance integer
--- @field public AirResistance integer
--- @field public PoisonResistance integer
--- @field public PiercingResistance integer
--- @field public PhysicalResistance integer
--- @field public ShadowResistance integer
--- @field public Flags string See AttributeFlags enumeration
--- @field public Requirements StatRequirement[]
--- @field public InventoryTab string See InventoryTabs enumeration
--- @field public RootTemplate string
--- @field public ObjectCategory string
--- @field public MinAmount integer
--- @field public MaxAmount integer
--- @field public Priority integer
--- @field public Unique integer
--- @field public MinLevel integer
--- @field public RuneEffectWeapon string
--- @field public RuneEffectUpperbody string
--- @field public RuneEffectAmulet string
--- @field public RuneLevel integer
--- @field public MaxLevel integer
--- @field public AddToBottomBar string See YesNo enumeration
--- @field public IgnoredByAI string See YesNo enumeration
local StatEntryObject = {
    --- @type string See ActPart enumeration
    ['Act part'] = ""
}


--- @class StatEntryCharacter
--- @field public Name string
--- @field public Level integer
--- @field public Using string
--- @field public Strength string See PreciseQualifier enumeration
--- @field public Finesse string See PreciseQualifier enumeration
--- @field public Intelligence string See PreciseQualifier enumeration
--- @field public Constitution string See PreciseQualifier enumeration
--- @field public Memory string See PreciseQualifier enumeration
--- @field public Wits string See PreciseQualifier enumeration
--- @field public SingleHanded integer
--- @field public TwoHanded integer
--- @field public Ranged integer
--- @field public DualWielding integer
--- @field public RogueLore integer
--- @field public WarriorLore integer
--- @field public RangerLore integer
--- @field public FireSpecialist integer
--- @field public WaterSpecialist integer
--- @field public AirSpecialist integer
--- @field public EarthSpecialist integer
--- @field public Sourcery integer
--- @field public Necromancy integer
--- @field public Polymorph integer
--- @field public Summoning integer
--- @field public PainReflection integer
--- @field public Leadership integer
--- @field public Perseverance integer
--- @field public Telekinesis integer
--- @field public Sneaking integer
--- @field public Thievery integer
--- @field public Loremaster integer
--- @field public Repair integer
--- @field public Barter integer
--- @field public Persuasion integer
--- @field public Luck integer
--- @field public FireResistance integer
--- @field public EarthResistance integer
--- @field public WaterResistance integer
--- @field public AirResistance integer
--- @field public PoisonResistance integer
--- @field public PiercingResistance integer
--- @field public PhysicalResistance integer
--- @field public Sight string See Penalty Qualifier enumeration
--- @field public Hearing string See Penalty Qualifier enumeration
--- @field public FOV integer
--- @field public APMaximum integer
--- @field public APStart integer
--- @field public APRecovery integer
--- @field public Initiative integer
--- @field public Vitality integer
--- @field public MagicPoints integer
--- @field public ChanceToHitBoost integer
--- @field public Movement integer
--- @field public MovementSpeedBoost integer
--- @field public CriticalChance string See Penalty Qualifier enumeration
--- @field public Gain string See BigQualifier enumeration
--- @field public Armor integer
--- @field public ArmorBoost integer
--- @field public ArmorBoostGrowthPerLevel integer
--- @field public MagicArmor integer
--- @field public MagicArmorBoost integer
--- @field public MagicArmorBoostGrowthPerLevel integer
--- @field public Accuracy integer
--- @field public Dodge integer
--- @field public Act string See Act enumeration
--- @field public MaxResistance integer
--- @field public Weight integer
--- @field public Talents string
--- @field public Traits string
--- @field public PathInfluence string
--- @field public Flags string See AttributeFlags enumeration
--- @field public Reflection string
--- @field public StepsType string See StepsType enumeration
--- @field public MaxSummons integer
--- @field public MPStart integer
--- @field public DamageBoost integer
--- @field public DamageBoostGrowthPerLevel integer
local StatEntryCharacter = {
    --- @type string See ActPart enumeration
    ['Act part'] = "",
    --- @type string See Penalty Qualifier enumeration
    ['Act strength'] = ""
}


--- @class StatEntrySkillData
--- @field public Name string
--- @field public Level integer
--- @field public Using string
--- @field public SkillType string
--- @field public Ability string See SkillAbility enumeration
--- @field public Element string See SkillElement enumeration
--- @field public Requirement string See SkillRequirement enumeration
--- @field public Requirements StatRequirement[]
--- @field public DisplayName string
--- @field public DisplayNameRef string
--- @field public Description string
--- @field public DescriptionRef string
--- @field public StatsDescription string
--- @field public StatsDescriptionRef string
--- @field public StatsDescriptionParams string
--- @field public Icon string
--- @field public FXScale integer
--- @field public PrepareAnimationInit string
--- @field public PrepareAnimationLoop string
--- @field public PrepareEffect string
--- @field public PrepareEffectBone string
--- @field public CastAnimation string
--- @field public CastTextEvent string
--- @field public CastAnimationCheck string See CastCheckType enumeration
--- @field public CastEffect string
--- @field public CastEffectTextEvent string
--- @field public TargetCastEffect string
--- @field public TargetHitEffect string
--- @field public TargetEffect string
--- @field public SourceTargetEffect string
--- @field public TargetTargetEffect string
--- @field public LandingEffect string
--- @field public ImpactEffect string
--- @field public MaleImpactEffects string
--- @field public FemaleImpactEffects string
--- @field public OnHitEffect string
--- @field public SelectedCharacterEffect string
--- @field public SelectedObjectEffect string
--- @field public SelectedPositionEffect string
--- @field public DisappearEffect string
--- @field public ReappearEffect string
--- @field public ReappearEffectTextEvent string
--- @field public RainEffect string
--- @field public StormEffect string
--- @field public FlyEffect string
--- @field public SpatterEffect string
--- @field public ShieldMaterial string
--- @field public ShieldEffect string
--- @field public ContinueEffect string
--- @field public SkillEffect string
--- @field public Template string
--- @field public TemplateCheck string See CastCheckType enumeration
--- @field public TemplateOverride string
--- @field public TemplateAdvanced string
--- @field public Totem string See YesNo enumeration
--- @field public Template1 string
--- @field public Template2 string
--- @field public Template3 string
--- @field public WeaponBones string
--- @field public TeleportSelf string See YesNo enumeration
--- @field public CanTargetCharacters string See YesNo enumeration
--- @field public CanTargetItems string See YesNo enumeration
--- @field public CanTargetTerrain string See YesNo enumeration
--- @field public ForceTarget string See YesNo enumeration
--- @field public TargetProjectiles string See YesNo enumeration
--- @field public UseCharacterStats string See YesNo enumeration
--- @field public UseWeaponDamage string See YesNo enumeration
--- @field public UseWeaponProperties string See YesNo enumeration
--- @field public SingleSource string See YesNo enumeration
--- @field public ContinueOnKill string See YesNo enumeration
--- @field public Autocast string See YesNo enumeration
--- @field public AmountOfTargets integer
--- @field public AutoAim string See YesNo enumeration
--- @field public AddWeaponRange string See YesNo enumeration
--- @field public ActionPoints integer
--- @field public Cooldown integer
--- @field public CooldownReduction integer
--- @field public ChargeDuration integer
--- @field public CastDelay integer
--- @field public Offset integer
--- @field public Lifetime integer
--- @field public Duration string See Qualifier enumeration
--- @field public TargetRadius integer
--- @field public ExplodeRadius integer
--- @field public AreaRadius integer
--- @field public HitRadius integer
--- @field public RadiusMax integer
--- @field public Range integer
--- @field public MaxDistance integer
--- @field public Angle integer
--- @field public TravelSpeed integer
--- @field public Acceleration integer
--- @field public Height integer
--- @field public Damage string See DamageSourceType enumeration
--- @field public DamageType string See Damage Type enumeration
--- @field public DamageMultiplier string See PreciseQualifier enumeration
--- @field public DeathType string See Death Type enumeration
--- @field public BonusDamage string See Qualifier enumeration
--- @field public HitPointsPercent integer
--- @field public MinHitsPerTurn integer
--- @field public MaxHitsPerTurn integer
--- @field public HitDelay integer
--- @field public MaxAttacks integer
--- @field public NextAttackChance integer
--- @field public NextAttackChanceDivider integer
--- @field public EndPosRadius integer
--- @field public JumpDelay integer
--- @field public TeleportDelay integer
--- @field public PointsMaxOffset integer
--- @field public RandomPoints integer
--- @field public ChanceToPierce integer
--- @field public MaxPierceCount integer
--- @field public MaxForkCount integer
--- @field public ForkLevels integer
--- @field public ForkChance integer
--- @field public HealAmount string See PreciseQualifier enumeration
--- @field public StatusClearChance integer
--- @field public SurfaceType string See Surface Type enumeration
--- @field public SurfaceLifetime integer
--- @field public SurfaceStatusChance integer
--- @field public SurfaceTileCollision string See SurfaceCollisionFlags enumeration
--- @field public SurfaceGrowInterval integer
--- @field public SurfaceGrowStep integer
--- @field public SurfaceRadius integer
--- @field public TotalSurfaceCells integer
--- @field public SurfaceMinSpawnRadius integer
--- @field public MinSurfaces integer
--- @field public MaxSurfaces integer
--- @field public MinSurfaceSize integer
--- @field public MaxSurfaceSize integer
--- @field public GrowSpeed integer
--- @field public GrowOnSurface string See SurfaceCollisionFlags enumeration
--- @field public GrowTimeout integer
--- @field public SkillBoost string
--- @field public SkillAttributeFlags string See AttributeFlags enumeration
--- @field public SkillProperties StatProperty[]
--- @field public CleanseStatuses string
--- @field public AoEConditions string See Conditions enumeration
--- @field public TargetConditions string See Conditions enumeration
--- @field public ForkingConditions string See Conditions enumeration
--- @field public CycleConditions string See Conditions enumeration
--- @field public ShockWaveDuration integer
--- @field public TeleportTextEvent string
--- @field public SummonEffect string
--- @field public ProjectileCount integer
--- @field public ProjectileDelay integer
--- @field public StrikeCount integer
--- @field public StrikeDelay integer
--- @field public PreviewStrikeHits string See YesNo enumeration
--- @field public SummonLevel integer
--- @field public StartTextEvent string
--- @field public StopTextEvent string
--- @field public Atmosphere string See AtmosphereType enumeration
--- @field public ConsequencesStartTime integer
--- @field public ConsequencesDuration integer
--- @field public HealthBarColor integer
--- @field public Skillbook string
--- @field public PreviewImpactEffect string
--- @field public IgnoreVisionBlock string See YesNo enumeration
--- @field public HealEffectId string
--- @field public AddRangeFromAbility string See Ability enumeration
--- @field public DivideDamage string See YesNo enumeration
--- @field public OverrideMinAP string See YesNo enumeration
--- @field public OverrideSkillLevel string See YesNo enumeration
--- @field public Tier string See SkillTier enumeration
--- @field public GrenadeBone string
--- @field public GrenadeProjectile string
--- @field public GrenadePath string
--- @field public MovingObject string
--- @field public SpawnObject string
--- @field public SpawnEffect string
--- @field public SpawnFXOverridesImpactFX string See YesNo enumeration
--- @field public SpawnLifetime integer
--- @field public ProjectileTerrainOffset string See YesNo enumeration
--- @field public ProjectileType string See ProjectileType enumeration
--- @field public HitEffect string
--- @field public PushDistance integer
--- @field public ForceMove string See YesNo enumeration
--- @field public Stealth string See YesNo enumeration
--- @field public Distribution string See ProjectileDistribution enumeration
--- @field public Shuffle string See YesNo enumeration
--- @field public PushPullEffect string
--- @field public BackStart integer
--- @field public FrontOffset integer
--- @field public TargetGroundEffect string
--- @field public PositionEffect string
--- @field public BeamEffect string
--- @field public PreviewEffect string
--- @field public CastSelfAnimation string
--- @field public IgnoreCursed string See YesNo enumeration
--- @field public IsEnemySkill string See YesNo enumeration
--- @field public DomeEffect string
--- @field public AuraSelf string
--- @field public AuraAllies string
--- @field public AuraEnemies string
--- @field public AuraNeutrals string
--- @field public AuraItems string
--- @field public AIFlags string See AIFlags enumeration
--- @field public Shape string
--- @field public Base integer
--- @field public AiCalculationSkillOverride string
--- @field public TeleportSurface string See YesNo enumeration
--- @field public ProjectileSkills string
--- @field public SummonCount integer
--- @field public LinkTeleports string See YesNo enumeration
--- @field public TeleportsUseCount integer
--- @field public HeightOffset integer
--- @field public ForGameMaster string See YesNo enumeration
--- @field public IsMelee string See YesNo enumeration
--- @field public MemorizationRequirements StatRequirement[]
--- @field public IgnoreSilence string See YesNo enumeration
--- @field public IgnoreHeight string See YesNo enumeration
local StatEntrySkillData = {
    ['Memory Cost'] = 0,
    ['Magic Cost'] = 0,
    ['Damage Multiplier'] = 0,
    ['Damage Range'] = 0,
    ['Chance To Hit Multiplier'] = 0,
    ['Damage On Jump'] = 0,
    ['Damage On Landing'] = 0,
    ['Healing Multiplier'] = 0,
    ['Stealth Damage Multiplier'] = 0,
    ['Distance Damage Multiplier'] = 0
}


--- @class StatEntryStatusData
--- @field public Name string
--- @field public Level integer
--- @field public Using string
--- @field public StatusType string
--- @field public Icon string
--- @field public DisplayName string
--- @field public DisplayNameRef string
--- @field public Description string
--- @field public DescriptionRef string
--- @field public DescriptionParams string
--- @field public OverrideDefaultDescription string See YesNo enumeration
--- @field public FormatColor string See FormatStringColor enumeration
--- @field public SavingThrow string See SavingThrow enumeration
--- @field public IsChanneled string See YesNo enumeration
--- @field public Instant string See YesNo enumeration
--- @field public StatusEffect string
--- @field public StatusEffectOverrideForItems string
--- @field public StatusEffectOnTurn string
--- @field public MaterialType string See MaterialType enumeration
--- @field public Material string
--- @field public MaterialApplyBody string See YesNo enumeration
--- @field public MaterialApplyArmor string See YesNo enumeration
--- @field public MaterialApplyWeapon string See YesNo enumeration
--- @field public MaterialApplyNormalMap string See YesNo enumeration
--- @field public MaterialFadeAmount integer
--- @field public MaterialOverlayOffset integer
--- @field public MaterialParameters string
--- @field public HealingEvent string See StatusEvent enumeration
--- @field public HealStat string See StatusHealType enumeration
--- @field public HealType string See HealValueType enumeration
--- @field public HealValue integer
--- @field public StatsId string
--- @field public IsInvulnerable string See YesNo enumeration
--- @field public IsDisarmed string See YesNo enumeration
--- @field public StackId string
--- @field public StackPriority integer
--- @field public AuraRadius integer
--- @field public AuraSelf string
--- @field public AuraAllies string
--- @field public AuraEnemies string
--- @field public AuraNeutrals string
--- @field public AuraItems string
--- @field public AuraFX string
--- @field public ImmuneFlag string See AttributeFlag enumeration
--- @field public CleanseStatuses string
--- @field public MaxCleanseCount integer
--- @field public ApplyAfterCleanse string See YesNo enumeration
--- @field public SoundStart string
--- @field public SoundLoop string
--- @field public SoundStop string
--- @field public DamageEvent string See StatusEvent enumeration
--- @field public DamageStats string
--- @field public DeathType string See Death Type enumeration
--- @field public DamageCharacters string See YesNo enumeration
--- @field public DamageItems string See YesNo enumeration
--- @field public DamageTorches string See YesNo enumeration
--- @field public FreezeTime integer
--- @field public SurfaceChange string
--- @field public PermanentOnTorch string See YesNo enumeration
--- @field public AbsorbSurfaceType string
--- @field public AbsorbSurfaceRange integer
--- @field public Skills string
--- @field public BonusFromAbility string See Ability enumeration
--- @field public Items string
--- @field public OnlyWhileMoving string See YesNo enumeration
--- @field public DescriptionCaster string
--- @field public DescriptionTarget string
--- @field public WinBoost string See Properties enumeration
--- @field public LoseBoost string See Properties enumeration
--- @field public WeaponOverride string
--- @field public ApplyEffect string
--- @field public ForGameMaster string See YesNo enumeration
--- @field public ResetCooldowns string
--- @field public ResetOncePerCombat string See YesNo enumeration
--- @field public PolymorphResult string
--- @field public DisableInteractions string See YesNo enumeration
--- @field public LoseControl string See YesNo enumeration
--- @field public AiCalculationSkillOverride string
--- @field public HealEffectId string
--- @field public ScaleWithVitality string See YesNo enumeration
--- @field public VampirismType string See VampirismType enumeration
--- @field public BeamEffect string
--- @field public HealMultiplier integer
--- @field public InitiateCombat string See YesNo enumeration
--- @field public Projectile string
--- @field public Radius integer
--- @field public Charges integer
--- @field public MaxCharges integer
--- @field public DefendTargetPosition string See YesNo enumeration
--- @field public TargetConditions string See Conditions enumeration
--- @field public Toggle string See YesNo enumeration
--- @field public LeaveAction string
--- @field public DieAction string
--- @field public PlayerSameParty string See YesNo enumeration
--- @field public PlayerHasTag string
--- @field public PeaceOnly string See YesNo enumeration
--- @field public Necromantic string See YesNo enumeration
--- @field public RetainSkills string
--- @field public BringIntoCombat string See YesNo enumeration
--- @field public ApplyStatusOnTick string
--- @field public IsResistingDeath string See YesNo enumeration
--- @field public TargetEffect string
--- @field public DamagePercentage integer
--- @field public ForceOverhead string See YesNo enumeration
--- @field public TickSFX string
--- @field public ForceStackOverwrite string See YesNo enumeration
--- @field public FreezeCooldowns string See YesNo enumeration
local StatEntryStatusData = {}


--- @class StatEntryCrime
--- @field public Name string
--- @field public Level integer
--- @field public Using string
--- @field public CrimeType string
--- @field public TensionWeight integer
--- @field public Lifetime integer
--- @field public Audible string See YesNo enumeration
--- @field public DetectionRange integer
--- @field public CreatesCrimescene string See YesNo enumeration
--- @field public Continuous string See YesNo enumeration
--- @field public VictimReaction string
--- @field public NoticedAD string
--- @field public WarningDialog string
--- @field public ArrestDialog string
--- @field public InterrogateDialog string
--- @field public ReactionCategory_Tag string
--- @field public ReactionCategory_Priority integer
--- @field public ReactionCategory_PrimaryReaction string
--- @field public ReactionCategory_SecondaryReaction string
--- @field public MaxInvestigators integer
--- @field public ContinuousDelayTimer integer
--- @field public SoundEvent string
--- @field public AudibleRange integer
--- @field public VictimDialog string
--- @field public CanRaiseAlarm string See YesNo enumeration
--- @field public CanMerge string See YesNo enumeration
local StatEntryCrime = {}


--- @class HitRequest
--- @field public Equipment integer
--- @field public TotalDamageDone integer
--- @field public DamageDealt integer
--- @field public DeathType string See DeathType enumeration
--- @field public DamageType string See DamageType enumeration
--- @field public AttackDirection integer
--- @field public ArmorAbsorption integer
--- @field public LifeSteal integer
--- @field public EffectFlags integer See HitFlags enumeration
--- @field public HitWithWeapon boolean
--- @field public DamageList DamageList
local HitRequest = {}


--- @class HitContext
--- Context information passed in StatusHitEnter/BeforeCharacterApplyDamage callbacks
--- @field public HitId integer Unique hit identifier for hit tracking
--- @field public Weapon StatItem
--- @field public Hit HitRequest
--- @field public HitType string See HitType enumeration
--- @field public NoHitRoll boolean
--- @field public ProcWindWalker boolean
--- @field public ForceReduceDurability boolean
--- @field public HighGround string
--- @field public CriticalRoll string
--- @field public HitStatus EsvStatus
local HitContext = {}


--- @class EsvCombatTeam
--- @field public TeamId integer A number identifying the team instance
--- @field public CombatId integer 	Identifies which combat the team is a participant of
--- @field public Initiative integer Computed initiative value of the team
--- @field public StillInCombat boolean Can the team currently fight, or is it temporarily out of combat?
--- @field public Character EsvCharacter|nil Character object if the team is a character; nil otherwise
--- @field public Item EsvItem|nil Item object if the team is an item; nil otherwise
local EsvCombatTeam = {}


--- @class EsvCombat
--- @field public CombatId integer A number identifying the combat instance
--- @field public LevelName string Level where the combat is taking place
--- @field public IsActive boolean
local EsvCombat = {}

--- Retrieves the turn order of the current round.
--- @param self EsvCombat
--- @return EsvCombatTeam[]
function EsvCombat.GetCurrentTurnOrder (self) end
    
--- Retrieves the turn order of the next round.
--- @param self EsvCombat
--- @return EsvCombatTeam[]
function EsvCombat.GetNextTurnOrder (self) end

--- Updates the turn order of the current round. 
--- The turnOrder argument should be a reordered version of the table returned by GetCurrentTurnOrder().
--- Notes:
---  It is possible to remove or add characters to the current turn by adding/removing their Team object from the table.
---  It is possible to add a character to the current turn more than once, the character will only appear once in the UI however.
---  The character whose turn is currently active (the very first item) should not be removed or reordered. 
---    This only applies for GetCurrentTurnOrder, the first item can be freely reordered in GetNextTurnOrder.
---  Changed performed using this function are synchronized to the client at the end of the current server tick.
--- @param self EsvCombat
--- @param turnOrder EsvCombatTeam[]
function EsvCombat.UpdateCurrentTurnOrder (self, turnOrder) end
    
--- Updates the turn order of the next round. 
--- The turnOrder argument should be a reordered version of the table returned by GetNextTurnOrder().
--- Notes:
---  It is possible to remove or add characters to the next turn by adding/removing their Team object from the table.
---  It is possible to add a character to the next turn more than once, the character will only appear once in the UI however.
---  Changed performed using this function are synchronized to the client at the end of the current server tick.
--- @param self EsvCombat
--- @param turnOrder EsvCombatTeam[]
function EsvCombat.UpdateNextTurnOrder (self, turnOrder) end
    
--- Retrieves all participants of the combat
--- @param self EsvCombat
--- @return EsvCombatTeam[]
function EsvCombat.GetAllTeams (self) end


--- @class FlashObject
--- Represents an object in Flash.
--- Implements the __index and __newindex metamethods using string keys (i.e. allows table-like behavior):
--- obj.field = 123 -- Can assign values to any object property
--- Ext.Print(obj.field) -- Can read any object property
---
--- Field values are returned using the appropriate Lua type;
--- Flash Boolean/Number/String = Lua boolean/number/string
--- Flash Object = Lua engine class FlashObject
--- Flash array = Lua engine class FlashArray
--- Flash function = Lua engine class FlashFunction
local FlashObject = {}


--- @class FlashArray
--- Represents an array in Flash.
--- Implements the __index, __newindex and __len metamethods using integer keys (i.e. allows table-like behavior):
--- arr[2] = 123 -- Can assign values to any array index
--- Ext.Print(arr[2]) -- Can read any array index
--- Ext.Print(#arr) -- Can query length of array
local FlashArray = {}


--- @class FlashFunction
--- Represents a function or method in Flash.
--- Implements the __call metamethod (i.e. can be called).
--- The passed arguments are forwarded to the Flash method and the return value of the Flash method is returned.
local FlashFunction = {}


--- @class UIObject
local UIObject = {}

--- @param self UIObject
--- @param x integer
--- @param y integer
function UIObject.SetPosition (self, x, y) end
    
--- @param self UIObject
--- @param width integer
--- @param height integer
function UIObject.Resize (self, width, height) end
    
--- @param self UIObject
function UIObject.Show (self) end
    
--- @param self UIObject
function UIObject.Hide (self) end
    
--- Calls a method on the main timeline object of the UI element. 
--- The first argument (func) is the name of the ActionScript function to call; 
--- all subsequent arguments are passed to the ActionScript function as parameters. 
--- Only string, number and boolean arguments are supported.
--- @param self UIObject
--- @param method string Method to call
--- @vararg any
function UIObject.Invoke (self, method, ...) end
    
--- Simulates an ExternalInterface.call(...) call from Flash, 
--- i.e. it calls an UI handler function in the game engine. 
--- The first argument (func) is the name of the UI function to call; 
--- all subsequent arguments are passed to the engine as parameters. 
--- Only string, number and boolean arguments are supported.
--- @param self UIObject
--- @param func string ExternalInterface function to call
--- @vararg any
function UIObject.ExternalInterfaceCall (self, func, ...) end
    
--- @param self UIObject
--- @param frame integer
function UIObject.GotoFrame (self, frame) end
    
--- Updates the specified public property of the main timeline object.
--- @param self UIObject
--- @param property string Property path to set
--- @param value number|boolean|string Value
--- @param arrayIndex integer|nil Pass integer to set property[arrayIndex]
function UIObject.SetValue (self, property, value, arrayIndex) end
    
--- Retrieves the specified public property of the main timeline object. 
--- "type" contains the type of value to retrieve and must be string, number or boolean.
--- @param self UIObject
--- @param property string Property path to set
--- @param type "'number'" | "'boolean'" | "'string'" | nil
--- @param arrayIndex integer|nil Pass integer to set property[arrayIndex]
--- @return number|boolean|string
function UIObject.GetValue (self, property, type, arrayIndex) end
    
--- Returns the root (MainTimeline) Flash object
--- @param self UIObject
--- @return FlashObject
function UIObject.GetRoot (self) end
    
--- Returns the engine UI type ID of the UI element
--- @param self UIObject
--- @return integer
function UIObject.GetTypeId (self) end
    
--- @param self UIObject
--- @return ObjectHandle
function UIObject.GetHandle (self) end
    
--- Returns the handle of the player that this UI element is assigned to.
--- (Only certain elements have a player assigned, like character sheet, inventory, etc.)
--- @param self UIObject
--- @return ObjectHandle|nil
function UIObject.GetPlayerHandle (self) end
    
--- @param self UIObject
function UIObject.Destroy (self) end


--- @class SurfaceInteractionSet
--- @field public TransformType string Surface transform to apply (Bless, Curse, Ignite, ...)
--- @field public ActionableSurfaces string[][] Surface types that this transform applies to
local SurfaceInteractionSet = {}


--- @alias ExtEngineEvent "'SessionLoading'" | "'SessionLoaded'" | "'ModuleLoading'" | "'ModuleLoadStarted'" | "'ModuleResume'" | "'GameStateChanged'" | "'SkillGetDescriptionParam'" | "'StatusGetDescriptionParam'" | "'GetSkillDamage'" | "'GetSkillAPCost'" | "'ComputeCharacterHit'" | "'CalculateTurnOrder'" | "'GetHitChance'" | "'StatusGetEnterChance'" | '"StatusHitEnter"' | "'BeforeCharacterApplyDamage'" | "'UIInvoke'" | "'UICall'" | "'AfterUIInvoke'" | "'AfterUICall'" | "'BeforeShootProjectile'" | "'ShootProjectile'" | "'ProjectileHit'" | "'GroundHit'"

--- @alias ExtGameStateChangedCallback fun(fromState: string, toState: string)
--- @alias ExtComputeCharacterHitCallback fun(target: StatCharacter, attacker: StatCharacter, weapon: StatItem, damageList: DamageList, hitType: string, noHitRoll: boolean, forceReduceDurability: boolean, hit: HitRequest, alwaysBackstab: boolean, highGroundFlag: string, criticalRoll: string): HitRequest
--- @alias ExtGetSkillDamageCallback fun(skill: StatEntrySkillData, attacker: StatCharacter, isFromItem: boolean, stealthed: boolean, attackerPos: number[], targetPos: number[], level: number, noRandomization: boolean): DamageList, string
--- @alias ExtCalculateTurnOrderCallback fun(combat: EsvCombat, order: EsvCombatTeam[]): EsvCombatTeam[]
--- @alias ExtGetHitChanceCallback fun(attacker: StatCharacter, target: StatCharacter): number
--- @alias ExtStatusGetEnterChanceCallback fun(status: EsvStatus, isEnterCheck: boolean): number
--- @alias ExtSkillGetDescriptionParamCallback fun(skill: StatEntrySkillData, character: StatCharacter, isFromItem: boolean, ...): string
--- @alias ExtStatusGetDescriptionParamCallback fun(status: EsvStatus, statusSource: EsvGameObject, character: StatCharacter, ...): string
--- @alias ExtGetSkillAPCostCallback fun(skill: StatEntrySkillData, character: StatCharacter, grid: AiGrid, position: number[]|nil, range: number|nil): number, boolean
--- @alias ExtBeforeCharacterApplyDamageCallback fun(target: EsvCharacter, attacker: StatCharacter|StatItem, hit: HitRequest, causeType: string, impactDirection: number[], context: HitContext): HitRequest
--- @alias ExtStatusHitEnterCallback fun(status: EsvStatus, context: HitContext)
--- @alias ExtBeforeShootProjectileCallback fun(projectile: EsvShootProjectileRequest)
--- @alias ExtShootProjectileCallback fun(projectile: EsvProjectile)
--- @alias ExtProjectileHitCallback fun(projectile: EsvProjectile, hitObject: EsvGameObject|nil, position: number[])
--- @alias ExtGroundHitCallback fun(caster: EsvGameObject, position: number[], damageList: DamageList)

--- @class Ext
Ext = {
    ExtraData = {
        FirstItemTypeShift = 9,
        SecondItemTypeShift = 16,
        PickpocketGoldValuePerPoint = 200,
        PickpocketWeightPerPoint = 2000,
        PickpocketExperienceLevelsPerPoint = 4,
        PersuasionAttitudeBonusPerPoint = 5,
        WandUsesMax = 10,
        AttributeBaseValue = 10,
        AttributeCharCreationBonus = 1,
        AttributeLevelGrowth = 2,
        AttributeBoostGrowth = 0.75,
        AttributeGrowthDamp = 0.7,
        AttributeSoftCap = 40,
        WitsGrowthDamp = 0.5,
        VitalityStartingAmount = 21,
        VitalityExponentialGrowth = 1.25,
        VitalityLinearGrowth = 9.091,
        VitalityToDamageRatio = 5,
        VitalityToDamageRatioGrowth = 0.2,
        ExpectedDamageBoostFromAttributePerLevel = 0.065,
        ExpectedDamageBoostFromSkillAbilityPerLevel = 0.015,
        ExpectedDamageBoostFromWeaponAbilityPerLevel = 0.025,
        ExpectedConGrowthForArmorCalculation = 1,
        FirstVitalityLeapLevel = 9,
        FirstVitalityLeapGrowth = 1.25,
        SecondVitalityLeapLevel = 13,
        SecondVitalityLeapGrowth = 1.25,
        ThirdVitalityLeapLevel = 16,
        ThirdVitalityLeapGrowth = 1.25,
        FourthVitalityLeapLevel = 18,
        FourthVitalityLeapGrowth = 1.35,
        DamageBoostFromAttribute = 0.05,
        MonsterDamageBoostPerLevel = 0.02,
        PhysicalArmourBoostFromAttribute = 0,
        MagicArmourBoostFromAttribute = 0,
        VitalityBoostFromAttribute = 0.07,
        DodgingBoostFromAttribute = 0,
        HealToDamageRatio = 1.3,
        ArmorToVitalityRatio = 0.55,
        ArmorRegenTimer = 0.01,
        ArmorRegenConstGrowth = 1,
        ArmorRegenPercentageGrowth = 10,
        ArmorAfterHitCooldown = -7,
        MagicArmorRegenTimer = 0.01,
        MagicArmorRegenConstGrowth = 1,
        MagicArmorRegenPercentageGrowth = 10,
        MagicArmorAfterHitCooldown = -7,
        ArmorHeadPercentage = 0.15,
        ArmorUpperBodyPercentage = 0.3,
        ArmorLowerBodyPercentage = 0.2,
        ArmorShieldPercentage = 0.5,
        ArmorHandsPercentage = 0.15,
        ArmorFeetPercentage = 0.15,
        ArmorBeltPercentage = 0.1,
        ArmorAmuletPercentage = 0.12,
        ArmorRingPercentage = 0.08,
        SkillMemoryCostReductionFromAbility = 0,
        CharacterBaseMemoryCapacity = 3,
        CharacterBaseMemoryCapacityGrowth = 0.5,
        CharacterAttributePointsPerMemoryCapacity = 1,
        LoremasterBonusToMemory = 2,
        AbilityBaseValue = 0,
        NumStartingCombatAbilityPoints = 2,
        CombatAbilityCap = 10,
        CombatAbilityLevelGrowth = 1,
        CombatAbilityNpcGrowth = 0.1,
        CombatAbilityDamageBonus = 5,
        CombatAbilityCritBonus = 1,
        CombatAbilityCritMultiplierBonus = 5,
        CombatAbilityAccuracyBonus = 5,
        CombatAbilityDodgingBonus = 1,
        CombatAbilityReflectionBonus = 5,
        LeadershipRange = 8,
        LeadershipDodgingBonus = 2,
        LeadershipAllResBonus = 3,
        NumStartingCivilAbilityPoints = 2,
        CivilAbilityCap = 5,
        CivilAbilityLevelGrowth = 4,
        CivilPointOffset = 2,
        SavethrowLowChance = 15,
        SavethrowHighChance = 50,
        SavethrowBelowLowPenalty = 5,
        SavethrowPenaltyCap = -30,
        CriticalBonusFromWits = 1,
        InitiativeBonusFromWits = 1,
        WeaponAccuracyPenaltyPerLevel = -20,
        WeaponAccuracyPenaltyCap = -80,
        ShieldAPCost = 0,
        CharacterWeightLight = 40000,
        CharacterWeightMedium = 70000,
        CharacterWeightHeavy = 120000,
        WeaponWeightLight = 1000,
        WeaponWeightMedium = 3000,
        WeaponWeightHeavy = 6000,
        HighGroundThreshold = 2.4,
        HighGroundBaseDamageBonus = 0.2,
        HighGroundMeleeRange = 1,
        HighGroundRangeMultiplier = 2.5,
        LowGroundBaseDamagePenalty = -0.1,
        SneakDefaultAPCost = 4,
        SneakSpeedBoost = -30,
        BlindRangePenalty = 3,
        RangeBoostedGlobalCap = 30,
        SurfaceDurationFromHitFloorReaction = 18,
        SurfaceDurationFireIgniteOverride = 12,
        SurfaceDurationFromCharacterBleeding = -1,
        SurfaceDurationBlessedCursed = -1,
        SurfaceDurationAfterDecay = -1,
        SmokeDurationAfterDecay = 6,
        DualWieldingAPPenalty = 2,
        DualWieldingDamagePenalty = 0.5,
        GhostLifeTime = 3,
        ChanceToSetStatusOnContact = 100,
        AbilityPhysArmorBonusBase = 5,
        AbilityPhysArmorBonusPerPoint = 2,
        AbilityPhysArmorBonusMax = 5,
        AbilityMagicArmorBonusBase = 5,
        AbilityMagicArmorBonusPerPoint = 2,
        AbilityMagicArmorBonusMax = 5,
        AbilityVitalityBonusBase = 3,
        AbilityVitalityBonusPerPoint = 1,
        AbilityVitalityBonusMax = 3,
        SkillAbilityDamageToPhysicalArmorPerPoint = 0,
        SkillAbilityDamageToMagicArmorPerPoint = 0,
        SkillAbilityArmorRestoredPerPoint = 5,
        SkillAbilityVitalityRestoredPerPoint = 5,
        SkillAbilityHighGroundBonusPerPoint = 5,
        SkillAbilityFireDamageBoostPerPoint = 5,
        SkillAbilityPoisonAndEarthDamageBoostPerPoint = 5,
        SkillAbilityAirDamageBoostPerPoint = 5,
        SkillAbilityWaterDamageBoostPerPoint = 5,
        SkillAbilityPhysicalDamageBoostPerPoint = 5,
        SkillAbilityLifeStealPerPoint = 10,
        LifestealFromReflectionModifier = 0,
        SkillAbilityCritMultiplierPerPoint = 5,
        SkillAbilityMovementSpeedPerPoint = 30,
        SkillAbilitySummonsStatsPerPoint = 4,
        SneakingAbilityMovementSpeedPerPoint = 6,
        TalentAttributePointsBonus = 2,
        TalentCombatAbilityPointsBonus = 1,
        TalentCivilAbilityPointsBonus = 1,
        TalentMemoryBonus = 3,
        TalentQuestRootedMemoryBonus = 3,
        TalentRagerPercentage = 70,
        SoftLevelCap = 20,
        PickpocketRequirementDecreaseFromFinesse = 1,
        SkillCombustionRadius = 3,
        TalentPerfectionistAccuracyBonus = 10,
        TalentPerfectionistCriticalChanceBonus = 10,
        TalentExecutionerActionPointBonus = 2,
        TalentPointOffset = 2,
        TalentViolentMagicCriticalChancePercent = 100,
        TalentPointPerLevels = 5,
        TalentQuickStepPartialApBonus = 1,
        SkillHeightRangeMultiplier = 1,
        AbilityPersuasionBonusPerPoint = 4,
        FreeMovementDistanceWhenAttacking = 1,
        TalentSneakingDamageBonus = 40,
        MaximumSummonsInCombat = 4,
        SpiritVisionFallbackRadius = 10,
        AbilityPerseveranceArmorPerPoint = 5,
        AiCoverProjectileTurnMemory = 2,
        CarryWeightBase = 0,
        CarryWeightPerStr = 10000,
        MoveToCarryWeightRatio = 0.75,
        TalentResistDeathVitalityPercentage = 20,
        DeflectProjectileRange = 1,
        SummoningAbilityBonus = 10,
        SurfaceAbsorbBoostPerTilesCount = 10,
        TalentWhatARushThreshold = 50,
        IncarnateSummoningLevel = 10,
        CleaveRangeOverride = 125,
        DamageToThrownWeightRatio = 0.5,
        FleeDistance = 13,
        GlobalGoldValueMultiplier = 1,
        PriceGrowth = 1.12,
        FirstPriceLeapLevel = 9,
        FirstPriceLeapGrowth = 1.75,
        SecondPriceLeapLevel = 13,
        SecondPriceLeapGrowth = 1.15,
        ThirdPriceLeapLevel = 16,
        ThirdPriceLeapGrowth = 1.5,
        FourthPriceLeapLevel = 18,
        FourthPriceLeapGrowth = 1.15,
        PriceModCasualDifficulty = 2.1,
        PriceModClassicDifficulty = 2.5,
        PriceModHardcoreDifficulty = 2.7,
        PriceBarterCoefficient = 0.1,
        PriceAttitudeCoefficient = 0.005,
        PriceRoundToFiveAfterAmount = 100,
        PriceRoundToTenAfterAmount = 1000,
        LevelCap = 35,
        GMCharacterAttributeCap = 100,
        GMCharacterArmorCap = 999999,
        GMCharacterResistanceMin = -1000,
        GMCharacterResistanceMax = 1000,
        GMCharacterAPCap = 100,
        GMCharacterSPCap = 3,
        GMItemLevelCap = 50,
        GMItemAttributeCap = 100,
        GMItemArmorMin = -999999,
        GMItemArmorMax = 999999,
        GMItemResistanceMin = -1000,
        GMItemResistanceMax = 1000,
        LoneWolfMaxAPBonus = 2,
        LoneWolfAPBonus = 2,
        LoneWolfMagicArmorBoostPercentage = 60,
        LoneWolfArmorBoostPercentage = 60,
        LoneWolfVitalityBoostPercentage = 30,
        LivingArmorHealPercentage = 35,
        TorturerDamageStatusTurnIncrease = 1,
        UnstableDamagePercentage = 50,
        UnstableRadius = 3,
        TalentResurrectExtraHealthPercent = 10,
        PoisonedFoodDamage = 1,
        PoisonedFoodDamageRange = 10,
        PoisonedFoodDamageMultiplier = 40,
        TraderDroppedItemsPercentage = 51,
        TraderDroppedItemsCap = 5,
        StatusDefaultDistancePerDamage = 0.75,
        TraderDonationsRequiredAttitude = -45,
        TeleportUnchainDistance = 50,
        TalentHumanCriticalMultiplier = 10,
        TalentHumanCriticalChance = 5,
        TalentSneakingAPCost = 1
    }
}

--- Returns the version number of the Osiris Extender
--- @return integer
function Ext.Version () end

--- Returns the version number of the game
--- @return string
function Ext.GameVersion () end

--- Loads the specified Lua file
--- @param modGuid string GUID of the module containing the Lua file
--- @param fileName string Path of Lua file, relative to Mods/<Mod>/ScriptExtender/Lua
function Ext.Require (modGuid, fileName) end

--- Registers a function to call when an extender event is thrown
--- @param event ExtEngineEvent Event to listen for
--- @param callback function Lua function to run when the event fires
function Ext.RegisterListener (event, callback) end

--- Registers a function that is called when certain Osiris functions are called.
--- Supports events, built-in queries, DBs, PROCs, QRYs (user queries).
--- @param name string Osiris function/database name
--- @param arity number Number of columns for DBs or the number of parameters (both IN and OUT) for functions
--- @param event string Event type ('before' - triggered before Osiris call; 'after' - after Osiris call; 'beforeDelete'/'afterDelete' - before/after delete from DB)
--- @param handler function Lua function to run when the event fires
function Ext.RegisterOsirisListener (name, arity, event, handler) end

--- Registers a new call in Osiris
--- @param func function Function to register
--- @param funcName string Name of call to register
--- @param arguments string Call argument list
function Ext.NewCall (func, funcName, arguments) end

--- Registers a new query in Osiris
--- @param func function Function to register
--- @param funcName string Name of query to register
--- @param arguments string Query argument list
function Ext.NewQuery (func, funcName, arguments) end

--- Registers a new event in Osiris
--- @param funcName string Name of event to register
--- @param arguments string Event argument list
function Ext.NewEvent (funcName, arguments) end

--- Print to console window and editor messages pane
function Ext.Print (...) end

--- Print warning to console window and editor messages pane
function Ext.PrintWarning (...) end

--- Print error to console window and editor messages pane
function Ext.PrintError (...) end

--- Parse a JSON document into a Lua object
--- @param json string JSON string to parse
--- @return any
function Ext.JsonParse (json) end

--- Converts a Lua value into a JSON document
--- @param val any Value to serialize
--- @return string JSON document
function Ext.JsonStringify (val) end

--- Returns whether the specified mod is loaded
--- @param modGuid string GUID of the module
--- @return boolean
function Ext.IsModLoaded (modGuid) end

--- Returns the list of loaded modules in load order
--- @return string[]
function Ext.GetModLoadOrder () end

--- Returns detailed information about the specified (loaded) module
--- @param modGuid string GUID of the module
--- @return ModInfo
function Ext.GetModInfo (modGuid) end

Ext.Stats = {}

--- Returns the list of loaded stat entries
--- @param type string|nil Type of stat entry to fetch (StatusData, SkillData, ...)
--- @return string[]
function Ext.Stats.GetAllStats (type) end

--- Returns the list of stat entries that were loaded before the specified mod
--- @param modGuid string Mod GUID to check
--- @param type string|nil Type of stat entry to fetch (StatusData, SkillData, ...)
--- @return string[]
function Ext.Stats.GetStatsLoadedBefore (modId, type) end

--- Registers a new skill property that can be triggered via SkillProperties
--- Stat syntax: data "SkillProperties" "EXT:<PROPERTY_NAME>[,<int>,<int>,<string>,<int>,<int>]"
--- The property name must always be preceded by the string "EXT:". 
--- Target contexts (SELF:, TARGET:, ...) and useing multiple actions in the same SkillProperties are supported.
--- Conditions for EXT: properties (i.e. "IF(COND):") are _NOT YET_ supported.
--- @param channel string Skill property name
--- @param defn CustomSkillProperty Event handlers for the skill property
function Ext.Stats.RegisterSkillProperty (name, defn) end

--- Replaces level scaling formula for the specified stat
--- @param statType string Stat entry type
--- @param attribute string Stat attribute name
--- @param func function Replacement scaling function
function Ext.Stats.StatSetLevelScaling (statType, attribute, func) end

--- Returns the property proxy of the specified stats entry
--- Returns level scaled values if the level parameter is not nil.
--- @param stat string Stat entry name
--- @param type string Stat entry type (i.e. SkillData, StatusData, etc.)
--- @param level integer|nil Level scaling level
--- @return StatEntryArmor|StatEntryCharacter|StatEntryObject|StatEntryPotion|StatEntryShield|StatEntrySkillData|StatEntryStatusData|StatEntryWeapon
function Ext.Stats.GetStat (stat, type, level) end

--- Creates a new stats entry on the server
--- @param name string Stat entry name
--- @param type string Stat entry type (i.e. SkillData, StatusData, etc.)
--- @param template string|nil When not nil, all properties are copied from the specified stats entry
--- @return StatEntryArmor|StatEntryCharacter|StatEntryObject|StatEntryPotion|StatEntryShield|StatEntrySkillData|StatEntryStatusData|StatEntryWeapon
function Ext.Stats.CreateStat (name, type, template) end

--- Synchronizes all modifications of the specified stat to all clients
--- @param name string Stat entry name
--- @param persist boolean|nil Persist stats entry to savegame?
function Ext.Stats.SyncStat (name, persist) end

--- Toggles whether the specified stats entry should be persisted to savegames
--- @param name string Stat entry name
--- @param persist boolean Persist stats entry to savegame?
function Ext.Stats.StatSetPersistence (name, persist) end

--- Returns the textual label assigned to the specified enumeration value
--- @param enum string Engine enumeration name
--- @param index number Value index to look up
--- @return string|nil
function Ext.Stats.EnumIndexToLabel (enum, index) end

--- Returns the numeric index assigned to the specified enumeration label
--- @param enum string Engine enumeration name
--- @param label string Value name to look for
--- @return number|nil
function Ext.Stats.EnumLabelToIndex (enum, label) end

-- Execute the SkillProperties of the specified skill on a target character.
--- @param skillId string Stats skill ID
--- @param attacker ObjectHandle|int|string Attacker character handle/NetID/GUID
--- @param target ObjectHandle|int|string Target character handle/NetID/GUID
--- @param position number[]
--- @param propertyContext string Target|AoE|Self|SelfOnHit|SelfOnEquip
--- @param isFromItem boolean
function Ext.Stats.ExecuteSkillPropertiesOnTarget(skillId, attacker, target, position, propertyContext, isFromItem)

-- Execute the SkillProperties of the specified skill on a position.
--- @param skillId string Stats skill ID
--- @param attacker ObjectHandle|int|string Attacker character handle/NetID/GUID
--- @param position number[]
--- @param radius number
--- @param propertyContext string Target|AoE|Self|SelfOnHit|SelfOnEquip
--- @param isFromItem boolean
function Ext.Stats.ExecuteSkillPropertiesOnPosition(skillId, attacker, position, radius, propertyContext, isFromItem)

--- Returns the transformation rules that are applied when two neighbouring surfaces interact.
--- @return SurfaceInteractionSet[][]
function Ext.GetSurfaceTransformRules () end

--- Returns the surface template for the specified surface type
--- @param type string See SurfaceType enumeration
--- @return SurfaceTemplate
function Ext.GetSurfaceTemplate (type) end

--- Updates the transformation rules that are applied when two neighbouring surfaces interact.
--- @param rules SurfaceInteractionSet[][] New rules to apply
function Ext.UpdateSurfaceTransformRules (rules) end

--- Prepares a new surface action for execution
--- @param type string Surface action type
--- @return EsvSurfaceAction
function Ext.CreateSurfaceAction (type) end

--- Executes a surface action
--- @param action EsvSurfaceAction Action to execute
function Ext.ExecuteSurfaceAction (action) end

--- CAncels a surface action
--- @param actionHandle integer Action to cancel
function Ext.CancelSurfaceAction (actionHandle) end

--- Starts creating a new item using template GUID or cloning an existing item.
--- @param from EsvItem|string Template GUID or item to clone
--- @param recursive boolean|nil Copy items in container? (cloning only)
--- @return ItemConstructor
function Ext.CreateItemConstructor (from, recursive) end

--- Returns the GUID of all characters on the specified level. 
--- Uses the current level if no level name was specified.
--- @param level string|nil Optional level name
--- @return string[]
function Ext.GetAllCharacters (level) end

--- Returns the GUID of all characters within a radius around the specified point.
--- @param x number
--- @param y number
--- @param z number
--- @param distance number
--- @return string[]
function Ext.GetCharactersAroundPosition (x, y, z, distance) end

--- Returns the GUID of all items on the specified level. 
--- Uses the current level if no level name was specified.
--- @param level string|nil Optional level name
--- @return string[]
function Ext.GetAllItems (level) end

--- Returns the GUID of all items within a radius around the specified point.
--- @param x number
--- @param y number
--- @param z number
--- @param distance number
--- @return string[]
function Ext.GetItemsAroundPosition (x, y, z, distance) end

--- Returns the property proxy of the specified character
--- @param id string|integer|ObjectHandle Character GUID or handle or NetID
--- @return EsvCharacter|EclCharacter
function Ext.GetCharacter (id) end

--- Returns the property proxy of the specified item
--- @param id string|integer|ObjectHandle Item GUID or handle or NetID
--- @return EsvItem|EclCharacter
function Ext.GetItem (id) end

--- Returns the property proxy of the specified item
--- @param handle ObjectHandle Game object handle
--- @return EsvGameObject|EclGameObject
function Ext.GetGameObject (handle) end

--- Returns the property proxy of the specified surface
--- @param handle ObjectHandle Surface handle
--- @return EsvSurface
function Ext.GetSurface (handle) end

--- Returns the property proxy of the specified status
--- @param character string|integer|ObjectHandle Character GUID or handle or NetID
--- @param handle integer|ObjectHandle Status handle or NetID
--- @return EsvStatus
function Ext.GetStatus (character, handle) end

--- Returns the specified turn-based combat
--- @param combatId integer Combat ID
--- @return EsvCombat
function Ext.GetCombat (combatId) end

--- Creates a new damage list object
--- @return DamageList
function Ext.NewDamageList () end

--- Returns whether Osiris is currently accessible or not.
--- @return boolean
function Ext.OsirisIsCallable () end

--- Returns a random number; equivalent to Lua random
--- @param low integer
--- @param up integer
--- @return integer|number
function Ext.Random (low, up) end

--- Rounds the specified number
--- @param n number
--- @return number
function Ext.Round (n) end

--- Generate Lua IDE helpers for the currently loaded module
--- @param builtin boolean|nil Only export built-in functions and names exported by Lua?
function Ext.GenerateIdeHelpers (builtin) end

--- Returns whether the code is executing in a client context
--- @return boolean
function Ext.IsClient () end

--- Returns whether the code is executing in a server context
--- @return boolean
function Ext.IsServer () end

--- Returns whether the Developer Mode switch is enabled
--- @return boolean
function Ext.IsDeveloperMode () end

--- Returns the current client/server game state machine state.
--- @return string
function Ext.GetGameState () end

--- Broadcast a message to all peers
--- @param channel string Channel that will receive the message
--- @param payload string Message payload
--- @param excludeCharacter string|nil Optional peer to exclude from broadcast
function Ext.BroadcastMessage (channel, payload, excludeCharacter) end

--- Sends a message to the peer that controls the specified character
--- @param characterGuid string Character that will receive the message
--- @param channel string Channel that will receive the message
--- @param payload string Message payload
function Ext.PostMessageToClient (characterGuid, channel, payload) end

--- Sends a message to the specified peer
--- @param userId number User that will receive the message
--- @param channel string Channel that will receive the message
--- @param payload string Message payload
function Ext.PostMessageToUser (userId, channel, payload) end

--- Sends a message to the server
--- @param channel string Channel that will receive the message
--- @param payload string Message payload
function Ext.PostMessageToServer (channel, payload) end

--- Registers a listener that is called when a network message is received on the specified channel
--- @param channel string Network channel name
--- @param handler function Lua handler
function Ext.RegisterNetListener (channel, handler) end

--- Registers a new dialog voice line for the specified speaker.
--- @param speakerGuid string Speaker character UUID
--- @param textKey string Translated string key of text line
--- @param path string Path to audio .WEM
--- @param length number Length of audio in seconds
function Ext.AddVoiceMetaData (speakerGuid, textKey, path, length) end

--- @param handle string Translated string handle
--- @param fallback string Fallback string if the specified handle is not found
--- @return string Translated string
function Ext.GetTranslatedString (handle, fallback) end

--- @param key string Translated string key
--- @return string,string Translated string and handle
function Ext.GetTranslatedStringFromKey (key) end

--- @param key string Translated string key
--- @param handle string Translated string handle
--- @return bool
function Ext.CreateTranslatedStringKey (key, handle) end

--- @param handle string Translated string handle
--- @param text string Display text
--- @return bool
function Ext.CreateTranslatedStringHandle (handle, text) end

--- @param key string Translated string key
--- @param text string Display text
--- @return string|nil Created string handle
function Ext.CreateTranslatedString (key, text) end

--- Redirects all file accesses to the specified path to another file.
--- @param path string Original path
--- @param newPath string New (redirected) path
function Ext.AddPathOverride (path, newPath) end

--- Constructs a new Flash UI element
--- @param name string User-defined unique name that identifies the UI element
--- @param path string Path of the SWF file relative to the data directory
--- @param layer integer Stack order of the UI element
--- @return UIObject|nil
function Ext.CreateUI (name, path, layer) end

--- Retrieves an UI element with the specified name
--- @param name string User-defined unique name that identifies the UI element
--- @return UIObject|nil
function Ext.GetUI (name) end

--- Retrieves a built-in UI element at the specified path.
--- If no such element exists, the function returns nil.
--- @param path string UI SWF path relative to Data\
--- @return UIObject|nil
function Ext.GetBuiltinUI (path) end

--- Retrieves an engine UI element with the specified engine type ID.
--- If no such element exists, the function returns nil.
--- @param typeId number Engine UI element type ID
--- @return UIObject|nil
function Ext.GetUIByType (typeId) end

--- Destroys the specified UI element
--- @param name string User-defined unique name that identifies the UI element
function Ext.DestroyUI (name) end

--- Refresh the UI of the specified character
--- @param character ObjectHandle Handle of character
--- @param flags integer UI elements to refresh
function Ext.UISetDirty (character, flags) end

--- Registers a listener that is called when the specified function is called from Flash
--- @param object UIObject UI object returned from Ext.CreateUI, Ext.GetUI or Ext.GetBuiltinUI
--- @param name string ExternalInterface function name
--- @param handler function Lua handler
--- @param type string|nil Event type - 'Before' or 'After'
function Ext.RegisterUICall (object, name, handler, type) end

--- Registers a listener that is called when the specified function is called from Flash.
--- The event is triggered for every UI element with the specified type ID.
--- @param typeId number Engine UI element type ID
--- @param name string ExternalInterface function name
--- @param handler function Lua handler
--- @param type string|nil Event type - 'Before' or 'After'
function Ext.RegisterUITypeCall (typeId, name, handler, type) end

--- Registers a listener that is called when the specified function is called from Flash.
--- The event is triggered regardless of which UI element it was called on.
--- (Function call capture must be enabled for every element type that needs to monitored!)
--- @param name string ExternalInterface function name
--- @param handler function Lua handler
--- @param type string|nil Event type - 'Before' or 'After'
function Ext.RegisterUINameCall (name, handler, type) end

--- Registers a listener that is called when the specified method is called on the main timeline of the Flash object
--- @param object UIObject UI object returned from Ext.CreateUI, Ext.GetUI or Ext.GetBuiltinUI
--- @param method string Flash method name
--- @param handler function Lua handler
--- @param type string|nil Event type - 'Before' or 'After'
function Ext.RegisterUIInvokeListener (object, name, handler, type) end

--- Registers a listener that is called when the specified method is called on the main timeline of the Flash object
--- The event is triggered for every UI element with the specified type ID.
--- @param typeId number Engine UI element type ID
--- @param method string Flash method name
--- @param handler function Lua handler
--- @param type string|nil Event type - 'Before' or 'After'
function Ext.RegisterUITypeInvokeListener (typeId, name, handler, type) end

--- Registers a listener that is called when the specified method is called on the main timeline of the Flash object
--- The event is triggered regardless of which UI element it was called on.
--- @param method string Flash method name
--- @param handler function Lua handler
--- @param type string|nil Event type - 'Before' or 'After'
function Ext.RegisterUINameInvokeListener (name, handler, type) end

--- Registers a listener that is called when a console command is entered in the dev console
--- @param cmd string Console command
--- @param handler function Lua handler
function Ext.RegisterConsoleCommand (cmd, handler) end

--- Write data to an external (persistent) file
--- @param path string File path relative to Documents\Larian Studios\Divinity Original Sin 2 Definitive Edition\Osiris Data
--- @param contents string File contents to write
function Ext.SaveFile (path, contents) end

--- Read data from an external (persistent) file
--- @param path string File path relative to Documents\Larian Studios\Divinity Original Sin 2 Definitive Edition\Osiris Data
--- @param context string|nil Path context (nil or "user" means relative to the Osiris Data directory; "data" means relative to game data path)
--- @return string File contents
function Ext.LoadFile (path, context) end

--- Returns a monotonic value representing the current time in milliseconds.
--- Useful for performance measurements / measuring real world time.
--- (Note: This value is not synchronized between peers and different clients may report different time values!)
--- @return number Time
function Ext.MonotonicTime () end

--- Returns whether the player has a compatible Script Extender version installed
--- @param playerGuid string GUID of player character
--- @return boolean
function Ext.PlayerHasExtender (playerGuid) end

--- Triggers a breakpoint in the Lua debugger.
--- If no debugger is connected, the function does nothing.
function Ext.DebugBreak () end

Ext.Audio = {}

--- Set audio state switch
--- @param gameObject ObjectHandle|string|nil Character handle, built-in sound object name or nil
--- @param switchGroup string Switch group name
--- @param state string Switch state
--- @return boolean
function Ext.Audio.SetSwitch (gameObject, switchGroup, state) end

--- Set audio state
--- @param stateGroup string State group name
--- @param state string State
--- @return boolean
function Ext.Audio.SetState (stateGroup, state) end

--- Set real-time parameter (RTPC) value
--- @param gameObject ObjectHandle|string|nil Character handle, built-in sound object name or nil
--- @param rtpc string Parameter name
--- @param value number Parameter value
--- @return boolean
function Ext.Audio.SetRTPC (gameObject, rtpc, value) end

--- Reset real-time parameter (RTPC) value
--- @param gameObject ObjectHandle|string|nil Character handle, built-in sound object name or nil
--- @param rtpc string Parameter Name
--- @return boolean
function Ext.Audio.ResetRTPC (gameObject, rtpc) end

--- Get real-time parameter (RTPC) value
--- @param gameObject ObjectHandle|string|nil Character handle, built-in sound object name or nil
--- @param rtpc string Parameter name
--- @return number
function Ext.Audio.GetRTPC (gameObject, rtpc) end

--- Stop audio
--- @param gameObject ObjectHandle|string|nil Character handle, built-in sound object name or nil
function Ext.Audio.Stop (gameObject) end

--- Pause audio playback
function Ext.Audio.PauseAllSounds () end

--- Resume audio playback
function Ext.Audio.ResumeAllSounds () end

--- Trigger an audio event
--- @param gameObject ObjectHandle|string|nil Character handle, built-in sound object name or nil
--- @param eventName string Event to trigger
--- @param positionSec number|nil Position in audio track
--- @return boolean
function Ext.Audio.PostEvent (gameObject, eventName, positionSec) end

--- Trigger an audio event using an external audio file
--- @param gameObject ObjectHandle|string|nil Character handle, built-in sound object name or nil
--- @param eventName string Event to trigger
--- @param path string Audio file path (relative to data directory)
--- @param codecId integer Codec ID
--- @return boolean
function Ext.Audio.PlayExternalSound (gameObject, eventName, path, codecId) end
