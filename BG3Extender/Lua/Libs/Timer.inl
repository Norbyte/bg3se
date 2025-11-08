#include <Lua/Libs/Timer.h>
#include <chrono>

/// <lua_module>Timer</lua_module>
BEGIN_NS(lua::timer)

void TimerManager::BaseTimer::SavegameVisit(ObjectVisitor* visitor)
{
    visitor->VisitDouble(GFS.strTime, Time, .0);
    visitor->VisitFloat(GFS.strTime, FrozenTime, .0f);
    visitor->VisitFloat(GFS.strTime, Repeat, .0f);
    visitor->VisitBool(GFS.strTime, Paused, false);
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
    se_assert(!Paused);
    FrozenTime = (float)std::max(Time - time, .0);
    Time = .0f;
    Paused = true;
}

void TimerManager::BaseTimer::Resume(double time)
{
    se_assert(Paused);
    Time = time + FrozenTime;
    FrozenTime = .0f;
    Paused = false;
    InvokeId++;
}

void TimerManager::PersistentTimer::SavegameVisit(ObjectVisitor* visitor)
{
    BaseTimer::SavegameVisit(visitor);
    visitor->VisitFixedString(GFS.strHandler, Callback, GFS.strEmpty);
    visitor->VisitSTDString(GFS.strArgs, ArgsJson, STDString{});
}

void TimerManager::PersistentTimer::FreezeBeforeSave(double time)
{
    if (!Paused) {
        FrozenTime = (float)std::max(Time - time, .0);
        Time = .0f;
    }
}

void TimerManager::PersistentTimer::UnfreezeAfterRestore(double time)
{
    if (!Paused) {
        Time = time + FrozenTime;
        FrozenTime = .0f;
    }
}

TimerManager::TimerManager(State& state, lua::DeferredLuaDelegateQueue& queue)
    : state_(state), eventQueue_(queue)
{}

TimerHandle TimerManager::Add(float delta, Ref callback, float repeat)
{
    uint32_t id;
    auto timer = ephemeralTimers_.Add(id);
    timer->Start(lastUpdate_ + delta, repeat);
    timer->Callback = LuaDelegate<void(TimerHandle)>(state_.GetState(), callback);

    TimerHandle handle{ id };
    QueueTimer(handle, *timer);
    return handle;
}

TimerHandle TimerManager::AddPersistent(float delta, FixedString const& callback, StringView argsJson, float repeat)
{
    uint32_t id;
    auto timer = persistentTimers_.Add(id);
    timer->Start(lastUpdate_ + delta, repeat);
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
    timer->UnfreezeAfterRestore(lastUpdate_);

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

bool TimerManager::Pause(TimerHandle handle)
{
    BaseTimer* timer{ nullptr };
    if (handle & PersistentFlag) {
        timer = persistentTimers_.Find((uint32_t)handle);
    } else {
        timer = ephemeralTimers_.Find((uint32_t)handle);
    }

    if (timer) {
        if (!timer->Paused) {
            timer->Pause(lastUpdate_);
        }
        return true;
    } else {
        return false;
    }
}

bool TimerManager::Resume(TimerHandle handle)
{
    BaseTimer* timer{ nullptr };
    if (handle & PersistentFlag) {
        timer = persistentTimers_.Find((uint32_t)handle);
    } else {
        timer = ephemeralTimers_.Find((uint32_t)handle);
    }

    if (timer) {
        if (timer->Paused) {
            timer->Resume(lastUpdate_);
            QueueTimer(handle, *timer);
        }
        return true;
    } else {
        return false;
    }
}

bool TimerManager::IsPaused(TimerHandle handle)
{
    BaseTimer* timer{ nullptr };
    if (handle & PersistentFlag) {
        timer = persistentTimers_.Find((uint32_t)handle);
    } else {
        timer = ephemeralTimers_.Find((uint32_t)handle);
    }

    return timer && timer->Paused;
}

void TimerManager::Update(double time)
{
    lastUpdate_ = time;

    for (auto const& timer : pendingRestore_) {
        RestorePersistent(timer);
    }

    pendingRestore_.clear();

    while (!queue_.empty() && queue_.top().Time <= time) {
        auto entry = queue_.top();
        queue_.pop();

        FireTimer(entry);
    }
}

void TimerManager::FireTimer(TimerQueueEntry const& entry)
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
            
            RepeatOrReleaseTimer(entry.Handle, *timer);
        }
    } else {
        auto timer = ephemeralTimers_.Find((uint32_t)entry.Handle);
        if (timer != nullptr && entry.Matches(*timer)) {
            eventQueue_.Call(timer->Callback, entry.Handle);
            RepeatOrReleaseTimer(entry.Handle, *timer);
        }
    }
}

