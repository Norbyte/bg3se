#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_SE()

struct IActionData
{
	virtual ~IActionData() = 0;
	virtual IActionData* Clone() = 0;
	virtual bool IsEqual(IActionData&) = 0;
	virtual bool Visit(ObjectVisitor&) = 0;
	virtual bool Unknown5() = 0;

    FixedString Animation;
    STDString Conditions;
    ActionDataType Type;
};

struct OpenCloseActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::OpenClose;
};

struct DestroyActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::Destroy;
};

struct TeleportActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::Teleport;

    FixedString Source;
    uint8_t SourceType;
    FixedString Target;
    uint8_t TargetType;
    FixedString EventID;
    uint8_t Visibility;
    bool AiUseInCombat;
    bool BlockMapMarkerNavigation;
    bool SnapToGround;
};

struct CreateSurfaceActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::CreateSurface;

    SurfaceType SurfaceType;
    float LifeTime;
    float Radius;
    bool ExternalCauseAsSurfaceOwner;
};

struct DestroyParametersActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::DestroyParameters;

    uint8_t TargetItemState;
    bool SnapToGround;
    FixedString ExplodeFX;
    FixedString VisualDestruction;
    FixedString VisualWithDynamicPhysics;
    float FadeOutDelay;
    FixedString FadeOutFX;
    FixedString TemplateAfterDestruction;
};

struct EquipActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::Equip;
};

struct ConsumeActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::Consume;

    FixedString StatsId;
    int32_t StatusDuration;
    bool Consume;
    bool IsHiddenStatus;
};

struct StoryUseActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::StoryUse;
};

struct DoorActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::Door;

    bool SecretDoor;
    //bool ScriptControlledDoor;
};

struct CreatePuddleActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::CreatePuddle;

    SurfaceType SurfaceType;
    float LifeTime;
    float Timeout;
    int TotalCells;
    int CellsAtGrow;
    float GrowTimer;
    bool ExternalCauseAsSurfaceOwner;
    bool ApplyDeathTypeBloodCheck;
};

struct BookActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::Book;

    FixedString BookId;
};

struct UseSpellActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::UseSpell;

    FixedString Spell;
    bool Consume;
    Guid Class;
};

struct SpellBookActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::SpellBook;

    FixedString Spell;
    bool Consume;
};

struct SitActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::Sit;

    float Heal;
};

struct LieActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::Lie;

    float Heal;
};

struct InsertActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::Insert;

    int8_t InsertSlots;
};

struct StandActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::Stand;
};

struct LockpickActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::Lockpick;

    bool Consume;
};

struct StoryUseInInventoryActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::StoryUseInInventory;
};

struct DisarmTrapActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::DisarmTrap;

    bool Consume;
};

struct ShowStoryElementUIActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::ShowStoryElementUI;

    int UIType;
    STDString UIStoryInstance;
};

struct CombineActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::Combine;

    int8_t CombineSlots;
    bool IsBase;
};

struct LadderActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::Ladder;

    float BottomHorizontalOffset;
    float BottomVerticalOffset;
    float TopMidOffset;
    float TopDetachOffset;
    float NodeLadderOffest;
    float TopAttachNearOffset;
    float TopMidToPlatformFixedLength;
    float TopLineTolerance;
    int ClimbDirection;
    bool AllowScaling;
};

struct PlaySoundActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::PlaySound;

    FixedString ActivateSoundEvent;
    bool PlayOnHUD;
};

struct SpawnCharacterActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::SpawnCharacter;

    FixedString RootTemplate;
    FixedString LocalTemplate;
    FixedString SpawnFX;
};

struct ConstrainActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::Constrain;

    float Damage;
};

struct RecipeActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::Recipe;

    FixedString Recipe;
};

struct Unknown31ActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::Unknown31;
};

struct ThrowActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::Throw;

    float FallbackPreviewRadius;
};

struct LearnSpellActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::LearnSpell;

    FixedString Spell;
    bool Consume;
};

struct Unknown34ActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::Unknown34;
};

struct Unknown35ActionData : public IActionData
{
    static constexpr auto Type = ActionDataType::Unknown35;
};

END_SE()

BEGIN_NS(lua)

LUA_POLYMORPHIC(IActionData)

END_NS()
