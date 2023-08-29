#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/Module.h>
#include <GameDefinitions/CommonTypes.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_SE()

struct IGameObject : public ProtectedGameObject<IGameObject>
{
	virtual ~IGameObject() = 0;
	virtual void HandleTextKeyEvent() = 0;
	virtual uint64_t Ret5() = 0;
	virtual void SetComponentHandle(ComponentHandle Handle) = 0;
	virtual void GetComponentHandle(ComponentHandle& Handle) const = 0;
	virtual void SetGuid(FixedString const& fs) = 0;
	virtual FixedString* GetGuid() const = 0;
	virtual void SetNetId(NetId netId) = 0;
	virtual void GetNetId(NetId& netId) const = 0;
	virtual void SetCurrentTemplate(void* esvTemplate) = 0;
	virtual void* GetCurrentTemplate() const = 0;
	virtual void SetGlobal(bool isGlobal) = 0;
	virtual bool IsGlobal() const = 0;
	virtual uint32_t GetComponentType() = 0;
	virtual void* GetEntityObjectByHandle(ComponentHandle handle) = 0;
	virtual void* GetUnknownHandle() = 0;
	virtual STDString* GetName() = 0;
	virtual void SetFlags(uint64_t flag) = 0;
	virtual void ClearFlags(uint64_t flag) = 0;
	virtual bool HasFlag(uint64_t flag) const = 0;
	virtual void SetAiColliding_M(bool colliding) = 0;
	virtual void Unknown1() = 0;
	virtual void Unknown2() = 0;
	virtual void Unknown3() = 0;
	virtual void Unknown4() = 0;
	virtual void Unknown5() = 0;
	/*virtual void GetTags(ObjectSet<FixedString> & tags) = 0;
	virtual bool IsTagged(FixedString & tag) = 0;*/
	virtual Vector3 const* GetTranslate() const = 0;
	virtual glm::mat3 const* GetRotation() const = 0;
	virtual float GetScale() const = 0;
	virtual void SetTranslate(Vector3 const& translate) = 0;
	virtual void SetRotation(glm::mat3 const& rotate) = 0;
	virtual void SetScale(float scale) = 0;
	virtual float GetHeight_M() = 0;
	virtual FixedString& GetParentUUID_M() = 0;
	virtual FixedString& GetCurrentLevel() = 0;
	virtual void SetCurrentLevel(FixedString const& fs) = 0;

	void* VMT2;
};

/*struct IEocClientObject : public IGameObject
{
	virtual eoc::Ai * GetAi() = 0;
	virtual void LoadAi() = 0;
	virtual void UnloadAi() = 0;
	virtual void Unknown0() = 0;
	virtual void Unknown1() = 0;
	virtual void Unknown2() = 0;
	virtual void Unknown3() = 0;
	virtual FixedString * Unknown4() = 0;
	virtual bool Unknown5() = 0;
	virtual TranslatedString* GetDisplayName(TranslatedString* name) = 0;
	virtual float Unknown6() = 0;
	virtual void SavegameVisit() = 0;
	virtual void SetLight(FixedString *) = 0;
	virtual void * GetLight() = 0;
	virtual void RemoveLight() = 0;
	virtual FixedString * GetPlayerRace(bool returnPolymorph, bool excludeUninitialized) = 0;
	virtual FixedString* GetPlayerOrigin(bool returnPolymorph, bool excludeUninitialized) = 0;
};*/

struct IEoCServerObject : public IGameObject
{
	virtual void MarkVisibleToPeerId(uint32_t* peerId) = 0;
	virtual void UnmarkVisibleToPeerId(uint32_t* peerId) = 0;
	virtual void ClearAllVisiblePeerIds() = 0;
	virtual void IsVisibleToPeer(uint32_t* peerId) = 0;
	virtual void* GetBoundComponent() = 0;
	virtual void LoadAi_M() = 0;
	virtual bool UnloadAi_M() = 0;
	virtual TranslatedString* GetDisplayName(TranslatedString& name) = 0;
	virtual void SetOriginalTemplate(void* esvTemplate) = 0;
	virtual bool SavegameVisit(void* visitor) = 0;
	virtual NetId& GetEntityNetworkId(NetId& netId) const = 0;
	virtual void SetTemplate(void* esvTemplate) = 0;
	virtual void SetTemplateId(uint8_t templateType, FixedString const& templateId) = 0;
	virtual void* GetSomeTransformValue() = 0;
	virtual void SetSomeTransformValue(void* unknown) = 0;

