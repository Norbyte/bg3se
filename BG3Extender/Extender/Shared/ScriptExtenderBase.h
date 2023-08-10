#pragma once

#include <GameDefinitions/Base/Base.h>
#include <functional>
#include <unordered_set>
#include <concurrent_queue.h>

BEGIN_SE()

class ThreadedExtenderState
{
public:
	bool IsInThread() const;
	bool IsInThread(DWORD threadId) const;
	std::unordered_set<DWORD> const& GetThreadIds() const
	{
		return threadIds_;
	}

	void EnqueueTask(std::function<void()> fun);
	void SubmitTaskAndWait(std::function<void()> fun);

protected:
	void RunPendingTasks();
	void AddThread(DWORD threadId);
	void RemoveThread(DWORD threadId);

private:
	std::unordered_set<DWORD> threadIds_;
	concurrency::concurrent_queue<std::function<void()>> threadTasks_;
};

END_SE()
