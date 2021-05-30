#include <stdafx.h>
#include <Lua/Server/LuaBindingServer.h>
#include <Lua/LuaSerializers.h>
#include <GameDefinitions/Character.h>
#include <GameDefinitions/Status.h>
#include <ScriptExtender.h>
#include <ExtensionStateServer.h>

namespace bg3se::esv::lua
{
	//#include <Lua/LevelIteratorFunctions.inl>
}

namespace bg3se::lua
{
	int StatusGetEngineType(lua_State* L, esv::Status* self)
	{
		push(L, self->GetStatusId());
		return 1;
	}
	

	void LuaMakeStatusProxy(lua_State* L, esv::Status* status, LifetimeHolder const& lifetime)
	{
#define S(ty, cls) case StatusType::ty: ObjectProxy::MakeRef<cls>(L, static_cast<cls*>(status), lifetime); break;

		switch (status->GetStatusId()) {
			S(DYING, esv::StatusDying)
			S(DESTROYING, esv::StatusDestroying)
			S(HEAL, esv::StatusHeal)
			S(KNOCKED_DOWN, esv::StatusKnockedDown)
			S(SUMMONING, esv::StatusSummoning)
			S(HEALING, esv::StatusHealing)
			S(TELEPORT_FALLING, esv::StatusTeleportFalling)
			S(BOOST, esv::StatusBoost)
			S(REACTION, esv::StatusReaction)
			S(STORY_FROZEN, esv::StatusStoryFrozen)
			S(SNEAKING, esv::StatusSneaking)
			S(UNLOCK, esv::StatusUnlock)
			S(FEAR, esv::StatusFear)
			S(SITTING, esv::StatusSitting)
			S(LYING, esv::StatusLying)
			S(SMELLY, esv::StatusSmelly)
			S(CLEAN, esv::StatusClean)
			S(INFECTIOUS_DISEASED, esv::StatusInfectiousDiseased)
			S(INVISIBLE, esv::StatusInvisible)
			S(ROTATE, esv::StatusRotate)
			S(ENCUMBERED, esv::StatusEncumbered)
			S(IDENTIFY, esv::StatusIdentify)
			S(REPAIR, esv::StatusRepair)
			S(MATERIAL, esv::StatusMaterial)
			S(WIND_WALKER, esv::StatusWindWalker)
			S(DECAYING_TOUCH, esv::StatusDecayingTouch)
			S(UNHEALABLE, esv::StatusUnhealable)
			S(FORCE_MOVE, esv::StatusForceMove)
			S(CLIMBING, esv::StatusClimbing)
			S(INCAPACITATED, esv::StatusIncapacitated)
			S(INSURFACE, esv::StatusInSurface)
			S(POLYMORPHED, esv::StatusPolymorphed)
			S(HEAL_SHARING, esv::StatusHealSharing)
			S(HEAL_SHARING_CASTER, esv::StatusHealSharingCaster)
			S(ACTIVE_DEFENSE, esv::StatusActiveDefense)
			S(CONSTRAINED, esv::StatusConstrained)
			S(EFFECT, esv::StatusEffect)
			S(DEACTIVATED, esv::StatusDeactivated)
			S(DOWNED, esv::StatusDowned)

		default:
			ObjectProxy::MakeRef<esv::Status>(L, status, lifetime);
			break;
		}

#undef S
	}
}

namespace bg3se::esv::lua
{
	using namespace ::bg3se::lua;

	char const* const StatusHandleProxy::MetatableName = "esv::HStatus";

	esv::Status* StatusHandleProxy::Get(lua_State* L)
	{
		auto character = gExtender->GetServerEntityHelpers().GetComponent<esv::Character>(character_);
		if (character == nullptr) {
			luaL_error(L, "Character handle invalid");
			return nullptr;
		}

		esv::Status* status;
		if (statusHandle_) {
			status = character->GetStatus(statusHandle_);
		} else {
			status = character->GetStatus(statusNetId_);
		}

		if (status == nullptr) luaL_error(L, "Status handle invalid");
		return status;
	}

	int StatusHandleProxy::Index(lua_State* L)
	{
		auto status = Get(L);
		if (!status) return 0;

		return luaL_error(L, "Not implemented yet!");

		/*StackCheck _(L, 1);
		auto prop = luaL_checkstring(L, 2);
		auto& propertyMap = StatusToPropertyMap(status);
		auto fetched = LuaPropertyMapGet(L, propertyMap, status, prop, true);
		if (!fetched) push(L, nullptr);
		return 1;*/
	}

	int StatusHandleProxy::NewIndex(lua_State* L)
	{
		StackCheck _(L, 0);
		auto status = Get(L);
		if (!status) return 0;

		return luaL_error(L, "Not implemented yet!");

		/*auto prop = luaL_checkstring(L, 2);
		auto& propertyMap = StatusToPropertyMap(status);
		LuaPropertyMapSet(L, 3, propertyMap, status, prop, true);
		return 0;*/
	}
}

