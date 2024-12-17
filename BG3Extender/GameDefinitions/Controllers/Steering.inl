#include <GameDefinitions/Controllers/Steering.h>

BEGIN_NS(lua)

void LuaPolymorphic<esv::SteeringState>::MakeRef(lua_State* L, esv::SteeringState* v, LifetimeHandle const& lifetime)
{
#define V(type) case esv::type::Type: \
            MakeDirectObjectRef(L, static_cast<esv::type*>(v), lifetime); break;

    switch (v->GetType()) {
        V(SSSteer)
        V(SSSteerTunnel)
        V(SSSteerTo)
        V(SSLookAt)

    default:
        MakeDirectObjectRef(L, v, lifetime);
        break;
    }

#undef V
}

END_NS()
