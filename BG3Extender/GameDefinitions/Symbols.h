#pragma once

#include <GameHooks/EngineHooksFwdDecl.h>
#include <GameDefinitions/FileReader.h>

BEGIN_SE()

struct ResourceManager;
struct App;

struct IndexSymbolInfo
{
    char const* name;
    int32_t* context;
};

struct StaticSymbols : Noncopyable<StaticSymbols>
{
    std::unordered_map<int32_t*, char const*> IndexSymbolToContextMaps;
    std::unordered_map<int32_t*, IndexSymbolInfo> IndexSymbolToNameMaps;
    std::unordered_map<void const*, char const*> StaticStringRegistrantMaps;

    struct EoCLibraryInfo
    {
        uint8_t const * initFunc;
        uint8_t const * freeFunc;
        uint32_t refs;
    };

    FixedString::GetStringProc* ls__FixedString__GetString{ nullptr };
    FixedString::IncRefProc* ls__FixedString__IncRef{ nullptr };
    GlobalStringTable::MainTable::CreateFromStringProc* ls__GlobalStringTable__MainTable__CreateFromString{ nullptr };
    GlobalStringTable::MainTable::DecRefProc* ls__GlobalStringTable__MainTable__DecRef{ nullptr };
    GlobalStringTable** ls__gGlobalStringTable{ nullptr };

    ThreadRegistry::RequestThreadIndexProc* ls__ThreadRegistry__RequestThreadIndex{ nullptr };
    FileReader__CtorProc* ls__FileReader__ctor{ nullptr };
    FileReader__DtorProc* ls__FileReader__dtor{ nullptr };
    STDString** ls__PathRoots{ nullptr };
    App__CtorProc* App__Ctor{ nullptr };
    App__UpdatePathsProc* App__UpdatePaths{ nullptr };

    ecl::EoCClient** ecl__EoCClient{ nullptr };
    esv::EoCServer** esv__EoCServer{ nullptr };

    ecl::EoCClient__HandleErrorProc* ecl__EoCClient__HandleError{ nullptr };
    ecl::CursorControl** ecl__gCursorControl{ nullptr };
    ecl::DragDropManager** ls__gDragDropManager{ nullptr };

    TranslatedStringRepository** ls__gTranslatedStringRepository{ nullptr };
    TranslatedStringKeyManager** ls__gTranslatedStringKeyManager{ nullptr };

    GameStateEventManager** ecl__gGameStateEventManager{ nullptr };
    GameStateEventManager** esv__gGameStateEventManager{ nullptr };
    ecl::GameStateThreaded__GameStateWorker__DoWorkProc* ecl__GameStateThreaded__GameStateWorker__DoWork{ nullptr };
    esv::GameStateThreaded__GameStateWorker__DoWorkProc* esv__GameStateThreaded__GameStateWorker__DoWork{ nullptr };
    ecl::GameStateMachine__UpdateProc* ecl__GameStateMachine__Update{ nullptr };
    esv::GameStateMachine__UpdateProc* esv__GameStateMachine__Update{ nullptr };
    App__LoadGraphicSettingsProc* App__LoadGraphicSettings{ nullptr };
    input::InputManager** ls__gInputManager{ nullptr };
    uint8_t const* ecl__MoveController__KeyboardModeGate{ nullptr };

    ecs::EntityWorld__UpdateProc* ecs__EntityWorld__Update{ nullptr };
    ecs::EntityWorld__FlushECBsProc* ecs__EntityWorld__FlushECBs{ nullptr };

    stats::SpellPrototypeManager** eoc__SpellPrototypeManager{ nullptr };
    stats::SpellPrototype__InitProc* eoc__SpellPrototype__Init{ nullptr };

    stats::StatusPrototypeManager** eoc__StatusPrototypeManager{ nullptr };
    stats::StatusPrototype__InitProc* eoc__StatusPrototype__Init{ nullptr };

    stats::BoostPrototypeManager** eoc__BoostPrototypeManager{ nullptr };

    stats::PassivePrototypeManager** eoc__PassivePrototypeManager{ nullptr };
    stats::PassivePrototype__InitProc* eoc__PassivePrototype__Init{ nullptr };

    stats::InterruptPrototypeManager** eoc__InterruptPrototypeManager{ nullptr };
    stats::InterruptPrototype__InitProc* eoc__InterruptPrototype__Init{ nullptr };

    DynamicStatsExpressionManager** eoc__DynamicStatsExpressionManager{ nullptr };

    stats::ParseStaticBoostsProc* eoc__ParseStaticBoosts{ nullptr };

