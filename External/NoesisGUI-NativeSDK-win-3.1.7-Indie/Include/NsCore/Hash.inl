////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Error.h>
#include <NsCore/Memory.h>
#include <NsCore/Symbol.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t Hash(uint32_t val)
{
    // FNV-1a
    return (2166136261u ^ val) * 16777619u;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t Hash(uint32_t val0, uint32_t val1)
{
    // FNV-1a
    return (((2166136261u ^ val0) * 16777619u) ^ val1) * 16777619u;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t Hash(uint64_t val)
{
    return Hash(uint32_t(val), uint32_t(val >> 32));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<int N> struct Int;
template<> struct Int<4> { typedef uint32_t Type; };
template<> struct Int<8> { typedef uint64_t Type; };

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t Hash(const void* val)
{
    return Hash((typename Int<sizeof(val)>::Type)(uintptr_t)val);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t Hash(float val)
{
    union { float f; uint32_t u; } ieee754 = { val };
    return Hash(ieee754.u);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t Hash(double val)
{
    union { double f; uint32_t u[2]; } ieee754 = { val };
    return Hash(ieee754.u[0], ieee754.u[1]);
}

// MurmurHash2, by Austin Appleby
// https://github.com/aappleby/smhasher

#define MMIX(h, k) k *= 0x5bd1e995; k ^= k >> 24; k *= 0x5bd1e995; h *= 0x5bd1e995; h ^= k;
#define MTAIL(h) h ^= h >> 13; h *= 0x5bd1e995; h ^= h >> 15;

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t HashBytes(const void* data_, uint32_t len)
{
    NS_ASSERT(len < 4 || Alignment<uint32_t>::IsAlignedPtr(data_));
    const uint8_t* data = (const uint8_t*)data_;
    uint32_t h = len;

    while (len >= 4)
    {
        uint32_t k = *(const uint32_t*)(data);
        MMIX(h, k);

        data += 4;
        len -= 4;
    }

    switch (len)
    {
        case 3: h ^= data[2] << 16;
        case 2: h ^= data[1] << 8;
        case 1: h ^= data[0];
                h *= 0x5bd1e995;
    }

    MTAIL(h);
    return h;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void HashCombine_(uint32_t& hash, uint64_t val)
{
    uint32_t k0 = uint32_t(val);
    MMIX(hash, k0);

    uint32_t k1 = uint32_t(val >> 32);
    MMIX(hash, k1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void HashCombine_(uint32_t& hash, int64_t val)
{
    uint32_t k0 = uint32_t(val);
    MMIX(hash, k0);

    uint32_t k1 = uint32_t(val >> 32);
    MMIX(hash, k1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void HashCombine_(uint32_t& hash, uint32_t val)
{
    MMIX(hash, val);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void HashCombine_(uint32_t& hash, int32_t val)
{
    uint32_t k0 = uint32_t(val);
    MMIX(hash, k0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void HashCombine_(uint32_t& hash, Symbol val)
{
    uint32_t k0 = uint32_t(val);
    MMIX(hash, k0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline void HashCombine_(uint32_t& hash, T* val)
{
    HashCombine_(hash, (typename Int<sizeof(val)>::Type)(uintptr_t)val);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void HashCombine_(uint32_t& hash, float val)
{
    union { float f; uint32_t u; } ieee754 = { val };
    uint32_t k = uint32_t(ieee754.u);
    MMIX(hash, k);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, typename... Types>
inline void HashCombine_(uint32_t& hash, const T& val, const Types&... args)
{
    HashCombine_(hash, val);
    HashCombine_(hash, args...);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename... Types>
inline uint32_t HashCombine(const Types&... args)
{
    uint32_t hash = sizeof...(args);
    HashCombine_(hash, args...);
    MTAIL(hash);
    return hash;
}

#undef MMIX
#undef MTAIL

}
