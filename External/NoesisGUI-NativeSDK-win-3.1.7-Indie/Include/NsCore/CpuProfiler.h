////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_CPUPROFILER_H__
#define __CORE_CPUPROFILER_H__


#include <NsCore/Noesis.h>


// Critical paths in Noesis source code contain markers for code instrumentation.
// These markers are disabled by default and can be enabled by activating any of the
// implementations listed below. Custom profilers can also be implemented by directly
// defining 'NS_PROFILE_CPU' and rest of preprocessor macros

#ifndef NS_PROFILER_TRACY
  #define NS_PROFILER_TRACY   0
#endif
#ifndef NS_PROFILER_OPTICK
  #define NS_PROFILER_OPTICK  0
#endif
#ifndef NS_PROFILER_ATRACE
  #define NS_PROFILER_ATRACE  0
#endif
#ifndef NS_PROFILER_PIX
  #define NS_PROFILER_PIX     0
#endif
#ifndef NS_PROFILER_RAZOR
  #define NS_PROFILER_RAZOR   0
#endif
#ifndef NS_PROFILER_NX
  #define NS_PROFILER_NX      0
#endif

#if NS_PROFILER_TRACY
    // https://github.com/wolfpld/tracy
    // Download from GitHub and copy the repository to 'NoesisSDK/Include/Tracy'
    #include <NsCore/KernelApi.h>
    #define __TRACYAPI_H__
    #define TRACY_API NS_CORE_KERNEL_API
    NS_WARNING_PUSH
    NS_MSVC_WARNING_DISABLE(4577)
    #define TRACY_ENABLE
    #include <Tracy/Tracy.hpp>
    NS_WARNING_POP

    constexpr uint32_t TracyColors[] =
    {
        0xff0c8189, 0xff001b5b, 0xff083672, 0xff036677, 0xff010589,
        0xff0a5987, 0xff0c548e, 0xff0b787a, 0xff3d10a5, 0xff075975
    };

    constexpr uint32_t TracyHash(const char* s, uint32_t h = 2166136261U)
    {
        return (!*s || *s == '/') ? h : TracyHash(s + 1, (uint32_t)((h ^ *s) * (uint64_t)16777619));
    }

    #define NS_PROFILER_CPU_FRAME FrameMark
    #define NS_PROFILE_CPU(name) ZoneScopedNC(name, TracyColors[TracyHash(name) % 10])
    #define NS_PROFILE_CPU_ALLOC(ptr, size) TracyAlloc(ptr, size)
    #define NS_PROFILE_CPU_FREE(ptr) TracyFree(ptr)
#endif

#if NS_PROFILER_OPTICK
    // https://github.com/bombomby/optick
    // Download from GitHub and copy headers to 'NoesisSDK/Include/Optick'
    // Copy 'OptickCore.lib' to Lib folder and 'OptickCore.dll' to Bin folder
    #include <Optick/optick.h>
    #pragma comment(lib, "OptickCore.lib")
    #define NS_PROFILER_CPU_FRAME OPTICK_FRAME("Frame")
    #define NS_PROFILE_CPU(name) OPTICK_EVENT(name)
#endif

#if NS_PROFILER_ATRACE
    // https://developer.android.com/studio/profile/systrace
    // Remember to enable debuggable in the manifest and custom events in systrace (-a)
    //  systrace.py -a com.noesis.Samples.HelloWorld perf sched gfx view
    #include <NsCore/KernelApi.h>
    #include <dlfcn.h> 

    extern NS_CORE_KERNEL_API void (*ATrace_beginSection_)(const char*);
    extern NS_CORE_KERNEL_API void (*ATrace_endSection_)();

    #define NS_PROFILE_CPU_INIT_DECLARE \
        void (*ATrace_beginSection_)(const char*); \
        void (*ATrace_endSection_)(); \
        static void ATraceInit() \
        { \
            void* android = dlopen("libandroid.so", RTLD_NOW | RTLD_LOCAL); \
            if (android != nullptr) \
            { \
                typedef void (*ATrace_beginSectionT)(const char*); \
                ATrace_beginSection_ = (ATrace_beginSectionT)dlsym(android, "ATrace_beginSection"); \
                typedef void (*ATrace_endSectionT)(void); \
                ATrace_endSection_ = (ATrace_endSectionT)dlsym(android, "ATrace_endSection"); \
            } \
        }
    #define NS_PROFILE_CPU_INIT ATraceInit()

    struct AScopedTrace
    {
        inline AScopedTrace(const char *name) { ATrace_beginSection_(name); }
        inline ~AScopedTrace() { ATrace_endSection_(); }
    };

    #define NS_PROFILER_CPU_FRAME NS_PROFILE_CPU("Frame")
    #define NS_PROFILE_CPU(name) AScopedTrace ___tracer(name)
#endif

#if NS_PROFILER_PIX
    #define USE_PIX
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN 1
    #endif
    #include <windows.h>
    #include <pix.h>
    #pragma comment(lib, "pixEvt")

    constexpr uint32_t PixColors[] =
    {
        0xff0c8189, 0xff001b5b, 0xff083672, 0xff036677, 0xff010589,
        0xff0a5987, 0xff0c548e, 0xff0b787a, 0xff3d10a5, 0xff075975
    };

    constexpr uint32_t PixHash(const char* s, uint32_t h = 2166136261U)
    {
        return (!*s || *s == '/') ? h : PixHash(s + 1, (uint32_t)((h ^ *s) * (uint64_t)16777619));
    }

    #define NS_PROFILER_CPU_FRAME NS_PROFILE_CPU("Frame")
    #define NS_PROFILE_CPU(name) PIXScopedEvent((UINT)PixColors[PixHash(name) % 10], name)
#endif

#if NS_PROFILER_RAZOR
    #include <razorcpu.h>
    #pragma comment(lib, "SceRazorCPU_stub_weak")
    #define NS_PROFILER_CPU_FRAME NS_PROFILE_CPU("Frame")
    #define NS_PROFILE_CPU(name) SCE_RAZOR_CPU_SCOPED_MARKER(name)
#endif

#if NS_PROFILER_NX
    // Remember to add 'libnn_profiler.a' to 'Additional Dependencies'
    #include <nn/profiler.h>
    #define NS_PROFILER_CPU_FRAME nn::profiler::RecordHeartbeat(nn::profiler::Heartbeats_Main)
    #define NS_PROFILE_CPU(name) nn::profiler::ScopedCodeBlock(name)
#endif

#ifndef NS_PROFILE_CPU_INIT_DECLARE
    #define NS_PROFILE_CPU_INIT_DECLARE
#endif

#ifndef NS_PROFILE_CPU_INIT
    #define NS_PROFILE_CPU_INIT NS_NOOP
#endif

#ifndef NS_PROFILER_CPU_FRAME
    #define NS_PROFILER_CPU_FRAME NS_NOOP
#endif

#ifndef NS_PROFILE_CPU
    #define NS_PROFILE_CPU(...) NS_NOOP
#endif

#ifndef NS_PROFILE_CPU_SHUTDOWN
    #define NS_PROFILE_CPU_SHUTDOWN NS_NOOP
#endif

#ifndef NS_PROFILE_CPU_ALLOC
    #define NS_PROFILE_CPU_ALLOC(...) NS_NOOP
#endif

#ifndef NS_PROFILE_CPU_FREE
    #define NS_PROFILE_CPU_FREE(...) NS_NOOP
#endif

#endif
