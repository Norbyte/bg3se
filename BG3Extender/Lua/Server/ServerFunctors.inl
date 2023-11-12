#include <GameDefinitions/Stats/Functors.h>
#include <Lua/LuaBinding.h>

namespace bg3se::esv::lua
{
	using namespace bg3se::lua;

	int CreateFunctor(lua_State* L)
	{
		StackCheck _(L, 1);

		auto type = get<bg3se::stats::FunctorId>(L, 1);
		auto& pool = GetServerLifetimePool();

		auto functor = GetStaticSymbols().GetStats()->ConstructFunctor(type);
		if (!functor) {
			push(L, nullptr);
			return 1;
		}

#define V(type) case bg3se::stats::FunctorId::type: LegacyObjectProxy::MakeOwner<bg3se::stats::type##Functor>(L, pool, static_cast<bg3se::stats::type##Functor*>(functor)); break;

		switch (type) {
			V(CustomDescription)
			V(ApplyStatus)
			V(SurfaceChange)
			V(Resurrect)
			V(Sabotage)
			V(Summon)
			V(Force)
			V(Douse)
			V(SwapPlaces)
			V(Equalize)
			V(Pickup)
			V(CreateSurface)
			V(CreateConeSurface)
			V(RemoveStatus)
			V(DealDamage)
			V(ExecuteWeaponFunctors)
			V(RegainHitPoints)
			V(TeleportSource)
			V(SetStatusDuration)
			V(UseSpell)
			V(UseActionResource)
			V(UseAttack)
			V(CreateExplosion)
			V(BreakConcentration)
			V(ApplyEquipmentStatus)
			V(RestoreResource)
			V(Spawn)
			V(Stabilize)
			V(Unlock)
			V(ResetCombatTurn)
			V(RemoveAuraByChildStatus)
			V(SummonInInventory)
			V(SpawnInInventory)
			V(RemoveUniqueStatus)
			V(DisarmWeapon)
			V(Extender)

		default:
			LuaError("Functor type not supported: " << type);
			push(L, nullptr);
		}

#undef V

		return 1;
	}

	int CreateFunctorExecParams(lua_State* L)
	{
		StackCheck _(L, 1);

		auto type = get<FunctorExecParamsType>(L, 1);
		auto& pool = GetServerLifetimePool();

#define V(ty) case FunctorExecParamsType::ty: { \
			auto params = LegacyObjectProxy::MakeOwner<bg3se::stats::FunctorExecParams##ty>(L, pool); \
			params->Get()->ParamsTypeId = type; \
			break; }

		switch (type) {
			V(Type1)
			V(Type2)
			V(Type3)
			V(Type4)
			V(Type5)
			V(Type6)
			V(Type7)
			V(Type8)

		default:
			LuaError("Functor params type not supported: " << type);
			push(L, nullptr);
		}

#undef V

		return 1;
	}

	int ExecuteFunctor(lua_State* L)
	{
		StackCheck _(L, 1);
		auto functor = checked_get_proxy<bg3se::stats::Functor>(L, 1);
		auto params = checked_get_proxy<bg3se::stats::BaseFunctorExecParams>(L, 2);
		auto& pool = GetServerLifetimePool();

		auto hit = LegacyObjectProxy::MakeOwner<HitResult>(L, pool)->Get();
		auto functorSet = GetStaticSymbols().GetStats()->ConstructFunctorSet(GFS.strEmpty);
		functorSet->VMT->AddOrUpdate(functorSet, functor);

#define V(type) case FunctorExecParamsType::type: \
			GetStaticSymbols().stats__Functors__Execute##type(hit, \
				functorSet, static_cast<bg3se::stats::FunctorExecParams##type*>(params)); break;

		switch (params->ParamsTypeId) {
			V(Type1)
			V(Type2)
			V(Type3)
			V(Type4)
			V(Type5)
			V(Type6)
			V(Type7)
			V(Type8)

		default:
			LuaError("Functor params type not supported: " << params->ParamsTypeId);
		}

#undef V

		return 1;
	}

	void RegisterFunctorsLibrary(lua_State* L)
	{
		static const luaL_Reg functorLib[] = {
			{"Create", CreateFunctor},
			{"CreateParams", CreateFunctorExecParams},
			{"Execute", ExecuteFunctor},
			{0,0}
		};

		RegisterLib(L, "Functor", functorLib);
	}
}
