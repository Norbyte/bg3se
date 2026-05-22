#include <GameDefinitions/Components/ServerData.h>

BEGIN_NS(esv)

SurfaceAction* SurfaceManager::CreateAction(SurfaceActionType type)
{
    auto factory = GetStaticSymbols().esv__gSurfaceActionFactory;
    auto ctor = GetStaticSymbols().esv__SurfaceActionFactory__DoCreateAction;

    if (!ctor || !factory || !*factory) {
        return nullptr;
    }

    auto action = ctor(*factory, type, ComponentHandle::NullHandle);

    if (action) {
        auto helpers = GetCurrentExtensionState()->GetLua()->GetEntitySystemHelpers();
        auto classDescs = helpers->GetResourceManager<resource::ClassDescription>();
        action->ClassDescriptionMgr = *classDescs;
    }

    return action;
}

void SurfaceManager::AddAction(SurfaceAction* action)
{
    if (action->Level) {
        ERR("Surface action is already activated!");
        return;
    }

    action->Level = Level;
    action->Enter();
    Actions.push_back(action);
}

END_NS()

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
