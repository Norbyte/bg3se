#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>

namespace bg3se
{
	struct GuidResourceDefinition : public HasObjectProxy
	{
		void* VMT;
		Guid ResourceUUID;
	};

	struct GuidResourceDefinitionManagerBase : ProtectedGameObject<GuidResourceDefinitionManagerBase>
	{
		void* VMT;
		VirtualMultiHashMap<Guid, Array<Guid>> ResourceGuidsByMod;
		FixedString field_60;
		FixedString LSXResourceNodeName;
	};

	template <class T>
	struct GuidResourceDefinitionManager : public GuidResourceDefinitionManagerBase
	{
		VirtualMultiHashMap<Guid, T> Resources;
		STDString Path;
		RefMap<FixedString, void*> field_E0;
	};


	struct GuidResourceDefinitions
	{
		__int64 field_0;
		MultiHashMap<int32_t, GuidResourceDefinitionManagerBase*> Definitions;
		Array<GuidResourceDefinitionManagerBase*> DefinitionManagers;
		__int64 field_70;
	};


	struct ActionResource : public GuidResourceDefinition
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


	struct ClassDescriptionResource : public GuidResourceDefinition
	{
		static constexpr auto ResourceManagerType = ExtResourceManagerType::ClassDescription;

		Guid ParentGuid;
		FixedString Name;
		FixedString DisplayName;
		FixedString Description;
		Guid ProgressionTableUUID;
		uint8_t MagicType; // FIXME - map to enumeration
		uint8_t SoundMagicType; // FIXME - map to enumeration
		uint8_t PrimaryAbility; // FIXME - map to enumeration
		uint8_t SpellCastingAbility; // FIXME - map to enumeration
		bool MustPrepareSpells;
		bool CanLearnSpells;
		bool HasGod;
		Array<Guid> Tags;
		FixedString ClassEquipment;
		FixedString CharacterCreationPose;
		uint8_t LearningStrategy; // FIXME - map to enumeration
		uint8_t field_71;
		int BaseHp;
		int HpPerLevel;
	};


	struct TagResource : public GuidResourceDefinition
	{
		static constexpr auto ResourceManagerType = ExtResourceManagerType::Tag;

		FixedString Name;
		STDString Description;
		FixedString Icon;
		uint16_t Categories; // FIXME - map to flags
		TranslatedString DisplayName;
		TranslatedString DisplayDescription;
	};


	struct FactionResource : public GuidResourceDefinition
	{
		static constexpr auto ResourceManagerType = ExtResourceManagerType::Faction;

		FixedString Faction;
		Guid ParentGuid;
	};


	struct RaceResource : public GuidResourceDefinition
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


	struct OriginResource : public GuidResourceDefinition
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


	struct BackgroundResource : public GuidResourceDefinition
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


	struct GodResource : public GuidResourceDefinition
	{
		static constexpr auto ResourceManagerType = ExtResourceManagerType::God;

		FixedString Name;
		FixedString DisplayName;
		FixedString Description;
		Array<Guid> Tags;
	};


	struct ProgressionResource : public GuidResourceDefinition
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


	struct ProgressionDescriptionResource : public GuidResourceDefinition
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


	struct GossipDefinition : public GuidResourceDefinition
	{
		static constexpr auto ResourceManagerType = ExtResourceManagerType::Gossip;

		FixedString Name;
		FixedString Type;
		int Priority;
		Array<Guid> ConditionFlags;
		Array<Guid> ResultFlags;
		Guid DialogUUID;
	};


	struct ActionResourceGroupDefinition : public GuidResourceDefinition
	{
		static constexpr auto ResourceManagerType = ExtResourceManagerType::ActionResourceGroup;

		STDString Name;
		TranslatedString field_38;
		TranslatedString field_48;
		Array<Guid> ActionResourceDefinitions;
	};


	struct ColorDefinitionResource : public GuidResourceDefinition
	{
		static constexpr auto ResourceManagerType = ExtResourceManagerType::ColorDefinition;

		FixedString Name;
		FixedString DisplayName;
		glm::vec4 Color;
	};


	struct EquipmentTypeResource : public GuidResourceDefinition
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


	struct FlagResource : public GuidResourceDefinition
	{
		static constexpr auto ResourceManagerType = ExtResourceManagerType::Flag;

	    FixedString Name;
	    STDString Description;
	    uint8_t Usage; // FIXME - map to flags
	};


	struct FeatResource : public GuidResourceDefinition
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


	struct FeatDescriptionResource : public GuidResourceDefinition
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
}
