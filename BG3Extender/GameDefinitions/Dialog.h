#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_NS(dlg)

struct Dialog;
struct DialogNode;
struct DialogManager;
struct DialogInstance;
struct DefaultDialogNode;
struct DialogVisitor;
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
    [[bg3::hidden]] FlagParamInfo* ParamInfo;
};


struct Flag : public ProtectedGameObject<Flag>
{
    virtual ~Flag() = 0;

    Guid Uuid;
    FixedString Type;
    bool Value;
    [[bg3::hidden]] uint32_t _Pad;
};


struct FlagCollection
{
    [[bg3::hidden]] DialogManager* DialogManager;
    LegacyRefMap<FixedString, Array<Flag*>> Flags;
};


struct ScriptFlag
{
    Guid Uuid;
    STDString Script;
    uint64_t field_28;
#if 0
    //STDString Name;
    //STDString Description;
    //STDString Path;
    //STDString ModName;
#endif
};


struct SpeakerGroup
{
    Guid Uuid;
    Guid OverwriteSpeakerUuid;
#if 0
    //STDString SpeakerName;
    //STDString Description;
    //STDString Path;
    //STDString ModName;
#endif
};


struct Variable
{
    Guid Uuid;
    uint8_t Type;
    STDString DefaultValue;
#if 0
    //STDString Name;
    //STDString Description;
    //STDString Path;
    //STDString ModName;
#endif
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


struct TagRuleEntry : public ProtectedGameObject<TagRuleGroup>
{
    virtual ~TagRuleEntry() = 0;
};


struct TagRule : public TagRuleEntry
{
    int SpeakerIndex;
    Array<Guid> CheckedTags;
    int CombineOp;
};


struct TagRuleGroup : public TagRuleEntry
{
    int Operator;
    Array<TagRuleEntry*> ChildRules;
};


struct LookAtInfo
{
    int16_t Speaker;
    int16_t Target;
};


struct DialogGameData : public ProtectedGameObject<DialogGameData>
{
    virtual ~DialogGameData() = 0;
    virtual DialogNode* GetParentNode() const = 0;
    virtual void SetParentNode(DialogNode&) = 0;
    virtual bool OnVisit(ObjectVisitor&) = 0;
    virtual void SerializeToBuffer(ScratchBuffer&) = 0;
    virtual void DeserializeFromBuffer(ScratchBuffer&) = 0;
    virtual void Unknown0() = 0;
    virtual void Unknown1() = 0;
    virtual bool ValidateNode(DialogInstance const&) = 0;
    virtual bool OnNodeStarted(DialogInstance&) = 0;
    virtual TranslatedFSString GetCustomDisplayText(DialogInstance const&, FixedString const&, unsigned int, TranslatedFSString&&) const = 0;
    virtual void* GetOverridenTag(DialogInstance const&, FixedString const&, unsigned int) const = 0;
    virtual void Clone(DialogGameData const*) = 0;

