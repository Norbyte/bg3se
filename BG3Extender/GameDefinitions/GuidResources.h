#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_NS(resource)

#define FOR_EACH_GUID_RESOURCE_TYPE() \
	FOR_RESOURCE_TYPE(ActionResource) \
	FOR_RESOURCE_TYPE(ClassDescription) \
	FOR_RESOURCE_TYPE(Tag) \
	FOR_RESOURCE_TYPE(Faction) \
	FOR_RESOURCE_TYPE(Race) \
	FOR_RESOURCE_TYPE(Origin) \
	FOR_RESOURCE_TYPE(Background) \
	FOR_RESOURCE_TYPE(God) \
	FOR_RESOURCE_TYPE(Progression) \
	FOR_RESOURCE_TYPE(ProgressionDescription) \
	FOR_RESOURCE_TYPE(Gossip) \
	FOR_RESOURCE_TYPE(ActionResourceGroup) \
	FOR_RESOURCE_TYPE(Color) \
	FOR_RESOURCE_TYPE(EquipmentType) \
	FOR_RESOURCE_TYPE(Flag) \
	FOR_RESOURCE_TYPE(Feat) \
	FOR_RESOURCE_TYPE(FeatDescription) \
	FOR_RESOURCE_TYPE(PassiveList) \
	FOR_RESOURCE_TYPE(SkillList) \
	FOR_RESOURCE_TYPE(SpellList) \
	FOR_RESOURCE_TYPE(AbilityList) \
	FOR_RESOURCE_TYPE(CompanionPreset) \
	FOR_RESOURCE_TYPE(AbilityDistributionPreset) \
	FOR_RESOURCE_TYPE(CharacterCreationAccessorySet) \
	FOR_RESOURCE_TYPE(CharacterCreationAppearanceMaterial) \
	FOR_RESOURCE_TYPE(CharacterCreationAppearanceVisual) \
	FOR_RESOURCE_TYPE(CharacterCreationEquipmentIcons) \
	FOR_RESOURCE_TYPE(CharacterCreationIconSettings) \
	FOR_RESOURCE_TYPE(CharacterCreationEyeColor) \
	FOR_RESOURCE_TYPE(CharacterCreationHairColor) \
	FOR_RESOURCE_TYPE(CharacterCreationSkinColor) \
	FOR_RESOURCE_TYPE(CharacterCreationMaterialOverride) \
	FOR_RESOURCE_TYPE(CharacterCreationPassiveAppearance) \
	FOR_RESOURCE_TYPE(CharacterCreationPreset) \
	FOR_RESOURCE_TYPE(CharacterCreationSharedVisual) \
	FOR_RESOURCE_TYPE(LevelMap) \
	FOR_RESOURCE_TYPE(ApprovalRating) \
	FOR_RESOURCE_TYPE(AreaLevelOverride) \
	FOR_RESOURCE_TYPE(AvatarContainerTemplates) \
	FOR_RESOURCE_TYPE(BackgroundGoal) \
	FOR_RESOURCE_TYPE(CalendarDayRange) \
	FOR_RESOURCE_TYPE(CampChestTemplate) \
	FOR_RESOURCE_TYPE(AbilityDefaultValues) \
	FOR_RESOURCE_TYPE(EquipmentDefaultValues) \
	FOR_RESOURCE_TYPE(FeatDefaultValues) \
	FOR_RESOURCE_TYPE(PassiveDefaultValues) \
	FOR_RESOURCE_TYPE(PreparedSpellDefaultValues) \
	FOR_RESOURCE_TYPE(SkillDefaultValues) \
	FOR_RESOURCE_TYPE(SpellDefaultValues) \
	FOR_RESOURCE_TYPE(CharacterCreationVOLine) \
	FOR_RESOURCE_TYPE(CinematicArenaFrequencyGroup) \
	FOR_RESOURCE_TYPE(CustomDiceTemplate) \
	FOR_RESOURCE_TYPE(ConditionErrorDescription) \
	FOR_RESOURCE_TYPE(DeathTypeEffect) \
	FOR_RESOURCE_TYPE(DifficultyClass) \
	FOR_RESOURCE_TYPE(DisturbanceProperty) \
	FOR_RESOURCE_TYPE(DLC) \
	FOR_RESOURCE_TYPE(EncumbranceType) \
	FOR_RESOURCE_TYPE(EquipmentList) \
	FOR_RESOURCE_TYPE(ExperienceRewards) \
	FOR_RESOURCE_TYPE(FeatSoundState) \
	FOR_RESOURCE_TYPE(FlagSoundState) \
	FOR_RESOURCE_TYPE(GoldRewards) \
	FOR_RESOURCE_TYPE(FixedHotBarSlot) \
	FOR_RESOURCE_TYPE(ItemThrowParams) \
	FOR_RESOURCE_TYPE(ItemWallTemplate) \
	FOR_RESOURCE_TYPE(TrajectoryRule) \
	FOR_RESOURCE_TYPE(LimbsMapping) \
	FOR_RESOURCE_TYPE(LongRestCost) \
	FOR_RESOURCE_TYPE(ManagedStatusVFX) \
	FOR_RESOURCE_TYPE(MultiEffectInfo) \
	FOR_RESOURCE_TYPE(OriginIntroEntity) \
	FOR_RESOURCE_TYPE(PassivesVFX) \
	FOR_RESOURCE_TYPE(ProjectileDefault) \
	FOR_RESOURCE_TYPE(RandomCastOutcome) \
	FOR_RESOURCE_TYPE(SoundSpellTrajectoryRule) \
	FOR_RESOURCE_TYPE(StatusSoundState) \
	FOR_RESOURCE_TYPE(SurfaceCursorMessage) \
	FOR_RESOURCE_TYPE(TadpolePower) \
	FOR_RESOURCE_TYPE(TagSoundState) \
	FOR_RESOURCE_TYPE(TooltipExtraText) \
	FOR_RESOURCE_TYPE(TooltipUpcastDescription) \
	FOR_RESOURCE_TYPE(TutorialEntry) \
	FOR_RESOURCE_TYPE(TutorialModalEntry) \
	FOR_RESOURCE_TYPE(TutorialUnifiedEntry) \
	FOR_RESOURCE_TYPE(TutorialEvent) \
	FOR_RESOURCE_TYPE(VFX) \
	FOR_RESOURCE_TYPE(Voice) \
	FOR_RESOURCE_TYPE(WeaponAnimationSet) \
	FOR_RESOURCE_TYPE(WeightCategories) \
	FOR_RESOURCE_TYPE(ShapeshiftRule) \
	FOR_RESOURCE_TYPE(AnimationSetPriority) \
	FOR_RESOURCE_TYPE(SpellMetaCondition) \
	FOR_RESOURCE_TYPE(ScriptMaterialParameterOverride) \
	FOR_RESOURCE_TYPE(ScriptMaterialPresetOverride) \
	FOR_RESOURCE_TYPE(VisualLocatorAttachment) \
	FOR_RESOURCE_TYPE(RulesetModifier) \
	FOR_RESOURCE_TYPE(RulesetModifierOption) \
	FOR_RESOURCE_TYPE(Ruleset) \
	FOR_RESOURCE_TYPE(RulesetValue) \
	FOR_RESOURCE_TYPE(RulesetSelectionPreset)


