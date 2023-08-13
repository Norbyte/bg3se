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

namespace esv
{
	struct Item;
	struct Character;
	struct Level;
	struct Status;
	struct StatusMachine;
	struct EntityWorld;
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
