#include <GameDefinitions/Components/ServerData.h>

BEGIN_NS(esv::surface)

Surface* SurfaceComponent::GetSurface(lua_State* L) const
{
    auto level = lua::State::FromLua(L)->GetExtensionState().GetLevelManager()->CurrentLevel;
    if (!level) return nullptr;

    return ((Level*)level)->SurfaceManager->Get(SurfaceHandle);
}

END_NS()

BEGIN_NS(lua)

void MakePolymorphicRef(lua_State* L, esv::SurfaceAction* v, LifetimeHandle lifetime)
{
#define V(type) case esv::type::Type: \
            MakeDirectObjectRef(L, static_cast<esv::type*>(v), lifetime); break;

    switch (v->GetTypeId()) {
        V(CreateSurfaceAction)
        V(CreatePuddleAction)
        V(ForceCreateSurfaceAction)
        V(CapsuleSurfaceAction)
        V(RemoveSurfaceAction)
        V(ZoneAction)
        V(TransformSurfaceAction)
        V(ChangeSurfaceOnPathAction)
        V(RectangleSurfaceAction)
        V(PolygonSurfaceAction)

    default:
        MakeDirectObjectRef(L, v, lifetime);
        break;
    }

#undef V
}

END_NS()
