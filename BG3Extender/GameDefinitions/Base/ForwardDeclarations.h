#pragma once

struct lua_State;

union SDL_Event;
struct SDL_Window;

struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;
struct ID3D11UnorderedAccessView;
struct ID3D11Resource;
struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11ComputeShader;
struct ID3D11DeviceContext;
struct ID3DUserDefinedAnnotation;

namespace Json
{
    class Value;
}

namespace Noesis
{
    class BaseObject;
    class BaseObservableCollection;
    class UIElementCollection;
    struct ReflectionInternals;
    struct SymbolManagerInternals;
    struct GridLengthHelper;
}

BEGIN_SE()

struct LegacyPropertyMapBase;

struct GameObjectTemplate;
struct EoCGameObjectTemplate;
struct CharacterTemplate;
struct ItemTemplate;
struct ProjectileTemplate;
struct SceneryTemplate;
struct SurfaceTemplate;
struct TriggerTemplate;
struct LevelTemplate;
struct WallConstructionTemplate;
struct LightProbeTemplate;
struct LightTemplate;
struct PhysicsTemplate;

struct LevelBase;
struct LevelDesc;
struct LevelMetaData;
struct LevelData;
struct LevelDataManager;
struct EoCLevel;
struct LevelManager;

struct ModuleInfo;
struct Module;
struct ModManager;

struct ObjectVisitor;
struct FileReader;
struct TranslatedStringRepository;

struct GlobalTemplateManager;
struct GlobalTemplateBank;
struct LocalTemplateManager;
struct CacheTemplateManagerBase;
struct GlobalCacheTemplateManager;
struct LevelCacheTemplateManager;

struct AiGrid;
struct AiPath;

struct TextKeyTypeProperties;
struct Lighting;
struct TLAutomatedLight;

struct AppliedMaterial;
struct TextureManager;
struct ResourceBank;
struct ResourceManager;
struct TextureAtlasMap;

struct BoundComponent;
struct UuidComponent;
struct UuidToHandleMappingComponent;

struct Lighting;

union TextureDescriptor;
struct MeshBinding;
struct Scene;
struct MoveableObject;
struct RenderableObject;
struct DecalObject;
struct Visual;
struct Scene;
struct Material;
struct IActionData;

struct Skeleton;
struct SkeletonBone;
struct SkeletonSocket;

struct SoundManager;
struct WwiseManager;
struct GameStateEventManager;
struct DynamicStatsExpressionManager;
struct GlobalSwitches;
struct App;

class SDLManager;

class ExtensionStateBase;

namespace rf
{
    struct Texture;
}

namespace resource
{
    struct GuidResource;
    struct GuidResourceBankBase;
    struct GuidResourceManager;

    struct Resource;
    struct AnimationBlueprintResource;
    struct VisualSet;
    struct EffectResource;
    struct TextureResource;
    struct VisualResource;
    struct Atmosphere;
}

namespace net
{
    struct Bitstream;
    struct BitstreamSerializer;
    struct Message;
    struct MessagePool;
    struct MessageFactory;
    struct Protocol;
    struct AbstractPeer;
    struct Host;
    struct Client;
    struct GameClient;
    struct GameServer;
}

namespace ecs
{
    struct EntityRef;
    struct EntityWorld;
    class EntitySystemHelpersBase;
}

namespace thoth::shared
{
    struct ConditionManager;
}

namespace input
{
    struct InputManager;
    struct InputEvent;
    struct InputEventText;
}

namespace stats
{
    struct Modifier;
    struct ModifierList;
    struct Requirement;
    struct Functor;
    struct Functors;
    struct ConditionId;
    struct SpellPrototype;
    struct PassivePrototype;
    struct RPGEnumeration;
    struct Object;
    struct TreasureTable;
    struct TreasureSubTable;
    struct TreasureCategory;
    struct RPGStats;

    struct SpellPrototype;
    struct StatusPrototype;
    struct PassivePrototype;
    struct InterruptPrototype;

    struct SpellPrototypeManager;
    struct StatusPrototypeManager;
    struct BoostPrototypeManager;
    struct PassivePrototypeManager;
    struct InterruptPrototypeManager;
}

namespace esv
{
    struct Item;
    struct Character;
    struct Level;
    struct LevelManager;

    struct Status;
    struct StatusMachine;
    struct BehaviourState;
    struct BehaviourMachine;
    struct ActionState;
    struct ActionMachine;
    struct MovementState;
    struct MovementMachine;
    struct SteeringState;
    struct SteeringMachine;
    struct TaskController;

    struct Surface;
    struct SurfaceAction;
    struct SurfaceManager;
    struct SavegameManager;
    struct EoCServer;

    class ExtensionState;

    namespace lua
    {
        class OsirisCallbackManager;
    }
}

namespace ecl
{
    struct Item;
    struct Character;
    struct Level;
    struct LevelManager;
    struct Status;
    struct StatusMachine;
    struct CursorControl;
    struct DragDropManager;
    struct InputController;
    struct EoCClient;

    class ExtensionState;

    namespace lua
    {
        class ClientState;
    }
}

class OsirisExtender;
class ScriptExtender;

// Forward declarations for custom Lua serializers
namespace lua
{
    struct LifetimeHandle;
    class Ref;
    class EntityHelper;
    class GenericPropertyMap;
    class CachedUserVariableManager;
    class CachedModVariableManager;
    class EntityReplicationEventHooks;
    class EntityComponentEventHooks;
    class State;

    struct CppObjectMetadata;
    struct CppObjectOpaque;
    struct CppValueOpaque;

    template <class T>
    class LuaDelegate;
}

namespace aspk
{
    struct Input;
    struct Component;
}

namespace gn
{
    struct GenomeManager;
    struct GenomeBlueprint;
    struct GenomeBlueprintInstance;
}

namespace phx
{
    struct PhysicsObject;
    struct PhysicsSkinnedObject;
    struct PhysicsShape;
    struct PhysicsSoftShape;
    struct PhysicsRagdoll;
    struct PhysicsSceneBase;
    struct PhysXScene;
}

namespace ui
{
    struct GameUI;
}

namespace extui
{
    class IMGUIManager;
    class IMGUIObjectManager;
    struct Renderable;
}

namespace osidbg
{
    class Debugger;
}

END_SE()
