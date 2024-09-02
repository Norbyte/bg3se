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
struct SurfaceTemplate;
struct TriggerTemplate;
struct LevelTemplate;
struct WallConstructionTemplate;
struct LightProbeTemplate;
struct LightTemplate;

struct LevelBase;
struct EoCLevel;

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

struct UuidComponent;
struct UuidToHandleMappingComponent;

union TextureDescriptor;
struct MoveableObject;
struct RenderableObject;
struct Visual;
struct Scene;
struct Material;
struct IActionData;

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
	struct GameServer;
}

namespace ecs
{
	struct EntityRef;
	struct EntityWorld;
	class EntitySystemHelpersBase;
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
	struct CppValueMetadata;

	template <class T>
	class LuaDelegate;
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
