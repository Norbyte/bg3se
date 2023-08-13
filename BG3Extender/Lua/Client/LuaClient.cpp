#include <stdafx.h>
#include <Lua/Shared/LuaStats.h>
#include <Lua/Client/LuaBindingClient.h>
#include <Lua/LuaSerializers.h>
#include <Extender/ScriptExtender.h>
#include <Extender/Client/ExtensionStateClient.h>
//#include <GameDefinitions/Ai.h>
#include "resource.h"

namespace bg3se::lua
{
/*	char const* const ObjectProxy<ecl::Status>::MetatableName = "ecl::Status";


	PropertyMapBase& ClientStatusToPropertyMap(ecl::Status* status)
	{
		// TODO - add property maps for statuses
		return gEclStatusPropertyMap;
	}

	ecl::Status* ObjectProxy<ecl::Status>::Get(lua_State* L)
	{
		if (obj_ == nullptr) luaL_error(L, "Status object no longer available");
		return obj_;
	}

	int ObjectProxy<ecl::Status>::Index(lua_State* L)
	{
		if (obj_ == nullptr) return luaL_error(L, "Status object no longer available");

		StackCheck _(L, 1);
		auto prop = luaL_checkstring(L, 2);

		if (strcmp(prop, "StatusType") == 0) {
			push(L, obj_->GetStatusId());
			return 1;
		}

		auto& propertyMap = ClientStatusToPropertyMap(obj_);
		auto fetched = LuaPropertyMapGet(L, propertyMap, obj_, prop, true);
		if (!fetched) push(L, nullptr);
		return 1;
	}

	int ObjectProxy<ecl::Status>::NewIndex(lua_State* L)
	{
		if (obj_ == nullptr) return luaL_error(L, "Status object no longer available");

		StackCheck _(L, 0);
		auto& propertyMap = ClientStatusToPropertyMap(obj_);
		return GenericSetter(L, propertyMap);
	}


#include <Lua/LuaShared.inl>

	char const* const ObjectProxy<ecl::PlayerCustomData>::MetatableName = "ecl::PlayerCustomData";

	ecl::PlayerCustomData* ObjectProxy<ecl::PlayerCustomData>::Get(lua_State* L)
	{
		if (obj_) return obj_;
		auto character = ecl::GetEntityWorld()->GetCharacter(handle_);
		if (character == nullptr) luaL_error(L, "Character handle invalid");

		if (character->PlayerData == nullptr
			// Always false on the client for some reason
			//|| !character->PlayerData->CustomData.Initialized
			) {
			OsiError("Character has no player data, or custom data was not initialized.");
			return nullptr;
		}

		return &character->PlayerData->CustomData;
	}

	int ObjectProxy<ecl::PlayerCustomData>::Index(lua_State* L)
	{
		return GenericGetter(L, gPlayerCustomDataPropertyMap);
	}

	int ObjectProxy<ecl::PlayerCustomData>::NewIndex(lua_State* L)
	{
		return GenericSetter(L, gPlayerCustomDataPropertyMap);
	}


	char const* const ObjectProxy<ecl::Character>::MetatableName = "ecl::Character";

	void ClientGetInventoryItems(lua_State* L, ComponentHandle inventoryHandle)
	{
		lua_newtable(L);

		auto inventory = ecl::FindInventoryByHandle(inventoryHandle);
		if (inventory != nullptr) {
			int32_t index = 1;
			for (auto itemHandle : inventory->ItemsBySlot) {
				if (itemHandle) {
					auto item = ecl::GetEntityWorld()->GetItem(itemHandle);
					if (item != nullptr) {
						settable(L, index++, item->MyGuid);
					}
				}
			}
		}
	}

	int ClientCharacterFetchProperty(lua_State* L, ecl::Character* character, FixedString const& prop)
	{
		if (prop == GFS.strPlayerCustomData) {
			if (character->PlayerData != nullptr
				// Always false on the client for some reason
				//&& character->PlayerData->CustomData.Initialized
				) {
				ComponentHandle handle;
				character->GetComponentHandle(handle);
				ObjectProxy<ecl::PlayerCustomData>::New(L, handle);
			} else {
				OsiError("Character has no player data, or custom data was not initialized.");
				push(L, nullptr);
			}
			return 1;
		}

		if (prop == GFS.strStats) {
			if (character->Stats != nullptr) {
				// FIXME - use handle based proxy
				ObjectProxy<CDivinityStats_Character>::New(L, character->Stats);
			} else {
				OsiError("Character has no stats.");
				push(L, nullptr);
			}
			return 1;
		}

		if (prop == GFS.strHandle) {
			push(L, character->Base.Component.Handle);
			return 1;
		}

		if (prop == GFS.strRootTemplate) {
			ObjectProxy<CharacterTemplate>::New(L, character->Template);
			return 1;
		}

		if (prop == GFS.strDisplayName) {
			return GameObjectGetDisplayName<ecl::Character>(L, character);
		}

		auto fetched = LuaPropertyMapGet(L, gEclCharacterPropertyMap, character, prop, true);
		if (!fetched) push(L, nullptr);
		return 1;
	}

	ecl::Character* ObjectProxy<ecl::Character>::Get(lua_State* L)
	{
		if (obj_) return obj_;
		auto character = ecl::GetEntityWorld()->GetCharacter(handle_);
		if (character == nullptr) luaL_error(L, "Character handle invalid");
		return character;
	}

	int ClientCharacterGetInventoryItems(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = get<ObjectProxy<ecl::Character>*>(L, 1);

		ClientGetInventoryItems(L, self->Get(L)->InventoryHandle);

		return 1;
	}

	int ClientCharacterGetItemBySlot(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = get<ObjectProxy<ecl::Character>*>(L, 1);
		auto slot = (uint32_t)get<ItemSlot32>(L, 2);

		auto inventory = ecl::FindInventoryByHandle(self->Get(L)->InventoryHandle);
		if (inventory != nullptr && slot < inventory->ItemsBySlot.Size) {
			auto itemHandle = inventory->ItemsBySlot[slot];
			if (itemHandle) {
				auto item = ecl::GetEntityWorld()->GetItem(itemHandle);
				if (item != nullptr) {
					push(L, item->MyGuid);
					return 1;
				}
			}
		}

		lua_pushnil(L);
		return 1;
	}

	int ObjectProxy<ecl::Character>::Index(lua_State* L)
	{
		auto character = Get(L);
		if (!character) return 0;

		auto prop = luaL_checkstring(L, 2);
		auto propFS = ToFixedString(prop);
		if (!propFS) {
			OsiError("Illegal property name: " << prop);
			return 0;
		}

		StackCheck _(L, 1);
		if (propFS == GFS.strGetInventoryItems) {
			lua_pushcfunction(L, &ClientCharacterGetInventoryItems);
			return 1;
		}

		if (propFS == GFS.strGetItemBySlot) {
			lua_pushcfunction(L, &ClientCharacterGetItemBySlot);
			return 1;
		}

		if (propFS == GFS.strHasTag) {
			lua_pushcfunction(L, &GameObjectHasTag<ecl::Character>);
			return 1;
		}

		if (propFS == GFS.strGetTags) {
			lua_pushcfunction(L, &GameObjectGetTags<ecl::Character>);
			return 1;
		}

		if (propFS == GFS.strGetStatus) {
			lua_pushcfunction(L, (&GameObjectGetStatus<ecl::Character, ecl::Status>));
			return 1;
		}

		if (propFS == GFS.strGetStatusByType) {
			lua_pushcfunction(L, (&GameObjectGetStatusByType<ecl::Character, ecl::Status>));
			return 1;
		}

		if (propFS == GFS.strGetStatuses) {
			lua_pushcfunction(L, (&GameObjectGetStatuses<ecl::Character>));
			return 1;
		}

		if (propFS == GFS.strGetStatusObjects) {
			lua_pushcfunction(L, (&GameObjectGetStatusObjects<ecl::Character, ecl::Status>));
			return 1;
		}

		if (propFS == GFS.strSetScale) {
			lua_pushcfunction(L, (&GameObjectSetScale<ecl::Character>));
			return 1;
		}

		return ClientCharacterFetchProperty(L, character, propFS);
	}

	int ObjectProxy<ecl::Character>::NewIndex(lua_State* L)
	{
		return GenericSetter(L, gEclCharacterPropertyMap);
	}


	char const* const ObjectProxy<ecl::Item>::MetatableName = "ecl::Item";

	ecl::Item* ObjectProxy<ecl::Item>::Get(lua_State* L)
	{
		if (obj_) return obj_;
		auto item = ecl::GetEntityWorld()->GetItem(handle_);
		if (item == nullptr) luaL_error(L, "Item handle invalid");
		return item;
	}

	int ClientItemGetInventoryItems(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = get<ObjectProxy<ecl::Item>*>(L, 1);

		ClientGetInventoryItems(L, self->Get(L)->InventoryHandle);

		return 1;
	}

	int ItemGetOwnerCharacter(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = get<ObjectProxy<ecl::Item>*>(L, 1);

		auto inventory = ecl::FindInventoryByHandle(self->Get(L)->InventoryParentHandle);

		for (;;) {
			if (inventory == nullptr) {
				break;
			}

			auto parent = inventory->ParentHandle;
			if (parent.GetType() == (uint32_t)ObjectType::ClientItem) {
				auto item = ecl::GetEntityWorld()->GetItem(parent);
				if (item) {
					inventory = ecl::FindInventoryByHandle(item->InventoryParentHandle);
				} else {
					break;
				}
			} else if (parent.GetType() == (uint32_t)ObjectType::ClientCharacter) {
				auto character = ecl::GetEntityWorld()->GetCharacter(parent);
				if (character) {
					push(L, character->MyGuid);
					return 1;
				} else {
					break;
				}
			} else {
				break;
			}
		}

		lua_pushnil(L);
		return 1;
	}

	int ObjectProxy<ecl::Item>::Index(lua_State* L)
	{
		auto item = Get(L);
		if (!item) return 0;

		StackCheck _(L, 1);
		auto prop = luaL_checkstring(L, 2);
		auto propFS = ToFixedString(prop);

		if (propFS == GFS.strGetInventoryItems) {
			lua_pushcfunction(L, &ClientItemGetInventoryItems);
			return 1;
		}

		if (propFS == GFS.strGetOwnerCharacter) {
			lua_pushcfunction(L, &ItemGetOwnerCharacter);
			return 1;
		}

		if (propFS == GFS.strHasTag) {
			lua_pushcfunction(L, &GameObjectHasTag<ecl::Item>);
			return 1;
		}

		if (propFS == GFS.strGetTags) {
			lua_pushcfunction(L, &GameObjectGetTags<ecl::Item>);
			return 1;
		}

		if (propFS == GFS.strGetStatus) {
			lua_pushcfunction(L, (&GameObjectGetStatus<ecl::Item, ecl::Status>));
			return 1;
		}

		if (propFS == GFS.strGetStatusByType) {
			lua_pushcfunction(L, (&GameObjectGetStatusByType<ecl::Item, ecl::Status>));
			return 1;
		}

		if (propFS == GFS.strGetStatuses) {
			lua_pushcfunction(L, (&GameObjectGetStatuses<ecl::Item>));
			return 1;
		}

		if (propFS == GFS.strGetStatusObjects) {
			lua_pushcfunction(L, (&GameObjectGetStatusObjects<ecl::Item, ecl::Status>));
			return 1;
		}

		if (propFS == GFS.strStats) {
			if (item->Stats != nullptr) {
				// FIXME - use handle based proxy
				ObjectProxy<CDivinityStats_Item>::New(L, item->Stats);
				return 1;
			} else {
				OsiError("Item has no stats.");
				push(L, nullptr);
				return 1;
			}
		}

		if (propFS == GFS.strHandle) {
			push(L, item->Base.Component.Handle);
			return 1;
		}

		if (propFS == GFS.strRootTemplate) {
			ObjectProxy::MakeRef<ItemTemplate>(L, item->CurrentTemplate);
			return 1;
		}

		if (propFS == GFS.strDisplayName) {
			return GameObjectGetDisplayName<ecl::Item>(L, item);
		}

		bool fetched = false;
		if (item->Stats != nullptr) {
			fetched = LuaPropertyMapGet(L, gItemStatsPropertyMap, item->Stats, propFS, false);
		}

		if (!fetched) {
			fetched = LuaPropertyMapGet(L, gEclItemPropertyMap, item, propFS, true);
		}

		if (!fetched) push(L, nullptr);
		return 1;
	}

	int ObjectProxy<ecl::Item>::NewIndex(lua_State* L)
	{
		StackCheck _(L, 0);
		return GenericSetter(L, gEclItemPropertyMap);
	}*/
}


