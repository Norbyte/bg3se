#include <Lua/Libs/Timer.h>

/// <lua_module>Timer</lua_module>
BEGIN_NS(lua::timer)

TimerManager::TimerManager(State& state, lua::DeferredLuaDelegateQueue& queue)
	: state_(state), eventQueue_(queue)
{}

TimerHandle TimerManager::Add(double time, Ref callback, float repeat)
{
	uint32_t id;
	auto timer = ephemeralTimers_.Add(id);
	timer->Time = time;
	timer->Callback = LuaDelegate<void(TimerHandle)>(state_.GetState(), callback);
	timer->Repeat = repeat;

	TimerHandle handle{ id };

	queue_.push(TimerQueueEntry{
		.Time = time,
		.Handle = handle
	});

	return handle;
}

TimerHandle TimerManager::AddPersistent(double time, FixedString const& callback, StringView argsJson)
{
	uint32_t id;
	auto timer = persistentTimers_.Add(id);
	timer->Time = time;
	timer->Callback = callback;
	timer->ArgsJson = argsJson;

	TimerHandle handle{ (uint64_t)id | PersistentFlag };

	queue_.push(TimerQueueEntry{
		.Time = time,
		.Handle = handle
	});

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

void TimerManager::Update(double time)
{
	while (!queue_.empty() && queue_.top().Time <= time) {
		auto handle = queue_.top().Handle;
		queue_.pop();

		FireTimer(handle, time);
	}
}

void TimerManager::FireTimer(TimerHandle handle, double time)
{
	if (handle & PersistentFlag) {
		auto timer = persistentTimers_.Find((uint32_t)handle);
		if (timer != nullptr) {
			auto callback = persistentCallbacks_.try_get(timer->Callback);
			if (callback) {
				auto L = state_.GetState();
				if (json::Parse(L, timer->ArgsJson)) {
					RegistryEntry args(L, -1);
					lua_pop(L, 1);
					eventQueue_.Call(*callback, std::move(args), handle);
				} else {
					ERR("Unable to parse persistent timer payload for '%s'!", timer->Callback.GetString());
				}
			} else {
				WARN("Tried to fire persistent timer '%s' but it has no callback registered!", timer->Callback.GetString());
			}
			
			persistentTimers_.Free((uint32_t)handle);
		}
	} else {
		auto timer = ephemeralTimers_.Find((uint32_t)handle);
		if (timer != nullptr) {
			eventQueue_.Call(timer->Callback, handle);

			if (timer->Repeat > 0.0f) {
				timer->Time = time + timer->Repeat;
				queue_.push(TimerQueueEntry{
					.Time = timer->Time,
					.Handle = handle
				});
			} else {
				ephemeralTimers_.Free((uint32_t)handle);
			}
		}
	}
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
				visitor->VisitDouble(GFS.strTime, timer.Time, 0.0);
				visitor->VisitFixedString(GFS.strHandler, timer.Callback, GFS.strEmpty);
				visitor->VisitSTDString(GFS.strArgs, timer.ArgsJson, STDString{});
				AddPersistent(timer.Time, timer.Callback, timer.ArgsJson);
			}
		}
	} else {
		uint32_t index{ 0 };
		while (true) {
			auto timer = persistentTimers_.Next(index);
			if (timer == nullptr) break;

			if (visitor->EnterNode(GFS.strTimer, GFS.strEmpty)) {
				visitor->VisitDouble(GFS.strTime, timer->Time, 0.0);
				visitor->VisitFixedString(GFS.strHandler, timer->Callback, GFS.strEmpty);
				visitor->VisitSTDString(GFS.strArgs, timer->ArgsJson, STDString{});
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
	MODULE_FUNCTION(Cancel)
	END_MODULE()
}

END_NS()
