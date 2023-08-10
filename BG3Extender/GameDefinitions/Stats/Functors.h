#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/Stats/Common.h>
#include <GameDefinitions/Hit.h>
#include <GameDefinitions/Misc.h>
#include <Lua/LuaHelpers.h>

namespace bg3se
{
	struct GuidResourceDefinitionManagerBase;

	struct StatsFunctorBase : HasObjectProxy
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
		bool IsSwap{ false };
		int32_t StoryActionId{ 0 };

		template <class T>
		std::optional<T*> Cast()
		{
			if (TypeId == T::FunctorId) {
				return static_cast<T*>(this);
			} else {
				return {};
			}
		}
	};

	struct BaseFunctorExecParams : public HasObjectProxy
	{
		FunctorExecParamsType ParamsTypeId{ 0 };
		StatsPropertyContext PropertyContext{ 0 };
		int32_t StoryActionId{ 0 };
		ActionOriginator Originator;
		GuidResourceDefinitionManagerBase* GuidResourceMgr{ nullptr };
		uint64_t Unkn{ 0 };
	};

	struct FunctorExecParamsType1 : public BaseFunctorExecParams
	{
		EntityWorldHandle Caster;
		EntityWorldHandle Target;
		glm::vec3 Position;
		bool IsFromItem;
		SpellIdWithPrototype SpellId;
		Hit Hit;
		DamageSums DamageSums;
		float SomeRadius;
		HitWith HitWith;
		int16_t Hit2Flags;
		FixedString field_26C;
		char field_270;
	};

	struct FunctorExecParamsType2 : public BaseFunctorExecParams
	{
		EntityWorldHandle Caster;
		glm::vec3 Position;
		float ExplodeRadius;
		bool IsFromItem;
		SpellIdWithPrototype SpellId;
		Hit Hit;
		DamageSums DamageSums;
		float SomeRadius;
	};

	struct FunctorExecParamsType3 : public BaseFunctorExecParams
	{
		EntityWorldHandle Caster;
		EntityWorldHandle Target;
		glm::vec3 Position;
	};

	struct FunctorExecParamsType4 : public BaseFunctorExecParams
	{
		EntityWorldHandle Caster;
		glm::vec3 Position;
		SpellIdWithPrototype SpellId;
		Hit Hit;
		DamageSums DamageSums;
		uint16_t Hit2_field_90;
	};

	struct FunctorExecParamsType5 : public BaseFunctorExecParams
	{
		EntityWorldHandle Owner_M;
		EntityWorldHandle Target;
		EntityWorldHandle Caster;
		glm::vec3 Position;
		bool IsFromItem;
		SpellIdWithPrototype SpellId;
		Hit Hit;
		DamageSums DamageSums;
		float SomeRadius;
		uint8_t field_268;
	};

	struct FunctorExecParamsType6 : public BaseFunctorExecParams
	{
		EntityWorldHandle Target;
		EntityWorldHandle TargetX;
		EntityWorldHandle Caster;
		glm::vec3 Position;
		bool IsFromItem;
		SpellIdWithPrototype SpellId;
		Hit Hit;
		DamageSums DamageSums;
		float SomeRadius;
		uint8_t field_268;
		int field_25C;
	};

	struct FunctorExecParamsType7 : public BaseFunctorExecParams
	{
		EntityWorldHandle Caster;
		EntityWorldHandle Caster2;
		EntityWorldHandle Target;
		glm::vec3 Position;
		bool IsFromItem;
		SpellIdWithPrototype SpellId;
		Hit Hit;
		DamageSums DamageSums;
		float SomeRadius;
		bool SummonInInventoryFlag;
	};

	struct FunctorExecParamsType8 : public BaseFunctorExecParams
	{
		EntityWorldHandle Target;
	};


	struct StatsFunctorSet : public Noncopyable<StatsFunctorSet>
	{
		using ExecuteType1Proc = void (NewHit* hit, StatsFunctorSet * self, FunctorExecParamsType1 * params);
		using ExecuteType2Proc = void (NewHit* hit, StatsFunctorSet * self, FunctorExecParamsType2 * params);
		using ExecuteType3Proc = void (NewHit* hit, StatsFunctorSet * self, FunctorExecParamsType3 * params);
		using ExecuteType4Proc = void (NewHit* hit, StatsFunctorSet * self, FunctorExecParamsType4 * params);
		using ExecuteType5Proc = void (NewHit* hit, StatsFunctorSet * self, FunctorExecParamsType5 * params);
		using ExecuteType6Proc = void (NewHit* hit, StatsFunctorSet * self, FunctorExecParamsType6 * params);
		using ExecuteType7Proc = void (NewHit* hit, StatsFunctorSet * self, FunctorExecParamsType7 * params);
		using ExecuteType8Proc = void (NewHit* hit, StatsFunctorSet * self, FunctorExecParamsType8 * params);

		struct BaseVMT
		{
			void (*Destroy)(StatsFunctorSet*);
			void (*ClearNextIndex)(StatsFunctorSet*);
			void (*Clear)(StatsFunctorSet*);
			void (*AddOrUpdate)(StatsFunctorSet*, StatsFunctorBase*);
			StatsFunctorBase* (*GetByIndex)(StatsFunctorSet*, int64_t);
			StatsFunctorBase* (*GetByName)(StatsFunctorSet*, FixedString const&);
			void (*Unknown_30)(StatsFunctorSet*);
			int64_t (*GetSize)(StatsFunctorSet*);
			StatsFunctorBase* (*GetByIndex2)(StatsFunctorSet*, int64_t);
			StatsFunctorBase* (*GetByIndex3)(StatsFunctorSet*, int64_t);
			void (*UpdateNameMap)(StatsFunctorSet*);
		};

		/*virtual ~StatsFunctorSet() = 0;
		virtual void ClearNextIndex() = 0;
		virtual void Clear() = 0;
		virtual void AddOrUpdate(StatsFunctorBase*) = 0;
		virtual StatsFunctorBase* GetByIndex(int64_t) = 0;
		virtual StatsFunctorBase* GetByName(FixedString const&) = 0;
		virtual void Unknown_30() = 0;
		virtual int64_t GetSize() = 0;
		virtual StatsFunctorBase* GetByIndex2(int64_t) = 0;
		virtual StatsFunctorBase* GetByIndex3(int64_t) = 0;
		virtual void UpdateNameMap() = 0;*/

		BaseVMT* VMT{ nullptr };
		ObjectSet<StatsFunctorBase*> FunctorList;
		Map<FixedString, StatsFunctorBase*> FunctorsByName;
		int NextFunctorIndex{ 0 };
		int Unknown{ 0 };
		FixedString UniqueName;
	};


	// FIXME - use custom implementation of base VMT or use base VMT?
	struct StatsFunctorSetImpl : public StatsFunctorSet
	{
		/*~StatsFunctorSetImpl() override;
		void ClearNextIndex() override;
		void Clear() override;
		void AddOrUpdate(StatsFunctorBase*) override;
		StatsFunctorBase* GetByIndex(int64_t) override;
		StatsFunctorBase* GetByName(FixedString const&) override;
		void Unknown_30() override;
		int64_t GetSize() override;
		StatsFunctorBase* GetByIndex2(int64_t) override;
		StatsFunctorBase* GetByIndex3(int64_t) override;
		void UpdateNameMap() override;*/
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

		int Amount{ 1 };
	};

	struct SummonFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::Summon;

		FixedString MovingObject; // Arg1
		FixedString Arg2;
		float SpawnLifetime{ 6.0f };
		ObjectSet<FixedString> StatusesToApply;
		FixedString Arg3;
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

		ExecuteWeaponFunctorsType WeaponType;
	};

	struct TeleportSourceFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::TeleportSource;

	};

	struct SetStatusDurationFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::SetStatusDuration;

		FixedString StatusId; // Arg0
		float Duration{ 6.0f }; // Arg1
		bool SetIfLonger{ false }; // Arg2
	};

	struct UseAttackFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::UseAttack;

		bool IgnoreChecks{ false }; // Arg0
	};

	struct BreakConcentrationFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::BreakConcentration;

	};

	struct RestoreResourceFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::RestoreResource;

		Guid ActionResourceUUID; // Arg0
		int Hex{ 0 }; // Arg2
		int field_34{ 0 };
		LuaExpression* LuaAmount{ nullptr }; // Arg1
		double Amount{ 0.0 }; // Arg1
		bool IsPercentage{ false }; // Arg1
	};

	struct SpawnFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::Spawn;

		FixedString TemplateId; // Arg0
		FixedString Arg1;
		VirtualMultiHashSet<FixedString> StatusesToApply;
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
		int Param1{ -1 }; // Arg5
		int Param2{ -1 }; // ???
		void* Arg2_DurationLuaExpression{ nullptr }; // Arg2
		bool HasParam6{ false };
	};

	struct DealDamageFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::DealDamage;

		using ApplyDamageProc = NewHit * (NewHit* result, DealDamageFunctor* functor, EntityWorldHandle* casterHandle, 
			EntityWorldHandle* targetHandle, glm::vec3* position, bool isFromItem, SpellIdWithPrototype* spellId, 
			int storyActionId, ActionOriginator* originator, GuidResourceDefinitionManagerBase* classResourceMgr, 
			Hit* hit, DamageSums* damageSums, uint64_t* unknownThothParam, HitWith hitWith);

		DamageType DamageType{ DamageType::None }; // Arg1
		DealDamageWeaponType WeaponType{ DealDamageWeaponType::None }; // Arg0
		DealDamageWeaponDamageType WeaponDamageType{ DealDamageWeaponDamageType::None }; // Arg1
		LuaExpression* Damage{ nullptr };
		bool Nonlethal{ false }; // Arg3
		bool Magical{ false }; // Arg2
		int32_t field_34{ 0 }; // Arg5
	};

	struct UseActionResourceFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::UseActionResource;

	    Guid ActionResourceUUID; // Arg0
		double Amount{ 0.0 }; // Arg1
		int ResourceIndex{ 0 }; // Arg2
		bool IsPercentage{ false }; // Arg1
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
		SurfaceChange SurfaceChange{ SurfaceChange::None };
	};

	struct ApplyEquipmentStatusFunctor : public ApplyStatusFunctor
	{
		static constexpr auto FunctorId = StatsFunctorActionId::ApplyEquipmentStatus;

		ItemSlot32 EquipmentSlot{ ItemSlot32::MainWeapon };
	};

	struct RegainHitPointsFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::RegainHitPoints;

		LuaExpression* HitPoints{ nullptr };
	};

	struct UseSpellFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::UseSpell;

		FixedString SpellId; // Arg0
		bool IgnoreHasSpell{ false }; // Arg1
		bool IgnoreChecks{ false }; // Arg2
		bool Arg3{ false };
	};

	struct ExtenderFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::Extender;
	};

	struct SummonInInventoryFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::SummonInInventory;

		FixedString Arg1;
		FixedString Arg7;
		float Arg2{ 0.0f }; // Duration?
		VirtualMultiHashSet<FixedString> AdditionalArgs;
		FixedString Arg8;
		float Arg3{ 0.0f };
		bool Arg4{ false };
		bool Arg5{ false };
		bool Arg6{ false };
	};

	struct SpawnInInventoryFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::SpawnInInventory;

		FixedString Arg1;
		FixedString Arg6;
		float Arg2{ 0.0f };
		bool Arg3{ false };
		bool Arg4{ false };
		bool Arg5{ false };
		VirtualMultiHashSet<FixedString> AdditionalArgs;
	};

	struct RemoveUniqueStatusFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::RemoveUniqueStatus;

		FixedString StatusId; // Arg0
	};

	struct DisarmWeaponFunctor : public StatsFunctorBase
	{
		static constexpr auto FunctorId = StatsFunctorActionId::DisarmWeapon;
	};

}

BEGIN_NS(lua)

LUA_POLYMORPHIC(BaseFunctorExecParams)

END_NS()

