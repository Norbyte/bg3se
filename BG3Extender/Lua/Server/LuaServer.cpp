#include <stdafx.h>
#include <resource.h>
#include <Lua/Shared/LuaStats.h>
#include <Lua/Shared/LuaModule.h>
#include <Lua/Server/LuaBindingServer.h>
#include <Lua/LuaSerializers.h>
#include <Extender/ScriptExtender.h>
#include <Extender/Server/ExtensionStateServer.h>
#include <Extender/Shared/ScriptHelpers.h>
#include <Extender/Shared/API.h>
#include <Lua/Debugger/LuaDebugger.h>
#include <GameDefinitions/Components/All.h>
#include <GameDefinitions/GuidResources.h>
#include <GameDefinitions/Resources.h>
#include <GameDefinitions/Net.h>
#include <GameDefinitions/Level.h>

#include <Lua/Server/FunctorEvents.inl>
#include <Lua/Server/ServerEntityReplicationEvents.inl>

namespace bg3se::esv::lua
{
    using namespace bg3se::lua;

    int RegisterOsirisListener(lua_State* L)
    {
        auto name = get<STDString>(L, 1);
        auto arity = get<int>(L, 2);
        auto typeName = get<char const*>(L, 3);
        luaL_checktype(L, 4, LUA_TFUNCTION);

        OsirisHookSignature::HookType type;
        if (strcmp(typeName, "before") == 0) {
            type = OsirisHookSignature::BeforeTrigger;
        } else if (strcmp(typeName, "after") == 0) {
            type = OsirisHookSignature::AfterTrigger;
        } else if (strcmp(typeName, "beforeDelete") == 0) {
            type = OsirisHookSignature::BeforeDeleteTrigger;
        } else if (strcmp(typeName, "afterDelete") == 0) {
            type = OsirisHookSignature::AfterDeleteTrigger;
        } else {
            luaL_error(L, "Hook type must be 'before', 'beforeDelete', 'after' or 'afterDelete'");
        }

        LuaServerPin lua(ExtensionState::Get());
        RegistryEntry handler(L, 4);
        auto subscriberId = lua->Osiris().GetOsirisCallbacks().Subscribe(name, arity, type, std::move(handler));
        push(L, subscriberId);
        return 1;
    }

    int UnregisterOsirisListener(lua_State* L)
    {
        auto subscriberId = get<uint32_t>(L, 1);

        LuaServerPin lua(ExtensionState::Get());
        push(L, lua->Osiris().GetOsirisCallbacks().Unsubscribe(subscriberId));
        return 1;
    }

    void RegisterOsirisLibrary(lua_State* L)
    {
        static const luaL_Reg extLib[] = {
            {"RegisterListener", RegisterOsirisListener},
            {"UnregisterListener", UnregisterOsirisListener},
            {0,0}
        };

        RegisterLib(L, "Osiris", extLib);
    }

    void ExtensionLibraryServer::RegisterLib(lua_State * L)
    {
        static const luaL_Reg extLib[] = {

            {0,0}
        };

        luaL_newlib(L, extLib); // stack: lib
        lua_setglobal(L, "Ext"); // stack: -

        RegisterSharedMetatables(L);
        RegisterServerMetatables(L);
        RegisterOsirisLibrary(L);
        gModuleRegistry.ConstructState(L, ModuleRole::Server);
    }

    void ExtensionLibraryServer::Register(lua_State * L)
    {
        ExtensionLibrary::Register(L);
        RegisterNameResolverMetatable(L);
        CreateNameResolver(L);
    }


    ServerState* ServerState::FromLua(lua_State* L)
    {
        return *static_cast<ServerState**>(lua_getextraspace(L));
    }

    ServerState::ServerState(ExtensionState& state, uint32_t generationId)
        : State(state, generationId, ContextType::Server),
        osiris_(state),
        functorHooks_(*this),
        replicationHooks_(*this)
    {}

    ServerState::~ServerState()
    {
        if (gExtender) {
            auto debugger = gExtender->GetLuaDebugger();
            if (debugger) {
                debugger->ServerStateDeleted();
            }

            // FIXME - HANDLE IN SERVER LOGIC!
            gExtender->GetServer().Osiris().GetCustomFunctionManager().ClearDynamicEntries();
        }
    }

    void ServerState::Initialize()
    {
        State::Initialize();

        StackCheck _(L, 0);

        library_.Register(L);

        GetExtensionState().LuaLoadBuiltinFile("ServerStartup.lua");
        // Ext is not writeable after loading SandboxStartup!
        GetExtensionState().LuaLoadBuiltinFile("SandboxStartup.lua");

        auto debugger = gExtender->GetLuaDebugger();
        if (debugger) {
            debugger->ServerStateCreated(this);
        }
    }


    std::optional<STDString> ServerState::GetModPersistentVars(STDString const& modTable)
    {
        std::tuple<std::optional<char const*>> ret;
        if (CallExtRet("_GetModPersistentVars", RestrictionFlag::All, ret, modTable)) {
            return std::get<0>(ret);
        } else {
            return {};
        }
    }


    void ServerState::RestoreModPersistentVars(STDString const& modTable, STDString const& vars)
    {
        CallExt("_RestoreModPersistentVars", RestrictionFlag::All, modTable, vars);
    }


    EntityReplicationEventHooks* ServerState::GetReplicationEventHooks()
    {
        return &replicationHooks_;
    }


    void ServerState::OnGameSessionLoaded()
    {
        osiris_.GetIdentityAdapterMap().UpdateAdapters();
        osiris_.GetNameResolver().RebuildCacheIfNecessary();

        State::OnGameSessionLoaded();
    }

    void ServerState::StoryFunctionMappingsUpdated()
    {
        auto helpers = library_.GenerateOsiHelpers();
        LoadScript(helpers, "bootstrapper");
    }
}

