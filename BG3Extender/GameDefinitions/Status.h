#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Misc.h>
#include <GameDefinitions/Stats/Common.h>
#include <GameDefinitions/Hit.h>
#include <variant>

namespace bg3se
{
	namespace esv
	{
		struct Status : public ProtectedGameObject<Status>
		{
			virtual ~Status() = 0;
			virtual void SetHandle(uint64_t handle) = 0;
			virtual ComponentHandle GetHandle(uint64_t handle) = 0;
			virtual StatusType GetStatusId() = 0;
			// 0 - Stackable
			// 1 - Apply only the first instance, discard new ones
			// 2 - Apply only one instance, new instances replace old ones
			// 3 - Apply only the first instance; triggers combat?
			virtual uint32_t GetStatusType() = 0;
			// TODO - map VMTs later

			Guid field_8;
			__int64 field_18;
			int StoryActionID;
			ActionOriginator Originator;
			double field_48;
			FixedString StatusId;
			float StartTimer;
			float LifeTime;
			float CurrentLifeTime;
			float TurnTimer;
			float Strength;
			uint8_t CauseType;
			uint8_t OriginCauseType;
			EntityHandle SourceEquippedItem;
			EntityHandle SourceUsedItem;
			ComponentHandle StatusHandle;
			EntityHandle SyncEntity;
			EntityHandle Owner;
			Array<EntityHandle> StatusOwner;
			ecs::EntityRef Cause;
			Guid CauseGUID;
			EntityHandle StatusSource;
			ecs::EntityRef RootCause;
			EntityHandle field_E0;
			Guid field_E8;
			stats::ConditionId RemoveConditions;
			uint32_t RemoveEvents;
			StatusFlags Flags;
			StatusFlags2 Flags2;
			StatusFlags3 Flags3;
			bool FreezeDuration;
			uint8_t ExitCause;
			uint8_t field_105;
			uint8_t SpellCastingAbility;
			SpellId SourceSpell;
			Guid SpellCastSourceUuid;
			FixedString StackId;
			int StackPriority;
			bool IsRecoverable;
			FixedString DifficultyStatus;
			Guid StackedSpellCastSourceUuid;
			Array<void*> ConditionRolls;
			uint8_t TickType;
			StatusFlags4 Flags4;
			// Flags5 is gone in patch3?
			// StatusFlags5 Flags5;
		};

		struct StatusAura : public Status
		{
			void* Aura;
		};

		struct StatusBoost : public StatusAura
		{
			ObjectSet<FixedString> Spell;
			ObjectSet<FixedString> Items;
			bool LoseControl;
			ObjectSet<EntityHandle> ItemHandles;
			float EffectTime;
			FixedString BoostStackId;
			glm::vec3 SourceDirection;
			ObjectSet<uint32_t> SurfaceChanges;
		};

		struct StatusClimbing : public Status
		{
			__int64 field_120;
			int field_128;
			int field_12C;
			glm::vec3 MoveDirection_M;
			int field_13C;
			__int64 field_140;
			EntityHandle Item;
			FixedString Level;
			uint8_t Status;
			bool Direction;
			bool JumpUpLadders_M;
			bool Incapacitated_M;
			bool Started_M;
		};

		struct StatusIncapacitated : public StatusBoost
		{
			float CurrentFreezeTime;
			float FreezeTime;
			uint8_t IncapacitateFlags;
			bool IncapacitationAnimationFinished;
		};

		struct StatusDowned : public StatusIncapacitated
		{
			int NumStableSuccess;
			int NumStableFailed;
			int StableRollDC;
			void* StableRoll_LuaExpression;
			void* OnRollsFailed;
			void* OnSuccess;
			void* Roll_M;
			bool IsStable;
			bool IsHealed;
			int RollSuccesses;
			int RollFailures;
			int DamageFailures;
		};

		struct StatusDying : public Status
		{
			ecs::EntityRef Source;
			__int64 field_128;
			EntityHandle field_130;
			HitDesc HitDescription;
			Guid Combat;
			uint8_t DyingFlags;
		};

		struct StatusHeal : public Status
		{
			float EffectTime;
			int HealAmount;
			int HealEffect;
			FixedString HealEffectId;
			uint8_t HealType; // TODO enum
			int AbsorbSurfaceRange;
			uint8_t TargetDependentHeal;
			ObjectSet<void*> AbsorbSurfaceTypes;
			int TargetDependentValue;
			int TargetDependentHealAmount;
		};

		struct StatusInSurface : public Status
		{
			struct SurfaceLayerCheck
			{
				uint8_t EvaluateSurfaceApplyTypes;
				float TurnTimerCheck;
				float SurfaceOnMoveDistanceMultiplier;
				float SurfaceDistanceCheck;
				int OnMoveCount;
				bool FullyEntered;
				bool Inside;
				bool Unknown;
			};

			SurfaceLayerCheck LayerChecks[2];
			glm::vec3 Translate;
			HashMap<FixedString, bool> Unknown;
		};

		struct StatusInvisible : public StatusBoost
		{
		    glm::vec3 InvisiblePosition;
		};

		struct StatusKnockedDown : public Status
		{
			uint8_t KnockedDownState;
			bool IsInstant;
		};

		struct StatusMaterial : public Status
		{
			FixedString MaterialUUID;
			uint8_t ApplyFlags;
			bool IsOverlayMaterial;
			bool Fading;
			bool ApplyNormalMap;
			bool Force;
		};

		struct StatusPolymorphed : public StatusBoost
		{
			Guid Id;
		};

		struct StatusReaction : public Status
		{
			ecs::EntityRef Source;
			ecs::EntityRef Target;
			glm::vec3 TargetPosition;
			ComponentHandle PartnerStatus;
			SpellId Spell;
			bool IgnoreHasSpell;
			bool IgnoreChecks;
			bool FromInterrupt;
			bool ShowOverhead;
			bool CheckProjectileTargets;
		};

		struct StatusStoryFrozen : public Status
		{
		};

		struct StatusRotate : public Status
		{
			float Yaw;
			float RotationSpeed;
		};

		struct StatusTeleportFalling : public Status
		{
			glm::vec3 Target;
			float ReappearTime;
			SpellId Spell;
			bool HasDamage;
			bool HasDamageBeenApplied;
		};

		struct StatusUnlock : public Status
		{
			EntityHandle Source;
			FixedString field_128;
			bool Success;
			int Unlocked;
		};

		struct StatusFear : public StatusBoost
		{
		};

		struct StatusSmelly : public Status
		{
		};

		struct StatusSneaking : public StatusBoost
		{
		    bool ClientRequestStop;
		};

		struct StatusEffect : public Status
		{
		};

		struct StatusDeactivated : public StatusBoost
		{
		};


		struct StatusMachine
		{
			using CreateStatusProc = Status* (StatusMachine* self, FixedString* statusId, uint64_t statusHandle, bool unkn1, bool unkn2);
			using ApplyStatusProc = Status* (StatusMachine* self, Status* status);

			Status* GetStatus(FixedString const& statusId) const;

			EntityHandle Owner;
			uint8_t field_18;
			Array<Status*> NewlyAddedStatuses;
			Array<Status*> Statuses;
			Array<Status*> CleanseStatuses;
			Array<Status*> UnknownStatuses;
			__int64 field_58;
			__int64 field_60;
			__int64 field_68;
			__int64 field_70;
			__int64 field_78;
			__int64 field_80;
			__int64 field_88;
			__int64 field_90;
			__int64 field_98;
			__int64 field_A0;
			Array<void*> RemoveStatusRequests;
		};
	}
}