    DialogNode* ParentNode;
    Array<FixedString> AiPersonality;
    Array<LookAtInfo> LookAt;
    STDString SoundEvent;
    STDString SoundPerMusicInstrument[4];
    FixedString OriginTheme;
    FixedString OriginOnly;
    bool OriginThemeAddInstrument;
    STDString CustomMovie;
    [[bg3::hidden]] IGameInterface* GameInterface;
    int CameraTarget;
    int ExtraWaitTime;
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
    [[bg3::hidden]] DialogManager* DialogManager;
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
    virtual DialogGameData* GetGameData() const = 0;
    virtual void SetGameData(DialogGameData*) = 0;
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
    [[bg3::hidden]] void* GameData;
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
    int32_t Speaker;
    int32_t AddressedSpeaker;
#if 0
    // STDString Value;
#endif
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
    [[bg3::hidden]] void* InputNodeVMT;
    int SpeakerId2;
    std::optional<FlagCollection> ValidatedFlags;
};


struct DialogRoll : public ProtectedGameObject<DialogRoll>
{
    [[bg3::hidden]] void* VMT;
    stats::RollType RollType;
    SkillId Skill;
    AbilityId Ability;
    uint8_t Advantage;
    Guid DifficultyClass;
    int RollTargetSpeaker;
    bool ExcludeCompanionsOptionalBonuses;
    bool ExcludeSpeakerOptionalBonuses;
    Array<Guid> ExcludedBonusesTags;
};


struct ActiveRollNode : public TaggedQuestionNode
{
    DialogRoll Roll;
    bool DetectThoughts;
    TranslatedString AdvantageReason;
    uint8_t Flags2;
    STDString AdvantageReasonReferenceString;
};


struct PassiveRollNode : public DefaultDialogNode
{
    DialogRoll Roll;
    bool IsFlatSkillCheck;
};


struct TradeNode : public DefaultDialogNode
{
    STDString ItemsTagFilter;
    int PlayerOverride;
    bool DefaultToBarter;
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

struct EmptyTarget
{
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
    [[bg3::hidden]] void* CustomNodeVisitor;
    [[bg3::hidden]] void* CustomNodeVisitData;
    Dialog* ActiveDialog;
    FixedString DialogResourceUUID;
    Dialog* OverriddenDialog;
    HashMap<FixedString, Dialog*> Dialogs;
    int32_t State;
    std::variant<EmptyTarget, Guid, FixedString> PartyTeleportOnEnd;
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
    int8_t LocalHighlightedAnswer;
    int8_t HostHighlightedAnswer;
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
    [[bg3::hidden]] Array<DialogEventListener*> EventListeners;
    Array<int32_t> PendingInstanceLoads;
    LegacyRefMap<int, DialogInstance*> Dialogs;
    LegacyRefMap<FixedString, FlagDescription*> FlagDescriptions;
    [[bg3::hidden]] LegacyRefMap<FixedString, INodeConstructor*> NodeConstructorMap;
    [[bg3::hidden]] void* NodeLogic;
    uint8_t field_58;
    [[bg3::hidden]] IGameInterface* GameInterface;
    int NextInstanceId;
    [[bg3::hidden]] Array<INodeConstructor*> NodeConstructors;
    Array<Dialog*> CachedDialogs;
    LegacyRefMap<EntityHandle, ActorRefCount> ActorRefCounts;
    [[bg3::hidden]] FlagParamInfo* TagFlagParamInfo;
    [[bg3::hidden]] void* KeywordParser;
    [[bg3::hidden]] void* KeywordParseCallback;
    LegacyRefMap<Guid, ScriptFlag> ScriptFlags;
    LegacyRefMap<Guid, Variable> DialogVariables;
    LegacyRefMap<Guid, SpeakerGroup> SpeakerGroups;
    LegacyRefMap<int, Guid> DialogToVariable;
    Array<int32_t> field_F8;
    [[bg3::hidden]] CRITICAL_SECTION CriticalSection;
    [[bg3::hidden]] ModManager* ModManager;
};



struct [[bg3::hidden]] SharedDialogGameInterface : public ProtectedGameObject<SharedDialogGameInterface>
{
    using DialogPreloadCallbackProc = void (DialogInstance&, DialogManager&, bool);

    virtual ~SharedDialogGameInterface();
    virtual void fun00();
    virtual void fun01();
    virtual DialogGameData* CreateGameData();
    virtual DialogGameData* CloneGameData(DialogGameData const*);
    virtual void DestroyGameData(DialogGameData*);
    virtual void PreLoadDialog(DialogInstance&, DialogPreloadCallbackProc*, bool);
    virtual void fun02();
    virtual void OnManagerShutdown(DialogManager*);
    virtual void OnManagerStart(DialogManager*);
    virtual float GetDefaultNodeWaitTime() const;
    virtual void fun03();

    uint64_t DialogIOLoadRequests_MPSCQueue[27];
    uint64_t DialogIOLoadRequests_MPSCQueue2[27];
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

BEGIN_NS(esv)


struct [[bg3::hidden]] DialogNodeConstructor : public ProtectedGameObject<DialogNodeConstructor>
{
    void* VMT;
    FixedString Name;
};

struct [[bg3::hidden]] DialogGameInterface : public dlg::SharedDialogGameInterface
{
    __int64 field_248;
    void* FlagParamInfo;
    dlg::FlagDescription GlobalFlag;
    dlg::FlagDescription ObjectFlag;
    dlg::FlagDescription PartyFlag;
    dlg::FlagDescription DialogFlag;
    dlg::FlagDescription UserFlag;
    dlg::FlagDescription UserShapeFlag;
    dlg::FlagDescription QuestFlag;
    dlg::FlagDescription DialogInstanceFlag;
    DialogNodeConstructor PopNodeConstructor;
    DialogNodeConstructor ActiveRollNodeConstructor;
    DialogNodeConstructor PassiveRollNodeConstructor;
    DialogNodeConstructor RollResultNodeConstructor;
    DialogNodeConstructor TradeNodeConstructor;
    HashMap<EntityHandle, void*> field_328_EH_pVariableManager;
};


struct [[bg3::hidden]] DialogSystem : public ProtectedGameObject<DialogSystem>
{
    static constexpr auto SystemType = ExtSystemType::ServerDialogSystem;

    void* VMT;
    void* field_8;
    void* TimelineSystem;
    void* CombatLogSystem;
    void* RollSystem;
    void* DialogEventsSystem;
    DialogGameInterface GameInterface;
    void* DialogEventListener1;
    void* DialogEventListenerAdapter2;
    void* DialogEventListenerAdapter;
    void* RequestedRollCallbacks;
    void* field_3B8_arr_TradeInstance;
    ScratchString Scratch;
};

END_NS()

BEGIN_NS(lua)

LUA_POLYMORPHIC(dlg::DialogNode);

END_NS()
