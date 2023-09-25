#pragma once

#include <GameDefinitions/Base/Base.h>
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
		struct PlayerCustomData : public ProtectedGameObject<PlayerCustomData>
		{
			void* VMT;
			bool Initialized;
			FixedString OwnerProfileID;
			FixedString ReservedProfileID;
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

		struct PlayerCustomData : public eoc::PlayerCustomData {};

		struct PlayerData : public ProtectedGameObject<PlayerData>
		{
			EntityHandle PlayerHandle;
			MultiHashMap<EntityHandle, void*> ShapeShiftVariableManager;
			FixedString QuestSelected;
			int field_4C;
			eoc::PlayerCustomData CustomData;
			EntityHandle field_68;
			Array<glm::vec3> PreviousPositions;
			int PreviousPositionId;
			bool HelmetOption;
			int Renown;
			uint8_t CachedTension;
			bool IsInDangerZone;
			FixedString Region;
			bool field_94;
		};

		struct Character
		{
			Status* GetStatus(FixedString statusId);
			Status* GetStatusByType(StatusType type);

			void* VMT;
			void* VMT2;
			EntityHandle field_10;
			CharacterFlags Flags;
			FixedString Level;
			ecs::EntityRef MyHandle;
			Array<PeerId> UpdatePeerIds;
			Array<void*> field_48;
			Array<void*> field_58;
			Array<void*> field_68;
			Array<FixedString> CreatedTemplateItems;
			Array<FixedString> Treasures;
			Array<FixedString> DisabledCrime;
			Array<Guid> PreferredAiTargets;
			Array<FixedString> field_B8;
			void* Template;
			void* OriginalTemplate;
			void* TemplateUsedForSpells;
			void* field_E0;
			void* BehaviourMachine;
			void* field_F0;
			void* SteeringMachine;
			void* CharacterSupervisor;
			void* DialogController;
			void* field_110;
			void* field_118;
			void* OsirisController;
			void* field_128;
			StatusMachine* StatusManager;
			void* VariableManager;
			void* ShapeShiftingVariableManager;
			void* field_148;
			PlayerData* PlayerData;
			EntityHandle Inventory;
			EntityHandle OwnerCharacter;
			EntityHandle FollowCharacter;
			EntityHandle EnemyCharacter;
			int Dialog;
			FixedString field_14C;
			FixedString CustomTradeTreasure;
			FixedString PreviousLevel;
			UserId UserID;
			UserId UserID2;
			float InvestigationTimer;
			int32_t CrimeHandle;
			int32_t PreviousCrimeHandle;
			int field_16C;
			uint8_t CrimeState;
			bool PreviousCrimeState;
			bool BlockNewDisturbanceReactions;
			bool field_173;
			uint8_t HasOsirisDialog;
			uint8_t NeedsUpdate;
			uint8_t ForceSynch;
			uint8_t NumConsumables;
			CharacterFlags2 Flags2;
			CharacterFlags3 Flags3;
		};

		struct CharacterComponent : public BaseComponent
		{
			static constexpr ExtComponentType ComponentType = ExtComponentType::ServerCharacter;
			static constexpr auto EngineClass = "esv::Character";

			Character* Character;
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
