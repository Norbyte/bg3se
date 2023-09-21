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

namespace bg3se::esv::lua
{
	using namespace ::bg3se::lua;

	esv::StatusMachine* GetEntityStatusMachine(EntityHandle const& entity)
	{
		/*auto character = gExtender->GetServer().GetEntityHelpers().GetComponent<Character>(entity);
		if (character != nullptr) {
			return character->StatusMachine;
		}

		auto item = gExtender->GetServer().GetEntityHelpers().GetComponent<Item>(entity);
		if (item != nullptr) {
			return item->StatusMachine;
		}*/

		return nullptr;
	}

	/*int PrepareStatus(lua_State* L)
	{
		StackCheck _(L, 1);

		auto gameObj = get<EntityProxy*>(L, 1);
		auto statusId = get<FixedString>(L, 2);
		float lifeTime = get<float>(L, 3);

		StatusMachine* statusMachine = GetEntityStatusMachine(gameObj->Handle());
		if (!statusMachine) {
			push(L, nullptr);
			return 1;
		}

		auto status = gOsirisProxy->GetStatusHelpers().PrepareStatus(statusMachine, statusId, lifeTime);
		if (!status) {
			push(L, nullptr);
			return 1;
		}

		StatusHandleProxy::New(L, ownerHandle, status->StatusHandle);
		return 1;
	}

	int ApplyStatus(lua_State* L)
	{
		auto status = StatusHandleProxy::CheckUserData(L, 1);
		StatusMachine* statusMachine{ nullptr };

		auto ownerHandle = status->OwnerHandle();
		if (ownerHandle.GetType() == (uint32_t)ObjectType::ServerCharacter) {
			auto character = GetEntityWorld()->GetCharacter(ownerHandle);
			if (character) {
				statusMachine = character->StatusMachine;
			}
		} else if (ownerHandle.GetType() == (uint32_t)ObjectType::ServerItem) {
			auto item = GetEntityWorld()->GetItem(ownerHandle);
			if (item) {
				statusMachine = item->StatusMachine;
			}
		}

		if (statusMachine == nullptr) {
			OsiError("No StatusMachine found for this status!");
			return 0;
		}

		auto applyStatus = GetStaticSymbols().esv__StatusMachine__ApplyStatus;
		if (applyStatus == nullptr) {
			OsiErrorS("esv::StatusMachine::ApplyStatus not found!");
			return 0;
		}

		auto statusObj = statusMachine->Get(status->StatusHandle());
		if (!statusObj) {
			OsiError("No status found with this handle!");
			return 0;
		}

		if ((statusObj->Flags2 & StatusFlags2::Started) == StatusFlags2::Started) {
			OsiError("Trying to apply status that was already started!");
			return 0;
		}

		applyStatus(statusMachine, statusObj);
		return 0;
	}*/
}

