////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Error.h>

#if defined(NS_PLATFORM_WINDOWS)
    union _LARGE_INTEGER;
    typedef _LARGE_INTEGER LARGE_INTEGER;
    extern "C" __declspec(dllimport) int __stdcall QueryPerformanceCounter(_Out_ LARGE_INTEGER*);
    extern "C" __declspec(dllimport) int __stdcall QueryPerformanceFrequency(_Out_ LARGE_INTEGER*);
#elif defined(NS_PLATFORM_APPLE)
    #include <mach/mach_time.h>
#elif defined(NS_PLATFORM_SCE)
    #include <kernel.h>
#elif defined(NS_PLATFORM_NX)
    #include <nn/os/os_Tick.h>
#elif defined(NS_PLATFORM_EMSCRIPTEN)
    #include <emscripten.h>
#else
    #include <sys/time.h>
#endif


namespace Noesis
{
namespace HighResTimer
{

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint64_t Ticks()
{
#if defined(NS_PLATFORM_WINDOWS)
    uint64_t count;
    QueryPerformanceCounter((LARGE_INTEGER*)&count); // will always succeed
    return count;

#elif defined(NS_PLATFORM_APPLE)
    return mach_absolute_time();

#elif defined(NS_PLATFORM_SCE)
    return sceKernelGetProcessTimeCounter();

#elif defined(NS_PLATFORM_NX)
    return nn::os::GetSystemTick().GetInt64Value();

#elif defined(NS_PLATFORM_EMSCRIPTEN)
    return (uint64_t)(emscripten_get_now() * 1000.0);

#else
    timeval tv;
    int error = gettimeofday(&tv, 0);
    NS_ASSERT(error == 0);
    return (uint64_t)tv.tv_usec + ((uint64_t)tv.tv_sec * 1000000);
#endif
}


#if defined(NS_PLATFORM_WINDOWS)

////////////////////////////////////////////////////////////////////////////////////////////////////
static double frequency = []()
{
    int64_t frequency_;
    QueryPerformanceFrequency((LARGE_INTEGER*)&frequency_);
    return (double)frequency_;
}();

////////////////////////////////////////////////////////////////////////////////////////////////////
inline double Seconds(uint64_t ticks)
{
    // int64_t -> double faster than uint64_t -> double
    return (int64_t)ticks / frequency;
}

#elif defined(NS_PLATFORM_APPLE)

////////////////////////////////////////////////////////////////////////////////////////////////////
static mach_timebase_info_data_t tbi = []()
{
    mach_timebase_info_data_t tbi_;
    mach_timebase_info(&tbi_);
    return tbi_;
}();

////////////////////////////////////////////////////////////////////////////////////////////////////
inline double Seconds(uint64_t ticks)
{
    return (double)(int64_t)ticks * tbi.numer / (tbi.denom * 1e9);
}

#elif defined(NS_PLATFORM_SCE)

static double frequency = (double)sceKernelGetProcessTimeCounterFrequency();

////////////////////////////////////////////////////////////////////////////////////////////////////
inline double Seconds(uint64_t ticks)
{
    return (int64_t)ticks / frequency;
}

#elif defined(NS_PLATFORM_NX)

static double frequency = (double)nn::os::GetSystemTickFrequency();

////////////////////////////////////////////////////////////////////////////////////////////////////
inline double Seconds(uint64_t ticks)
{
    return (int64_t)ticks / frequency;
}

#else

////////////////////////////////////////////////////////////////////////////////////////////////////
inline double Seconds(uint64_t ticks)
{
    return (int64_t)ticks / 1000000.0;
}

#endif

}
}
