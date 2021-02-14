#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/Stats/Common.h>

namespace bg3se
{
	struct StatsFunctorBase
	{
		struct FunctorVMT
		{
			void* Destroy;
			void* ParseParams;
			void* GetDescription;
			void* GetDescription2;
			void* GetTooltipText;
			void* Clone;
		};

		FunctorVMT* VMT{ nullptr };
		FixedString UniqueName;
		StatsFunctorActionId TypeId{ StatsFunctorActionId::CustomDescription };
		StatsPropertyContext PropertyContext{ 0 };
		int32_t StatsConditionsId{ -1 };
		bool IsSelf{ false };
		int StoryActionId{ 0 };

		template <class T>
		std::optional<T*> Cast()
		{
			if (TypeId == typename T::FunctorId) {
				return static_cast<T*>(this);
			} else {
				return {};
			}
		}
	};


	struct StatsFunctorSet : public ProtectedGameObject<StatsFunctorSet>
	{
		struct BaseVMT
		{
			void* Destroy;
			void* ClearNextIndex;
			void* Clear;
			void* AddOrUpdate;
			StatsFunctorBase* (*GetByIndex)(StatsFunctorSet*, int64_t);
			StatsFunctorBase* (*GetByName)(StatsFunctorSet*, FixedString const&);
			void* field_30;
			void* (__fastcall* GetSize)(StatsFunctorSet*);
			StatsFunctorBase* (*GetByIndex2)(StatsFunctorSet*, int64_t);
			StatsFunctorBase* (*GetByIndex3)(StatsFunctorSet*, int64_t);
			void* UpdateNameMap;
		};

		virtual ~StatsFunctorSet() = 0;
		virtual void ClearNextIndex() = 0;
		virtual void Clear() = 0;
		virtual void AddOrUpdate(StatsFunctorBase*) = 0;
		virtual StatsFunctorBase* GetByIndex(int64_t) = 0;
		virtual StatsFunctorBase* GetByName(FixedString const&) = 0;
		virtual void Unknown_30() = 0;
		virtual int64_t GetSize() = 0;
		virtual StatsFunctorBase* GetByIndex2(int64_t) = 0;
		virtual StatsFunctorBase* GetByIndex3(int64_t) = 0;
		virtual void UpdateNameMap() = 0;

		ObjectSet<StatsFunctorBase*> FunctorList;
		Map<FixedString, StatsFunctorBase*> FunctorsByName_M;
		int NextFunctorIndex;
		FixedString UniqueName;
		FixedString UniqueName2;
	};


	struct StatsFunctorSetImpl : public StatsFunctorSet
	{
		~StatsFunctorSetImpl() override;
		void ClearNextIndex() override;
		void Clear() override;
		void AddOrUpdate(StatsFunctorBase*) override;
		StatsFunctorBase* GetByIndex(int64_t) override;
		StatsFunctorBase* GetByName(FixedString const&) override;
		void Unknown_30() override;
		int64_t GetSize() override;
		StatsFunctorBase* GetByIndex2(int64_t) override;
		StatsFunctorBase* GetByIndex3(int64_t) override;
		void UpdateNameMap() override;
	};


	struct LuaExpression : public ProtectedGameObject<LuaExpression>
	{
	    Array<STDString> ExpressionParams_M;
	    STDString Code;
	    int FastLock;
	};

	struct LuaExpressionManager : public ProtectedGameObject<LuaExpressionManager>
	{
	    void* VMT;
	    RefMap<UUID, LuaExpression*> Expressions;
	    char field_18;
	};


	struct CustomDescriptionFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::CustomDescription;

