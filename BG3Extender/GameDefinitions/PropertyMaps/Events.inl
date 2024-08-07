BEGIN_CLS(lua::EventBase)
P_RO(Name)
P_RO(CanPreventAction)
P_RO(ActionPrevented)
P_RO(Stopped)
P_FUN(StopPropagation, lua::EventBase::StopPropagation)
P_FUN(PreventAction, lua::EventBase::PreventAction)
END_CLS()

BEGIN_CLS(lua::EmptyEvent)
INHERIT(lua::EventBase)
END_CLS()

BEGIN_CLS(ecl::lua::GameStateChangedEvent)
INHERIT(lua::EventBase)
P_RO(FromState)
P_RO(ToState)
END_CLS()

BEGIN_CLS(ecl::lua::KeyInputEvent)
INHERIT(lua::EventBase)
P_RO(Event)
P_RO(Key)
P_RO(Modifiers)
P_BITMASK(Modifiers)
P_RO(Pressed)
P_RO(Repeat)
END_CLS()

BEGIN_CLS(ecl::lua::MouseButtonEvent)
INHERIT(lua::EventBase)
P_RO(Button)
P_RO(Pressed)
P_RO(Clicks)
P_RO(X)
P_RO(Y)
END_CLS()

BEGIN_CLS(ecl::lua::MouseWheelEvent)
INHERIT(lua::EventBase)
P_RO(ScrollX)
P_RO(ScrollY)
P_RO(X)
P_RO(Y)
END_CLS()

BEGIN_CLS(ecl::lua::ViewportResizedEvent)
INHERIT(lua::EventBase)
P_RO(Width)
P_RO(Height)
END_CLS()

BEGIN_CLS(esv::lua::GameStateChangedEvent)
INHERIT(lua::EventBase)
P_RO(FromState)
P_RO(ToState)
END_CLS()

BEGIN_CLS(lua::TickEvent)
INHERIT(lua::EventBase)
P(Time)
END_CLS()

BEGIN_CLS(lua::NetMessageEvent)
INHERIT(lua::EventBase)
P(Channel)
P(Payload)
P(UserID)
END_CLS()

BEGIN_CLS(lua::DoConsoleCommandEvent)
INHERIT(lua::EventBase)
P_RO(Command)
END_CLS()

BEGIN_CLS(esv::lua::DealDamageEvent)
INHERIT(lua::EventBase)
P(Functor)
P(Caster)
P(Target)
P(Position)
P(IsFromItem)
P(SpellId)
P(StoryActionId)
P(Originator)
P(Hit)
P_RENAMED(Attack, DamageSums)
P(HitWith)
END_CLS()

BEGIN_CLS(esv::lua::DealtDamageEvent)
INHERIT(esv::lua::DealDamageEvent)
P(Result)
END_CLS()

BEGIN_CLS(esv::lua::BeforeDealDamageEvent)
INHERIT(lua::EventBase)
P(Hit)
P_RENAMED(Attack, DamageSums)
END_CLS()

BEGIN_CLS(esv::lua::ExecuteFunctorEvent)
INHERIT(lua::EventBase)
P(Functor)
P(Params)
END_CLS()

BEGIN_CLS(esv::lua::AfterExecuteFunctorEvent)
INHERIT(lua::EventBase)
P(Functor)
P(Params)
P(Hit)
END_CLS()
