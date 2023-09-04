#pragma once

struct lua_State;

namespace Json
{
	class Value;
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

struct Level;

struct ModuleInfo;
struct Module;
struct ModManager;

struct ObjectVisitor;
struct FileReader;
struct TranslatedStringRepository;

struct GlobalTemplateManager;
struct LocalTemplateManager;
struct GlobalCacheTemplateManager;
struct LevelCacheTemplateManager;

namespace resource
{
	struct GuidResource;
	struct GuidResourceBankBase;
	struct GuidResourceManager;
}

namespace ecs
{
	struct EntityRef;
	struct EntityWorld;
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
}

class OsirisExtender;
class ScriptExtender;

// Forward declarations for custom Lua serializers
namespace lua
{
	struct LifetimeHandle;
	class Ref;
	class EntityProxy;
	class ComponentHandleProxy;
	//class CachedUserVariableManager;
	//class CachedModVariableManager;
}

namespace osidbg
{
	class Debugger;
}

END_SE()
