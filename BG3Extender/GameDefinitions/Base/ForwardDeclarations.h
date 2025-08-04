#pragma once

struct lua_State;

union SDL_Event;
struct SDL_Window;

namespace Json
{
    class Value;
}

namespace Noesis
{
    class BaseObject;
    class BaseObservableCollection;
    class UIElementCollection;
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

struct AiGrid;
struct AiPath;

struct BoundComponent;
struct UuidComponent;
struct UuidToHandleMappingComponent;

union TextureDescriptor;
struct Scene;
struct MoveableObject;
struct RenderableObject;
struct Visual;
struct Scene;
struct Material;
struct IActionData;

struct Skeleton;
struct SkeletonBone;
struct SkeletonSocket;

struct SoundManager;
struct WwiseManager;
class SDLManager;

class ExtensionStateBase;

namespace resource
{
    struct GuidResource;
    struct GuidResourceBankBase;
    struct GuidResourceManager;

    struct Resource;
    struct VisualSet;
    struct EffectResource;
    struct TextureResource;
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
}

namespace esv
{
    struct Item;
    struct Character;
    struct Level;

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
    struct Status;
    struct StatusMachine;
    struct CursorControl;
    struct DragDropManager;
    struct InputController;
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
    struct UIManager;
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