void TimerManager::RepeatOrReleaseTimer(TimerHandle handle, BaseTimer& timer)
{
    if (timer.Repeat > 0.0f) {
        timer.Time = lastUpdate_ + timer.Repeat;
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
    se_assert(!timer.Paused);
    queue_.push(TimerQueueEntry{
        .Time = timer.Time,
        .Handle = handle,
        .InvokeId = timer.InvokeId
    });
}

void TimerManager::SavegameVisit(ObjectVisitor* visitor)
{
    if (visitor->IsReading()) {
        uint32_t numVars;
        visitor->VisitCount(GFS.strTimer, &numVars);

        for (uint32_t i = 0; i < numVars; i++) {
            if (visitor->EnterNode(GFS.strTimer, GFS.strEmpty)) {
                PersistentTimer timer;
                timer.SavegameVisit(visitor);
                pendingRestore_.push_back(timer);
            }
        }
    } else {
        uint32_t index{ 0 };
        while (true) {
            auto timer = persistentTimers_.Next(index);
            if (timer == nullptr) break;

            if (visitor->EnterNode(GFS.strTimer, GFS.strEmpty)) {
                auto saveTimer = *timer;
                saveTimer.FreezeBeforeSave(lastUpdate_);
                saveTimer.SavegameVisit(visitor);
                visitor->ExitNode(GFS.strTimer);
            }
        }
    }
}


TimerSystem::TimerSystem(State& state, bool isServer)
    : realtime_(state, eventQueue_),
    game_(state, eventQueue_),
    isServer_(isServer),
    eventQueue_("Timer event")
{}

void TimerSystem::Update(double time)
{
    OPTICK_EVENT();
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

bool TimerSystem::Pause(TimerHandle handle)
{
    if (handle & TimerManager::RealtimeFlag) {
        return realtime_.Pause(handle);
    } else {
        return game_.Pause(handle);
    }
}

bool TimerSystem::Resume(TimerHandle handle)
{
    if (handle & TimerManager::RealtimeFlag) {
        return realtime_.Resume(handle);
    } else {
        return game_.Resume(handle);
    }
}

bool TimerSystem::IsPaused(TimerHandle handle)
{
    if (handle & TimerManager::RealtimeFlag) {
        return realtime_.IsPaused(handle);
    } else {
        return game_.IsPaused(handle);
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

    return state->GetTimers().GameTimer().Add(delay / 1000.0f, callback, repeat ? (*repeat / 1000.0f) : 0.0f);
}

TimerHandle WaitForPersistent(lua_State* L, float delay, FixedString callback, Ref args, std::optional<float> repeat)
{
    auto state = State::FromLua(L);

    if (!state->GetTimers().SupportsPersistence()) {
        luaL_error(L, "Persistent timers are only supported on the server");
    }

    json::StringifyContext ctx;
    ctx.Beautify = false;
    auto str = json::Stringify(L, ctx, args.Index());
    return state->GetTimers().GameTimer().AddPersistent(delay / 1000.0f, callback, str, repeat ? (*repeat / 1000.0f) : 0.0f);
}

TimerHandle WaitForRealtime(lua_State* L, float delay, Ref callback, std::optional<float> repeat)
{
    auto state = State::FromLua(L);

    auto handle = state->GetTimers().RealtimeTimer().Add(delay / 1000.0f, callback, repeat ? (*repeat / 1000.0f) : 0.0f);
    return handle | TimerManager::RealtimeFlag;
}

void RegisterPersistentHandler(lua_State* L, FixedString name, Ref callback)
{
    return State::FromLua(L)->GetTimers().GameTimer().RegisterPersistentCallback(name, callback);
}

bool Pause(lua_State* L, TimerHandle handle)
{
    return State::FromLua(L)->GetTimers().Pause(handle);
}

bool Resume(lua_State* L, TimerHandle handle)
{
    return State::FromLua(L)->GetTimers().Resume(handle);
}

bool IsPaused(lua_State* L, TimerHandle handle)
{
    return State::FromLua(L)->GetTimers().IsPaused(handle);
}

bool Cancel(lua_State* L, TimerHandle handle)
{
    return State::FromLua(L)->GetTimers().Cancel(handle);
}

int64_t MonotonicTime()
{
    using namespace std::chrono;
    return (int64_t)time_point_cast<milliseconds>(steady_clock::now()).time_since_epoch().count();
}

// Keep a reference for game start to reduce precision loss caused by casting an uint64 to double
LARGE_INTEGER AppStartCounter;

double MicrosecTime()
{
    LARGE_INTEGER perf, freq;
    QueryPerformanceCounter(&perf);
    QueryPerformanceFrequency(&freq);

    return (perf.QuadPart - AppStartCounter.QuadPart) / (double(freq.QuadPart) / 1000000.0);
}

double GameTime(lua_State* L)
{
    return ExtensionStateBase::FromLua(L).Time().Time;
}

int64_t ClockEpoch(lua_State* L)
{
    using namespace std::chrono;
    auto now = system_clock::now().time_since_epoch();
    return duration_cast<std::chrono::seconds>(now).count();
}

STDString ClockTime(lua_State* L)
{
    using namespace std::chrono;
    auto now = system_clock::now();
    return std::format("{0:%F} {0:%T}", now).c_str();
}

void RegisterTimerLib()
{
    QueryPerformanceCounter(&AppStartCounter);

    DECLARE_MODULE(Timer, Both)
    BEGIN_MODULE()
    MODULE_FUNCTION(MonotonicTime)
    MODULE_FUNCTION(MicrosecTime)
    MODULE_FUNCTION(GameTime)
    MODULE_FUNCTION(ClockEpoch)
    MODULE_FUNCTION(ClockTime)

    MODULE_FUNCTION(WaitFor)
    MODULE_FUNCTION(WaitForPersistent)
    MODULE_FUNCTION(WaitForRealtime)
    MODULE_FUNCTION(RegisterPersistentHandler)
    MODULE_FUNCTION(Pause)
    MODULE_FUNCTION(Resume)
    MODULE_FUNCTION(IsPaused)
    MODULE_FUNCTION(Cancel)
    END_MODULE()
}

END_NS()
