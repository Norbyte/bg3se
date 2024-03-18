#pragma once

BEGIN_NS(party)

struct View
{
	int UserID;
	[[bg3::legacy(field_8)]] Guid UserUuid;
	[[bg3::legacy(field_18)]] Guid ViewUuid;
	Array<EntityHandle> Characters;
};


struct ViewComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::PartyView;
	static constexpr auto EngineClass = "eoc::party::ViewComponent";

	[[bg3::legacy(field_0)]] Guid PartyUuid;
	Array<View> Views;
	Array<EntityHandle> Characters;
};


struct CompositionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::PartyComposition;
	static constexpr auto EngineClass = "eoc::party::CompositionComponent";

	struct Member
	{
		[[bg3::legacy(field_0)]] int32_t UserId;
		[[bg3::legacy(field_8)]] Guid UserUuid;
		[[bg3::legacy(field_18)]] Guid ViewUuid;
		Array<uint8_t> field_28;
	};


	EntityHandle Party;
	[[bg3::legacy(field_8)]] Guid PartyUuid;
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

	[[bg3::legacy(UserID)]] int UserId;
	[[bg3::legacy(field_8)]] Guid UserUuid;
	EntityHandle Party;
	[[bg3::legacy(field_20)]] Guid ViewUuid;
	[[bg3::legacy(field_30)]] bool IsPermanent;
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


BEGIN_NS(esv::party)

struct UserSnapshotComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerUserSnapshot;
	static constexpr auto EngineClass = "esv::party::UserSnapshotComponent";

	[[bg3::legacy(Snapshot)]] MultiHashMap<Guid, Array<Array<EntityHandle>>> PerUserCharacters;
};

END_NS()


BEGIN_NS(esv::escort)

struct FollowerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::EscortFollower;
	static constexpr auto EngineClass = "esv::escort::FollowerComponent";

	EntityHandle Following;
};

struct LeaderComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::EscortLeader;
	static constexpr auto EngineClass = "esv::escort::LeaderComponent";

	FixedString Group;
};

struct LeaderPriorityComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::EscortLeaderPriority;
	static constexpr auto EngineClass = "esv::escort::LeaderPriorityComponent";

	MultiHashMap<FixedString, int32_t> Priorities;
};

struct MemberComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::EscortMember;
	static constexpr auto EngineClass = "esv::escort::MemberComponent";

	FixedString Group;
};

struct StragglersTrackerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::EscortStragglersTracker;
	static constexpr auto EngineClass = "esv::escort::StragglersTrackerComponent";

	Array<EntityHandle> Stragglers;
	glm::vec3 field_10;
};

DEFINE_TAG_COMPONENT(esv::escort, HasStragglersComponent, EscortHasStragglers)

END_NS()