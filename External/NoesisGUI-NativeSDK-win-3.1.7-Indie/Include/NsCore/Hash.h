////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_HASH_H__
#define __CORE_HASH_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>


namespace Noesis
{

/// These functions for specific cases are supposed to be faster than HashBytes or HashCombine
uint32_t Hash(uint32_t val);
uint32_t Hash(uint32_t val0, uint32_t val1);
uint32_t Hash(uint64_t val);
uint32_t Hash(const void* val);
uint32_t Hash(float val);
uint32_t Hash(double val);

/// Creates a hash value for a memory buffer
uint32_t HashBytes(const void* data, uint32_t len);

/// Creates a hash value from a list of arguments
template<typename... Types>
uint32_t HashCombine(const Types&... args);

}

#include <NsCore/Hash.inl>

#endif
