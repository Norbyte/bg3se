#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Misc.h>
#include <GameDefinitions/Stats/Common.h>
#include <GameDefinitions/Hit.h>

namespace bg3se
{
	namespace esv
	{
		struct Status : public ProtectedProxyGameObject<Status>
		{
			using EnterProc = bool(Status* self);

			struct VMT
			{
				void* Destroy;
				void* SetObjectHandle1;
				void* SetOldIDs;
				void* SetMyHandle;
				void* GetMyHandle;
				void* GetStatusId;
				void* GetStatusType;
				void* SetStringParam;
				void* SetFloatParams;
				void* VMT48;
				void* VMT50;
				void* CanEnter;
				void* Init;
				Status::EnterProc* Enter;
				void* Resume;
				void* Update;
				void* Tick;
				void* Exit;
				void* VMT90;
				void* LoseControl;
				void* VMTA0;
				void* VMTA8;
				void* GetOwnerHandle;
				void* OnEndTurn;
				void* GetSyncData;
				void* VMTC8;
				void* Visit;
				void* VMTD8;
				void* ReevaluateHandles_M;
				void* VMTE8;
				void* VMTF0;
			};

			virtual ~Status() = 0;
			virtual void SetObjectHandle1(ObjectHandle Handle) = 0;
			virtual void CopySomeFields() = 0;
			virtual void SetMyHandle(ObjectHandle Handle) = 0;
			virtual void GetMyHandle(ObjectHandle* Handle) = 0;
			virtual StatusType GetStatusId() = 0;
			// 0 - Stackable
			// 1 - Apply only the first instance, discard new ones
			// 2 - Apply only one instance, new instances replace old ones
			// 3 - Apply only the first instance; triggers combat?
			virtual uint32_t GetStatusType() = 0;

			virtual uint32_t SetStringParam() = 0;
			virtual uint32_t SetFloatParams() = 0;
			virtual uint32_t VMT48() = 0;
			virtual uint32_t VMT50() = 0;

			virtual bool CanEnter() = 0;
			virtual void Init() = 0;
			virtual bool Enter() = 0;
			virtual bool Resume() = 0;
			virtual void Update(GameTime* time) = 0;
			virtual void Tick(UUID* combatUuid, float deltaTime) = 0;
			virtual void Exit() = 0;
			virtual void VMT90() = 0;
			virtual bool LoseControl() = 0;
			virtual void VMTA0() = 0;
			virtual void VMTA8() = 0;
			virtual ObjectHandle* GetOwnerHandle() = 0;
			virtual void VMTB0() = 0;
			virtual void GetSyncData(void* msgWriter) = 0;
			virtual void VMTC8() = 0;
			virtual void Visit(ObjectVisitor* visitor) = 0;
			virtual void VMTD8() = 0;
			virtual void ReevaluateHandles_M() = 0;
			virtual void VMTE8() = 0;
			virtual void VMTF0() = 0;

			Array<ObjectHandle> field_8;
			ObjectHandle SomeHandle;
			ObjectHandle SomeEntityHandle_Old;
			int SomeState;
			FixedString StatusID_Old;
			ObjectHandle field_38;
			NetId NetID_Old;
			FixedString field_48;
			NetId NetID;
			int StoryActionID;
			ActionOriginator Originator;
			double field_78;
			FixedString StatusId;
			float StartTimer;
			float LifeTime;
			float CurrentLifeTime;
			float TurnTimer;
			float Strength;
			uint8_t CauseType; // TODO - CauseType enum?
			ObjectHandle StatusHandle;
			ObjectHandle UnknownHandle;
			EntityHandle OwnerHandle;
			ObjectSet<EntityHandle> StatusOwner;
			EntityWorldHandle StatusSource;
			UUID StatusSourceUUID;
			EntityHandle StatusSourceEntityHandle;
			EntityWorldHandle StatusSource_M;
			EntityHandle CleansedByHandle_M;
			UUID field_110;
			int Conditions;
			uint16_t RemoveEvents;
			uint8_t Flags2; // TODO - typing flags
			uint8_t Flags0; // TODO - typing flags
			int field_118;
			uint8_t field_11C;
			uint8_t TickType;
			uint8_t Flags3; // TODO - typing flags
			uint8_t SomeFlags;
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
			ObjectSet<ObjectHandle> ItemHandles;
			float EffectTime;
			FixedString StackId;
			glm::vec3 SourceDirection;
			ObjectSet<uint32_t> SurfaceChanges;
		};

