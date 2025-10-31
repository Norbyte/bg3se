////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_COMPILERSETTINGS_H__
#define __CORE_COMPILERSETTINGS_H__


#define NS_STRINGIFY_(x) #x
#define NS_STRINGIFY(x) NS_STRINGIFY_(x)

#ifndef NS_PRIVATE_API
    #define NS_PRIVATE_API
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Compiler dependant settings
////////////////////////////////////////////////////////////////////////////////////////////////////

// Microsoft VC++
#if defined(_MSC_VER) && _MSC_VER >= 1400
    #define NS_COMPILER_MSVC
    #define NS_COMPILER_VERSION _MSC_VER

    #define NS_MACRO_BEGIN \
        __pragma(warning(push)) \
        __pragma(warning(disable:4127)) \
        do {
    #define NS_MACRO_END \
        } while(false) \
        __pragma(warning(pop))

    #ifndef NS_DLL_EXPORT
        #ifdef NS_STATIC_LIBRARY
            #define NS_DLL_EXPORT
        #else
            #define NS_DLL_EXPORT __declspec(dllexport)
        #endif
    #endif

    #ifndef NS_DLL_IMPORT
        #ifdef NS_STATIC_LIBRARY
            #define NS_DLL_IMPORT
        #else
            #define NS_DLL_IMPORT __declspec(dllimport)
        #endif
    #endif

    #ifndef NS_DLL_LOCAL
        #define NS_DLL_LOCAL
    #endif

    #define NS_UNUSED(...) (void)(true ? (void)0 : ((void)(__VA_ARGS__)))

    #ifdef _DEBUG
        #define NS_BEGIN_COLD_REGION
        #define NS_END_COLD_REGION
    #else
        #define NS_BEGIN_COLD_REGION __pragma(optimize("s", on))
        #define NS_END_COLD_REGION __pragma(optimize("", on))
    #endif

    #ifndef NS_COLD_FUNC
        #define NS_COLD_FUNC
    #endif

    #define NS_DEBUG_BREAK __debugbreak()
    #define NS_NOOP __noop
    #define NS_FORCE_INLINE __forceinline
    #define NS_NO_INLINE __declspec(noinline)
    #define NS_UNUSED_FUNC
    #define NS_WARNING_PUSH __pragma(warning(push))
    #define NS_MSVC_WARNING_DISABLE(n) __pragma(warning(disable:n))
    #define NS_MSVC_WARNING_SUPPRESS(n) __pragma(warning(suppress:n))
    #define NS_GCC_CLANG_WARNING_DISABLE(x)
    #define NS_GCC_WARNING_DISABLE(x)
    #define NS_CLANG_WARNING_DISABLE(x)
    #define NS_CLANG_HAS_WARNING(x) 0
    #define NS_WARNING_POP __pragma(warning(pop))
    #define NS_INTERFACE struct __declspec(novtable)
    #define NS_ASSUME(cond) __assume(cond)
    #define NS_UNREACHABLE __assume(0)
    #define NS_COUNTOF(_x) _countof(_x)
    #define NS_LIKELY(x) (x)
    #define NS_UNLIKELY(x) (x)
    #define NS_FORMAT_PRINTF_ATTR(n)

    #ifdef _PREFAST_
        #include <sal.h>
        #define NS_FORMAT_PRINTF _Printf_format_string_
    #else
        #define NS_FORMAT_PRINTF
    #endif

    // Workaround for C2187 when using /permissive- with Windows SDK 8.1
    struct IUnknown; 