namespace bg3se::ecl::lua
{
	using namespace bg3se::lua;

	LifetimeHandle GetClientLifetime()
	{
		assert(gExtender->GetClient().IsInClientThread());
		return ExtensionState::Get().GetLua()->GetCurrentLifetime();
	}

	LifetimePool& GetClientLifetimePool()
	{
		assert(gExtender->GetClient().IsInClientThread());
		return ExtensionState::Get().GetLua()->GetLifetimePool();
	}

	void ExtensionLibraryClient::Register(lua_State * L)
	{
		ExtensionLibrary::Register(L);
		/*StatusHandleProxy::RegisterMetatable(L);
		ObjectProxy<Status>::RegisterMetatable(L);
		ObjectProxy<PlayerCustomData>::RegisterMetatable(L);
		ObjectProxy<Character>::RegisterMetatable(L);
		ObjectProxy<Item>::RegisterMetatable(L);*/
	}


/*	ecl::Character* GetCharacter(lua_State* L, int index)
	{
		ecl::Character* character = nullptr;
		switch (lua_type(L, index)) {
		case LUA_TLIGHTUSERDATA:
		{
			auto handle = get<ComponentHandle>(L, index);
			if (handle.GetType() == (uint32_t)ObjectType::ServerCharacter) {
				OsiError("Attempted to resolve server ComponentHandle on the client");
			}
			else {
				character = GetEntityWorld()->GetCharacter(handle);
			}
			break;
		}

		case LUA_TNUMBER:
		{
			auto value = lua_tointeger(L, index);
			if (value > 0xffffffff) {
				OsiError("Resolving integer object handles is deprecated since v52!")
				ComponentHandle handle{ value };
				if (handle.GetType() == (uint32_t)ObjectType::ServerCharacter) {
					OsiError("Attempted to resolve server ComponentHandle on the client");
				} else {
					character = GetEntityWorld()->GetCharacter(handle);
				}
			} else {
				NetId netId{ (uint32_t)value };
				character = GetEntityWorld()->GetCharacter(netId);
			}
			break;
		}

		case LUA_TSTRING:
		{
			auto guid = lua_tostring(L, index);
			character = GetEntityWorld()->GetCharacter(guid);
			break;
		}

		default:
			OsiError("Expected character Guid, Handle or NetId");
			break;
		}

		return character;
	}

	int GetCharacter(lua_State* L)
	{
		StackCheck _(L, 1);
		LuaClientPin lua(ExtensionState::Get());

		ecl::Character* character = GetCharacter(L, 1);

		if (character != nullptr) {
			ComponentHandle handle;
			character->GetComponentHandle(handle);
			ObjectProxy<ecl::Character>::New(L, handle);
		} else {
			push(L, nullptr);
		}

		return 1;
	}

	int GetItem(lua_State* L)
	{
		StackCheck _(L, 1);
		LuaClientPin lua(ExtensionState::Get());

		ecl::Item* item = nullptr;
		switch (lua_type(L, 1)) {
		case LUA_TLIGHTUSERDATA:
		{
			auto handle = get<ComponentHandle>(L, 1);
			if (handle.GetType() == (uint32_t)ObjectType::ServerItem) {
				OsiError("Attempted to resolve server ComponentHandle on the client");
			} else {
				item = GetEntityWorld()->GetItem(handle);
			}
			break;
		}

		case LUA_TNUMBER:
		{
			auto value = lua_tointeger(L, 1);
			if (value > 0xffffffff) {
				ComponentHandle handle{ value };
				if (handle.GetType() == (uint32_t)ObjectType::ServerItem) {
					OsiError("Attempted to resolve server ComponentHandle on the client");
				} else {
					item = GetEntityWorld()->GetItem(handle);
				}
			} else {
				NetId netId{ (uint32_t)value };
				item = GetEntityWorld()->GetItem(netId);
			}
			break;
		}

		case LUA_TSTRING:
		{
			auto guid = lua_tostring(L, 1);
			item = GetEntityWorld()->GetItem(guid);
			break;
		}

		default:
			OsiError("Expected item Guid, Handle or NetId; got " << lua_typename(L, lua_type(L, 1)));
			return 0;
		}

		if (item != nullptr) {
			ComponentHandle handle;
			item->GetComponentHandle(handle);
			ObjectProxy<ecl::Item>::New(L, handle);
		} else {
			push(L, nullptr);
		}

		return 1;
	}

	int GetStatus(lua_State* L)
	{
		StackCheck _(L, 1);
		LuaClientPin lua(ExtensionState::Get());

		auto character = GetCharacter(L, 1);
		if (character == nullptr) return 0;

		ecl::Status* status;
		if (lua_type(L, 2) == LUA_TLIGHTUSERDATA) {
			auto statusHandle = get<ComponentHandle>(L, 2);
			status = character->GetStatus(statusHandle);

			if (status != nullptr) {
				ComponentHandle characterHandle;
				character->GetComponentHandle(characterHandle);
				StatusHandleProxy::New(L, characterHandle, statusHandle);
				return 1;
			}

			OsiError("Character has no status with ComponentHandle 0x" << std::hex << statusHandle.Handle);
		} else {
			auto index = lua_tointeger(L, 2);
			NetId statusNetId{ (uint32_t)index };
			status = character->GetStatus(statusNetId);

			if (status != nullptr) {
				ComponentHandle characterHandle;
				character->GetComponentHandle(characterHandle);
				StatusHandleProxy::New(L, characterHandle, statusNetId);
				return 1;
			}

			OsiError("Character has no status with NetId 0x" << std::hex << index);
		}

		push(L, nullptr);
		return 1;
	}

	int GetAiGrid(lua_State* L)
	{
		auto level = GetStaticSymbols().GetCurrentClientLevel();
		if (!level || !level->AiGrid) {
			OsiError("Current level not available yet!");
			return 0;
		}

		ObjectProxy<eoc::AiGrid>::New(L, level->AiGrid);
		return 1;
	}

	bool OsirisIsCallableClient(lua_State* L)
	{
		return false;
	}


	void PostMessageToServer(lua_State * L, char const* channel, char const* payload)
	{
		auto & networkMgr = gExtender->GetNetworkManager();
		auto msg = networkMgr.GetFreeClientMessage();
		if (msg != nullptr) {
			auto postMsg = msg->GetMessage().mutable_post_lua();
			postMsg->set_channel_name(channel);
			postMsg->set_payload(payload);
			networkMgr.ClientSend(msg);
		} else {
			OsiErrorS("Could not get free message!");
		}
	}

	char const* const StatusHandleProxy::MetatableName = "ecl::HStatus";

	ecl::Status* StatusHandleProxy::Get(lua_State* L)
	{
		auto character = GetEntityWorld()->GetCharacter(character_);
		if (character == nullptr) {
			luaL_error(L, "Character handle invalid");
			return nullptr;
		}

		ecl::Status* status;
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
		StackCheck _(L, 1);
		auto status = Get(L);

		auto prop = luaL_checkstring(L, 2);
		auto& propertyMap = ClientStatusToPropertyMap(status);
		auto fetched = LuaPropertyMapGet(L, propertyMap, status, prop, true);
		if (!fetched) push(L, nullptr);
		return 1;
	}

	int StatusHandleProxy::NewIndex(lua_State* L)
	{
		StackCheck _(L, 0);
		auto status = Get(L);
		if (!status) return 0;

		auto prop = luaL_checkstring(L, 2);
		auto& propertyMap = ClientStatusToPropertyMap(status);
		LuaPropertyMapSet(L, 3, propertyMap, status, prop, true);
		return 0;
	}

	int GetGameState(lua_State* L)
	{
		StackCheck _(L, 1);
		auto state = GetStaticSymbols().GetClientState();
		if (state) {
			push(L, *state);
		} else {
			lua_pushnil(L);
		}

		return 1;
	}

	int UpdateShroud(lua_State* L)
	{
		StackCheck _(L, 0);
		auto x = get<float>(L, 1);
		auto y = get<float>(L, 2);
		auto layer = get<ShroudType>(L, 3);
		auto value = get<int>(L, 4);

		if (value < 0 || value > 255) {
			OsiError("Can only set shroud cell values between 0 and 255");
			return 0;
		}

		auto level = GetStaticSymbols().GetCurrentClientLevel();
		if (!level || !level->ShroudManager || !level->ShroudManager->ShroudData) {
			OsiError("No ShroudManager for current level?");
			return 0;
		}

		level->ShroudManager->ShroudData->SetByteAtPos(layer, x, y, (uint8_t)value);
		return 0; 
	}


	WrapLuaFunction(OsirisIsCallableClient)
	WrapLuaFunction(PostMessageToServer)

	namespace audio
	{

	EoCSoundManager* GetSoundManager()
	{
		auto resourceMgr = GetStaticSymbols().GetResourceManager();
		if (resourceMgr != nullptr && resourceMgr->SoundManager != nullptr) {
			return resourceMgr->SoundManager;
		}

		LuaError("Sound manager is not available!");
		return nullptr;
	}

	std::optional<SoundObjectId> GetSoundObjectId(lua_State* L, int idx)
	{
		auto snd = GetSoundManager();
		if (!snd) return {};

		switch (lua_type(L, idx)) {
		case LUA_TNIL:
			return 0xffffffffffffffffull;

		case LUA_TSTRING:
		{
			auto name = get<char const*>(L, idx);
			if (strcmp(name, "Global") == 0) {
				return 0xffffffffffffffffull;
			} else if (strcmp(name, "Music") == 0) {
				return snd->MusicHandle;
			} else if (strcmp(name, "Ambient") == 0) {
				return snd->AmbientHandle;
			} else if (strcmp(name, "HUD") == 0) {
				return snd->HUDHandle;
			} else if (strcmp(name, "GM") == 0) {
				return snd->GMSoundHandle;
			} else if (strcmp(name, "Player1") == 0) {
				return snd->PlayerSoundHandles[0];
			} else if (strcmp(name, "Player2") == 0) {
				return snd->PlayerSoundHandles[1];
			} else if (strcmp(name, "Player3") == 0) {
				return snd->PlayerSoundHandles[2];
			} else if (strcmp(name, "Player4") == 0) {
				return snd->PlayerSoundHandles[3];
			} else {
				LuaError("Unknown built-in sound object name: " << name);
				return {};
			}
		}

		case LUA_TLIGHTUSERDATA:
		{
			auto handle = get<ComponentHandle>(L, idx);
			if (handle.GetType() == (uint32_t)ObjectType::ClientCharacter) {
				auto character = GetEntityWorld()->GetCharacter(handle);
				if (character) {
					return character->SoundObjectHandles[0];
				} else {
					return {};
				}
			} else {
				LuaError("Only character handles are supported as sound objects");
				return {};
			}
		}

		default:
			LuaError("Must specify nil, character handle or built-in name as sound object");
			return {};
		}
	}

	int SetSwitch(lua_State* L)
	{
		auto soundObject = GetSoundObjectId(L, 1);
		auto switchGroup = get<char const*>(L, 2);
		auto state = get<char const*>(L, 3);

		if (!soundObject) {
			push(L, false);
			return 1;
		}

		bool ok = GetSoundManager()->SetSwitch(switchGroup, state, *soundObject);
		push(L, ok);
		return 1;
	}

	int SetState(lua_State* L)
	{
		auto stateGroup = get<char const*>(L, 1);
		auto state = get<char const*>(L, 2);

		if (!GetSoundManager()) {
			push(L, false);
			return 1;
		}

		bool ok = GetSoundManager()->SetState(stateGroup, state);
		push(L, ok);
		return 1;
	}

	int SetRTPC(lua_State* L)
	{
		auto soundObject = GetSoundObjectId(L, 1);
		auto rtpcName = get<char const*>(L, 2);
		auto value = get<float>(L, 3);

		if (!soundObject) {
			push(L, false);
			return 1;
		}

		bool ok = GetSoundManager()->SetRTPCValue(*soundObject, rtpcName, value) == 1;
		push(L, ok);
		return 1;
	}

	int GetRTPC(lua_State* L)
	{
		auto soundObject = GetSoundObjectId(L, 1);
		auto rtpcName = get<char const*>(L, 2);

		if (!soundObject) {
			push(L, nullptr);
			return 1;
		}

		float value = GetSoundManager()->GetRTPCValue(*soundObject, rtpcName);
		push(L, value);
		return 1;
	}

	int ResetRTPC(lua_State* L)
	{
		auto soundObject = GetSoundObjectId(L, 1);
		auto rtpcName = get<char const*>(L, 2);

		if (!soundObject) {
			push(L, false);
			return 1;
		}

		bool ok = GetSoundManager()->ResetRTPCValue(*soundObject, rtpcName) == 1;
		push(L, ok);
		return 1;
	}

	int Stop(lua_State* L)
	{
		auto snd = GetSoundManager();
		if (!snd) {
			return 0;
		}

		if (lua_gettop(L) == 0) {
			snd->StopAll();
		} else {
			auto soundObject = GetSoundObjectId(L, 1);
			if (soundObject) {
				snd->StopAllOnObject(*soundObject);
			}
		}

		return 0;
	}

	int PauseAllSounds(lua_State* L)
	{
		auto snd = GetSoundManager();
		if (!snd) {
			return 0;
		}

		snd->PauseAllSounds();
		return 0;
	}

	int ResumeAllSounds(lua_State* L)
	{
		auto snd = GetSoundManager();
		if (!snd) {
			return 0;
		}

		snd->ResumeAllSounds();
		return 0;
	}

	int PostEvent(lua_State* L)
	{
		auto soundObject = GetSoundObjectId(L, 1);
		auto eventName = get<char const*>(L, 2);
		float positionSec = 0.0f;
		if (lua_gettop(L) > 2) {
			positionSec = get<float>(L, 3);
		}

		if (!soundObject) {
			push(L, false);
			return 1;
		}

		bool ok = GetSoundManager()->PostEvent(*soundObject, eventName, positionSec, nullptr);
		push(L, ok);
		return 1;
	}

	int PlayExternalSound(lua_State* L)
	{
		auto soundObject = GetSoundObjectId(L, 1);
		auto eventName = get<char const*>(L, 2);
		auto path = get<char const*>(L, 3);
		auto codecId = get<unsigned int>(L, 4);

		if (!soundObject) {
			push(L, false);
			return 1;
		}

		Path lsPath;
		lsPath.Name = GetStaticSymbols().ToPath(path, PathRootType::Data);
		auto eventId = GetSoundManager()->GetIDFromString(eventName);
		bool ok = GetSoundManager()->PlayExternalSound(*soundObject, eventId, lsPath, codecId, nullptr);
		push(L, ok);
		return 1;
	}

	}*/



