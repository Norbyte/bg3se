#pragma once

#include <Extender/Shared/ExtensionState.h>
#include <Lua/Server/LuaBindingServer.h>

namespace Json { class Value; }

namespace bg3se::esv
{
    class ExtensionState;
    using LuaServerPin = LuaStatePin<ExtensionState, lua::ServerState>;

    class ExtensionState : public ExtensionStateBase
    {
    public:
        ExtensionState();
        ~ExtensionState() override;

        void Reset() override;
        lua::State * GetLua() override;
        ModManager * GetModManager() override;
        bg3se::LevelManager* GetLevelManager() override;
        void OnUpdate(GameTime const& time) override;
        void OnGameSessionLoading() override;

        inline char const * GetBootstrapFileName() override
        {
            return "BootstrapServer.lua";
        }

        std::optional<STDString> GetModPersistentVars(FixedString const& mod);
        void RestoreModPersistentVars(FixedString const& mod, STDString const& vars);
        std::unordered_set<FixedString> GetPersistentVarMods();
        std::optional<STDString> GetModTable(FixedString const& mod);

        void StoryLoaded();
        void StoryFunctionMappingsUpdated();
        void StorySetMerging(bool isMerging);

        static ExtensionState & Get();

    protected:
        friend LuaStatePin<ExtensionState, lua::ServerState>;
        std::unique_ptr<lua::ServerState> Lua;
        std::unordered_map<FixedString, STDString> cachedPersistentVars_;
        uint32_t nextGenerationId_{ 1 };

        void ShutdownLuaState() override;
        void InitializeLuaState() override;
        void BootstrapLua() override;
    };
}