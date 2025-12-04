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

    struct BaseTimer 
    {
        double Time{ .0 };
        float FrozenTime{ .0f };
        float Repeat{ .0f };
        uint32_t InvokeId{ 0 };
        bool Paused{ false };

        void SavegameVisit(ObjectVisitor* visitor);
        void Start(double time, float repeat = .0f);
        void Pause(double time);
        void Resume(double time);
    };

    struct EphemeralTimer : public BaseTimer
    {
        LuaDelegate<void(TimerHandle)> Callback;
    };
    
    struct PersistentTimer : public BaseTimer
    {
        FixedString Callback;
        STDString ArgsJson;

        void SavegameVisit(ObjectVisitor* visitor);
        void FreezeBeforeSave(double time);
        void UnfreezeAfterRestore(double time);
    };
    
    struct TimerQueueEntry
    {
        double Time;
        TimerHandle Handle;
        uint32_t InvokeId;

        inline bool Matches(BaseTimer const& timer) const
        {
            return InvokeId == timer.InvokeId
                && Time == timer.Time
                && !timer.Paused;
        }

        inline bool operator > (TimerQueueEntry const& o) const
        {
            return Time > o.Time;
        }
    };

    TimerManager(State& state, DeferredLuaDelegateQueue& queue);
    TimerHandle Add(float delta, Ref callback, float repeat = .0f);
    TimerHandle AddPersistent(float delta, FixedString const& callback, StringView argsJson, float repeat = .0f);
    void RegisterPersistentCallback(FixedString const& name, Ref callback);
    bool Pause(TimerHandle handle);
    bool Resume(TimerHandle handle);
    bool IsPaused(TimerHandle handle);
    bool Cancel(TimerHandle handle);
    void Update(double time);
    void SavegameVisit(ObjectVisitor* visitor);

private:
    SaltedPool<EphemeralTimer> ephemeralTimers_;
    SaltedPool<PersistentTimer> persistentTimers_;
    HashMap<FixedString, LuaDelegate<void(RegistryEntry, TimerHandle)>> persistentCallbacks_;
    std::priority_queue<TimerQueueEntry, Vector<TimerQueueEntry>, std::greater<TimerQueueEntry>> queue_;
    Array<PersistentTimer> pendingRestore_;

    State& state_;
    DeferredLuaDelegateQueue& eventQueue_;
    double lastUpdate_{ .0f };

    void FireTimer(TimerQueueEntry const& entry);
    void RepeatOrReleaseTimer(TimerHandle handle, BaseTimer& timer);
    void QueueTimer(TimerHandle handle, BaseTimer const& timer);
    TimerHandle RestorePersistent(PersistentTimer const& timer);
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
    bool Pause(TimerHandle handle);
    bool Resume(TimerHandle handle);
    bool IsPaused(TimerHandle handle);
    void SavegameVisit(ObjectVisitor* visitor);

private:
    TimerManager realtime_;
    TimerManager game_;
    DeferredLuaDelegateQueue eventQueue_;
    bool isServer_;
};

END_NS()
