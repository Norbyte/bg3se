#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_NS(resource)

struct GuidResource : ProtectedGameObject<GuidResource>
{
	void* VMT;
	Guid ResourceUUID;
};

struct GuidResourceBankBase : ProtectedGameObject<GuidResourceBankBase>
{
	void* VMT;
	FixedString LSXRegionName;
	FixedString LSXResourceNodeName;
	MultiHashMap<Guid, Array<Guid>> ResourceGuidsByMod;
};

template <class T>
struct GuidResourceBank : public GuidResourceBankBase
{
	MultiHashMap<Guid, T> Resources;
	STDString Path;
	RefMap<Guid, void*> field_E0;
};


struct GuidResourceManager
{
	MultiHashMap<int32_t, GuidResourceBankBase*> Definitions;
};


struct ActionResource : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ActionResource;

	FixedString Name;
	TranslatedString DisplayName;
	TranslatedString Description;
	TranslatedString Error;
	uint32_t MaxLevel;
	double MaxValue;
	uint8_t DiceType; // FIXME - map to enumeration
	uint8_t ReplenishType; // FIXME - map to enumeration
	bool ShowOnActionResourcePanel;
	bool UpdatesSpellPowerLevel;
	bool PartyActionResource;
	bool IsSpellResource;
	bool IsHidden;
};


struct ClassDescription : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ClassDescription;

	Guid ParentGuid;
	FixedString Name;
	TranslatedString DisplayName;
	TranslatedString Unused;
	TranslatedString SubclassTitle;
	TranslatedString Description;
	Guid ProgressionTableUUID;
	FixedString SoundClassType;
	uint8_t PrimaryAbility; // FIXME - map to enumeration
	uint8_t SpellCastingAbility; // FIXME - map to enumeration
	bool MustPrepareSpells;
	bool CanLearnSpells;
	bool HasGod;
	bool IsDefaultForUseSpellAction;
	bool IsSomaticWithInstrument;
	Array<Guid> Tags;
	FixedString ClassEquipment;
	FixedString CharacterCreationPose;
	uint8_t LearningStrategy; // FIXME - map to enumeration
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


struct Tag : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Tag;

	FixedString Name;
	STDString Description;
	FixedString Icon;
	uint32_t Categories; // FIXME - map to flags
	uint32_t Properties; // FIXME - map to flags
	TranslatedString DisplayName;
	TranslatedString DisplayDescription;
};


struct Faction : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Faction;

	FixedString Faction;
	Guid ParentGuid;
};


struct Race : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Race;

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


struct Origin : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Origin;

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
	Array<Guid> field_C8;
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


struct Background : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Background;

	TranslatedString DisplayName;
	TranslatedString Description;
	STDString Passives;
	__int64 field_40;
	__int64 field_48;
	Array<Guid> Tags;
	bool Hidden;
};


struct God : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::God;

	FixedString Name;
	TranslatedString DisplayName;
	TranslatedString Description;
	Array<Guid> Tags;
};


struct Progression : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Progression;

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
		int64_t Amount;
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
	Array<Guid> field_50;
	STDString PassivesAdded;
	Array<Guid> field_78;
	STDString PassivesRemoved;
	Array<Guid> field_A0;
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


struct ProgressionDescription : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ProgressionDescription;

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


struct Gossip : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Gossip;

	FixedString Name;
	FixedString Type;
	int Priority;
	Array<Guid> ConditionFlags;
	Array<Guid> ResultFlags;
	Guid DialogUUID;
};


struct ActionResourceGroup : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ActionResourceGroup;

	STDString Name;
	TranslatedString field_38;
	TranslatedString field_48;
	Array<Guid> ActionResourceDefinitions;
};


struct Color : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ColorDefinition;

	FixedString Name;
	TranslatedString DisplayName;
	glm::vec4 Color;
};


struct EquipmentType : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::EquipmentType;

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


struct Flag : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Flag;

	FixedString Name;
	STDString Description;
	uint8_t Usage; // FIXME - map to flags
};


struct Feat : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Feat;

	FixedString Name;
	STDString Requirements;
	Array<Guid> field_40;
	STDString PassivesAdded;
	Array<Guid> field_78;
	STDString PassivesRemoved;
	Array<Guid> field_B0;
	STDString Boosts;
	Array<Guid> field_E8;
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


struct FeatDescription : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::FeatDescription;

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


struct PassiveList : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::PassiveList;

	Array<FixedString> Passives;
};


struct SkillList : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::SkillList;

	Array<SkillId> Skills;
};


struct SpellList : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::SpellList;

	MultiHashSet<FixedString> Spells;
};


END_NS()
