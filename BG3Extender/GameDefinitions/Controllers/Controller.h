#pragma once

BEGIN_NS(esv)

struct Task : public ProtectedGameObject<Task>
{
	virtual ~Task() = 0;
	virtual bool Update(GameTime const&) = 0;
	virtual bool SavegameVisit(ObjectVisitor& visitor) = 0;
	virtual bool SetObjectHandle(EntityHandle) = 0;
	virtual char const* GetName() = 0;
	virtual void FillExceptionInterruptInfo(void*) = 0;
	virtual void Start(GameTime const&) = 0;
	virtual void Cancel() = 0;
	virtual void ForceCompletion() = 0;
	virtual void Continue() = 0;
	virtual void VMT50() = 0;
	virtual void OnResume() = 0;

	[[bg3::readonly]] EntityHandle Entity;
	[[bg3::readonly]] OsirisTaskType Type;
	int TaskState;
	PathSettings PathSettings;
	bool Failed;
	int Reason;
	uint32_t Flags;
};


struct BaseController : public ProtectedGameObject<BaseController>
{
	virtual ~BaseController() = 0;
	virtual char const* GetName() = 0;
	virtual int GetPriority() = 0;
	virtual int GetTypeId() = 0;
	virtual void Update(GameTime const&) = 0;
	virtual bool Pause() = 0;
	virtual void Resume() = 0;
	virtual void Reset() = 0;
	virtual bool SavegameVisit(ObjectVisitor& visitor) = 0;
	virtual void ResetState() = 0;
	virtual void Ret0() = 0;
	virtual void ScheduleTask(Task* task, uint32_t action) = 0;

	ecs::EntityRef Owner;
};


struct [[bg3::hidden]] OsirisTaskFactory
{
	using CreateTaskProc = Task * (OsirisTaskFactory* self, OsirisTaskType type, uint64_t characterHandle);
	using ReleaseTaskProc = void (OsirisTaskFactory* self, Task* task);

	Pool Pools[26];
};


struct TaskController : public BaseController
{
	Queue<Task*> Tasks;
	[[bg3::readonly]] bool RemoveNextTask_M;
	[[bg3::readonly]] bool UpdateInProgress;
	bool FlushRequested;

	void PurgeQueue();
	void FlushQueue();
	void QueueTask(Task* task);
};

END_NS()
