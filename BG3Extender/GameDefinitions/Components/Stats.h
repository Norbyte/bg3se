#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

namespace bg3se
{
	struct ArmorComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Armor;
		static constexpr auto EngineClass = "eoc::ArmorComponent";

		int ArmorType;
		int ArmorClass;
		int AbilityModifierCap;
		uint8_t ArmorClassAbility;
		uint8_t Shield;
	};

	struct BaseHpComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::BaseHp;
		static constexpr auto EngineClass = "eoc::BaseHpComponent";

		int Vitality;
		int VitalityBoost;
	};

	struct CustomStatsComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CustomStats;
		static constexpr auto EngineClass = "eoc::CustomStatsComponent";

		Map<FixedString, int> Stats;
	};

	struct DataComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Data;
		static constexpr auto EngineClass = "eoc::DataComponent";

		int32_t Weight;
		FixedString StatsId;
		uint32_t StepsType;
	};

	struct ExperienceComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Experience;
		static constexpr auto EngineClass = "eoc::exp::ExperienceComponent";

		int CurrentLevelExperience;
		int NextLevelExperience;
		int TotalExperience;
		int SomeExperience;
		uint8_t field_28;
	};

	struct HealthComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Health;
		static constexpr auto EngineClass = "eoc::HealthComponent";

		std::array<int, 14> Resistances;
		int field_38;
		int Hp;
		Guid field_40;
		int MaxHp;
		int TemporaryHp;
		int MaxTemporaryHp;
		int AC;
		std::array<int, 14> PerDamageTypeHealthThresholds;
		std::array<int, 14> PerDamageTypeHealthThresholds2;
		bool IsInvulnerable;
	};

	struct DifficultyCheckComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::DifficultyCheck;
		static constexpr auto EngineClass = "eoc::DifficultyCheckComponent";

		int SpellDC;
		int field_4;
		int field_8;
	};

	struct AttributeFlagsComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AttributeFlags;
		static constexpr auto EngineClass = "eoc::AttributeFlagsComponent";

		uint32_t AttributeFlags;
	};

	struct BodyTypeComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::BodyType;
		static constexpr auto EngineClass = "eoc::BodyTypeComponent";

		uint8_t BodyType;
		uint8_t BodyType2;
	};

	struct ObjectSizeComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ObjectSize;
		static constexpr auto EngineClass = "eoc::ObjectSizeComponent";

		uint8_t Size;
		uint8_t field_1;
	};

	struct BaseStatsComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::BaseStats;
		static constexpr auto EngineClass = "eoc::BaseStatsComponent";

		std::array<int, 7> BaseAbilities;
	};

	struct ExpertiseComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Expertise;
		static constexpr auto EngineClass = "eoc::expertise::ExpertiseComponent";

		MultiHashSet<SkillId> Expertise;
	};

	struct HealBlockComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::HealBlock;
		static constexpr auto EngineClass = "eoc::heal::BlockComponent";

		uint8_t Dummy;
	};

	struct StatsComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Stats;
		static constexpr auto EngineClass = "eoc::StatsComponent";

		int field_0;
		std::array<int, 7> Abilities;
		std::array<int, 7> AbilityModifiers;
		std::array<int, 18> Skills;
		int ProficiencyBonus;
		AbilityId SpellCastingAbility;
		int field_8C;
		int field_90;
		int ArmorType;
		int ArmorType2;
		AbilityId UnarmedAttackAbility;
		AbilityId RangedAttackAbility;
	};

	struct StatusImmunitiesComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::StatusImmunities;
		static constexpr auto EngineClass = "eoc::StatusImmunitiesComponent";

		MultiHashMap<FixedString, Guid> PersonalStatusImmunities;
	};

	struct ValueComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Value;
		static constexpr auto EngineClass = "eoc::ValueComponent";

		int Value;
		uint8_t Rarity;
		bool Unique;
	};

	struct WeaponComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Weapon;
		static constexpr auto EngineClass = "eoc::WeaponComponent";

		struct Element1 : StatsExpressionParam
		{
			uint8_t field_28;
			BoostCause Cause;
			TranslatedString field_48;
			uint8_t field_58;
		};


		RefMap<AbilityId, Array<DiceValues>> Rolls;
		RefMap<AbilityId, Array<DiceValues>> Rolls2;
		float WeaponRange;
		float DamageRange;
		void* WeaponFunctors;
		uint32_t WeaponProperties;
		uint8_t WeaponGroup;
		AbilityId Ability;
		Array<Element1> field_38;
		DiceSizeId DamageDice;
		DiceSizeId VersatileDamageDice;
	};

	struct BackgroundComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Background;
		static constexpr auto EngineClass = "eoc::BackgroundComponent";

		Guid Background;
	};
	
	struct GodComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::God;
		static constexpr auto EngineClass = "eoc::god::GodComponent";

		Guid God;
	};


	struct LevelUpUpgrades
	{
		struct AbilityData
		{
			uint8_t field_0;
			Guid Feat;
			Guid Class;
			int field_28;
			int field_2C;
			int field_30;
			Guid Ability;
			Array<uint8_t> Array_b8;
			STDString field_60;
			int field_80;
		};
		
		struct SkillData
		{
			uint8_t field_0;
			Guid field_8;
			Guid field_18;
			int field_28;
			int field_2C;
			int field_30;
			Guid Skill;
			Array<uint8_t> Array_b8;
			Array<uint8_t> Array_b8_2;
			STDString field_60;
			int field_80;
		};
		
		struct SpellData
		{
			struct StringPair
			{
				FixedString A;
				FixedString B;
			};

			uint8_t field_0;
			Guid Race;
			Guid field_18;
			int field_28;
			int field_2C;
			int field_30;
			Guid Spell;
			Array<FixedString> Array_FS;
			Array<StringPair> Array_FS2;
			STDString field_78;
		};
		
		struct LevelUpDataInner4
		{
			uint8_t field_0;
			Guid field_8;
			Guid Class;
			int field_28;
			int field_2C;
			int field_30;
			Guid field_38;
			Array<FixedString> Array_FS;
			STDString field_60;
			int field_80;
		};

		Array<AbilityData> Abilities;
		Array<SkillData> Skills;
		Array<AbilityData> Abilities2;
		Array<SpellData> Spells;
		Array<LevelUpDataInner4> field_48;
	};

	struct LevelUpData3
	{
		FixedString field_0;
		uint8_t field_4;
		uint64_t _Pad;
		Guid field_10;
	};

	struct LevelUpData
	{
		Guid Class;
		Guid SubClass;
		Guid Feat;
		std::array<int, 7> Abilities;
		LevelUpUpgrades Upgrades;
		Array<LevelUpData3> field_B0;
	};


	struct LevelUpComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::LevelUp;
		static constexpr auto EngineClass = "eoc::progression::LevelUpComponent";

		Array<LevelUpData> field_18;
	};

	struct FloatingComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Floating;
		static constexpr auto EngineClass = "eoc::FloatingComponent";

		int field_18;
		int field_1C;
	};

	struct VoiceComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Voice;
		static constexpr auto EngineClass = "eoc::VoiceComponent";

		Guid Voice;
	};

	struct CustomIconComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CustomIcon;
		static constexpr auto EngineClass = "eoc::CustomIconComponent";

		ScratchBuffer Buffer;
	};

	struct CharacterCreationStatsComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CharacterCreationStats;
		static constexpr auto EngineClass = "eoc::CharacterCreationStatsComponent";

		Guid Race;
		Guid SubRace;
		uint8_t field_20;
		uint8_t field_21;
		STDString Name;
		std::array<int32_t, 7> Abilities;
		uint8_t field_5C;
	};

	struct DisarmableComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Disarmable;
		static constexpr auto EngineClass = "eoc::DisarmableComponent";

		Guid field_0;
		uint8_t field_10;
		uint8_t field_11;
	};

	struct ShortRestComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ShortRest;
		static constexpr auto EngineClass = "eoc::rest::ShortRestComponent";

		uint8_t StateId;
	};

	struct IsSummonComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::IsSummon;
		static constexpr auto EngineClass = "eoc::summon::IsSummonComponent";

		ComponentHandle Owner_M;
	};

	struct SummonContainerComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SummonContainer;
		static constexpr auto EngineClass = "eoc::summon::ContainerComponent";

		MultiHashMap<FixedString, Array<EntityHandle>> field_18;
		MultiHashSet<ComponentHandle> Characters;
		MultiHashSet<ComponentHandle> Items;
	};

	struct StealthComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Stealth;
		static constexpr auto EngineClass = "eoc::StealthComponent";

		bool SeekHiddenFlag;
		glm::vec3 Position;
		float SeekHiddenTimeout;
		int field_2C;
	};

	struct ClientControlComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ClientControl;
		static constexpr auto EngineClass = "eoc::ClientControlComponent";

		uint8_t Dummy;
	};

	struct IsGlobalComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::IsGlobal;
		static constexpr auto EngineClass = "ls::IsGlobalComponent";

		uint8_t Flag;
	};

	struct SavegameComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Savegame;
		static constexpr auto EngineClass = "ls::SavegameComponent";

		uint8_t field_0;
	};

	struct DisabledEquipmentComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::DisabledEquipment;
		static constexpr auto EngineClass = "eoc::DisabledEquipmentComponent";

		bool ShapeshiftFlag;
	};

	struct LootingStateComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::LootingState;
		static constexpr auto EngineClass = "eoc::LootingStateComponent";

		EntityHandle Looter_M;
		uint8_t State;
		int field_24;
	};

	struct LootComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Loot;
		static constexpr auto EngineClass = "eoc::LootComponent";

		uint8_t Flags;
		uint8_t InventoryType;
	};

	struct LockComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Lock;
		static constexpr auto EngineClass = "eoc::lock::LockComponent";

		FixedString Key_M;
		int LockDC;
		Guid field_8;
		Array<Guid> field_18;
	};

	struct SummonLifetimeComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SummonLifetime;
		static constexpr auto EngineClass = "eoc::summon::LifetimeComponent";

		float Lifetime;
	};

	struct IconComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Icon;
		static constexpr auto EngineClass = "eoc::IconComponent";

		FixedString Icon;
	};

	struct HotbarContainerComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::HotbarContainer;
		static constexpr auto EngineClass = "eoc::hotbar::ContainerComponent";

		struct Element
		{
			EntityHandle Item;
			SpellId SpellId;
			FixedString Passive;
			uint32_t field_34;
			uint8_t field_38;
		};

		struct Bar
		{
			uint8_t Index;
			uint8_t field_1;
			Array<Element> Elements;
			uint8_t field_18;
			uint32_t field_1C;
			STDString field_20;
		};

		MultiHashMap<FixedString, Array<Bar>> Containers;
		FixedString ActiveContainer;
	};

	/// <summary>
	/// ///////////////////////// START SERVER
	/// </summary>


	struct RecruitedByComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerRecruitedBy;
		static constexpr auto EngineClass = "esv::recruit::RecruitedByComponent";

		EntityHandle RecruitedBy;
	};

	struct GameTimerComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerGameTimer;
		static constexpr auto EngineClass = "esv::GameTimerComponent";

		FixedString field_18;
		EntityHandle field_20;
		int field_28;
		int field_2C;
		int field_30;
		int field_34;
		uint8_t field_38;
	};

	struct ExperienceGaveOutComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerExperienceGaveOut;
		static constexpr auto EngineClass = "esv::exp::ExperienceGaveOutComponent";

		int Experience;
	};


	struct ReplicationDependencyComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerReplicationDependency;
		static constexpr auto EngineClass = "esv::replication::ReplicationDependencyComponent";

		EntityHandle Dependency;
	};

	struct IsUnsummoningComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerIsUnsummoning;
		static constexpr auto EngineClass = "esv::summon::IsUnsummoningComponent";
	};

	struct FleeBlockedComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerFleeBlocked;
		static constexpr auto EngineClass = "esv::combat::FleeBlockedComponent";
	};

	struct ActivationGroupContainerComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerActivationGroupContainer;
		static constexpr auto EngineClass = "esv::ActivationGroupContainerComponent";

		struct ActivationGroup
		{
			FixedString field_0;
			FixedString field_4;
		};

		Array<ActivationGroup> Groups;
	};

	struct AnubisTagComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerAnubisTag;
		static constexpr auto EngineClass = "esv::tags::AnubisTagComponent";

		Array<Guid> Tags;
	};

	struct DialogTagComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerDialogTag;
		static constexpr auto EngineClass = "esv::tags::DialogTagComponent";

		Array<Guid> Tags;
	};

	struct DisplayName
	{
		TranslatedString NameKey;
		uint8_t field_10;
		STDString Name;
	};

	struct DisplayNameTranslatedString
	{
		TranslatedString NameKey;
		uint8_t field_10;
	};

	struct DisplayNameListComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerDisplayNameList;
		static constexpr auto EngineClass = "esv::DisplayNameListComponent";

		Array<DisplayName> Names;
		Array<DisplayNameTranslatedString> TranslatedStrings;
	};

	struct IconListComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerIconList;
		static constexpr auto EngineClass = "esv::IconListComponent";

		struct Icon
		{
			FixedString Icon;
			uint32_t field_4;
		};

		Array<Icon> Icons;
		uint8_t field_30;
	};

	struct RaceTagComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerRaceTag;
		static constexpr auto EngineClass = "esv::tags::RaceTagComponent";

		Array<Guid> Tags;
	};

	struct TemplateTagComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerTemplateTag;
		static constexpr auto EngineClass = "esv::tags::TemplateTagComponent";

		Array<Guid> Tags;
	};

	struct ToggledPassivesComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerToggledPassives;
		static constexpr auto EngineClass = "esv::passive::ToggledPassivesComponent";

		MultiHashMap<FixedString, bool> Passives;
	};

	struct BoostTagComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerBoostTag;
		static constexpr auto EngineClass = "esv::tags::BoostTagComponent";

		Array<Guid> Tags;
	};

	struct SafePositionComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerSafePosition;
		static constexpr auto EngineClass = "esv::SafePositionComponent";

		glm::vec3 Position;
		bool field_24;
	};

	struct AnubisExecutorComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerAnubisExecutor;
		static constexpr auto EngineClass = "esv::AnubisExecutorComponent";

		__int64 field_18;
		__int64 field_20;
		uint8_t field_28;
		uint8_t field_29;
	};

	struct LeaderComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerLeader;
		static constexpr auto EngineClass = "esv::LeaderComponent";

		MultiHashSet<EntityHandle> Followers_M;
	};

	struct BreadcrumbComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerBreadcrumb;
		static constexpr auto EngineClass = "esv::BreadcrumbComponent";

		struct Element
		{
			int field_0;
			uint8_t field_4;
			glm::vec3 field_8;
			glm::vec3 field_14;
		};

		std::array<Element, 8> field_18;
		glm::vec3 field_118;
	};


	struct DelayDeathCauseComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerDelayDeathCause;
		static constexpr auto EngineClass = "esv::death::DelayDeathCauseComponent";

		int DelayCount;
		int Blocked_M;
	};


	struct PickpocketComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerPickpocket;
		static constexpr auto EngineClass = "esv::pickpocket::PickpocketComponent";

		struct PickpocketEntry
		{
			EntityHandle field_0;
			EntityHandle field_8;
			int field_10;
			bool field_14;
			EntityHandle field_18;
		};

		Array<PickpocketEntry> field_18;
	};

	struct ReplicationDependencyOwnerComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerReplicationDependencyOwner;
		static constexpr auto EngineClass = "esv::replication::ReplicationDependencyOwnerComponent";

		Array<EntityHandle> Dependents;
	};

	/// <summary>
	/// //////////////////////////////////////// END SERVER
	/// </summary>


	struct OriginTagComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::OriginTag;
		static constexpr auto EngineClass = "eoc::OriginTagComponent";

		Array<Guid> Tags;
	};

	struct PassiveInfo
	{
		uint32_t field_0;
		uint32_t Passive;
	};

	struct OriginPassivesComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::OriginPassives;
		static constexpr auto EngineClass = "eoc::OriginPassivesComponent";

		Array<PassiveInfo> field_18;
	};

	struct ClassTagComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ClassTag;
		static constexpr auto EngineClass = "eoc::ClassTagComponent";

		Array<Guid> Tags;
	};

	struct BackgroundTagComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::BackgroundTag;
		static constexpr auto EngineClass = "eoc::BackgroundTagComponent";

		Array<Guid> Tags;
	};

	struct BackgroundPassivesComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::BackgroundPassives;
		static constexpr auto EngineClass = "eoc::BackgroundPassivesComponent";

		Array<PassiveInfo> field_18;
	};

	struct GlobalShortRestDisabledComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::GlobalShortRestDisabled;
		static constexpr auto EngineClass = "eoc::GlobalShortRestDisabledComponent";
	};

	struct GlobalLongRestDisabledComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::GlobalLongRestDisabled;
		static constexpr auto EngineClass = "eoc::GlobalLongRestDisabledComponent";
	};

	struct StoryShortRestDisabledComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::StoryShortRestDisabled;
		static constexpr auto EngineClass = "eoc::StoryShortRestDisabledComponent";
	};

	struct FleeCapabilityComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::FleeCapability;
		static constexpr auto EngineClass = "eoc::FleeCapabilityComponent";

		uint8_t field_18;
		float field_1C;
		float field_20;
	};

	struct CanDoRestComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CanDoRest;
		static constexpr auto EngineClass = "eoc::CanDoRestComponent";

		uint8_t field_18;
		uint8_t field_19;
		uint8_t field_1A;
		uint8_t field_1B;
	};

	struct ActiveCharacterLightComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ActiveCharacterLight;
		static constexpr auto EngineClass = "eoc::light::ActiveCharacterLightComponent";

		FixedString Light;
	};

	struct VisualComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Visual;
		static constexpr auto EngineClass = "ls::VisualComponent";

		__int64 field_18;
		uint8_t field_20;
		uint8_t field_21;
	};

	struct CanModifyHealthComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CanModifyHealth;
		static constexpr auto EngineClass = "eoc::CanModifyHealthComponent";
		
		uint16_t Flags;
	};

	struct AvailableLevelComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AvailableLevel;
		static constexpr auto EngineClass = "eoc::exp::AvailableLevelComponent";

		int Level;
	};

	struct CanBeLootedComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CanBeLooted;
		static constexpr auto EngineClass = "eoc::CanBeLootedComponent";

		uint16_t Flags;
	};

	struct CanBeDisarmedComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CanBeDisarmed;
		static constexpr auto EngineClass = "eoc::CanBeDisarmedComponent";

		uint16_t Flags;
	};

	struct CanTriggerRandomCastsComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CanTriggerRandomCasts;
		static constexpr auto EngineClass = "eoc::CanTriggerRandomCastsComponent";

		uint8_t Dummy;
	};

	struct CanDoActionsComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CanDoActions;
		static constexpr auto EngineClass = "eoc::CanDoActionsComponent";

		uint16_t Flags;
	};

	struct CanMoveComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CanMove;
		static constexpr auto EngineClass = "eoc::CanMoveComponent";

		uint16_t Flags;
	};

	struct CanSenseComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CanSense;
		static constexpr auto EngineClass = "eoc::CanSenseComponent";

		uint16_t Flags;
	};

	struct ConcentrationComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Concentration;
		static constexpr auto EngineClass = "eoc::concentration::ConcentrationComponent";

		struct Element
		{
			EntityHandle field_0;
			EntityHandle field_8;
			EntityHandle field_10;
			int16_t field_18;
			bool field_1A;
		};

		EntityHandle field_0;
		Array<Element> field_8;
		SpellId SpellId;
	};

	struct DarknessComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Darkness;
		static constexpr auto EngineClass = "eoc::DarknessComponent";

		uint8_t field_18;
		uint8_t field_19;
		uint8_t field_1A;
		uint8_t field_1B;
		uint8_t field_1C;
		uint8_t field_1D;
		int field_20;
	};

	struct DualWieldingComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::DualWielding;
		static constexpr auto EngineClass = "eoc::DualWieldingComponent";

		uint8_t field_18;
		uint8_t field_19;
		uint8_t field_1A;
		uint8_t field_1B;
		uint8_t field_1C;
		uint8_t field_1D;
		uint8_t field_1E;
	};

	struct GameObjectVisualData
	{
		struct AppearanceElement
		{
			Guid Material;
			Guid Color;
			float ColorIntensity;
			uint32_t MetallicTint;
			float GlossyTint;
		};

		ScratchBuffer Buffer;
		Guid field_58;
		Guid field_68;
		Guid field_78;
		Guid field_88;
		Guid field_98;
		Array<Guid> Visuals;
		Array<AppearanceElement> Elements;
		Array<int32_t> field_C8;
	};

	struct GameObjectVisualComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::GameObjectVisual;
		static constexpr auto EngineClass = "eoc::GameObjectVisualComponent";

		FixedString RootTemplateId;
		uint8_t RootTemplateType;
		FixedString Icon;
		float field_24;
		uint8_t field_28;
		GameObjectVisualData VisualData;
		FixedString field_F0;
	};

	struct GravityDisabledComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::GravityDisabled;
		static constexpr auto EngineClass = "eoc::GravityDisabledComponent";

		uint8_t Dummy;
	};

	struct GravityDisabledUntilMovedComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::GravityDisabledUntilMoved;
		static constexpr auto EngineClass = "eoc::GravityDisabledUntilMovedComponent";

		uint8_t Dummy;
	};

	struct InteractionFilterComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::InteractionFilter;
		static constexpr auto EngineClass = "eoc::InteractionFilterComponent";

		MultiHashSet<Guid> field_0;
		uint8_t field_30;
		uint8_t field_31;
	};

	struct InvisibilityComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Invisibility;
		static constexpr auto EngineClass = "eoc::InvisibilityComponent";

		uint8_t field_0;
		glm::vec3 field_4;
		uint8_t field_10;
	};

	struct IsInTurnBasedModeComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::IsInTurnBasedMode;
		static constexpr auto EngineClass = "eoc::IsInTurnBasedModeComponent";

		uint8_t Dummy;
	};

	struct ItemBoostsComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ItemBoosts;
		static constexpr auto EngineClass = "eoc::ItemBoostsComponent";

		Array<EntityHandle> Boosts;
	};

	struct DisplayNameComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::DisplayName;
		static constexpr auto EngineClass = "eoc::DisplayNameComponent";

		TranslatedString NameKey;
		TranslatedString UnknownKey;
		STDString Name;
	};

	struct EquipableComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Equipable;
		static constexpr auto EngineClass = "eoc::EquipableComponent";

		Guid field_18;
		ItemSlot Slot;
	};

	struct GameplayLightComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::GameplayLight;
		static constexpr auto EngineClass = "eoc::GameplayLightComponent";

		int field_18;
		uint8_t field_1C;
		int field_20;
		int field_24;
		int field_28;
	};

	struct ProgressionContainerComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ProgressionContainer;
		static constexpr auto EngineClass = "eoc::ProgressionContainerComponent";

		Array<Array<EntityHandle>> Progressions;
	};

	struct ProgressionMetaComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ProgressionMeta;
		static constexpr auto EngineClass = "eoc::progression::MetaComponent";

		uint8_t field_18;
		Guid Progression;
		Guid Race;
		int field_40;
		uint8_t field_44;
		int field_48;
		EntityHandle Owner;
	};


	struct RaceComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Race;
		static constexpr auto EngineClass = "eoc::RaceComponent";

		Guid Race;
	};

	struct CanTravelComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CanTravel;
		static constexpr auto EngineClass = "eoc::CanTravelComponent";

		__int16 field_18;
		__int16 field_1A;
		__int16 field_1C;
	};

	struct MovementComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Movement;
		static constexpr auto EngineClass = "eoc::MovementComponent";

		float field_18;
		float field_1C;
		float field_20;
		float field_24;
		int field_28;
		int field_2C;
	};

	struct ObjectInteractionComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ObjectInteraction;
		static constexpr auto EngineClass = "eoc::ObjectInteractionComponent";

		Array<ComponentHandle> field_0;
	};

	struct StaticPhysicsComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::StaticPhysics;
		static constexpr auto EngineClass = "ls::StaticPhysicsComponent";

		void* field_0;
	};

	struct PathingComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Pathing;
		static constexpr auto EngineClass = "eoc::PathingComponent"; 
		
		struct Param
		{
			Array<int32_t> Values1;
			Array<float> Values2;
			Array<glm::vec3> Values3;
			int field_30;
		};


		RefMap<FixedString, glm::vec4> VectorParameters;
		RefMap<FixedString, Param> PathParameters;
		__int64 field_20;
		int field_28;
		FixedString field_2C;
		__int64 field_30;
		int field_38;
		int field_3C;
		uint8_t field_40;
		float field_44;
		int field_48;
		uint8_t field_4C;
	};

	struct SteeringComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Steering;
		static constexpr auto EngineClass = "eoc::SteeringComponent";

		glm::vec3 field_0;
		float field_C;
		float field_10;
		uint8_t field_14;
		float field_18;
		uint8_t field_1C;
	};

	struct CanDeflectProjectilesComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CanDeflectProjectiles;
		static constexpr auto EngineClass = "eoc::CanDeflectProjectilesComponent";

		uint16_t Flags;
	};

	struct ActiveSkeletonSlotsComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ActiveSkeletonSlots;
		static constexpr auto EngineClass = "ls::ActiveSkeletonSlotsComponent";

		Array<FixedString> Slots;
	};

	struct NetComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Net;
		static constexpr auto EngineClass = "esv::NetComponent";

		uint8_t field_0;
	};

	struct PhysicsComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Physics;
		static constexpr auto EngineClass = "ls::PhysicsComponent";

		__int64 field_18;
	};

	struct SomeSharedServerClientObjId
	{
		__int64 field_0;
		int field_8;
	};


	struct SomeSharedServerClientObjId2 : public SomeSharedServerClientObjId
	{
		__int64 field_10;
		int field_18;
	};

	struct FTBParticipantComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::FTBParticipant;
		static constexpr auto EngineClass = "eoc::ftb::ParticipantComponent";

		EntityHandle field_18;
	};


	struct ApprovalRatingsComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ApprovalRatings;
		static constexpr auto EngineClass = "eoc::approval::RatingsComponent";

		MultiHashMap<EntityHandle, int> Ratings;
		MultiHashSet<Guid> field_70;
	};


	struct AttitudesToPlayersComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AttitudesToPlayers;
		static constexpr auto EngineClass = "eoc::attitude::AttitudesToPlayersComponent";

		struct Key
		{
			EntityHandle field_0;
			uint8_t field_8;
			Guid field_10;
			uint8_t field_20;
		};

		MultiHashMap<Key, int> Attitudes;
	};


	struct CharacterCreationAppearanceComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CharacterCreationAppearance;
		static constexpr auto EngineClass = "eoc::character_creation::AppearanceComponent";

		Array<Guid> Visuals;
		Array<GameObjectVisualData::AppearanceElement> Elements;
		Array<int32_t> AdditionalChoices;
		Guid SkinColor;
		Guid EyeColor;
		Guid SecondEyeColor;
		Guid HairColor;
	};
}

BEGIN_NS(inventory)

struct DataComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InventoryData;
	static constexpr auto EngineClass = "eoc::inventory::DataComponent";

	uint8_t field_0;
	uint16_t Flags;
};

struct OwnerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InventoryOwner;
	static constexpr auto EngineClass = "eoc::inventory::OwnerComponent";

	Array<EntityHandle> Inventories;
	EntityHandle PrimaryInventory;
};

struct ContainerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InventoryContainer;
	static constexpr auto EngineClass = "eoc::inventory::ContainerComponent";

	struct Item
	{
		EntityHandle Item;
		uint32_t field_8;
	};

	MultiHashMap<uint16_t, Item> Items;
};

struct MemberComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InventoryMember;
	static constexpr auto EngineClass = "eoc::inventory::MemberComponent";

	EntityHandle Inventory;
	int16_t EquipmentSlot;
};

END_NS()
