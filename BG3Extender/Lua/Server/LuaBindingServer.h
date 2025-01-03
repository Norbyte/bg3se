#pragma once

#include <Lua/LuaBinding.h>
#include <Lua/Osiris/Binding.h>
#include <Lua/Server/ServerEntityReplicationEvents.h>
#include <GameDefinitions/Stats/Functors.h>
#include <GameDefinitions/Status.h>
#include <Extender/Shared/ExtensionHelpers.h>
#include <Lua/Shared/LuaStats.h>
#include <Lua/Server/ServerEvents.h>

BEGIN_NS(lua)

void RegisterLibraries();
void InitObjectProxyPropertyMaps();
void RegisterSharedMetatables(lua_State* L);

END_NS()

namespace bg3se::esv
{
    struct PendingHit;
    class ExtensionState;
}

BEGIN_NS(esv::lua)

using namespace ::bg3se::lua;

LifetimeHandle GetServerLifetime();
LifetimePool& GetServerLifetimePool();
void RegisterServerMetatables(lua_State* L);

class FunctorEventHooks
{
public:
    FunctorEventHooks();
    ~FunctorEventHooks();

private:
    HitResult* OnDealDamage(bg3se::stats::DealDamageFunctor::ApplyDamageProc* next, HitResult* result, bg3se::stats::DealDamageFunctor* functor, ecs::EntityRef* casterHandle,
        ecs::EntityRef* targetHandle, glm::vec3* position, bool isFromItem, SpellIdWithPrototype* spellId,
        int storyActionId, ActionOriginator* originator, resource::GuidResourceBankBase* classResourceMgr,
        HitDesc* hit, AttackDesc* attack, EntityHandle* sourceHandle2, HitWith hitWith, int conditionRollIndex,
        bool entityDamagedEventParam, __int64 a17, SpellId* spellId2);
    void OnEntityDamageEvent(bg3se::stats::StatsSystem_ThrowDamageEventProc* next, void* statsSystem, void* temp5, 
        HitDesc* hit, AttackDesc* attack, bool a5, bool a6);

    template <class TParams>
    void OnFunctorExecute(bg3se::stats::ExecuteFunctorProc<TParams>* next, HitResult* hit, bg3se::stats::Functors* self, TParams* params);
};


class ExtensionLibraryServer : public ExtensionLibrary
{
public:
    void Register(lua_State * L) override;
    void RegisterLib(lua_State * L) override;
    STDString GenerateOsiHelpers();

private:
    static char const * const NameResolverMetatableName;

    void RegisterNameResolverMetatable(lua_State * L);
    void CreateNameResolver(lua_State * L);
};

class ServerState : public State
{
public:
    static ServerState* FromLua(lua_State* L);

    ServerState(ExtensionState& state, uint32_t generationId);
    ~ServerState();

    void Initialize() override;
    bool IsClient() override;

    inline OsirisBinding& Osiris()
    {
        return osiris_;
    }

    void OnGameSessionLoaded() override;
    void StoryFunctionMappingsUpdated();

    ecs::EntityWorld* GetEntityWorld() override;
    ecs::EntitySystemHelpersBase* GetEntitySystemHelpers() override;
    EntityReplicationEventHooks* GetReplicationEventHooks() override;

    inline ServerEntityReplicationEventHooks& GetServerReplicationEventHooks()
    {
        return replicationHooks_;
    }

    template <class TArg>
    void Call(char const* mod, char const* func, std::vector<TArg> const & args)
    {
        auto L = GetState();
        LifetimeStackPin _(L, GetStack());
        lua_checkstack(L, (int)args.size() + 1);
        auto stackSize = lua_gettop(L);

        try {
            if (mod != nullptr) {
                PushModFunction(L, mod, func); // stack: func
            } else {
                lua_getglobal(L, func); // stack: func
            }

            for (auto & arg : args) {
                OsiToLua(L, arg); // stack: func, arg0 ... argn
            }

            auto status = CallWithTraceback(L, (int)args.size(), 0);
            if (status != LUA_OK) {
                LuaError("Failed to call function '" << func << "': " << lua_tostring(L, -1));
                // stack: errmsg
                lua_pop(L, 1); // stack: -
            }
        } catch (Exception &) {
            auto stackRemaining = lua_gettop(L) - stackSize;
            if (stackRemaining > 0) {
                if (mod != nullptr) {
                    LuaError("Call to mod function '" << mod << "'.'" << func << "' failed: " << lua_tostring(L, -1));
                } else {
                    LuaError("Call to mod function '" << func << "' failed: " << lua_tostring(L, -1));
                }
                lua_pop(L, stackRemaining);
            } else {
                if (mod != nullptr) {
                    LuaError("Internal error during call to mod function '" << mod << "'.'" << func << "'");
                } else {
                    LuaError("Internal error during call to mod function '" << func << "'");
                }
            }
        }
    }

    std::optional<STDString> GetModPersistentVars(STDString const& modTable);
    void RestoreModPersistentVars(STDString const& modTable, STDString const& vars);
    void OnGameStateChanged(GameState fromState, GameState toState);

    bool Query(char const* mod, char const* name, RegistryEntry * func,
        std::vector<CustomFunctionParam> const & signature, OsiArgumentDesc & params);

private:
    ExtensionLibraryServer library_;
    OsirisBinding osiris_;
    FunctorEventHooks functorHooks_;
    ServerEntityReplicationEventHooks replicationHooks_;

    bool QueryInternal(char const* mod, char const* name, RegistryEntry * func,
        std::vector<CustomFunctionParam> const & signature, OsiArgumentDesc & params);
};

END_NS()