    esv::OsirisVariableHelper__SavegameVisitProc* esv__OsirisVariableHelper__SavegameVisit{ nullptr };

    stats::DealDamageFunctor__ApplyDamageProc* stats__DealDamageFunctor__ApplyDamage{ nullptr };
    esv::StatsSystem__ThrowDamageEventProc* esv__StatsSystem__ThrowDamageEvent{ nullptr };

    stats::ExecuteFunctorProc<stats::AttackTargetContextData>* esv__ExecuteStatsFunctor_AttackTargetContext{ nullptr };
    stats::ExecuteFunctorProc<stats::AttackPositionContextData>* esv__ExecuteStatsFunctor_AttackPositionContext{ nullptr };
    stats::ExecuteFunctorProc<stats::MoveContextData>* esv__ExecuteStatsFunctor_MoveContext{ nullptr };
    stats::ExecuteFunctorProc<stats::TargetContextData>* esv__ExecuteStatsFunctor_TargetContext{ nullptr };
    stats::ExecuteFunctorProc<stats::NearbyAttackedContextData>* esv__ExecuteStatsFunctor_NearbyAttackedContext{ nullptr };
    stats::ExecuteFunctorProc<stats::NearbyAttackingContextData>* esv__ExecuteStatsFunctor_NearbyAttackingContext{ nullptr };
    stats::ExecuteFunctorProc<stats::EquipContextData>* esv__ExecuteStatsFunctor_EquipContext{ nullptr };
    stats::ExecuteFunctorProc<stats::SourceContextData>* esv__ExecuteStatsFunctor_SourceContext{ nullptr };
    stats::ExecuteInterruptFunctorProc* esv__ExecuteStatsFunctor_InterruptContext{ nullptr };

    stats::RPGStats** gRPGStats{ nullptr };
    stats::RPGStats__LoadProc* RPGStats__Load{ nullptr };
    stats::RPGStats__ParseDataBuffersProc* RPGStats__ParseDataBuffers{ nullptr };
    stats::Object__SetPropertyStringProc* stats__Object__SetPropertyString{ nullptr };

    esv::LevelManager** esv__LevelManager{ nullptr };
    ecl::LevelManager** ecl__LevelManager{ nullptr };
    GlobalTemplateManager** ls__GlobalTemplateManager{ nullptr };
    LevelCacheTemplateManager** esv__CacheTemplateManager{ nullptr };

    void** esv__gSurfaceActionFactory{ nullptr };
    esv::SurfaceActionFactoryCreateProc* esv__SurfaceActionFactory__DoCreateAction{ nullptr };
    esv::TransformSurfaceAction__InitProc* esv__TransformSurfaceAction__Init{ nullptr };

    AiGrid__FindPathProc* eoc__AiGrid__FindPath{ nullptr };
    AiGrid__FindPathImmediateProc* eoc__AiGrid__FindPathImmediate{ nullptr };

    esv::SavegameManager** esv__SavegameManager{ nullptr };

    App** AppInstance{ nullptr };

    std::map<uint8_t const *, EoCLibraryInfo> Libraries;

    ls__GlobalAllocator__GetProc* ls__GlobalAllocator__Get{ nullptr };
    ls__GlobalAllocator__AllocProc* ls__GlobalAllocator__Alloc{ nullptr };
    ls__GlobalAllocator__FreeProc* ls__GlobalAllocator__Free{ nullptr };

    resource::GuidResourceManager** eoc__gGuidResourceManager{ nullptr };
    ResourceManager** ls__gGlobalResourceManager{ nullptr };
    TextureAtlasMap** ls__gTextureAtlasMap{ nullptr };

    AppliedMaterial__LoadTextureProc* ls__AppliedMaterial__LoadTexture{ nullptr };
    TextureManager__UnloadTextureProc* ls__TextureManager__UnloadTexture{ nullptr };
    AppliedMaterial__LoadVirtualTextureProc* ls__AppliedMaterial__LoadVirtualTexture{ nullptr };
    AppliedMaterial__TryOverrideTexture2DParameterProc* ls__AppliedMaterial__TryOverrideTexture2DParameter{ nullptr };

    Visual__UpdateBlendshapeWeightsFromSkeletonProc* ls__Visual__UpdateBlendshapeWeightsFromSkeleton{ nullptr };

    std::atomic<uint32_t>* Noesis__Reflection__LockedByThreadId{ nullptr };
    SRWLOCK* Noesis__Reflection__Lock{ nullptr };
    std::atomic<uint32_t>* Noesis__Reflection__NumLocks{ nullptr };
    void* Noesis__Reflection__Data{ nullptr };

