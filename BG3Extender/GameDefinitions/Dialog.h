#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_NS(dlg)

struct Dialog;
struct DialogNode;
struct DialogManager;
struct DialogInstance;
struct DefaultDialogNode;
struct DialogVisitor;
struct IGameData;
struct IInputNode;
struct DialogueVisualTag;
struct Flag;
struct TagRuleGroup;
struct DialogEventListener;
struct IGameInterface;
struct INodeConstructor;
struct FlagParamInfo;


struct FlagDescription
{
	FixedString Name;
	bool HasFlagParamInfo;
	FlagParamInfo* ParamInfo;
};


struct FlagCollection
{
	Dialog* Dialog;
	LegacyRefMap<FixedString, Array<Flag*>> Flags;
};


struct ScriptFlag
{
	Guid Uuid;
	STDString Script;
	uint64_t field_28;
	STDString Name;
	STDString Description;
	STDString Path;
	STDString ModName;
};


struct SpeakerGroup
{
	Guid Uuid;
	Guid OverwriteSpeakerUuid;
	STDString SpeakerName;
	STDString Description;
	STDString Path;
	STDString ModName;
};


struct Variable
{
	Guid Uuid;
	uint8_t Type;
	STDString DefaultValue;
	STDString Name;
	STDString Description;
	STDString Path;
	STDString ModName;
};


struct TextLine
{
	Guid LineId;
	Guid CustomSequenceId;
	TranslatedFSString TagText;
	TranslatedFSString OldText;
	Array<FixedString> DialogVariables;
	bool HasOverrideText;
	STDString Content;
	uint8_t field_70;
	bool Stub;
	TranslatedFSString ActualText;
};


struct TaggedText
{
	TagRuleGroup* RootRule;
	Array<TextLine> Lines;
};


struct NodeData
{
	FixedString UUID;
	int field_4;
	int field_8;
	int field_C;
	Guid field_10;
};


struct Dialog : public ProtectedGameObject<Dialog>
{
	bool Automated;
	bool IsWorld;
	bool IsBehaviour;
	bool IsSFXDialog;
	bool AllowDeadSpeakers;
	bool IsAllowingJoinCombat;
	bool IsSubbedDialog;
	bool IsSubsAnonymous;
	bool IsPrivateDialog;
	LegacyRefMap<FixedString, DialogNode*> Nodes;
	DialogManager* DialogManager;
	STDString Category;
	Array<STDString> SpeakerGroups;
	Array<STDString> SpeakerTags;
	Array<Guid> SpeakerMappingIds;
	HashMap<int, int> DefaultAddressedSpeakers;
	HashSet<int> PeanutSlots;
	Array<DialogNode*> RootNodes;
	STDString FileName;
	FixedString UUID;
	FixedString TimelineId;
	double UnloadedTimeStamp;
	int RefCount;
	int DefaultSpeakerIndex;
	__int64 field_128;
	__int64 field_130;
};



struct DialogNode : public ProtectedGameObject<DialogNode>
{
	virtual ~DialogNode() = 0;
	virtual Guid GetApprovalRatingID() const = 0;
	virtual void SetParentDialog(Dialog*) = 0;
	virtual bool NeedsUserInput() const = 0;
	virtual bool IsValid(DialogInstance const&, DialogNode const*) const = 0;
	virtual bool CanUserSelect(DialogInstance const&, EntityHandle const&) const = 0;
	virtual bool ReferencesNodes() const = 0;
	virtual void GetReferencedNodes(ObjectSet<FixedString>&) const = 0;
	virtual PrimitiveSet<TaggedText*> const* GetTaggedTexts() const = 0;
	virtual bool Visit(DialogVisitor&) = 0;
	virtual IGameData* GetGameData() const = 0;
	virtual void SetGameData(IGameData*) = 0;
	virtual bool IsPresentable() const = 0;
	virtual ObjectSet<Guid> const& GetTags() const = 0;
	virtual void ClearTags() = 0;
	virtual void AddTag(Guid const&) = 0;
	virtual void SerializeToBuffer(ScratchBuffer&) = 0;
	virtual void DeserializeFromBuffer(ScratchBuffer&, DialogManager*) = 0;
	virtual DefaultDialogNode const* ToDefaultDialogNode() const = 0;
	virtual DefaultDialogNode const* ToDefaultDialogNode2() const = 0;
	virtual IInputNode const* ToInputNode() const = 0;
	virtual IInputNode const* ToInputNode2() const = 0;
	virtual bool Start(DialogInstance&) = 0;
	virtual void Resume(DialogInstance&) = 0;
	virtual void OnNodeSelected(DialogInstance&, NodeData&) = 0;
	virtual void OnPresentedForSelection(DialogInstance const&, unsigned int&, bool, EntityHandle) = 0;
	virtual void CollectAllTextLines(ObjectSet<STDString>&) = 0;
	virtual void DoSerializeStateToBuffer(ScratchBuffer&, DialogInstance const&) const = 0;
	virtual void DoDeserializeStateFromBuffer(ScratchBuffer&, DialogInstance&) = 0;
	virtual bool SupportsEmptySpeaker() const = 0;
	virtual bool HasNarratorSpeaker() const = 0;
	virtual int GetSpeakerIndex() const = 0;
	virtual int GetAddressedSpeakerIndex() const = 0;
	virtual TranslatedFSString GetDisplayText(DialogInstance const&, unsigned int, bool, bool) const = 0;
	virtual TextLine* GetTextLine(DialogInstance const&, unsigned int) const = 0;
	virtual PrimitiveSet<TaggedText*> const* GetTaggedTexts2() const = 0;
	virtual Array<DialogueVisualTag> GetDialogueVisualTag(DialogInstance const&, unsigned int) const = 0;
	virtual bool SupportsEmptySpeaker2() const = 0;
	virtual void SetSpeakerIndex(int) = 0;
	virtual void SetAddressedSpeakerIndex(int) = 0;
	virtual bool SupportsEmptySpeaker3() const = 0;


