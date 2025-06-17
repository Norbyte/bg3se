#include <Extender/Shared/ScriptExtenderBase.h>
#include <Extender/ScriptExtender.h>

BEGIN_SE()

thread_local ContextType gCurrentContext = ContextType::None;

ContextType GetCurrentContextType()
{
    return gCurrentContext;
}

ContextGuard::ContextGuard(ContextType ctx)
    : context_(ctx)
{
    se_assert(ctx != ContextType::None);
    se_assert(gCurrentContext == ContextType::None || gCurrentContext == ctx);
    previousContext_ = gCurrentContext;
    gCurrentContext = context_;
}

ContextGuard::~ContextGuard()
{
    se_assert(gCurrentContext == context_);
    gCurrentContext = previousContext_;
}

ContextGuardAnyThread::ContextGuardAnyThread(ContextType ctx)
    : context_(ctx)
{
    se_assert(ctx != ContextType::None);
    previousContext_ = gCurrentContext;
    gCurrentContext = context_;
}

ContextGuardAnyThread::~ContextGuardAnyThread()
{
    se_assert(gCurrentContext == context_);
    gCurrentContext = previousContext_;
}

ThreadedExtenderState::ThreadedExtenderState(ContextType ctx)
    : context_(ctx)
{}

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

void ThreadedExtenderState::BindToThreadPersistent()
{
    se_assert(gCurrentContext == ContextType::None || gCurrentContext == context_);
    gCurrentContext = context_;
}

bool ThreadedExtenderState::IsInContext() const
{
    return gCurrentContext == context_;
}

END_SE()
