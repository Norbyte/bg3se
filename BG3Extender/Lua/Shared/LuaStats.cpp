#include <stdafx.h>
#include <Extender/ScriptExtender.h>
#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>
#include <Lua/Shared/Proxies/LuaPropertyMapHelpers.h>


namespace bg3se::lua::stats
{
	void PushStatsFunctor(lua_State* L, LifetimeHandle const& lifetime, Functor* functor)
	{
#define V(cls) case cls::FunctorType: \
		MakeObjectRef(L, static_cast<cls*>(functor), lifetime); \
		break;

		switch (functor->TypeId) {
			V(CustomDescriptionFunctor)
			V(ResurrectFunctor)
			V(SabotageFunctor)
			V(SummonFunctor)
			V(ForceFunctor)
			V(DouseFunctor)
			V(SwapPlacesFunctor)
			V(EqualizeFunctor)
			V(PickupFunctor)
			V(CreateSurfaceFunctor)
			V(CreateConeSurfaceFunctor)
			V(RemoveStatusFunctor)
			V(ExecuteWeaponFunctorsFunctor)
			V(TeleportSourceFunctor)
			V(SetStatusDurationFunctor)
			V(UseAttackFunctor)
			V(BreakConcentrationFunctor)
			V(RestoreResourceFunctor)
			V(SpawnFunctor)
			V(StabilizeFunctor)
			V(UnlockFunctor)
			V(ResetCombatTurnFunctor)
			V(RemoveAuraByChildStatusFunctor)
			V(ApplyStatusFunctor)
			V(DealDamageFunctor)
			V(UseActionResourceFunctor)
			V(CreateExplosionFunctor)
			V(SurfaceChangeFunctor)
			V(ApplyEquipmentStatusFunctor)
			V(RegainHitPointsFunctor)
			V(UseSpellFunctor)
			V(SummonInInventoryFunctor)
			V(SpawnInInventoryFunctor)
			V(RemoveUniqueStatusFunctor)
			V(DisarmWeaponFunctor)
			V(ExtenderFunctor)

		default:
			ERR("Unable to push unknown stats functor type %d to Lua!", functor->TypeId);
			push(L, nullptr);
			break;
		}
#undef V
	}
}
