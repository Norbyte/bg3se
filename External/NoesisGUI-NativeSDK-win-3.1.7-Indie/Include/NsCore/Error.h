////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_ERROR_H__
#define __CORE_ERROR_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>


///  ----------------------------------------------------------------------------------------
///  |                     Debug    Profile    Release                                      |
///  ----------------------------------------------------------------------------------------
///  | NS_ASSERT            [x]       [ ]        [ ]         Internal Debug checks          |
///  | NS_CHECK             [x]       [x]        [ ]         Development checks             |
///  | NS_ERROR             [x]       [x]        [x]         Recoverable errors             |
///  | NS_FATAL             [x]       [x]        [x]         Non-recoverable errors         |
///  ----------------------------------------------------------------------------------------

#ifndef NS_INTERNAL_ASSERT
    #define NS_INTERNAL_ASSERT(expr) \
        NS_MACRO_BEGIN \
            if (NS_UNLIKELY(!(expr))) \
            { \
                if (Noesis::InvokeAssertHandler(__FILE__, __LINE__, #expr)) \
                { \
                    NS_DEBUG_BREAK; \
                } \
            } \
        NS_MACRO_END
#endif

#ifndef NS_ASSERT
    #ifdef _PREFAST_
        #define NS_ASSERT(expr) __analysis_assume(!!(expr))
    #elif defined(__clang_analyzer__)
        #include <assert.h>
        #define NS_ASSERT(expr) assert(expr)
    #elif defined(NS_DEBUG)
        #define NS_ASSERT(expr) NS_INTERNAL_ASSERT(expr)
    #else
        #define NS_ASSERT(expr) NS_UNUSED(expr)
    #endif
#endif

#ifndef NS_CHECK
    #ifdef _PREFAST_
        #define NS_CHECK(expr, ...) __analysis_assume(!!(expr))
    #elif defined(__clang_analyzer__)
        #include <assert.h>
        #define NS_CHECK(expr, ...) assert(expr)
    #elif defined(NS_PROFILE)
        #define NS_CHECK(expr, ...) \
            NS_MACRO_BEGIN \
                if (NS_UNLIKELY(!(expr))) \
                { \
                    Noesis::InvokeErrorHandler(__FILE__, __LINE__, false, __VA_ARGS__); \
                } \
            NS_MACRO_END
    #else
        #define NS_CHECK(...) NS_UNUSED(__VA_ARGS__)
    #endif
#endif

#ifndef NS_ERROR
    #define NS_ERROR(...) Noesis::InvokeErrorHandler(__FILE__, __LINE__, false, __VA_ARGS__)
#endif

#ifndef NS_FATAL
    #include <stdlib.h>
    #define NS_FATAL(...) \
        Noesis::InvokeErrorHandler(__FILE__, __LINE__, true, __VA_ARGS__); \
        NS_DEBUG_BREAK; \
        abort()
#endif

#define NS_ASSERT_UNREACHABLE NS_ASSERT(false); NS_UNREACHABLE
#define NS_ASSERT_ASSUME(expr) NS_ASSERT(expr); NS_ASSUME(expr)

namespace Noesis
{

/// Installs custom assert handler and returns previous one
typedef bool (*AssertHandler)(const char* file, uint32_t line, const char* expr);
NS_CORE_KERNEL_API AssertHandler SetAssertHandler(AssertHandler handler);

/// Executes currently installed assert handler
NS_CORE_KERNEL_API bool InvokeAssertHandler(const char* file, uint32_t line, const char* expr);

/// Installs custom error handler and returns previous one
typedef void (*ErrorHandler)(const char* file, uint32_t line, const char* message, bool fatal);
NS_CORE_KERNEL_API ErrorHandler SetErrorHandler(ErrorHandler handler);

/// Executes currently installed error handler
NS_CORE_KERNEL_API void InvokeErrorHandler(const char* file, uint32_t line, bool fatal,
    NS_FORMAT_PRINTF const char* format, ...) NS_FORMAT_PRINTF_ATTR(4);

}

#endif
