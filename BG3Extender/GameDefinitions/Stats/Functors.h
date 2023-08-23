#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/Stats/Common.h>
#include <GameDefinitions/Hit.h>
#include <GameDefinitions/Misc.h>
#include <Lua/LuaHelpers.h>

BEGIN_NS(stats)

struct Functor : HasObjectProxy
{
	struct FunctorVMT
	{
		void* Destroy;
		void* ParseParams;
		void* Clone;
	};

	struct RollCondition
	{
		RollType Type;
		int32_t ConditionId{ -1 };
	};

	FunctorVMT* VMT{ nullptr };
	FixedString UniqueName;
	uint64_t Unknown1{ 0 };
	uint64_t Unknown2{ 0 };
	Array<RollCondition> RollConditions;
	int32_t StatsConditionsId{ -1 };
	PropertyContext PropertyContext{ 0 };
	uint32_t field_40{ 0 };
	ObserverType ObserverType{ ObserverType::None };
	FunctorId TypeId{ FunctorId::CustomDescription };
	FunctorFlags Flags{ 0 };

	template <class T>
	std::optional<T*> Cast()
	{
		if (TypeId == T::FunctorType) {
			return static_cast<T*>(this);
		} else {
			return {};
		}
	}
};

struct BaseFunctorExecParams : public HasObjectProxy
{
	FunctorExecParamsType ParamsTypeId{ 0 };
	PropertyContext PropertyContext{ 0 };
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


struct Functors : public Noncopyable<Functors>
{
	using ExecuteType1Proc = void (NewHit* hit, Functors* self, FunctorExecParamsType1 * params);
	using ExecuteType2Proc = void (NewHit* hit, Functors* self, FunctorExecParamsType2 * params);
	using ExecuteType3Proc = void (NewHit* hit, Functors* self, FunctorExecParamsType3 * params);
	using ExecuteType4Proc = void (NewHit* hit, Functors* self, FunctorExecParamsType4 * params);
	using ExecuteType5Proc = void (NewHit* hit, Functors* self, FunctorExecParamsType5 * params);
	using ExecuteType6Proc = void (NewHit* hit, Functors* self, FunctorExecParamsType6 * params);
	using ExecuteType7Proc = void (NewHit* hit, Functors* self, FunctorExecParamsType7 * params);
	using ExecuteType8Proc = void (NewHit* hit, Functors* self, FunctorExecParamsType8 * params);

	struct BaseVMT
	{
		void (*Destroy)(Functors*);
		void (*ClearNextIndex)(Functors*);
		void (*Clear)(Functors*);
		void (*AddOrUpdate)(Functors*, Functor*);
		Functor* (*GetByIndex)(Functors*, int64_t);
		Functor* (*GetByName)(Functors*, FixedString const&);
		void (*Unknown_30)(Functors*);
		int64_t (*GetSize)(Functors*);
		Functor* (*GetByIndex2)(Functors*, int64_t);
		Functor* (*GetByIndex3)(Functors*, int64_t);
		void (*UpdateNameMap)(Functors*);
	};

	/*virtual ~Functors() = 0;
	virtual void ClearNextIndex() = 0;
	virtual void Clear() = 0;
	virtual void AddOrUpdate(Functor*) = 0;
	virtual Functor* GetByIndex(int64_t) = 0;
	virtual Functor* GetByName(FixedString const&) = 0;
	virtual void Unknown_30() = 0;
	virtual int64_t GetSize() = 0;
	virtual Functor* GetByIndex2(int64_t) = 0;
	virtual Functor* GetByIndex3(int64_t) = 0;
	virtual void UpdateNameMap() = 0;*/

	BaseVMT* VMT{ nullptr };
	Array<Functor*> Functors;
	MultiHashMap<FixedString, Functor*> FunctorsByName;
	int NextFunctorIndex{ 0 };
	int Unknown{ 0 };
	FixedString UniqueName;
};


// FIXME - use custom implementation of base VMT or use base VMT?
struct StatsFunctorSetImpl : public Functors
{
	/*~StatsFunctorSetImpl() override;
	void ClearNextIndex() override;
	void Clear() override;
	void AddOrUpdate(Functor*) override;
	Functor* GetByIndex(int64_t) override;
	Functor* GetByName(FixedString const&) override;
	void Unknown_30() override;
	int64_t GetSize() override;
	Functor* GetByIndex2(int64_t) override;
	Functor* GetByIndex3(int64_t) override;
	void UpdateNameMap() override;*/
};


struct CustomDescriptionFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::CustomDescription;

	FixedString Description; // Arg0
};

struct ResurrectFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::Resurrect;

	float Probability{ 1.0f }; // Arg0
	float HealthPercentage{ 1.0f }; // Arg1
	ResurrectFlags Flags{ 0 };
};

struct SabotageFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::Sabotage;

	int Amount{ 1 };
};

struct SummonFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::Summon;

	FixedString MovingObject; // Arg1
	FixedString Arg2;
	float SpawnLifetime{ 6.0f };
	uint8_t LifetimeType{ 0 };
	MultiHashSet<FixedString> StatusesToApply;
	FixedString Arg4;
	bool Arg3;
	bool Arg9;
};

struct ForceFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::Force;

	FixedString Distance; // Arg0
	ForceFunctorOrigin Origin{ ForceFunctorOrigin::OriginToEntity }; // Arg1
	ForceFunctorAggression Aggression{ ForceFunctorAggression::Aggressive }; // Arg2
	LuaExpression* Unkn{ nullptr };
	bool Arg3{ false };
	bool Arg4{ false };
};

struct DouseFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::Douse;

	float field_20{ -1.0f };
	float field_24{ 1.0f };
};

