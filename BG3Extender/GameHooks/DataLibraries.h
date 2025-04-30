#pragma once

#include <GameDefinitions/Character.h>
#include <GameDefinitions/Item.h>
#include <GameDefinitions/Misc.h>
#include <GameDefinitions/Osiris.h>
#include <GameDefinitions/Symbols.h>
#include <CoreLib/SymbolMapper.h>
#include <CoreLib/Wrappers.h>
#include <optional>

namespace bg3se {

    struct GameVersionInfo
    {
        uint16_t Major{ 0 }, Minor{ 0 }, Revision{ 0 }, Build{ 0 };

        inline bool IsSupported() const
        {
            // We need v4.65 or later for game
            return (Major == 4 && Minor >= 68);
        }
    };

    class LibraryManager
    {
    public:
        LibraryManager();
        bool FindLibraries(uint32_t gameRevision);
        bool PostStartupFindLibraries();
        void ApplyCodePatches();
        bool GetGameVersion(GameVersionInfo & version);
        HMODULE GetAppHandle();

        void ShowStartupError(STDString const & msg, bool wait, bool exitGame);
        void ShowStartupError(STDString const & msg, bool exitGame);

        bool WasPatchApplied(std::string const& mapping);
        bool ApplyCodePatch(std::string const& mapping);
        bool UndoCodePatch(std::string const& mapping);

        inline bool CriticalInitializationFailed() const
        {
            return CriticalInitFailed;
        }

        inline bool InitializationFailed() const
        {
            return InitFailed;
        }

        inline SymbolMapper& Mapper()
        {
            return symbolMapper_;
        }

    private:
        void PreRegisterLibraries(SymbolMappingLoader& loader);
        void RegisterLibraries(SymbolMapper& mapper);
        void RegisterSymbols();
        bool BindApp();
        SymbolMapper::MappingResult BindECSContext(uint8_t const*);
        SymbolMapper::MappingResult BindECSIndex(uint8_t const*);
        SymbolMapper::MappingResult BindECSStaticStringConstructor(uint8_t const*);
        SymbolMapper::MappingResult BindECSStaticRegistrant(uint8_t const*);
        SymbolMapper::MappingResult BindComponentReplicationIDRef(uint8_t const*);

        bool CanShowError();
        bool CanShowMessages();

        bool ApplyPatch(SymbolMappings::Patch& patch);
        bool UndoPatch(SymbolMappings::Patch& patch);

        SymbolMappings mappings_;
        SymbolMapper symbolMapper_;
        SymbolMapper::ModuleInfo appModule_;

        bool InitFailed{ false };
        bool CriticalInitFailed{ false };
        bool PostLoaded{ false };
        bool EnabledCustomStats{ false };
    };
}
