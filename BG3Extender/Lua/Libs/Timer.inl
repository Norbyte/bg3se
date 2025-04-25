#include <Lua/Libs/Timer.h>

/// <lua_module>Timer</lua_module>
BEGIN_NS(lua::timer)

void TimerManager::BaseTimer::SavegameVisit(ObjectVisitor* visitor)
{
    STDString nullStr;
    if (visitor->IsReading()) {
        visitor->VisitDouble(GFS.strTime, Time, .0);
        visitor->VisitFloat(GFS.strTime, FrozenTime, .0f);
        visitor->VisitFloat(GFS.strTime, Repeat, .0f);
        visitor->VisitBool(GFS.strTime, Paused, false);
    } else {
        visitor->VisitDouble(GFS.strTime, Time, .0);
        visitor->VisitFloat(GFS.strTime, FrozenTime, .0f);
        visitor->VisitFloat(GFS.strTime, Repeat, .0f);
        visitor->VisitBool(GFS.strTime, Paused, false);
    }
}

void TimerManager::BaseTimer::Start(double time, float repeat)
{
    Time = time;
    FrozenTime = .0f;
    Repeat = repeat;
    Paused = false;
    InvokeId = 0;
}

void TimerManager::BaseTimer::Pause(double time)
{
    assert(!Paused);
    FrozenTime = (float)std::max(Time - time, .0);
    Time = .0f;
    Paused = true;
}

void TimerManager::BaseTimer::Resume(double time)
{
    assert(Paused);
    Time = time + FrozenTime;
    FrozenTime = .0f;
    Paused = false;
    InvokeId++;
}

void TimerManager::PersistentTimer::SavegameVisit(ObjectVisitor* visitor)
{
    BaseTimer::SavegameVisit(visitor);

    if (visitor->IsReading()) {
        visitor->VisitFixedString(GFS.strHandler, Callback, GFS.strEmpty);
        visitor->VisitSTDString(GFS.strArgs, ArgsJson, STDString{});
    } else {
        visitor->VisitFixedString(GFS.strHandler, Callback, GFS.strEmpty);
        visitor->VisitSTDString(GFS.strArgs, ArgsJson, STDString{});
    }
}

TimerManager::TimerManager(State& state, lua::DeferredLuaDelegateQueue& queue)
    : state_(state), eventQueue_(queue)
{}

TimerHandle TimerManager::Add(double time, Ref callback, float repeat)
{
    uint32_t id;
    auto timer = ephemeralTimers_.Add(id);
    timer->Start(time, repeat);
    timer->Callback = LuaDelegate<void(TimerHandle)>(state_.GetState(), callback);

    TimerHandle handle{ id };
    QueueTimer(handle, *timer);
    return handle;
}

TimerHandle TimerManager::AddPersistent(double time, FixedString const& callback, StringView argsJson)
{
    uint32_t id;
    auto timer = persistentTimers_.Add(id);
    timer->Start(time);
    timer->Callback = callback;
    timer->ArgsJson = argsJson;

    TimerHandle handle{ (uint64_t)id | PersistentFlag };
    QueueTimer(handle, *timer);
    return handle;
}

TimerHandle TimerManager::RestorePersistent(PersistentTimer const& t)
{
    uint32_t id;
    auto timer = persistentTimers_.Add(id);
    *timer = t;

    TimerHandle handle{ (uint64_t)id | PersistentFlag };
    if (!timer->Paused) {
        QueueTimer(handle, *timer);
    }

    return handle;
}

void TimerManager::RegisterPersistentCallback(FixedString const& name, Ref callback)
{
    persistentCallbacks_.set(name, LuaDelegate<void(RegistryEntry, TimerHandle)>(state_.GetState(), callback));
}

bool TimerManager::Cancel(TimerHandle handle)
{
    if (handle & PersistentFlag) {
        return persistentTimers_.Free((uint32_t)handle);
    } else {
        return ephemeralTimers_.Free((uint32_t)handle);
    }
}

