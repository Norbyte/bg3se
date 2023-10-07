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

END_NS()
