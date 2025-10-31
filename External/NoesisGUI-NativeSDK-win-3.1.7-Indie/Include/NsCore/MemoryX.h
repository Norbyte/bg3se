////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_MEMORY_H__
#define __CORE_MEMORY_H__


#include <stdint.h>
#include <NsCore/KernelApi.h>


namespace Noesis
{

typedef decltype(sizeof(int)) SizeT;

template<class T> struct Alignment
{
    enum { Mask = sizeof(T) - 1 };
    static uint32_t Align(uint32_t size) { return (size + Mask) & ~Mask; }
    static bool IsAligned(uint32_t size) { return (size & Mask) == 0; }
    static bool IsAlignedPtr(const void* ptr) { return (uintptr_t(ptr) & Mask) == 0; }
};

struct MemoryCallbacks
{
    void* user;
    void* (*alloc)(void* user, SizeT size);
    void* (*realloc)(void* user, void* ptr, SizeT size);
    void (*dealloc)(void* user, void* ptr);
    SizeT (*allocSize)(void* user, void* ptr);
};

NS_CORE_KERNEL_API void SetMemoryCallbacks(const MemoryCallbacks& callbacks);

NS_CORE_KERNEL_API void* Alloc(SizeT size);
NS_CORE_KERNEL_API void* Realloc(void* ptr, SizeT size);
NS_CORE_KERNEL_API void Dealloc(void* ptr);

NS_CORE_KERNEL_API uint32_t GetAllocatedMemory();
NS_CORE_KERNEL_API uint32_t GetAllocatedMemoryAccum();
NS_CORE_KERNEL_API uint32_t GetAllocationsCount();

}

#endif
