#include <Extender/Shared/ScriptExtenderBase.h>
#include <Extender/ScriptExtender.h>

BEGIN_SE()

void ThreadedExtenderState::AddThread(DWORD threadId)
{
	if (threadIds_.find(threadId) == threadIds_.end()) {
		threadIds_.insert(threadId);
	}
}

void ThreadedExtenderState::RemoveThread(DWORD threadId)
{
	auto it = threadIds_.find(threadId);
	if (it != threadIds_.end()) {
		threadIds_.erase(it);
	}
}

void ThreadedExtenderState::EnqueueTask(std::function<void()> fun)
{
	threadTasks_.push(fun);
}

void ThreadedExtenderState::SubmitTaskAndWait(std::function<void()> fun)
{
	std::mutex mutex;
	std::condition_variable completion;
	bool completed = false;

	auto submitFunc = [&completion, &mutex, &completed, &fun]() {
		std::unique_lock<std::mutex> lk(mutex);
		fun();

		completed = true;
		lk.unlock();
		completion.notify_one();
	};

	std::unique_lock<std::mutex> lk(mutex);

	EnqueueTask(submitFunc);
	completion.wait(lk, [&completed] { return completed; });
}

void ThreadedExtenderState::RunPendingTasks()
{
	std::function<void()> fun;
	while (threadTasks_.try_pop(fun)) {
		fun();
	}
}

bool ThreadedExtenderState::IsInThread() const
{
	auto tid = GetCurrentThreadId();
	return threadIds_.find(tid) != threadIds_.end();
}

bool ThreadedExtenderState::IsInThread(DWORD threadId) const
{
	return threadIds_.find(threadId) != threadIds_.end();
}

END_SE()
