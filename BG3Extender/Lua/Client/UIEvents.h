#pragma once

#include <NsGui/UIElement.h>
#include <Lua/Shared/LuaReference.h>

BEGIN_NS(ecl::lua)

using namespace Noesis;
using namespace bg3se::lua;

class UIEventHooks
{
public:
    using SubscriptionIndex = uint32_t;

    UIEventHooks(ClientState& state);
    ~UIEventHooks();

    SubscriptionIndex Subscribe(UIElement* target, RoutedEvent const* event, bg3se::FixedString const& eventName, RegistryEntry&& hook);
    bool Unsubscribe(SubscriptionIndex index);
    void EventFired(SubscriptionIndex index, Noesis::BaseComponent* target, const RoutedEventArgs& args);

private:
    struct EventHandler
    {
        inline EventHandler() {}
        ~EventHandler();

        EventHandler(EventHandler const&) = delete;
        EventHandler(EventHandler &&) noexcept;
        EventHandler& operator = (EventHandler const&) = delete;
        EventHandler& operator = (EventHandler&&) noexcept;

        void Unsubscribe();

        Ptr<UIElement> Target;
        bg3se::FixedString Event;
        RoutedEvent const* EventType;
        RegistryEntry Handler;
        SubscriptionIndex Index;
        bool IsActive{ false };
    };

    ClientState& state_;
    SaltedPool<EventHandler> subscriptions_;
};

END_SE()
