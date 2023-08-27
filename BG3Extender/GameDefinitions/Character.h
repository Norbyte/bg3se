#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/GameState.h>
/*
#include "Status.h"
#include "ActionMachine.h"
#include "Sound.h"
*/

namespace bg3se
{
	struct CharacterTemplate;

	namespace eoc
	{
		/*struct PlayerUpgrade : public ProtectedGameObject<PlayerUpgrade>
		{
			CDivinityStats_Character * Stats;
			uint32_t AttributePoints;
			uint32_t CombatAbilityPoints;
			uint32_t CivilAbilityPoints;
			uint32_t TalentPoints;
			uint32_t Unknown;
			ObjectSet<int> Attributes;
			ObjectSet<int> Abilities;
			BitArray<4> Talents;
			ObjectSet<uint16_t> Traits;
			bool IsCustom;
		};*/

		struct PlayerCustomData : public ProtectedProxyGameObject<PlayerCustomData>
		{
			void* VMT;
			bool Initialized;
			bool CustomLookEnabled;
			uint32_t ClothColor1;
			uint32_t ClothColor2;
			uint32_t ClothColor3;
			FixedString ClassType;
			FixedString MusicInstrumentID;
			FixedString OwnerProfileID;
			FixedString ReservedProfileID;
			FixedString AiPersonality;
		};
	}

	using PlayerId = int16_t;
	using InputPlayerIndex = uint32_t;

	/*struct PlayerManager
	{
		struct PlayerInfo
		{
			uint32_t PlayerId;
			void* RenderView;
		};

		void * VMT;
		uint16_t NextPlayerId;
		ObjectSet<PlayerId> FreePlayerIds;
		ObjectSet<PlayerId> PlayerList;
		RefMap<PlayerId, PlayerInfo> PlayerIds;
		RefMap<InputPlayerIndex, PlayerId> InputPlayerIndices;
		ObjectSet<void *> RenderViews; // rf::RenderView*
	};*/


	namespace esv
	{
		struct Status;
		struct StatusMachine;

		/*struct SkillConditions
		{
			int32_t MinimumHealthPercentage;
			int32_t MaximumHealthPercentage;
			bool HasNoPhysicalArmor;
			bool HasNoMagicalArmor;
			ObjectSet<FixedString> Tags;
		};

		struct SkillInfo
		{
			float ScoreModifier;
			int32_t StartRound;
			int32_t MinimumImpact;
			bool OnlyCastOnSelf;
			uint8_t AIFlags; // Enum
			SkillConditions SourceConditions;
			SkillConditions TargetConditions;
			bool CasualExplorer;
			bool Classic;
			bool TacticianHardcore;
			bool HonorHardcore;
			uint32_t Unknown;
		};

		struct Skill : public ProtectedGameObject<Skill>
		{
			void * VMT;
			FixedString UnknownFS;
			NetId NetID;
			SkillInfo Info;
			ObjectSet<ComponentHandle> CauseList;
			ComponentHandle UnknownHandle;
			uint32_t Unknown1;
			ComponentHandle OwnerHandle;
			FixedString SkillId;
			float ActiveCooldown;
			bool IsActivated;
			bool IsLearned;
			bool ZeroMemory;
			bool OncePerCombat;
			bool Unknown2;
			int32_t NumCharges;
			uint64_t Unknown3;
		};

		struct SkillManager : public ProtectedGameObject<SkillManager>
		{
			void * FreeSkillState;
			ComponentHandle OwnerHandle;
			Map<FixedString, Skill *> Skills;
			RefMap<FixedString, uint32_t> TimeItemAddedToSkillManager;
			bool IsLoading;
			uint32_t FreeMemorySlots;
		};

		struct SkillBarItem : public ProtectedGameObject<SkillBarItem>
		{
			enum ItemType : uint32_t
			{
				kNone = 0,
				kSkill = 1,
				kItem = 2
			};

			ItemType Type;
			FixedString SkillOrStatId;
			ComponentHandle ItemHandle;
		};*/

		struct PlayerCustomData : public eoc::PlayerCustomData {};

		struct PlayerData : public ProtectedProxyGameObject<PlayerData>
		{
			ComponentHandle PlayerHandle;
			uint64_t SkillBar;
			uint64_t ShapeShiftVariableManagers[10];
			Map<FixedString, void*> ShapeShiftAttitudeMaps;
			bool LevelUpMarker;
			FixedString QuestSelected;
			PlayerCustomData CustomData;
			EntityHandle SomeHandle;
			ObjectSet<glm::vec3> PreviousPositions;
			int PreviousPositionId;
			bool HelmetOption;
			uint32_t Renown;
			uint8_t CachedTension;
			__int64 field_E0;
			bool IsInDangerZone;
			FixedString OriginalTemplate;
			FixedString Region;
			char field_100;
		};

