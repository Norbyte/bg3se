#include <GameDefinitions/Controllers/Action.h>

BEGIN_NS(lua)

void LuaPolymorphic<esv::ActionState>::MakeRef(lua_State* L, esv::ActionState* v, LifetimeHandle lifetime)
{
#define V(type) case esv::type::Type: \
            MakeDirectObjectRef(L, static_cast<esv::type*>(v), lifetime); break;

    switch (v->GetType()) {
        V(ASAnimation)
        V(ASMoveItem)
        V(ASKnockedDown)
        V(ASUseItem)
        V(ASCombineItem)
        V(ASTeleportFalling)
        V(ASIncapacitated)

    default:
        MakeDirectObjectRef(L, v, lifetime);
        break;
    }

#undef V
}

END_NS()

BEGIN_NS(esv)

ActionType ActionState::LuaGetType() const
{
    return GetType();
}

END_NS()
