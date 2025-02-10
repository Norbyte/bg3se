#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_NS(gn)

struct GenomeVariant;

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

struct GenomeParametrizedEventArgs
{
    virtual ~GenomeParametrizedEventArgs();

    std::array<GenomeVariant, 22> Args;
};

struct GenomeSequenceTemplate : public ProtectedGameObject<GenomeSequenceTemplate>
{
    [[bg3::hidden]] void* VMT;
    [[bg3::readonly]] uint16_t Index;
    Array<uint16_t> InputVariables;
    Array<uint16_t> OutputVariables;
    LegacyRefMap<FixedString, Array<uint16_t>> NodeEvents;
};

struct GenomeNodeTemplate : public ProtectedGameObject<GenomeNodeTemplate>
{
    [[bg3::hidden]] void* VMT;
    [[bg3::readonly]] uint16_t Index;
};

struct GenomeVariableTemplate : public ProtectedGameObject<GenomeVariableTemplate>
{
    [[bg3::readonly]] uint16_t Index;
    [[bg3::readonly]] uint8_t Type;
    GenomeVariant Value;
    Guid ID;
    STDString Name;
};

struct GenomeExternalGroupTemplate : public ProtectedGameObject<GenomeExternalGroupTemplate>
{
    Array<uint16_t> Variables;
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
    LegacyRefMap<Guid, int16_t> VariableIndices;
    [[bg3::hidden]] void* DebugContext;
    STDString Path;
    [[bg3::hidden]] __int64 field_98;
    [[bg3::hidden]] GenomeManager* GenomeManager;
    Version Version;
    bool IsValid;
    Array<GenomeVariant> Stack;
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
    [[bg3::hidden]] void* VMT;
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

struct BlueprintInstanceData
{
    GenomeBlueprint* Blueprint;
    [[bg3::readonly]] uint32_t Flags;
    [[bg3::readonly]] uint64_t ThreadEvent;
};

// Unmapped
struct GenomeNodeTypeDesc {};
struct GenomeSequenceTypeDesc {};
struct GenomeSocketTypeDesc {};
struct GenomeLinkTypeDesc {};
struct GenomeExternalGroupDesc {};

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
    SRWLOCK SRWLock;
    int LockThreadID;
    int LoaderEnterCount;
    __int64 field_B0;
    Array<void*> SourceReaderPlugins;
    StringView CompiledPathRoot;
    StringView DebugPathRoot;
    Version CodeVersion;
    Version MinVersion;
    __int64 field_F8;
    SRWLOCK SRWLock2;
    uint64_t field_108[33];
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

struct [[bg3::hidden]] AnimationBlueprintSystem : public ProtectedGameObject<AnimationBlueprintSystem>
{
    static constexpr auto SystemType = ExtSystemType::AnimationBlueprint;

    void* VMT;
    ecs::EntityWorld* EntityWorld;
    void* VMT_IBlueprintInstanceListener;
    void* VMT_IGenomeManagerCallback;
    UnknownSignal StopUpdateSignal;
    UnknownSignal AnimationReceivedTextKeyEventSignal;
    UnknownSignal AnimationReceivedGameplayEventSignal;
    UnknownSignal BlueprintCreatedSignal;
    UnknownSignal BlueprintDestroyedSignal;
    HashMap<EntityHandle, void*> AnimationUpdates;
    Queue<AnimationReceivedGameplayEvent> GameplayEventsToGenome;
    CRITICAL_SECTION GameplayEventsToGenomeLock;
    Queue<AnimationReceivedTextKeyEvent> TextKeyEventsToGenome;
    CRITICAL_SECTION TextKeyEventsToGenomeLock;
    __int64 field_158;
    __int64 field_160;
    Array<void*> field_168;
    Array<void*> field_178;
    GameTime Time;
    bool IsInterestedInVisuals;
    void* Batch1;
    void* Batch2;
    void* ThreadTicket;
    Queue<void*> RootMotion;
    CRITICAL_SECTION RootMotionLock;
    Queue<AnimationReceivedGameplayEvent> GenomeGameplayEvents;
    CRITICAL_SECTION GenomeGameplayEventsLock;
    Queue<AnimationReceivedTextKeyEvent> GenomeTextKeyEvents;
    CRITICAL_SECTION GenomeTextKeyEventsLock;
    Pool Pool1;
    void* ThreadDispatcher;
    gn::GenomeManager* GenomeManager;
    int32_t NextInstanceId;
    HashMap<EntityHandle, Array<uint8_t>> AnimationInstanceChanges;
    __int64 field_2E8;
    __int64 field_2F0;
    void* AnimationDebugWorker;
};


END_SE()

BEGIN_NS(lua)

LUA_POLYMORPHIC(TextKeyTypeProperties);

END_NS()
