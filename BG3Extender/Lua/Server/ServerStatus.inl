#include <stdafx.h>
#include <Lua/Server/LuaBindingServer.h>
#include <Lua/LuaSerializers.h>
#include <GameDefinitions/Character.h>
#include <GameDefinitions/Status.h>
#include <OsirisProxy.h>
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
}

namespace bg3se::esv::lua
{
	using namespace ::bg3se::lua;

	char const* const StatusHandleProxy::MetatableName = "esv::HStatus";

	esv::Status* StatusHandleProxy::Get(lua_State* L)
	{
		auto character = gOsirisProxy->GetServerEntityHelpers().GetComponent<esv::Character>(character_);
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

namespace bg3se::esv::lua
{
	using namespace ::bg3se::lua;

	esv::Character* GetCharacter(lua_State* L, int index);


	int GetStatus(lua_State* L)
	{
		LuaServerPin lua(esv::ExtensionState::Get());
		if (lua->RestrictionFlags & State::RestrictHandleConversion) {
			return luaL_error(L, "Attempted to resolve status handle in restricted context");
		}

		esv::Character* character = GetCharacter(L, 1);
		if (character == nullptr) return 0;

		StackCheck _(L, 1);
		esv::Status* status{ nullptr };
		if (lua_type(L, 2) == LUA_TLIGHTUSERDATA) {
			auto statusHandle = checked_get<ObjectHandle>(L, 2);
			status = character->GetStatus(statusHandle);
			if (status != nullptr) {
				ObjectHandle characterHandle;
				character->GetObjectHandle(characterHandle);
				StatusHandleProxy::New(L, characterHandle, statusHandle);
				return 1;
			}

			OsiError("Character has no status with ObjectHandle 0x" << std::hex << statusHandle.Handle);
		} else {
			auto index = lua_tointeger(L, 2);

			// We need to keep integer status handle support since some extender Osiris events
			// (eg. NRD_OnHit, NRD_OnPrepareHit, etc.) use these handles and Osiris doesn't support lightuserdata
			if (index > 0xffffffff) {
				ObjectHandle statusHandle{ index };
				status = character->GetStatus(statusHandle);
				if (status != nullptr) {
					ObjectHandle characterHandle;
					character->GetObjectHandle(characterHandle);
					StatusHandleProxy::New(L, characterHandle, statusHandle);
					return 1;
				}

				OsiError("Character has no status with ObjectHandle 0x" << std::hex << statusHandle.Handle);
			} else {
				NetId statusNetId{ (uint32_t)index };
				status = character->GetStatus(statusNetId);
				if (status != nullptr) {
					ObjectHandle characterHandle;
					character->GetObjectHandle(characterHandle);
					StatusHandleProxy::New(L, characterHandle, statusNetId);
					return 1;
				}

				OsiError("Character has no status with NetId 0x" << std::hex << index);
			}
		}

		push(L, nullptr);
		return 1;
	}
}