	BaseComponent Base;
};

END_SE()

BEGIN_NS(esv)

struct EntityManager;

/*
struct CharacterFactory : public NetworkObjectFactory<esv::Character, (uint32_t)ObjectType::ServerCharacter>
{
	void* VMT2;
	void* VMT3;
	Map<FixedString, void*> FSMap_ReloadComponent;
	EntityWorld* Entities;
	uint64_t Unkn8[2];
};

struct ItemFactory : public NetworkObjectFactory<esv::Item, (uint32_t)ObjectType::ServerItem>
{
	void* VMT2;
	void* VMT3;
	Map<FixedString, void*> FSMap_ReloadComponent;
	EntityWorld* Entities;
	uint64_t Unkn8[2];
};

struct InventoryFactory : public NetworkObjectFactory<esv::Inventory, (uint32_t)ObjectType::ServerInventory>
{
	// TODO
};*/

struct GameStateMachine : public ProtectedGameObject<GameStateMachine>
{
	uint8_t Unknown;
	void * CurrentState;
	GameState State;
	void ** TargetStates;
	uint32_t TargetStateBufSize;
	uint32_t NumTargetStates;
	uint32_t ReadStateIdx;
	uint32_t WriteStateIdx;
};

struct EoCServer : public ProtectedGameObject<EoCServer>
{
	char field_0;
	__int64 field_8;
	__int64 field_10;
	__int64 field_18;
	__int64 field_20;
	__int64 field_28;
	__int64 field_30;
	__int64 field_38;
	__int64 field_40;
	__int64 field_48;
	__int64 field_50;
	__int64 field_58;
	__int64 field_60;
	__int64 field_68;
	ScratchString field_70;
	FixedString field_90;
	FixedString field_94;
	FixedString field_98;
	FixedString field_9C;
	GameStateMachine* GameStateMachine;
	__int64 GameServer;
	__int64 field_B0;
	int Rand1;
	int Rand2;
	__int64 GlobalRandom;
	__int64 ConditionManager;
	ModManager ModManager;
	char field_280;
	ecs::EntityWorld* EntityWorld;
	__int64 field_290;
	__int64 LobbyManager;
	char field_2A0;
	__int64 field_2A8;
	__int64 field_2B0;
};

typedef void(*GameStateEventManager__ExecuteGameStateChangedEvent)(void* self, GameState fromState, GameState toState);
typedef void(*GameStateThreaded__GameStateWorker__DoWork)(void* self);
typedef void(*GameStateMachine__Update)(void* self, GameTime* time);

END_NS()


BEGIN_NS(ecl)

struct GameStateMachine : public ProtectedGameObject<GameStateMachine>
{
	void* CurrentState;
	GameState State;
};
		
// Also update SimplifiedEoCClient in BG3Updater when changing this!
struct EoCClient : public ProtectedGameObject<EoCClient>
{
	using HandleErrorProc = void (void* self, TranslatedString const& message, bool exitGame, TranslatedString const& a4);

	__int64 VMT;
	__int64 VMT2;
	__int64 field_10;
	__int64 VMT3;
	__int64 field_20;
	__int64 VMT4;
	__int64 VMT5;
	__int64 field_38;
	Array<void*> field_40;
	__int64 field_50;
	Array<void*> field_58;
	__int64 field_68;
	Array<void*> field_70;
	__int64 field_80;
	GameStateMachine* GameStateMachine;
	void* GameClient;
	void* field_98;
	__int64 field_A0;
	__int64 field_A8;
	__int64 field_B0;
	FixedString FS1;
	FixedString LevelName;
	FixedString SomeGUID;
	FixedString FS_CurrentSaveGameGUID;
	char IsLoading;
	char IsLoading2;
	char field_CA;
	__int64 field_D0;
	__int64 field_D8;
	char field_E0;
	__int64 field_E8;
	__int64 field_F0;
	char field_F8;
	ScratchString field_100;
	ModManager* ModManager;
	__int64 field_128;
	TranslatedString LastError;
	int field_140;
	int field_144;
	int field_148;
	ecs::EntityWorld* EntityWorld;
	__int64 EntityManager_M;
	__int64 field_160;
	int field_168;
	int field_16C;
	// etc.
};

typedef void(*GameStateEventManager__ExecuteGameStateChangedEvent)(void* self, GameState fromState, GameState toState);
typedef void(*GameStateThreaded__GameStateWorker__DoWork)(void* self);
typedef void(*GameStateMachine__Update)(void* self, GameTime* time);

END_NS()
