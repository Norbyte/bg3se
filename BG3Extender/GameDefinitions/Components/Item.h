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

	glm::vec4 Color;
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
