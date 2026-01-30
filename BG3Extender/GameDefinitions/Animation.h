#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_NS(gn)

struct GenomeVariant;

using SequenceIndex = uint16_t;
using NodeIndex = uint16_t;
using VariableIndex = uint16_t;
using EGenomeExecutionResult = uint32_t;

struct [[bg3::hidden]] GenomeVarTypeDesc : public ProtectedGameObject<GenomeVarTypeDesc>
{
    using AssignProc = void (GenomeVariant* variant, GenomeVariant* other);
    using AssignFromRawValueProc = void (GenomeVariant* this_, void* data);
    using VisitProc = bool (ObjectVisitor* visitor, FixedString const& node, GenomeVariant* variant);
    using FromStringProc = void (GenomeVariant* variant, char const* str);
    using ToStringProc = void (GenomeVariant* variant, STDString* out);
    using DestroyProc = void (GenomeVariant* variant);
    using ComparatorProc = void (void* this_, void* other);

    AssignProc* Assign;
    AssignFromRawValueProc* AssignFromRawValue;
    VisitProc* Visit;
    FromStringProc* FromString;
    ToStringProc* ToString;
    DestroyProc* Destroy;
    ComparatorProc* Comparator;

    uint32_t TypeHash;
    uint32_t TypeHash2;
    uint64_t Size;
    uint64_t Alignment;
    FixedString TypeName;
    bool Trivial;
    GenomeVariant* DefaultValue;
};

struct GenomeEventTypeIndex
{
    uint8_t Type;
    uint16_t Index;
};

struct [[bg3::hidden]] GenomeVariant
{
    void* Value;
    GenomeVarTypeDesc* Type;
#if 0
    // Only present in editor build
    // bool Assigned;
    // bool Constant;
#endif

    FixedString GetTypeName() const;
    UserReturn LuaGetValue(lua_State* L) const;
};

template <class T>
struct [[bg3::hidden]] TGenomeSet
{
    void* VMT;
    bool IsOwned;
    Array<T>* Values;
};

struct GenomeEventArgs
{
    virtual ~GenomeEventArgs();
};

struct GenomeParametrizedEventArgs : public GenomeEventArgs
{
    virtual ~GenomeParametrizedEventArgs();

    std::array<GenomeVariant, 22> Args;
};

// Unmapped compiler internals
class GenomeGlobalCompilationContext;
class GenomeSequenceCompilationContext;
class GenomeCompilerSequence;
class GenomeCompilerNode;
class GenomeCompilerLink;
class GenomeCompilerSocket;
class GenomeCompilerFunctionDataSocket;
class GenomeNodeSocketPrototype;
class GenomeVisitor;
class GenomeForceTime;
class GenomeDebugNode;
class GenomeDebugSequence;
class GenomeInstanceDebugContext;
struct GenomeExecutionContext;

struct GenomeSequenceTemplate : public ProtectedGameObject<GenomeSequenceTemplate>
{
    virtual ~GenomeSequenceTemplate() = 0;
    virtual FixedString GetTypeID() const = 0;
    virtual bool PreprocessSequence(GenomeSequenceCompilationContext&, GenomeCompilerSequence const&) = 0;
    virtual void SetIndex(SequenceIndex const&) = 0;
    virtual void RegisterNodeForEvent(NodeIndex const&, FixedString const& event) = 0;
    virtual void RegisterSequenceForEvent(SequenceIndex const&, FixedString const&) = 0;
    virtual bool Visit(GenomeVisitor*) = 0;

    [[bg3::readonly]] SequenceIndex Index;
    Array<VariableIndex> InputVariables;
    Array<VariableIndex> OutputVariables;
    LegacyRefMap<FixedString, Array<NodeIndex>> NodeEvents;
};

