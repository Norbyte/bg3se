#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_SE()

using SoundNameId = int32_t;
using SoundObjectId = uint64_t;
static constexpr uint64_t InvalidSoundObjectId = 0xffffffffffffffffull;

struct SoundManager : public ProtectedGameObject<SoundManager>
{
	virtual void Destroy() = 0;
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update() = 0;
	virtual void SetListenerTransformation() = 0;
	virtual void SetListenerTransformation2() = 0;
	virtual void SetScalingFactor() = 0;
	virtual void SetListeners() = 0;
	virtual void ActivateListener() = 0;
	virtual void DeactivateListener() = 0;
	virtual void GetMaxRadius() = 0;
	virtual void SetEnvironmentSends() = 0;
	virtual void SetPanningRule() = 0;
	virtual void sub_142F96D20() = 0;
	virtual void sub_142F96E20() = 0;
	virtual void AddControllerOutput() = 0;
	virtual void RemoveControllerOutput() = 0;
	virtual void sub_142F95140() = 0;
	virtual void sub_142F95110() = 0;

	virtual bool SetSwitch(const char* switchGroup, const char* switchState, SoundObjectId gameObjectId) = 0;
	virtual bool SetState(const char* stateGroup, const char* state) = 0;
	virtual bool SetRTPCValue(SoundObjectId gameObjectId, const char* name, float value, bool bypassInternalValueInterpolation = false) = 0;
	virtual float GetRTPCValue(SoundObjectId gameObjectId, const char* name) = 0;
	virtual void ResetRTPCValue(SoundObjectId gameObjectId, const char* name) = 0;
	virtual void CancelEvent(void* cookie) = 0;
	virtual uint32_t PausePlaying(uint32_t playingId) = 0;
	virtual uint32_t ResumePlaying(uint32_t playingId) = 0;
	virtual uint32_t StopPlayingId(uint32_t playingId, uint32_t transitionDurationMs = 0) = 0;
	virtual void StopAllOnObject(SoundObjectId gameObjectId, uint32_t transitionDurationMs = 0) = 0;
	virtual void StopAll() = 0;
	virtual void StopAllHUDSounds(uint8_t playerIndex) = 0;
	virtual void StopAllCineHUDSounds(uint8_t playerIndex) = 0;
	virtual void StopAllMusicSounds() = 0;
	virtual void StopAllAmbientSounds(uint8_t playerIndex) = 0;
	virtual void PauseAllSounds() = 0;
	virtual void ResumeAllSounds() = 0;
	virtual void SetObjectObstructionAndOcclusion(SoundObjectId emitterGameObjectId, uint8_t playerIndex, float obstructionLevel, float occlusionLevel) = 0;
	virtual void* GetSpeakerConfiguration(uint8_t playerIndex) = 0;
	virtual void SetListenerSpatialization(uint8_t playerIndex, bool spatialized, float* speakerVolumes) = 0;
	virtual void RegisterAudioDeviceStatusCallback() = 0;
	virtual void sub_142F972F0() = 0;
	virtual bool GetSourcePlayPosition(uint32_t playingId, uint32_t* position) = 0;
	virtual SoundNameId GetIDFromString(char const* name) = 0;
	virtual void Suspend() = 0;
	virtual void WakeupFromSuspend() = 0;
	virtual void Initialize() = 0;
	virtual void RenderAudio() = 0;
	virtual void Terminate() = 0;
	virtual void ClearBanks() = 0;
	virtual void ClearBanksAndEvents() = 0;
	virtual void LoadBanks() = 0;
	virtual SoundObjectId* RegisterSoundObject(SoundObjectId& soundObjectId) = 0;
	virtual void UnregisterSoundObject(SoundObjectId soundObjectId) = 0;
	virtual void RegisterBusMeteringCallback() = 0;
	virtual bool LoadEvent(SoundNameId eventId) = 0;
	virtual bool LoadEvent2(SoundNameId eventId) = 0;
	virtual bool UnloadEvent(SoundNameId eventId) = 0;
	virtual bool UnloadEvent2(SoundNameId eventId) = 0;
	virtual bool AsyncLoadEvent(SoundNameId eventId, void* callback) = 0;
	virtual bool AsyncLoadEvent2(SoundNameId eventId, void* callback) = 0;
	virtual bool AsyncUnloadEvent(SoundNameId eventId, void* callback) = 0;
	virtual bool AsyncUnloadEvent2(SoundNameId eventId, void* callback) = 0;
	virtual SoundNameId* LoadBank(SoundNameId& bankId, const char* bankName) = 0;
	virtual bool UnloadBank(SoundNameId bankId) = 0;
	virtual SoundNameId* AsyncLoadBank(SoundNameId& bankId, const char* bankName, void* callback) = 0;
	virtual bool AsyncUnloadBank(SoundNameId bankId, void* callback) = 0;
	virtual SoundNameId* PrepareBank(SoundNameId& bankId, const char* name) = 0;
	virtual bool UnprepareBank(SoundNameId bankId) = 0;
	virtual SoundNameId* AsyncPrepareBank(SoundNameId& bankId, const char* name, void* callback) = 0;
	virtual bool AsyncUnpepareBank(SoundNameId bankId, void* callback) = 0;
	virtual bool LoadGameSync(int gameSyncType, const char* groupName, const char* gameSyncName) = 0;
	virtual bool UnloadGameSyncs(int gameSyncType, const char* groupName, const char* gameSyncName) = 0;
	virtual bool AsyncLoadGameSync(int gameSyncType, const char* groupName, const char* gameSyncName, void* callback) = 0;
	virtual bool AsyncUnloadGameSync(int gameSyncType, const char* groupName, const char* gameSyncName, void* callback) = 0;
	virtual bool PostEventById(SoundObjectId gameObjectId, SoundNameId eventId, float seekPosition, bool enableGetSourcePlayPosition, void* callback) = 0;
	virtual bool PostEventByName(SoundObjectId gameObjectId, char const* eventName, float seekPosition, bool enableGetSourcePlayPosition, void* callback) = 0;
	virtual void QueueSoundEvent(void* evt) = 0;
	virtual bool SetChannelConfig() = 0;
	virtual bool PlayExternalSound(SoundObjectId gameObjectId, SoundNameId eventId, STDString const& path, uint8_t codecId, float seekPosition, bool enableGetSourcePlayPosition, void* callback) = 0;


	ecs::ComponentCallbackList field_10;
	Module Module;
	Array<FixedString> OS_SoundbankSearchPath_SfxSoundbankSearchPaths;
	Array<FixedString> OS_SoundbankSearchPath2_LocaSoundbankSearchPaths;
	SoundObjectId ControllerSpeakerListener[4];
	SoundObjectId Listener[4];
	SoundObjectId RumbleListener[4];
	SoundObjectId PlayerEmitter[4];
	SoundObjectId Ambient[4];
	SoundObjectId HUD[4];
	SoundObjectId CineHUD[4];
	SoundObjectId MusicHandle;
	__int64 field_270;
};

struct WwiseManager : public SoundManager
{};

END_NS()
