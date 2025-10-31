////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_PLATFORMSETTINGS_H__
#define __CORE_PLATFORMSETTINGS_H__


#ifndef NS_MULTITHREADING
    #define NS_MULTITHREADING 1
#endif


#if defined(__APPLE__) && __APPLE__
    #include <TargetConditionals.h>
    #define NS_PLATFORM_APPLE
    
    // iPhone
    // TARGET_OS_IPHONE will be undefined on an unknown compiler, and will be defined on gcc.
    #if defined(__IPHONE__) || (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) || (defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR)
        #define NS_PLATFORM_IPHONE
        #if TARGET_OS_IOS
            #define NS_PLATFORM_IOS
            #if defined(__arm__)
                #define NS_PLATFORM_DESCRIPTION "iOS on ARM"
            #elif defined(__arm64__)
                #define NS_PLATFORM_DESCRIPTION "iOS on ARM64"
            #elif defined(__i386__)
                #define NS_PLATFORM_IPHONE_SIMULATOR
                #define NS_PLATFORM_DESCRIPTION "iOS simulator on x86"
            #elif defined(__x86_64)
                #define NS_PLATFORM_IPHONE_SIMULATOR
                #define NS_PLATFORM_DESCRIPTION "iOS simulator on x86_64"
            #else
                #error Unknown processor
            #endif
        #elif TARGET_OS_TV
            #define NS_PLATFORM_TVOS
            #if defined(__arm__)
                #define NS_PLATFORM_DESCRIPTION "tvOS on ARM"
            #elif defined(__arm64__)
                #define NS_PLATFORM_DESCRIPTION "tvOS on ARM64"
            #elif defined(__i386__)
                #define NS_PLATFORM_IPHONE_SIMULATOR
                #define NS_PLATFORM_DESCRIPTION "tvOS simulator on x86"
            #elif defined(__x86_64)
                #define NS_PLATFORM_IPHONE_SIMULATOR
                #define NS_PLATFORM_DESCRIPTION "tvOS simulator on x86_64"
            #else
                #error Unknown processor
            #endif
        #else
            #error Unknown target
        #endif

    // Macintosh OSX
    // TARGET_OS_MAC is defined by the Metrowerks and older AppleC compilers.
    // Howerver, TARGET_OS_MAC is defined to be 1 in all cases.
    // __i386__ and __intel__ are defined by the GCC compiler.
    // __dest_os is defined by the Metrowerks compiler.
    // __MACH__ is defined by the Metrowerks and GCC compilers.
    // powerc and __powerc are defined by the Metrowerks and GCC compilers.
    #elif defined(__MACH__) || (defined(__MSL__) && (__dest_os == __mac_os_x))
        #define NS_PLATFORM_OSX
        #if defined(__i386__) || defined(__intel__)
            #define NS_PLATFORM_DESCRIPTION "macOS on x86"
        #elif defined(__x86_64) || defined(__amd64)
            #define NS_PLATFORM_DESCRIPTION "macOS on x86_64"
        #elif defined(__arm64__)
            #define NS_PLATFORM_DESCRIPTION "macOS on ARM64"
        #else
            #error Unknown processor
        #endif
    #else
        #error Unknown Apple Platform
    #endif

#elif defined(_XBOX_ONE)
    #error XDK is no longer suported, use GDK instead

#elif defined(_GAMING_XBOX)
    #define NS_PLATFORM_GAME_CORE
    #define NS_PLATFORM_WINDOWS
    #if defined(_GAMING_XBOX_SCARLETT)
        #define NS_PLATFORM_SCARLETT
        #define NS_PLATFORM_DESCRIPTION "Xbox Series"
    #elif defined(_GAMING_XBOX_XBOXONE)
        #define NS_PLATFORM_XBOX_ONE_CORE
        #define NS_PLATFORM_DESCRIPTION "Xbox One"
    #else
        #error Unknown Platform
    #endif

#elif defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)
    #define NS_PLATFORM_WINDOWS
    #ifdef WINAPI_FAMILY
        #include <winapifamily.h>
        #if WINAPI_FAMILY == WINAPI_FAMILY_PC_APP
            #define NS_PLATFORM_WINDOWS_STORE
            #define NS_PLATFORM_WINRT
            #define NS_PLATFORM_NAME "Windows Store"
        #elif WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
            #define NS_PLATFORM_WINDOWS_PHONE
            #define NS_PLATFORM_WINRT
            #define NS_PLATFORM_NAME "Windows Phone"
        #elif WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
            #define NS_PLATFORM_WINDOWS_DESKTOP
            #define NS_PLATFORM_NAME "Windows"
        #else
            #error Unknown WINAPI_FAMILY value
        #endif
    #else
        #define NS_PLATFORM_WINDOWS_DESKTOP
        #define NS_PLATFORM_NAME "Windows"
    #endif

    #if defined(_M_AMD64) || defined(_AMD64_) || defined(_M_X64) || defined(__x86_64__)
        #define NS_PLATFORM_DESCRIPTION NS_PLATFORM_NAME " on x86_64"
    #elif defined(_M_IX86) || defined(_X86_)
        #define NS_PLATFORM_DESCRIPTION NS_PLATFORM_NAME " on x86"
    #elif defined(_M_ARM)
        #define NS_PLATFORM_DESCRIPTION NS_PLATFORM_NAME " on ARM"
    #else
        #error Unknown processor
    #endif

#elif defined(__ANDROID__) || defined(ANDROID)
    #define NS_PLATFORM_ANDROID
    #if defined(__arm__)
        #define NS_PLATFORM_DESCRIPTION "Android on ARM"
    #elif defined(__aarch64__)
    #define NS_PLATFORM_DESCRIPTION "Android on ARM64"
    #elif defined(__i386__)
        #define NS_PLATFORM_DESCRIPTION "Android on x86"
    #elif defined(__x86_64__)
        #define NS_PLATFORM_DESCRIPTION "Android on x86_64"
    #else
        #error Unknown processor
    #endif

#elif defined(__EMSCRIPTEN__)
    #define NS_PLATFORM_EMSCRIPTEN
    #define NS_PLATFORM_DESCRIPTION "Emscripten"
    #undef NS_MULTITHREADING
    #define NS_MULTITHREADING 0

#elif defined(__linux) || defined(__linux__)
    #define NS_PLATFORM_LINUX
    #if defined(__i386__) || defined(__intel__) || defined(_M_IX86)
        #define NS_PLATFORM_DESCRIPTION "Linux on x86"
    #elif defined(__x86_64__)
        #define NS_PLATFORM_DESCRIPTION "Linux on x86_64"
    #elif defined(__arm__)
        #define NS_PLATFORM_DESCRIPTION "Linux on ARM"
    #elif defined(__aarch64__)
        #define NS_PLATFORM_DESCRIPTION "Linux on ARM64"
    #else
        #error Unknown processor
    #endif

#elif defined(__ORBIS__)
    #define NS_PLATFORM_SCE
    #define NS_PLATFORM_PS4
    #define NS_PLATFORM_DESCRIPTION "PlayStation 4"

#elif defined(__PROSPERO__)
    #define NS_PLATFORM_SCE
    #define NS_PLATFORM_PS5
    #define NS_PLATFORM_DESCRIPTION "PlayStation 5"

#elif defined(NN_NINTENDO_SDK)
    #define NS_PLATFORM_NX
    #define NS_PLATFORM_DESCRIPTION "Nintendo Switch"

#else
    #error Platform not supported
#endif

#endif
