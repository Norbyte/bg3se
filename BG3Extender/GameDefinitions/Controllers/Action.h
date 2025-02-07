#pragma once

BEGIN_NS(esv)

struct ActionState : public ProtectedGameObject<ActionState>
{
    virtual ~ActionState() = 0;
    virtual ActionType GetType() const = 0;
    virtual bool Enter() = 0;
    virtual bool Continue() = 0;
    virtual void Update(GameTime const&) = 0;
    virtual bool Terminate() = 0;
    virtual void Exit() = 0;
    virtual bool IsFinished() = 0;
    virtual void VMT40() = 0;
    virtual bool OnAnimationEvent() = 0;
    virtual void VMT50() = 0;
    virtual bool SetSyncDataDirty(bool) = 0;
    virtual bool SyncData(ScratchBuffer&) = 0;
    virtual bool SavegameVisit(ObjectVisitor&) = 0;
    virtual void VMT70() = 0;
    virtual bool IsFinished2() = 0;
    virtual void OnAnimationFinished() = 0;
    virtual void OnActionInterruptible() = 0;
    virtual void OnActionUpdate() = 0;

    ActionMachine* ActionMachine;
    [[bg3::readonly]] int TransactionId;
    bool SyncDataDirty;
    bool Finished;
    uint8_t field_16;
    ActionOriginator Originator;

    // Lua getter helpers
    ActionType LuaGetType() const;
    //# P_GETTER(Type, LuaGetType)
};

struct ActionLayer
{
    ActionState* State;
    ActionState* SyncState;
    bool Force;
    bool NeedsSync;
    bool HasValidState;
};

struct ActionMachine : public ProtectedGameObject<ActionMachine>
{
    [[bg3::hidden]] void* VMT;
    EntityHandle Entity;
    std::array<ActionLayer, 3> Layers;
    std::array<bool, 3> Entering;
    std::array<bool, 3> ExitRequested;
    [[bg3::hidden]] std::array<ActionState*, 11> CachedActions;
};

struct ASAnimation : public ActionState
{
    static constexpr auto Type = ActionType::Animation;

    Array<FixedString> StartAnimations;
    Array<FixedString> LoopAnimations;
    Array<FixedString> EndAnimations;
    float Time;
    float LastTime;
    uint8_t field_70;
    bool ShapeshiftFinishFired;
    bool RequestCancel;
    bool RequestCancel2;
    bool RequestCancelAction2;
    bool CancelRequested;
    uint8_t field_76;
};

struct ASMoveBase : public ActionState
{
    uint8_t field_38;
    float OsirisRequestTimer;
    uint8_t field_40;
    int StoryRequestId;
};

struct ASMoveItem : public ASMoveBase
{
    static constexpr auto Type = ActionType::MoveItem;

    Array<EntityHandle> ItemsToMove;
    Array<glm::vec3> MoveToPositions;
    Array<glm::quat> Rotations;
    Array<float> MoveTimes;
    int Amount;
    bool MoveFinished;
};

struct ASKnockedDown : public ASMoveBase
{
    static constexpr auto Type = ActionType::KnockedDown;

    uint8_t State;
    uint8_t RequestedState;
    int MaxAnimationPriority;
    bool Prone;
    bool RequestGenomeEvent;
    uint8_t field_42;
};

struct ASUseItem : public ASMoveBase
{
    static constexpr auto Type = ActionType::UseItem;

    EntityHandle Item;
    float UseTimer;
    float ClimbDuration;
    float UseDuration2;
    bool UseCompleted;
    bool UseFailed;
    bool IsInteraction;
    bool UseItem;
    bool InUse;
    bool UseStartRequested;
    uint8_t field_62;
    uint8_t ActionType;
    FixedString field_64;
    IActionData* UseAction;
};

struct ASCombineItem : public ASMoveBase
{
    static constexpr auto Type = ActionType::CombineItem;

    Array<EntityHandle> Ingredients;
    FixedString Recipe;
    float field_5C;
    float TimeRemaining;
    uint8_t field_64;
    uint8_t field_65;
};

struct ASTeleportFalling : public ActionState
{
    static constexpr auto Type = ActionType::TeleportFalling;

    uint8_t field_38;
    uint8_t field_39;
    float TickTime;
    bool HasDamageBeenApplied;
    bool IsFromItem;
    SpellId Spell;
    EntityHandle Cause;
    int StoryActionId;
};

struct ASIncapacitated : public ActionState
{
    static constexpr auto Type = ActionType::Incapacitated;

    FixedString StartAnimation;
    FixedString LoopAnimation;
    FixedString EndAnimation;
    int field_44;
    float Duration;
    bool StateChanged;
    bool StatusRemoved;
    int MaxAnimationPriority;
    int State;
    bool FinishEventFired;
    uint8_t field_59;
};

END_NS()

BEGIN_NS(lua)

LUA_POLYMORPHIC(esv::ActionState);

END_NS()
