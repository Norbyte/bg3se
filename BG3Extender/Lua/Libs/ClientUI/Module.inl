#include <Lua/Libs/ClientUI/Builtins.inl>
#include <Lua/Libs/ClientUI/NsHelpers.inl>
#include <Lua/Libs/ClientUI/CustomProperties.inl>
#include <NsGui/UIElementCollection.h>
#include <NsCore/Boxing.h>
#include <NsCore/String.h>
#include <NsGui/IList.h>
#include <GameDefinitions/DragDrop.h>
#include <GameDefinitions/Picking.h>

BEGIN_NS(lua)

#define FOR_NOESIS_TYPE(T) if (typeName == Noesis::StaticSymbol<T>()) { \
    MakeDirectObjectRef(L, static_cast<T*>(obj), lifetime); return; \
}

void NoesisPush(lua_State* L, Noesis::BaseObject* obj, LifetimeHandle lifetime)
{
    Noesis::gStaticSymbols.Initialize();

    auto cls = obj->GetClassType();

    do {
        auto typeName = cls->GetTypeId();
        FOR_EACH_NOESIS_TYPE()
        cls = cls->GetBase();
    } while (cls != nullptr);

    MakeDirectObjectRef(L, obj, lifetime);
}

#undef FOR_NOESIS_TYPE

#define FOR_NOESIS_TYPE(T) void MakePolymorphicRef(lua_State* L, T* value, LifetimeHandle lifetime) { \
    NoesisPush(L, value, lifetime); \
}

FOR_EACH_NOESIS_TYPE()
#undef FOR_NOESIS_TYPE

void MakePolymorphicRef(lua_State* L, Noesis::RoutedEventArgs* value, LifetimeHandle lifetime)
{
    auto evtName = value->routedEvent->GetName();
    auto const& events = Noesis::gStaticSymbols.Events;

    #define DEFN_EVENT(e, args) if (evtName == events.e) { MakeDirectObjectRef(L, static_cast<Noesis::args##Args*>(value), lifetime); return; }
    #include <Lua/Libs/ClientUI/Events.inl>
    #undef DEFN_EVENT

    MakeDirectObjectRef(L, value, lifetime);
}

END_NS()

/// <lua_module>UI</lua_module>
BEGIN_NS(ecl::lua::ui)

Noesis::FrameworkElement* GetRoot(lua_State* L)
{
    Noesis::gStaticSymbols.Initialize();

    if (!ClientState::FromLua(L)->GetDeferredUIEvents().IsDispatchingOnRenderThread()) {
        WARN_ONCE(
            "Accessing Noesis state (Ext.UI.GetRoot()) from the global context carries a risk of crashing; "
            "use Ext.UI.Defer(...) to ensure Noesis is only accessed from a deferred context."
        );
    }

    return (*GetStaticSymbols().ls__gGlobalResourceManager)->UI->NoesisUIManager.MainCanvas;
}

bg3se::ui::UIStateMachine* GetStateMachine()
{
    Noesis::gStaticSymbols.Initialize();
    return (*GetStaticSymbols().ls__gGlobalResourceManager)->UI->StateMachine.StateMachineComponent;
}

using FireStateEventProc = void(bg3se::ui::UIStateMachine*, bg3se::ui::UIStateMachine::EventResult&, bg3se::ui::UIStateMachine::EntityContext const&, bg3se::ui::UIStateMachine::EventArgs const&);

void SetState(lua_State* L, FixedString state, std::optional<FixedString> subState, std::optional<bool> clearState, std::optional<int16_t> playerId)
{
    ERR("Ext.UI.SetState(): Deprecated");
}

bool RegisterType(lua_State* L, StringView name, HashMap<FixedString, bg3se::ui::CustomPropertyDefn> properties,
    std::optional<StringView> wrappedContextType)
{
    Noesis::gStaticSymbols.Initialize();
    auto clsName = ClassDefinitionBuilder::MakeFullName(name);

    // Fixup names
    for (auto& prop : properties) {
        prop.Value().Name = prop.Key();
    }

    // Name conflicts with an existing Noesis type?
    if (Noesis::Reflection::GetType(clsName) != nullptr) {
        
        auto dynClass = gDynamicClasses.try_get(FixedString(clsName.Str()));
        if (!dynClass) {
            // Not an SE type, cannot replace
            luaL_error(L, "A Noesis type already exists with this name: %s", name.data());
            return false;
        }

        // If the definition didn't change, just replace the handlers without modifying the class defn
        if ((*dynClass)->MatchesDefinition(properties, wrappedContextType)) {
            (*dynClass)->UpdateHandlers(properties);
            return true;
        }

        if (gExtender->GetConfig().DeveloperMode) {
            WARN("Re-registering Noesis type '%s' with different definition - this is only supported in developer mode!", clsName.Str());
        } else {
            luaL_error(L, "Attempted to re-register Noesis type '%s' with different definition", clsName.Str());
            return false;
        }
    }

    return ClassDefinitionBuilder::RegisterNew(L, clsName, properties, wrappedContextType);
}

Noesis::BaseComponent* Instantiate(lua_State* L, STDString name, std::optional<Noesis::BaseComponent*> wrappedContext)
{
    if (name.substr(0, 4) != "se::") {
        name = "se::" + name;
    }

    auto cls = gDynamicClasses.try_get(FixedString(name));
    if (!cls) {
        luaL_error(L, "No custom class found with name '%s'", name.c_str());
        return nullptr;
    }

    auto inst = (*cls)->Construct(wrappedContext.value_or(nullptr));
    if (!inst) {
        luaL_error(L, "Unable to construct data context '%s' - invalid parameters", name.c_str());
        return nullptr;
    }

    return inst;
}

