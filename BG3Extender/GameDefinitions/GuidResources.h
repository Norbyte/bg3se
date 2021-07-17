#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>

namespace bg3se
{
	struct GuidResourceDefinition : public HasObjectProxy
	{
		void* VMT;
		UUID ResourceUUID;
	};

	struct GuidResourceDefinitionManagerBase : ProtectedGameObject<GuidResourceDefinitionManagerBase>
	{
		void* VMT;
		VirtualMultiHashMap<UUID, Array<UUID>> ResourceGuidsByMod;
		FixedString field_60;
		FixedString LSXResourceNodeName;
	};

	template <class T>
	struct GuidResourceDefinitionManager : public GuidResourceDefinitionManagerBase
	{
		VirtualMultiHashMap<UUID, T> Resources;
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

		UUID ParentGuid;
		FixedString Name;
		FixedString DisplayName;
		FixedString Description;
		UUID ProgressionTableUUID;
		uint8_t MagicType; // FIXME - map to enumeration
		uint8_t SoundMagicType; // FIXME - map to enumeration
		uint8_t PrimaryAbility; // FIXME - map to enumeration
		uint8_t SpellCastingAbility; // FIXME - map to enumeration
		bool MustPrepareSpells;
		bool CanLearnSpells;
		bool HasGod;
		Array<UUID> Tags;
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
		UUID ParentGuid;
	};


	struct RaceResource : public GuidResourceDefinition
	{
		static constexpr auto ResourceManagerType = ExtResourceManagerType::Race;

		FixedString Name;
		FixedString DisplayName;
		FixedString Description;
		UUID ParentGuid;
		Array<UUID> Tags;
		UUID ProgressionTableUUID;
		FixedString RaceSoundSwitch;
		Array<UUID> HairColors;
		Array<UUID> SkinColors;
		Array<UUID> EyeColors;
		Array<UUID> TattooColors;
		Array<UUID> MakeupColors;
		Array<UUID> Visuals;
		Array<UUID> Gods;
		Array<UUID> ExcludedGods;
	};


	struct OriginResource : public GuidResourceDefinition
	{
		static constexpr auto ResourceManagerType = ExtResourceManagerType::Origin;

		bool AvailableInCharacterCreation;
		FixedString Name;
		FixedString DisplayName;
		FixedString Description;
		uint8_t Gender;
		UUID RaceUUID;
		UUID SubRaceUUID;
		UUID BackgroundUUID;
		UUID GodUUID;
		UUID ClassUUID;
		UUID SubClassUUID;
		bool LockClass;
		UUID GlobalTemplate;
		STDString Passives;
		Array<UUID> field_C8;
		Array<UUID> Tags;
		uint32_t Flags;
		STDString Overview;
		STDString CloseUpA;
		STDString CloseUpB;
		FixedString ClassEquipmentOverride;
		UUID VoiceTableUUID;
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
		Array<UUID> Tags;
	};


	struct GodResource : public GuidResourceDefinition
	{
		static constexpr auto ResourceManagerType = ExtResourceManagerType::God;

		FixedString Name;
		FixedString DisplayName;
		FixedString Description;
		Array<UUID> Tags;
	};


	struct ProgressionResource : public GuidResourceDefinition
	{
		static constexpr auto ResourceManagerType = ExtResourceManagerType::Progression;

		UUID TableUUID;
		STDString Name;
		Array<UUID> SubClasses;
		Array<UUID> field_60;
		STDString PassivesAdded;
		Array<UUID> field_98;
		STDString PassivesRemoved;
		Array<UUID> field_D0;
		STDString Boosts;
		uint8_t ProgressionType; // FIXME - map to enumeration
		uint8_t Level;
		bool AllowImprovement;
		Array<UUID> SelectAbilities;
		Array<UUID> SelectSkills;
		Array<UUID> SelectSpells;
		Array<UUID> SelectPassives;
		Array<UUID> AddSpells;
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
		UUID ProgressionId;
		UUID ProgressionTableId;
		FixedString SelectorId;
		FixedString PassivePrototype;
	};


	struct GossipDefinition : public GuidResourceDefinition
	{
		static constexpr auto ResourceManagerType = ExtResourceManagerType::Gossip;

		FixedString Name;
		FixedString Type;
		int Priority;
		Array<UUID> ConditionFlags;
		Array<UUID> ResultFlags;
		UUID DialogUUID;
	};


	struct ActionResourceGroupDefinition : public GuidResourceDefinition
	{
		static constexpr auto ResourceManagerType = ExtResourceManagerType::ActionResourceGroup;

		STDString Name;
		TranslatedString field_38;
		TranslatedString field_48;
		Array<UUID> ActionResourceDefinitions;
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
		Array<UUID> field_40;
		STDString PassivesAdded;
		Array<UUID> field_78;
		STDString PassivesRemoved;
		Array<UUID> field_B0;
		STDString Boosts;
		Array<UUID> field_E8;
		bool CanBeTakenMultipleTimes;
		Array<UUID> SelectAbilities;
		Array<UUID> SelectSkills;
		Array<UUID> SelectSpells;
		Array<UUID> SelectPassives;
		Array<UUID> AddSpells;
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
	    UUID FeatId;
	    FixedString SelectorId;
	    FixedString PassivePrototype;
	};
}