struct GuidResource : ProtectedGameObject<GuidResource>
{
	[[bg3::hidden]]
	void* VMT;
	Guid ResourceUUID;
};

struct [[bg3::hidden]] GuidResourceBankBase : ProtectedGameObject<GuidResourceBankBase>
{
	void* VMT;
	FixedString LSXRegionName;
	FixedString LSXResourceNodeName;
	MultiHashMap<Guid, Array<Guid>> ResourceGuidsByMod;
};

template <class T>
struct [[bg3::hidden]] GuidResourceBank : public GuidResourceBankBase
{
	MultiHashMap<Guid, T> Resources;
	STDString Path;
	RefMap<Guid, void*> field_E0;
};


struct [[bg3::hidden]] GuidResourceManager
{
	MultiHashMap<int32_t, GuidResourceBankBase*> Definitions;
};


struct ActionResource : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ActionResource;
	static constexpr auto EngineClass = "eoc::ActionResourceTypes";

	FixedString Name;
	TranslatedString DisplayName;
	TranslatedString Description;
	TranslatedString Error;
	uint32_t MaxLevel;
	double MaxValue;
	DiceSizeId DiceType;
	ResourceReplenishType ReplenishType;
	bool ShowOnActionResourcePanel;
	bool UpdatesSpellPowerLevel;
	bool PartyActionResource;
	bool IsSpellResource;
	bool IsHidden;
};


struct ClassDescription : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ClassDescription;
	static constexpr auto EngineClass = "eoc::ClassDescriptions";

	Guid ParentGuid;
	FixedString Name;
	TranslatedString DisplayName;
	TranslatedString Unused;
	TranslatedString SubclassTitle;
	TranslatedString Description;
	Guid ProgressionTableUUID;
	FixedString SoundClassType;
	AbilityId PrimaryAbility;
	AbilityId SpellCastingAbility;
	bool MustPrepareSpells;
	bool CanLearnSpells;
	bool HasGod;
	bool IsDefaultForUseSpellAction;
	bool IsSomaticWithInstrument;
	Array<Guid> Tags;
	FixedString ClassEquipment;
	FixedString CharacterCreationPose;
	SpellLearningStrategy LearningStrategy;
	uint8_t field_71;
	int BaseHp;
	int HpPerLevel;
	int CommonHotbarColumns;
	int ClassHotbarColumns;
	int ItemsHotbarColumns;
	int AnimationSetPriority;
	double MulticlassSpellcasterModifier;
	Guid SpellList;
};


struct Tag : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Tag;
	static constexpr auto EngineClass = "ls::TagManager";

	FixedString Name;
	STDString Description;
	FixedString Icon;
	uint32_t Categories; // FIXME - map to flags
	uint32_t Properties; // FIXME - map to flags
	TranslatedString DisplayName;
	TranslatedString DisplayDescription;
};


struct Faction : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Faction;
	static constexpr auto EngineClass = "eoc::FactionContainer";

	FixedString Faction;
	Guid ParentGuid;
};


struct Race : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Race;
	static constexpr auto EngineClass = "eoc::RaceManager";

	FixedString Name;
	TranslatedString DisplayName;
	TranslatedString Description;
	Guid ParentGuid;
	Guid DisplayTypeUUID;
	Array<Guid> Tags;
	Guid ProgressionTableUUID;
	FixedString RaceSoundSwitch;
	FixedString RaceEquipment;
	Array<Guid> HairColors;
	Array<Guid> SkinColors;
	Array<Guid> EyeColors;
	Array<Guid> TattooColors;
	Array<Guid> MakeupColors;
	Array<Guid> LipsMakeupColors;
	Array<Guid> HairHighlightColors;
	Array<Guid> HairGrayingColors;
	Array<Guid> HornColors;
	Array<Guid> HornTipColors;
	Array<Guid> Visuals;
	Array<Guid> Gods;
	Array<Guid> ExcludedGods;
};


struct Origin : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Origin;
	static constexpr auto EngineClass = "eoc::OriginManager";

	uint8_t AvailableInCharacterCreation;
	FixedString Name;
	TranslatedString DisplayName;
	TranslatedString Description;
	uint8_t BodyType;
	uint8_t BodyShape;
	bool LockBody;
	Guid RaceUUID;
	Guid SubRaceUUID;
	bool LockRace;
	Guid BackgroundUUID;
	Guid GodUUID;
	Guid ClassUUID;
	Guid SubClassUUID;
	bool LockClass;
	Guid GlobalTemplate;
	Guid DefaultsTemplate;
	STDString Passives;
	[[bg3::hidden]]
	Array<void*> field_C8;
	Array<Guid> AppearanceTags;
	Array<Guid> ReallyTags;
	uint32_t Flags;
	STDString Overview;
	STDString CloseUpA;
	STDString CloseUpB;
	FixedString ClassEquipmentOverride;
	Guid VoiceTableUUID;
	Guid IntroDialogUUID;
	bool IsHenchman;
	uint8_t Identity;
	Guid ExcludesOriginUUID;
};


