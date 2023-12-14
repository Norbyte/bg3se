#pragma once

BEGIN_NS(party)

struct View
{
	int UserID;
	Guid field_8;
	Guid field_18;
	Array<EntityHandle> Characters;
};


struct ViewComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::PartyView;
	static constexpr auto EngineClass = "eoc::party::ViewComponent";

	Guid field_0;
	Array<View> Views;
	Array<EntityHandle> Characters;
};


struct CompositionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::PartyComposition;
	static constexpr auto EngineClass = "eoc::party::CompositionComponent";

	struct Member
	{
		Guid field_8;
		Guid field_18;
		Array<uint8_t> field_28;
	};


	EntityHandle Party;
	Guid field_8;
	Array<Member> Members;
};


struct PortalsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::PartyPortals;
	static constexpr auto EngineClass = "eoc::party::PortalsComponent";

	MultiHashSet<EntityHandle> Portals;
};


struct Recipe
{
	FixedString Name;
	uint8_t field_4;
};


struct RecipesComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::PartyRecipes;
	static constexpr auto EngineClass = "eoc::party::RecipesComponent";

	Array<Recipe> Recipes;
};


struct Waypoint
{
	FixedString Name;
	Guid field_8;
	FixedString Level;

	inline bool operator == (Waypoint const& o) const
	{
		return Name == o.Name;
	}
};


struct WaypointsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::PartyWaypoints;
	static constexpr auto EngineClass = "eoc::party::WaypointsComponent";

	MultiHashSet<Waypoint> Waypoints;
};


struct MemberComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::PartyMember;
	static constexpr auto EngineClass = "eoc::party::MemberComponent";

	int UserID;
	Guid field_8;
	EntityHandle Party;
	Guid field_20;
	uint8_t field_30;
};


struct FollowerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::PartyFollower;
	static constexpr auto EngineClass = "eoc::party::FollowerComponent";

	EntityHandle Following;
};


DEFINE_TAG_COMPONENT(eoc::party, CurrentlyFollowingPartyComponent, CurrentlyFollowingParty)
DEFINE_TAG_COMPONENT(eoc::party, BlockFollowComponent, BlockFollow)

END_NS()


BEGIN_SE()

template <>
inline uint64_t MultiHashMapHash<party::Waypoint>(party::Waypoint const& v)
{
	return MultiHashMapHash(v.Name);
}

END_SE()


BEGIN_NS(recruit)

struct RecruiterComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Recruiter;
	static constexpr auto EngineClass = "eoc::recruit::RecruiterComponent";

	MultiHashSet<EntityHandle> Recruiters;
};

END_NS()
