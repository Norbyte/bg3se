#pragma once

BEGIN_NS(hotbar)

struct Slot
{
    EntityHandle Item;
    SpellId SpellId;
    FixedString Passive;
    uint32_t Slot;
    bool IsNew{ false };
    bool Delete{ false };
};

struct Bar
{
    uint8_t Index{ 0 }; // 0=Common, 1=Class, 2=Item
    [[bg3::legacy(field_1)]] uint8_t Controller{ 0 };
    Array<Slot> Elements;
    uint8_t Width{ 1 };
    uint32_t Height{ 1 };
    [[bg3::legacy(field_20)]] STDString Name;
};

struct ContainerComponent : public BaseComponent
{
    DEFINE_COMPONENT(HotbarContainer, "eoc::hotbar::ContainerComponent")

    HashMap<FixedString, Array<Bar>> Containers;
    FixedString ActiveContainer;
    bool Locked{ false };
};

struct CurrentDecksComponent : public BaseComponent
{
    DEFINE_COMPONENT(HotbarDecks, "eoc::hotbar::CurrentDecksComponent")

    HashMap<FixedString, int32_t> Decks;
};

struct ContainerKeyCreatedOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(HotbarContainerKeyCreated, "eoc::hotbar::ContainerKeyCreatedOneFrameComponent")

    FixedString Key;
};

END_NS()

BEGIN_NS(esv::hotbar)

DEFINE_TAG_COMPONENT(esv::hotbar, OrderComponent, ServerHotbarOrder)

struct AddSlotEntryData
{
    bool ForceAdd;
    bool field_1;
    int32_t HotbarIndex;
    uint8_t HotBarController;
    bg3se::hotbar::Slot Slot;
    std::optional<uint32_t> SlotIndex;
    bool AllowDuplicates;
};

struct RemoveSlotEntryData
{
    bool field_0;
    bool RemoveSpells;
    int32_t HotbarIndex;
    bg3se::hotbar::Slot Slot;
    std::optional<uint32_t> SlotIndex;
    bool IgnoreRequirements;
    bool field_61;
};

struct RemoveColumnData
{
    uint32_t HotbarIndex;
    uint32_t Index;
};

struct SetRowData
{
    uint32_t HotbarIndex;
    uint32_t Index;
};

struct SetColumnData
{
    uint32_t HotbarIndex;
    uint32_t Width;
};

struct AddCustomDeckData
{
    STDString Name;
};

struct RemoveCustomDeckData
{
    uint32_t Index;
};

struct ChangeNameData
{
    uint32_t Index;
    STDString Name;
};

struct AddedSlot
{
    bg3se::hotbar::Slot Slot;
    uint8_t Controller;
};

struct SlotEventData
{
    bg3se::hotbar::Slot Slot;
    bool IsController;
    bool IsNew;
};

struct SlotsAddedOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(ServerHotbarSlotsAdded, "esv::hotbar::SlotsAddedOneFrameComponent")

    Array<SlotEventData> Events;
};

struct SlotsRemovedOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(ServerHotbarSlotsRemoved, "esv::hotbar::SlotsRemovedOneFrameComponent")

    Array<SlotEventData> Events;
};


struct HotbarSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerHotbar, "esv::hotbar::System")

    HashMap<EntityHandle, Array<AddSlotEntryData>> AddSlot;
    HashMap<EntityHandle, Array<RemoveSlotEntryData>> RemoveSlot;
    HashMap<EntityHandle, Array<bg3se::hotbar::Slot>> RemoveSlotIsNewFlagRequests;
    HashMap<EntityHandle, Array<SetColumnData>> SetColumn;
    HashMap<EntityHandle, Array<SetRowData>> SetRow;
    HashMap<EntityHandle, Array<RemoveColumnData>> RemoveRow;
    HashMap<EntityHandle, bool> Lock;
    HashMap<EntityHandle, Array<AddCustomDeckData>> AddCustomDeck;
    HashMap<EntityHandle, Array<RemoveCustomDeckData>> RemoveCustomDeck;
    HashMap<EntityHandle, Array<ChangeNameData>> ChangeName;
    HashSet<EntityHandle> field_290;
    HashSet<EntityHandle> field_2c0;
    HashMap<EntityHandle, Array<inventory::MemberData>> field_2f0;
    [[bg3::hidden]] void* GameControl;
    [[bg3::hidden]] void* SpellPrototypeManager;
    [[bg3::hidden]] void* Passives;
    [[bg3::hidden]] void* FixedHotBarSlots;
    bool SavegameLoading;
    uint32_t field_354;
    [[bg3::hidden]] UnknownFunction SignalConnectionCollection;
    [[bg3::hidden]] void* UnitTestHelper;
};

END_NS()