struct GenomeNodeTemplate : public ProtectedGameObject<GenomeNodeTemplate>
{
    virtual ~GenomeNodeTemplate() = 0;
    virtual void Create(NodeIndex const&) = 0;
    virtual bool PreprocessNode(GenomeSequenceCompilationContext&, GenomeCompilerNode const&) = 0;
    virtual bool DependencyResolve(GenomeSequenceCompilationContext&, GenomeCompilerNode const&) = 0;
    virtual bool FlowResolve(GenomeSequenceCompilationContext&, GenomeCompilerNode const&, GenomeCompilerLink const*) = 0;
    virtual FixedString const& GetTypeID() const = 0;
    virtual bool IsEntryPoint(GenomeSequenceCompilationContext const&) const = 0;
    virtual bool ShouldResetOnParentReset() const = 0;
    virtual VariableIndex const* GetInputSocketVariables(int, int*) const = 0;
    virtual VariableIndex const* GetOutputSocketVariables(int, int*) const = 0;
    virtual GenomeNodeSocketPrototype const* GetInputSocketPrototypeTypes(uint64_t&) const = 0;
    virtual GenomeNodeSocketPrototype const* GetOutputSocketPrototypeTypes(uint64_t&) const = 0;
    virtual bool Visit(GenomeVisitor*) = 0;
    virtual uint64_t GetProcessType()const = 0;
    virtual uint64_t GetEntryTrigger(GenomeSequenceCompilationContext const&) const = 0;
    virtual void AddSocketLinkDependencies(GenomeSequenceCompilationContext&, GenomeCompilerNode const&, GenomeCompilerSocket const&, bool) = 0;
    virtual bool CreateAndLinkOutputSockets(GenomeSequenceCompilationContext&, GenomeCompilerNode const&) = 0;
    virtual bool CreateAndLinkInputSockets(GenomeSequenceCompilationContext&, GenomeCompilerNode const&) = 0;
    virtual bool ValidateAnyTypeSocket(GenomeSequenceCompilationContext&, GenomeCompilerNode const&, GenomeCompilerFunctionDataSocket const&, bool) = 0;
    virtual VariableIndex& GetSocketVariable(VariableIndex&, GenomeSequenceCompilationContext&, GenomeCompilerNode const&, GenomeCompilerFunctionDataSocket const&, int, bool) = 0;
    virtual VariableIndex& GetOutputSocketPassthrough(VariableIndex&, GenomeSequenceCompilationContext&, GenomeCompilerNode const&, GenomeCompilerFunctionDataSocket const&, int) = 0;
    virtual bool ResolveDataInputs(GenomeSequenceCompilationContext&, GenomeCompilerNode const&) = 0;
    virtual bool HandleResolveDataInput(GenomeSequenceCompilationContext&, FixedString const&, CompactSet<GenomeCompilerSocket*> const&) = 0;
    virtual bool HandleResolveDataSocket(GenomeSequenceCompilationContext&, FixedString const&, GenomeCompilerSocket const*, uint64_t) = 0;
    virtual bool ResolveFlow(GenomeSequenceCompilationContext&, GenomeCompilerNode const&) = 0;
    virtual bool HandleResolveFlow(GenomeSequenceCompilationContext&, FixedString const&, CompactSet<GenomeCompilerSocket*> const&) = 0;

    [[bg3::readonly]] NodeIndex Index;
};

struct GenomeVariableTemplate : public ProtectedGameObject<GenomeVariableTemplate>
{
    [[bg3::readonly]] VariableIndex Index;
    [[bg3::readonly]] uint8_t Type;
    GenomeVariant Value;
    Guid ID;
    STDString Name;
};

struct GenomeExternalGroupTemplate : public ProtectedGameObject<GenomeExternalGroupTemplate>
{
    Array<VariableIndex> Variables;
};