struct Background : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Background;
	static constexpr auto EngineClass = "eoc::BackgroundManager";

	TranslatedString DisplayName;
	TranslatedString Description;
	STDString Passives;
	__int64 field_40;
	__int64 field_48;
	Array<Guid> Tags;
	bool Hidden;
};


struct God : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::God;
	static constexpr auto EngineClass = "eoc::GodManager";

	FixedString Name;
	TranslatedString DisplayName;
	TranslatedString Description;
	Array<Guid> Tags;
};


struct Progression : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Progression;
	static constexpr auto EngineClass = "eoc::ProgressionManager";

	struct Spell
	{
		Guid SpellUUID;
		int32_t Amount;
		int32_t Arg3;
		STDString SelectorId;
		AbilityId CastingAbility;
		Guid ActionResource;
		SpellPrepareType PrepareType;
		SpellCooldownType CooldownType;
		Guid ClassUUID;
	};
	
	struct AddedSpell
	{
		Guid SpellUUID;
		STDString SelectorId;
		AbilityId Ability;
		Guid ActionResource;
		SpellPrepareType PrepareType;
		SpellCooldownType CooldownType;
		Guid ClassUUID;
	};
	
	struct Ability
	{
		Guid UUID;
		int32_t Arg2;
		int32_t Arg3;
		STDString Arg4;
	};


	struct AbilityBonus
	{
		Guid UUID;
		int32_t Amount;
		Array<int32_t> Amounts;
		STDString BonusType;
	};

	struct Skill
	{
		Guid UUID;
		int32_t Amount;
		STDString Arg3;
	};

	struct SkillExpertise
	{
		Guid UUID;
		int32_t Amount;
		bool Arg3;
		STDString Arg4;
	};

	struct Passive
	{
		Guid UUID;
		int32_t Amount;
		int32_t Amount2;
		STDString Arg3;
	};

	struct Equipment
	{
		Guid UUID;
		int32_t Amount;
		STDString Arg3;
	};

	Guid TableUUID;
	STDString Name;
	Array<Guid> SubClasses;
	[[bg3::hidden]]
	Array<void*> field_50;
	STDString PassivesAdded;
	[[bg3::hidden]]
	Array<void*> field_78;
	STDString PassivesRemoved;
	[[bg3::hidden]]
	Array<void*> field_A0;
	STDString Boosts;
	uint8_t ProgressionType; // FIXME - map to enumeration
	uint8_t Level;
	bool AllowImprovement;
	Array<FixedString> field_D0;
	bool IsMulticlass;
	Array<Ability> SelectAbilities;
	Array<AbilityBonus> SelectAbilityBonus;
	Array<Skill> SelectSkills;
	Array<SkillExpertise> SelectSkillsExpertise;
	Array<Spell> SelectSpells;
	Array<Passive> SelectPassives;
	Array<Equipment> SelectEquipment;
	Array<AddedSpell> AddSpells;
};


struct ProgressionDescription : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ProgressionDescription;
	static constexpr auto EngineClass = "eoc::ProgressionDescriptionManager";

	TranslatedString DisplayName;
	TranslatedString Description;
	bool Hidden;
	FixedString ExactMatch;
	FixedString Type;
	FixedString ParamMatch;
	Guid ProgressionId;
	Guid ProgressionTableId;
	FixedString SelectorId;
	FixedString PassivePrototype;
};


struct Gossip : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Gossip;
	static constexpr auto EngineClass = "eoc::GossipContainer";

	FixedString Name;
	FixedString Type;
	int Priority;
	Array<Guid> ConditionFlags;
	Array<Guid> ResultFlags;
	Guid DialogUUID;
};


struct ActionResourceGroup : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ActionResourceGroup;
	static constexpr auto EngineClass = "eoc::ActionResourceGroupManager";

	STDString Name;
	TranslatedString field_38;
	TranslatedString field_48;
	Array<Guid> ActionResourceDefinitions;
};


struct Color : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ColorDefinition;
	static constexpr auto EngineClass = "eoc::ColorDefinitions";

	FixedString Name;
	TranslatedString DisplayName;
	glm::vec4 Color;
};


struct EquipmentType : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::EquipmentType;
	static constexpr auto EngineClass = "eoc::EquipmentTypes";

	FixedString Name;
	FixedString WeaponType_OneHanded;
	FixedString WeaponType_TwoHanded;
	FixedString BoneMainSheathed;
	FixedString BoneMainUnsheathed;
	FixedString BoneOffHandSheathed;
	FixedString BoneOffHandUnsheathed;
	FixedString BoneVersatileSheathed;
	FixedString BoneVersatileUnsheathed;
	FixedString SourceBoneSheathed;
	FixedString SourceBoneVersatileSheathed;
	FixedString SourceBoneVersatileUnsheathed;
	FixedString SourceBoneAlternativeUnsheathed;
	FixedString SoundAttackType;
	FixedString SoundEquipmentType;
};


struct Flag : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Flag;
	static constexpr auto EngineClass = "ls::FlagManager";

	FixedString Name;
	STDString Description;
	uint8_t Usage; // FIXME - map to flags
};


struct Feat : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Feat;
	static constexpr auto EngineClass = "eoc::FeatManager";

	FixedString Name;
	STDString Requirements;
	[[bg3::hidden]]
	Array<void*> field_40;
	STDString PassivesAdded;
	[[bg3::hidden]]
	Array<void*> field_78;
	STDString PassivesRemoved;
	[[bg3::hidden]]
	Array<void*> field_B0;
	STDString Boosts;
	[[bg3::hidden]]
	Array<void*> field_E8;
	bool CanBeTakenMultipleTimes;
	Array<Progression::Ability> SelectAbilities;
	Array<Progression::AbilityBonus> SelectAbilityBonus;
	Array<Progression::Skill> SelectSkills;
	Array<Progression::SkillExpertise> SelectSkillsExpertise;
	Array<Progression::Spell> SelectSpells;
	Array<Progression::Passive> SelectPassives;
	Array<Progression::Equipment> SelectEquipment;
	Array<Progression::AddedSpell> AddSpells;
};


