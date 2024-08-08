#pragma once

BEGIN_NS(item)

struct DestroyInfo
{
	FixedString field_0;
	uint8_t field_4;
};

struct DestroyedComponent : public BaseComponent
{
	DEFINE_COMPONENT(ItemDestroyed, "eoc::item::DestroyedComponent")

	std::optional<DestroyInfo> Info;
};

struct DyeComponent : public BaseComponent
{
	DEFINE_COMPONENT(ItemDye, "eoc::item::DyeComponent")

	Guid Color;
};

struct MapMarkerStyleComponent : public BaseComponent
{
	DEFINE_COMPONENT(MapMarkerStyle, "eoc::item::MapMarkerStyleComponent")

	FixedString Style;
};

struct PortalComponent : public BaseComponent
{
	DEFINE_COMPONENT(ItemPortal, "eoc::item::PortalComponent")

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
	DEFINE_COMPONENT(ActionType, "eoc::item_template::ActionTypeComponent")

	HashSet<uint8_t> ActionTypes;
};

struct UseActionComponent : public BaseComponent
{
	DEFINE_COMPONENT(UseAction, "eoc::item_template::UseActionComponent")

	Array<IActionData*> UseActions;
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
	DEFINE_COMPONENT(OwneeCurrent, "eoc::ownership::OwneeCurrentComponent")

	EntityHandle Ownee;
};


DEFINE_TAG_COMPONENT(eoc::ownership, OwnedAsLootComponent, OwnedAsLoot)

END_NS()


BEGIN_NS(esv::item)

struct DynamicLayerOwnerComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerDynamicLayerOwner, "esv::item::DynamicLayerOwnerComponent")

	FixedString Owner;
};

END_NS()


BEGIN_NS(esv::ownership)

struct IsCurrentOwnerComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerIsCurrentOwner, "esv::ownership::IsCurrentOwnerComponent")

	HashSet<EntityHandle> Owner;
};

struct IsLatestOwnerComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerIsLatestOwner, "esv::ownership::IsLatestOwnerComponent")

	HashSet<EntityHandle> Owner;
};

struct IsPreviousOwnerComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerIsPreviousOwner, "esv::ownership::IsPreviousOwnerComponent")

	HashSet<EntityHandle> Owner;
};

struct IsOriginalOwnerComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerIsOriginalOwner, "esv::ownership::IsOriginalOwnerComponent")

	HashSet<EntityHandle> Owner;
};

struct OwneeHistoryComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerOwneeHistory, "esv::ownership::OwneeHistoryComponent")

	EntityHandle OriginalOwner;
	EntityHandle LatestOwner;
	EntityHandle PreviousOwner;
};

struct OwneeRequestComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerOwneeRequest, "esv::ownership::OwneeRequestComponent")

	std::optional<EntityHandle> NewCurrentOwnee;
	std::optional<EntityHandle> LatestOwner;
	std::optional<EntityHandle> OriginalOwner;
};

END_NS()