struct GenomeBlueprint : public ProtectedGameObject<GenomeBlueprint>
{
    Array<GenomeSequenceTemplate*> Sequences;
    Array<GenomeNodeTemplate*> Nodes;
    Array<GenomeVariableTemplate*> Variables;
    Array<GenomeVariableTemplate*> StackVariables;
    LegacyRefMap<FixedString, GenomeExternalGroupTemplate*> ExternalGroups;
    [[bg3::hidden]] void* GenomePluginDataWrapper;
    LegacyRefMap<FixedString, GenomeEventTypeIndex> Events;
    LegacyRefMap<Guid, VariableIndex> VariableIndices;
    // Editor only
#if 0
    // [[bg3::hidden]] void* DebugContext;
    // STDString Path;
    // [[bg3::hidden]] __int64 field_98;
#endif
    [[bg3::hidden]] GenomeManager* GenomeManager;
    Version Version;
    bool IsValid;
    Array<Array<GenomeVariant>*> Stacks;
    [[bg3::hidden]] CRITICAL_SECTION StackLock;
};

struct GenomeSequence : public ProtectedGameObject<GenomeSequence>
{
    [[bg3::hidden]] void* VMT;
    GenomeSequenceTemplate* Template;
    int Clock;
};

struct GenomeNode : public ProtectedGameObject<GenomeNode>
{
    virtual ~GenomeNode() = 0;
    virtual void OnGenomeEvent(GenomeExecutionContext&, FixedString const& evt, GenomeEventArgs const& args) = 0;
    virtual void Reset(GenomeExecutionContext&) = 0;
    virtual void Create(GenomeBlueprintInstance*, GenomeNodeTemplate const*) = 0;
    virtual void PreExecute(GenomeExecutionContext&) = 0;
    virtual EGenomeExecutionResult Execute(GenomeExecutionContext&) = 0;
    virtual void ForceClock(GenomeExecutionContext&, GenomeForceTime const&) = 0;
};

// Unmapped
struct GenomePluginDataDesc {};

struct GenomeBlueprintInstance : public ProtectedGameObject<GenomeBlueprintInstance>
{
    [[bg3::hidden]] Array<GenomePluginDataDesc*>* PluginData;
    [[bg3::hidden]] void* Listener;
    Array<GenomeSequence*> Sequences;
    Array<GenomeNode*> Nodes;
    Array<GenomeVariant> VariablesForEdit;
    GenomeBlueprint* Parent;
    [[bg3::hidden]] GenomeManager* GenomeManager;
    [[bg3::hidden]] void* GenomeInstanceVariableOverrider;
    ecs::EntityRef Entity;
    uint32_t CreationFlags;
    // Editor only
    // [[bg3::hidden]] void* DebugContext;
    // [[bg3::hidden]] bool DumpStateOnNextRun;
};

struct BlueprintInstanceData : public ProtectedGameObject<BlueprintInstanceData>
{
    GenomeBlueprint* Blueprint;
    [[bg3::readonly]] uint32_t Flags;
    [[bg3::readonly]] uint64_t ThreadEvent;
};

struct [[bg3::hidden]] GenomeNodeTypeDesc : public ProtectedGameObject<GenomeNodeTypeDesc>
{
    using CreateNodeTemplateProc = GenomeNodeTemplate* ();
    using CreateNodeProc = GenomeNode* ();
    using CreateCompilerNodeProc = GenomeCompilerNode* ();
    using NodeFunctionProc = uint64_t (GenomeExecutionContext&);
    using CreateDebugNodeProc = GenomeDebugNode* ();
    using GetSocketProc = GenomeNodeSocketPrototype* (uint64_t);

    CreateNodeTemplateProc* CreateNodeTemplate;
    CreateNodeProc* CreateNode;
    CreateCompilerNodeProc* CreateCompilerNode;
    uint32_t TypeHash;
    CreateDebugNodeProc* CreateDebugNode;
    GetSocketProc* GetInputSockets;
    GetSocketProc* GetOutputSockets;
};

