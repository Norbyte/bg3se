#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Module.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_SE()

struct IGameObject : public ProtectedGameObject<IGameObject>
{
	virtual ~IGameObject() = 0;
	virtual void HandleTextKeyEvent() = 0;
	virtual uint64_t Ret5() = 0;
	virtual void SetHandle(EntityHandle Handle) = 0;
	virtual void GetHandle(EntityHandle& Handle) const = 0;
	virtual void SetGuid(FixedString const& fs) = 0;
	virtual FixedString* GetGuid() const = 0;
	virtual void SetNetId(NetId netId) = 0;
	virtual void GetNetId(NetId& netId) const = 0;
	virtual void SetCurrentTemplate(void* esvTemplate) = 0;
	virtual void* GetCurrentTemplate() const = 0;
	virtual void SetGlobal(bool isGlobal) = 0;
	virtual bool IsGlobal() const = 0;
	virtual uint32_t GetComponentType() = 0;
	virtual void* GetEntityByHandle(EntityHandle handle) = 0;
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

struct GameStateEventManager
{
	void* VMT;
	Array<void*> Callbacks;
};

struct App
{
	using LoadGraphicSettingsProc = void (App*);
	using CtorProc = App * (App*);
};

END_SE()

BEGIN_NS(esv)

struct EntityManager;

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
	net::GameServer* GameServer;
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

	void* VMT_InputEventListener;
	void* VMT_NetEventListener;
	void* field_10;
	void* VMT_DLCEventListener;
	void* VMT_MessageBoxOwner;
	ecs::ComponentCallbackList ComponentCallbacks;
	ecs::ComponentCallbackList SetControllerModeCallbacks;
	ecs::ComponentCallbackList IncrementAchievementCallbacks;
	ecs::ComponentCallbackList UnlockedAchievementCallbacks;
	//ecs::ComponentCallbackList field_88;
	void* Eoc;
	GameStateMachine* GameStateMachine;
	net::GameClient* GameClient;
	__int64 field_B0;
	void* LobbyLogicManager;
	__int64 field_B8Y;
	void* UIManager;
	__int64 field_B8W;
	FixedString FS1;
	FixedString LoadedLevel;
	FixedString GameId;
	FixedString CurrentGameSession;
	bool IsReturningToMainMenu;
	bool MainMenuEntered;
	bool GameStateInitDone;
	bool IsQuitting;
	Array<int16_t> GameInputDisabled;
	uint8_t GameMode;
	// void* ItemCombinationManager;
	void* CastRequirementsManager;
	bool UsingTempScratchString;
	ScratchString TempScratchString;
	ModManager* ModManager;
	__int64 field_128;
	TranslatedString LastError;
	bool HasErrorMessage;
	Array<void*> field_170;
	Array<int> field_180;
	Array<void*> field_190; // EMessageBoxLocalId
	int BusyMessage;
	int InitializationState;
	int AutoPlayRequestState;
	int field_1AC;
	//void* Console;
	//__int64 field_158;
	ecs::EntityWorld* EntityWorld;
	void* EntityManager;
	void* LobbyDataCache;
	// etc.
};

typedef void(*GameStateEventManager__ExecuteGameStateChangedEvent)(void* self, GameState fromState, GameState toState);
typedef void(*GameStateThreaded__GameStateWorker__DoWork)(void* self);
typedef void(*GameStateMachine__Update)(void* self, GameTime* time);

END_NS()
