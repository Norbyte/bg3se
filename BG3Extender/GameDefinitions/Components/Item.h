#pragma once

BEGIN_NS(item)

struct DestroyInfo
{
	FixedString field_0;
	uint8_t field_4;
};

struct DestroyedComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ItemDestroyed;
	static constexpr auto EngineClass = "eoc::item::DestroyedComponent";

	std::optional<DestroyInfo> Info;
};

struct DyeComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ItemDye;
	static constexpr auto EngineClass = "eoc::item::DyeComponent";

	Guid Color;
};

struct MapMarkerStyleComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::MapMarkerStyle;
	static constexpr auto EngineClass = "eoc::item::MapMarkerStyleComponent";

	FixedString Style;
};

struct PortalComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ItemPortal;
	static constexpr auto EngineClass = "eoc::item::PortalComponent";

	uint8_t field_0;
	uint8_t field_1;
};


DEFINE_TAG_COMPONENT(eoc::item, DoorComponent, IsDoor)
DEFINE_TAG_COMPONENT(eoc::item, ExamineDisabledComponent, ExamineDisabled)
DEFINE_TAG_COMPONENT(eoc::item, HasMovedComponent, ItemHasMoved)
DEFINE_TAG_COMPONENT(eoc::item, HasOpenedComponent, HasOpened)
DEFINE_TAG_COMPONENT(eoc::item, IsGoldComponent, IsGold)
DEFINE_TAG_COMPONENT(eoc::item, ItemComponent, IsItem)
DEFINE_TAG_COMPONENT(eoc::item, NewInInventoryComponent, NewInInventory)
DEFINE_TAG_COMPONENT(eoc::item, ShouldDestroyOnSpellCastComponent, ShouldDestroyOnSpellCast)

END_NS()


BEGIN_NS(item_template)

struct ActionTypeComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ActionType;
	static constexpr auto EngineClass = "eoc::item_template::ActionTypeComponent";

	MultiHashSet<uint8_t> ActionTypes;
};

struct UseActionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::UseAction;
	static constexpr auto EngineClass = "eoc::item_template::UseActionComponent";

	[[bg3::hidden]] Array<void*> UseActions;
};


DEFINE_TAG_COMPONENT(eoc::item_template, CanMoveComponent, ItemCanMove)
DEFINE_TAG_COMPONENT(eoc::item_template, ClimbOnComponent, ClimbOn)
DEFINE_TAG_COMPONENT(eoc::item_template, DestroyedComponent, ItemTemplateDestroyed)
DEFINE_TAG_COMPONENT(eoc::item_template, InteractionDisabledComponent, InteractionDisabled)
DEFINE_TAG_COMPONENT(eoc::item_template, IsStoryItemComponent, IsStoryItem)
DEFINE_TAG_COMPONENT(eoc::item_template, LadderComponent, Ladder)
DEFINE_TAG_COMPONENT(eoc::item_template, WalkOnComponent, WalkOn)

END_NS()


BEGIN_NS(ownership)

struct OwneeCurrentComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::OwneeCurrent;
	static constexpr auto EngineClass = "eoc::ownership::OwneeCurrentComponent";

	EntityHandle Ownee;
};


DEFINE_TAG_COMPONENT(eoc::ownership, OwnedAsLootComponent, OwnedAsLoot)

END_NS()


BEGIN_NS(esv::item)

struct DynamicLayerOwnerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerDynamicLayerOwner;
	static constexpr auto EngineClass = "esv::item::DynamicLayerOwnerComponent";

	FixedString Owner;
};

END_NS()


BEGIN_NS(esv::ownership)

struct IsCurrentOwnerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerIsCurrentOwner;
	static constexpr auto EngineClass = "esv::ownership::IsCurrentOwnerComponent";

	MultiHashSet<EntityHandle> Owner;
};

struct IsLatestOwnerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerIsLatestOwner;
	static constexpr auto EngineClass = "esv::ownership::IsLatestOwnerComponent";

	MultiHashSet<EntityHandle> Owner;
};

struct IsPreviousLatestOwnerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerIsPreviousLatestOwner;
	static constexpr auto EngineClass = "esv::ownership::IsPreviousLatestOwnerComponent";

	MultiHashSet<EntityHandle> Owner;
};

struct IsPreviousOwnerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerIsPreviousOwner;
	static constexpr auto EngineClass = "esv::ownership::IsPreviousOwnerComponent";

	MultiHashSet<EntityHandle> Owner;
};

struct IsOriginalOwnerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerIsOriginalOwner;
	static constexpr auto EngineClass = "esv::ownership::IsOriginalOwnerComponent";

	MultiHashSet<EntityHandle> Owner;
};

struct OwneeHistoryComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerOwneeHistory;
	static constexpr auto EngineClass = "esv::ownership::OwneeHistoryComponent";

	EntityHandle OriginalOwner;
	EntityHandle LatestOwner;
	EntityHandle PreviousOwner;
	EntityHandle PreviousLatestOwner;
};

struct OwneeRequestComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerOwneeRequest;
	static constexpr auto EngineClass = "esv::ownership::OwneeRequestComponent";

	EntityHandle NewCurrentOwnee;
	bool RequestChangeCurrentOwnee;
	EntityHandle LatestOwner;
	bool RequestChangeLatestOwner;
	EntityHandle OriginalOwner;
	bool RequestChangeOriginalOwner;
};

END_NS()
