#include <Lua/Shared/LuaMethodCallHelpers.h>
#include <GameDefinitions/UI.h>

BEGIN_NS(ecl::lua)

struct DummyDelegate
{
	void Handler(Noesis::BaseComponent* o, const RoutedEventArgs& args)
	{
		LuaClientPin lua(gExtender->GetClient().GetExtensionState());
		if (lua) {
			auto index = (UIEventHooks::SubscriptionIndex)(uintptr_t)this;
			lua->GetUIEvents().EventFired(index, o, args);
		}
	}
};

UIEventHooks::EventHandler::~EventHandler()
{
	Unsubscribe();
}

UIEventHooks::EventHandler::EventHandler(EventHandler&& o)
	: Target(std::move(o.Target)),
	Event(o.Event),
	EventType(o.EventType),
	Handler(std::move(o.Handler)),
	Index(o.Index)
{
	o.Target.Reset();
}

UIEventHooks::EventHandler& UIEventHooks::EventHandler::operator = (EventHandler&& o)
{
	Target = std::move(o.Target);
	Event = o.Event;
	EventType = o.EventType;
	Handler = std::move(o.Handler);
	Index = o.Index;

	o.Target.Reset();
	return *this;
}

void UIEventHooks::EventHandler::Unsubscribe()
{
	if (Target) {
		auto event = Target->mRoutedEventHandlers.Find(EventType);
		if (event != Target->mRoutedEventHandlers.End()) {
			auto self = (DummyDelegate*)(uintptr_t)(Index);
			event->value.Remove(RoutedEventHandler { self, & DummyDelegate::Handler });
		}

		Target.Reset();
	}
}

UIEventHooks::UIEventHooks(ClientState& state)
	: state_(state)
{}

UIEventHooks::~UIEventHooks()
{}

UIEventHooks::SubscriptionIndex UIEventHooks::Subscribe(UIElement* target, RoutedEvent const* event, Symbol eventName, RegistryEntry&& hook)
{
	SubscriptionIndex index;
	auto sub = subscriptions_.Add(index);
	auto self = (DummyDelegate*)(uintptr_t)(index);

	auto handlers = target->mRoutedEventHandlers.Find(event);
	if (handlers == target->mRoutedEventHandlers.End()) {
		target->mRoutedEventHandlers.Insert(event, RoutedEventHandler{ self, &DummyDelegate::Handler });
	} else {
		handlers->value.Add(RoutedEventHandler{ self, &DummyDelegate::Handler });
	}

	sub->Target.Reset(target);
	sub->Event = eventName;
	sub->EventType = event;
	sub->Handler = std::move(hook);
	sub->Index = index;

	return index;
}

bool UIEventHooks::Unsubscribe(SubscriptionIndex index)
{
	auto sub = subscriptions_.Find(index);
	if (sub == nullptr) {
		return false;
	}

	sub->Unsubscribe();
	subscriptions_.Free(index);
	return true;
}

void UIEventHooks::EventFired(SubscriptionIndex index, Noesis::BaseComponent* target, const RoutedEventArgs& args)
{
	auto sub = subscriptions_.Find(index);
	if (sub == nullptr) {
		return;
	}

	auto L = state_.GetState();
	sub->Handler.Push();
	Ref func(L, lua_absindex(L, -1));

	auto eventArgs = const_cast<RoutedEventArgs*>(&args);
	ProtectedFunctionCaller<std::tuple<Noesis::BaseComponent*, RoutedEventArgs*>, void> caller{ func, std::tuple(target, eventArgs) };
	caller.Call(L, "UI event dispatch");
	lua_pop(L, 1);
}

END_NS()
