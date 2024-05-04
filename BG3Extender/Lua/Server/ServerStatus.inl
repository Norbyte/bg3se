#include <stdafx.h>
#include <Lua/Server/LuaBindingServer.h>
#include <Lua/LuaSerializers.h>
#include <GameDefinitions/Character.h>
#include <GameDefinitions/Status.h>
#include <Extender/ScriptExtender.h>
#include <Extender/Server/ExtensionStateServer.h>

namespace bg3se::lua
{
	int StatusGetEngineType(lua_State* L, esv::Status* self)
	{
		push(L, self->GetStatusId());
		return 1;
	}
	

	void LuaMakeStatusProxy(lua_State* L, esv::Status* status, LifetimeHandle const& lifetime)
	{
#define S(ty, cls) case StatusType::ty: MakeDirectObjectRef(L, static_cast<cls*>(status), lifetime); break;

		switch (status->GetStatusId()) {
			S(DYING, esv::StatusDying)
			S(HEAL, esv::StatusHeal)
			S(KNOCKED_DOWN, esv::StatusKnockedDown)
			S(TELEPORT_FALLING, esv::StatusTeleportFalling)
			S(BOOST, esv::StatusBoost)
			S(REACTION, esv::StatusReaction)
			S(STORY_FROZEN, esv::StatusStoryFrozen)
			S(SNEAKING, esv::StatusSneaking)
			S(UNLOCK, esv::StatusUnlock)
			S(FEAR, esv::StatusFear)
			S(SMELLY, esv::StatusSmelly)
			S(INVISIBLE, esv::StatusInvisible)
			S(ROTATE, esv::StatusRotate)
			S(MATERIAL, esv::StatusMaterial)
			S(CLIMBING, esv::StatusClimbing)
			S(INCAPACITATED, esv::StatusIncapacitated)
			S(INSURFACE, esv::StatusInSurface)
			S(POLYMORPHED, esv::StatusPolymorphed)
			S(EFFECT, esv::StatusEffect)
			S(DEACTIVATED, esv::StatusDeactivated)
			S(DOWNED, esv::StatusDowned)

		default:
			MakeDirectObjectRef(L, status, lifetime);
			break;
		}

#undef S
	}
}