namespace bg3se::esv
{

    ExtensionState & ExtensionState::Get()
    {
        return gExtender->GetServer().GetExtensionState();
    }

    ExtensionState::ExtensionState()
        : ExtensionStateBase(ContextType::Server)
    {}

    ExtensionState::~ExtensionState()
    {
        if (Lua) {
            IncLuaRefs();
            ShutdownLuaState();
            DecLuaRefs(false);
        }
    }

    lua::State * ExtensionState::GetLua() const
    {
        if (Lua) {
            return Lua.get();
        } else {
            return nullptr;
        }
    }

    ModManager * ExtensionState::GetModManager() const
    {
        auto server = GetEoCServer();
        if (server) {
            return &server->ModManager;
        } else {
            return nullptr;
        }
    }

    bg3se::LevelManager* ExtensionState::GetLevelManager() const
    {
        return GetStaticSymbols().GetServerLevelManager();
    }

    void ExtensionState::OnUpdate(GameTime const& time)
    {
        OPTICK_EVENT("Server update");
        ExtensionStateBase::OnUpdate(time);
    }

    void ExtensionState::OnGameSessionLoading()
    {
        cachedPersistentVars_.clear();
        bg3se::ExtensionStateBase::OnGameSessionLoading();
    }

    void ExtensionState::Reset()
    {
        ExtensionStateBase::Reset();
    }

    void ExtensionState::ShutdownLuaState()
    {
        EnteredCheck();
        if (Lua) {
            gExtender->GetExternalAPI().OnLuaStateEvent(ContextType::Server, bg3api::APILuaStateEvent::Unloading, Lua->GetState());
            Lua->OnShutdown();
            Lua->Shutdown();
            Lua.reset();
            gExtender->GetExternalAPI().OnLuaStateEvent(ContextType::Server, bg3api::APILuaStateEvent::Unloaded, nullptr);
        }
    }

    void ExtensionState::InitializeLuaState()
    {
        se_assert(!Lua);
        context_ = nextContext_;
        EnteredCheck();

        Lua = std::make_unique<lua::ServerState>(*this, gExtender->CreateGenerationId());
        Lua->Initialize();
        Lua->StoryFunctionMappingsUpdated();
    }

    void ExtensionState::BootstrapLua()
    {
        ExtensionStateBase::BootstrapLua();

        auto gameState = GetStaticSymbols().GetServerState();
        if (gameState
            && (*gameState == esv::GameState::LoadLevel
                || (*gameState == esv::GameState::LoadModule && WasStatLoadTriggered())
                || *gameState == esv::GameState::LoadSession
                || *gameState == esv::GameState::Sync
                || *gameState == esv::GameState::Paused
                || *gameState == esv::GameState::Running)) {
            lua::EmptyEventParams evt;
            ThrowEvent<ModuleResumeEvent>(evt);
        }
    }


    std::optional<STDString> ExtensionState::GetModPersistentVars(FixedString const& mod)
    {
        auto modTable = GetModTable(mod);
        if (modTable) {
            LuaServerPin lua(*this);
            if (lua) {
                auto vars = lua->GetModPersistentVars(*modTable);
                if (vars) {
                    return vars;
                }
            }
        }

        // If we failed to retrieve vars from the Lua state for some reason, 
        // return the persistent vars we loaded from the savegame instead
        auto cachedIt = cachedPersistentVars_.find(mod);
        if (cachedIt != cachedPersistentVars_.end()) {
            ERR("Persistent variables for mod %s could not be retrieved, saving cached values!", mod.GetString());
            return cachedIt->second;
        } else {
            return {};
        }
    }

    void ExtensionState::RestoreModPersistentVars(FixedString const& mod, STDString const& vars)
    {
        cachedPersistentVars_.insert(std::make_pair(mod, vars));

        auto modTable = GetModTable(mod);
        if (modTable) {
            LuaServerPin lua(*this);
            if (lua) {
                lua->RestoreModPersistentVars(*modTable, vars);
                if (vars.size() > 3) {
                    WARN("Mod %s: PersistentVars is deprecated; consider using ModVars/UserVars instead", mod.GetString());
                }
            }
        } else {
            WARN("Savegame has persistent variables for mod %s, but it is not loaded or has no ModTable! Variables may be lost on next save!", mod.GetString());
        }
    }

    std::optional<STDString> ExtensionState::GetModTable(FixedString const& mod)
    {
        auto const& configs = GetConfigs();
        auto configIt = configs.find(mod);
        if (configIt != configs.end() && !configIt->second.ModTable.empty()) {
            return configIt->second.ModTable;
        } else {
            return {};
        }
    }

    std::unordered_set<FixedString> ExtensionState::GetPersistentVarMods()
    {
        std::unordered_set<FixedString> names;
        for (auto const& kv : cachedPersistentVars_) {
            names.insert(kv.first);
        }

        for (auto const& config : GetConfigs()) {
            if (config.second.MinimumVersion >= 4 && !config.second.ModTable.empty()) {
                names.insert(config.first);
            }
        }

        return names;
    }

    void ExtensionState::StoryLoaded()
    {
        DEBUG("ExtensionStateServer::StoryLoaded()");
        LuaServerPin lua(*this);
        if (lua) {
            lua->Osiris().StoryLoaded();
        }
    }

    void ExtensionState::StoryFunctionMappingsUpdated()
    {
        LuaServerPin lua(*this);
        if (lua) {
            lua->StoryFunctionMappingsUpdated();
        }
    }

    void ExtensionState::StorySetMerging(bool isMerging)
    {
        LuaServerPin lua(*this);
        if (lua) {
            lua->Osiris().StorySetMerging(isMerging);
        }
    }
}
