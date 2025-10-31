////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_ATOMIC_H__
#define __CORE_ATOMIC_H__


#include <NsCore/Noesis.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// An atomic type for signed 32-bits integers. Except when indicated, operations don't act as a
/// memory barrier. This is the relaxed memory order in C++11 parlance.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct AtomicInteger
{
    AtomicInteger() = default;
    AtomicInteger(int32_t val);
    AtomicInteger(const AtomicInteger&) = delete;
    AtomicInteger& operator=(const AtomicInteger&) = delete;

    /// Atomically obtains the value
    int32_t Load() const;

    /// Atomically obtains the value with Acquire semantics
    int32_t LoadAcquire() const;

    /// Atomically replaces the value
    void Store(int32_t v);

    /// Atomically replaces the value with Release semantics
    void StoreRelease(int32_t v);

    /// Increments the value and returns the result
    int32_t IncrementAndFetch();

    /// Decrements the value and returns the result
    int32_t DecrementAndFetch();

    /// Adds the given value and returns the result
    int32_t AddAndFetch(int32_t v);

    /// Substracts the given value and returns the result
    int32_t SubAndFetch(int32_t v);

#if NS_MULTITHREADING
    volatile int32_t val;
#else
    int32_t val;
#endif
};

}

#include <NsCore/Atomic.inl>

#endif
