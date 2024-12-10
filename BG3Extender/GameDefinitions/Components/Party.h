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
    DEFINE_COMPONENT(PartyView, "eoc::party::ViewComponent")

    [[bg3::legacy(field_0)]] Guid PartyUuid;
    Array<View> Views;
    Array<EntityHandle> Characters;
};

struct Member
{
    [[bg3::legacy(field_0)]] int32_t UserId;
    [[bg3::legacy(field_8)]] Guid UserUuid;
    [[bg3::legacy(field_18)]] Guid ViewUuid;
    Array<uint8_t> field_28;
};

struct CompositionComponent : public BaseComponent
{
    DEFINE_COMPONENT(PartyComposition, "eoc::party::CompositionComponent")

    EntityHandle Party;
    [[bg3::legacy(field_8)]] Guid PartyUuid;
    Array<Member> Members;
};


struct PortalsComponent : public BaseComponent
{
    DEFINE_COMPONENT(PartyPortals, "eoc::party::PortalsComponent")

    HashSet<EntityHandle> Portals;
};


struct Recipe
{
    FixedString Name;
    uint8_t field_4;
};


struct RecipesComponent : public BaseComponent
{
    DEFINE_COMPONENT(PartyRecipes, "eoc::party::RecipesComponent")

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
    DEFINE_COMPONENT(PartyWaypoints, "eoc::party::WaypointsComponent")

    HashSet<Waypoint> Waypoints;
};


struct MemberComponent : public BaseComponent
{
    DEFINE_COMPONENT(PartyMember, "eoc::party::MemberComponent")

    [[bg3::legacy(UserID)]] int UserId;
    [[bg3::legacy(field_8)]] Guid UserUuid;
    EntityHandle Party;
    [[bg3::legacy(field_20)]] Guid ViewUuid;
    [[bg3::legacy(field_30)]] bool IsPermanent;
};


struct FollowerComponent : public BaseComponent
{
    DEFINE_COMPONENT(PartyFollower, "eoc::party::FollowerComponent")

    EntityHandle Following;
};


DEFINE_TAG_COMPONENT(eoc::party, CurrentlyFollowingPartyComponent, CurrentlyFollowingParty)
DEFINE_TAG_COMPONENT(eoc::party, BlockFollowComponent, BlockFollow)

END_NS()


BEGIN_SE()

template <>
inline uint64_t HashMapHash<party::Waypoint>(party::Waypoint const& v)
{
    return HashMapHash(v.Name);
}

END_SE()


BEGIN_NS(recruit)

struct RecruiterComponent : public BaseComponent
{
    DEFINE_COMPONENT(Recruiter, "eoc::recruit::RecruiterComponent")

    HashSet<EntityHandle> Recruiters;
};

END_NS()


BEGIN_NS(esv::party)

struct UserSnapshotComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerUserSnapshot, "esv::party::UserSnapshotComponent")

    [[bg3::legacy(Snapshot)]] HashMap<Guid, Array<Array<EntityHandle>>> PerUserCharacters;
};

END_NS()


BEGIN_NS(esv::escort)

struct FollowerComponent : public BaseComponent
{
    DEFINE_COMPONENT(EscortFollower, "esv::escort::FollowerComponent")

    EntityHandle Following;
};

struct LeaderComponent : public BaseComponent
{
    DEFINE_COMPONENT(EscortLeader, "esv::escort::LeaderComponent")

    FixedString Group;
};

struct LeaderPriorityComponent : public BaseComponent
{
    DEFINE_COMPONENT(EscortLeaderPriority, "esv::escort::LeaderPriorityComponent")

    HashMap<FixedString, int32_t> Priorities;
};

struct MemberComponent : public BaseComponent
{
    DEFINE_COMPONENT(EscortMember, "esv::escort::MemberComponent")

    FixedString Group;
};

struct StragglersTrackerComponent : public BaseComponent
{
    DEFINE_COMPONENT(EscortStragglersTracker, "esv::escort::StragglersTrackerComponent")

    Array<EntityHandle> Stragglers;
    glm::vec3 field_10;
};

DEFINE_TAG_COMPONENT(esv::escort, HasStragglersComponent, EscortHasStragglers)

END_NS()