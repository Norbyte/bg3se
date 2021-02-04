#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>
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

		struct PlayerCustomData : public ProtectedGameObject<PlayerCustomData>
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
			ObjectSet<ObjectHandle> CauseList;
			ObjectHandle UnknownHandle;
			uint32_t Unknown1;
			ObjectHandle OwnerHandle;
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
			ObjectHandle OwnerHandle;
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
			ObjectHandle ItemHandle;
		};*/

		struct PlayerCustomData : public eoc::PlayerCustomData {};

		struct PlayerData : public ProtectedGameObject<PlayerData>
		{
			ObjectHandle PlayerHandle;
			uint64_t SkillBar;
			uint64_t ShapeShiftVariableManagers[10];
			Map<FixedString, void*> ShapeShiftAttitudeMaps;
			bool LevelUpMarker;
			FixedString QuestSelected;
			PlayerCustomData CustomData;
			ObjectHandle SomeHandle;
			ObjectSet<glm::vec3> PreviousPositions;
			int PreviousPositionId;
			bool HelmetOption;
			uint32_t Renown;
			uint8_t CachedTension;
			__int64 field_E0;
			uint8_t WaypointTravelState;
			uint8_t RestState;
			bool IsInCamp;
			bool CampWaypointEnabled;
			bool IsInDangerZone;
			ObjectHandle RecruiterHandle;
			FixedString OriginalTemplate;
			FixedString Region;
			char field_100;
		};

		struct Character : public IEoCServerObject
		{
			static constexpr ExtComponentType ComponentType = ExtComponentType::ServerCharacter;

			Status* GetStatus(ObjectHandle statusHandle) const;
			Status* GetStatus(NetId handle) const;

			FixedString GUID; // Part of IEoCServerObject?
			NetId NetID; // Part of IEoCServerObject?
			uint64_t Flags;
			FixedString CurrentLevel;
			EntityHandle Handle;
			Array<uint32_t> VisibleToPeerIds;
			void* CurrentTemplate;
			void* OriginalTemplate;
			void* TemplateUsedForSpells;
			uint8_t Flags2;
			uint8_t Flags3;
			bool WaitForTeleportAck;
			int8_t Team;
			int HasOsirisDialog;
			ObjectSet<void*> VoiceOverrides;
			bool NeedsUpdate;
			bool ForceSynch;
			int8_t field_133;
			ObjectHandle InventoryHandle;
			void* MovementMachine_M;
			void* BehaviourMachine;
			void* ActionMachine_M;
			void* SteeringMachine;
			void* CharacterSupervisor;
			void* DialogController;
			void* FallbackController;
			void* NetworkController_M;
			void* OsirisController;
			void* ScriptController;
			void* GameplayController;
			StatusMachine* StatusMachine;
			void* SkillManager_M;
			void* VariableManager;
			void* ShapeShiftVariableManager_M;
			Map<uint32_t, void*> Attitudes;
			void* SkillBeingPrepared_M;
			void** pCurrentTemplate_MAYBE;
			int Dialog;
			char IsDialogAiControlled;
			float LifeTime;
			float TurnTimer;
			float TriggerTrapsTimer_M;
			UserId UserID;
			UserId ReservedUserID;
			int field_214;
			ObjectHandle OwnerCharacterHandle;
			ObjectHandle FollowCharacterHandle;
			ObjectHandle EnemyCharacterHandle;
			ObjectHandle SpiritCharacterHandle;
			ObjectHandle CorpseCharacterHandle;
			ObjectHandle ObjectHandle6;
			ObjectSet<ObjectHandle> EnemyHandleSet;
			ObjectSet<void*> SurfacePathInfluenceSet;
			ObjectSet<ObjectHandle> SummonHandleSet;
			void* PlanManager;
			float MovementAP;
			char AnimType;
			char DelayDeathCount;
			void* DyingStatus;
			ObjectSet<ObjectHandle> RegisteredTriggerHandles_M;
			ObjectSet<FixedString> RegisteredTriggerFSSet;
			PlayerData* PlayerData;
			int MaxMagicArmorPatchCheck_M;
			float field_474;
			float field_478;
			ObjectHandle PartyHandle;
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
			ObjectHandle MovingCasterHandle;
			FixedString EquipmentColor;
			FixedString ProjectileTemplate;
			uint32_t TimeElapsed;
			ObjectSet<UUID> PreferredAiTargetTags;
			void* CharacterBody_M;
			RefMap<ObjectHandle, ObjectSet<void*>> StatusesFromObjects;
			ObjectSet<FixedString> TagsFromItems;
			bool ReadyCheckBlocked;
			uint8_t NumConsumables;
			bool CorpseLootable;
			uint8_t CharmedNumConsumablesConsumed;
			FixedString PreviousLevel;
		};
	}

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
			Status* GetStatus(ObjectHandle statusHandle) const;
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
			ObjectHandle InventoryHandle;
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
			ObjectHandle OwnerCharacterHandle;
			ObjectHandle OH3;
			ObjectHandle CorpseCharacterHandle;
			ObjectHandle OH5;
			int field_138;
			ObjectHandle HighlightCircleEffect;
			ObjectHandle OH7;
			ObjectHandle ViewConeEffectHandle;
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
			ObjectHandle OH9;
			ObjectHandle FollowCharacterHandle;
			char PickpocketNLootingFlags;
			char Flags2;
			int Flags3;
			char field_288;
			char field_289;
			TranslatedString* DisplayNameOverride;
			TranslatedString StoryDisplayName;
			TranslatedString OriginalDisplayName;
			ObjectHandle TalkingIconEffect;
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
			ObjectHandle OH13;
			FixedString Archetype;
			FixedString FS3;
			int field_498;
			char field_49C;
			ObjectSet<ObjectHandle> ObjectHandles;
			Map<FixedString, void*> field_4C0;
			__int64 field_4D8;
			__int64 field_4E0;
			ObjectSet<FixedString> ItemTags;
			void* VisualSetIndices;
			bool CorpseLootable;
		};*/

	}
}
