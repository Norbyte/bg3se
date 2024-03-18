#include <Lua/Libs/ClientUI/NsHelpers.inl>
#include <NsGui/UIElementCollection.h>
#include <NsGui/IList.h>

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

using FireStateEventProc = void(bg3se::ui::UIStateMachine*, bg3se::ui::UIStateMachine::EventResult&, bg3se::ui::UIStateMachine::EntityContext const&, bg3se::ui::UIStateMachine::EventArgs const&);

void SetState(lua_State* L, FixedString state, std::optional<FixedString> subState, std::optional<bool> clearState, std::optional<int16_t> playerId)
{
	auto stateMachine = (*GetStaticSymbols().ls__gGlobalResourceManager)->UIManager->field_358.StateMachine;
	auto fireStateEvent = (FireStateEventProc*)GetStaticSymbols().ls__UIStateMachine__FireStateEvent2;

	bg3se::ui::UIStateMachine::EventResult result;
	
	bg3se::ui::UIStateMachine::ECSData ecs;
	auto world = State::FromLua(L)->GetEntityWorld();
	ecs.EntityWorld = world;
	ecs.EntityWorld2 = world;
	ecs.EntityTypes = world->EntityTypes;
	ecs.QuerySystem = &world->Queries;
	ecs.CriticalSection = &world->CS;

	bg3se::ui::UIStateMachine::EntityContext context;
	context.ECS = &ecs;

	bg3se::ui::UIStateMachine::EventArgs args;
	args.StateEvent = state;
	if (subState) {
		args.SubState = *subState;
	}

	if (clearState) {
		args.RemoveState = *clearState;
	}

	if (playerId) {
		args.PlayerId = *playerId;
	}

	fireStateEvent(stateMachine, result, context, args);
}

void RegisterUILib()
{
	DECLARE_MODULE(UI, Client)
	BEGIN_MODULE()
	MODULE_FUNCTION(GetRoot)
	MODULE_FUNCTION(SetState)
	END_MODULE()
}

END_NS()