struct SwapPlacesFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::SwapPlaces;

	FixedString Animation; // Arg0
	bool Arg1{ false };
	bool Arg2{ false };
};

struct EqualizeFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::Equalize;

	StatusHealType HealType{ StatusHealType::None }; // Arg0
};

struct PickupFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::Pickup;

	FixedString Arg0;
};

struct CreateSurfaceFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::CreateSurface;

	FixedString SurfaceType; // Arg2 - SurfaceType or DamageType
	float Radius{ -1.0f }; // Arg0
	float Arg4{ 0.0f };
	float Duration{ -1.0f }; //Arg1
	bool IsControlledByConcentration{ true }; // Arg3
	bool Arg5{ false };
};

struct CreateConeSurfaceFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::CreateConeSurface;

	FixedString Arg2;
	float Arg0{ -1.0f };
	float Arg1{ -1.0f };
	bool Arg3{ false };
	bool field_55{ false };
};

struct RemoveStatusFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::RemoveStatus;

	FixedString StatusId; // Arg0
};

struct ExecuteWeaponFunctorsFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::ExecuteWeaponFunctors;

	ExecuteWeaponFunctorsType WeaponType;
};

struct TeleportSourceFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::TeleportSource;

	bool Arg1{ false };
	bool Arg2{ false };
};

struct SetStatusDurationFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::SetStatusDuration;

	FixedString StatusId; // Arg0
	float Duration{ 6.0f }; // Arg1
	bool SetIfLonger{ false }; // Arg2
};

struct UseAttackFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::UseAttack;

	bool IgnoreChecks{ false }; // Arg0
};

struct BreakConcentrationFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::BreakConcentration;

};

struct RestoreResourceFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::RestoreResource;

	Guid ActionResourceUUID; // Arg0
	int Hex{ 0 }; // Arg2
	int field_34{ 0 };
	LuaExpression* LuaAmount{ nullptr }; // Arg1
	double Amount{ 0.0 }; // Arg1
	bool IsPercentage{ false }; // Arg1
};

struct SpawnFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::Spawn;

	FixedString TemplateId; // Arg0
	FixedString Arg1;
	VirtualMultiHashSet<FixedString> StatusesToApply;
	bool Arg6{ false };
};

struct StabilizeFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::Stabilize;

};

struct UnlockFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::Unlock;

};

struct ResetCombatTurnFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::ResetCombatTurn;

};

struct RemoveAuraByChildStatusFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::RemoveAuraByChildStatus;

	FixedString StatusId; // Arg0
};

struct ApplyStatusFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::ApplyStatus;

	FixedString StatusId; // Arg0
	FixedString StringParam; // Arg3
	STDString StatsConditions; // Arg6
	int StatsConditionsId{ -1 }; // Arg6
	int Param5{ -1 }; // Arg5
	int Param6{ -1 }; // ???
	bool Param8{ false };
	void* Arg2_DurationLuaExpression{ nullptr }; // Arg2
	bool HasParam6{ false };
};

struct DealDamageFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::DealDamage;

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

struct UseActionResourceFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::UseActionResource;

	Guid ActionResourceUUID; // Arg0
	double Amount{ 0.0 }; // Arg1
	int ResourceIndex{ 0 }; // Arg2
	bool IsPercentage{ false }; // Arg1
	bool Arg3{ false }; // Arg3
};

struct CreateExplosionFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::CreateExplosion;

	FixedString SpellId; // Arg0
};

struct SurfaceChangeFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::SurfaceChange;

	float Chance{ 1.0f };
	float field_24{ 0.0f };
	float field_28{ 0.0f };	
	float field_2C{ -1.0f };
	SurfaceChange SurfaceChange{ SurfaceChange::None };
};

struct ApplyEquipmentStatusFunctor : public ApplyStatusFunctor
{
	static constexpr auto FunctorType = FunctorId::ApplyEquipmentStatus;

	StatsItemSlot EquipmentSlot{ StatsItemSlot::MainWeapon };
};

struct RegainHitPointsFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::RegainHitPoints;

	LuaExpression* HitPoints{ nullptr };
	ResurrectFlags Flags{ 0 };
};

struct UseSpellFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::UseSpell;

	FixedString SpellId; // Arg0
	bool IgnoreHasSpell{ false }; // Arg1
	bool IgnoreChecks{ false }; // Arg2
	bool Arg3{ false };
	Guid Arg4;
};

struct ExtenderFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::Extender;
};

struct SummonInInventoryFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::SummonInInventory;

	FixedString Arg1;
	FixedString Arg8;
	float Duration{ 0.0f }; // Arg2
	uint8_t DurationType{ 0 };
	MultiHashSet<FixedString> AdditionalArgs;
	FixedString Arg8;
	float Arg3{ 0.0f };
	bool Arg4{ false };
	bool Arg5{ false };
	bool Arg6{ false };
	bool Arg7{ false };
};

struct SpawnInInventoryFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::SpawnInInventory;

	FixedString Arg1;
	FixedString Arg6;
	float Arg2{ 0.0f };
	bool Arg3{ false };
	bool Arg4{ false };
	bool Arg5{ false };
	VirtualMultiHashSet<FixedString> AdditionalArgs;
};

struct RemoveUniqueStatusFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::RemoveUniqueStatus;

	FixedString StatusId; // Arg0
};

struct DisarmWeaponFunctor : public Functor
{
	static constexpr auto FunctorType = FunctorId::DisarmWeapon;
};

END_NS()

BEGIN_NS(lua)

LUA_POLYMORPHIC(bg3se::stats::BaseFunctorExecParams)

END_NS()
