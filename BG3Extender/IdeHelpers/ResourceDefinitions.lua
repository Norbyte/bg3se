
--- @class UUID

--- @class BaseResourceDefinition
--- @field public ResourceUUID UUID


--- @class ActionResource : BaseResourceDefinition
--- @field public Name string
--- @field public DisplayName string
--- @field public Description string
--- @field public Error string
--- @field public MaxLevel integer
--- @field public DiceType integer
--- @field public ReplenishType integer
--- @field public ShowOnActionResourcePanel boolean
--- @field public UpdatesSpellPowerLevel boolean
--- @field public PartyActionResource boolean
--- @field public IsSpellResource boolean


--- @class ClassDescriptionResource : BaseResourceDefinition
--- @field public ParentGuid string
--- @field public Name string
--- @field public DisplayName string
--- @field public Description string
--- @field public ProgressionTableUUID UUID
--- @field public MagicType integer
--- @field public SoundMagicType integer
--- @field public PrimaryAbility integer
--- @field public SpellCastingAbility integer
--- @field public MustPrepareSpells boolean
--- @field public CanLearnSpells boolean
--- @field public Tags UUID[]
--- @field public ClassEquipment string
--- @field public CharacterCreationPose integer
--- @field public LearningStrategy integer
--- @field public BaseHp integer
--- @field public HpPerLevel integer


--- @class TagResource : BaseResourceDefinition
--- @field public Name string
--- @field public Description string
--- @field public Icon string
--- @field public Categories integer
--- @field public DisplayName string
--- @field public DisplayDescription string


--- @class FactionResource : BaseResourceDefinition
--- @field public Faction string
--- @field public ParentGuid string


--- @class RaceResource : BaseResourceDefinition
--- @field public Name string
--- @field public DisplayName string
--- @field public Description string
--- @field public ParentGuid UUID
--- @field public Tags UUID[]
--- @field public ProgressionTableUUID UUID
--- @field public RaceSoundSwitch string
--- @field public HairColors UUID[]
--- @field public SkinColors UUID[]
--- @field public EyeColors UUID[]
--- @field public TattooColors UUID[]
--- @field public MakeupColors UUID[]
--- @field public Visuals UUID[]
--- @field public Gods UUID[]
--- @field public ExcludedGods UUID[]


--- @class OriginResource : BaseResourceDefinition
--- @field public Name string
--- @field public DisplayName string
--- @field public Description string
--- @field public Gender integer
--- @field public RaceUUID UUID
--- @field public SubRaceUUID UUID
--- @field public BackgroundUUID UUID
--- @field public GodUUID UUID
--- @field public ClassUUID UUID
--- @field public SubClassUUID UUID
--- @field public LockClass boolean
--- @field public GlobalTemplate UUID
--- @field public Passives string
--- @field public Tags UUID[]
--- @field public Flags integer
--- @field public Overview string
--- @field public CloseUpA string
--- @field public CloseUpB string
--- @field public ClassEquipmentOverride string
--- @field public VoiceTableUUID UUID


--- @class BackgroundResource : BaseResourceDefinition
--- @field public DisplayName string
--- @field public Description string
--- @field public Passives string
--- @field public Tags UUID[]


--- @class GodResource : BaseResourceDefinition
--- @field public Name string
--- @field public DisplayName string
--- @field public Description string
--- @field public Tags UUID[]


--- @class ProgressionResource : BaseResourceDefinition
--- @field public TableUUID UUID
--- @field public Name string
--- @field public SubClasses UUID[]
--- @field public PassivesAdded string
--- @field public PassivesRemoved string
--- @field public Boosts string
--- @field public ProgressionType integer
--- @field public Level integer
--- @field public AllowImprovement boolean
--- @field public SelectAbilities UUID[]
--- @field public SelectSkills UUID[]
--- @field public SelectSpells UUID[]
--- @field public SelectPassives UUID[]
--- @field public AddSpells UUID[]


--- @class ProgressionDescriptionResource : BaseResourceDefinition
--- @field public DisplayName string
--- @field public Description string
--- @field public Hidden boolean
--- @field public ExactMatch string
--- @field public Type string
--- @field public ParamMatch string
--- @field public ProgressionId UUID
--- @field public ProgressionTableId UUID
--- @field public SelectorId string
--- @field public PassivePrototype string


--- @class GossipDefinition : BaseResourceDefinition
--- @field public Name string
--- @field public Type string
--- @field public Priority integer
--- @field public ConditionFlags UUID[]
--- @field public ResultFlags UUID[]
--- @field public DialogUUID UUID


--- @class ActionResourceGroupDefinition : BaseResourceDefinition
--- @field public Name string
--- @field public ActionResourceDefinitions UUID[]


--- @class ColorDefinitionResource : BaseResourceDefinition
--- @field public Name string
--- @field public DisplayName string
--- @field public Color number[]


--- @class EquipmentTypeResource : BaseResourceDefinition
--- @field public Name string
--- @field public WeaponType_OneHanded string
--- @field public WeaponType_TwoHanded string
--- @field public BoneMainSheathed string
--- @field public BoneMainUnsheathed string
--- @field public BoneOffHandSheathed string
--- @field public BoneOffHandUnsheathed string
--- @field public BoneVersatileSheathed string
--- @field public BoneVersatileUnsheathed string
--- @field public SourceBoneSheathed string
--- @field public SourceBoneVersatileSheathed string
--- @field public SourceBoneVersatileUnsheathed string
--- @field public SoundAttackType string
--- @field public SoundEquipmentType string


--- @class FlagResource : BaseResourceDefinition
--- @field public Name string
--- @field public Description string
--- @field public Usage integer


--- @class FeatResource : BaseResourceDefinition
--- @field public Name string
--- @field public Requirements string
--- @field public PassivesAdded string
--- @field public PassivesRemoved string
--- @field public Boosts string
--- @field public SelectAbilities UUID[]
--- @field public SelectSkills UUID[]
--- @field public SelectSpells UUID[]
--- @field public SelectPassives UUID[]
--- @field public AddSpells UUID[]


--- @class FeatDescriptionResource : BaseResourceDefinition
--- @field public DisplayName string
--- @field public Description string
--- @field public Hidden boolean
--- @field public ExactMatch string
--- @field public Type string
--- @field public ParamMatch string
--- @field public FeatId UUID
--- @field public SelectorId string
--- @field public PassivePrototype string
