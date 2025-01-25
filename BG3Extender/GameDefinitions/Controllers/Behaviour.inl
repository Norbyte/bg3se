#include <GameDefinitions/Controllers/Behaviour.h>

BEGIN_NS(lua)

void LuaPolymorphic<esv::BehaviourState>::MakeRef(lua_State* L, esv::BehaviourState* v, LifetimeHandle lifetime)
{
#define V(type) case esv::type::Type: \
            MakeDirectObjectRef(L, static_cast<esv::type*>(v), lifetime); break;

    switch (v->GetType()) {
        V(BSMove)
        V(BSMoveToAndTalk)
        V(BSMoveItem)
        V(BSMoveAndUseItem)
        V(BSPickupItem)
        V(BSWander)
        V(BSAppear)
        V(BSFollowCharacter)
        V(BSPatrol)
        V(BSForce)

    default:
        MakeDirectObjectRef(L, v, lifetime);
        break;
    }

#undef V
}

END_NS()

BEGIN_NS(esv)

BehaviourType BehaviourState::LuaGetType() const
{
    return GetType();
}

END_NS()