struct FeatDescription : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::FeatDescription;
	static constexpr auto EngineClass = "eoc::FeatDescriptionManager";

	TranslatedString DisplayName;
    TranslatedString Description;
	bool Hidden;
	FixedString ExactMatch;
	FixedString Type;
	FixedString ParamMatch;
	Guid FeatId;
	FixedString SelectorId;
	FixedString PassivePrototype;
};


struct PassiveList : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::PassiveList;
	static constexpr auto EngineClass = "eoc::PassiveListManager";

	Array<FixedString> Passives;
};


struct SkillList : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::SkillList;
	static constexpr auto EngineClass = "eoc::SkillListManager";

	Array<SkillId> Skills;
};


struct SpellList : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::SpellList;
	static constexpr auto EngineClass = "eoc::SpellListManager";

	MultiHashSet<FixedString> Spells;
};


struct AbilityList : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::AbilityList;
	static constexpr auto EngineClass = "eoc::AbilityListManager";

	Array<AbilityId> Spells;
};


struct CompanionPreset : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::CompanionPreset;
	static constexpr auto EngineClass = "eoc::CompanionPresetManager";

	Guid RaceUuid;
	Guid SubRaceUuid;
	uint8_t BodyType;
	uint8_t BodyShape;
	Guid RootTemplate;
	STDString Overview;
	STDString CloseUpA;
	STDString CloseUpB;
	Guid VoiceTableUuid;
	Guid VOLinesTableUuid;
};


struct AbilityDistributionPreset : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::AbilityDistributionPreset;
	static constexpr auto EngineClass = "eoc::AbilityDistributionPresetManager";

	Guid ClassUuid;
	int field_28;
	int Strength;
	int Dexterity;
	int Constitution;
	int Intelligence;
	int Wisdom;
	int Charisma;
};


struct CharacterCreationAccessorySet : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::CharacterCreationAccessorySet;
	static constexpr auto EngineClass = "eoc::CharacterCreationAccessorySetManager";

	bool CharacterCreationSet;
	TranslatedString DisplayName;
	Array<Guid> VisualUUID;
	FixedString SlotName;
	Guid RaceUUID;
	Array<Guid> DefaultForRootTemplates;
};


struct CharacterCreationAppearanceMaterial : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::CharacterCreationAppearanceMaterial;
	static constexpr auto EngineClass = "eoc::CharacterCreationAppearanceMaterialManager";

	FixedString Name;
	TranslatedString DisplayName;
	Guid MaterialPresetUUID;
	FixedString MaterialType;
	FixedString MaterialType2;
	glm::vec4 UIColor;
	Guid MaleRootTemplate;
	FixedString MaleCameraName;
	Guid FemaleRootTemplate;
	FixedString FemaleCameraName;
	Guid DragonbornMaleRootTemplate;
	Guid DragonbornFemaleRootTemplate;
};


struct CharacterCreationAppearanceVisual : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::CharacterCreationAppearanceVisual;
	static constexpr auto EngineClass = "eoc::CharacterCreationAppearanceVisualManager";

	Guid RootTemplate;
	Guid RaceUUID;
	uint8_t BodyType;
	uint8_t BodyShape;
	uint32_t field_3C;
	FixedString SlotName;
	Guid VisualResource;
	Guid HeadAppearanceUUID;
	Guid DefaultSkinColor;
	TranslatedString DisplayName;
	FixedString IconIdOverride;
	uint8_t DefaultForBodyType;
	FixedString TextureEntryPart;
	Array<Guid> Tags;
};


struct CharacterCreationEquipmentIcons : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::CharacterCreationEquipmentIcons;
	static constexpr auto EngineClass = "eoc::CharacterCreationEquipmentIconsManager";

	Guid RootTemplate;
	Guid EquipmentTemplate;
	Guid AnimationUUID;
	FixedString IconGenerationTrigger;
	FixedString SlotName;
	bool MeshIsTwoSided;
};


struct CharacterCreationIconSettings : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::CharacterCreationIconSettings;
	static constexpr auto EngineClass = "eoc::CharacterCreationIconSettingsManager";

	Guid RootTemplate;
	uint8_t BodyShape;
	Guid HeadAppearanceUUID;
};


struct CharacterCreationColor : public resource::GuidResource
{
	FixedString Name;
	TranslatedString DisplayName;
	Guid MaterialPresetUUID;
	glm::vec4 UIColor;
	FixedString SkinType;
};


struct CharacterCreationEyeColor : public resource::CharacterCreationColor
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::CharacterCreationEyeColor;
	static constexpr auto EngineClass = "eoc::CharacterCreationEyeColorManager";
};


struct CharacterCreationHairColor : public resource::CharacterCreationColor
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::CharacterCreationHairColor;
	static constexpr auto EngineClass = "eoc::CharacterCreationHairColorManager";
};


struct CharacterCreationSkinColor : public resource::CharacterCreationColor
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::CharacterCreationSkinColor;
	static constexpr auto EngineClass = "eoc::CharacterCreationSkinColorManager";
};


struct CharacterCreationMaterialOverride : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::CharacterCreationMaterialOverride;
	static constexpr auto EngineClass = "eoc::CharacterCreationMaterialOverrideManager";

	int32_t MaterialType;
	Guid ActiveMaterialPresetUUID;
	Guid InactiveMaterialPresetUUID;
	FixedString SourceMaterialUUID;
	FixedString TargetMaterialUUID;
};


struct CharacterCreationPassiveAppearance : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::CharacterCreationPassiveAppearance;
	static constexpr auto EngineClass = "eoc::CharacterCreationPassiveAppearanceManager";

	FixedString Passive;
	Guid RaceUUID;
	Array<Guid> AppearanceMaterialUUIDs;
	Array<Guid> ColorMaterialUUIDs;
	Array<Guid> AccessorySetUUIDs;
};


