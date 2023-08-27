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

	FunctorsProxy::FunctorsProxy(LifetimeHandle const& lifetime, Functors* obj)
		: object_(obj), lifetime_(lifetime)
	{}

	FunctorsProxy::~FunctorsProxy()
	{}

	void* FunctorsProxy::GetRaw()
	{
		return object_;
	}

	char const* FunctorsProxy::GetTypeName() const
	{
		return "StatsFunctorSet";
	}

	bool FunctorsProxy::GetElement(lua_State* L, unsigned arrayIndex)
	{
		if (object_ != nullptr && arrayIndex > 0 && arrayIndex <= object_->Functors.Size()) {
			PushStatsFunctor(L, lifetime_, object_->Functors[arrayIndex - 1]);
			return true;
		} else {
			return false;
		}
	}

	bool FunctorsProxy::SetElement(lua_State* L, unsigned arrayIndex, int luaIndex)
	{
		return false;
	}

	unsigned FunctorsProxy::Length()
	{
		return object_ != nullptr ? object_->Functors.Size() : 0;
	}

	int FunctorsProxy::Next(lua_State* L, int key)
	{
		if (object_ != nullptr && key >= 0 && key < (int)object_->Functors.Size()) {
			push(L, ++key);
			PushStatsFunctor(L, lifetime_, object_->Functors[key - 1]);
			return 2;
		} else {
			return 0;
		}
	}
}