		FixedString Description; // Arg0
	};

	struct ResurrectFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::Resurrect;

		float Probability{ 1.0f }; // Arg0
		float HealthPercentage{ 1.0f }; // Arg1
	};

	struct SabotageFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::Sabotage;

		int field_20{ 1 };
	};

	struct SummonFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::Summon;

		FixedString MovingObject; // Arg1
		FixedString field_24;
		float SpawnLifetime{ 6.0f };
		ObjectSet<FixedString> StatusesToApply;
		FixedString field_48;
	};

	struct ForceFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::Force;

		float Arg0{ 8.0f }; // Arg0
		ForceFunctorOrigin Origin{ ForceFunctorOrigin::OriginToEntity }; // Arg1
		ForceFunctorAggression Aggression{ ForceFunctorAggression::Aggressive }; // Arg2
	};

	struct DouseFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::Douse;

		float field_20{ -1.0f };
		float field_24{ 1.0f };
	};

	struct SwapPlacesFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::SwapPlaces;

		FixedString Arg0;
	};

	struct EqualizeFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::Equalize;

		StatusHealType HealType{ StatusHealType::None }; // Arg0
	};

	struct PickupFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::Pickup;

		FixedString Arg0;
	};

	struct CreateSurfaceFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::CreateSurface;

		FixedString SurfaceType; // Arg2 - SurfaceType or DamageType
		float Radius{ -1.0f }; // Arg0
		float Arg4{ 0.0f };
		float Duration{ -1.0f }; //Arg1
		bool IsControlledByConcentration{ true }; // Arg3
	};

	struct CreateConeSurfaceFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::CreateConeSurface;

		FixedString Arg2;
		float Arg0{ -1.0f };
		float Arg1{ -1.0f };
		float Arg3{ 1.0f };
	};

	struct RemoveStatusFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::RemoveStatus;

		FixedString StatusId; // Arg0
	};

	struct ExecuteWeaponFunctorsFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::ExecuteWeaponFunctors;

		ExecuteWeaponFunctorsType Type;
	};

	struct TeleportSourceFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::TeleportSource;

	};

	struct SetStatusDurationFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::SetStatusDuration;

		FixedString StatusId; // Arg0
		float Duration; // Arg1
		bool SetIfLonger; // Arg2
	};

	struct UseAttackFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::UseAttack;

		bool IgnoreChecks; // Arg0
	};

	struct BreakConcentrationFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::BreakConcentration;

	};

	struct RestoreResourceFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::RestoreResource;

		UUID ActionResourceUUID; // Arg0
		int Hex; // Arg2
		int field_34;
		LuaExpression* LuaAmount; // Arg1
		double Amount; // Arg1
		bool IsPercentage; // Arg1
	};

	struct SpawnFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::Spawn;

		FixedString TemplateId; // Arg0
		FixedString Arg1;
		ObjectSet<FixedString> StatusesToApply;
	};

	struct StabilizeFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::Stabilize;

	};

	struct UnlockFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::Unlock;

	};

	struct ResetCombatTurnFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::ResetCombatTurn;

	};

	struct RemoveAuraByChildStatusFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::RemoveAuraByChildStatus;

		FixedString StatusId; // Arg0
	};

	struct ApplyStatusFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::ApplyStatus;

		FixedString StatusId; // Arg0
		FixedString StringParam; // Arg3
		STDString StatsConditions; // Arg6
		int StatsConditionsId{ -1 }; // Arg6
		float Duration{ 6.0 }; // Arg2
		int Param1{ -1 }; // Arg4
		int Param2{ -1 }; // Arg5
	};

	struct DealDamageFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::DealDamage;

		DamageType DamageType; // Arg1
		DealDamageWeaponType WeaponType; // Arg0
		DealDamageWeaponDamageType WeaponDamageType; // Arg1
		LuaExpression* Damage;
		bool Nonlethal; // Arg3
		bool Magical; // Arg2
	};

	struct UseActionResourceFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::UseActionResource;

	    UUID ActionResourceUUID; // Arg0
	    double Amount; // Arg1
	    int ResourceIndex; // Arg2
	    bool IsPercentage; // Arg1
	};

	struct CreateExplosionFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::CreateExplosion;

	    FixedString SpellId; // Arg0
	};

	struct SurfaceChangeFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::SurfaceChange;

		float Chance{ 1.0f };
		float field_24{ 0.0f };
		float field_28{ 0.0f };	
		float field_2C{ -1.0f };
		SurfaceChange SurfaceChange;
	};

	struct ApplyEquipmentStatusFunctor : public ApplyStatusFunctor
	{
		static constexpr auto FunctorId = StatsFunctorActionId::ApplyEquipmentStatus;

		ItemSlot32 EquipmentSlot;
	};

	struct RegainHitPointsFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::RegainHitPoints;

		LuaExpression* HitPoints;
	};

	struct UseSpellFunctor : public StatsFunctorBase
	{
		uint64_t field_18;
		FixedString SpellId; // Arg0
		bool IgnoreHasSpell; // Arg1
		bool IgnoreChecks; // Arg2
		bool Arg3;
	};

	struct ExtenderFunctor : public StatsFunctorBase
	{
	};

}
