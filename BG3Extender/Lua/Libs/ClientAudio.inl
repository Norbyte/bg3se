#include <GameDefinitions/Symbols.h>
#include <GameDefinitions/Sound.h>

/// <lua_module>Audio</lua_module>
BEGIN_NS(ecl::lua::audio)

WwiseManager* GetSoundManager()
{
	auto resourceMgr = GetStaticSymbols().ls__gGlobalResourceManager;
	if (resourceMgr != nullptr && *resourceMgr != nullptr && (*resourceMgr)->SoundManager != nullptr) {
		return (*resourceMgr)->SoundManager;
	}

	LuaError("Sound manager is not available!");
	return nullptr;
}
/*
SoundObjectId GetSoundObjectId(lua_State* L, ComponentHandle const& handle, CharacterSoundObjectType type)
{
	EntityHandle entity;

	if (handle.GetType() == (uint32_t)ObjectHandleType::ClientCharacter) {
		auto character = GetEntityWorld()->GetComponent<Character>(handle);
		if (character) {
			entity = character->SoundComponents[(unsigned)type];
		} else {
			luaL_error(L, "No character object exists with the specified handle");
		}
	} else if (handle.GetType() == (uint32_t)ObjectHandleType::ClientItem) {
		entity = GetEntityWorld()->GetComponent<Item>(handle)->Base.Entity;
	} else {
		luaL_error(L, "Only character and item handles are supported as sound objects");
	}

	auto soundComp = GetEntityWorld()->GetComponent<SoundComponent>(entity);
	if (soundComp && soundComp->ActiveData) {
		return soundComp->ActiveData->WwiseSoundObjectId;
	} else {
		return InvalidSoundObjectId;
	}
}
*/
SoundObjectId GetSoundObjectId(lua_State* L, int idx)
{
	auto snd = GetSoundManager();
	if (!snd) return InvalidSoundObjectId;

	switch (lua_type(L, idx)) {
	case LUA_TNIL:
		return InvalidSoundObjectId;

	case LUA_TSTRING:
	{
		auto name = get<STDString>(L, idx);

		unsigned playerIndex = 0;
		if (name.size() > 1 && *name.rbegin() >= '1' && *name.rbegin() <= '4') {
			playerIndex = (*name.rbegin() - '1');
			name = name.substr(0, name.size() - 1);
		}

		if (name == "Global") {
			return InvalidSoundObjectId;
		} else if (name == "Music") {
			return snd->MusicHandle;
		} else if (name == "ControllerSpeakerListener") {
			return snd->ControllerSpeakerListener[playerIndex];
		} else if (name == "Listener") {
			return snd->Listener[playerIndex];
		} else if (name == "RumbleListener") {
			return snd->RumbleListener[playerIndex];
		} else if (name == "PlayerEmitter") {
			return snd->PlayerEmitter[playerIndex];
		} else if (name == "Ambient") {
			return snd->Ambient[playerIndex];
		} else if (name == "HUD") {
			return snd->HUD[playerIndex];
		} else if (name == "CineHUD") {
			return snd->CineHUD[playerIndex];
		} else {
			luaL_error(L, "Unknown built-in sound object name: %s", name);
		}
	}

	/*
	case LUA_TLIGHTUSERDATA:
	{
		auto handle = get<ComponentHandle>(L, idx);
		return GetSoundObjectId(L, handle, CharacterSoundObjectType::General);
	}*/

	default:
		luaL_error(L, "Must specify nil, character handle or built-in name as sound object");
		return InvalidSoundObjectId;
	}
}

END_NS()

BEGIN_SE()

enum class LuaSoundObjectId : SoundObjectId {};
MARK_INTEGRAL_ALIAS(LuaSoundObjectId)

END_SE()

BEGIN_NS(lua)

LuaSoundObjectId do_get(lua_State* L, int index, Overload<LuaSoundObjectId>)
{
	return LuaSoundObjectId{ ecl::lua::audio::GetSoundObjectId(L, index) };
}

END_NS()

BEGIN_NS(ecl::lua::audio)

using namespace bg3se::lua;

bool SetSwitch(LuaSoundObjectId soundObject, char const* switchGroup, char const* state)
{
	return GetSoundManager()->SetSwitch(switchGroup, state, (SoundObjectId)soundObject);
}