struct CharacterCreationPreset : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::CharacterCreationPreset;
	static constexpr auto EngineClass = "eoc::CharacterCreationPresetManager";

	Guid RaceUUID;
	Guid SubRaceUUID;
	uint8_t BodyType;
	uint8_t BodyShape;
	Guid RootTemplate;
	STDString Overview;
	STDString CloseUpA;
	STDString CloseUpB;
	Guid VOLinesTableUUID;
};


struct CharacterCreationSharedVisual : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::CharacterCreationSharedVisual;
	static constexpr auto EngineClass = "eoc::CharacterCreationSharedVisualManager";

	FixedString SlotName;
	Guid VisualResource;
	FixedString BoneName;
	TranslatedString DisplayName;
	Array<Guid> Tags;
};


struct LevelMap : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::LevelMap;
	static constexpr auto EngineClass = "eoc::LevelMapValues";

	struct DiceRoll
	{
		DiceSizeId DiceValue;
		uint8_t AmountOfDices;
	};

	FixedString Name;
	Guid PreferredClassUUID;
	std::variant<int32_t, DiceRoll> FallbackValue;
	Array<std::variant<int32_t, DiceRoll>> LevelMaps;
};


struct ApprovalRating : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ApprovalRating;
	static constexpr auto EngineClass = "eoc::ApprovalRatingManager";

	struct Reaction
	{
		Guid id;
		int32_t value;
	};

	uint8_t Scope;
	Array<Reaction> Reactions;
	Array<Reaction> ReactionsSpeakers;
};


struct AreaLevelOverride : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::AreaLevelOverride;
	static constexpr auto EngineClass = "eoc::AreaLevelOverrideManager";

	Guid ParentUuid;
	int32_t EntityLevel;
	int32_t TreasureLevel;
};


struct AvatarContainerTemplates : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::AvatarContainerTemplate;
	static constexpr auto EngineClass = "eoc::AvatarContainerTemplateManager";

	Guid TemplateId;
};


struct BackgroundGoal : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::BackgroundGoal;
	static constexpr auto EngineClass = "eoc::background::Goals";

	TranslatedString Title;
	TranslatedString Description;
	Guid BackgroundUuid;
	int32_t InspirationPoints;
	int32_t RewardLevel;
	Guid ExperienceReward;
};


struct CalendarDayRange : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::CalendarDayRange;
	static constexpr auto EngineClass = "eoc::calendar::DayRanges";

	int32_t Start;
	int32_t End;
	int32_t LeapYearStart;
	int32_t LeapYearEnd;
	FixedString Name;
	TranslatedString DisplayName;
	TranslatedString DisplayCommonName;
};

struct CampChestTemplate : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::CampChestTemplate;
	static constexpr auto EngineClass = "eoc::CampChestTemplateManager";

	Guid TemplateId;
};


struct DefaultValueGuidResource : public resource::GuidResource
{
	Guid TableUUID;
	Guid OriginUuid;
	Guid RaceUuid;
	Guid ClassUuid;
	Guid SubclassUuid;
	int32_t Level;
	STDString SelectorId;
};


struct AbilityDefaultValues : public DefaultValueGuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::AbilityDefaultValues;
	static constexpr auto EngineClass = "eoc::character_creation::_private::DefaultValuesManager<struct eoc::character_creation::_private::DefaultValues<enum EAbility> >";

	Array<AbilityId> Add;
};


struct EquipmentDefaultValues : public DefaultValueGuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::EquipmentDefaultValues;
	static constexpr auto EngineClass = "eoc::character_creation::_private::DefaultValuesManager<struct eoc::character_creation::_private::DefaultValues<class ls::FixedString> >";

	Array<FixedString> Add;
};


struct FeatDefaultValues : public DefaultValueGuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::FeatDefaultValues;
	static constexpr auto EngineClass = "eoc::character_creation::_private::DefaultValuesManager<struct eoc::character_creation::FeatDefaultValues>";

	Guid FeatUuid;
};


struct PassiveDefaultValues : public DefaultValueGuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::PassiveDefaultValues;
	static constexpr auto EngineClass = "eoc::character_creation::_private::DefaultValuesManager<struct eoc::character_creation::_private::ReplaceDefaultValues<class eoc::PassivePrototype> >";

	struct Replacement
	{
		FixedString From;
		FixedString To;
	};

	Array<FixedString> Add;
	Array<Replacement> Replace;
};


struct PreparedSpellDefaultValues : public DefaultValueGuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::PreparedSpellDefaultValues;
	static constexpr auto EngineClass = "eoc::character_creation::_private::DefaultValuesManager<struct eoc::character_creation::PreparedSpellDefaultValues>";

	Array<FixedString> Prepare;
	Array<FixedString> Unprepare;
};


struct SkillDefaultValues : public DefaultValueGuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::SkillDefaultValues;
	static constexpr auto EngineClass = "eoc::character_creation::_private::DefaultValuesManager<struct eoc::character_creation::_private::DefaultValues<enum ESkill> >";

	Array<SkillId> Add;
};


struct SpellDefaultValues : public DefaultValueGuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::SpellDefaultValues;
	static constexpr auto EngineClass = "eoc::character_creation::_private::DefaultValuesManager<struct eoc::character_creation::_private::ReplaceDefaultValues<class eoc::SpellPrototype> >";

	struct Replacement
	{
		FixedString From;
		FixedString To;
	};

	Array<FixedString> Add;
	Array<Replacement> Replace;
};


struct CharacterCreationVOLine : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::CharacterCreationVOLine;
	static constexpr auto EngineClass = "eoc::CharacterCreationVOLineManager";

	FixedString VoiceLine;
	Guid TableUuid;
};


struct CinematicArenaFrequencyGroup : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::CinematicArenaFrequencyGroup;
	static constexpr auto EngineClass = "eoc::CinematicArenaFrequencyGroupManager";

	FixedString Name;
	float MaxFrequency;
	double PercentageChance;
};


