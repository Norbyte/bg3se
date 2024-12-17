#include <GameDefinitions/Controllers/OsirisTask.h>

BEGIN_NS(lua)

void LuaPolymorphic<esv::Task>::MakeRef(lua_State* L, esv::Task* v, LifetimeHandle const& lifetime)
{
#define V(type) case esv::type::Type: \
            MakeDirectObjectRef(L, static_cast<esv::type*>(v), lifetime); break;

    switch (v->Type) {
        V(OsirisMoveToLocationTask)
        V(OsirisMoveToObjectTask)
        V(OsirisFleeFromGridTask)
        V(OsirisMoveInRangeTask)
        V(OsirisTeleportToLocationTask)
        V(OsirisAppearTask)
        V(OsirisDisappearTask)
        V(OsirisFollowNPCTask)
        V(OsirisFollowOwnerOrLeaderTask)
        V(OsirisWanderTask)
        V(OsirisSteerTask)
        V(OsirisDropTask)
        V(OsirisPickupItemTask)
        V(OsirisUseItemTask)
        V(OsirisMoveItemTask)
        V(OsirisResurrectTask)
        V(OsirisUseSpellTask)
        V(OsirisMoveToAndTalkTask)
        V(OsirisLookAtTask)
        V(OsirisTimerTask)
        V(OsirisAutomatedDialogTask)
        V(OsirisRateLimitedAutomatedDialogTask)
        V(OsirisFleeFromRelationTask)
        V(OsirisFleeFromEntityTask)
        V(OsirisCombineTask)

    default:
        MakeDirectObjectRef(L, v, lifetime);
        break;
    }

#undef V
}

END_NS()
