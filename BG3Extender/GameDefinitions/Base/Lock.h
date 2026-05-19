#pragma once

BEGIN_SE()

template <class Pred>
inline void SpinWait(Pred pred)
{
    while (!pred()) {
        unsigned spinCount = 0;
        while (!pred()) {
            _mm_pause();
            if (spinCount++ > 400) {
                Sleep(0);
                break;
            }
        }
    }
}

class SRWSpinLock
{
public:
    SRWSpinLock() = default;
    SRWSpinLock(SRWSpinLock const&) = delete;
    SRWSpinLock(SRWSpinLock &&) = delete;
    SRWSpinLock& operator = (SRWSpinLock const&) = delete;
    SRWSpinLock& operator = (SRWSpinLock &&) = delete;

    void ReadLock();
    void ReadUnlock();
    void WriteLock();
    void WriteUnlock();

private:
    static constexpr DWORD NullThreadId = 0xffffffffu;

    std::atomic<uint32_t> FastLock;
    DWORD OwningThreadId{ NullThreadId };
    std::atomic<uint32_t> WriteEnterCount{ 0 };

    void WriteWait();
    void ReadWait();
};

class SRWLock
{
public:
    SRWLock();
    ~SRWLock();
    SRWLock(SRWLock const&) = delete;
    SRWLock(SRWLock &&) = delete;
    SRWLock& operator = (SRWLock const&) = delete;
    SRWLock& operator = (SRWLock &&) = delete;

    void ReadLock();
    void ReadUnlock();
    void WriteLock();
    void WriteUnlock();

private:
    static constexpr DWORD NullThreadId = 0xffffffffu;

    SRWLOCK Lock;
    DWORD OwningThreadId{ NullThreadId };
    std::atomic<uint32_t> WriteEnterCount{ 0 };
};

END_SE()