	void ExtensionLibraryClient::RegisterLib(lua_State * L)
	{
		static const luaL_Reg extLib[] = {
			/*{"GetCharacter", GetCharacter},
			{"GetItem", GetItem},
			{"GetStatus", GetStatus},
			{"GetAiGrid", GetAiGrid},
			{"NewDamageList", NewDamageList},
			{"GetSurfaceTemplate", GetSurfaceTemplate},
			{"OsirisIsCallable", OsirisIsCallableClientWrapper},*/

			// EXPERIMENTAL FUNCTIONS
			//{"UpdateShroud", UpdateShroud},

			//{"PostMessageToServer", PostMessageToServerWrapper},
			{0,0}
		};

		luaL_newlib(L, extLib); // stack: lib
		lua_setglobal(L, "Ext"); // stack: -

		/*static const luaL_Reg soundLib[] = {
			{"SetSwitch", audio::SetSwitch},
			{"SetState", audio::SetState},
			{"SetRTPC", audio::SetRTPC},
			{"GetRTPC", audio::GetRTPC},
			{"ResetRTPC", audio::ResetRTPC},
			{"Stop", audio::Stop},
			{"PauseAllSounds", audio::PauseAllSounds},
			{"ResumeAllSounds", audio::ResumeAllSounds},
			{"PostEvent", audio::PostEvent},
			{"PlayExternalSound", audio::PlayExternalSound},
			{0,0}
		};

		lua_getglobal(L, "Ext"); // stack: Ext
		luaL_newlib(L, soundLib); // stack: ext, lib
		lua_setfield(L, -2, "Audio");
		lua_pop(L, 1);*/

		RegisterSharedLibraries(L);
	}


