#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/GameState.h>

BEGIN_SE()

struct CharacterTemplate;

struct PlayerCustomData : public ProtectedGameObject<PlayerCustomData>
{
    void* VMT;
    bool Initialized;
    FixedString OwnerProfileID;
    FixedString ReservedProfileID;
};

using PlayerId = int16_t;
using InputPlayerIndex = uint32_t;

/*struct PlayerManager
{
    struct PlayerInfo
    {
        uint32_t PlayerId;
        void* RenderView;
    };

    void * VMT;
    uint16_t NextPlayerId;
    ObjectSet<PlayerId> FreePlayerIds;
    ObjectSet<PlayerId> PlayerList;
    LegacyRefMap<PlayerId, PlayerInfo> PlayerIds;
    LegacyRefMap<InputPlayerIndex, PlayerId> InputPlayerIndices;
    ObjectSet<void *> RenderViews; // rf::RenderView*
};*/

END_SE()

BEGIN_NS(esv)

struct Status;
struct StatusMachine;

struct PlayerData : public ProtectedGameObject<PlayerData>
{
    EntityHandle PlayerHandle;
    HashMap<EntityHandle, void*> ShapeShiftVariableManager;
    FixedString QuestSelected;
    int field_4C;
    PlayerCustomData CustomData;
    EntityHandle field_68;
    Array<glm::vec3> PreviousPositions;
    int PreviousPositionId;
    uint8_t HelmetOption;
    int Renown;
    uint8_t CachedTension;
    bool IsInDangerZone;
    FixedString Region;
    bool field_94;
};

struct Character : public BaseProxyComponent
{
    DEFINE_COMPONENT(ServerCharacter, "esv::Character")

    Status* GetStatus(FixedString statusId);
    Status* GetStatusByType(StatusType type);

    void* VMT;
    void* VMT2;
    EntityHandle field_10;
    CharacterFlags Flags;
    ecs::EntityRef MyHandle;
    FixedString Level;
    FixedString VisualResource;
    Array<PeerId> UpdatePeerIds;
    Array<SurfacePathInfluence> SurfacePathInfluences;
    Array<void*> field_58;
    Array<EntityHandle> Summons;
    Array<FixedString> CreatedTemplateItems;
    Array<FixedString> Treasures;
    Array<FixedString> DisabledCrime;
    Array<Guid> PreferredAiTargets;
    Array<FixedString> ServerControl;
    CharacterTemplate* Template;
    CharacterTemplate* OriginalTemplate;
    CharacterTemplate* TemplateUsedForSpells;
    ActionMachine* AiActionMachine;
    BehaviourMachine* AiBehaviourMachine;
    MovementMachine* AiMovementMachine;
    SteeringMachine* AiSteeringMachine;
    void* AiSupervisor;
    void* DialogController;
    void* FallbackController;
    void* NetworkController;
    TaskController* OsirisController;
    void* GameplayController;
    StatusMachine* StatusManager;
    void* VariableManager;
    void* ShapeShiftingVariableManager;
    void* CharacterMover;
    PlayerData* PlayerData;
    EntityHandle Inventory;
    EntityHandle OwnerCharacter;
    EntityHandle FollowCharacter;
    EntityHandle EnemyCharacter;
    int Dialog;
    FixedString CustomTradeTreasure;
    FixedString BaseVisual;
    UserId UserID;
    UserId UserID2;
    float GeneralSpeedMultiplier;
    int32_t CrimeHandle;
    int32_t PreviousCrimeHandle;
    float InvestigationTimer;
    uint8_t CrimeState;
    uint8_t PreviousCrimeState;
    uint8_t BlockNewDisturbanceReactions;
    uint8_t field_173;
    uint8_t HasOsirisDialog;
    uint8_t NeedsUpdate;
    uint8_t ForceSynch;
    uint8_t NumConsumables;
    CharacterFlags2 Flags2;
    CharacterFlags3 Flags3;
    [[bg3::hidden]] void* _PAD;

    RefReturn<Character> LuaGetSelf() const;
};

END_NS()


BEGIN_NS(ecl)

struct StatusMachine;

struct PlayerCustomData : public ProtectedGameObject<PlayerCustomData>
{
    uint8_t field_0;
    __int64 field_8;
    bg3se::PlayerCustomData Base;
    uint8_t field_28;
    int field_2C;
    NetId PickpocketTarget;
    NetId LootTarget;
    uint8_t field_40;
    uint8_t ArmorOptionState;
    uint8_t CachedTension;
    FixedString QuestSelected;
    Array<FixedString> MemorizedSpells;
    char field_58;
    FixedString field_5C;
};

struct Character : public BaseProxyComponent
{
    DEFINE_COMPONENT(ClientCharacter, "ecl::Character")

    void* VMT;
    void* VMT2;
    EntityHandle Entity;

    CharacterFlags Flags;
    FixedString Level;
    ecs::EntityRef SelfEntity;

    Array<SurfacePathInfluence> SurfacePathInfluences;
    NetId DialogRequest;
    NetId ListenRequest;
    CharacterTemplate* Template;
    CharacterTemplate* OriginalTemplate;
    void* AiAction;
    void* AiBehaviour;
    void* AiMovement;
    void* AiSteering;
    void* AiSupervisor;
    void* DialogController;
    void* FallbackController;
    InputController* InputController;
    void* NetworkController;
    void* GameplayController;
    StatusMachine* StatusManager;
    void* CharacterMover;
    PlayerCustomData* PlayerData;
    Visual* ClothVisual;
    void* PhysicsTemplate;
    uint64_t VertexColorMaskTextureID;
    EntityHandle Owner;
    EntityHandle InUseByCharacter;
    Array<void*> FXCircles_Arr_EntityRef;
    EntityHandle ObscurementIndicationEffect;
    EntityHandle FollowCharacter;
    EntityHandle DialogEffect;
    EntityHandle Light;
    UserId OwnerUserID;
    UserId ReservedUserID;
    Array<void*> CircleColors_Arr_EntityRef;
    float LastTick;
    float SneakCheck;
    float SpeedMultiplier;
    int DialogData;
    __int16 PhysicsFlags_M;
    uint8_t FeetStatus;
    CharacterFlags2 Flags2;
    CharacterFlags3 Flags3;
    [[bg3::hidden]] void* _PAD;
};

END_NS()
