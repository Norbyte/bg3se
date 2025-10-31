////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Hash.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
inline Pair<T1, T2>::Pair(const T1& x, const T2& y): first(x), second(y)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2> template<class U, class V>
inline Pair<T1, T2>::Pair(const Pair<U, V>& p): first(p.first), second(p.second)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2> template<class U, class V>
inline Pair<T1, T2>::Pair(Pair<U, V>&& p): first(ForwardArg<U>(p.first)), second(ForwardArg<V>(p.second))
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2> template<class U, class V>
inline Pair<T1, T2>& Pair<T1, T2>::operator=(const Pair<U, V>& p)
{
    first = p.first;
    second = p.second;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2> template<class U, class V>
inline Pair<T1, T2>& Pair<T1, T2>::operator=(Pair<U, V>&& p)
{
    first = ForwardArg<U>(p.first);
    second = ForwardArg<V>(p.second);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
inline bool operator==(const Pair<T1, T2>& a, const Pair<T1, T2>& b)
{
    return ((a.first == b.first) && (a.second == b.second));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
inline bool operator!=(const Pair<T1, T2>& a, const Pair<T1, T2>& b)
{
    return !(a == b);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
inline bool operator<(const Pair<T1, T2>& a, const Pair<T1, T2>& b)
{
    return (a.first == b.first) ? (a.second < b.second) : (a.first < b.first);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
inline bool operator>(const Pair<T1, T2>& a, const Pair<T1, T2>& b)
{
    return b < a;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
inline bool operator>=(const Pair<T1, T2>& a, const Pair<T1, T2>& b)
{
    return !(a < b);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
inline bool operator<=(const Pair<T1, T2>& a, const Pair<T1, T2>& b)
{
    return !(b < a);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
inline Pair<T1, T2> MakePair(T1&& a, T2&& b)
{
    return Pair<T1, T2>(ForwardArg<T1>(a), ForwardArg<T2>(b));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> struct HashKeyInfo;
template<class T1, class T2> struct HashKeyInfo<Pair<T1, T2>>
{
    static bool IsEmpty(const Pair<T1, T2>& p) { return HashKeyInfo<T1>::IsEmpty(p.first); }
    static void MarkEmpty(Pair<T1, T2>& p) { HashKeyInfo<T1>::MarkEmpty(p.first); }
    static uint32_t HashValue(const Pair<T1, T2>& p) { return HashCombine(p.first, p.second); }
    static bool IsEqual(const Pair<T1, T2>& lhs, const Pair<T1, T2>& rhs)
    {
        return !IsEmpty(lhs) && lhs == rhs;
    }
};

}
