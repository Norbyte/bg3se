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


struct Action : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ActionResource;

	FixedString Name;
	FixedString DisplayName;
	FixedString Description;
	FixedString Error;
	uint32_t MaxLevel;
	double MaxValue;
	uint8_t DiceType; // FIXME - map to enumeration
	uint8_t ReplenishType; // FIXME - map to enumeration
	bool ShowOnActionResourcePanel;
	bool UpdatesSpellPowerLevel;
	bool PartyActionResource;
	bool IsSpellResource;
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
	uint16_t Categories; // FIXME - map to flags
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
	FixedString DisplayName;
	FixedString Description;
	Guid ParentGuid;
	Array<Guid> Tags;
	Guid ProgressionTableUUID;
	FixedString RaceSoundSwitch;
	Array<Guid> HairColors;
	Array<Guid> SkinColors;
	Array<Guid> EyeColors;
	Array<Guid> TattooColors;
	Array<Guid> MakeupColors;
	Array<Guid> Visuals;
	Array<Guid> Gods;
	Array<Guid> ExcludedGods;
};


struct Origin : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Origin;

	bool AvailableInCharacterCreation;
	FixedString Name;
	FixedString DisplayName;
	FixedString Description;
	uint8_t Gender;
	Guid RaceUUID;
	Guid SubRaceUUID;
	Guid BackgroundUUID;
	Guid GodUUID;
	Guid ClassUUID;
	Guid SubClassUUID;
	bool LockClass;
	Guid GlobalTemplate;
	STDString Passives;
	Array<Guid> field_C8;
	Array<Guid> Tags;
	uint32_t Flags;
	STDString Overview;
	STDString CloseUpA;
	STDString CloseUpB;
	FixedString ClassEquipmentOverride;
	Guid VoiceTableUUID;
};


struct Background : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Background;

	FixedString DisplayName;
	FixedString Description;
	STDString Passives;
	__int64 field_40;
	__int64 field_48;
	__int64 field_50;
	Array<Guid> Tags;
};


struct God : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::God;

	FixedString Name;
	FixedString DisplayName;
	FixedString Description;
	Array<Guid> Tags;
};


struct Progression : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::Progression;

	Guid TableUUID;
	STDString Name;
	Array<Guid> SubClasses;
	Array<Guid> field_60;
	STDString PassivesAdded;
	Array<Guid> field_98;
	STDString PassivesRemoved;
	Array<Guid> field_D0;
	STDString Boosts;
	uint8_t ProgressionType; // FIXME - map to enumeration
	uint8_t Level;
	bool AllowImprovement;
	Array<Guid> SelectAbilities;
	Array<Guid> SelectSkills;
	Array<Guid> SelectSpells;
	Array<Guid> SelectPassives;
	Array<Guid> AddSpells;
};


struct ProgressionDescription : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::ProgressionDescription;

	FixedString DisplayName;
	FixedString Description;
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
	FixedString DisplayName;
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
	Array<Guid> SelectAbilities;
	Array<Guid> SelectSkills;
	Array<Guid> SelectSpells;
	Array<Guid> SelectPassives;
	Array<Guid> AddSpells;
};


struct FeatDescription : public GuidResource
{
	static constexpr auto ResourceManagerType = ExtResourceManagerType::FeatDescription;

	FixedString DisplayName;
    FixedString Description;
	bool Hidden;
	FixedString ExactMatch;
	FixedString Type;
	FixedString ParamMatch;
	Guid FeatId;
	FixedString SelectorId;
	FixedString PassivePrototype;
};

END_NS()