struct [[bg3::hidden]] GenomeSequenceTypeDesc : public ProtectedGameObject<GenomeSequenceTypeDesc>
{
    using CreateTemplateProc = GenomeSequenceTemplate* ();
    using CreateInstanceProc = GenomeSequence* ();
    using CreateCompilationContextProc = GenomeSequenceCompilationContext* (GenomeGlobalCompilationContext*, GenomeSequenceTemplate*);
    using CreateCompilerSequenceProc = GenomeCompilerSequence* ();
    using CreateDebugSequenceProc = GenomeDebugSequence* ();

    CreateTemplateProc* CreateTemplate;
    CreateInstanceProc* CreateInstance;
    CreateCompilationContextProc* CreateCompilationContext;
    CreateCompilerSequenceProc* CreateCompilerSequence;
    CreateDebugSequenceProc* CreateDebugSequence;
};

struct [[bg3::hidden]] GenomeSocketTypeDesc {
    // FIXME
};
struct [[bg3::hidden]] GenomeLinkTypeDesc {};

struct [[bg3::hidden]] GenomeExternalGroupDesc
{
    Guid* Guid;
    uint32_t Index;
    FixedString Name;
};


struct [[bg3::hidden]] GenomeTypeManager : public ProtectedGameObject<GenomeTypeManager>
{
    LegacyRefMap<uint32_t, GenomeVarTypeDesc*> VarTypesByHash;
    LegacyRefMap<FixedString, GenomeVarTypeDesc> VarTypes;
    LegacyRefMap<uint32_t, GenomeNodeTypeDesc*> NodesById;
    LegacyRefMap<FixedString, GenomeNodeTypeDesc> NodesByName;
    LegacyRefMap<FixedString, GenomeSequenceTypeDesc> Sequences;
    LegacyRefMap<FixedString, GenomeSocketTypeDesc> Sockets;
    LegacyRefMap<FixedString, GenomeLinkTypeDesc> LinkTypes;
    LegacyRefMap<FixedString, GenomeExternalGroupDesc> ExternalGroups;
    [[bg3::hidden]] Array<void*> field_80;
    [[bg3::hidden]] Array<void*> BlueprintPluginDataList;
    [[bg3::hidden]] Array<void*> BlueprintCompilerPluginDataList_Arr_GenomePluginDataDesc;
    [[bg3::hidden]] Array<void*> BlueprintManagerPluginDataList;
    [[bg3::hidden]] Array<void*> CompilerSavePlugins;
    [[bg3::hidden]] Array<void*> BlueprintLoadPlugins;
    [[bg3::hidden]] Array<void*> BlueprintInstanceInitPlugins_Arr_func;
    [[bg3::hidden]] Array<void*> BlueprintInstancePreExePlugins;
    [[bg3::hidden]] Array<void*> BlueprintInstancePostExePlugins;
    [[bg3::hidden]] Array<void*> BlueprintSequencePreExePlugins;
    [[bg3::hidden]] Array<void*> BlueprintSequencePostExePlugins;
    LegacyRefMap<FixedString, uint32_t> VarTypesFromSource;
    LegacyRefMap<FixedString, FixedString> NodeTypesFromSource;
    LegacyRefMap<FixedString, FixedString> SequenceTypesFromSource;
    LegacyRefMap<FixedString, FixedString> SocketTypesFromSource;
    LegacyRefMap<FixedString, FixedString> LinkTypesFromSource;
};

struct [[bg3::hidden]] GenomeVariableRange
{
    uint16_t Start;
    uint16_t End;
};

