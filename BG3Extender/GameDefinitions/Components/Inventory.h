#pragma once

BEGIN_NS(inventory)

struct DataComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InventoryData;
	static constexpr auto EngineClass = "eoc::inventory::DataComponent";

	uint8_t field_0;
	uint16_t Flags;
};

struct OwnerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InventoryOwner;
	static constexpr auto EngineClass = "eoc::inventory::OwnerComponent";

	Array<EntityHandle> Inventories;
	EntityHandle PrimaryInventory;
};

struct IsOwnedComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InventoryIsOwned;
	static constexpr auto EngineClass = "eoc::inventory::IsOwnedComponent";

	EntityHandle Owner;
};

struct ContainerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InventoryContainer;
	static constexpr auto EngineClass = "eoc::inventory::ContainerComponent";

	struct Item
	{
		EntityHandle Item;
		uint32_t field_8;
	};

	MultiHashMap<uint16_t, Item> Items;
};

struct MemberComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InventoryMember;
	static constexpr auto EngineClass = "eoc::inventory::MemberComponent";

	EntityHandle Inventory;
	int16_t EquipmentSlot;
};

struct MemberTransformComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InventoryMemberTransform;
	static constexpr auto EngineClass = "eoc::inventory::MemberTransformComponent";

	Transform Transform;
};

struct StackComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InventoryStack;
	static constexpr auto EngineClass = "eoc::inventory::StackComponent";

	Array<EntityHandle> Arr_u64;
	Array<uint8_t> Arr_u8;
};

struct StackMemberComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InventoryStackMember;
	static constexpr auto EngineClass = "eoc::inventory::StackMemberComponent";

	EntityHandle Stack;
};

struct WeightComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InventoryWeight;
	static constexpr auto EngineClass = "eoc::inventory::WeightComponent";

	int Weight;
};

struct WieldedComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Wielded;
	static constexpr auto EngineClass = "eoc::inventory::WieldedComponent";

	Guid field_0;
};


DEFINE_TAG_COMPONENT(eoc::inventory, CanBeInComponent, CanBeInInventory)
DEFINE_TAG_COMPONENT(eoc::inventory, CannotBePickpocketedComponent, CannotBePickpocketed)
DEFINE_TAG_COMPONENT(eoc::inventory, CannotBeTakenOutComponent, CannotBeTakenOut)
DEFINE_TAG_COMPONENT(eoc::inventory, DropOnDeathBlockedComponent, DropOnDeathBlocked)
DEFINE_TAG_COMPONENT(eoc::inventory, NewItemsInsideComponent, NewItemsInside)
DEFINE_TAG_COMPONENT(eoc::inventory, NonTradableComponent, NonTradable)

END_NS()
