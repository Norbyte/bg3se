#pragma once

BEGIN_NS(inventory)

struct DataComponent : public BaseComponent
{
	DEFINE_COMPONENT(InventoryData, "eoc::inventory::DataComponent")

	uint8_t field_0;
	uint16_t Flags;
};

struct OwnerComponent : public BaseComponent
{
	DEFINE_COMPONENT(InventoryOwner, "eoc::inventory::OwnerComponent")

	Array<EntityHandle> Inventories;
	EntityHandle PrimaryInventory;
};

struct IsOwnedComponent : public BaseComponent
{
	DEFINE_COMPONENT(InventoryIsOwned, "eoc::inventory::IsOwnedComponent")

	EntityHandle Owner;
};

struct ContainerSlotData
{
	EntityHandle Item;
	uint32_t field_8;
};

struct ContainerComponent : public BaseComponent
{
	DEFINE_COMPONENT(InventoryContainer, "eoc::inventory::ContainerComponent")

	HashMap<uint16_t, ContainerSlotData> Items;
};

struct MemberComponent : public BaseComponent
{
	DEFINE_COMPONENT(InventoryMember, "eoc::inventory::MemberComponent")

	EntityHandle Inventory;
	int16_t EquipmentSlot;
};

struct MemberTransformComponent : public BaseComponent
{
	DEFINE_COMPONENT(InventoryMemberTransform, "eoc::inventory::MemberTransformComponent")

	Transform Transform;
};

struct StackComponent : public BaseComponent
{
	DEFINE_COMPONENT(InventoryStack, "eoc::inventory::StackComponent")

	[[bg3::legacy(Arr_u64)]] Array<EntityHandle> Elements;
	Array<uint8_t> Arr_u8;
};

struct StackMemberComponent : public BaseComponent
{
	DEFINE_COMPONENT(InventoryStackMember, "eoc::inventory::StackMemberComponent")

	EntityHandle Stack;
};

struct WeightComponent : public BaseComponent
{
	DEFINE_COMPONENT(InventoryWeight, "eoc::inventory::WeightComponent")

	int Weight;
};

struct WieldedComponent : public BaseComponent
{
	DEFINE_COMPONENT(Wielded, "eoc::inventory::WieldedComponent")

	Guid field_0;
};


DEFINE_TAG_COMPONENT(eoc::inventory, CanBeInComponent, CanBeInInventory)
DEFINE_TAG_COMPONENT(eoc::inventory, CannotBePickpocketedComponent, CannotBePickpocketed)
DEFINE_TAG_COMPONENT(eoc::inventory, CannotBeTakenOutComponent, CannotBeTakenOut)
DEFINE_TAG_COMPONENT(eoc::inventory, DropOnDeathBlockedComponent, DropOnDeathBlocked)
DEFINE_TAG_COMPONENT(eoc::inventory, NewItemsInsideComponent, NewItemsInside)
DEFINE_TAG_COMPONENT(eoc::inventory, NonTradableComponent, NonTradable)

END_NS()

BEGIN_NS(esv::inventory)

struct ContainerDataComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerInventoryContainerData, "esv::inventory::ContainerDataComponent")

	uint16_t Flags;
	int field_4;
};

struct GroupCheckComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerInventoryGroupCheck, "esv::inventory::GroupCheckComponent")

	stats::ConditionId Conditions;
};

struct ShapeshiftEquipmentHistoryComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerShapeshiftEquipmentHistory, "esv::inventory::ShapeshiftEquipmentHistoryComponent")

	Array<Guid> History;
};

DEFINE_TAG_COMPONENT(esv::inventory, CharacterHasGeneratedTradeTreasureComponent, CharacterHasGeneratedTradeTreasure)
DEFINE_TAG_COMPONENT(esv::inventory, IsReplicatedWithComponent, ServerInventoryIsReplicatedWith)
DEFINE_TAG_COMPONENT(esv::inventory, ReadyToBeAddedToInventoryComponent, ReadyToBeAddedToInventory)

END_NS()


BEGIN_NS(esv)

struct InventoryPropertyCanBePickpocketedComponent : public BaseComponent
{
	DEFINE_COMPONENT(InventoryPropertyCanBePickpocketed, "esv::InventoryPropertyCanBePickpocketedComponent")

	GenericPropertyTag Tag;
};

struct InventoryPropertyIsDroppedOnDeathComponent : public BaseComponent
{
	DEFINE_COMPONENT(InventoryPropertyIsDroppedOnDeath, "esv::InventoryPropertyIsDroppedOnDeathComponent")

	GenericPropertyTag Tag;
};

struct InventoryPropertyIsTradableComponent : public BaseComponent
{
	DEFINE_COMPONENT(InventoryPropertyIsTradable, "esv::InventoryPropertyIsTradableComponent")

	GenericPropertyTag Tag;
};

DEFINE_TAG_COMPONENT(esv, ScriptPropertyCanBePickpocketedComponent, ScriptPropertyCanBePickpocketed)
DEFINE_TAG_COMPONENT(esv, ScriptPropertyIsDroppedOnDeathComponent, ScriptPropertyIsDroppedOnDeath)
DEFINE_TAG_COMPONENT(esv, ScriptPropertyIsTradableComponent, ScriptPropertyIsTradable)

END_NS()
