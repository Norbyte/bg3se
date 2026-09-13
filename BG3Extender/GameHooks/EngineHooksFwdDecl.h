#pragma once

// Forward declarations to ensure we don't need to pull full headers for hook definitions

using FindFirstFileWProc = WINBASEAPI HANDLE WINAPI(_In_ LPCWSTR lpFileName, _Out_ LPWIN32_FIND_DATAW lpFindFileData);
using FindNextFileWProc = WINBASEAPI BOOL WINAPI(_In_ HANDLE hFindFile, _Out_ LPWIN32_FIND_DATAW lpFindFileData);
using FindCloseProc = WINBASEAPI BOOL WINAPI(_Inout_ HANDLE hFindFile);


BEGIN_SE()

struct HitResult;
struct HitDesc;
struct AttackDesc;

struct FileReader;
struct AiGrid;
struct Visual;
struct CameraGlobalSwitches;

using AiPathId = int32_t;

using FileReader__CtorProc = FileReader* (FileReader* self, Path const& path, unsigned int type, unsigned int unknown);
using FileReader__DtorProc = void (FileReader* self);

using AiGrid__FindPathProc = void(AiGrid* self, AiPathId pathId);
using AiGrid__FindPathImmediateProc = bool(AiGrid* self, AiPathId pathId);

using Visual__UpdateBlendshapeWeightsFromSkeletonProc = void(Visual* self);

using App__LoadGraphicSettingsProc = void (App*);
using App__CtorProc = App* (App*);
using App__UpdatePathsProc = void (App*);

using AppliedMaterial__LoadTextureProc = TextureDescriptor * (AppliedMaterial* self, FixedString const& textureGuid);
using AppliedMaterial__LoadVirtualTextureProc = void* (AppliedMaterial* self, FixedString const& textureGuid);
using AppliedMaterial__TryOverrideTexture2DParameterProc = bool (AppliedMaterial* self, FixedString const& param, FixedString const& textureGuid);

using TextureManager__UnloadTextureProc = bool (TextureManager* self, FixedString const& textureGuid);

END_SE()

BEGIN_NS(input)

struct InputEvent;

END_NS()

BEGIN_NS(stats)

struct DealDamageFunctor;
struct Functors;

struct AttackTargetContextData;
struct AttackPositionContextData;
struct MoveContextData;
struct TargetContextData;
struct NearbyAttackedContextData;
struct NearbyAttackingContextData;
struct EquipContextData;
struct SourceContextData;
struct InterruptContextData;

using RPGStats__LoadProc = void(RPGStats* self, Array<STDString>* paths);
using RPGStats__ParseStructureFolderProc = void(RPGStats* self, Array<STDString>* paths);
using RPGStats__ParseDataBuffersProc = bool (RPGStats* self);

using Object__SetPropertyStringProc = void (Object* self, FixedString const& propertyName, char const* value);

using DealDamageFunctor__ApplyDamageProc = HitResult* (HitResult* result, DealDamageFunctor* functor, ecs::EntityRef* casterHandle,
    ecs::EntityRef* targetHandle, glm::vec3* position, bool isFromItem, SpellIdWithPrototype* spellId,
    int storyActionId, ActionOriginator* originator, resource::GuidResourceBankBase* classResourceMgr,
    HitDesc* hit, AttackDesc* attack, EntityHandle* sourceHandle2, HitWith hitWith, int conditionRollIndex,
    bool entityDamagedEventParam, __int64 a17, SpellId* spellId2);

template <class TContext>
using ExecuteFunctorProc = void (HitResult* hit, Functors* self, TContext* params);

using ExecuteInterruptFunctorProc = void(HitResult* hit, ecs::EntityWorld* world, Functors* self, InterruptContextData* params);

using SpellPrototype__InitProc = void (SpellPrototype* self, FixedString const& spellId);
using StatusPrototype__InitProc = void (StatusPrototype* self, FixedString const& statusId, uint8_t flags);
using PassivePrototype__InitProc = void (PassivePrototype* self, stats::Object* stats);
using InterruptPrototype__InitProc = void (InterruptPrototype* self, stats::Object* stats);

using ParseStaticBoostsProc = bool(LSStringView& str, Array<Guid>& boosts, void* temp);

END_NS()

BEGIN_NS(ecs)

struct EntityWorld;

using EntityWorld__UpdateProc = void (EntityWorld* self, GameTime const& time);
using EntityWorld__FlushECBsProc = bool (EntityWorld* self);

END_NS()

BEGIN_NS(esv)

struct OsirisVariableHelper;
struct SavegameVisitor;
struct StatsSystem;
struct SurfaceAction;
struct TransformSurfaceAction;

using OsirisVariableHelper__SavegameVisitProc = bool(OsirisVariableHelper*, SavegameVisitor*);

using StatsSystem__ThrowDamageEventProc = void(StatsSystem* statsSystem, void* temp5, HitDesc* hit, AttackDesc* attack, bool a5, bool a6);

using SurfaceActionFactoryCreateProc = SurfaceAction* (void* self, SurfaceActionType type, uint64_t actionHandle);
using TransformSurfaceAction__InitProc = void (TransformSurfaceAction*, SurfaceTransformActionType transformAction, SurfaceLayer8 layer, SurfaceType originSurface);

using GameStateEventManager__ExecuteGameStateChangedEventProc = void (void* self, GameState fromState, GameState toState);
using GameStateThreaded__GameStateWorker__DoWorkProc = void (void* self);
using GameStateMachine__UpdateProc = void (void* self, GameTime* time);

END_NS()

BEGIN_NS(ecl)

struct GameCameraBehavior;
struct CameraUpdateTime;
struct CameraUpdateComponentView;
struct CameraZoomUpdateComponentView;
struct CameraInputComponentView;
struct CameraDefaultZoomComponentView;

using EoCClient__HandleErrorProc = void (EoCClient* self, TranslatedString const& message, bool exitGame, TranslatedString const& a4, uint8_t messageBoxLocalId);

using GameStateEventManager__ExecuteGameStateChangedEventProc = void (void* self, GameState fromState, GameState toState);
using GameStateThreaded__GameStateWorker__DoWorkProc = void (void* self);
using GameStateMachine__UpdateProc = void (void* self, GameTime* time);
using CameraSystem__UpdateCameraProc = void (void* self, void* systemContext, CameraUpdateTime const* time, CameraUpdateComponentView* view);
using CameraSystem__UpdateCameraPitchProc = void (void* self, void* entity, GameCameraBehavior* camera, CameraUpdateTime const* time);
using CameraSystem__UpdateCameraZoomProc = void (void* self, void* entity, CameraZoomUpdateComponentView* view, CameraUpdateTime const* time);
using GameCameraBehavior__GetCameraDefinitionProc = CameraGlobalSwitches* (GameCameraBehavior const* camera);
using GameCameraBehavior__CalculatePitchProc = float (GameCameraBehavior* camera, bool ignoreAvoidance, bool selectMode);
using CameraSystem__HandleInputProc = uint16_t* (void* self, uint16_t* result, CameraInputComponentView* view, input::InputEvent* inputEvent);
using CameraSystem__SetDefaultZoomProc = void (CameraDefaultZoomComponentView* view);

END_NS()

BEGIN_NS(ui)

struct DataContextProvider;

using DataContextProvider__ExecuteCommandQueuesProc = void(DataContextProvider*, void* /* ls::AnyView* */);

END_NS()