		struct Character : public IEoCServerObject
		{
			static constexpr ExtComponentType ComponentType = ExtComponentType::ServerCharacter;

			Status* GetStatus(ComponentHandle statusHandle) const;
			Status* GetStatus(NetId handle) const;

			FixedString GUID; // Part of IEoCServerObject?
			NetId NetID; // Part of IEoCServerObject?
			uint64_t Flags;
			FixedString CurrentLevel;
			ecs::EntityRef Handle;
			Array<uint32_t> VisibleToPeerIds;
			CharacterTemplate* CurrentTemplate;
			CharacterTemplate* OriginalTemplate;
			CharacterTemplate* TemplateUsedForSpells;
			uint8_t Flags2;
			uint8_t Flags3;
			bool WaitForTeleportAck;
			int8_t Team;
			int HasOsirisDialog;
			ObjectSet<void*> VoiceOverrides;
			bool NeedsUpdate;
			bool ForceSynch;
			int8_t field_133;
			ComponentHandle InventoryHandle;
			void* MovementMachine_M;
			void* BehaviourMachine;
			void* ActionMachine_M;
			void* SteeringMachine;
			void* CharacterSupervisor;
			void* DialogController;
			void* FallbackController;
			void* NetworkController_M;
			void* OsirisController;
			void* GameplayController;
			StatusMachine* StatusMachine;
			void* SkillManager_M;
			void* VariableManager;
			void* ShapeShiftVariableManager_M;
			Map<uint32_t, void*> Attitudes;
			void* SkillBeingPrepared_M;
			void** pCurrentTemplate_MAYBE;
			int Dialog;
			bool IsDialogAiControlled;
			float TurnTimer;
			float TriggerTrapsTimer;
			UserId UserID;
			UserId ReservedUserID;
			EntityHandle OwnerCharacterHandle;
			EntityHandle FollowCharacterHandle;
			EntityHandle EnemyCharacterHandle;
			EntityHandle SpiritCharacterHandle;
			ObjectSet<EntityHandle> EnemyHandleSet;
			Array<void*> SurfacePathInfluenceSet;
			ObjectSet<EntityHandle> SummonHandleSet;
			FixedString field_190;
			ObjectSet<ComponentHandle> RegisteredTriggerHandles;
			ObjectSet<FixedString> RegisteredTriggers;
			PlayerData* PlayerData;
			int LoseControl_M;
			int field_474;
			float MovementSpeedMultiplier;
			float MovementSpeedMultiplier2;
			ComponentHandle PartyHandle;
			ObjectSet<FixedString> CreatedTemplateItems;
			ObjectSet<FixedString> Treasures;
			FixedString CustomTradeTreasure;
			uint32_t CrimeHandle;
			uint32_t PreviousCrimeHandle;
			uint8_t CrimeState;
			uint8_t PreviousCrimeState;
			bool CrimeWarningsEnabled;
			bool CrimeInterrogationEnabled;
			bool BlockNewDisturbanceReactions;
			ObjectSet<FixedString> DisabledCrimes;
			uint64_t DamageCounter;
			uint64_t HealCounter;
			uint64_t KillCounter;
			EntityHandle MovingCasterHandle;
			FixedString EquipmentColor;
			FixedString ProjectileTemplate;
			uint32_t TimeElapsed;
			ObjectSet<Guid> PreferredAiTargetTags;
			void* CharacterBody_M;
			RefMap<EntityHandle, ObjectSet<void*>> StatusesFromObjects;
			ObjectSet<FixedString> TagsFromItems;
			bool ReadyCheckBlocked;
			uint8_t NumConsumables;
			bool CorpseLootable;
			uint8_t CharmedNumConsumablesConsumed;
			FixedString PreviousLevel;
		};
	}

	HAS_OBJECT_PROXY(esv::Character);