// GCC - Clang
#elif defined(__GNUG__) && __GNUG__ >= 4
    #define NS_COMPILER_GCC
    #define NS_COMPILER_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)

    #define NS_MACRO_BEGIN do {
    #define NS_MACRO_END } while (false)

    #ifndef __has_declspec_attribute
        #define __has_declspec_attribute(_x) 0
    #endif

    #ifndef NS_DLL_EXPORT
        #ifdef NS_STATIC_LIBRARY
            #define NS_DLL_EXPORT
        #else
            #if __has_declspec_attribute(dllexport)
                #define NS_DLL_EXPORT __declspec(dllexport)
            #else
                #define NS_DLL_EXPORT __attribute__ ((visibility("default")))
            #endif
        #endif
    #endif

    #ifndef NS_DLL_IMPORT
        #ifdef NS_STATIC_LIBRARY
            #define NS_DLL_IMPORT
        #else
            #if __has_declspec_attribute(dllimport)
                #define NS_DLL_IMPORT __declspec(dllimport)
            #else
                #define NS_DLL_IMPORT __attribute__ ((visibility("default")))
            #endif
        #endif
    #endif

    #ifndef NS_DLL_LOCAL
        #ifdef NS_STATIC_LIBRARY
            #define NS_DLL_LOCAL
        #else
            #define NS_DLL_LOCAL __attribute__ ((visibility("hidden")))
        #endif
    #endif

    // https://stackoverflow.com/questions/23235910/variadic-unused-function-macro
    #define NS_UNUSED_1(x1) (void)(true ? (void)0 : ((void)(x1)))
    #define NS_UNUSED_2(x1, x2) NS_UNUSED_1(x1); NS_UNUSED_1(x2);
    #define NS_UNUSED_3(x1, x2, x3) NS_UNUSED_2(x1, x2); NS_UNUSED_1(x3);
    #define NS_UNUSED_4(x1, x2, x3, x4) NS_UNUSED_3(x1, x2, x3); NS_UNUSED_1(x4);
    #define NS_UNUSED_5(x1, x2, x3, x4, x5) NS_UNUSED_4(x1, x2, x3, x4); NS_UNUSED_1(x5);
    #define NS_UNUSED_6(x1, x2, x3, x4, x5, x6) NS_UNUSED_5(x1, x2, x3, x4, x5); NS_UNUSED_1(x6);
    #define NS_UNUSED_7(x1, x2, x3, x4, x5, x6, x7) NS_UNUSED_6(x1, x2, x3, x4, x5, x6); NS_UNUSED_1(x7);
    #define NS_UNUSED_8(x1, x2, x3, x4, x5, x6, x7, x8) NS_UNUSED_7(x1, x2, x3, x4, x5, x6, x7); NS_UNUSED_1(x8);
    #define NS_UNUSED_9(x1, x2, x3, x4, x5, x6, x7, x8, x9) NS_UNUSED_8(x1, x2, x3, x4, x5, x6, x7, x8); NS_UNUSED_1(x9);
    #define NS_UNUSED_10(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) NS_UNUSED_9(x1, x2, x3, x4, x5, x6, x7, x8, x9); NS_UNUSED_1(x10);
    #define NS_UNUSED_11(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11) NS_UNUSED_10(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10); NS_UNUSED_1(x11);
    #define NS_UNUSED_12(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12) NS_UNUSED_11(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11); NS_UNUSED_1(x12);

    #define VA_NUM_ARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, N, ...) N
    #define VA_NUM_ARGS(...) VA_NUM_ARGS_IMPL(__VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

    #define NS_UNUSED_IMPL_(n) NS_UNUSED_##n
    #define NS_UNUSED_IMPL(n) NS_UNUSED_IMPL_(n)
    #define NS_UNUSED(...) NS_UNUSED_IMPL(VA_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

    #define NS_BEGIN_COLD_REGION
    #define NS_END_COLD_REGION

    #ifndef NS_COLD_FUNC
        #ifdef __clang__
            #if defined(__ORBIS__) || defined(__PROSPERO__)
                // 'minsize' cannot be used with Edit and Continue (function alignment #2376)
                #define NS_COLD_FUNC __attribute__((cold, aligned(16)))
            #else
                #define NS_COLD_FUNC __attribute__((cold, minsize))
            #endif
        #else
            #define NS_COLD_FUNC __attribute__((cold))
        #endif
    #endif

    #if defined(__ORBIS__)
        #define NS_DEBUG_BREAK __asm volatile ("int $0x41")
    #elif defined(__i386__) || defined(__x86_64__)
        #define NS_DEBUG_BREAK __asm__("int $0x03")
    #elif defined(__EMSCRIPTEN__)
        extern "C" void emscripten_debugger();
        #define NS_DEBUG_BREAK emscripten_debugger()
    #elif defined(__arm__)
        #include <unistd.h>
        #include <signal.h>
        #define NS_DEBUG_BREAK \
            __asm__ __volatile__ ( \
                "mov r0, %0\n" \
                "mov r1, %1\n" \
                "mov r12, #37\n" \
                "swi 128\n" \
                : : "r" (getpid()), "r" (SIGINT) : "r12", "r0", "r1", "cc" \
            )
    #else
        #define NS_DEBUG_BREAK __builtin_trap()
    #endif

    #define NS_NOOP (void)(sizeof(0))
    #define NS_FORCE_INLINE __attribute__ ((always_inline)) inline
    #define NS_NO_INLINE __attribute__ ((noinline))
    #define NS_UNUSED_FUNC __attribute__ ((unused))
    #define NS_WARNING_PUSH _Pragma("GCC diagnostic push")
    #define NS_MSVC_WARNING_DISABLE(n)
    #define NS_MSVC_WARNING_SUPPRESS(n)
    #define NS_GCC_CLANG_WARNING_DISABLE(x) _Pragma(NS_STRINGIFY(GCC diagnostic ignored x))

    #ifdef __clang__
        #define NS_GCC_WARNING_DISABLE(x)
        #define NS_CLANG_WARNING_DISABLE(x) _Pragma(NS_STRINGIFY(clang diagnostic ignored x))
        #define NS_CLANG_HAS_WARNING(x) __has_warning(x)
        #define NS_ASSUME(cond) __builtin_assume(cond)
    #else
        #define NS_GCC_WARNING_DISABLE(x) _Pragma(NS_STRINGIFY(GCC diagnostic ignored x))
        #define NS_CLANG_HAS_WARNING(x) 0
        #define NS_CLANG_WARNING_DISABLE(x)
        #define NS_ASSUME(cond) ((cond) ? (void)0 : __builtin_unreachable())
    #endif

    #define NS_WARNING_POP _Pragma("GCC diagnostic pop")
    #define NS_INTERFACE struct
    #define NS_UNREACHABLE __builtin_unreachable()

    // http://cnicholson.net/2011/01/stupid-c-tricks-a-better-sizeof_array/
    template<typename T, int N> char (&NS_COUNTOF_REQUIRES_ARRAY_ARGUMENT(const T(&)[N]))[N];
    #define NS_COUNTOF(_x) sizeof(NS_COUNTOF_REQUIRES_ARRAY_ARGUMENT(_x))

    #define NS_LIKELY(x) __builtin_expect(!!(x), 1)
    #define NS_UNLIKELY(x) __builtin_expect(!!(x), 0)
    #define NS_FORMAT_PRINTF_ATTR(n) __attribute__ ((format (printf, n, n + 1)))
    #define NS_FORMAT_PRINTF

#else
    #error Compiler not supported
#endif

#endif
