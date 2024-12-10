BEGIN_NS(lua)

void LuaPolymorphic<esv::Status>::MakeRef(lua_State* L, esv::Status* v, LifetimeHandle const& lifetime)
{
#define V(type) case esv::Status##type::Type: \
            MakeDirectObjectRef(L, static_cast<esv::Status##type*>(v), lifetime); break;

    switch (v->GetStatusId()) {
        V(Dying)
        V(Heal)
        V(KnockedDown)
        V(TeleportFalling)
        V(Boost)
        V(Reaction)
        V(StoryFrozen)
        V(Sneaking)
        V(Unlock)
        V(Fear)
        V(Smelly)
        V(Invisible)
        V(Rotate)
        V(Material)
        V(Climbing)
        V(Incapacitated)
        V(InSurface)
        V(Polymorphed)
        V(Effect)
        V(Deactivated)
        V(Downed)

    default:
        MakeDirectObjectRef(L, v, lifetime);
        break;
    }

#undef V
}

void LuaPolymorphic<ecl::Status>::MakeRef(lua_State* L, ecl::Status* v, LifetimeHandle const& lifetime)
{
#define V(type) case ecl::Status##type::Type: \
            MakeDirectObjectRef(L, static_cast<ecl::Status##type*>(v), lifetime); break;

    switch (v->GetStatusId()) {
        V(Dying)
        V(Heal)
        V(KnockedDown)
        V(TeleportFalling)
        V(Boost)
        V(Reaction)
        V(StoryFrozen)
        V(Sneaking)
        V(Unlock)
        V(Fear)
        V(Smelly)
        V(Invisible)
        V(Rotate)
        V(Material)
        V(Climbing)
        V(Incapacitated)
        V(InSurface)
        V(Polymorphed)
        V(Effect)
        V(Deactivated)
        V(Downed)

    default:
        MakeDirectObjectRef(L, v, lifetime);
        break;
    }

#undef V
}

END_NS()
