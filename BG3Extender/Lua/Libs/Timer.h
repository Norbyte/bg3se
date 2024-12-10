#pragma once

#include <queue>

/// <lua_module>Timer</lua_module>
BEGIN_NS(lua::timer)

using TimerHandle = uint64_t;

class TimerManager
{
public:
    static constexpr uint64_t PersistentFlag = 0x100000000ull;
    static constexpr uint64_t RealtimeFlag = 0x200000000ull;

    struct EphemeralTimer 
    {
        double Time;
        LuaDelegate<void(TimerHandle)> Callback;
        float Repeat;
    };
    
    struct PersistentTimer
    {
        double Time;
        FixedString Callback;
        STDString ArgsJson;
    };
    
    struct TimerQueueEntry
    {
        double Time;
        TimerHandle Handle;

        inline bool operator > (TimerQueueEntry const& o) const
        {
            return Time > o.Time;
        }
    };

    TimerManager(State& state, DeferredLuaDelegateQueue& queue);
    TimerHandle Add(double time, Ref callback, float repeat = 0.0f);
    TimerHandle AddPersistent(double time, FixedString const& callback, StringView argsJson);
    void RegisterPersistentCallback(FixedString const& name, Ref callback);
    bool Cancel(TimerHandle handle);
    void Update(double time);
    void SavegameVisit(ObjectVisitor* visitor);

private:
    SaltedPool<EphemeralTimer> ephemeralTimers_;
    SaltedPool<PersistentTimer> persistentTimers_;
    HashMap<FixedString, LuaDelegate<void(RegistryEntry, TimerHandle)>> persistentCallbacks_;
    std::priority_queue<TimerQueueEntry, Vector<TimerQueueEntry>, std::greater<TimerQueueEntry>> queue_;

    State& state_;
    DeferredLuaDelegateQueue& eventQueue_;

    void FireTimer(TimerHandle handle, double time);
};

class TimerSystem
{
public:
    TimerSystem(State& state, bool isServer);

    inline TimerManager& RealtimeTimer()
    {
        return realtime_;
    }

    inline TimerManager& GameTimer()
    {
        return game_;
    }

    inline bool SupportsPersistence() const
    {
        return isServer_;
    }

    void Update(double time);
    bool Cancel(TimerHandle handle);
    void SavegameVisit(ObjectVisitor* visitor);

private:
    TimerManager realtime_;
    TimerManager game_;
    DeferredLuaDelegateQueue eventQueue_;
    bool isServer_;
};

END_NS()
