#pragma once

struct lua_State;

namespace Json
{
	class Value;
}

namespace Noesis
{
	class BaseObject;
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

struct UuidComponent;
struct UuidToHandleMappingComponent;

struct MoveableObject;
struct RenderableObject;
struct Visual;

namespace resource
{
	struct GuidResource;
	struct GuidResourceBankBase;
	struct GuidResourceManager;

	struct Resource;
	struct VisualSet;
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
	struct Functors;
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
}

namespace ui
{
	struct UIManager;
}

namespace osidbg
{
	class Debugger;
}

END_SE()