struct [[bg3::hidden]] GenomeExecutionContext
{
    GameTime GameTime;
    GenomeBlueprintInstance* Instance;
    GenomeSequence* qword20;
    GenomeSequence* qword28;
    NodeIndex NodeIndex;
    GenomeVariableRange* InputRangesStart;
    GenomeVariableRange* InputRangesEnd;
    VariableIndex* InputsStart;
    VariableIndex* InputsEnd;
    GenomeVariableRange* OutputRangesStart;
    GenomeVariableRange* OutputRangesEnd;
    VariableIndex* OutputsStart;
    VariableIndex* OutputsEnd;
    Array<GenomeVariant>* Variables;
    GenomeVariant* StackStart;
    GenomeVariant* StackEnd;
    GenomeInstanceDebugContext* DebugContext;
};

struct [[bg3::hidden]] GenomeManager : public ProtectedGameObject<GenomeManager>
{
    GenomeTypeManager* TypeManager;
    void* PluginData;
    Array<void*> Listeners_pListener;
    CRITICAL_SECTION ListenerLock;
    LegacyRefMap<GenomeBlueprint*, Array<GenomeBlueprintInstance*>> BlueprintInstances;
    CRITICAL_SECTION InstanceLock;
    LegacyRefMap<STDString, BlueprintInstanceData> LoadedBlueprints;
    LegacyRefMap<GenomeBlueprint*, STDString> BlueprintsToPath;
    SRWLOCK BlueprintInstantiationRWLock;
    int LockThreadID;
    int LockWriteEnterCount;
    // Editor only?
    // __int64 field_B0;
    Array<void*> SourceReaderPlugins;
    StringView CompiledPathRoot;
    StringView DebugPathRoot;
    Version CodeVersion;
    Version MinVersion;
    SynchronizedMPMCQueueBounded<GenomeBlueprintInstance*> UnloadInstanceQueue;
    SynchronizedMPMCQueueBounded<GenomeBlueprint*> UnloadQueue;
    void* UnloadBatch;
    void* UnloadBatchJob;
    CRITICAL_SECTION PendingBlueprintsLock;
    Array<std::pair<bool, Path>> PendingBlueprints;
    double SyncedTime;
    double LastSyncTime;
    bool IsMasterTime;
};


END_NS()

BEGIN_SE()

struct TextKeyTypeProperties : public ProtectedGameObject<TextKeyTypeProperties>
{
    virtual ~TextKeyTypeProperties() = 0;
    virtual bool Visit(ObjectVisitor* visitor) = 0;
    virtual TextKeyType GetType() = 0;
    virtual void Init(TextKeyTypeProperties* props) = 0;
    virtual void Prepare(void* prepareData) = 0;
    virtual void Unprepare() = 0;
};

struct TextKeySoundTypeProperties : public TextKeyTypeProperties
{
    uint8_t SoundObjectIndex;
    FixedString Resource;
};

struct TextKeyEffectTypeProperties : public TextKeyTypeProperties
{
    FixedString Effect;
    FixedString Bone;
    FixedString field_10;
    bool HardAttach;
    bool InterruptLoopAtEnd;
    int16_t BoneId;
};

struct TextKeyFootStepTypeProperties : public TextKeyTypeProperties
{
    uint8_t FootID;
    bool Slide;
    bool PlaySound;
    bool PlayScuff;
    bool PlayStepFX;
    bool PlaySlideFX;
    bool PlayHearingFX;
};

struct TextKeyAttachTypeProperties : public TextKeyTypeProperties
{
    FixedString Bone;
    FixedString AttachBone;
    bool Detach;
};

struct TextKeyWeaponEffectTypeProperties : public TextKeyTypeProperties
{
    uint32_t Weapon;
    uint32_t EffectType;
};

struct TextKeyGenomeTypeProperties : public TextKeyTypeProperties
{
    // Empty
};

struct TextKeyAttackTypeProperties : public TextKeyTypeProperties
{
    // Empty
};

struct TextKeyRagdollTypeProperties : public TextKeyTypeProperties
{
    float AngularVelocityModifier;
    float LinearVelocityModifier;
    float StiffnessMultiplier;
    float DampingMultiplier;
    float DurationHardLimit;
    float DurationSoftLimit;
    float HitImpulseThreshold;
    bool HoldLastFrame;
    FixedString AnimationResourceID;
    bool LockLimits;
};