bool TimerManager::Pause(TimerHandle handle, double curTime)
{
    BaseTimer* timer{ nullptr };
    if (handle & PersistentFlag) {
        timer = persistentTimers_.Find((uint32_t)handle);
    } else {
        timer = ephemeralTimers_.Find((uint32_t)handle);
    }

    if (timer) {
        if (!timer->Paused) {
            timer->Pause(curTime);
        } else {
            WARN("Tried to pause timer '%ld' that is already paused!", handle);
        }
        return true;
    } else {
        return false;
    }
}

bool TimerManager::Resume(TimerHandle handle, double curTime)
{
    BaseTimer* timer{ nullptr };
    if (handle & PersistentFlag) {
        timer = persistentTimers_.Find((uint32_t)handle);
    } else {
        timer = ephemeralTimers_.Find((uint32_t)handle);
    }

    if (timer) {
        if (timer->Paused) {
            timer->Resume(curTime);
            QueueTimer(handle, *timer);
        } else {
            WARN("Tried to resume timer '%ld' that is not paused!", handle);
        }
        return true;
    } else {
        return false;
    }
}

void TimerManager::Update(double time)
{
    while (!queue_.empty() && queue_.top().Time <= time) {
        auto entry = queue_.top();
        queue_.pop();

        FireTimer(entry, time);
    }
}

void TimerManager::FireTimer(TimerQueueEntry const& entry, double time)
{
    if (entry.Handle & PersistentFlag) {
        auto timer = persistentTimers_.Find((uint32_t)entry.Handle);
        if (timer != nullptr && entry.Matches(*timer)) {
            auto callback = persistentCallbacks_.try_get(timer->Callback);
            if (callback) {
                auto L = state_.GetState();
                if (json::Parse(L, timer->ArgsJson)) {
                    RegistryEntry args(L, -1);
                    lua_pop(L, 1);
                    eventQueue_.Call(*callback, std::move(args), entry.Handle);
                } else {
                    ERR("Unable to parse persistent timer payload for '%s'!", timer->Callback.GetString());
                }
            } else {
                WARN("Tried to fire persistent timer '%s' but it has no callback registered!", timer->Callback.GetString());
            }
            
            RepeatOrReleaseTimer(entry.Handle, *timer, time);
        }
    } else {
        auto timer = ephemeralTimers_.Find((uint32_t)entry.Handle);
        if (timer != nullptr && entry.Matches(*timer)) {
            eventQueue_.Call(timer->Callback, entry.Handle);
            RepeatOrReleaseTimer(entry.Handle, *timer, time);
        }
    }
}

void TimerManager::RepeatOrReleaseTimer(TimerHandle handle, BaseTimer& timer, double time)
{
    if (timer.Repeat > 0.0f) {
        timer.Time = time + timer.Repeat;
        QueueTimer(handle, timer);
    } else {
        if (handle & PersistentFlag) {
            persistentTimers_.Free((uint32_t)handle);
        } else {
            ephemeralTimers_.Free((uint32_t)handle);
        }
    }
}

void TimerManager::QueueTimer(TimerHandle handle, BaseTimer const& timer)
{
    assert(!timer.Paused);
    queue_.push(TimerQueueEntry{
        .Time = timer.Time,
        .Handle = handle,
        .InvokeId = timer.InvokeId
    });
}

void TimerManager::SavegameVisit(ObjectVisitor* visitor)
{
    STDString nullStr;
    if (visitor->IsReading()) {
        uint32_t numVars;
        visitor->VisitCount(GFS.strTimer, &numVars);

        for (uint32_t i = 0; i < numVars; i++) {
            if (visitor->EnterNode(GFS.strTimer, GFS.strEmpty)) {
                PersistentTimer timer;
                timer.SavegameVisit(visitor);
                RestorePersistent(timer);
            }
        }
    } else {
        uint32_t index{ 0 };
        while (true) {
            auto timer = persistentTimers_.Next(index);
            if (timer == nullptr) break;

            if (visitor->EnterNode(GFS.strTimer, GFS.strEmpty)) {
                timer->SavegameVisit(visitor);
                visitor->ExitNode(GFS.strTimer);
            }
        }
    }
}


TimerSystem::TimerSystem(State& state, bool isServer)
    : realtime_(state, eventQueue_),
    game_(state, eventQueue_),
    isServer_(isServer)
{}

