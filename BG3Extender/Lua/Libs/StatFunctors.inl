BEGIN_NS(lua::stats)

using namespace bg3se::stats;

Functor* CreateFunctor(lua_State* L, FunctorId type)
{
    return GetStaticSymbols().GetStats()->ConstructFunctor(type);
}

template <class T>
void DoExecuteFunctors(ExecuteFunctorProc<T>* proc, Functors* functors, HitResult& hit, T* context)
{
    if (!proc) {
        ERR("Handle not found for executing functors in context %d", context->Type);
        return;
    }

    proc(&hit, functors, context);
}

void DoExecuteInterruptFunctors(ExecuteInterruptFunctorProc* proc, ecs::EntityWorld* world, Functors* functors, HitResult& hit, InterruptContextData* context)
{
    if (!proc) {
        ERR("Handle not found for executing functors in context %d", context->Type);
        return;
    }

    proc(&hit, world, functors, context);
}

#define P(ty) \
    case FunctorContextType::ty: \
        DoExecuteFunctors(GetStaticSymbols().esv__ExecuteStatsFunctor_##ty##Context, functors, hit, static_cast<ty##ContextData*>(context)); \
        break;

void ExecuteFunctors(lua_State* L, Functors* functors, ContextData* context)
{
    OPTICK_EVENT();
    HitResult hit;
    switch (context->Type) {
    P(AttackTarget)
    P(AttackPosition)
    P(Move)
    P(Target)
    P(NearbyAttacked)
    P(NearbyAttacking)
    P(Equip)
    P(Source)

    case FunctorContextType::Interrupt:
        DoExecuteInterruptFunctors(GetStaticSymbols().esv__ExecuteStatsFunctor_InterruptContext, State::FromLua(L)->GetEntityWorld(), functors, hit, static_cast<InterruptContextData*>(context));
        break;

    default:
        ERR("Don't know how to execute functors in context %d", context->Type);
        break;
    }
}

#undef P

void ExecuteFunctor(lua_State* L, Functor* functor, ContextData* context)
{
    OPTICK_EVENT();
    Functors functors;
    functors.Insert(functor->Clone());
    ExecuteFunctors(L, &functors, context);
}

template <class T>
T* DefaultInitFunctorParams(lua_State* L)
{
    auto* helpers = State::FromLua(L)->GetEntitySystemHelpers();
    auto classDescs = helpers->GetResourceManager<resource::ClassDescription>();

    static T ctx;

    new (&ctx) T();
    ctx.Type = T::ContextType;
    ctx.PropertyContext = stats::PropertyContext::TARGET | stats::PropertyContext::AOE;
    ctx.ClassResources = *classDescs;
    return &ctx;
}

#define CTX(ty) case FunctorContextType::ty: return DefaultInitFunctorParams<ty##ContextData>(L);

ContextData* PrepareFunctorParams(lua_State* L, FunctorContextType type)
{
    switch (type) {
        CTX(AttackTarget)
        CTX(AttackPosition)
        CTX(Move)
        CTX(Target)
        CTX(NearbyAttacked)
        CTX(NearbyAttacking)
        CTX(Equip)
        CTX(Source)
        CTX(Interrupt)

    default:
        {
            luaL_error(L, "Unsupported context type");
            return nullptr;
        }
    }
}

END_NS()