struct TextKeyVisualCullFlagTypeProperties : public TextKeyTypeProperties
{
    uint32_t VisualFlag;
    bool Enabled;
};

struct TextKeyFloatDataTypeProperties : public TextKeyTypeProperties
{
    float Data;
};

struct TextKeyFoleyTypeProperties : public TextKeySoundTypeProperties
{
    uint8_t FoleyType;
    uint8_t FoleyIntensity;
};

struct TextKeyVocalTypeProperties : public TextKeyTypeProperties
{
    uint8_t VocalType;
};

struct TextKeyFootMoveTypeProperties : public TextKeyTypeProperties
{
    uint8_t Data;
};

struct TextKeyReactTypeProperties : public TextKeyTypeProperties
{
    // Empty
};

struct TextKeyEvent
{
    FixedString ID;
    float Time;
    float Length;
    uint8_t Track;
    TextKeyTypeProperties* Properties;
};

struct AnimationReceivedGameplayEvent
{
    EntityHandle Entity;
    FixedString Event;
    gn::GenomeParametrizedEventArgs Args;
};

struct AnimationReceivedTextKeyEvent
{
    EntityHandle Entity;
    TextKeyEvent* Event;
    [[bg3::hidden]] void* Skeleton;
};

struct AnimationReceivedRootMotionEvent
{
    EntityHandle Entity;
    float LocalPose[7];
};

struct [[bg3::hidden]] AnimationBlueprintSystem : public BaseSystem
{
    DEFINE_SYSTEM(AnimationBlueprint, "ls::AnimationBlueprintSystem")

    void* VMT_IBlueprintInstanceListener;
    void* VMT_IGenomeManagerCallback;
    UnknownSignal StopUpdateSignal;
    UnknownSignal AnimationReceivedTextKeyEventSignal;
    UnknownSignal AnimationReceivedGameplayEventSignal;
    UnknownSignal BlueprintCreatedSignal;
    UnknownSignal BlueprintDestroyedSignal;
    HashMap<EntityHandle, void*> AnimationUpdates; /* UpdateJob* */
    Queue<AnimationReceivedGameplayEvent> GameplayEventsToGenome;
    CRITICAL_SECTION GameplayEventsToGenomeLock;
    // Editor only 
    // Queue<AnimationReceivedTextKeyEvent> DebugTextKeyEventsToGenome;
    // CRITICAL_SECTION DebugTextKeyEventsToGenomeLock;
    HashSet<EntityHandle> LoadedBlueprints;
    GameTime Time;
    bool IsInterestedInVisuals;
    void* Batch1;
    void* Batch2;
    void* ThreadTicket;
    Queue<AnimationReceivedRootMotionEvent> RootMotion;
    CRITICAL_SECTION RootMotionLock;
    Queue<AnimationReceivedGameplayEvent> GameplayEventsFromGenome;
    CRITICAL_SECTION GameplayEventsFromGenomeLock;
    Queue<AnimationReceivedTextKeyEvent> TextKeyEventsFromGenome;
    CRITICAL_SECTION TextKeyEventsFromGenomeLock;
    Queue<Visual*> DelayedUpdateAttachments;
    CRITICAL_SECTION DelayedUpdateAttachmentsLock;
    Pool UpdateJobPool;
    void* ThreadDispatcher;
    gn::GenomeManager* GenomeManager;
    int32_t NextInstanceId;
    HashMap<EntityHandle, Array<AnimationInstanceChangeType>> AnimationInstanceChanges;
    Array<EntityHandle> LinkedEntities;
    // Editor only 
    // void* AnimationDebugWorker;
    // void* AnimationDebugThread;
};


END_SE()

BEGIN_NS(lua)

LUA_POLYMORPHIC(TextKeyTypeProperties);

END_NS()