bool SetState(char const* stateGroup, char const* state)
{
	return GetSoundManager()->SetState(stateGroup, state);
}

bool SetRTPC(LuaSoundObjectId soundObject, char const* rtpcName, float value, std::optional<bool> bypassInternalValueInterpolation)
{
	return GetSoundManager()->SetRTPCValue((SoundObjectId)soundObject, rtpcName, value, bypassInternalValueInterpolation ? *bypassInternalValueInterpolation : false);
}

float GetRTPC(LuaSoundObjectId soundObject, char const* rtpcName)
{
	return GetSoundManager()->GetRTPCValue((SoundObjectId)soundObject, rtpcName);
}

void ResetRTPC(LuaSoundObjectId soundObject, char const* rtpcName)
{
	GetSoundManager()->ResetRTPCValue((SoundObjectId)soundObject, rtpcName);
}

void Stop(std::optional<LuaSoundObjectId> soundObject)
{
	auto snd = GetSoundManager();
	if (!snd) {
		return;
	}

	if (soundObject) {
		snd->StopAllOnObject((SoundObjectId)*soundObject);
	} else {
		snd->StopAll();
	}
}

void PauseAllSounds()
{
	auto snd = GetSoundManager();
	if (!snd) {
		return;
	}

	snd->PauseAllSounds();
}

void ResumeAllSounds()
{
	auto snd = GetSoundManager();
	if (!snd) {
		return;
	}

	snd->ResumeAllSounds();
}

bool PostEvent(LuaSoundObjectId soundObject, char const* eventName, std::optional<float> positionSec)
{
	return GetSoundManager()->PostEventByName((SoundObjectId)soundObject, eventName, positionSec.value_or(0.0f), false, nullptr);
}

bool LoadEvent(char const* eventName)
{
	SoundNameId eventId = GetSoundManager()->GetIDFromString(eventName);
	return GetSoundManager()->LoadEvent(eventId);
}

bool UnloadEvent(char const* eventName)
{
	SoundNameId eventId = GetSoundManager()->GetIDFromString(eventName);
	return GetSoundManager()->UnloadEvent(eventId);
}

bool PlayExternalSound(LuaSoundObjectId soundObject, char const* eventName, char const* path, unsigned int codecId, std::optional<float> positionSec)
{
	STDString lsPath = GetStaticSymbols().ToPath(path, PathRootType::Data);
	auto eventId = GetSoundManager()->GetIDFromString(eventName);
	return GetSoundManager()->PlayExternalSound((SoundObjectId)soundObject, eventId, lsPath, (uint8_t)codecId, positionSec.value_or(0.0f), false, nullptr);
}

bool LoadBank(const char* bankName)
{
	SoundNameId bankId{ -1 };
	GetSoundManager()->LoadBank(bankId, bankName);
	return bankId != -1;
}

bool UnloadBank(const char* bankName)
{
	SoundNameId bankId = GetSoundManager()->GetIDFromString(bankName);
	return GetSoundManager()->UnloadBank(bankId);
}

bool PrepareBank(const char* bankName)
{
	SoundNameId bankId{ -1 };
	GetSoundManager()->PrepareBank(bankId, bankName);
	return bankId != -1;
}

bool UnprepareBank(const char* bankName)
{
	SoundNameId bankId = GetSoundManager()->GetIDFromString(bankName);
	return GetSoundManager()->UnloadBank(bankId);
}

void RegisterAudioLib()
{
	DECLARE_MODULE(Audio, Client)
	BEGIN_MODULE()
	MODULE_FUNCTION(SetSwitch)
	MODULE_FUNCTION(SetState)
	MODULE_FUNCTION(SetRTPC)
	MODULE_FUNCTION(GetRTPC)
	MODULE_FUNCTION(ResetRTPC)
	MODULE_FUNCTION(Stop)
	MODULE_FUNCTION(PauseAllSounds)
	MODULE_FUNCTION(ResumeAllSounds)
	MODULE_FUNCTION(PostEvent)
	MODULE_FUNCTION(LoadEvent)
	MODULE_FUNCTION(UnloadEvent)
	MODULE_FUNCTION(PlayExternalSound)
	MODULE_FUNCTION(LoadBank)
	MODULE_FUNCTION(UnloadBank)
	MODULE_FUNCTION(PrepareBank)
	MODULE_FUNCTION(UnprepareBank)
	END_MODULE()
}

END_NS()