struct CustomDiceTemplate : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::CustomDice;
	static constexpr auto EngineClass = "eoc::customdice::TemplatesManager";

	STDString Name;
	STDString FontColour;
	TranslatedString DisplayNamme;
	TranslatedString Description;
	bool IsDefault;
	bool Flip1;
	bool Flip20;
	bool Icon;
	bool Icon20;
};


struct ConditionErrorDescription : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ConditionError;
	static constexpr auto EngineClass = "eoc::ConditionErrorDescriptionManager";

	FixedString Identifier;
	TranslatedString DisplayName;
	uint8_t ErrorDescriptionType;
	bool Hidden;
};


struct DeathTypeEffect : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::DeathTypeEffect;
	static constexpr auto EngineClass = "eoc::DeathTypeEffectsManager";

	STDString Name;
	Guid ParentUuid;
	FixedString Fallback;
	MultiHashMap<uint8_t, FixedString> Effects;
};


struct DifficultyClass : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::DifficultyClass;
	static constexpr auto EngineClass = "eoc::DifficultyClassManager";

	STDString Name;
	Array<int32_t> Difficulties;
	uint8_t field_40;
};


struct DisturbanceProperty : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::DisturbanceProperty;
	static constexpr auto EngineClass = "eoc::DisturbanceProperties";

	FixedString Name;
};


struct DLC : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::DLC;
	static constexpr auto EngineClass = "eoc::DLCManager";

	STDString Name;
	uint32_t SteamAPICode;
	uint32_t GalaxyAPICode;
	uint32_t PS5APICode;
	uint32_t XLiveAPICode;
	uint32_t TwitchAPICode;
	Guid CustomDice;
	uint8_t UnlockType;
};


struct EncumbranceType : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::EncumbranceType;
	static constexpr auto EngineClass = "eoc::EncumbranceTypesManager";

	FixedString Status;
};


struct EquipmentList : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::EquipmentList;
	static constexpr auto EngineClass = "eoc::EquipmentListManager";

	Array<FixedString> Items;
};


struct ExperienceRewards : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ExperienceReward;
	static constexpr auto EngineClass = "eoc::ExperienceRewards";

	uint8_t RewardType;
	uint8_t LevelSource;
	FixedString Name;
	Array<int32_t> PerLevelRewards;
};


struct FeatSoundState : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::FeatSoundState;
	static constexpr auto EngineClass = "eoc::feat::SoundStateManager";

	FixedString FeatName;
	Guid FeatUuid;
	FixedString OverrideSwitchGroup;
	FixedString OverrideSwitch;
	FixedString DefaultSwitch;
};


struct FlagSoundState : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::FlagSoundState;
	static constexpr auto EngineClass = "eoc::flag::SoundStateManager";

	FixedString FlagName;
	Guid FlagUuid;
	FixedString OverrideSwitchGroup;
	FixedString OverrideSwitch;
	FixedString DefaultSwitch;
};


struct GoldRewards : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::GoldReward;
	static constexpr auto EngineClass = "eoc::GoldRewards";

	FixedString Name;
	Guid ParentUuid;
	double ParentScale;
	Array<int32_t> PerLevelRewards;
};


struct FixedHotBarSlot : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::FixedHotBarSlot;
	static constexpr auto EngineClass = "eoc::hotbar::FixedHotBarSlots";

	int32_t SlotIndex;
	FixedString SpellId;
	uint8_t HotBarController;
	uint8_t HotBarType;
};


struct ItemThrowParams : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ItemThrowParams;
	static constexpr auto EngineClass = "eoc::ItemThrowParamsManager";

	int32_t Priority;
	STDString Conditions;
	int32_t MaxDistForOneRotation;
	int32_t MaxDistForTwoRotations;
	int32_t MaxDistForZeroRotations;
	glm::vec3 StartAngle;
	uint32_t RotationAxis;
};


struct ItemWallTemplate : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ItemWallTemplate;
	static constexpr auto EngineClass = "eoc::itemwall::TemplatesManager";

	STDString Name;
	STDString HighLeft;
	STDString HighMiddle;
	STDString HighRight;
	STDString LowLeft;
	STDString LowMiddle;
	STDString LowRight;
	float Padding;
	float RandomRotation;
	FixedString Animation;
};


struct TrajectoryRule : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::TrajectoryRules;
	static constexpr auto EngineClass = "eoc::jump::TrajectoryRules";

	FixedString Template;
	int32_t Priority;
	float HeightMin;
	float HeightMax;
	float LengthMin;
	float LengthMax;
};


struct LimbsMapping : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::LimbsMapping;
	static constexpr auto EngineClass = "eoc::LimbsMappingManager";

	Guid RootTemplate;
	Array<uint8_t> BackRight;
	Array<uint8_t> BackLeft;
	Array<uint8_t> FrontRight;
	Array<uint8_t> FrontLeft;
};


struct LongRestCost : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::LongRestCost;
	static constexpr auto EngineClass = "eoc::LongRestCosts";

	uint32_t CampQuality;
	int32_t RequiredSupplies;
	FixedString CampGrowthDifficulty;
};


struct ManagedStatusVFX : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ManagedStatusVFX;
	static constexpr auto EngineClass = "eoc::ManagedStatusVFXContainer";

	FixedString Group;
	FixedString PositiveEffect;
	FixedString NegativeEffect;
	FixedString MixedEffect;
};


struct EffectInfo
{
	Guid EffectResourceGuid;
	STDString ResourceId;
	STDString StartTextKey;
	STDString EndTextKey;
	float MinDistance;
	float MaxDistance;
	uint32_t Flags;
	bool Pivot;
	uint32_t BindSourceTo;
	uint32_t BindTargetTo;
	DamageType DamageType;
	uint8_t VerbalIntent;
	Array<STDString> SourceSkeletonSlot;
	Array<STDString> SourceBone;
	Array<Guid> SourceSurface;
	Array<Guid> SourceTag;
	Array<Guid> SourceIgnoreTag;
	bool SourceCheckEquipmentTagConditions;
	Array<STDString> TargetSkeletonSlot;
	Array<STDString> TargetBone;
	Array<Guid> TargetSurface;
	Array<Guid> TargetTag;
	Array<Guid> TargetIgnoreTag;
	bool TargetCheckEquipmentTagConditions;
};


