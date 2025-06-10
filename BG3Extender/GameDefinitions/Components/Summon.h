#pragma once

BEGIN_NS(esv::summon)

DEFINE_TAG_COMPONENT(esv::summon, IsUnsummoningComponent, ServerIsUnsummoning)

struct SpawnCreatedEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SummonCreatedEvent, "esv::summon::SpawnCreatedEventOneFrameComponent")

    EntityHandle Entity;
};

struct AddConcentrationRequestOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SummonAddConcentrationRequest, "esv::summon::AddConcentrationRequestOneFrameComponent")

    SpellId Spell;
    EntityHandle Caster;
    [[bg3::legacy(field_30)]] bool UnsummonOnConcentrationBreak;
};

struct AddToExistingConcentrationRequestOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SummonAddToExistingConcentrationRequest, "esv::summon::AddToExistingConcentrationRequestOneFrameComponent")

    EntityHandle field_0;
    uint8_t field_8;
};

struct AttachToProjectileRequestOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SummonAttachToProjectileRequest, "esv::summon::AttachToProjectileRequestOneFrameComponent")

    EntityHandle field_0;
};

struct OwnerSetEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SummonOwnerSetEvent, "esv::summon::OwnerSetEventOneFrameComponent")

    EntityHandle Owner;
};

struct PlaceInInventoryRequestOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SummonPlaceInInventoryRequest, "esv::summon::PlaceInInventoryRequestOneFrameComponent")

    EntityHandle Owner;
    bool field_8;
    bool field_9;
    bool CannotRemove;
};

struct SetLifetimeRequestOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SummonSetLifetimeRequest, "esv::summon::SetLifetimeRequestOneFrameComponent")

    std::optional<float> Lifetime;
};

DEFINE_ONEFRAME_TAG_COMPONENT(esv::summon, DespawnRequestOneFrameComponent, SummonDespawnRequest)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::summon, ExpiredRequestOneFrameComponent, SummonExpiredRequest)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::summon, LateJoinPenaltyOneFrameComponent, SummonLateJoinPenalty)


struct SummonRequest
{
    EntityHandle Owner;
    EntityHandle Summon;
    FixedString field_10;
    Guid field_18;
};

struct ItemSplitEvent
{
    EntityHandle Item1;
    EntityHandle Item2;
};

struct SummonInInventoryDesc
{
    EntityHandle Source;
    std::optional<EntityHandle> SourceInventory;
    FixedString MovingObject;
    FixedString Level;
    std::optional<float> Lifetime;
    uint32_t Amount{ 1 };
    bool PlaceInInventory{ false };
    bool Equip{ false };
    bool AllowOffHand{ false };
    bool ExtendExistingConcentration{ false };
    HashSet<FixedString> StatusesToApply;
    SpellIdWithPrototype Spell;
    ActionOriginator Originator;
    FixedString SpellContainer;
    int StoryActionId{ -1 };
    HitDesc Hit;
    AttackDesc Attack;
};

struct SummonDesc
{
    EntityHandle SummonTopOwner;
    FixedString MovingObject;
    FixedString Level;
    glm::vec3 Position{ .0f, .0f, .0f };
    glm::quat Rotation{ .0f, .0f, .0f, 1.0f };
    std::optional<float> Lifetime;
    HashSet<FixedString> StatusesToApply;
    SpellIdWithPrototype SpellId;
    bool IsFromFunctor{ false };
    bool AddToExistingConcentration{ false };
    bool ApplyLateJoinPenalty{ false };
    bool UseCasterPassives{ false };
    ActionOriginator Originator;
    FixedString Container;
    EntityHandle AttachToProjectile;
    int StoryActionId{ -1 };
    HitDesc Hit;
    AttackDesc Attack;
};

struct SpawnInInventoryDesc
{
    EntityHandle Source;
    std::optional<EntityHandle> SourceInventory;
    FixedString MovingObject;
    FixedString Level;
    int Amount{ 1 };
    bool PlaceInInventory{ false };
    bool Equip{ false };
    bool EquipOffHand{ false };
    HashSet<FixedString> StatusesToApply;
    SpellId SpellId;
    Guid SpellCastSourceGuid;
    ActionOriginator Originator;
    int StoryActionId{ -1 };
    HitDesc Hit;
    AttackDesc Attack;
};

struct SpawnDesc
{
    EntityHandle Caster;
    FixedString Template;
    FixedString Level;
    HashSet<FixedString> StatusesToApply;
    SpellId Spell;
    Guid SpellCastSourceGuid;
    ActionOriginator Originator;
    glm::vec3 Position{ .0f, .0f, .0f };
    int StoryActionId{ -1 };
    HitDesc Hit;
    AttackDesc Attack;
    bool ApplyLateJoinPenalty{ false };
};

struct SpawnSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerSummonSpawn, "esv::summon::SpawnSystem")

    [[bg3::hidden]] void* SpawnSystemHelper;
    [[bg3::hidden]] void* EntityManager;
    [[bg3::hidden]] void* SpellPrototypeManager;
    [[bg3::hidden]] void* ClassDescriptions;
    [[bg3::hidden]] void* GlobalTemplateManager;
    [[bg3::hidden]] void* LevelManager;
    [[bg3::hidden]] UnknownFunction field_40;
    Array<SummonRequest> RemovedSummon;
    Array<SummonRequest> AttachSummon;
    Array<ItemSplitEvent> SummonedItemWasSplit;
    HashSet<EntityHandle> SetupOwner;
    Array<SummonDesc> SummonDescs;
    Array<SummonInInventoryDesc> SummonInInventoryDescs;
    Array<SpawnDesc> CreateSpawn;
    Array<SpawnInInventoryDesc> SpawnInInventoryDescs;
};


struct DespawnSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerSummonDespawn, "esv::summon::DespawnSystem")

    HashMap<EntityHandle, HashMap<EntityHandle, bool>> KillSummon;
    HashMap<EntityHandle, bool> KillSummons;
};


END_NS()