	FixedString UUID;
	FixedString GroupID;
	int GroupIndex;
	FixedString ConstructorID;
	int TransitionMode;
	DialogNodeFlags Flags;
	FlagCollection SetFlags;
	FlagCollection CheckFlags;
	Dialog* ParentDialog;
	void* GameData;
	Array<FixedString> Children;
	float WaitTime;
	Array<Guid> Tags;
	Guid ApprovalRatingID;
};


struct JumpNode : public DialogNode
{
	FixedString JumpTarget;
	int JumpTargetPoint;
};


struct NestedDialogNode : public DialogNode
{
	Guid NestedDialogNodeUUID;
	HashMap<int, int> SpeakerLinking;
};


struct VisualStateNode : public DialogNode
{
};


struct DefaultDialogNode : public DialogNode
{
	FixedString Speaker;
	FixedString AddressedSpeaker;
	STDString Value;
	bool Stub;
};


struct CinematicTaggedNode : public DefaultDialogNode
{
	uint8_t BehaviourNodeType;
};


struct AliasNode : public DefaultDialogNode
{
	FixedString SourceNode;
	bool OverrideApprovalRatingID;
};


struct BaseTaggedNode : public DefaultDialogNode
{
	Array<TaggedText*> TaggedTexts;
};


struct FallibleQuestionResultNode : public DefaultDialogNode
{
	bool Result;
};


struct SelectSpeakerNode : public BaseTaggedNode
{
	int SpeakerId;
};


struct TaggedQuestionNode : public BaseTaggedNode
{
	void* InputNode;
	int SpeakerId2;
	FlagCollection ValidatedFlags;
	bool ValidatedHasValue;
};


struct QueuedActor
{
	EntityHandle field_0;
	int SpeakerId;
	bool field_C;
	bool field_D;
	bool field_E;
	bool field_F;
};


struct VisitedNode
{
	int field_0;
	int field_4;
	__int64 field_8;
	__int64 field_10;
	int field_18;
	int field_1C;
	uint8_t NodePlayed;
	uint8_t field_21;
	uint8_t field_22;
};


struct PlayedNestedDialog
{
	FixedString DialogResourceUuid;
	FixedString NestedDialogResourceUuid;
};


struct NodeSelectionInfo
{
	DialogNode* Node;
	int32_t LineId;
	bool field_C;
};