struct MultiEffectInfo : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::MultiEffectInfo;
	static constexpr auto EngineClass = "eoc::MultiEffectInfoRegistry";

	STDString Name;
	Array<EffectInfo> EffectInfo;
};


struct OriginIntroEntity : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::OriginIntroEntity;
	static constexpr auto EngineClass = "eoc::OriginIntroEntityManager";

	FixedString Name;
	int32_t PlayerIndex;
	Guid DummyUuid;
};


struct PassivesVFX : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::PassiveVFX;
	static constexpr auto EngineClass = "eoc::PassivesVFXManager";

	FixedString PassiveName;
	FixedString PrepareEffect;
	FixedString CastEffect;
};


struct ProjectileDefault : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ProjectileDefault;
	static constexpr auto EngineClass = "eoc::ProjectileDefaultContainer";

	uint8_t ProjectileDefaultType;
	Guid ProjectileTemplateId;
};


struct RandomCastOutcome : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::RandomCastOutcome;
	static constexpr auto EngineClass = "eoc::RandomCastOutcomes";

	FixedString GroupName;
	FixedString Spell;
	int32_t Level;
	Guid ClassUuid;
	int32_t ClassLevel;
};


struct SoundSpellTrajectoryRule : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::SpellSoundTrajectory;
	static constexpr auto EngineClass = "eoc::sound::SpellTrajectoryRules";

	FixedString RuleName;
	FixedString CurveUuid;
	FixedString SoundEvent;
	float DistancePastListener;
};


struct StatusSoundState : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::StatusSoundState;
	static constexpr auto EngineClass = "eoc::status::SoundStateManager";

	FixedString SoundStateName;
	FixedString CombatVocalOverrideSwitch;
	FixedString SpellOverrideSwitchGroup;
	FixedString SpellOverrideSwitchState;
	FixedString SpellOverrideSwitchClearState;
	bool BlockVocals;
};


struct SurfaceCursorMessage : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::SurfaceCursorMessage;
	static constexpr auto EngineClass = "eoc::SurfaceCursorMessageManager";

	FixedString Name;
	TranslatedString CursorMessage;
	int Priority;
};


struct TadpolePower : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::TadpolePower;
	static constexpr auto EngineClass = "eoc::tadpole_tree::TadpolePowersTree";

	Array<Guid> Prerequisites;
	FixedString Name;
	bool NeedsHalfIllithidToUnlock;
};


struct TagSoundState : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::TagSoundState;
	static constexpr auto EngineClass = "eoc::tag::SoundStateManager";

	FixedString TagName;
	Guid TagUuid;
	FixedString OverrideSwitchGroup;
	FixedString OverrideSwitch;
	FixedString DefaultSwitch;
};


struct TooltipExtraText : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::TooltipExtraText;
	static constexpr auto EngineClass = "eoc::TooltipExtraTextManager";

	TranslatedString Text;
};


struct TooltipUpcastDescription : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::TooltipUpcastDescription;
	static constexpr auto EngineClass = "eoc::TooltipUpcastDescriptionManager";

	FixedString Name;
	TranslatedString Text;
};


struct TutorialEntry : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::TutorialEntries;
	static constexpr auto EngineClass = "eoc::tutorial::EntriesManager";

	TranslatedString DisplayTitle;
	TranslatedString KeyboardDescription;
	TranslatedString ControllerDescription;
	STDString KeyboardVideo;
	STDString ControllerVideo;
	uint8_t Section;
	uint8_t InputType;
	bool Hidden;
};


struct TutorialModalEntry : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::TutorialModalEntries;
	static constexpr auto EngineClass = "eoc::tutorial::ModalEntriesManager";

	FixedString TutorialName;
	int32_t TutorialID;
	TranslatedString DisplayTitle;
	TranslatedString KeyboardDescription;
	TranslatedString ControllerDescription;
	uint8_t Section;
	uint8_t ModalType;
	bool WaitForEndDialog;
	bool WaitForEndCC;
	FixedString ExtraData;
};


struct TutorialUnifiedEntry : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::TutorialUnifiedEntry;
	static constexpr auto EngineClass = "eoc::tutorial::UnifiedEntriesManager";

	FixedString TutorialName;
	uint8_t ModalType;
	TranslatedString DisplayTitle;
	TranslatedString KeyboardDescription;
	TranslatedString ControllerDescription;
	TranslatedString KeyboardJournalDescription;
	TranslatedString ControllerJournalDescription;
	TranslatedString KeyboardInputList;
	TranslatedString ControllerInputList;
	uint8_t Section;
	bool WaitForEndDialog;
	bool WaitForEndCC;
	bool WaitForLayers;
	bool ShowInSplitScreen;
	uint8_t InputType;
	uint32_t field_98;
	STDString Icon;
	int32_t LifeTime;
	int32_t PositionOffsetX;
	int32_t PositionOffsetY;
};


struct TutorialEvent : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::TutorialEvent;
	static constexpr auto EngineClass = "eoc::tutorial::TutorialEventManager";

	STDString Name;
	uint8_t EventType;
	Guid ActionResource;
	uint32_t HotbarSlotFlags;
	uint8_t UserAction;
};


struct VFX : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::VFX;
	static constexpr auto EngineClass = "eoc::VFXContainer";

	FixedString VFXGameplayName;
	FixedString VFXEffectName;
};


struct Voice : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Voice;
	static constexpr auto EngineClass = "eoc::VoiceManager";

	Guid TableUuid;
	TranslatedString DisplayName;
	Guid SpeakerUuid;
	uint8_t BodyType;
	Array<Guid> Tags;
};


struct WeaponAnimationSet : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::WeaponAnimationSet;
	static constexpr auto EngineClass = "eoc::WeaponAnimationSetData";

	FixedString MainHand;
	FixedString OffHand;
	FixedString AnimationSetUuid;
};


struct WeightCategories : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::WeightCategory;
	static constexpr auto EngineClass = "eoc::weight::WeightCategories";

	int32_t MaxWeight;
	Array<Guid> Tags;
	uint8_t ObjectSize;
};


