#include <GameDefinitions/Base/Lock.h>

BEGIN_SE()

void SRWSpinLock::ReadLock()
{
    if (OwningThreadId == NullThreadId || OwningThreadId != GetCurrentThreadId()) {
        ReadWait();
    }
}

void SRWSpinLock::ReadUnlock()
{
    if (OwningThreadId == NullThreadId || OwningThreadId != GetCurrentThreadId()) {
        se_assert((FastLock & 0x000fffffu) > 0);
        --FastLock;
    }
}

void SRWSpinLock::WriteLock()
{
    if (OwningThreadId == NullThreadId || OwningThreadId != GetCurrentThreadId()) {
        WriteWait();

        OwningThreadId = GetCurrentThreadId();
    }

    ++WriteEnterCount;
}

void SRWSpinLock::WriteUnlock()
{
    se_assert(WriteEnterCount > 0);
    if (--WriteEnterCount == 0) {
        se_assert(OwningThreadId == GetCurrentThreadId());
        se_assert((FastLock & 0xfff00000u) > 0);
        OwningThreadId = NullThreadId;
        FastLock -= 0x100000u;
    }
}

void SRWSpinLock::WriteWait()
{
    for (;;) {
        SpinWait([&] () { return (FastLock & 0xfff00000u) == 0; });

        if ((FastLock.fetch_add(0x100000u) & 0xfff00000u) == 0) {
            break;
        }

        FastLock -= 0x100000u;
    }

    if ((FastLock & 0x000fffffu) != 0) {
        SpinWait([&] () { return (FastLock & 0x000fffffu) == 0; });
    }
}

void SRWSpinLock::ReadWait()
{
    for (;;) {
        SpinWait([&] () { return (FastLock & 0xfff00000u) == 0; });

        if ((FastLock.fetch_add(1) & 0xfff00000u) == 0) {
            break;
        }

        --FastLock;
    }
}


SRWLock::SRWLock()
{
    InitializeSRWLock(&Lock);
}

SRWLock::~SRWLock()
{
    assert(OwningThreadId == NullThreadId);
}

void SRWLock::ReadLock()
{
    if (OwningThreadId == NullThreadId || OwningThreadId != GetCurrentThreadId()) {
        AcquireSRWLockShared(&Lock);
    }
}

void SRWLock::ReadUnlock()
{
    if (OwningThreadId == NullThreadId || OwningThreadId != GetCurrentThreadId()) {
        ReleaseSRWLockShared(&Lock);
    }
}

void SRWLock::WriteLock()
{
    if (OwningThreadId == NullThreadId || OwningThreadId != GetCurrentThreadId()) {
        AcquireSRWLockExclusive(&Lock);
        OwningThreadId = GetCurrentThreadId();
    }

    ++WriteEnterCount;
}

void SRWLock::WriteUnlock()
{
    se_assert(WriteEnterCount > 0);
    if (--WriteEnterCount == 0) {
        se_assert(OwningThreadId == GetCurrentThreadId());
        OwningThreadId = NullThreadId;
        ReleaseSRWLockExclusive(&Lock);
    }
}

END_SE()