// Boxes made by Box(), kept for the session: the UI may hold on to them (a command parameter)
// after the Lua value is gone, and the same few values (event names) come back. Never destroyed:
// at exit, releasing them could come after Noesis has shut down.
static std::unordered_map<std::string, Noesis::Ptr<Noesis::BoxedValue>>& Boxes()
{
    static auto* boxes = new std::unordered_map<std::string, Noesis::Ptr<Noesis::BoxedValue>>();
    return *boxes;
}

// Puts a Lua string, boolean or number in a Noesis box (BoxedValue), the way XAML passes a
// CommandParameter, e.g. widget.DataContext.CustomEvent:Execute(Ext.UI.Box("OpenOptions"))
Noesis::BaseComponent* Box(lua_State* L, lua::AnyRef value)
{
    auto& boxes = Boxes();
    std::string key;
    Noesis::Ptr<Noesis::BoxedValue> boxed;
    switch (lua_type(L, value.Index)) {
    case LUA_TSTRING:
    {
        auto str = lua::get<char const*>(L, value.Index);
        key = std::string("s:") + str;
        if (auto it = boxes.find(key); it != boxes.end()) return it->second.GetPtr();
        boxed = Noesis::Boxing::Box(Noesis::String(str));
        break;
    }
    case LUA_TBOOLEAN:
    {
        auto b = lua_toboolean(L, value.Index) != 0;
        key = b ? "b:1" : "b:0";
        if (auto it = boxes.find(key); it != boxes.end()) return it->second.GetPtr();
        boxed = Noesis::Boxing::Box(b);
        break;
    }
    case LUA_TNUMBER:
    {
        if (lua_isinteger(L, value.Index)) {
            auto n = lua_tointeger(L, value.Index);
            if (n < INT32_MIN || n > INT32_MAX) {
                luaL_error(L, "Ext.UI.Box: integer out of the 32-bit range");
                return nullptr;
            }
            auto i = (int32_t)n;
            key = "i:" + std::to_string(i);
            if (auto it = boxes.find(key); it != boxes.end()) return it->second.GetPtr();
            boxed = Noesis::Boxing::Box(i);
        } else {
            auto f = (float)lua_tonumber(L, value.Index);
            // (keyed on the exact bits: two floats never share a box)
            uint32_t bits;
            memcpy(&bits, &f, sizeof(bits));
            key = "f:" + std::to_string(bits);
            if (auto it = boxes.find(key); it != boxes.end()) return it->second.GetPtr();
            boxed = Noesis::Boxing::Box(f);
        }
        break;
    }
    default:
        luaL_error(L, "Ext.UI.Box: expected a string, a boolean or a number");
        return nullptr;
    }

    boxes.insert({ key, boxed });
    return boxed.GetPtr();
}

PlayerPickingHelper* GetPickingHelper(uint16_t playerIndex)
{
    auto picking = ecl::ExtensionState::Get().GetClientLua()->GetEntitySystemHelpers()->GetSystem<ecl::PickingHelperManager>();
    auto it = picking->PlayerHelpers.find(playerIndex);
    if (it != picking->PlayerHelpers.end()) {
        return it.Value();
    }
    else {
        return nullptr;
    }
}

ecl::CursorControl* GetCursorControl()
{
    auto cc = GetStaticSymbols().ecl__gCursorControl;
    if (cc && *cc) {
        return *cc;
    } else {
        return nullptr;
    }
}

ecl::PlayerDragData* GetDragDrop(uint16_t playerId)
{
    auto dragDrop = GetStaticSymbols().ls__gDragDropManager;
    if (dragDrop && *dragDrop) {
        return (*dragDrop)->PlayerData.try_get(playerId);
    }

    return nullptr;
}

void NoesisErrorHandler(const char* file, uint32_t line, const char* message, bool fatal)
{
    ERR("[Noesis] %s", message);
}

void EnableErrorReporting(bool enable)
{
    auto handler = (Noesis::ErrorHandler*)GetStaticSymbols().Noesis__gErrorHandler;
    if (enable) {
        *handler = &NoesisErrorHandler;
    } else {
        *handler = nullptr;
    }
}

void Defer(lua_State* L, FunctionRef callback)
{
    ClientState::FromLua(L)->GetDeferredUIEvents().DeferNoesisThreadCallback(
        RegistryEntry(L, callback.Index)
    );
}

void RegisterUILib()
{
    DECLARE_MODULE(UI, Client)
    BEGIN_MODULE()
    MODULE_FUNCTION(GetRoot)
    MODULE_FUNCTION(GetStateMachine)
    MODULE_FUNCTION(SetState)
    MODULE_FUNCTION(RegisterType)
    MODULE_FUNCTION(Instantiate)
    MODULE_FUNCTION(Box)
    MODULE_FUNCTION(GetPickingHelper)
    MODULE_FUNCTION(GetCursorControl)
    MODULE_FUNCTION(GetDragDrop)
    MODULE_FUNCTION(Defer)
    MODULE_FUNCTION(EnableErrorReporting)
    END_MODULE()
}

END_NS()