	ClientState::ClientState()
		: State(false)
	{}

	ClientState::~ClientState()
	{
		auto & sym = GetStaticSymbols();

#if !defined(OSI_NO_DEBUGGER)
		if (gExtender) {
			auto debugger = gExtender->GetLuaDebugger();
			if (debugger) {
				debugger->ClientStateDeleted();
			}
		}
#endif
	}

	void ClientState::Initialize()
	{
		StackCheck _(L, 0);
		library_.Register(L);

		gExtender->GetClient().GetExtensionState().LuaLoadBuiltinFile("ClientStartup.lua");
		/*
		lua_getglobal(L, "Ext"); // stack: Ext
		StatsExtraDataProxy::New(L); // stack: Ext, ExtraDataProxy
		lua_setfield(L, -2, "ExtraData"); // stack: Ext
		lua_pop(L, 1); // stack: -
		*/

		// Ext is not writeable after loading SandboxStartup!
		gExtender->GetClient().GetExtensionState().LuaLoadBuiltinFile("SandboxStartup.lua");

#if !defined(OSI_NO_DEBUGGER)
		auto debugger = gExtender->GetLuaDebugger();
		if (debugger) {
			debugger->ClientStateCreated(this);
		}
#endif
	}

	bool ClientState::IsClient()
	{
		return true;
	}

	void ClientState::OnUpdate(GameTime const& time)
	{
		State::OnUpdate(time);
	}


	EntityWorldBase* ClientState::GetEntityWorld()
	{
		return GetStaticSymbols().GetClientEntityWorld();
	}


	EntitySystemHelpersBase* ClientState::GetEntitySystemHelpers()
	{
		return &gExtender->GetClient().GetEntityHelpers();
	}


	void ClientState::OnGameStateChanged(GameState fromState, GameState toState)
	{
		GameStateChangeEventParams params{ fromState, toState };
		ThrowEvent("GameStateChanged", params, false, 0, ReadOnlyEvent{});
	}
}
