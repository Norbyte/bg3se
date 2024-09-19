#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_SE()

struct IActionData;

struct [[bg3::hidden]] IActionDataVMT
{
    using DtorProc = void (IActionData*, bool);
    using CloneProc = IActionData* (IActionData*);
    using IsEqualProc = bool (IActionData*, IActionData const&);
    using VisitProc = bool (IActionData*, ObjectVisitor&);
    using Unknown5proc = bool (IActionData*);

    DtorProc* Dtor;
    CloneProc* Clone;
    IsEqualProc* IsEqual;
    VisitProc* Visit;
    Unknown5proc* Unknown5;
};

struct IActionData
{
    [[bg3::hidden]] IActionDataVMT* vftable{ nullptr };
    FixedString Animation;
    STDString Conditions;
    [[bg3::readonly]] ActionDataType Type{ ActionDataType::Unknown };
};

struct OpenCloseActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::OpenClose;
};

struct DestroyActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::Destroy;
};

struct TeleportActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::Teleport;

    FixedString Source;
    uint8_t SourceType{ 0 };
    FixedString Target;
    uint8_t TargetType{ 0 };
    FixedString EventID;
    uint8_t Visibility{ 0 };
    bool AiUseInCombat{ false };
    bool BlockMapMarkerNavigation{ false };
    bool SnapToGround{ false };
};

struct CreateSurfaceActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::CreateSurface;

    SurfaceType SurfaceType{ SurfaceType::None };
    float LifeTime{ 0.0f };
    float Radius{ 0.0f };
    bool ExternalCauseAsSurfaceOwner{ false };
};

struct DestroyParametersActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::DestroyParameters;

    uint8_t TargetItemState{ 0 };
    bool SnapToGround{ false };
    FixedString ExplodeFX;
    FixedString VisualDestruction;
    FixedString VisualWithDynamicPhysics;
    float FadeOutDelay{ 0.0f };
    FixedString FadeOutFX;
    FixedString TemplateAfterDestruction;
};

struct EquipActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::Equip;
};

struct ConsumeActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::Consume;

    FixedString StatsId;
    int32_t StatusDuration{ 0 };
    bool Consume{ false };
    bool IsHiddenStatus{ false };
};

struct StoryUseActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::StoryUse;
};

struct DoorActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::Door;

    bool SecretDoor{ false };
    //bool ScriptControlledDoor{ false };
};

struct CreatePuddleActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::CreatePuddle;

    SurfaceType SurfaceType{ SurfaceType::None };
    float LifeTime{ 0.0f };
    float Timeout{ 0.0f };
    int TotalCells{ 0 };
    int CellsAtGrow{ 0 };
    float GrowTimer{ 0.0f };
    bool ExternalCauseAsSurfaceOwner{ false };
    bool ApplyDeathTypeBloodCheck{ false };
};

struct BookActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::Book;

    FixedString BookId;
};

struct UseSpellActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::UseSpell;

    FixedString Spell;
    bool Consume{ false };
    Guid Class;
};

struct SpellBookActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::SpellBook;

    FixedString Spell;
    bool Consume{ false };
};

struct SitActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::Sit;

    float Heal{ 0.0f };
};

struct LieActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::Lie;

    float Heal{ 0.0f };
};

struct InsertActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::Insert;

    int8_t InsertSlots{ 0 };
};

struct StandActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::Stand;
};

struct LockpickActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::Lockpick;

    bool Consume{ false };
};

struct StoryUseInInventoryActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::StoryUseInInventory;
};

struct DisarmTrapActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::DisarmTrap;

    bool Consume{ false };
};

struct ShowStoryElementUIActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::ShowStoryElementUI;

    int UIType{ 0 };
    STDString UIStoryInstance;
};

struct CombineActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::Combine;

    int8_t CombineSlots{ 0 };
    bool IsBase{ false };
};

struct LadderActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::Ladder;

    float BottomHorizontalOffset{ 0.0f };
    float BottomVerticalOffset{ 0.0f };
    float TopMidOffset{ 0.0f };
    float TopDetachOffset{ 0.0f };
    float NodeLadderOffest{ 0.0f };
    float TopAttachNearOffset{ 0.0f };
    float TopMidToPlatformFixedLength{ 0.0f };
    float TopLineTolerance{ 0.0f };
    int ClimbDirection{ 0 };
    bool AllowScaling{ false };
};

struct PlaySoundActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::PlaySound;

    FixedString ActivateSoundEvent;
    bool PlayOnHUD{ false };
};

struct SpawnCharacterActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::SpawnCharacter;

    FixedString RootTemplate;
    FixedString LocalTemplate;
    FixedString SpawnFX;
};

struct ConstrainActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::Constrain;

    float Damage{ 0.0f };
};

struct RecipeActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::Recipe;

    FixedString Recipe;
};

struct Unknown31ActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::Unknown31;
};

struct ThrowActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::Throw;

    float FallbackPreviewRadius{ 0.0f };
};

struct LearnSpellActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::LearnSpell;

    FixedString Spell;
    bool Consume{ false };
};

struct Unknown34ActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::Unknown34;
};

struct Unknown35ActionData : public IActionData
{
    static constexpr auto StaticType = ActionDataType::Unknown35;
};

END_SE()

BEGIN_NS(lua)

LUA_POLYMORPHIC(IActionData)

END_NS()
