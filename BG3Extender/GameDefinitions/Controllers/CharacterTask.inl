#include <GameDefinitions/Controllers/CharacterTask.h>

BEGIN_NS(lua)

void LuaPolymorphic<ecl::CharacterTask>::MakeRef(lua_State* L, ecl::CharacterTask* v, LifetimeHandle lifetime)
{
#define V(type) case ecl::CharacterTask_##type::Type: \
            MakeDirectObjectRef(L, static_cast<ecl::CharacterTask_##type*>(v), lifetime); break;

    switch (v->TaskType) {
        V(MoveTo)
        V(ControllerSelection)
        V(MoveController)
        V(MoveInDirection)
        V(Spell)
        V(Dialog)
        V(Item_Use_Remotely)
        V(Item_Use)
        V(Drop)
        V(Item_Interact_Area)
        V(Item_Move)
        V(Item_Move_Dummy)
        V(Item_Combine)
        V(Item_Combine_Dummy)
        V(SheathUnsheath)
        V(Lockpick)
        V(Disarm_Trap)
        V(Pickpocket)
        V(PickUp)
        V(Listen)
        V(Loot)
        V(DefaultTargetAction)
        V(ClimbTo)

    default:
        MakeDirectObjectRef(L, v, lifetime);
        break;
    }

#undef V
}

void LuaPolymorphic<ecl::CharacterMoveTask>::MakeRef(lua_State* L, ecl::CharacterMoveTask* v, LifetimeHandle lifetime)
{
    return MakeObjectRef(L, static_cast<ecl::CharacterTask*>(v), lifetime);
}

END_NS()