    SRWLOCK* Noesis__SymbolManager__Lock{ nullptr };
    void* Noesis__SymbolManager__Data{ nullptr };
    void** Noesis__gErrorHandler{ nullptr };
    void* Noesis__GUI__LoadXaml{ nullptr };
    void* Noesis__Visual__AddVisualChild{ nullptr };
    void* Noesis__Visual__RemoveVisualChild{ nullptr };
    void* Noesis__Visual__PointFromScreen{ nullptr };
    void* Noesis__Visual__PointToScreen{ nullptr };
    void* Noesis__VisualTreeHelper__HitTest{ nullptr };

    void* ui__DataContextProvider__ExecuteCommandQueues{ nullptr };

    GlobalSwitches** ls__GlobalSwitches{ nullptr };

    FindFirstFileWProc* Kernel_FindFirstFileW{ nullptr };
    FindNextFileWProc* Kernel_FindNextFileW{ nullptr };
    FindCloseProc* Kernel_FindClose{ nullptr };

    inline StaticSymbols() {}
    StaticSymbols(StaticSymbols const &) = delete;
    StaticSymbols & operator = (StaticSymbols const &) = delete;

    inline GlobalSwitches * GetGlobalSwitches() const
    {
        if (ls__GlobalSwitches && *ls__GlobalSwitches) {
            return *ls__GlobalSwitches;
        } else {
            return nullptr;
        }
    }

    inline ecl::EoCClient * GetEoCClient() const
    {
        if (ecl__EoCClient == nullptr || *ecl__EoCClient == nullptr) {
            return nullptr;
        } else {
            return *ecl__EoCClient;
        }
    }

    inline esv::EoCServer* GetEoCServer() const
    {
        if (esv__EoCServer == nullptr || *esv__EoCServer == nullptr) {
            return nullptr;
        } else {
            return *esv__EoCServer;
        }
    }

    ModManager* GetModManagerClient() const;
    ModManager* GetModManagerServer() const;

    inline stats::RPGStats* GetStats() const
    {
        if (gRPGStats && *gRPGStats) {
            return *gRPGStats;
        } else {
            return nullptr;
        }
    }

    std::optional<ecl::GameState> GetClientState() const;
    std::optional<esv::GameState> GetServerState() const;

    inline TranslatedStringRepository* GetTranslatedStringRepository() const
    {
        if (ls__gTranslatedStringRepository != nullptr) {
            return *ls__gTranslatedStringRepository;
        } else {
            return nullptr;
        }
    }

    inline TranslatedStringKeyManager* GetTranslatedStringKeyManager() const
    {
        if (ls__gTranslatedStringKeyManager != nullptr) {
            return *ls__gTranslatedStringKeyManager;
        } else {
            return nullptr;
        }
    }

    inline esv::LevelManager* GetServerLevelManager() const
    {
        if (esv__LevelManager != nullptr
            && *esv__LevelManager != nullptr) {
            return *esv__LevelManager;
        } else {
            return {};
        }
    }

    esv::Level* GetCurrentServerLevel() const;
    FixedString GetCurrentServerLevelName() const;

    inline GlobalTemplateManager* GetGlobalTemplateManager() const
    {
        if (ls__GlobalTemplateManager) {
            return *ls__GlobalTemplateManager;
        } else {
            return nullptr;
        }
    }

    inline ecl::LevelManager* GetClientLevelManager() const
    {
        if (ecl__LevelManager != nullptr
            && *ecl__LevelManager != nullptr) {
            return *ecl__LevelManager;
        } else {
            return {};
        }
    }

    ecl::Level* GetCurrentClientLevel() const;

    GlobalTemplateBank* GetGlobalTemplateBank() const;
    ResourceBank* GetCurrentResourceBank() const;

    void CanonicalizePath(STDString & path) const;
    STDString ToPath(StringView path, PathRootType root, bool canonicalize = true) const;
    FileReaderPin MakeFileReader(StringView path, PathRootType root = PathRootType::Data, bool canonicalize = true) const;
    FileReaderPin MakeFileReaderAbsolute(StringView path) const;
    void DestroyFileReader(FileReader* reader);
    bool FileExists(StringView path, PathRootType root = PathRootType::Data, bool canonicalize = true) const;
};

extern StaticSymbols* gStaticSymbols;

void InitStaticSymbols();

inline StaticSymbols& GetStaticSymbols()
{
        return *gStaticSymbols;
}

END_SE()