		struct StatusActiveDefense : public StatusBoost
		{
			int Charges;
			glm::vec3 TargetPos;
			ObjectHandle Target;
			float Radius;
			FixedString Projectile;
			int TargetConditions_M;
			ObjectSet<glm::vec3> UnknVectors;
			ObjectSet<ObjectHandle> PreviousTargets;
		};

		struct StatusClimbing : public Status
		{
			__int64 field_120;
			int field_128;
			int field_12C;
			glm::vec3 MoveDirection_M;
			int field_13C;
			__int64 field_140;
			ObjectHandle Item;
			FixedString Level;
			uint8_t Status;
			bool Direction;
			bool JumpUpLadders_M;
			bool Incapacitated_M;
			bool Started_M;
		};

		struct StatusDestroying : public Status
		{
			Hit HitDescription;
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
			EntityWorldHandle Source;
			__int64 field_128;
			ObjectHandle field_130;
			Hit HitDescription;
			UUID Combat;
			uint8_t DyingFlags;
		};

		struct StatusEncumbered : public Status
		{
			int field_1A8;
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

		struct StatusHealSharing : public StatusBoost
		{
			ObjectHandle Caster;
		};

		struct StatusHealSharingCaster : public StatusBoost
		{
			Array<ObjectHandle> Targets;
			VirtualMultiHashMap<ObjectHandle, ObjectHandle> field_1C0;
		};
		
		struct StatusHealing : public StatusBoost
		{
			int HealAmount;
			float TimeElapsed;
			int HealEffect;
			FixedString HealEffectId;
			bool SkipInitialEffect;
			uint16_t HealingEvent; // TODO enum
			uint8_t HealType; // TODO enum
			int AbsorbSurfaceRange;
		};

		struct StatusIdentify : public Status
		{
			int Level;
			int Identified;
			ObjectHandle Identifier;
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
			VirtualMultiHashMap<FixedString, bool> Unknown;
		};

		struct StatusInfectiousDiseased : public StatusBoost
		{
			int InfectiousDiseaseDepth;
			float InfectTimer;
			float InfectiousDiseaseRadius;
			ObjectHandle Target;
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

		struct StatusSummoning : public Status
		{
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
			UUID Id;
		};

		struct StatusReaction : public Status
		{
			EntityWorldHandle Source;
			EntityWorldHandle Target;
			glm::vec3 TargetPosition;
			ObjectHandle Partner;
			bool ShowOverhead;
			SpellId Spell;
			bool IgnoreHasSpell;
			bool IgnoreChecks;
		};

		struct StatusStoryFrozen : public Status
		{
		};

		struct StatusRepair : public Status
		{
			int Level;
			int Repaired;
			ObjectHandle Repairer;
		};

		struct StatusRotate : public Status
		{
			float Yaw;
			float RotationSpeed;
		};

		struct StatusSitting : public Status
		{
			EntityHandle Item;
			glm::vec3 Position;
			int Index;
			float field_138;
			int TimeElapsed;
			int Heal;
		};

		struct StatusLying : public StatusSitting
		{
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
			ObjectHandle Source;
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

		struct StatusClean : public Status
		{
		};

		struct StatusSneaking : public StatusBoost
		{
		    bool ClientRequestStop;
		};

		struct StatusWindWalker : public StatusBoost
		{
		};

		struct StatusDecayingTouch : public StatusBoost
		{
		};

		struct StatusUnhealable : public Status
		{
		};

		struct StatusForceMove : public Status
		{
		};

		struct StatusConstrained : public StatusSitting
		{
		};

		struct StatusEffect : public Status
		{
		};

		struct StatusDeactivated : public StatusBoost
		{
		};


		struct StatusMachine
		{
			using CreateStatusProc = Status* (StatusMachine* self, FixedString* statusId, uint64_t statusHandle);
			using ApplyStatusProc = Status* (StatusMachine* self, Status* status);

			Status* GetStatus(ObjectHandle handle) const;
			Status* GetStatus(NetId netId) const;
			Status* GetStatus(FixedString const& statusId) const;

			void* VMT;
			ObjectHandle OwnerHandle;
			ObjectHandle OwnerEntityHandle;
			uint8_t field_18;
			ObjectSet<Status*> NewlyAddedStatuses;
			ObjectSet<Status*> StackedStatuses;
			ObjectSet<Status*> CleanseStatuses;
			ObjectSet<Status*> UnknownStatuses;
			__int64 field_80;
			__int64 field_88;
			void* SomeProc1;
			void* SomeProc2;
			void* SomeProc3;
			__int64 field_A8;
			__int64 field_B0;
			__int64 field_B8;
			__int64 field_C0;
			void* StatusFactory;
		};
	}
}