	namespace ecl 
	{
		/*struct PlayerManager : public bg3se::PlayerManager
		{
			void* VMT_IProfileSelector;
			void* VMT_EventListener;
			void* VMT_PlayerManager2;
			RefMap<PlayerId, FixedString> PlayerIdToProfileGuid;
			RefMap<PlayerId, NetId> PlayerIdToNetIdMap;
			ObjectSet<InputPlayerIndex> InputPlayerIndices;
			uint64_t Unknown[3];
			ObjectSet<void*> field_F0;
			uint64_t Unknown2[6];
		};

		struct PlayerCustomData : public eoc::PlayerCustomData {};

		struct PlayerData : public ProtectedGameObject<PlayerData>
		{
			ObjectSet<void*> SkillBarItems; // ecl::SkillBarItem
			ObjectSet<uint32_t> LockedAbilities;
			Map<FixedString, void*> AttitudeOverrideMap; // ObjectHandleMap<int>*
			uint8_t SelectedSkillSet;
			__int64 field_60;
			char field_68;
			PlayerCustomData CustomData;
			char field_1C8;
			__int64 field_1D0;
			NetId PickpocketTargetNetID;
			NetId CorpseLootTargetNetID;
			bool HelmetOptionState;
			bool ArmorOptionState;
			int CachedTension;
			FixedString QuestSelected;
			ObjectSet<FixedString> MemorisedSkills;
			char field_210;
			char field_211;
			int NetID3;
			FixedString OriginalTemplate;
			FixedString Region;
		};



		struct Character : public IEocClientObject
		{
			Status* GetStatus(ComponentHandle statusHandle) const;
			Status* GetStatus(NetId handle) const;

			glm::vec3 WorldPos; // Saved
			uint32_t _Pad2;
			uint64_t Flags; // Saved
			uint32_t U2;
			FixedString CurrentLevel; // Saved
			glm::mat3 WorldRot;
			float Scale;
			glm::vec3 Velocity;
			int field_34;
			int field_38;
			uint64_t field_40;
			__int64 field_48;
			__int64 field_50;
			void* PhysicsObject;
			void* Light;
			void* AiObject;
			int field_70;
			UserId UserID;
			int32_t UserId2;
			NetId NetID2;
			NetId NetID3;
			CharacterTemplate* Template;
			CharacterTemplate* OriginalTemplate;
			CDivinityStats_Character* Stats;
			ComponentHandle InventoryHandle;
			void* MovementMachine;
			void* ActionStateMachine;
			void* SteeringMachine;
			void* BehaviourMachine;
			void* InputController;
			void* NetworkController;
			void* StatusController;
			void* DialogController;
			void* CharacterSupervisir;
			StatusMachine* StatusMachine;
			void* SkillManager;
			int field_100;
			__int64 field_108;
			void* CharacterBody;
			ComponentHandle OwnerCharacterHandle;
			ComponentHandle OH3;
			ComponentHandle CorpseCharacterHandle;
			ComponentHandle OH5;
			int field_138;
			ComponentHandle HighlightCircleEffect;
			ComponentHandle OH7;
			ComponentHandle ViewConeEffectHandle;
			__int64 field_158;
			__int64 field_160;
			ecl::PlayerData* PlayerData;
			__int64 field_170;
			int AttributeGrowthBonus;
			int CombatAbilityBonus;
			int CivilAbilityBonus;
			int TalentGrowthBonus;
			__int64 field_188;
			__int64 field_190;
			__int64 field_198;
			__int64 field_1A0;
			__int64 field_1A8;
			__int64 field_1B0;
			__int64 field_1B8;
			__int64 field_1C0;
			__int64 field_1C8;
			__int64 field_1D0;
			__int64 field_1D8;
			__int64 field_1E0;
			__int64 field_1E8;
			__int64 field_1F0;
			__int64 field_1F8;
			char field_200;
			ObjectSet<void*> SurfacePathInfluences;
			ObjectSet<FixedString> Tags;
			__int64 field_248;
			float field_250;
			SoundObjectId SoundObjectHandles[3];
			ComponentHandle OH9;
			ComponentHandle FollowCharacterHandle;
			char PickpocketNLootingFlags;
			char Flags2;
			int Flags3;
			char field_288;
			char field_289;
			TranslatedString* DisplayNameOverride;
			TranslatedString StoryDisplayName;
			TranslatedString OriginalDisplayName;
			ComponentHandle TalkingIconEffect;
			float field_3F0;
			int SoundBoneIndex;
			int field_3F8;
			int field_3FC;
			ObjectSet<FixedString> FixedStrings2;
			FixedString AnimationSetOverride;
			float WalkSpeedOverride;
			float RunSpeedOverride;
			__int64 field_430;
			__int64 OH12;
			__int64 field_440;
			void* ResourceTemplate2;
			char Cloth;
			void* ResourceTemplate1;
			int field_460;
			__int64 field_468;
			__int64 field_470;
			__int64 field_478;
			ComponentHandle OH13;
			FixedString Archetype;
			FixedString FS3;
			int field_498;
			char field_49C;
			ObjectSet<ComponentHandle> ObjectHandles;
			Map<FixedString, void*> field_4C0;
			__int64 field_4D8;
			__int64 field_4E0;
			ObjectSet<FixedString> ItemTags;
			void* VisualSetIndices;
			bool CorpseLootable;
		};*/

	}
}