struct ShapeshiftRule : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ShapeshiftRule;
	static constexpr auto EngineClass = "esv::shapeshift::rules::Rulebook";

	struct TypedInt
	{
		uint8_t Type;
		int32_t Value;
	};

	struct TypedFloat
	{
		uint8_t Type;
		float Value;
	};

	STDString RuleName;
	TypedInt Hp;
	TypedInt TemporaryHp;
	bool WildShapeHotBar;
	TypedFloat Weight;
	uint8_t FootstepsType;
	Array<TypedInt> AbilityChanges;
	bool RemoveOldTags;
	bool ApplyTagsFromTemplate;
	Array<Guid> TagsAdd;
	Array<Guid> TagsRemove;
	uint16_t ActionCapabilities;
	uint16_t InteractionsCapabilities;
	uint16_t AwarenessCapabilities;
	uint16_t SpeakingCapabilities;
	bool ChangeRace;
	bool ChangeBodyType;
	bool UseShapeshiftIdentity;
	bool ChangeAi;
	std::optional<STDString> ChangeScript;
	bool ApplyVisual;
	bool ApplyVisualInCC;
	bool IgnoreCustomLooks;
	bool RetainDisplayName;
	bool ChangeIcon;
	TypedFloat Scale;
	bool MuteEquipmentSound;
	bool OverrideSpeaker;
	bool RemovePrevSpells;
	bool ApplySpellsFromTemplate;
	Array<FixedString> SpellsAdd;
	Array<FixedString> SpellsRemove;
	bool RetainCanSeeThrough;
	bool RetainCanShootThrough;
	bool RetainCanWalkThrough;
	bool BaseACOverride;
	bool UnarmedAbilityFromTemplate;
	bool DisableEquipmentSlots;
	bool UseTemplateEquipmentSet;
	bool KillEntityAtZeroHP;
	STDString PathInfluence;
	std::optional<Guid> DummyClass;
	bool BlockLevelUp;
	uint8_t FlagsInheritanceType;
	uint8_t DefaultBoostsInheritanceType;
	uint8_t PersonalStatusImmunitiesInheritanceType;
	uint8_t SpellCastingAbilityInheritanceType;
	uint8_t PassivesInheritanceType;
	uint8_t ResistancesInheritanceType;
	uint8_t DifficultyStatusesInheritanceType;
	uint8_t ProficiencyGroupsInheritanceType;
	uint8_t BloodSurfaceTypeInheritanceType;
	uint8_t HitBloodTypeInheritanceType;
	uint8_t DeathActionsInheritanceType;
};


struct AnimationSetPriority : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::AnimationSetPriority;
	static constexpr auto EngineClass = "ls::AnimationSetPriorityManager";

	STDString Name;
	int32_t Priority;
	Array<Guid> AddidionalObjects;
};


struct SpellMetaCondition : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::SpellMetaCondition;
	static constexpr auto EngineClass = "eoc::SpellMetaConditionManager";

	SpellMetaConditionType ConditionType;
	STDString Filter;
	STDString AdditionalConditions;
	bool OverrideOriginalCondition;
};


struct ScriptMaterialParameterOverride : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ScriptMaterialParameterOverride;
	static constexpr auto EngineClass = "eoc::ScriptMaterialParameterOverrideManager";

	FixedString ParameterName;
	FixedString ParameterType;
	uint8_t SetMode;
	uint8_t IsFixedString;
	[[bg3::hidden]] __int64 field_28;
	std::variant<float, glm::vec3, glm::vec4, FixedString> Limit;
	[[bg3::hidden]] __int64 field_48;
	std::variant<float, glm::vec3, glm::vec4, FixedString> ParameterValue;
	[[bg3::hidden]] __int64 field_68;
};


struct ScriptMaterialPresetOverride : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ScriptMaterialPresetOverride;
	static constexpr auto EngineClass = "eoc::ScriptMaterialPresetOverrideManager";

	Array<Guid> ParameterUuids;
};


struct VisualLocatorAttachment : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::VisualLocatorAttachment;
	static constexpr auto EngineClass = "ls::VisualLocatorAttachmentManager";

	FixedString DisplayName;
	FixedString LocatorName;
};


struct RulesetModifier : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::RulesetModifier;
	static constexpr auto EngineClass = "eoc::ruleset::RulesetModifiers";

	STDString Name;
	TranslatedString DisplayName;
	TranslatedString Description;
	uint8_t RulesetModifierType;
	int field_54;
	float Max;
	float Step;
	STDString Default;
	[[bg3::hidden]] __int64 field_78;
	STDString LoadModule;
	bool ShowInCustom;
	bool EditableDuringGame;
};


struct RulesetModifierOption : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::RulesetModifierOption;
	static constexpr auto EngineClass = "eoc::ruleset::RulesetModifierOptions";

	STDString Name;
	TranslatedString DisplayName;
	TranslatedString Description;
	Guid Modifier;
	STDString Value;
	[[bg3::hidden]] __int64 field_78;
	STDString LoadModule;
	bool ShowInCustom;
};


struct Ruleset : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Ruleset;
	static constexpr auto EngineClass = "eoc::ruleset::Rulesets";

	STDString Name;
	uint8_t Type;
	TranslatedString DisplayName;
	TranslatedString Description;
	Array<Guid> Parent;
	bool ShowInCustom;
};


struct RulesetValue : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::RulesetValue;
	static constexpr auto EngineClass = "eoc::ruleset::RulesetValues";

	struct NoValue
	{
	};

	Guid Ruleset;
	Guid Modifier;
	STDString RawValue;
	std::variant<NoValue, float, int, FixedString, bool> Value;
};


struct RulesetSelectionPreset : public resource::GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::RulesetSelectionPreset;
	static constexpr auto EngineClass = "eoc::ruleset::RulesetSelectionPresets";

	STDString Name;
	Array<Guid> Rulesets;
	TranslatedString DisplayName;
	TranslatedString Description;
	STDString Asset;
	bool IsCustom;
};


END_NS()
