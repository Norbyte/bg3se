////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef NS_COMPILER_MSVC
#include <intrin.h>
#endif


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
inline AtomicInteger::AtomicInteger(int32_t val_): val(val_)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int32_t AtomicInteger::Load() const
{
#if NS_MULTITHREADING
    #ifdef NS_COMPILER_MSVC
        #if defined(_M_ARM) || defined(_M_ARM64)
            return __iso_volatile_load32(&val);
        #else
            return val;
        #endif
    #else
        return __atomic_load_n(&val, __ATOMIC_RELAXED);
    #endif
#else
    return val;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int32_t AtomicInteger::LoadAcquire() const
{
#if NS_MULTITHREADING
    #ifdef NS_COMPILER_MSVC
        #if defined(_M_ARM)
            int32_t v = __iso_volatile_load32(&val);
            __dmb(_ARM_BARRIER_ISH);
            return v;
        #elif defined(_M_ARM64)
            int32_t v = __iso_volatile_load32(&val);
            __dmb(_ARM64_BARRIER_ISH)
            return v;
        #else
            int32_t v = val;
            _ReadWriteBarrier();
            return v;
        #endif
    #else
        return __atomic_load_n(&val, __ATOMIC_ACQUIRE);
    #endif
#else
    return val;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void AtomicInteger::Store(int32_t v)
{
#if NS_MULTITHREADING
    #ifdef NS_COMPILER_MSVC
        #if defined(_M_ARM) || defined(_M_ARM64)
            __iso_volatile_store32(&val, v);
        #else
            val = v;
        #endif
    #else
        __atomic_store_n(&val, v, __ATOMIC_RELAXED);
    #endif
#else
    val = v;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void AtomicInteger::StoreRelease(int32_t v)
{
#if NS_MULTITHREADING
    #ifdef NS_COMPILER_MSVC
        #if defined(_M_ARM)
            __dmb(_ARM_BARRIER_ISH);
            __iso_volatile_store32(&val, v);
        #elif defined(_M_ARM64)
            __dmb(_ARM64_BARRIER_ISH)
            __iso_volatile_store32(&val, v);
        #else
            _ReadWriteBarrier();
            val = v;
        #endif
    #else
        __atomic_store_n(&val, v, __ATOMIC_RELEASE);
    #endif
#else
    val = v;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int32_t AtomicInteger::IncrementAndFetch()
{
#if NS_MULTITHREADING
    #ifdef NS_COMPILER_MSVC
        #if defined(_M_ARM) || defined(_M_ARM64)
            return _InterlockedIncrement_nf((volatile long*)&val);
        #else
            return _InterlockedIncrement((volatile long*)&val);
        #endif
    #else
        return __atomic_add_fetch(&val, 1, __ATOMIC_RELAXED);
    #endif
#else
    return ++val;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int32_t AtomicInteger::DecrementAndFetch()
{
#if NS_MULTITHREADING
    #ifdef NS_COMPILER_MSVC
        #if defined(_M_ARM) || defined(_M_ARM64)
            return _InterlockedDecrement_nf((volatile long*)&val);
        #else
            return _InterlockedDecrement((volatile long*)&val);
        #endif
    #else
        return __atomic_sub_fetch(&val, 1, __ATOMIC_RELAXED);
    #endif
#else
    return --val;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int32_t AtomicInteger::AddAndFetch(int32_t v)
{
#if NS_MULTITHREADING
    #ifdef NS_COMPILER_MSVC
        #if defined(_M_ARM) || defined(_M_ARM64)
            return _InterlockedExchangeAdd_nf((volatile long*)&val, v) + v;
        #else
            return _InterlockedExchangeAdd((volatile long*)&val, v) + v;
        #endif
    #else
        return __atomic_add_fetch(&val, v, __ATOMIC_RELAXED);
    #endif
#else
    return val += v;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int32_t AtomicInteger::SubAndFetch(int32_t v)
{
    return AddAndFetch(-v);
}

}
