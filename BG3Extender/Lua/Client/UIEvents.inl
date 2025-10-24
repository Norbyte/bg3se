#include <Lua/Shared/LuaMethodCallHelpers.h>
#include <GameDefinitions/UI.h>

BEGIN_NS(ecl::lua)

struct DummyDelegate
{
    void Handler(Noesis::BaseComponent* o, const RoutedEventArgs& args)
    {
        ContextGuardAnyThread ctx(ContextType::Client);
        if (gExtender->GetClient().HasExtensionState()) {
            LuaClientPin lua(gExtender->GetClient().GetExtensionState());
            if (lua) {
                auto index = (UIEventHooks::SubscriptionIndex)(uintptr_t)this;
                lua->GetUIEvents().EventFired(index, o, args);
            }
        }
    }
};

UIEventHooks::EventHandler::~EventHandler()
{
    Unsubscribe();
}

UIEventHooks::EventHandler::EventHandler(EventHandler&& o) noexcept
    : Target(std::move(o.Target)),
    Event(o.Event),
    EventType(o.EventType),
    Handler(std::move(o.Handler)),
    Index(o.Index)
{
    o.Target.Reset();
}

UIEventHooks::EventHandler& UIEventHooks::EventHandler::operator = (EventHandler&& o) noexcept
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
    IsActive = true;

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

UIEventHooks::SubscriptionIndex UIEventHooks::Subscribe(UIElement* target, RoutedEvent const* event, bg3se::FixedString const& eventName, RegistryEntry&& hook)
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

    // HACK - don't set reference to avoid dtor crash in patch 8+
    sub->Target.Reset();
    sub->Event = eventName;
    sub->EventType = event;
    sub->Handler = std::move(hook);
    sub->Index = index;
    sub->IsActive = true;

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
    if (sub == nullptr || !sub->IsActive) {
        return;
    }

    auto L = state_.GetState();
    sub->Handler.Push(L);
    Ref func(L, lua_absindex(L, -1));

    auto eventArgs = const_cast<RoutedEventArgs*>(&args);
    ProtectedFunctionCaller<std::tuple<Noesis::BaseComponent*, RoutedEventArgs*>, void> caller{ func, std::tuple(target, eventArgs) };
    caller.Call(L, "UI event dispatch");
    lua_pop(L, 1);
}




DeferredUIEvents::DeferredUIEvents(ClientState& state)
    : state_(state)
{}

void DeferredUIEvents::PostUpdate()
{
    Array<DeferredCommand> commands;
    Array<DeferredPropertyChange> propertyChanges;
    // Avoid corruption if commands/prop changes are queued during update
    std::swap(commands, commands_);
    std::swap(propertyChanges, propertyChanges_);

    auto L = state_.GetState();
    for (auto const& command : commands) {
        LuaDelegate<void(Noesis::BaseCommand*, Noesis::BaseComponent*)> handler(L, command.Handler.ToRef(L));
        handler.Call(L, { command.Command.GetPtr(), command.Parameter.GetPtr() });
    }

    for (auto const& change : propertyChanges) {
        LuaDelegate<void(Noesis::BaseComponent*, Noesis::Symbol)> handler(L, change.Handler.ToRef(L));
        handler.Call(L, { change.Object.GetPtr(), change.Property });
    }
}

void DeferredUIEvents::OnCommand(lua::PersistentRegistryEntry const& handler, Noesis::BaseCommand* command, Noesis::BaseComponent* parameter)
{
    commands_.push_back(DeferredCommand{
        .Handler = handler,
        .Command = Noesis::Ptr(command),
        .Parameter = Noesis::Ptr(parameter)
    });
}

void DeferredUIEvents::OnPropertyChanged(lua::PersistentRegistryEntry const& handler, Noesis::BaseComponent* object, Noesis::Symbol property)
{
    propertyChanges_.push_back(DeferredPropertyChange{
        .Handler = handler,
        .Object = Noesis::Ptr(object),
        .Property = property
    });
}

END_NS()
