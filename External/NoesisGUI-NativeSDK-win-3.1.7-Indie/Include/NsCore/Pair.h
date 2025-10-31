////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_PAIR_H__
#define __CORE_PAIR_H__


#include <NsCore/Noesis.h>
#include <NsCore/CompilerTools.h>


namespace Noesis
{

/// Provides a way to store two heterogeneous objects as a single unit
template<class T1, class T2>
struct Pair
{
    T1 first;
    T2 second;

    Pair() = default;
    Pair(const Pair& p) = default;
    Pair(Pair&& p) = default;
    Pair(const T1& x, const T2& y);
    template<class U, class V> Pair(const Pair<U, V>& p);
    template<class U, class V> Pair(Pair<U, V>&& p);

    Pair& operator=(const Pair& p) = default;
    Pair& operator=(Pair&& p) = default;
    template<class U, class V> Pair& operator=(const Pair<U, V>& p);
    template<class U, class V> Pair& operator=(Pair<U, V>&& p);
};

/// Creates a pair object of type, defined by the argument types
template<class T1, class T2>
Pair<T1, T2> MakePair(T1&& a, T2&& b);

}

#include <NsCore/Pair.inl>

#endif