void TimerSystem::Update(double time)
{
    realtime_.Update(time);
    game_.Update(time);
    eventQueue_.Flush();
}

bool TimerSystem::Cancel(TimerHandle handle)
{
    if (handle & TimerManager::RealtimeFlag) {
        return realtime_.Cancel(handle);
    } else {
        return game_.Cancel(handle);
    }
}

bool TimerSystem::Pause(TimerHandle handle, double time)
{
    if (handle & TimerManager::RealtimeFlag) {
        return realtime_.Pause(handle, time);
    } else {
        return game_.Pause(handle, time);
    }
}

bool TimerSystem::Resume(TimerHandle handle, double time)
{
    if (handle & TimerManager::RealtimeFlag) {
        return realtime_.Resume(handle, time);
    } else {
        return game_.Resume(handle, time);
    }
}

void TimerSystem::SavegameVisit(ObjectVisitor* visitor)
{
    if (visitor->EnterNode(GFS.strPersistentTimers, GFS.strEmpty)) {
        if (visitor->EnterNode(GFS.strGameTimers, GFS.strEmpty)) {
            game_.SavegameVisit(visitor);
            visitor->ExitNode(GFS.strGameTimers);
        }
        
        if (visitor->EnterNode(GFS.strRealtimeTimers, GFS.strEmpty)) {
            realtime_.SavegameVisit(visitor);
            visitor->ExitNode(GFS.strRealtimeTimers);
        }

        visitor->ExitNode(GFS.strPersistentTimers);
    }
}


TimerHandle WaitFor(lua_State* L, float delay, Ref callback, std::optional<float> repeat)
{
    auto state = State::FromLua(L);
    double time = ExtensionStateBase::FromLua(L).Time().Time + delay / 1000.0f;

    return state->GetTimers().GameTimer().Add(time, callback, repeat ? (*repeat / 1000.0f) : 0.0f);
}

TimerHandle WaitForPersistent(lua_State* L, float delay, FixedString callback, Ref args)
{
    auto state = State::FromLua(L);
    double time = ExtensionStateBase::FromLua(L).Time().Time + delay / 1000.0f;

    if (!state->GetTimers().SupportsPersistence()) {
        luaL_error(L, "Persistent timers are only supported on the server");
    }

    json::StringifyContext ctx;
    ctx.Beautify = false;
    auto str = json::Stringify(L, ctx, args.Index());
    return state->GetTimers().GameTimer().AddPersistent(time, callback, str);
}

TimerHandle WaitForRealtime(lua_State* L, float delay, Ref callback, std::optional<float> repeat)
{
    auto state = State::FromLua(L);
    double time = ExtensionStateBase::FromLua(L).Time().Time + delay / 1000.0f;

    auto handle = state->GetTimers().RealtimeTimer().Add(time, callback, repeat ? (*repeat / 1000.0f) : 0.0f);
    return handle | TimerManager::RealtimeFlag;
}

void RegisterPersistentHandler(lua_State* L, FixedString name, Ref callback)
{
    return State::FromLua(L)->GetTimers().GameTimer().RegisterPersistentCallback(name, callback);
}

bool Pause(lua_State* L, TimerHandle handle)
{
    double time = ExtensionStateBase::FromLua(L).Time().Time;
    return State::FromLua(L)->GetTimers().Pause(handle, time);
}

bool Resume(lua_State* L, TimerHandle handle)
{
    double time = ExtensionStateBase::FromLua(L).Time().Time;
    return State::FromLua(L)->GetTimers().Resume(handle, time);
}

bool Cancel(lua_State* L, TimerHandle handle)
{
    return State::FromLua(L)->GetTimers().Cancel(handle);
}

void RegisterTimerLib()
{
    DECLARE_MODULE(Timer, Both)
    BEGIN_MODULE()
    MODULE_FUNCTION(WaitFor)
    MODULE_FUNCTION(WaitForPersistent)
    MODULE_FUNCTION(WaitForRealtime)
    MODULE_FUNCTION(RegisterPersistentHandler)
    MODULE_FUNCTION(Pause)
    MODULE_FUNCTION(Resume)
    MODULE_FUNCTION(Cancel)
    END_MODULE()
}

END_NS()
