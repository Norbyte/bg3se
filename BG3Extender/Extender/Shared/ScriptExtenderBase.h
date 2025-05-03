#pragma once

#include <GameDefinitions/Base/Base.h>
#include <functional>
#include <unordered_set>
#include <concurrent_queue.h>

BEGIN_SE()

enum class ContextType
{
    None,
    Server,
    Client
};

ContextType GetCurrentContextType();

class ContextGuard
{
public:
    ContextGuard(ContextType ctx);
    ~ContextGuard();

private:
    ContextType context_;
    ContextType previousContext_;
};

// Same as ContextGuard, but allows temporarily "conversion" of 
// a client thread into server context and vice versa
class ContextGuardAnyThread
{
public:
    ContextGuardAnyThread(ContextType ctx);
    ~ContextGuardAnyThread();

private:
    ContextType context_;
    ContextType previousContext_;
};

class ThreadedExtenderState
{
public:
    ThreadedExtenderState(ContextType ctx);

    bool IsInContext() const;
    void EnqueueTask(std::function<void()> fun);
    void SubmitTaskAndWait(std::function<void()> fun);

protected:
    void RunPendingTasks();
    void BindToThreadPersistent();

private:
    ContextType context_;
    concurrency::concurrent_queue<std::function<void()>> threadTasks_;
};

END_SE()