struct DialogInstance : public ProtectedGameObject<DialogInstance>
{
	int DialogId;
	int TimeAccumulator;
	__int64 field_8;
	Array<FixedString> PopLevels;
	Array<NodeSelectionInfo> NodeSelection;
	Array<EntityHandle> Speakers;
	Array<VisitedNode> VisitedNodes;
	Array<QueuedActor> QueuedActors;
	HashMap<FixedString, uint32_t> NodeCustomData;
	HashSet<FixedString> PlayedDialogs;
	NodeData CurrentNode;
	void* CustomNodeVisitor;
	void* CustomNodeVisitData;
	void* ActiveDialog2;
	FixedString DialogResourceUUID;
	void* OverriddenDialog;
	HashMap<FixedString, Dialog*> Dialogs_FS_pDialog;
	int32_t State;
	int32_t _Pad;
	FixedString PartyTeleportTargetOnEnd;
	int field_164;
	__int64 field_168;
	bool PartyTeleportTargetOnEndType;
	int32_t _Pad2;
	bool WasActivated;
	bool AutomatedDialog;
	bool AllowDeadSpeakers;
	bool IsAllowingJoinCombat;
	bool IsWorld;
	bool IsBehaviour;
	bool IsPrivateDialog;
	bool IsTimelineEnabled;
	uint8_t LoadedSerializedState;
	bool StartPaused;
	bool UnloadRequested;
	bool ForceStopped;
	uint8_t GracefulExitState;
	bool SmoothExit;
	bool WorldHadTriggered;
	bool IsPlayerWatchingTimeline;
	bool TimelineConfirmed;
	bool IsPreview;
	uint8_t LocalHighlightedAnswer;
	uint8_t HostHighlightedAnswer;
	uint8_t Flags;
	int LastPlayerSpeakerIndex;
	bool CanAttack;
	bool UseSpellVocals;
	uint8_t LevelStartingDialog;
	float DialogWaitTime;
	float CullingDistance;
	float UnloadTimeout;
	Guid TimelineSceneTriggerId;
	bool IsPaused;
	bool IsOnlyPlayers;
	bool CachedIsOnlyPlayers;
	Array<PlayedNestedDialog> PlayedNestedDialogs;
	FixedString InitialDialogResourceUUID;
	int CharacterCreationUser;
	Array<HashMap<int, int>> SpeakerLinkings;
	Array<int> UniqueSpeakerLinkingIndices;
	int SpeakerLinkingIndex;
};


struct ActorRefCount
{
	uint16_t field_0;
	uint16_t field_2;
	uint16_t field_4;
	uint16_t field_8;
};


struct DialogManager : public ProtectedGameObject<DialogManager>
{
	Array<DialogEventListener*> EventListeners;
	Array<int32_t> PendingInstanceLoads;
	LegacyRefMap<int, DialogInstance*> Dialogs;
	LegacyRefMap<FixedString, FlagDescription*> FlagDescriptions;
	LegacyRefMap<FixedString, INodeConstructor*> NodeConstructorMap;
	void* NodeLogic;
	uint8_t field_58;
	IGameInterface* GameInterface;
	int NextInstanceId;
	Array<INodeConstructor*> NodeConstructors;
	Array<Dialog*> CachedDialogs;
	LegacyRefMap<EntityHandle, ActorRefCount> ActorRefCounts;
	FlagParamInfo* TagFlagParamInfo;
	void* KeywordParser;
	void* KeywordParseCallback;
	LegacyRefMap<Guid, ScriptFlag> ScriptFlags;
	LegacyRefMap<Guid, Variable> DialogVariables;
	LegacyRefMap<Guid, SpeakerGroup> SpeakerGroups;
	LegacyRefMap<int, Guid> DialogToVariable;
	Array<int32_t> field_F8;
	CRITICAL_SECTION CriticalSection;
	ModManager* ModManager;
};



struct SharedDialogGameInterface : public ProtectedGameObject<SharedDialogGameInterface>
{
	using DialogPreloadCallbackProc = void (DialogInstance&, DialogManager&, bool);

	virtual ~SharedDialogGameInterface() = 0;
	virtual void fun00() = 0;
	virtual void fun01() = 0;
	virtual IGameData* CreateGameData() = 0;
	virtual IGameData* CloneGameData(IGameData const*) = 0;
	virtual void DestroyGameData(IGameData*) = 0;
	virtual void PreLoadDialog(DialogInstance&, DialogPreloadCallbackProc*, bool) = 0;
	virtual void fun02() = 0;
	virtual void OnManagerShutdown(DialogManager*) = 0;
	virtual void OnManagerStart(DialogManager*) = 0;
	virtual float GetDefaultNodeWaitTime() const = 0;
	virtual void fun03() = 0;


	uint8_t DialogIOLoadRequests_MPSCQueue[0xE8];
	uint8_t DialogIOLoadRequests_MPSCQueue2[0xE8];
	Array<void*> DialogIOLoadRequests;
	DialogManager* DialogManager;
	Pool DialogGameData_Pool;
	ScratchString Scratch1;
	Array<void*> Syntax;
	Array<void*> field_220;
	__int64 field_230;
	__int64 field_238;
	__int64 field_240;
};



END_NS()
