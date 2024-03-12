#include <Lua/Libs/ClientUI/NsHelpers.inl>

BEGIN_NS(lua)

#define FOR_NOESIS_TYPE(T) if (typeName == Noesis::StaticSymbol<T>()) { \
	MakeDirectObjectRef(L, static_cast<T*>(obj), lifetime); return; \
}

void NoesisPush(lua_State* L, Noesis::BaseObject* obj, LifetimeHandle const& lifetime)
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

#define FOR_NOESIS_TYPE(T) void LuaPolymorphic<T>::MakeRef(lua_State* L, T* value, LifetimeHandle const& lifetime) { \
	NoesisPush(L, value, lifetime); \
}

FOR_EACH_NOESIS_TYPE()
#undef FOR_NOESIS_TYPE

void LuaPolymorphic<Noesis::RoutedEventArgs>::MakeRef(lua_State* L, Noesis::RoutedEventArgs* value, LifetimeHandle const& lifetime)
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

Noesis::FrameworkElement* GetRoot()
{
	Noesis::gStaticSymbols.Initialize();
	return (*GetStaticSymbols().ls__gGlobalResourceManager)->UIManager->field_70.MainCanvasGrid;
}

Noesis::BaseComponent* CreateWidget(char const* path)
{
	Noesis::gStaticSymbols.Initialize();
	auto contentRoot = static_cast<Noesis::FrameworkElement*>(GetRoot()->FindNodeName("ContentRoot"));
	if (contentRoot == nullptr) {
		ERR("Failed to find content root.");
		return nullptr;
	}

	auto widget = Noesis::LoadXaml(path);
	if (!widget) return nullptr;

	contentRoot->AddVisualChild(widget.GetPtr());

	return widget.GetPtr();
}

void RegisterUILib()
{
	DECLARE_MODULE(UI, Client)
	BEGIN_MODULE()
	MODULE_FUNCTION(GetRoot)
	MODULE_FUNCTION(CreateWidget)
	END_MODULE()
}

END_NS()
