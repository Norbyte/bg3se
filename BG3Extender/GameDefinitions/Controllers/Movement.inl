#include <GameDefinitions/Controllers/Movement.h>

BEGIN_NS(lua)

void LuaPolymorphic<esv::MovementState>::MakeRef(lua_State* L, esv::MovementState* v, LifetimeHandle lifetime)
{
#define V(type) case esv::type::Type: \
            MakeDirectObjectRef(L, static_cast<esv::type*>(v), lifetime); break;

    switch (v->GetType()) {
        V(MSMoveTo)
        V(MSIdle)
        V(MSMoveToSync)
        V(MSClimbing)
        V(MSFalling)
        V(MSRepose)

    default:
        MakeDirectObjectRef(L, v, lifetime);
        break;
    }

#undef V
}

END_NS()

BEGIN_NS(esv)

MovementType MovementState::LuaGetType() const
{
    return GetType();
}

END_NS()
